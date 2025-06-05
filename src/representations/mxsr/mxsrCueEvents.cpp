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

#include "mxsrCueEvents.h"


namespace MusicFormats
{

//________________________________________________________________________

std::string mxsrCueEventKindAsString (
  mxsrCueEventKind noteCueEventKind)
{
  std::string result;

  switch (noteCueEventKind) {
    case mxsrCueEventKind::kCueEvent_NONE:
      result = "kCueEvent_NONE";
      break;
    case mxsrCueEventKind::kCueEventBegin:
      result = "kCueEventBegin";
      break;
    case mxsrCueEventKind::kCueEventEnd:
      result = "kCueEventEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrCueEventKind& elt)
{
  os << mxsrCueEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrCueEvent mxsrCueEvent::create (
  mxsrCueEventKind                 cueEventKind,
  const mxsrNoteSequentialNumber&  noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrCueEvent* obj =
    new mxsrCueEvent (
      cueEventKind,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrCueEvent::mxsrCueEvent (
  mxsrCueEventKind                 cueEventKind,
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
      eventInputLineNumber)
{
  fCueEventKind = cueEventKind;
}

mxsrCueEvent::~mxsrCueEvent ()
{}

std::string mxsrCueEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[CueEvent" <<
    ", fCueEventKind: " << fCueEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fNoteSequentialNumber: " << fNoteSequentialNumber <<
    ", fNoteStaffNumber: " << fNoteStaffNumber <<
    ", fNoteVoiceNumber: " << fNoteVoiceNumber <<

    ", fEventSequentialNumber: " << fEventSequentialNumber <<
    ']';

  return ss.str ();
}

std::string mxsrCueEvent::asString () const
{
  return asShortString ();
}

void mxsrCueEvent::print (std::ostream& os) const
{
  os <<
    "[CueEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 26;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fCueEventKind" << ": " << fCueEventKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": " << fNoteSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteStaffNumber" << ": S" << fNoteStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteVoiceNumber" << ": V" << fNoteVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": " << fEventSequentialNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrCueEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrCueEvent& elt)
{
  elt.print (os);
  return os;
}


} // namespace

