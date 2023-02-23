/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "mfStaticSettings.h"

#include "visitor.h"

#include "mfAssert.h"

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "mfStaticSettings.h"

#include "msrMeasureConstants.h"

#include "msrDoubleTremolos.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrDoubleTremoloTypeKindAsString (
  msrDoubleTremoloTypeKind tremoloTypeKind)
{
  std::string result;

  switch (tremoloTypeKind) {
    case msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN:
      result = "kDoubleTremoloType_UNKNOWN";
      break;
    case msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle:
      result = "kDoubleTremoloTypeSingle";
      break;
    case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart:
      result = "kDoubleTremoloTypeStart";
      break;
    case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop:
      result = "kDoubleTremoloTypeStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrDoubleTremoloTypeKind& elt)
{
  os << msrDoubleTremoloTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrDoubleTremolo msrDoubleTremolo::create (
  int                       inputLineNumber,
  const S_msrMeasure&       upLinkToMeasure,
  msrDoubleTremoloKind      doubleTremoloKind,
  msrDoubleTremoloTypeKind  doubleDoubleTremoloTypeKind,
  int                       doubleTremoloMarksNumber,
  msrPlacementKind          doubleTremoloPlacementKind)
{
  msrDoubleTremolo* o =
    new msrDoubleTremolo (
      inputLineNumber,
      upLinkToMeasure,
      doubleTremoloKind,
      doubleDoubleTremoloTypeKind,
      doubleTremoloMarksNumber,
      doubleTremoloPlacementKind);
  assert (o != nullptr);
  return o;
}

S_msrDoubleTremolo msrDoubleTremolo::create (
  int                       inputLineNumber,
  msrDoubleTremoloKind      doubleTremoloKind,
  msrDoubleTremoloTypeKind  doubleDoubleTremoloTypeKind,
  int                       doubleTremoloMarksNumber,
  msrPlacementKind          doubleTremoloPlacementKind)
{
  return
    msrDoubleTremolo::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      doubleTremoloKind,
      doubleDoubleTremoloTypeKind,
      doubleTremoloMarksNumber,
      doubleTremoloPlacementKind);
}

msrDoubleTremolo::msrDoubleTremolo (
  int                       inputLineNumber,
  const S_msrMeasure&       upLinkToMeasure,
  msrDoubleTremoloKind      doubleTremoloKind,
  msrDoubleTremoloTypeKind  doubleDoubleTremoloTypeKind,
  int                       doubleTremoloMarksNumber,
  msrPlacementKind          doubleTremoloPlacementKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fDoubleTremoloKind           = doubleTremoloKind;
  fDoubleDoubleTremoloTypeKind = doubleDoubleTremoloTypeKind;
  fDoubleTremoloMarksNumber    = doubleTremoloMarksNumber;
  fDoubleTremoloPlacementKind  = doubleTremoloPlacementKind;

  fDoubleTremoloSoundingWholeNotes =
    msrWholeNotes (-1, 1); // will be set later

  fDoubleTremoloNumberOfRepeats = -1; // will be set later
}

msrDoubleTremolo::~msrDoubleTremolo ()
{}

void msrDoubleTremolo::setDoubleTremoloUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLog <<
      "==> Setting the uplink to measure of double tremolo " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fDoubleTremoloUpLinkToMeasure = measure;
}

S_msrDoubleTremolo msrDoubleTremolo::createDoubleTremoloNewbornClone (
  const S_msrElement& doubleTremoloFirstElement,
  const S_msrElement& doubleTremoloSecondElement)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of a double tremolo" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    doubleTremoloFirstElement != nullptr,
    "doubleTremoloFirstElement is null");

  mfAssert (
    __FILE__, __LINE__,
    doubleTremoloSecondElement != nullptr,
    "doubleTremoloSecondElement is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrDoubleTremolo
    newbornClone =
      msrDoubleTremolo::create (
        fInputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        fDoubleTremoloKind,
        fDoubleDoubleTremoloTypeKind,
        fDoubleTremoloMarksNumber,
        fDoubleTremoloPlacementKind);

  newbornClone->fDoubleTremoloFirstElement =
    doubleTremoloFirstElement;
  newbornClone->fDoubleTremoloSecondElement =
    doubleTremoloSecondElement;

  newbornClone->fDoubleTremoloSoundingWholeNotes =
    fDoubleTremoloSoundingWholeNotes;

  newbornClone->fDoubleTremoloElementsWholeNotes =
    fDoubleTremoloElementsWholeNotes;

  newbornClone->fDoubleTremoloNumberOfRepeats =
    fDoubleTremoloNumberOfRepeats;

  newbornClone->fDoubleTremoloPlacementKind =
    fDoubleTremoloPlacementKind;

  return newbornClone;
}

