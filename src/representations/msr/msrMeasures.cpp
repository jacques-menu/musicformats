/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfRational.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrBreaks.h"
#include "msrDoubleTremolos.h"
#include "msrHiddenMeasureAndBarLines.h"
#include "msrMeasures.h"
#include "msrRehearsalMarks.h"
#include "msrTempos.h"
#include "msrVoiceStaffChanges.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrWae.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
int msrMeasure::gGlobalMeasureDebugNumber = 0;

S_msrMeasure msrMeasure::create (
  int                 inputLineNumber,
  const std::string&  measureNumber,
  const S_msrSegment& measureUpLinkToSegment)
{
  msrMeasure* o =
    new msrMeasure (
      inputLineNumber,
      measureNumber,
      measureUpLinkToSegment);
  assert (o != nullptr);
  return o;
}

msrMeasure::msrMeasure (
  int                 inputLineNumber,
  const std::string&  measureNumber,
  const S_msrSegment& measureUpLinkToSegment)
    : msrSegmentElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureUpLinkToSegment != nullptr,
    "measureUpLinkToSegment is null");

  // set measure's segment upLink
  fMeasureUpLinkToSegment =
    measureUpLinkToSegment;

  // set measure numbers
  fMeasureNumber = measureNumber;
  fMeasureOrdinalNumberInVoice = -1;

  fMeasurePuristNumber = -1; // default irrealist value
  fNextMeasureNumber = "";

  // set debug number
  fMeasureDebugNumber = ++gGlobalMeasureDebugNumber;

  // do other initializations
  initializeMeasure ();
}

void msrMeasure::initializeMeasure ()
{
  S_msrVoice
    upLinkToVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Initializing measure '" <<
      fMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      upLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  // measure kind
  fMeasureKind = msrMeasureKind::kMeasureKindUnknown;

  // measure 'first in segment' kind
  fMeasureFirstInSegmentKind = kMeasureFirstInSegmentKindUnknown;

  // measure 'first in voice'
  fMeasureIsFirstInVoice = false; // default value

  // single-measure rest?
  fMeasureIsAFullBarRest = false;

  // fetch the staff
  S_msrStaff
    upLinkToStaff =
      upLinkToVoice->
        getVoiceUpLinkToStaff ();

  // set the full measure whole notes duration from time if relevant
  S_msrTimeSignature
    staffCurrentTimeSignature =
      upLinkToStaff->
        getStaffCurrentTime ();

  if (staffCurrentTimeSignature) {
    setFullMeasureWholeNotesDurationFromTime (
      staffCurrentTimeSignature);
  }

  // measure shortest note duration
//  fMeasureShortestNoteDuration = Rational (INT_MAX, 1);

  // measure shortest note tuplet factor
//  fMeasureShortestNoteTupletFactor = Rational (1, 1);

  // measure doesn't contain music yet
  fMeasureContainsMusic = false;

  // regular measure ends detection
  fMeasureEndRegularKind = kMeasureEndRegularKindUnknown;

  // repeat context
  fMeasureRepeatContextKind = msrMeasureRepeatContextKind::kMeasureRepeatContext_NO_;

  // measure whole notes duration
  // initialize measure whole notes
  setMeasureWholeNotesDuration (
    fInputLineNumber,
    Rational (0, 1)); // ready to receive the first note

  // voice position
  fMeasureVoicePosition =
    fMeasureUpLinkToSegment->
      getSegmentUpLinkToVoice ()->
        getCurrentVoicePosition ();

  fMeasureVoiceMoment =
    fMeasureUpLinkToSegment->
      getSegmentUpLinkToVoice ()->
        getCurrentVoiceMoment ();

  // measure finalization
  fMeasureHasBeenFinalized = false;
  fMeasureKindAndPuristNumberHaveBeenDetermined = false;
}

msrMeasure::~msrMeasure ()
{}

S_msrVoice msrMeasure::fetchMeasureUpLinkToVoice () const
{
  S_msrVoice result;

  if (fMeasureUpLinkToSegment) {
    result =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();
  }

  return result;
}

S_msrStaff msrMeasure::fetchMeasureUpLinkToStaff () const
{
  S_msrStaff result;

  if (fMeasureUpLinkToSegment) {
    result =
      fMeasureUpLinkToSegment->
        fetchSegmentUpLinkToStaff ();
  }

  return result;
}

S_msrPart msrMeasure::fetchMeasureUpLinkToPart () const
{
  S_msrPart result;

  if (fMeasureUpLinkToSegment) {
    result =
      fMeasureUpLinkToSegment->
        fetchSegmentUpLinkToPart ();
  }

  return result;
}

S_msrPartGroup msrMeasure::fetchMeasureUpLinkToPartGroup () const
{
  S_msrPartGroup result;

  if (fMeasureUpLinkToSegment) {
    result =
      fMeasureUpLinkToSegment->
        fetchSegmentUpLinkToPartGroup ();
  }

  return result;
}

S_msrScore msrMeasure::fetchMeasureUpLinkToScore () const
{
  S_msrScore result;

  if (fMeasureUpLinkToSegment) {
    result =
      fMeasureUpLinkToSegment->
        fetchSegmentUpLinkToScore ();
  }

  return result;
}

S_msrMeasure msrMeasure::createMeasureNewbornClone (
  const S_msrSegment& containingSegment)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");

  S_msrVoice
    containingSegmentUpLinkToVoice =
      containingSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating a newborn clone of measure " <<
      this->asShortString () <<
      fMeasureDebugNumber <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
      containingSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");

  // create newborn clone
  S_msrMeasure
    newbornClone =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureNumber,
        containingSegment);

  // set newbornClone's ordinal number
  newbornClone->
    setMeasureOrdinalNumberInVoice (
      containingSegmentUpLinkToVoice->
        incrementVoiceCurrentMeasureOrdinalNumber ());

  // next measure number
  newbornClone->fNextMeasureNumber =
    fNextMeasureNumber;

    /* JMI
  // measure ordinal number in voice
  newbornClone->fMeasureOrdinalNumberInVoice =
    fMeasureOrdinalNumberInVoice;

  // lengthes
  newbornClone->fFullMeasureWholeNotesDuration =
    fFullMeasureWholeNotesDuration;

  // measure purist number
  newbornClone->fMeasurePuristNumber = // JMI don't compute that again for clones ???
    fMeasurePuristNumber;

  // measure 'first in segment' kind
  newbornClone->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;
    */

/* JMI
  // single-measure rest?
  newbornClone->fMeasureIsAFullBarRest =
    fMeasureIsAFullBarRest;

  // regular measure ends detection // JMI TEMP
  newbornClone->fMeasureEndRegularKind =
    fMeasureEndRegularKind;
*/

  // chords handling

  // elements

  // upLinks

  return newbornClone;
}

S_msrMeasure msrMeasure::createMeasureDeepClone (
  const S_msrSegment& containingSegment)
{
  S_msrVoice
    containingSegmentUpLinkToVoice =
      containingSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating a deep clone of measure " <<
      this->asShortString () <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
      containingSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");

  // create deep clone
  S_msrMeasure
    deepClone =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureNumber,
        containingSegment);

  // measure numbers
  deepClone->
    setMeasureOrdinalNumberInVoice (
      containingSegmentUpLinkToVoice->
        incrementVoiceCurrentMeasureOrdinalNumber ());

  deepClone->fMeasurePuristNumber =
    fMeasurePuristNumber;

  deepClone->fMeasureDebugNumber =
    fMeasureDebugNumber;

  // measure lengthes, in whole notes
  deepClone->fFullMeasureWholeNotesDuration =
    fFullMeasureWholeNotesDuration;

  deepClone->fMeasureWholeNotesDuration = // JMI ???
    fMeasureWholeNotesDuration;

  // measure print layout, MusicXML specific
  deepClone->fMeasureMusicXMLPrintLayout =
    fMeasureMusicXMLPrintLayout;

  // measure longest note
  deepClone->fMeasureLongestNote = // JMI ???
    fMeasureLongestNote;

  // measure kind
  deepClone->fMeasureKind =
    fMeasureKind;

  // next measure number
  deepClone->fNextMeasureNumber =
    fNextMeasureNumber;

  // measure 'first in segment' kind
  deepClone->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;

    // multiple full-bar rest?

  deepClone->fMeasureIsAFullBarRest =
    fMeasureIsAFullBarRest;

  // elements

  int numberOfMeasureElements =
    fMeasureElementsList.size ();

  if (numberOfMeasureElements) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        mfSingularOrPluralWithoutNumber (
          numberOfMeasureElements, "There is", "There are") <<
        ' ' <<
        mfSingularOrPlural (
          numberOfMeasureElements, "element", "elements") <<
        " to be deep copied in measure " <<
        this->asShortString () <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
          containingSegment->
            getSegmentUpLinkToVoice ()->
              getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif

    for (
      std::list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
      i != fMeasureElementsList.end ();
      ++i
    ) {
      S_msrMeasureElement element = (*i);

      // handlle deep cloning
      S_msrMeasureElement elementDeepClone;

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*element))
        ) {
        // create the note deep clone
        elementDeepClone =
          note->createNoteDeepClone (
            fetchMeasureUpLinkToVoice ());

/* JMI
        // append the element deep clone to the measure deep clone
        deepClone->
          appendMeasureElementToMeasure (
            elementDeepClone);
*/
      }

      else if (
        S_msrTimeSignature timeSignature = dynamic_cast<msrTimeSignature*>(&(*element))
        ) {
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
          gLogStream <<
            "Sharing time " <<
            timeSignature->asShortString () <<
            " in measure " <<
            this->asShortString () <<
            "deep clone" <<
            ", line " << fInputLineNumber <<
            std::endl;
        }
#endif

        // share the element with the original measure
        elementDeepClone = element;
      }

      else {
        // share the element with the original measure
        elementDeepClone = element;
      }

      // append the element deep clone to the measure deep clone
      deepClone->
        appendMeasureElementToMeasure (
          elementDeepClone);
    } // for
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "There are no elements in measure to be deep copied" <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
        containingSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif
  }

  // upLinks

  // fMeasureUpLinkToSegment JMI ???

  return deepClone;
}

S_msrMeasure msrMeasure::createMeasureCopyWithNotesOnly (
  const S_msrSegment& containingSegment,
  const std::string&  measureNumber)
{
  S_msrVoice
    containingSegmentUpLinkToVoice =
      containingSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating a copy with notes only of measure " <<
      this->asShortString () <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
      containingSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");

  // create copy
  S_msrMeasure
    measureCopy =
      msrMeasure::create (
        fInputLineNumber,
        measureNumber,
        containingSegment);

  // set measureCopy's ordinal number JMI ???
  measureCopy->
    setMeasureOrdinalNumberInVoice (
      containingSegmentUpLinkToVoice->
        incrementVoiceCurrentMeasureOrdinalNumber ());

/* JMI
  // lengthes
  measureCopy->fFullMeasureWholeNotesDuration =
    fFullMeasureWholeNotesDuration;

  measureCopy->fMeasureWholeNotesDuration =
    fMeasureWholeNotesDuration;

  // measure kind
  measureCopy->fMeasureKind =
    fMeasureKind;

  // next measure number
  measureCopy->fNextMeasureNumber =
    fNextMeasureNumber;

  // measure 'first in segment' kind
  measureCopy->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;
*/

  // elements

  int numberOfMeasureElements =
    fMeasureElementsList.size ();

  if (numberOfMeasureElements) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        mfSingularOrPluralWithoutNumber (
          numberOfMeasureElements, "There is", "There are") <<
        ' ' <<
        mfSingularOrPlural (
          numberOfMeasureElements, "element", "elements") <<
        " to be copied with notes only in measure " <<
        this->asShortString () <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
          containingSegment->
            getSegmentUpLinkToVoice ()->
              getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif

    for (
      std::list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
      i != fMeasureElementsList.end ();
      ++i ) {
      S_msrMeasureElement element = (*i);
      S_msrMeasureElement elementToBeCopied;

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*element))
      ) {
        // copy the note
        elementToBeCopied = note;
      }

      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*element))
      ) {
        // create the chord
        elementToBeCopied = chord;
      }

      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*element))
      ) {
        // create the tuplet
        elementToBeCopied = tuplet;
      }

/* JMI
      else if (
        S_msrTimeSignature timeSignature = dynamic_cast<msrTimeSignature*>(&(*element))
        ) {
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
          gLogStream <<
            "Sharing time " <<
            timeSignature->asShortString () <<
            " in measure " <<
            this->asShortString () <<
            "deep clone" <<
            ", line " << fInputLineNumber <<
            std::endl;
        }
#endif

        // share the element with the original measure
        elementDeepClone = time;
      }
      */

      else {
        // don't share the element with the original measure
      }

      if (elementToBeCopied) {
        // append elementToBeCopied to the measure copy
        measureCopy->
          appendMeasureElementToMeasure (
            elementToBeCopied);
      }
    } // for
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "There are no elements in measure to be deep copied" <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
        containingSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif
  }

  return measureCopy;
}

/* JMI
void msrMeasure::setMeasureShortestNoteDuration (
  const Rational& duration)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalMsrOahGroup->getTraceMsrDurations ()
  ) {
    gLogStream <<
      "Setting the shortest note duration of measure " <<
      fMeasureNumber <<
      " to " <<
      duration <<
      std::endl;
  }
#endif

  fMeasureShortestNoteDuration = duration;
}

void msrMeasure::setMeasureShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalMsrOahGroup->getTraceMsrDurations ()
  ) {
    gLogStream <<
      "Setting the shortest note tuplet factor of measure " <<
      fMeasureNumber <<
      " to " <<
      noteTupletFactor <<
      std::endl;
  }
#endif

  fMeasureShortestNoteTupletFactor = noteTupletFactor;
}
*/

void msrMeasure::setMeasureEndRegularKind (
  msrMeasureEndRegularKind measureEndRegularKind)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Setting regular kind of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        msrMeasureEndRegularKindAsString (measureEndRegularKind) <<
        "' in segment " <<
        fMeasureUpLinkToSegment->asString () <<
        " in voice \"" <<
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        std::endl;
    }
#endif

  fMeasureEndRegularKind = measureEndRegularKind;
}

void msrMeasure::setMeasureRepeatContextKind (
  msrMeasureRepeatContextKind measureRepeatContextKind)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Setting repeat context kind of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        measureRepeatContextKind <<
        "' in segment " <<
        fMeasureUpLinkToSegment->asString () <<
        " in voice \"" <<
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        std::endl;
    }
#endif

  fMeasureRepeatContextKind = measureRepeatContextKind;
}

void msrMeasure::setMeasureNumber (
  const std::string& measureNumber)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Setting measure element measure number of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        measureNumber <<
        "' in segment " <<
        fMeasureUpLinkToSegment->asString () <<
        " in voice \"" <<
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        std::endl;
    }
#endif

  fMeasureNumber = measureNumber;
}

void msrMeasure::setMeasureOrdinalNumberInVoice (
  int measureOrdinalNumber)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Setting ordinal number of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        measureOrdinalNumber <<
        "' in segment " <<
        fMeasureUpLinkToSegment->asString () <<
        " in voice \"" <<
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        std::endl;
    }
#endif

  fMeasureOrdinalNumberInVoice = measureOrdinalNumber;
}

void msrMeasure::setMeasurePuristNumber (
  int measurePuristNumber)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Setting purist number of measure '" <<
        fMeasureNumber <<
        "' to '" <<
        measurePuristNumber <<
        "' in segment " <<
        fMeasureUpLinkToSegment->asString () <<
        " in voice \"" <<
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        std::endl;
    }
#endif

  fMeasurePuristNumber = measurePuristNumber;
}

// void msrMeasure::incrementMeasureVoicePosition (
// Rational wholeNotesDelta)
// {
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
//       gLogStream <<
//         "Incrementing the measure voice position " <<
//         this->asShortString () <<
//         " by " <<
//         wholeNotesDelta <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//     }
// #endif
//
//   fMeasureVoicePosition += wholeNotesDelta;
// }

void msrMeasure::appendMeasureElementToMeasure (
  const S_msrMeasureElement& elem)
{
  int inputLineNumber =
    elem->getInputLineNumber ();

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    elem != nullptr,
    "appendMeasureElementToMeasure(): elem is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending element " <<
      elem->asShortString () <<
      " to measure " <<
      asShortString () <<
//       " in voice \"" <<
//       fetchMeasureUpLinkToVoice ()-> JMI not yet set v0.9.66
//         getVoiceName () <<
//       "\"" <<
      ", fMeasureWholeNotesDuration: " <<
      fMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // populate elem uplink to measure
  elem->
    setMeasureElementUpLinkToMeasure (this);

  // set elem's measure number
  S_msrMeasure upLinkToMeasure;

  elem->
    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

  upLinkToMeasure->
    setMeasureNumber (
      fMeasureNumber);

  // set elem's measure position
  elem->
    setMeasureElementMeasurePosition (
      this,
      fMeasureWholeNotesDuration,
      "appendMeasureElementToMeasure()");

  // append elem to the measure elements list
  fMeasureElementsList.push_back (elem);

  // take elem's sounding whole notes into account
  incrementMeasureWholeNotesDuration (
    inputLineNumber,
    elem->
      getMeasureElementSoundingWholeNotes ());
}

