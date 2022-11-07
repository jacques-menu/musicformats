/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrMeasures.h"

#include "msrDoubleTremolos.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrDoubleTremolo msrDoubleTremolo::create (
  int                  inputLineNumber,
  S_msrMeasure         upLinkToMeasure,
  msrDoubleTremoloKind doubleTremoloKind,
  msrTremoloTypeKind   doubleTremoloTypeKind,
  int                  doubleTremoloMarksNumber,
  msrPlacementKind     doubleTremoloPlacementKind)
{
  msrDoubleTremolo* o =
    new msrDoubleTremolo (
      inputLineNumber,
      upLinkToMeasure,
      doubleTremoloKind,
      doubleTremoloTypeKind,
      doubleTremoloMarksNumber,
      doubleTremoloPlacementKind);
  assert (o != nullptr);
  return o;
}

msrDoubleTremolo::msrDoubleTremolo (
  int                  inputLineNumber,
  S_msrMeasure         upLinkToMeasure,
  msrDoubleTremoloKind doubleTremoloKind,
  msrTremoloTypeKind   doubleTremoloTypeKind,
  int                  doubleTremoloMarksNumber,
  msrPlacementKind     doubleTremoloPlacementKind)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{
  fDoubleTremoloKind          = doubleTremoloKind;
  fDoubleTremoloTypeKind      = doubleTremoloTypeKind;
  fDoubleTremoloMarksNumber   = doubleTremoloMarksNumber;
  fDoubleTremoloPlacementKind = doubleTremoloPlacementKind;

  fDoubleTremoloSoundingWholeNotes =
    Rational (-1, 1); // will be set later

  fDoubleTremoloNumberOfRepeats = -1; // will be set later
}

S_msrDoubleTremolo msrDoubleTremolo::createDoubleTremoloNewbornClone (
  S_msrElement doubleTremoloFirstElement,
  S_msrElement doubleTremoloSecondElement)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Creating a newborn clone of a double tremolo" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    doubleTremoloFirstElement != nullptr,
    "doubleTremoloFirstElement is null");
  mfAssert (
    __FILE__, __LINE__,
    doubleTremoloSecondElement != nullptr,
    "doubleTremoloSecondElement is null");

  S_msrDoubleTremolo
    newbornClone =
      msrDoubleTremolo::create (
        fInputLineNumber,
        nullptr, // will be set when double tremolo is appended to a measure JMI v0.9.66 PIM
        fDoubleTremoloKind,
        fDoubleTremoloTypeKind,
        fDoubleTremoloMarksNumber,
        fDoubleTremoloPlacementKind);

  newbornClone->fDoubleTremoloFirstElement =
    doubleTremoloFirstElement;
  newbornClone->fDoubleTremoloSecondElement =
    doubleTremoloSecondElement;

  newbornClone->fDoubleTremoloSoundingWholeNotes =
    fDoubleTremoloSoundingWholeNotes;

  newbornClone->fDoubleTremoloElementsDuration =
    fDoubleTremoloElementsDuration;

  newbornClone->fDoubleTremoloNumberOfRepeats =
    fDoubleTremoloNumberOfRepeats;

  newbornClone->fDoubleTremoloPlacementKind =
    fDoubleTremoloPlacementKind;

  return newbornClone;
}

msrDoubleTremolo::~msrDoubleTremolo ()
{}

void msrDoubleTremolo::setDoubleTremoloMeasurePosition (
  const S_msrMeasure measure,
  const Rational&    measurePosition,
  const string&      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    ++gIndenter;

    gLogStream <<
      "Setting measure element position in measure of double tremolo " <<
      asString () <<
      " to " <<
      measurePosition <<
      " (was " <<
      fMeasureElementMeasurePosition <<
      ") in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      fetchMeasureElementMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"" <<
      endl;

    --gIndenter;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set double tremolo's position in measure
  fMeasureElementMeasurePosition = measurePosition;
}

