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

#include <map>

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "mxsr2msrEvents.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
/* this class is purely virtual
S_mxsrEvent mxsrEvent::create (
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrEvent* obj =
    new mxsrEvent (
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

mxsrEvent::mxsrEvent (
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
    : fEventSequentialNumber (
        eventSequentialNumber),
      fEventInputLineNumber (
        eventInputLineNumber)
{}

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
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
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
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
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
std::string mxsrMultipleMeasureRestEventKindAsString (
  mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind)
{
  std::string result;

  switch (multipleMeasureRestEventKind) {
    case mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEvent_NONE:
      result = "kMultipleMeasureRestEvent_NONE";
      break;
    case mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEventBegin:
      result = "kMultipleMeasureRestEventBegin";
      break;
    case mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEventEnd:
      result = "kMultipleMeasureRestEventEnd";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrMultipleMeasureRestEventKind& elt)
{
  os << mxsrMultipleMeasureRestEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrMultipleMeasureRestEvent mxsrMultipleMeasureRestEvent::create (
  mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              multipleMeasureRestNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrMultipleMeasureRestEvent* obj =
    new mxsrMultipleMeasureRestEvent (
      multipleMeasureRestEventKind,
      partName,
      measureNumber,
      multipleMeasureRestNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

// S_mxsrMultipleMeasureRestEvent mxsrMultipleMeasureRestEvent::createAMultipleMeasureRestEnd (
//   const std::string&       partName,
//   const mfMeasureNumber&   measureNumber,
//   int                      multipleMeasureRestNumber,
//   const mfInputLineNumber& eventInputLineNumber)
// {
//   ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand
//
//   S_mxsrMultipleMeasureRestEvent
//     multipleMeasureRestEndEvent =
//       mxsrMultipleMeasureRestEvent::create (
//         mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEventEnd,
//         partName,
//         measureNumber,
//         multipleMeasureRestNumber,
//         fCurrentEventSequentialNumber,
//         eventInputLineNumber);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Registering multiple measure rest end event " <<
//       multipleMeasureRestEndEvent->asString () <<
//       ", line " << eventInputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return multipleMeasureRestEndEvent;
// }

mxsrMultipleMeasureRestEvent::mxsrMultipleMeasureRestEvent (
  mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              multipleMeasureRestNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
  : mxsrPartEvent (
      partName,
      measureNumber,
      eventSequentialNumber,
      eventInputLineNumber)
{
  fMultipleMeasureRestEventKind = multipleMeasureRestEventKind;

  fMultipleMeasureRestNumber = multipleMeasureRestNumber;
}

mxsrMultipleMeasureRestEvent::~mxsrMultipleMeasureRestEvent ()
{}

std::string mxsrMultipleMeasureRestEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[MultipleMeasureRestEvent" <<
    ", fMultipleMeasureRestEventKind: " << fMultipleMeasureRestEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fPartName: " << fPartName <<
    ", fMeasureNumber: " << fMeasureNumber <<
    ", fMultipleMeasureRestNumber: " << fMultipleMeasureRestNumber <<

    ", fEventSequentialNumber: " << fEventSequentialNumber <<
    ']';

  return ss.str ();
}

std::string mxsrMultipleMeasureRestEvent::asString () const
{
  return asShortString ();
}

void mxsrMultipleMeasureRestEvent::print (std::ostream& os) const
{
  os <<
    "[MultipleMeasureRestEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMultipleMeasureRestEventKind" << ": " << fMultipleMeasureRestEventKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<

    std::endl <<
    std::setw (fieldWidth) <<
    "fPartName" << ": " << fPartName <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureNumber" << ": " << fMeasureNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMultipleMeasureRestNumber" << ": " << fMultipleMeasureRestNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": " << fEventSequentialNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrMultipleMeasureRestEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrMultipleMeasureRestEvent& elt)
{
  elt.print (os);
  return os;
}

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
  mxsrMeasureRepeatEventKind       measureRepeatEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              measureRepeatNumber,
  int                              measureRepeatSlashes,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrMeasureRepeatEvent* obj =
    new mxsrMeasureRepeatEvent (
      measureRepeatEventKind,
      partName,
      measureNumber,
      measureRepeatNumber,
      measureRepeatSlashes,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrMeasureRepeatEvent::mxsrMeasureRepeatEvent (
  mxsrMeasureRepeatEventKind       measureRepeatEventKind,
  const std::string&               partName,
  const mfMeasureNumber&           measureNumber,
  int                              measureRepeatNumber,
  int                              measureRepeatSlashes,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
  : mxsrPartEvent (
      partName,
      measureNumber,
      eventSequentialNumber,
      eventInputLineNumber)
{
  fMeasureRepeatEventKind = measureRepeatEventKind;

  fMeasureRepeatSlashes = measureRepeatSlashes;
}

mxsrMeasureRepeatEvent::~mxsrMeasureRepeatEvent ()
{}

std::string mxsrMeasureRepeatEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[MeasureRepeatEvent" <<
    ", fMeasureRepeatEventKind: " << fMeasureRepeatEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fPartName: " << fPartName <<
    ", fMeasureNumber: " << fMeasureNumber <<
    ", fMeasureRepeatSlashes: " << fMeasureRepeatSlashes <<

    ", fEventSequentialNumber: event_" << fEventSequentialNumber <<
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
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<

    std::endl <<
    std::setw (fieldWidth) <<
    "fPartName" << ": " << fPartName <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureNumber" << ": " << fMeasureNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureRepeatSlashes" << ": " << fMeasureRepeatSlashes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": " << fEventSequentialNumber <<
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
  const mxsrNoteSequentialNumber&  noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrNoteEvent* obj =
    new mxsrNoteEvent (
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/



mxsrNoteEvent::mxsrNoteEvent (
  const mxsrNoteSequentialNumber&  noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
    : mxsrEvent (
        eventSequentialNumber,
        eventInputLineNumber),
      fNoteSequentialNumber (
        noteSequentialNumber)
{
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
  mxsrStaffChangeEventKind         staffChangeEventKind,
  const mxsrNoteSequentialNumber & noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mfStaffNumber&             takeOffStaffNumber,
  const mfInputLineNumber&         takeOffInputLineNumber, // superflous ???
  const mfStaffNumber&             landingStaffNumber,
  const mfInputLineNumber&         landingInputLineNumber, // superflous ???
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrStaffChangeEvent* obj =
    new mxsrStaffChangeEvent (
      staffChangeEventKind,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      takeOffStaffNumber,
      takeOffInputLineNumber,
      landingStaffNumber,
      landingInputLineNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrStaffChangeEvent::mxsrStaffChangeEvent (
  mxsrStaffChangeEventKind         staffChangeEventKind,
  const mxsrNoteSequentialNumber & noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mfStaffNumber&             takeOffStaffNumber,
  const mfInputLineNumber&         takeOffInputLineNumber, // superflous ???
  const mfStaffNumber&             landingStaffNumber,
  const mfInputLineNumber&         landingInputLineNumber, // superflous ???
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
  : mxsrNoteEvent (
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventSequentialNumber,
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
    ", fStaffChangeEventKind: " << fStaffChangeEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fTakeOffInputLineNumber: " << fTakeOffInputLineNumber <<
    ", fLandingInputLineNumber: " << fLandingInputLineNumber <<
    ", fNoteSequentialNumber: " << fNoteSequentialNumber <<
    ", fNoteStaffNumber: " << fNoteStaffNumber <<
    ", fNoteVoiceNumber: " << fNoteVoiceNumber <<
    ", staff number change: " <<
    fTakeOffStaffNumber << " ->> " << fLandingStaffNumber <<
    ", fLandingInputLineNumber: " << fLandingInputLineNumber <<

    ", fEventSequentialNumber: " << fEventSequentialNumber <<
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
    "fStaffChangeEventKind" << ": " << fStaffChangeEventKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "staff change" << ": " <<
    fTakeOffStaffNumber << "/" << "" << fNoteVoiceNumber <<
    " ->> " <<
    fLandingStaffNumber << "/" << "" << fNoteVoiceNumber <<
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
    "fTakeOffStaffNumber" << ": " << fTakeOffStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTakeOffInputLineNumber" << ": " << fTakeOffInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fLandingStaffNumber" << ": " << fLandingStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLandingInputLineNumber" << ": " << fLandingInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": " << fEventSequentialNumber <<
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
  mxsrChordEventKind               chordEventKind,
  const mxsrNoteSequentialNumber&  noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrChordEvent* obj =
    new mxsrChordEvent (
      chordEventKind,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrChordEvent::mxsrChordEvent (
  mxsrChordEventKind               chordEventKind,
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
  fChordEventKind = chordEventKind;
}

mxsrChordEvent::~mxsrChordEvent ()
{}

std::string mxsrChordEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[ChordEvent" <<
    ", fChordEventKind: " << fChordEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fNoteSequentialNumber: " << fNoteSequentialNumber <<
    ", fNoteStaffNumber: " << fNoteStaffNumber <<
    ", fNoteVoiceNumber: " << fNoteVoiceNumber <<

    ", fEventSequentialNumber: " << fEventSequentialNumber <<
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
    "fChordEventKind" << ": " << fChordEventKind <<
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
  // sanity check JMI v0.9.70
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

//________________________________________________________________________
S_mxsrEventsCollection mxsrEventsCollection::create ()
{
  mxsrEventsCollection* obj =
    new mxsrEventsCollection ();
  assert (obj != nullptr);
  return obj;
}

mxsrEventsCollection::mxsrEventsCollection ()
{}

mxsrEventsCollection::~mxsrEventsCollection ()
{}

//________________________________________________________________________
S_mxsrMultipleMeasureRestEvent mxsrEventsCollection::createAMultipleMeasureRestBegin (
  const std::string&       partName,
  const mfMeasureNumber&   measureNumber,
  int                      multipleMeasureRestNumber,
  const mfInputLineNumber& eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrMultipleMeasureRestEvent
    multipleMeasureRestBeginEvent =
      mxsrMultipleMeasureRestEvent::create (
        mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEventBegin,
        partName,
        measureNumber,
        multipleMeasureRestNumber,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering multiple measure rest begin event " <<
      multipleMeasureRestBeginEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return multipleMeasureRestBeginEvent;
}

void mxsrEventsCollection::registerMultipleMeasureRestBegin (
  S_mxsrMultipleMeasureRestEvent multipleMeasureRestBeginEvent)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering multiple measure rest begin event " <<
      multipleMeasureRestBeginEvent->asString () <<
      ", line " << multipleMeasureRestBeginEvent->getEventInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMultipleMeasureRestBeginsMap.insert (
    std::make_pair (

#ifdef MF_USE_WRAPPED_TYPES
      multipleMeasureRestBeginEvent->getMeasureNumber ().getBareValue (),
#else
      multipleMeasureRestBeginEvent->getMeasureNumber (),
#endif // MF_USE_WRAPPED_TYPES

      multipleMeasureRestBeginEvent));

  fAllEventsList.push_back (multipleMeasureRestBeginEvent);
}

//________________________________________________________________________
// S_mxsrMultipleMeasureRestEvent mxsrEventsCollection::createAMultipleMeasureRestEnd (
//   const std::string&       partName,
//   const mfMeasureNumber&   measureNumber,
//   int                      multipleMeasureRestNumber,
//   const mfInputLineNumber& eventInputLineNumber)
// {
//   ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand
//
//   S_mxsrMultipleMeasureRestEvent
//     multipleMeasureRestEndEvent =
//       mxsrMultipleMeasureRestEvent::create (
//         mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEventEnd,
//         partName,
//         measureNumber,
//         multipleMeasureRestNumber,
//         fCurrentEventSequentialNumber,
//         eventInputLineNumber);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Registering multiple measure rest end event " <<
//       multipleMeasureRestEndEvent->asString () <<
//       ", line " << eventInputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return multipleMeasureRestEndEvent;
// }
//
// void mxsrEventsCollection::registerMultipleMeasureRestEnd (
//   S_mxsrMultipleMeasureRestEvent multipleMeasureRestEndEvent)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Registering multiple measure rest end event " <<
//       multipleMeasureRestEndEvent->asString () <<
//       ", line " << multipleMeasureRestEndEvent->getEventInputLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fMultipleMeasureRestEndsMap.insert (
//     std::make_pair (
//       multipleMeasureRestEndEvent->getMeasureNumber ().getBareValue (),
//       multipleMeasureRestEndEvent));
//
//   fAllEventsList.push_back (multipleMeasureRestEndEvent);
// }

void mxsrEventsCollection::createAMultipleMeasureRestEndAndRegisterIt (
  const std::string&       partName,
  const mfMeasureNumber&   measureNumber,
  int                      multipleMeasureRestNumber,
  const mfInputLineNumber& eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrMultipleMeasureRestEvent
    multipleMeasureRestEndEvent =
      mxsrMultipleMeasureRestEvent::create (
        mxsrMultipleMeasureRestEventKind::kMultipleMeasureRestEventEnd,
        partName,
        measureNumber,
        multipleMeasureRestNumber,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering multiple measure rest end event " <<
      multipleMeasureRestEndEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering multiple measure rest end event " <<
      multipleMeasureRestEndEvent->asString () <<
      ", line " << multipleMeasureRestEndEvent->getEventInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMultipleMeasureRestEndsMap.insert (
    std::make_pair (

#ifdef MF_USE_WRAPPED_TYPES
      multipleMeasureRestEndEvent->getMeasureNumber ().getBareValue (),
#else
      multipleMeasureRestEndEvent->getMeasureNumber (),
#endif // MF_USE_WRAPPED_TYPES

      multipleMeasureRestEndEvent));

  fAllEventsList.push_back (multipleMeasureRestEndEvent);
}

//________________________________________________________________________
S_mxsrMeasureRepeatEvent mxsrEventsCollection::createAMeasureRepeatBegin (
  const std::string&       partName,
  const mfMeasureNumber&   measureNumber,
  int                      measureRepeatNumber,
  int                      measureRepeatSlashes,
  const mfInputLineNumber& eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrMeasureRepeatEvent
    measureRepeatBeginEvent =
      mxsrMeasureRepeatEvent::create (
        mxsrMeasureRepeatEventKind::kMeasureRepeatEventBegin,
        partName,
        measureNumber,
        measureRepeatNumber,
        measureRepeatSlashes,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Creating a measure repeat begin event " <<
      measureRepeatBeginEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return measureRepeatBeginEvent;
}

void mxsrEventsCollection::registerMeasureRepeatBegin (
  S_mxsrMeasureRepeatEvent measureRepeatBeginEvent)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering measure repeat begin event " <<
      measureRepeatBeginEvent->asString () <<
      ", line " << measureRepeatBeginEvent->getEventInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureRepeatBeginsMap.insert (
    std::make_pair (

#ifdef MF_USE_WRAPPED_TYPES
      measureRepeatBeginEvent->getMeasureNumber ().getBareValue (),
#else
      measureRepeatBeginEvent->getMeasureNumber (),
#endif // MF_USE_WRAPPED_TYPES

      measureRepeatBeginEvent));

  fAllEventsList.push_back (measureRepeatBeginEvent);
}

// S_mxsrMeasureRepeatEvent mxsrEventsCollection::createAMeasureRepeatEnd (
//   const std::string&       partName,
//   const mfMeasureNumber&   measureNumber,
//   int                      measureRepeatNumber,
//   int                      measureRepeatSlashes,
//   const mfInputLineNumber& eventInputLineNumber)
// {
//   ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand
//
//   S_mxsrMeasureRepeatEvent
//     measureRepeatEndEvent =
//       mxsrMeasureRepeatEvent::create (
//         mxsrMeasureRepeatEventKind::kMeasureRepeatEventEnd,
//         partName,
//         measureNumber,
//         measureRepeatSlashes,
//         fCurrentEventSequentialNumber,
//         eventInputLineNumber);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Creating a measure repeat end event " <<
//       measureRepeatEndEvent->asString () <<
//       ", line " << eventInputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return measureRepeatEndEvent;
// }
//
// void mxsrEventsCollection::registerMeasureRepeatEnd (
//   S_mxsrMeasureRepeatEvent measureRepeatEndEvent)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Registering measure repeat end event " <<
//       measureRepeatEndEvent->asString () <<
//       ", line " << measureRepeatEndEvent->getMeasureNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fMeasureRepeatEndsMap.insert (
//     std::make_pair (
//       measureRepeatEndEvent->getMeasureNumber ().getBareValue (),
//       measureRepeatEndEvent));
//
//   fAllEventsList.push_back (measureRepeatEndEvent);
// }

void mxsrEventsCollection::createAMeasureRepeatEndAndRegisterIt (
  const std::string&       partName,
  const mfMeasureNumber&   measureNumber,
  int                      measureRepeatNumber,
  int                      measureRepeatSlashes,
  const mfInputLineNumber& eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrMeasureRepeatEvent
    measureRepeatEndEvent =
      mxsrMeasureRepeatEvent::create (
        mxsrMeasureRepeatEventKind::kMeasureRepeatEventEnd,
        partName,
        measureNumber,
        measureRepeatNumber,
        measureRepeatSlashes,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Creating a measure repeat end event " <<
      measureRepeatEndEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering measure repeat end event " <<
      measureRepeatEndEvent->asString () <<
      ", line " << measureRepeatEndEvent->getMeasureNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureRepeatEndsMap.insert (
    std::make_pair (

#ifdef MF_USE_WRAPPED_TYPES
      measureRepeatEndEvent->getMeasureNumber ().getBareValue (),
#else
      measureRepeatEndEvent->getMeasureNumber (),
#endif // MF_USE_WRAPPED_TYPES

      measureRepeatEndEvent));

  fAllEventsList.push_back (measureRepeatEndEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerStaffChangeTakeOff (
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfStaffNumber&            takeOffStaffNumber,
  const mfInputLineNumber&        takeOffInputLineNumber,
  const mfStaffNumber&            landingStaffNumber,
  const mfInputLineNumber&        landingInputLineNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrStaffChangeEvent
    staffChangeEvent =
      mxsrStaffChangeEvent::create (
        mxsrStaffChangeEventKind::kStaffChangeEventTakeOff,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        takeOffStaffNumber,
        takeOffInputLineNumber,
        landingStaffNumber,
        landingInputLineNumber,
        fCurrentEventSequentialNumber,
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

#ifdef MF_USE_WRAPPED_TYPES
      noteSequentialNumber.getBareValue (),
#else
      noteSequentialNumber,
#endif // MF_USE_WRAPPED_TYPES

      staffChangeEvent));

  fStaffChangeEventsList.push_back (staffChangeEvent);

  fAllEventsList.push_back (staffChangeEvent);
}

void mxsrEventsCollection::registerStaffChangeLanding (
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfStaffNumber&            takeOffStaffNumber,
  const mfInputLineNumber&        takeOffInputLineNumber,
  const mfStaffNumber&            landingStaffNumber,
  const mfInputLineNumber&        landingInputLineNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrStaffChangeEvent
    staffChangeEvent =
      mxsrStaffChangeEvent::create (
        mxsrStaffChangeEventKind::kStaffChangeEventLanding,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        takeOffStaffNumber,
        takeOffInputLineNumber,
        landingStaffNumber,
        landingInputLineNumber,
        fCurrentEventSequentialNumber,
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

#ifdef MF_USE_WRAPPED_TYPES
      noteSequentialNumber.getBareValue (),
#else
      noteSequentialNumber,
#endif // MF_USE_WRAPPED_TYPES

      staffChangeEvent));

  fStaffChangeEventsList.push_back (staffChangeEvent);

  fAllEventsList.push_back (staffChangeEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerGraceBegin (
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrGraceEvent
    graceEvent =
      mxsrGraceEvent::create (
        mxsrGraceEventKind::kGraceEventBegin,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        fCurrentEventSequentialNumber,
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
      noteSequentialNumber,
      graceEvent));

  fAllEventsList.push_back (graceEvent);
}

void mxsrEventsCollection::registerGraceEnd (
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrGraceEvent
    graceEvent =
      mxsrGraceEvent::create (
        mxsrGraceEventKind::kGraceEventEnd,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        fCurrentEventSequentialNumber,
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
      noteSequentialNumber,
      graceEvent));

  fAllEventsList.push_back (graceEvent);
}

//________________________________________________________________________
S_mxsrCueEvent mxsrEventsCollection::createACueBeginEvent (
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrCueEvent
    cueEvent =
      mxsrCueEvent::create (
        mxsrCueEventKind::kCueEventBegin,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Creating a cue begin event " <<
      cueEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return cueEvent;
}

void mxsrEventsCollection::registerCueBeginEvent (
  S_mxsrCueEvent cueBeginEvent)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering cue begin event " <<
      cueBeginEvent->asString () <<
      ", line " << cueBeginEvent->getEventInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCueBeginsMap.insert (
    std::make_pair (
//       cueBeginEvent->getNoteSequentialNumber ().getBareValue (),
      cueBeginEvent->getNoteSequentialNumber (),
      cueBeginEvent));

  fAllEventsList.push_back (cueBeginEvent);
}

S_mxsrCueEvent mxsrEventsCollection::createACueEndEvent (
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrCueEvent
    cueEvent =
      mxsrCueEvent::create (
        mxsrCueEventKind::kCueEventEnd,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Creating a cue end event " <<
      cueEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return cueEvent;
}

void mxsrEventsCollection::registerCueEndEvent (
  S_mxsrCueEvent cueEndEvent)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering cue end event " <<
      cueEndEvent->asString () <<
      ", line " << cueEndEvent->getEventInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCueEndsMap.insert (
    std::make_pair (
//       cueEndEvent->getNoteSequentialNumber ().getBareValue (),
      cueEndEvent->getNoteSequentialNumber (),
      cueEndEvent));

  fAllEventsList.push_back (cueEndEvent);
}

//________________________________________________________________________
S_mxsrChordEvent mxsrEventsCollection::createAChordBeginEvent (
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrChordEvent
    chordEvent =
      mxsrChordEvent::create (
        mxsrChordEventKind::kChordEventBegin,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Creating a chord begin event " <<
      chordEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return chordEvent;
}

void mxsrEventsCollection::registerChordBeginEvent (
  const S_mxsrChordEvent& chordBeginEvent)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering chord begin event " <<
      chordBeginEvent->asString () <<
      ", line " << chordBeginEvent->getEventInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordBeginsMap.insert (
    std::make_pair (
//       cueEndEchordBeginEventvent->getNoteSequentialNumber ().getBareValue (),
      chordBeginEvent->getNoteSequentialNumber (),
      chordBeginEvent));

  fAllEventsList.push_back (chordBeginEvent);
}

EXP S_mxsrChordEvent mxsrEventsCollection::createAChordEndEvent (
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrChordEvent
    chordEvent =
      mxsrChordEvent::create (
        mxsrChordEventKind::kChordEventEnd,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Creating a chord end event " <<
      chordEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return chordEvent;
}

void mxsrEventsCollection::registerChordEndEvent (
  const S_mxsrChordEvent& chordEndEvent)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering chord end event " <<
      chordEndEvent->asString () <<
      ", line " << chordEndEvent->getEventInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordEndsMap.insert (
    std::make_pair (

#ifdef MF_USE_WRAPPED_TYPES
      chordEndEvent->getNoteSequentialNumber ().getBareValue (),
#else
      chordEndEvent->getNoteSequentialNumber (),
#endif // MF_USE_WRAPPED_TYPES

      chordEndEvent));

  fAllEventsList.push_back (chordEndEvent);
}

//________________________________________________________________________
S_mxsrTupletEvent mxsrEventsCollection::createATupletBeginEvent (
  const mxsrTupletNumber&         tupletNumber,
  const msrTupletFactor&          tupletFactor,
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrTupletEvent
    tupletEvent =
      mxsrTupletEvent::create (
        mxsrTupletEventKind::kTupletEventBegin,
        tupletNumber,
        tupletFactor,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Creating a tuplet begin event " <<
      tupletEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return tupletEvent;
}

S_mxsrTupletEvent mxsrEventsCollection::createATupletEndEvent (
  const mxsrTupletNumber&         tupletNumber,
  const msrTupletFactor&          tupletFactor,
  const mxsrNoteSequentialNumber& noteSequentialNumber,
  const mfStaffNumber&            noteStaffNumber,
  const mfVoiceNumber&            noteVoiceNumber,
  const mfInputLineNumber&        eventInputLineNumber)
{
  ++fCurrentEventSequentialNumber;
    // cannot pass this as an argument to craate,
    // since it is an instance of a template type
    // do we do it beforehand

  S_mxsrTupletEvent
    tupletEvent =
      mxsrTupletEvent::create (
        mxsrTupletEventKind::kTupletEventEnd,
        tupletNumber,
        tupletFactor,
        noteSequentialNumber,
        noteStaffNumber,
        noteVoiceNumber,
        fCurrentEventSequentialNumber,
        eventInputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Creating tuplet end event " <<
      tupletEvent->asString () <<
      ", line " << eventInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return tupletEvent;
}

void mxsrEventsCollection::registerTupletBeginEvent (
  S_mxsrTupletEvent tupletBeginEvent)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering tuplet begin event " <<
      tupletBeginEvent->asString () <<
      ", line " << tupletBeginEvent->getEventInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletBeginsMultiMap.insert (
    std::make_pair (

#ifdef MF_USE_WRAPPED_TYPES
      tupletBeginEvent->getNoteSequentialNumber ().getBareValue (),
#else
      tupletBeginEvent->getNoteSequentialNumber (),
#endif // MF_USE_WRAPPED_TYPES

      tupletBeginEvent));

  fTupletBeginNumbersMap.insert (
    std::make_pair (

#ifdef MF_USE_WRAPPED_TYPES
      tupletBeginEvent->getTupletNumber ().getBareValue (),
#else
      tupletBeginEvent->getTupletNumber (),
#endif // MF_USE_WRAPPED_TYPES

      tupletBeginEvent));

  fAllEventsList.push_back (tupletBeginEvent);
}

void mxsrEventsCollection::registerTupletEndEvent (
  S_mxsrTupletEvent tupletEndEvent)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering tuplet end event " <<
      tupletEndEvent->asString () <<
      ", line " << tupletEndEvent->getNoteSequentialNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletEndsMultiMap.insert (
    std::make_pair (

#ifdef MF_USE_WRAPPED_TYPES
      tupletEndEvent->getNoteSequentialNumber ().getBareValue (),
#else
      tupletEndEvent->getNoteSequentialNumber (),
#endif // MF_USE_WRAPPED_TYPES

      tupletEndEvent));

  fTupletEndNumbersMap.insert (
    std::make_pair (

#ifdef MF_USE_WRAPPED_TYPES
      tupletEndEvent->getTupletNumber ().getBareValue (),
#else
      tupletEndEvent->getTupletNumber (),
#endif // MF_USE_WRAPPED_TYPES

      tupletEndEvent));

  fAllEventsList.push_back (tupletEndEvent);
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

  // sort the all events list by ??? JMI
  fAllEventsList.sort ();

  // sort the staff change events list by ??? JMI
  fStaffChangeEventsList.sort ();

//   fTupletBeginsMultiMap.sort ();
//   fTupletEndsMultiMap.sort ();
}

//________________________________________________________________________
S_mxsrMultipleMeasureRestEvent mxsrEventsCollection::fetchMultipleMeasureRestBeginAtBareMeasureNumber (
  const std::string& bareMeasureNumber) const
{
  S_mxsrMultipleMeasureRestEvent result;

  std::map <std::string, S_mxsrMultipleMeasureRestEvent>::const_iterator
    it =
      fMultipleMeasureRestBeginsMap.find (bareMeasureNumber);

  if (it != fMultipleMeasureRestBeginsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrMultipleMeasureRestEvent mxsrEventsCollection::fetchMultipleMeasureRestEndAtBareMeasureNumber (
  const std::string& bareMeasureNumber) const
{
  S_mxsrMultipleMeasureRestEvent result;

  std::map <std::string, S_mxsrMultipleMeasureRestEvent>::const_iterator
    it =
      fMultipleMeasureRestEndsMap.find (bareMeasureNumber);

  if (it != fMultipleMeasureRestEndsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
S_mxsrMeasureRepeatEvent mxsrEventsCollection::fetchMeasureRepeatBeginAtBareMeasureNumber (
  const std::string& bareMeasureNumber) const
{
  S_mxsrMeasureRepeatEvent result;

  std::map <std::string, S_mxsrMeasureRepeatEvent>::const_iterator
    it =
      fMeasureRepeatBeginsMap.find (bareMeasureNumber);

  if (it != fMeasureRepeatBeginsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrMeasureRepeatEvent mxsrEventsCollection::fetchMeasureRepeatEndAtBareMeasureNumber (
  const std::string& bareMeasureNumber) const
{
  S_mxsrMeasureRepeatEvent result;

  std::map <std::string, S_mxsrMeasureRepeatEvent>::const_iterator
    it =
      fMeasureRepeatEndsMap.find (bareMeasureNumber);

  if (it != fMeasureRepeatEndsMap.end ()) {
    result = (*it).second;
  }

  return result;
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

  std::map <mxsrNoteSequentialNumber, S_mxsrGraceEvent>::const_iterator it;

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

  std::map <mxsrNoteSequentialNumber, S_mxsrGraceEvent>::const_iterator it;

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
S_mxsrTupletEvent mxsrEventsCollection::fetchTupletBeginForTupletNumber (
  int tupletBareNumber)
{
  S_mxsrTupletEvent result;

  result = fTupletBeginNumbersMap [tupletBareNumber];

//   for (std::pair <int, S_mxsrTupletEvent> thePair : fTupletBeginsMultiMap) {
//     S_mxsrTupletEvent
//       tupletBeginEvent = thePair.second;
//
//     if (tupletBeginEvent->tupletEventIsFortupletBareNumber (tupletBareNumber)) {
//       result = tupletBeginEvent;
//       break;
//     }
//   } // for

  return result;
}

void mxsrEventsCollection::fetchTupletBeginsAtNoteSequentialNumber (
  int                            noteSequentialNumber,
  std::list <S_mxsrTupletEvent>& recipientTupletBeginsList)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMxsrEvents ()) {
    std::stringstream ss;

    ss <<
      "--> fetchTupletBeginsAtNoteSequentialNumber()" <<
      ", noteSequentialNumber: " << noteSequentialNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // see https://cplusplus.com/reference/map/multimap/equal_range/

  std::pair <
    std::multimap <int, S_mxsrTupletEvent>::const_iterator,
    std::multimap <int, S_mxsrTupletEvent>::const_iterator
  > iteratorsPair;

  iteratorsPair = fTupletBeginsMultiMap.equal_range (
    noteSequentialNumber);

  for (
    std::multimap <int, S_mxsrTupletEvent>::const_iterator it =
      iteratorsPair.first;
    it != iteratorsPair.second;
    ++it
  ) {
    S_mxsrTupletEvent tupletEvent = it->second;

//     std::cout << "tupletEvent: " << tupletEvent << std::endl; JMI

    recipientTupletBeginsList.push_back (tupletEvent);
//     recipientTupletBeginsList.push_front (tupletEvent);
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    printTupletEventsList (
      gLog,
      recipientTupletBeginsList,
      "fetchTupletBeginsList(), recipientTupletBeginsList:");
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrEventsCollection::fetchTupletEndsListAtNoteSequentialNumber (
  int                            noteSequentialNumber,
  std::list <S_mxsrTupletEvent>& recipientTupletEndsList)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMxsrEvents ()) {
    std::stringstream ss;

    ss <<
      "--> fetchTupletEndsListAtNoteSequentialNumber()" <<
      ", noteSequentialNumber: " << noteSequentialNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::pair <
    std::multimap <int, S_mxsrTupletEvent>::const_iterator,
    std::multimap <int, S_mxsrTupletEvent>::const_iterator
  > iteratorsPair;

  iteratorsPair = fTupletEndsMultiMap.equal_range (
    noteSequentialNumber);

  for (
    std::multimap <int, S_mxsrTupletEvent>::const_iterator it =
      iteratorsPair.first;
    it != iteratorsPair.second;
    ++it
  ) {
    S_mxsrTupletEvent tupletEvent = it->second;

//     std::cout << "tupletEvent: " << tupletEvent << std::endl; JMI

    recipientTupletEndsList.push_back (tupletEvent);
//     recipientTupletEndsList.push_front (tupletEvent);
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    printTupletEventsList (
      gLog,
      recipientTupletEndsList,
      "fetchTupletEndsList(), resultingEndsList:");
  }
#endif // MF_TRACE_IS_ENABLED
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

    ", fTupletBeginsMultiMap.size (): " << fTupletBeginsMultiMap.size () <<
    ", fTupletEndsMultiMap.size (): " << fTupletEndsMultiMap.size () <<

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
void mxsrEventsCollection::printMultipleMeasureRestEvents (std::ostream& os) const
{
  os <<
    "fMultipleMeasureRestBeginsMap: " <<
    mfSingularOrPlural (
      fMultipleMeasureRestBeginsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <std::string, S_mxsrMultipleMeasureRestEvent> thePair : fMultipleMeasureRestBeginsMap) {
    std::string
      bareMeasureNumber = thePair.first;

    S_mxsrMultipleMeasureRestEvent
      multipleMeasureRestEvent = thePair.second;

    os <<
      "Measure " << bareMeasureNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      multipleMeasureRestEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "fMultipleMeasureRestEndsMap: " <<
    mfSingularOrPlural (
      fMultipleMeasureRestEndsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <std::string, S_mxsrMultipleMeasureRestEvent> thePair : fMultipleMeasureRestEndsMap) {
    std::string
      bareMeasureNumber = thePair.first;

    S_mxsrMultipleMeasureRestEvent
      multipleMeasureRestEvent = thePair.second;

    os <<
      "Note " << bareMeasureNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      multipleMeasureRestEvent <<
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
      bareMeasureNumber = thePair.first;

    S_mxsrMeasureRepeatEvent
      measureRepeatEvent = thePair.second;

    os <<
      "Measure " << bareMeasureNumber <<
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
      bareMeasureNumber = thePair.first;

    S_mxsrMeasureRepeatEvent
      measureRepeatEvent = thePair.second;

    os <<
      "Note " << bareMeasureNumber <<
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
      eventBareSequentialNumber = thePair.first;

    S_mxsrStaffChangeEvent
      staffChangeEvent = thePair.second;

    os <<
      "Note " << eventBareSequentialNumber <<
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
      eventBareSequentialNumber = thePair.first;

    S_mxsrStaffChangeEvent
      staffChangeEvent = thePair.second;

    os <<
      "Note " << eventBareSequentialNumber <<
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

  for (std::pair <mxsrNoteSequentialNumber, S_mxsrGraceEvent> thePair : fGraceBeginsMap) {
    mxsrNoteSequentialNumber
      eventBareSequentialNumber = thePair.first;

    S_mxsrGraceEvent
      graceEvent = thePair.second;

    os <<
      "Note " << eventBareSequentialNumber <<
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

  for (std::pair <mxsrNoteSequentialNumber, S_mxsrGraceEvent> thePair : fGraceEndsMap) {
    mxsrNoteSequentialNumber
      eventBareSequentialNumber = thePair.first;

    S_mxsrGraceEvent
      graceEvent = thePair.second;

    os <<
      "Note " << eventBareSequentialNumber <<
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
      eventBareSequentialNumber = thePair.first;

    S_mxsrCueEvent
      cueEvent = thePair.second;

    os <<
      "Note " << eventBareSequentialNumber <<
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
      eventBareSequentialNumber = thePair.first;

    S_mxsrCueEvent
      cueEvent = thePair.second;

    os <<
      "Note " << eventBareSequentialNumber <<
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
      eventBareSequentialNumber = thePair.first;

    S_mxsrChordEvent
      chordEvent = thePair.second;

    os <<
      "Note " << eventBareSequentialNumber <<
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
      eventBareSequentialNumber = thePair.first;

    S_mxsrChordEvent
      chordEvent = thePair.second;

    os <<
      "Note " << eventBareSequentialNumber <<
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
    "fTupletBeginsMultiMap: " <<
    mfSingularOrPlural (
      fTupletBeginsMultiMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrTupletEvent> thePair : fTupletBeginsMultiMap) {
    S_mxsrTupletEvent
      tupletEvent = thePair.second;

    mxsrEventSequentialNumber
      eventBareSequentialNumber =

#ifdef MF_USE_WRAPPED_TYPES
        tupletEvent->getEventSequentialNumber ().getBareValue ();
#else
        tupletEvent->getEventSequentialNumber ();
#endif // MF_USE_WRAPPED_TYPES

    os <<
      "Note " << eventBareSequentialNumber <<
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
    "fTupletEndsMultiMap: " <<
    mfSingularOrPlural (
      fTupletEndsMultiMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrTupletEvent> thePair : fTupletEndsMultiMap) {
    S_mxsrTupletEvent
      tupletEvent = thePair.second;

    mxsrEventSequentialNumber
      eventBareSequentialNumber =

#ifdef MF_USE_WRAPPED_TYPES
        tupletEvent->getEventSequentialNumber ().getBareValue ();
#else
        tupletEvent->getEventSequentialNumber ();
#endif // MF_USE_WRAPPED_TYPES

    os <<
      "Note " << eventBareSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      tupletEvent <<
      std::endl;
    --gIndenter;
  } // for

  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "fTupletBeginNumbersMap: " <<
    mfSingularOrPlural (
      fTupletBeginNumbersMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrTupletEvent> thePair : fTupletBeginNumbersMap) {
    int
      tupletBareNumber = thePair.first;

    S_mxsrTupletEvent
      tupletEvent = thePair.second;

    os <<
      "Tuplet begin event " << tupletBareNumber <<
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
    mxsrEventSequentialNumber
      eventBareSequentialNumber =

#ifdef MF_USE_WRAPPED_TYPES
        tupletEvent->getEventSequentialNumber ().getBareValue ();
#else
        tupletEvent->getEventSequentialNumber ();
#endif // MF_USE_WRAPPED_TYPES

    os <<
      "Note " <<  eventBareSequentialNumber <<
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

  constexpr int fieldWidth = 30;

  // all events

  os << std::left <<
    std::setw (fieldWidth) <<
    "fAllEventsList" << ": " <<
    mfSingularOrPlural (
      fAllEventsList.size (),
      "element",
      "elements") <<
    std::endl;

  // multiple measure rest events

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMultipleMeasureRestBeginsMap" << ": " <<
    mfSingularOrPlural (
      fMultipleMeasureRestBeginsMap.size (),
      "element",
      "elements") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMultipleMeasureRestEndsMap" << ": " <<
    mfSingularOrPlural (
      fMultipleMeasureRestEndsMap.size (),
      "element",
      "elements") <<
    std::endl;

  // measure repeat events

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureRepeatBeginsMap" << ": " <<
    mfSingularOrPlural (
      fMeasureRepeatBeginsMap.size (),
      "element",
      "elements") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureRepeatEndsMap" << ": " <<
    mfSingularOrPlural (
      fMeasureRepeatEndsMap.size (),
      "element",
      "elements") <<
    std::endl;

  // staff change events

  os << std::left <<
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
    std::endl;

  // grace events

  os << std::left <<
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
    std::endl;

  // cue events

  os << std::left <<
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
    std::endl;

  // chord events

  os << std::left <<
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
    std::endl;

  // tuplet events

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletBeginsMultiMap" << ": " <<
    mfSingularOrPlural (
      fTupletBeginsMultiMap.size (),
      "element",
      "elements") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletEndsMultiMap" << ": " <<
    mfSingularOrPlural (
      fTupletEndsMultiMap.size (),
      "element",
      "elements") <<
    std::endl;


  os << std::endl;


  // all events
#ifdef MF_TRACE_IS_ENABLED
  if (! fAllEventsList.empty ()) {
    os << "--------" << std::endl << std::endl;
    printAllEventsList (os);
  }
#endif // MF_TRACE_IS_ENABLED

  // multiple measure rest events
#ifdef MF_TRACE_IS_ENABLED
  if (! (fMultipleMeasureRestBeginsMap.empty () && fMultipleMeasureRestEndsMap.empty ())) {
    os << "--------" << std::endl << std::endl;
    printMultipleMeasureRestEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

  // measure repeat events
#ifdef MF_TRACE_IS_ENABLED
  if (! (fMeasureRepeatBeginsMap.empty () && fMeasureRepeatEndsMap.empty ())) {
    os << "--------" << std::endl << std::endl;
    prinMeasureRepeatEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

  // staff change events
#ifdef MF_TRACE_IS_ENABLED
  if (! (fStaffChangeTakeOffsMap.empty () && fStaffChangeLandingsMap.empty ())) {
    os << "--------" << std::endl << std::endl;
    printStaffChangeEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

  // grace events
#ifdef MF_TRACE_IS_ENABLED
  if (! (fGraceBeginsMap.empty () && fGraceEndsMap.empty ())) {
    os << "--------" << std::endl << std::endl;
    printGraceEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

  // cue events
#ifdef MF_TRACE_IS_ENABLED
  if (! (fCueBeginsMap.empty () && fCueEndsMap.empty ())) {
    os << "--------" << std::endl << std::endl;
    printCueEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

  // chord events
#ifdef MF_TRACE_IS_ENABLED
  if (! (fChordBeginsMap.empty () && fChordEndsMap.empty ())) {
    os << "--------" << std::endl << std::endl;
    printChordEvents (os);
  }
#endif // MF_TRACE_IS_ENABLED

  // tuplet events
#ifdef MF_TRACE_IS_ENABLED
  if (! (fTupletBeginsMultiMap.empty () && fTupletEndsMultiMap.empty ())) {
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

