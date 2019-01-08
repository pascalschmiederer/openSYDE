//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Combo box item delagate for custom drawing (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     15.12.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_OGECBXICONDELEGATE_H
#define C_OGECBXICONDELEGATE_H

/* -- Includes ------------------------------------------------------------- */
#include <QStyledItemDelegate>
#include "stwtypes.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_OgeCbxIconDelegate :
   public QStyledItemDelegate
{
public:
   C_OgeCbxIconDelegate(void);

   void SetPaddingLeft(const stw_types::sint16 os16_Value);

   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void paint(QPainter * const opc_Painter, const QStyleOptionViewItem & orc_Option,
                      const QModelIndex & orc_Index) const;
   //lint -restore


private:
   stw_types::sint16 ms16_PaddingLeft;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
