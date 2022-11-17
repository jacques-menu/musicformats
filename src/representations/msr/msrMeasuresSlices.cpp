/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <climits>      // INT_MIN, INT_MAX

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrMeasuresSlices.h"
// #include "msrNotes.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrNoteEventKindAsString (
  msrNoteEventKind noteEventKind)
{
  std::string result;

  switch (noteEventKind) {
    case msrNoteEventKind::kNoteEventStart:
      result = "noteEventStart";
      break;
    case msrNoteEventKind::kNoteEventStop:
      result = "noteEventStop";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrNoteEvent msrNoteEvent::create (
  const Rational&  noteEventMeasurePosition,
  S_msrNote        noteEventNote,
  msrNoteEventKind noteEventKind)
{
  msrNoteEvent* o = new
    msrNoteEvent (
      noteEventMeasurePosition,
      noteEventNote,
      noteEventKind);
  assert (o != nullptr);
  return o;
}

msrNoteEvent::msrNoteEvent (
  const Rational& noteEventMeasurePosition,
  S_msrNote        noteEventNote,
  msrNoteEventKind noteEventKind)
{
  fNoteEventMeasurePosition = noteEventMeasurePosition;
  fNoteEventNote              = noteEventNote;
  fNoteEventKind              = noteEventKind;
}

msrNoteEvent::~msrNoteEvent ()
{}

bool msrNoteEvent::compareNotesEventsByIncreasingMeasurePosition (
  const SMARTP<msrNoteEvent>& first,
  const SMARTP<msrNoteEvent>& second)
{
  // stop events should be smaller than
  // start events with the same position in measure
  bool result = false;

  Rational
    firstMeasurePosition =
      first->
        getNoteEventMeasurePosition (),
    secondMeasurePosition =
      second->
        getNoteEventMeasurePosition ();

  S_msrNote
    firstNote =
      first->
        getNoteEventNote (),
    secondNote =
      second->
        getNoteEventNote ();

  msrNoteEventKind
    firstNoteEventKind =
      first->
        getNoteEventKind (),
    secondNoteEventKind =
      second->
        getNoteEventKind ();

  if (firstMeasurePosition == secondMeasurePosition) {
    switch (firstNoteEventKind) {
      case msrNoteEventKind::kNoteEventStart:
        switch (secondNoteEventKind) {
          case msrNoteEventKind::kNoteEventStart:
            result = true;
            break;
          case msrNoteEventKind::kNoteEventStop:
            result = false;
            break;
        } // switch
        break;

      case msrNoteEventKind::kNoteEventStop:
        switch (secondNoteEventKind) {
          case msrNoteEventKind::kNoteEventStart:
            result = true;
            break;
          case msrNoteEventKind::kNoteEventStop:
            result = false;
            break;
        } // switch
        break;
    } // switch
  }

  else {
    result =
      bool (
        firstMeasurePosition
          <
        secondMeasurePosition
      );
  }

  return result;
}

std::string msrNoteEvent::asString () const
{
  std::stringstream s;

  s <<
    "[NoteEvent" <<
    " @:" << fNoteEventMeasurePosition <<
    ' ' <<
    fNoteEventNote->asShortStringForMeasuresSlices () <<
    ' ';

  switch (fNoteEventKind) {
    case msrNoteEventKind::kNoteEventStart:
      s << "starts";
      break;
    case msrNoteEventKind::kNoteEventStop:
      s << "stops";
      break;
  } // switch

  S_msrVoice
    voice =
      fNoteEventNote->
        fetchNoteUpLinkToVoice ();

  if (voice) {
    gLogStream <<
      " in voice \"" <<
      voice->getVoiceName () <<
      "\"";
  }

  s << ']';

  return s.str ();
}

void msrNoteEvent::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrNoteEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrSimultaneousNotesChunk msrSimultaneousNotesChunk::create (
  const Rational& chunkMeasurePosition)
{
  msrSimultaneousNotesChunk* o = new
    msrSimultaneousNotesChunk (
      chunkMeasurePosition);
  assert (o != nullptr);
  return o;
}

msrSimultaneousNotesChunk::msrSimultaneousNotesChunk (
  const Rational& chunkMeasurePosition)
{
  fChunkMeasurePosition = chunkMeasurePosition;
}

msrSimultaneousNotesChunk::~msrSimultaneousNotesChunk ()
{}

bool msrSimultaneousNotesChunk::compareSimultaneousNotesChunksByIncreasingMeasurePosition (
  const SMARTP<msrNoteEvent>& first,
  const SMARTP<msrNoteEvent>& second)
{
  return
    bool (
      first->getNoteEventMeasurePosition ()
        <
      second->getNoteEventMeasurePosition ()
    );
}

std::string msrSimultaneousNotesChunk::asString () const
{
  std::stringstream s;

  size_t chunkNotesListSize =
    fChunkNotesList.size ();

  s <<
    "[msrSimultaneousNotesChunk" <<
    " @:" << fChunkMeasurePosition <<
    ", " <<
    mfSingularOrPlural (
      chunkNotesListSize, "note", "notes") <<
    ": ";

  if (chunkNotesListSize) {
    std::list<S_msrNote>::const_iterator
      iBegin = fChunkNotesList.begin (),
      iEnd   = fChunkNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrNote
        note = (*i);

      // print the measure
      s <<
        note->asShortStringForTimeView ();

      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  s << ']';

  return s.str ();
}

void msrSimultaneousNotesChunk::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSimultaneousNotesChunk& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrMeasuresSlice msrMeasuresSlice::create (
  const std::string& measureNumber,
  int           puristMeasureNumber)
{
  msrMeasuresSlice* o =
    new msrMeasuresSlice (
      measureNumber,
      puristMeasureNumber);
  assert (o != nullptr);
  return o;
}

msrMeasuresSlice::msrMeasuresSlice (
  const std::string& measureNumber,
  int           puristMeasureNumber)
{
  fSliceMeasureNumber = measureNumber;

  fSlicePuristMeasureNumber = puristMeasureNumber;
}

msrMeasuresSlice::~msrMeasuresSlice ()
{}

S_msrMeasuresSlice msrMeasuresSlice::createMeasuresSliceShallowCopy ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Creating a shallow copy of measures slice " <<
      this->asShortString () <<
      std::endl;
  }
#endif

  // create the shallow copy
  S_msrMeasuresSlice
    shallowCopy =
      msrMeasuresSlice::create (
      fSliceMeasureNumber,
      fSlicePuristMeasureNumber);

  // populate it
  for (
    std::vector<S_msrMeasure>::const_iterator i =
      fSliceMeasuresVector.begin ();
    i != fSliceMeasuresVector.end ();
    ++i
  ) {
    S_msrMeasure measure = (*i);

    shallowCopy->fSliceMeasuresVector.push_back (measure);
  } // for

  return shallowCopy;
}

void msrMeasuresSlice::appendMeasureToMeasureSlice (
  int          inputLineNumber,
  const S_msrMeasure& measure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Appending measure " <<
      measure->asShortStringForMeasuresSlices () <<
      " to measures slice " <<
      asShortStringForMeasuresSlices () <<
      std::endl;
  }
#endif

  fSliceMeasuresVector.push_back (measure);
}

void msrMeasuresSlice::appendSliceMeasuresFrom (
  int                inputLineNumber,
  const S_msrMeasuresSlice& otherSlice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Appending measures from measures slice " <<
      otherSlice->asShortStringForMeasuresSlices () <<
      " to measures slice " <<
      asShortStringForMeasuresSlices () <<
      std::endl;
  }
#endif

  // sanity check
  if (
    otherSlice->getSliceMeasureNumber ()
      !=
    fSliceMeasureNumber
  ) {
    std::stringstream s;

    s <<
      "cannot append measures slices " <<
      otherSlice->asShortString () <<
      " to " <<
      asShortString () <<
      " which has not the same measure number";

if (true) // JMI
    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  size_t otherSliceSize = otherSlice->getSliceSize ();

  if (otherSliceSize) {
    // get the other slices std::vector
    const std::vector<S_msrMeasure>&
      otherMeasuresVector =
        otherSlice->
          getSliceMeasuresVector ();

    for (size_t i = 0; i < otherSliceSize; ++i) {
      S_msrMeasure
        otherMeasure =
          otherMeasuresVector [i];

      fSliceMeasuresVector.push_back (otherMeasure);
    } // for
  }
}

void msrMeasuresSlice::collectNonSkipNotesFromMeasuresSliceMeasures ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Collecting the notes from the measures slices in " <<
      asShortStringForMeasuresSlices () <<
      std::endl;
  }
