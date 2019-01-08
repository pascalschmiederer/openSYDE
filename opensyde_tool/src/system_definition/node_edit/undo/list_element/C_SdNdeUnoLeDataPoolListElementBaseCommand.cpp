//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Data pool list element base command (implementation)

   Data pool list element base command

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     25.01.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "stwtypes.h"
#include "C_SdNdeUnoLeDataPoolListElementBaseCommand.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
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

   \param[in]     oru32_NodeIndex                  Node index
   \param[in]     oru32_DataPoolIndex              Node data pool index
   \param[in]     oru32_DataPoolListIndex          Node data pool list index
   \param[in,out] opc_DataPoolListModelViewManager Data pool lists model view manager to get objects to perform actions on
   \param[in]     orc_Text                         Optional command text for informational display
   \param[in,out] opc_Parent                       Optional pointer to parent

   \created     25.01.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_SdNdeUnoLeDataPoolListElementBaseCommand::C_SdNdeUnoLeDataPoolListElementBaseCommand(const uint32 & oru32_NodeIndex,
                                                                                       const uint32 & oru32_DataPoolIndex, const uint32 & oru32_DataPoolListIndex, C_SdNdeDataPoolListModelViewManager * const opc_DataPoolListModelViewManager, const QString & orc_Text,
                                                                                       QUndoCommand * const opc_Parent)
   :
   QUndoCommand(orc_Text, opc_Parent),
   mu32_NodeIndex(oru32_NodeIndex),
   mu32_DataPoolIndex(oru32_DataPoolIndex),
   mu32_DataPoolListIndex(oru32_DataPoolListIndex),
   mpc_DataPoolListModelViewManager(opc_DataPoolListModelViewManager)
{
}
