//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       openSYDE: Diagnostic protocol driver abstract base class

   For details cf. documentation in .h file.

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     02.03.2017  STW/A.Stangl
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <iostream>
#include "C_OSCDiagProtocolBase.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_opensyde_core;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */
//-----------------------------------------------------------------------------
/*!
   \brief   constructor

   Set up class

   \created     21.03.2017  STW/A.Stangl
*/
//-----------------------------------------------------------------------------
C_OSCDiagProtocolBase::C_OSCDiagProtocolBase(void) :
   mpr_OnDataPoolReadEventReceived(NULL),
   mpr_OnDataPoolReadEventErrorReceived(NULL),
   mpv_OnAsyncInstance(NULL)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   constructor

   Tear down class

   \created     17.03.2017  STW/A.Stangl
*/
//-----------------------------------------------------------------------------
C_OSCDiagProtocolBase::~C_OSCDiagProtocolBase(void)
{
   mpv_OnAsyncInstance = NULL;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Initialization of the protocol

   Setting all callbacks for the async messages

   \param[in]  opr_DataPoolReadEventReceived       function to be called if an async DataPoolReadEvent is received
   \param[in]  opr_DataPoolReadEventErrorReceived  function to be called if an async DataPoolReadEvent error message
                                                   is received
   \param[in]  opv_Instance                        instance pointer to pass back when invoking read event callback

   \created     21.07.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_OSCDiagProtocolBase::Initialize(const PR_DataPoolReadEventReceived opr_DataPoolReadEventReceived,
                                       const PR_DataPoolReadEventErrorReceived opr_DataPoolReadEventErrorReceived,
                                       void * const opv_Instance)
{
   mpr_OnDataPoolReadEventErrorReceived = opr_DataPoolReadEventErrorReceived;
   mpr_OnDataPoolReadEventReceived = opr_DataPoolReadEventReceived;
   mpv_OnAsyncInstance = opv_Instance;
}