// void msrDoubleTremolo::setMeasurePosition (
//   const S_msrMeasure& measure,
//   const msrWholeNotes&     measurePosition,
//   const std::string&  context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasurePositions ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting measure element measure position of double tremolo " <<
//       asString () <<
//       " to " <<
//       measurePosition <<
//       " (was " <<
//       fMeasurePosition <<
//       ") in measure " <<
//       measure->asShortString () <<
//       " (measureElementMeasureNumber: " <<
//       fBarLineUpLinkToMeasure->getMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measurePosition != K_MEASURE_POSITION_UNKNOWN,
//     "measurePosition == K_MEASURE_POSITION_UNKNOWN");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // set double tremolo's measure position
//   fMeasurePosition = measurePosition;
// }

// void msrDoubleTremolo::setMeasurePosition ( ??? JMI v0.9.66
//   const msrWholeNotes& measurePosition)
// {
//   std::string context =
//     "setMeasurePosition()";
//
//   setMeasurePosition ( // JMI v0.9.66 CLEAN THAT!!!
//     measurePosition,
//     context);
//
//   if (false) { // JMI
//   // compute double tremolo's voice position
//   msrWholeNotes
//      voicePosition =
//       fDoubleTremoloUpLinkToMeasure->getMeasureVoicePosition ()
//         +
//       measurePosition;
//
//   // set double tremolo's voice position
//   setVoicePosition (
//     voicePosition,
//     context);
// }
//
//   // update current voice position
//   S_msrVoice
//     voice =
//       fDoubleTremoloUpLinkToMeasure->
//         fetchMeasureUpLinkToVoice ();
//
//   // the two elements of a double tremolo are notes or chords
//   switch (fDoubleTremoloKind) {
//     case msrDoubleTremoloKind::kDoubleTremoloNotes:
//       if (
//         S_msrNote
//           note =
//             dynamic_cast<msrNote*>(&(*fDoubleTremoloFirstElement))
//       ) {
//         voice->
//           incrementCurrentVoicePosition (
//             note->
//               getSoundingWholeNotes ());
//       }
//       else {
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, __LINE__,
//           "notes double tremolo first element should be a note");
//       }
//       break;
//
//     case msrDoubleTremoloKind::kDoubleTremoloChords:
//       if (
//         S_msrChord
//           chord =
//             dynamic_cast<msrChord*>(&(*fDoubleTremoloFirstElement))
//       ) {
//         voice->
//           incrementCurrentVoicePosition (
//             chord->
//               getSoundingWholeNotes ());
//       }
//       else {
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, __LINE__,
//           "chords double tremolo first element should be a chord");
//       }
//       break;
//   } // switch
// }

