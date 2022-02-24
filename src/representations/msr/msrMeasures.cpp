/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "rational.h"

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

#include "oahOah.h"

#include "msrOah.h"

#include "msrHiddenMeasureAndBarLines.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
int msrMeasure::gGlobalMeasureDebugNumber = 0;

S_msrMeasure msrMeasure::create (
  int           inputLineNumber,
  const string& measureNumber,
  S_msrSegment  measureSegmentUpLink)
{
  msrMeasure* o =
    new msrMeasure (
      inputLineNumber,
      measureNumber,
      measureSegmentUpLink);
  assert (o != nullptr);

  return o;
}

msrMeasure::msrMeasure (
  int           inputLineNumber,
  const string& measureNumber,
  S_msrSegment  measureSegmentUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureSegmentUpLink != nullptr,
    "measureSegmentUpLink is null");

  // set measure's segment upLink
  fMeasureSegmentUpLink =
    measureSegmentUpLink;

  // set measure numbers
  fMeasureElementMeasureNumber = measureNumber;
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
    voiceUpLink =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Initializing measure '" <<
      fMeasureElementMeasureNumber <<
      ", measureDebugNumber: '" <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voiceUpLink->getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // measure kind
  fMeasureKind = msrMeasureKind::kMeasureKindUnknown;

  // measure 'first in segment' kind
  fMeasureFirstInSegmentKind = kMeasureFirstInSegmentKindUnknown;

  // measure 'first in voice'
  fMeasureFirstInVoice = false; // default value

  // single-measure rest?
  fMeasureIsAFullMeasureRest = false;

  // initialize measure whole notes
  setCurrentMeasureWholeNotesDuration (
    fInputLineNumber,
    rational (0, 1)); // ready to receive the first note

  // fetch the staff
  S_msrStaff
    staffUpLink =
      voiceUpLink->
        getVoiceStaffUpLink ();

  // set the full measure whole notes duration from time if relevant
  S_msrTimeSignature
    staffCurrentTimeSignature =
      staffUpLink->
        getStaffCurrentTime ();

  if (staffCurrentTimeSignature) {
    setFullMeasureWholeNotesDurationFromTime (
      staffCurrentTimeSignature);
  }

  // measure shortest note duration
//  fMeasureShortestNoteDuration = rational (INT_MAX, 1);

  // measure shortest note tuplet factor
//  fMeasureShortestNoteTupletFactor = rational (1, 1);

  // measure doesn't contain music yet
  fMeasureContainsMusic = false;

  // regular measure ends detection
  fMeasureEndRegularKind = kMeasureEndRegularKindUnknown;

  // repeat context
  fMeasureRepeatContextKind = msrMeasure::kMeasureRepeatContextKindUnknown;

  // current measure whole notes duration
  fCurrentMeasureWholeNotesDuration = rational (0, 1);

  // position in voice
  fMeasurePositionInVoice =
    fMeasureSegmentUpLink->
      getSegmentVoiceUpLink ()->
        getCurrentPositionInVoice ();

  fMeasureMomentInVoice =
    fMeasureSegmentUpLink->
      getSegmentVoiceUpLink ()->
        getCurrentMomentInVoice ();

  // measure finalization
  fMeasureHasBeenFinalized = false;
  fMeasureKindAndPuristNumberHaveBeenDetermined = false;
}

msrMeasure::~msrMeasure ()
{}

S_msrVoice msrMeasure::fetchMeasureVoiceUpLink () const
{
  S_msrVoice result;

  if (fMeasureSegmentUpLink) {
    result =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();
  }

  return result;
}

S_msrStaff msrMeasure::fetchMeasureStaffUpLink () const
{
  S_msrStaff result;

  if (fMeasureSegmentUpLink) {
    result =
      fMeasureSegmentUpLink->
        fetchSegmentStaffUpLink ();
  }

  return result;
}

S_msrPart msrMeasure::fetchMeasurePartUpLink () const
{
  S_msrPart result;

  if (fMeasureSegmentUpLink) {
    result =
      fMeasureSegmentUpLink->
        fetchSegmentPartUpLink ();
  }

  return result;
}

S_msrPartGroup msrMeasure::fetchMeasurePartGroupUpLink () const
{
  S_msrPartGroup result;

  if (fMeasureSegmentUpLink) {
    result =
      fMeasureSegmentUpLink->
        fetchSegmentPartGroupUpLink ();
  }

  return result;
}

S_msrScore msrMeasure::fetchMeasureScoreUpLink () const
{
  S_msrScore result;

  if (fMeasureSegmentUpLink) {
    result =
      fMeasureSegmentUpLink->
        fetchSegmentScoreUpLink ();
  }

  return result;
}

S_msrMeasure msrMeasure::createMeasureNewbornClone (
  S_msrSegment containingSegment)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");

  S_msrVoice
    containingSegmentVoiceUpLink =
      containingSegment->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating a newborn clone of measure " <<
      this->asShortString () <<
      fMeasureDebugNumber <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
      containingSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
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
        fMeasureElementMeasureNumber,
        containingSegment);

  // set newbornClone's ordinal number
  newbornClone->
    setMeasureOrdinalNumberInVoice (
      containingSegmentVoiceUpLink->
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
  newbornClone->fMeasureIsAFullMeasureRest =
    fMeasureIsAFullMeasureRest;

  // regular measure ends detection // JMI TEMP
  newbornClone->fMeasureEndRegularKind =
    fMeasureEndRegularKind;
*/

  /*
  // fCurrentMeasureWholeNotesDuration and fFullMeasureWholeNotesDuration
  // will be computed on the fly
  // when appending notes to the measure newborn clone

  // measure kind
  newbornClone->fMeasureKind =
    fMeasureKind;
*/

  // chords handling

  // elements

  // upLinks

  return newbornClone;
}

S_msrMeasure msrMeasure::createMeasureDeepClone (
  S_msrSegment containingSegment)
{
  S_msrVoice
    containingSegmentVoiceUpLink =
      containingSegment->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating a deep clone of measure " <<
      this->asShortString () <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
      containingSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");

  // create deep clone
  S_msrMeasure
    measureDeepClone =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureElementMeasureNumber,
        containingSegment);

  // measure numbers
  measureDeepClone->
    setMeasureOrdinalNumberInVoice (
      containingSegmentVoiceUpLink->
        incrementVoiceCurrentMeasureOrdinalNumber ());

  measureDeepClone->fMeasurePuristNumber =
    fMeasurePuristNumber;

  measureDeepClone->fMeasureDebugNumber =
    fMeasureDebugNumber;

  // measure lengthes, in whole notes
  measureDeepClone->fFullMeasureWholeNotesDuration =
    fFullMeasureWholeNotesDuration;

  measureDeepClone->fCurrentMeasureWholeNotesDuration = // JMI ???
    fCurrentMeasureWholeNotesDuration;

  // measure print layout, MusicXML specific
  measureDeepClone->fMeasurePrintLayout =
    fMeasurePrintLayout;

  // measure longest note
  measureDeepClone->fMeasureLongestNote = // JMI ???
    fMeasureLongestNote;

  // measure kind
  measureDeepClone->fMeasureKind =
    fMeasureKind;

  // next measure number
  measureDeepClone->fNextMeasureNumber =
    fNextMeasureNumber;

  // measure 'first in segment' kind
  measureDeepClone->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;

    // full measure rest?

  measureDeepClone->fMeasureIsAFullMeasureRest =
    fMeasureIsAFullMeasureRest;

  // elements

  int numberOfMeasureElements =
    fMeasureElementsList.size ();

  if (numberOfMeasureElements) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        mfSingularOrPluralWithoutNumber (
          numberOfMeasureElements, "There is", "There are") <<
        " " <<
        mfSingularOrPlural (
          numberOfMeasureElements, "element", "elements") <<
        " to be deep copied in measure " <<
        this->asShortString () <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
          containingSegment->
            getSegmentVoiceUpLink ()->
              getVoiceName () <<
        "\"" <<
        endl;
    }
#endif

    for (
      list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
      i != fMeasureElementsList.end ();
      ++i ) {
      S_msrMeasureElement element = (*i);

      // handlle deep cloning
      S_msrMeasureElement
        elementDeepClone;

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*element))
        ) {
        // create the note deep clone
        elementDeepClone =
          note->createNoteDeepClone (
            fetchMeasureVoiceUpLink ());

/* JMI
        // append the element deep clone to the measure deep clone
        measureDeepClone->
          appendElementToMeasure (
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
            endl;
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
      measureDeepClone->
        appendElementToMeasure (
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
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        endl;
    }
#endif
  }

  // upLinks

  // fMeasureSegmentUpLink JMI ???

  return measureDeepClone;
}

S_msrMeasure msrMeasure::createMeasureCopyWithNotesOnly (
  S_msrSegment  containingSegment,
  const string& measureNumber)
{
  S_msrVoice
    containingSegmentVoiceUpLink =
      containingSegment->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating a copy with notes only of measure " <<
      this->asShortString () <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
      containingSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
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
      containingSegmentVoiceUpLink->
        incrementVoiceCurrentMeasureOrdinalNumber ());

/* JMI
  // lengthes
  measureCopy->fFullMeasureWholeNotesDuration =
    fFullMeasureWholeNotesDuration;

  measureCopy->fCurrentMeasureWholeNotesDuration =
    fCurrentMeasureWholeNotesDuration;

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
        " " <<
        mfSingularOrPlural (
          numberOfMeasureElements, "element", "elements") <<
        " to be copied with notes only in measure " <<
        this->asShortString () <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
          containingSegment->
            getSegmentVoiceUpLink ()->
              getVoiceName () <<
        "\"" <<
        endl;
    }
#endif

    for (
      list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
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
            endl;
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
          appendElementToMeasure (
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
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        endl;
    }
#endif
  }

  return measureCopy;
}

/* JMI
void msrMeasure::setMeasureShortestNoteDuration (
  const rational& duration)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalMsrOahGroup->getTraceMsrDurations ()
  ) {
    gLogStream <<
      "Setting the shortest note duration of measure " <<
      fMeasureElementMeasureNumber <<
      " to " <<
      duration <<
      endl;
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
      fMeasureElementMeasureNumber <<
      " to " <<
      noteTupletFactor <<
      endl;
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
        fMeasureElementMeasureNumber <<
        "' to '" <<
        measureEndRegularKindAsString (measureEndRegularKind) <<
        "' in segment " <<
        fMeasureSegmentUpLink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        endl;
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
        fMeasureElementMeasureNumber <<
        "' to '" <<
        measureRepeatContextKindAsString (measureRepeatContextKind) <<
        "' in segment " <<
        fMeasureSegmentUpLink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        endl;
    }
#endif

  fMeasureRepeatContextKind = measureRepeatContextKind;
}

void msrMeasure::setMeasurePuristNumber (
  int measurePuristNumber)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Setting purist number of measure '" <<
        fMeasureElementMeasureNumber <<
        "' to '" <<
        measurePuristNumber <<
        "' in segment " <<
        fMeasureSegmentUpLink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        endl;
    }
#endif

  fMeasurePuristNumber = measurePuristNumber;
}

// void msrMeasure::incrementMeasurePositionInVoice (
// rational wholeNotesDelta)
// {
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
//       gLogStream <<
//         "Incrementing the measure position in voice " <<
//         this->asShortString () <<
//         " by " <<
//         wholeNotesDelta <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         endl;
//     }
// #endif
//
//   fMeasurePositionInVoice += wholeNotesDelta;
// }

void msrMeasure::appendElementToMeasure (S_msrMeasureElement elem)
{
  int inputLineNumber =
    elem->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending element " <<
      elem->asShortString () <<
      " to measure " <<
      asShortString () <<
      " in voice \"" <<
      fetchMeasureVoiceUpLink ()->
        getVoiceName () <<
      "\", currentMeasureWholeNotesDuration = " <<
      fCurrentMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // set elem's measure number
  elem->
    setMeasureElementMeasureNumber (
      fMeasureElementMeasureNumber);

  // set elem's position in measure
  elem->
    setMeasureElementPositionInMeasure (
      fCurrentMeasureWholeNotesDuration,
      "appendElementToMeasure()");

  fMeasureElementsList.push_back (elem);

  // take elem's sounding whole notes into account JMI ???
if (false) // JMI CAFE
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    elem->
      getMeasureElementSoundingWholeNotes ());
}

void msrMeasure::insertElementInMeasureBeforeIterator (
  int                                 inputLineNumber,
  list<S_msrMeasureElement>::iterator iter,
  S_msrMeasureElement                 elem)
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
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()
          ->getVoiceName () <<
      "\", currentMeasureWholeNotesDuration = " <<
      fCurrentMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // set elem's measure number
  elem->
    setMeasureElementMeasureNumber (
      fMeasureElementMeasureNumber);

  // set elem's position in measure
  elem->
    setMeasureElementPositionInMeasure (
      fCurrentMeasureWholeNotesDuration,
      "insertElementInMeasureBeforeIterator()");

  // insert elem in the measure elements list before (*iter)
  fMeasureElementsList.insert (
    iter, elem);

  // account for elem's duration in current measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    elem->getMeasureElementSoundingWholeNotes ());
}

void msrMeasure::appendElementAtTheEndOfMeasure (
  S_msrMeasureElement elem)
{
  int inputLineNumber =
    elem->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceBarLines ()
  ) {
    gLogStream <<
      "Appending element " <<
      elem->asString () <<
      " at the end of measure " <<
      asString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()
          ->getVoiceName () <<
      "\", has position in measure '" <<
      elem->getMeasureElementPositionInMeasure () <<
      ", currentMeasureWholeNotesDuration = " <<
      fCurrentMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      endl;

    typedef list<S_msrMeasureElement>::iterator iter_type;

    gLogStream <<
      endl <<
      "==> appendPaddingNoteAtTheEndOfMeasure(), fMeasureElementsList:" <<
      endl;
    ++gIndenter;
    for (
      iter_type it = fMeasureElementsList.begin ();
      it != fMeasureElementsList.end ();
      ++it
    ) {
      gLogStream << ' ' << *it << endl;
    } // for
    gLogStream << endl;
    --gIndenter;
  }

  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "appendPaddingNoteAtTheEndOfMeasure() 1");
  }
#endif

  if (! fMeasureElementsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (
      gGlobalTracingOahGroup->getTraceMeasures ()
        ||
      gGlobalTracingOahGroup->getTraceBarLines ()
    ) {
      gLogStream <<
        "fMeasureElementsList is empty " <<
        ", inserting elem at the end" <<
        endl;
    }
#endif

    appendElementToMeasure (elem);

    // take elem's sounding whole notes duration into account
    // could be done elsewhere ??? JMI
    incrementCurrentMeasureWholeNotesDuration (
      inputLineNumber,
      elem->
        getMeasureElementSoundingWholeNotes ());
  }

  else {
    S_msrMeasureElement
      lastMeasureElement =
        fMeasureElementsList.back ();

#ifdef TRACING_IS_ENABLED
    if (
      gGlobalTracingOahGroup->getTraceMeasures ()
        ||
      gGlobalTracingOahGroup->getTraceBarLines ()
    ) {
      gLogStream <<
        "fMeasureElementsList is NOT empty " <<
        ", lastMeasureElement: " <<
        lastMeasureElement <<
        endl;
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
            endl;
        }
#endif

        // fetch iterator to list end
        list<S_msrMeasureElement>::iterator it =
            fMeasureElementsList.end ();

        // fetch iterator to list last element
        --it;

        // insert elem before it in list
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
            endl;
        }
#endif

        appendElementToMeasure (elem);
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
          endl;
      }
#endif

      appendElementToMeasure (elem);
    }
  }

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceBarLines ()
  ) {
    typedef list<S_msrMeasureElement>::iterator iter_type;

    gLogStream <<
      endl <<
      "<== appendPaddingNoteAtTheEndOfMeasure(), fMeasureElementsList:" <<
      endl;
    ++gIndenter;
    for (
      iter_type it = fMeasureElementsList.begin ();
      it != fMeasureElementsList.end ();
      ++it
    ) {
      gLogStream << ' ' << *it << endl;
    } // for
    gLogStream << endl;
    --gIndenter;
  }

  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "appendPaddingNoteAtTheEndOfMeasure() 2");
  }
#endif
}

void msrMeasure::insertElementAtPositionInMeasure (
  int                 inputLineNumber,
  const rational&     positionInMeasure,
  S_msrMeasureElement elem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Inserting element " <<
      elem->asShortString () <<
      " at position " <<
      positionInMeasure <<
      " in measure " <<
      asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()
          ->getVoiceName () <<
      "\", currentMeasureWholeNotesDuration = " <<
      fCurrentMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  unsigned int
    measureElementsListSize =
      fMeasureElementsList.size ();

  if (measureElementsListSize) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement
        currentElement = (*i);

      rational currentPositionInMeasure =
        currentElement->
          getMeasureElementPositionInMeasure ();

      if (positionInMeasure == currentPositionInMeasure) {
        // insert elem in the measure elements list before (*i)
        fMeasureElementsList.insert (
          i, elem);
      }
      else if (positionInMeasure >= currentPositionInMeasure) {
        stringstream s;

        s <<
          "cannot insert element " <<
          elem->asShortString () <<
          " at position " <<
          positionInMeasure <<
          " in measure " <<
          asShortString () <<
          " in voice \"" <<
          fMeasureSegmentUpLink->
            getSegmentVoiceUpLink ()
              ->getVoiceName () <<
          "\", currentMeasureWholeNotesDuration = " <<
          fCurrentMeasureWholeNotesDuration <<
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
    stringstream s;

    s <<
      "cannot insert element " <<
      elem->asShortString () <<
      " at position " <<
      positionInMeasure <<
      " in measure " <<
      asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()
          ->getVoiceName () <<
      "\", currentMeasureWholeNotesDuration = " <<
      fCurrentMeasureWholeNotesDuration <<
      " since it is empty" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // set elem's measure number
  elem->
    setMeasureElementMeasureNumber (
      fMeasureElementMeasureNumber);

  // set elem's position in measure
  elem->
    setMeasureElementPositionInMeasure (
      positionInMeasure,
      "insertElementAtPositionInMeasure()");

  // account for elem's duration in current measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    elem->getMeasureElementSoundingWholeNotes ());
}

void msrMeasure::setNextMeasureNumber (const string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      "' in measure " <<
      this->asShortString () <<
      " in segment " <<
      fMeasureSegmentUpLink-> asString () <<
      /* JMI
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      */
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fNextMeasureNumber = nextMeasureNumber;
}

