//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Label for group item (header)

   Label for group item (note: main module description should be in .cpp file)

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     27.09.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_OGELABGROUPITEM_H
#define C_OGELABGROUPITEM_H

/* -- Includes ------------------------------------------------------------- */

#include "C_OgeLabToolTipBase.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_OgeLabGroupItem :
   public C_OgeLabToolTipBase
{
   Q_OBJECT

public:
   C_OgeLabGroupItem(QWidget * const opc_Parent = NULL);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
