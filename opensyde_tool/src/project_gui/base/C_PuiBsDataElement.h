//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Base class for graphics items with any data elements (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     19.07.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_PUIBSDATAELEMENT_H
#define C_PUIBSDATAELEMENT_H

/* -- Includes ------------------------------------------------------------- */
#include "stwtypes.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_PuiBsDataElement
{
public:
   C_PuiBsDataElement(const stw_types::sint32 & ors32_Index);
   virtual ~C_PuiBsDataElement(void);

   stw_types::sint32 GetIndex(void) const;

   virtual void LoadData(void) = 0;
   virtual void UpdateData(void) = 0;
   virtual void DeleteData(void) = 0;

   enum E_Action
   {
      eDELETE, ///< Change type: delete
      eADD     ///< Change type: add
   };

protected:
   stw_types::sint32 ms32_Index; ///< Index of according data item in specific data array
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
