//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Visualization of the node boundary with its filled colors (header)

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     04.08.2016  STW/S.Singer
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_GINODEBOUNDARY_H
#define C_GINODEBOUNDARY_H

/* -- Includes ------------------------------------------------------------- */
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsDropShadowEffect>
#include <QPixmap>

#include "stwtypes.h"

#include "C_GiBiSizeableItem.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_GiNodeBoundary :
   public QGraphicsItem,
   public C_GiBiSizeableItem
{
public:
   C_GiNodeBoundary(const QString & orc_Text, const stw_types::float64 of64_Width, const stw_types::float64 of64_Height,
                    QGraphicsItem * const opc_Parent = NULL);
   virtual ~C_GiNodeBoundary();

   virtual QRectF boundingRect() const;
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void paint(QPainter * const opc_Painter, const QStyleOptionGraphicsItem * const opc_Option,
                      QWidget * const opc_Widget);
   //lint -restore
   virtual void Redraw(void) override;
   void SetText(const QString & orc_Text);
   void SetFont(const QFont & orc_Font);
   void SetDrawBorder(const bool oq_Active);
   void SetDrawWhiteFilter(const bool oq_Active);

private:
   //Avoid call
   C_GiNodeBoundary(const C_GiNodeBoundary &);
   C_GiNodeBoundary & operator =(const C_GiNodeBoundary &);

   QString mc_Text;
   QFont mc_Font;
   QGraphicsDropShadowEffect * mpc_Shadow;

   void m_DrawBackground(QPainter * const opc_Painter) const;
   bool mq_DrawBoder;
   bool mq_DrawWhiteFilter;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