void msrMeasure::setMeasureFirstInVoice ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting measure " <<
      this->asShortString () <<
      " as first measure in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      "' in segment " <<
      fMeasureSegmentUpLink-> asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fMeasureFirstInVoice = true;
}

string msrMeasure::fullMeasureWholeNotesDurationAsMsrString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFullMeasureWholeNotesDuration);
}

void msrMeasure::setFullMeasureWholeNotesDuration (
  const rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceWholeNotes ()
  ) {
    gLogStream <<
      "Setting full whole notes duration of measure " <<
      this->asShortString () <<
      " to '"  <<
      wholeNotes <<
      "'" <<
      endl;
  }
#endif

  fFullMeasureWholeNotesDuration = wholeNotes;
}

void msrMeasure::setCurrentMeasureWholeNotesDuration (
  int             inputLineNumber,
  const rational& wholeNotes)
{
  // rationalise the measure whole notes duration
  rational
    rationalisedCurrentMeasureWholeNotesDuration =
      wholeNotes;
  rationalisedCurrentMeasureWholeNotesDuration.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceWholeNotes ()
  ) {
    gLogStream <<
      "Setting current whole notes duration of measure " <<
      this->asShortString () <<
      " to '"  <<
      rationalisedCurrentMeasureWholeNotesDuration <<
      "'";

    if (
      rationalisedCurrentMeasureWholeNotesDuration.getDenominator ()
        !=
      wholeNotes.getDenominator ()
    ) {
      gLogStream <<
        " (rationalised from '" << wholeNotes << "')";
    }

    gLogStream <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // set measure whole notes
  fCurrentMeasureWholeNotesDuration =
    rationalisedCurrentMeasureWholeNotesDuration;
}

void msrMeasure::incrementCurrentMeasureWholeNotesDuration (
  int             inputLineNumber,
  const rational& wholeNotesDelta)
{
  // sanity check
if (false) // JMI
  mfAssert (
    __FILE__, __LINE__,
    wholeNotesDelta.getNumerator () != 0,
    "wholeNotesDelta.getNumerator () == 0");

  // compute the new measure whole notes duration
  rational
    newMeasureWholeNotesDuration =
      fCurrentMeasureWholeNotesDuration + wholeNotesDelta;
  newMeasureWholeNotesDuration.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceWholeNotes ()
  ) {
    gLogStream <<
      "Incrementing current whole notes duration of measure " <<
      this->asShortString ()<<
      " from '"  <<
      fCurrentMeasureWholeNotesDuration <<
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
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // set new measure whole notes duration
  fCurrentMeasureWholeNotesDuration =
    newMeasureWholeNotesDuration;
}

string msrMeasure::currentMeasureWholeNotesDurationAsMsrString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fCurrentMeasureWholeNotesDuration);
}

void msrMeasure::setMeasureKind (
  msrMeasureKind measureKind)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Setting measure kind of measure " <<
        this->asShortString ()<<
        " to '" <<
        msrMeasureKindAsString (measureKind) <<
        "' in segment " <<
        fMeasureSegmentUpLink->asString () <<
        " in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputLineNumber <<
        endl;
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
      if (gGlobalTracingOahGroup->getTraceMeasures ()) {
        gLogStream <<
          "!!! Setting measure kind of measure " <<
          this->asShortString ()<<
          " to '" <<
          msrMeasureKindAsString (measureKind) <<
          "' in segment " <<
          fMeasureSegmentUpLink->asString () <<
          " in voice \"" <<
          fMeasureSegmentUpLink->
            getSegmentVoiceUpLink ()->
              getVoiceName () <<
          "\"" <<
          ", line " << fInputLineNumber <<
          endl;

        gLogStream <<
          endl <<
          "--> this measure:" <<
          endl;
        ++gIndenter;
        this->print (gLogStream);
        gLogStream << endl;
        --gIndenter;
      }
#endif
      break;

    case msrMeasureKind::kMeasureKindCadenza:
      break;

    case msrMeasureKind::kMeasureKindMusicallyEmpty:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMeasures ()) {
        gLogStream <<
          "??? Setting measure kind of measure " <<
          this->asShortString ()<<
          " to '" <<
          msrMeasureKindAsString (measureKind) <<
          "' in segment " <<
          fMeasureSegmentUpLink->asString () <<
          " in voice \"" <<
          fMeasureSegmentUpLink->
            getSegmentVoiceUpLink ()->
              getVoiceName () <<
          "\"" <<
          ", line " << fInputLineNumber <<
          endl;

        gLogStream <<
          endl <<
          "--> this measure:" <<
          endl;
        ++gIndenter;
        this->print (gLogStream);
        gLogStream << endl;
        --gIndenter;
      }
#endif

      break;
  } // switch

  fMeasureKind = measureKind;
}

void msrMeasure::appendPrintLayoutToMeasure (
  S_msrPrintLayout printLayout)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePrintLayouts ()) {
    gLogStream <<
      "Appending print layout " << printLayout->asString () <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureVoiceUpLink ()->
        getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // append it to the measure elements list
  prependOtherElementToMeasure (printLayout);

  // register it for MusicXML generation from MSR
  fMeasurePrintLayout = printLayout;
}

void msrMeasure::appendClefToMeasure (S_msrClef clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef " << clef->asString () <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureVoiceUpLink ()->
        getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // append it to the measure elements list
  appendElementToMeasure (clef);
}

void msrMeasure::appendKeyToMeasure (S_msrKey key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " << key->asString () <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureVoiceUpLink ()->
        getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // append it to the measure elements list
  appendElementToMeasure (key);
}

void msrMeasure::appendTimeSignatureToMeasure (S_msrTimeSignature timeSignature)
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
      endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    gLogStream <<
      "' to measure " <<
      this->asShortString ()<<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // append timeSignature to the measure elements list
  appendElementToMeasure (timeSignature);

  // set the measure whole notes per full measure
  setFullMeasureWholeNotesDurationFromTime (
    timeSignature);

  --gIndenter;
}

void msrMeasure::appendTimeSignatureToMeasureClone (S_msrTimeSignature timeSignature)
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
      endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    S_msrVoice
      voiceUpLink =
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ();

    gLogStream <<
      "to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // append timeSignature to the measure elements list
  appendElementToMeasure (timeSignature);
}

void msrMeasure::insertHiddenMeasureAndBarLineInMeasureClone (
  int             inputLineNumber,
  const rational& positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      positionInMeasure <<
      "' in measure clone \"" <<
      this->asShortString () <<
      "\" in segment clone '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
// JMI      "' in voice \"" <<
//      voiceUpLink->getVoiceName () <<
//      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create a hidden measure and barLine
  S_msrHiddenMeasureAndBarLine
    hiddenMeasureAndBarLine =
      msrHiddenMeasureAndBarLine::create (
        inputLineNumber);

/* JMI BLARK
  // insert it in the measure elements list
  insertElementAtPositionInMeasure (
    inputLineNumber,
    positionInMeasure,
    hiddenMeasureAndBarLine);
    */
}

void msrMeasure::setFullMeasureWholeNotesDurationFromTime (
  S_msrTimeSignature timeSignature)
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
      ||
    gGlobalTracingOahGroup->getTraceMeasures ()
  ) {
    gLogStream <<
      "Setting measure full measure whole notes from time signature:" <<
      endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    gLogStream <<
      "for measure " <<
      this->asShortString ()<<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;
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

        rational
          wholeNotesPerMeasure =
            timeSignature->
              wholeNotesPerMeasure ();

#ifdef TRACING_IS_ENABLED
        if (
          gGlobalTracingOahGroup->getTraceTimeSignatures ()
            ||
          gGlobalTracingOahGroup->getTraceMeasures ()) {
          gLogStream <<
            "There are " <<
            wholeNotesPerMeasure <<
            " whole note(s) per measure in time signature:" <<
            endl;

          ++gIndenter;

          gLogStream <<
            timeSignature;

          --gIndenter;

          gLogStream <<
            "in measure " <<
            this->asShortString () <<
            " in voice \"" <<
            fMeasureSegmentUpLink->
              getSegmentVoiceUpLink ()->
                getVoiceName () <<
            "\"" <<
            ", line " << fInputLineNumber <<
            endl;
        }
#endif

        // set full measure whole notes
        setFullMeasureWholeNotesDuration (
          wholeNotesPerMeasure);

#ifdef TRACING_IS_ENABLED
        if (
          gGlobalTracingOahGroup->getTraceTimeSignatures ()
            ||
          gGlobalTracingOahGroup->getTraceMeasures ()
        ) {
          gLogStream <<
            "Measure " <<
            this->asShortString () <<
            "'in voice \"" <<
            fMeasureSegmentUpLink->
              getSegmentVoiceUpLink ()->
                getVoiceName () <<
            "\"" <<
            " has " <<
            fFullMeasureWholeNotesDuration <<
            " full measure whole notes " <<
            endl;
        }
#endif
      }
      break;

    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:

      // this measure is senza misura

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceTimeSignatures ()
          ||
        gGlobalTracingOahGroup->getTraceMeasures ()
      ) {
        gLogStream <<
          "Measure " <<
          this->asShortString () <<
           " in voice \"" <<
          fMeasureSegmentUpLink->
            getSegmentVoiceUpLink ()->
              getVoiceName () <<
          "\"" <<
          " is senza misura" <<
          endl;
      }
#endif

      setMeasureKind (msrMeasureKind::kMeasureKindCadenza);

      setFullMeasureWholeNotesDuration (
        rational (INT_MAX, 1));
      break;
  } // switch

  --gIndenter;
}

void msrMeasure::appendTransposeToMeasure (
  S_msrTranspose transpose)
{
  // append it to the measure elements list
  appendElementToMeasure (transpose);
}

void msrMeasure::printMeasurePendingMeasureElementsList ()
{
  gLogStream <<
    endl <<
    "===> printMeasurePendingMeasureElementsList ()" <<
    endl;

  ++gIndenter;

  if (fMeasurePendingMeasureElementsList.size ()) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasurePendingMeasureElementsList.begin (),
      iEnd   = fMeasurePendingMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i)->asShortString ();
      if (++i == iEnd) break;
      gLogStream << endl;
    } // for
  }
  else {
    gLogStream <<
      "none" <<
      endl;
  }

  --gIndenter;

  gLogStream <<
    "<===" <<
    endl << endl;
}

void msrMeasure::appendBarLineToMeasure (S_msrBarLine barLine)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

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
      endl;
  }
#endif

  // register barLine's measure number
  barLine->
    setMeasureElementMeasureNumber (
      fMeasureElementMeasureNumber);

  // set barLine's position in measure if relevant
  switch (voice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // register barLine position in measure
      barLine->
        setMeasureElementPositionInMeasure (
          fCurrentMeasureWholeNotesDuration,
          "appendBarLineToMeasure()");
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass: // JMI
      // DON'T set barLine's position in measure here:
      // we need to keep the one set when this same barLine
      // has been appended to the regular voice (just above),
      // so as not to change its relative place in the measure
      // (there are holes in the harmonies voice until
      // they are filled with skips in finalizeHarmoniesInHarmoniesMeasure()
      break;
  } // switch

  fMeasureElementsList.push_back (barLine);
}

void msrMeasure::prependBarLineToMeasure (S_msrBarLine barLine)
{
  // append it to the measure elements list
  fMeasureElementsList.push_front (barLine);
}

void msrMeasure::appendSegnoToMeasure (S_msrSegno segno)
{
  // append it to the measure elements list
  appendElementToMeasure (segno);
}

void msrMeasure::appendCodaToMeasure (S_msrCoda coda)
{
  // append it to the measure elements list
  appendElementToMeasure (coda);
}

void msrMeasure::appendEyeGlassesToMeasure (
  S_msrEyeGlasses eyeGlasses)
{
  // append it to the measure elements list
  appendElementToMeasure (eyeGlasses);
}

void msrMeasure::appendPedalToMeasure (S_msrPedal pedal)
{
  // append it to the measure elements list
  appendElementToMeasure (pedal);
}

void msrMeasure::appendDampToMeasure (S_msrDamp damp)
{
  // append it to the measure elements list
  appendElementToMeasure (damp);
}

void msrMeasure::appendDampAllToMeasure (S_msrDampAll dampAll)
{
  // append it to the measure elements list
  appendElementToMeasure (dampAll);
}

void msrMeasure::appendBarCheckToMeasure (S_msrBarCheck barCheck)
{
  // append it to the measure elements list
  appendElementToMeasure (barCheck);
}

void msrMeasure::appendVoiceStaffChangeToMeasure (
  S_msrVoiceStaffChange voiceStaffChange)
{
  // append it to the measure elements list
  appendElementToMeasure (voiceStaffChange);
}

void msrMeasure::appendNoteToMeasure (
  S_msrNote       note,
  const rational& partCurrentPositionInMeasure)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  rational
    positionsDelta =
      partCurrentPositionInMeasure
        -
      fCurrentMeasureWholeNotesDuration;

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTracePositionsInMeasures ()
  ) {
    gLogStream <<
      "Appending note " << note->asShortString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      ", partCurrentPositionInMeasure: " <<
      partCurrentPositionInMeasure <<
      ", fCurrentMeasureWholeNotesDuration: " <<
      fCurrentMeasureWholeNotesDuration <<
      ", positionsDelta: " <<
      positionsDelta <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // should a skip note be appended before note?
  if (positionsDelta.getNumerator () > 0) {
    // fetch the voice
    S_msrVoice
      voice =
        fetchMeasureVoiceUpLink ();

    // create a skip note of duration wholeNotesDelta
    S_msrNote
      skipNote =
        msrNote::createSkipNote (
          inputLineNumber,
          fMeasureElementMeasureNumber,
          positionsDelta,
          positionsDelta,
          0); // dots number JMI ???
          /* JMI
          voice->
            getVoiceStaffUpLink ()->
              getStaffNumber (),
          voice->
            getVoiceNumber ());
            */

    // append it to the measure
    appendNoteOrPaddingToMeasure (
      skipNote);
  }

  else if (positionsDelta.getNumerator () < 0) {
    stringstream s;

    s <<
      "partCurrentPositionInMeasure " <<
      partCurrentPositionInMeasure <<
      " is smaller than fCurrentMeasureWholeNotesDuration " <<
      fCurrentMeasureWholeNotesDuration <<
      "' in measure " <<
      this->asShortString () <<
      ", cannot padup in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      ", fCurrentMeasureWholeNotesDuration " <<
      fCurrentMeasureWholeNotesDuration <<
      ", partCurrentPositionInMeasure " <<
      partCurrentPositionInMeasure <<
      ", positionsDelta " << positionsDelta <<
      ", line " << inputLineNumber;

 // JMI   msrInternalError (
    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
  //    __FILE__, __LINE__,
      s.str ());
  }

  else {
    // this measure is already at the part current position in measure,
    // nothing to do
  }

  // append note to measure
  appendNoteOrPaddingToMeasure (note);

  // is note a full measure rest?
  if (note->fetchNoteIsAFullNoteRest ()) {
    setMeasureIsAFullMeasureRest ();
  }

  // set grace notes group 'before' elements position in measure in relevant
  S_msrGraceNotesGroup
    noteGraceNotesGroupBefore =
      note->
        getNoteGraceNotesGroupBefore ();

  if (noteGraceNotesGroupBefore) {
    noteGraceNotesGroupBefore->
      setGraceNotesGroupElementsPositionInMeasure (
        this,
        note->getMeasureElementPositionInMeasure ());
  }

  // set grace notes group 'after' elements position in measure in relevant
  S_msrGraceNotesGroup
    noteGraceNotesGroupAfter =
      note->
        getNoteGraceNotesGroupAfter ();

  if (noteGraceNotesGroupAfter) {
    noteGraceNotesGroupAfter->
      setGraceNotesGroupElementsPositionInMeasure (
        this,
        note->getMeasureElementPositionInMeasure ()
         +
        note->getNoteSoundingWholeNotes ());
  }
}

void msrMeasure::appendNoteOrPaddingToMeasure (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // fetch note sounding whole notes
  rational
    noteSoundingWholeNotes =
      note->
        getNoteSoundingWholeNotes ();

  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending note or padding '" << note->asShortString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
       voice->getVoiceName () <<
      "\"" <<
      ", currentMeasureWholeNotesDuration: " <<
      fCurrentMeasureWholeNotesDuration <<
      ", noteSoundingWholeNotes: " << noteSoundingWholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // populate measure upLink
  note->
    setNoteDirectMeasureUpLink (this);

  // set note's measure number
  note->
    setMeasureElementMeasureNumber (
      fMeasureElementMeasureNumber);

  // set the note's position in measure,
  // this can lead to set the position in measure of the harmonies
  // attached to the note
  note->
    setNotePositionInMeasure (
      fCurrentMeasureWholeNotesDuration);

  // append it to the measure elements list
// JMI  appendElementToMeasure (note); ???
  fMeasureElementsList.push_back (note);

  // append it to the measure notes flat list if relevant
  switch (note->getNoteKind ()) {
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      break;

    default:
      appendNoteToMeasureNotesFlatList (note);
  } // switch

  // account for note duration in measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    noteSoundingWholeNotes);

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
      note->getNoteSoundingWholeNotes ()
        >
      fMeasureLongestNote->getNoteSoundingWholeNotes ()
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
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTracePositionsInMeasures ()
  ) {
    gLogStream <<
      "Accounting for the duration of tuplet member note " <<
      note->asShortString () <<
      " in measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // set note's position in measure
  note->
    setNotePositionInMeasure (
      fCurrentMeasureWholeNotesDuration);

  // fetch note sounding whole notes
  rational
    noteSoundingWholeNotes =
      note->getNoteSoundingWholeNotes ();

  // append it to the measure notes flat list
  appendNoteToMeasureNotesFlatList (note);

  // account for note duration in measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    noteSoundingWholeNotes);
}

void msrMeasure::appendPaddingNoteAtTheEndOfMeasure (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceBarLines ()
  ) {
    gLogStream <<
      "Appending padding note " << note->asString () <<
      " at the end of measure " <<
      this->asString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // populate measure upLink
  note->
    setNoteDirectMeasureUpLink (this);

  // append the note at the end of the measure
  appendElementAtTheEndOfMeasure (note);

  // account for note duration in measure whole notes
if (false) // JMI kMeasureKindMusicallyEmpty issue, see basic/EmptyMeasureInAVoice.xml CAFE
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    note->getNoteSoundingWholeNotes ());

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
      note->getNoteSoundingWholeNotes ()
        >
      fMeasureLongestNote->getNoteSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }

    if (
      note->getNoteDisplayWholeNotes ()
        >
      fMeasureLongestNote->getNoteSoundingWholeNotes ()
    ) {
      fMeasureLongestNote = note;
    }
  }

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendNoteToMeasureClone (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note '" << note->asShortString () <<
      "' to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

    /* JMI
  if (appendMeasureIfOverflow (inputLineNumber)) {
    // a new measure has been appended to the segment
    // append note to it via the segment
    fMeasureSegmentUpLink->
      appendNoteToSegment (note);
  }

  else {
  */
    // regular insertion in current measure

    // populate measure upLink
    note->
      setNoteDirectMeasureUpLink (this);

    // append the note to the measure elements list
  // JMI  // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendElementToMeasure (note);

    // fetch note sounding whole notes
    rational
      noteSoundingWholeNotes =
        note->
          getNoteSoundingWholeNotes ();

    // account for note duration in measure whole notes
    incrementCurrentMeasureWholeNotesDuration (
      inputLineNumber,
      noteSoundingWholeNotes);

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
  S_msrDoubleTremolo doubleTremolo)
{
  int inputLineNumber =
    doubleTremolo->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // register doubleTremolo measure number
  doubleTremolo->
    setDoubleTremoloMeasureNumber (
      fMeasureElementMeasureNumber);

  // register doubleTremolo measure position in measure
  doubleTremolo->
    setDoubleTremoloPositionInMeasure (
      fCurrentMeasureWholeNotesDuration);

  // copy measure number to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloMeasureNumber (
      fMeasureElementMeasureNumber);

  // copy measure position to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloPositionInMeasure (
      fCurrentMeasureWholeNotesDuration);

  // append the doubleTremolo to the measure elements list
  appendElementToMeasure (doubleTremolo);

  // fetch doubleTremolo sounding whole notes
  rational
    doubleTremoloSoundingWholeNotes =
      doubleTremolo->
        getDoubleTremoloSoundingWholeNotes ();

  // account for doubleTremolo duration in measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    doubleTremoloSoundingWholeNotes);

  // determine if the doubleTremolo occupies a full measure
// XXL  JMI  if (doubleTremoloSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // doubleTremolo->setDoubleTremoloOccupiesAFullMeasure ();

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendChordToMeasure (S_msrChord chord)
{
  int inputLineNumber =
    chord->getInputLineNumber ();

  // populate measure upLink
  chord->setChordDirectMeasureUpLink (this);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending chord '" << chord->asString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // set the chord kind
  chord->setChordKind (msrChordInKind::kChordInMeasure);

  // append the chord to the measure elements list
  appendElementToMeasure (chord);

  // fetch chord sounding whole notes
  rational
    chordSoundingWholeNotes =
      chord->
        getChordSoundingWholeNotes ();

  // account for chord duration in measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    chordSoundingWholeNotes);

  // determine if the chord occupies a full measure
// XXL  JMI  if (chordSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // chord->setChordOccupiesAFullMeasure ();

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendTupletToMeasure (S_msrTuplet tuplet)
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
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // set the tuplet kind
  tuplet->setTupletKind (msrTupletInKind::kTupletInMeasure);

  // populate measure upLink
  tuplet->setTupletDirectMeasureUpLink (this);

  // append the tuplet to the measure elements list
  appendElementToMeasure (tuplet);

  // the duration of the individual members of the tuplet
  // have already been accounted for in current measure's whole notes

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendHarmonyToMeasure (S_msrHarmony harmony)
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
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\", currentMeasureWholeNotesDuration = " <<
      fCurrentMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // set harmony's measure number
  harmony->
    setMeasureElementMeasureNumber (
      fMeasureElementMeasureNumber);

  // append the harmony to the measure elements list
  // DON'T call 'appendElementToMeasure (harmony)':
  // that would override harmony's position in measure,
  // which already has the correct value, thus:
  fMeasureElementsList.push_back (harmony);

  // get harmony sounding whole notes
  rational
    harmonySoundingWholeNotes =
      harmony->
        getHarmonySoundingWholeNotes ();

  // account for harmony duration in measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    harmonySoundingWholeNotes);

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendHarmonyToMeasureClone (S_msrHarmony harmony)
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
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\", currentMeasureWholeNotesDuration = " <<
      fCurrentMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append the harmony to the measure elements list
  appendElementToMeasure (harmony);

  // get harmony sounding whole notes
  rational
    harmonySoundingWholeNotes =
      harmony->
        getHarmonySoundingWholeNotes ();

  // account for harmony duration in measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    harmonySoundingWholeNotes);

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendFiguredBassElementToMeasure (
  S_msrFiguredBassElement figuredBassElement)
{
  int inputLineNumber =
    figuredBassElement->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass element " << figuredBassElement->asString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\", currentMeasureWholeNotesDuration = " <<
      fCurrentMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // set figuredBassElement's measure number
  figuredBassElement->
    setMeasureElementMeasureNumber (
      fMeasureElementMeasureNumber);

/* JMI
  // append the figured bass to the measure elements list
  appendElementToMeasure (figuredBassElement);
*/

  // append the figuredBassElement to the measure elements list
  // DON'T call 'appendElementToMeasure (figuredBassElement)': // JMI
  // that would override figured bass's position in measure,
  // which already has the correct value, thus:
  fMeasureElementsList.push_back (figuredBassElement);

  // fetch figured bass sounding whole notes
  rational
    figuredBassSoundingWholeNotes =
      figuredBassElement->
        getMeasureElementSoundingWholeNotes ();

  // account for figuredBassElement duration in measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    figuredBassSoundingWholeNotes);

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

void msrMeasure::appendFiguredBassElementToMeasureClone (
  S_msrFiguredBassElement figuredBassElement)
{
  int inputLineNumber =
    figuredBassElement->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass " << figuredBassElement->asString () <<
      " to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\", currentMeasureWholeNotesDuration = " <<
      fCurrentMeasureWholeNotesDuration <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append the figuredBassElement to the measure elements list
  appendElementToMeasure (figuredBassElement);

  // get figuredBassElement sounding whole notes
  rational
    figuredBassSoundingWholeNotes =
      figuredBassElement->
        getMeasureElementSoundingWholeNotes ();

  // account for figuredBassElement duration in measure whole notes
  incrementCurrentMeasureWholeNotesDuration (
    inputLineNumber,
    figuredBassSoundingWholeNotes);

  // this measure contains music
  fMeasureContainsMusic = true;

  --gIndenter;
}

S_msrNote msrMeasure::createPaddingSkipNoteForVoice (
  int             inputLineNumber,
  const rational& duration,
  S_msrVoice      voice)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTracePositionsInMeasures ()
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
      endl;
  }
#endif

  // create a skip note
  S_msrNote
    skipNote =
      msrNote::createSkipNote (
        inputLineNumber,
        fMeasureElementMeasureNumber,
        duration,
        duration,
        0); // dots number JMI ???
        /* JMI
        voice->
          getVoiceStaffUpLink ()->
            getStaffNumber (),
        voice->
          getVoiceNumber ());
          */

  return skipNote;
}

void msrMeasure::padUpToPositionInMeasureInMeasure (
  int             inputLineNumber,
  const rational& wholeNotes)
{
  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) { // JMI
    this->print (gLogStream);

    gLogStream <<
      "Padding from current measure whole notes '" <<
      fCurrentMeasureWholeNotesDuration <<
      "' to '" << wholeNotes <<
      "' in measure " <<
      this->asShortString () <<
      " in segment " <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    wholeNotes.getNumerator () >= 0,
    "wholeNotes.getNumerator () is negative in padUpToPositionInMeasureInMeasure()");

  if (fCurrentMeasureWholeNotesDuration < wholeNotes) {
    ++gIndenter;

    // appending a padding rest or skip to this measure to reach wholeNotes
    rational
      missingDuration =
        wholeNotes - fCurrentMeasureWholeNotesDuration;
    missingDuration.rationalise ();

    // create a padding skip note
    S_msrNote
      paddingNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Appending skip " << paddingNote->asString () <<
        " (missingDuration " << missingDuration <<
        " whole notes) to skip from length '" <<
        fCurrentMeasureWholeNotesDuration <<
        " to length '" << wholeNotes << "'"
        " in measure " <<
        this->asShortString () <<
        " in voice \"" <<
        measureVoice->getVoiceName () <<
        endl;
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

  else if (fCurrentMeasureWholeNotesDuration == wholeNotes) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "No need to pad from current measure whole notes '" <<
        fCurrentMeasureWholeNotesDuration <<
        "' to '" <<
        wholeNotes <<
        "' since they are equal in measure " <<
        this->asShortString () <<
        " in segment " <<
        fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
        " in voice \"" <<
        measureVoice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        endl;
    }
#endif
  }

  else {
    measureVoice->print (gLogStream); // JMI
    this->print (gLogStream);

    stringstream s;

    s <<
        "Cannot pad from current measure whole notes '" <<
        fCurrentMeasureWholeNotesDuration <<
        "' to '" <<
        wholeNotes <<
        "' since the former is larger than the latter in measure " <<
        this->asShortString () <<
        " in segment " <<
        fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
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

void msrMeasure::backupByWholeNotesStepLengthInMeasure ( // JMI USELESS ???
  int             inputLineNumber,
  const rational& backupTargetMeasureElementPositionInMeasure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    backupTargetMeasureElementPositionInMeasure.getNumerator () >= 0,
    "backupTargetMeasureElementPositionInMeasure.getNumerator () is negative");

  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTracePositionsInMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceWholeNotes ()
  ) {
    this->print (gLogStream);

    gLogStream <<
      "Backup by a '" <<
      backupTargetMeasureElementPositionInMeasure <<
      "' whole notes step length in measure " <<
      this->asShortString () <<
      ", currentMeasureWholeNotesDuration: '" <<
      fCurrentMeasureWholeNotesDuration <<
      ", fullMeasureWholeNotesDuration: '" <<
      fFullMeasureWholeNotesDuration <<
      "' in segment " <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  // determine the measure position 'backupTargetMeasureElementPositionInMeasure' backward
  rational
    positionInMeasure =
      fFullMeasureWholeNotesDuration - backupTargetMeasureElementPositionInMeasure;
  positionInMeasure.rationalise ();

  // pad up to it
  padUpToPositionInMeasureInMeasure (
    inputLineNumber,
    positionInMeasure);
}

void msrMeasure::appendPaddingSkipNoteToMeasure (
  int             inputLineNumber,
  const rational& forwardStepLength)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending padding skip note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to measure " <<
      this->asShortString () <<
      " in segment " <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

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
  S_msrTempo tempo)
{
  appendElementToMeasure (tempo);
}

void msrMeasure::appendRehearsalMarkToMeasure (
  S_msrRehearsalMark rehearsalMark)
{
  appendElementToMeasure (rehearsalMark);
}

void msrMeasure::appendOctaveShiftToMeasure (
  S_msrOctaveShift octaveShift)
{
  appendElementToMeasure (octaveShift);
}

void msrMeasure::appendScordaturaToMeasure (
  S_msrScordatura scordatura)
{
  appendElementToMeasure (scordatura);
}

void msrMeasure::appendAccordionRegistrationToMeasure (
  S_msrAccordionRegistration
    accordionRegistration)
{
  appendElementToMeasure (accordionRegistration);
}

void msrMeasure::appendHarpPedalsTuningToMeasure (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
  appendElementToMeasure (harpPedalsTuning);
}

void msrMeasure::appendLineBreakToMeasure (S_msrLineBreak lineBreak)
{
  appendElementToMeasure (lineBreak);
}

void msrMeasure::appendPageBreakToMeasure (S_msrPageBreak pageBreak)
{
  appendElementToMeasure (pageBreak);
}

void msrMeasure::appendStaffDetailsToMeasure (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Appending staff details '" << staffDetails->asShortString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  appendElementToMeasure (staffDetails);
}

void msrMeasure::appendBarNumberCheckToMeasure (
  S_msrBarNumberCheck barNumberCheck)
{
  appendElementToMeasure (barNumberCheck);
}

void msrMeasure::prependOtherElementToMeasure (S_msrMeasureElement elem)
{
  fMeasureElementsList.push_front (elem); // JMI

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::appendOtherElementToMeasure  (S_msrMeasureElement elem)
{
  appendElementToMeasure (elem);

  // this measure contains music
  fMeasureContainsMusic = true;
}

void msrMeasure::removeNoteFromMeasure (
  int       inputLineNumber,
  S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"," <<
      "fMeasureLastHandledNote:" <<
      endl <<
      fMeasureLastHandledNote->asShortString () <<
      endl;
  }
#endif

  for (
    list<S_msrMeasureElement>::iterator i=fMeasureElementsList.begin ();
    i!=fMeasureElementsList.end ();
    ++i
  ) {
    if ((*i) == note) {
      // found note, erase it
      i = fMeasureElementsList.erase (i);

      // update current measure whole notes
      setCurrentMeasureWholeNotesDuration (
        inputLineNumber,
        fCurrentMeasureWholeNotesDuration
          -
        fMeasureLastHandledNote->getNoteSoundingWholeNotes ());

      // return from function
      return;
    }
  } // for

  S_msrVoice
    segmentVoiceUpLink =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  S_msrPart
    segmentVoicePart =
      fetchMeasurePartUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      endl << endl <<
      "@@@@@@@@@@@@@@@@@ segmentVoicePart" <<
      endl <<
      segmentVoicePart <<
      endl <<
      "@@@@@@@@@@@@@@@@@" <<
      endl << endl;

    gLogStream <<
      endl << endl <<
      "@@@@@@@@@@@@@@@@@ segmentVoiceUpLink" <<
      endl <<
      segmentVoiceUpLink <<
      endl <<
      "@@@@@@@@@@@@@@@@@" <<
      endl << endl;
  }
#endif

  stringstream s;

  s <<
    "cannot remove note " <<
    note <<
    " from measure " <<
    this->asShortString () <<
    " in voice \"" <<
    segmentVoiceUpLink->getVoiceName () <<
    "\"," <<
    " since this note has not been found in fMeasureElementsList";

  msrInternalError (
    gGlobalServiceRunData->getInputSourceName (),
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

void msrMeasure::removeElementFromMeasure (
  int          inputLineNumber,
  S_msrElement element)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Removing element:" <<
      endl;

    ++gIndenter;
    gLogStream <<
      " JMI ??? element->elementAsString ()" << endl;
    --gIndenter;

    gLogStream <<
      endl <<
      " from measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\"," <<
      endl;

    ++gIndenter;
    gLogStream <<
      "fMeasureLastHandledNote:" <<
      endl <<
      fMeasureLastHandledNote <<
      endl;
    --gIndenter;
  }
#endif

  for (
    list<S_msrMeasureElement>::iterator i=fMeasureElementsList.begin ();
    i!=fMeasureElementsList.end ();
    ++i
  ) {
    if ((*i) == element) {
      // found element, erase it
      i = fMeasureElementsList.erase (i);

      // update current measure whole notes
      setCurrentMeasureWholeNotesDuration (
        inputLineNumber,
        fCurrentMeasureWholeNotesDuration
          -
        fMeasureLastHandledNote->getNoteSoundingWholeNotes ());

      // return from function
      return;
    }
  } // for

  stringstream s;

  s <<
    "cannot remove element " <<
    element->asString () <<
    " from measure " <<
    this->asShortString () <<
    " in voice \"" <<
    fMeasureSegmentUpLink->
      getSegmentVoiceUpLink ()->
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
  S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "Appending note " <<
      note->asShortStringForMeasuresSlices () <<
      " to the notes flat list of measure " <<
      this->asShortString () <<
      endl;
  }
#endif

  fMeasureNotesFlatList.push_back (note);
}

