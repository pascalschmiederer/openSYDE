//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Button for options (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_OGEPUBOPTIONS_H
#define C_OGEPUBOPTIONS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "C_OgePubToolTipBase.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_OgePubOptions :
   public C_OgePubToolTipBase
{
   Q_OBJECT

public:
   C_OgePubOptions(QWidget * const opc_Parent = NULL);
   void SetCustomIcon(const QString & orc_IconPathEnabled, const QString & orc_IconPathDisabled);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void paintEvent(QPaintEvent * const opc_Event) override;
   //lint -restore

   QIcon mc_IconEnabled;
   QIcon mc_IconDisabled;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
