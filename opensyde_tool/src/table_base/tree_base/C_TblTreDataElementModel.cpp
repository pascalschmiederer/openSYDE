//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Tree model for data elements (implementation)

   Tree model for data elements

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QElapsedTimer>

#include "TGLUtils.h"
#include "constants.h"
#include "stwerrors.h"
#include "C_GtGetText.h"
#include "CSCLChecksums.h"
#include "C_PuiSvHandler.h"
#include "C_PuiSdHandler.h"
#include "C_PuiSdUtil.h"
#include "C_SyvRoRouteCalculation.h"
#include "C_TblTreDataElementModel.h"
#include "C_SdUtil.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_tgl;
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_core;
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */
const QString C_TblTreDataElementModel::mhc_IconNode = ":/images/system_definition/IconNode.svg";
const QString C_TblTreDataElementModel::mhc_IconDatapool = ":/images/system_definition/IconDataPoolSmall.svg";
const QString C_TblTreDataElementModel::mhc_IconList = ":/images/system_definition/IconDatapoolList.svg";
const QString C_TblTreDataElementModel::mhc_IconVariable = ":/images/system_definition/IconVariable.svg";
const QString C_TblTreDataElementModel::mhc_IconParameter = ":/images/system_definition/IconParameter.svg";
const QString C_TblTreDataElementModel::mhc_IconSignal = ":/images/system_definition/IconSignal.svg";
const QString C_TblTreDataElementModel::mhc_IconEthernet = ":/images/system_definition/IconBus.svg";
const QString C_TblTreDataElementModel::mhc_IconCAN = ":/images/system_definition/IconBus.svg";
const QString C_TblTreDataElementModel::mhc_IconMessage = ":/images/system_definition/IconMessage.svg";
const QString C_TblTreDataElementModel::mhc_AdditionalDataPoolInfo = " (Already assigned)";
const QString C_TblTreDataElementModel::mhc_AdditionalWriteOnlyInfo = " (Not supported, read only)";
const QString C_TblTreDataElementModel::mhc_AdditionalArrayInfo = " (Not supported, array or string type)";
const QString C_TblTreDataElementModel::mhc_AdditionalArrayStringInfo = " (Not supported, string type)";
const QString C_TblTreDataElementModel::mhc_AdditionalArrayIndexInfo = " (Not supported, array index)";
const QString C_TblTreDataElementModel::mhc_Additional64BitInfo = " (Not supported, 64 bit value)";

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */
QMap<std::vector<stw_types::uint32>,
     C_TblTreDataElementModel::C_TblTreDataElementModelState> C_TblTreDataElementModel::mhc_ViewSetupsNL;
QMap<std::vector<stw_types::uint32>,
     C_TblTreDataElementModel::C_TblTreDataElementModelState> C_TblTreDataElementModel::mhc_ViewSetupsDE;
QMap<std::vector<stw_types::uint32>,
     C_TblTreDataElementModel::C_TblTreDataElementModelState> C_TblTreDataElementModel::mhc_ViewSetupsBS;

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_Parent Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_TblTreDataElementModel::C_TblTreDataElementModel(QObject * const opc_Parent) :
   C_TblTreModel(opc_Parent),
   me_Mode(eDATAPOOL_ELEMENT)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Default destructor

   Clean up.