void msrMeasure::determineMeasureKindAndPuristNumber (
  int     inputLineNumber,
  msrMeasure::msrMeasureRepeatContextKind
          measureRepeatContextKind)
{
  if (fMeasureKindAndPuristNumberHaveBeenDetermined) {
    stringstream s;

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
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // get voice after repeat component phase kind
  msrVoiceRepeatPhaseKind
    currentVoiceRepeatPhaseKind =
      voice->
        getCurrentVoiceRepeatPhaseKind ();

  // regular measure ends detection
  rational
    wholeNotesSinceLastRegularMeasureEnd =
      voice->
        getWholeNotesSinceLastRegularMeasureEnd ();

  rational
    newWholeNotesSinceLastRegularMeasureEnd =
      wholeNotesSinceLastRegularMeasureEnd
        +
      fCurrentMeasureWholeNotesDuration;
   newWholeNotesSinceLastRegularMeasureEnd.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Determining the measure kind and purist number of measure " <<
      this->asShortString () <<
      ", fullMeasureWholeNotesDuration: " <<
      fFullMeasureWholeNotesDuration <<
      "', currentMeasureWholeNotesDuration: " <<
      fCurrentMeasureWholeNotesDuration <<
      ", wholeNotesSinceLastRegularMeasureEnd: " <<
      wholeNotesSinceLastRegularMeasureEnd <<
      ", newWholeNotesSinceLastRegularMeasureEnd: " <<
      newWholeNotesSinceLastRegularMeasureEnd <<
      ", currentVoiceRepeatPhaseKind: " <<
      voiceRepeatPhaseKindAsString (
        currentVoiceRepeatPhaseKind) <<
    "' in voice \"" << voice->getVoiceName () <<
    ", line " << inputLineNumber <<
    endl;
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

  if (fCurrentMeasureWholeNotesDuration.getNumerator () == 0) {
    // empty measure

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
      voice->
        displayVoiceRepeatsStackFullMeasureRestsMeasureRepeatAndVoice (
          inputLineNumber,
          "determineMeasureKindAndPuristNumber() 2 measure has 0 current measure whole notes");

      stringstream s;

      s <<
        "measure " <<
        this->asShortString () <<
        " has 0 current measure whole notes" <<
        ", " <<
        asString () <<
        ", line " << inputLineNumber;

      if (false) // JMI
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

  else if (fCurrentMeasureWholeNotesDuration == fFullMeasureWholeNotesDuration) {
    // this is a regular measure

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

  else {
    // this measure is incomplete or overfull

    // increment voice whole notes since last regular measure end
    voice->
      setWholeNotesSinceLastRegularMeasureEnd (
        inputLineNumber,
        newWholeNotesSinceLastRegularMeasureEnd);

    if (fCurrentMeasureWholeNotesDuration < fFullMeasureWholeNotesDuration) {
      //  this is an incomplete measure

      if (fMeasureFirstInVoice) {
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
          case msrMeasure::kMeasureRepeatContextKindUnknown: // JMI ???
            {
              stringstream s;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
              displayMeasure (
                inputLineNumber,
                "determineMeasureKindAndPuristNumber() 6 kMeasureRepeatContextKindUnknown");
  }
#endif

              s <<
                "measure " <<
                this->asShortString () <<
                " is kMeasureRepeatContextKindUnknown " <<
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

          case msrMeasure::kMeasureRepeatContextKindNone:
            // update the voice current measure purist number if relevant
            switch (fMeasureEndRegularKind) {
              case msrMeasure::kMeasureEndRegularKindUnknown:
                // JMI ???
                break;

              case msrMeasure::kMeasureEndRegularKindYes:
                // set it's measure kind
                setMeasureKind (
                  msrMeasureKind::kMeasureKindIncompleteStandalone);

                // don't increment the voice current measure purist number,
                // this has already been done for the 'first part' of the measure
                break;

              case msrMeasure::kMeasureEndRegularKindNo:
                // set it's measure kind
                setMeasureKind (
                  msrMeasureKind::kMeasureKindIncompleteStandalone);
                break;
            } // switch
            break;

          case msrMeasure::kMeasureRepeatContextKindCommonPartLastMeasure:
            setMeasureKind (
              msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart);
            break;
          case msrMeasure::kMeasureRepeatContextKindHookedEndingLastMeasure:
            setMeasureKind (
              msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding);
            break;
          case msrMeasure::kMeasureRepeatContextKindHooklessEndingLastMeasure:
            setMeasureKind (
              msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding);
            break;

          case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterCommonPart:
            setMeasureKind (
              msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart);
            break;
          case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterHookedEnding:
            setMeasureKind (
              msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding);
            break;
          case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterHooklessEnding:
            setMeasureKind (
              msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding);
            break;
        } // switch
      }
    }

    else { // fCurrentMeasureWholeNotesDuration > fFullMeasureWholeNotesDuration
      // this is an overfull measure

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

void msrMeasure::padUpToPositionInMeasure (
  int             inputLineNumber,
  const rational& positionInMeasureToPadUpTo)
{
  // fetch the voice
  S_msrVoice
    measureVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Padding up to position '" <<
      positionInMeasureToPadUpTo <<
      "' in measure " <<
      this->asShortString () <<
      " in segment " <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionInMeasure() 1");
  }
#endif

  ++gIndenter;

  if (fCurrentMeasureWholeNotesDuration < positionInMeasureToPadUpTo) {
    // appending a rest to this measure to reach positionInMeasureToPadUpTo
    rational
      missingDuration =
        positionInMeasureToPadUpTo - fCurrentMeasureWholeNotesDuration;
    missingDuration.rationalise ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
       "Creating a padding note for measure debug number " <<
       fMeasureDebugNumber <<
       ", missingDuration: " << missingDuration <<
       " in voice \"" << measureVoice->getVoiceName () <<
       "\", measure: " <<
        this->asShortString () <<
       ", currentMeasureWholeNotesDuration: " <<
       fCurrentMeasureWholeNotesDuration <<
       endl;
   }
#endif

    // create a padding skip note
    S_msrNote
      paddingNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
       "Appending " << paddingNote->asString () <<
       " (" << missingDuration << " whole notes)" <<
       " to finalize \"" << measureVoice->getVoiceName () <<
       "\" measure: " <<
       this->asShortString () <<
       ", currentMeasureWholeNotesDuration: " <<
       fCurrentMeasureWholeNotesDuration <<
       endl;
   }
#endif

    // append the paddingNote to the measure
    appendNoteOrPaddingToMeasure (paddingNote);

    // this measure contains music
    fMeasureContainsMusic = true;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionInMeasure() 2");
  }
#endif

  --gIndenter;
}

void msrMeasure::padUpToPositionAtTheEndOfTheMeasure (
  int             inputLineNumber,
  const rational& positionInMeasureToPadUpTo)
{
  // fetch the voice
  S_msrVoice
    measureVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Padding up from position " <<
      fCurrentMeasureWholeNotesDuration <<
      " to position '" <<
      positionInMeasureToPadUpTo <<
      "' at the end of measure " <<
      this->asString () <<
      " in segment " <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
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

  if (fCurrentMeasureWholeNotesDuration < positionInMeasureToPadUpTo) {
    // appending a rest to this measure to reach positionInMeasureToPadUpTo
    rational
      missingDuration =
        positionInMeasureToPadUpTo - fCurrentMeasureWholeNotesDuration;
    missingDuration.rationalise ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Creating a padding note" <<
        ", missingDuration: " << missingDuration <<
        " at the end of measure " <<
        this->asString () <<
        " in voice \"" << measureVoice->getVoiceName () << "\",  " <<
        ", currentMeasureWholeNotesDuration: " <<
        fCurrentMeasureWholeNotesDuration <<
        ", line " << inputLineNumber <<
        endl;
   }
#endif

//     gLogStream << JMI
//       endl << endl <<
//       "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" <<
//       endl << endl;
//     this->
//       print (gLogStream);
//     gLogStream <<
//       endl << endl <<
//       "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" <<
//       endl << endl;

    // create a padding skip note
    S_msrNote
      paddingNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          missingDuration,
          measureVoice);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
       "Appending padding note " << paddingNote->asString () <<
       " (" << missingDuration << " whole notes)" <<
       " to finalize \"" << measureVoice->getVoiceName () <<
       "\" measure: " <<
       this->asShortString () <<
       " currentMeasureWholeNotesDuration: " <<
       fCurrentMeasureWholeNotesDuration <<
       endl;
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

void msrMeasure::finalizeRegularMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  const string&               context)
{
  // fetch the regular voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // fetch the regular voice's part
  S_msrPart
    regularPart =
      voice->
        fetchVoicePartUpLink ();

  mfAssert (
    __FILE__, __LINE__,
    regularPart != nullptr,
    "regularPart is null");

  if (false) { // JMI
    gLogStream <<
      "---> regularPart: " <<
      endl;

    ++gIndenter;
    gLogStream <<
      regularPart <<
      endl;
    --gIndenter;
    gLogStream << endl;
  }

  rational
    measureWholeNotesDurationFromPartMeasuresVector =
      regularPart->
        fetchPartMeasuresWholeNotesDurationsVectorAt (
          inputLineNumber,
          fMeasureOrdinalNumberInVoice - 1);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing regular measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in regular voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", measureWholeNotesDurationFromPartMeasuresVector: " <<
      measureWholeNotesDurationFromPartMeasuresVector <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeRegularMeasure() 1");
  }
#endif

  padUpToPositionAtTheEndOfTheMeasure (
    inputLineNumber,
    measureWholeNotesDurationFromPartMeasuresVector);

  // register this measures's length in the part
  S_msrPart
    part =
      this->fetchMeasurePartUpLink ();

  part->
    registerOrdinalMeasureNumberWholeNotesDuration (
      inputLineNumber,
      fMeasureOrdinalNumberInVoice,
      fCurrentMeasureWholeNotesDuration);

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
      fMeasureLongestNote-> getNoteSoundingWholeNotes ()
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
          fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
          "' in voice \"" <<
          voice->getVoiceName () <<
          "\", line " << inputLineNumber <<
          endl;
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
  int          inputLineNumber,
  S_msrVoice   voice,
  S_msrHarmony currentHarmony)
{
  // currentHarmony is the first harmony in the measure

  // get currentHarmony' position in the measure
  rational
    currentHarmonyPositionInMeasure =
      currentHarmony->
        getMeasureElementPositionInMeasure ();

  // the position to pad up to is the minimum
  // of those of the currentHarmony and currentHarmonyNoteUpLink,
  // to keep comparison points between the regular voice and its harmonies voice
  rational
    positionInMeasureToPadUpTo =
// JMI        currentHarmonyNoteUpLinkPositionInMeasure;
      currentHarmonyPositionInMeasure;

/* JMI
  if (
    currentHarmonyPositionInMeasure
      <
    currentHarmonyNoteUpLinkPositionInMeasure
  ) {
    positionInMeasureToPadUpTo =
      currentHarmonyPositionInMeasure;
  }
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "handleFirstHarmonyInHarmoniesMeasure() 1" <<
      ", currentHarmonyPositionInMeasure: " <<
      currentHarmonyPositionInMeasure <<
      ", positionInMeasureToPadUpTo: " <<
      positionInMeasureToPadUpTo <<
      endl;
  }
#endif

//   // is a padding skip note needed?
//   if (positionInMeasureToPadUpTo.getNumerator () != 0) {
//     // create a padding skip note
//     S_msrNote
//       skipNote =
//         createPaddingSkipNoteForVoice (
//           inputLineNumber,
//           positionInMeasureToPadUpTo,
//           voice);
//
//     // insert skipNote before currentHarmony in the measure's elements list
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
//       gLogStream <<
//         "Inserting first padding note " <<
//         skipNote->asString () <<
//         " before currentHarmony " <<
//         currentHarmony->asString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         endl;
//     }
// #endif
//
//     // insert skipNote in the measure elements list before (*i)
//     insertElementInMeasureBeforeIterator (
//       inputLineNumber,
//       i,
//       skipNote);
//
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
//       displayMeasure (
//         inputLineNumber,
//         "handleFirstHarmonyInHarmoniesMeasure() 2");
//     }
// #endif
//   }
}

void msrMeasure::handleSubsequentHarmonyInHarmoniesMeasure (
  int          inputLineNumber,
  S_msrVoice   voice,
  S_msrHarmony previousHarmony,
  S_msrHarmony currentHarmony)
{
  // currentHarmony is a subsequent harmony in the measure,
  // occurring after previousHarmony

  // get currentHarmony' position in the measure
  rational
    currentHarmonyPositionInMeasure =
      currentHarmony->
        getMeasureElementPositionInMeasure ();

  // get previousHarmony's position in the measure
  rational
    previousHarmonyPositionInMeasure =
      previousHarmony->getMeasureElementPositionInMeasure ();

  // get previousHarmony's duration
  rational
    previousHarmonySoundingWholeNotes =
      previousHarmony->getHarmonySoundingWholeNotes ();

  // compute the position in measure following previousHarmony
  rational
    positionInMeasureFollowingPreviousHarmony =
      previousHarmonyPositionInMeasure
        +
      previousHarmonySoundingWholeNotes;
  positionInMeasureFollowingPreviousHarmony.rationalise ();

  // compute the positions in measure delta
  // between previousHarmony and currentHarmony
  rational
    positionsInMeasureDelta =
      currentHarmonyPositionInMeasure
        -
      positionInMeasureFollowingPreviousHarmony;
  positionsInMeasureDelta.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "handleSubsequentHarmonyInHarmoniesMeasure() 1" <<
      ", previousHarmony: ";

    if (previousHarmony) {
      gLogStream <<
        previousHarmony->asString ();
    }
    else {
      gLogStream << "none";
    }

    gLogStream <<
      ", currentHarmony: " <<
      currentHarmony->asString () <<
      ", previousHarmonyPositionInMeasure: " <<
      previousHarmonyPositionInMeasure <<
      ", currentHarmonyPositionInMeasure: " <<
      currentHarmonyPositionInMeasure <<
      ", positionInMeasureFollowingPreviousHarmony: " <<
      positionInMeasureFollowingPreviousHarmony <<
      ", positionsInMeasureDelta: " <<
      positionsInMeasureDelta <<
      endl;
  }
#endif

  // update the previous harmony sounding whole notes duration if relevant
  // to 'fill the gap' to the current harmony
  if (previousHarmony) {
    rational
      newPreviousHarmonyWholeNotes =
        currentHarmony->getMeasureElementPositionInMeasure ()
          -
        previousHarmony->getMeasureElementPositionInMeasure ();
    newPreviousHarmonyWholeNotes.rationalise ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Setting the sounding whole notes duration of harmony " <<
        previousHarmony->asString () <<
        " to " <<
        newPreviousHarmonyWholeNotes <<
        endl;
    }
#endif

    previousHarmony->
      setHarmonySoundingWholeNotes (
        newPreviousHarmonyWholeNotes);
  }
}

void msrMeasure::postHandleCurrentHarmonyInHarmoniesMeasure (
  int          inputLineNumber,
  S_msrVoice   voice,
  S_msrHarmony currentHarmony)
{
  // does currentHarmony overflow the measure?

  // get the currentHarmony's position in the measure
  rational
    currentHarmonyPositionInMeasure =
      currentHarmony->
        getMeasureElementPositionInMeasure ();

  // get the currentHarmony's sounding whole notes
  rational
    currentHarmonySoundingWholeNotes =
      currentHarmony->
        getHarmonySoundingWholeNotes ();

  // compute the position in measure following currentHarmony
  rational
    positionInMeasureFollowingCurrentHarmony =
      currentHarmonyPositionInMeasure
        +
      currentHarmonySoundingWholeNotes;
  positionInMeasureFollowingCurrentHarmony.rationalise ();

  // get the currentHarmony's note uplink
  S_msrNote
    currentHarmonyNoteUpLink  =
      currentHarmony->
        getHarmonyNoteUpLink ();

  // compute the measure overflow whole notes
  rational
    measureOverflowWholeNotes =
      positionInMeasureFollowingCurrentHarmony
        -
      fFullMeasureWholeNotesDuration;
  measureOverflowWholeNotes.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "--> postHandleCurrentHarmonyInHarmoniesMeasure():" <<
      endl;

    ++gIndenter;

    gLogStream <<
      "currentHarmony: ";

    if (currentHarmony) {
      gLogStream <<
        currentHarmony->asString ();
    }
    else {
      gLogStream << "none";
    }

    gLogStream <<
      endl <<
      "currentHarmonyNoteUpLink:" <<
      endl;
    ++gIndenter;
    gLogStream <<
      currentHarmonyNoteUpLink->asString () <<
      endl;
    --gIndenter;

    gLogStream <<
      ", currentHarmonyPositionInMeasure: " <<
      currentHarmonyPositionInMeasure <<
      ", currentHarmonySoundingWholeNotes: " <<
      currentHarmonySoundingWholeNotes <<
      ", positionInMeasureFollowingCurrentHarmony: " <<
      positionInMeasureFollowingCurrentHarmony <<
      /* JMI
      ", positionInMeasureFollowingCurrentHarmonyNoteUpLink: " <<
      positionInMeasureFollowingCurrentHarmonyNoteUpLink <<
      ", currentHarmonyNoteUpLinkSoundingWholeNotes: " <<
      currentHarmonyNoteUpLinkSoundingWholeNotes <<
      ", positionInMeasureFollowingCurrentHarmonyNoteUpLink: " <<
      positionInMeasureFollowingCurrentHarmonyNoteUpLink <<
      */
      ", currentHarmonySoundingWholeNotes: " <<
      currentHarmonySoundingWholeNotes <<
      ", measureOverflowWholeNotes: " <<
      measureOverflowWholeNotes <<
      endl;

    --gIndenter;
  }
