//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Label for widget heading sub title (implementation)

   Label for widget heading sub title.
   This class does not contain any functionality,
   but needs to exist, to have a unique group,
   to apply a specific stylesheet for.

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     27.09.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "C_OgeLabHeadingWidgetSubTitle.h"

/* -- Used Namespaces ------------------------------------------------------ */

using namespace stw_opensyde_gui_elements;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_Parent Optional pointer to parent

   \created     27.09.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_OgeLabHeadingWidgetSubTitle::C_OgeLabHeadingWidgetSubTitle(QWidget * const opc_Parent) :
   QLabel(opc_Parent)
{
}
