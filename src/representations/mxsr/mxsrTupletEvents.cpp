/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "mfIndentedTextOutput.h"

#include "mxsrTupletEvents.h"


namespace MusicFormats
{

//________________________________________________________________________
std::string mxsrTupletEventKindAsString (
  mxsrTupletEventKind noteTupletEventKind)
{
  std::string result;

  switch (noteTupletEventKind) {
    case mxsrTupletEventKind::kTupletEvent_NONE:
      result = "kTupletEvent_NONE";
      break;
    case mxsrTupletEventKind::kTupletEventBegin:
      result = "kTupletEventBegin";
      break;
    case mxsrTupletEventKind::kTupletEventEnd:
      result = "kTupletEventEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrTupletEventKind& elt)
{
  os << mxsrTupletEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrTupletEvent mxsrTupletEvent::create (
  mxsrTupletEventKind              tupletEventKind,
  const mxsrTupletNumber&          tupletNumber,
  const msrTupletFactor&           tupletFactor,
  const mxsrNoteSequentialNumber&  noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrTupletEvent* obj =
    new mxsrTupletEvent (
      tupletEventKind,
      tupletNumber,
      tupletFactor,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrTupletEvent::mxsrTupletEvent (
  mxsrTupletEventKind              tupletEventKind,
  const mxsrTupletNumber&          tupletNumber,
  const msrTupletFactor&           tupletFactor,
  const mxsrNoteSequentialNumber&  noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
  : mxsrNoteEvent (
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventSequentialNumber,
      eventInputLineNumber),
    fTupletNumber (
      tupletNumber),
    fTupletFactor (
      tupletFactor)
{
#ifdef MF_TRACE_IS_ENABLED
	if (gTraceOahGroup->getTraceTupletsBasics ()) {
		std::stringstream ss;

		ss <<
			"--> mxsrTupletEvent::mxsrTupletEvent()" <<
			", tupletEventKind: " <<
			tupletEventKind <<
			", tupletNumber: " <<
			tupletNumber <<

			", tupletFactor: " <<
			tupletFactor <<

			", noteSequentialNumber: " <<
			noteSequentialNumber <<
			", noteStaffNumber: " <<
			noteStaffNumber <<
			", noteVoiceNumber: " <<
			noteVoiceNumber <<

			", eventSequentialNumber: " <<
			eventSequentialNumber <<
			", line " << eventInputLineNumber;

		gWaeHandler->waeTrace (
			__FILE__, __LINE__,
			ss.str ());
	}
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI 0.9.70
  mfAssert (
    __FILE__, __LINE__,
#ifdef MF_USE_WRAPPED_TYPES
    tupletNumber.getBareValue () > 0,
#else
    tupletNumber > 0,
#endif // MF_USE_WRAPPED_TYPES
    "tupletNumber is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fTupletEventKind = tupletEventKind;
}

mxsrTupletEvent::~mxsrTupletEvent ()
{}

std::string mxsrTupletEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[TupletEvent" <<
    ", fTupletEventKind: " << fTupletEventKind <<
    ", fTupletNumber: " << fTupletNumber <<
    ", fTupletFactor:" << fTupletFactor <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fNoteSequentialNumber: " << fNoteSequentialNumber <<
    ", fNoteStaffNumber: " << fNoteStaffNumber <<
    ", fNoteVoiceNumber: " << fNoteVoiceNumber <<

    ", fEventSequentialNumber: " << fEventSequentialNumber <<
    ']';

  return ss.str ();
}

std::string mxsrTupletEvent::asString () const
{
  return asShortString ();
}

void mxsrTupletEvent::print (std::ostream& os) const
{
  os <<
    "[TupletEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletEventKind" << ": " << fTupletEventKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletNumber" << ": " << fTupletNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletFactor" << ": " << fTupletFactor <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": " << fNoteSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteStaffNumber" << ": " << fNoteStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteVoiceNumber" << ": " << fNoteVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": " << fEventSequentialNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrTupletEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrTupletEvent& elt)
{
  elt.print (os);
  return os;
}


} // namespace