*/
//----------------------------------------------------------------------------------------------------------------------
C_TblTreDataElementModel::~C_TblTreDataElementModel(void)
{
   m_CleanUpLastModel();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Set the active node index

   \param[in] ou32_NodeIndex               Active node index
   \param[in] os32_SkipApplicationIndex    Application index to not display as used
   \param[in] orc_UsedDataPoolIndicesIndex Data pools to always display as used
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::InitSD(const uint32 ou32_NodeIndex, const sint32 os32_SkipApplicationIndex,
                                      const std::vector<uint32> & orc_UsedDataPoolIndicesIndex)
{
   C_TblTreItem * const pc_NodeItem = new C_TblTreItem();
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(ou32_NodeIndex);
   bool q_NodeValid = false;

   this->beginResetModel();
   this->me_Mode = C_TblTreDataElementModel::eDATAPOOLS;

   //Clear
   m_CleanUpLastModel();
   this->mpc_InvisibleRootItem = new C_TblTreItem();

   //Static
   pc_NodeItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconNode);

   //Node
   if (pc_Node != NULL)
   {
      //Data pool type nodes
      C_TblTreItem * const pc_DiagItem = new C_TblTreItem();
      C_TblTreItem * const pc_NvmItem = new C_TblTreItem();
      C_TblTreItem * const pc_ComItem = new C_TblTreItem();
      bool q_DataPoolDiagValid = false;
      bool q_DataPoolNvmValid = false;
      bool q_DataPoolComValid = false;
      pc_DiagItem->c_Name = C_GtGetText::h_GetText("DIAG Datapools");
      pc_NvmItem->c_Name = C_GtGetText::h_GetText("NVM Datapools");
      pc_ComItem->c_Name = C_GtGetText::h_GetText("COMM Datapools");
      pc_DiagItem->c_ToolTipHeading = C_GtGetText::h_GetText("DIAG Datapools");
      pc_NvmItem->c_ToolTipHeading = C_GtGetText::h_GetText("NVM Datapools");
      pc_ComItem->c_ToolTipHeading = C_GtGetText::h_GetText("COMM Datapools");
      pc_DiagItem->q_Selectable = false;
      pc_NvmItem->q_Selectable = false;
      pc_ComItem->q_Selectable = false;

      //Use some unique index for expansion restoration mechanism
      pc_DiagItem->u32_Index = 0;
      pc_NvmItem->u32_Index = 1;
      pc_ComItem->u32_Index = 2;

      //Init current node
      pc_NodeItem->u32_Index = ou32_NodeIndex;
      pc_NodeItem->q_Selectable = false;
      pc_NodeItem->c_Name = pc_Node->c_Properties.c_Name.c_str();
      pc_NodeItem->c_ToolTipHeading = pc_Node->c_Properties.c_Name.c_str();
      pc_NodeItem->c_ToolTipContent = pc_Node->c_Properties.c_Comment.c_str();

      //Data pools
      pc_NodeItem->ReserveChildrenSpace(3UL);
      pc_DiagItem->ReserveChildrenSpace(pc_Node->c_DataPools.size());
      pc_NvmItem->ReserveChildrenSpace(pc_Node->c_DataPools.size());
      pc_ComItem->ReserveChildrenSpace(pc_Node->c_DataPools.size());
      for (uint32 u32_ItDataPool = 0; u32_ItDataPool < pc_Node->c_DataPools.size(); ++u32_ItDataPool)
      {
         const C_OSCNodeDataPool & rc_DataPool = pc_Node->c_DataPools[u32_ItDataPool];
         C_TblTreItem * const pc_DataPoolItem = new C_TblTreItem();

         //Init current Datapool
         pc_DataPoolItem->u32_Index = u32_ItDataPool;
         pc_DataPoolItem->c_Name = rc_DataPool.c_Name.c_str();
         pc_DataPoolItem->c_ToolTipHeading = rc_DataPool.c_Name.c_str();
         pc_DataPoolItem->c_ToolTipContent = rc_DataPool.c_Comment.c_str();
         pc_DataPoolItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconDatapool);

         // Append protocol type if COMM datapool
         if (rc_DataPool.e_Type == C_OSCNodeDataPool::eCOM)
         {
            pc_DataPoolItem->c_Name += C_GtGetText::h_GetText(" (Protocol: ");
            pc_DataPoolItem->c_Name +=
               C_PuiSdUtil::h_ConvertProtocolTypeToString(C_PuiSdUtil::h_GetRelatedCANProtocolType(ou32_NodeIndex,
                                                                                                   u32_ItDataPool));
            pc_DataPoolItem->c_Name +=  ")";
            pc_DataPoolItem->c_ToolTipHeading = pc_DataPoolItem->c_Name;
         }

         // Set disabled/enabled
         if ((rc_DataPool.s32_RelatedDataBlockIndex < 0) ||
             ((os32_SkipApplicationIndex >= 0) && (rc_DataPool.s32_RelatedDataBlockIndex == os32_SkipApplicationIndex)))
         {
            bool q_Found = false;
            for (uint32 u32_It = 0UL; u32_It < orc_UsedDataPoolIndicesIndex.size(); ++u32_It)
            {
               if (u32_ItDataPool == orc_UsedDataPoolIndicesIndex[u32_It])
               {
                  q_Found = true;
               }
            }
            if (q_Found == false)
            {
               pc_DataPoolItem->q_Selectable = true;
            }
            else
            {
               pc_DataPoolItem->q_Enabled = false;
               pc_DataPoolItem->q_Selectable = false;
               pc_DataPoolItem->c_Name += C_TblTreDataElementModel::mhc_AdditionalDataPoolInfo;
            }
         }
         else
         {
            pc_DataPoolItem->q_Enabled = false;
            pc_DataPoolItem->q_Selectable = false;
            pc_DataPoolItem->c_Name += C_TblTreDataElementModel::mhc_AdditionalDataPoolInfo;
         }

         //Flags
         q_NodeValid = true;
         switch (rc_DataPool.e_Type)
         {
         case C_OSCNodeDataPool::eDIAG:
            q_DataPoolDiagValid = true;
            pc_DiagItem->AddChild(pc_DataPoolItem);
            break;
         case C_OSCNodeDataPool::eNVM:
            q_DataPoolNvmValid = true;
            pc_NvmItem->AddChild(pc_DataPoolItem);
            break;
         case C_OSCNodeDataPool::eCOM:
            q_DataPoolComValid = true;
            pc_ComItem->AddChild(pc_DataPoolItem);
            break;
         default:
            delete (pc_DataPoolItem);
            break;
         }
      }
      if (q_DataPoolDiagValid == true)
      {
         pc_NodeItem->AddChild(pc_DiagItem);
      }
      else
      {
         delete (pc_DiagItem);
      }
      if (q_DataPoolNvmValid == true)
      {
         pc_NodeItem->AddChild(pc_NvmItem);
      }
      else
      {
         delete (pc_NvmItem);
      }
      if (q_DataPoolComValid == true)
      {
         pc_NodeItem->AddChild(pc_ComItem);
      }
      else
      {
         delete (pc_ComItem);
      }
   }
   if (q_NodeValid == true)
   {
      this->mpc_InvisibleRootItem->AddChild(pc_NodeItem);
   }
   else
   {
      delete (pc_NodeItem);
   }
   this->endResetModel();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Initialize tree structure

   \param[in] ou32_ViewIndex            View index
   \param[in] oe_Mode                   Item mode
   \param[in] oq_ShowOnlyWriteElements  Optional flag to show only writable elements
   \param[in] oq_ShowArrayElements      Optional flag to hide all array elements (if false)
   \param[in] oq_ShowArrayIndexElements Optional flag to hide all array index elements (if false)
   \param[in] oq_Show64BitValues        Optional flag to hide all 64 bit elements (if false)
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::InitSV(const uint32 ou32_ViewIndex, const E_Mode oe_Mode,
                                      const bool oq_ShowOnlyWriteElements, const bool oq_ShowArrayElements,
                                      const bool oq_ShowArrayIndexElements, const bool oq_Show64BitValues)
{
   QElapsedTimer c_Timer;

   if (mq_TIMING_OUTPUT)
   {
      c_Timer.start();
   }
   QMap<std::vector<stw_types::uint32>, C_TblTreDataElementModelState>::const_iterator c_It;
   const std::vector<uint32> c_Hashes = C_TblTreDataElementModel::mh_GetViewSdHash(ou32_ViewIndex);

   this->me_Mode = oe_Mode;
   this->beginResetModel();
   //Clear
   m_CleanUpLastModel();
   //Init
   switch (this->me_Mode)
   {
   case eDATAPOOL_ELEMENT:
      c_It = C_TblTreDataElementModel::mhc_ViewSetupsDE.find(c_Hashes);
      if (c_It != C_TblTreDataElementModel::mhc_ViewSetupsDE.end())
      {
         const C_TblTreDataElementModelState & rc_It = c_It.value();
         this->mpc_InvisibleRootItem = rc_It.pc_Tree;
         this->mc_MessageSyncManagers = rc_It.c_SyncManagers;
         this->m_UpdateDatapoolElement(oq_ShowOnlyWriteElements, oq_ShowArrayElements, oq_ShowArrayIndexElements,
                                       oq_Show64BitValues);
         if (mq_TIMING_OUTPUT)
         {
            std::cout << "Restored" << &std::endl;
         }
      }
      else
      {
         this->mpc_InvisibleRootItem = new C_TblTreItem();
         m_InitDatapoolElement(ou32_ViewIndex, oq_ShowOnlyWriteElements, oq_ShowArrayElements,
                               oq_ShowArrayIndexElements, oq_Show64BitValues);
         //Clean up (old values probably not necessary in future);
         mh_CleanUp(C_TblTreDataElementModel::mhc_ViewSetupsDE);
         //Directly store the model (after filling it-> for sync managers)
         C_TblTreDataElementModel::mhc_ViewSetupsDE.insert(c_Hashes,
                                                           C_TblTreDataElementModel::C_TblTreDataElementModelState(
                                                              this->mpc_InvisibleRootItem,
                                                              this->mc_MessageSyncManagers));
         if (mq_TIMING_OUTPUT)
         {
            std::cout << "New" << &std::endl;
         }
      }
      break;
   case eBUS_SIGNAL:
      c_It = C_TblTreDataElementModel::mhc_ViewSetupsBS.find(c_Hashes);
      if (c_It != C_TblTreDataElementModel::mhc_ViewSetupsBS.end())
      {
         const C_TblTreDataElementModelState & rc_It = c_It.value();
         this->mpc_InvisibleRootItem = rc_It.pc_Tree;
         this->mc_MessageSyncManagers = rc_It.c_SyncManagers;
         this->m_UpdateDatapoolElement(oq_ShowOnlyWriteElements, oq_ShowArrayElements, oq_ShowArrayIndexElements,
                                       oq_Show64BitValues);
         if (mq_TIMING_OUTPUT)
         {
            std::cout << "Restored" << &std::endl;
         }
      }
      else
      {
         this->mpc_InvisibleRootItem = new C_TblTreItem();
         m_InitBusSignal(ou32_ViewIndex, oq_ShowOnlyWriteElements, oq_ShowArrayElements,
                         oq_Show64BitValues);
         //Clean up (old values probably not necessary in future);
         mh_CleanUp(C_TblTreDataElementModel::mhc_ViewSetupsBS);
         //Directly store the model (after filling it-> for sync managers)
         C_TblTreDataElementModel::mhc_ViewSetupsBS.insert(c_Hashes,
                                                           C_TblTreDataElementModel::C_TblTreDataElementModelState(
                                                              this->mpc_InvisibleRootItem,
                                                              this->mc_MessageSyncManagers));
         if (mq_TIMING_OUTPUT)
         {
            std::cout << "New" << &std::endl;
         }
      }
      break;
   case eNVM_LIST:
      c_It = C_TblTreDataElementModel::mhc_ViewSetupsNL.find(c_Hashes);
      if (c_It != C_TblTreDataElementModel::mhc_ViewSetupsNL.end())
      {
         const C_TblTreDataElementModelState & rc_It = c_It.value();
         this->mpc_InvisibleRootItem = rc_It.pc_Tree;
         this->mc_MessageSyncManagers = rc_It.c_SyncManagers;
         this->m_UpdateDatapoolElement(oq_ShowOnlyWriteElements, oq_ShowArrayElements, oq_ShowArrayIndexElements,
                                       oq_Show64BitValues);
         if (mq_TIMING_OUTPUT)
         {
            std::cout << "Restored" << &std::endl;
         }
      }
      else
      {
         this->mpc_InvisibleRootItem = new C_TblTreItem();
         m_InitNvmList(ou32_ViewIndex);
         //Clean up (old values probably not necessary in future);
         mh_CleanUp(C_TblTreDataElementModel::mhc_ViewSetupsNL);
         //Directly store the model (after filling it-> for sync managers)
         C_TblTreDataElementModel::mhc_ViewSetupsNL.insert(c_Hashes,
                                                           C_TblTreDataElementModel::C_TblTreDataElementModelState(
                                                              this->mpc_InvisibleRootItem,
                                                              this->mc_MessageSyncManagers));
         if (mq_TIMING_OUTPUT)
         {
            std::cout << "New" << &std::endl;
         }
      }
      break;
   default:
      //NO SV use case
      tgl_assert(false);
   }
   this->endResetModel();
   if (mq_TIMING_OUTPUT)
   {
      std::cout << "Setup data element tree:" << c_Timer.elapsed() << " ms" << &std::endl;
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get data elements

   \param[in] orc_Index Index

   \return
   Data elements
*/
//----------------------------------------------------------------------------------------------------------------------
std::vector<C_PuiSvDbNodeDataPoolListElementId> C_TblTreDataElementModel::GetDataElements(const QModelIndex & orc_Index)
const
{
   std::vector<C_PuiSvDbNodeDataPoolListElementId> c_Retval;
   if (orc_Index.isValid() == true)
   {
      switch (this->me_Mode)
      {
      case eDATAPOOLS:
         c_Retval = m_GetDatapools(orc_Index);
         break;
      case eDATAPOOL_ELEMENT:
         c_Retval = m_GetDatapoolElements(orc_Index);
         break;
      case eBUS_SIGNAL:
         c_Retval = m_GetBusSignals(orc_Index);
         break;
      case eNVM_LIST:
         c_Retval = m_GetNvmList(orc_Index);
         break;
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Preparation for final clean up
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::h_CleanUp(void)
{
   mh_CleanUp(C_TblTreDataElementModel::mhc_ViewSetupsBS);
   C_TblTreDataElementModel::mhc_ViewSetupsBS.clear();
   mh_CleanUp(C_TblTreDataElementModel::mhc_ViewSetupsDE);
   C_TblTreDataElementModel::mhc_ViewSetupsDE.clear();
   mh_CleanUp(C_TblTreDataElementModel::mhc_ViewSetupsNL);
   C_TblTreDataElementModel::mhc_ViewSetupsNL.clear();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get tree column count

   \param[in] orc_Parent Parent

   \return
   Column count
*/
//----------------------------------------------------------------------------------------------------------------------
sintn C_TblTreDataElementModel::columnCount(const QModelIndex & orc_Parent) const
{
   Q_UNUSED(orc_Parent)
   return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Convert generic item representation to model index

   \param[in] orc_ItemIndices Generic item representation

   \return
   Model index
*/
//----------------------------------------------------------------------------------------------------------------------
QModelIndex C_TblTreDataElementModel::GetIndexForItem(const std::vector<uint32> & orc_ItemIndices) const
{
   QModelIndex c_Retval;

   if (orc_ItemIndices.size() > 0UL)
   {
      QModelIndex c_PreviousParent;
      uint32 u32_Counter = 0UL;
      bool q_FoundSomething;

      do
      {
         q_FoundSomething = false;
         for (sintn sn_ItChild = 0; sn_ItChild < this->rowCount(c_PreviousParent); ++sn_ItChild)
         {
            const sintn sn_Column = 0;
            const QModelIndex c_Tmp = this->index(sn_ItChild, sn_Column, c_PreviousParent);
            //lint -e{925}  Result of Qt interface restrictions, set by index function
            const C_TblTreItem * const pc_TreeItem =
               static_cast<const C_TblTreItem * const>(c_Tmp.internalPointer());
            if (pc_TreeItem != NULL)
            {
               if (pc_TreeItem->u32_Index == orc_ItemIndices[u32_Counter])
               {
                  c_PreviousParent = c_Tmp;
                  ++u32_Counter;
                  q_FoundSomething = true;
                  if (u32_Counter == orc_ItemIndices.size())
                  {
                     c_Retval = c_Tmp;
                  }
                  //Stop otherwise the counter is invalid and we already have found the correct element
                  break;
               }
            }
         }
      }
      while ((u32_Counter < orc_ItemIndices.size()) && (q_FoundSomething == true));
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Convert model index to generic item representation

   \param[in] orc_ItemIndex Model index

   \return
   Generic item representation
*/
//----------------------------------------------------------------------------------------------------------------------
std::vector<uint32> C_TblTreDataElementModel::GetGenericRepresentationForIndex(const QModelIndex & orc_ItemIndex)
const
{
   std::vector<uint32> c_Retval;
   QModelIndex c_CurItem = orc_ItemIndex;
   while (c_CurItem.isValid() == true)
   {
      //lint -e{925}  Result of Qt interface restrictions, set by index function
      const C_TblTreItem * const pc_TreeItem = static_cast<const C_TblTreItem * const>(c_CurItem.internalPointer());
      if (pc_TreeItem != NULL)
      {
         c_Retval.insert(c_Retval.begin(), pc_TreeItem->u32_Index);
         c_CurItem = c_CurItem.parent();
      }
      else
      {
         break;
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Default constructor

   \param[in,out] opc_Tree         Tree layout to remember
   \param[in]     orc_SyncManagers Sync managers to store
*/
//----------------------------------------------------------------------------------------------------------------------
C_TblTreDataElementModel::C_TblTreDataElementModelState::C_TblTreDataElementModelState(
   C_TblTreSimpleItem * const opc_Tree, const std::vector<C_PuiSdNodeCanMessageSyncManager *> & orc_SyncManagers) :
   pc_Tree(opc_Tree),
   c_SyncManagers(orc_SyncManagers)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Preparation for final clean up
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::C_TblTreDataElementModelState::CleanUp(void)
{
   delete (this->pc_Tree);
   this->pc_Tree = NULL;
   for (uint32 u32_It = 0UL; u32_It < this->c_SyncManagers.size(); ++u32_It)
   {
      delete (this->c_SyncManagers[u32_It]);
   }
   this->c_SyncManagers.clear();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Clean up all current sync managers
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::m_ClearSyncManagers(void)
{
   for (uint32 u32_It = 0UL; u32_It < this->mc_MessageSyncManagers.size(); ++u32_It)
   {
      delete (this->mc_MessageSyncManagers[u32_It]);
   }
   this->mc_MessageSyncManagers.clear();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Clean up current model

   Warning: pointer might be invalid after call of this function
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::m_CleanUpLastModel(void)
{
   if (this->mpc_InvisibleRootItem != NULL)
   {
      //Check if current model is stored, only discard if not stored
      if (mh_Contains(C_TblTreDataElementModel::mhc_ViewSetupsBS, this->mpc_InvisibleRootItem) == false)
      {
         if (mh_Contains(C_TblTreDataElementModel::mhc_ViewSetupsDE, this->mpc_InvisibleRootItem) == false)
         {
            if (mh_Contains(C_TblTreDataElementModel::mhc_ViewSetupsNL, this->mpc_InvisibleRootItem) == false)
            {
               delete (this->mpc_InvisibleRootItem);
               this->mpc_InvisibleRootItem = NULL;
               m_ClearSyncManagers();
            }
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Translate signal index to data pool index

   WARNING: Only works if message contains at least one signal

   \param[in] orc_Indices      Message identification indices
   \param[in] ou32_SignalIndex Signal index

   \return
   Parsed element ID
*/
//----------------------------------------------------------------------------------------------------------------------
C_PuiSvDbNodeDataPoolListElementId C_TblTreDataElementModel::mh_Translate(
   const C_OSCCanMessageIdentificationIndices & orc_Indices, const uint32 ou32_SignalIndex)
{
   C_PuiSvDbNodeDataPoolListElementId c_Retval(0, 0, 0, 0, C_PuiSvDbNodeDataPoolListElementId::eDATAPOOL_ELEMENT,
                                               false, 0UL, false);
   const C_OSCNodeDataPool * const pc_DataPool = C_PuiSdHandler::h_GetInstance()->GetOSCCanDataPool(
      orc_Indices.u32_NodeIndex, orc_Indices.e_ComProtocol, orc_Indices.u32_DatapoolIndex);

   if (pc_DataPool != NULL)
   {
      uint32 u32_ListIndex;
      if (C_OSCCanProtocol::h_GetComListIndex(*pc_DataPool, orc_Indices.u32_InterfaceIndex, orc_Indices.q_MessageIsTx,
                                              u32_ListIndex) == C_NO_ERR)
      {
         const C_OSCCanMessageContainer * const pc_Container =
            C_PuiSdHandler::h_GetInstance()->GetCanProtocolMessageContainer(orc_Indices.u32_NodeIndex,
                                                                            orc_Indices.e_ComProtocol,
                                                                            orc_Indices.u32_InterfaceIndex,
                                                                            orc_Indices.u32_DatapoolIndex);

         const C_OSCCanProtocol * const pc_Protocol = C_PuiSdHandler::h_GetInstance()->GetCanProtocol(
            orc_Indices.u32_NodeIndex, orc_Indices.e_ComProtocol, orc_Indices.u32_DatapoolIndex);

         if ((pc_Container != NULL) && (pc_Protocol != NULL))
         {
            const uint32 u32_SignalDataStartIndex = pc_Container->GetMessageSignalDataStartIndex(
               orc_Indices.q_MessageIsTx,
               orc_Indices.u32_MessageIndex);
            c_Retval = C_PuiSvDbNodeDataPoolListElementId(orc_Indices.u32_NodeIndex, pc_Protocol->u32_DataPoolIndex,
                                                          u32_ListIndex, u32_SignalDataStartIndex + ou32_SignalIndex,
                                                          C_PuiSvDbNodeDataPoolListElementId::eBUS_SIGNAL, false, 0UL);
         }
      }
   }

   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Initialize tree structure for bus signals

   \param[in] ou32_ViewIndex           View index
   \param[in] oq_ShowOnlyWriteElements Optional flag to show only writable elements
   \param[in] oq_ShowArrayElements     Optional flag to hide all array elements (if false)
   \param[in] oq_Show64BitValues       Optional flag to hide all 64 bit elements (if false)
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::m_InitBusSignal(const uint32 ou32_ViewIndex, const bool oq_ShowOnlyWriteElements,
                                               const bool oq_ShowArrayElements, const bool oq_Show64BitValues)
{
   const C_PuiSvData * const pc_View = C_PuiSvHandler::h_GetInstance()->GetView(ou32_ViewIndex);

   if ((pc_View != NULL) && (pc_View->GetPcData().GetConnected() == true))
   {
      //Expected to be already clean
      this->mc_MessageSyncManagers.clear();
      this->mc_MessageSyncManagers.reserve(3U);
      //Init sync managers
      //Busses
      if (C_PuiSvHandler::h_GetInstance()->CheckBusDisabled(ou32_ViewIndex,
                                                            pc_View->GetPcData().GetBusIndex()) == false)
      {
         C_TblTreItem * const pc_BusItem = new C_TblTreItem();
         const C_OSCSystemBus * const pc_Bus = C_PuiSdHandler::h_GetInstance()->GetOSCBus(
            pc_View->GetPcData().GetBusIndex());
         bool q_BusValid = false;
         //Node
         if (pc_Bus != NULL)
         {
            //Init current node
            pc_BusItem->u32_Index = pc_View->GetPcData().GetBusIndex();
            pc_BusItem->c_Name = pc_Bus->c_Name.c_str();
            pc_BusItem->q_Selectable = false;
            pc_BusItem->c_ToolTipHeading = pc_Bus->c_Name.c_str();
            pc_BusItem->c_ToolTipContent = pc_Bus->c_Comment.c_str();
            switch (pc_Bus->e_Type)
            {
            case C_OSCSystemBus::eCAN:
               pc_BusItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconCAN);
               break;
            case C_OSCSystemBus::eETHERNET:
               pc_BusItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconEthernet);
               break;
            }
            //Protocols
            pc_BusItem->ReserveChildrenSpace(3UL);
            for (uint8 u8_ItProtocol = 0U; u8_ItProtocol < 3U; ++u8_ItProtocol)
            {
               bool q_ProtocolValid;
               C_PuiSdNodeCanMessageSyncManager * const pc_SyncManager = new C_PuiSdNodeCanMessageSyncManager();
               C_OSCCanProtocol::E_Type e_Type;
               std::vector<C_OSCCanMessageIdentificationIndices> c_UniqueMessages;
               C_TblTreItem * const pc_ProtocolItem = new C_TblTreItem();
               //Init current node
               pc_ProtocolItem->u32_Index = static_cast<uint32>(u8_ItProtocol);
               pc_ProtocolItem->q_Selectable = false;
               switch (u8_ItProtocol)
               {
               case 0U:
                  e_Type = C_OSCCanProtocol::eLAYER2;
                  pc_ProtocolItem->c_Name = "LAYER2 protocol";
                  pc_ProtocolItem->c_ToolTipHeading = pc_ProtocolItem->c_Name;
                  break;
               case 1U:
                  e_Type = C_OSCCanProtocol::eECES;
                  pc_ProtocolItem->c_Name = "ECES protocol";
                  pc_ProtocolItem->c_ToolTipHeading = pc_ProtocolItem->c_Name;
                  break;
               case 2U:
                  e_Type = C_OSCCanProtocol::eCAN_OPEN_SAFETY;
                  pc_ProtocolItem->c_Name = "ECOS protocol";
                  pc_ProtocolItem->c_ToolTipHeading = pc_ProtocolItem->c_Name;
                  break;
               default:
                  e_Type = C_OSCCanProtocol::eLAYER2;
                  tgl_assert(true);
                  break;
               }
               pc_SyncManager->Init(pc_View->GetPcData().GetBusIndex(), e_Type);
               c_UniqueMessages = pc_SyncManager->GetUniqueMessages();
               this->mc_MessageSyncManagers.push_back(pc_SyncManager);
               //Flag
               q_ProtocolValid = false;

               //Messages
               pc_ProtocolItem->ReserveChildrenSpace(c_UniqueMessages.size());
               for (uint32 u32_ItMessage = 0U; u32_ItMessage < c_UniqueMessages.size(); ++u32_ItMessage)
               {
                  C_TblTreItem * const pc_MessageItem = new C_TblTreItem();
                  const C_OSCCanMessage * const pc_Message = C_PuiSdHandler::h_GetInstance()->GetCanMessage(
                     c_UniqueMessages[u32_ItMessage]);
                  //Flag
                  bool q_MessageValid = false;
                  if (pc_Message != NULL)
                  {
                     //Init current node
                     pc_MessageItem->u32_Index = u32_ItMessage;
                     pc_MessageItem->c_Name =
                        QString("%1 (0x%2)").arg(pc_Message->c_Name.c_str()).arg(QString::number(pc_Message->u32_CanId,
                                                                                                 16));
                     pc_MessageItem->c_ToolTipHeading = pc_MessageItem->c_Name;
                     pc_MessageItem->c_ToolTipContent =
                        C_SdUtil::h_GetToolTipContentMessage(c_UniqueMessages[u32_ItMessage]);
                     pc_MessageItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconMessage);
                     //Signals
                     pc_MessageItem->ReserveChildrenSpace(pc_Message->c_Signals.size());
                     for (uint32 u32_ItSignal = 0; u32_ItSignal < pc_Message->c_Signals.size(); ++u32_ItSignal)
                     {
                        const C_OSCNodeDataPoolListElement * const pc_Element =
                           C_PuiSdHandler::h_GetInstance()->GetOSCCanDataPoolListElement(
                              c_UniqueMessages[u32_ItMessage],
                              u32_ItSignal);
                        if (pc_Element != NULL)
                        {
                           C_TblTreItem * const pc_ElementItem =
                              new C_TblTreItem();
                           //Init current node
                           pc_ElementItem->u32_Index = u32_ItSignal;
                           pc_ElementItem->c_Name = pc_Element->c_Name.c_str();
                           pc_ElementItem->c_ToolTipHeading = pc_Element->c_Name.c_str();
                           // todo
                           pc_ElementItem->c_ToolTipContent =
                              C_SdUtil::h_GetToolTipContentSignal(c_UniqueMessages[u32_ItMessage], u32_ItSignal);
                           pc_ElementItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconSignal);
                           if ((pc_Element->GetArray() == false) || (oq_ShowArrayElements == true))
                           {
                              if ((((pc_Element->GetType() != C_OSCNodeDataPoolContent::eFLOAT64) &&
                                    (pc_Element->GetType() != C_OSCNodeDataPoolContent::eUINT64)) &&
                                   (pc_Element->GetType() != C_OSCNodeDataPoolContent::eSINT64)) ||
                                  (oq_Show64BitValues == true))
                              {
                                 if ((pc_Element->e_Access == C_OSCNodeDataPoolListElement::eACCESS_RW) ||
                                     (oq_ShowOnlyWriteElements == false))
                                 {
                                    //Valid no adaptation
                                 }
                                 else
                                 {
                                    pc_ElementItem->q_Enabled = false;
                                    pc_ElementItem->q_Selectable = false;
                                    //Explanation
                                    pc_ElementItem->c_Name +=
                                       C_TblTreDataElementModel::mhc_AdditionalWriteOnlyInfo;
                                 }
                              }
                              else
                              {
                                 pc_ElementItem->q_Enabled = false;
                                 pc_ElementItem->q_Selectable = false;
                                 //Explanation
                                 pc_ElementItem->c_Name += C_TblTreDataElementModel::mhc_Additional64BitInfo;
                              }
                           }
                           else
                           {
                              pc_ElementItem->q_Enabled = false;
                              pc_ElementItem->q_Selectable = false;
                              //Explanation
                              pc_ElementItem->c_Name += C_TblTreDataElementModel::mhc_AdditionalArrayInfo;
                           }
                           //Signal
                           q_BusValid = true;
                           q_ProtocolValid = true;
                           q_MessageValid = true;
                           //Add
                           pc_MessageItem->AddChild(pc_ElementItem);
                        }
                     }
                  }
                  if (q_MessageValid == true)
                  {
                     pc_ProtocolItem->AddChild(pc_MessageItem);
                  }
                  else
                  {
                     delete (pc_MessageItem);
                  }
               }
               if (q_ProtocolValid == true)
               {
                  pc_BusItem->AddChild(pc_ProtocolItem);
               }
               else
               {
                  delete (pc_ProtocolItem);
               }
               //lint -e{429} Deleted at a later point
            }
         }
         if (q_BusValid == true)
         {
            this->mpc_InvisibleRootItem->AddChild(pc_BusItem);
         }
         else
         {
            delete (pc_BusItem);
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Initialize tree structure for data pool elements

   \param[in] ou32_ViewIndex            View index
   \param[in] oq_ShowOnlyWriteElements  Optional flag to show only writable elements
   \param[in] oq_ShowArrayElements      Optional flag to hide all array elements (if false)
   \param[in] oq_ShowArrayIndexElements Optional flag to hide all array index elements (if false)
   \param[in] oq_Show64BitValues        Optional flag to hide all 64 bit elements (if false)
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::m_InitDatapoolElement(const uint32 ou32_ViewIndex, const bool oq_ShowOnlyWriteElements,
                                                     const bool oq_ShowArrayElements,
                                                     const bool oq_ShowArrayIndexElements,
                                                     const bool oq_Show64BitValues)
{
   const C_PuiSvData * const pc_View = C_PuiSvHandler::h_GetInstance()->GetView(ou32_ViewIndex);

   if (pc_View != NULL)
   {
      const std::vector<uint8> & rc_NodeActiveFlags = pc_View->GetNodeActiveFlags();
      bool q_NodeValid;
      bool q_DataPoolDiagValid;
      bool q_DataPoolNvmValid;
      bool q_DataPoolComValid;
      bool q_DataPoolValid;
      bool q_ListValid;
      const uint32 u32_NodeSize = C_PuiSdHandler::h_GetInstance()->GetOSCNodesSize();

      //Nodes
      this->mpc_InvisibleRootItem->ReserveChildrenSpace(u32_NodeSize);
      tgl_assert(rc_NodeActiveFlags.size() == u32_NodeSize);
      for (uint32 u32_ItNode = 0; u32_ItNode < u32_NodeSize; ++u32_ItNode)
      {
         if ((rc_NodeActiveFlags[u32_ItNode] == true) && (mh_CheckNodeDiagnostic(ou32_ViewIndex, u32_ItNode) == true))
         {
            C_TblTreItem * const pc_NodeItem = new C_TblTreItem();
            const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(u32_ItNode);
            q_NodeValid = false;
            //Static
            pc_NodeItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconNode);
            //Node
            if (pc_Node != NULL)
            {
               //Data pool type nodes
               C_TblTreItem * const pc_DiagItem = new C_TblTreItem();
               C_TblTreItem * const pc_NvmItem = new C_TblTreItem();
               C_TblTreItem * const pc_ComItem = new C_TblTreItem();
               pc_DiagItem->c_Name = C_GtGetText::h_GetText("DIAG Datapools");
               pc_NvmItem->c_Name = C_GtGetText::h_GetText("NVM Datapools");
               pc_ComItem->c_Name = C_GtGetText::h_GetText("COMM Datapools");
               pc_DiagItem->c_ToolTipHeading = C_GtGetText::h_GetText("DIAG Datapools");
               pc_NvmItem->c_ToolTipHeading = C_GtGetText::h_GetText("NVM Datapools");
               pc_ComItem->c_ToolTipHeading = C_GtGetText::h_GetText("COMM Datapools");
               pc_DiagItem->q_Selectable = false;
               pc_NvmItem->q_Selectable = false;
               pc_ComItem->q_Selectable = false;
               //Use some unique index for expansion restoration mechanism
               pc_DiagItem->u32_Index = 0;
               pc_NvmItem->u32_Index = 1;
               pc_ComItem->u32_Index = 2;
               q_DataPoolDiagValid = false;
               q_DataPoolNvmValid = false;
               q_DataPoolComValid = false;
               //Init current node
               pc_NodeItem->u32_Index = u32_ItNode;
               pc_NodeItem->q_Selectable = false;
               pc_NodeItem->c_Name = pc_Node->c_Properties.c_Name.c_str();
               pc_NodeItem->c_ToolTipHeading = pc_Node->c_Properties.c_Name.c_str();
               pc_NodeItem->c_ToolTipContent = pc_Node->c_Properties.c_Comment.c_str();
               //Data pools
               pc_NodeItem->ReserveChildrenSpace(3UL);
               pc_DiagItem->ReserveChildrenSpace(pc_Node->c_DataPools.size());
               pc_NvmItem->ReserveChildrenSpace(pc_Node->c_DataPools.size());
               pc_ComItem->ReserveChildrenSpace(pc_Node->c_DataPools.size());
               for (uint32 u32_ItDataPool = 0; u32_ItDataPool < pc_Node->c_DataPools.size(); ++u32_ItDataPool)
               {
                  const C_OSCNodeDataPool & rc_DataPool = pc_Node->c_DataPools[u32_ItDataPool];
                  C_TblTreItem * const pc_DataPoolItem = new C_TblTreItem();
                  //Init current Datapool
                  pc_DataPoolItem->u32_Index = u32_ItDataPool;
                  pc_DataPoolItem->c_Name = rc_DataPool.c_Name.c_str();
                  pc_DataPoolItem->c_ToolTipHeading = rc_DataPool.c_Name.c_str();
                  pc_DataPoolItem->c_ToolTipContent = rc_DataPool.c_Comment.c_str();
                  pc_DataPoolItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconDatapool);
                  pc_DataPoolItem->q_Selectable = false;
                  // Append protocol type if COMM datapool
                  if (rc_DataPool.e_Type == C_OSCNodeDataPool::eCOM)
                  {
                     pc_DataPoolItem->c_Name += C_GtGetText::h_GetText(" (Protocol: ");
                     pc_DataPoolItem->c_Name +=
                        C_PuiSdUtil::h_ConvertProtocolTypeToString(C_PuiSdUtil::h_GetRelatedCANProtocolType(
                                                                      u32_ItNode, u32_ItDataPool));
                     pc_DataPoolItem->c_Name +=  ")";
                     pc_DataPoolItem->c_ToolTipHeading = pc_DataPoolItem->c_Name;
                  }
                  //Flag
                  q_DataPoolValid = false;
                  //Data pool
                  //Lists
                  pc_DataPoolItem->ReserveChildrenSpace(rc_DataPool.c_Lists.size());
                  for (uint32 u32_ItList = 0; u32_ItList < rc_DataPool.c_Lists.size(); ++u32_ItList)
                  {
                     const C_OSCNodeDataPoolList & rc_List = rc_DataPool.c_Lists[u32_ItList];
                     C_TblTreItem * const pc_ListItem = new C_TblTreItem();
                     //Init current node
                     pc_ListItem->u32_Index = u32_ItList;
                     pc_ListItem->c_Name = rc_List.c_Name.c_str();
                     pc_ListItem->c_ToolTipHeading = rc_List.c_Name.c_str();
                     // tooltip content: do not use h_GetToolTipContentDpList because we do not want so much info and
                     // consistency with superior tree items
                     pc_ListItem->c_ToolTipContent = rc_List.c_Comment.c_str();
                     pc_ListItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconList);
                     pc_ListItem->q_Selectable = false;
                     //Flag
                     q_ListValid = false;
                     //Elements
                     pc_ListItem->ReserveChildrenSpace(rc_List.c_Elements.size());
                     for (uint32 u32_ItElement = 0; u32_ItElement < rc_List.c_Elements.size(); ++u32_ItElement)
                     {
                        const C_PuiSdNodeDataPoolListElement * const pc_UiElement =
                           C_PuiSdHandler::h_GetInstance()->GetUIDataPoolListElement(u32_ItNode, u32_ItDataPool,
                                                                                     u32_ItList,
                                                                                     u32_ItElement);
                        const C_OSCNodeDataPoolListElement & rc_Element = rc_List.c_Elements[u32_ItElement];
                        C_TblTreItem * const pc_ElementItem =
                           new C_TblTreItem();
                        const C_OSCNodeDataPoolListElementId c_NodeDpListElement(u32_ItNode, u32_ItDataPool, u32_ItList,
                                                                                 u32_ItElement);
                        const bool oq_IsString = (pc_UiElement != NULL) ? pc_UiElement->q_InterpretAsString : false;
                        //Init current node
                        pc_ElementItem->u32_Index = u32_ItElement;
                        pc_ElementItem->c_ToolTipHeading = rc_Element.c_Name.c_str();
                        pc_ElementItem->c_ToolTipContent = C_SdUtil::h_GetToolTipContentDpListElement(
                           c_NodeDpListElement);
                        //Signal
                        q_NodeValid = true;
                        q_DataPoolValid = true;
                        q_ListValid = true;
                        //Type
                        switch (rc_DataPool.e_Type)
                        {
                        case C_OSCNodeDataPool::eDIAG:
                           q_DataPoolDiagValid = true;
                           pc_ElementItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconVariable);
                           break;
                        case C_OSCNodeDataPool::eNVM:
                           q_DataPoolNvmValid = true;
                           pc_ElementItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconParameter);
                           break;
                        case C_OSCNodeDataPool::eCOM:
                           q_DataPoolComValid = true;
                           pc_ElementItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconSignal);
                           break;
                        default:
                           break;
                        }

                        // Adapt the element item
                        C_TblTreDataElementModel::mh_ConfigureDatapoolElement(oq_ShowOnlyWriteElements,
                                                                              oq_ShowArrayElements,
                                                                              oq_ShowArrayIndexElements,
                                                                              oq_Show64BitValues,
                                                                              rc_Element, pc_ElementItem, oq_IsString);

                        //Array elements
                        C_TblTreDataElementModel::mh_CreateOrUpdateArrayElementNodes(oq_ShowOnlyWriteElements,
                                                                                     oq_ShowArrayIndexElements,
                                                                                     oq_Show64BitValues, rc_Element,
                                                                                     oq_IsString, pc_ElementItem);

                        //Add
                        pc_ListItem->AddChild(pc_ElementItem);
                     }
                     if (q_ListValid == true)
                     {
                        pc_DataPoolItem->AddChild(pc_ListItem);
                     }
                     else
                     {
                        delete (pc_ListItem);
                     }
                  }
                  if (q_DataPoolValid == true)
                  {
                     switch (rc_DataPool.e_Type)
                     {
                     case C_OSCNodeDataPool::eDIAG:
                        pc_DiagItem->AddChild(pc_DataPoolItem);
                        break;
                     case C_OSCNodeDataPool::eNVM:
                        pc_NvmItem->AddChild(pc_DataPoolItem);
                        break;
                     case C_OSCNodeDataPool::eCOM:
                        pc_ComItem->AddChild(pc_DataPoolItem);
                        break;
                     default:
                        delete (pc_DataPoolItem);
                        break;
                     }
                  }
                  else
                  {
                     delete (pc_DataPoolItem);
                  }
               }
               if (q_DataPoolDiagValid == true)
               {
                  pc_NodeItem->AddChild(pc_DiagItem);
               }
               else
               {
                  delete (pc_DiagItem);
               }
               if (q_DataPoolNvmValid == true)
               {
                  pc_NodeItem->AddChild(pc_NvmItem);
               }
               else
               {
                  delete (pc_NvmItem);
               }
               if (q_DataPoolComValid == true)
               {
                  pc_NodeItem->AddChild(pc_ComItem);
               }
               else
               {
                  delete (pc_ComItem);
               }
            }
            if (q_NodeValid == true)
            {
               this->mpc_InvisibleRootItem->AddChild(pc_NodeItem);
            }
            else
            {
               delete (pc_NodeItem);
            }
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Adapts the properties of already existing tree structure for data pool elements

   \param[in] oq_ShowOnlyWriteElements  Optional flag to show only writable elements
   \param[in] oq_ShowArrayElements      Optional flag to hide all array elements (if false)
   \param[in] oq_ShowArrayIndexElements Show array index elements
   \param[in] oq_Show64BitValues        Optional flag to hide all 64 bit elements (if false)
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::m_UpdateDatapoolElement(const bool oq_ShowOnlyWriteElements,
                                                       const bool oq_ShowArrayElements,
                                                       const bool oq_ShowArrayIndexElements,
                                                       const bool oq_Show64BitValues)
{
   if (this->mpc_InvisibleRootItem != NULL)
   {
      uint32 u32_NodeCounter;

      // Node layer
      for (u32_NodeCounter = 0U; u32_NodeCounter < this->mpc_InvisibleRootItem->c_Children.size(); ++u32_NodeCounter)
      {
         //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
         C_TblTreItem * const pc_NodeItem =
            dynamic_cast<C_TblTreItem * const>(this->mpc_InvisibleRootItem->c_Children[u32_NodeCounter]);
         const C_OSCNode * pc_OscNode = NULL;

         if (pc_NodeItem != NULL)
         {
            pc_OscNode = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(pc_NodeItem->u32_Index);
         }

         if ((pc_OscNode != NULL) &&
             (pc_NodeItem != NULL))
         {
            uint32 u32_DpTypeCounter;

            // Datapool type layer
            for (u32_DpTypeCounter = 0U; u32_DpTypeCounter < pc_NodeItem->c_Children.size(); ++u32_DpTypeCounter)
            {
               //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
               C_TblTreItem * const pc_DpTypeItem =
                  dynamic_cast<C_TblTreItem * const>(pc_NodeItem->c_Children[u32_DpTypeCounter]);

               if (pc_DpTypeItem != NULL)
               {
                  uint32 u32_DatapoolCounter;

                  // Datapool layer
                  for (u32_DatapoolCounter = 0U; u32_DatapoolCounter < pc_DpTypeItem->c_Children.size();
                       ++u32_DatapoolCounter)
                  {
                     //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
                     C_TblTreItem * const pc_DatapoolItem =
                        dynamic_cast<C_TblTreItem * const>(pc_DpTypeItem->c_Children[u32_DatapoolCounter]);

                     if (pc_DatapoolItem != NULL)
                     {
                        uint32 u32_ListCounter;

                        // List Layer
                        for (u32_ListCounter = 0U; u32_ListCounter < pc_DatapoolItem->c_Children.size();
                             ++u32_ListCounter)
                        {
                           //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
                           C_TblTreItem * const pc_ListItem =
                              dynamic_cast<C_TblTreItem * const>(pc_DatapoolItem->c_Children[u32_ListCounter]);

                           if (pc_ListItem != NULL)
                           {
                              uint32 u32_ElementCounter;

                              // Element layer
                              for (u32_ElementCounter = 0U; u32_ElementCounter < pc_ListItem->c_Children.size();
                                   ++u32_ElementCounter)
                              {
                                 //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
                                 C_TblTreItem * const pc_ElementItem =
                                    dynamic_cast<C_TblTreItem * const>(pc_ListItem->c_Children[u32_ElementCounter]);

                                 // Check if the element index is valid
                                 if ((pc_ElementItem != NULL) &&
                                     (pc_DatapoolItem->u32_Index < pc_OscNode->c_DataPools.size()) &&
                                     (pc_ListItem->u32_Index <
                                      pc_OscNode->c_DataPools[pc_DatapoolItem->u32_Index].c_Lists.size()) &&
                                     (pc_ElementItem->u32_Index <
                                      pc_OscNode->c_DataPools[pc_DatapoolItem->u32_Index].
                                      c_Lists[pc_ListItem->u32_Index].c_Elements.size()))
                                 {
                                    const C_PuiSdNodeDataPoolListElement * const pc_UiElement =
                                       C_PuiSdHandler::h_GetInstance()->GetUIDataPoolListElement(pc_NodeItem->u32_Index,
                                                                                                 pc_DatapoolItem->u32_Index,
                                                                                                 pc_ListItem->u32_Index,
                                                                                                 pc_ElementItem->u32_Index);
                                    const bool oq_IsString = (pc_UiElement !=
                                                              NULL) ? pc_UiElement->q_InterpretAsString : false;
                                    const C_OSCNodeDataPoolListElement & rc_Element =
                                       pc_OscNode->c_DataPools[pc_DatapoolItem->u32_Index].
                                       c_Lists[pc_ListItem->u32_Index].c_Elements[pc_ElementItem->u32_Index];

                                    // Adapt the element item
                                    C_TblTreDataElementModel::mh_ConfigureDatapoolElement(oq_ShowOnlyWriteElements,
                                                                                          oq_ShowArrayElements,
                                                                                          oq_ShowArrayIndexElements,
                                                                                          oq_Show64BitValues,
                                                                                          rc_Element,
                                                                                          pc_ElementItem, oq_IsString);

                                    //Array element layer
                                    C_TblTreDataElementModel::mh_CreateOrUpdateArrayElementNodes(
                                       oq_ShowOnlyWriteElements,
                                       oq_ShowArrayIndexElements,
                                       oq_Show64BitValues,
                                       rc_Element, oq_IsString,
                                       pc_ElementItem);
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Adapts the properties of the tree item

   Sets the enabled and selectable flag dependent of the paramters oq_ShowOnlyWriteElements, oq_ShowArrayElements and
   oq_Show64BitValues

   \param[in] oq_ShowOnlyWriteElements  Optional flag to show only writable elements
   \param[in] oq_ShowArrayElements      Optional flag to hide all array elements (if false)
   \param[in] oq_ShowArrayIndexElements Optional flag to hide all array index elements (if false)
   \param[in] oq_Show64BitValues        Optional flag to hide all 64 bit elements (if false)
   \param[in] orc_Element               Datapool element of tree item
   \param[in] opc_ElementItem           Tree item for datapool element
   \param[in] oq_IsString               Is string
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::mh_ConfigureDatapoolElement(const bool oq_ShowOnlyWriteElements,
                                                           const bool oq_ShowArrayElements,
                                                           const bool oq_ShowArrayIndexElements,
                                                           const bool oq_Show64BitValues,
                                                           const C_OSCNodeDataPoolListElement & orc_Element,
                                                           C_TblTreItem * const opc_ElementItem, const bool oq_IsString)
{
   if (opc_ElementItem != NULL)
   {
      // Set the name always here, in case of an earlier update, the name was adapted with the explanation
      opc_ElementItem->c_Name = orc_Element.c_Name.c_str();

      if ((orc_Element.GetArray() == false) || (oq_ShowArrayElements == true))
      {
         if ((((orc_Element.GetType() != C_OSCNodeDataPoolContent::eFLOAT64) &&
               (orc_Element.GetType() != C_OSCNodeDataPoolContent::eUINT64)) &&
              (orc_Element.GetType() != C_OSCNodeDataPoolContent::eSINT64)) ||
             (oq_Show64BitValues == true))
         {
            if ((orc_Element.e_Access == C_OSCNodeDataPoolListElement::eACCESS_RW) ||
                (oq_ShowOnlyWriteElements == false))
            {
               //Valid
               opc_ElementItem->q_Enabled = true;
               opc_ElementItem->q_Selectable = true;
            }
            else
            {
               opc_ElementItem->q_Enabled = false;
               opc_ElementItem->q_Selectable = false;
               //Explanation
               opc_ElementItem->c_Name += C_TblTreDataElementModel::mhc_AdditionalWriteOnlyInfo;
            }
         }
         else
         {
            opc_ElementItem->q_Enabled = false;
            opc_ElementItem->q_Selectable = false;
            //Explanation
            opc_ElementItem->c_Name += C_TblTreDataElementModel::mhc_Additional64BitInfo;
         }
      }
      else
      {
         if (oq_IsString)
         {
            opc_ElementItem->q_Enabled = false;
            opc_ElementItem->q_Selectable = false;
            //Explanation
            opc_ElementItem->c_Name += C_TblTreDataElementModel::mhc_AdditionalArrayStringInfo;
         }
         else
         {
            //If there are select-able child items don't disable the parent node
            if (oq_ShowArrayIndexElements)
            {
               opc_ElementItem->q_Enabled = true;
            }
            else
            {
               opc_ElementItem->q_Enabled = false;
            }
            opc_ElementItem->q_Selectable = false;
            //If there are select-able child items don't add explanation
            if (oq_ShowArrayIndexElements == false)
            {
               //Explanation
               opc_ElementItem->c_Name += C_TblTreDataElementModel::mhc_AdditionalArrayInfo;
            }
         }
      }
   }
   //lint -e{429}  no memory leak because of no ownership of the element opc_ElementItem here
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Adapts the properties of the tree item

   Sets the enabled and selectable flag dependent of the paramters oq_ShowOnlyWriteElements, oq_ShowArrayElements,
   oq_ShowArrayIndexElements and oq_Show64BitValues

   \param[in]     oq_ShowOnlyWriteElements  Optional flag to show only writable elements
   \param[in]     oq_ShowArrayIndexElements Optional flag to hide all array index elements (if false)
   \param[in]     oq_Show64BitValues        Optional flag to hide all 64 bit elements (if false)
   \param[in]     orc_Element               Datapool element of tree item
   \param[in,out] opc_ArrayItem             Array item
   \param[in]     ou32_Index                Index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::mh_ConfigureDatapoolArrayElement(const bool oq_ShowOnlyWriteElements,
                                                                const bool oq_ShowArrayIndexElements,
                                                                const bool oq_Show64BitValues,
                                                                const C_OSCNodeDataPoolListElement & orc_Element,
                                                                C_TblTreItem * const opc_ArrayItem,
                                                                const uint32 ou32_Index)
{
   if (opc_ArrayItem != NULL)
   {
      // Set the name always here, in case of an earlier update, the name was adapted with the explanation
      opc_ArrayItem->c_Name = QString("[%1]").arg(ou32_Index);

      if (oq_ShowArrayIndexElements == true)
      {
         if ((((orc_Element.GetType() != C_OSCNodeDataPoolContent::eFLOAT64) &&
               (orc_Element.GetType() != C_OSCNodeDataPoolContent::eUINT64)) &&
              (orc_Element.GetType() != C_OSCNodeDataPoolContent::eSINT64)) ||
             (oq_Show64BitValues == true))
         {
            if ((orc_Element.e_Access == C_OSCNodeDataPoolListElement::eACCESS_RW) ||
                (oq_ShowOnlyWriteElements == false))
            {
               //Valid
               opc_ArrayItem->q_Enabled = true;
               opc_ArrayItem->q_Selectable = true;
            }
            else
            {
               opc_ArrayItem->q_Enabled = false;
               opc_ArrayItem->q_Selectable = false;
               //Explanation
               opc_ArrayItem->c_Name += C_TblTreDataElementModel::mhc_AdditionalWriteOnlyInfo;
            }
         }
         else
         {
            opc_ArrayItem->q_Enabled = false;
            opc_ArrayItem->q_Selectable = false;
            //Explanation
            opc_ArrayItem->c_Name += C_TblTreDataElementModel::mhc_Additional64BitInfo;
         }
      }
      else
      {
         opc_ArrayItem->q_Enabled = false;
         opc_ArrayItem->q_Selectable = false;
         //Explanation
         opc_ArrayItem->c_Name += C_TblTreDataElementModel::mhc_AdditionalArrayIndexInfo;
      }
   }
   //lint -e{429}  no memory leak because of no ownership of the element opc_ElementItem here
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Handle array element section

   \param[in] oq_ShowOnlyWriteElements  Optional flag to show only writable elements
   \param[in] oq_ShowArrayIndexElements Optional flag to hide all array index elements (if false)
   \param[in] oq_Show64BitValues        Optional flag to hide all 64 bit elements (if false)
   \param[in] orc_Element               Datapool element of tree item
   \param[in] oq_IsStringElement        Flag if current item is an string element
   \param[in] opc_ElementItem           Tree item for datapool element
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::mh_CreateOrUpdateArrayElementNodes(const bool oq_ShowOnlyWriteElements,
                                                                  const bool oq_ShowArrayIndexElements,
                                                                  const bool oq_Show64BitValues,
                                                                  const C_OSCNodeDataPoolListElement & orc_Element,
                                                                  const bool oq_IsStringElement,
                                                                  C_TblTreItem * const opc_ElementItem)
{
   if (opc_ElementItem != NULL)
   {
      if ((orc_Element.GetArray()) && (oq_IsStringElement == false))
      {
         if (opc_ElementItem->c_Children.size() == orc_Element.GetArraySize())
         {
            //Update
            for (uint32 u32_ArrayIndex = 0U; u32_ArrayIndex < orc_Element.GetArraySize(); ++u32_ArrayIndex)
            {
               //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
               C_TblTreItem * const pc_ArrayItem =
                  dynamic_cast<C_TblTreItem * const>(opc_ElementItem->c_Children[
                                                        u32_ArrayIndex]);

               C_TblTreDataElementModel::mh_ConfigureDatapoolArrayElement(oq_ShowOnlyWriteElements,
                                                                          oq_ShowArrayIndexElements,
                                                                          oq_Show64BitValues,
                                                                          orc_Element,
                                                                          pc_ArrayItem, u32_ArrayIndex);
            }
         }
         else
         {
            //Create new
            for (uint32 u32_ArrayIndex = 0U; u32_ArrayIndex < orc_Element.GetArraySize();
                 ++u32_ArrayIndex)
            {
               C_TblTreItem * const pc_ArrayItem = new C_TblTreItem();
               //Init current node
               pc_ArrayItem->u32_Index = u32_ArrayIndex;
               pc_ArrayItem->c_Name = QString("[%1]").arg(u32_ArrayIndex);
               pc_ArrayItem->c_ToolTipHeading = pc_ArrayItem->c_Name;
               pc_ArrayItem->c_ToolTipContent =
                  QString(C_GtGetText::h_GetText("Array element %1 of \"%2\"")).arg(u32_ArrayIndex).arg(
                     orc_Element.c_Name.c_str());

               //Configure
               C_TblTreDataElementModel::mh_ConfigureDatapoolArrayElement(oq_ShowOnlyWriteElements,
                                                                          oq_ShowArrayIndexElements,
                                                                          oq_Show64BitValues,
                                                                          orc_Element,
                                                                          pc_ArrayItem, u32_ArrayIndex);

               //Add
               opc_ElementItem->AddChild(pc_ArrayItem);
            }
         }
      }
      else
      {
         opc_ElementItem->ClearChildren();
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Initialize tree structure for data pool lists

   \param[in] ou32_ViewIndex View index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::m_InitNvmList(const uint32 ou32_ViewIndex)
{
   const C_PuiSvData * const pc_View = C_PuiSvHandler::h_GetInstance()->GetView(ou32_ViewIndex);

   if (pc_View != NULL)
   {
      const std::vector<uint8> & rc_NodeActiveFlags = pc_View->GetNodeActiveFlags();
      bool q_NodeValid;
      bool q_DataPoolDiagValid;
      bool q_DataPoolNvmValid;
      bool q_DataPoolComValid;
      bool q_DataPoolValid;
      const uint32 u32_NodeSize = C_PuiSdHandler::h_GetInstance()->GetOSCNodesSize();

      //Nodes
      this->mpc_InvisibleRootItem->ReserveChildrenSpace(u32_NodeSize);
      tgl_assert(rc_NodeActiveFlags.size() == u32_NodeSize);
      for (uint32 u32_ItNode = 0; u32_ItNode < u32_NodeSize; ++u32_ItNode)
      {
         if ((rc_NodeActiveFlags[u32_ItNode] == true) && (mh_CheckNodeDiagnostic(ou32_ViewIndex, u32_ItNode) == true))
         {
            C_TblTreItem * const pc_NodeItem = new C_TblTreItem();
            const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(u32_ItNode);
            q_NodeValid = false;
            //Static
            pc_NodeItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconNode);
            //Node
            if (pc_Node != NULL)
            {
               //Data pool type nodes
               C_TblTreItem * const pc_DiagItem = new C_TblTreItem();
               C_TblTreItem * const pc_NvmItem = new C_TblTreItem();
               C_TblTreItem * const pc_ComItem = new C_TblTreItem();
               pc_DiagItem->c_Name = C_GtGetText::h_GetText("DIAG Datapools");
               pc_NvmItem->c_Name = C_GtGetText::h_GetText("NVM Datapools");
               pc_ComItem->c_Name = C_GtGetText::h_GetText("COMM Datapools");
               pc_DiagItem->c_ToolTipHeading = C_GtGetText::h_GetText("DIAG Datapools");
               pc_NvmItem->c_ToolTipHeading = C_GtGetText::h_GetText("NVM Datapools");
               pc_ComItem->c_ToolTipHeading = C_GtGetText::h_GetText("COMM Datapools");
               pc_DiagItem->q_Selectable = false;
               pc_NvmItem->q_Selectable = false;
               pc_ComItem->q_Selectable = false;
               //Probably invalid index to detect errors
               pc_DiagItem->u32_Index = 12345678;
               pc_NvmItem->u32_Index = 12345678;
               pc_ComItem->u32_Index = 12345678;
               q_DataPoolDiagValid = false;
               q_DataPoolNvmValid = false;
               q_DataPoolComValid = false;
               //Init current node
               pc_NodeItem->u32_Index = u32_ItNode;
               pc_NodeItem->q_Selectable = false;
               pc_NodeItem->c_Name = pc_Node->c_Properties.c_Name.c_str();
               pc_NodeItem->c_ToolTipHeading = pc_Node->c_Properties.c_Name.c_str();
               pc_NodeItem->c_ToolTipContent = pc_Node->c_Properties.c_Comment.c_str();
               //Data pools
               pc_NodeItem->ReserveChildrenSpace(3UL);
               pc_DiagItem->ReserveChildrenSpace(pc_Node->c_DataPools.size());
               pc_NvmItem->ReserveChildrenSpace(pc_Node->c_DataPools.size());
               pc_ComItem->ReserveChildrenSpace(pc_Node->c_DataPools.size());
               for (uint32 u32_ItDataPool = 0; u32_ItDataPool < pc_Node->c_DataPools.size(); ++u32_ItDataPool)
               {
                  const C_OSCNodeDataPool & rc_DataPool = pc_Node->c_DataPools[u32_ItDataPool];
                  C_TblTreItem * const pc_DataPoolItem = new C_TblTreItem();
                  //Init current node
                  pc_DataPoolItem->u32_Index = u32_ItDataPool;
                  pc_DataPoolItem->q_Selectable = false;
                  pc_DataPoolItem->c_Name = rc_DataPool.c_Name.c_str();
                  pc_DataPoolItem->c_ToolTipHeading = rc_DataPool.c_Name.c_str();
                  pc_DataPoolItem->c_ToolTipContent = rc_DataPool.c_Comment.c_str();
                  pc_DataPoolItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconDatapool);
                  //Flag
                  q_DataPoolValid = false;
                  //Data pool
                  //Lists
                  pc_DataPoolItem->ReserveChildrenSpace(rc_DataPool.c_Lists.size());
                  for (uint32 u32_ItList = 0; u32_ItList < rc_DataPool.c_Lists.size(); ++u32_ItList)
                  {
                     const C_OSCNodeDataPoolList & rc_List = rc_DataPool.c_Lists[u32_ItList];
                     C_TblTreItem * const pc_ListItem = new C_TblTreItem();
                     //Init current node
                     pc_ListItem->u32_Index = u32_ItList;
                     pc_ListItem->c_Name = rc_List.c_Name.c_str();
                     pc_ListItem->c_ToolTipHeading = rc_List.c_Name.c_str();
                     pc_ListItem->c_ToolTipContent =
                        C_SdUtil::h_GetToolTipContentDpList(u32_ItNode, u32_ItDataPool, u32_ItList);
                     pc_ListItem->c_Icon = QIcon(C_TblTreDataElementModel::mhc_IconList);
                     //Elements
                     if (rc_List.c_Elements.size() > 0)
                     {
                        //Signal
                        q_NodeValid = true;
                        q_DataPoolValid = true;
                        switch (rc_DataPool.e_Type)
                        {
                        case C_OSCNodeDataPool::eNVM:
                           q_DataPoolNvmValid = true;
                           //Check usage
                           if (pc_View->CheckNvmParamListUsage(C_OSCNodeDataPoolListId(u32_ItNode, u32_ItDataPool,
                                                                                       u32_ItList)) == true)
                           {
                              pc_ListItem->c_Name += C_GtGetText::h_GetText(" (Already used)");
                              pc_ListItem->q_Selectable = false;
                              pc_ListItem->q_Enabled = false;
                           }
                           else
                           {
                              pc_ListItem->q_Selectable = true;
                              pc_ListItem->q_Enabled = true;
                           }
                           break;
                        default:
                           break;
                        }
                        //Add
                        pc_DataPoolItem->AddChild(pc_ListItem);
                     }
                     else
                     {
                        delete (pc_ListItem);
                     }
                  }
                  if (q_DataPoolValid == true)
                  {
                     switch (rc_DataPool.e_Type)
                     {
                     case C_OSCNodeDataPool::eDIAG:
                        pc_DiagItem->AddChild(pc_DataPoolItem);
                        break;
                     case C_OSCNodeDataPool::eNVM:
                        pc_NvmItem->AddChild(pc_DataPoolItem);
                        break;
                     case C_OSCNodeDataPool::eCOM:
                        pc_ComItem->AddChild(pc_DataPoolItem);
                        break;
                     default:
                        delete (pc_DataPoolItem);
                        break;
                     }
                  }
                  else
                  {
                     delete (pc_DataPoolItem);
                  }
               }
               //lint -e{948}  Kept because of similarities to other init functions
               if (q_DataPoolDiagValid == true)
               {
                  pc_NodeItem->AddChild(pc_DiagItem);
               }
               else
               {
                  delete (pc_DiagItem);
               }
               if (q_DataPoolNvmValid == true)
               {
                  pc_NodeItem->AddChild(pc_NvmItem);
               }
               else
               {
                  delete (pc_NvmItem);
               }
               //lint -e{948}  Kept because of similarities to other init functions
               if (q_DataPoolComValid == true)
               {
                  pc_NodeItem->AddChild(pc_ComItem);
               }
               else
               {
                  delete (pc_ComItem);
               }
            }
            if (q_NodeValid == true)
            {
               this->mpc_InvisibleRootItem->AddChild(pc_NodeItem);
            }
            else
            {
               delete (pc_NodeItem);
            }
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get bus signals

   \param[in] orc_Index Index

   \return
   Bus signals
*/
//----------------------------------------------------------------------------------------------------------------------
std::vector<C_PuiSvDbNodeDataPoolListElementId> C_TblTreDataElementModel::m_GetBusSignals(const QModelIndex & orc_Index)
const
{
   std::vector<C_PuiSvDbNodeDataPoolListElementId> c_Retval;
   //lint -e{925}  Result of Qt interface restrictions, set by index function
   const C_TblTreItem * const pc_TreeItem =
      static_cast<const C_TblTreItem * const>(orc_Index.internalPointer());

   if (pc_TreeItem != NULL)
   {
      //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
      const C_TblTreItem * const pc_FirstParent = dynamic_cast<const C_TblTreItem * const>(pc_TreeItem->pc_Parent);
      if ((pc_FirstParent != NULL) && (pc_FirstParent->pc_Parent != NULL))
      {
         //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
         const C_TblTreItem * const pc_SecondParent =
            dynamic_cast<const C_TblTreItem * const>(pc_FirstParent->pc_Parent);
         if ((pc_SecondParent != NULL) && (pc_SecondParent->pc_Parent != NULL))
         {
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            const C_TblTreItem * const pc_ThirdParent =
               dynamic_cast<const C_TblTreItem * const>(pc_SecondParent->pc_Parent);
            if ((pc_ThirdParent != NULL) && (pc_ThirdParent->pc_Parent != NULL))
            {
               //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
               const C_TblTreItem * const pc_FourthParent =
                  dynamic_cast<const C_TblTreItem * const>(pc_ThirdParent->pc_Parent);
               if ((pc_FourthParent != NULL) && (pc_FourthParent->pc_Parent != NULL))
               {
                  //Should not happen
                  tgl_assert(false);
               }
               else
               {
                  //Signal element
                  //4: Invisible item
                  //3: Bus item
                  //2: Protocol item
                  //1: Message item

                  //3 Sync managers, second stores index of type
                  const uint32 u32_MessageVectorIndex = pc_SecondParent->u32_Index;
                  if (u32_MessageVectorIndex < this->mc_MessageSyncManagers.size())
                  {
                     const C_PuiSdNodeCanMessageSyncManager * const pc_MessageSyncManager =
                        this->mc_MessageSyncManagers[u32_MessageVectorIndex];
                     if (pc_MessageSyncManager != NULL)
                     {
                        const std::vector<C_OSCCanMessageIdentificationIndices> c_Messages =
                           pc_MessageSyncManager->GetUniqueMessages();
                        if (pc_FirstParent->u32_Index < c_Messages.size())
                        {
                           c_Retval.push_back(mh_Translate(c_Messages[pc_FirstParent->u32_Index],
                                                           pc_TreeItem->u32_Index));
                        }
                     }
                  }
               }
            }
            else
            {
               //Message
               //3: Invisible item
               //2: Bus item
               //1: Protocol item
               //TODO ?
            }
         }
         else
         {
            //Protocol
            //2: Invisible item
            //1: Bus item
            //TODO ?
         }
      }
      else
      {
         //Bus
         //1: Invisible item
         //No valid parent
         //TODO ?
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get data pools

   \param[in] orc_Index Index

   \return
   Data pools
*/
//----------------------------------------------------------------------------------------------------------------------
std::vector<C_PuiSvDbNodeDataPoolListElementId> C_TblTreDataElementModel::m_GetDatapools(const QModelIndex & orc_Index)
const
{
   std::vector<C_PuiSvDbNodeDataPoolListElementId> c_Retval;
   //lint -e{925}  Result of Qt interface restrictions, set by index function
   const C_TblTreItem * const pc_TreeItem =
      static_cast<const C_TblTreItem * const>(orc_Index.internalPointer());

   if (pc_TreeItem != NULL)
   {
      //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
      const C_TblTreItem * const pc_FirstParent = dynamic_cast<const C_TblTreItem * const>(pc_TreeItem->pc_Parent);
      if ((pc_FirstParent != NULL) && (pc_FirstParent->pc_Parent != NULL))
      {
         //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
         const C_TblTreItem * const pc_SecondParent =
            dynamic_cast<const C_TblTreItem * const>(pc_FirstParent->pc_Parent);
         if ((pc_SecondParent != NULL) && (pc_SecondParent->pc_Parent != NULL))
         {
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            const C_TblTreItem * const pc_ThirdParent =
               dynamic_cast<const C_TblTreItem * const>(pc_SecondParent->pc_Parent);
            if ((pc_ThirdParent != NULL) && (pc_ThirdParent->pc_Parent != NULL))
            {
               //Should not happen
               tgl_assert(false);
            }
            else
            {
               //Data pool
               //3: Invisible item
               //2: Node item
               //1: Data pool type item
               c_Retval.push_back(C_PuiSvDbNodeDataPoolListElementId(pc_SecondParent->u32_Index,
                                                                     pc_TreeItem->u32_Index, 0UL, 0UL,
                                                                     C_PuiSvDbNodeDataPoolListElementId::
                                                                     eDATAPOOL_ELEMENT, false, 0UL));
            }
         }
         else
         {
            //Data pool type
            //2: Invisible item
            //1: Node item
            //TODO ?
         }
      }
      else
      {
         //Node
         //1: Invisible item
         //No valid parent
         //TODO ?
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get data pool elements

   \param[in] orc_Index Index

   \return
   Data pool elements
*/
//----------------------------------------------------------------------------------------------------------------------
std::vector<C_PuiSvDbNodeDataPoolListElementId> C_TblTreDataElementModel::m_GetDatapoolElements(
   const QModelIndex & orc_Index) const
{
   std::vector<C_PuiSvDbNodeDataPoolListElementId> c_Retval;
   //lint -e{925}  Result of Qt interface restrictions, set by index function
   const C_TblTreItem * const pc_TreeItem =
      static_cast<const C_TblTreItem * const>(orc_Index.internalPointer());

   if (pc_TreeItem != NULL)
   {
      //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
      const C_TblTreItem * const pc_FirstParent = dynamic_cast<const C_TblTreItem * const>(pc_TreeItem->pc_Parent);
      if ((pc_FirstParent != NULL) && (pc_FirstParent->pc_Parent != NULL))
      {
         //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
         const C_TblTreItem * const pc_SecondParent =
            dynamic_cast<const C_TblTreItem * const>(pc_FirstParent->pc_Parent);
         if ((pc_SecondParent != NULL) && (pc_SecondParent->pc_Parent != NULL))
         {
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            const C_TblTreItem * const pc_ThirdParent =
               dynamic_cast<const C_TblTreItem * const>(pc_SecondParent->pc_Parent);
            if ((pc_ThirdParent != NULL) && (pc_ThirdParent->pc_Parent != NULL))
            {
               //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
               const C_TblTreItem * const pc_FourthParent =
                  dynamic_cast<const C_TblTreItem * const>(pc_ThirdParent->pc_Parent);
               if ((pc_FourthParent != NULL) && (pc_FourthParent->pc_Parent != NULL))
               {
                  //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
                  const C_TblTreItem * const pc_FifthParent =
                     dynamic_cast<const C_TblTreItem * const>(pc_FourthParent->pc_Parent);
                  if ((pc_FifthParent != NULL) && (pc_FifthParent->pc_Parent != NULL))
                  {
                     //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
                     const C_TblTreItem * const pc_SixthParent =
                        dynamic_cast<const C_TblTreItem * const>(pc_FifthParent->pc_Parent);
                     if ((pc_SixthParent != NULL) && (pc_SixthParent->pc_Parent != NULL))
                     {
                        //Should not happen
                        tgl_assert(false);
                     }
                     else
                     {
                        //Data array index element
                        //6: Invisible item
                        //5: Node item
                        //4: Data pool type item
                        //3: Data pool item
                        //2: List item
                        //1: Element item
                        c_Retval.push_back(C_PuiSvDbNodeDataPoolListElementId(pc_FifthParent->u32_Index,
                                                                              pc_FourthParent->u32_Index,
                                                                              pc_SecondParent->u32_Index,
                                                                              pc_FirstParent->u32_Index,
                                                                              C_PuiSvDbNodeDataPoolListElementId::
                                                                              eDATAPOOL_ELEMENT, true,
                                                                              pc_TreeItem->u32_Index));
                     }
                  }
                  else
                  {
                     //Data element
                     //5: Invisible item
                     //4: Node item
                     //3: Data pool type item
                     //2: Data pool item
                     //1: List item
                     c_Retval.push_back(C_PuiSvDbNodeDataPoolListElementId(pc_FourthParent->u32_Index,
                                                                           pc_SecondParent->u32_Index,
                                                                           pc_FirstParent->u32_Index,
                                                                           pc_TreeItem->u32_Index,
                                                                           C_PuiSvDbNodeDataPoolListElementId::
                                                                           eDATAPOOL_ELEMENT, false, 0UL));
                  }
               }
               else
               {
                  //List
                  //4: Invisible item
                  //3: Node item
                  //2: Data pool type item
                  //1: Data pool item
                  //TODO ?
               }
            }
            else
            {
               //Data pool
               //3: Invisible item
               //2: Node item
               //1: Data pool type item
               //TODO ?
            }
         }
         else
         {
            //Data pool type
            //2: Invisible item
            //1: Node item
            //TODO ?
         }
      }
      else
      {
         //Node
         //1: Invisible item
         //No valid parent
         //TODO ?
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get data pool elements

   \param[in] orc_Index Index

   \return
   Data pool elements
*/
//----------------------------------------------------------------------------------------------------------------------
std::vector<C_PuiSvDbNodeDataPoolListElementId> C_TblTreDataElementModel::m_GetNvmList(const QModelIndex & orc_Index)
const
{
   std::vector<C_PuiSvDbNodeDataPoolListElementId> c_Retval;
   //lint -e{925}  Result of Qt interface restrictions, set by index function
   const C_TblTreItem * const pc_TreeItem =
      static_cast<const C_TblTreItem * const>(orc_Index.internalPointer());

   if (pc_TreeItem != NULL)
   {
      //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
      const C_TblTreItem * const pc_FirstParent = dynamic_cast<const C_TblTreItem * const>(pc_TreeItem->pc_Parent);
      if ((pc_FirstParent != NULL) && (pc_FirstParent->pc_Parent != NULL))
      {
         //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
         const C_TblTreItem * const pc_SecondParent =
            dynamic_cast<const C_TblTreItem * const>(pc_FirstParent->pc_Parent);
         if ((pc_SecondParent != NULL) && (pc_SecondParent->pc_Parent != NULL))
         {
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            const C_TblTreItem * const pc_ThirdParent =
               dynamic_cast<const C_TblTreItem * const>(pc_SecondParent->pc_Parent);
            if ((pc_ThirdParent != NULL) && (pc_ThirdParent->pc_Parent != NULL))
            {
               //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
               const C_TblTreItem * const pc_FourthParent =
                  dynamic_cast<const C_TblTreItem * const>(pc_ThirdParent->pc_Parent);
               if ((pc_FourthParent != NULL) && (pc_FourthParent->pc_Parent != NULL))
               {
                  //Should not happen
                  tgl_assert(false);
               }
               else
               {
                  //List
                  //4: Invisible item
                  //3: Node item
                  //2: Data pool type item
                  //1: Data pool item
                  const uint32 u32_NodeIndex = pc_ThirdParent->u32_Index;
                  const uint32 u32_DataPoolIndex = pc_FirstParent->u32_Index;
                  const uint32 u32_ListIndex = pc_TreeItem->u32_Index;
                  const C_OSCNodeDataPoolList * const pc_OSCList = C_PuiSdHandler::h_GetInstance()->GetOSCDataPoolList(
                     u32_NodeIndex, u32_DataPoolIndex, u32_ListIndex);
                  if (pc_OSCList != NULL)
                  {
                     for (uint32 u32_ItElement = 0; u32_ItElement < pc_OSCList->c_Elements.size(); ++u32_ItElement)
                     {
                        c_Retval.push_back(C_PuiSvDbNodeDataPoolListElementId(u32_NodeIndex,
                                                                              u32_DataPoolIndex,
                                                                              u32_ListIndex,
                                                                              u32_ItElement,
                                                                              C_PuiSvDbNodeDataPoolListElementId::
                                                                              eDATAPOOL_ELEMENT, false, 0UL));
                     }
                  }
                  else
                  {
                     //Should not happen
                     tgl_assert(false);
                  }
               }
            }
            else
            {
               //Data pool
               //3: Invisible item
               //2: Node item
               //1: Data pool type item
               //TODO ?
            }
         }
         else
         {
            //Data pool type
            //2: Invisible item
            //1: Node item
            //TODO ?
         }
      }
      else
      {
         //Node
         //1: Invisible item
         //No valid parent
         //TODO ?
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Clean up map

   \param[in] orc_Map Map to clean up
*/
//----------------------------------------------------------------------------------------------------------------------
void C_TblTreDataElementModel::mh_CleanUp(QMap<std::vector<uint32>, C_TblTreDataElementModelState> & orc_Map)
{
   for (QMap<std::vector<uint32>, C_TblTreDataElementModelState>::iterator c_It = orc_Map.begin();
        c_It != orc_Map.end(); ++c_It)
   {
      c_It.value().CleanUp();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Check if item in map

   \param[in] orc_Map  Map to look in
   \param[in] opc_Item Item to look for

   \return
   True  Found
   False Not found
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_TblTreDataElementModel::mh_Contains(const QMap<std::vector<uint32>, C_TblTreDataElementModelState> & orc_Map,
                                           const C_TblTreSimpleItem * const opc_Item)
{
   bool q_Retval = false;

   for (QMap<std::vector<uint32>, C_TblTreDataElementModelState>::const_iterator c_It = orc_Map.begin();
        c_It != orc_Map.end(); ++c_It)
   {
      if (c_It.value().pc_Tree == opc_Item)
      {
         q_Retval = true;
         break;
      }
   }
   return q_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get view and system definition hash in combination

   \param[in] ou32_ViewIndex View index

   \return
   View and system definition hash in combination
*/
//----------------------------------------------------------------------------------------------------------------------
std::vector<uint32> C_TblTreDataElementModel::mh_GetViewSdHash(const uint32 ou32_ViewIndex)
{
   std::vector<uint32> c_Retval;
   uint32 u32_Hash;
   const C_PuiSvData * const pc_View = C_PuiSvHandler::h_GetInstance()->GetView(ou32_ViewIndex);
   if (pc_View != NULL)
   {
      bool q_Data;
      uint32 u32_Data;
      const std::vector<uint8> & rc_NodeActiveFlags = pc_View->GetNodeActiveFlags();
      u32_Hash = 0xFFFFFFFFUL;
      //Active flags
      for (uint32 u32_ItNode = 0; u32_ItNode < rc_NodeActiveFlags.size(); ++u32_ItNode)
      {
         q_Data = static_cast<bool>(rc_NodeActiveFlags[u32_ItNode]);
         stw_scl::C_SCLChecksums::CalcCRC32(&q_Data, sizeof(q_Data), u32_Hash);
      }
      //Relevant PC data
      q_Data = pc_View->GetPcData().GetConnected();
      u32_Data = pc_View->GetPcData().GetBusIndex();
      stw_scl::C_SCLChecksums::CalcCRC32(&q_Data, sizeof(q_Data), u32_Hash);
      stw_scl::C_SCLChecksums::CalcCRC32(&u32_Data, sizeof(u32_Data), u32_Hash);
   }
   else
   {
      u32_Hash = 0UL;
   }
   c_Retval.push_back(u32_Hash);
   u32_Hash = C_PuiSdHandler::h_GetInstance()->CalcHashSystemDefinition();
   c_Retval.push_back(u32_Hash);
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Check if diagnostic mode activated

   \param[in] ou32_ViewIndex View index
   \param[in] ou32_NodeIndex Node index

   \return
   True  Diagnostic mode activated
   False No diagnostic mode activated
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_TblTreDataElementModel::mh_CheckNodeDiagnostic(const uint32 ou32_ViewIndex, const uint32 ou32_NodeIndex)
{
   bool q_Retval = false;
   const C_SyvRoRouteCalculation c_RouteCalculation(ou32_ViewIndex, ou32_NodeIndex,
                                                    C_OSCRoutingCalculation::eDIAGNOSTIC);

   const sint32 s32_Retval = c_RouteCalculation.GetState();

   if (s32_Retval == C_NO_ERR)
   {
      q_Retval = true;
   }

   return q_Retval;
}
