//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Button in material design without borders

   \implementation
   project     opensyde
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     06.07.2016  STW/B.Bayer
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <QPainter>
#include "C_OgePubStandard.h"

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

   \created     06.07.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
C_OgePubStandard::C_OgePubStandard(QWidget * const opc_Parent) :
   C_OgePubToolTipBase(opc_Parent)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Adaption of the pen color

   Adapts the pen color depending of the button state.
   Colors are set in the QPalette of the element:
   - ButtonText: Not active
   - BrightText: Mouse over
   - HighlightedText: Mouse click

   \param[in,out] opc_Painter  Pointer to actual QPainter instance

   \created     06.07.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_OgePubStandard::m_SetPenColorForFont(QPainter * const opc_Painter) const
{
   if (this->underMouse() == true)
   {
      if (this->isDown() == false)
      {
         opc_Painter->setPen(QColor(250, 250, 250));
      }
      else
      {
         opc_Painter->setPen(QColor(94, 163, 67));
      }
   }
   else
   {
      opc_Painter->setPen(QColor(211, 211, 216));
   }
}
