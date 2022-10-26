/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrTuplets.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  int                     inputLineNumber,
  const string&           tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  msrTupletFactor         tupletFactor,
  const Rational&         memberNotesSoundingWholeNotes,
  const Rational&         memberNotesDisplayWholeNotes)
{
  msrTuplet* o =
    new msrTuplet (
      inputLineNumber,
      tupletMeasureNumber,
      tupletNumber,
      tupletBracketKind,
      tupletLineShapeKind,
      tupletShowNumberKind,
      tupletShowTypeKind,
      tupletFactor,
      memberNotesSoundingWholeNotes,
      memberNotesDisplayWholeNotes);
  assert (o != nullptr);
  return o;
}

msrTuplet::msrTuplet (
  int                     inputLineNumber,
  const string&           tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  msrTupletFactor         tupletFactor,
  const Rational&         memberNotesSoundingWholeNotes,
  const Rational&         memberNotesDisplayWholeNotes)
    : msrTupletElement (inputLineNumber)
{
  fTupletKind = msrTupletInKind::k_NoTupletIn;

  fTupletNumber = tupletNumber;

  fTupletBracketKind    = tupletBracketKind;
  fTupletLineShapeKind  = tupletLineShapeKind;
  fTupletShowNumberKind = tupletShowNumberKind;
  fTupletShowTypeKind   = tupletShowTypeKind;

  fTupletFactor = tupletFactor;

  fMemberNotesSoundingWholeNotes = memberNotesSoundingWholeNotes;
  fMemberNotesDisplayWholeNotes  = memberNotesDisplayWholeNotes;

  doSetMeasureElementSoundingWholeNotes (
    Rational (0, 1),
    "msrTuplet::msrTuplet()");

  fTupletDisplayWholeNotes          = Rational (0, 1);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Creating tuplet:" <<
      endl;

    ++gIndenter;

    this->print (gLogStream);

    --gIndenter;
  }
#endif
}

msrTuplet::~msrTuplet ()
{}

S_msrTuplet msrTuplet::createTupletNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Creating a newborn clone of tuplet " <<
      asString () <<
      endl;
  }
#endif

  S_msrTuplet
    newbornClone =
      msrTuplet::create (
        fInputLineNumber,
        fMeasureElementMeasureNumber,
        fTupletNumber,
        fTupletBracketKind,
        fTupletLineShapeKind,
        fTupletShowNumberKind,
        fTupletShowTypeKind,
        fTupletFactor,
        fMemberNotesSoundingWholeNotes,
        fMemberNotesDisplayWholeNotes);

/* JMI ???
  newbornClone->fMeasureElementSoundingWholeNotes =
    fMeasureElementSoundingWholeNotes;

  newbornClone->fTupletDisplayWholeNotes =
    fTupletDisplayWholeNotes;

  newbornClone->fMeasureElementMeasureNumber =
    fMeasureElementMeasureNumber;

  newbornClone->fMeasureElementPositionInMeasure =
    fMeasureElementPositionInMeasure;
*/

  return newbornClone;
}

void msrTuplet::setTupletPositionInMeasure (
  const S_msrMeasure measure,
  const Rational&    positionInMeasure,
  const string&      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting tuplet's position in measure of " << asString () <<
      " to " <<
      positionInMeasure <<
      " (was " <<
      fMeasureElementPositionInMeasure <<
      ") in measure " <<
      measure->asShortString () <<
      " (fMeasureElementMeasureNumber: " <<
      fMeasureElementMeasureNumber <<
      "), context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    positionInMeasure != msrMoment::K_NO_POSITION,
    "positionInMeasure == msrMoment::K_NO_POSITION");

  // set time signature's position in measure
  fMeasureElementPositionInMeasure = positionInMeasure;
}

S_msrMeasure msrTuplet::fetchTupletUpLinkToMeasure () const
{
  S_msrMeasure result;

  switch (fTupletKind) {
    case msrTupletInKind::k_NoTupletIn:
      break;

    case msrTupletInKind::kTupletInMeasure:
      result = fTupletDirectUpLinkToMeasure;
      break;

    case msrTupletInKind::kTupletInTuplet:
      if (fTupletDirectUpLinkToTuplet) {
        result =
          fTupletDirectUpLinkToTuplet->
            fetchTupletUpLinkToMeasure ();
      }
      break;
  } // switch

  return result;
}

