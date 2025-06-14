/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <climits>      // INT_MIN, INT_MAX

#include "mfPreprocessorSettings.h"

#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrMeasuresSlices.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"

#include "msrWae.h"

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
  const mfPositionInMeasure&  noteEventPositionInMeasure,
  const S_msrNote& noteEventNote,
  msrNoteEventKind noteEventKind)
{
  msrNoteEvent* obj = new
    msrNoteEvent (
      noteEventPositionInMeasure,
      noteEventNote,
      noteEventKind);
  assert (obj != nullptr);
  return obj;
}

msrNoteEvent::msrNoteEvent (
  const mfPositionInMeasure& noteEventPositionInMeasure,
  const S_msrNote& noteEventNote,
  msrNoteEventKind noteEventKind)
{
  fNoteEventPositionInMeasure = noteEventPositionInMeasure;
  fNoteEventNote = noteEventNote;
  fNoteEventKind = noteEventKind;
}

msrNoteEvent::~msrNoteEvent ()
{}

bool msrNoteEvent::compareNoteEventsByIncreasingPositionInMeasure (
  const SMARTP<msrNoteEvent>& first,
  const SMARTP<msrNoteEvent>& second)
{
  // stop events should be smaller than
  // start events with the same measure position
  bool result = false;

  mfPositionInMeasure
    firstPositionInMeasure =
      first->
        fNoteEventPositionInMeasure,
    secondPositionInMeasure =
      second->
        fNoteEventPositionInMeasure;

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

  if (firstPositionInMeasure == secondPositionInMeasure) {
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
        firstPositionInMeasure
          <
        secondPositionInMeasure
      );
  }

  return result;
}