#endif

  // collect the notes from the measures
  for (
    std::vector<S_msrMeasure>::const_iterator i =
      fSliceMeasuresVector.begin ();
    i != fSliceMeasuresVector.end ();
    ++i
  ) {
    S_msrMeasure measure = (*i);

    const std::list<S_msrNote>&
      measureNotesFlatList =
        measure->
          getMeasureNotesFlatList ();

    ++gIndenter;

    // append the measure's notes to the slices sequence notes flat std::list
    for (
      std::list<S_msrNote>::const_iterator i = measureNotesFlatList.begin ();
      i != measureNotesFlatList.end ();
      ++i
    ) {
      S_msrNote note = (*i);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
        gLogStream <<
          "--> note: " <<
          note->asShortString () <<
          std::endl;
      }
#endif

      // don't keep the skip notes,
      // they're the ones that may make another note or rest 'solo'
      Bool doKeepTheNote (true);

      switch (note->getNoteKind ()) {
        case msrNoteKind::kNoteSkipInMeasure:
        case msrNoteKind::kNoteSkipInGraceNotesGroup:
          doKeepTheNote = false;
          break;
        default:
          doKeepTheNote = true;
      } // switch

      if (doKeepTheNote) {
        Rational
          noteMeasurePosition =
            note->
              getMeasureElementMeasurePosition ();

        // append note to the slice notes flat std::list
        fSliceNotesFlatList.push_back (note);

        // append a note start event to the slice notes events std::list
        S_msrNoteEvent
          noteStartEvent =
            msrNoteEvent::create (
              noteMeasurePosition,
              note,
              msrNoteEventKind::kNoteEventStart);

        fSliceNoteEventsList.push_back (noteStartEvent);

        // append a note stop event to the slice notes events std::list
        Rational
          noteEndMeasurePosition =
            noteMeasurePosition
              +
            note->
              getMeasureElementSoundingWholeNotes ();

        S_msrNoteEvent
          noteStopEvent =
            msrNoteEvent::create (
              noteEndMeasurePosition,
              note,
              msrNoteEventKind::kNoteEventStop);

        fSliceNoteEventsList.push_back (noteStopEvent);
      }
    } // for

    --gIndenter;
  } // for

  // sort the slice notes flat std::list by position in measure
  fSliceNotesFlatList.sort (
    msrNote::compareNotesByIncreasingMeasurePosition);

  // sort the slice notes events std::list by position in measure
  fSliceNoteEventsList.sort (
    msrNoteEvent::compareNotesEventsByIncreasingMeasurePosition);

  // build the simultaneous notes chunks std::list
  buildTheSimutaneousNotesChunksList ();

  // identify the solo notes and rests in measusre slice
  identifySoloNotesAndRestsInMeasuresSlice ();
}

void msrMeasuresSlice::buildTheSimutaneousNotesChunksList ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Building the simultaneous notes chunks std::list in " <<
      asShortStringForMeasuresSlices () <<
      std::endl;
  }
