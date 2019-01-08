//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for complete lists handling (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     23.02.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_SDNDEDATAPOOLLISTSWIDGET_H
#define C_SDNDEDATAPOOLLISTSWIDGET_H

/* -- Includes ------------------------------------------------------------- */

#include <QWidget>
#include "stwtypes.h"

/* -- Namespace ------------------------------------------------------------ */

namespace Ui
{
class C_SdNdeDataPoolListsWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */
class C_SdNdeDataPoolListsWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeDataPoolListsWidget(QWidget * const opc_Parent = NULL);
   ~C_SdNdeDataPoolListsWidget(void);

   void InitText(void) const;

   void Clear(void) const;
   void SetDataPool(const stw_types::uint32 & oru32_NodeIndex, const stw_types::uint32 & oru32_DataPoolIndex);
   void OpenDetail(const stw_types::sint32 os32_ListIndex, const stw_types::sint32 os32_DataElementIndex) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigErrorChange(const bool & orq_Error);
   void SigSizeChange(void);
   void SigChanged(void);
   void SigSave(void);
   void SigSaveAs(void);

private:
   //Avoid call
   C_SdNdeDataPoolListsWidget(const C_SdNdeDataPoolListsWidget &);
   C_SdNdeDataPoolListsWidget & operator =(const C_SdNdeDataPoolListsWidget &);

   Ui::C_SdNdeDataPoolListsWidget * mpc_Ui;
   stw_types::uint32 mu32_NodeIndex;
   stw_types::uint32 mu32_DataPoolIndex;
   stw_types::uint32 mu32_LastKnownListSelectionCount;
   stw_types::uint32 mu32_LastKnownTableSelectionCount;

   void m_InitButtonIcons(void) const;
   void m_HandleErrorChange(const bool & orq_Error);
   void m_HandleSizeChange(void);
   void m_HandleSelection(const stw_types::uint32 & oru32_Count, const bool & orq_List);
   void m_HandleButtonChange(const bool & orq_AddActive, const bool & orq_CutActive, const bool & orq_CopyActive,
                             const bool & orq_PasteActive, const bool & orq_DeleteActive,
                             const bool & orq_MoveDownActive, const bool & orq_MoveUpActive) const;
   void m_HandleChanged(void);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif
