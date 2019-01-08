//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Data pool list data change undo command (implementation)

   Data pool list data change undo command

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     10.03.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "stwtypes.h"
#include "stwerrors.h"
#include "C_SdNdeUnoDataPoolListDataChangeCommand.h"
#include "C_PuiSdHandler.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_core;
using namespace stw_errors;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Default constructor

   \param[in]     oru32_NodeIndex                Node index
   \param[in]     oru32_DataPoolIndex            Node data pool index
   \param[in,out] opc_DataPoolListsTreeWidget    Data pool lists tree widget to perform actions on
   \param[in]     oru32_DataPoolListElementIndex Node data pool list index
   \param[in]     orc_NewData                    New data
   \param[in]     ore_DataChangeType             Data change type
   \param[in,out] opc_Parent                     Optional pointer to parent

   \created     10.03.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_SdNdeUnoDataPoolListDataChangeCommand::C_SdNdeUnoDataPoolListDataChangeCommand(const uint32 & oru32_NodeIndex,
                                                                                 const uint32 & oru32_DataPoolIndex,
                                                                                 stw_opensyde_gui::C_SdNdeDataPoolListsTreeWidget * const opc_DataPoolListsTreeWidget, const uint32 & oru32_DataPoolListIndex, const QVariant & orc_NewData, const C_SdNdeDataPoolUtil::E_ListDataChangeType & ore_DataChangeType,
                                                                                 QUndoCommand * const opc_Parent) :
   C_SdNdeUnoDataPoolListBaseCommand(oru32_NodeIndex, oru32_DataPoolIndex, opc_DataPoolListsTreeWidget,
                                     "Change List data", opc_Parent),
   mu32_DataPoolListIndex(oru32_DataPoolListIndex),
   mc_NewData(orc_NewData),
   me_DataChangeType(ore_DataChangeType)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Redo

   \created     10.03.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeUnoDataPoolListDataChangeCommand::redo(void)
{
   m_Change(this->mc_PreviousData, this->mc_NewData);
   //Apply parent taks = auto min max after current change
   C_SdNdeUnoDataPoolListBaseCommand::redo();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Undo

   \created     10.03.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeUnoDataPoolListDataChangeCommand::undo(void)
{
   m_Change(this->mc_NewData, this->mc_PreviousData);
   //Apply parent taks = auto min max after current change
   C_SdNdeUnoDataPoolListBaseCommand::undo();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Change data values and store previous value

   \param[out] orc_PreviousData Previous data value storage
   \param[in]  orc_NewData      New data value assignment

   \created     13.03.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeUnoDataPoolListDataChangeCommand::m_Change(QVariant & orc_PreviousData, const QVariant & orc_NewData)
{
   C_OSCNodeDataPoolList c_OSCList;
   C_PuiSdNodeDataPoolList c_UIList;

   if (C_PuiSdHandler::h_GetInstance()->GetDataPoolList(
          this->mu32_NodeIndex, this->mu32_DataPoolIndex, this->mu32_DataPoolListIndex, c_OSCList,
          c_UIList) == C_NO_ERR)
   {
      //Save previous
      mh_ConvertListTypeToGeneric(c_OSCList, c_UIList, this->me_DataChangeType, orc_PreviousData);

      //Apply new
      switch (this->me_DataChangeType)
      {
      case C_SdNdeDataPoolUtil::eLIST_NAME:
         c_OSCList.c_Name = orc_NewData.toString().toStdString().c_str();
         break;
      case C_SdNdeDataPoolUtil::eLIST_COMMENT:
         c_OSCList.c_Comment = orc_NewData.toString().toStdString().c_str();
         break;
      case C_SdNdeDataPoolUtil::eLIST_SIZE:
         c_OSCList.u32_NvMSize = static_cast<uint32>(orc_NewData.toULongLong());
         break;
      case C_SdNdeDataPoolUtil::eLIST_CRC:
         c_OSCList.q_NvMCRCActive = orc_NewData.toBool();
         break;
      default:
         //Unknown
         break;
      }
      static_cast<void>(C_PuiSdHandler::h_GetInstance()->SetDataPoolList(
                           this->mu32_NodeIndex, this->mu32_DataPoolIndex, this->mu32_DataPoolListIndex, c_OSCList,
                           c_UIList));
      if (this->mpc_DataPoolListsTreeWidget != NULL)
      {
         this->mpc_DataPoolListsTreeWidget->RegisterSizeChange();
         this->mpc_DataPoolListsTreeWidget->UpdateUI();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Convert list type to generic

   \param[in]  orc_OSCElement OSC Element
   \param[in]  orc_UIElement  UI Element
   \param[in]  ore_Type       Type to convert
   \param[out] orc_Generic    Generic output

   \created     13.03.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeUnoDataPoolListDataChangeCommand::mh_ConvertListTypeToGeneric(const C_OSCNodeDataPoolList & orc_OSCElement,
                                                                          const C_PuiSdNodeDataPoolList & orc_UIElement,
                                                                          const C_SdNdeDataPoolUtil::E_ListDataChangeType & ore_Type,
                                                                          QVariant & orc_Generic)
{
   //TODO use for data set changes
   Q_UNUSED(orc_UIElement)
   switch (ore_Type)
   {
   case C_SdNdeDataPoolUtil::eLIST_NAME:
      orc_Generic = orc_OSCElement.c_Name.c_str();
      break;
   case C_SdNdeDataPoolUtil::eLIST_COMMENT:
      orc_Generic = orc_OSCElement.c_Comment.c_str();
      break;
   case C_SdNdeDataPoolUtil::eLIST_SIZE:
      orc_Generic = static_cast<uint64>(orc_OSCElement.u32_NvMSize);
      break;
   case C_SdNdeDataPoolUtil::eLIST_CRC:
      orc_Generic = orc_OSCElement.q_NvMCRCActive;
      break;
   default:
      break;
   }
}