void msrMeasure::insertElementInMeasureBeforeIterator (
  int                                      inputLineNumber,
  std::list<S_msrMeasureElement>::iterator iter,
  const S_msrMeasureElement&               elem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Inserting element " <<
      elem->asShortString () <<
      " before iterator " <<
      (*iter)->asShortString () <<
      " in measure " <<
      asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()
          ->getVoiceName () <<
      "\", measureWholeNotesDuration = " <<
      fMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // set elem's measure number
  S_msrMeasure upLinkToMeasure;

  elem->
    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

  upLinkToMeasure->
    setMeasureNumber (
      fMeasureNumber);

  // set elem's measure position
  elem->
    setMeasureElementMeasurePosition (
      this,
      fMeasureWholeNotesDuration,
      "insertElementInMeasureBeforeIterator()");

  // insert elem in the measure elements list before (*iter)
  fMeasureElementsList.insert (
    iter, elem);

  // account for elem's duration in measure whole notes
  incrementMeasureWholeNotesDuration (
    inputLineNumber,
    elem->
      getMeasureElementSoundingWholeNotes ());
}

void msrMeasure::appendElementAtTheEndOfMeasure (
  const S_msrMeasureElement& elem)
{
  int inputLineNumber =
    elem->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceBarLines ()
  ) {
    gLogStream <<
      "Appending element " <<
      elem->asString () <<
      " at the end of measure " <<
      asString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()
          ->getVoiceName () <<
      "\", has measure position '" <<
      elem->getMeasureElementMeasurePosition () <<
      ", fMeasureWholeNotesDuration: " <<
      fMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      std::endl;
  }

  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    // JMI ??? v0.9.63
    typedef std::list<S_msrMeasureElement>::iterator iter_type;

    gLogStream <<
      std::endl <<
      "==> appendPaddingNoteAtTheEndOfMeasure(), fMeasureElementsList:" <<
      std::endl;
    ++gIndenter;
    for (
      iter_type it = fMeasureElementsList.begin ();
      it != fMeasureElementsList.end ();
      ++it
    ) {
      gLogStream << ' ' << *it << std::endl;
    } // for
    gLogStream << std::endl;
    --gIndenter;

    displayMeasure (
      inputLineNumber,
      "appendPaddingNoteAtTheEndOfMeasure() 1");
  }
#endif

  if (! fMeasureElementsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "fMeasureElementsList is empty " <<
        ", inserting elem at the end" <<
        std::endl;
    }
#endif

    appendMeasureElementToMeasure (elem);

    // take elem's sounding whole notes duration into account
    // could be done elsewhere ??? JMI
    incrementMeasureWholeNotesDuration (
      inputLineNumber,
      elem->
        getMeasureElementSoundingWholeNotes ());
  }

  else {
    S_msrMeasureElement
      lastMeasureElement =
        fMeasureElementsList.back ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "fMeasureElementsList is NOT empty " <<
        ", lastMeasureElement: " <<
        lastMeasureElement <<
        std::endl;
    }
#endif

    if (
        // barLine?
        S_msrBarLine
          barLine =
            dynamic_cast<msrBarLine*>(&(*lastMeasureElement))
    ) {
      if (true || barLine->barLineIsAFinalBar ()) { // JMI
#ifdef TRACING_IS_ENABLED
        if (
          gGlobalTracingOahGroup->getTraceMeasures ()
            ||
          gGlobalTracingOahGroup->getTraceBarLines ()
        ) {
          gLogStream <<
            "last measure element " <<
            lastMeasureElement->asString () <<
            " is a final barLine, inserting elem before it" <<
            std::endl;
        }
#endif

        // fetch iterator to std::list end
        std::list<S_msrMeasureElement>::iterator it =
            fMeasureElementsList.end ();

        // fetch iterator to std::list last element
        --it;

        // insert elem before it in std::list
        // (will increment this measure's whole notes duration)
        insertElementInMeasureBeforeIterator (
          inputLineNumber,
          it,
          elem);

        // register this measure as regular in case it was empty
        // should be done elsewhere JMI ???
      }

      else {
#ifdef TRACING_IS_ENABLED
        if (
          gGlobalTracingOahGroup->getTraceMeasures ()
            ||
          gGlobalTracingOahGroup->getTraceBarLines ()
        ) {
          gLogStream <<
            "last measure element " <<
            lastMeasureElement->asString () <<
            " is a barLine, but not a final one, inserting elem after it" <<
            std::endl;
        }
#endif

        appendMeasureElementToMeasure (elem);
      }
    }

    else {
#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceMeasures ()
          ||
        gGlobalTracingOahGroup->getTraceBarLines ()
      ) {
        gLogStream <<
          "last measure element " <<
          lastMeasureElement->asString () <<
          " is no barLine, inserting elem after it" <<
          std::endl;
      }
#endif

      appendMeasureElementToMeasure (elem);
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    typedef std::list<S_msrMeasureElement>::iterator iter_type;

    gLogStream <<
      std::endl <<
      "<== appendPaddingNoteAtTheEndOfMeasure(), fMeasureElementsList:" <<
      std::endl;
    ++gIndenter;
    for (
      iter_type it = fMeasureElementsList.begin ();
      it != fMeasureElementsList.end ();
      ++it
    ) {
      gLogStream << ' ' << *it << std::endl;
    } // for
    gLogStream << std::endl;
    --gIndenter;

    displayMeasure (
      inputLineNumber,
      "appendPaddingNoteAtTheEndOfMeasure() 2");
  }
#endif
}

void msrMeasure::insertElementAtMeasurePosition (
  int                 inputLineNumber,
  const Rational&     measurePosition,
  const S_msrMeasureElement& elem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Inserting element " <<
      elem->asShortString () <<
      " at position " <<
      measurePosition <<
      " in measure " <<
      asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()
          ->getVoiceName () <<
      "\", measureWholeNotesDuration = " <<
      fMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  size_t
    measureElementsListSize =
      fMeasureElementsList.size ();

  if (measureElementsListSize) {
    std::list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement
        currentElement = (*i);

      Rational currentMeasurePosition =
        currentElement->
          getMeasureElementMeasurePosition ();

      if (measurePosition == currentMeasurePosition) {
        // insert elem in the measure elements list before (*i)
        fMeasureElementsList.insert (
          i, elem);
      }
      else if (measurePosition >= currentMeasurePosition) {
        std::stringstream s;

        s <<
          "cannot insert element " <<
          elem->asShortString () <<
          " at position " <<
          measurePosition <<
          " in measure " <<
          asShortString () <<
          " in voice \"" <<
          fMeasureUpLinkToSegment->
            getSegmentUpLinkToVoice ()
              ->getVoiceName () <<
          "\", measureWholeNotesDuration = " <<
          fMeasureWholeNotesDuration <<
          " since there's no element at this exact position " <<
          ", line " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      if (++i == iEnd) break;
    } // for
  }

  else {
    std::stringstream s;

    s <<
      "cannot insert element " <<
      elem->asShortString () <<
      " at position " <<
      measurePosition <<
      " in measure " <<
      asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()
          ->getVoiceName () <<
      "\", measureWholeNotesDuration = " <<
      fMeasureWholeNotesDuration <<
      " since it is empty" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // set elem's measure number
  S_msrMeasure upLinkToMeasure;

  elem->
    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

  upLinkToMeasure->
    setMeasureNumber (
      fMeasureNumber);

  // set elem's measure position
  elem->
    setMeasureElementMeasurePosition (
      this,
      measurePosition,
      "insertElementAtMeasurePosition()");

  // account for elem's duration in measure whole notes
  incrementMeasureWholeNotesDuration (
    inputLineNumber,
    elem->
      getMeasureElementSoundingWholeNotes ());
}

void msrMeasure::setNextMeasureNumber (const std::string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      "' in measure " <<
      this->asShortString () <<
      " in segment " <<
      fMeasureUpLinkToSegment-> asString () <<
      /* JMI
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      */
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fNextMeasureNumber = nextMeasureNumber;
}

void msrMeasure::setMeasureIsFirstInVoice ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting measure " <<
      this->asShortString () <<
      " as first measure in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      "' in segment " <<
      fMeasureUpLinkToSegment-> asString () <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fMeasureIsFirstInVoice = true;
}

std::string msrMeasure::fullMeasureWholeNotesDurationAsMsrString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFullMeasureWholeNotesDuration);
}

void msrMeasure::setFullMeasureWholeNotesDuration (
  const Rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "Setting full whole notes duration of measure " <<
      this->asShortString () <<
      " to '"  <<
      wholeNotes <<
      "'" <<
      std::endl;
  }
#endif

  fFullMeasureWholeNotesDuration = wholeNotes;
}

void msrMeasure::setMeasureWholeNotesDuration (
  int             inputLineNumber,
  const Rational& wholeNotes)
{
  // rationalise the measure whole notes duration
  Rational
    rationalisedMeasureWholeNotesDuration =
      wholeNotes;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "Setting measure whole notes duration of  " <<
      this->asShortString () <<
      " to '"  <<
      rationalisedMeasureWholeNotesDuration <<
      "'";

    if (
      rationalisedMeasureWholeNotesDuration.getDenominator ()
        !=
      wholeNotes.getDenominator ()
    ) {
      gLogStream <<
        " (rationalised from '" << wholeNotes << "')";
    }

    gLogStream <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  // set measure whole notes
  fMeasureWholeNotesDuration =
    rationalisedMeasureWholeNotesDuration;
}

void msrMeasure::incrementMeasureWholeNotesDuration (
  int             inputLineNumber,
  const Rational& wholeNotesDelta)
{
//   // sanity check
// if (false) // JMI v0.9.66
//   mfAssert (
//     __FILE__, __LINE__,
//     wholeNotesDelta.getNumerator () != 0,
//     "wholeNotesDelta.getNumerator () == 0");

  // compute the new measure whole notes duration
  Rational
    newMeasureWholeNotesDuration =
      fMeasureWholeNotesDuration + wholeNotesDelta;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "Incrementing whole notes duration of measure " <<
      this->asShortString ()<<
      " from '"  <<
      fMeasureWholeNotesDuration <<
      "' by '"  <<
      wholeNotesDelta <<
      "' to '"  <<
      newMeasureWholeNotesDuration <<
      "'";

/* JMI
    if (
      newMeasureWholeNotesDuration.getDenominator ()
        !=
      wholeNotes.getDenominator ()
    ) {
      gLogStream <<
        " (rationalised from '" << wholeNotes << "')";
    }
*/

    gLogStream <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  // set new measure whole notes duration
  setMeasureWholeNotesDuration (
    inputLineNumber,
    newMeasureWholeNotesDuration);
}

std::string msrMeasure::measureWholeNotesDurationAsMsrString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureWholeNotesDuration);
}

void msrMeasure::setMeasureKind (
  msrMeasureKind measureKind)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
      gLogStream <<
        "Setting measure kind of measure " <<
        this->asShortString ()<<
        " to '" <<
        measureKind <<
        "' in segment " <<
        fMeasureUpLinkToSegment->asString () <<
        " in voice \"" <<
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        std::endl;
    }
#endif

  switch (measureKind) {
    case msrMeasureKind::kMeasureKindUnknown:
      break;
    case msrMeasureKind::kMeasureKindRegular:
      break;
    case msrMeasureKind::kMeasureKindAnacrusis:
      break;
    case msrMeasureKind::kMeasureKindIncompleteStandalone:
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
      break;
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
      break;
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
      break;
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      break;

    case msrMeasureKind::kMeasureKindOvercomplete:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
        gLogStream <<
          "!!! Setting measure kind of measure " <<
          this->asShortString ()<<
          " to '" <<
          measureKind <<
          "' in segment " <<
          fMeasureUpLinkToSegment->asString () <<
          " in voice \"" <<
          fMeasureUpLinkToSegment->
            getSegmentUpLinkToVoice ()->
              getVoiceName () <<
          "\"" <<
          ", line " << fInputLineNumber <<
          std::endl;

        gLogStream <<
          std::endl <<
          "--> this measure:" <<
          std::endl;
        ++gIndenter;
        this->print (gLogStream);
        gLogStream << std::endl;
        --gIndenter;
      }
#endif
      break;

    case msrMeasureKind::kMeasureKindCadenza:
      break;

    case msrMeasureKind::kMeasureKindMusicallyEmpty:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
        gLogStream <<
          "??? Setting measure kind of measure " <<
          this->asShortString ()<<
          " to '" <<
          measureKind <<
          "' in segment " <<
          fMeasureUpLinkToSegment->asString () <<
          " in voice \"" <<
          fMeasureUpLinkToSegment->
            getSegmentUpLinkToVoice ()->
              getVoiceName () <<
          "\"" <<
          ", line " << fInputLineNumber <<
          std::endl;

        gLogStream <<
          std::endl <<
          "--> this measure:" <<
          std::endl;
        ++gIndenter;
        this->print (gLogStream);
        gLogStream << std::endl;
        --gIndenter;
      }
#endif

      break;
  } // switch

  fMeasureKind = measureKind;
}

void msrMeasure::appendMusicXMLPrintLayoutToMeasure (
  const S_msrMusicXMLPrintLayout& musicXMLPrintLayout)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMusicXMLPrintLayouts ()) {
    gLogStream <<
      "Appending print layout " << musicXMLPrintLayout->asString () <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureUpLinkToVoice ()->
        getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // append it to the measure elements list
  prependOtherElementToMeasure (musicXMLPrintLayout);

  // register it for MusicXML generation from MSR
  fMeasureMusicXMLPrintLayout = musicXMLPrintLayout;
}

void msrMeasure::appendClefToMeasure (
  const S_msrClef& clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef " << clef->asString () <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureUpLinkToVoice ()->
        getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // append it to the measure elements list
  appendMeasureElementToMeasure (clef);
}

void msrMeasure::appendKeyToMeasure (
  const S_msrKey& key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " << key->asString () <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureUpLinkToVoice ()->
        getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // append it to the measure elements list
  appendMeasureElementToMeasure (key);
}

void msrMeasure::appendTimeSignatureToMeasure (
  const S_msrTimeSignature& timeSignature)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    timeSignature != nullptr,
    "timeSignature is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    gLogStream <<
      "' to measure " <<
      this->asShortString ()<<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append timeSignature to the measure elements list
  appendMeasureElementToMeasure (timeSignature);

  // set the measure whole notes per full measure
  setFullMeasureWholeNotesDurationFromTime (
    timeSignature);

  --gIndenter;
}

void msrMeasure::appendTimeSignatureToMeasureClone (
  const S_msrTimeSignature& timeSignature)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    timeSignature != nullptr,
    "timeSignature is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    S_msrVoice
      upLinkToVoice =
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ();

    gLogStream <<
      "to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      upLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // append timeSignature to the measure elements list
  appendMeasureElementToMeasure (timeSignature);
}

void msrMeasure::insertHiddenMeasureAndBarLineInMeasureClone (
  int             inputLineNumber,
  const Rational& measurePosition)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      measurePosition <<
      "' in measure clone \"" <<
      this->asShortString () <<
      "\" in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
// JMI      "' in voice \"" <<
//      upLinkToVoice->getVoiceName () <<
//      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a hidden measure and barLine
  S_msrHiddenMeasureAndBarLine
    hiddenMeasureAndBarLine =
      msrHiddenMeasureAndBarLine::create (
        inputLineNumber,
        nullptr); // will be set when hidden measure and barline is appended to a measure JMI v0.9.66 PIM

/* JMI BLARK
  // insert it in the measure elements list
  insertElementAtMeasurePosition (
    inputLineNumber,
    measurePosition,
    hiddenMeasureAndBarLine);
    */
}

void msrMeasure::setFullMeasureWholeNotesDurationFromTime (
  const S_msrTimeSignature& timeSignature)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    timeSignature != nullptr,
    "timeSignature is null");