// tuplet upLink
S_msrTuplet msrTuplet::fetchTupletUpLinkToTuplet () const
{
  S_msrTuplet result;

  switch (fTupletKind) {
    case msrTupletInKind::k_NoTupletIn:
      break;

    case msrTupletInKind::kTupletInMeasure:
      break;

    case msrTupletInKind::kTupletInTuplet:
      result = fTupletDirectUpLinkToTuplet;
      break;
  } // switch

  return result;
}

string msrTuplet::tupletTypeKindAsString (
  msrTupletTypeKind tupletTypeKind)
{
  string result;

  switch (tupletTypeKind) {
    case msrTuplet::kTupletTypeNone:
      result = "kTupletTypeNone";
      break;
    case msrTuplet::kTupletTypeStart:
      result = "kTupletTypeStart";
      break;
    case msrTuplet::kTupletTypeContinue:
      result = "kTupletTypeContinue";
      break;
    case msrTuplet::kTupletTypeStop:
      result = "kTupletTypeStop";
      break;
    case msrTuplet::kTupletTypeStartAndStopInARow:
      result = "kTupletTypeStartAndStopInARow";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletBracketKindAsString (
  msrTupletBracketKind tupletBracketKind)
{
  string result;

  switch (tupletBracketKind) {
    case msrTuplet::kTupletBracketYes:
      result = "kTupletBracketYes";
      break;
    case msrTuplet::kTupletBracketNo:
      result = "kTupletBracketNo";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletLineShapeKindAsString (
  msrTupletLineShapeKind tupletLineShapeKind)
{
  string result;

  switch (tupletLineShapeKind) {
    case msrTuplet::kTupletLineShapeStraight:
      result = "kTupletLineShapeStraight";
      break;
    case msrTuplet::kTupletLineShapeCurved:
      result = "kTupletLineShapeCurved";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletShowNumberKindAsString (
  msrTupletShowNumberKind tupletShowNumberKind)
{
  string result;

  switch (tupletShowNumberKind) {
    case msrTuplet::kTupletShowNumberActual:
      result = "kTupletShowNumberActual";
      break;
    case msrTuplet::kTupletShowNumberBoth:
      result = "kTupletShowNumberBoth";
      break;
    case msrTuplet::kTupletShowNumberNone:
      result = "kTupletShowNumberNone";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletShowTypeKindAsString (
  msrTupletShowTypeKind tupletShowTypeKind)
{
  string result;

  switch (tupletShowTypeKind) {
    case msrTuplet::kTupletShowTypeActual:
      result = "kTupletShowTypeActual";
      break;
    case msrTuplet::kTupletShowTypeBoth:
      result = "kTupletShowTypeBoth";
      break;
    case msrTuplet::kTupletShowTypeNone:
      result = "kTupletShowTypeNone";
      break;
  } // switch

  return result;
}

void msrTuplet::appendNoteToTuplet (
  S_msrNote  note,
  S_msrVoice voice)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending note " <<
      note->asShortString () <<
      // the information is missing to display it the normal way
      " to tuplet " <<
      asString () <<
      endl;
  }
#endif

  ++gIndenter;

  // append note to elements list
  fTupletElementsList.push_back (note);

  // set note's position in tuplet
  note->setPositionInTuplet (
    fTupletElementsList.size ());

  // register note's tuplet upLink
  note->
    setNoteDirectUpLinkToTuplet (this);

  // register note's measure upLink // JMI ???
  note->
    setNoteDirectUpLinkToMeasure (fTupletDirectUpLinkToMeasure);

  // account for note duration in tuplet duration
  fMeasureElementSoundingWholeNotes +=
    note->getMeasureElementSoundingWholeNotes ();

  fTupletDisplayWholeNotes += // JMI
    note->getNoteDisplayWholeNotes ();

  // register note's tuplet factor
  note->
    setNoteTupletFactor (fTupletFactor);

  // is this note the shortest one in this voice?
  voice->
    registerShortestNoteInVoiceIfRelevant (note);

  // fetch voice last measure
  S_msrMeasure
    voiceLastMeasure =
      voice->fetchVoiceLastMeasure (
        inputLineNumber);

  // account for the duration of note in voice last measure
  voiceLastMeasure->
    accountForTupletMemberNoteDurationInMeasure (
      note);

  --gIndenter;
}

void msrTuplet::appendChordToTuplet (S_msrChord chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending chord " <<
      chord->asString () <<
      " to tuplet " <<
      asString () <<
      endl;
  }
#endif

  // set the chord kind
  chord->setChordKind (msrChordInKind::kChordInTuplet);

  // append chord to elements list
  fTupletElementsList.push_back (chord);

  // set chord's position in tuplet
  chord->setPositionInTuplet (
    fTupletElementsList.size ());

  // set chord's position in tuplet
  chord->setPositionInTuplet (
    fTupletElementsList.size ());

  // DO NOT account for the chord duration,
  // since its first note has been accounted for already
  /* JMI
  fMeasureElementSoundingWholeNotes +=
    chord->getMeasureElementSoundingWholeNotes ();
*/

  fTupletDisplayWholeNotes += // JMI
    chord->getChordDisplayWholeNotes ();

/* too early JMI
  // populate chord's measure number
  chord->setChordMeasureNumber (
    fMeasureElementMeasureNumber);
*/
}

void msrTuplet::appendTupletToTuplet (S_msrTuplet tuplet)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending tuplet " <<
      tuplet->asString () <<
      " to tuplet " <<
      asString () <<
      endl;
  }
#endif

/* JMI
  // unapply containing tuplet factor,
  // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
  tuplet->
    unapplySoundingFactorToTupletMembers (
      this->getTupletNormalNotes (),
      this->getTupletNormalNotes ();
  */

  // set the tuplet kind
  tuplet->setTupletKind (msrTupletInKind::kTupletInTuplet);

  // append tuplet to elements list
  fTupletElementsList.push_back (tuplet);

  // set tuplet's position in tuplet
  tuplet->setPositionInTuplet (
    fTupletElementsList.size ());

  // account for tuplet duration
  fMeasureElementSoundingWholeNotes +=
    tuplet->getMeasureElementSoundingWholeNotes ();

  fTupletDisplayWholeNotes += // JMI
    tuplet->getTupletDisplayWholeNotes ();
}

void msrTuplet::appendTupletToTupletClone (S_msrTuplet tuplet)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending tuplet " <<
      tuplet->asString () <<
      " to tuplet clone " <<
      asString () <<
      endl;
  }
#endif

  // dont' unapply containing tuplet factor,
  // this has been done when building the MSR from MusicXML

  // append tuplet to elements list
  fTupletElementsList.push_back (tuplet);

  // set tuplet's position in tuplet
  tuplet->setPositionInTuplet (
    fTupletElementsList.size ());

  // account for tuplet duration
  fMeasureElementSoundingWholeNotes +=
    tuplet->getMeasureElementSoundingWholeNotes ();

  fTupletDisplayWholeNotes +=
    tuplet->getTupletDisplayWholeNotes ();
}

S_msrNote msrTuplet::fetchTupletFirstNonGraceNote () const
{
  S_msrNote result;

  if (fTupletElementsList.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElementsList.front ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*firstTupletElement))
      ) {
      // first element is a note, we have it
      result = note;
    }

    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*firstTupletElement))
      ) {
      // first element is another tuplet, recurse
      result = tuplet->fetchTupletFirstNonGraceNote ();
    }
  }

  else {
    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      "cannot access the first note of an empty tuplet");
  }

  return result;
}

S_msrNote msrTuplet::removeFirstNoteFromTuplet (
  int inputLineNumber)
{
  S_msrNote result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Removing first note from tuplet " <<
      asString () <<
      endl;
  }
#endif

  if (fTupletElementsList.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElementsList.front ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*firstTupletElement))
      ) {
      fTupletElementsList.pop_front ();
      result = note;
    }

    else {
      if (true) { // JMI ???
        this->print (gLogStream);
      }

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "removeFirstNoteFromTuplet () expects a note as the first tuplet element");
    }

/* JMI
    for (
      list<S_msrElement>::iterator i=fTupletElementsList.begin ();
      i!=fTupletElementsList.end ();
      ++i) {
      if ((*i) == note) {
        // found note, erase it
        i = fTupletElementsList.erase (i);

        // account for note duration
        fMeasureElementSoundingWholeNotes -=
          note->getMeasureElementSoundingWholeNotes ();
        fMeasureElementSoundingWholeNotes.rationalis ();

        fTupletDisplayWholeNotes -= // JMI
          note->getNoteDisplayWholeNotes ();

        // don't update measure number nor position in measure: // JMI
        // they have not been set yet

        // return from function
        return;
      }
    } // for

    stringstream s;

    s <<
      "cannot remove note " <<
      note <<
      " from tuplet " << asString () <<
      " in voice \"" <<
      fTupletDirectUpLinkToMeasure->
        fetchMeasureUpLinkToVoice ()->
          getVoiceName () <<
      "\"," <<
      " since this note has not been found in fTupletElementsList";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  */
  }

  else {
    stringstream s;

    s <<
      "cannot remove the first note of an empty tuplet " <<
      " in voice \"" <<
      fTupletDirectUpLinkToMeasure->
        fetchMeasureUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

S_msrNote msrTuplet::removeLastNoteFromTuplet (
  int inputLineNumber)
{
  S_msrNote result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Removing last note from tuplet " <<
      asString () <<
      endl;
  }
#endif

  if (fTupletElementsList.size ()) {
    S_msrElement
      lastTupletElement =
        fTupletElementsList.back ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*lastTupletElement))
    ) {
      // remove note from tuplet elements list
      fTupletElementsList.pop_back ();

/*
      // decrement the tuplet sounding whole notes accordingly ??? JMI BAD???
      fMeasureElementSoundingWholeNotes +=
        note->getMeasureElementSoundingWholeNotes ();
*/

      result = note;
    }

    else {
      if (true) { // JMI
        this->print (gLogStream);
      }

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "removeLastNoteFromTuplet () expects a note as the last tuplet element");
    }
  }

  else {
    stringstream s;

    s <<
      "cannot remove the last note of an empty tuplet " <<
      " in voice \"" <<
      fTupletDirectUpLinkToMeasure->
        fetchMeasureUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "This last note from tuplet " <<
      asString () <<
      " turns out to be " <<
      result->asShortString () <<
      endl;
  }
#endif

  return result;
}

Rational msrTuplet::setTupletMembersPositionsInMeasure (
  S_msrMeasure    measure,
  const Rational& positionInMeasure)
  // returns the position in measure after the tuplet
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting tuplet members positions in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      "'" <<
      endl;
  }
#endif

  string context = // JMI v0.9.66
    "setTupletMembersPositionsInMeasure()";

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    positionInMeasure != msrMoment::K_NO_POSITION,
    "positionInMeasure == msrMoment::K_NO_POSITION");

  // set tuplet's position in measure
  fMeasureElementPositionInMeasure = positionInMeasure;

if (false) { // JMI
  // compute tuplet's position in voice
  Rational
     positionFromBeginningOfVoice =
      fTupletDirectUpLinkToMeasure->getMeasurePositionFromBeginningOfVoice ()
        +
      positionInMeasure;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    positionInMeasure != msrMoment::K_NO_POSITION,
    "positionInMeasure == msrMoment::K_NO_POSITION");

  // set tuplet's position in measure
  fMeasureElementPositionInMeasure = positionInMeasure;

  // update current position in voice
  S_msrVoice
    voice =
      fTupletDirectUpLinkToMeasure->
        fetchMeasureUpLinkToVoice ();

  voice->
    incrementCurrentPositionFromBeginningOfVoice (
      fMeasureElementSoundingWholeNotes);
}

  // current position
  Rational currentPosition = positionInMeasure;

  // compute position in measure for the tuplets elements
  for (
    list<S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    ++i
  ) {
    // set tuplet element position in measure

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
    ) {
      // note
      note->
        setNoteDirectUpLinkToMeasure (
          measure);

      note->
        setNotePositionInMeasure (
          measure,
          currentPosition,
          "msrTuplet::setTupletMembersPositionsInMeasure()");

      currentPosition +=
        note->
          getMeasureElementSoundingWholeNotes ();
    }

    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
    ) {
      // chord
      chord->
        setChordMembersPositionInMeasure (
          measure,
          currentPosition);

      currentPosition +=
        chord->
          getMeasureElementSoundingWholeNotes ();
    }

    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
    ) {
      // nested tuplet
//       currentPosition =
//         tuplet->
//           setTupletPositionInMeasure ( // a function JMI ??? v0.9.66
//             measure,
//             currentPosition);

      currentPosition +=
        tuplet->
          getMeasureElementSoundingWholeNotes ();
    }

    else {
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "tuplet member should be a note, a chord or another tuplet");
    }
  } // for

  return currentPosition;
}

