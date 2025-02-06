/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "mxsr2msrEvents.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
/* this class is purely virtual
S_mxsrEvent mxsrEvent::create (
  int               eventSequentialNumber,
  mfInputLineNumber eventInputLineNumber)
{
  mxsrEvent* obj =
    new mxsrEvent (
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

mxsrEvent::mxsrEvent (
  int               eventSequentialNumber,
  mfInputLineNumber eventInputLineNumber)
{
  fEventSequentialNumber = eventSequentialNumber;

  fEventInputLineNumber = eventInputLineNumber;
}

mxsrEvent::~mxsrEvent ()
{}

// std::ostream& operator << (std::ostream& os, const S_mxsrEvent& elt)
// {
//   if (elt) {
//     elt->print (os);
//   }
//   else {
//     os << "[NULL]" << std::endl;
//   }
//
//   return os;
// }
//
// std::ostream& operator << (std::ostream& os, const mxsrEvent& elt)
// {
//   elt.print (os);
//   return os;
// }

//________________________________________________________________________
// bool compareEventsByIncreasingSequentialNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second)
// {
//   return
//     first->getEventSequentialNumber ()
//       <
//     second->getEventSequentialNumber ();
// }
//
// bool compareEventsByIncreasingInputLineNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second)
// {
//   return
//     first->getEventInputLineNumber ()
//       <
//     second->getEventInputLineNumber ();
// }
//
// bool compareEventsByIncreasingEventNoteSequentialNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second)
// {
//   return
//     first->getNoteSequentialNumber ()
//       <
//     second->getNoteSequentialNumber ();
// }
//
// bool operator< (
//   const S_mxsrEvent& first,
//   const S_mxsrEvent& second)
// {
//   return
//     first->getNoteSequentialNumber ()
//       <
//     second->getNoteSequentialNumber ();
// }
//
// bool compareStaffChangeEventsByIncreasingInputLineNumber (
//   S_mxsrStaffChangeEvent& first,
//   S_mxsrStaffChangeEvent& second)
// {
//   return
//     first->getEventInputLineNumber ()
//       <
//     second->getEventInputLineNumber ();
// }

//________________________________________________________________________
/* this class is purely virtual
mxsr-Event mxsrPartEvent::create (
  int                partEventSequentialNumber,
  const std::string& partName,
  const std::string& measureNumber,
  int                eventInputLineNumber)
{
  mxsrPartEvent* obj =
    new mxsrPartEvent (
      eventSequentialNumber,
      partName,
      measureNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

mxsrPartEvent::mxsrPartEvent (
  int                eventSequentialNumber,
  const std::string& partName,
  const std::string& measureNumber,
  int                eventInputLineNumber)
  : mxsrEvent (
      eventSequentialNumber,
      eventInputLineNumber)
{
  fEventSequentialNumber = eventSequentialNumber;

  fPartName = partName;
  fMeasureNumber = measureNumber;
}

mxsrPartEvent::~mxsrPartEvent ()
{}

//________________________________________________________________________
std::string mxsrMeasureRepeatEventKindAsString (
  mxsrMeasureRepeatEventKind measureRepeatEventKind)
{
  std::string result;

  switch (measureRepeatEventKind) {
    case mxsrMeasureRepeatEventKind::kMeasureRepeatEvent_NONE:
      result = "kMeasureRepeatEvent_NONE";
      break;
    case mxsrMeasureRepeatEventKind::kMeasureRepeatEventBegin:
      result = "kMeasureRepeatEventBegin";
      break;
    case mxsrMeasureRepeatEventKind::kMeasureRepeatEventEnd:
      result = "kMeasureRepeatEventEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrMeasureRepeatEventKind& elt)
{
  os << mxsrMeasureRepeatEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrMeasureRepeatEvent mxsrMeasureRepeatEvent::create (
  int                        eventSequentialNumber,
  const std::string&         partName,
  const std::string&         measureNumber,
  mxsrMeasureRepeatEventKind measureRepeatEventKind,
  int                        repeatNumber,
  int                        eventInputLineNumber)
{
  mxsrMeasureRepeatEvent* obj =
    new mxsrMeasureRepeatEvent (
      eventSequentialNumber,
      partName,
      measureNumber,
      measureRepeatEventKind,
      repeatNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrMeasureRepeatEvent::mxsrMeasureRepeatEvent (
  int                        eventSequentialNumber,
  const std::string&         partName,
  const std::string&         measureNumber,
  mxsrMeasureRepeatEventKind measureRepeatEventKind,
  int                        repeatNumber,
  int                        eventInputLineNumber)
  : mxsrPartEvent (
      eventSequentialNumber,
      partName,
      measureNumber,
      eventInputLineNumber)
{
  fMeasureRepeatEventKind = measureRepeatEventKind;

  fRepeatNumber = repeatNumber;
}

mxsrMeasureRepeatEvent::~mxsrMeasureRepeatEvent ()
{}

std::string mxsrMeasureRepeatEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[MeasureRepeatEvent" <<
    ", fMeasureRepeatEventKind: " << fMeasureRepeatEventKind <<
    ", fEventInputLineNumber: L" << fEventInputLineNumber <<

    ", fEventSequentialNumber: E" << fEventSequentialNumber <<

    ", fPartName: " << fPartName <<
    ", fMeasureNumber: " << fMeasureNumber <<

    ", fRepeatNumber: " << fRepeatNumber <<
    ']';

  return ss.str ();
}

std::string mxsrMeasureRepeatEvent::asString () const
{
  return asShortString ();
}

void mxsrMeasureRepeatEvent::print (std::ostream& os) const
{
  os <<
    "[MeasureRepeatEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureRepeatEventKind" << ": " << fMeasureRepeatEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": L" << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": " << fPartName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureNumber" << ": " << fMeasureNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fRepeatNumber" << ": " << fRepeatNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrMeasureRepeatEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrMeasureRepeatEvent& elt)
{
  elt.print (os);
  return os;
}

//________________________________________________________________________
/* this class is purely virtual
mxsrNoteEvent mxsrNoteEvent::create (
  int eventSequentialNumber,
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int eventInputLineNumber)
{
  mxsrNoteEvent* obj =
    new mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      chordEventKind,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

mxsrNoteEvent::mxsrNoteEvent (
  int eventSequentialNumber,
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int eventInputLineNumberr)
    : mxsrEvent (
      eventSequentialNumber,
      eventInputLineNumberr)
{
  fNoteSequentialNumber = noteSequentialNumber;

  fNoteStaffNumber = noteStaffNumber;
  fNoteVoiceNumber = noteVoiceNumber;
}

mxsrNoteEvent::~mxsrNoteEvent ()
{}

//________________________________________________________________________
std::string mxsrStaffChangeEventKindAsString (
  mxsrStaffChangeEventKind noteStaffChangeEventKind)
{
  std::string result;

  switch (noteStaffChangeEventKind) {
    case mxsrStaffChangeEventKind::kStaffChangeEvent_NONE:
      result = "kStaffChangeEvent_NONE";
      break;
    case mxsrStaffChangeEventKind::kStaffChangeEventTakeOff:
      result = "kStaffChangeEventTakeOff";
      break;
    case mxsrStaffChangeEventKind::kStaffChangeEventLanding:
      result = "kStaffChangeEventLanding";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEventKind& elt)
{
  os << mxsrStaffChangeEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
  S_mxsrStaffChangeEvent mxsrStaffChangeEvent::create (
  int                      eventSequentialNumber,
  int                      noteSequentialNumber,
  int                      noteStaffNumber,
  int                      noteVoiceNumber,
  mxsrStaffChangeEventKind staffChangeEventKind,
  int                      takeOffStaffNumber,
  int                      landingStaffNumber,
  int                      takeOffInputLineNumber,
  int                      landingInputLineNumber,
  int                      eventInputLineNumber)
{
  mxsrStaffChangeEvent* obj =
    new mxsrStaffChangeEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      staffChangeEventKind,
      takeOffStaffNumber,
      landingStaffNumber,
      takeOffInputLineNumber,
      landingInputLineNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrStaffChangeEvent::mxsrStaffChangeEvent (
  int                      eventSequentialNumber,
  int                      noteSequentialNumber,
  int                      noteStaffNumber,
  int                      noteVoiceNumber,
  mxsrStaffChangeEventKind staffChangeEventKind,
  int                      takeOffStaffNumber,
  int                      landingStaffNumber,
  int                      takeOffInputLineNumber,
  int                      landingInputLineNumber,
  int                      eventInputLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventInputLineNumber)
{
  gLog <<
    "======>>>> " <<
    "takeOffStaffNumber: " << takeOffStaffNumber <<
    ", landingStaffNumber: " << landingStaffNumber <<
    std::endl;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (landingStaffNumber == takeOffStaffNumber) {
    std::stringstream ss;

    ss <<
      "staff change event" <<
      staffChangeEventKind <<
      " to one and the same staff number " <<
      landingStaffNumber;

    mfAssert (
      __FILE__, __LINE__,
      false,
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fStaffChangeEventKind = staffChangeEventKind;

  fTakeOffStaffNumber = takeOffStaffNumber;
  fLandingStaffNumber = landingStaffNumber;

  fTakeOffInputLineNumber = takeOffInputLineNumber;
  fLandingInputLineNumber = landingInputLineNumber;
}

mxsrStaffChangeEvent::~mxsrStaffChangeEvent ()
{}

std::string mxsrStaffChangeEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[StaffChangeEvent" <<
    ", fEventInputLineNumber: L" << fEventInputLineNumber <<
    ", fTakeOffInputLineNumber: L" << fTakeOffInputLineNumber <<
    ", fLandingInputLineNumber: L" << fLandingInputLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
    ", fNoteStaffNumber: S" << fNoteStaffNumber <<
    ", fNoteVoiceNumber: V" << fNoteVoiceNumber <<
    ", fStaffChangeEventKind: " << fStaffChangeEventKind <<
    ", staff number change" <<
       ": S" << fTakeOffStaffNumber << " ->> S" << fLandingStaffNumber <<
    ", fLandingInputLineNumber: L" << fLandingInputLineNumber <<
    ']';

  return ss.str ();
}

std::string mxsrStaffChangeEvent::asString () const
{
  return asShortString ();
}

void mxsrStaffChangeEvent::print (std::ostream& os) const
{
  os <<
    "[StaffChangeEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": L" << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "staff change" << ": " <<
     "S" << fTakeOffStaffNumber << "/" << "V" << fNoteVoiceNumber <<
     " ->> " <<
     "S" << fLandingStaffNumber << "/" << "V" << fNoteVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": N" << fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteStaffNumber" << ": S" << fNoteStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteVoiceNumber" << ": V" << fNoteVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffChangeEventKind" << ": " << fStaffChangeEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTakeOffInputLineNumber" << ": L" << fTakeOffInputLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLandingInputLineNumber" << ": L" << fLandingInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTakeOffStaffNumber" << ": S" << fTakeOffStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLandingStaffNumber" << ": S" << fLandingStaffNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrStaffChangeEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEvent& elt)
{
  elt.print (os);
  return os;
}

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
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                noteStaffNumber,
  int                noteVoiceNumber,
  mxsrGraceEventKind graceEventKind,
  int                eventInputLineNumber)
{
  mxsrGraceEvent* obj =
    new mxsrGraceEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      graceEventKind,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrGraceEvent::mxsrGraceEvent (
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                noteStaffNumber,
  int                noteVoiceNumber,
  mxsrGraceEventKind graceEventKind,
  int                eventInputLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
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
    ", fEventInputLineNumber: " << fEventInputLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
    ", fNoteStaffNumber: S" << fNoteStaffNumber <<
    ", fNoteVoiceNumber: V" << fNoteVoiceNumber <<
    ", fGraceEventKind: " << fGraceEventKind <<
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
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceEventKind" << ": " << fGraceEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteStaffNumber" << ": S" << fNoteStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteVoiceNumber" << ": V" << fNoteVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": N" << fNoteSequentialNumber <<
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
  int              eventSequentialNumber,
  int              noteSequentialNumber,
  int              noteStaffNumber,
  int              noteVoiceNumber,
  mxsrCueEventKind cueEventKind,
  int              eventInputLineNumber)
{
  mxsrCueEvent* obj =
    new mxsrCueEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      cueEventKind,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrCueEvent::mxsrCueEvent (
  int              eventSequentialNumber,
  int              noteSequentialNumber,
  int              noteStaffNumber,
  int              noteVoiceNumber,
  mxsrCueEventKind cueEventKind,
  int              eventInputLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
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
    ", fEventInputLineNumber: " << fEventInputLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
    ", fNoteStaffNumber: S" << fNoteStaffNumber <<
    ", fNoteVoiceNumber: V" << fNoteVoiceNumber <<
    ", fCueEventKind: " << fCueEventKind <<
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
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCueEventKind" << ": " << fCueEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": N" << fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteStaffNumber" << ": S" << fNoteStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteVoiceNumber" << ": V" << fNoteVoiceNumber <<
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

//________________________________________________________________________
std::string mxsrChordEventKindAsString (
  mxsrChordEventKind noteChordEventKind)
{
  std::string result;

  switch (noteChordEventKind) {
    case mxsrChordEventKind::kChordEvent_NONE:
      result = "kChordEvent_NONE";
      break;
    case mxsrChordEventKind::kChordEventBegin:
      result = "kChordEventBegin";
      break;
    case mxsrChordEventKind::kChordEventEnd:
      result = "kChordEventEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrChordEventKind& elt)
{
  os << mxsrChordEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrChordEvent mxsrChordEvent::create (
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                noteStaffNumber,
  int                noteVoiceNumber,
  mxsrChordEventKind chordEventKind,
  int                eventInputLineNumber)
{
  mxsrChordEvent* obj =
    new mxsrChordEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      chordEventKind,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrChordEvent::mxsrChordEvent (
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                noteStaffNumber,
  int                noteVoiceNumber,
  mxsrChordEventKind chordEventKind,
  int                eventInputLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventInputLineNumber)
{
  fChordEventKind = chordEventKind;
}

mxsrChordEvent::~mxsrChordEvent ()
{}

std::string mxsrChordEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[ChordEvent" <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
    ", fNoteStaffNumber: S" << fNoteStaffNumber <<
    ", fNoteVoiceNumber: V" << fNoteVoiceNumber <<
    ", fChordEventKind: " << fChordEventKind <<
    ']';

  return ss.str ();
}

std::string mxsrChordEvent::asString () const
{
  return asShortString ();
}

void mxsrChordEvent::print (std::ostream& os) const
{
  os <<
    "[ChordEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 26;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fChordEventKind" << ": " << fChordEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": N" << fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteStaffNumber" << ": S" << fNoteStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteVoiceNumber" << ": V" << fNoteVoiceNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrChordEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrChordEvent& elt)
{
  elt.print (os);
  return os;
}

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
  int                 eventSequentialNumber,
  int                 noteSequentialNumber,
  int                 noteStaffNumber,
  int                 noteVoiceNumber,
  mxsrTupletEventKind tupletEventKind,
  int                 tupletNumber,
  int                 eventInputLineNumber)
{
  mxsrTupletEvent* obj =
    new mxsrTupletEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      tupletEventKind,
      tupletNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrTupletEvent::mxsrTupletEvent (
  int                 eventSequentialNumber,
  int                 noteSequentialNumber,
  int                 noteStaffNumber,
  int                 noteVoiceNumber,
  mxsrTupletEventKind tupletEventKind,
  int                 tupletNumber,
  int                 eventInputLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventInputLineNumber)
{
  fTupletEventKind = tupletEventKind;

  fTupletNumber = tupletNumber;
}

mxsrTupletEvent::~mxsrTupletEvent ()
{}

std::string mxsrTupletEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[TupletEvent" <<
    ", fTupletEventKind: " << fTupletEventKind <<
    ", fTupletNumber: T" << fTupletNumber <<
    ", fEventInputLineNumber: L " << fEventInputLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
    ", fNoteStaffNumber: S" << fNoteStaffNumber <<
    ", fNoteVoiceNumber: V" << fNoteVoiceNumber <<
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
    "fTupletNumber" << ": T" << fTupletNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": L" << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": N" << fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteStaffNumber" << ": S" << fNoteStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteVoiceNumber" << ": V" << fNoteVoiceNumber <<
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

//________________________________________________________________________
S_mxsrEventsCollection mxsrEventsCollection::create ()
{
  mxsrEventsCollection* obj =
    new mxsrEventsCollection ();
  assert (obj != nullptr);
  return obj;
}

mxsrEventsCollection::mxsrEventsCollection ()
{
  fCurrentEventSequentialNumber = 0;
}

mxsrEventsCollection::~mxsrEventsCollection ()
{}

//________________________________________________________________________
void mxsrEventsCollection::registerMeasureRepeatBegin (
  const std::string&         partName,
  const std::string&         measureNumber,
  int                        repeatNumber,
  int                        eventInputLineNumber)
{
  S_mxsrMeasureRepeatEvent
    measureRepeatEvent =
      mxsrMeasureRepeatEvent::create (
        ++fCurrentEventSequentialNumber,
        partName,
        measureNumber,
        mxsrMeasureRepeatEventKind::kMeasureRepeatEventBegin,
        repeatNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering measure repeat event " <<
      measureRepeatEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureRepeatBeginsMap.insert (
    std::make_pair (
      measureNumber, measureRepeatEvent));

  fAllEventsList.push_back (measureRepeatEvent);
}

void mxsrEventsCollection::registerMeasureRepeatEnd (
  const std::string&         partName,
  const std::string&         measureNumber,
  int                        repeatNumber,
  int                        eventInputLineNumber)
{
  S_mxsrMeasureRepeatEvent
    measureRepeatEvent =
      mxsrMeasureRepeatEvent::create (
        ++fCurrentEventSequentialNumber,
        partName,
        measureNumber,
        mxsrMeasureRepeatEventKind::kMeasureRepeatEventEnd,
        repeatNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering measure repeat event " <<
      measureRepeatEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureRepeatEndsMap.insert (
    std::make_pair (
      measureNumber, measureRepeatEvent));

  fAllEventsList.push_back (measureRepeatEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerStaffChangeTakeOff (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int takeOffStaffNumber,
  int landingStaffNumber,
  int takeOffInputLineNumber,
  int landingInputLineNumber,
  int eventInputLineNumber)
{
  S_mxsrStaffChangeEvent
    staffChangeEvent =
      mxsrStaffChangeEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrStaffChangeEventKind::kStaffChangeEventTakeOff,
        takeOffStaffNumber,
        landingStaffNumber,
        takeOffInputLineNumber,
        landingInputLineNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering staff change event " <<
      staffChangeEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffChangeTakeOffsMap.insert (
    std::make_pair (
      noteSequentialNumber, staffChangeEvent));

  fStaffChangeEventsList.push_back (staffChangeEvent);

  fAllEventsList.push_back (staffChangeEvent);
}

void mxsrEventsCollection::registerStaffChangeLanding (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int takeOffStaffNumber,
  int landingStaffNumber,
  int takeOffInputLineNumber,
  int landingInputLineNumber,
  int eventInputLineNumber)
{
  S_mxsrStaffChangeEvent
    staffChangeEvent =
      mxsrStaffChangeEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrStaffChangeEventKind::kStaffChangeEventLanding,
        takeOffStaffNumber,
        landingStaffNumber,
        takeOffInputLineNumber,
        landingInputLineNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering staff change event " <<
      staffChangeEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffChangeLandingsMap.insert (
    std::make_pair (
      noteSequentialNumber, staffChangeEvent));

  fStaffChangeEventsList.push_back (staffChangeEvent);

  fAllEventsList.push_back (staffChangeEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerGraceBegin (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int eventInputLineNumber)
{
  S_mxsrGraceEvent
    graceEvent =
      mxsrGraceEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrGraceEventKind::kGraceEventBegin,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering grace note event " <<
      graceEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fGraceBeginsMap.insert (
    std::make_pair (
      noteSequentialNumber, graceEvent));

  fAllEventsList.push_back (graceEvent);
}

void mxsrEventsCollection::registerGraceEnd (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int eventInputLineNumber)
{
  S_mxsrGraceEvent
    graceEvent =
      mxsrGraceEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrGraceEventKind::kGraceEventEnd,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering grace note event " <<
      graceEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fGraceEndsMap.insert (
    std::make_pair (
      noteSequentialNumber, graceEvent));

  fAllEventsList.push_back (graceEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerCueBegin (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int eventInputLineNumber)
{
  S_mxsrCueEvent
    cueEvent =
      mxsrCueEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrCueEventKind::kCueEventBegin,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering cue note event " <<
      cueEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCueBeginsMap.insert (
    std::make_pair (
      noteSequentialNumber, cueEvent));

  fAllEventsList.push_back (cueEvent);
}

void mxsrEventsCollection::registerCueEnd (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int eventInputLineNumber)
{
  S_mxsrCueEvent
    cueEvent =
      mxsrCueEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrCueEventKind::kCueEventEnd,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering cue note event " <<
      cueEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCueEndsMap.insert (
    std::make_pair (
      noteSequentialNumber, cueEvent));

  fAllEventsList.push_back (cueEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerChordBegin (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int eventInputLineNumber)
{
  S_mxsrChordEvent
    chordEvent =
      mxsrChordEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrChordEventKind::kChordEventBegin,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering chord event " <<
      chordEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordBeginsMap.insert (
    std::make_pair (
      noteSequentialNumber, chordEvent));

  fAllEventsList.push_back (chordEvent);
}

void mxsrEventsCollection::registerChordEnd (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int eventInputLineNumber)
{
  S_mxsrChordEvent
    chordEvent =
      mxsrChordEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrChordEventKind::kChordEventEnd,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering chord event " <<
      chordEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordEndsMap.insert (
    std::make_pair (
      noteSequentialNumber, chordEvent));

  fAllEventsList.push_back (chordEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerTupletBeginEvent (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int tupletNumber,
  int eventInputLineNumber)
{
  S_mxsrTupletEvent
    tupletEvent =
      mxsrTupletEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrTupletEventKind::kTupletEventBegin,
        tupletNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering tuplet begin event " <<
      tupletEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // push tupletEvent onto fTupletBeginsList VITAL
  fTupletBeginsList.push_front (tupletEvent);

  fTupletEventsMultiMap.insert (
    std::make_pair (
      noteSequentialNumber, tupletEvent));

  fAllEventsList.push_back (tupletEvent);
}

void mxsrEventsCollection::registerTupletEndEvent (
  int noteSequentialNumber,
  int noteStaffNumber,
  int noteVoiceNumber,
  int tupletNumber,
  int eventInputLineNumber)
{
  S_mxsrTupletEvent
    tupletEvent =
      mxsrTupletEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        mxsrTupletEventKind::kTupletEventEnd,
        tupletNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering tuplet end event " <<
      tupletEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletEndsList.push_back (tupletEvent);

  fTupletEventsMultiMap.insert (
    std::make_pair (
      noteSequentialNumber, tupletEvent));

  fAllEventsList.push_back (tupletEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::sortTheMxsrEventsLists ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMxsrEvents ()) {
    std::stringstream ss;

    ss <<
      "--> Sorting the all notes events list";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fAllEventsList.sort ();

  fStaffChangeEventsList.sort ();

  fTupletBeginsList.sort ();
  fTupletEndsList.sort ();
}

//________________________________________________________________________
S_mxsrStaffChangeEvent mxsrEventsCollection::fetchStaffChangeTakeOffAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrStaffChangeEvent result;

  std::map <int, S_mxsrStaffChangeEvent>::const_iterator it;

  it = fStaffChangeTakeOffsMap.find (noteSequentialNumber);

  if (it != fStaffChangeTakeOffsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrStaffChangeEvent mxsrEventsCollection::fetchStaffChangeLandingAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrStaffChangeEvent result;

  std::map <int, S_mxsrStaffChangeEvent>::const_iterator it;

  it = fStaffChangeLandingsMap.find (noteSequentialNumber);

  if (it != fStaffChangeLandingsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
S_mxsrGraceEvent mxsrEventsCollection::fetchGraceBeginAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrGraceEvent result;

  std::map <int, S_mxsrGraceEvent>::const_iterator it;

  it = fGraceBeginsMap.find (noteSequentialNumber);

  if (it != fGraceBeginsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrGraceEvent mxsrEventsCollection::fetchGraceEndAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrGraceEvent result;

  std::map <int, S_mxsrGraceEvent>::const_iterator it;

  it = fGraceEndsMap.find (noteSequentialNumber);

  if (it != fGraceEndsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
S_mxsrCueEvent mxsrEventsCollection::fetchCueBeginAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrCueEvent result;

  std::map <int, S_mxsrCueEvent>::const_iterator it;

  it = fCueBeginsMap.find (noteSequentialNumber);

  if (it != fCueBeginsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrCueEvent mxsrEventsCollection::fetchCueEndAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrCueEvent result;

  std::map <int, S_mxsrCueEvent>::const_iterator it;

  it = fCueEndsMap.find (noteSequentialNumber);

  if (it != fCueEndsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
S_mxsrChordEvent mxsrEventsCollection::fetchChordBeginAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrChordEvent result;

  std::map <int, S_mxsrChordEvent>::const_iterator
    it =
      fChordBeginsMap.find (noteSequentialNumber);

  if (it != fChordBeginsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrChordEvent mxsrEventsCollection::fetchChordEndAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrChordEvent result;

  std::map <int, S_mxsrChordEvent>::const_iterator
    it =
      fChordEndsMap.find (noteSequentialNumber);

  if (it != fChordEndsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
void mxsrEventsCollection::fetchTupletBeginsList (
  int                            noteSequentialNumber,
  std::list <S_mxsrTupletEvent>& recipientTupletBeginsList)
{
  // look for the first tuplet event matching noteSequentialNumber
  std::list <S_mxsrTupletEvent>::iterator startIt =
    std::find_if (
      std::begin (fTupletBeginsList),
      std::end (fTupletBeginsList),
      [&] (const S_mxsrTupletEvent tupletEvent)
        {
          return
            tupletEvent->getNoteSequentialNumber () == noteSequentialNumber;
        });

  if (startIt != fTupletBeginsList.end ()) {
//     gLog << "(*startIt): " << (*startIt) << std::endl << std::flush;

    // look for the first next tuplet event not matching noteSequentialNumber
    std::list <S_mxsrTupletEvent>::iterator endIt =
      std::find_if (
        std::next (startIt),
        std::end (fTupletBeginsList),
        [&] (const S_mxsrTupletEvent tupletEvent )
          {
            return
              tupletEvent->getNoteSequentialNumber () != noteSequentialNumber;
          });

//     if (endIt == fTupletBeginsList.end ()) {
//       gLog << "fTupletBeginsList.end ()";
//     }
//     else {
//       gLog << (*endIt);
//     }
//     gLog << std::endl << std::endl << std::flush;

    // move the found tuplets events to recipientTupletBeginsList
    recipientTupletBeginsList.splice (
      recipientTupletBeginsList.begin (), fTupletBeginsList, startIt, endIt);
  }

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTupletsBasics ()) {
//     printTupletEventsList (
//       gLog,
//       recipientTupletBeginsList,
//       "fetchTupletBeginsList(), recipientTupletBeginsList:");
//   }
// #endif // MF_TRACE_IS_ENABLED
}

void mxsrEventsCollection::fetchTupletEndsList (
  int                            noteSequentialNumber,
  std::list <S_mxsrTupletEvent>& recipientTupletEndsList)
{
  // look for the first tuplet event matching noteSequentialNumber
  std::list <S_mxsrTupletEvent>::iterator startIt =
    std::find_if (
      std::begin (fTupletEndsList),
      std::end (fTupletEndsList),
      [&] (const S_mxsrTupletEvent tupletEvent)
        {
          return
            tupletEvent->getNoteSequentialNumber () == noteSequentialNumber;
        });

  if (startIt != fTupletEndsList.end ()) {
//     gLog << "(*startIt): " << (*startIt) << std::endl << std::flush;

    // look for the first next tuplet event not matching noteSequentialNumber
    std::list <S_mxsrTupletEvent>::iterator endIt =
      std::find_if (
        std::next (startIt),
        std::end (fTupletEndsList),
        [&] (const S_mxsrTupletEvent tupletEvent )
          {
            return
              tupletEvent->getNoteSequentialNumber () != noteSequentialNumber;
          });

//     if (endIt == fTupletEndsList.end ()) {
//       gLog << "fTupletEndsList.end ()";
//     }
//     else {
//       gLog << (*endIt);
//     }
//     gLog << std::endl << std::endl << std::flush;

    // move the found tuplets events to recipientTupletEndsList
    recipientTupletEndsList.splice (
      recipientTupletEndsList.begin (), fTupletEndsList, startIt, endIt);
  }

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTupletsBasics ()) {
//     printTupletEventsList (
//       gLog,
//       recipientTupletEndsList,
//       "fetchTupletEndsList(), resultingEndsList:");
//   }
// #endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
std::string mxsrEventsCollection::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[EventsCollection" <<
    ", fStaffChangeTakeOffsMap.size (): " << fStaffChangeTakeOffsMap.size () <<
    ", fStaffChangeLandingsMap.size (): " << fStaffChangeLandingsMap.size () <<

    ", fGraceBeginsMap.size (): " << fGraceBeginsMap.size () <<
    ", fGraceEndsMap.size (): " << fGraceEndsMap.size () <<

    ", fCueBeginsMap.size (): " << fCueBeginsMap.size () <<
    ", fCueEndsMap.size (): " << fCueEndsMap.size () <<

    ", fChordBeginsMap.size (): " << fChordBeginsMap.size () <<
    ", fChordEndsMap.size (): " << fChordEndsMap.size () <<

    ", fTupletBeginsList.size (): " << fTupletBeginsList.size () <<
    ", fTupletEndsList.size (): " << fTupletEndsList.size () <<
    ", fTupletEventsMultiMap.size (): " << fTupletEventsMultiMap.size () <<

    ", fAllEventsList.size (): " << fAllEventsList.size () <<
    ']';

  return ss.str ();
}

std::string mxsrEventsCollection::asString () const
{
  return asShortString ();
}

//------------------------------------------------------------------------
void mxsrEventsCollection::printAllEventsList (std::ostream& os) const
{
  os <<
    "fAllEventsList: " <<
    mfSingularOrPlural (
      fAllEventsList.size (),
      "element",
      "elements") <<
    ", sorted by event sequential number" <<
    std::endl;

  ++gIndenter;

  for (S_mxsrEvent event : fAllEventsList) {
    os <<
      "Event " << event->getEventSequentialNumber () <<
      ':' <<
      std::endl;

    ++gIndenter;
    event->print (os);
    os <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;
}

//------------------------------------------------------------------------
void mxsrEventsCollection::prinMeasureRepeatEvents (std::ostream& os) const
{
  os <<
    "fMeasureRepeatBeginsMap: " <<
    mfSingularOrPlural (
      fMeasureRepeatBeginsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <std::string, S_mxsrMeasureRepeatEvent> thePair : fMeasureRepeatBeginsMap) {
    std::string
      measureNumber = thePair.first;

    S_mxsrMeasureRepeatEvent
      measureRepeatEvent = thePair.second;

    os <<
      "Measure " << measureNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      measureRepeatEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "fMeasureRepeatEndsMap: " <<
    mfSingularOrPlural (
      fMeasureRepeatEndsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <std::string, S_mxsrMeasureRepeatEvent> thePair : fMeasureRepeatEndsMap) {
    std::string
      measureNumber = thePair.first;

    S_mxsrMeasureRepeatEvent
      measureRepeatEvent = thePair.second;

    os <<
      "Note " << measureNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      measureRepeatEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

}

//------------------------------------------------------------------------
void mxsrEventsCollection::printStaffChangeEvents (std::ostream& os) const
{
  os <<
    "fStaffChangeEventsList: " <<
    mfSingularOrPlural (
      fStaffChangeEventsList.size (),
      "element",
      "elements") <<
    ", sorted by input start line number" <<
    std::endl;

  ++gIndenter;

  for (S_mxsrEvent event : fStaffChangeEventsList) {
    os <<
      "Note " << event->getEventSequentialNumber () <<
      ':' <<
      std::endl;

    ++gIndenter;
    event->print (os);
    os <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << "--------" << std::endl << std::endl;

  os <<
    "fStaffChangeTakeOffsMap: " <<
    mfSingularOrPlural (
      fStaffChangeTakeOffsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrStaffChangeEvent> thePair : fStaffChangeTakeOffsMap) {
    int
      eventSequentialNumber = thePair.first;
    S_mxsrStaffChangeEvent
      staffChangeEvent = thePair.second;

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      staffChangeEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << "--------" << std::endl << std::endl;

  os <<
    "fStaffChangeLandingsMap: " <<
    mfSingularOrPlural (
      fStaffChangeLandingsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrStaffChangeEvent> thePair : fStaffChangeLandingsMap) {
    int
      eventSequentialNumber = thePair.first;
    S_mxsrStaffChangeEvent
      staffChangeEvent = thePair.second;

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      staffChangeEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;
  }

//------------------------------------------------------------------------
void mxsrEventsCollection::printGraceEvents (std::ostream& os) const
{
  os <<
    "fGraceBeginsMap: " <<
    mfSingularOrPlural (
      fGraceBeginsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrGraceEvent> thePair : fGraceBeginsMap) {
    int
      eventSequentialNumber = thePair.first;

    S_mxsrGraceEvent
      graceEvent = thePair.second;

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      graceEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "fGraceEndsMap: " <<
    mfSingularOrPlural (
      fGraceEndsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrGraceEvent> thePair : fGraceEndsMap) {
    int
      eventSequentialNumber = thePair.first;

    S_mxsrGraceEvent
      graceEvent = thePair.second;

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      graceEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

}

//------------------------------------------------------------------------
void mxsrEventsCollection::printCueEvents (std::ostream& os) const
{
  os <<
    "fCueBeginsMap: " <<
    mfSingularOrPlural (
      fCueBeginsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrCueEvent> thePair : fCueBeginsMap) {
    int
      eventSequentialNumber = thePair.first;

    S_mxsrCueEvent
      cueEvent = thePair.second;

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      cueEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "fCueEndsMap: " <<
    mfSingularOrPlural (
      fCueEndsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrCueEvent> thePair : fCueEndsMap) {
    int
      eventSequentialNumber = thePair.first;

    S_mxsrCueEvent
      cueEvent = thePair.second;

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      cueEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

}

//------------------------------------------------------------------------
void mxsrEventsCollection::printChordEvents (std::ostream& os) const
{
  os <<
    "fChordBeginsMap: " <<
    mfSingularOrPlural (
      fChordBeginsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrChordEvent> thePair : fChordBeginsMap) {
    int
      eventSequentialNumber = thePair.first;

    S_mxsrChordEvent
      chordEvent = thePair.second;

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      chordEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "fChordEndsMap: " <<
    mfSingularOrPlural (
      fChordEndsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrChordEvent> thePair : fChordEndsMap) {
    int
      eventSequentialNumber = thePair.first;

    S_mxsrChordEvent
      chordEvent = thePair.second;

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      chordEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

}

//------------------------------------------------------------------------
void mxsrEventsCollection::printTupletEvents (std::ostream& os) const
{
  os <<
    "fTupletEventsMultiMap: " <<
    mfSingularOrPlural (
      fTupletEventsMultiMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrTupletEvent> thePair : fTupletEventsMultiMap) {
    int
      eventSequentialNumber = thePair.first;
    S_mxsrTupletEvent
      tupletEvent = thePair.second;

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      tupletEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "fTupletBeginsList: " <<
    mfSingularOrPlural (
      fTupletBeginsList.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (S_mxsrTupletEvent tupletEvent : fTupletBeginsList) {
    int
      eventSequentialNumber = tupletEvent->getEventSequentialNumber ();

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      tupletEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "fTupletEndsList: " <<
    mfSingularOrPlural (
      fTupletEndsList.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (S_mxsrTupletEvent tupletEvent : fTupletEndsList) {
    int
      eventSequentialNumber = tupletEvent->getEventSequentialNumber ();

    os <<
      "Note " << eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      tupletEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;
}

//________________________________________________________________________
void mxsrEventsCollection::printTupletEventsList (
  std::ostream&                        os,
  const std::list <S_mxsrTupletEvent>& tupletEventsList,
  const std::string&                   context) const
{
  os <<
    "--> printTupletEventsList()" <<
    ", context: " <<
    context <<
    ", " <<
    mfSingularOrPlural (
      tupletEventsList.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (S_mxsrTupletEvent tupletEvent : tupletEventsList) {
    int
      eventSequentialNumber = tupletEvent->getEventSequentialNumber ();

    os <<
      "Note " <<  eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      tupletEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;
}

//________________________________________________________________________
void mxsrEventsCollection::print (std::ostream& os) const
{
  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "[EventsCollection" <<
    std::endl;

  ++gIndenter;

  os <<
    "fThereIsAnImplicitInitialForwardRepeat: " <<
    fThereIsAnImplicitInitialForwardRepeat <<
    std::endl;

  os << std::endl << "--------" << std::endl << std::endl;

  constexpr int fieldWidth = 26;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fAllEventsList" << ": " <<
    mfSingularOrPlural (
      fAllEventsList.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffChangeTakeOffsMap" << ": " <<
    mfSingularOrPlural (
      fStaffChangeTakeOffsMap.size (),
      "element",
      "elements") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStaffChangeLandingsMap" << ": " <<
    mfSingularOrPlural (
      fStaffChangeLandingsMap.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceBeginsMap" << ": " <<
    mfSingularOrPlural (
      fGraceBeginsMap.size (),
      "element",
      "elements") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGraceEndsMap" << ": " <<
    mfSingularOrPlural (
      fGraceEndsMap.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCueBeginsMap" << ": " <<
    mfSingularOrPlural (
      fCueBeginsMap.size (),
      "element",
      "elements") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fCueEndsMap" << ": " <<
    mfSingularOrPlural (
      fCueEndsMap.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fChordBeginsMap" << ": " <<
    mfSingularOrPlural (
      fChordBeginsMap.size (),
      "element",
      "elements") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordEndsMap" << ": " <<
    mfSingularOrPlural (
      fChordEndsMap.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletEventsMultiMap" << ": " <<
    mfSingularOrPlural (
      fTupletEventsMultiMap.size (),
      "element",
      "elements") <<
    std::endl << std::endl;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMxsrEvents ()) {
    os << "--------" << std::endl << std::endl;
    printAllEventsList (os);
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
    os << "--------" << std::endl << std::endl;
    prinMeasureRepeatEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
    os << "--------" << std::endl << std::endl;
    printStaffChangeEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotesBasics ()) {
    os << "--------" << std::endl << std::endl;
    printGraceEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
    os << "--------" << std::endl << std::endl;
    printCueEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    os << "--------" << std::endl << std::endl;
    printChordEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    os << "--------" << std::endl << std::endl;
    printTupletEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrEventsCollection& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrEventsCollection& elt)
{
  elt.print (os);
  return os;
}


} // namespace

