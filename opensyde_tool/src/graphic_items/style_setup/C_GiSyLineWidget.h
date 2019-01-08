//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for bus style setup to customize bus optics (header)

   \implementation
   project     opensyde
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     12.08.2016  STW/S.Singer
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_GISYLINEWIDGET_H
#define C_GISYLINEWIDGET_H

/* -- Includes ------------------------------------------------------------- */
#include <QWidget>
#include <QColor>
#include <QLabel>

#include "stwtypes.h"

#include "C_GiSyBaseWidget.h"
#include "C_PuiBsLineArrow.h"
#include "C_OgeCbxIconOnly.h"

/* -- Namespace ------------------------------------------------------------ */
namespace Ui
{
class C_GiSyLineWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */
class C_GiSyLineWidget :
   public QWidget
{
   Q_OBJECT

public:
   enum E_Type
   {
      eCAN_BUS = 0,
      eETHERNET_BUS = 1,
      eLINE = 2
   };

   C_GiSyLineWidget(const E_Type oe_Mode, C_GiSyBaseWidget & orc_Parent);
   ~C_GiSyLineWidget(void);

   void InitStaticNames(void) const;

   stw_types::sintn GetLineWidth(void) const;
   void SetLineWidth(const stw_types::sintn osn_Value);

   QColor GetLineColor(void) const;
   void SetLineColor(const QColor & orc_Value);

   QColor GetInnerLineColor(void) const;
   void SetInnerLineColor(const QColor & orc_Value);

   stw_opensyde_gui_logic::C_PuiBsLineArrow::E_LineType GetLineStyle(void) const;
   void SetLineStyle(const stw_opensyde_gui_logic::C_PuiBsLineArrow::E_LineType oe_Value);

   stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType GetStartArrow(void) const;
   void SetStartArrow(const stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType oe_Value);

   stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType GetEndArrow(void) const;
   void SetEndArrow(const stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType oe_Value);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void showEvent(QShowEvent * const opc_Event) override;

private:
   //Avoid call
   C_GiSyLineWidget(const C_GiSyLineWidget &);
   C_GiSyLineWidget & operator =(const C_GiSyLineWidget &);

   void m_UpdatePreview(void);

   void m_ColorClicked(void);
   void m_ColorInnerClicked(void);
   void m_ValueChanged(void);
   static void mh_InitArrow(stw_opensyde_gui_elements::C_OgeCbxIconOnly * const opc_ComboBox,
                            const bool & orq_Rotate = false);

   Ui::C_GiSyLineWidget * mpc_Ui;
   C_GiSyBaseWidget * mpc_ParentDialog;
   const E_Type me_Mode;
   QColor mc_Color;
   QColor mc_InnerLineColor;
   stw_types::sint32 ms32_LineStyle;

   QLabel * mpc_LabelColor;
   QLabel * mpc_LabelColorInner;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
