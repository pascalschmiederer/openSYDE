//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Label for tree node data pool (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     22.11.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_OGELABTREENODEDATAPOOL_H
#define C_OGELABTREENODEDATAPOOL_H

/* -- Includes ------------------------------------------------------------- */
#include <QLabel>

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_OgeLabTreeNodeDataPool :
   public QLabel
{
   Q_OBJECT

public:
   C_OgeLabTreeNodeDataPool(QWidget * const opc_Parent = NULL);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
