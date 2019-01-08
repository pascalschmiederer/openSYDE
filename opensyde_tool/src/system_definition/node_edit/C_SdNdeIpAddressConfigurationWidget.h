//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for IP Address configuration (implementation)

   Widget for IP Address configuration

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     26.02.2018  STW/S.Singer
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SDNDEIPADDRESSCONFIGURATIONWIDGET_H
#define C_SDNDEIPADDRESSCONFIGURATIONWIDGET_H

/* -- Includes ------------------------------------------------------------- */
#include <QWidget>
#include <QRegExpValidator>
#include "stwtypes.h"
#include "C_OgePopUpDialog.h"

/* -- Namespace ------------------------------------------------------------ */
namespace Ui
{
class C_SdNdeIpAddressConfigurationWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_SdNdeIpAddressConfigurationWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeIpAddressConfigurationWidget(stw_opensyde_gui_elements::C_OgePopUpDialog & orc_Parent,
                                                const stw_types::uint32 ou32_NodeIndex,
                                                const stw_types::uint32 ou32_ComIf);
   ~C_SdNdeIpAddressConfigurationWidget(void);

   void InitStaticNames(void) const;

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void keyPressEvent(QKeyEvent * const opc_KeyEvent) override;
   //lint -restore

private:
   Ui::C_SdNdeIpAddressConfigurationWidget * mpc_Ui;
   //lint -e{1725} Only problematic if copy or assignment is allowed
   stw_opensyde_gui_elements::C_OgePopUpDialog & mrc_ParentDialog;
   const stw_types::uint32 mu32_NodeIndex;
   const stw_types::uint32 mu32_ComIf;
   QRegularExpressionValidator * mpc_Validator;

   void m_LoadData(void) const;
   void m_OkClicked(void);
   void m_CancelClicked(void);
   bool mq_VerifyInput(void) const;
   void m_OnSubNetMaskFocusIn(void) const;
   void m_OnSubNetMaskFocusOut(void) const;
   void m_OnGatewayMaskFocusIn(void) const;
   void m_OnGatewayMaskFocusOut(void) const;

   //Avoid call
   C_SdNdeIpAddressConfigurationWidget(const C_SdNdeIpAddressConfigurationWidget &);
   C_SdNdeIpAddressConfigurationWidget & operator =(const C_SdNdeIpAddressConfigurationWidget &);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
