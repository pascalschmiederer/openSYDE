//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Widget for showing use case specific widgets based on class C_NagUseCaseWidget

   This widget offers the title design and the mechanism to switch between
   use case widgets.
   This widget is designed in a ui file.

   \implementation
   project     opensyde
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     11.07.2016  STW/B.Bayer
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <QSpacerItem>
#include <QPalette>

#include "C_NagUseCaseViewWidget.h"
#include "C_GtGetText.h"
#include "ui_C_NagUseCaseViewWidget.h"

#include "constants.h"
#include "stwtypes.h"

/* -- Used Namespaces ------------------------------------------------------ */

using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_types;

/* -- Module Global Constants ---------------------------------------------- */
const QString C_NagUseCaseViewWidget::mhc_SVGIconPath = "://images/IconNavigationArrow.svg";

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

   \created     11.07.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
C_NagUseCaseViewWidget::C_NagUseCaseViewWidget(QWidget * const opc_Parent) :
   QWidget(opc_Parent),
   mpc_Ui(new Ui::C_NagUseCaseViewWidget),
   mpc_Widget(NULL),
   ms32_Mode(ms32_MODE_SYSDEF),
   ms32_Submode(ms32_SUBMODE_SYSDEF_TOPOLOGY)
{
   mpc_Ui->setupUi(this);

   //Deactivate debug string
   this->mpc_Ui->pc_GroupBoxSubSubMode->setTitle("");

   //Set icons
   //lint -e{1938}  static const is guaranteed preinitialized before main
   this->mpc_Ui->pc_LabelSpace->SetSvg(C_NagUseCaseViewWidget::mhc_SVGIconPath);
   //lint -e{1938}  static const is guaranteed preinitialized before main
   this->mpc_Ui->pc_LabelSpace_2->SetSvg(C_NagUseCaseViewWidget::mhc_SVGIconPath);

   this->mpc_Ui->pc_PushButtonIcon->setIconSize(mc_ICON_SIZE_24);
   this->mpc_Ui->pc_PushButtonIcon->SetToolTipInformation(C_GtGetText::h_GetText(
                                                             "Switch Mode"),
                                                          C_GtGetText::h_GetText("Switch between dark and bright mode."));
   connect(this->mpc_Ui->pc_PushButtonIcon, &QPushButton::clicked, this,
           &C_NagUseCaseViewWidget::m_OnPushButtonIconPress);
}