#ifdef TRACING_IS_ENABLED
  if ( // JMI
    gGlobalTracingOahGroup->getTraceWholeNotes ()
      ||
    gGlobalTracingOahGroup->getTraceTimeSignatures ()
  ) {
    gLogStream <<
      "Setting measure full measure whole notes from time signature:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    gLogStream <<
      "for measure " <<
      this->asShortString ()<<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  switch (timeSignature->getTimeSignatureSymbolKind ()) {
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon:
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut:
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote:
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote:
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber:
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone:
      {
        // this measure is con misura

        Rational
          wholeNotesPerMeasure =
            timeSignature->
              wholeNotesDurationPerMeasure ();

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
          gLogStream <<
            "There are " <<
            wholeNotesPerMeasure <<
            " whole note(s) per measure in time signature:" <<
            std::endl;

          ++gIndenter;

          gLogStream <<
            timeSignature;

          --gIndenter;

          gLogStream <<
            "in measure " <<
            this->asShortString () <<
            " in voice \"" <<
            fMeasureUpLinkToSegment->
              getSegmentUpLinkToVoice ()->
                getVoiceName () <<
            "\"" <<
            ", line " << fInputLineNumber <<
            std::endl;
        }
#endif

        // set full measure whole notes
        setFullMeasureWholeNotesDuration (
          wholeNotesPerMeasure);

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
          gLogStream <<
            "Measure " <<
            this->asShortString () <<
            "'in voice \"" <<
            fMeasureUpLinkToSegment->
              getSegmentUpLinkToVoice ()->
                getVoiceName () <<
            "\"" <<
            " has " <<
            fFullMeasureWholeNotesDuration <<
            " full measure whole notes " <<
            std::endl;
        }
#endif
      }
      break;

    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:

      // this measure is senza misura

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
        gLogStream <<
          "Measure " <<
          this->asShortString () <<
           " in voice \"" <<
          fMeasureUpLinkToSegment->
            getSegmentUpLinkToVoice ()->
              getVoiceName () <<
          "\"" <<
          " is senza misura" <<
          std::endl;
      }
#endif

      setMeasureKind (msrMeasureKind::kMeasureKindCadenza);

      setFullMeasureWholeNotesDuration (
        Rational (INT_MAX, 1));
      break;
  } // switch

  --gIndenter;
}

void msrMeasure::appendTranspositionToMeasure (
  const S_msrTransposition& transposition)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    // fetch the voice
    S_msrVoice
      voice =
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ();

    gLogStream <<
      "Appending transposition " <<
      transposition->asShortString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      voice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // append it to the measure elements list
  appendMeasureElementToMeasure (transposition);
}

void msrMeasure::printMeasurePendingMeasureElementsList ()
{
  gLogStream <<
    std::endl <<
    "===> printMeasurePendingMeasureElementsList ()" <<
    std::endl;

  ++gIndenter;

  if (fMeasurePendingMeasureElementsList.size ()) {
    std::list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasurePendingMeasureElementsList.begin (),
      iEnd   = fMeasurePendingMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i)->asShortString ();
      if (++i == iEnd) break;
      gLogStream << std::endl;
    } // for
  }
  else {
    gLogStream <<
      "[NONE]" <<
      std::endl;
  }

  --gIndenter;

  gLogStream <<
    "<===" <<
    std::endl << std::endl;
}

void msrMeasure::appendBarLineToMeasure (
  const S_msrBarLine& barLine)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " <<
      barLine->asShortString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      voice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  appendMeasureElementToMeasure (barLine);
}

void msrMeasure::prependBarLineToMeasure (
  const S_msrBarLine& barLine)
{
  // append it to the measure elements list
  fMeasureElementsList.push_front (barLine);
}

void msrMeasure::appendSegnoToMeasure (
  const S_msrSegno& segno)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (segno);
}

void msrMeasure::appendCodaToMeasure (
  const S_msrCoda& coda)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (coda);
}

void msrMeasure::appendEyeGlassesToMeasure (
  const S_msrEyeGlasses& eyeGlasses)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (eyeGlasses);
}

void msrMeasure::appendPedalToMeasure (
  const S_msrPedal& pedal)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (pedal);
}

void msrMeasure::appendDampToMeasure (
  const S_msrDamp& damp)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (damp);
}

void msrMeasure::appendDampAllToMeasure (
  const S_msrDampAll& dampAll)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (dampAll);
}

void msrMeasure::appendBarCheckToMeasure (
  const S_msrBarCheck& barCheck)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (barCheck);
}

void msrMeasure::appendVoiceStaffChangeToMeasure (
  const S_msrVoiceStaffChange& voiceStaffChange)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (voiceStaffChange);
}

void msrMeasure::appendNoteToMeasure (
  const S_msrNote& note,
  const Rational&  partMeasurePosition)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  Rational
    positionsDelta =
      partMeasurePosition
        -
      fMeasureWholeNotesDuration;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note " << note->asShortString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", partMeasurePosition: " <<
      partMeasurePosition <<
      ", fMeasureWholeNotesDuration: " <<
      fMeasureWholeNotesDuration <<
      ", positionsDelta: " <<
      positionsDelta <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // should a skip note be appended before note?
  if (positionsDelta.getNumerator () > 0) {
    // fetch the voice
    S_msrVoice
      voice =
        fetchMeasureUpLinkToVoice ();

    // create a skip note of duration wholeNotesDelta
    S_msrNote
      skipNote =
        msrNote::createSkipNote (
          inputLineNumber,
          fMeasureNumber,
          positionsDelta,
          positionsDelta,
          0); // dots number JMI ???
          /* JMI
          voice->
            getVoiceUpLinkToStaff ()->
              getStaffNumber (),
          voice->
            getVoiceNumber ());
            */

    // append it to the measure
    appendNoteOrPaddingToMeasure (
      skipNote);
  }

  else if (positionsDelta.getNumerator () < 0) {
    std::stringstream s;

    s <<
      "partMeasurePosition " <<
      partMeasurePosition <<
      " is smaller than fMeasureWholeNotesDuration " <<
      fMeasureWholeNotesDuration <<
      "' in measure " <<
      this->asShortString () <<
      ", cannot padup in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", fMeasureWholeNotesDuration " <<
      fMeasureWholeNotesDuration <<
      ", partMeasurePosition " <<
      partMeasurePosition <<
      ", positionsDelta " << positionsDelta <<
      ", line " << inputLineNumber;

//  msrInternalError ( // JMI v0.9.66
    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
  //    __FILE__, __LINE__,
      s.str ());
  }

  else {
    // this measure is already at the part current measure position,
    // nothing to do
  }

  // append note to measure
  appendNoteOrPaddingToMeasure (note);

  // is note a multiple full-bar rest?
  if (note->fetchNoteIsAFullNoteRest ()) {
    setMeasureIsAFullBarRest ();
  }

  // set grace notes group 'before' elements measure position in relevant
  S_msrGraceNotesGroup
    noteGraceNotesGroupBefore =
      note->
        getNoteGraceNotesGroupBefore ();

  if (noteGraceNotesGroupBefore) {
    noteGraceNotesGroupBefore->
      setGraceNotesGroupElementsMeasurePositions (
        this,
        note->getMeasureElementMeasurePosition ());
  }

  // set grace notes group 'after' elements measure position in relevant
  S_msrGraceNotesGroup
    noteGraceNotesGroupAfter =
      note->
        getNoteGraceNotesGroupAfter ();

  if (noteGraceNotesGroupAfter) {
    noteGraceNotesGroupAfter->
      setGraceNotesGroupElementsMeasurePositions (
        this,
        note->getMeasureElementMeasurePosition ()
         +
        note->getMeasureElementSoundingWholeNotes ());
  }

  --gIndenter;
}

void msrMeasure::appendNoteOrPaddingToMeasure (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // fetch note sounding whole notes
  Rational
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note or padding '" << note->asShortString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
       voice->getVoiceName () <<
      "\"" <<
      ", measureWholeNotesDuration: " <<
      fMeasureWholeNotesDuration <<
      ", noteSoundingWholeNotes: " << noteSoundingWholeNotes <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  appendMeasureElementToMeasure (note);

  // append it to the measure notes flat std::list if relevant
  switch (note->getNoteKind ()) {
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      break;

    default:
      appendNoteToMeasureNotesFlatList (note);
  } // switch

  // register note as the last one in this measure
  fMeasureLastHandledNote = note;

/* JMI
  // is this note the shortest one in this voice?
  voice->
    registerShortestNoteInVoiceIfRelevant (
      note);
*/

  // is this note the longest one in this measure?
  if (! fMeasureLongestNote) {
    fMeasureLongestNote = note;
  }
  else {
    // register note as measure longest sounding and/or display note if relevant
    // JMI ???
    if (
      note->getMeasureElementSoundingWholeNotes ()
        >
      fMeasureLongestNote->getMeasureElementSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }

    if (
      note->getNoteDisplayWholeNotes ()
        >
      fMeasureLongestNote->getNoteDisplayWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }
  }

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::accountForTupletMemberNoteDurationInMeasure (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Accounting for the duration of tuplet member note " <<
      note->asShortString () <<
      " in measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // set note's measure position JMI v0.9.66
//   note->
//     setMeasureElementMeasurePosition (
//       this,
//       fMeasureWholeNotesDuration,
//       "accountForTupletMemberNoteDurationInMeasure()");

  // fetch note sounding whole notes
  Rational
    noteSoundingWholeNotes =
      note->getMeasureElementSoundingWholeNotes ();

  // append it to the measure notes flat std::list
  appendNoteToMeasureNotesFlatList (note);

  // account for note duration in measure whole notes
  incrementMeasureWholeNotesDuration (
    inputLineNumber,
    noteSoundingWholeNotes);
}

void msrMeasure::appendPaddingNoteAtTheEndOfMeasure (const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Appending padding note " << note->asString () <<
      " at the end of measure " <<
      this->asString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // populate uplink to measure
  note->
    setMeasureElementUpLinkToMeasure (this);

  // append the note at the end of the measure
  appendElementAtTheEndOfMeasure (note);

  // register note as the last one in this measure
  fMeasureLastHandledNote = note;

  // is this note the longest one in this measure?
  if (! fMeasureLongestNote) {
    fMeasureLongestNote = note;
  }
  else {
    // register note as measure longest sounding and/or display note if relevant
    // JMI ???
    if (
      note->getMeasureElementSoundingWholeNotes ()
        >
      fMeasureLongestNote->getMeasureElementSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }

    if (
      note->getNoteDisplayWholeNotes ()
        >
      fMeasureLongestNote->getMeasureElementSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }
  }

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendNoteToMeasureClone (const S_msrNote& note)
{
//   int inputLineNumber =
//     note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note '" << note->asShortString () <<
      "' to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

    /* JMI
  if (appendMeasureIfOverflow (inputLineNumber)) {
    // a new measure has been appended to the segment
    // append note to it via the segment
    fMeasureUpLinkToSegment->
      appendNoteToSegment (note);
  }

  else {
  */
    // regular insertion in current measure

    // populate uplink to measure
    note->
      setMeasureElementUpLinkToMeasure (this);

    // append the note to the measure elements list
  // JMI  // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendMeasureElementToMeasure (note);

    // fetch note sounding whole notes
    Rational
      noteSoundingWholeNotes =
        note->
          getMeasureElementSoundingWholeNotes ();

  /* JMI
    // determine whether the note occupies a full measure
    if (noteSoundingWholeNotes == fFullMeasureWholeNotesDuration)
      note->
        setNoteOccupiesAFullMeasure ();
      */

    // register note as the last one in this measure
    fMeasureLastHandledNote = note;

  // this measure contains music
  fMeasureContainsMusic = true;
 // JMI }

  --gIndenter;
}

void msrMeasure::appendDoubleTremoloToMeasure (
  const S_msrDoubleTremolo& doubleTremolo)
{
//   int inputLineNumber =
//     doubleTremolo->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  appendMeasureElementToMeasure (doubleTremolo);

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendChordToMeasure (const S_msrChord& chord)
{
//   int inputLineNumber =
//     chord->getInputLineNumber ();

  // populate uplink to measure
  chord->setMeasureElementUpLinkToMeasure (this);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending chord '" << chord->asString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // set the chord kind
  chord->setChordKind (msrChordInKind::kChordInMeasure);

  // set the chord's measure position
//   chord->
//     setChordMeasurePosition (
//       this,
//       fMeasureWholeNotesDuration,
//       "msrMeasure::appendChordToMeasure (const S_msrChord& chord)");

  // append the chord to the measure elements list
  appendMeasureElementToMeasure (chord);

//   // fetch chord sounding whole notes
//   Rational
//     chordSoundingWholeNotes =
//       chord->
//         getMeasureElementSoundingWholeNotes ();
//
  // determine if the chord occupies a full measure
// XXL  JMI  if (chordSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // chord->setChordOccupiesAFullMeasure ();

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendTupletToMeasure (const S_msrTuplet& tuplet)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    tuplet->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending tuplet " << tuplet->asString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // set the tuplet kind
  tuplet->setTupletKind (msrTupletInKind::kTupletInMeasure);

  // set the tuplet's measure position
//   tuplet->
//     setTupletMeasurePosition (
//       this,
//       fMeasureWholeNotesDuration,
//       "msrMeasure::appendTupletToMeasure (const S_msrChord& chord)");

  // populate uplink to measure
  tuplet->setMeasureElementUpLinkToMeasure (this);

  // append the tuplet to the measure elements list
  appendMeasureElementToMeasure (tuplet);

  // the duration of the individual members of the tuplet
  // have already been accounted for in current measure's whole notes

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendHarmonyToMeasure (const S_msrHarmony& harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", measureWholeNotesDuration = " <<
      fMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append harmony to the measure elements list
  appendMeasureElementToMeasure (harmony);

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendHarmonyToMeasureClone (const S_msrHarmony& harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", measureWholeNotesDuration = " <<
      fMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append harmony to the measure elements list
  appendMeasureElementToMeasure (harmony);

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendFiguredBassToMeasure (
  const S_msrFiguredBass& figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass element " << figuredBass->asString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", measureWholeNotesDuration = " <<
      fMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // get the measure position of the note
  Rational
    measurePosition =
      figuredBass->
        getFiguredBassUpLinkToNote ()->
          getMeasureElementMeasurePosition ();

  // pad up to it
  padUpToMeasurePosition (
    inputLineNumber,
    measurePosition);

  // append harmony to the measure elements list
  appendMeasureElementToMeasure (figuredBass);

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendFiguredBassToMeasureClone (
  const S_msrFiguredBass& figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", measureWholeNotesDuration = " <<
      fMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append the figuredBass to the measure elements list
  appendMeasureElementToMeasure (figuredBass);

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

S_msrNote msrMeasure::createPaddingSkipNoteForVoice (
  int             inputLineNumber,
  const Rational& duration,
  const S_msrVoice&      voice)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasurePositions ()
      ||
    gGlobalTracingOahGroup->getTraceVoices ()
  ) {
    gLogStream <<
      "Creating a padding note for voice \"" <<
      voice->getVoiceName () <<
      "', duration = '" <<
      duration <<
      "\" in measure " <<
      this->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a skip note
  S_msrNote
    skipNote =
      msrNote::createSkipNote (
        inputLineNumber,
        fMeasureNumber,
        duration,
        duration,
        0); // dots number JMI ???
        /* JMI
        voice->
          getVoiceUpLinkToStaff ()->
            getStaffNumber (),
        voice->
          getVoiceNumber ());
          */

  return skipNote;
}

void msrMeasure::padUpToMeasurePosition (
  int             inputLineNumber,
  const Rational& measurePositionToPadUpTo)
{
  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    this->print (gLogStream);

    gLogStream <<
      "Padding from measure whole notes '" <<
      fMeasureWholeNotesDuration <<
      "' to '" << measurePositionToPadUpTo <<
      "' in measure " <<
      this->asShortString () <<
      " in segment " <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePositionToPadUpTo.getNumerator () >= 0,
    "measurePositionToPadUpTo.getNumerator () is negative in padUpToMeasurePosition()");

  if (fMeasureWholeNotesDuration < measurePositionToPadUpTo) {
    ++gIndenter;

    // appending a padding rest or skip to this measure to reach measurePositionToPadUpTo
    Rational
      missingDuration =
        measurePositionToPadUpTo - fMeasureWholeNotesDuration;

    // create a padding skip note
    S_msrNote
      paddingNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "Appending skip " << paddingNote->asString () <<
        " (missingDuration " << missingDuration <<
        " whole notes) to skip from length '" <<
        fMeasureWholeNotesDuration <<
        " to length '" << measurePositionToPadUpTo << "'"
        " in measure " <<
        this->asShortString () <<
        " in voice \"" <<
        measureVoice->getVoiceName () <<
        std::endl;
    }
#endif

    // append the paddingNote to the measure
    appendNoteOrPaddingToMeasure (paddingNote);

/* JMI
    // set this measure as being padded // JMI
    this->
      setMeasureCreatedForARepeatKind (
        msrMeasureKind::kMeasureKindCreatedForARepeatPadded);
    */

    // this measure contains music
    fMeasureContainsMusic = true;

    --gIndenter;
  }

  else if (fMeasureWholeNotesDuration == measurePositionToPadUpTo) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "No need to pad from measure whole notes '" <<
        fMeasureWholeNotesDuration <<
        "' to '" <<
        measurePositionToPadUpTo <<
        "' since they are equal in measure " <<
        this->asShortString () <<
        " in segment " <<
        fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
        " in voice \"" <<
        measureVoice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif
  }

  else {
    measureVoice->print (gLogStream); // JMI
    this->print (gLogStream);

    std::stringstream s;

    s <<
        "Cannot pad from measure whole notes '" <<
        fMeasureWholeNotesDuration <<
        "' to '" <<
        measurePositionToPadUpTo <<
        "' since the former is larger than the latter in measure " <<
        this->asShortString () <<
        " in segment " <<
        fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
        " in voice \"" <<
        measureVoice->getVoiceName () <<
        "\", line " << inputLineNumber;

//    msrError ( JMI
    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
 //     __FILE__, __LINE__,
      s.str ());
  }
}

// void msrMeasure::padUpToMeasurePosition (
//   int             inputLineNumber,
//   const Rational& measurePositionToPadUpTo)
// {
//   // fetch the voice
//   S_msrVoice
//     measureVoice =
//       fMeasureUpLinkToSegment->
//         getSegmentUpLinkToVoice ();
//
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//     gLogStream <<
//       "Padding up to position '" <<
//       measurePositionToPadUpTo <<
//       "' in measure " <<
//       this->asShortString () <<
//       " in segment " <<
//       fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
//       " in voice \"" <<
//       measureVoice->getVoiceName () <<
//       "\", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif
//
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
//     displayMeasure (
//       inputLineNumber,
//       "padUpToMeasurePosition() 1");
//   }
// #endif
//
//   ++gIndenter;
//
//   if (fMeasureWholeNotesDuration < measurePositionToPadUpTo) {
//     // appending a rest to this measure to reach measurePositionToPadUpTo
//     Rational
//       missingDuration =
//         measurePositionToPadUpTo - fMeasureWholeNotesDuration;
//
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//       gLogStream <<
//        "Creating a padding note for measure debug number " <<
//        fMeasureDebugNumber <<
//        ", missingDuration: " << missingDuration <<
//        " in voice \"" << measureVoice->getVoiceName () <<
//        "\", measure: " <<
//         this->asShortString () <<
//        ", measureWholeNotesDuration: " <<
//        fMeasureWholeNotesDuration <<
//        std::endl;
//    }
// #endif
//
//     // create a padding skip note
//     S_msrNote
//       paddingNote =
//         createPaddingSkipNoteForVoice (
//           inputLineNumber,
//           missingDuration,
//           measureVoice);
//
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//       gLogStream <<
//        "Appending padding note " << paddingNote->asString () <<
//        " (" << missingDuration << " whole notes)" <<
//        " to finalize \"" << measureVoice->getVoiceName () <<
//        "\" measure: " <<
//        this->asShortString () <<
//        ", measureWholeNotesDuration: " <<
//        fMeasureWholeNotesDuration <<
//        std::endl;
//    }
// #endif
//
//     // append the paddingNote to the measure
//     appendNoteOrPaddingToMeasure (paddingNote);
//
//     // this measure contains music
//     fMeasureContainsMusic = true;
//   }
//
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
//     displayMeasure (
//       inputLineNumber,
//       "padUpToMeasurePosition() 2");
//   }
// #endif
//
//   --gIndenter;
// }

void msrMeasure::padUpToPositionAtTheEndOfTheMeasure (
  int             inputLineNumber,
  const Rational& measurePositionToPadUpTo)
{
  // fetch the voice
  S_msrVoice
    measureVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Padding up from position " <<
      fMeasureWholeNotesDuration <<
      " to position '" <<
      measurePositionToPadUpTo <<
      "' at the end of measure " <<
      this->asString () <<
      " in segment " <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionAtTheEndOfTheMeasure() 1");
  }
#endif

  ++gIndenter;

  if (fMeasureWholeNotesDuration < measurePositionToPadUpTo) {
    // appending a rest to this measure to reach measurePositionToPadUpTo
    Rational
      missingDuration =
        measurePositionToPadUpTo - fMeasureWholeNotesDuration;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
      gLogStream <<
        "Creating a padding note" <<
        ", missingDuration: " << missingDuration <<
        " at the end of measure " <<
        this->asString () <<
        " in voice \"" << measureVoice->getVoiceName () << "\",  " <<
        ", measureWholeNotesDuration: " <<
        fMeasureWholeNotesDuration <<
        ", line " << inputLineNumber <<
        std::endl;
   }
#endif

//     gLogStream << JMI
//       std::endl << std::endl <<
//       "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" <<
//       std::endl << std::endl;
//     this->
//       print (gLogStream);
//     gLogStream <<
//       std::endl << std::endl <<
//       "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" <<
//       std::endl << std::endl;

    // create a padding skip note
    S_msrNote
      paddingNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
      gLogStream <<
       "Appending padding note " << paddingNote->asString () <<
       " (" << missingDuration << " whole notes)" <<
       " to finalize \"" << measureVoice->getVoiceName () <<
       "\" measure: " <<
       this->asShortString () <<
       " measureWholeNotesDuration: " <<
       fMeasureWholeNotesDuration <<
       std::endl;
   }
#endif

    // append the rest to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord JMI
    appendPaddingNoteAtTheEndOfMeasure (paddingNote);

    // this measure contains music
    fMeasureContainsMusic = true;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionAtTheEndOfTheMeasure() 2");
  }
#endif

  --gIndenter;
}