void msrDoubleTremolo::setDoubleTremoloNoteFirstElement (const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Setting note " << note->asShortString () <<
      " as first element of double tremolo " <<
      asShortString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int inputLineNumber =
    note->getInputLineNumber ();

  // register note as first element of this double tremolo
  fDoubleTremoloFirstElement = note;

  // mark it as being a double tremolo first element
  note->
    setNoteIsFirstNoteInADoubleTremolo ();

  // fetch note display whole notes
  msrWholeNotes
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();

  // set double tremolo sounding whole notes
  // to the note's display whole notes,
   // i.e. the duration of each of the two notes
  fDoubleTremoloSoundingWholeNotes =
    noteDisplayWholeNotes;

  // compute double tremolo elements duration
  // the marks number determines the duration of the two elements:
  // '8' for 1, '16' for 2, etc
  fDoubleTremoloElementsWholeNotes =
    msrWholeNotes (
      1,
      /* JMI
      int (
        pow (
          2,
          fDoubleTremoloMarksNumber + 2)));
          */
      1 << (fDoubleTremoloMarksNumber + 2));

  // set note's sounding whole notes
  note->
    setSoundingWholeNotes (
      fDoubleTremoloElementsWholeNotes,
      "msrDoubleTremolo::setDoubleTremoloNoteFirstElement()");

  // setting number of repeats
  mfRational
    numberOfRepeatsAsRational =
      fDoubleTremoloSoundingWholeNotes
        /
      fDoubleTremoloElementsWholeNotes
        /
      2; // there are two repeated notes

  if (numberOfRepeatsAsRational.getDenominator () != 1) {
    std::stringstream ss;

    ss <<
      "cannot handle numberOfRepeatsAsRational.getDenominator () != 1" <<
      std::endl <<
      "numberOfRepeatsAsRational = " <<
      numberOfRepeatsAsRational <<
      std::endl <<
      "fDoubleTremoloSoundingWholeNotes = '" <<
      fDoubleTremoloSoundingWholeNotes << "'" <<
      std::endl <<
      "fDoubleTremoloElementsWholeNotes = '" <<
      fDoubleTremoloElementsWholeNotes << "'" <<
      std::endl <<
      "line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fDoubleTremoloNumberOfRepeats =
    numberOfRepeatsAsRational.getNumerator ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Setting notes double tremolo number of repeats to '" <<
      fDoubleTremoloNumberOfRepeats <<
      "', fDoubleTremoloSoundingWholeNotes = '" <<
      fDoubleTremoloSoundingWholeNotes << "'" <<
      ", fDoubleTremoloElementsWholeNotes = '" <<
      fDoubleTremoloElementsWholeNotes << "'" <<
      "', line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrDoubleTremolo::setDoubleTremoloNoteSecondElement (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Setting note " << note->asShortString () <<
      " as second element of double tremolo " <<
      asShortString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int inputLineNumber =
    note->getInputLineNumber ();

  // register note as second element of this double tremolo
  fDoubleTremoloSecondElement = note;

  // mark it as being a double tremolo second element
  note->
    setNoteIsSecondNoteInADoubleTremolo ();

  // fetch note display whole notes
  msrWholeNotes
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();

  // compute expected double tremolo sounding whole notes
  msrWholeNotes
    expectedDoubleTremoloSoundingWholeNotes =
      noteDisplayWholeNotes;

  // check that expected double tremolo sounding whole notes
  // match the known double tremolo sounding whole notes
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Checking notes double tremolo second note duration" <<
      ", doubleTremoloNumberOfRepeats = '" <<
      fDoubleTremoloNumberOfRepeats <<
      "', doubleTremoloSoundingWholeNotes = '" <<
      fDoubleTremoloSoundingWholeNotes <<
      "', doubleTremoloElementsNotesDuration = '" <<
      fDoubleTremoloElementsWholeNotes << "'" <<
      "', line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (
    expectedDoubleTremoloSoundingWholeNotes
      !=
    fDoubleTremoloSoundingWholeNotes) {
    std::stringstream ss;

    ss <<
      "attempt to set notes double tremolo whole notes both to " <<
      fDoubleTremoloSoundingWholeNotes << " (existing)" <<
      " and " <<
      expectedDoubleTremoloSoundingWholeNotes <<
      " on note second element:" <<
      std::endl;

    ++gIndenter;

    ss <<
      note <<
      std::endl;

    --gIndenter;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  // set note's sounding whole notes
  note->
    setSoundingWholeNotes (
      fDoubleTremoloElementsWholeNotes,
      "msrDoubleTremolo::setDoubleTremoloNoteSecondElement()");
}

void msrDoubleTremolo::setDoubleTremoloChordFirstElement (
  const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Setting chord " << chord->asString () <<
      " as first element of double tremolo " <<
      asShortString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register chord as first element of this double tremolo
  fDoubleTremoloFirstElement = chord;

  // mark it as being a double tremolo first element
  chord->
    setChordIsFirstChordInADoubleTremolo ();

  // fetch chord displayed whole notes
  msrWholeNotes
    chordDisplayWholeNotes =
      chord->
        getChordDisplayWholeNotes ();

  // compute expected double tremolo sounding whole notes
  msrWholeNotes
    expectedDoubleTremoloSoundingWholeNotes =
      chordDisplayWholeNotes * 2;

  // set double tremolo whole notes to the chords's displayed whole notes
  if (fDoubleTremoloSoundingWholeNotes.getNumerator () != 0) {
    if (
      expectedDoubleTremoloSoundingWholeNotes
        !=
      fDoubleTremoloSoundingWholeNotes
    ) {
      std::stringstream ss;

      ss <<
        "attempt to set chord double tremolo sounding whole notes both to " <<
        fDoubleTremoloSoundingWholeNotes << " (existing)" <<
        " and " <<
        expectedDoubleTremoloSoundingWholeNotes <<
        " on chord first element:" <<
        std::endl;

      ++gIndenter;

      ss <<
        chord->asString () <<
        std::endl;

      --gIndenter;

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        chord->getInputLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  fDoubleTremoloSoundingWholeNotes =
    chordDisplayWholeNotes * 2; // taking the second note into account
}

void msrDoubleTremolo::setDoubleTremoloChordSecondElement (const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Setting chord " << chord->asString () <<
      " as second element of double tremolo " <<
      asShortString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register chord as second element of this double tremolo
  fDoubleTremoloSecondElement = chord;

  // mark it as being a double tremolo second element
  chord->
    setChordIsSecondChordInADoubleTremolo ();

  // fetch chord displayed whole notes
  msrWholeNotes
    chordDisplayWholeNotes =
      chord->
        getChordDisplayWholeNotes ();

  // set double tremolo whole notes to the chords's displayed whole notes
  if (fDoubleTremoloSoundingWholeNotes.getNumerator () != 0) {
    if (chordDisplayWholeNotes != fDoubleTremoloSoundingWholeNotes) { // JMI
      std::stringstream ss;

     ss <<
        "attempt to set double tremolo whole notes both to " <<
        fDoubleTremoloSoundingWholeNotes << " (existing)" <<
        " and " <<
        chordDisplayWholeNotes <<
        " on chord second element:" << " (chord)" <<
        std::endl;

      ++gIndenter;

      ss <<
        chord <<
        std::endl;

      --gIndenter;

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        chord->getInputLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  else {
    fDoubleTremoloSoundingWholeNotes =
      chordDisplayWholeNotes;
  }
}

void msrDoubleTremolo::setDoubleTremoloMeasureNumber (
  const std::string& measureNumber)
{
//   fDoubleTremoloUpLinkToMeasure->getMeasureNumber () =  measureNumber;
}

void msrDoubleTremolo::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDoubleTremolo::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDoubleTremolo::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrDoubleTremolo::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDoubleTremolo::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDoubleTremolo::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrDoubleTremolo::browseData (basevisitor* v)
{
  if (fDoubleTremoloFirstElement) {
    // browse the first element
    msrBrowser<msrElement> browser (v);
    browser.browse (*fDoubleTremoloFirstElement);
  }

  if (fDoubleTremoloSecondElement) {
    // browse the second element
    msrBrowser<msrElement> browser (v);
    browser.browse (*fDoubleTremoloSecondElement);
  }
}

std::string msrDoubleTremoloKindAsString (
  msrDoubleTremoloKind doubleTremolotKind)
{
  std::string result;

  switch (doubleTremolotKind) {
    case msrDoubleTremoloKind::kDoubleTremoloNotes:
      result = "Notes";
      break;
    case msrDoubleTremoloKind::kDoubleTremoloChords:
      result = "Chords";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrDoubleTremoloKind& elt)
{
  os << msrDoubleTremoloKindAsString (elt);
  return os;
}

std::string msrDoubleTremolo::doubleTremoloPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fDoubleTremoloPlacementKind);
}

std::string msrDoubleTremolo::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[DoubleTremolo" <<
    ", fDoubleDoubleTremoloTypeKind: " << fDoubleDoubleTremoloTypeKind <<
    ", fDoubleTremoloMarksNumber: " <<
    mfSingularOrPlural (
      fDoubleTremoloMarksNumber, "mark", "marks") <<
    ", placement: " << doubleTremoloPlacementKindAsString () <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrDoubleTremolo::asString () const
{
  std::stringstream ss;

  ss <<
    "[DoubleTremolo " <<
    ", fDoubleDoubleTremoloTypeKind: " << fDoubleDoubleTremoloTypeKind <<
    ", fDoubleTremoloMarksNumber: " <<fDoubleTremoloMarksNumber <<
    ", placement: " << doubleTremoloPlacementKindAsString () <<
    ",fDoubleTremoloSoundingWholeNotes: " << fDoubleTremoloSoundingWholeNotes <<
    ", line " << fInputLineNumber;

  if (fDoubleTremoloFirstElement) { // it may not be set yet
    ss <<
      ", first element " << " = ";

    switch (fDoubleTremoloKind) {
      case msrDoubleTremoloKind::kDoubleTremoloNotes:
        if (
          S_msrNote
            note =
              dynamic_cast<msrNote*>(&(*fDoubleTremoloFirstElement))
        ) {
          note->asShortString ();
        }
        else {
          msrInternalError (
            gServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            "notes double tremolo first element should be a note");
        }
        break;

      case msrDoubleTremoloKind::kDoubleTremoloChords:
        if (
          S_msrChord
            chord =
              dynamic_cast<msrChord*>(&(*fDoubleTremoloFirstElement))
        ) {
          chord->asString ();
        }
        else {
          msrInternalError (
            gServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            "chords double tremolo first element should be a chord");
        }
        break;
    } // switch
  }

  if (fDoubleTremoloSecondElement) { // it may not be set yet
    ss <<
      ", second element " << " = ";

    switch (fDoubleTremoloKind) {
      case msrDoubleTremoloKind::kDoubleTremoloNotes:
        if (
          S_msrNote
            note =
              dynamic_cast<msrNote*>(&(*fDoubleTremoloSecondElement))
        ) {
          note->asShortString ();
        }
        else {
          msrInternalError (
            gServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            "notes double tremolo second element should be a note");
        }
        break;

      case msrDoubleTremoloKind::kDoubleTremoloChords:
        if (
          S_msrChord
            chord =
              dynamic_cast<msrChord*>(&(*fDoubleTremoloSecondElement))
        ) {
          chord->asString ();
        }
        else {
          msrInternalError (
            gServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            "chords double tremolo second element should be a chord");
        }
        break;
    } // switch
  }

  ss << ']';

  return ss.str ();
}

void msrDoubleTremolo::print (std::ostream& os) const
{
  os <<
    "[DoubleTremolo" <<
    ", " << msrDoubleTremoloTypeKindAsString (fDoubleDoubleTremoloTypeKind) <<
    ", on " << msrDoubleTremoloKindAsString (fDoubleTremoloKind) <<
    ", line " << fInputLineNumber <<
    std::endl;

  const int fieldWidth = 32;

  ++gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "doubleTremoloSoundingWholeNotes" << ": " <<
    fDoubleTremoloSoundingWholeNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "doubleTremoloMarksNumber" << ": " <<
    fDoubleTremoloMarksNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "doubleTremoloElementsNotesDuration" << ": " <<
    fDoubleTremoloElementsWholeNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "numberOfRepeats" << ": " <<
    fDoubleTremoloNumberOfRepeats <<
    std::endl <<

    std::setw (fieldWidth) <<
    "placement" << ": " <<
    doubleTremoloPlacementKindAsString () <<
    std::endl;

  os <<
    "First element:";
  if (fDoubleTremoloFirstElement) { // it may not yet be set
    os << std::endl;

    ++gIndenter;
    os <<
      fDoubleTremoloFirstElement;
    --gIndenter;
  }
  else {
    os <<
      " [NONE]" <<
      std::endl;
  }

  os <<
    "Second element:";
  if (fDoubleTremoloSecondElement) { // it may not yet be set
    os << std::endl;

    ++gIndenter;
    os <<
      fDoubleTremoloSecondElement;
    --gIndenter;
  }
  else {
    os <<
      " [NONE]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrDoubleTremolo& elt)
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
