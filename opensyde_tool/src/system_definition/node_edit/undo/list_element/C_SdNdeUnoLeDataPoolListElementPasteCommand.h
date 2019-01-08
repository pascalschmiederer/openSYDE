//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Data pool list element paste undo command (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     25.01.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SDNDEUNOLEDATAPOOLLISTELEMENTPASTECOMMAND_H
#define C_SDNDEUNOLEDATAPOOLLISTELEMENTPASTECOMMAND_H

/* -- Includes ------------------------------------------------------------- */

#include "C_SdNdeUnoLeDataPoolListElementAddDeleteBaseCommand.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_SdNdeUnoLeDataPoolListElementPasteCommand :
   public C_SdNdeUnoLeDataPoolListElementAddDeleteBaseCommand
{
public:
   C_SdNdeUnoLeDataPoolListElementPasteCommand(const stw_types::uint32 & oru32_NodeIndex,
                                               const stw_types::uint32 & oru32_DataPoolIndex,
                                               const stw_types::uint32 & oru32_DataPoolListIndex,
                                               C_SdNdeDataPoolListModelViewManager * const opc_DataPoolListModelViewManager,
                                               QUndoCommand * const opc_Parent = NULL);
   bool InitialSetup(const stw_types::uint32 & oru32_FirstIndex);
   virtual void redo(void) override;
   virtual void undo(void) override;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