#endif

  S_msrSimultaneousNotesChunk
    currentSimultaneousNotesChunk;

  Rational
    currentChunkMeasurePosition = Rational (-1, 1);

  for (
    std::list<S_msrNoteEvent>::const_iterator i = fSliceNoteEventsList.begin ();
    i != fSliceNoteEventsList.end ();
    ++i
  ) {
    S_msrNoteEvent noteEvent = (*i);

    Rational
      noteEventMeasurePosition =
        noteEvent->
          getNoteEventMeasurePosition ();

    if (noteEventMeasurePosition != currentChunkMeasurePosition) {
      // a new chunk starts here, create it
      currentSimultaneousNotesChunk =
        msrSimultaneousNotesChunk::create (
          noteEventMeasurePosition);

      // append it to the simultaneous notes chunks std::list
      fSliceSimultaneousNotesChunksList.push_back (
        currentSimultaneousNotesChunk);

      // update currentChunkMeasurePosition
      currentChunkMeasurePosition =
        noteEventMeasurePosition;
    }

    // handle the note event
    S_msrNote
      note =
        noteEvent->
          getNoteEventNote ();

    msrNoteEventKind
      noteEventKind =
        noteEvent->
          getNoteEventKind ();

    switch (noteEventKind) {
      case msrNoteEventKind::kNoteEventStart:
        currentSimultaneousNotesChunk->
          appendNoteToSimultaneousNotesChunk (
            note);
        break;
      case msrNoteEventKind::kNoteEventStop:
        break;
    } // switch
  } // for
}

void msrMeasuresSlice::identifySoloNotesAndRestsInMeasuresSlice ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Identifying solo notes and rests in measures slice " <<
      asShortStringForMeasuresSlices () <<
      std::endl;
  }
#endif

  int simultaneousNotesNumber = 0;

  S_msrNote soloCandidate;

  for (
    std::list<S_msrNoteEvent>::const_iterator i = fSliceNoteEventsList.begin ();
    i != fSliceNoteEventsList.end ();
    ++i
  ) {
    S_msrNoteEvent noteEvent = (*i);

    // handle the note event
//     Rational JMI
//       noteEventMeasurePosition =
//         noteEvent->
//           getNoteEventMeasurePosition ();

    S_msrNote
      note =
        noteEvent->
          getNoteEventNote ();

    // this note is no solo note or rest by default
    note->
      setSoloNoteOrRestInStaffKind (
        msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffNo);

    msrNoteEventKind
      noteEventKind =
        noteEvent->
          getNoteEventKind ();

    switch (noteEventKind) {
      case msrNoteEventKind::kNoteEventStart:
        ++simultaneousNotesNumber;

        if (simultaneousNotesNumber == 1) {
          soloCandidate = note;

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
            gLogStream <<
              "Solo note or rest " <<
              note->asShortStringForTimeView ();

            S_msrVoice
              voice =
                note->
                  fetchNoteUpLinkToVoice ();

            if (voice) {
              gLogStream <<
                " in voice \"" <<
                voice->getVoiceName () <<
                "\"";
            }

            gLogStream <<
              " is a solo candidate" <<
              std::endl;
          }
#endif
        }

        else {
          // forget about the solo candidate
          if (soloCandidate) {
#ifdef TRACING_IS_ENABLED
            if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
              gLogStream <<
                "Forget about solo note or rest candidate " <<
                soloCandidate->asShortStringForTimeView ();

              S_msrVoice
                voice =
                  note->
                    fetchNoteUpLinkToVoice ();

              if (voice) {
                gLogStream <<
                  " in voice \"" <<
                  voice->getVoiceName () <<
                  "\"";
              }
            }
#endif

            soloCandidate = nullptr;
          }
        }
        break;

      case msrNoteEventKind::kNoteEventStop:
        if (simultaneousNotesNumber == 1) {
          if (soloCandidate == note) {
            switch (note->getNoteKind ()) {
              // in measures
              case msrNoteKind::kNoteRegularInMeasure:
              case msrNoteKind::kNoteRestInMeasure:
              case msrNoteKind::kNoteUnpitchedInMeasure:

              // in tuplets
              case msrNoteKind::kNoteRegularInTuplet:
              case msrNoteKind::kNoteRestInTuplet:

              // in double-tremolos
              case msrNoteKind::kNoteInDoubleTremolo:
                {
                  S_msrStaff
                    noteStaff =
                      note->fetchUpLinkToNoteToStaff ();

                  const std::list<S_msrVoice>&
                    staffRegularVoicesList =
                      noteStaff->
                        getStaffRegularVoicesList ();

                  size_t
                    staffRegularVoicesListSize =
                      staffRegularVoicesList.size ();


#ifdef TRACING_IS_ENABLED
                  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
                    gLogStream <<
                      "Solo note or rest? " <<
                      note->asShortStringForTimeView ();
                  }
#endif

                  if (staffRegularVoicesListSize > 1) {
                    // this is a solo note or rest
                    note->
                      setSoloNoteOrRestInStaffKind (
                        msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffYes);

#ifdef TRACING_IS_ENABLED
                    if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
                      gLogStream <<
                        "Solo note or rest " <<
                        note->asShortStringForTimeView ();

                      S_msrVoice
                        voice =
                          note->
                            fetchNoteUpLinkToVoice ();

                      if (voice) {
                        gLogStream <<
                          " in voice \"" <<
                          voice->getVoiceName () <<
                          "\"";
                      }

                      gLogStream <<
                        " has been identified" <<
                        std::endl;
                    }
#endif
                  }
                }
                break;

              // all the other cases
              default:
                // this is no solo note or rest
                ;
            } // switch
          }

          else {
            // this is no solo note or rest
            ;
          }
        }

        --simultaneousNotesNumber;
        break;
    } // switch
  } // for
}


