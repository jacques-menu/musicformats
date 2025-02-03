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
// #include "mfTraceOah.h"

#include "mxsr2msrEvents.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
std::string mxsrStaffChangeEventKindAsString (
  mxsrStaffChangeEventKind noteStaffChangeEventKind)
{
  std::string result;

  switch (noteStaffChangeEventKind) {
    case mxsrStaffChangeEventKind::kEventStaffChange_NONE:
      result = "kEventStaffChange_NONE";
      break;
    case mxsrStaffChangeEventKind::kEventStaffChangeTakeOff:
      result = "kEventStaffChangeTakeOff";
      break;
    case mxsrStaffChangeEventKind::kEventStaffChangeLanding:
      result = "kEventStaffChangeLanding";
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
std::string mxsrGraceEventKindAsString (
  mxsrGraceEventKind noteGraceEventKind)
{
  std::string result;

  switch (noteGraceEventKind) {
    case mxsrGraceEventKind::kEventGrace_NONE:
      result = "kEventGrace_NONE";
      break;
    case mxsrGraceEventKind::kEventGraceBegin:
      result = "kEventGraceBegin";
      break;
    case mxsrGraceEventKind::kEventGraceEnd:
      result = "kEventGraceEnd";
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
std::string mxsrCueEventKindAsString (
  mxsrCueEventKind noteCueEventKind)
{
  std::string result;

  switch (noteCueEventKind) {
    case mxsrCueEventKind::kEventCue_NONE:
      result = "kEventCue_NONE";
      break;
    case mxsrCueEventKind::kEventCueBegin:
      result = "kEventCueBegin";
      break;
    case mxsrCueEventKind::kEventCueEnd:
      result = "kEventCueEnd";
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
std::string mxsrChordEventKindAsString (
  mxsrChordEventKind noteChordEventKind)
{
  std::string result;

  switch (noteChordEventKind) {
    case mxsrChordEventKind::kEventChord_NONE:
      result = "kEventChord_NONE";
      break;
    case mxsrChordEventKind::kEventChordBegin:
      result = "kEventChordBegin";
      break;
    case mxsrChordEventKind::kEventChordEnd:
      result = "kEventChordEnd";
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
std::string mxsrTupletEventKindAsString (
  mxsrTupletEventKind noteTupletEventKind)
{
  std::string result;

  switch (noteTupletEventKind) {
    case mxsrTupletEventKind::kEventTuplet_NONE:
      result = "kEventTuplet_NONE";
      break;
    case mxsrTupletEventKind::kEventTupletBegin:
      result = "kEventTupletBegin";
      break;
    case mxsrTupletEventKind::kEventTupletEnd:
      result = "kEventTupletEnd";
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
/* this class is purely virtual
S_mxsrEvent mxsrEvent::create (
  int eventSequentialNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  mxsrEvent* obj =
    new mxsrEvent (
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

mxsrEvent::mxsrEvent (
  int eventSequentialNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber,
  int eventNoteSequentialNumber)
{
  fEventSequentialNumber = eventSequentialNumber;

  fEventInputStartLineNumber = eventInputStartLineNumber;
  fEventInputEndLineNumber   = eventInputEndLineNumber;

  fEventNoteSequentialNumber = eventNoteSequentialNumber;
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
// bool compareEventsByIncreasingInputStartLineNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second)
// {
//   return
//     first->getEventInputStartLineNumber ()
//       <
//     second->getEventInputStartLineNumber ();
// }
//
// bool compareEventsByIncreasingEventNoteSequentialNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second)
// {
//   return
//     first->getEventNoteSequentialNumber ()
//       <
//     second->getEventNoteSequentialNumber ();
// }
//
// bool operator< (
//   const S_mxsrEvent& first,
//   const S_mxsrEvent& second)
// {
//   return
//     first->getEventNoteSequentialNumber ()
//       <
//     second->getEventNoteSequentialNumber ();
// }
//
// bool compareStaffChangeEventsByIncreasingInputStartLineNumber (
//   S_mxsrStaffChangeEvent& first,
//   S_mxsrStaffChangeEvent& second)
// {
//   return
//     first->getEventInputStartLineNumber ()
//       <
//     second->getEventInputStartLineNumber ();
// }

//________________________________________________________________________
/* this class is purely virtual
mxsrNoteEvent mxsrNoteEvent::create (
  int eventSequentialNumber,
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  mxsrNoteEvent* obj =
    new mxsrNoteEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      chordEventKind,
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

mxsrNoteEvent::mxsrNoteEvent (
  int eventSequentialNumber,
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
  : mxsrEvent (
      eventSequentialNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber,
      eventNoteSequentialNumber)
{
  fNoteEventStaffNumber = noteEventStaffNumber;
  fNoteEventVoiceNumber = noteEventVoiceNumber;
}

mxsrNoteEvent::~mxsrNoteEvent ()
{}

//________________________________________________________________________
  S_mxsrStaffChangeEvent mxsrStaffChangeEvent::create (
  int                      eventSequentialNumber,
  int                      eventNoteSequentialNumber,
  int                      noteEventStaffNumber,
  int                      noteEventVoiceNumber,
  mxsrStaffChangeEventKind staffChangeEventKind,
  int                      takeOffStaffNumber,
  int                      landingStaffNumber,
  int                      takeOffInputStartLineNumber,
  int                      landingInputStartLineNumber,
  int                      eventInputStartLineNumber,
  int                      eventInputEndLineNumber)
{
  mxsrStaffChangeEvent* obj =
    new mxsrStaffChangeEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      staffChangeEventKind,
      takeOffStaffNumber,
      landingStaffNumber,
      takeOffInputStartLineNumber,
      landingInputStartLineNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrStaffChangeEvent::mxsrStaffChangeEvent (
  int                      eventSequentialNumber,
  int                      eventNoteSequentialNumber,
  int                      noteEventStaffNumber,
  int                      noteEventVoiceNumber,
  mxsrStaffChangeEventKind staffChangeEventKind,
  int                      takeOffStaffNumber,
  int                      landingStaffNumber,
  int                      takeOffInputStartLineNumber,
  int                      landingInputStartLineNumber,
  int                      eventInputStartLineNumber,
  int                      eventInputEndLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber)
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

  fTakeOffInputStartLineNumber = takeOffInputStartLineNumber;
  fLandingInputStartLineNumber = landingInputStartLineNumber;
}

mxsrStaffChangeEvent::~mxsrStaffChangeEvent ()
{}

std::string mxsrStaffChangeEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[StaffChangeEvent" <<
    ", fEventInputStartLineNumber: L" << fEventInputStartLineNumber <<
    ", fTakeOffInputStartLineNumber: L" << fTakeOffInputStartLineNumber <<
    ", fLandingInputStartLineNumber: L" << fLandingInputStartLineNumber <<
//     ", fEventInputEndLineNumber: L" << fEventInputEndLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fEventNoteSequentialNumber: N" << fEventNoteSequentialNumber <<
    ", fNoteEventStaffNumber: S" << fNoteEventStaffNumber <<
    ", fNoteEventVoiceNumber: V" << fNoteEventVoiceNumber <<
    ", fStaffChangeEventKind: " << fStaffChangeEventKind <<
    ", staff number change" <<
       ": S" << fTakeOffStaffNumber << " ->> S" << fLandingStaffNumber <<
    ", fLandingInputStartLineNumber: L" << fLandingInputStartLineNumber <<
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
    "fEventInputStartLineNumber" << ": L" << fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << ": L" << fEventInputEndLineNumber <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "staff change" << ": " <<
     "S" << fTakeOffStaffNumber << "/" << "V" << fNoteEventVoiceNumber <<
     " ->> " <<
     "S" << fLandingStaffNumber << "/" << "V" << fNoteEventVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventNoteSequentialNumber" << ": N" << fEventNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteEventStaffNumber" << ": S" << fNoteEventStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteEventVoiceNumber" << ": V" << fNoteEventVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffChangeEventKind" << ": " << fStaffChangeEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTakeOffInputStartLineNumber" << ": L" << fTakeOffInputStartLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLandingInputStartLineNumber" << ": L" << fLandingInputStartLineNumber <<
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
S_mxsrGraceEvent mxsrGraceEvent::create (
  int                eventSequentialNumber,
  int                eventNoteSequentialNumber,
  int                noteEventStaffNumber,
  int                noteEventVoiceNumber,
  mxsrGraceEventKind graceEventKind,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
{
  mxsrGraceEvent* obj =
    new mxsrGraceEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      graceEventKind,
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrGraceEvent::mxsrGraceEvent (
  int                eventSequentialNumber,
  int                eventNoteSequentialNumber,
  int                noteEventStaffNumber,
  int                noteEventVoiceNumber,
  mxsrGraceEventKind graceEventKind,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber)
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
    ", fEventInputStartLineNumber: " << fEventInputStartLineNumber <<
//     ", fEventInputEndLineNumber: " << fEventInputEndLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fEventNoteSequentialNumber: N" << fEventNoteSequentialNumber <<
    ", fNoteEventStaffNumber: S" << fNoteEventStaffNumber <<
    ", fNoteEventVoiceNumber: V" << fNoteEventVoiceNumber <<
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
    "fEventInputStartLineNumber" << ": " << fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << " :L" << fEventInputEndLineNumber <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fGraceEventKind" << ": " << fGraceEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteEventStaffNumber" << ": S" << fNoteEventStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteEventVoiceNumber" << ": V" << fNoteEventVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventNoteSequentialNumber" << ": N" << fEventNoteSequentialNumber <<
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
S_mxsrCueEvent mxsrCueEvent::create (
  int              eventSequentialNumber,
  int              eventNoteSequentialNumber,
  int              noteEventStaffNumber,
  int              noteEventVoiceNumber,
  mxsrCueEventKind cueEventKind,
  int              eventInputStartLineNumber,
  int              eventInputEndLineNumber)
{
  mxsrCueEvent* obj =
    new mxsrCueEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      cueEventKind,
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrCueEvent::mxsrCueEvent (
  int              eventSequentialNumber,
  int              eventNoteSequentialNumber,
  int              noteEventStaffNumber,
  int              noteEventVoiceNumber,
  mxsrCueEventKind cueEventKind,
  int              eventInputStartLineNumber,
  int              eventInputEndLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber)
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
    ", fEventInputStartLineNumber: " << fEventInputStartLineNumber <<
//     ", fEventInputEndLineNumber: " << fEventInputEndLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fEventNoteSequentialNumber: N" << fEventNoteSequentialNumber <<
    ", fNoteEventStaffNumber: S" << fNoteEventStaffNumber <<
    ", fNoteEventVoiceNumber: V" << fNoteEventVoiceNumber <<
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
    "fEventInputStartLineNumber" << ": " << fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << " :L" << fEventInputEndLineNumber <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fCueEventKind" << ": " << fCueEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventNoteSequentialNumber" << ": N" << fEventNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteEventStaffNumber" << ": S" << fNoteEventStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteEventVoiceNumber" << ": V" << fNoteEventVoiceNumber <<
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
S_mxsrChordEvent mxsrChordEvent::create (
  int                eventSequentialNumber,
  int                eventNoteSequentialNumber,
  int                noteEventStaffNumber,
  int                noteEventVoiceNumber,
  mxsrChordEventKind chordEventKind,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
{
  mxsrChordEvent* obj =
    new mxsrChordEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      chordEventKind,
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrChordEvent::mxsrChordEvent (
  int                eventSequentialNumber,
  int                eventNoteSequentialNumber,
  int                noteEventStaffNumber,
  int                noteEventVoiceNumber,
  mxsrChordEventKind chordEventKind,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber)
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
    ", fEventInputStartLineNumber: " << fEventInputStartLineNumber <<
//     ", fEventInputEndLineNumber: " << fEventInputEndLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fEventNoteSequentialNumber: N" << fEventNoteSequentialNumber <<
    ", fNoteEventStaffNumber: S" << fNoteEventStaffNumber <<
    ", fNoteEventVoiceNumber: V" << fNoteEventVoiceNumber <<
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
    "fEventInputStartLineNumber" << ": " << fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << " :L" << fEventInputEndLineNumber <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fChordEventKind" << ": " << fChordEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventNoteSequentialNumber" << ": N" << fEventNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteEventStaffNumber" << ": S" << fNoteEventStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteEventVoiceNumber" << ": V" << fNoteEventVoiceNumber <<
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
S_mxsrTupletEvent mxsrTupletEvent::create (
  int                 eventSequentialNumber,
  int                 eventNoteSequentialNumber,
  int                 noteEventStaffNumber,
  int                 noteEventVoiceNumber,
  mxsrTupletEventKind tupletEventKind,
  int                 tupletNumber,
  int                 eventInputStartLineNumber,
  int                 eventInputEndLineNumber)
{
  mxsrTupletEvent* obj =
    new mxsrTupletEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      tupletEventKind,
      tupletNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrTupletEvent::mxsrTupletEvent (
  int                 eventSequentialNumber,
  int                 eventNoteSequentialNumber,
  int                 noteEventStaffNumber,
  int                 noteEventVoiceNumber,
  mxsrTupletEventKind tupletEventKind,
  int                 tupletNumber,
  int                 eventInputStartLineNumber,
  int                 eventInputEndLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      eventNoteSequentialNumber,
      noteEventStaffNumber,
      noteEventVoiceNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber)
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
    ", fEventInputStartLineNumber: L " << fEventInputStartLineNumber <<
//     ", fEventInputEndLineNumber: " << fEventInputEndLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fEventNoteSequentialNumber: N" << fEventNoteSequentialNumber <<
    ", fNoteEventStaffNumber: S" << fNoteEventStaffNumber <<
    ", fNoteEventVoiceNumber: V" << fNoteEventVoiceNumber <<
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
    "fEventInputStartLineNumber" << ": L" << fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << ": L" << fEventInputEndLineNumber <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventNoteSequentialNumber" << ": N" << fEventNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteEventStaffNumber" << ": S" << fNoteEventStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteEventVoiceNumber" << ": V" << fNoteEventVoiceNumber <<
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
void mxsrEventsCollection::registerStaffChangeTakeOff (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int takeOffStaffNumber,
  int landingStaffNumber,
  int takeOffInputStartLineNumber,
  int landingInputStartLineNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrStaffChangeEvent
    staffChangeEvent =
      mxsrStaffChangeEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrStaffChangeEventKind::kEventStaffChangeTakeOff,
        takeOffStaffNumber,
        landingStaffNumber,
        takeOffInputStartLineNumber,
        landingInputStartLineNumber,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering staff change event " <<
      staffChangeEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffChangeTakeOffsMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, staffChangeEvent));

  fStaffChangeEventsList.push_back (staffChangeEvent);

  fAllEventsList.push_back (staffChangeEvent);
}

void mxsrEventsCollection::registerStaffChangeLanding (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int takeOffStaffNumber,
  int landingStaffNumber,
  int takeOffInputStartLineNumber,
  int landingInputStartLineNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrStaffChangeEvent
    staffChangeEvent =
      mxsrStaffChangeEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrStaffChangeEventKind::kEventStaffChangeLanding,
        takeOffStaffNumber,
        landingStaffNumber,
        takeOffInputStartLineNumber,
        landingInputStartLineNumber,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering staff change event " <<
      staffChangeEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffChangeLandingsMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, staffChangeEvent));

  fStaffChangeEventsList.push_back (staffChangeEvent);

  fAllEventsList.push_back (staffChangeEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerGraceBegin (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrGraceEvent
    graceEvent =
      mxsrGraceEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrGraceEventKind::kEventGraceBegin,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering grace note event " <<
      graceEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fGraceBeginsMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, graceEvent));

  fAllEventsList.push_back (graceEvent);
}

void mxsrEventsCollection::registerGraceEnd (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrGraceEvent
    graceEvent =
      mxsrGraceEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrGraceEventKind::kEventGraceEnd,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering grace note event " <<
      graceEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fGraceEndsMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, graceEvent));

  fAllEventsList.push_back (graceEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerCueBegin (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrCueEvent
    cueEvent =
      mxsrCueEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrCueEventKind::kEventCueBegin,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering cue note event " <<
      cueEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCueBeginsMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, cueEvent));

  fAllEventsList.push_back (cueEvent);
}

void mxsrEventsCollection::registerCueEnd (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrCueEvent
    cueEvent =
      mxsrCueEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrCueEventKind::kEventCueEnd,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCueNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering cue note event " <<
      cueEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCueEndsMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, cueEvent));

  fAllEventsList.push_back (cueEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerChordBegin (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrChordEvent
    chordEvent =
      mxsrChordEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrChordEventKind::kEventChordBegin,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering chord event " <<
      chordEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordBeginsMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, chordEvent));

  fAllEventsList.push_back (chordEvent);
}

void mxsrEventsCollection::registerChordEnd (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrChordEvent
    chordEvent =
      mxsrChordEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrChordEventKind::kEventChordEnd,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering chord event " <<
      chordEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordEndsMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, chordEvent));

  fAllEventsList.push_back (chordEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerTupletBeginEvent (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int tupletNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrTupletEvent
    tupletEvent =
      mxsrTupletEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrTupletEventKind::kEventTupletBegin,
        tupletNumber,
        eventInputStartLineNumber,
        eventInputEndLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering tuplet begin event " <<
      tupletEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // push tupletEvent onto fTupletBeginsList VITAL
  fTupletBeginsList.push_front (tupletEvent);

  fTupletEventsMultiMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, tupletEvent));

  fAllEventsList.push_back (tupletEvent);
}

void mxsrEventsCollection::registerTupletEndEvent (
  int eventNoteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int tupletNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  S_mxsrTupletEvent
    tupletEvent =
      mxsrTupletEvent::create (
        ++fCurrentEventSequentialNumber,
        eventNoteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrTupletEventKind::kEventTupletEnd,
        tupletNumber,
        eventInputStartLineNumber,
        eventInputEndLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering tuplet end event " <<
      tupletEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletEndsList.push_back (tupletEvent);

  fTupletEventsMultiMap.insert (
    std::make_pair (
      eventNoteSequentialNumber, tupletEvent));

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
  int eventNoteSequentialNumber) const
{
  S_mxsrStaffChangeEvent result;

  std::map <int, S_mxsrStaffChangeEvent>::const_iterator it;

  it = fStaffChangeTakeOffsMap.find (eventNoteSequentialNumber);

  if (it != fStaffChangeTakeOffsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrStaffChangeEvent mxsrEventsCollection::fetchStaffChangeLandingAtNoteSequentialNumber (
  int eventNoteSequentialNumber) const
{
  S_mxsrStaffChangeEvent result;

  std::map <int, S_mxsrStaffChangeEvent>::const_iterator it;

  it = fStaffChangeLandingsMap.find (eventNoteSequentialNumber);

  if (it != fStaffChangeLandingsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
S_mxsrGraceEvent mxsrEventsCollection::fetchGraceBeginAtNoteSequentialNumber (
  int eventNoteSequentialNumber) const
{
  S_mxsrGraceEvent result;

  std::map <int, S_mxsrGraceEvent>::const_iterator it;

  it = fGraceBeginsMap.find (eventNoteSequentialNumber);

  if (it != fGraceBeginsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrGraceEvent mxsrEventsCollection::fetchGraceEndAtNoteSequentialNumber (
  int eventNoteSequentialNumber) const
{
  S_mxsrGraceEvent result;

  std::map <int, S_mxsrGraceEvent>::const_iterator it;

  it = fGraceEndsMap.find (eventNoteSequentialNumber);

  if (it != fGraceEndsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
S_mxsrCueEvent mxsrEventsCollection::fetchCueBeginAtNoteSequentialNumber (
  int eventNoteSequentialNumber) const
{
  S_mxsrCueEvent result;

  std::map <int, S_mxsrCueEvent>::const_iterator it;

  it = fCueBeginsMap.find (eventNoteSequentialNumber);

  if (it != fCueBeginsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrCueEvent mxsrEventsCollection::fetchCueEndAtNoteSequentialNumber (
  int eventNoteSequentialNumber) const
{
  S_mxsrCueEvent result;

  std::map <int, S_mxsrCueEvent>::const_iterator it;

  it = fCueEndsMap.find (eventNoteSequentialNumber);

  if (it != fCueEndsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
S_mxsrChordEvent mxsrEventsCollection::fetchChordBeginAtNoteSequentialNumber (
  int eventNoteSequentialNumber) const
{
  S_mxsrChordEvent result;

  std::map <int, S_mxsrChordEvent>::const_iterator
    it =
      fChordBeginsMap.find (eventNoteSequentialNumber);

  if (it != fChordBeginsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrChordEvent mxsrEventsCollection::fetchChordEndAtNoteSequentialNumber (
  int eventNoteSequentialNumber) const
{
  S_mxsrChordEvent result;

  std::map <int, S_mxsrChordEvent>::const_iterator
    it =
      fChordEndsMap.find (eventNoteSequentialNumber);

  if (it != fChordEndsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
void mxsrEventsCollection::fetchTupletBeginsList (
  int                            eventNoteSequentialNumber,
  std::list <S_mxsrTupletEvent>& recipientTupletBeginsList)
{
  // look for the first tuplet event matching eventNoteSequentialNumber
  std::list <S_mxsrTupletEvent>::iterator startIt =
    std::find_if (
      std::begin (fTupletBeginsList),
      std::end (fTupletBeginsList),
      [&] (const S_mxsrTupletEvent tupletEvent)
        {
          return
            tupletEvent->getEventNoteSequentialNumber () == eventNoteSequentialNumber;
        });

  if (startIt != fTupletBeginsList.end ()) {
//     gLog << "(*startIt): " << (*startIt) << std::endl << std::flush;

    // look for the first next tuplet event not matching eventNoteSequentialNumber
    std::list <S_mxsrTupletEvent>::iterator endIt =
      std::find_if (
        std::next (startIt),
        std::end (fTupletBeginsList),
        [&] (const S_mxsrTupletEvent tupletEvent )
          {
            return
              tupletEvent->getEventNoteSequentialNumber () != eventNoteSequentialNumber;
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
  int                            eventNoteSequentialNumber,
  std::list <S_mxsrTupletEvent>& recipientTupletEndsList)
{
  // look for the first tuplet event matching eventNoteSequentialNumber
  std::list <S_mxsrTupletEvent>::iterator startIt =
    std::find_if (
      std::begin (fTupletEndsList),
      std::end (fTupletEndsList),
      [&] (const S_mxsrTupletEvent tupletEvent)
        {
          return
            tupletEvent->getEventNoteSequentialNumber () == eventNoteSequentialNumber;
        });

  if (startIt != fTupletEndsList.end ()) {
//     gLog << "(*startIt): " << (*startIt) << std::endl << std::flush;

    // look for the first next tuplet event not matching eventNoteSequentialNumber
    std::list <S_mxsrTupletEvent>::iterator endIt =
      std::find_if (
        std::next (startIt),
        std::end (fTupletEndsList),
        [&] (const S_mxsrTupletEvent tupletEvent )
          {
            return
              tupletEvent->getEventNoteSequentialNumber () != eventNoteSequentialNumber;
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

// #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    os << "--------" << std::endl << std::endl;
    printChordEvents (os);
  }
// #endif // MF_TRACE_IS_ENABLED

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

