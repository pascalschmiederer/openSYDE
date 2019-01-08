//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for update PieChart (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     22.02.2018  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SYVUPPIECHART_H
#define C_SYVUPPIECHART_H

/* -- Includes ------------------------------------------------------------- */

#include <QWidget>
#include <QMovie>
#include "stwtypes.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_SyvUpPieChart :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SyvUpPieChart(QWidget * const opc_Parent = NULL);
   ~C_SyvUpPieChart(void);

   void SetProgress100(const stw_types::uint16 ou16_Progress100, const bool oq_Finished = false);

   void SetActiveColor(const QColor & orc_Value);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void paintEvent(QPaintEvent * const opc_Event) override;
   //lint -restore

private:
   static const QString mhc_FilePath;
   stw_types::uint16 mu16_ValueInPercent;
   QColor mc_ActiveColor;
   bool mq_FinishedAnimation;
   QMovie * mpc_Movie;

   void m_OnFrameChanged(void);
   //Avoid call
   C_SyvUpPieChart(const C_SyvUpPieChart &);
   C_SyvUpPieChart & operator =(const C_SyvUpPieChart &);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
