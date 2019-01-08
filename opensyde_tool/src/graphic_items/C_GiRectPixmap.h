//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Graphics item for showing an optional icon and a background (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     26.06.2017  STW/B.Bayer
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_GIRECTPIXMAP_H
#define C_GIRECTPIXMAP_H

/* -- Includes ------------------------------------------------------------- */
#include <QGraphicsRectItem>
#include <QSvgRenderer>

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_GiRectPixmap :
   public QGraphicsRectItem
{
public:
   C_GiRectPixmap(const QRectF & orc_Rect, QGraphicsItem * const opc_Parent = NULL);
   virtual ~C_GiRectPixmap();

   void SetBackgroundColor(const QColor & orc_Color);
   void SetPixmap(const QPixmap & orc_Pixmap);
   void SetDrawPixmap(const bool oq_Active);
   void SetNewSize(const QSizeF & orc_Size);
   void SetNewRect(const QRectF & orc_Rect);
   void SetSvg(const QString & orc_Value);

   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void paint(QPainter * const opc_Painter, const QStyleOptionGraphicsItem * const opc_Option,
                      QWidget * const opc_Widget) override;
   //lint -restore

protected:
   QPixmap mc_Pixmap;
   QPixmap mc_PixmapScaled;
   bool mq_DrawPixmap;
   bool mq_DrawSvg;

private:
   QSvgRenderer * mpc_SvgRenderer;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