//-----------------------------------------------------------------------------
/*!
   \brief   default destructor

   Clean up.

   \created     11.07.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
C_NagUseCaseViewWidget::~C_NagUseCaseViewWidget()
{
   delete mpc_Ui;
   //lint -e{1740}  no memory leak because of the parent of mpc_Widget and the Qt memory management
}

//-----------------------------------------------------------------------------
/*!
   \brief   Initializes all visible strings on the widget

   \created     08.07.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_NagUseCaseViewWidget::InitText(void) const
{
   switch (this->ms32_Mode)
   {
   case ms32_MODE_SYSDEF:
      this->mpc_Ui->pc_LabelSubmode->setVisible(true);
      this->mpc_Ui->pc_LabelSpace->setVisible(true);

      this->mpc_Ui->pc_LabelMode->setText(C_GtGetText::h_GetText("SYSTEM DEFINITION"));
      m_HandleSubSections();

      //Generic push button visibility
      this->mpc_Ui->pc_PushButtonIcon->setVisible(false);
      break;

   case ms32_MODE_SYSVIEW:
      this->mpc_Ui->pc_LabelSubmode->setVisible(true);
      this->mpc_Ui->pc_LabelSpace->setVisible(true);

      this->mpc_Ui->pc_LabelMode->setText(C_GtGetText::h_GetText("SYSTEM COMMISSIONING"));
      m_HandleSubSections();

      //Generic push button visibility
      switch (this->ms32_Submode)
      {
      case ms32_SUBMODE_SYSVIEW_DASHBOARD:
         this->mpc_Ui->pc_PushButtonIcon->setVisible(true);
         break;
      default:
         this->mpc_Ui->pc_PushButtonIcon->setVisible(false);
         break;
      }
      break;

   default:
      break;
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Init Background color

   \created     08.07.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_NagUseCaseViewWidget::InitBackground()
{
   QPalette c_Palette;
   QBrush c_Brush;

   //get palette
   c_Palette = this->palette();
   c_Brush = c_Palette.window();

   if ((this->ms32_Mode == ms32_MODE_SYSDEF) &&
       (this->ms32_Submode == ms32_SUBMODE_SYSDEF_TOPOLOGY))
   {
      //gray background
      c_Brush.setColor(QColor(245, 247, 248));
   }
   else
   {
      //white background
      c_Brush.setColor(mc_STYLE_GUIDE_COLOR_0);
   }

   //set palette
   c_Palette.setBrush(QPalette::Window, c_Brush);
   this->setPalette(c_Palette);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set icon path

   \param[in] orc_IconPath Icon path

   \created     02.08.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagUseCaseViewWidget::SetPushButtonIcon(const QString & orc_IconPath) const
{
   this->mpc_Ui->pc_PushButtonIcon->setIcon(QIcon(orc_IconPath));
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle push button press

   \created     02.08.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagUseCaseViewWidget::m_OnPushButtonIconPress(void)
{
   Q_EMIT this->SigPushButtonIconPressed();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Update sub sub section if necessary

   \created     22.08.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_NagUseCaseViewWidget::m_HandleSubSections(void) const
{
   if (this->mc_ItemSubSubName.compare("") == 0)
   {
      this->mpc_Ui->pc_GroupBoxSubSubMode->setVisible(false);
      this->mpc_Ui->pc_LabelSubmode->setText(this->mc_ItemName);
   }
   else
   {
      this->mpc_Ui->pc_GroupBoxSubSubMode->setVisible(true);
      this->mpc_Ui->pc_LabelMidSection->setText(this->mc_ItemName);
      this->mpc_Ui->pc_LabelSubmode->setText(this->mc_ItemSubSubName);
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Sets the widget which will be showed

   This functions does not delete the widget! It removes the parent.

   \param[in]   opc_widget         Widget
   \param[in]   os32_Mode          Actual mode
   \param[in]   os32_SubMode       Actual sub mode
   \param[in]   orc_ItemName       Item name for showing
   \param[in]   orc_ItemSubSubName Selected sub sub mode name

   \created     11.07.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_NagUseCaseViewWidget::SetUseCaseWidget(C_NagUseCaseWidget * const opc_Widget, const sint32 os32_Mode,
                                              const stw_types::sint32 os32_SubMode, const QString & orc_ItemName,
                                              const QString & orc_ItemSubSubName)
{
   sintn sn_Index;

   if (opc_Widget != NULL)
   {
      // remove the old widget
      if (this->mpc_Widget != NULL)
      {
         this->mpc_Ui->pc_VerticalLayout->removeWidget(this->mpc_Widget);
         this->mpc_Widget->SetParentHook(NULL);
         this->mpc_Widget = NULL;
      }

      // set the new widget
      this->mpc_Widget = opc_Widget;
      //sn_Index = this->mpc_Ui->pc_verticalLayout->indexOf(this->mpc_Ui->pc_horizontalLayout);
      //this->mpc_Ui->pc_VerticalLayout->addSpacing(40);
      this->mpc_Ui->pc_VerticalLayout->addWidget(this->mpc_Widget);
      sn_Index = this->mpc_Ui->pc_VerticalLayout->indexOf(this->mpc_Widget);
      this->mpc_Ui->pc_VerticalLayout->setStretch(sn_Index, 1);

      // Use specific set parent function. The widgets can react on the parent change if necessary.
      if (this->mpc_Widget != NULL)
      {
         this->mpc_Widget->SetParentHook(this);
         connect(this->mpc_Widget, &C_NagUseCaseWidget::SigNameChanged, this, &C_NagUseCaseViewWidget::UpdateItemName);
      }

      // set the text of labels
      this->ms32_Mode = os32_Mode;
      this->ms32_Submode = os32_SubMode;
      this->mc_ItemName = orc_ItemName;
      this->mc_ItemSubSubName = orc_ItemSubSubName;
      this->InitText();
      this->InitBackground();
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Removes the actual setted widget

   This functions does not delete the widget! It removes the parent.

   \created     11.07.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_NagUseCaseViewWidget::RemoveUseCaseWidget(void)
{
   // remove the old widget
   if (this->mpc_Widget != NULL)
   {
      QSpacerItem * pc_Item;

      this->mpc_Ui->pc_VerticalLayout->removeWidget(this->mpc_Widget);
      //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
      pc_Item = dynamic_cast<QSpacerItem *>(this->mpc_Ui->pc_VerticalLayout->itemAt(3));

      // is it a SpacerItem?
      if (pc_Item != NULL)
      {
         this->mpc_Ui->pc_VerticalLayout->removeItem(pc_Item);
      }
      disconnect(this->mpc_Widget, &C_NagUseCaseWidget::SigNameChanged, this, &C_NagUseCaseViewWidget::UpdateItemName);
      this->mpc_Widget->SetParentHook(NULL);
      this->mpc_Widget = NULL;
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Sets the parameters of the item

   \param[in]   os32_SubMode    Actual sub mode
   \param[in]   orc_ItemName    Item name for showing
   \param[in]   orc_ItemSubName Selected sub sub mode name

   \created     01.03.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_NagUseCaseViewWidget::UpdateUseCaseWidget(const stw_types::sint32 os32_SubMode, const QString & orc_ItemName,
                                                 const QString & orc_ItemSubName)
{
   this->mc_ItemName = orc_ItemName;
   this->mc_ItemSubSubName = orc_ItemSubName;
   this->ms32_Submode = os32_SubMode;

   this->InitText();
   this->InitBackground();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Sets the name of the item

   \param[in]   orc_ItemName    Item name for showing
   \param[in]   orc_ItemSubName Selected sub sub mode name

   \created     18.05.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_NagUseCaseViewWidget::UpdateItemName(const QString & orc_ItemName, const QString & orc_ItemSubName)
{
   this->mc_ItemName = orc_ItemName;
   this->mc_ItemSubSubName = orc_ItemSubName;
   m_HandleSubSections();
}
