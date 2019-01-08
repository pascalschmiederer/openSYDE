//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Move undo command (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     18.11.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SEBUNOMOVECOMMAND_H
#define C_SEBUNOMOVECOMMAND_H

/* -- Includes ------------------------------------------------------------- */

#include "C_SebUnoBaseCommand.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_SebUnoMoveCommand :
   public C_SebUnoBaseCommand
{
public:
   C_SebUnoMoveCommand(QGraphicsScene * const opc_Scene, const std::vector<stw_types::uint64> & orc_IDs,
                       const QPointF & orc_PositionDifference, QUndoCommand * const opc_Parent = NULL);
   virtual ~C_SebUnoMoveCommand(void);
   virtual void undo(void) override;
   virtual void redo(void) override;

   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual bool mergeWith(const QUndoCommand * const opc_Command);
   //lint -restore

private:
   QPointF mc_PositionDifference;

   void m_UndoSingle(QGraphicsItem * const opc_Item) const;
   void m_RedoSingle(QGraphicsItem * const opc_Item) const;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
