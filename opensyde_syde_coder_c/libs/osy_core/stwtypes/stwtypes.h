//-----------------------------------------------------------------------------
/*!
   \file       stwtypes.h
   \brief      STW specific standard defines and types

   Standard typedefs in compliance with STW C++ Coding Rules.
   here: for Windows applications.

   Suitable for applications compiled with:
   - Borland C++ Builder
   - MinGW
   - MS Visual C++

   \implementation
   project     to be used in all kinds of applications
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     23.08.2007  STW/U.Meyler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef STWTYPESH
#define STWTYPESH

/* -- Includes ------------------------------------------------------------- */

/* -- Namespace ------------------------------------------------------------ */
#ifdef __cplusplus
namespace stw_types
{
#endif
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

typedef unsigned char uint8;       ///< data type  8bit unsigned
typedef signed char sint8;         ///< data type  8bit signed
typedef unsigned short uint16;     ///< data type 16bit unsigned
typedef signed short sint16;       ///< data type 16bit signed
typedef unsigned long uint32;      ///< data type 32bit unsigned
typedef signed long sint32;        ///< data type 32bit signed
typedef unsigned long long uint64; ///< data type 64bit unsigned
typedef signed long long sint64;   ///< data type 64bit signed
typedef float float32;             ///< data type IEEE 32bit float
typedef double float64;            ///< data type IEEE 64bit float
typedef long double float80;       ///< data type IEEE 80bit float

// native data types
typedef unsigned int uintn; ///< data type native unsigned int
typedef signed int sintn;   ///< data type native signed int
typedef char charn;         ///< data type native char

/* -- Extern Global Variables ---------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
