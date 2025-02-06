/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// #include <climits>      // INT_MIN, INT_MAX
//
#include <sstream>
#include <iomanip>

#include "mfIndentedTextOutput.h"

#include "mxsr2msrTuplets.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mxsrTuplet mxsrTuplet::create (
	int               tupletInputLineNumber,
	int               tupletNumber,
	msrTupletTypeKind tupletTypeKind)
{
  mxsrTuplet* obj = new
    mxsrTuplet (
    	tupletInputLineNumber,
    	tupletNumber,
    	tupletTypeKind);
  assert (obj != nullptr);
  return obj;
}

mxsrTuplet::mxsrTuplet (
	int               tupletInputLineNumber,
	int               tupletNumber,
	msrTupletTypeKind tupletTypeKind)
{
  fTupletInputLineNumber =tupletInputLineNumber;
  fTupletNumber = tupletNumber;
  fTupletTypeKind = tupletTypeKind;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    gLog <<
      std::endl <<
      "Creating an MXSR tuplet: " <<
      std::endl;

			++gIndenter;

			constexpr int fieldWidth = 28;

	    gLog << std::left <<
    		std::setw (fieldWidth) <<
	    	"fTupletInputLineNumber" << fTupletInputLineNumber <<
	    	std::endl <<
    		std::setw (fieldWidth) <<
	    	"fTupletNumber" << fTupletNumber <<
	    	std::endl <<
    		std::setw (fieldWidth) <<
	    	"fTupletTypeKind" << fTupletTypeKind <<
	    	std::endl << std::endl;

			--gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

mxsrTuplet::~mxsrTuplet ()
{}

std::string mxsrTuplet::asString () const
{
  std::stringstream ss;

  ss <<
		"[mxsrTuplet" <<
		", fTupletTypeKind:" << fTupletTypeKind <<
		", fTupletNumber: " << fTupletNumber <<
		", fTupletInputLineNumber: " << fTupletInputLineNumber <<
    ']';

  return ss.str ();
}

void mxsrTuplet::print (std::ostream& os) const
{
  os <<
    "[mxsrTuplet" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletTypeKind" << ": " << fTupletTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletNumber" << ": " << fTupletNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletInputLineNumber" << ": " <<
    fTupletInputLineNumber <<
    std::endl;

  --gIndenter;

	os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const mxsrTuplet& elt) {
  elt.print (os);
  return os;
}

std::ostream& operator << (std::ostream& os, const S_mxsrTuplet& elt) {
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mxsr2msrPendingTupletStop mxsr2msrPendingTupletStop::create (
  int               eventSequentialNumber,
  int               noteSequentialNumber,
  int               staffNumber,
  int               voiceNumber,
  int               tupletNumber,
  mfInputLineNumber inputLineNumber)
{
  mxsr2msrPendingTupletStop* obj = new
    mxsr2msrPendingTupletStop (
      eventSequentialNumber,
      noteSequentialNumber,
      staffNumber,
      voiceNumber,
      tupletNumber,
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsr2msrPendingTupletStop::mxsr2msrPendingTupletStop (
  int               eventSequentialNumber,
  int               noteSequentialNumber,
  int               staffNumber,
  int               voiceNumber,
  int               tupletNumber,
  mfInputLineNumber inputLineNumber)
{
  fEventSequentialNumber = eventSequentialNumber;
  fNoteSequentialNumber = noteSequentialNumber;

	fStaffNumber = staffNumber;
  fVoiceNumber = voiceNumber;

  fTupletNumber = tupletNumber;

  fInputLineNumber = inputLineNumber;
}

mxsr2msrPendingTupletStop::~mxsr2msrPendingTupletStop ()
{}

std::string mxsr2msrPendingTupletStop::asString () const
{
  std::stringstream ss;

  ss <<
		"[PendingTupletStop" <<
		", fInputLineNumber: " << fInputLineNumber <<
		", fEventSequentialNumber: E" << fEventSequentialNumber <<
		", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
		", fStaffNumber: S" << fStaffNumber <<
		", fVoiceNumber:V" << fVoiceNumber <<
		", fTupletNumber: T" << fTupletNumber <<
    ']' <<
    std::endl;

  return ss.str ();
}

void mxsr2msrPendingTupletStop::print (std::ostream& os) const
{
	os <<
		"[PendingTupletStop" <<
		std::endl;

	++gIndenter;

  constexpr int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fInputLineNumber" << ": " <<
    fInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" <<
    fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": N" <<
    fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffNumber" << ": S" <<
    fStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceNumber" << ": V" <<
    fVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletNumber" << ": T" <<
    fTupletNumber <<
    std::endl;

  --gIndenter;

	os <<
		']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const mxsr2msrPendingTupletStop& elt)
{
  elt.print (os);
  return os;
}

std::ostream& operator << (std::ostream& os, const S_mxsr2msrPendingTupletStop& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


} // namespace
