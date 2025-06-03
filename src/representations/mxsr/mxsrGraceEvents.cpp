/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// #include <algorithm>
// #include <cassert>
// #include <iostream>
// #include <sstream>
#include <iomanip>

#include <map>

// #include "mfAssert.h"
// #include "mfStringsHandling.h"

#include "mxsrGraceEvents.h"

// #include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
std::string mxsrGraceEventKindAsString (
  mxsrGraceEventKind noteGraceEventKind)
{
  std::string result;

  switch (noteGraceEventKind) {
    case mxsrGraceEventKind::kGraceEvent_NONE:
      result = "kGraceEvent_NONE";
      break;
    case mxsrGraceEventKind::kGraceEventBegin:
      result = "kGraceEventBegin";
      break;
    case mxsrGraceEventKind::kGraceEventEnd:
      result = "kGraceEventEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrGraceEventKind& elt)
{
  os << mxsrGraceEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrGraceEvent mxsrGraceEvent::create (
  mxsrGraceEventKind               graceEventKind,
  const mxsrNoteSequentialNumber&  noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrGraceEvent* obj =
    new mxsrGraceEvent (
      graceEventKind,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrGraceEvent::mxsrGraceEvent (
  mxsrGraceEventKind               graceEventKind,
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
  fGraceEventKind = graceEventKind;
}

mxsrGraceEvent::~mxsrGraceEvent ()
{}

std::string mxsrGraceEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[GraceEvent" <<
    ", fGraceEventKind: " << fGraceEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fNoteSequentialNumber: " << fNoteSequentialNumber <<
    ", fNoteStaffNumber: " << fNoteStaffNumber <<
    ", fNoteVoiceNumber: " << fNoteVoiceNumber <<

    ", fEventSequentialNumber: " << fEventSequentialNumber <<
    ']';

  return ss.str ();
}

std::string mxsrGraceEvent::asString () const
{
  return asShortString ();
}

void mxsrGraceEvent::print (std::ostream& os) const
{
  os <<
    "[GraceEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 26;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fGraceEventKind" << ": " << fGraceEventKind <<
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

std::ostream& operator << (std::ostream& os, const S_mxsrGraceEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrGraceEvent& elt)
{
  elt.print (os);
  return os;
}


} // namespace

