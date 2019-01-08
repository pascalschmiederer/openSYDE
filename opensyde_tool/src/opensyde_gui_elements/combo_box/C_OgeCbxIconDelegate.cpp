//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Combo box item delagate for custom drawing (implementation)

   Combo box item delagate for custom drawing

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     15.12.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <QPainter>
#include "constants.h"
#include "C_OgeCbxIconDelegate.h"
#include "stwtypes.h"
#include "C_Uti.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_opensyde_gui_elements;
using namespace stw_opensyde_gui_logic;
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

   \created     15.12.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_OgeCbxIconDelegate::C_OgeCbxIconDelegate(void) :
   QStyledItemDelegate(),
   ms16_PaddingLeft(5)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set padding

   \param[in] os16_Value New value

   \created     29.10.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgeCbxIconDelegate::SetPaddingLeft(const stw_types::sint16 os16_Value)
{
   this->ms16_PaddingLeft = os16_Value;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Custom paint to add a padding left

   - add padding (ms16_PaddingLeft)
   - call original paint
   - paint selection rectangle if required

   \param[in,out] opc_Painter Painter
   \param[in]     orc_Option  Option
   \param[in]     orc_Index   Index

   \created     15.12.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OgeCbxIconDelegate::paint(QPainter * const opc_Painter, const QStyleOptionViewItem & orc_Option,
                                 const QModelIndex & orc_Index) const
{
   QStyleOptionViewItem c_Option = orc_Option;
   QRect c_Rect;

   //Prepare additional rectangle to draw selection for complete item
   c_Rect.setTopLeft(orc_Option.rect.topLeft());
   c_Rect.setHeight(orc_Option.rect.height());
   c_Rect.setWidth(this->ms16_PaddingLeft);

   //Adapt original item position
   c_Option.rect.setX(orc_Option.rect.x() + ms16_PaddingLeft);

   //Draw original item & icon
   QStyledItemDelegate::paint(opc_Painter, c_Option, orc_Index);

   //Draw selection rectangle
   if (C_Uti::h_CheckStyleState(orc_Option.state, QStyle::State_MouseOver) == true)
   {
      opc_Painter->save();
      opc_Painter->setBrush(stw_opensyde_gui::mc_STYLE_GUIDE_COLOR_25); //selection color
      opc_Painter->setPen(Qt::NoPen);
      opc_Painter->drawRect(c_Rect);
      opc_Painter->restore();
   }
}