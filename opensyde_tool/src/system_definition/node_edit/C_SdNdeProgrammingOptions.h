//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Dialog for system node programming properties (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     29.03.2018  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SDNDEPROGRAMMINGOPTIONS_H
#define C_SDNDEPROGRAMMINGOPTIONS_H

/* -- Includes ------------------------------------------------------------- */
#include <QWidget>
#include "stwtypes.h"
#include "C_OgePopUpDialog.h"

namespace Ui
{
class C_SdNdeProgrammingOptions;
}
/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */
class C_SdNdeProgrammingOptions :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeProgrammingOptions(stw_opensyde_gui_elements::C_OgePopUpDialog & orc_Parent,
                                      const stw_types::uint32 ou32_NodeIndex);
   ~C_SdNdeProgrammingOptions(void);

   void InitStaticNames(void) const;
   void Save(void) const;

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void keyPressEvent(QKeyEvent * const opc_KeyEvent) override;
   //lint -restore

private:
   Ui::C_SdNdeProgrammingOptions * mpc_Ui;
   //lint -e{1725} Only problematic if copy or assignment is allowed
   stw_opensyde_gui_elements::C_OgePopUpDialog & mrc_ParentDialog;
   const stw_types::uint32 mu32_NodeIndex;

   void m_Load(void) const;

   void m_OkClicked(void);
   void m_CancelClicked(void);
   //Avoid call
   C_SdNdeProgrammingOptions(const C_SdNdeProgrammingOptions &);
   C_SdNdeProgrammingOptions & operator =(const C_SdNdeProgrammingOptions &);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