void msrMeasure::backupByWholeNotesStepLengthInMeasure ( // JMI USELESS ??? v0.9.66
  int             inputLineNumber,
  const Rational& backupTargetMeasureElementMeasurePosition)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    backupTargetMeasureElementMeasurePosition.getNumerator () >= 0,
    "backupTargetMeasureElementMeasurePosition.getNumerator () is negative");

  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasurePositions ()
      ||
    gGlobalTracingOahGroup->getTraceWholeNotes ()
  ) {
    this->print (gLogStream);

    gLogStream <<
      "Backup by a '" <<
      backupTargetMeasureElementMeasurePosition <<
      "' whole notes step length in measure " <<
      this->asShortString () <<
      ", measureWholeNotesDuration: '" <<
      fMeasureWholeNotesDuration <<
      ", fullMeasureWholeNotesDuration: '" <<
      fFullMeasureWholeNotesDuration <<
      "' in segment " <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // determine the measure position 'backupTargetMeasureElementMeasurePosition' backward
  Rational
    measurePosition =
      fFullMeasureWholeNotesDuration - backupTargetMeasureElementMeasurePosition;

  // pad up to it
  padUpToMeasurePosition (
    inputLineNumber,
    measurePosition);
}

void msrMeasure::appendPaddingSkipNoteToMeasure (
  int             inputLineNumber,
  const Rational& forwardStepLength)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Appending padding skip note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to measure " <<
      this->asShortString () <<
      " in segment " <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  // create a padding skip note
  S_msrNote
    paddingNote =
      createPaddingSkipNoteForVoice (
        inputLineNumber,
        forwardStepLength,
        measureVoice);

  // append the paddingNote to the measure
  appendNoteOrPaddingToMeasure (paddingNote);

  --gIndenter;
}


void msrMeasure::appendTempoToMeasure (
  const S_msrTempo& tempo)
{
  appendMeasureElementToMeasure (tempo);
}

void msrMeasure::appendRehearsalMarkToMeasure (
  const S_msrRehearsalMark& rehearsalMark)
{
  appendMeasureElementToMeasure (rehearsalMark);
}

void msrMeasure::appendOctaveShiftToMeasure (
  const S_msrOctaveShift& octaveShift)
{
  appendMeasureElementToMeasure (octaveShift);
}

void msrMeasure::appendScordaturaToMeasure (
  const S_msrScordatura& scordatura)
{
  appendMeasureElementToMeasure (scordatura);
}

void msrMeasure::appendAccordionRegistrationToMeasure (
  const S_msrAccordionRegistration&
    accordionRegistration)
{
  appendMeasureElementToMeasure (accordionRegistration);
}

void msrMeasure::appendHarpPedalsTuningToMeasure (
  const S_msrHarpPedalsTuning&
    harpPedalsTuning)
{
  appendMeasureElementToMeasure (harpPedalsTuning);
}

void msrMeasure::appendLineBreakToMeasure (
  const  S_msrLineBreak& lineBreak)
{
  appendMeasureElementToMeasure (lineBreak);
}

void msrMeasure::appendPageBreakToMeasure (
  const S_msrPageBreak& pageBreak)
{
  appendMeasureElementToMeasure (pageBreak);
}

void msrMeasure::appendStaffDetailsToMeasure (
  const S_msrStaffDetails& staffDetails)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Appending staff details '" << staffDetails->asShortString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  appendMeasureElementToMeasure (staffDetails);
}

void msrMeasure::appendBarNumberCheckToMeasure (
  const S_msrBarNumberCheck& barNumberCheck)
{
  appendMeasureElementToMeasure (barNumberCheck);
}

void msrMeasure::prependOtherElementToMeasure (
  const S_msrMeasureElement& elem)
{
  fMeasureElementsList.push_front (elem); // JMI

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendOtherElementToMeasure  (
  const S_msrMeasureElement& elem)
{
  appendMeasureElementToMeasure (elem);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::removeNoteFromMeasure (
  int              inputLineNumber,
  const S_msrNote& note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"," <<
      "fMeasureLastHandledNote:" <<
      std::endl <<
      fMeasureLastHandledNote->asShortString () <<
      std::endl;
  }
#endif

  for (
    std::list<S_msrMeasureElement>::iterator i=fMeasureElementsList.begin ();
    i!=fMeasureElementsList.end ();
    ++i
  ) {
    if ((*i) == note) {
      // found note, erase it
      i = fMeasureElementsList.erase (i);

      // update measure whole notes
      setMeasureWholeNotesDuration (
        inputLineNumber,
        fMeasureWholeNotesDuration
          -
        fMeasureLastHandledNote->getMeasureElementSoundingWholeNotes ());

      // return from function
      return;
    }
  } // for

  S_msrVoice
    segmentUpLinkToVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  S_msrPart
    segmentVoicePart =
      fetchMeasureUpLinkToPart ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      std::endl << std::endl <<
      "@@@@@@@@@@@@@@@@@ segmentVoicePart" <<
      std::endl <<
      segmentVoicePart <<
      std::endl <<
      "@@@@@@@@@@@@@@@@@" <<
      std::endl << std::endl;

    gLogStream <<
      std::endl << std::endl <<
      "@@@@@@@@@@@@@@@@@ segmentUpLinkToVoice" <<
      std::endl <<
      segmentUpLinkToVoice <<
      std::endl <<
      "@@@@@@@@@@@@@@@@@" <<
      std::endl << std::endl;
  }
#endif

  std::stringstream s;

  s <<
    "cannot remove note " <<
    note <<
    " from measure " <<
    this->asShortString () <<
    " in voice \"" <<
    segmentUpLinkToVoice->getVoiceName () <<
    "\"," <<
    " since this note has not been found in fMeasureElementsList";

  msrInternalError (
    gGlobalServiceRunData->getInputSourceName (),
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

void msrMeasure::removeElementFromMeasure (
  int                 inputLineNumber,
  const S_msrElement& element)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Removing element:" <<
      std::endl;

    ++gIndenter;
    gLogStream <<
      " JMI ??? element->elementAsString ()" << std::endl;
    --gIndenter;

    gLogStream <<
      std::endl <<
      " from measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"," <<
      std::endl;

    ++gIndenter;
    gLogStream <<
      "fMeasureLastHandledNote:" <<
      std::endl <<
      fMeasureLastHandledNote <<
      std::endl;
    --gIndenter;
  }
#endif

  for (
    std::list<S_msrMeasureElement>::iterator i=fMeasureElementsList.begin ();
    i!=fMeasureElementsList.end ();
    ++i
  ) {
    if ((*i) == element) {
      // found element, erase it
      i = fMeasureElementsList.erase (i);

      // update measure whole notes
      setMeasureWholeNotesDuration (
        inputLineNumber,
        fMeasureWholeNotesDuration
          -
        fMeasureLastHandledNote->getMeasureElementSoundingWholeNotes ());

      // return from function
      return;
    }
  } // for

  std::stringstream s;

  s <<
    "cannot remove element " <<
    element->asString () <<
    " from measure " <<
    this->asShortString () <<
    " in voice \"" <<
    fMeasureUpLinkToSegment->
      getSegmentUpLinkToVoice ()->
        getVoiceName () <<
    "\"," <<
    " since this element has not been found in fMeasureElementsList";

  msrInternalError (
    gGlobalServiceRunData->getInputSourceName (),
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

void msrMeasure::appendNoteToMeasureNotesFlatList (
  const S_msrNote& note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Appending note " <<
      note->asShortStringForMeasuresSlices () <<
      " to the notes flat std::list of measure " <<
      this->asShortString () <<
      std::endl;
  }
#endif

  fMeasureNotesFlatList.push_back (note);
}

void msrMeasure::determineMeasureKindAndPuristNumber (
  int     inputLineNumber,
  msrMeasureRepeatContextKind
          measureRepeatContextKind)
{
  if (fMeasureKindAndPuristNumberHaveBeenDetermined) {
    std::stringstream s;

    s <<
      "Attempting to determine kind and purist number for measure \"" <<
      asShortString () <<
      "\" more than once";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());

/*
 // JMI   msrInternalError (
    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
  //    __FILE__, __LINE__,
      s.str ());

    return;
    */
  }

  // register measureRepeatContextKind
// JMI ???
  setMeasureRepeatContextKind (measureRepeatContextKind);

  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  // get voice after repeat component phase kind
  msrVoiceRepeatPhaseKind
    currentVoiceRepeatPhaseKind =
      voice->
        getCurrentVoiceRepeatPhaseKind ();

  // regular measure ends detection
  Rational
    wholeNotesSinceLastRegularMeasureEnd =
      voice->
        getWholeNotesSinceLastRegularMeasureEnd ();

  Rational
    newWholeNotesSinceLastRegularMeasureEnd =
      wholeNotesSinceLastRegularMeasureEnd
        +
      fMeasureWholeNotesDuration;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Determining the measure kind and purist number of measure " <<
      this->asShortString () <<
      ", fullMeasureWholeNotesDuration: " << fFullMeasureWholeNotesDuration <<
      "', measureWholeNotesDuration: " << fMeasureWholeNotesDuration <<
      ", wholeNotesSinceLastRegularMeasureEnd: " << wholeNotesSinceLastRegularMeasureEnd <<
      ", newWholeNotesSinceLastRegularMeasureEnd: " << newWholeNotesSinceLastRegularMeasureEnd <<
      ", currentVoiceRepeatPhaseKind: " << currentVoiceRepeatPhaseKind <<
    "' in voice \"" << voice->getVoiceName () <<
    ", line " << inputLineNumber <<
    std::endl;
  }

  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "determineMeasureKindAndPuristNumber() 1");
  }
#endif

  ++gIndenter;

  // set measure purist number
  setMeasurePuristNumber (
    voice->
      getVoiceCurrentMeasurePuristNumber ());

  // determine the measure kind
#ifdef TRACING_IS_ENABLED
    if (true || gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "--> determineMeasureKindAndPuristNumber()" <<
        "', fMeasureWholeNotesDuration: " <<
        fMeasureWholeNotesDuration <<
        ", fFullMeasureWholeNotesDuration: " <<
        fFullMeasureWholeNotesDuration <<
        ", fMeasureEndRegularKind: " <<
        fMeasureEndRegularKind <<
        std::endl;
    }
#endif

  if (fMeasureWholeNotesDuration.getNumerator () == 0) {
    // empty measure
      handleEmptyMeasure (
        inputLineNumber,
        voice);
 }

  else if (fMeasureWholeNotesDuration == fFullMeasureWholeNotesDuration) {
    // this is a regular measure
      handleRegularMeasure (
        inputLineNumber,
        voice);
  }

  else {
    // this measure is incomplete or overfull

    // increment voice whole notes since last regular measure end
    voice->
      setWholeNotesSinceLastRegularMeasureEnd (
        inputLineNumber,
        newWholeNotesSinceLastRegularMeasureEnd);

    // set measure kind
    if (fMeasureWholeNotesDuration < fFullMeasureWholeNotesDuration) {
      //  this is an incomplete measure
      handleIncompleteMeasure (
        inputLineNumber,
        voice,
        measureRepeatContextKind,
        newWholeNotesSinceLastRegularMeasureEnd);
    }

    else { // fMeasureWholeNotesDuration > fFullMeasureWholeNotesDuration
      // this is an overfull measure
      handleOverFullMeasure (
        inputLineNumber,
        voice);
    }
  }

  // forget about voice after repeat component phase kind if relevant
  switch (currentVoiceRepeatPhaseKind) {
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseNone:
      break;

    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart:
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHookedEnding:
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHooklessEnding:
      // reset incomplete measures after repeats detection
      voice->
        setCurrentVoiceRepeatPhaseKind (
          inputLineNumber,
          msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseNone);
      break;
  } // switch

  fMeasureKindAndPuristNumberHaveBeenDetermined = true;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "determineMeasureKindAndPuristNumber() 8");
  }
