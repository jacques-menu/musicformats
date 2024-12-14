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

#include "mfStringsHandling.h"
#include "mfTraceOah.h"

#include "mxsr2msrEvents.h"

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
  int eventInputEndLineNumber)
{
  fEventSequentialNumber = eventSequentialNumber;

  fEventInputStartLineNumber = eventInputStartLineNumber;
  fEventInputEndLineNumber   = eventInputEndLineNumber;
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
bool compareEventsByIncreasingSequentialNumber (
  S_mxsrEvent& first,
  S_mxsrEvent& second)
{
  return
    first->getEventSequentialNumber ()
      <
    second->getEventSequentialNumber ();
}

bool compareEventsByIncreasingInputStartLineNumber (
  S_mxsrEvent& first,
  S_mxsrEvent& second)
{
  return
    first->getEventInputStartLineNumber ()
      <
    second->getEventInputStartLineNumber ();
}

bool compareStaffChangeEventsByIncreasingInputStartLineNumber (
  S_mxsrStaffChangeEvent& first,
  S_mxsrStaffChangeEvent& second)
{
  return
    first->getEventInputStartLineNumber ()
      <
    second->getEventInputStartLineNumber ();
}

//________________________________________________________________________
/* this class is purely virtual
S_mxsrChordEvent mxsrNoteEvent::create (
  int eventSequentialNumber,
  int noteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
{
  mxsrNoteEvent* obj =
    new mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
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
  int noteSequentialNumber,
  int noteEventStaffNumber,
  int noteEventVoiceNumber,
  int eventInputStartLineNumber,
  int eventInputEndLineNumber)
  : mxsrEvent (
      eventSequentialNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber)
{
  fNoteSequentialNumber = noteSequentialNumber;

  fNoteEventStaffNumber = noteEventStaffNumber;
  fNoteEventVoiceNumber = noteEventVoiceNumber;
}

mxsrNoteEvent::~mxsrNoteEvent ()
{}

//________________________________________________________________________
  S_mxsrStaffChangeEvent mxsrStaffChangeEvent::create (
  int                      eventSequentialNumber,
  int                      noteSequentialNumber,
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
      noteSequentialNumber,
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
  int                      noteSequentialNumber,
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
      noteSequentialNumber,
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
    "[StaffChangeEvent, " <<
    ", fEventInputStartLineNumber: L" << fEventInputStartLineNumber <<
    ", fTakeOffInputStartLineNumber: L" << fTakeOffInputStartLineNumber <<
    ", fLandingInputStartLineNumber: L" << fLandingInputStartLineNumber <<
//     ", fEventInputEndLineNumber: L" << fEventInputEndLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
    ", fNoteEventStaffNumber: S" << fNoteEventStaffNumber <<
    ", fNoteEventVoiceNumber: V" << fNoteEventVoiceNumber <<
    ", fStaffChangeEventKind: " << fStaffChangeEventKind <<
    ", staff number change" <<
       " : S" << fTakeOffStaffNumber << " ->> S" << fLandingStaffNumber <<
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

  const int fieldWidth = 29;
  os << std::left <<
    std::setw (fieldWidth) <<
    "fEventInputStartLineNumber" << " : L" << fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << " : L" << fEventInputEndLineNumber <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << " : E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << " : N" << fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteEventStaffNumber" << " : S" << fNoteEventStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteEventVoiceNumber" << " : V" << fNoteEventVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffChangeEventKind" << " : " << fStaffChangeEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTakeOffInputStartLineNumber" << " : L" << fTakeOffInputStartLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLandingInputStartLineNumber" << " : L" << fLandingInputStartLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTakeOffStaffNumber" << " : S" << fTakeOffStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLandingStaffNumber" << " : S" << fLandingStaffNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "staff change" << " : " <<
     "S" << fTakeOffStaffNumber << "/" << "V" << fNoteEventVoiceNumber <<
     " ->> " <<
     "S" << fLandingStaffNumber << "/" << "V" << fNoteEventVoiceNumber <<
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
S_mxsrChordEvent mxsrChordEvent::create (
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                noteEventStaffNumber,
  int                noteEventVoiceNumber,
  mxsrChordEventKind chordEventKind,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
{
  mxsrChordEvent* obj =
    new mxsrChordEvent (
      eventSequentialNumber,
      noteSequentialNumber,
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
  int                noteSequentialNumber,
  int                noteEventStaffNumber,
  int                noteEventVoiceNumber,
  mxsrChordEventKind chordEventKind,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
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
    "[ChordEvent, " <<
    ", fEventInputStartLineNumber: " << fEventInputStartLineNumber <<
//     ", fEventInputEndLineNumber: " << fEventInputEndLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
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

  const int fieldWidth = 26;
  os << std::left <<
    std::setw (fieldWidth) <<
    "fEventInputStartLineNumber" << " : " << fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << " :L" << fEventInputEndLineNumber <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << " : E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << " : N" << fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteEventStaffNumber" << " : S" << fNoteEventStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteEventVoiceNumber" << " : V" << fNoteEventVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fChordEventKind" << " : " << fChordEventKind <<
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
  int                 noteSequentialNumber,
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
      noteSequentialNumber,
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
  int                 noteSequentialNumber,
  int                 noteEventStaffNumber,
  int                 noteEventVoiceNumber,
  mxsrTupletEventKind tupletEventKind,
  int                 tupletNumber,
  int                 eventInputStartLineNumber,
  int                 eventInputEndLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
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
    "[TupletEvent, " <<
    ", fEventInputStartLineNumber: " << fEventInputStartLineNumber <<
//     ", fEventInputEndLineNumber: " << fEventInputEndLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
    ", fNoteEventStaffNumber: S" << fNoteEventStaffNumber <<
    ", fNoteEventVoiceNumber: V" << fNoteEventVoiceNumber <<
    ", fTupletEventKind: " << fTupletEventKind <<
    ", fTupletNumber: T" << fTupletNumber <<
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

  const int fieldWidth = 26;
  os << std::left <<
    std::setw (fieldWidth) <<
    "fEventInputStartLineNumber" << " : " << fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << " : " << fEventInputEndLineNumber <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << " : E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << " : N" << fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteEventStaffNumber" << " : S" << fNoteEventStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteEventVoiceNumber" << " : V" << fNoteEventVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletEventKind" << " : " << fTupletEventKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletNumber" << " : T" << fTupletNumber <<
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
  int                      noteSequentialNumber,
  int                      noteEventStaffNumber,
  int                      noteEventVoiceNumber,
  int                      takeOffStaffNumber,
  int                      landingStaffNumber,
  int                      takeOffInputStartLineNumber,
  int                      landingInputStartLineNumber,
  int                      eventInputStartLineNumber,
  int                      eventInputEndLineNumber)
{
  S_mxsrStaffChangeEvent
    staffChangeEvent =
      mxsrStaffChangeEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
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
  if (gGlobalMxsr2msrOahGroup->getTraceStaffChangesBasics ()) {
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
    std::make_pair (noteSequentialNumber, staffChangeEvent));

  fStaffChangeEventsList.push_back (staffChangeEvent);
  fAllEventsList.push_back (staffChangeEvent);
}

void mxsrEventsCollection::registerStaffChangeLanding (
  int                      noteSequentialNumber,
  int                      noteEventStaffNumber,
  int                      noteEventVoiceNumber,
  int                      takeOffStaffNumber,
  int                      landingStaffNumber,
  int                      takeOffInputStartLineNumber,
  int                      landingInputStartLineNumber,
  int                      eventInputStartLineNumber,
  int                      eventInputEndLineNumber)
{
  S_mxsrStaffChangeEvent
    staffChangeEvent =
      mxsrStaffChangeEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
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
  if (gGlobalMxsr2msrOahGroup->getTraceStaffChangesBasics ()) {
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
    std::make_pair (noteSequentialNumber, staffChangeEvent));

  fStaffChangeEventsList.push_back (staffChangeEvent);
  fAllEventsList.push_back (staffChangeEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerChordBegin (
  int                noteSequentialNumber,
  int                noteEventStaffNumber,
  int                noteEventVoiceNumber,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
{
  S_mxsrChordEvent
    chordEvent =
      mxsrChordEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrChordEventKind::kEventChordBegin,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceChordsBasics ()) {
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

  fChordsBeginsMap.insert (
    std::make_pair (noteSequentialNumber, chordEvent));

  fAllEventsList.push_back (chordEvent);
}

void mxsrEventsCollection::registerChordEnd (
  int                noteSequentialNumber,
  int                noteEventStaffNumber,
  int                noteEventVoiceNumber,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
{
  S_mxsrChordEvent
    chordEvent =
      mxsrChordEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrChordEventKind::kEventChordEnd,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceChordsBasics ()) {
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

  fChordsEndsMap.insert (
    std::make_pair (noteSequentialNumber, chordEvent));

  fAllEventsList.push_back (chordEvent);
}

//________________________________________________________________________
void mxsrEventsCollection::registerTupletBeginEvent (
  int                 noteSequentialNumber,
  int                 noteEventStaffNumber,
  int                 noteEventVoiceNumber,
  int                 tupletNumber,
  int                 eventInputStartLineNumber,
  int                 eventInputEndLineNumber)
{
  S_mxsrTupletEvent
    tupletEvent =
      mxsrTupletEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrTupletEventKind::kEventTupletBegin,
        tupletNumber,
        eventInputStartLineNumber,
        eventInputEndLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (true || gGlobalMxsr2msrOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering tuplet event " <<
      tupletEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletsBeginsList.push_back (tupletEvent);

  fTupletsEventsMultiMap.insert (
    std::make_pair (noteSequentialNumber, tupletEvent));

  fAllEventsList.push_back (tupletEvent);
}

void mxsrEventsCollection::registerTupletEndEvent (
  int                 noteSequentialNumber,
  int                 noteEventStaffNumber,
  int                 noteEventVoiceNumber,
  int                 tupletNumber,
  int                 eventInputStartLineNumber,
  int                 eventInputEndLineNumber)
{
  S_mxsrTupletEvent
    tupletEvent =
      mxsrTupletEvent::create (
        ++fCurrentEventSequentialNumber,
        noteSequentialNumber,
        noteEventStaffNumber,
        noteEventVoiceNumber,
        mxsrTupletEventKind::kEventTupletEnd,
        tupletNumber,
        eventInputStartLineNumber,
        eventInputEndLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (true || gGlobalMxsr2msrOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> Registering tuplet event " <<
      tupletEvent->asString () <<
      ", line " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletsEndsList.push_back (tupletEvent);

  fTupletsEventsMultiMap.insert (
    std::make_pair (noteSequentialNumber, tupletEvent));

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

  fAllEventsList.sort (
    compareEventsByIncreasingInputStartLineNumber);

  fStaffChangeEventsList.sort (
    compareStaffChangeEventsByIncreasingInputStartLineNumber);
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
S_mxsrChordEvent mxsrEventsCollection::fetchChordBeginAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrChordEvent result;

  std::map <int, S_mxsrChordEvent>::const_iterator it;

  it = fChordsBeginsMap.find (noteSequentialNumber);

  if (it != fChordsBeginsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mxsrChordEvent mxsrEventsCollection::fetchChordEndAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  S_mxsrChordEvent result;

  std::map <int, S_mxsrChordEvent>::const_iterator it;

  it = fChordsEndsMap.find (noteSequentialNumber);

  if (it != fChordsEndsMap.end ()) {
    result = (*it).second;
  }

  return result;
}

//________________________________________________________________________
void mxsrEventsCollection::fetchTupletBeginsList (
  int                            noteSequentialNumber,
  std::list <S_mxsrTupletEvent>& collectedBeginsList)
{
  // look for the first tuplet event matching noteSequentialNumber
  std::list <S_mxsrTupletEvent>::iterator startIt =
    std::find_if (
      std::begin (fTupletsBeginsList),
      std::end (fTupletsBeginsList),
      [&] (const S_mxsrTupletEvent tupletEvent)
        {
          return
            tupletEvent->getNoteSequentialNumber () == noteSequentialNumber;
        });

  if (startIt != fTupletsBeginsList.end ()) {
    gLog << "(*startIt): " << (*startIt) << std::endl << std::flush;

    // look for the first next tuplet event not matching noteSequentialNumber
    std::list <S_mxsrTupletEvent>::iterator endIt =
      std::find_if (
        std::begin (fTupletsBeginsList),
        std::end (fTupletsBeginsList),
        [&] (const S_mxsrTupletEvent tupletEvent )
          {
            return
              tupletEvent->getNoteSequentialNumber () != noteSequentialNumber;
          });

    gLog << "(*endIt): ";
    if (endIt == fTupletsBeginsList.end ()) {
      gLog << "fTupletsBeginsList.end ()";
    }
    else {
      gLog << (*endIt);
    }
    gLog << std::endl << std::endl << std::flush;

    // move the found tuplets events to collectedBeginsList
    collectedBeginsList.splice (
      collectedBeginsList.begin (), fTupletsBeginsList, startIt, endIt);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceTupletsBasics ()) {
    printTupletEventsList (
      gLog,
      noteSequentialNumber,
      collectedBeginsList,
      "fetchTupletBeginsList(), collectedBeginsList:");
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrEventsCollection::fetchTupletEndsList (
  int                            noteSequentialNumber,
  std::list <S_mxsrTupletEvent>& collectedEndsList)
{
  // look for the first tuplet event matching noteSequentialNumber
  std::list <S_mxsrTupletEvent>::iterator startIt =
    std::find_if (
      std::begin (fTupletsEndsList),
      std::end (fTupletsEndsList),
      [&] (const S_mxsrTupletEvent tupletEvent)
        {
          return
            tupletEvent->getNoteSequentialNumber () == noteSequentialNumber;
        });

  if (startIt != fTupletsEndsList.end ()) {
    gLog << "(*startIt): " << (*startIt) << std::endl << std::flush;

    // look for the first next tuplet event not matching noteSequentialNumber
    std::list <S_mxsrTupletEvent>::iterator endIt =
      std::find_if (
        std::begin (fTupletsEndsList),
        std::end (fTupletsEndsList),
        [&] (const S_mxsrTupletEvent tupletEvent )
          {
            return
              tupletEvent->getNoteSequentialNumber () != noteSequentialNumber;
          });

    gLog << "(*endIt): ";
    if (endIt == fTupletsEndsList.end ()) {
      gLog << "fTupletsEndsList.end ()";
    }
    else {
      gLog << (*endIt);
    }
    gLog << std::endl << std::endl << std::flush;

    // move the found tuplets events to collectedEndsList
    collectedEndsList.splice (
      collectedEndsList.begin (), fTupletsEndsList, startIt, endIt);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceTupletsBasics ()) {
    printTupletEventsList (
      gLog,
      noteSequentialNumber,
      collectedEndsList,
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

    ", fChordsBeginsMap.size (): " << fChordsBeginsMap.size () <<
    ", fChordsEndsMap.size (): " << fChordsEndsMap.size () <<

    ", fTupletsBeginsList.size (): " << fTupletsEventsMultiMap.size () <<
    ", fTupletsEndsList.size (): " << fTupletsEventsMultiMap.size () <<
    ", fTupletsEventsMultiMap.size (): " << fTupletsEventsMultiMap.size () <<

    ", fAllEventsList.size (): " << fAllEventsList.size () <<
    ']';

  return ss.str ();
}

std::string mxsrEventsCollection::asString () const
{
  return asShortString ();
}

//------------------------------------------------------------------------
void mxsrEventsCollection::printAllEvents (std::ostream& os) const
{
  os <<
    "fAllEventsList: " <<
    mfSingularOrPlural (
      fAllEventsList.size (),
      "element",
      "elements") <<
    ", sorted by input start line number" <<
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
void mxsrEventsCollection::printChordEvents (std::ostream& os) const
{
  os <<
    "fChordsBeginsMap: " <<
    mfSingularOrPlural (
      fChordsBeginsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrChordEvent> thePair : fChordsBeginsMap) {
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
    "fChordsEndsMap: " <<
    mfSingularOrPlural (
      fChordsEndsMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrChordEvent> thePair : fChordsEndsMap) {
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
    "fTupletsEventsMultiMap: " <<
    mfSingularOrPlural (
      fTupletsEventsMultiMap.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair <int, S_mxsrTupletEvent> thePair : fTupletsEventsMultiMap) {
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
    "fTupletsBeginsList: " <<
    mfSingularOrPlural (
      fTupletsBeginsList.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (S_mxsrTupletEvent tupletEvent : fTupletsBeginsList) {
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
    "fTupletsEndsList: " <<
    mfSingularOrPlural (
      fTupletsEndsList.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    std::endl;

  ++gIndenter;

  for (S_mxsrTupletEvent tupletEvent : fTupletsEndsList) {
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
  const std::list <S_mxsrTupletEvent>& tupletsList,
  const std::string&                   context,
  int                                  inputLineNumber) const
{
  os <<
    "--> printTupletEventsList()" <<
    ", context " <<
    mfSingularOrPlural (
      tupletsList.size (),
      "element",
      "elements") <<
    ", in note sequential number order" <<
    ", line " << inputLineNumber <<
    std::endl;

  ++gIndenter;

  for (S_mxsrTupletEvent tupletEvent : tupletsList) {
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

  const int fieldWidth = 26;
  os << std::left <<
    std::setw (fieldWidth) <<
    "fAllEventsList" << " : " <<
    mfSingularOrPlural (
      fAllEventsList.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffChangeTakeOffsMap" << " : " <<
    mfSingularOrPlural (
      fStaffChangeTakeOffsMap.size (),
      "element",
      "elements") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStaffChangeLandingsMap" << " : " <<
    mfSingularOrPlural (
      fStaffChangeLandingsMap.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fChordsBeginsMap" << " : " <<
    mfSingularOrPlural (
      fChordsBeginsMap.size (),
      "element",
      "elements") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordsEndsMap" << " : " <<
    mfSingularOrPlural (
      fChordsEndsMap.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletsEventsMultiMap" << " : " <<
    mfSingularOrPlural (
      fTupletsEventsMultiMap.size (),
      "element",
      "elements") <<
    std::endl << std::endl;

  os << "--------" << std::endl << std::endl;

  printAllEvents (os);

  os << "--------" << std::endl << std::endl;

  printStaffChangeEvents (os);

  os << "--------" << std::endl << std::endl;

  printChordEvents (os);

  os << "--------" << std::endl << std::endl;

  printTupletEvents (os);

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

