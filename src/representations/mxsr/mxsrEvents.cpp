/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cassert>
#include <iostream>
#include <sstream>

#include "mfStringsHandling.h"
#include "mfTraceOah.h"

#include "mxsrEvents.h"

namespace MusicFormats
{

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

//________________________________________________________________________
/* this class is purely virtual
S_mxsrChordEvent mxsrNoteEvent::create (
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                staffNumber,
  int                voiceNumber,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
{
  mxsrNoteEvent* obj =
    new mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      staffNumber,
      voiceNumber,
      chordEventKind,
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

mxsrNoteEvent::mxsrNoteEvent (
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                staffNumber,
  int                voiceNumber,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
  : mxsrEvent (
      eventSequentialNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber)
{
  fNoteSequentialNumber = noteSequentialNumber;

  fStaffNumber = staffNumber;
  fVoiceNumber = voiceNumber;
}

mxsrNoteEvent::~mxsrNoteEvent ()
{}

//________________________________________________________________________
S_mxsrChordEvent mxsrChordEvent::create (
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                staffNumber,
  int                voiceNumber,
  mxsrChordEventKind chordEventKind,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
{
  mxsrChordEvent* obj =
    new mxsrChordEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      staffNumber,
      voiceNumber,
      chordEventKind,
      eventInputStartLineNumber,
      eventInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrChordEvent::mxsrChordEvent (
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                staffNumber,
  int                voiceNumber,
  mxsrChordEventKind chordEventKind,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      staffNumber,
      voiceNumber,
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
    ", fStaffNumber: S" << fStaffNumber <<
    ", fVoiceNumber: V" << fVoiceNumber <<
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
    "fStaffNumber" << " : S" << fStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceNumber" << " : V" << fVoiceNumber <<
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
  int                 staffNumber,
  int                 voiceNumber,
  mxsrTupletEventKind tupletEventKind,
  int                 tupletNumber,
  int                 eventInputStartLineNumber,
  int                 eventInputEndLineNumber)
{
  mxsrTupletEvent* obj =
    new mxsrTupletEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      staffNumber,
      voiceNumber,
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
  int                 staffNumber,
  int                 voiceNumber,
  mxsrTupletEventKind tupletEventKind,
  int                 tupletNumber,
  int                 eventInputStartLineNumber,
  int                 eventInputEndLineNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      staffNumber,
      voiceNumber,
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
    ", fStaffNumber: S" << fStaffNumber <<
    ", fVoiceNumber: V" << fVoiceNumber <<
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
    "fStaffNumber" << " : S" << fStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceNumber" << " : V" << fVoiceNumber <<
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
S_mxsrStaffChangeEvent mxsrStaffChangeEvent::create (
  int                      eventSequentialNumber,
  int                      noteSequentialNumber,
  int                      staffNumber,
  int                      voiceNumber,
  mxsrStaffChangeEventKind staffChangeEventKind,
  int                      eventInputStartLineNumber,
  int                      eventInputEndLineNumber,
  int                      fromStaffNumber,
  int                      toStaffNumber)
{
  mxsrStaffChangeEvent* obj =
    new mxsrStaffChangeEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      staffNumber,
      voiceNumber,
      staffChangeEventKind,
      eventInputStartLineNumber,
      eventInputEndLineNumber,
      fromStaffNumber,
      toStaffNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrStaffChangeEvent::mxsrStaffChangeEvent (
  int                      eventSequentialNumber,
  int                      noteSequentialNumber,
  int                      staffNumber,
  int                      voiceNumber,
  mxsrStaffChangeEventKind staffChangeEventKind,
  int                      eventInputStartLineNumber,
  int                      eventInputEndLineNumber,
  int                      fromStaffNumber,
  int                      toStaffNumber)
  : mxsrNoteEvent (
      eventSequentialNumber,
      noteSequentialNumber,
      staffNumber,
      voiceNumber,
      eventInputStartLineNumber,
      eventInputEndLineNumber)
{
  fStaffChangeEventKind = staffChangeEventKind;

  fFromStaffNumber = fromStaffNumber;
  fToStaffNumber   = toStaffNumber;
}

mxsrStaffChangeEvent::~mxsrStaffChangeEvent ()
{}

std::string mxsrStaffChangeEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[StaffChangeEvent, " <<
    ", fEventInputStartLineNumber: " << fEventInputStartLineNumber <<
//     ", fEventInputEndLineNumber: " << fEventInputEndLineNumber <<
    ", fEventSequentialNumber: E" << fEventSequentialNumber <<
    ", fNoteSequentialNumber: N" << fNoteSequentialNumber <<
    ", fStaffNumber: S" << fStaffNumber <<
    ", fVoiceNumber: V" << fVoiceNumber <<
    ", fStaffChangeEventKind: " << fStaffChangeEventKind <<
    ", fFromStaffNumber: fS" << fFromStaffNumber <<
    ", fToStaffNumber: tS" << fToStaffNumber <<
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

  const int fieldWidth = 26;
  os << std::left <<
    std::setw (fieldWidth) <<
    "fEventInputStartLineNumber" << " : " << fEventInputStartLineNumber <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     std::setw (fieldWidth) <<
//     "fEventInputEndLineNumber" << " : " << fEventInputEndLineNumber <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << " : E" << fEventSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << " : N" << fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffNumber" << " : S" << fStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceNumber" << " : V" << fVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffChangeEventKind" << " : " << fStaffChangeEventKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "staff number change" << " : S" << fFromStaffNumber << " ->> S" << fToStaffNumber <<
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
S_mxsrEventsCollection mxsrEventsCollection::create ()
{
  mxsrEventsCollection* obj =
    new mxsrEventsCollection ();
  assert (obj != nullptr);
  return obj;
}

mxsrEventsCollection::mxsrEventsCollection ()
{
}

mxsrEventsCollection::~mxsrEventsCollection ()
{}

void mxsrEventsCollection::registerChordEvent (
  int                eventSequentialNumber,
  int                noteSequentialNumber,
  int                staffNumber,
  int                voiceNumber,
  mxsrChordEventKind chordEventKind,
  int                eventInputStartLineNumber,
  int                eventInputEndLineNumber)
{
  S_mxsrChordEvent
    chordEvent =
      mxsrChordEvent::create (
        eventSequentialNumber,
        noteSequentialNumber,
        staffNumber,
        voiceNumber,
        chordEventKind,
        eventInputStartLineNumber,
        eventInputStartLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceChordsBasics ()) {
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

  fChordsEventsMultiMap.insert (
    std::make_pair (eventSequentialNumber, chordEvent));

  fAllEventsList.push_back (chordEvent);
}

void mxsrEventsCollection::registerTupletEvent (
  int                 eventSequentialNumber,
  int                 noteSequentialNumber,
  int                 staffNumber,
  int                 voiceNumber,
  mxsrTupletEventKind tupletEventKind,
  int                 tupletNumber,
  int                 eventInputStartLineNumber,
  int                 eventInputEndLineNumber)
{
  S_mxsrTupletEvent
    tupletEvent =
      mxsrTupletEvent::create (
        eventSequentialNumber,
        noteSequentialNumber,
        staffNumber,
        voiceNumber,
        tupletEventKind,
        tupletNumber,
        eventInputStartLineNumber,
        eventInputEndLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceTupletsBasics ()) {
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

//   fTupletsEventsMultiMap [eventSequentialNumber] = tupletEvent;
  fTupletsEventsMultiMap.insert (
    std::make_pair (eventSequentialNumber, tupletEvent));

  fAllEventsList.push_back (tupletEvent);
}

void mxsrEventsCollection::registerStaffChangeEvent (
  int                      eventSequentialNumber,
  int                      noteSequentialNumber,
  int                      staffNumber,
  int                      voiceNumber,
  mxsrStaffChangeEventKind staffChangeEventKind,
  int                      eventInputStartLineNumber,
  int                      eventInputEndLineNumber,
  int                      fromStaffNumber,
  int                      toStaffNumber)
{
  S_mxsrStaffChangeEvent
    staffChangeEvent =
      mxsrStaffChangeEvent::create (
        eventSequentialNumber,
        noteSequentialNumber,
        staffNumber,
        voiceNumber,
        staffChangeEventKind,
        eventInputStartLineNumber,
        eventInputStartLineNumber,
        fromStaffNumber,
        toStaffNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceStaffChangesBasics ()) {
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

//   fStaffChangesEventMultiMap [eventSequentialNumber] = staffChangeEvent;
  fStaffChangesEventMultiMap.insert (
    std::make_pair (eventSequentialNumber, staffChangeEvent));

  fAllEventsList.push_back (staffChangeEvent);
}

void mxsrEventsCollection::sortTheAllMxsrEventsList ()
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
}

mxsrChordEventKind mxsrEventsCollection::chordEventAtNoteSequentialNumber (
  int eventSequentialNumber) const
{
  mxsrChordEventKind
    result =
      mxsrChordEventKind::kEventChord_NONE;

  std::multimap<int, S_mxsrChordEvent>::const_iterator it;

  it = fChordsEventsMultiMap.find (eventSequentialNumber);

  if (it != fChordsEventsMultiMap.end ()) {
    result =
      it->second->getChordEventKind ();
  }

  return result;
}

mxsrTupletEventKind mxsrEventsCollection::tupletEventAtNoteSequentialNumber (
  int eventSequentialNumber) const
{
  mxsrTupletEventKind
    result =
      mxsrTupletEventKind::kEventTuplet_NONE;

  std::multimap<int, S_mxsrTupletEvent>::const_iterator it;

  it = fTupletsEventsMultiMap.find (eventSequentialNumber);

  if (it != fTupletsEventsMultiMap.end ()) {
    result =
      it->second->getTupletEventKind ();
  }

  return result;
}

mxsrStaffChangeEventKind mxsrEventsCollection::staffChangeEventAtNoteSequentialNumber (
  int eventSequentialNumber) const
{
  mxsrStaffChangeEventKind
    result =
      mxsrStaffChangeEventKind::kEventStaffChange_NONE;

  std::multimap<int, S_mxsrStaffChangeEvent>::const_iterator it;

  it = fStaffChangesEventMultiMap.find (eventSequentialNumber);

  if (it != fStaffChangesEventMultiMap.end ()) {
    result =
      it->second->getStaffChangeEventKind ();
  }

  return result;
}

std::string mxsrEventsCollection::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[EventsCollection" <<
    ", fChordsEventsMultiMap.size (): " << fChordsEventsMultiMap.size () <<
    ", fTupletsEventsMultiMap.size (): " << fTupletsEventsMultiMap.size () <<
    ", fStaffChangesEventMultiMap.size (): " << fStaffChangesEventMultiMap.size () <<
    ", fAllEventsList.size (): " << fAllEventsList.size () <<
    ", fAllEventsList: " <<
    ']';

  return ss.str ();
}

std::string mxsrEventsCollection::asString () const
{
  return asShortString ();
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
  os <<
    std::setw (fieldWidth) <<
    "fAllEventsList" << " : " <<
    mfSingularOrPlural (
      fAllEventsList.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fChordsEventsMultiMap" << " : " <<
    mfSingularOrPlural (
      fChordsEventsMultiMap.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletsEventsMultiMap" << " : " <<
    mfSingularOrPlural (
      fTupletsEventsMultiMap.size (),
      "element",
      "elements") <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffChangesEventMultiMap" << " : " <<
    mfSingularOrPlural (
      fStaffChangesEventMultiMap.size (),
      "element",
      "elements") <<
    std::endl << std::endl;

  os << "--------" << std::endl << std::endl;

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
      event->getEventSequentialNumber () <<
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
    "fChordsEventsMultiMap: " <<
    mfSingularOrPlural (
      fChordsEventsMultiMap.size (),
      "element",
      "elements") <<
    ", in event sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair<int, S_mxsrChordEvent> thePair : fChordsEventsMultiMap) {
    int
      eventSequentialNumber = thePair.first;
    S_mxsrChordEvent
      chordEvent = thePair.second;

    os <<
      eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      chordEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << "--------" << std::endl << std::endl;

  os <<
    "fTupletsEventsMultiMap: " <<
    mfSingularOrPlural (
      fTupletsEventsMultiMap.size (),
      "element",
      "elements") <<
    ", in event sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair<int, S_mxsrTupletEvent> thePair : fTupletsEventsMultiMap) {
    int
      eventSequentialNumber = thePair.first;
    S_mxsrTupletEvent
      tupletEvent = thePair.second;

    os <<
      eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      tupletEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

  os << "--------" << std::endl << std::endl;

  os <<
    "fStaffChangesEventMultiMap: " <<
    mfSingularOrPlural (
      fStaffChangesEventMultiMap.size (),
      "element",
      "elements") <<
    ", in event sequential number order" <<
    std::endl;

  ++gIndenter;

  for (std::pair<int, S_mxsrStaffChangeEvent> thePair : fStaffChangesEventMultiMap) {
    int
      eventSequentialNumber = thePair.first;
    S_mxsrStaffChangeEvent
      staffChangeEvent = thePair.second;

    os <<
      eventSequentialNumber <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      staffChangeEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;

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

