//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Common scene elements (implementation)

   Common scene elements

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     20.07.2017  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "stwerrors.h"
#include "TGLUtils.h"
#include "C_PuiBsElements.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_tgl;
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Default constructor

   \created     20.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_PuiBsElements::C_PuiBsElements(void)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Assignment operator

   \param[in] orc_Snapshot Instance to copy

   \return
   Instance with new values

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_PuiBsElements & C_PuiBsElements::operator =(const C_PuiBsElements & orc_Snapshot)
{
   if (&orc_Snapshot != this)
   {
      this->c_Boundaries = orc_Snapshot.c_Boundaries;
      this->c_Images = orc_Snapshot.c_Images;
      this->c_LineArrows = orc_Snapshot.c_LineArrows;
      this->c_TextElements = orc_Snapshot.c_TextElements;
   }
   return *this;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Default destructor

   Clean up.

   \created     24.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_PuiBsElements::~C_PuiBsElements(void)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Clear elements

   \created     20.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_PuiBsElements::Clear(void)
{
   this->c_Boundaries.clear();
   this->c_Images.clear();
   this->c_LineArrows.clear();
   this->c_TextElements.clear();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Replace common items of snapshot

   \param[in,out] orc_Snapshot Snapshot to replace

   \created     20.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_PuiBsElements::ReplaceSnapshotElements(C_PuiBsElements & orc_Snapshot) const
{
   orc_Snapshot.c_Boundaries = this->c_Boundaries;
   orc_Snapshot.c_Images = this->c_Images;
   orc_Snapshot.c_LineArrows = this->c_LineArrows;
   orc_Snapshot.c_TextElements = this->c_TextElements;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Calculates the hash value over all data

   The hash value is a 32 bit CRC value.

   \param[in,out] oru32_HashValue Hash value with init [in] value and result [out] value

   \created     20.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_PuiBsElements::CalcHash(uint32 & oru32_HashValue) const
{
   for (uint32 u32_Counter = 0U; u32_Counter < this->c_Boundaries.size(); ++u32_Counter)
   {
      this->c_Boundaries[u32_Counter].CalcHash(oru32_HashValue);
   }

   for (uint32 u32_Counter = 0U; u32_Counter < this->c_Images.size(); ++u32_Counter)
   {
      this->c_Images[u32_Counter].CalcHash(oru32_HashValue);
   }

   for (uint32 u32_Counter = 0U; u32_Counter < this->c_LineArrows.size(); ++u32_Counter)
   {
      this->c_LineArrows[u32_Counter].CalcHash(oru32_HashValue);
   }

   for (uint32 u32_Counter = 0U; u32_Counter < this->c_TextElements.size(); ++u32_Counter)
   {
      this->c_TextElements[u32_Counter].CalcHash(oru32_HashValue);
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Count sum of all items

   \created     24.11.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
stw_types::uint32 C_PuiBsElements::Count(void) const
{
   uint32 u32_Retval = 0;

   u32_Retval += this->c_Boundaries.size();
   u32_Retval += this->c_Images.size();
   u32_Retval += this->c_LineArrows.size();
   u32_Retval += this->c_TextElements.size();
   return u32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add new boundary

   \param[in] orc_Data New value

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_PuiBsElements::AddBoundary(const C_PuiBsBoundary & orc_Data)
{
   tgl_assert(this->InsertBoundary(this->c_Boundaries.size(), orc_Data) == C_NO_ERR);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Insert new boundary

   \param[in] oru32_Index Boundary index
   \param[in] orc_Data    New value

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::InsertBoundary(const uint32 & oru32_Index, const C_PuiBsBoundary & orc_Data)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index <= this->c_Boundaries.size())
   {
      this->c_Boundaries.insert(this->c_Boundaries.begin() + oru32_Index, orc_Data);
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add new image

   \param[in] orc_Data New value

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_PuiBsElements::AddImage(const C_PuiBsImage & orc_Data)
{
   tgl_assert(this->InsertImage(this->c_Images.size(), orc_Data) == C_NO_ERR);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Insert new image

   \param[in] oru32_Index Image index
   \param[in] orc_Data    New value

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::InsertImage(const uint32 & oru32_Index, const C_PuiBsImage & orc_Data)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index <= this->c_Images.size())
   {
      this->c_Images.insert(this->c_Images.begin() + oru32_Index, orc_Data);
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add new line arrow

   \param[in] orc_Data New value

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_PuiBsElements::AddLineArrow(const C_PuiBsLineArrow & orc_Data)
{
   tgl_assert(this->InsertLineArrow(this->c_LineArrows.size(), orc_Data) == C_NO_ERR);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Insert new line arrow

   \param[in] oru32_Index Line arrow index
   \param[in] orc_Data    New value

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::InsertLineArrow(const uint32 & oru32_Index, const C_PuiBsLineArrow & orc_Data)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index <= this->c_LineArrows.size())
   {
      this->c_LineArrows.insert(this->c_LineArrows.begin() + oru32_Index, orc_Data);
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Add new text element

   \param[in] orc_Data New value

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_PuiBsElements::AddTextElement(const C_PuiBsTextElement & orc_Data)
{
   tgl_assert(this->InsertTextElement(this->c_TextElements.size(), orc_Data) == C_NO_ERR);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Insert new text element

   \param[in] oru32_Index Text element index
   \param[in] orc_Data    New value

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::InsertTextElement(const uint32 & oru32_Index, const C_PuiBsTextElement & orc_Data)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index <= this->c_TextElements.size())
   {
      this->c_TextElements.insert(this->c_TextElements.begin() + oru32_Index, orc_Data);
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Delete boundary

   \param[in] oru32_Index Boundary index

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::DeleteBoundary(const uint32 & oru32_Index)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index < this->c_Boundaries.size())
   {
      this->c_Boundaries.erase(this->c_Boundaries.begin() + oru32_Index);
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Delete image

   \param[in] oru32_Index Image index

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::DeleteImage(const uint32 & oru32_Index)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index < this->c_Images.size())
   {
      this->c_Images.erase(this->c_Images.begin() + oru32_Index);
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Delete line arrow

   \param[in] oru32_Index Line arrow index

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::DeleteLineArrow(const uint32 & oru32_Index)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index < this->c_LineArrows.size())
   {
      this->c_LineArrows.erase(this->c_LineArrows.begin() + oru32_Index);
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Delete text element

   \param[in] oru32_Index Text element index

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::DeleteTextElement(const uint32 & oru32_Index)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index < this->c_TextElements.size())
   {
      this->c_TextElements.erase(this->c_TextElements.begin() + oru32_Index);
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get boundary

   \param[in] oru32_Index Boundary index

   \return
   NULL Boundary not found
   Else Valid boundary

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
const C_PuiBsBoundary * C_PuiBsElements::GetBoundary(const uint32 & oru32_Index) const
{
   const C_PuiBsBoundary * pc_Retval = NULL;

   if (oru32_Index < this->c_Boundaries.size())
   {
      pc_Retval = &this->c_Boundaries[oru32_Index];
   }
   return pc_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set boundary

   \param[in] oru32_Index Boundary index
   \param[in] orc_Data    New value

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::SetBoundary(const uint32 & oru32_Index, const C_PuiBsBoundary & orc_Data)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index < this->c_Boundaries.size())
   {
      this->c_Boundaries[oru32_Index] = orc_Data;
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get image

   \param[in] oru32_Index Image index

   \return
   NULL Image not found
   Else Valid image

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
const C_PuiBsImage * C_PuiBsElements::GetImage(const uint32 & oru32_Index) const
{
   const C_PuiBsImage * pc_Retval = NULL;

   if (oru32_Index < this->c_Images.size())
   {
      pc_Retval = &this->c_Images[oru32_Index];
   }
   return pc_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set image

   \param[in] oru32_Index Image index
   \param[in] orc_Data    New value

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::SetImage(const uint32 & oru32_Index, const C_PuiBsImage & orc_Data)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index < this->c_Images.size())
   {
      this->c_Images[oru32_Index] = orc_Data;
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get line arrow

   \param[in] oru32_Index Line arrow index

   \return
   NULL Line arrow not found
   Else Valid line arrow

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
const C_PuiBsLineArrow * C_PuiBsElements::GetLineArrow(const uint32 & oru32_Index) const
{
   const C_PuiBsLineArrow * pc_Retval = NULL;

   if (oru32_Index < this->c_LineArrows.size())
   {
      pc_Retval = &this->c_LineArrows[oru32_Index];
   }
   return pc_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set line arrow

   \param[in] oru32_Index Line arrow index
   \param[in] orc_Data    New value

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::SetLineArrow(const uint32 & oru32_Index, const C_PuiBsLineArrow & orc_Data)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index < this->c_LineArrows.size())
   {
      this->c_LineArrows[oru32_Index] = orc_Data;
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get text element

   \param[in] oru32_Index Text element index

   \return
   NULL Text element not found
   Else Valid text element

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
const C_PuiBsTextElement * C_PuiBsElements::GetTextElement(const uint32 & oru32_Index) const
{
   const C_PuiBsTextElement * pc_Retval = NULL;

   if (oru32_Index < this->c_TextElements.size())
   {
      pc_Retval = &this->c_TextElements[oru32_Index];
   }
   return pc_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set text element

   \param[in] oru32_Index Text element index
   \param[in] orc_Data    New value

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid

   \created     21.07.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_PuiBsElements::SetTextElement(const uint32 & oru32_Index, const C_PuiBsTextElement & orc_Data)
{
   sint32 s32_Retval = C_NO_ERR;

   if (oru32_Index < this->c_TextElements.size())
   {
      this->c_TextElements[oru32_Index] = orc_Data;
   }
   else
   {
      s32_Retval = C_RANGE;
   }
   return s32_Retval;
}