//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Text edit with automatic eliding (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     19.05.2017  STW/B.Bayer
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_OGETEDELIDED_H
#define C_OGETEDELIDED_H

/* -- Includes ------------------------------------------------------------- */
#include "C_OgeTedContextMenuBase.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_OgeTedElided :
   public C_OgeTedContextMenuBase
{
public:
   C_OgeTedElided(QWidget * const opc_Parent = NULL);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void paintEvent(QPaintEvent * const opc_Event) override;
   //lint -restore
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif