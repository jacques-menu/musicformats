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

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"
#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  int                     inputLineNumber,
  const S_msrMeasure&     upLinkToMeasure,
  const std::string&      tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  const msrTupletFactor&  tupletFactor,
  const Rational&         memberNotesSoundingWholeNotes,
  const Rational&         memberNotesDisplayWholeNotes)
{
  msrTuplet* o =
    new msrTuplet (
      inputLineNumber,
      upLinkToMeasure,
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

S_msrTuplet msrTuplet::create (
  int                     inputLineNumber,
  const std::string&      tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  const msrTupletFactor&  tupletFactor,
  const Rational&         memberNotesSoundingWholeNotes,
  const Rational&         memberNotesDisplayWholeNotes)
{
  return
    msrTuplet::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      tupletMeasureNumber,
      tupletNumber,
      tupletBracketKind,
      tupletLineShapeKind,
      tupletShowNumberKind,
      tupletShowTypeKind,
      tupletFactor,
      memberNotesSoundingWholeNotes,
      memberNotesDisplayWholeNotes);
}

msrTuplet::msrTuplet (
  int                     inputLineNumber,
  const S_msrMeasure&     upLinkToMeasure,
  const std::string&      tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  const msrTupletFactor&  tupletFactor,
  const Rational&         memberNotesSoundingWholeNotes,
  const Rational&         memberNotesDisplayWholeNotes)
    : msrTupletElement (
        inputLineNumber)
{
  fTupletUpLinkToMeasure = upLinkToMeasure;

  fTupletKind = msrTupletInKind::kTupletIn_NO_;

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
      std::endl;

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
      std::endl;
  }
#endif

  S_msrTuplet
    newbornClone =
      msrTuplet::create (
        fInputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        fTupletUpLinkToMeasure->getMeasureNumber (),
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

  newbornClone->fTupletUpLinkToMeasure->getMeasureNumber () =
    fTupletUpLinkToMeasure->getMeasureNumber ();

  newbornClone->fMeasureElementMeasurePosition =
    fMeasureElementMeasurePosition;
*/

  return newbornClone;
}

// S_msrMeasure msrTuplet::fetchMeasureElementUpLinkToMeasure () const
// {
//   S_msrMeasure result;
//
//   switch (fTupletKind) {
//     case msrTupletInKind::kTupletIn_NO_:
//       break;
//
//     case msrTupletInKind::kTupletInMeasure:
//       result = fTupletUpLinkToMeasure;
//       break;
//
//     case msrTupletInKind::kTupletInTuplet:
//       if (fTupletDirectUpLinkToTuplet) {
//         result =
//           fTupletDirectUpLinkToTuplet->
//             fetchMeasureElementUpLinkToMeasure ();
//       }
//       break;
//   } // switch
//
//   return result;
// }

// S_msrTuplet msrTuplet::fetchTupletUpLinkToTuplet () const
// {
//   S_msrTuplet result;
//
//   switch (fTupletKind) {
//     case msrTupletInKind::kTupletIn_NO_:
//       break;
//
//     case msrTupletInKind::kTupletInMeasure:
//       break;
//
//     case msrTupletInKind::kTupletInTuplet:
//       result = fTupletDirectUpLinkToTuplet;
//       break;
//   } // switch
//
//   return result;
// }

void msrTuplet::setMeasureElementUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  setTupletUpLinkToMeasure (measure);
}

void msrTuplet::getMeasureElementUpLinkToMeasure (
  S_msrMeasure& upLinkToMeasure) const
{
  upLinkToMeasure = getTupletUpLinkToMeasure ();
}

void msrTuplet::setTupletUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of tuplet " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fTupletUpLinkToMeasure = measure;
}

S_msrMeasure msrTuplet::getTupletUpLinkToMeasure () const
{
  return fTupletUpLinkToMeasure;
}

//______________________________________________________________________________
void msrTuplet::appendNoteToTuplet (
  const S_msrNote&  note,
  const S_msrVoice& voice)
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
      std::endl;
  }
#endif

  ++gIndenter;

  // append note to elements list
  fTupletElementsList.push_back (note);

  // set note's position in tuplet
  note->setPositionInTuplet (
    fTupletElementsList.size ());

  // register note's uplink to tuplet
  note->
    setNoteDirectUpLinkToTuplet (this);

  // register note's uplink to measure // JMI ???
  note->
    setMeasureElementUpLinkToMeasure (
      fTupletUpLinkToMeasure);

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

void msrTuplet::appendChordToTuplet (const S_msrChord& chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending chord " <<
      chord->asString () <<
      " to tuplet " <<
      asString () <<
      std::endl;
  }