// void msrDoubleTremolo::setDoubleTremoloMeasurePosition ( ??? JMI v0.9.66
//   const Rational& measurePosition)
// {
//   string context =
//     "setDoubleTremoloMeasurePosition()";
//
//   setDoubleTremoloMeasurePosition ( // JMI v0.9.66 CLEAN THAT!!!
//     measurePosition,
//     context);
//
//   if (false) { // JMI
//   // compute double tremolo's position in voice
//   Rational
//      voicePosition =
//       fDoubleTremoloUpLinkToMeasure->getMeasureVoicePosition ()
//         +
//       measurePosition;
//
//   // set double tremolo's position in voice
//   setMeasureElementVoicePosition (
//     voicePosition,
//     context);
// }
//
//   // update current position in voice
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
//               getMeasureElementSoundingWholeNotes ());
//       }
//       else {
//         msrInternalError (
//           gGlobalServiceRunData->getInputSourceName (),
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
//               getMeasureElementSoundingWholeNotes ());
//       }
//       else {
//         msrInternalError (
//           gGlobalServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, __LINE__,
//           "chords double tremolo first element should be a chord");
//       }
//       break;
//   } // switch
// }

void msrDoubleTremolo::setDoubleTremoloNoteFirstElement (S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Setting note " << note->asShortString () <<
      " as first element of double tremolo " <<
      asShortString () <<
      endl;
  }