#endif

  --gIndenter;
}

void msrMeasure::handleEmptyMeasure (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef TRACING_IS_ENABLED
    if (true || gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "--> handleEmptyMeasure()" <<
        ", fMeasureEndRegularKind: " << fMeasureEndRegularKind <<
        std::endl;
    }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    voice->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "determineMeasureKindAndPuristNumber() 2 measure has 0 measure whole notes");

    std::stringstream s;

    s <<
      "measure " <<
      this->asShortString () <<
      " has 0 measure whole notes" <<
      ", " <<
      asString () <<
      ", line " << inputLineNumber;

    if (false) // JMI v0.9.66
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    else
      msrInternalWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
  }
#endif

  // set it's measure kind
  setMeasureKind (msrMeasureKind::kMeasureKindMusicallyEmpty);

  // increment voice's current measure purist number
  voice->
    incrementVoiceCurrentMeasurePuristNumber (
      inputLineNumber,
      "determineMeasureKindAndPuristNumber() 3 msrMeasureKind::kMeasureKindMusicallyEmpty");

  // reset voice whole notes since last regular measure end  // JMI ???
  voice->
    setWholeNotesSinceLastRegularMeasureEnd (
      inputLineNumber,
      0);
}

void msrMeasure::handleRegularMeasure (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef TRACING_IS_ENABLED
    if (true || gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "--> handleRegularMeasure()" <<
        ", fMeasureEndRegularKind: " << fMeasureEndRegularKind <<
        std::endl;
    }
#endif

  // this is a regular measure end
  setMeasureEndRegularKind (kMeasureEndRegularKindYes);

  // set it's measure kind
  setMeasureKind (msrMeasureKind::kMeasureKindRegular);

  // this is a regular measure end
  setMeasureEndRegularKind (kMeasureEndRegularKindYes);

  // increment voice's current measure purist number
  voice->
    incrementVoiceCurrentMeasurePuristNumber (
      inputLineNumber,
      "determineMeasureKindAndPuristNumber() 4 kMeasureEndRegularKindYes");

  // reset voice whole notes since last regular measure end
  voice->
    setWholeNotesSinceLastRegularMeasureEnd (
      inputLineNumber,
      0);
}

void msrMeasure::handleIncompleteMeasure (
  int               inputLineNumber,
  const S_msrVoice& voice,
  msrMeasureRepeatContextKind
                    measureRepeatContextKind,
  const Rational&   newWholeNotesSinceLastRegularMeasureEnd)
{
#ifdef TRACING_IS_ENABLED
    if (true || gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "--> handleIncompleteMeasure()" <<
        ", fMeasureEndRegularKind: " << fMeasureEndRegularKind <<
        std::endl;
    }
#endif

  if (fMeasureIsFirstInVoice) {
    // this is an anacrusis

    // set it's measure kind
    setMeasureKind (msrMeasureKind::kMeasureKindAnacrusis);

    // this is a regular measure end
    setMeasureEndRegularKind (kMeasureEndRegularKindYes);

    // fix measure purist number
    setMeasurePuristNumber (0);

    // voice current measure purist number remains at 1
  }

  else {
    // this is an incomplete measure within the voice

    if (
      newWholeNotesSinceLastRegularMeasureEnd == fFullMeasureWholeNotesDuration
    ) {
      // this is a regular measure end
      setMeasureEndRegularKind (kMeasureEndRegularKindYes);

      // increment voice's current measure purist number
      voice->
        incrementVoiceCurrentMeasurePuristNumber (
          inputLineNumber,
          "determineMeasureKindAndPuristNumber() 5 kMeasureEndRegularKindYes");

      // reset voice whole notes since last regular measure end
      voice->
        setWholeNotesSinceLastRegularMeasureEnd (
          inputLineNumber,
          0);
    }
    else {
      // this is no regular measure end
      setMeasureEndRegularKind (kMeasureEndRegularKindNo);
    }

    // set measure's kind according to measureRepeatContextKind
    switch (measureRepeatContextKind) {
      case msrMeasureRepeatContextKind::kMeasureRepeatContext_NO_: // JMI ???
        {
          std::stringstream s;

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
            displayMeasure (
              inputLineNumber,
              "determineMeasureKindAndPuristNumber() 6 kMeasureRepeatContext_NO_");
          }
#endif

          s <<
            "measure " <<
            this->asShortString () <<
            " is kMeasureRepeatContext_NO_ " <<
            asShortString () <<
            ", line " << inputLineNumber;

    // JMI      msrInternalError (
          msrInternalWarning (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
     //       __FILE__, __LINE__,
            s.str ());
        }

        // set it's measure kind
        setMeasureKind (
          msrMeasureKind::kMeasureKindIncompleteStandalone); // JMI
        break;

      case msrMeasureRepeatContextKind::kMeasureRepeatContextNone:
        // update the voice current measure purist number if relevant
        switch (fMeasureEndRegularKind) {
          case msrMeasureEndRegularKind::kMeasureEndRegularKindUnknown:
            // JMI ???
            break;

          case msrMeasureEndRegularKind::kMeasureEndRegularKindYes:
            // set it's measure kind
            setMeasureKind (
              msrMeasureKind::kMeasureKindIncompleteStandalone);

            // don't increment the voice current measure purist number,
            // this has already been done for the 'first part' of the measure
            break;

          case msrMeasureEndRegularKind::kMeasureEndRegularKindNo:
            // set it's measure kind
            setMeasureKind (
              msrMeasureKind::kMeasureKindIncompleteStandalone);
            break;
        } // switch
        break;

      case msrMeasureRepeatContextKind::kMeasureRepeatContextCommonPartLastMeasure:
        setMeasureKind (
          msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart);
        break;
      case msrMeasureRepeatContextKind::kMeasureRepeatContextHookedEndingLastMeasure:
        setMeasureKind (
          msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding);
        break;
      case msrMeasureRepeatContextKind::kMeasureRepeatContextHooklessEndingLastMeasure:
        setMeasureKind (
          msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding);
        break;

      case msrMeasureRepeatContextKind::kMeasureRepeatContextNextMeasureAfterCommonPart:
        setMeasureKind (
          msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart);
        break;
      case msrMeasureRepeatContextKind::kMeasureRepeatContextNextMeasureAfterHookedEnding:
        setMeasureKind (
          msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding);
        break;
      case msrMeasureRepeatContextKind::kMeasureRepeatContextNextMeasureAfterHooklessEnding:
        setMeasureKind (
          msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding);
        break;
    } // switch
  }
}

void msrMeasure::handleOverFullMeasure (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef TRACING_IS_ENABLED
    if (true || gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "--> handleOverFullMeasure()" <<
        ", fMeasureEndRegularKind: " << fMeasureEndRegularKind <<
        std::endl;
    }
#endif

  // set it's measure kind
  setMeasureKind (msrMeasureKind::kMeasureKindOvercomplete);

  // this is a regular measure end
  setMeasureEndRegularKind (kMeasureEndRegularKindYes);

  // increment voice's current measure purist number
  voice->
    incrementVoiceCurrentMeasurePuristNumber (
      inputLineNumber,
      "determineMeasureKindAndPuristNumber() 7 msrMeasureKind::kMeasureKindOvercomplete");

  // reset voice whole notes since last regular measure end
  voice->
    setWholeNotesSinceLastRegularMeasureEnd (
      inputLineNumber,
      0);
}

void msrMeasure::finalizeRegularMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  const std::string&     context)
{
  // fetch the regular voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  // fetch the regular voice's part
  S_msrPart
    regularPart =
      voice->
        fetchVoiceUpLinkToPart ();

  mfAssert (
    __FILE__, __LINE__,
    regularPart != nullptr,
    "regularPart is null");

  if (false) { // JMI
    gLogStream <<
      "---> regularPart: " <<
      std::endl;

    ++gIndenter;
    gLogStream <<
      regularPart <<
      std::endl;
    --gIndenter;
    gLogStream << std::endl;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing regular measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in regular voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", fMeasureOrdinalNumberInVoice: " <<
      fMeasureOrdinalNumberInVoice <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

//* JMI v0.9.66 USELESS ???
  Rational
    measureWholeNotesDurationFromPartMeasuresVector =
      regularPart->
        fetchPartMeasuresWholeNotesDurationsVectorAt (
          inputLineNumber,
          fMeasureOrdinalNumberInVoice - 1);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "===> measureWholeNotesDurationFromPartMeasuresVector: " <<
      measureWholeNotesDurationFromPartMeasuresVector <<
      std::endl;
  }
#endif
//*/

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeRegularMeasure() 1");
  }
#endif

//* JMI v0.9.66 USELESS ??? harmonies problem
  padUpToPositionAtTheEndOfTheMeasure (
    inputLineNumber,
    measureWholeNotesDurationFromPartMeasuresVector);
//*/

  // register this measures's whole notes duration in the part
  S_msrPart
    part =
      this->fetchMeasureUpLinkToPart ();

  part->
    registerOrdinalMeasureNumberWholeNotesDuration (
      inputLineNumber,
      fMeasureOrdinalNumberInVoice,
      fMeasureWholeNotesDuration);

  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    measureRepeatContextKind);

  // pad measure up to whole measure whole notes high tide JMI ???
  switch (fMeasureKind) {
    case msrMeasureKind::kMeasureKindCadenza:
      break;

    case msrMeasureKind::kMeasureKindOvercomplete:
    case msrMeasureKind::kMeasureKindAnacrusis:
    case msrMeasureKind::kMeasureKindRegular:
    case msrMeasureKind::kMeasureKindIncompleteStandalone: // JMI
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart: // JMI
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding: // JMI
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding: // JMI
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart: // JMI
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding: // JMI
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding: // JMI
      break;

    case msrMeasureKind::kMeasureKindUnknown:
      // JMI ???
      break;

    case msrMeasureKind::kMeasureKindMusicallyEmpty:
      {
      /* JMI
          */
      }
      break;
  } // switch

  // is there a single note or rest occupying the full measure?
  if (fMeasureLongestNote) {
    if (
      fMeasureLongestNote-> getMeasureElementSoundingWholeNotes ()
        ==
      fFullMeasureWholeNotesDuration
    ) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMeasures ()) {
        gLogStream <<
          "Note '" <<
          fMeasureLongestNote->asShortString () <<
          "' occupies measure " <<
          this->asShortString () <<
          " fully in segment '" <<
          fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
          "' in voice \"" <<
          voice->getVoiceName () <<
          "\", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fMeasureLongestNote->
        setNoteOccupiesAFullMeasure ();
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeRegularMeasure() 2");
  }
#endif

  --gIndenter;
}

void msrMeasure::handleFirstHarmonyInHarmoniesMeasure (
  int                 inputLineNumber,
  const S_msrVoice&   voice,
  const S_msrHarmony& currentHarmony)
{
  // currentHarmony is the first harmony in the measure

  // get currentHarmony's position in the measure
  Rational
    currentHarmonyMeasurePosition =
      currentHarmony->
        getMeasureElementMeasurePosition ();

  // get currentHarmony's note uplink
  S_msrNote
    currentUpLinkToHarmonyToNote  =
      currentHarmony->
        getHarmonyUpLinkToNote ();

  // set currentHarmony's sounding whole notes
  currentHarmony->
    setMeasureElementSoundingWholeNotes (
      currentUpLinkToHarmonyToNote->
        getMeasureElementSoundingWholeNotes (),
      "msrMeasure::handleFirstHarmonyInHarmoniesMeasure()");

  // get the currentHarmony's note uplink position in the measure
  Rational
    currentUpLinkToHarmonyToNoteMeasurePosition =
      currentUpLinkToHarmonyToNote->
        getMeasureElementMeasurePosition ();

  // the position to pad up to is the minimum
  // of those of the currentHarmony and currentUpLinkToHarmonyToNote,
  // to keep comparison points between the regular voice and its harmonies voice
  Rational
    measurePositionToPadUpTo =
      currentUpLinkToHarmonyToNoteMeasurePosition;
//      currentHarmonyMeasurePosition;

/* JMI
  if (
    currentHarmonyMeasurePosition
      <
    currentUpLinkToHarmonyToNoteMeasurePosition
  ) {
    measurePositionToPadUpTo =
      currentHarmonyMeasurePosition;
  }
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "--> handleFirstHarmonyInHarmoniesMeasure() 1" <<
      ", currentHarmonyMeasurePosition: " <<
      currentHarmonyMeasurePosition <<
      ", measurePositionToPadUpTo: " <<
      measurePositionToPadUpTo <<
      std::endl;
  }
#endif

  // is a padding skip note needed?
  if (measurePositionToPadUpTo.getNumerator () != 0) {
    // create a padding skip note
    S_msrNote
      skipNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          measurePositionToPadUpTo,
          voice);

    // insert skipNote before currentHarmony in the measure's elements list
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Inserting first padding note " <<
        skipNote->asString () <<
        " before currentHarmony " <<
        currentHarmony->asString () <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif

//     // insert skipNote in the measure elements list before (*i)
//     insertElementInMeasureBeforeIterator (
//       inputLineNumber,
//       i,
//       skipNote);

  // set current harmony's element measure position
//   currentHarmony->
//     setMeasureElementMeasurePosition (
//       this,
//       measurePositionToPadUpTo,
//       "first harmony in measure");

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
      displayMeasure (
        inputLineNumber,
        "handleFirstHarmonyInHarmoniesMeasure() 2");
    }
#endif
  }
}

void msrMeasure::handleSubsequentHarmonyInHarmoniesMeasure (
  int                 inputLineNumber,
  const S_msrVoice&   voice,
  const S_msrHarmony& previousHarmony,
  const S_msrHarmony& currentHarmony)
{
  // currentHarmony is a subsequent harmony in the measure,
  // occurring after previousHarmony

  // get currentHarmony' position in the measure
  Rational
    currentHarmonyMeasurePosition =
      currentHarmony->
        getMeasureElementMeasurePosition ();

  // get previousHarmony's position in the measure
  Rational
    previousHarmonyMeasurePosition =
      previousHarmony->getMeasureElementMeasurePosition ();

  // get previousHarmony's duration
  Rational
    previousHarmonySoundingWholeNotes =
      previousHarmony->getMeasureElementSoundingWholeNotes ();

  // compute the measure position following previousHarmony
  Rational
    measurePositionFollowingPreviousHarmony =
      previousHarmonyMeasurePosition
        +
      previousHarmonySoundingWholeNotes;

  // compute the measure positions delta
  // between previousHarmony and currentHarmony
  Rational
    measurePositionsDelta =
      currentHarmonyMeasurePosition
        -
      measurePositionFollowingPreviousHarmony;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "---> handleSubsequentHarmonyInHarmoniesMeasure() 1" <<
      ", previousHarmony: ";

    if (previousHarmony) {
      gLogStream <<
        previousHarmony->asString ();
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      ", currentHarmony: " <<
      currentHarmony->asString () <<
      ", previousHarmonyMeasurePosition: " <<
      previousHarmonyMeasurePosition <<
      ", currentHarmonyMeasurePosition: " <<
      currentHarmonyMeasurePosition <<
      ", measurePositionFollowingPreviousHarmony: " <<
      measurePositionFollowingPreviousHarmony <<
      ", measurePositionsDelta: " <<
      measurePositionsDelta <<
      std::endl;
  }
#endif

  // update the previous harmony sounding whole notes duration if relevant
  // to 'fill the gap' to the current harmony
  if (previousHarmony) {
    Rational
      newPreviousHarmonyWholeNotes =
        currentHarmony->getMeasureElementMeasurePosition ()
          -
        previousHarmony->getMeasureElementMeasurePosition ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Setting the sounding whole notes duration of harmony " <<
        previousHarmony->asString () <<
        " to " <<
        newPreviousHarmonyWholeNotes <<
        std::endl;
    }
#endif

    previousHarmony->
      setMeasureElementSoundingWholeNotes (
        newPreviousHarmonyWholeNotes,
        "msrMeasure::handleSubsequentHarmonyInHarmoniesMeasure()");
  }
}

void msrMeasure::postHandleCurrentHarmonyInHarmoniesMeasure (
  int                 inputLineNumber,
  const S_msrVoice&   voice,
  const S_msrHarmony& currentHarmony)
{
  // does currentHarmony overflow the measure?

  // get the currentHarmony's position in the measure
  Rational
    currentHarmonyMeasurePosition =
      currentHarmony->
        getMeasureElementMeasurePosition ();

  // get the currentHarmony's sounding whole notes
  Rational
    currentHarmonySoundingWholeNotes =
      currentHarmony->
        getMeasureElementSoundingWholeNotes ();

  // compute the measure position following currentHarmony
  Rational
    measurePositionFollowingCurrentHarmony =
      currentHarmonyMeasurePosition
        +
      currentHarmonySoundingWholeNotes;

  // get the currentHarmony's note uplink
  S_msrNote
    currentUpLinkToHarmonyToNote  =
      currentHarmony->
        getHarmonyUpLinkToNote ();

  // compute the measure overflow whole notes
  Rational
    measureOverflowWholeNotes =
      measurePositionFollowingCurrentHarmony
        -
      fFullMeasureWholeNotesDuration;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "--> postHandleCurrentHarmonyInHarmoniesMeasure():" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      "currentHarmony: ";

    if (currentHarmony) {
      gLogStream <<
        currentHarmony->asString ();
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      std::endl <<
      "currentUpLinkToHarmonyToNote:" <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      currentUpLinkToHarmonyToNote->asString () <<
      std::endl;
    --gIndenter;

    gLogStream <<
      ", currentHarmonyMeasurePosition: " <<
      currentHarmonyMeasurePosition <<
      ", currentHarmonySoundingWholeNotes: " <<
      currentHarmonySoundingWholeNotes <<
      ", measurePositionFollowingCurrentHarmony: " <<
      measurePositionFollowingCurrentHarmony <<
      ", fFullMeasureWholeNotesDuration: " <<
      fFullMeasureWholeNotesDuration <<
      /* JMI
      ", measurePositionFollowingCurrentUpLinkToHarmonyToNote: " <<
      measurePositionFollowingCurrentUpLinkToHarmonyToNote <<
      ", currentUpLinkToHarmonyToNoteSoundingWholeNotes: " <<
      currentUpLinkToHarmonyToNoteSoundingWholeNotes <<
      ", measurePositionFollowingCurrentUpLinkToHarmonyToNote: " <<
      measurePositionFollowingCurrentUpLinkToHarmonyToNote <<
      */
      ", currentHarmonySoundingWholeNotes: " <<
      currentHarmonySoundingWholeNotes <<
      ", measureOverflowWholeNotes: " <<
      measureOverflowWholeNotes <<
      std::endl;

    --gIndenter;
  }
