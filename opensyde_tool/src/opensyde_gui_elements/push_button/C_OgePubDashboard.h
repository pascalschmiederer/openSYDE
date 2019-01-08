//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Push button part of push button item (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     28.08.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_OGECHXDASHBOARD_H
#define C_OGECHXDASHBOARD_H

/* -- Includes ------------------------------------------------------------- */
#include <QPushButton>
#include <QSvgRenderer>

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_OgePubDashboard :
   public QPushButton
{
   Q_OBJECT

public:
   C_OgePubDashboard(QWidget * const opc_Parent = NULL);
   ~C_OgePubDashboard(void);

   void SetFiles(const QString & orc_SvgActivated, const QString & orc_SvgDeactivated);

protected:
   virtual void paintEvent(QPaintEvent * const opc_Event) override;

private:
   QSvgRenderer * mpc_RendererActivated;
   QSvgRenderer * mpc_RendererDeactivated;

   static QRectF mh_AdaptToAspcetRatio(const QRect & orc_Target, const QRect & orc_Rect);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