#endif

  int inputLineNumber =
    note->getInputLineNumber ();

  // register note as first element of this double tremolo
  fDoubleTremoloFirstElement = note;

  // mark it as being a double tremolo first element
  note->
    setNoteIsFirstNoteInADoubleTremolo ();

  // fetch note display whole notes
  Rational
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
  fDoubleTremoloElementsDuration =
    Rational (
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
    setMeasureElementSoundingWholeNotes (
      fDoubleTremoloElementsDuration,
      "msrDoubleTremolo::setDoubleTremoloNoteFirstElement()");

  // setting number of repeats
  Rational
    numberOfRepeatsAsRational =
      fDoubleTremoloSoundingWholeNotes
        /
      fDoubleTremoloElementsDuration
        /
      2; // there are two repeated notes

  if (numberOfRepeatsAsRational.getDenominator () != 1) {
    stringstream s;

    s <<
      "cannot handle numberOfRepeatsAsRational.getDenominator () != 1" <<
      endl <<
      "numberOfRepeatsAsRational = " <<
      numberOfRepeatsAsRational <<
      endl <<
      "fDoubleTremoloSoundingWholeNotes = '" <<
      fDoubleTremoloSoundingWholeNotes << "'" <<
      endl <<
      "fDoubleTremoloElementsDuration = '" <<
      fDoubleTremoloElementsDuration << "'" <<
      endl <<
      "line " << inputLineNumber;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fDoubleTremoloNumberOfRepeats =
    numberOfRepeatsAsRational.getNumerator ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Setting notes double tremolo number of repeats to '" <<
      fDoubleTremoloNumberOfRepeats <<
      "', fDoubleTremoloSoundingWholeNotes = '" <<
      fDoubleTremoloSoundingWholeNotes << "'" <<
      ", fDoubleTremoloElementsDuration = '" <<
      fDoubleTremoloElementsDuration << "'" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif
}

void msrDoubleTremolo::setDoubleTremoloNoteSecondElement (
  S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Setting note " << note->asShortString () <<
      " as second element of double tremolo " <<
      asShortString () <<
      endl;
  }
#endif

  int inputLineNumber =
    note->getInputLineNumber ();

  // register note as second element of this double tremolo
  fDoubleTremoloSecondElement = note;

  // mark it as being a double tremolo second element
  note->
    setNoteIsSecondNoteInADoubleTremolo ();

  // fetch note display whole notes
  Rational
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();

  // compute expected double tremolo sounding whole notes
  Rational
    expectedDoubleTremoloSoundingWholeNotes =
      noteDisplayWholeNotes;

  // check that expected double tremolo sounding whole notes
  // match the known double tremolo sounding whole notes
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Checking notes double tremolo second note duration" <<
      ", doubleTremoloNumberOfRepeats = '" <<
      fDoubleTremoloNumberOfRepeats <<
      "', doubleTremoloSoundingWholeNotes = '" <<
      fDoubleTremoloSoundingWholeNotes <<
      "', doubleTremoloElementsDuration = '" <<
      fDoubleTremoloElementsDuration << "'" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  if (
    expectedDoubleTremoloSoundingWholeNotes
      !=
    fDoubleTremoloSoundingWholeNotes) {
    stringstream s;

    s <<
      "attempt to set notes double tremolo whole notes both to " <<
      fDoubleTremoloSoundingWholeNotes << " (existing)" <<
      " and " <<
      expectedDoubleTremoloSoundingWholeNotes <<
      " on note second element:" <<
      endl;

    ++gIndenter;

    s <<
      note <<
      endl;

    --gIndenter;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // set note's sounding whole notes
  note->
    setMeasureElementSoundingWholeNotes (
      fDoubleTremoloElementsDuration,
      "msrDoubleTremolo::setDoubleTremoloNoteSecondElement()");
}

void msrDoubleTremolo::setDoubleTremoloChordFirstElement (
  S_msrChord chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Setting chord " << chord->asString () <<
      " as first element of double tremolo " <<
      asShortString () <<
      endl;
  }
#endif

  // register chord as first element of this double tremolo
  fDoubleTremoloFirstElement = chord;

  // mark it as being a double tremolo first element
  chord->
    setChordIsFirstChordInADoubleTremolo ();

  // fetch chord displayed whole notes
  Rational
    chordDisplayWholeNotes =
      chord->
        getChordDisplayWholeNotes ();

  // compute expected double tremolo sounding whole notes
  Rational
    expectedDoubleTremoloSoundingWholeNotes =
      chordDisplayWholeNotes * 2;

  // set double tremolo whole notes to the chords's displayed whole notes
  if (fDoubleTremoloSoundingWholeNotes.getNumerator () != 0) {
    if (
      expectedDoubleTremoloSoundingWholeNotes
        !=
      fDoubleTremoloSoundingWholeNotes
    ) {
      stringstream s;

      s <<
        "attempt to set chord double tremolo sounding whole notes both to " <<
        fDoubleTremoloSoundingWholeNotes << " (existing)" <<
        " and " <<
        expectedDoubleTremoloSoundingWholeNotes <<
        " on chord first element:" <<
        endl;

      ++gIndenter;

      s <<
        chord->asString () <<
        endl;

      --gIndenter;

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        chord->getInputLineNumber (),
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fDoubleTremoloSoundingWholeNotes =
    chordDisplayWholeNotes * 2; // taking the second note into account
}

void msrDoubleTremolo::setDoubleTremoloChordSecondElement (S_msrChord chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Setting chord " << chord->asString () <<
      " as second element of double tremolo " <<
      asShortString () <<
      endl;
  }
#endif

  // register chord as second element of this double tremolo
  fDoubleTremoloSecondElement = chord;

  // mark it as being a double tremolo second element
  chord->
    setChordIsSecondChordInADoubleTremolo ();

  // fetch chord displayed whole notes
  Rational
    chordDisplayWholeNotes =
      chord->
        getChordDisplayWholeNotes ();

  // set double tremolo whole notes to the chords's displayed whole notes
  if (fDoubleTremoloSoundingWholeNotes.getNumerator () != 0) {
    if (chordDisplayWholeNotes != fDoubleTremoloSoundingWholeNotes) { // JMI
      stringstream s;

     s <<
        "attempt to set double tremolo whole notes both to " <<
        fDoubleTremoloSoundingWholeNotes << " (existing)" <<
        " and " <<
        chordDisplayWholeNotes <<
        " on chord second element:" << " (chord)" <<
        endl;

      ++gIndenter;

      s <<
        chord <<
        endl;

      --gIndenter;

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        chord->getInputLineNumber (),
        __FILE__, __LINE__,
        s.str ());
    }
  }

  else {
    fDoubleTremoloSoundingWholeNotes =
      chordDisplayWholeNotes;
  }
}