#endif

  if (measureOverflowWholeNotes.getNumerator () < 0) {
    // the currentHarmony's duration is too short,
    // i.e. there are notes after the one that bears it
    // or it is the last one in the voice v0.9.66 ??? JMI

    if (
      fMeasureNumber
        !=
      fetchMeasureUpLinkToScore ()->getScoreLastMeasureNumber ()
    ) {
      // currentHarmony is not the last one in the score
      // increment its duration to fill the measure
      Rational
        wholeNotesDelta =
          Rational (0) - measureOverflowWholeNotes; // JMI SAXO v0.9.66

      // increment currentHarmony's sounding whole notes till the end of the measure
      currentHarmony->
        incrementHarmonySoundingWholeNotesDuration (
          inputLineNumber,
          wholeNotesDelta);

      // take wholeNotesDelta into account in measure whole notes duration
  #ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
        gLogStream <<
          "Incrementing the sounding whole notes of measure " <<
          this->asShortString () <<
          " by " <<
          wholeNotesDelta <<
          " in voice \"" <<
          voice->getVoiceName () <<
          "\", line " << inputLineNumber <<
          std::endl;
      }
  #endif

      incrementMeasureWholeNotesDuration ( // JMI v0.9.66
        inputLineNumber,
        wholeNotesDelta);
    }
  }

  else if (measureOverflowWholeNotes.getNumerator () > 0) {
    // the currentHarmony's duration is too large
    std::stringstream s;

    s <<
      "The current harmony " <<
      currentHarmony->asString () <<
      " overflows the current measure " <<
      asString ();

    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());

    // compute currentHarmony's future sounding whole notes
    Rational
      reducedSoundingWholeNotes =
        currentHarmonySoundingWholeNotes
          - // the delta is positive
        measureOverflowWholeNotes;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Reducing the sounding whole notes of harmony FII " <<
        currentHarmony->asString () <<
        " from " <<
        currentHarmonySoundingWholeNotes <<
        " to " <<
        reducedSoundingWholeNotes <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif

    if (reducedSoundingWholeNotes.getNumerator () == 0) {
      std::stringstream s;

      s <<
        "Cannot reduce the duration of harmony " <<
        currentHarmony->asShortString () <<
        " to 0 : leaving it as it is";

      msrInternalWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
//  JMI             __FILE__, __LINE__,
        s.str ());
    }
    else {
      // set currentHarmony's duration to the reduced value
      currentHarmony->
        setMeasureElementSoundingWholeNotes (
          reducedSoundingWholeNotes,
          "msrMeasure::postHandleCurrentHarmonyInHarmoniesMeasure()");
    }
  }

  else {
    // the currentHarmony reaches the end of the measure,
    // i.e. there are not notes after the one that bears it
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Harmony " <<
        currentHarmony->asString () <<
        " reaches the end of measure " <<
        this->asShortString () <<
        ", nothing more to do" <<
        ", in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif
  }
}

void msrMeasure::finalizeTheHarmoniesInHarmoniesMeasure (
  int                inputLineNumber,
  const std::string& context)
{
  // running this method for each and every measure in turn
  // in harmonies voices is actually a partial pass

  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Finalizing harmonies in harmonies measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fMeasureElementsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeTheHarmoniesInHarmoniesMeasure() 1");
    }
#endif

    // in MusicXML, harmonies don't have their own duration
    // and may have a non-null offset:
    // we thus have to sort them in the measure by increasing position,
    // taking their offset into account
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Sorting the elements in harmonies measure " <<
        this->asShortString () <<
        " in segment '" <<
        fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
        "' in voice \"" <<
        voice->getVoiceName () <<
        "\" (" << context << ")" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    fMeasureElementsList.sort (
      msrMeasureElement::
        compareMeasureElementsByIncreasingMeasurePosition);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeTheHarmoniesInHarmoniesMeasure() 2");
    }
#endif

    // consider each measure element (a harmony in fact) in turn,
    // updating their duration and adding skips if needed
    S_msrHarmony
      previousHarmony = nullptr,
      currentHarmony  = nullptr;

    for (S_msrMeasureElement measureElement : fMeasureElementsList) {
      int inputLineNumber =
        measureElement->
          getInputLineNumber ();

      if (
        // harmony?
        // don't assign currentHarmony here yet,
        // this would set it to nullptr if there's anything else
        // after the last harmony in the voice JMI ??? v0.9.66
        S_msrHarmony
          harmony =
            dynamic_cast<msrHarmony*>(&(*measureElement))
      ) {
        currentHarmony = harmony;

        // handle currentHarmony
        finalizeHarmonyInHarmoniesMeasure (
          inputLineNumber,
          voice,
          previousHarmony,
          currentHarmony,
          context);

        previousHarmony = currentHarmony;
      }

      else if (
        // note? JMI
        S_msrNote
          note =
            dynamic_cast<msrNote*>(&(*measureElement))
      ) {
        std::stringstream s;

        s <<
          "Finalizing a harmony voice measure containing note " <<
          note->asShortString () <<
          std::endl;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    } // for

    // handle the last harmony in the measure if any
    if (currentHarmony) {
      postHandleCurrentHarmonyInHarmoniesMeasure (
        inputLineNumber,
        voice,
        currentHarmony);
    }
/* JMI
    else {
      gLogStream <<
        "*** currentHarmony is null ***" << // JMI
        std::endl;
    }
    */

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeTheHarmoniesInHarmoniesMeasure() 3");
    }
#endif
  }
}

void msrMeasure::finalizeHarmonyInHarmoniesMeasure (
  int                 inputLineNumber,
  const S_msrVoice&   voice,
  const S_msrHarmony& previousHarmony,
  const S_msrHarmony& currentHarmony,
  const std::string&  context)
{
  // handle the currentHarmony
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Finalizing harmony " <<
      currentHarmony->asString () <<
      " while finalizing harmonies measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // get currentHarmony's note uplink
  S_msrNote
    currentUpLinkToHarmonyToNote =
      currentHarmony->
        getHarmonyUpLinkToNote (); // JMI v0.9.66

  // set currentHarmony's sounding whole notes
  currentHarmony->
    setMeasureElementSoundingWholeNotes (
      currentUpLinkToHarmonyToNote->
        getMeasureElementSoundingWholeNotes (),
      "msrMeasure::finalizeHarmonyInHarmoniesMeasure()");

#ifdef TRACING_IS_ENABLED
  // get the currentHarmony's note uplink position in the measure
  Rational
    currentUpLinkToHarmonyToNoteMeasurePosition =
      currentUpLinkToHarmonyToNote->
        getMeasureElementMeasurePosition ();

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "--> finalizeHarmonyInHarmoniesMeasure() 2" <<
      ", previousHarmony: ";

    if (previousHarmony) {
      gLogStream <<
        previousHarmony->asString ();
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      ", currentUpLinkToHarmonyToNoteMeasurePosition: " <<
      currentUpLinkToHarmonyToNoteMeasurePosition <<
      std::endl;
  }
#endif

  if (! previousHarmony) {
    handleFirstHarmonyInHarmoniesMeasure (
      inputLineNumber,
      voice,
      currentHarmony);
  }

  else {
    handleSubsequentHarmonyInHarmoniesMeasure (
      inputLineNumber,
      voice,
      previousHarmony,
      currentHarmony);
  }
}

void msrMeasure::handleFirstFiguredBassInFiguredBassMeasure (
  int                     inputLineNumber,
  const S_msrVoice&       voice,
  std::list<S_msrMeasureElement>::iterator&
                          i,
  const S_msrFiguredBass& previousFiguredBass,
  const S_msrFiguredBass& currentFiguredBass,
  const Rational&         currentFiguredBassMeasurePosition)
{
  // currentFiguredBass is the first figured bass in the measure

  // the position to pad up to is the minimum
  // of those of the currentFiguredBass and currentFiguredBassUpLinkToNote,
  // to keep comparison points between the regular voice and its figured bass voice
  Rational
    measurePositionToPadUpTo =
// JMI        currentFiguredBassUpLinkToNoteMeasurePosition;
      currentFiguredBassMeasurePosition;

/* JMI
  if (
    currentFiguredBassMeasurePosition
      <
    currentFiguredBassUpLinkToNoteMeasurePosition
  ) {
    measurePositionToPadUpTo =
      currentFiguredBassMeasurePosition;
  }
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "handleFirstFiguredBassInFiguredBassMeasure() 5" <<
      ", previousFiguredBass is null, measurePositionToPadUpTo: " <<
      measurePositionToPadUpTo <<
      std::endl;
  }
#endif

  // is a padding skip note needed?
  if (measurePositionToPadUpTo.getNumerator () != 0) {
    // create a padding skip note
    S_msrNote
      skipNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          measurePositionToPadUpTo,
          voice);

    // insert skipNote before currentFiguredBass in the measure's elements list
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Inserting first padding note " <<
        skipNote->asString () <<
        " before currentFiguredBass " <<
        currentFiguredBass->asString () <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif

    // insert skipNote in the measure elements list before (*i)
    insertElementInMeasureBeforeIterator (
      inputLineNumber,
      i,
      skipNote);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "handleFirstFiguredBassInFiguredBassMeasure() 6");
    }
#endif
  }
}

void msrMeasure::handleSubsequentFiguredBassInFiguredBassMeasure (
  int                     inputLineNumber,
  const S_msrVoice&       voice,
  std::list<S_msrMeasureElement>::iterator&
                          i,
  const S_msrFiguredBass& previousFiguredBass,
  const S_msrFiguredBass& currentFiguredBass,
  const Rational&         currentFiguredBassMeasurePosition)
{
  // this is a subsequent figured bass in the measure

  // get the previousFiguredBass's position in the measure
  Rational
    previousFiguredBassMeasurePosition =
      previousFiguredBass->getMeasureElementMeasurePosition ();

  // get the previousFiguredBass's duration
  Rational
    previousFiguredBassSoundingWholeNotes =
      previousFiguredBass->getMeasureElementSoundingWholeNotes ();

  // compute the measure position following previousFiguredBass
  Rational
    measurePositionFollowingPreviousFiguredBass =
      previousFiguredBassMeasurePosition
        +
      previousFiguredBassSoundingWholeNotes;

  // compute the measure positions delta
  Rational
    measurePositionsDelta =
      currentFiguredBassMeasurePosition
        -
      measurePositionFollowingPreviousFiguredBass;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "handleSubsequentFiguredBassInFiguredBassMeasure() 7" <<
      ", previousFiguredBass: ";

    if (previousFiguredBass) {
      gLogStream <<
        previousFiguredBass->asString ();
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      ", currentFiguredBass: " <<
      currentFiguredBass->asString () <<
      ", previousFiguredBassMeasurePosition: " <<
      previousFiguredBassMeasurePosition <<
      ", currentFiguredBassMeasurePosition: " <<
      currentFiguredBassMeasurePosition <<
      ", measurePositionFollowingPreviousFiguredBass: " <<
      measurePositionFollowingPreviousFiguredBass <<
      ", measurePositionsDelta: " <<
      measurePositionsDelta <<
      std::endl;
  }
#endif

  // is a padding skip note needed?
  if (measurePositionsDelta.getNumerator () > 0) {
    // create a padding skip note
    S_msrNote
      skipNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          measurePositionsDelta,
          voice);

    // set its measure position
//     skipNote->
//       setMeasureElementMeasurePosition (
//         this,
//         fMeasureWholeNotesDuration,
//         "handleSubsequentFiguredBassInFiguredBassMeasure() 8");

    // insert skipNote before currentFiguredBass in the measure's elements list
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Inserting subsequent padding note " <<
        skipNote->asString () <<
        " before currentFiguredBass " <<
        currentFiguredBass->asString () <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif

    // insert skipNote in the measure elements list before (*i)
    insertElementInMeasureBeforeIterator (
      inputLineNumber,
      i,
      skipNote);
  }

  else if (measurePositionsDelta.getNumerator () < 0) {
    // the two harmonies overlap in time
    std::stringstream s;

    s <<
      "Previous figured bass " <<
      previousFiguredBass->asString () <<
      " overlaps current figured bass " <<
      currentFiguredBass->asString ();

    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());

    // compute previousFiguredBass's future sounding whole notes
    Rational
      reducedSoundingWholeNotes =
        previousFiguredBassSoundingWholeNotes
          + // the delta is negative
        measurePositionsDelta;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Reducing the sounding whole notes of figured bass FOO " <<
        previousFiguredBass->asString () <<
        " from " <<
        previousFiguredBassSoundingWholeNotes <<
        " to " <<
        reducedSoundingWholeNotes <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif

    if (reducedSoundingWholeNotes.getNumerator () == 0) {
      std::stringstream s;

      s <<
        "Cannot reduce the duration of figured bass " <<
        previousFiguredBass->asShortString () <<
        " to 0 : leaving it as it is";

      msrInternalWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
//  JMI             __FILE__, __LINE__,
        s.str ());
    }
    else {
      // set previousFiguredBass's duration to the reduced value
      previousFiguredBass->
        setMeasureElementSoundingWholeNotes (
          reducedSoundingWholeNotes,
          "msrMeasure::handleSubsequentFiguredBassInFiguredBassMeasure ()");
    }
  }
}

void msrMeasure::postHandleCurrentFiguredBassInFiguredBassMeasure (
  int                     inputLineNumber,
  const S_msrVoice&       voice,
  const S_msrFiguredBass& currentFiguredBass)
{
  // does currentFiguredBass overflow the measure?

  // get the currentFiguredBass's position in the measure
  Rational
    currentFiguredBassMeasurePosition =
      currentFiguredBass->getMeasureElementMeasurePosition ();

  // get the currentFiguredBass's sounding whole notes
  Rational
    currentFiguredBassSoundingWholeNotes =
      currentFiguredBass->
        getMeasureElementSoundingWholeNotes ();

  // compute the measure position following currentFiguredBass
  Rational
    measurePositionFollowingCurrentFiguredBass =
      currentFiguredBassMeasurePosition
        +
      currentFiguredBassSoundingWholeNotes;

  // get the currentFiguredBass's note uplink
  S_msrNote
    currentFiguredBassUpLinkToNote  =
      currentFiguredBass->
        getFiguredBassUpLinkToNote ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "postHandleCurrentFiguredBassInFiguredBassMeasure() 1" <<
      ", currentFiguredBassUpLinkToNote:" <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      currentFiguredBassUpLinkToNote <<
      std::endl;
    --gIndenter;
  }
#endif

  // compute the measure overflow whole notes
  Rational
    measureOverflowWholeNotes =
      measurePositionFollowingCurrentFiguredBass
        -
      fFullMeasureWholeNotesDuration;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "postHandleCurrentFiguredBassInFiguredBassMeasure() 2" <<
      ", currentFiguredBass: ";

    if (currentFiguredBass) {
      gLogStream <<
        currentFiguredBass->asString ();
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      ", currentFiguredBassMeasurePosition: " <<
      currentFiguredBassMeasurePosition <<
      ", currentFiguredBassSoundingWholeNotes: " <<
      currentFiguredBassSoundingWholeNotes <<
      ", measurePositionFollowingCurrentFiguredBass: " <<
      measurePositionFollowingCurrentFiguredBass <<
      /* JMI
      ", measurePositionFollowingCurrentFiguredBassUpLinkToNote: " <<
      measurePositionFollowingCurrentFiguredBassUpLinkToNote <<
      ", currentFiguredBassUpLinkToNoteSoundingWholeNotes: " <<
      currentFiguredBassUpLinkToNoteSoundingWholeNotes <<
      ", measurePositionFollowingCurrentFiguredBassUpLinkToNote: " <<
      measurePositionFollowingCurrentFiguredBassUpLinkToNote <<
      */
      ", currentFiguredBassSoundingWholeNotes: " <<
      currentFiguredBassSoundingWholeNotes <<
      ", measureOverflowWholeNotes: " <<
      measureOverflowWholeNotes <<
      std::endl;
  }
