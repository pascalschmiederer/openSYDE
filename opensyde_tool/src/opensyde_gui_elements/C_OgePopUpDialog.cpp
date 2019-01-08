//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Custom pop-up dialog (implementation)

   Custom pop-up dialog

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     20.07.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <QPainter>
#include <QGraphicsColorizeEffect>
#include <QGraphicsDropShadowEffect>

#include "stwtypes.h"
#include "C_HeHandler.h"
#include "C_OgePopUpDialog.h"
#include "ui_C_OgePopUpDialog.h"
#include "C_OgeWiUtil.h"

#include "constants.h"

/* -- Used Namespaces ------------------------------------------------------ */

using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_elements;
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

   \param[in,out] opc_Parent            Optional pointer to parent
   \param[in]     opc_EffectTargetChild Optional pointer to child widget of dialog to apply shadow effect to

   \created     20.07.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_OgePopUpDialog::C_OgePopUpDialog(QWidget * const opc_Parent, QWidget * const opc_EffectTargetChild) :
   QDialog(opc_Parent),
   mpc_Ui(new Ui::C_OgePopUpDialog),
   mq_Pressed(false),
   mc_Overlay(new C_OgeOverlay(opc_Parent, opc_EffectTargetChild)),
   mpc_EffectTargetChild(opc_EffectTargetChild),
   mq_NotifyAndBlockClose(false)
{
   mpc_Ui->setupUi(this);
   //background not selectable
   if (opc_EffectTargetChild == NULL)
   {
      this->setWindowModality(Qt::ApplicationModal);
   }
   else
   {
      this->setWindowModality(Qt::WindowModal);
   }
   //No frame
   this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
   //transparency
   setAttribute(Qt::WA_TranslucentBackground);
   //shadow
   {
      QColor c_Color;
      QGraphicsDropShadowEffect * pc_Shadow = new QGraphicsDropShadowEffect(this->mpc_Ui->pc_GroupBoxPopUp);
      pc_Shadow->setBlurRadius(40.0);
      pc_Shadow->setOffset(0.0);
      c_Color = mc_STYLE_GUIDE_COLOR_34;
      c_Color.setAlpha(128);
      pc_Shadow->setColor(c_Color);
      this->mpc_Ui->pc_GroupBoxPopUp->setGraphicsEffect(pc_Shadow);
      //lint -e{429}  no memory leak because of the parent of pc_Shadow and the Qt memory management
   }
   //Deactivate debug info
   this->mpc_Ui->pc_GroupBoxTitle->setTitle("");
   //Overlay
   mc_Overlay->show();

   //Window icon
   C_OgeWiUtil::h_SetWindowIcon(this);

   //Events
   connect(this->mpc_Ui->pc_LabelTitle, &C_OgeLabPopUpTitle::SigMousePress, this,
           &C_OgePopUpDialog::HandleMousePressEvent);
   connect(this->mpc_Ui->pc_LabelTitle, &C_OgeLabPopUpTitle::SigMouseMove, this,
           &C_OgePopUpDialog::HandleMouseMoveEvent);
   connect(this->mpc_Ui->pc_LabelTitle, &C_OgeLabPopUpTitle::SigMouseRelease, this,
           &C_OgePopUpDialog::HandleMouseReleaseEvent);
   connect(this->mpc_Ui->pc_LabelSubTitle, &C_OgeLabPopUpTitle::SigMousePress, this,
           &C_OgePopUpDialog::HandleMousePressEvent);
   connect(this->mpc_Ui->pc_LabelSubTitle, &C_OgeLabPopUpTitle::SigMouseMove, this,
           &C_OgePopUpDialog::HandleMouseMoveEvent);
   connect(this->mpc_Ui->pc_LabelSubTitle, &C_OgeLabPopUpTitle::SigMouseRelease, this,
           &C_OgePopUpDialog::HandleMouseReleaseEvent);

   // hide overlay when popup is closed (needed for popups if two read-only mode nodes are open)
   connect(this, &C_OgePopUpDialog::accepted, this, &C_OgePopUpDialog::HideOverlay);
   connect(this, &C_OgePopUpDialog::rejected, this, &C_OgePopUpDialog::HideOverlay);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Default destructor

   Clean up.

   \created     20.07.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------

C_OgePopUpDialog::~C_OgePopUpDialog()
{
   //Overlay
   HideOverlay();
   delete this->mpc_Ui;
   //lint -e{1540} Never took ownership of item
}

//-----------------------------------------------------------------------------
/*!
   \brief  Place widget in pop up dialog

   \param[in,out] opc_Widget Widget to place in pop up dialog

   \created     21.07.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------

void C_OgePopUpDialog::SetWidget(QWidget * const opc_Widget)
{
   if (opc_Widget != NULL)
   {
      stw_types::sintn sn_Index;

      //save class name for help call
      //lint -e{10,48,64,734,746,1013,1055,1960} Will be defined via moc compiler, PC lint unable to handle this construct
      this->mc_WidgetClassName = opc_Widget->metaObject()->className();

      opc_Widget->setParent(this);
      this->mpc_Ui->pc_VerticalLayout->addWidget(opc_Widget);
      sn_Index = this->mpc_Ui->pc_VerticalLayout->indexOf(opc_Widget);
      this->mpc_Ui->pc_VerticalLayout->setStretch(sn_Index, 1);
      opc_Widget->setAutoFillBackground(true);
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set title

   \param[in,out] orc_Str New title (rich text supported)

   \created     21.07.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------

void C_OgePopUpDialog::SetTitle(const QString & orc_Str)
{
   this->mpc_Ui->pc_LabelTitle->setText(orc_Str);
   mc_Title = orc_Str;
   m_SetWindowTitle();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set sub title

   \param[in,out] orc_Str New sub title (rich text supported)

   \created     28.10.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------

void C_OgePopUpDialog::SetSubTitle(const QString & orc_Str)
{
   this->mpc_Ui->pc_LabelSubTitle->setText(orc_Str);
   mc_Subtitle = orc_Str;
   m_SetWindowTitle();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set window title

   Set the title of the little window shown as preview when hovering
   over the windows task bar.

   \created     22.05.2018  STW/G.Scupin
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::m_SetWindowTitle()
{
   this->setWindowTitle("openSYDE - " + mc_Title + " - " + mc_Subtitle);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Activate move window

   \param[in,out] opc_Event Event identification and information

   \created     20.07.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------

void C_OgePopUpDialog::HandleMousePressEvent(const QMouseEvent * const opc_Event)
{
   mq_Pressed = true;
   this->mc_LastPos = this->mapToGlobal(opc_Event->pos());
}

//-----------------------------------------------------------------------------
/*!
   \brief   Deactivate move window

   \param[in,out] opc_Event Event identification and information

   \created     20.07.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------

void C_OgePopUpDialog::HandleMouseReleaseEvent(QMouseEvent * const opc_Event)
{
   Q_UNUSED(opc_Event)
   mq_Pressed = false;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Move window

   \param[in,out] opc_Event Event identification and information

   \created     20.07.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::HandleMouseMoveEvent(const QMouseEvent * const opc_Event)
{
   if (mq_Pressed == true)
   {
      const QPoint c_Cur = this->mapToGlobal(opc_Event->pos());
      const QPoint c_Diff = c_Cur - mc_LastPos;
      this->move(this->pos() + c_Diff);
      this->mc_LastPos = c_Cur;
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Resize new size

   \param[in] orc_Size New size

   \created     30.05.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::SetSize(const QSize & orc_Size)
{
   this->resize(orc_Size);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Resize widget to allowed maximum

   If opc_Widget is NULL, the top widget will be used as orientation.

   \param[in]  opc_Widget   Optional widget for orientation for maximum size

   \created     01.06.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::ApplyMaximumSize(const QWidget * const opc_Widget)
{
   const QWidget * pc_UsedWidget;

   if (opc_Widget != NULL)
   {
      pc_UsedWidget = opc_Widget;
   }
   else
   {
      pc_UsedWidget = C_OgeOverlay::h_GetTopWidget();
   }

   if (pc_UsedWidget != NULL)
   {
      this->resize(pc_UsedWidget->rect().adjusted(5, 5, -5, -5).size());
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Hide title bar

   \created     01.06.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::HideTitle(void) const
{
   this->mpc_Ui->pc_GroupBoxTitle->hide();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Hide overlay

   \created     28.06.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::HideOverlay(void) const
{
   if (this->mc_Overlay != NULL)
   {
      this->mc_Overlay->hide();
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Activate close notification

   Warning: proper handling of SigCloseIgnored required

   \param[in] oq_NotifyClose Flag to notify on close

   \created     07.06.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::SetNotifyAndBlockClose(const bool oq_NotifyAndBlockClose)
{
   this->mq_NotifyAndBlockClose = oq_NotifyAndBlockClose;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten key press event slot

   Here: Trigger help key press handling

   \param[in,out] opc_KeyEvent Event identification and information

   \created     04.11.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::keyPressEvent(QKeyEvent * const opc_KeyEvent)
{
   if (stw_opensyde_gui_logic::C_HeHandler::CheckHelpKey(opc_KeyEvent) == true)
   {
      stw_opensyde_gui_logic::C_HeHandler::GetInstance().CallSpecificHelpPage(this->mc_WidgetClassName);
   }
   QDialog::keyPressEvent(opc_KeyEvent);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten show event slot

   Here: Reposition to main window center

   \param[in,out] opc_Event Event identification and information

   \created     15.12.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::showEvent(QShowEvent * const opc_Event)
{
   const QWidget * pc_TopWidget;

   if (this->mpc_EffectTargetChild != NULL)
   {
      pc_TopWidget = C_OgeWiUtil::h_GetWidgetUnderNextPopUp(this->mpc_EffectTargetChild);
   }
   else
   {
      pc_TopWidget = C_OgeOverlay::h_GetTopWidget();
   }

   //Center dialog
   if (pc_TopWidget != NULL)
   {
      QPoint c_DialogCenter = mapToGlobal(this->rect().center());
      QPoint c_ParentWindowCenter = pc_TopWidget->window()->mapToGlobal(pc_TopWidget->window()->rect().center());
      move(c_ParentWindowCenter - c_DialogCenter);
   }
   QDialog::showEvent(opc_Event);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten close event slot

   Here: Ignore and forward close requests

   \param[in,out] opc_Event Event identification and information

   \created     07.06.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgePopUpDialog::closeEvent(QCloseEvent * const opc_Event)
{
   if (this->mq_NotifyAndBlockClose == true)
   {
      Q_EMIT this->SigCloseIgnored();
      opc_Event->ignore();
   }
   else
   {
      QDialog::closeEvent(opc_Event);
   }
}