#endif

  if (measureOverflowWholeNotes.getNumerator () < 0) {
    // the currentHarmony's duration is too short,
    // i.e. there are notes after the one that bears it

    rational
      wholeNotesDelta =
        rational (0) - measureOverflowWholeNotes; // JMI SAXO

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
        endl;
    }
#endif

    incrementCurrentMeasureWholeNotesDuration (
      inputLineNumber,
      wholeNotesDelta);
  }

  else if (measureOverflowWholeNotes.getNumerator () > 0) {
    // the currentHarmony's duration is too large
    stringstream s;

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
    rational
      reducedSoundingWholeNotes =
        currentHarmonySoundingWholeNotes
          - // the delta is positive
        measureOverflowWholeNotes;
    reducedSoundingWholeNotes.rationalise ();

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
        endl;
    }
#endif

    if (reducedSoundingWholeNotes.getNumerator () == 0) {
      stringstream s;

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
        setHarmonySoundingWholeNotes (
          reducedSoundingWholeNotes);
    }
  }

  else {
    // the currentHarmony's reached the end of the measure,
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
        endl;
    }
#endif
  }
}

void msrMeasure::finalizeHarmoniesInHarmoniesMeasure (
  int           inputLineNumber,
  const string& context)
{
  // running this method for each and every measure in turn
  // in harmonies voices is actually a partial pass

  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Finalizing harmonies in harmonies measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fMeasureElementsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeHarmoniesInHarmoniesMeasure() 1");
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
        fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
        "' in voice \"" <<
        voice->getVoiceName () <<
        "\" (" << context << ")" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    fMeasureElementsList.sort (
      msrMeasureElement::compareMeasureElementsByIncreasingPositionInMeasure);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeHarmoniesInHarmoniesMeasure() 2");
    }
#endif

    // consider each measure element (a harmony in fact) in turn,
    // updating their duration and adding skips if needed
    list<S_msrMeasureElement>::iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;

    S_msrHarmony
      previousHarmony = nullptr,
      currentHarmony  = nullptr;

    for ( ; ; ) {
      S_msrMeasureElement
        measureElement = (*i);

      int inputLineNumber =
        measureElement->
          getInputLineNumber ();

      if (
        // harmony?
        // don't assign currentHarmony here yet,
        // this would set it to nullptr if there's anything else
        // after the last harmony in the voice JMI ???
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
//         if (note-> ()) { JMI
          stringstream s;

          s <<
            "Finalizing a harmony voice measure containing note " <<
            note->asShortString () <<
            endl;

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
//         }
      }

      if (++i == iEnd) break; // JMI
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
        endl;
    }
    */

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeHarmoniesInHarmoniesMeasure() 3");
    }
#endif
  }
}

void msrMeasure::finalizeHarmonyInHarmoniesMeasure (
  int           inputLineNumber,
  S_msrVoice    voice,
  S_msrHarmony  previousHarmony,
  S_msrHarmony  currentHarmony,
  const string& context)
{
  // handle the currentHarmony
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Handlig harmony " <<
      currentHarmony->asString () <<
      " while finalizing harmonies measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // get currentHarmony's note uplink
  S_msrNote
    currentHarmonyNoteUpLink  =
      currentHarmony->
        getHarmonyNoteUpLink ();

#ifdef TRACING_IS_ENABLED
  // get the currentHarmony's note uplink position in the measure
  rational
    currentHarmonyNoteUpLinkPositionInMeasure =
      currentHarmonyNoteUpLink->
        getMeasureElementPositionInMeasure ();

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "finalizeHarmonyInHarmoniesMeasure() 2" <<
      ", previousHarmony: ";

    if (previousHarmony) {
      gLogStream <<
        previousHarmony->asString ();
    }
    else {
      gLogStream << "none";
    }

    gLogStream <<
      ", currentHarmonyNoteUpLinkPositionInMeasure: " <<
      currentHarmonyNoteUpLinkPositionInMeasure <<
      endl;
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

void msrMeasure::handleFirstFiguredBassElementInFiguredBassMeasure (
  int                     inputLineNumber,
  S_msrVoice              voice,
  list<S_msrMeasureElement>::iterator&
                          i,
  S_msrFiguredBassElement previousFiguredBass,
  S_msrFiguredBassElement currentFiguredBass,
  const rational&         currentFiguredBassPositionInMeasure)
{
  // currentFiguredBass is the first figured bass in the measure

  // the position to pad up to is the minimum
  // of those of the currentFiguredBass and currentFiguredBassNoteUpLink,
  // to keep comparison points between the regular voice and its figured bass voice
  rational
    positionInMeasureToPadUpTo =
// JMI        currentFiguredBassNoteUpLinkPositionInMeasure;
      currentFiguredBassPositionInMeasure;

/* JMI
  if (
    currentFiguredBassPositionInMeasure
      <
    currentFiguredBassNoteUpLinkPositionInMeasure
  ) {
    positionInMeasureToPadUpTo =
      currentFiguredBassPositionInMeasure;
  }
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "handleFirstFiguredBassElementInFiguredBassMeasure() 5" <<
      ", previousFiguredBass is null, positionInMeasureToPadUpTo: " <<
      positionInMeasureToPadUpTo <<
      endl;
  }
#endif

  // is a padding skip note needed?
  if (positionInMeasureToPadUpTo.getNumerator () != 0) {
    // create a padding skip note
    S_msrNote
      skipNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          positionInMeasureToPadUpTo,
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
        endl;
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
        "handleFirstFiguredBassElementInFiguredBassMeasure() 6");
    }
#endif
  }
}

void msrMeasure::handleSubsequentFiguredBassElementInFiguredBassMeasure (
  int                    inputLineNumber,
  S_msrVoice              voice,
  list<S_msrMeasureElement>::iterator&
                          i,
  S_msrFiguredBassElement previousFiguredBass,
  S_msrFiguredBassElement currentFiguredBass,
  const rational&         currentFiguredBassPositionInMeasure)
{
  // this is a subsequent figured bass in the measure

  // get the previousFiguredBass's position in the measure
  rational
    previousFiguredBassPositionInMeasure =
      previousFiguredBass->getMeasureElementPositionInMeasure ();

  // get the previousFiguredBass's duration
  rational
    previousFiguredBassSoundingWholeNotes =
      previousFiguredBass->getMeasureElementSoundingWholeNotes ();

  // compute the position in measure following previousFiguredBass
  rational
    positionInMeasureFollowingPreviousFiguredBass =
      previousFiguredBassPositionInMeasure
        +
      previousFiguredBassSoundingWholeNotes;
  positionInMeasureFollowingPreviousFiguredBass.rationalise ();

  // compute the positions in measure delta
  rational
    positionsInMeasureDelta =
      currentFiguredBassPositionInMeasure
        -
      positionInMeasureFollowingPreviousFiguredBass;
  positionsInMeasureDelta.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "handleSubsequentFiguredBassElementInFiguredBassMeasure() 7" <<
      ", previousFiguredBass: ";

    if (previousFiguredBass) {
      gLogStream <<
        previousFiguredBass->asString ();
    }
    else {
      gLogStream << "none";
    }

    gLogStream <<
      ", currentFiguredBass: " <<
      currentFiguredBass->asString () <<
      ", previousFiguredBassPositionInMeasure: " <<
      previousFiguredBassPositionInMeasure <<
      ", currentFiguredBassPositionInMeasure: " <<
      currentFiguredBassPositionInMeasure <<
      ", positionInMeasureFollowingPreviousFiguredBass: " <<
      positionInMeasureFollowingPreviousFiguredBass <<
      ", positionsInMeasureDelta: " <<
      positionsInMeasureDelta <<
      endl;
  }
#endif

  // is a padding skip note needed?
  if (positionsInMeasureDelta.getNumerator () > 0) {
    // create a padding skip note
    S_msrNote
      skipNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          positionsInMeasureDelta,
          voice);

    // set its position in measure
    skipNote->
      setMeasureElementPositionInMeasure (
        fCurrentMeasureWholeNotesDuration,
        "handleSubsequentFiguredBassElementInFiguredBassMeasure() 8");

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
        endl;
    }
#endif

    // insert skipNote in the measure elements list before (*i)
    insertElementInMeasureBeforeIterator (
      inputLineNumber,
      i,
      skipNote);
  }

  else if (positionsInMeasureDelta.getNumerator () < 0) {
    // the two harmonies overlap in time
    stringstream s;

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
    rational
      reducedSoundingWholeNotes =
        previousFiguredBassSoundingWholeNotes
          + // the delta is negative
        positionsInMeasureDelta;
    reducedSoundingWholeNotes.rationalise ();

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
        endl;
    }
#endif

    if (reducedSoundingWholeNotes.getNumerator () == 0) {
      stringstream s;

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
          "handleSubsequentFiguredBassElementInFiguredBassMeasure ()");
    }
  }
}

void msrMeasure::postHandleCurrentFiguredBassElementInFiguredBassMeasure (
  int                     inputLineNumber,
  S_msrVoice              voice,
  S_msrFiguredBassElement currentFiguredBass)
{
  // does currentFiguredBass overflow the measure?

  // get the currentFiguredBass's position in the measure
  rational
    currentFiguredBassPositionInMeasure =
      currentFiguredBass->getMeasureElementPositionInMeasure ();

  // get the currentFiguredBass's sounding whole notes
  rational
    currentFiguredBassSoundingWholeNotes =
      currentFiguredBass->
        getMeasureElementSoundingWholeNotes ();

  // compute the position in measure following currentFiguredBass
  rational
    positionInMeasureFollowingCurrentFiguredBass =
      currentFiguredBassPositionInMeasure
        +
      currentFiguredBassSoundingWholeNotes;
  positionInMeasureFollowingCurrentFiguredBass.rationalise ();

  // get the currentFiguredBass's note uplink
  S_msrNote
    currentFiguredBassNoteUpLink  =
      currentFiguredBass->
        getFiguredBassNoteUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "postHandleCurrentFiguredBassElementInFiguredBassMeasure() 1" <<
      ", currentFiguredBassNoteUpLink:" <<
      endl;
    ++gIndenter;
    gLogStream <<
      currentFiguredBassNoteUpLink <<
      endl;
    --gIndenter;
  }
#endif

  // compute the measure overflow whole notes
  rational
    measureOverflowWholeNotes =
      positionInMeasureFollowingCurrentFiguredBass
        -
      fFullMeasureWholeNotesDuration;
  measureOverflowWholeNotes.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "postHandleCurrentFiguredBassElementInFiguredBassMeasure() 2" <<
      ", currentFiguredBass: ";

    if (currentFiguredBass) {
      gLogStream <<
        currentFiguredBass->asString ();
    }
    else {
      gLogStream << "none";
    }

    gLogStream <<
      ", currentFiguredBassPositionInMeasure: " <<
      currentFiguredBassPositionInMeasure <<
      ", currentFiguredBassSoundingWholeNotes: " <<
      currentFiguredBassSoundingWholeNotes <<
      ", positionInMeasureFollowingCurrentFiguredBass: " <<
      positionInMeasureFollowingCurrentFiguredBass <<
      /* JMI
      ", positionInMeasureFollowingCurrentFiguredBassNoteUpLink: " <<
      positionInMeasureFollowingCurrentFiguredBassNoteUpLink <<
      ", currentFiguredBassNoteUpLinkSoundingWholeNotes: " <<
      currentFiguredBassNoteUpLinkSoundingWholeNotes <<
      ", positionInMeasureFollowingCurrentFiguredBassNoteUpLink: " <<
      positionInMeasureFollowingCurrentFiguredBassNoteUpLink <<
      */
      ", currentFiguredBassSoundingWholeNotes: " <<
      currentFiguredBassSoundingWholeNotes <<
      ", measureOverflowWholeNotes: " <<
      measureOverflowWholeNotes <<
      endl;
  }
#endif

  if (measureOverflowWholeNotes.getNumerator () > 0) {
    // the currentFiguredBass's duration is too large
    stringstream s;

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
    rational
      reducedSoundingWholeNotes =
        currentFiguredBassSoundingWholeNotes
          - // the delta is positive
        measureOverflowWholeNotes;
    reducedSoundingWholeNotes.rationalise ();

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
        endl;
    }
#endif

    if (reducedSoundingWholeNotes.getNumerator () == 0) {
      stringstream s;

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
          "postHandleCurrentFiguredBassElementInFiguredBassMeasure () 3");
    }
  }
}

