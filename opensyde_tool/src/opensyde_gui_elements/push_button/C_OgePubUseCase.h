//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Button in material design of main view

   \implementation
   project     opensyde
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     06.07.2016  STW/B.Bayer
   \endimplementation
*/
//-----------------------------------------------------------------------------

#ifndef C_OGEPUBUSECASE_H
#define C_OGEPUBUSECASE_H

/* -- Includes ------------------------------------------------------------- */

#include "precomp_headers.h"
#include "C_OgePubStandard.h"
#include <QWidget>

/* -- Namespace ------------------------------------------------------------ */

namespace stw_opensyde_gui_elements
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_OgePubUseCase :
   public C_OgePubStandard
{
   Q_OBJECT

public:
   explicit C_OgePubUseCase(QWidget * const opc_Parent = NULL);

   void SetDisabledIcon(const QString & orc_Path);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void paintEvent(QPaintEvent * const opc_Event) override;
   //lint -restore

private:
   QString mc_DisabledIconPath;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
