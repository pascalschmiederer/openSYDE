//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Button for options (implementation)

   Button for options.

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     06.12.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <QPainter>

#include "C_OgePubOptions.h"

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

   \created     06.12.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_OgePubOptions::C_OgePubOptions(QWidget * const opc_Parent) :
   C_OgePubToolTipBase(opc_Parent)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set the icon for enabled and disabled mode

   \param[in]     oc_IconPathEnabled      String for Enabled Icon Path
                  oc_IconPathDisabled     String for Disabled Icon Path

   \created     22.08.2018  STW/G.Scupin
*/
//-----------------------------------------------------------------------------
void C_OgePubOptions::SetCustomIcon(QString oc_IconPathEnabled, QString oc_IconPathDisabled)
{
   mc_IconEnabled.addPixmap(QPixmap(oc_IconPathEnabled));
   mc_IconDisabled.addPixmap(QPixmap(oc_IconPathDisabled));
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overridden paint event

   Here: Draw icon

   \param[in,out] opc_Event  Event identification and information

   \created     22.08.2018  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_OgePubOptions::paintEvent(QPaintEvent * const opc_Event)
{
   C_OgePubToolTipBase::paintEvent(opc_Event);

   // draw the icon
   if (this->isEnabled() == true)
   {
      QPainter c_Painter(this);
      QPixmap c_PixmapEnabled;
      c_PixmapEnabled = this->mc_IconEnabled.pixmap(this->iconSize());
      //Calculate icon top left based on fixed center
      c_Painter.drawPixmap(26 - (this->iconSize().width() / 2), 15 - (this->iconSize().height() / 2), c_PixmapEnabled);
   }
   else
   {
      QPainter c_Painter(this);
      QPixmap c_PixmapDisabled;
      c_PixmapDisabled = this->mc_IconDisabled.pixmap(this->iconSize());
      //Calculate icon top left based on fixed center
      c_Painter.drawPixmap(26 - (this->iconSize().width() / 2), 15 - (this->iconSize().height() / 2), c_PixmapDisabled);
   }
}