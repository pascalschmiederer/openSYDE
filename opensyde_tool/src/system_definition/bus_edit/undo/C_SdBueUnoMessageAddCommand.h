//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Message add undo command (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     20.04.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SDBUEUNOMESSAGEADDCOMMAND_H
#define C_SDBUEUNOMESSAGEADDCOMMAND_H

/* -- Includes ------------------------------------------------------------- */
#include "C_SdBueUnoMessageAddDeleteBaseCommand.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_SdBueUnoMessageAddCommand :
   public C_SdBueUnoMessageAddDeleteBaseCommand
{
public:
   C_SdBueUnoMessageAddCommand(const stw_opensyde_core::C_OSCCanMessageIdentificationIndices & orc_MessageId,
                               C_PuiSdNodeCanMessageSyncManager * const opc_MessageSyncManager,
                               stw_opensyde_gui::C_SdBueMessageSelectorTreeWidget * const opc_MessageTreeWidget,
                               QUndoCommand * const opc_Parent = NULL);

   void SetInitialData(const stw_opensyde_core::C_OSCCanMessage & orc_Message,
                       const std::vector<stw_opensyde_core::C_OSCNodeDataPoolListElement> & orc_OSCSignalCommons,
                       const std::vector<C_PuiSdNodeDataPoolListElement> & orc_UISignalCommons,
                       const std::vector<C_PuiSdNodeCanSignal> & orc_UISignals,
                       const std::vector<QString> & orc_OwnerNodeName,
                       const std::vector<stw_types::uint32> & orc_OwnerNodeInterfaceIndex,
                       const std::vector<bool> & orc_OwnerIsTxFlag);

   virtual void redo(void);
   virtual void undo(void);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
