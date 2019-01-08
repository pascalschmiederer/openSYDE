//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Simple line with style utility functions (implementation)

   Simple line with style utility functions

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     18.08.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <QPen>
#include "C_GiLiSimpleLine.h"
#include "gitypes.h"
#include "C_OSCUtils.h"

/* -- Used Namespaces ------------------------------------------------------ */

using namespace stw_opensyde_gui;
using namespace stw_opensyde_core;
using namespace stw_types;

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

   \created     18.08.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_GiLiSimpleLine::C_GiLiSimpleLine(QGraphicsItem * const opc_Parent) :
   QGraphicsLineItem(opc_Parent)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Default destructor

   Clean up.

   \created     18.08.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_GiLiSimpleLine::~C_GiLiSimpleLine()
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Returns the type of this itme

   \return  ID

   \created     01.09.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
sintn C_GiLiSimpleLine::type() const
{
   return msn_GRAPHICS_ITEM_SIMPLELINE;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set line width

   \param[in] ors32_Width New line width

   \created     17.08.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_GiLiSimpleLine::SetWidth(const stw_types::sint32 & ors32_Width)
{
   QPen c_Pen = this->pen();

   c_Pen.setWidth(ors32_Width);
   this->setPen(c_Pen);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set color

   \param[in] orc_Color New color

   \created     17.08.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_GiLiSimpleLine::SetColor(const QColor & orc_Color)
{
   QPen c_Pen = this->pen();

   c_Pen.setColor(orc_Color);
   this->setPen(c_Pen);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get width

   \created     18.08.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
sint32 C_GiLiSimpleLine::GetWidth() const
{
   const QPen c_Pen = this->pen();

   return c_Pen.width();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get color

   \created     18.08.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
QColor C_GiLiSimpleLine::GetColor() const
{
   const QPen c_Pen = this->pen();

   return c_Pen.color();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Returns the position

   Returns the position which is most top left. The return value is either the start point p1
   or the end point p2 of the line.

   \created     05.09.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
QPointF C_GiLiSimpleLine::GetPos(void) const
{
   QPointF c_Pos;
   QLineF c_Line = this->line();

   // Which point is most top?
   if (c_Line.p1().y() < c_Line.p2().y())
   {
      c_Pos = this->mapToScene(c_Line.p1());
   }
   else if (c_Line.p1().y() > c_Line.p2().y())
   {
      c_Pos = this->mapToScene(c_Line.p2());
   }
   else
   {
      // Which point is most left?
      if (c_Line.p1().x() < c_Line.p2().x())
      {
         c_Pos = this->mapToScene(c_Line.p1());
      }
      else
      {
         c_Pos = this->mapToScene(c_Line.p2());
      }
   }

   return c_Pos;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Returns the coordinates of P1 of the line

   \created     06.09.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
QPointF C_GiLiSimpleLine::GetP1() const
{
   return this->line().p1();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Returns the coordinates of P2 of the line

   \created     06.09.2016  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
QPointF C_GiLiSimpleLine::GetP2() const
{
   return this->line().p2();
}
