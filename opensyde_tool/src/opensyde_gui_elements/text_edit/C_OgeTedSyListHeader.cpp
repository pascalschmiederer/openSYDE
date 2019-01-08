//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Text edit for system view list header (implementation)

   Text edit for system view list header.
   This class does not contain any functionality,
   but needs to exist, to have a unique group,
   to apply a specific stylesheet for.

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     23.11.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "C_OgeTedSyListHeader.h"

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

   \created     23.11.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_OgeTedSyListHeader::C_OgeTedSyListHeader(QWidget * const opc_Parent) :
   C_OgeTedListHeaderBase(opc_Parent)
{
   this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten resize event slot

   Here: Reevaluate text length

   \param[in,out] opc_Event Event identification and information

   \created     18.01.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgeTedSyListHeader::resizeEvent(QResizeEvent * const opc_Event)
{
   C_OgeTedListHeaderBase::resizeEvent(opc_Event);
   //Do not touch if user has focus
   this->HideFullText(false);
}
