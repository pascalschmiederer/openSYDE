//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       List widget for view widgets (implementation)

   List widget for view widgets

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     26.07.2018  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <QDrag>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include "constants.h"
#include "C_NagViewList.h"
#include "C_PuiSvHandler.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_Parent Optional pointer to parent

   \created     26.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_NagViewList::C_NagViewList(QWidget * const opc_Parent) :
   QListView(opc_Parent)
{
   this->setModel(&this->mc_Model);
   this->setItemDelegate(&this->mc_Delegate);
   this->mc_Delegate.SetListView(this);

   //Options
   this->setDragEnabled(true);
   this->setAcceptDrops(true);
   this->setDragDropMode(QAbstractItemView::DragDrop);
   this->setDropIndicatorShown(true);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Default destructor

   Clean up.

   \created     03.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_NagViewList::~C_NagViewList(void)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Init

   \created     26.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::Init(void)
{
   this->mc_Model.SetNumRows(static_cast<sintn>(C_PuiSvHandler::h_GetInstance()->GetViewCount()));
   for (uint32 u32_ItView = 0UL; u32_ItView < C_PuiSvHandler::h_GetInstance()->GetViewCount(); ++u32_ItView)
   {
      const QModelIndex c_Index = this->mc_Model.index(u32_ItView);
      //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
      C_NagViewItem * pc_ViewWidget = dynamic_cast<C_NagViewItem *>(this->indexWidget(c_Index));
      if (pc_ViewWidget == NULL)
      {
         //Add item widget
         pc_ViewWidget = new C_NagViewItem(this);
         this->setIndexWidget(c_Index, pc_ViewWidget);

         connect(pc_ViewWidget, &C_NagViewItem::SigStartDrag, this, &C_NagViewList::m_OnStartDrag);
         connect(pc_ViewWidget, &C_NagViewItem::SigSetName, this, &C_NagViewList::m_OnSetName);
         connect(pc_ViewWidget, &C_NagViewItem::SigExpanded, this, &C_NagViewList::m_OnExpand);
         connect(pc_ViewWidget, &C_NagViewItem::SigDuplicate, this, &C_NagViewList::m_OnDuplicate);
         connect(pc_ViewWidget, &C_NagViewItem::SigDelete, this, &C_NagViewList::m_OnDelete);
         connect(pc_ViewWidget, &C_NagViewItem::SigSelect, this, &C_NagViewList::m_OnSelect);
      }
      pc_ViewWidget->Init(u32_ItView);
      //lint -e429 Clean up via Qt parent mechanism
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Load all user settings

   \created     08.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::LoadUserSettings(void) const
{
   for (sintn sn_ItView = 0UL; sn_ItView < this->mc_Model.rowCount(); ++sn_ItView)
   {
      C_NagViewItem * const pc_View = this->GetItemAt(sn_ItView);
      if (pc_View != NULL)
      {
         pc_View->LoadUserSettings();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Save all user settings

   \created     08.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::SaveUserSettings(void) const
{
   for (sintn sn_ItView = 0UL; sn_ItView < this->mc_Model.rowCount(); ++sn_ItView)
   {
      const C_NagViewItem * const pc_View = this->GetItemAt(sn_ItView);
      if (pc_View != NULL)
      {
         pc_View->SaveUserSettings();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Update all view names

   \created     03.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::UpdateNames(void) const
{
   for (sintn sn_ItView = 0UL; sn_ItView < this->mc_Model.rowCount(); ++sn_ItView)
   {
      C_NagViewItem * const pc_View = this->GetItemAt(sn_ItView);
      if (pc_View != NULL)
      {
         pc_View->UpdateName();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Trigger update of decoration role

   \created     30.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::UpdateDeco(void) const
{
   for (sintn sn_ItView = 0UL; sn_ItView < this->mc_Model.rowCount(); ++sn_ItView)
   {
      C_NagViewItem * const pc_View = this->GetItemAt(sn_ItView);
      if (pc_View != NULL)
      {
         pc_View->UpdateDeco();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get the maximum height required to display all items of this widget in their current state

   \return
   The maximum height required to display all items of this widget in their current state

   \created     08.11.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_NagViewList::GetMaximumRequiredHeight(void) const
{
   sint32 s32_Retval = 0;

   for (sint32 s32_ItRow = 0; s32_ItRow < this->mc_Model.rowCount(); ++s32_ItRow)
   {
      const QStyleOptionViewItem c_Style;
      const QModelIndex c_Index = this->mc_Model.index(s32_ItRow);
      const QSize c_Size = this->mc_Delegate.sizeHint(c_Style, c_Index);
      s32_Retval += c_Size.height();
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set active state

   \param[in] ou32_ViewIndex Active view index
   \param[in] os32_SubMode   New active sub mode

   \created     30.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::SetActive(const uint32 ou32_ViewIndex, const sint32 os32_SubMode) const
{
   for (sintn sn_ItView = 0UL; sn_ItView < this->mc_Model.rowCount(); ++sn_ItView)
   {
      C_NagViewItem * const pc_View = this->GetItemAt(sn_ItView);
      if (pc_View != NULL)
      {
         if (ou32_ViewIndex == static_cast<uint32>(sn_ItView))
         {
            pc_View->SetActive(true, os32_SubMode);
         }
         else
         {
            pc_View->SetActive(false, os32_SubMode);
         }
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get item widget at index

   \param[in] osn_Index Item index

   \return
   NULL Item not found
   Else Valid item

   \created     26.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_NagViewItem * C_NagViewList::GetItemAt(const sintn osn_Index) const
{
   //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
   return dynamic_cast<C_NagViewItem *>(this->indexWidget(this->mc_Model.index(osn_Index)));
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten drag enter event slot

   Here: Accept drag

   \param[in,out] opc_Event Event identification and information

   \created     02.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::dragEnterEvent(QDragEnterEvent * const opc_Event)
{
   const QMimeData * const pc_MimeData = opc_Event->mimeData();

   QListView::dragEnterEvent(opc_Event);
   if (pc_MimeData != NULL)
   {
      if (pc_MimeData->hasFormat(this->mc_Model.mimeTypes().at(0)) == true)
      {
         opc_Event->acceptProposedAction();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten drag move event slot

   Here: Accept drag

   \param[in,out] opc_Event Event identification and information

   \created     02.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::dragMoveEvent(QDragMoveEvent * const opc_Event)
{
   const QMimeData * const pc_MimeData = opc_Event->mimeData();

   QListView::dragMoveEvent(opc_Event);
   if (pc_MimeData != NULL)
   {
      if (pc_MimeData->hasFormat(this->mc_Model.mimeTypes().at(0)) == true)
      {
         opc_Event->acceptProposedAction();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten drop event slot

   Here: Trigger view move

   \param[in,out] opc_Event Event identification and information

   \created     02.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::dropEvent(QDropEvent * const opc_Event)
{
   const QMimeData * const pc_MimeData = opc_Event->mimeData();

   QListView::dropEvent(opc_Event);
   if (pc_MimeData != NULL)
   {
      const QString c_MimeType = this->mc_Model.mimeTypes().at(0);
      if (pc_MimeData->hasFormat(c_MimeType) == true)
      {
         bool q_Ok;
         const QString c_Content(pc_MimeData->data(c_MimeType));
         const sintn sn_Source = c_Content.toLong(&q_Ok);
         if (q_Ok == true)
         {
            const QListView::DropIndicatorPosition e_DropIndicator = this->dropIndicatorPosition();
            sint32 s32_TargetRow = this->indexAt(opc_Event->pos()).row();
            //If drag below inserted adapt target row
            if (sn_Source < s32_TargetRow)
            {
               --s32_TargetRow;
            }

            //If indicator below item insert at next index
            if (e_DropIndicator == QAbstractItemView::BelowItem)
            {
               ++s32_TargetRow;
            }

            if (e_DropIndicator == QAbstractItemView::OnViewport)
            {
               // special case: drop behind the last item in the free space
               s32_TargetRow = this->mc_Model.rowCount();
            }
            if ((sn_Source >= 0) && (s32_TargetRow >= 0))
            {
               Q_EMIT this->SigMoveView(static_cast<uint32>(sn_Source), static_cast<uint32>(s32_TargetRow));
            }
         }
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten start drag event

   Here: send custom drag event

   \param[in] oc_SupportedActions Supported actions

   \created     02.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::startDrag(const Qt::DropActions oc_SupportedActions)
{
   const QModelIndexList c_SelectedIndices = this->selectedIndexes();

   Q_UNUSED(oc_SupportedActions)
   if (c_SelectedIndices.size() > 0)
   {
      this->m_StartDrag(c_SelectedIndices.at(0));
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Clean up UI after drag

   \created     02.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::m_AfterDrag(void) const
{
   for (sintn sn_ItView = 0UL; sn_ItView < this->mc_Model.rowCount(); ++sn_ItView)
   {
      C_NagViewItem * const pc_View = this->GetItemAt(sn_ItView);
      if (pc_View != NULL)
      {
         pc_View->SetDragged(false);
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Start drag action

   \param[in] orc_Index Selected index

   \created     02.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::m_StartDrag(const QModelIndex & orc_Index)
{
   //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
   C_NagViewItem * const pc_Item = dynamic_cast<C_NagViewItem *>(this->indexWidget(orc_Index));

   if (pc_Item != NULL)
   {
      QModelIndexList c_IndexList;

      //start manual drag
      QDrag * const pc_Drag = new QDrag(this);
      QMimeData * pc_MimeData;
      const QRect c_Rect = QRect(QPoint(0, 0), pc_Item->sizeHint());
      QPixmap c_Screenshot(c_Rect.size() + QSize(3 + 3, 3 + 1));

      c_Screenshot.fill(mc_STYLE_GUIDE_COLOR_25);

      //Apply new state BEFORE rendering
      pc_Item->SetDragged(true);

      // create the screenshot
      pc_Item->render(&c_Screenshot, QPoint(3, 1), QRegion(c_Rect));

      //Overwrite first two rows
      {
         QImage c_Tmp = c_Screenshot.toImage();

         for (uint32 u32_Row = 0; u32_Row < 3; ++u32_Row)
         {
            for (sint32 s32_Col = 0; s32_Col < c_Tmp.size().width(); ++s32_Col)
            {
               c_Tmp.setPixelColor(s32_Col, static_cast<sint32>(u32_Row), mc_STYLE_GUIDE_COLOR_25.rgb());
            }
         }
         c_Screenshot.convertFromImage(c_Tmp);
      }

      pc_Drag->setPixmap(c_Screenshot);

      c_IndexList.push_back(orc_Index);
      pc_MimeData = this->mc_Model.mimeData(c_IndexList);
      pc_Drag->setMimeData(pc_MimeData);
      pc_Drag->exec();
      connect(pc_Drag, &QDrag::destroyed, this, &C_NagViewList::m_AfterDrag);
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle start drag action

   \param[in] opc_Sender Sender

   \created     02.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::m_OnStartDrag(const C_NagViewItem * const opc_Sender)
{
   for (sintn sn_It = 0; sn_It < this->mc_Model.rowCount(); ++sn_It)
   {
      if (this->indexWidget(this->mc_Model.index(sn_It)) == opc_Sender)
      {
         const QModelIndex c_Index = this->model()->index(sn_It, 0);
         this->m_StartDrag(c_Index);
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle expand action

   \param[in] opc_Sender Sender

   \created     26.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::m_OnExpand(const C_NagViewItem * const opc_Sender)
{
   for (sintn sn_It = 0; sn_It < this->mc_Model.rowCount(); ++sn_It)
   {
      if (this->indexWidget(this->mc_Model.index(sn_It)) == opc_Sender)
      {
         const QModelIndex c_Index = this->model()->index(sn_It, 0);
         this->mc_Delegate.RegisterExpand(c_Index);
         Q_EMIT this->SigSizeChange();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle delete action

   \param[in] opc_Sender Sender

   \created     30.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::m_OnDelete(const C_NagViewItem * const opc_Sender)
{
   for (sintn sn_It = 0; sn_It < this->mc_Model.rowCount(); ++sn_It)
   {
      if (this->indexWidget(this->mc_Model.index(sn_It)) == opc_Sender)
      {
         Q_EMIT this->SigDelete(static_cast<uint32>(sn_It));
         Q_EMIT this->SigSizeChange();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle duplicate action

   \param[in] opc_Sender Sender

   \created     30.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::m_OnDuplicate(const C_NagViewItem * const opc_Sender)
{
   for (sintn sn_It = 0; sn_It < this->mc_Model.rowCount(); ++sn_It)
   {
      if (this->indexWidget(this->mc_Model.index(sn_It)) == opc_Sender)
      {
         Q_EMIT this->SigDuplicate(static_cast<uint32>(sn_It));
         Q_EMIT this->SigSizeChange();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle set name

   \param[in] opc_Sender Sender
   \param[in] orc_Name   Name

   \created     30.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::m_OnSetName(const C_NagViewItem * const opc_Sender, const QString & orc_Name)
{
   for (sintn sn_It = 0; sn_It < this->mc_Model.rowCount(); ++sn_It)
   {
      if (this->indexWidget(this->mc_Model.index(sn_It)) == opc_Sender)
      {
         Q_EMIT this->SigSetName(static_cast<uint32>(sn_It), orc_Name);
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle select

   \param[in] opc_Sender         Sender
   \param[in] os32_SubMode       Selected sub mode
   \param[in] orc_Name           Name to display
   \param[in] orc_SubSubItemName Selected sub sub mode name

   \created     30.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagViewList::m_OnSelect(const C_NagViewItem * const opc_Sender, const sint32 os32_SubMode,
                               const QString & orc_Name, const QString & orc_SubSubItemName)
{
   for (sintn sn_It = 0; sn_It < this->mc_Model.rowCount(); ++sn_It)
   {
      if (this->indexWidget(this->mc_Model.index(sn_It)) == opc_Sender)
      {
         const uint32 u32_ViewIndex = static_cast<uint32>(sn_It);
         Q_EMIT this->SigSelect(u32_ViewIndex, os32_SubMode, orc_Name, orc_SubSubItemName);
         Q_EMIT this->SigSizeChange();
      }
   }
}
