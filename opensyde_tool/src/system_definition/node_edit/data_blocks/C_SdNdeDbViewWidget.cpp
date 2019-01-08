//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Widget for showing all node applications of a specific node (implementation)

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     30.01.2016 STW/B.Bayer
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "TGLUtils.h"
#include "stwerrors.h"
#include "C_UsHandler.h"
#include "C_GtGetText.h"
#include "C_PuiSdHandler.h"
#include "C_SdNdeDbAdd.h"
#include "C_SdNdeDbViewWidget.h"
#include "ui_C_SdNdeDbViewWidget.h"
#include "C_OgeWiCustomMessage.h"
#include "C_SdNdeDbAddNewProject.h"
#include "C_SdNdeDbProperties.h"
#include "C_SdNdeProgrammingOptions.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_tgl;
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_core;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;

/* -- Module Global Constants ---------------------------------------------- */
const QString C_SdNdeDbViewWidget::mhc_DefaultDataBlockName = "DataBlock";

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Default constructor

   \param[in,out] opc_Parent Optional pointer to parent

   \created     30.01.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
C_SdNdeDbViewWidget::C_SdNdeDbViewWidget(QWidget * const opc_Parent) :
   QWidget(opc_Parent),
   mpc_Ui(new Ui::C_SdNdeDbViewWidget),
   mu32_NodeIndex(0)
{
   this->mpc_Ui->setupUi(this);

   InitStaticNames();

   // init icons
   this->mpc_Ui->pc_PushButtonAdd->SetCustomIcons("://images/IconAddEnabled.svg", "://images/IconAddHovered.svg",
                                                  "://images/IconAddClicked.svg", "://images/IconAddDisabled.svg");
   this->mpc_Ui->pc_PushButtonCodeGenerationOptions->SetSvg("://images/system_definition/IconGenerateCodeSettings.svg",
                                                            "://images/system_definition/IconGenerateCodeSettings.svg");

   //Connects
   connect(this->mpc_Ui->pc_ListWidget, &C_SdNdeDbListWidget::SigDelete, this,
           &C_SdNdeDbViewWidget::m_OnDelete);
   connect(this->mpc_Ui->pc_ListWidget, &C_SdNdeDbListWidget::SigErrorChange, this,
           &C_SdNdeDbViewWidget::SigErrorChange);
   connect(this->mpc_Ui->pc_ListWidget, &C_SdNdeDbListWidget::SigDuplicate, this,
           &C_SdNdeDbViewWidget::m_OnDuplicate);
   connect(this->mpc_Ui->pc_ListWidget, &C_SdNdeDbListWidget::SigAppDisplay, this,
           &C_SdNdeDbViewWidget::m_OnAppDisplay);
   connect(this->mpc_Ui->pc_PushButtonAdd, &stw_opensyde_gui_elements::C_OgePubIconOnly::clicked, this,
           &C_SdNdeDbViewWidget::AddApp);
   connect(this->mpc_Ui->pc_ListWidget, &C_SdNdeDbListWidget::SigReloadDataPools, this,
           &C_SdNdeDbViewWidget::SigReloadDataPools);
   connect(this->mpc_Ui->pc_ListWidget, &C_SdNdeDbListWidget::SigCheckDataPoolInteraction, this,
           &C_SdNdeDbViewWidget::SigCheckDataPoolInteraction);
   connect(this->mpc_Ui->pc_ListWidget, &C_SdNdeDbListWidget::SigOpenDataPool, this,
           &C_SdNdeDbViewWidget::SigOpenDataPool);
   connect(this->mpc_Ui->pc_PushButtonCodeGenerationOptions, &QPushButton::clicked, this,
           &C_SdNdeDbViewWidget::m_ProgrammingOptions);
}

