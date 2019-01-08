//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Data pool list element add undo command (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     27.01.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SDNDEUNOLEDATAPOOLLISTELEMENTADDCOMMAND_H
#define C_SDNDEUNOLEDATAPOOLLISTELEMENTADDCOMMAND_H

/* -- Includes ------------------------------------------------------------- */

#include "C_SdNdeUnoLeDataPoolListElementAddDeleteBaseCommand.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_SdNdeUnoLeDataPoolListElementAddCommand :
   public C_SdNdeUnoLeDataPoolListElementAddDeleteBaseCommand
{
public:
   C_SdNdeUnoLeDataPoolListElementAddCommand(const stw_types::uint32 & oru32_NodeIndex,
                                             const stw_types::uint32 & oru32_DataPoolIndex,
                                             const stw_types::uint32 & oru32_DataPoolListIndex,
                                             C_SdNdeDataPoolListModelViewManager * const opc_DataPoolListModelViewManager,
                                             const std::vector<stw_types::uint32> & orc_Indices,
                                             QUndoCommand * const opc_Parent = NULL);
   virtual void redo(void) override;
   virtual void undo(void) override;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
