//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Text browser field for report widgets (implementation)

   Text browser field for report widgets.
   This class does not contain any functionality,
   but needs to exist, to have a unique group,
   to apply a specific stylesheet for.

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     17.07.2018  STW/G.Scupin
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "C_OgeTebReport.h"

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

   \created     17.07.2018  STW/G.Scupin
*/
//-----------------------------------------------------------------------------
C_OgeTebReport::C_OgeTebReport(QWidget * const opc_Parent) :
   C_OgeTebContextMenuBase(opc_Parent)
{
}
