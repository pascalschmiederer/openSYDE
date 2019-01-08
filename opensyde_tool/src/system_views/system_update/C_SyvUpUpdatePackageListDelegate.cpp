//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Node update package list drawing delegate (implementation)

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     12.02.2018  STW/B.Bayer
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <QPainter>

#include "stwtypes.h"
#include "C_SyvUpUpdatePackageListDelegate.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
using namespace stw_opensyde_gui;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Default constructor

   \param[in,out] opc_Parent        Optional pointer to parent

   \created     12.02.2018  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
C_SyvUpUpdatePackageListDelegate::C_SyvUpUpdatePackageListDelegate(QObject * const opc_Parent) :
   QStyledItemDelegate(opc_Parent),
   mpc_ActualWidget(NULL),
   ms32_IndexPaint(-1)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten paint event slot

   Here: Draw widget for drag event

   \param[in,out] opc_Painter Painter
   \param[in,out] opc_Option  Option
   \param[in,out] opc_Widget  Widget

   \created     31.01.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SyvUpUpdatePackageListDelegate::paint(QPainter * const opc_Painter, const QStyleOptionViewItem & orc_Option,
                                             const QModelIndex & orc_Index) const
{
   QStyledItemDelegate::paint(opc_Painter, orc_Option, orc_Index);
   if ((ms32_IndexPaint == orc_Index.row()) &&
       (orc_Option.rect.x() == 0) &&
       (orc_Option.rect.y() == 0) &&
       (this->mpc_ActualWidget != NULL))
   {
      if ((orc_Index.isValid() == true) && (orc_Index.parent().isValid() == false))
      {
         uint32 u32_Count;
         uint32 u32_Count2;
         // creating local QWidget (that's why i think it should be fasted, cause we
         // don't touch the heap and don't deal with a QWidget except painting)
         C_SyvUpUpdatePackageListNodeWidget c_ItemWidget(this->mpc_ActualWidget->GetViewIndex(),
                                                         this->mpc_ActualWidget->GetPositionNumber(),
                                                         this->mpc_ActualWidget->GetNodeIndex(),
                                                         this->mpc_ActualWidget->GetNodeName(),
                                                         NULL);
         c_ItemWidget.setGeometry(0, 0, this->mpc_ActualWidget->width(), this->mpc_ActualWidget->height());

         c_ItemWidget.setStyleSheet(this->mpc_ActualWidget->styleSheet());

         c_ItemWidget.CheckAllFiles(u32_Count, u32_Count2);

         // rendering of QWidget itself
         // store the configuration of the original painter before translation. The change of the coordinates
         // is only necessary here.
         opc_Painter->save();
         opc_Painter->translate(orc_Option.rect.topLeft());
         c_ItemWidget.render(opc_Painter,
                             QPoint(0, 0),
                             QRegion(0, 0, orc_Option.rect.width(), orc_Option.rect.height()),
                             QWidget::RenderFlag::DrawChildren);
         // restore the previous coordinates
         opc_Painter->restore();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Update index to paint widget in drag

   \param[in] s32_Index          Update index for
   \param[in] opc_MovingWidget   Actual widget which will be moved

   \created     01.02.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SyvUpUpdatePackageListDelegate::StartPaint(const sint32 s32_Index,
                                                  C_SyvUpUpdatePackageListNodeWidget * const opc_Widget)
{
   this->ms32_IndexPaint = s32_Index;
   this->mpc_ActualWidget = opc_Widget;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Stop paint widget in drag

   \created     21.02.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SyvUpUpdatePackageListDelegate::StopPaint(void)
{
   this->ms32_IndexPaint = -1;
   this->mpc_ActualWidget = NULL;
}
