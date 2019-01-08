//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Load/save system definition data from/to file

   Load/save system definition data from/to XML file

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     02.09.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include <cstdio>
#include "stwtypes.h"
#include "stwerrors.h"
#include "C_OSCSystemDefinitionFiler.h"
#include "TGLFile.h"
#include "TGLUtils.h"
#include "C_OSCLoggingHandler.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_opensyde_core;
using namespace stw_types;
using namespace stw_errors;
using namespace stw_tgl;
using namespace stw_scl;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Load system definition

   Steps:
   * Load device definitions (if not already loaded)
   * Load system definition
   * for each node set a pointer to the used device definition

   \param[out]    orc_SystemDefinition       Pointer to storage
   \param[in]     orc_PathSystemDefinition   Path to system definition
   \param[in]     orc_PathDeviceDefinitions  Path to device definition description file

   \return
   C_NO_ERR    data read
   C_RANGE     specified system definition file does not exist
   C_NOACT     specified file is present but structure is invalid (e.g. invalid XML file)
   C_CONFIG    system definition file content is invalid or incomplete
               device definition file could not be loaded
   C_OVERFLOW  node in system definition references a device not part of the device definitions

   \created     01.09.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_OSCSystemDefinitionFiler::h_LoadSystemDefinitionFile(C_OSCSystemDefinition & orc_SystemDefinition,
                                                              const stw_scl::C_SCLString & orc_PathSystemDefinition,
                                                              const stw_scl::C_SCLString & orc_PathDeviceDefinitions)
{
   sint32 s32_Retval = C_NO_ERR;

   if (TGL_FileExists(orc_PathSystemDefinition) == true)
   {
      C_OSCXMLParser c_XMLParser;
      s32_Retval = c_XMLParser.LoadFromFile(orc_PathSystemDefinition);
      if (s32_Retval == C_NO_ERR)
      {
         s32_Retval = mh_LoadSystemDefinition(orc_SystemDefinition, c_XMLParser, orc_PathDeviceDefinitions);
      }
      else
      {
         osc_write_log_error("Loading System Definition",
                             "File \"" + orc_PathSystemDefinition + "\" could not be opened.");
         s32_Retval = C_NOACT;
      }
   }
   else
   {
      osc_write_log_error("Loading System Definition", "File \"" + orc_PathSystemDefinition + "\" does not exist.");
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Save system definition

   Save system definition
   Will overwrite the file if it already exists.
   Does NOT write the device definition file(s)

   \param[in]   orc_SystemDefinition Pointer to storage
   \param[in]   orc_Path             Path of system definition

   \return
   C_NO_ERR   data saved
   C_RD_WR    could not erase pre-existing file before saving
   C_RD_WR    could not write to file (e.g. missing write permissions; missing folder)

   \created     01.09.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_OSCSystemDefinitionFiler::h_SaveSystemDefinitionFile(const C_OSCSystemDefinition & orc_SystemDefinition,
                                                              const stw_scl::C_SCLString & orc_Path)
{
   sint32 s32_Return = C_NO_ERR;

   if (TGL_FileExists(orc_Path) == true)
   {
      //erase it:
      sintn sn_Return;
      sn_Return = std::remove(orc_Path.c_str());
      if (sn_Return != 0)
      {
         osc_write_log_error("Saving System Definition", "Could not erase pre-existing file \"" + orc_Path + "\".");
         s32_Return = C_RD_WR;
      }
   }
   if (s32_Return == C_NO_ERR)
   {
      C_OSCXMLParser c_XMLParser;
      mh_SaveSystemDefinition(orc_SystemDefinition, c_XMLParser);

      s32_Return = c_XMLParser.SaveToFile(orc_Path);
      if (s32_Return != C_NO_ERR)
      {
         osc_write_log_error("Saving System Definition", "Could not write to file \"" + orc_Path + "\".");
         s32_Return = C_RD_WR;
      }
   }
   return s32_Return;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Load system definition

   * Load device definitions from files (if not already loaded)
   * Load system definition from string
   * for each node set a pointer to the used device definition

   \param[out]    orc_SystemDefinition       Pointer to storage
   \param[in]     orc_Content                XML Content string
   \param[in]     orc_PathDeviceDefinitions  Path to device definition description file

   \return
   C_NO_ERR   data read
   C_CONFIG   content is invalid or incomplete
   C_OVERFLOW  node in system definition references a device not part of the device definitions

   \created     01.09.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_OSCSystemDefinitionFiler::h_LoadSystemDefinitionString(C_OSCSystemDefinition & orc_SystemDefinition,
                                                                const C_SCLString & orc_Content,
                                                                const stw_scl::C_SCLString & orc_PathDeviceDefinitions)
{
   sint32 s32_Retval;
   //Set up parser
   C_OSCXMLParserString c_XMLParser;

   s32_Retval = c_XMLParser.LoadFromString(orc_Content);
   if (s32_Retval == C_NO_ERR)
   {
      s32_Retval = mh_LoadSystemDefinition(orc_SystemDefinition, c_XMLParser, orc_PathDeviceDefinitions);
   }
   else
   {
      s32_Retval = C_CONFIG;
   }

   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Save system definition

   Save system definition
   Does NOT write the device definition file(s)

   \param[in]     orc_SystemDefinition Pointer to storage
   \param[out]    orc_Content          XML Content string

   \created     01.09.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OSCSystemDefinitionFiler::h_SaveSystemDefinitionString(const C_OSCSystemDefinition & orc_SystemDefinition,
                                                              C_SCLString & orc_Content)
{
   //Set up parser
   C_OSCXMLParserString c_XMLParser;

   mh_SaveSystemDefinition(orc_SystemDefinition, c_XMLParser);
   c_XMLParser.SaveToString(orc_Content);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Load nodes

   Load nodes data.
   * load node data and add to system definition
   * for each node set a pointer to the used device definition

    The caller is responsible to provide a static life-time of orc_DeviceDefinitions.
    Otherwise the "device definition" pointers in C_OSCNode will point to invalid data.

   \param[in]     ou16_XmlFormatVersion  version of XML format
   \param[out]    orc_Nodes              data storage
   \param[in,out] orc_XMLParser          XML with "nodes" active
   \param[in]     orc_DeviceDefinitions  List of known devices (must contain all device types used by nodes)

   \return
   C_NO_ERR    no error
   C_CONFIG    content is invalid or incomplete
   C_OVERFLOW  node in system definition references a device not part of the device definitions

   \created     09.05.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_OSCSystemDefinitionFiler::h_LoadNodes(const uint16 ou16_XmlFormatVersion, std::vector<C_OSCNode> & orc_Nodes,
                                               C_OSCXMLParserBase & orc_XMLParser,
                                               const C_OSCDeviceManager & orc_DeviceDefinitions)

{
   sint32 s32_Retval = C_NO_ERR;
   C_SCLString c_SelectedNode = orc_XMLParser.SelectNodeChild("node");

   //clear list of nodes:
   orc_Nodes.clear();

   if (c_SelectedNode == "node")
   {
      do
      {
         C_OSCNode c_Item;
         s32_Retval = C_OSCNodeFiler::h_LoadNode(ou16_XmlFormatVersion, c_Item, orc_XMLParser);
         if (s32_Retval != C_NO_ERR)
         {
            break;
         }
         orc_Nodes.push_back(c_Item);
         //Next
         c_SelectedNode = orc_XMLParser.SelectNodeNext("node");
      }
      while (c_SelectedNode == "node");
      if (s32_Retval == C_NO_ERR)
      {
         //Return
         tgl_assert(orc_XMLParser.SelectNodeParent() == "nodes");
      }
   }

   if (s32_Retval == C_NO_ERR)
   {
      //set pointers to device definitions
      for (uint32 u32_NodeIndex = 0U; u32_NodeIndex < orc_Nodes.size(); u32_NodeIndex++)
      {
         const C_OSCDeviceDefinition * const pc_Device =
            orc_DeviceDefinitions.LookForDevice(orc_Nodes[u32_NodeIndex].c_DeviceType);
         if (pc_Device == NULL)
         {
            s32_Retval = C_OVERFLOW;
            osc_write_log_error("Loading System Definition",
                                "System Definition contains node \"" + orc_Nodes[u32_NodeIndex].c_Properties.c_Name +
                                "\" of device type \"" +
                                orc_Nodes[u32_NodeIndex].c_DeviceType + "\" which is not a known device.");
            break;
         }
         else
         {
            orc_Nodes[u32_NodeIndex].pc_DeviceDefinition = pc_Device;
         }
      }
   }

   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Load buses

   Load buses data.
   The bus data will be loaded and the bus will be added.

   \param[in,out] orc_Buses     data storage
   \param[in,out] orc_XMLParser XML with "buses" active

   \return
   C_NO_ERR   no error
   C_CONFIG   content is invalid or incomplete

   \created     09.05.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_OSCSystemDefinitionFiler::h_LoadBuses(std::vector<C_OSCSystemBus> & orc_Buses,
                                               C_OSCXMLParserBase & orc_XMLParser)
{
   sint32 s32_Retval = C_NO_ERR;
   C_SCLString c_SelectedNode = orc_XMLParser.SelectNodeChild("bus");

   orc_Buses.clear();
   if (c_SelectedNode == "bus")
   {
      do
      {
         C_OSCSystemBus c_Item;
         s32_Retval = C_OSCSystemBusFiler::h_LoadBus(c_Item, orc_XMLParser);
         if (s32_Retval == C_NO_ERR)
         {
            orc_Buses.push_back(c_Item);
         }

         //Next
         c_SelectedNode = orc_XMLParser.SelectNodeNext("bus");
      }
      while (c_SelectedNode == "bus");
      //Return
      tgl_assert(orc_XMLParser.SelectNodeParent() == "buses");
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Save nodes

   Save nodes data.
   The node data will be saved and the node will be added.

   \param[in]     orc_Nodes     data storage
   \param[in,out] orc_XMLParser XML with "nodes" active

   \return
   C_NO_ERR   no error
   C_CONFIG   content is invalid or incomplete

   \created     09.05.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OSCSystemDefinitionFiler::h_SaveNodes(const std::vector<C_OSCNode> & orc_Nodes,
                                             C_OSCXMLParserBase & orc_XMLParser)
{
   for (uint32 u32_Index = 0U; u32_Index < orc_Nodes.size(); u32_Index++)
   {
      tgl_assert(orc_XMLParser.CreateAndSelectNodeChild("node") == "node");
      C_OSCNodeFiler::h_SaveNode(orc_Nodes[u32_Index], orc_XMLParser);
      //Return
      tgl_assert(orc_XMLParser.SelectNodeParent() == "nodes");
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Save buses

   Save buses data.
   The bus data will be saved and the bus will be added to the system definition (sorted).

   \param[in]     orc_Buses     data storage
   \param[in,out] orc_XMLParser XML with "buses" active

   \return
   C_NO_ERR   no error
   C_CONFIG   content is invalid or incomplete

   \created     09.05.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OSCSystemDefinitionFiler::h_SaveBuses(const std::vector<C_OSCSystemBus> & orc_Buses,
                                             C_OSCXMLParserBase & orc_XMLParser)
{
   for (uint32 u32_Index = 0U; u32_Index < orc_Buses.size(); u32_Index++)
   {
      tgl_assert(orc_XMLParser.CreateAndSelectNodeChild("bus") == "bus");
      C_OSCSystemBusFiler::h_SaveBus(orc_Buses[u32_Index], orc_XMLParser);
      //Return
      tgl_assert(orc_XMLParser.SelectNodeParent() == "buses");
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Load system definition

   Steps:
   * Load device definitions (if not already loaded)
   * Load system definition
   * for each node set a pointer to the used device definition

   \param[out]    orc_SystemDefinition   Pointer to storage
   \param[in,out] orc_XMLParser          XML with default state
   \param[in]     orc_PathDeviceDefinitions  Path to device definition description file

   \return
   C_NO_ERR    data read
   C_CONFIG    system definition content is invalid or incomplete
               device definition could not be loaded
   C_OVERFLOW  node in system definition references a device not part of the device definitions

   \created     01.09.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_OSCSystemDefinitionFiler::mh_LoadSystemDefinition(C_OSCSystemDefinition & orc_SystemDefinition,
                                                           C_OSCXMLParserBase & orc_XMLParser,
                                                           const stw_scl::C_SCLString & orc_PathDeviceDefinitions)
{
   sint32 s32_Retval = C_NO_ERR;
   uint16 u16_FileVersion = 0U;

   //do we need to load the device definitions ?
   if (C_OSCSystemDefinition::hc_Devices.WasLoaded() == false)
   {
      s32_Retval = C_OSCSystemDefinition::hc_Devices.LoadFromFile(orc_PathDeviceDefinitions);
      if (s32_Retval != C_NO_ERR)
      {
         osc_write_log_error("Loading System Definition", "Could not load Device definitions.");
         s32_Retval = C_CONFIG;
      }
   }

   if (orc_XMLParser.SelectRoot() == "opensyde-system-definition")
   {
      //File version
      if (orc_XMLParser.SelectNodeChild("file-version") == "file-version")
      {
         try
         {
            u16_FileVersion = static_cast<uint16>(orc_XMLParser.GetNodeContent().ToInt());
         }
         catch (...)
         {
            osc_write_log_error("Loading System Definition", "\"file-version\" could not be converted to a number.");
            s32_Retval = C_CONFIG;
         }

         //is the file version one we know ?
         if (s32_Retval == C_NO_ERR)
         {
            osc_write_log_info("Loading System Definition", "Value of \"file-version\": " +
                               C_SCLString::IntToStr(u16_FileVersion));
            if ((u16_FileVersion != hu16_FILE_VERSION_1) && (u16_FileVersion != hu16_FILE_VERSION_2))
            {
               osc_write_log_error("Loading System Definition",
                                   "Version defined by \"file-version\" is not supported.");
               s32_Retval = C_CONFIG;
            }
         }

         //Return
         tgl_assert(orc_XMLParser.SelectNodeParent() == "opensyde-system-definition");
      }
      else
      {
         osc_write_log_error("Loading System Definition", "Could not find \"file-version\" node.");
         s32_Retval = C_CONFIG;
      }
      //Node
      orc_SystemDefinition.c_Nodes.clear();
      if (s32_Retval == C_NO_ERR)
      {
         if (orc_XMLParser.SelectNodeChild("nodes") == "nodes")
         {
            s32_Retval = h_LoadNodes(u16_FileVersion, orc_SystemDefinition.c_Nodes, orc_XMLParser,
                                     C_OSCSystemDefinition::hc_Devices);
            if (s32_Retval == C_NO_ERR)
            {
               //Return
               tgl_assert(orc_XMLParser.SelectNodeParent() == "opensyde-system-definition");
            }
         }
         else
         {
            osc_write_log_error("Loading System Definition", "Could not find \"nodes\" node.");
            s32_Retval = C_CONFIG;
         }
      }

      //Bus
      orc_SystemDefinition.c_Buses.clear();
      if (s32_Retval == C_NO_ERR)
      {
         if (orc_XMLParser.SelectNodeChild("buses") == "buses")
         {
            s32_Retval = h_LoadBuses(orc_SystemDefinition.c_Buses, orc_XMLParser);
            if (s32_Retval == C_NO_ERR)
            {
               //Return
               tgl_assert(orc_XMLParser.SelectNodeParent() == "opensyde-system-definition");
            }
         }
         else
         {
            osc_write_log_error("Loading System Definition", "Could not find \"buses\" node.");
            s32_Retval = C_CONFIG;
         }
      }
   }
   else
   {
      osc_write_log_error("Loading System Definition", "Could not find \"opensyde-system-definition\" node.");
      s32_Retval = C_CONFIG;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Save system definition

   Save system definition
   Does NOT write the device definition file(s)

   \param[in]     orc_SystemDefinition Pointer to storage
   \param[in,out] orc_XMLParser        XML with default state

   \created     01.09.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OSCSystemDefinitionFiler::mh_SaveSystemDefinition(const C_OSCSystemDefinition & orc_SystemDefinition,
                                                         C_OSCXMLParserBase & orc_XMLParser)
{
   orc_XMLParser.CreateNodeChild("opensyde-system-definition");
   tgl_assert(orc_XMLParser.SelectRoot() == "opensyde-system-definition");
   //File version
   tgl_assert(orc_XMLParser.CreateAndSelectNodeChild("file-version") == "file-version");
   orc_XMLParser.SetNodeContent(C_SCLString::IntToStr(hu16_FILE_VERSION_LATEST));
   //Return
   tgl_assert(orc_XMLParser.SelectNodeParent() == "opensyde-system-definition");
   //Node
   tgl_assert(orc_XMLParser.CreateAndSelectNodeChild("nodes") == "nodes");
   h_SaveNodes(orc_SystemDefinition.c_Nodes, orc_XMLParser);
   //Return
   tgl_assert(orc_XMLParser.SelectNodeParent() == "opensyde-system-definition");

   //Bus
   tgl_assert(orc_XMLParser.CreateAndSelectNodeChild("buses") == "buses");
   h_SaveBuses(orc_SystemDefinition.c_Buses, orc_XMLParser);
   //Return
   tgl_assert(orc_XMLParser.SelectNodeParent() == "opensyde-system-definition");
}
