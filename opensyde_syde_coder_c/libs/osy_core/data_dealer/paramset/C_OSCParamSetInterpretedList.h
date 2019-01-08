//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Data class, containing the interpreted entries for one list in a parameter set file (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     12.10.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_OSCPARAMSETINTERPRETEDLIST_H
#define C_OSCPARAMSETINTERPRETEDLIST_H

/* -- Includes ------------------------------------------------------------- */
#include <vector>
#include "C_OSCParamSetInterpretedElement.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_core
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

///Data class, containing the interpreted entries for one list in a parameter set file
class C_OSCParamSetInterpretedList
{
public:
   C_OSCParamSetInterpretedList(void);

   stw_scl::C_SCLString c_Name;                             ///< List name
   std::vector<C_OSCParamSetInterpretedElement> c_Elements; ///< Interpreted elements of this list
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
