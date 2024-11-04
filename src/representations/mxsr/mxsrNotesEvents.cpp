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

#include "mxsrNotesEvents.h"

namespace MusicFormats
{

//________________________________________________________________________
std::string mxsrNoteEventKindAsString (
  mxsrNoteEventKind noteEventKind)
{
  std::string result;

  switch (noteEventKind) {
    case mxsrNoteEventKind::kNoteEventChordBegin:
      result = "kNoteEventChordBegin";
      break;
    case mxsrNoteEventKind::kNoteEventChordEnd:
      result = "kNoteEventChordEnd";
      break;

    case mxsrNoteEventKind::kNoteEventTupletBegin:
      result = "kNoteEventTupletBegin";
      break;
    case mxsrNoteEventKind::kNoteEventTupletEnd:
      result = "kNoteEventTupletEnd";
      break;

    case mxsrNoteEventKind::kNoteEventStaffChangeTakeOff:
      result = "kNoteEventStaffChangeTakeOff";
      break;
    case mxsrNoteEventKind::kNoteEventStaffChangeLanding:
      result = "kNoteEventStaffChangeLanding";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrNoteEventKind& elt)
{
  os << mxsrNoteEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrNoteEvent mxsrNoteEvent::create (
  mxsrNoteEventKind noteEventKind,
  int               noteSequentialNumber,
  int               noteInputStartLineNumber,
  int               noteInputEndLineNumber)
{
  mxsrNoteEvent* obj =
    new mxsrNoteEvent (
      noteEventKind,
      noteSequentialNumber,
      noteInputStartLineNumber,
      noteInputEndLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrNoteEvent::mxsrNoteEvent (
  mxsrNoteEventKind noteEventKind,
  int               noteSequentialNumber,
  int               noteInputStartLineNumber,
  int               noteInputEndLineNumber)
{
  fNoteEventKind            = noteEventKind;

  fNoteSequentialNumber     = noteSequentialNumber;

  fNoteInputStartLineNumber = noteInputStartLineNumber;
  fNoteInputEndLineNumber   = noteInputEndLineNumber;
}

mxsrNoteEvent::~mxsrNoteEvent ()
{}

std::string mxsrNoteEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[NoteEvent, " <<
    ", fNoteEventKind: " << fNoteEventKind <<
    ", fNoteSequentialNumber: " << fNoteSequentialNumber <<
    ", fNoteInputStartLineNumber: " << fNoteInputStartLineNumber <<
    ", fNoteInputEndLineNumber: " << fNoteInputEndLineNumber <<
    ']';

  return ss.str ();
}

std::string mxsrNoteEvent::asString () const
{
  return asShortString ();
}

void mxsrNoteEvent::print (std::ostream& os) const
{
  os <<
    "[NoteEvent" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 26;
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteEventKind" << " : " << fNoteEventKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << " : " << fNoteSequentialNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteInputStartLineNumber" << " : " << fNoteInputStartLineNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteInputEndLineNumber" << " : " << fNoteInputEndLineNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrNoteEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrNoteEvent& elt)
{
  elt.print (os);
  return os;
}

//________________________________________________________________________
bool compareFrameNotesByIncreasingSequentialNumber (
  S_mxsrNoteEvent& first,
  S_mxsrNoteEvent& second)
{
  return
    first->getNoteSequentialNumber ()
      <
    second->getNoteSequentialNumber ();
}

//________________________________________________________________________
S_mxsrScoreNotesEvents mxsrScoreNotesEvents::create ()
{
  mxsrScoreNotesEvents* obj =
    new mxsrScoreNotesEvents ();
  assert (obj != nullptr);
  return obj;
}

mxsrScoreNotesEvents::mxsrScoreNotesEvents ()
{
}

mxsrScoreNotesEvents::~mxsrScoreNotesEvents ()
{}

void mxsrScoreNotesEvents::registerChordBeginEvent (
  int noteSequentialNumber,
  int noteInputStartLineNumber,
  int noteInputEndLineNumber)
{
  S_mxsrNoteEvent
    theNoteEvent =
      mxsrNoteEvent::create (
        mxsrNoteEventKind::kNoteEventChordBegin,
        noteSequentialNumber,
        noteInputStartLineNumber,
        noteInputStartLineNumber);

  fChordsBeginNotesEventsMap [noteSequentialNumber] = theNoteEvent;

  fAllNotesEventsList.push_back (theNoteEvent);

#ifdef MF_TRACE_IS_ENABLED
  if (true || gGlobalMxsrOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> A chord begins upon note at " <<
      theNoteEvent->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrScoreNotesEvents::registerChordEndEvent (
  int noteSequentialNumber,
  int noteInputStartLineNumber,
  int noteInputEndLineNumber)
{
  S_mxsrNoteEvent
    theNoteEvent =
      mxsrNoteEvent::create (
        mxsrNoteEventKind::kNoteEventChordEnd,
        noteSequentialNumber,
        noteInputStartLineNumber,
        noteInputStartLineNumber);

  fChordsEndNotesEventsMap [noteSequentialNumber] = theNoteEvent;

  fAllNotesEventsList.push_back (theNoteEvent);

#ifdef MF_TRACE_IS_ENABLED
  if (gtrue || GlobalMxsrOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> A chord ends upon note at " <<
      theNoteEvent->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrScoreNotesEvents::registerTupletBeginEvent (
  int noteSequentialNumber,
  int noteInputStartLineNumber,
  int noteInputEndLineNumber)
{
  S_mxsrNoteEvent
    theNoteEvent =
      mxsrNoteEvent::create (
        mxsrNoteEventKind::kNoteEventTupletBegin,
        noteSequentialNumber,
        noteInputStartLineNumber,
        noteInputStartLineNumber);

  fTupletsBeginNotesEventsMap [noteSequentialNumber] = theNoteEvent;

  fAllNotesEventsList.push_back (theNoteEvent);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> A tuplet begins upon note at " <<
      theNoteEvent->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrScoreNotesEvents::registerTupletEndEvent (
  int noteSequentialNumber,
  int noteInputStartLineNumber,
  int noteInputEndLineNumber)
{
  S_mxsrNoteEvent
    theNoteEvent =
      mxsrNoteEvent::create (
        mxsrNoteEventKind::kNoteEventTupletEnd,
        noteSequentialNumber,
        noteInputStartLineNumber,
        noteInputStartLineNumber);

  fTupletsEndNotesEventsMap [noteSequentialNumber] = theNoteEvent;

  fAllNotesEventsList.push_back (theNoteEvent);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--> A tuplet ends upon note at " <<
      theNoteEvent->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrScoreNotesEvents::registerStaffChangeTakeOffEvent (
  int noteSequentialNumber,
  int noteInputStartLineNumber,
  int noteInputEndLineNumber)
{
  S_mxsrNoteEvent
    theNoteEvent =
      mxsrNoteEvent::create (
        mxsrNoteEventKind::kNoteEventStaffChangeTakeOff,
        noteSequentialNumber,
        noteInputStartLineNumber,
        noteInputStartLineNumber);

  fStaffChangesTakesOffsPositionsMap [noteSequentialNumber] = theNoteEvent;

  fAllNotesEventsList.push_back (theNoteEvent);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceStaffChangesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> A staff change takes off upon note at " <<
      theNoteEvent->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrScoreNotesEvents::registerStaffChangeLandingEvent (
  int noteSequentialNumber,
  int noteInputStartLineNumber,
  int noteInputEndLineNumber)
{
  S_mxsrNoteEvent
    theNoteEvent =
      mxsrNoteEvent::create (
        mxsrNoteEventKind::kNoteEventStaffChangeLanding,
        noteSequentialNumber,
        noteInputStartLineNumber,
        noteInputStartLineNumber);

 fStaffChangesLandingsPositionsMap [noteSequentialNumber] = theNoteEvent;

  fAllNotesEventsList.push_back (theNoteEvent);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceStaffChangesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> A staff change lands upon note at " <<
      theNoteEvent->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrScoreNotesEvents::sortAllNotesEventsList ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gGlobalMxsrOahGroup->getTraceChordsBasics ()
      ||
    gGlobalMxsrOahGroup->getTraceTupletsBasics ()
      ||
    gTraceOahGroup->getTraceStaffChanges ()
  ) {
    std::stringstream ss;

    ss <<
      "--> Sorting the all notes events list";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fAllNotesEventsList.sort (
    compareFrameNotesByIncreasingSequentialNumber);
}

Bool mxsrScoreNotesEvents::aChordBeginsAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  Bool result;

  std::map<int, S_mxsrNoteEvent>::const_iterator it;

  it = fChordsBeginNotesEventsMap.find (noteSequentialNumber);

  if (it != fChordsBeginNotesEventsMap.end ()) {
    result =
      it->second->getNoteSequentialNumber () == noteSequentialNumber;
  }
  else {
    result = false;
  }

  return result;
}

Bool mxsrScoreNotesEvents::aChordEndsAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  Bool result;

  std::map<int, S_mxsrNoteEvent>::const_iterator it;

  it = fChordsEndNotesEventsMap.find (noteSequentialNumber);

  if (it != fChordsEndNotesEventsMap.end ()) {
    result =
      it->second->getNoteSequentialNumber () == noteSequentialNumber;
  }
  else {
    result = false;
  }

  return result;
}

Bool mxsrScoreNotesEvents::aTupletBeginsAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  Bool result;

  std::map<int, S_mxsrNoteEvent>::const_iterator it;

  it = fTupletsBeginNotesEventsMap.find (noteSequentialNumber);

  if (it != fTupletsBeginNotesEventsMap.end ()) {
    result =
      it->second->getNoteSequentialNumber () == noteSequentialNumber;
  }
  else {
    result = false;
  }

  return result;
}

Bool mxsrScoreNotesEvents::aTupletEndsAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  Bool result;

  std::map<int, S_mxsrNoteEvent>::const_iterator it;

  it = fTupletsEndNotesEventsMap.find (noteSequentialNumber);

  if (it != fTupletsEndNotesEventsMap.end ()) {
    result =
      it->second->getNoteSequentialNumber () == noteSequentialNumber;
  }
  else {
    result = false;
  }

  return result;
}

Bool mxsrScoreNotesEvents::aStaffChangeTakesOffAtNoteSequentialNumber (
  int noteSequentialNumber) const
{
  Bool result;

  std::map<int, S_mxsrNoteEvent>::const_iterator it;

  it = fStaffChangesTakesOffsPositionsMap.find (noteSequentialNumber);

  if (it != fStaffChangesTakesOffsPositionsMap.end ()) {
    result =
      it->second->getNoteSequentialNumber () == noteSequentialNumber;
  }
  else {
    result = false;
  }

  return result;
}

Bool mxsrScoreNotesEvents::aTStaffChangeLandsAtNoteSequentialNumber (
 int noteSequentialNumber) const
{
  Bool result;

  std::map<int, S_mxsrNoteEvent>::const_iterator it;

  it = fStaffChangesLandingsPositionsMap.find (noteSequentialNumber);

  if (it != fStaffChangesLandingsPositionsMap.end ()) {
    result =
      it->second->getNoteSequentialNumber () == noteSequentialNumber;
  }
  else {
    result = false;
  }

  return result;
}

std::string mxsrScoreNotesEvents::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[ScoreNotesEvents" <<
    ", fAllNotesEventsList: " <<
    mfSingularOrPlural (
      fAllNotesEventsList.size (),
      "element",
      "elements") <<
    ']';

  return ss.str ();
}

std::string mxsrScoreNotesEvents::asString () const
{
  return asShortString ();
}

void mxsrScoreNotesEvents::print (std::ostream& os) const
{
  if (false) { // JMI CHORDS_TUP v0.9.71
    os <<
      "[ScoreNotesEvents" <<
      std::endl;

    ++gIndenter;

    os <<
      "fChordsBeginNotesEventsMap: " <<
      mfSingularOrPlural (
        fChordsBeginNotesEventsMap.size (),
        "element",
        "elements") <<
      std::endl;

    ++gIndenter;

    for (std::pair<int, S_mxsrNoteEvent> thePair : fChordsBeginNotesEventsMap) {
      int
        noteEventSeauentialNumber = thePair.first;
      S_mxsrNoteEvent
        noteEvent = thePair.second;

      os <<
        noteEventSeauentialNumber <<
        ':' <<
        std::endl;

      ++gIndenter;
      os <<
        noteEvent <<
        std::endl;
      --gIndenter;
    } // for

    --gIndenter;

    os << std::endl << "--" << std::endl << std::endl;

    os <<
      "fChordsEndNotesEventsMap: " <<
      mfSingularOrPlural (
        fChordsEndNotesEventsMap.size (),
        "element",
        "elements") <<
      std::endl;

    ++gIndenter;

    for (std::pair<int, S_mxsrNoteEvent> thePair : fChordsEndNotesEventsMap) {
      int
        noteEventSeauentialNumber = thePair.first;
      S_mxsrNoteEvent
        noteEvent = thePair.second;

      os <<
        noteEventSeauentialNumber <<
        ':' <<
        std::endl;

      ++gIndenter;
      os <<
        noteEvent <<
        std::endl;
      --gIndenter;
    } // for

    --gIndenter;

    os << std::endl << "--" << std::endl << std::endl;

    os <<
      "fTupletsBeginNotesEventsMap: " <<
      mfSingularOrPlural (
        fTupletsBeginNotesEventsMap.size (),
        "element",
        "elements") <<
      std::endl;

    ++gIndenter;

    for (std::pair<int, S_mxsrNoteEvent> thePair : fTupletsBeginNotesEventsMap) {
      int
        noteEventSeauentialNumber = thePair.first;
      S_mxsrNoteEvent
        noteEvent = thePair.second;

      os <<
        noteEventSeauentialNumber <<
        ':' <<
        std::endl;

      ++gIndenter;
      os <<
        noteEvent <<
        std::endl;
      --gIndenter;
    } // for

    --gIndenter;

    os << std::endl << "--" << std::endl << std::endl;

    os <<
      "fTupletsEndNotesEventsMap: " <<
      mfSingularOrPlural (
        fTupletsEndNotesEventsMap.size (),
        "element",
        "elements") <<
      std::endl;

    ++gIndenter;

    for (std::pair<int, S_mxsrNoteEvent> thePair : fTupletsEndNotesEventsMap) {
      int
        noteEventSeauentialNumber = thePair.first;
      S_mxsrNoteEvent
        noteEvent = thePair.second;

      os <<
        noteEventSeauentialNumber <<
        ':' <<
        std::endl;

      ++gIndenter;
      os <<
        noteEvent <<
        std::endl;
      --gIndenter;
    } // for

    --gIndenter;

    os << std::endl << "--" << std::endl << std::endl;

    os <<
      "fStaffChangesTakesOffsPositionsMap: " <<
      mfSingularOrPlural (
        fStaffChangesTakesOffsPositionsMap.size (),
        "element",
        "elements") <<
      std::endl;

    ++gIndenter;

    for (std::pair<int, S_mxsrNoteEvent> thePair : fStaffChangesTakesOffsPositionsMap) {
      int
        noteEventSeauentialNumber = thePair.first;
      S_mxsrNoteEvent
        noteEvent = thePair.second;

      os <<
        noteEventSeauentialNumber <<
        ':' <<
        std::endl;

      ++gIndenter;
      os <<
        noteEvent <<
        std::endl;
      --gIndenter;
    } // for

    --gIndenter;

    os << std::endl << "--" << std::endl << std::endl;

    os <<
      "fStaffChangesLandingsPositionsMap: " <<
      mfSingularOrPlural (
        fStaffChangesLandingsPositionsMap.size (),
        "element",
        "elements") <<
      std::endl;

    ++gIndenter;

    for (std::pair<int, S_mxsrNoteEvent> thePair : fStaffChangesLandingsPositionsMap) {
      int
        noteEventSeauentialNumber = thePair.first;
      S_mxsrNoteEvent
        noteEvent = thePair.second;

      os <<
        noteEventSeauentialNumber <<
        ':' <<
        std::endl;

      ++gIndenter;
      os <<
        noteEvent <<
        std::endl;
      --gIndenter;
    } // for

    --gIndenter;
  }

  os << std::endl << "--------" << std::endl << std::endl;

  os <<
    "fAllNotesEventsList: " <<
    mfSingularOrPlural (
      fAllNotesEventsList.size (),
      "element",
      "elements") <<
    std::endl;

  ++gIndenter;

  for (S_mxsrNoteEvent noteEvent : fAllNotesEventsList) {
    os <<
      noteEvent->getNoteSequentialNumber () <<
      ':' <<
      std::endl;

    ++gIndenter;
    os <<
      noteEvent <<
      std::endl;
    --gIndenter;
  } // for

  --gIndenter;


  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrScoreNotesEvents& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrScoreNotesEvents& elt)
{
  elt.print (os);
  return os;
}


} // namespace