std::string msrMeasuresSlice::asString () const
{
  std::stringstream s;

  s <<
    "[MeasuresSlice" <<
    ", sliceMeasureNumber: " << fSliceMeasureNumber <<
    ", " <<
    mfSingularOrPlural (
      fSliceMeasuresVector.size (), "measure", "measures") <<
    ']';

  return s.str ();
}

std::string msrMeasuresSlice::asShortString () const
{
  std::stringstream s;

  size_t sliceMeasuresVectorSize =
    fSliceMeasuresVector.size ();

  s <<
    "[MeasuresSlice" <<
    ", sliceMeasureNumber: " << fSliceMeasureNumber <<
    ", " <<
    mfSingularOrPlural (
      sliceMeasuresVectorSize, "measure", "measures");

  if (sliceMeasuresVectorSize) {
    std::vector<S_msrMeasure>::const_iterator
      iBegin = fSliceMeasuresVector.begin (),
      iEnd   = fSliceMeasuresVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasure
        measure = (*i);

      // print the measure
      s <<
        measure->asShortStringForMeasuresSlices ();

      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  s <<
    ']';

  return s.str ();
}

std::string msrMeasuresSlice::asShortStringForMeasuresSlices () const
{
  std::stringstream s;

  size_t sliceMeasuresVectorSize =
    fSliceMeasuresVector.size ();

  s <<
    "[MeasuresSlice" <<
    ", sliceMeasureNumber: " << fSliceMeasureNumber <<
    ", " <<
    mfSingularOrPlural (
      sliceMeasuresVectorSize, "measure", "measures");

  if (sliceMeasuresVectorSize) {

    std::vector<S_msrMeasure>::const_iterator
      iBegin = fSliceMeasuresVector.begin (),
      iEnd   = fSliceMeasuresVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasure
        measure = (*i);

      // print the measure
      s <<
        measure->asShortStringForMeasuresSlices ();

      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  s <<
    ']';

  return s.str ();
}

void msrMeasuresSlice::print (std::ostream& os) const
{
  size_t sliceMeasuresVectorSize =
    fSliceMeasuresVector.size ();

  os <<
    "[MeasuresSlice" <<
    " (" <<
    mfSingularOrPlural (
      sliceMeasuresVectorSize, "measure", "measures") <<
    ")" <<
    std::endl;

  ++gIndenter;

  os <<
    "sliceMeasureNumber: " << fSliceMeasureNumber <<
    std::endl;

  // print the slice measures std::vector
  if (sliceMeasuresVectorSize) {
    os <<
      "sliceMeasuresVector: " <<
      std::endl;

    ++gIndenter;

    std::vector<S_msrMeasure>::const_iterator
      iBegin = fSliceMeasuresVector.begin (),
      iEnd   = fSliceMeasuresVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasure
        measure = (*i);

      // print the measure
      os <<
        measure->
          asShortStringForMeasuresSlices () <<
          std::endl;

      if (++i == iEnd) break;
      os << std::endl;
    } // for
// JMI    os << std::endl;

    --gIndenter;
  }

  // print the slice notes flat std::list
  size_t sliceNotesFlatListSize =
    fSliceNotesFlatList.size ();

  if (sliceNotesFlatListSize) {
    os <<
      "sliceNotesFlatList: " <<
      std::endl;

    ++gIndenter;

    std::list<S_msrNote>::const_iterator
      iBegin = fSliceNotesFlatList.begin (),
      iEnd   = fSliceNotesFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrNote note = (*i);

      // print the measure
      os <<
        note->
          asShortStringForTimeView ();

      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << std::endl;

    --gIndenter;
  }

  // print the slice notes events std::list
  size_t sliceNoteEventsListSize =
    fSliceNoteEventsList.size ();

  if (sliceNoteEventsListSize) {
    os <<
      "sliceNoteEventsList: " <<
      std::endl;

    ++gIndenter;

    std::list<S_msrNoteEvent>::const_iterator
      iBegin = fSliceNoteEventsList.begin (),
      iEnd   = fSliceNoteEventsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrNoteEvent noteEvent = (*i);

      // print the measure
      os <<
        noteEvent->
          asString();

      if (++i == iEnd) break;
      os << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }

  // print the simultaneous notes chunks std::list
  size_t sliceSimultaneousNotesChunksListSize =
    fSliceSimultaneousNotesChunksList.size ();

  if (sliceSimultaneousNotesChunksListSize) {
    os <<
      "sliceSimultaneousNotesChunksList: " <<
      std::endl;

    ++gIndenter;

    std::list<S_msrSimultaneousNotesChunk>::const_iterator
      iBegin = fSliceSimultaneousNotesChunksList.begin (),
      iEnd   = fSliceSimultaneousNotesChunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrSimultaneousNotesChunk simultaneousNotesChunk = (*i);

      // print the measure
      os <<
        simultaneousNotesChunk->
          asString();

      if (++i == iEnd) break;
      os << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrMeasuresSlice::printShort (std::ostream& os) const
{
  print (os);
}

std::ostream& operator << (std::ostream& os, const S_msrMeasuresSlice& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrMeasuresSlicesSequence msrMeasuresSlicesSequence::create (
  const std::string& measuresOrigin)
{
  msrMeasuresSlicesSequence* o =
    new msrMeasuresSlicesSequence (
      measuresOrigin);
  assert (o != nullptr);
  return o;
}

S_msrMeasuresSlicesSequence msrMeasuresSlicesSequence::create (
  const std::string& measuresOrigin,
  int           measuresNumber)
{
  msrMeasuresSlicesSequence* o =
    new msrMeasuresSlicesSequence (
      measuresOrigin,
      measuresNumber);
  assert (o != nullptr);
  return o;
}

msrMeasuresSlicesSequence::msrMeasuresSlicesSequence (
  const std::string& measuresOrigin)
{
  fMeasuresOrigin = measuresOrigin;
}

msrMeasuresSlicesSequence::msrMeasuresSlicesSequence (
  const std::string& measuresOrigin,
  int           measuresNumber)
{
  fMeasuresOrigin = measuresOrigin;

  for (int i = 1; i <= measuresNumber; ++i) {
    fMeasuresSlicesVector.push_back (
      msrMeasuresSlice::create (
        __FILE__, // JMI
        __LINE__) // JMI
      );
  } // for
}

msrMeasuresSlicesSequence::~msrMeasuresSlicesSequence ()
{}

S_msrMeasuresSlicesSequence msrMeasuresSlicesSequence::createMeasuresSlicesSequenceShallowCopy (
  const std::string& measuresOrigin)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Creating a shallow copy of measures slices sequence " <<
      this->asShortString () <<
      std::endl;
  }
#endif

  // create shallow copy
  S_msrMeasuresSlicesSequence
    shallowCopy =
      msrMeasuresSlicesSequence::create (
        fMeasuresOrigin + "_" + measuresOrigin);

  // populate it
  for (
    std::vector<S_msrMeasuresSlice>::const_iterator i =
      fMeasuresSlicesVector.begin ();
    i != fMeasuresSlicesVector.end ();
    ++i
  ) {
    S_msrMeasuresSlice measuresSlice = (*i);

    shallowCopy->
      appendMeasuresSliceToSequence (
        measuresSlice->
          createMeasuresSliceShallowCopy ());
  } // for

  return shallowCopy;
}

void msrMeasuresSlicesSequence::appendMeasuresSliceToSequence (
  const S_msrMeasuresSlice& measuresSlice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Appending measures slice " <<
      measuresSlice->asShortStringForMeasuresSlices () <<
      " to measures slices sequence " <<
      asShortStringForMeasuresSlices () <<
      std::endl;
  }
#endif

  fMeasuresSlicesVector.push_back (
    measuresSlice);
}

S_msrMeasuresSlicesSequence msrMeasuresSlicesSequence::mergeWithMeasuresSlicesSequence (
  int           inputLineNumber,
  const std::string& measuresOrigin,
  const S_msrMeasuresSlicesSequence&
                otherMeasuresSlicesSequence)
{
  S_msrMeasuresSlicesSequence result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Merging measures slices sequence " <<
      asShortStringForMeasuresSlices () <<
      " with " <<
      otherMeasuresSlicesSequence->asShortStringForMeasuresSlices () <<
      std::endl;
  }
#endif

  size_t sequenceSize =
    getSlicesSequenceSize ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "---> sequenceSize: " <<
      sequenceSize <<
      std::endl;
  }
