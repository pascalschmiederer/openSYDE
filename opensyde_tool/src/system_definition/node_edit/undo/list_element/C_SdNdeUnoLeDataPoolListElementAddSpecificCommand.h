//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Data pool list element add specific undo command (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     20.02.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SDNDEUNOLEDATAPOOLLISTELEMENTADDSPECIFICCOMMAND_H
#define C_SDNDEUNOLEDATAPOOLLISTELEMENTADDSPECIFICCOMMAND_H

/* -- Includes ------------------------------------------------------------- */

#include "C_SdNdeUnoLeDataPoolListElementAddCommand.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_SdNdeUnoLeDataPoolListElementAddSpecificCommand :
   public C_SdNdeUnoLeDataPoolListElementAddCommand
{
public:
   C_SdNdeUnoLeDataPoolListElementAddSpecificCommand(const stw_types::uint32 & oru32_NodeIndex,
                                                     const stw_types::uint32 & oru32_DataPoolIndex,
                                                     const stw_types::uint32 & oru32_DataPoolListIndex,
                                                     C_SdNdeDataPoolListModelViewManager * const opc_DataPoolListModelViewManager,
                                                     const std::vector<stw_types::uint32> & orc_Indices,
                                                     const std::vector<stw_opensyde_core::C_OSCNodeDataPoolListElement> & orc_OSCData,
                                                     const std::vector<stw_opensyde_gui_logic::
                                                                       C_PuiSdNodeDataPoolListElement> & orc_UIData,
                                                     QUndoCommand * const opc_Parent = NULL);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