void msrDoubleTremolo::setDoubleTremoloMeasureNumber (
  const string& measureNumber)
{
  fetchMeasureElementMeasureNumber () =  measureNumber;
}

void msrDoubleTremolo::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDoubleTremolo::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDoubleTremolo::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDoubleTremolo::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrDoubleTremolo::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrDoubleTremolo::visitEnd ()" <<
            endl;
        }
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

string msrDoubleTremoloKindAsString (
  msrDoubleTremoloKind doubleTremolotKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrDoubleTremoloKind& elt)
{
  os << msrDoubleTremoloKindAsString (elt);
  return os;
}

string msrDoubleTremolo::doubleTremoloPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fDoubleTremoloPlacementKind);
}

string msrDoubleTremolo::asShortString () const
{
  stringstream s;

  s <<
    "[DoubleTremolo" <<
    ", fDoubleTremoloTypeKind: " << fDoubleTremoloTypeKind <<
    ", fDoubleTremoloMarksNumber: " <<
    mfSingularOrPlural (
      fDoubleTremoloMarksNumber, "mark", "marks") <<
    ", placement: " << doubleTremoloPlacementKindAsString () <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

string msrDoubleTremolo::asString () const
{
  stringstream s;

  s <<
    "[DoubleTremolo " <<
    ", fDoubleTremoloTypeKind: " << fDoubleTremoloTypeKind <<
    ", fDoubleTremoloMarksNumber: " <<fDoubleTremoloMarksNumber <<
    ", placement: " << doubleTremoloPlacementKindAsString () <<
    ",fDoubleTremoloSoundingWholeNotes: " << fDoubleTremoloSoundingWholeNotes <<
    ", line " << fInputLineNumber;

  if (fDoubleTremoloFirstElement) { // it may not be set yet
    s <<
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
            gGlobalServiceRunData->getInputSourceName (),
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
            gGlobalServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            "chords double tremolo first element should be a chord");
        }
        break;
    } // switch
  }

  if (fDoubleTremoloSecondElement) { // it may not be set yet
    s <<
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
            gGlobalServiceRunData->getInputSourceName (),
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
            gGlobalServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            "chords double tremolo second element should be a chord");
        }
        break;
    } // switch
  }

  s << ']';

  return s.str ();
}

void msrDoubleTremolo::print (ostream& os) const
{
  os <<
    "[DoubleTremolo" <<
    ", " << msrTremoloTypeKindAsString (fDoubleTremoloTypeKind) <<
    ", on " << msrDoubleTremoloKindAsString (fDoubleTremoloKind) <<
    ", line " << fInputLineNumber <<
    endl;

  const int fieldWidth = 32;

  ++gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "doubleTremoloSoundingWholeNotes" << " : " <<
    fDoubleTremoloSoundingWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "doubleTremoloMarksNumber" << " : " <<
    fDoubleTremoloMarksNumber <<
    endl <<

    setw (fieldWidth) <<
    "doubleTremoloElementsDuration" << " : " <<
    fDoubleTremoloElementsDuration <<
    endl <<

    setw (fieldWidth) <<
    "numberOfRepeats" << " : " <<
    fDoubleTremoloNumberOfRepeats <<
    endl <<

    setw (fieldWidth) <<
    "placement" << " : " <<
    doubleTremoloPlacementKindAsString () <<
    endl;

  os <<
    "First element:";
  if (fDoubleTremoloFirstElement) { // it may not yet be set
    os << endl;

    ++gIndenter;
    os <<
      fDoubleTremoloFirstElement;
    --gIndenter;
  }
  else {
    os <<
      " [NONE]" <<
      endl;
  }

  os <<
    "Second element:";
  if (fDoubleTremoloSecondElement) { // it may not yet be set
    os << endl;

    ++gIndenter;
    os <<
      fDoubleTremoloSecondElement;
    --gIndenter;
  }
  else {
    os <<
      " [NONE]" <<
      endl;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrDoubleTremolo& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
