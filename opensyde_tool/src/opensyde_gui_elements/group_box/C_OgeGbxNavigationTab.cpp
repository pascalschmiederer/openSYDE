//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Group box for navigation tab style (implementation)

   Group box for navigation tab style.
   This class does not contain any functionality,
   but needs to exist, to have a unique group,
   to apply a specific stylesheet for.

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     30.07.2018  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "C_OgeWiUtil.h"
#include "C_OgeGbxNavigationTab.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_opensyde_gui_logic;
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

   \created     30.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_OgeGbxNavigationTab::C_OgeGbxNavigationTab(QWidget * const opc_Parent) :
   QGroupBox(opc_Parent)
{
   C_OgeWiUtil::h_ApplyStylesheetProperty(this, "IncludeLeftBorder", false);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set include left border flag

   \param[in] oq_Include Include left border flag

   \created     30.07.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgeGbxNavigationTab::SetIncludeLeftBorder(const bool oq_Include)
{
   C_OgeWiUtil::h_ApplyStylesheetProperty(this, "IncludeLeftBorder", oq_Include);
}