std::string msrNoteEvent::asString () const
{
  std::stringstream ss;

  ss <<
    "[NoteEvent" <<
    " @:" << fNoteEventPositionInMeasure.asString () <<
    ' ' <<
    fNoteEventNote->asStringForMeasuresSlices () <<
    ' ';

  switch (fNoteEventKind) {
    case msrNoteEventKind::kNoteEventStart:
      ss << "starts";
      break;
    case msrNoteEventKind::kNoteEventStop:
      ss << "stops";
      break;
  } // switch

  S_msrVoice
    voice =
      fNoteEventNote->
        fetchNoteUpLinkToVoice ();

  if (voice) {
    gLog <<
      " in voice \"" <<
      voice->getVoiceName () <<
      "\"";
  }

  ss << ']';

  return ss.str ();
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrSimultaneousNotesChunk msrSimultaneousNotesChunk::create (
  const mfPositionInMeasure& chunkPositionInMeasure)
{
  msrSimultaneousNotesChunk* obj = new
    msrSimultaneousNotesChunk (
      chunkPositionInMeasure);
  assert (obj != nullptr);
  return obj;
}

msrSimultaneousNotesChunk::msrSimultaneousNotesChunk (
  const mfPositionInMeasure& chunkPositionInMeasure)
{
  fChunkPositionInMeasure = chunkPositionInMeasure;
}

msrSimultaneousNotesChunk::~msrSimultaneousNotesChunk ()
{}

bool msrSimultaneousNotesChunk::compareSimultaneousNotesChunksByIncreasingPositionInMeasure (
  const SMARTP<msrNoteEvent>& first,
  const SMARTP<msrNoteEvent>& second)
{
  return
    bool (
      first->getNoteEventPositionInMeasure ()
        <
      second->getNoteEventPositionInMeasure ()
    );
}

std::string msrSimultaneousNotesChunk::asString () const
{
  std::stringstream ss;

  size_t chunkNotesListSize =
    fChunkNotesList.size ();

  ss <<
    "[msrSimultaneousNotesChunk" <<
    " @:" << fChunkPositionInMeasure.asString () <<
    ", " <<
    mfSingularOrPlural (
      chunkNotesListSize, "note", "notes") <<
    ": ";

  if (chunkNotesListSize) {
    std::list <S_msrNote>::const_iterator
      iBegin = fChunkNotesList.begin (),
      iEnd   = fChunkNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrNote
        note = (*i);

      // print the measure
      ss <<
        note->asStringForMeasuresSlices ();

      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  ss << ']';

  return ss.str ();
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrMeasuresSlice msrMeasuresSlice::create (
  const mfMeasureNumber& measureNumber,
  int                    puristMeasureNumber)
{
  msrMeasuresSlice* obj =
    new msrMeasuresSlice (
      measureNumber,
      puristMeasureNumber);
  assert (obj != nullptr);
  return obj;
}

msrMeasuresSlice::msrMeasuresSlice (
  const mfMeasureNumber& measureNumber,
  int                    puristMeasureNumber)
{
  fSliceMeasureNumber = measureNumber;

  fSlicePuristMeasureNumber = puristMeasureNumber;
}

msrMeasuresSlice::~msrMeasuresSlice ()
{}

S_msrMeasuresSlice msrMeasuresSlice::createMeasuresSliceShallowCopy ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "Creating a shallow copy of measures slice " <<
      this->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the shallow copy
  S_msrMeasuresSlice
    shallowCopy =
      msrMeasuresSlice::create (
        fSliceMeasureNumber,
        fSlicePuristMeasureNumber);

  // populate it
  for (
    std::vector <S_msrMeasure>::const_iterator i =
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
  const S_msrMeasure& measure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "Appending measure " <<
      measure->asStringForMeasuresSlices () <<
      " to measures slice " <<
      asStringForMeasuresSlices ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSliceMeasuresVector.push_back (measure);
}

void msrMeasuresSlice::appendSliceMeasuresFrom (
  const S_msrMeasuresSlice& otherSlice)
{
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasuresSlices ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending measures from measures slice " <<
//       otherSlice->asStringForMeasuresSlices () <<
//       " to measures slice " <<
//       asStringForMeasuresSlices ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   if (
//     otherSlice->getSliceMeasureNumber ()
//       !=
//     fSliceMeasureNumber
//   ) {
//     std::stringstream ss;
//
//     ss <<
//       "cannot append measures slices " <<
//       otherSlice->asShortString () <<
//       " to " <<
//       asShortString () <<
//       " which has not the same measure number";
//
// if (true) // JMI
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   size_t otherSliceSize = otherSlice->getSliceSize ();
//
//   if (otherSliceSize) {
//     // get the other slices vector
//     const std::vector <S_msrMeasure>&
//       otherMeasuresVector =
//         otherSlice->
//           getSliceMeasuresVector ();
//
//     for (size_t i = 0; i < otherSliceSize; ++i) {
//       S_msrMeasure
//         otherMeasure =
//           otherMeasuresVector.at (i);
//
//       fSliceMeasuresVector.push_back (otherMeasure);
//     } // for
//   }
}

void msrMeasuresSlice::collectNonSkipNotesFromMeasuresSliceMeasures ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "Collecting the notes from the measures slices in " <<
      asStringForMeasuresSlices ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // collect the notes from the measures
  for (
    std::vector <S_msrMeasure>::const_iterator i =
      fSliceMeasuresVector.begin ();
    i != fSliceMeasuresVector.end ();
    ++i
  ) {
    S_msrMeasure measure = (*i);

    const std::list <S_msrNote>&
      measureNotesFlatList =
        measure->
          getMeasureNotesFlatList ();

    ++gIndenter;

    // append the measure's notes to the measures slices sequence notes flat list
    for (
      std::list <S_msrNote>::const_iterator i = measureNotesFlatList.begin ();
      i != measureNotesFlatList.end ();
      ++i
    ) {
      S_msrNote note = (*i);

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMeasuresSlices ()) {
        std::stringstream ss;

        ss <<
          "--> note: " <<
          note->asShortString ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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
        mfPositionInMeasure
          notePositionInMeasure =
            note->
              getMeasureElementPositionInMeasure ();

        // append note to the slice notes flat list
        fSliceNotesFlatList.push_back (note);

        // append a note start event to the slice notes events list
        S_msrNoteEvent
          noteStartEvent =
            msrNoteEvent::create (
              notePositionInMeasure,
              note,
              msrNoteEventKind::kNoteEventStart);

        fSliceNoteEventsList.push_back (noteStartEvent);

        // append a note stop event to the slice notes events list
        mfPositionInMeasure
          noteEndPositionInMeasure =
            notePositionInMeasure
              +
            note->
              getMeasureElementSoundingWholeNotes ();

        S_msrNoteEvent
          noteStopEvent =
            msrNoteEvent::create (
              noteEndPositionInMeasure,
              note,
              msrNoteEventKind::kNoteEventStop);

        fSliceNoteEventsList.push_back (noteStopEvent);
      }
    } // for

    --gIndenter;
  } // for

  // sort the slice notes flat list by measure position
  fSliceNotesFlatList.sort (
    msrNote::compareNotesByIncreasingPositionInMeasure);

  // sort the slice notes events list by measure position
  fSliceNoteEventsList.sort (
    msrNoteEvent::compareNoteEventsByIncreasingPositionInMeasure);

  // build the simultaneous notes chunks list
  buildTheSimutaneousNotesChunksList ();

  // identify the solo notes and rests in measure slice
  identifySoloNotesAndRestsInMeasuresSlice ();
}

void msrMeasuresSlice::buildTheSimutaneousNotesChunksList ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "Building the simultaneous notes chunks list in " <<
      asStringForMeasuresSlices ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrSimultaneousNotesChunk
    currentSimultaneousNotesChunk;

  mfPositionInMeasure
    currentChunkPositionInMeasure (-1, 1);

  for (
    std::list <S_msrNoteEvent>::const_iterator i = fSliceNoteEventsList.begin ();
    i != fSliceNoteEventsList.end ();
    ++i
  ) {
    S_msrNoteEvent noteEvent = (*i);

    mfPositionInMeasure
      noteEventPositionInMeasure =
        noteEvent->
          getNoteEventPositionInMeasure ();

    if (noteEventPositionInMeasure != currentChunkPositionInMeasure) {
      // a new chunk starts here, create it
      currentSimultaneousNotesChunk =
        msrSimultaneousNotesChunk::create (
          noteEventPositionInMeasure);

      // append it to the simultaneous notes chunks list
      fSliceSimultaneousNotesChunksList.push_back (
        currentSimultaneousNotesChunk);

      // update currentChunkPositionInMeasure
      currentChunkPositionInMeasure =
        noteEventPositionInMeasure;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "Identifying solo notes and rests in measures slice " <<
      asStringForMeasuresSlices ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int simultaneousNotesNumber = 0;

  S_msrNote soloCandidate;

  for (
    std::list <S_msrNoteEvent>::const_iterator i = fSliceNoteEventsList.begin ();
    i != fSliceNoteEventsList.end ();
    ++i
  ) {
    S_msrNoteEvent noteEvent = (*i);

    // handle the note event
//     mfWholeNotes JMI
//       noteEventPositionInMeasure =
//         noteEvent->
//           getNoteEventPositionInMeasure ();

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

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceMeasuresSlices ()) {
            gLog <<
              "Solo note or rest " <<
              note->asStringForMeasuresSlices ();

            S_msrVoice
              voice =
                note->
                  fetchNoteUpLinkToVoice ();

            if (voice) {
              gLog <<
                " in voice \"" <<
                voice->getVoiceName () <<
                "\"";
            }

            gLog <<
              " is a solo candidate" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED
        }

        else {
          // forget about the solo candidate
          if (soloCandidate) {
#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTraceMeasuresSlices ()) {
              gLog <<
                "Forget about solo note or rest candidate " <<
                soloCandidate->asStringForMeasuresSlices ();

              S_msrVoice
                voice =
                  note->
                    fetchNoteUpLinkToVoice ();

              if (voice) {
                gLog <<
                  " in voice \"" <<
                  voice->getVoiceName () <<
                  "\"";
              }
            }
#endif // MF_TRACE_IS_ENABLED

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

                  const std::list <S_msrVoice>&
                    staffRegularVoicesList =
                      noteStaff->
                        getStaffRegularVoicesList ();

                  size_t
                    staffRegularVoicesListSize =
                      staffRegularVoicesList.size ();


#ifdef MF_TRACE_IS_ENABLED
                  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
                    gLog <<
                      "Solo note or rest? " <<
                      note->asStringForMeasuresSlices ();
                  }
#endif // MF_TRACE_IS_ENABLED

                  if (staffRegularVoicesListSize > 1) {
                    // this is a solo note or rest
                    note->
                      setSoloNoteOrRestInStaffKind (
                        msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffYes);

#ifdef MF_TRACE_IS_ENABLED
                    if (gTraceOahGroup->getTraceMeasuresSlices ()) {
                      gLog <<
                        "Solo note or rest " <<
                        note->asStringForMeasuresSlices ();

                      S_msrVoice
                        voice =
                          note->
                            fetchNoteUpLinkToVoice ();

                      if (voice) {
                        gLog <<
                          " in voice \"" <<
                          voice->getVoiceName () <<
                          "\"";
                      }

                      gLog <<
                        " has been identified" <<
                        std::endl;
                    }
#endif // MF_TRACE_IS_ENABLED
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
  std::stringstream ss;

  ss <<
    "[MeasuresSlice" <<
    ", sliceMeasureNumber: " << fSliceMeasureNumber <<
    ", " <<
    mfSingularOrPlural (
      fSliceMeasuresVector.size (), "measure", "measures") <<
    ']';

  return ss.str ();
}

std::string msrMeasuresSlice::asShortString () const
{
  std::stringstream ss;

  size_t sliceMeasuresVectorSize =
    fSliceMeasuresVector.size ();

  ss <<
    "[MeasuresSlice" <<
    ", sliceMeasureNumber: " << fSliceMeasureNumber <<
    ", " <<
    mfSingularOrPlural (
      sliceMeasuresVectorSize, "measure", "measures");

  if (sliceMeasuresVectorSize) {
    std::vector <S_msrMeasure>::const_iterator
      iBegin = fSliceMeasuresVector.begin (),
      iEnd   = fSliceMeasuresVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasure
        measure = (*i);

      // print the measure
      ss <<
        measure->asStringForMeasuresSlices ();

      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  ss <<
    ']';

  return ss.str ();
}

std::string msrMeasuresSlice::asStringForMeasuresSlices () const
{
  std::stringstream ss;

  size_t sliceMeasuresVectorSize =
    fSliceMeasuresVector.size ();

  ss <<
    "[MeasuresSlice" <<
    ", sliceMeasureNumber: " << fSliceMeasureNumber <<
    ", " <<
    mfSingularOrPlural (
      sliceMeasuresVectorSize, "measure", "measures");

  if (sliceMeasuresVectorSize) {

    std::vector <S_msrMeasure>::const_iterator
      iBegin = fSliceMeasuresVector.begin (),
      iEnd   = fSliceMeasuresVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasure
        measure = (*i);

      // print the measure
      ss <<
        measure->asStringForMeasuresSlices ();

      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  ss <<
    ']';

  return ss.str ();
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
    "fSliceMeasureNumber: " << fSliceMeasureNumber <<
    std::endl;

  // print the slice measures vector
  if (sliceMeasuresVectorSize) {
    os <<
      "fSliceMeasuresVector: " <<
      std::endl;

    ++gIndenter;

    std::vector <S_msrMeasure>::const_iterator
      iBegin = fSliceMeasuresVector.begin (),
      iEnd   = fSliceMeasuresVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasure
        measure = (*i);

      // print the measure
      os <<
        measure->
          asStringForMeasuresSlices () <<
          std::endl;

      if (++i == iEnd) break;
      os << std::endl;
    } // for
// JMI    os << std::endl;

    --gIndenter;
  }

  // print the slice notes flat list
  size_t sliceNotesFlatListSize =
    fSliceNotesFlatList.size ();

  if (sliceNotesFlatListSize) {
    os <<
      "fSliceNotesFlatList: " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrNote>::const_iterator
      iBegin = fSliceNotesFlatList.begin (),
      iEnd   = fSliceNotesFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrNote note = (*i);

      // print the measure
      os <<
        note->
          asStringForMeasuresSlices ();

      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << std::endl;

    --gIndenter;
  }

  // print the slice notes events list
  size_t sliceNoteEventsListSize =
    fSliceNoteEventsList.size ();

  if (sliceNoteEventsListSize) {
    os <<
      "fSliceNoteEventsList: " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrNoteEvent>::const_iterator
      iBegin = fSliceNoteEventsList.begin (),
      iEnd   = fSliceNoteEventsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrNoteEvent noteEvent = (*i);

      // print the note event
      os <<
        noteEvent->
          asString();

      if (++i == iEnd) break;
      os << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }

  // print the simultaneous notes chunks list
  size_t sliceSimultaneousNotesChunksListSize =
    fSliceSimultaneousNotesChunksList.size ();

  if (sliceSimultaneousNotesChunksListSize) {
    os <<
      "fSliceSimultaneousNotesChunksList: " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrSimultaneousNotesChunk>::const_iterator
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

std::ostream& operator << (std::ostream& os, const S_msrMeasuresSlice& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrMeasuresSlicesSequence msrMeasuresSlicesSequence::create (
  const std::string& measuresOrigin)
{
  msrMeasuresSlicesSequence* obj =
    new msrMeasuresSlicesSequence (
      measuresOrigin);
  assert (obj != nullptr);
  return obj;
}

S_msrMeasuresSlicesSequence msrMeasuresSlicesSequence::create (
  const std::string& measuresOrigin,
  int           measuresNumber)
{
  msrMeasuresSlicesSequence* obj =
    new msrMeasuresSlicesSequence (
      measuresOrigin,
      measuresNumber);
  assert (obj != nullptr);
  return obj;
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
//     fMeasuresSlicesVector.push_back (
//       msrMeasuresSlice::create (
//         fSliceMeasureNumber,
//         fSlicePuristMeasureNumber) // JMI ??? 0.9.75 MISSING
//       );
  } // for
}

msrMeasuresSlicesSequence::~msrMeasuresSlicesSequence ()
{}

S_msrMeasuresSlicesSequence msrMeasuresSlicesSequence::createMeasuresSlicesSequenceShallowCopy (
  const std::string& measuresOrigin)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "Creating a shallow copy of measures slices sequence " <<
      this->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create shallow copy
  S_msrMeasuresSlicesSequence
    shallowCopy =
      msrMeasuresSlicesSequence::create (
        fMeasuresOrigin + "_" + measuresOrigin);

  // populate it
  for (
    std::vector <S_msrMeasuresSlice>::const_iterator i =
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "Appending measures slice " <<
      measuresSlice->asStringForMeasuresSlices () <<
      " to measures slices sequence " <<
      asStringForMeasuresSlices ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasuresSlicesVector.push_back (
    measuresSlice);
}

S_msrMeasuresSlicesSequence msrMeasuresSlicesSequence::mergeWithMeasuresSlicesSequence (
  const mfInputLineNumber& inputLineNumber,
  const std::string& measuresOrigin,
  const S_msrMeasuresSlicesSequence&
                otherMeasuresSlicesSequence)
{
  S_msrMeasuresSlicesSequence result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "Merging measures slices sequence " <<
      asStringForMeasuresSlices () <<
      " with " <<
      otherMeasuresSlicesSequence->asStringForMeasuresSlices ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  size_t slicesSequenceSize =
    getSlicesSequenceSize ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "---> slicesSequenceSize: " <<
      slicesSequenceSize;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (slicesSequenceSize == 0) {
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresSlices ()) {
      size_t
        otherMeasuresSlicesSequenceSize =
          otherMeasuresSlicesSequence->getSlicesSequenceSize ();

      std::stringstream ss;

      ss <<
        "msrMeasuresSlicesSequence()" <<
        ", otherMeasuresSlicesSequenceSize: " <<
        otherMeasuresSlicesSequenceSize;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
    // sanity check
    size_t
      otherMeasuresSlicesSequenceSize =
        otherMeasuresSlicesSequence->getSlicesSequenceSize ();

    if (otherMeasuresSlicesSequenceSize != slicesSequenceSize) {
      std::stringstream ss;

      ss <<
        "cannot enrich measures slices sequence " <<
        asShortString () <<
        " with " <<
        otherMeasuresSlicesSequence->asShortString () <<
        " which has not the same size";

      gLog <<
        std::endl <<
        S_msrMeasuresSlicesSequence (this) <<
        std::endl <<
        otherMeasuresSlicesSequence <<
        std::endl;

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }

    else {
#endif // MF_SANITY_CHECKS_ARE_ENABLED
      if (slicesSequenceSize) {
        // the two measures slices sequences have the same non null size

        // get the other sequence's measures slices vector
        const std::vector <S_msrMeasuresSlice>&
          otherMeasuresSlicesVector =
            otherMeasuresSlicesSequence->
              getMeasuresSlicesVector ();

        // iterate over the measures slices
        for (size_t i = 0; i < slicesSequenceSize; ++i) {
          S_msrMeasuresSlice
            currentSlice =
              fMeasuresSlicesVector.at (i);

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
//               inputLineNumber,
              currentSlice);

          // append the measures from the other sequence's slice to resultSlice
          resultSlice->
            appendSliceMeasuresFrom (
//               inputLineNumber,
              otherMeasuresSlicesVector.at (i));
        } // for
      }
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
    }
#endif // MF_SANITY_CHECKS_ARE_ENABLED
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "---> mergeWithMeasuresSlicesSequence()" <<
      ", result: " <<
      std::endl;
    ++gIndenter;
    gLog <<
      result->asStringForMeasuresSlices () <<
      std::endl;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

void msrMeasuresSlicesSequence::identifySoloNotesAndRests ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "Identifying solo notes and rests in measures slices sequence " <<
      asStringForMeasuresSlices () <<
      ":" <<
      std::endl;

    ++gIndenter;
    gLog << std::hex << std::showbase << this << std::dec; // JMI 0.9.69
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // collect the notes from the sequence's measures slices
  for (
    std::vector <S_msrMeasuresSlice>::const_iterator i =
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
  std::stringstream ss;

  ss <<
    "[MeasuresSlicesSequence" <<
    ", fMeasuresOrigin: " << fMeasuresOrigin <<
    ", " <<
    mfSingularOrPlural (
      fMeasuresSlicesVector.size (), "measure slice", "measures slices") <<
    ']';

  return ss.str ();
}

std::string msrMeasuresSlicesSequence::asShortString () const
{
  return asString ();
}

std::string msrMeasuresSlicesSequence::asStringForMeasuresSlices () const
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

    std::vector <S_msrMeasuresSlice>::const_iterator
      iBegin = fMeasuresSlicesVector.begin (),
      iEnd   = fMeasuresSlicesVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasuresSlice measuresSlice = (*i);

      // print the measures slice
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

void msrMeasuresSlicesSequence::printMeasuresSlicesVector (std::ostream& os) const
{
  size_t measuresSlicesVectorSize =
    fMeasuresSlicesVector.size ();

  gLog <<
    "MeasuresSlicesSequence contains " <<
    mfSingularOrPlural (
      measuresSlicesVectorSize,
      "slice", "slices") <<
    std::endl;

  ++gIndenter;

  os <<
    "measuresOrigin: " << fMeasuresOrigin <<
    std::endl;
  --gIndenter;

  if (measuresSlicesVectorSize) {
    os << std::endl;
    ++gIndenter;

    std::vector <S_msrMeasuresSlice>::const_iterator
      iBegin = fMeasuresSlicesVector.begin (),
      iEnd   = fMeasuresSlicesVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasuresSlice measuresSlice = (*i);

      // print the measures slice
      os <<
        measuresSlice;

      if (++i == iEnd) break;
      os << std::endl;
    } // for

    os << std::endl;
    --gIndenter;
  }

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrMeasuresSlicesSequence& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