void msrTuplet::unapplySoundingFactorToTupletMembers (
  const msrTupletFactor& containingTupletFactor)
  /*
  int containingTupletActualNotes,
  int containingTupletNormalNotes)
  */
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "unapplySoundingFactorToTupletMembers ()" <<
      endl;

    ++gIndenter;

    gLogStream <<
      "% fTupletFactor = " << fTupletFactor.asString () <<
      endl <<
      "% containingTupletFactor = " << containingTupletFactor.asString () <<
      endl;

    --gIndenter;
  }
#endif

  fTupletFactor.setTupletActualNotes (
    fTupletFactor.getTupletActualNotes ()
      /
    containingTupletFactor.getTupletActualNotes ());
  fTupletFactor.setTupletNormalNotes (
    fTupletFactor.getTupletNormalNotes ()
      /
    containingTupletFactor.getTupletNormalNotes ());

/* JMI
  fTupletFactor.getTupletNormalNotes () /=
    containingTupletFactor.getTupletNormalNotes ();
  fTupletFactor.fTupletNormalNotes /=
    containingTupletFactor.fTupletNormalNotes;
  */
}

/* JMI ???
void msrTuplet::finalizeTuplet (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Finalizing tuplet " <<
      asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/ * JMI v0.9.66
  // we can now set the position in measure for all the tuplet members
  setTupletMembersPositionsInMeasure (
    fMeasureElementPositionInMeasure);
  * /
}
*/

void msrTuplet::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTuplet::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTuplet::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTuplet::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTuplet::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTuplet::browseData (basevisitor* v)
{
  for (
    list<S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    ++i
  ) {
    // browse tuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrTuplet::asString () const
{
  stringstream s;

  s <<
    "[Tuplet" <<
    ", tupletKind: " << fTupletKind <<
    fTupletFactor.asString () <<
    ' ' << fMeasureElementSoundingWholeNotes << " tupletSoundingWholeNotes" <<
    ", measure ' " <<
    ", line " << fInputLineNumber <<
    fMeasureElementMeasureNumber <<
    "':";

  if (fMeasureElementPositionInMeasure.getNumerator () < 0) {
    s << "?";
  }
  else {
    s << fMeasureElementPositionInMeasure;
  }

  s << "[[";

  if (fTupletElementsList.size ()) {
    list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
        ) {
        s <<
          note->asShortString ();
      }

      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
        ) {
        s <<
          chord->asString ();
      }

      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
        ) {
        s <<
          tuplet->asString ();
      }

      else {
        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          "tuplet member should be a note, a chord or another tuplet");
      }

      if (++i == iEnd) break;
      s << ' ';

    } // for
  }

  s << "]]" << ']';

  return s.str ();
}