void msrMeasure::finalizeFiguredBassElementsInFiguredBassMeasure (
  int           inputLineNumber,
  const string& context)
{
  // running this method for each and every measure in turn
  // in figured bass voices is actually a partial pass

  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Finalizing harmonies in figured bass measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fMeasureElementsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeFiguredBassElementsInFiguredBassMeasure() 1");
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
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

    fMeasureElementsList.sort (
      msrMeasureElement::compareMeasureElementsByIncreasingPositionInMeasure);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeFiguredBassElementsInFiguredBassMeasure() 2");
    }
#endif

    // consider each measure element (a figured bass in fact) in turn,
    // updating their duration and adding skips if needed
    list<S_msrMeasureElement>::iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;

    S_msrFiguredBassElement
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
        S_msrFiguredBassElement
          figuredBassElement =
            dynamic_cast<msrFiguredBassElement*>(&(*measureElement))
      ) {
        currentFiguredBass = figuredBassElement;

        // handle the currentFiguredBass
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
          gLogStream <<
            "finalizeFiguredBassElementsInFiguredBassMeasure() 3" <<
            ", currentFiguredBass: ";
            ++gIndenter;
            gLogStream <<
              currentFiguredBass->asString () <<
              endl;
            --gIndenter;
        }
#endif

        // its position in the measure should take it's offset into account
        rational
          currentFiguredBassPositionInMeasure =
            currentFiguredBass->
              getMeasureElementPositionInMeasure ();

        // get the currentFiguredBass's note uplink
        S_msrNote
          currentFiguredBassNoteUpLink  =
            currentFiguredBass->
              getFiguredBassNoteUpLink ();

#ifdef TRACING_IS_ENABLED
        // get the currentFiguredBass's note uplink position in the measure
        rational
          currentFiguredBassNoteUpLinkPositionInMeasure =
            currentFiguredBassNoteUpLink->
              getMeasureElementPositionInMeasure ();

        if (gGlobalTracingOahGroup->getTraceHarmonies () ) {
          gLogStream <<
            "finalizeFiguredBassElementsInFiguredBassMeasure() 4" <<
            ", previousFiguredBass: ";

          if (previousFiguredBass) {
            gLogStream <<
              previousFiguredBass->asString ();
          }
          else {
            gLogStream << "none";
          }

          gLogStream <<
            ", currentFiguredBassPositionInMeasure: " <<
            currentFiguredBassPositionInMeasure <<
            ", currentFiguredBassNoteUpLinkPositionInMeasure: " <<
            currentFiguredBassNoteUpLinkPositionInMeasure <<
            endl;
        }
#endif

        if (! previousFiguredBass) {
          handleFirstFiguredBassElementInFiguredBassMeasure (
            inputLineNumber,
            voice,
            i,
            previousFiguredBass,
            currentFiguredBass,
            currentFiguredBassPositionInMeasure);
        }

        else {
          handleSubsequentFiguredBassElementInFiguredBassMeasure (
            inputLineNumber,
            voice,
            i,
            previousFiguredBass,
            currentFiguredBass,
            currentFiguredBassPositionInMeasure);
        }

        previousFiguredBass = currentFiguredBass;
      }

      if (++i == iEnd) break;
    } // while

    if (currentFiguredBass) {
      postHandleCurrentFiguredBassElementInFiguredBassMeasure (
        inputLineNumber,
        voice,
        currentFiguredBass);
    }
/* JMI
    else {
      gLogStream <<
        "*** currentFiguredBass is null ***" << // JMI
        endl;
    }
    */

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeFiguredBassElementsInFiguredBassMeasure() 5");
    }
#endif
  }
}

void msrMeasure::finalizeHarmoniesMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  const string&               context)
{
  // fetch the harmonies voice
  S_msrVoice
    harmoniesVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // fetch the harmonies part
  S_msrPart
    harmoniesPart =
      harmoniesVoice->
        fetchVoicePartUpLink ();

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
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in harmonies voice \"" <<
      harmoniesVoice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeHarmoniesMeasure() 1");
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
      "fMeasureOrdinalNumberInVoice = " <<
      fMeasureOrdinalNumberInVoice <<
      ", harmoniesPartNumberOfMeasures = " <<
      harmoniesPartNumberOfMeasures <<
      endl;
  }
#endif

  // handle the harmonies in this measure
  finalizeHarmoniesInHarmoniesMeasure (
    inputLineNumber,
    context);

  // the measureWholeNotesDuration has to be computed
  // only now because finalizeHarmoniesInHarmoniesMeasure()
  // may have incremented a harmony sounding whole notes duration
//   rational
//     measureWholeNotesDurationFromPartMeasuresVector = USELESS ??? JMI
//       harmoniesPart->
//         fetchPartMeasuresWholeNotesDurationsVectorAt (
//           inputLineNumber,
//           fMeasureOrdinalNumberInVoice - 1);

  // DO NOT pad the measure up to measureWholeNotesDurationFromPartMeasuresVector,
  // since the last harmony in the measure have been extended to the end of the measure
//   padUpToPositionAtTheEndOfTheMeasure ( // JMI
//     inputLineNumber,
//     measureWholeNotesDurationFromPartMeasuresVector);

  // determine the measure kind and purist number
  determineMeasureKindAndPuristNumber (
    inputLineNumber,
    measureRepeatContextKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeHarmoniesMeasure() 2");
  }
#endif

  --gIndenter;
}

void msrMeasure::finalizeFiguredBassMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  const string&               context)
{
  // fetch the figured bass voice
  S_msrVoice
    figuredBassVoice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  // fetch the figured bass part
  S_msrPart
    figuredBassPart =
      figuredBassVoice->
        fetchVoicePartUpLink ();

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
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in figured bass voice \"" <<
      figuredBassVoice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
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
      "fMeasureOrdinalNumberInVoice = " <<
      fMeasureOrdinalNumberInVoice <<
      ", figuredBassPartNumberOfMeasures = " <<
      figuredBassPartNumberOfMeasures <<
      endl;
  }
#endif

  // the measureWholeNotesDuration has to be computed
  rational
    measureWholeNotesDuration =
      figuredBassPart->
        fetchPartMeasuresWholeNotesDurationsVectorAt (
          inputLineNumber,
          fMeasureOrdinalNumberInVoice - 1);

  // handle the figured bass elements in this measure
  finalizeFiguredBassElementsInFiguredBassMeasure (
    inputLineNumber,
    context);

  // pad the measure up to fFullMeasureWholeNotesDuration
  padUpToPositionAtTheEndOfTheMeasure (
    inputLineNumber,
    measureWholeNotesDuration);

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
  const string&               context)
{
  if (fMeasureHasBeenFinalized) {
    stringstream s;

    s <<
      "Attempting to finalize measure " <<
      this->asShortString () <<
      " more than once in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "', context: " << context <<
      "', measureFinalizationContext: " << fMeasureFinalizationContext <<
      " in voice \"" <<
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ()->
          getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        endl <<
        s.str () <<
        endl << endl;

      S_msrStaff
        staff =
          fetchMeasureStaffUpLink ();

      gLogStream <<
        "staff:" <<
        endl;
      ++gIndenter;
      staff->printShort (gLogStream);
      gLogStream << endl;
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
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Finalizing measure " <<
        this->asString () <<
        " in segment '" <<
        fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
        "' in voice \"" <<
        fMeasureSegmentUpLink->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
        "\" (" << context << ")" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    if (fCurrentMeasureWholeNotesDuration.getNumerator () == 0) {
      stringstream s;

      s <<
        "Measure " <<
        this->asShortString () <<
        " in segment '" <<
        fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
        "' in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        ", doesn't contain any music" <<
        endl;

      msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }
  }
#endif

    // delegate to voice kind specific methods
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
        finalizeHarmoniesMeasure (
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

    // position in voice
    rational
      positionInVoice =
        fetchMeasureVoiceUpLink ()->
          getCurrentPositionInVoice ();

    // delegate position measure assignment to the elements in the measure
    for (
      list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
      i != fMeasureElementsList.end ();
      ++i
    ) {
      S_msrMeasureElement measureElement = (*i);

      measureElement->
        assignMeasureElementPositionInVoice (
          positionInVoice,
          "finalizeMeasure()");
    } // for

    // register finalization
    fMeasureHasBeenFinalized = true;
    fMeasureFinalizationContext = context;
  }
}

void msrMeasure::finalizeMeasureClone (
  int          inputLineNumber,
  S_msrMeasure originalMeasure,
  S_msrVoice   voiceClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing measure clone " <<
      this->asString () <<
      " in segment '" <<
      fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
// JMI      fMeasureSegmentUpLink->getSegmentVoiceUpLink ()->getVoiceName () <<
      voiceClone->getVoiceName () <<
// JMI      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureClone() 1");
  }
#endif

  if (fMeasureHasBeenFinalized) {
    stringstream s;

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
      rational (1, 1));
  }
  else {
    // set the full length from time
    setFullMeasureWholeNotesDurationFromTime (
      voiceCurrentTimeSignature);
  }

/* JMI
  // is this note the shortest one in this measure?
  rational
    voiceShortestNoteDuration =
      voiceClone->
        getVoiceShortestNoteDuration ();

  if (fMeasureShortestNoteDuration < voiceShortestNoteDuration) {
    voiceClone->
      registerShortestNoteInVoiceIfRelevant (
        note);

    // measure shortest note tuplet factor // JMI
    fMeasureShortestNoteTupletFactor = rational (1, 1);
  }
*/

  // register this measures's length in the part clone
  S_msrPart
    part =
      voiceClone->
        getVoiceStaffUpLink ()->
          getStaffPartUpLink ();

  part->
    registerOrdinalMeasureNumberWholeNotesDuration (
      inputLineNumber,
      fMeasureOrdinalNumberInVoice,
      fCurrentMeasureWholeNotesDuration);

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
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      this->
        displayMeasure (
          inputLineNumber,
          "finalizeMeasureClone() 2 - cloneMeasure");

      originalMeasure->
        displayMeasure (
          inputLineNumber,
          "finalizeMeasureClone() 3 - originalMeasure");

      stringstream s;

      s <<
        "*********>> measure clone '" <<
        fMeasureElementMeasureNumber <<
        "': measure kind '" <<
        msrMeasureKindAsString (
          fMeasureKind) <<
        "' differs from original measure measure kind '" <<
        msrMeasureKindAsString (
          originalMeasureMeasureKind) <<
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
        stringstream s;

        s <<
          "*********>> measure clone '" <<
          fMeasureElementMeasureNumber <<
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

  // register finalization
  fMeasureHasBeenFinalized = true;
  fMeasureFinalizationContext = "finalizeMeasureClone()";

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
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
      endl;
  }

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasure::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasure::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasure::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasure::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasure::browseData ()" <<
      endl;
  }

  for (
    list<S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
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
      endl;
  }
}

string msrMeasure::measureFirstInSegmentKindAsString (
    msrMeasureFirstInSegmentKind measureFirstInSegmentKind)
{
  string result;

  switch (measureFirstInSegmentKind) {
    case msrMeasure::kMeasureFirstInSegmentKindUnknown:
      result = "***kMeasureFirstInSegmentKindUnknown***";
      break;
    case msrMeasure::kMeasureFirstInSegmentKindYes:
      result = "kMeasureFirstInSegmentKindYes";
      break;
    case msrMeasure::kMeasureFirstInSegmentKindNo:
      result = "kMeasureFirstInSegmentKindNo";
      break;
  } // switch

  return result;
}

