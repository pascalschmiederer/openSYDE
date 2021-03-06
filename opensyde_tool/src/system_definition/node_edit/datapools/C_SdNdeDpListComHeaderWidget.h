//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget to display one data pool list com entry (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDEDPLISTCOMHEADERWIDGET_H
#define C_SDNDEDPLISTCOMHEADERWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QIcon>
#include <QWidget>
#include "stwtypes.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */

namespace Ui
{
class C_SdNdeDpListComHeaderWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeDpListComHeaderWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeDpListComHeaderWidget(const stw_types::uint32 ou32_NodeIndex,
                                         const stw_types::uint32 ou32_DataPoolIndex,
                                         const stw_types::uint32 ou32_ListIndex, const bool & orq_UseAlternatingColor,
                                         QWidget * const opc_Parent = NULL);
   ~C_SdNdeDpListComHeaderWidget(void);
   void InitStaticNames(void);
   bool GetErrorActive(void) const;
   void UpdateMessagesAndSignals(void) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigEdit(const stw_types::uint32 ou32_DataPoolIndex, const stw_types::uint32 ou32_ListIndex);
   void SigSwitchToBus(const stw_types::uint32 & oru32_BusIndex, const QString & orc_BusName);
   void SigErrorChange(void) const;

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void paintEvent(QPaintEvent * const opc_Event) override;
   //lint -restore

private:
   //Avoid call
   C_SdNdeDpListComHeaderWidget(const C_SdNdeDpListComHeaderWidget &);
   C_SdNdeDpListComHeaderWidget & operator =(const C_SdNdeDpListComHeaderWidget &);

   Ui::C_SdNdeDpListComHeaderWidget * const mpc_Ui;
   const stw_types::uint32 mu32_NodeIndex;
   const stw_types::uint32 mu32_DataPoolIndex;
   const stw_types::uint32 mu32_ListIndex;
   bool mq_Connected;

   void m_UpdateBusLink(void);
   void m_OnLinkEdit(void);
   void m_OnLinkSwitchToBus(void);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