void msrTuplet::print (ostream& os) const
{
  os <<
    "[Tuplet" <<
    ", tupletKind: " << fTupletKind <<
    fTupletNumber <<
    ", " <<
    fTupletFactor.asString () <<
    ", " <<
    mfSingularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", whole notes: " <<
    fMeasureElementSoundingWholeNotes << " sounding, " <<
    fTupletDisplayWholeNotes << " displayed" <<
    ", meas " <<
    fMeasureElementMeasureNumber <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 30;

  os << left <<
    setw (fieldWidth) <<
    "fTupletBracketKind" << " : " <<
    tupletBracketKindAsString (
      fTupletBracketKind) <<
    endl <<
    setw (fieldWidth) <<
    "fTupletLineShapeKind" << " : " <<
    tupletLineShapeKindAsString (
      fTupletLineShapeKind) <<
    endl <<
    setw (fieldWidth) <<
    "fTupletShowNumberKind" << " : " <<
    tupletShowNumberKindAsString (
      fTupletShowNumberKind) <<
    endl <<
    setw (fieldWidth) <<
    "fTupletShowTypeKind" << " : " <<
    tupletShowTypeKindAsString (
      fTupletShowTypeKind) <<
    endl <<

    setw (fieldWidth) <<
    "fMemberNotesSoundingWholeNotes" << " : " <<
    fMemberNotesSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "fMemberNotesDisplayWholeNotes" << " : " <<
    fMemberNotesDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "fTupletSoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "fTupletDisplayWholeNotes" << " : " <<
    fTupletDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "fTupletMeasureNumber" << " : " <<
    fMeasureElementMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "fPositionInMeasure" << " : " <<
    fMeasureElementPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "fPositionFromBeginningOfVoice" << " : " <<
    fMeasureElementPositionFromBeginningOfVoice <<
    endl << endl;

/* JMI ???
  os << left <<
    setw (fieldWidth) <<
    "(position in measure" << " : ";
  if (fMeasureElementPositionInMeasure.getNumerator () < 0) {
    os << "???)";
  }
  else {
    os << fMeasureElementPositionInMeasure << ")";
  }
  os << endl;
    */

  --gIndenter;

  if (fTupletElementsList.size ()) {
    ++gIndenter;

    list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;

  // JMI  os << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fTupletDirectUpLinkToMeasure" << " : ";
  if (fTupletDirectUpLinkToMeasure) {
    os <<
      fTupletDirectUpLinkToMeasure->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fTupletDirectUpLinkToTuplet" << " : ";
  if (fTupletDirectUpLinkToTuplet) {
    os <<
      fTupletDirectUpLinkToTuplet->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "fPositionInTuplet" << " : " <<
    fPositionInTuplet <<
    endl;

  os << ']' << endl;
}

void msrTuplet::printShort (ostream& os)
{
  os <<
    "[Tuplet" <<
    ", line " << fInputLineNumber <<
    ", fTupletNumber" << fTupletNumber <<
    ", " <<
    fTupletFactor.asString () <<
    ", " <<
    mfSingularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", whole notes: " <<
    fMeasureElementSoundingWholeNotes << " sounding, " <<
    fTupletDisplayWholeNotes << " displayed" <<
    ", meas " <<
    fMeasureElementMeasureNumber <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 44;

/*
  os << left <<
    setw (fieldWidth) <<
    "fTupletBracketKind" << " : " <<
    tupletBracketKindAsString (
      fTupletBracketKind) <<
    endl <<
    setw (fieldWidth) <<
    "fTupletLineShapeKind" << " : " <<
    tupletLineShapeKindAsString (
      fTupletLineShapeKind) <<
    endl <<
    setw (fieldWidth) <<
    "fTupletShowNumberKind" << " : " <<
    tupletShowNumberKindAsString (
      fTupletShowNumberKind) <<
    endl <<
    setw (fieldWidth) <<
    "fTupletShowTypeKind" << " : " <<
    tupletShowTypeKindAsString (
      fTupletShowTypeKind) <<
    endl <<
*/

  os << left <<
    setw (fieldWidth) <<
    "fMemberNotesSoundingWholeNotes" << " : " <<
    fMemberNotesSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "fMemberNotesDisplayWholeNotes" << " : " <<
    fMemberNotesDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "fTupletSoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "fTupletDisplayWholeNotes" << " : " <<
    fTupletDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "fTupletMeasureNumber" << " : " <<
    fMeasureElementMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "fMeasureElementPositionInMeasure" << " : " <<
    fMeasureElementPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "fMeasureElementPositionFromBeginningOfVoice" << " : " <<
    fMeasureElementPositionFromBeginningOfVoice <<
    endl << endl;

/* JMI ???
  os << left <<
    setw (fieldWidth) <<
    "(position in measure" << " : ";
  if (fMeasureElementPositionInMeasure.getNumerator () < 0) {
    os << "???)";
  }
  else {
    os << fMeasureElementPositionInMeasure << ")";
  }
  os << endl;
    */

  --gIndenter;

  if (fTupletElementsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
        "fTupletElements" << " :" <<
        endl;

    ++gIndenter;

    list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
//       os <<
//         (*i)->asShortString () <<
//         endl;
      (*i)->printShort (os); // JMI
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;

  // JMI  os << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fTupletDirectUpLinkToMeasure" << " : ";
  if (fTupletDirectUpLinkToMeasure) {
    os <<
      fTupletDirectUpLinkToMeasure->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fTupletDirectUpLinkToTuplet" << " : ";
  if (fTupletDirectUpLinkToTuplet) {
    os <<
      fTupletDirectUpLinkToTuplet->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrTuplet& elt)
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
