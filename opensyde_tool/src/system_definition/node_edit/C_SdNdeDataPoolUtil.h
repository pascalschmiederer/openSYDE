//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Node data pool util class (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     27.02.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SDNDEDATAPOOLUTIL_H
#define C_SDNDEDATAPOOLUTIL_H

/* -- Includes ------------------------------------------------------------- */

#include <vector>
#include <QItemSelection>
#include <QRect>
#include <QPainter>
#include <QWidget>
#include <QAbstractItemModel>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractTableModel>
#include "stwtypes.h"
#include "C_PuiSdNodeDataPoolListElement.h"
#include "C_OSCNodeDataPool.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_SdNdeDataPoolUtil
{
public:
   enum E_ListDataChangeType
   {
      eLIST_NAME,
      eLIST_COMMENT,
      eLIST_SIZE,
      eLIST_CRC
   };

   enum E_DataSetDataChangeType
   {
      eDATA_SET_NAME,
      eDATA_SET_COMMENT
   };

   enum E_ElementDataChangeType
   {
      eELEMENT_NAME,
      eELEMENT_COMMENT,
      eELEMENT_VALUE_TYPE,
      eELEMENT_ARRAY,
      eELEMENT_AUTO_MIN_MAX,
      eELEMENT_MIN,
      eELEMENT_MAX,
      eELEMENT_FACTOR,
      eELEMENT_OFFSET,
      eELEMENT_UNIT,
      eELEMENT_DATA_SET,
      eELEMENT_ACCESS,
      eELEMENT_EVENT_CALL
   };

   enum E_ArrayEditType
   {
      eARRAY_EDIT_MIN,
      eARRAY_EDIT_MAX,
      eARRAY_EDIT_DATA_SET
   };

   static bool h_CheckConnected(const std::vector<stw_types::uint32> & orc_Items);
   static std::vector<stw_types::uint32> h_ConvertVector(const QModelIndexList & orc_Items,
                                                         const bool & orq_Rows = true);
   static void h_ConvertToGeneric(const stw_opensyde_core::C_OSCNodeDataPoolListElement & orc_OSCElement,
                                  const C_PuiSdNodeDataPoolListElement & orc_UIElement,
                                  const E_ElementDataChangeType & ore_Type, QVariant & orc_Generic,
                                  const stw_types::uint32 & oru32_Index, const stw_types::sint32 & ors32_DataSetIndex);
   static QVariant h_DataVariable(const stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Input,
                                  const stw_types::uint32 & oru32_Index);
   static QVariant h_ScaledDataVariable(const stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Input,
                                        const stw_types::float64 of64_Factor, const stw_types::float64 of64_Offset,
                                        const stw_types::uint32 & oru32_Index);
   static stw_types::sint32 h_SetDataVariable(const QVariant & orc_Value,
                                              stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Output,
                                              const stw_types::uint32 & oru32_Index);
   static stw_types::sint32 h_SetUnscaledDataVariable(const QVariant & orc_Value,
                                                      stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Output,
                                                      const stw_types::float64 of64_Factor,
                                                      const stw_types::float64 of64_Offset,
                                                      const stw_types::uint32 & oru32_Index);
   static stw_types::sint8 h_ConvertContentTypeToComboBox(
      const stw_opensyde_core::C_OSCNodeDataPoolContent::E_Type & ore_Type);
   static stw_opensyde_core::C_OSCNodeDataPoolContent::E_Type h_ConvertContentTypeFromComboBox(
      const stw_types::sint8 & ors8_Type);
   static stw_types::sint8 h_ConvertElementAccessToComboBox(
      const stw_opensyde_core::C_OSCNodeDataPoolListElement::E_Access & ore_Type);
   static stw_opensyde_core::C_OSCNodeDataPoolListElement::E_Access h_ConvertElementAccesFromComboBox(
      const stw_types::sint8 & ors8_Type);
   static QString h_ConvertContentTypeToString(const stw_opensyde_core::C_OSCNodeDataPoolContent::E_Type & ore_Type);
   static QString h_ConvertElementAccessToString(
      const stw_opensyde_core::C_OSCNodeDataPoolListElement::E_Access & ore_Type);
   static void h_DrawList(QPainter * const opc_Painter, const QRect & orc_Rectangle,
                          const stw_types::uint32 & oru32_NodeIndex, const stw_types::uint32 & oru32_DataPoolIndex,
                          const stw_types::uint32 & oru32_ListIndex);
   static QWidget * h_CreateGenericEditor(QWidget * const opc_Parent, const QModelIndex & orc_Index,
                                          const stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Min,
                                          const stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Max,
                                          const stw_types::float64 of64_Factor, const stw_types::float64 of64_Offset,
                                          const stw_types::uint32 & oru32_ArrayIndex, const bool oq_UseParamVariant);
   static void h_SetGenericEditorDataVariable(QWidget * const opc_Editor, const QModelIndex & orc_Index);
   static void h_SetModelGenericDataVariable(QWidget * const opc_Editor, QAbstractItemModel * const opc_Model,
                                             const QModelIndex & orc_Index);
   static QString h_ConvertToString(const stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Data);
   static void h_SetString(const QString & orc_String, stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Output);
   static QString h_ConvertToString(const std::vector<stw_types::sint8> & orc_Data);
   static QString h_ConvertTypeToNameSimplified(const stw_opensyde_core::C_OSCNodeDataPoolContent::E_Type & ore_Type);
   static void h_DrawTableBoolean(QPainter * const opc_Painter, const QStyleOptionViewItem & orc_Option,
                                  const QModelIndex & orc_Index, const QPixmap & orc_CheckMark,
                                  const bool & orq_IsEditing);
   static void h_DrawTableBackground(QPainter * const opc_Painter, const QStyleOptionViewItem & orc_Option,
                                     const bool & orq_UseSpecialBackgroundColor);
   static void h_DrawTableBackgroundGeneric(QPainter * const opc_Painter, const QStyleOptionViewItem & orc_Option,
                                            const QColor & orc_Default, const QColor & orc_Alternate,
                                            const QColor & orc_Selected);
   static void h_DrawTableSelected(QPainter * const opc_Painter, const QStyleOptionViewItem & orc_Option,
                                   const QModelIndex & orc_Index, const QAbstractTableModel * const opc_TableModel);
   static bool h_CompareSpecifiedItemSmaller(const stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Content1,
                                             const stw_opensyde_core::C_OSCNodeDataPoolContent & orc_Content2,
                                             const stw_types::uint32 & oru32_Index);
   static stw_types::sint32 h_GetTableSize(const stw_types::uint32 ou32_NodeIndex,
                                           const stw_types::uint32 ou32_DataPoolIndex,
                                           const stw_types::uint32 ou32_ListIndex);

private:
   C_SdNdeDataPoolUtil();

   static void mh_ApplyStyleSheet(QWidget * const opc_Widget, const QFont & orc_Font, const QColor & orc_Color);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
