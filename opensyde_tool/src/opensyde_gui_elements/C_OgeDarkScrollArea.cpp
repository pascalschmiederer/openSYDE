//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Custom scroll area with dark mode support (implementation)

   Custom scroll area with dark mode support

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     21.11.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <QScrollBar>
#include "C_OgeDarkScrollArea.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
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

   \created     21.11.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_OgeDarkScrollArea::C_OgeDarkScrollArea(QWidget * const opc_Parent) :
   QScrollArea(opc_Parent)
{
   // configure the scrollbar to stop resizing the widget when showing or hiding the scrollbar
   this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   this->horizontalScrollBar()->hide();
   // configure the scrollbar to stop resizing the widget when showing or hiding the scrollbar
   this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   this->verticalScrollBar()->hide();
   //Connects
   connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, this,
           &C_OgeDarkScrollArea::m_ScrollBarRangeChangedVer);
   connect(this->horizontalScrollBar(), &QScrollBar::rangeChanged, this,
           &C_OgeDarkScrollArea::m_ScrollBarRangeChangedHor);

   // Deactivate custom context menu of scroll bar
   this->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
   this->horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
}
//-----------------------------------------------------------------------------
void C_OgeDarkScrollArea::m_ScrollBarRangeChangedVer(const sintn osn_Min, const sintn osn_Max) const
{
   // manual showing and hiding of the scrollbar to stop resizing the parent widget when showing or hiding the scrollbar
   if ((osn_Min == 0) && (osn_Max == 0))
   {
      this->verticalScrollBar()->hide();
   }
   else
   {
      this->verticalScrollBar()->show();
   }
}
//-----------------------------------------------------------------------------
void C_OgeDarkScrollArea::m_ScrollBarRangeChangedHor(const sintn osn_Min, const sintn osn_Max) const
{
   // manual showing and hiding of the scrollbar to stop resizing the parent widget when showing or hiding the scrollbar
   if ((osn_Min == 0) && (osn_Max == 0))
   {
      this->horizontalScrollBar()->hide();
   }
   else
   {
      this->horizontalScrollBar()->show();
   }
}