#endif

  // set the chord kind
  chord->setChordKind (msrChordInKind::kChordInTuplet);

  // append chord to elements list
  fTupletElementsList.push_back (chord);

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
    fTupletUpLinkToMeasure->getMeasureNumber ());
*/
}

void msrTuplet::appendTupletToTuplet (const S_msrTuplet& tuplet)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending tuplet " <<
      tuplet->asString () <<
      " to tuplet " <<
      asString () <<
      std::endl;
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

void msrTuplet::appendTupletToTupletClone (const S_msrTuplet& tuplet)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending tuplet " <<
      tuplet->asString () <<
      " to tuplet clone " <<
      asString () <<
      std::endl;
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
      std::endl;
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
      std::list<S_msrElement>::iterator i=fTupletElementsList.begin ();
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

        // don't update measure number nor measure position: // JMI
        // they have not been set yet

        // return from function
        return;
      }
    } // for

    std::stringstream s;

    s <<
      "cannot remove note " <<
      note <<
      " from tuplet " << asString () <<
      " in voice \"" <<
      fTupletUpLinkToMeasure->
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
    std::stringstream s;

    s <<
      "cannot remove the first note of an empty tuplet " <<
      " in voice \"" <<
      fTupletUpLinkToMeasure->
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
      std::endl;
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
    std::stringstream s;

    s <<
      "cannot remove the last note of an empty tuplet " <<
      " in voice \"" <<
      fTupletUpLinkToMeasure->
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
      std::endl;
  }
#endif

  return result;
}

Rational msrTuplet::setTupletMembersMeasurePositions (
  const S_msrMeasure&   measure,
  const Rational& measurePosition)
  // returns the measure position after the tuplet
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting tuplet members positions in measure of " << asString () <<
      " to '" <<
      measurePosition <<
      "'" <<
      std::endl;
  }
#endif

  std::string context = // JMI v0.9.66
    "setTupletMembersMeasurePositions()";

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set tuplet's measure position
  fMeasureElementMeasurePosition = measurePosition;

if (false) { // JMI
  // compute tuplet's voice position
  Rational
     voicePosition =
      fTupletUpLinkToMeasure->getMeasureVoicePosition ()
        +
      measurePosition;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set tuplet's measure position
  fMeasureElementMeasurePosition = measurePosition;

  // update current voice position
  S_msrVoice
    voice =
      fTupletUpLinkToMeasure->
        fetchMeasureUpLinkToVoice ();

  voice->
    incrementCurrentVoicePosition (
      fMeasureElementSoundingWholeNotes);
}

  // current position
  Rational currentPosition = measurePosition;

  // compute measure position for the tuplets elements
  for (
    std::list<S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    ++i
  ) {
    // set tuplet element measure position

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
    ) {
      // note
      note->
        setMeasureElementUpLinkToMeasure (
          measure);

      note->
        setMeasureElementMeasurePosition (
          measure,
          currentPosition,
          "msrTuplet::setTupletMembersMeasurePositions()");

      currentPosition +=
        note->
          getMeasureElementSoundingWholeNotes ();
    }

    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
    ) {
      // chord
      chord->
        setChordMembersMeasurePosition (
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
//           setTupletMeasurePosition ( // a function JMI ??? v0.9.66
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
      std::endl;

    ++gIndenter;

    gLogStream <<
      "% fTupletFactor: " << fTupletFactor.asString () <<
      std::endl <<
      "% containingTupletFactor: " << containingTupletFactor.asString () <<
      std::endl;

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
      std::endl;
  }
#endif

/ * JMI v0.9.66
  // we can now set the measure position for all the tuplet members
  setTupletMembersMeasurePositions (
    fMeasureElementMeasurePosition);
  * /
}
*/