string msrMeasure::measureRepeatContextKindAsString (
  msrMeasureRepeatContextKind measureRepeatContextKind)
{
  string result;

  switch (measureRepeatContextKind) {
    case msrMeasure::kMeasureRepeatContextKindUnknown:
      result = "***kMeasureRepeatContextKindUnknown***";
      break;
    case msrMeasure::kMeasureRepeatContextKindNone:
      result = "kMeasureRepeatContextNone";
      break;
    case msrMeasure::kMeasureRepeatContextKindCommonPartLastMeasure:
      result = "kMeasureRepeatContextCommonPartLastMeasure";
      break;
    case msrMeasure::kMeasureRepeatContextKindHookedEndingLastMeasure:
      result = "kMeasureRepeatContextHookedEndingLastMeasure";
      break;
    case msrMeasure::kMeasureRepeatContextKindHooklessEndingLastMeasure:
      result = "kMeasureRepeatContextHooklessEndingLastMeasure";
      break;
    case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterCommonPart:
      result = "kMeasureRepeatContextNextMeasureAfterCommonPart";
      break;
    case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterHookedEnding:
      result = "kMeasureRepeatContextNextMeasureAfterHookedEnding";
      break;
    case msrMeasure::kMeasureRepeatContextKindNextMeasureAfterHooklessEnding:
      result = "kMeasureRepeatContextNextMeasureAfterHooklessEnding";
      break;
  } // switch

  return result;
}

string msrMeasure::measureEndRegularKindAsString (
    msrMeasureEndRegularKind measureEndRegularKind)
{
  string result;

  switch (measureEndRegularKind) {
    case msrMeasure::kMeasureEndRegularKindUnknown:
      result = "***kMeasureEndRegularUnknown***";
      break;
    case msrMeasure::kMeasureEndRegularKindYes:
      result = "kMeasureEndRegularYes";
      break;
    case msrMeasure::kMeasureEndRegularKindNo:
      result = "kMeasureEndRegularNo";
      break;
  } // switch

  return result;
}

string msrMeasure::asShortString () const
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  stringstream s;

  s <<
    "[Measure " <<
    fMeasureElementMeasureNumber <<
// JMI    ", measureKind: " <<
    ", " <<
    msrMeasureKindAsString (fMeasureKind) <<
// JMI    ", voice: " <<
    ", " <<
    voice->getVoiceName () <<
    /* JMI
    ", measureOrdinalNumberInVoice: " <<
    fMeasureOrdinalNumberInVoice <<
    ", measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    */
/* JMI
    ", currentMeasureWholeNotesDuration: " << fCurrentMeasureWholeNotesDuration <<
    ", fullMeasureWholeNotesDuration: " << fFullMeasureWholeNotesDuration <<
    */
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrMeasure::asShortStringForMeasuresSlices () const
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  stringstream s;

  s <<
    fMeasureElementMeasureNumber <<
    " (" << voice->getVoiceName () <<
    "): [";
/* JMI
// JMI    ", measureKind: " <<
    ": [";
    msrMeasureKindasShortStringForMeasuresSlices (fMeasureKind) <<
    ", voice: " <<
    ", " <<
    voice->getVoiceName () <<
    ", measureOrdinalNumberInVoice: " <<
    fMeasureOrdinalNumberInVoice <<
    ", measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    ", currentMeasureWholeNotesDuration: " << fCurrentMeasureWholeNotesDuration <<
    ", fullMeasureWholeNotesDuration: " << fFullMeasureWholeNotesDuration <<
    ", ";
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    ", ";
*/

  unsigned int
    measureElementsListSize =
      fMeasureElementsList.size ();

/* JMI JMI
  s <<
    "fMeasureElementsList: " <<
    mfSingularOrPlural (
      measureElementsListSize, "element", "elements") <<
    endl;
*/

  if (measureElementsListSize) {
// JMI    s << endl;

    ++gIndenter;

    list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement measureElement = (*i);

      s << measureElement->asShortStringForMeasuresSlices ();
      if (++i == iEnd) break;
      s << ", ";
    } // for

    --gIndenter;
  }

  s << "]";

  return s.str ();
}

string msrMeasure::asString () const
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

  stringstream s;

  s <<
    "[Measure '" <<
    fMeasureElementMeasureNumber <<
    ", measureKind: " <<
    msrMeasureKindAsString (fMeasureKind) <<
    ", voice: " <<
    voice->getVoiceName () <<
    ", measureOrdinalNumberInVoice: " <<
    fMeasureOrdinalNumberInVoice <<
    ", measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    ", currentMeasureWholeNotesDuration: " <<
    fCurrentMeasureWholeNotesDuration <<
    ", fullMeasureWholeNotesDuration: " <<
    fFullMeasureWholeNotesDuration <<
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrMeasure::displayMeasure (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    "*********>> Measure '" <<
    fMeasureElementMeasureNumber <<
    /* JMI
    "', measurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", measureDebugNumber: '" <<
    fMeasureDebugNumber <<
    */
    " (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    endl << endl;
}

void msrMeasure::print (ostream& os) const
{
  os <<
    "[Measure '" <<
    fMeasureElementMeasureNumber <<
    "', " << msrMeasureKindAsString (fMeasureKind) <<
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 45;

  os << left <<
    setw (fieldWidth) <<
    "fCurrentMeasureWholeNotesDuration" << " : " <<
    fCurrentMeasureWholeNotesDuration <<
    endl <<
    setw (fieldWidth) <<
    "fullMeasureWholeNotesDuration" << " : " <<
    fFullMeasureWholeNotesDuration <<
    endl <<

    setw (fieldWidth) <<
    "fMeasureOrdinalNumberInVoice" << " : " <<
    fMeasureOrdinalNumberInVoice <<
    endl <<

    setw (fieldWidth) <<
    "fMeasurePuristNumber" << " : " <<
    fMeasurePuristNumber <<
    endl <<
    setw (fieldWidth) <<
    "fMeasureDebugNumber" << " : " <<
    fMeasureDebugNumber <<
    endl <<

    setw (fieldWidth) <<
    "fMeasureEndRegularKind" << " : " <<
    measureEndRegularKindAsString (
      fMeasureEndRegularKind) <<
    endl <<
    setw (fieldWidth) <<
    "fMeasureRepeatContextKind" << " : " <<
    measureRepeatContextKindAsString (
      fMeasureRepeatContextKind) <<
    endl <<

    setw (fieldWidth) <<
    "fMeasureFirstInVoice" << " : " <<
    fMeasureFirstInVoice <<
    endl <<
    setw (fieldWidth) <<
    "fMeasureFirstInSegmentKind" << " : " <<
    msrMeasure::measureFirstInSegmentKindAsString (
      fMeasureFirstInSegmentKind) <<
    endl <<

    setw (fieldWidth) <<
    "segmentUpLink" << " : " <<
    fMeasureSegmentUpLink->asShortString () <<
    endl;


#ifdef TRACING_IS_ENABLED
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUpLink->
        getSegmentVoiceUpLink ();

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
  os << left <<
    setw (fieldWidth) << "voiceCurrentClef" << " : ";
  if (voiceCurrentClef) {
    os <<
      voiceCurrentClef;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentKey" << " : ";
  if (voiceCurrentKey) {
    os <<
      voiceCurrentKey;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "voiceCurrentTimeSignature" << " : ";
  if (voiceCurrentTimeSignature) {
    os <<
      voiceCurrentTimeSignature;
  }
  else {
    os << "null" << endl;
  }
#endif

    /* JMI

    setw (fieldWidth) <<
    "fCurrentMeasureWholeNotesDurationAsMsrString" << " : " <<
    currentMeasureWholeNotesDurationAsMsrString () <<
    endl <<
    setw (fieldWidth) <<
    "fullMeasureWholeNotesDurationAsMsrString" << " : " <<
    fullMeasureWholeNotesDurationAsMsrString () <<
    endl <<
      */

/* JMI
  os << left <<
    setw (fieldWidth) <<
    "fMeasureShortestNoteDuration" << " : " <<
    fMeasureShortestNoteDuration <<
    endl <<
    setw (fieldWidth) <<
    "fMeasureShortestNoteTupletFactor" << " : " <<
    fMeasureShortestNoteTupletFactor <<
    endl;
*/

  os << left <<
    setw (fieldWidth) <<
    "fMeasureLongestNote" << " : ";
  if (fMeasureLongestNote) {
    os <<
      fMeasureLongestNote->asShortString ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fMeasureContainsMusic" << " : " <<
    fMeasureContainsMusic <<
    endl <<

    setw (fieldWidth) <<
    "fMeasureKindAndPuristNumberHaveBeenDetermined" << " : " <<
    fMeasureKindAndPuristNumberHaveBeenDetermined <<
    endl <<

    setw (fieldWidth) <<
    "fMeasurePositionInVoice" << " : " <<
    fMeasurePositionInVoice <<
    endl <<
    setw (fieldWidth) <<
    "fMeasureMomentInVoice" << " : " <<
    fMeasureMomentInVoice.asString () <<
    endl <<

    setw (fieldWidth) <<
    "fMeasureHasBeenFinalized" << " : " <<
    fMeasureHasBeenFinalized <<
    endl <<
    setw (fieldWidth) <<
    "fMeasureFinalizationContext" << " : " <<
    fMeasureFinalizationContext <<
    endl <<

    setw (fieldWidth) <<
    "fMeasureIsAFullMeasureRest" << " : " <<
    fMeasureIsAFullMeasureRest <<
    endl;

  os <<
    setw (fieldWidth) <<
    "nextMeasureNumber" << " : \"" <<
    fNextMeasureNumber <<
    "\"" <<
    endl;

  // print measure elements list
  unsigned int
    measureElementsListSize =
      fMeasureElementsList.size ();

  os <<
    setw (fieldWidth) <<
    "fMeasureElementsList" << " : " <<
    mfSingularOrPlural (
      measureElementsListSize, "element", "elements") <<
    endl;

  if (measureElementsListSize) {
    os << endl;

    ++gIndenter;

    list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement measureElement = (*i);

      os << measureElement;
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  os << endl;

  // print measure notes flat list
  unsigned int
    measureNotesFlatListSize =
      fMeasureNotesFlatList.size ();

  os <<
    "fMeasureNotesFlatList: " <<
    mfSingularOrPlural (
      measureNotesFlatListSize, "note", "notes") <<
    endl;

  if (measureNotesFlatListSize) {
    ++gIndenter;

    list<S_msrNote>::const_iterator
      iBegin = fMeasureNotesFlatList.begin (),
      iEnd   = fMeasureNotesFlatList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote note = (*i);

      os <<
        note->asShortStringForTimeView ();

      if (++i == iEnd) break;
      os << " ";
    } // for
    os << endl;

    --gIndenter;
  }

  --gIndenter;

  os << "]" << endl;
}

void msrMeasure::printShort (ostream& os) const
{
  os <<
    "[Measure '" <<
    fMeasureElementMeasureNumber <<
    "', " << msrMeasureKindAsString (fMeasureKind) <<
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 29;

  os << left <<
    setw (fieldWidth) <<
    "fMeasureOrdinalNumberInVoice" << " : " <<
    fMeasureOrdinalNumberInVoice <<
    endl <<

    setw (fieldWidth) <<
    "fMeasurePuristNumber" << " : " <<
    fMeasurePuristNumber <<
    endl <<
    setw (fieldWidth) <<
    "fMeasureDebugNumber" << " : " <<
    fMeasureDebugNumber <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fullMeasureWholeNotesDuration" << " : " <<
    fFullMeasureWholeNotesDuration <<
    endl;

  unsigned int
    measureElementsListSize =
      fMeasureElementsList.size ();

  os <<
    setw (fieldWidth) <<
    "fMeasureElementsList" << " : " <<
    mfSingularOrPlural (
      measureElementsListSize, "element", "elements") <<
    endl;

  if (measureElementsListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;

    --gIndenter;
  }

  --gIndenter;

  os << "]" << endl;
}

ostream& operator<< (ostream& os, const S_msrMeasure& elt)
{
  elt->print (os);
  return os;
}


}

/*
//   if (positionsInMeasureDelta.getNumerator () > 0) {
//     // there is at least one note between
//   }

//   // is a padding skip note needed?
//   if (positionsInMeasureDelta.getNumerator () > 0) {
//     // create a padding skip note
//     S_msrNote
//       skipNote =
//         createPaddingSkipNoteForVoice (
//           inputLineNumber,
//           positionsInMeasureDelta,
//           voice);
//
//     // set its position in measure
//     skipNote->
//       setMeasureElementPositionInMeasure (
//         fCurrentMeasureWholeNotesDuration,
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
//         endl;
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
//   else if (positionsInMeasureDelta.getNumerator () < 0) {
//     // the two harmonies overlap in time
//     stringstream s;
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
//     rational
//       reducedSoundingWholeNotes =
//         previousHarmonySoundingWholeNotes
//           + // the delta is negative
//         positionsInMeasureDelta;
//     reducedSoundingWholeNotes.rationalise ();
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
//         endl;
//     }
// #endif
//
//     if (reducedSoundingWholeNotes.getNumerator () == 0) {
//       stringstream s;
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
//         setHarmonySoundingWholeNotes (
//           reducedSoundingWholeNotes);
//     }
//   }
*/
