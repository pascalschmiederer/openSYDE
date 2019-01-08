//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Class to find bus or node by string

   Class to find bus or node by string

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     08.09.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "CSCLString.h"
#include "C_PuiBsFindNameHelper.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Constructor

   Save string

   \param[in]     orc_Cmp   Search string

   \created     08.09.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_PuiBsFindNameHelper::C_PuiBsFindNameHelper(const stw_scl::C_SCLString & orc_Cmp) :
   mc_Cmp(orc_Cmp)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Compare string with string

   \param[in] opc_Cur String to compare

   \return
   true: match
   false: no match

   \created     08.09.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
bool C_PuiBsFindNameHelper::operator ()(const stw_scl::C_SCLString * const opc_Cur) const
{
   bool q_Retval = false;

   if (opc_Cur != NULL)
   {
      if (*opc_Cur == mc_Cmp)
      {
         q_Retval = true;
      }
   }
   return q_Retval;
}
