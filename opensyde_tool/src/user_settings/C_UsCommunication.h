//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Project bus user settings (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     27.04.2018  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_USCOMMUNICATION_H
#define C_USCOMMUNICATION_H

/* -- Includes ------------------------------------------------------------- */
#include <QString>
#include "C_OSCCanMessageIdentificationIndices.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_UsCommunication
{
public:
   C_UsCommunication(void);

   void SetMessageSelected(const stw_opensyde_core::C_OSCCanProtocol::E_Type oe_SelectedProtocol,
                           const bool oq_MessageSelected, const QString & orc_SelectedMessageName,
                           const bool oq_SignalSelected, const QString & orc_SelectedSignalName);
   void GetLastSelectedMessage(stw_opensyde_core::C_OSCCanProtocol::E_Type & ore_SelectedProtocol,
                               bool & orq_MessageSelected, QString & orc_SelectedMessageName, bool & orq_SignalSelected,
                               QString & orc_SelectedSignalName) const;

private:
   stw_opensyde_core::C_OSCCanProtocol::E_Type me_SelectedProtocol;
   bool mq_MessageSelected;
   QString mc_SelectedMessageName;
   bool mq_SignalSelected;
   QString mc_SelectedSignalName;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
