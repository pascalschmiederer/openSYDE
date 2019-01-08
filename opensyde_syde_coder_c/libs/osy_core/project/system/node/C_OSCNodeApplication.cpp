//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Data class for application related information (implementation)

   Data class for application related information

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     22.12.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "stwerrors.h"

#include "C_OSCNodeApplication.h"

#include "CSCLChecksums.h"
#include "C_OSCLoggingHandler.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_core;
using namespace stw_scl;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Default constructor

   \created     22.12.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_OSCNodeApplication::C_OSCNodeApplication(void) :
   e_Type(eBINARY),
   c_Name(""),
   c_Comment(""),
   q_Active(true),
   u8_ProcessId(0),
   c_ProjectPath(""),
   c_IDECall(""),
   c_CodeGeneratorPath(""),
   c_GeneratePath(""),
   c_ResultPath("")
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Calculates the hash value over all data

   The hash value is a 32 bit CRC value.
   It is not endian-safe, so it should only be used on the same system it is created on.

   \param[in,out] oru32_HashValue    Hash value with initial [in] value and result [out] value

   \created     21.03.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_OSCNodeApplication::CalcHash(uint32 & oru32_HashValue) const
{
   stw_scl::C_SCLChecksums::CalcCRC32(&this->e_Type, sizeof(this->e_Type), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(this->c_Name.c_str(), this->c_Name.Length(), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(this->c_Comment.c_str(), this->c_Comment.Length(), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->q_Active, sizeof(this->q_Active), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->u8_ProcessId, sizeof(this->u8_ProcessId), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(this->c_ProjectPath.c_str(), this->c_ProjectPath.Length(), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(this->c_IDECall.c_str(), this->c_IDECall.Length(), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(this->c_CodeGeneratorPath.c_str(),
                                      this->c_CodeGeneratorPath.Length(), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(this->c_GeneratePath.c_str(), this->c_GeneratePath.Length(), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(this->c_ResultPath.c_str(), this->c_ResultPath.Length(), oru32_HashValue);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Transform application type to string

   \param[in] ore_Application Application type

   \return
   Stringified application type

   \created     10.01.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_SCLString C_OSCNodeApplication::h_ApplicationToString(const C_OSCNodeApplication::E_Type & ore_Application)
{
   C_SCLString c_Retval;

   switch (ore_Application)
   {
   case C_OSCNodeApplication::eFILE_CONTAINER:
      c_Retval = "file_container";
      break;
   case C_OSCNodeApplication::eBINARY:
      c_Retval = "binary";
      break;
   case C_OSCNodeApplication::ePROGRAMMABLE_APPLICATION:
      c_Retval = "programming_application";
      break;
   default:
      c_Retval = "invalid";
      break;
   }
   return c_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Transform string to application type

   \param[in]  orc_String String to interpret
   \param[out] ore_Type   Application type

   \return
   C_NO_ERR   no error

   \created     10.01.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_OSCNodeApplication::h_StringToApplication(const C_SCLString & orc_String,
                                                   C_OSCNodeApplication::E_Type & ore_Type)
{
   sint32 s32_Retval = C_NO_ERR;

   if (orc_String == "file_container")
   {
      ore_Type = C_OSCNodeApplication::eFILE_CONTAINER;
   }
   else if (orc_String == "binary")
   {
      ore_Type = C_OSCNodeApplication::eBINARY;
   }
   else if (orc_String == "programming_application")
   {
      ore_Type = C_OSCNodeApplication::ePROGRAMMABLE_APPLICATION;
   }
   else
   {
      //set binary if unknown type read
      ore_Type = C_OSCNodeApplication::eBINARY;
      osc_write_log_warning("Loading node definition",
                            "Invalid value for application type. Unknown type: " + orc_String + ". "
                            "Type is replaced with: " + h_ApplicationToString(C_OSCNodeApplication::eBINARY));
   }

   return s32_Retval;
}