#endif

  if (measureOverflowWholeNotes.getNumerator () > 0) {
    // the currentFiguredBass's duration is too large
    std::stringstream s;

    s <<
      "The current figured bass " <<
      currentFiguredBass->asString () <<
      " overflows the current measure " <<
      asString ();

    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());

    // compute currentFiguredBass's future sounding whole notes
    Rational
      reducedSoundingWholeNotes =
        currentFiguredBassSoundingWholeNotes
          - // the delta is positive
        measureOverflowWholeNotes;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Reducing the sounding whole notes of figured bass FII " <<
        currentFiguredBass->asString () <<
        " from " <<
        currentFiguredBassSoundingWholeNotes <<
        " to " <<
        reducedSoundingWholeNotes <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif

    if (reducedSoundingWholeNotes.getNumerator () == 0) {
      std::stringstream s;

      s <<
        "Cannot reduce the duration of figured bass " <<
        currentFiguredBass->asShortString () <<
        " to 0 : leaving it as it is";

      msrInternalWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
//  JMI             __FILE__, __LINE__,
        s.str ());
    }
    else {
      // set currentFiguredBass's duration to the reduced value
      currentFiguredBass->
        setMeasureElementSoundingWholeNotes (
          reducedSoundingWholeNotes,
          "msrMeasure::postHandleCurrentFiguredBassInFiguredBassMeasure () 3");
    }
  }
}

void msrMeasure::finalizeFiguredBassesInFiguredBassMeasure (
  int                inputLineNumber,
  const std::string& context)
{
  // running this method for each and every measure in turn
  // in figured bass voices is actually a partial pass

  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Finalizing harmonies in figured bass measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fMeasureElementsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeFiguredBassesInFiguredBassMeasure() 1");
    }
#endif

    // in MusicXML, harmonies don't have their own duration
    // and may have a non-null offset:
    // we thus have to sort them in the measure by increasing position,
    // taking their offset into account
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Sorting the elements in figured bass measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

    fMeasureElementsList.sort (
      msrMeasureElement::compareMeasureElementsByIncreasingMeasurePosition);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeFiguredBassesInFiguredBassMeasure() 2");
    }
#endif

    // consider each measure element (a figured bass in fact) in turn,
    // updating their duration and adding skips if needed
    std::list<S_msrMeasureElement>::iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;

    S_msrFiguredBass
      previousFiguredBass = nullptr,
      currentFiguredBass  = nullptr;

    while (true) {
      S_msrMeasureElement
        measureElement = (*i);

      int inputLineNumber =
        measureElement->
          getInputLineNumber ();

      if (
        // figured bass?
        // don't assign currentFiguredBass here yet,
        // this would set it to nullptr if there's anything else
        // after the last figured bass in the voice
        S_msrFiguredBass
          figuredBass =
            dynamic_cast<msrFiguredBass*>(&(*measureElement))
      ) {
        currentFiguredBass = figuredBass;

        // handle the currentFiguredBass
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
          gLogStream <<
            "finalizeFiguredBassesInFiguredBassMeasure() 3" <<
            ", currentFiguredBass: ";
            ++gIndenter;
            gLogStream <<
              currentFiguredBass->asString () <<
              std::endl;
            --gIndenter;
        }
#endif

        // its position in the measure should take it's offset into account
        Rational
          currentFiguredBassMeasurePosition =
            currentFiguredBass->
              getMeasureElementMeasurePosition ();

        // get the currentFiguredBass's note uplink
        S_msrNote
          currentFiguredBassUpLinkToNote  =
            currentFiguredBass->
              getFiguredBassUpLinkToNote ();

#ifdef TRACING_IS_ENABLED
        // get the currentFiguredBass's note uplink position in the measure
        Rational
          currentFiguredBassUpLinkToNoteMeasurePosition =
            currentFiguredBassUpLinkToNote->
              getMeasureElementMeasurePosition ();

        if (gGlobalTracingOahGroup->getTraceHarmonies () ) {
          gLogStream <<
            "finalizeFiguredBassesInFiguredBassMeasure() 4" <<
            ", previousFiguredBass: ";

          if (previousFiguredBass) {
            gLogStream <<
              previousFiguredBass->asString ();
          }
          else {
            gLogStream << "[NONE]";
          }

          gLogStream <<
            ", currentFiguredBassMeasurePosition: " <<
            currentFiguredBassMeasurePosition <<
            ", currentFiguredBassUpLinkToNoteMeasurePosition: " <<
            currentFiguredBassUpLinkToNoteMeasurePosition <<
            std::endl;
        }
#endif

        if (! previousFiguredBass) {
          handleFirstFiguredBassInFiguredBassMeasure (
            inputLineNumber,
            voice,
            i,
            previousFiguredBass,
            currentFiguredBass,
            currentFiguredBassMeasurePosition);
        }

        else {
          handleSubsequentFiguredBassInFiguredBassMeasure (
            inputLineNumber,
            voice,
            i,
            previousFiguredBass,
            currentFiguredBass,
            currentFiguredBassMeasurePosition);
        }

        previousFiguredBass = currentFiguredBass;
      }

      if (++i == iEnd) break;
    } // while

    if (currentFiguredBass) {
      postHandleCurrentFiguredBassInFiguredBassMeasure (
        inputLineNumber,
        voice,
        currentFiguredBass);
    }
/* JMI
    else {
      gLogStream <<
        "*** currentFiguredBass is null ***" << // JMI
        std::endl;
    }
    */

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeFiguredBassesInFiguredBassMeasure() 5");
    }
#endif
  }
}

void msrMeasure::finalizeHarmonyMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  const std::string&          context)
{
  // fetch the harmonies voice
  S_msrVoice
    harmoniesVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  // fetch the harmonies part
  S_msrPart
    harmoniesPart =
      harmoniesVoice->
        fetchVoiceUpLinkToPart ();

  mfAssert (
    __FILE__, __LINE__,
    harmoniesPart != nullptr,
    "harmoniesPart is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Finalizing harmonies measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in harmonies voice \"" <<
      harmoniesVoice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeHarmonyMeasure() 1");
  }
#endif

#ifdef TRACING_IS_ENABLED
  // get the harmoniesPart number of measures
  int
    harmoniesPartNumberOfMeasures =
      harmoniesPart->
        getPartNumberOfMeasures ();

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "--> fMeasureOrdinalNumberInVoice: " <<
      fMeasureOrdinalNumberInVoice <<
      ", harmoniesPartNumberOfMeasures: " <<
      harmoniesPartNumberOfMeasures <<
      std::endl;
  }
#endif

  // handle the harmonies in this measure
  finalizeTheHarmoniesInHarmoniesMeasure (
    inputLineNumber,
    context);

  // the measureWholeNotesDuration has to be computed
  // only now because finalizeTheHarmoniesInHarmoniesMeasure()
  // may have incremented a harmony sounding whole notes duration
  Rational
    measureWholeNotesDurationFromPartMeasuresVector = // USELESS ??? JMI v0.9.66
      harmoniesPart->
        fetchPartMeasuresWholeNotesDurationsVectorAt (
          inputLineNumber,
          fMeasureOrdinalNumberInVoice - 1);

  // DO NOT pad the measure up to measureWholeNotesDurationFromPartMeasuresVector,
  // since the last harmony in the measure have been extended to the end of the measure
  padUpToPositionAtTheEndOfTheMeasure ( // JMI ??? v0.9.66
    inputLineNumber,
    measureWholeNotesDurationFromPartMeasuresVector);

  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    measureRepeatContextKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeHarmonyMeasure() 2");
  }
#endif

  --gIndenter;
}

void msrMeasure::finalizeFiguredBassMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  const std::string&          context)
{
  // fetch the figured bass voice
  S_msrVoice
    figuredBassVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  // fetch the figured bass part
  S_msrPart
    figuredBassPart =
      figuredBassVoice->
        fetchVoiceUpLinkToPart ();

  mfAssert (
    __FILE__, __LINE__,
    figuredBassPart != nullptr,
    "figuredBassPart is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Finalizing figured bass measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in figured bass voice \"" <<
      figuredBassVoice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBassDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeFiguredBassMeasure() 1");
  }
#endif

#ifdef TRACING_IS_ENABLED
  // get the figuredBassPart number of measures
  int
    figuredBassPartNumberOfMeasures =
      figuredBassPart->
        getPartNumberOfMeasures ();

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "fMeasureOrdinalNumberInVoice: " <<
      fMeasureOrdinalNumberInVoice <<
      ", figuredBassPartNumberOfMeasures = " <<
      figuredBassPartNumberOfMeasures <<
      std::endl;
  }
#endif

  // handle the figured bass element in this measure
  finalizeFiguredBassesInFiguredBassMeasure (
    inputLineNumber,
    context);

  // the measureWholeNotesDuration has to be computed
  // only now because finalizeTheHarmoniesInHarmoniesMeasure()
  // may have incremented a harmony sounding whole notes duration
  Rational
    measureWholeNotesDurationFromPartMeasuresVector = // USELESS ??? JMI v0.9.66
      figuredBassPart->
        fetchPartMeasuresWholeNotesDurationsVectorAt (
          inputLineNumber,
          fMeasureOrdinalNumberInVoice - 1);

  // pad the measure up to fFullMeasureWholeNotesDuration // JMI ??? v0.9.66
  padUpToPositionAtTheEndOfTheMeasure (
    inputLineNumber,
    measureWholeNotesDurationFromPartMeasuresVector);

  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    measureRepeatContextKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBassDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeFiguredBassMeasure() 2");
  }
#endif

  --gIndenter;
}

void msrMeasure::finalizeMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  const std::string&     context)
{
  if (fMeasureHasBeenFinalized) {
    std::stringstream s;

    s <<
      "Attempting to finalize measure " <<
      this->asShortString () <<
      " more than once in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "', context: " << context <<
      "', measureFinalizationContext: " << fMeasureFinalizationContext <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
      gLogStream <<
        std::endl <<
        s.str () <<
        std::endl << std::endl;

      S_msrStaff
        staff =
          fetchMeasureUpLinkToStaff ();

      gLogStream <<
        "staff:" <<
        std::endl;
      ++gIndenter;
      staff->printShort (gLogStream);
      gLogStream << std::endl;
      --gIndenter;
    }
#endif

    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      s.str ());
  }

  else {
    S_msrVoice
      voice =
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Finalizing measure " <<
        this->asString () <<
        " in segment '" <<
        fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
        "' in voice \"" <<
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\" (" << context << ")" <<
        ", lines " << inputLineNumber << " .. " << fMeasureEndInputLineNumber << // JMI
        std::endl;
    }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    if (fMeasureWholeNotesDuration.getNumerator () == 0) {
      std::stringstream s;

      s <<
        "Measure " <<
        this->asShortString () <<
        " in segment '" <<
        fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
        "' in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        ", doesn't contain any music" <<
        std::endl;

      msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureClone() 1");
  }
#endif

    // delegate finalization to voice kind specific methods
    switch (voice->getVoiceKind ()) {
      case msrVoiceKind::kVoiceKindRegular:
        finalizeRegularMeasure (
          inputLineNumber,
          measureRepeatContextKind,
          context);
        break;

      case msrVoiceKind::kVoiceKindDynamics:
        break;

      case msrVoiceKind::kVoiceKindHarmonies:
        finalizeHarmonyMeasure (
          inputLineNumber,
          measureRepeatContextKind,
          context);
        break;

      case msrVoiceKind::kVoiceKindFiguredBass:
        finalizeFiguredBassMeasure (
          inputLineNumber,
          measureRepeatContextKind,
          context);
        break;
    } // switch

    // voice position
    Rational
      voicePosition =
        fetchMeasureUpLinkToVoice ()->
          getCurrentVoicePosition ();

    // delegate position measure assignment to the elements in the measure
    for (
      std::list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
      i != fMeasureElementsList.end ();
      ++i
    ) {
      S_msrMeasureElement measureElement = (*i);

//       measureElement->
//         setMeasureElementVoicePosition ( // JMI v0.9.66
//           voicePosition,
//           "finalizeMeasure()");
    } // for

    // register finalization
    fMeasureHasBeenFinalized = true;
    fMeasureFinalizationContext = context;
  }
}

void msrMeasure::finalizeMeasureClone (
  int                 inputLineNumber,
  const S_msrMeasure& originalMeasure,
  const S_msrVoice&   voiceClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing measure clone " <<
      this->asString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
// JMI      fMeasureUpLinkToSegment->getSegmentUpLinkToVoice ()->getVoiceName () <<
      voiceClone->getVoiceName () <<
        ", lines " << inputLineNumber << " .. " << fMeasureEndInputLineNumber << // JMI
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    if (fMeasureWholeNotesDuration.getNumerator () == 0) {
      std::stringstream s;

      s <<
        "Measure " <<
        this->asShortString () <<
        " in segment '" <<
        fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
        "' in voice clone \"" <<
        voiceClone->getVoiceName () <<
        "\", line " << inputLineNumber <<
        ", doesn't contain any music" <<
        std::endl;

      msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureClone() 1");
  }
#endif

  if (fMeasureHasBeenFinalized) {
    std::stringstream s;

    s <<
      "Attempting to finalize measure clone \"" <<
      asShortString () <<
      "\" more than once";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  ++gIndenter;

  // fetch the voiceClone's current time
  S_msrTimeSignature
    voiceCurrentTimeSignature =
      voiceClone->
        getVoiceCurrentTimeSignature ();

  if (! voiceCurrentTimeSignature) {
    // take the implicit 4/4 measure whole notes into account
    setFullMeasureWholeNotesDuration (
      Rational (1, 1));
  }
  else {
    // set the full length from time
    setFullMeasureWholeNotesDurationFromTime (
      voiceCurrentTimeSignature);
  }

/* JMI
  // is this note the shortest one in this measure?
  Rational
    voiceShortestNoteDuration =
      voiceClone->
        getVoiceShortestNoteDuration ();

  if (fMeasureShortestNoteDuration < voiceShortestNoteDuration) {
    voiceClone->
      registerShortestNoteInVoiceIfRelevant (
        note);

    // measure shortest note tuplet factor // JMI
    fMeasureShortestNoteTupletFactor = Rational (1, 1);
  }
*/

  // register this measures's length in the part clone
  S_msrPart
    part =
      voiceClone->
        getVoiceUpLinkToStaff ()->
          getStaffUpLinkToPart ();

  part->
    registerOrdinalMeasureNumberWholeNotesDuration (
      inputLineNumber,
      fMeasureOrdinalNumberInVoice,
      fMeasureWholeNotesDuration);

  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    originalMeasure->getMeasureRepeatContextKind ());

  // consistency check
  msrMeasureKind
    originalMeasureMeasureKind =
      originalMeasure->getMeasureKind ();

  if (fMeasureKind != originalMeasureMeasureKind) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
      this->
        displayMeasure (
          inputLineNumber,
          "finalizeMeasureClone() 2 - cloneMeasure");

      originalMeasure->
        displayMeasure (
          inputLineNumber,
          "finalizeMeasureClone() 3 - originalMeasure");

      std::stringstream s;

      s <<
        "*********>> measure clone '" <<
        fMeasureNumber <<
        "': measure kind '" <<
        fMeasureKind <<
        "' differs from original measure measure kind '" <<
        originalMeasureMeasureKind <<
        "' in voice \"" <<
        voiceClone->getVoiceName () <<
        "', line " << inputLineNumber;

   //   msrInternalError ( // JMI
      msrInternalWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
   //      __FILE__, __LINE__,
        s.str ());

      if (fFullMeasureWholeNotesDuration.getNumerator () == 0) { // JMI
        std::stringstream s;

        s <<
          "*********>> measure clone '" <<
          fMeasureNumber <<
          "' full measure whole notes is 0" <<
          ", line " << inputLineNumber;

     //   msrInternalError ( // JMI
        msrInternalWarning (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
     //      __FILE__, __LINE__,
          s.str ());
      }
    }