#endif

  if (sequenceSize == 0) {
    // merge an empty measures slices sequence with another

    // create the result as a shallow copy of the other sequence
    result =
      otherMeasuresSlicesSequence->
        createMeasuresSlicesSequenceShallowCopy (
          measuresOrigin);

    // set its measure origin
    result->
      setMeasuresOrigin (measuresOrigin);
  }

  else {
    // non-empty sequence, create the result
    result =
      msrMeasuresSlicesSequence::create (
        measuresOrigin);

    size_t otherSequenceSize =
      otherMeasuresSlicesSequence->getSlicesSequenceSize ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
      gLogStream <<
        "mergeWithMeasuresSlicesSequence()" <<
        ", otherSequenceSize: " <<
        otherSequenceSize <<
        std::endl;
    }
#endif

    // sanity check
    if (otherSequenceSize != sequenceSize) {
      std::stringstream s;

      s <<
        "cannot enrich measures slices sequence " <<
        asShortString () <<
        " with " <<
        otherMeasuresSlicesSequence->asShortString () <<
        " which has not the same size";

      gLogStream <<
        std::endl <<
        S_msrMeasuresSlicesSequence (this) <<
        std::endl <<
        otherMeasuresSlicesSequence <<
        std::endl;

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

    else {
      if (sequenceSize) {
        // the two measures slices sequences have the same non null size

        // get the other sequence's measures slices std::vector
        const std::vector<S_msrMeasuresSlice>&
          otherMeasuresSlicesVector =
            otherMeasuresSlicesSequence->
              getMeasuresSlicesVector ();

        // iterate over the measures slices
        for (size_t i = 0; i < sequenceSize; ++i) {
          S_msrMeasuresSlice
            currentSlice =
              fMeasuresSlicesVector [i];

          // create the result slice
          S_msrMeasuresSlice
            resultSlice =
              msrMeasuresSlice::create (
                currentSlice->getSliceMeasureNumber (),
                currentSlice->getSlicePuristMeasureNumber ());

          // append it to resultSlice
          result->
            appendMeasuresSliceToSequence (
              resultSlice);

          // append the measures from this sequence's slice to resultSlice
          resultSlice->
            appendSliceMeasuresFrom (
              inputLineNumber,
              currentSlice);

          // append the measures from the other sequence's slice to resultSlice
          resultSlice->
            appendSliceMeasuresFrom (
              inputLineNumber,
              otherMeasuresSlicesVector [i]);
        } // for
      }
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "---> mergeWithMeasuresSlicesSequence()" <<
      ", result: " <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      result->asShortStringForMeasuresSlices () <<
      std::endl;
    --gIndenter;
  }
#endif

  return result;
}