void msrTuplet::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTuplet::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTuplet::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTuplet::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTuplet::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrTuplet::browseData (basevisitor* v)
{
  for (
    std::list<S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    ++i
  ) {
    // browse tuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string msrTuplet::asString () const
{
  std::stringstream s;

  s <<
    "[Tuplet" <<
    ", tupletKind: " << fTupletKind <<
    fTupletFactor.asString () <<
    ' ' << fMeasureElementSoundingWholeNotes << " tupletSoundingWholeNotes" <<
    ", measure ' " <<
    ", line " << fInputLineNumber <<
    fTupletUpLinkToMeasure->getMeasureNumber () <<
    "':";

  if (fMeasureElementMeasurePosition.getNumerator () < 0) {
    s << "?";
  }
  else {
    s << fMeasureElementMeasurePosition;
  }

  s << "[[";

  if (fTupletElementsList.size ()) {
    std::list<S_msrTupletElement>::const_iterator
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

void msrTuplet::print (std::ostream& os) const
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
    fTupletUpLinkToMeasure->getMeasureNumber () <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 30;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletBracketKind" << ": " <<
    msrTupletBracketKindAsString (
      fTupletBracketKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletLineShapeKind" << ": " <<
    msrTupletLineShapeKindAsString (
      fTupletLineShapeKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletShowNumberKind" << ": " <<
    msrTupletShowNumberKindAsString (
      fTupletShowNumberKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletShowTypeKind" << ": " <<
    msrTupletShowTypeKindAsString (
      fTupletShowTypeKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMemberNotesSoundingWholeNotes" << ": " <<
    fMemberNotesSoundingWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMemberNotesDisplayWholeNotes" << ": " <<
    fMemberNotesDisplayWholeNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletSoundingWholeNotes" << ": " <<
    fMeasureElementSoundingWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletDisplayWholeNotes" << ": " <<
    fTupletDisplayWholeNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletMeasureNumber" << ": " <<
    fTupletUpLinkToMeasure->getMeasureNumber () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasurePosition" << ": " <<
    fMeasureElementMeasurePosition <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoicePosition" << ": " <<
    fMeasureElementVoicePosition <<
    std::endl << std::endl;

/* JMI ???
  os << std::left <<
    std::setw (fieldWidth) <<
    "(measure position" << ": ";
  if (fMeasureElementMeasurePosition.getNumerator () < 0) {
    os << "???)";
  }
  else {
    os << fMeasureElementMeasurePosition << ")";
  }
  os << std::endl;
    */

  --gIndenter;

  if (fTupletElementsList.size ()) {
    ++gIndenter;

    std::list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
//       os << (*i); // JMI v0.9.66
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;

  // JMI  os << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletUpLinkToMeasure" << ": ";
  if (fTupletUpLinkToMeasure) {
    os <<
      fTupletUpLinkToMeasure->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletDirectUpLinkToTuplet" << ": ";
  if (fTupletDirectUpLinkToTuplet) {
    os <<
      fTupletDirectUpLinkToTuplet->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fPositionInTuplet" << ": " <<
    fPositionInTuplet <<
    std::endl;

  os << ']' << std::endl;
}

void msrTuplet::printShort (std::ostream& os)
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
    fTupletUpLinkToMeasure->getMeasureNumber () <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 44;

/*
  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletBracketKind" << ": " <<
    msrTupletBracketKindAsString (
      fTupletBracketKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletLineShapeKind" << ": " <<
    msrTupletLineShapeKindAsString (
      fTupletLineShapeKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletShowNumberKind" << ": " <<
    msrTupletShowNumberKindAsString (
      fTupletShowNumberKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletShowTypeKind" << ": " <<
    msrTupletShowTypeKindAsString (
      fTupletShowTypeKind) <<
    std::endl <<
*/

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMemberNotesSoundingWholeNotes" << ": " <<
    fMemberNotesSoundingWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMemberNotesDisplayWholeNotes" << ": " <<
    fMemberNotesDisplayWholeNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletSoundingWholeNotes" << ": " <<
    fMeasureElementSoundingWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletDisplayWholeNotes" << ": " <<
    fTupletDisplayWholeNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletMeasureNumber" << ": " <<
    fTupletUpLinkToMeasure->getMeasureNumber () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << ": " <<
    fMeasureElementMeasurePosition <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << ": " <<
//     fMeasureElementVoicePosition <<
    std::endl << std::endl;

/* JMI ???
  os << std::left <<
    std::setw (fieldWidth) <<
    "(measure position" << ": ";
  if (fMeasureElementMeasurePosition.getNumerator () < 0) {
    os << "???)";
  }
  else {
    os << fMeasureElementMeasurePosition << ")";
  }
  os << std::endl;
    */

  --gIndenter;

  if (fTupletElementsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
        "fTupletElements" << " :" <<
        std::endl;

    ++gIndenter;

    std::list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
//       os <<
//         (*i)->asShortString () <<
//         std::endl;
//       (*i)->printShort (os); // JMI v0.9.66
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;

  // JMI  os << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletUpLinkToMeasure" << ": ";
  if (fTupletUpLinkToMeasure) {
    os <<
      fTupletUpLinkToMeasure->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletDirectUpLinkToTuplet" << ": ";
  if (fTupletDirectUpLinkToTuplet) {
    os <<
      fTupletDirectUpLinkToTuplet->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTuplet& elt)
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