//-----------------------------------------------------------------------------
/*!
   \brief   default destructor

   Clean up.

   \created     30.01.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
C_SdNdeDbViewWidget::~C_SdNdeDbViewWidget()
{
   delete this->mpc_Ui;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Initialize all displayed static names

   \created     26.03.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::InitStaticNames(void) const
{
   //Tool tips
   this->mpc_Ui->pc_LabelApplicationName->SetToolTipInformation(C_GtGetText::h_GetText("Data Blocks"),
                                                                C_GtGetText::h_GetText(
                                                                   "Data Blocks specify files that must be updated to the node."
                                                                   "\n\n"
                                                                   "Types of Data Blocks:\n"
                                                                   " - Programmable Application \n"
                                                                   "   Application developed by using the openSYDE \"code generation\" feature.\n"
                                                                   "   Available for devices with programming support. \n\n"
                                                                   " - Binary\n"
                                                                   "   Any kind of application or data created by an other tool chain.\n\n"
                                                                   " - File Container \n"
                                                                   "   A file container for any number of files you want to transfer to your file based device.\n"
                                                                   "   Available for file based devices. \n"));

   this->mpc_Ui->pc_PushButtonAdd->SetToolTipInformation(C_GtGetText::h_GetText("Add"),
                                                         C_GtGetText::h_GetText("Add new Data Block."));

   this->mpc_Ui->pc_PushButtonCodeGenerationOptions->SetToolTipInformation(C_GtGetText::h_GetText("Code Generation Settings"),
                                                                           C_GtGetText::h_GetText(
                                                                              "Edit code generation settings."));
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set node index

   \param[in] ou32_NodeIndex Node index

   \created     15.12.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::SetNodeIndex(const stw_types::uint32 ou32_NodeIndex)
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(ou32_NodeIndex);

   this->mu32_NodeIndex = ou32_NodeIndex;
   tgl_assert(pc_Node != NULL);
   if (pc_Node != NULL)
   {
      this->mpc_Ui->pc_ListWidget->SetIndex(ou32_NodeIndex);

      this->mpc_Ui->pc_ListWidget->clear();
      for (uint32 u32_ItApp = 0; u32_ItApp < pc_Node->c_Applications.size(); ++u32_ItApp)
      {
         this->mpc_Ui->pc_ListWidget->AddApplication(ou32_NodeIndex, u32_ItApp);
      }
   }

   this->m_UpdateCount();
   this->m_HandleNoDatablocksLabel();
   this->m_HandleAddButtonAvailability();
   this->m_HandleCodeGenerationSettingsButtonAvailability();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Scrolls to the application with the index ou32_ApplicationIndex

   \param[in]     ou32_ApplicationIndex     Index of application

   \created     26.01.2018  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::ShowApplication(const uint32 ou32_ApplicationIndex) const
{
   this->mpc_Ui->pc_ListWidget->scrollToItem(
      this->mpc_Ui->pc_ListWidget->item(static_cast<sintn>(ou32_ApplicationIndex)));
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add a new application

   \created     15.12.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::AddApp(void)
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   tgl_assert(pc_Node != NULL);
   if (pc_Node != NULL)
   {
      C_OgeWiCustomMessage c_MessageBox(this);

      if (pc_Node->IsAnyUpdateAvailable() == true)
      {
         QPointer<C_OgePopUpDialog> const c_New = new C_OgePopUpDialog(this, this);
         const C_SdNdeDbAdd * const pc_Dialog = new C_SdNdeDbAdd(*c_New, this->mu32_NodeIndex);

         //Resize
         c_New->SetSize(QSize(710, 390));

         if (c_New->exec() == static_cast<sintn>(QDialog::Accepted))
         {
            if ((pc_Dialog->GetApplicationType() == C_OSCNodeApplication::eFILE_CONTAINER) ||
                (pc_Dialog->GetApplicationType() == C_OSCNodeApplication::eBINARY))
            {
               m_AddManualApplication(pc_Dialog->GetApplicationType());
            }
            else
            {
               if (pc_Dialog->GetFromTSP() == true)
               {
                  const uint32 u32_NumApplications = pc_Node->c_Applications.size();
                  //Clear all existing
                  for (uint32 u32_It = u32_NumApplications; u32_It > 0UL; --u32_It)
                  {
                     //Re-get application as our vector might not be valid anymore
                     const C_OSCNodeApplication * const pc_App = C_PuiSdHandler::h_GetInstance()->GetApplication(
                        this->mu32_NodeIndex,
                        u32_It -
                        1UL);
                     if ((pc_App != NULL) && (pc_App->e_Type == C_OSCNodeApplication::ePROGRAMMABLE_APPLICATION))
                     {
                        tgl_assert(C_PuiSdHandler::h_GetInstance()->RemoveApplication(this->mu32_NodeIndex,
                                                                                      u32_It - 1UL) == C_NO_ERR);
                     }
                  }
                  //First reload: update deleted data blocks
                  this->SetNodeIndex(this->mu32_NodeIndex);
                  m_AddFromTSP();
                  //Second/final reload: show data blocks as they were imported
                  this->SetNodeIndex(this->mu32_NodeIndex);
               }
               else
               {
                  m_AddManualApplication(C_OSCNodeApplication::ePROGRAMMABLE_APPLICATION);
               }
            }
         }

         if (c_New != NULL)
         {
            c_New->HideOverlay();
         }
         //lint -e{429}  no memory leak because of the parent of pc_Dialog and the Qt memory management
      }
      else
      {
         //no fbl support
         c_MessageBox.SetType(C_OgeWiCustomMessage::E_Type::eWARNING);
         c_MessageBox.SetHeading(C_GtGetText::h_GetText("Data blocks add"));
         c_MessageBox.SetDescription(C_GtGetText::h_GetText(
                                        "There is no Flashloader support for this device type. Data Blocks cannot be added."));
         c_MessageBox.Execute();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Update applications

   \created     03.04.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::UpdateApplications(void) const
{
   this->mpc_Ui->pc_ListWidget->UpdateApplications();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Show/hide no data blocks declared label


   \created     20.03.2018  STW/S.Singer
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_HandleNoDatablocksLabel(void) const
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   if ((pc_Node != NULL) && (pc_Node->IsAnyUpdateAvailable() == true))
   {
      //are there any data blocks?
      if (pc_Node->c_Applications.empty() == true)
      {
         //no datablocks display label
         this->mpc_Ui->pc_LabelNoDatablocks->setVisible(true);
         this->mpc_Ui->pc_LabelNoDatablocks->setText(C_GtGetText::h_GetText(
                                                        "No Data Block declared. You may add any via the '+' button"));
      }
      else
      {
         //data blocks present, no label required
         this->mpc_Ui->pc_LabelNoDatablocks->setVisible(false);
      }
   }
   else
   {
      this->mpc_Ui->pc_LabelNoDatablocks->setVisible(true);
      this->mpc_Ui->pc_LabelNoDatablocks->setText(C_GtGetText::h_GetText(
                                                     "No Flashloader support. Data Blocks cannot be added."));
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle add button availability

   \created     20.04.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_HandleAddButtonAvailability(void) const
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   if ((pc_Node != NULL) && (pc_Node->pc_DeviceDefinition != NULL))
   {
      bool q_Enabled = true;
      if (pc_Node->IsAnyUpdateAvailable() == true)
      {
         if (pc_Node->pc_DeviceDefinition->q_FlashloaderOpenSydeIsFileBased == true)
         {
            if (pc_Node->pc_DeviceDefinition->q_ProgrammingSupport == false)
            {
               if (pc_Node->c_Applications.size() > 0UL)
               {
                  q_Enabled = false;
               }
               else
               {
                  q_Enabled = true;
               }
            }
            else
            {
               q_Enabled = true;
            }
         }
         else
         {
            q_Enabled = true;
         }
      }
      else
      {
         q_Enabled = false;
      }
      this->mpc_Ui->pc_PushButtonAdd->setEnabled(q_Enabled);
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add new application

   \param[in] oe_Type Application type

   \return
   Application position (in node)

   \created     05.10.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
uint32 C_SdNdeDbViewWidget::m_AddApplication(const C_OSCNodeApplication::E_Type oe_Type) const
{
   C_OSCNodeApplication c_Appl;
   uint32 u32_Retval;

   // add datablock
   c_Appl.c_Name = "DataBlock";
   c_Appl.c_Comment = "";
   c_Appl.q_Active = true;
   c_Appl.e_Type = oe_Type;
   u32_Retval = m_AddApplication(c_Appl);

   return u32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add new application

   \param[in,out] orc_Application Application content

   \return
   Application position (in node)


   \created     05.10.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
uint32 C_SdNdeDbViewWidget::m_AddApplication(C_OSCNodeApplication & orc_Application) const
{
   uint32 u32_Retval = 0UL;
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   tgl_assert(pc_Node != NULL);
   if (pc_Node != NULL)
   {
      u32_Retval = pc_Node->c_Applications.size();
      tgl_assert(C_PuiSdHandler::h_GetInstance()->InsertApplication(this->mu32_NodeIndex, u32_Retval,
                                                                    orc_Application) == C_NO_ERR);
      //No reload required
      this->mpc_Ui->pc_ListWidget->AddApplication(this->mu32_NodeIndex, u32_Retval);

      this->m_UpdateCount();
      this->m_HandleAddButtonAvailability();
      this->m_HandleCodeGenerationSettingsButtonAvailability();
   }
   return u32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   On App Display

   \param[in] ou32_NodeIndex        Node index
   \param[in] ou32_ApplicationIndex Application index

   \created     15.12.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_OnAppDisplay() const
{
   this->m_UpdateCount();
   this->m_HandleNoDatablocksLabel();
   this->m_HandleAddButtonAvailability();
   this->m_HandleCodeGenerationSettingsButtonAvailability();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle delete

   \param[in] ou32_NodeIndex        Node index
   \param[in] ou32_ApplicationIndex Application index

   \created     15.12.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_OnDelete(const uint32 ou32_NodeIndex, const uint32 ou32_ApplicationIndex)
{
   tgl_assert(this->mu32_NodeIndex == ou32_NodeIndex);
   if (C_PuiSdHandler::h_GetInstance()->RemoveApplication(ou32_NodeIndex, ou32_ApplicationIndex) == C_NO_ERR)
   {
      //Trigger reload (also important for index update)
      this->SetNodeIndex(ou32_NodeIndex);
   }

   this->m_UpdateCount();
   this->m_HandleNoDatablocksLabel();
   this->m_HandleAddButtonAvailability();
   this->m_HandleCodeGenerationSettingsButtonAvailability();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle duplicate action

   \param[in] ou32_NodeIndex        Node index
   \param[in] ou32_ApplicationIndex Application index

   \created     28.02.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_OnDuplicate(const uint32 ou32_NodeIndex, const uint32 ou32_ApplicationIndex) const
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(ou32_NodeIndex);

   tgl_assert(pc_Node != NULL);
   if ((pc_Node != NULL) && (ou32_ApplicationIndex < pc_Node->c_Applications.size()))
   {
      const C_OSCDeviceDefinition * const pc_DevDef = pc_Node->pc_DeviceDefinition;
      C_OgeWiCustomMessage c_MessageBox(this->parentWidget());
      const uint32 u32_ApplicationIndex = pc_Node->c_Applications.size();
      C_OSCNodeApplication c_Appl = pc_Node->c_Applications[ou32_ApplicationIndex];

      //if file based, only one datablock is permitted
      if ((pc_DevDef->q_FlashloaderOpenSydeIsFileBased == true) && (pc_Node->c_Applications.empty() == false))
      {
         //if file based and there is already a datablock
         c_MessageBox.SetType(C_OgeWiCustomMessage::E_Type::eERROR);
         c_MessageBox.SetHeading(C_GtGetText::h_GetText("Data Block duplicate"));
         c_MessageBox.SetDescription(C_GtGetText::h_GetText("There is only one Data Block permitted on nodes with file "
                                                            "based flash access."));
         c_MessageBox.Execute();
      }
      else
      {
         //do it
         tgl_assert(C_PuiSdHandler::h_GetInstance()->InsertApplication(ou32_NodeIndex, u32_ApplicationIndex,
                                                                       c_Appl) == C_NO_ERR);
         //No reload required
         this->mpc_Ui->pc_ListWidget->AddApplication(ou32_NodeIndex, u32_ApplicationIndex);
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add new project action

   \created     02.10.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_AddFromTSP(void)
{
   QPointer<C_OgePopUpDialog> const c_New = new C_OgePopUpDialog(this, this);
   C_SdNdeDbAddNewProject * const pc_Dialog = new C_SdNdeDbAddNewProject(this->mu32_NodeIndex, *c_New);

   //Help
   //connect(pc_New, &C_OgePopUpDialog::SigHelp, pc_SettingsWidget, &C_GiSyLineWidget::HandleHelp);

   //Resize
   c_New->SetSize(QSize(800, 680));

   //init
   pc_Dialog->SetTSPPath(C_UsHandler::h_GetInstance()->GetProjSdTopologyLastKnownTSPPath());

   if (c_New->exec() == static_cast<sintn>(QDialog::Accepted))
   {
      C_OgeWiCustomMessage c_Message(this, C_OgeWiCustomMessage::eINFORMATION);
      c_Message.SetHeading(C_GtGetText::h_GetText("Add new Data Blocks"));
      c_Message.SetDescription(QString(C_GtGetText::h_GetText(
                                          "Successfully created %1 Data Blocks.")).arg(
                                  pc_Dialog->GetTSPApplicationCount()));
      for (uint32 u32_It = 0; u32_It < pc_Dialog->GetTSPApplicationCount(); ++u32_It)
      {
         C_OSCNodeApplication c_Tmp;
         c_Tmp.e_Type = C_OSCNodeApplication::ePROGRAMMABLE_APPLICATION;
         c_Tmp.q_Active = true;
         pc_Dialog->AddSelectedProject(u32_It, c_Tmp);
         m_AddApplication(c_Tmp);
      }
      //Last step
      pc_Dialog->HandleCodeGenerationConfig();
      c_Message.Execute();
   }

   if (c_New != NULL)
   {
      C_UsHandler::h_GetInstance()->SetProjSdTopologyLastKnownTSPPath(pc_Dialog->GetTSPPath());
      c_New->HideOverlay();
   }
   //lint -e{429}  no memory leak because of the parent of pc_Dialog and the Qt memory management
}

//-----------------------------------------------------------------------------
/*!
   \brief   Update data block count

   \created     15.10.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_UpdateCount(void) const
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   tgl_assert(pc_Node != NULL);
   if (pc_Node != NULL)
   {
      this->mpc_Ui->pc_LabelApplicationName->setText(QString("Data Blocks (%1)").arg(pc_Node->c_Applications.size()));
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Slot of programming options button

   \created     08.10.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_ProgrammingOptions(void) const
{
   const std::vector<const C_OSCNodeApplication *> c_ProgrammableApplications =
      C_PuiSdHandler::h_GetInstance()->GetProgrammableApplications(this->mu32_NodeIndex);

   if (c_ProgrammableApplications.size() > 0)
   {
      //Set parent for better hierarchy handling via window manager
      QPointer<C_OgePopUpDialog> c_New = new C_OgePopUpDialog(this->parentWidget(), this->parentWidget());
      QPointer<const C_SdNdeProgrammingOptions> c_Dialog = new C_SdNdeProgrammingOptions(*c_New, this->mu32_NodeIndex);

      //Help
      //connect(pc_New, &C_OgePopUpDialog::SigHelp, pc_SettingsWidget, &C_GiSyLineWidget::HandleHelp);

      //Resize
      c_New->SetSize(QSize(810, 690));

      if (c_New->exec() == static_cast<sintn>(QDialog::Accepted))
      {
         if (c_Dialog.isNull() == false)
         {
            c_Dialog->Save();
         }
      }
      //Hide overlay after dialog is not relevant anymore
      if (c_New.isNull() == false)
      {
         c_New->HideOverlay();
      }
      //lint -e{429}  no memory leak because of the parent of pc_New and pc_Dialog and the Qt memory management
   }
   else
   {
      C_OgeWiCustomMessage c_Message(this->parentWidget(), C_OgeWiCustomMessage::E_Type::eWARNING);
      c_Message.SetHeading(C_GtGetText::h_GetText("Code generation settings"));
      c_Message.SetDescription(C_GtGetText::h_GetText(
                                  "Code generation settings are not available. "
                                  "\nThere are no Data Blocks of type programmable applications declared."));
      c_Message.Execute();
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add new manual application

   \param[in] oe_Type Application type to create

   \created     05.10.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_AddManualApplication(const C_OSCNodeApplication::E_Type oe_Type)
{
   C_OSCNodeApplication c_Tmp;

   QPointer<C_OgePopUpDialog> const c_New = new C_OgePopUpDialog(this, this);
   C_SdNdeDbProperties * const pc_Dialog = new C_SdNdeDbProperties(this->mu32_NodeIndex, -1, *c_New);

   //Resize (200 -> use minimum possible height automatically)
   if (oe_Type != C_OSCNodeApplication::ePROGRAMMABLE_APPLICATION)
   {
      c_New->SetSize(C_SdNdeDbProperties::h_GetBinaryWindowSize());
   }
   else
   {
      c_New->SetSize(C_SdNdeDbProperties::h_GetDefaultWindowSize());
   }

   //default
   c_Tmp.c_Name = C_PuiSdHandler::h_GetInstance()->GetUniqueApplicationName(this->mu32_NodeIndex,
                                                                            C_SdNdeDbViewWidget::mhc_DefaultDataBlockName.toStdString().c_str());
   c_Tmp.u8_ProcessId = C_PuiSdHandler::h_GetInstance()->GetUniqueApplicationProcessId(this->mu32_NodeIndex, 0);
   c_Tmp.e_Type = oe_Type;
   c_Tmp.q_Active = true;

   //init
   pc_Dialog->LoadFromData(c_Tmp);

   if (c_New->exec() == static_cast<sintn>(QDialog::Accepted))
   {
      uint32 u32_Index;
      pc_Dialog->ApplyNewData(c_Tmp);
      u32_Index = m_AddApplication(c_Tmp);
      pc_Dialog->HandleDataPools(u32_Index);
      //Reload data pool section
      this->UpdateApplications();
   }

   if (c_New != NULL)
   {
      c_New->HideOverlay();
   }
   //lint -e{429}  no memory leak because of the parent of pc_Dialog and the Qt memory management
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle visibility of button "Code generation settings"

   \created     08.10.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdNdeDbViewWidget::m_HandleCodeGenerationSettingsButtonAvailability(void) const
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   tgl_assert(pc_Node != NULL);
   if (pc_Node != NULL)
   {
      tgl_assert(pc_Node->pc_DeviceDefinition != NULL);
      if (pc_Node->pc_DeviceDefinition != NULL)
      {
         if ((pc_Node->pc_DeviceDefinition->q_ProgrammingSupport == true) &&
             (C_PuiSdHandler::h_GetInstance()->GetProgrammableApplications(this->mu32_NodeIndex).size() > 0UL))
         {
            this->mpc_Ui->pc_PushButtonCodeGenerationOptions->setVisible(true);
         }
         else
         {
            this->mpc_Ui->pc_PushButtonCodeGenerationOptions->setVisible(false);
         }
      }
   }
}