void msrMeasuresSlicesSequence::identifySoloNotesAndRests ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Identifying solo notes and rests in measures slices sequence " <<
      asShortStringForMeasuresSlices () <<
      ":" <<
      std::endl;

    ++gIndenter;
    this->printShort (gLogStream);
    --gIndenter;
  }
#endif

  // collect the notes from the sequence's measures slices
  for (
    std::vector<S_msrMeasuresSlice>::const_iterator i =
      fMeasuresSlicesVector.begin ();
    i != fMeasuresSlicesVector.end ();
    ++i
  ) {
    S_msrMeasuresSlice measuresSlice = (*i);

    measuresSlice->
      collectNonSkipNotesFromMeasuresSliceMeasures ();
  } // for
}

std::string msrMeasuresSlicesSequence::asString () const
{
  std::stringstream s;

  s <<
    "[MeasuresSlicesSequence" <<
    ", measuresOrigin: " << fMeasuresOrigin <<
    ", " <<
    mfSingularOrPlural (
      fMeasuresSlicesVector.size (), "measure slice", "measures slices") <<
    ']';

  return s.str ();
}

std::string msrMeasuresSlicesSequence::asShortString () const
{
  return asString ();
}

std::string msrMeasuresSlicesSequence::asShortStringForMeasuresSlices () const
{
  return asString ();
}

void msrMeasuresSlicesSequence::print (std::ostream& os) const
{
  size_t measuresSlicesVectorSize =
    fMeasuresSlicesVector.size ();

  os <<
    "[MeasuresSlicesSequence (" <<
    mfSingularOrPlural (
      measuresSlicesVectorSize, "measures slice", "measures slices") <<
    ")" <<
    std::endl;

  ++gIndenter;
  os <<
    "measuresOrigin: " << fMeasuresOrigin <<
    std::endl;
  --gIndenter;

  if (measuresSlicesVectorSize) {
    os << std::endl;
    ++gIndenter;

    std::vector<S_msrMeasuresSlice>::const_iterator
      iBegin = fMeasuresSlicesVector.begin (),
      iEnd   = fMeasuresSlicesVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasuresSlice measuresSlice = (*i);

      // print the measure
      os <<
        measuresSlice;

      if (++i == iEnd) break;
      os << std::endl;
    } // for

    os << std::endl;
    --gIndenter;
  }

  os << ']' << std::endl;
}

void msrMeasuresSlicesSequence::printShort (std::ostream& os) const
{
  print (os);
}

std::ostream& operator << (std::ostream& os, const S_msrMeasuresSlicesSequence& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