#endif
  }

  // delegate finalization to voice kind specific methods

  // JMI v0.9.66 BOFF quick quirk, repeat context kinds will probably be thrown away in the future
  // JMI v0.9.66 BOFF quick quirk
//   const std::string context = "????? finalizeMeasureClone context?????";
//
//   switch (voiceClone->getVoiceKind ()) { // JMI v0.9.66 NEEDED ???
//     case msrVoiceKind::kVoiceKindRegular:
//       finalizeRegularMeasure (
//         inputLineNumber,
//         fMeasureRepeatContextKind,
//         context);
//       break;
//
//     case msrVoiceKind::kVoiceKindDynamics:
//       break;
//
//     case msrVoiceKind::kVoiceKindHarmonies:
//       finalizeHarmonyMeasure (
//         inputLineNumber,
//         fMeasureRepeatContextKind,
//         context);
//       break;
//
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       finalizeFiguredBassMeasure (
//         inputLineNumber,
//         fMeasureRepeatContextKind,
//         context);
//       break;
//   } // switch

  // voice position
  Rational
    voicePosition =
      fetchMeasureUpLinkToVoice ()->
        getCurrentVoicePosition ();

  // delegate position measure assignment to the elements in the measure
  for (
    std::list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    ++i
  ) {
    S_msrMeasureElement measureElement = (*i);

//     measureElement->
//       setMeasureElementVoicePosition ( // JMI v0.9.66
//         voicePosition,
//         "finalizeMeasure()");
  } // for

  // register finalization
  fMeasureHasBeenFinalized = true;
  fMeasureFinalizationContext = "finalizeMeasureClone()";

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureClone() 4");
  }
#endif

  --gIndenter;
}

void msrMeasure::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasure::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasure::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasure::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasure::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasure::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasure::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasure::browseData ()" <<
      std::endl;
  }

  for (
    std::list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    ++i
  ) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrMeasure::browseData ()" <<
      std::endl;
  }
}

// std::string msrMeasureFirstInSegmentKindAsString (
//     msrMeasureFirstInSegmentKind measureFirstInSegmentKind)
// {
//   std::string result;
//
//   switch (measureFirstInSegmentKind) {
//     case msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindUnknown:
//       result = "kMeasureFirstInSegmentKindUnknown***";
//       break;
//     case msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindYes:
//       result = "kMeasureFirstInSegmentKindYes";
//       break;
//     case msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindNo:
//       result = "kMeasureFirstInSegmentKindNo";
//       break;
//   } // switch
//
//   return result;
// }
//
// std::string msrMeasureRepeatContextKindAsString (
//   msrMeasureRepeatContextKind measureRepeatContextKind)
// {
//   std::string result;
//
//   switch (measureRepeatContextKind) {
//     case msrMeasureRepeatContextKind::kMeasureRepeatContext_NO_:
//       result = "kMeasureRepeatContext_NO_";
//       break;
//     case msrMeasureRepeatContextKind::kMeasureRepeatContextNone:
//       result = "kMeasureRepeatContextNone";
//       break;
//     case msrMeasureRepeatContextKind::kMeasureRepeatContextCommonPartLastMeasure:
//       result = "kMeasureRepeatContextCommonPartLastMeasure";
//       break;
//     case msrMeasureRepeatContextKind::kMeasureRepeatContextHookedEndingLastMeasure:
//       result = "kMeasureRepeatContextHookedEndingLastMeasure";
//       break;
//     case msrMeasureRepeatContextKind::kMeasureRepeatContextHooklessEndingLastMeasure:
//       result = "kMeasureRepeatContextHooklessEndingLastMeasure";
//       break;
//     case msrMeasureRepeatContextKind::kMeasureRepeatContextNextMeasureAfterCommonPart:
//       result = "kMeasureRepeatContextNextMeasureAfterCommonPart";
//       break;
//     case msrMeasureRepeatContextKind::kMeasureRepeatContextNextMeasureAfterHookedEnding:
//       result = "kMeasureRepeatContextNextMeasureAfterHookedEnding";
//       break;
//     case msrMeasureRepeatContextKind::kMeasureRepeatContextNextMeasureAfterHooklessEnding:
//       result = "kMeasureRepeatContextNextMeasureAfterHooklessEnding";
//       break;
//   } // switch
//
//   return result;
// }
//
// std::string msrMeasureEndRegularKindAsString (
//     msrMeasureEndRegularKind measureEndRegularKind)
// {
//   std::string result;
//
//   switch (measureEndRegularKind) {
//     case msrMeasureEndRegularKind::kMeasureEndRegularKindUnknown:
//       result = "kMeasureEndRegularUnknown***";
//       break;
//     case msrMeasureEndRegularKind::kMeasureEndRegularKindYes:
//       result = "kMeasureEndRegularYes";
//       break;
//     case msrMeasureEndRegularKind::kMeasureEndRegularKindNo:
//       result = "kMeasureEndRegularNo";
//       break;
//   } // switch
//
//   return result;
// }

std::string msrMeasure::asShortString () const
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  std::stringstream s;

  s <<
    "[Measure " <<
    fMeasureNumber <<
// JMI    ", fMeasureKind: " <<
    ", " <<
    fMeasureKind <<
// JMI    ", voice: " <<
    ", " <<
    voice->getVoiceName () <<
    /* JMI
    ", fMeasureOrdinalNumberInVoice: " <<
    fMeasureOrdinalNumberInVoice <<
    ", fMeasurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", fMeasureDebugNumber: '" <<
    fMeasureDebugNumber <<
    */
/* JMI
    ", fMeasureWholeNotesDuration: " <<
    fMeasureWholeNotesDuration <<
    ", fFullMeasureWholeNotesDuration: " <<
    fFullMeasureWholeNotesDuration <<
    */
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrMeasure::asShortStringForMeasuresSlices () const
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  std::stringstream s;

  s <<
    fMeasureNumber <<
    " (" << voice->getVoiceName () <<
    "): [";
/* JMI
// JMI    ", measureKind: " <<
    ": [";
    msrMeasureKindasShortStringForMeasuresSlices (fMeasureKind) <<
    ", voice: " <<
    ", " <<
    voice->getVoiceName () <<
    ", fMeasureOrdinalNumberInVoice: " <<
    fMeasureOrdinalNumberInVoice <<
    ", fMeasurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", fMeasureDebugNumber: '" <<
    fMeasureDebugNumber <<
    ", fMeasureWholeNotesDuration: " <<
    fMeasureWholeNotesDuration <<
    ", fFullMeasureWholeNotesDuration: " <<
    fFullMeasureWholeNotesDuration <<
    ", ";
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    ", ";
*/

  size_t
    measureElementsListSize =
      fMeasureElementsList.size ();

/* JMI JMI
  s <<
    "fMeasureElementsList: " <<
    mfSingularOrPlural (
      measureElementsListSize, "element", "elements") <<
    std::endl;
*/

  if (measureElementsListSize) {
// JMI    s << std::endl;

    ++gIndenter;

    std::list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement measureElement = (*i);

      s <<
        measureElement->asShortStringForMeasuresSlices ();
      if (++i == iEnd) break;
      s << ", ";
    } // for

    --gIndenter;
  }

  s << ']';

  return s.str ();
}

std::string msrMeasure::asString () const
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  std::stringstream s;

  s <<
    "[Measure '" <<
    fMeasureNumber <<
    "', fMeasureKind: " <<
    fMeasureKind <<
    ", voice: " <<
    voice->getVoiceName () <<
    ", fMeasureOrdinalNumberInVoice: " <<
    fMeasureOrdinalNumberInVoice <<
    ", fMeasurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", fMeasureDebugNumber: '" <<
    fMeasureDebugNumber <<
    ", fMeasureWholeNotesDuration: " <<
    fMeasureWholeNotesDuration <<
    ", fFullMeasureWholeNotesDuration: " <<
    fFullMeasureWholeNotesDuration <<
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrMeasure::displayMeasure (
  int                inputLineNumber,
  const std::string& context)
{
  gLogStream <<
    std::endl <<
    "*********>> Measure " <<
    fMeasureNumber <<
    /* JMI
    "', fMeasurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", fMeasureDebugNumber: '" <<
    fMeasureDebugNumber <<
    */
    " (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    std::endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    std::endl << std::endl;
}

void msrMeasure::print (std::ostream& os) const
{
  os <<
    "[Measure '" <<
    fMeasureNumber <<
    "', " << fMeasureKind <<
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", fMeasureWholeNotesDuration: " <<
    fMeasureWholeNotesDuration <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 45;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fullMeasureWholeNotesDuration" << ": " <<
    fFullMeasureWholeNotesDuration <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureOrdinalNumberInVoice" << ": " <<
    fMeasureOrdinalNumberInVoice <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasurePuristNumber" << ": " <<
    fMeasurePuristNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureDebugNumber" << ": " <<
    fMeasureDebugNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureEndInputLineNumber" << ": " <<
    fMeasureEndInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureEndRegularKind" << ": " <<
    msrMeasureEndRegularKindAsString (
      fMeasureEndRegularKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureRepeatContextKind" << ": " <<
    msrMeasureRepeatContextKindAsString (
      fMeasureRepeatContextKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureIsFirstInVoice" << ": " <<
    fMeasureIsFirstInVoice <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureFirstInSegmentKind" << ": " <<
    msrMeasureFirstInSegmentKindAsString (
      fMeasureFirstInSegmentKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureUpLinkToSegment" << ": " <<
    fMeasureUpLinkToSegment->asShortString () <<
    std::endl;


#ifdef TRACING_IS_ENABLED
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  // fetch voice's current clef, key and time signature
  S_msrClef
    voiceCurrentClef =
      voice->getVoiceCurrentClef ();
  S_msrKey
    voiceCurrentKey =
      voice->getVoiceCurrentKey ();
  S_msrTimeSignature
    voiceCurrentTimeSignature =
      voice->getVoiceCurrentTimeSignature ();

  // print the voice current clef, key and time signature
  os << std::left <<
    std::setw (fieldWidth) <<
    "voiceCurrentClef" << ": ";
  if (voiceCurrentClef) {
    os <<
      voiceCurrentClef;
  }
  else {
    os << "null" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "voiceCurrentKey" << ": ";
  if (voiceCurrentKey) {
    os <<
      voiceCurrentKey;
  }
  else {
    os << "null" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "voiceCurrentTimeSignature" << ": ";
  if (voiceCurrentTimeSignature) {
    os << std::endl;
    ++gIndenter;

    os <<
      voiceCurrentTimeSignature;

    --gIndenter;
  }
  else {
    os << "null" << std::endl;
  }
#endif

    /* JMI

    std::setw (fieldWidth) <<
    "measureWholeNotesDurationAsMsrString" << ": " <<
    measureWholeNotesDurationAsMsrString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fullMeasureWholeNotesDurationAsMsrString" << ": " <<
    fullMeasureWholeNotesDurationAsMsrString () <<
    std::endl <<
      */

/* JMI
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureShortestNoteDuration" << ": " <<
    fMeasureShortestNoteDuration <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureShortestNoteTupletFactor" << ": " <<
    fMeasureShortestNoteTupletFactor <<
    std::endl;
*/

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureLongestNote" << ": ";
  if (fMeasureLongestNote) {
    os <<
      fMeasureLongestNote->asShortString ();
  }
  else {
    os <<
      "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureContainsMusic" << ": " <<
    fMeasureContainsMusic <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureKindAndPuristNumberHaveBeenDetermined" << ": " <<
    fMeasureKindAndPuristNumberHaveBeenDetermined <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureVoicePosition" << ": " <<
    fMeasureVoicePosition <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureVoiceMoment" << ": " <<
    fMeasureVoiceMoment.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureHasBeenFinalized" << ": " <<
    fMeasureHasBeenFinalized <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureFinalizationContext" << ": " <<
    fMeasureFinalizationContext <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureIsAFullBarRest" << ": " <<
    fMeasureIsAFullBarRest <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fNextMeasureNumber" << " : \"" <<
    fNextMeasureNumber <<
    "\"" <<
    std::endl;

  // print measure elements list
  size_t
    measureElementsListSize =
      fMeasureElementsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fMeasureElementsList" << ": " <<
    mfSingularOrPlural (
      measureElementsListSize, "element", "elements") <<
    std::endl;

  if (measureElementsListSize) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement measureElement = (*i);

      os << measureElement;
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  os << std::endl;

  // print measure notes flat std::list
  size_t
    measureNotesFlatListSize =
      fMeasureNotesFlatList.size ();

  os <<
    "fMeasureNotesFlatList: " <<
    mfSingularOrPlural (
      measureNotesFlatListSize, "note", "notes") <<
    std::endl;

  if (measureNotesFlatListSize) {
    ++gIndenter;

    std::list<S_msrNote>::const_iterator
      iBegin = fMeasureNotesFlatList.begin (),
      iEnd   = fMeasureNotesFlatList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote note = (*i);

      os <<
        note->asShortStringForTimeView ();

      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << std::endl;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrMeasure::printShort (std::ostream& os) const
{
  os <<
    "[Measure '" <<
    fMeasureNumber <<
    "', " << fMeasureKind <<
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", fMeasureWholeNotesDuration: " <<
    fMeasureWholeNotesDuration <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureIsFirstInVoice" << ": " <<
    fMeasureIsFirstInVoice <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureOrdinalNumberInVoice" << ": " <<
    fMeasureOrdinalNumberInVoice <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasurePuristNumber" << ": " <<
    fMeasurePuristNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureDebugNumber" << ": " <<
    fMeasureDebugNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureEndInputLineNumber" << ": " <<
    fMeasureEndInputLineNumber <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fullMeasureWholeNotesDuration" << ": " <<
    fFullMeasureWholeNotesDuration <<
    std::endl;

  size_t
    measureElementsListSize =
      fMeasureElementsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fMeasureElementsList" << ": " <<
    mfSingularOrPlural (
      measureElementsListSize, "element", "elements") <<
    std::endl;

  if (measureElementsListSize) {
    os << std::endl;
    ++gIndenter;

    std::list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrMeasure& elt)
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

/*
//   if (measurePositionsDelta.getNumerator () > 0) {
//     // there is at least one note between
//   }

//   // is a padding skip note needed?
//   if (measurePositionsDelta.getNumerator () > 0) {
//     // create a padding skip note
//     S_msrNote
//       skipNote =
//         createPaddingSkipNoteForVoice (
//           inputLineNumber,
//           measurePositionsDelta,
//           voice);
//
//     // set its measure position
//     skipNote->
//       setMeasureElementMeasurePosition (
//         this,
//         fMeasureWholeNotesDuration,
//         "handleSubsequentHarmonyInHarmoniesMeasure() 2");
//
//     // insert skipNote before currentHarmony in the measure's elements list
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
//       gLogStream <<
//         "Inserting subsequent padding note " <<
//         skipNote->asString () <<
//         " before currentHarmony " <<
//         currentHarmony->asString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//     }
// #endif
//
//     // insert skipNote in the measure elements list before (*i)
//     insertElementInMeasureBeforeIterator (
//       inputLineNumber,
//       i,
//       skipNote);
//   }
//
//   else if (measurePositionsDelta.getNumerator () < 0) {
//     // the two harmonies overlap in time
//     std::stringstream s;
//
//     s <<
//       "Previous harmony " <<
//       previousHarmony->asString () <<
//       " overlaps current harmony " <<
//       currentHarmony->asString ();
//
//     msrInternalWarning (
//       gGlobalServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       s.str ());
//
//     // compute previousHarmony's future sounding whole notes
//     Rational
//       reducedSoundingWholeNotes =
//         previousHarmonySoundingWholeNotes
//           + // the delta is negative
//         measurePositionsDelta;
//
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
//       gLogStream <<
//         "Reducing the sounding whole notes of harmony FOO " <<
//         previousHarmony->asString () <<
//         " from " <<
//         previousHarmonySoundingWholeNotes <<
//         " to " <<
//         reducedSoundingWholeNotes <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//     }
// #endif
//
//     if (reducedSoundingWholeNotes.getNumerator () == 0) {
//       std::stringstream s;
//
//       s <<
//         "Cannot reduce the duration of harmony " <<
//         previousHarmony->asShortString () <<
//         " to 0 : leaving it as it is";
//
//       msrInternalWarning (
//         gGlobalServiceRunData->getInputSourceName (),
//         inputLineNumber,
// //  JMI             __FILE__, __LINE__,
//         s.str ());
//     }
//     else {
//       // set previousHarmony's duration to the reduced value
//       previousHarmony->
//         setMeasureElementSoundingWholeNotes (
//           reducedSoundingWholeNotes);
//     }
//   }
*/
