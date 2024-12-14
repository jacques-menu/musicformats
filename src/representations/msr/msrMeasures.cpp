/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
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
#include "msrTuplets.h"
#include "msrVoiceStaffChanges.h"

#include "oahOah.h"

#include "waeOah.h"

#include "msrOah.h"

#include "msrWae.h"
#include "mxsr2msrWae.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
int msrMeasure::sGlobalMeasureDebugNumber = 0;

S_msrMeasure msrMeasure::create (
  int                 inputLineNumber,
  const std::string&  measureNumber,
  const S_msrSegment& measureUpLinkToSegment)
{
  msrMeasure* obj =
    new msrMeasure (
      inputLineNumber,
      measureNumber,
      measureUpLinkToSegment);
  assert (obj != nullptr);
  return obj;
}

msrMeasure::msrMeasure (
  int                 inputLineNumber,
  const std::string&  measureNumber,
  const S_msrSegment& measureUpLinkToSegment)
    : msrSegmentElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureUpLinkToSegment != nullptr,
    "measureUpLinkToSegment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set measure's segment upLink
  fMeasureUpLinkToSegment =
    measureUpLinkToSegment;

  // set measure numbers
  fMeasureNumber = measureNumber;
  fMeasureOrdinalNumberInVoice = -1;

  fMeasurePuristNumber = -1; // default irrealist value

  // set debug number
  fMeasureDebugNumber = ++sGlobalMeasureDebugNumber;

  // do other initializations
  initializeMeasure ();
}

void msrMeasure::initializeMeasure ()
{
  S_msrVoice
    upLinkToVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Initializing measure " <<
      fMeasureNumber <<
      ", measureDebugNumber: " <<
      fMeasureDebugNumber <<
      "' in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      upLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // measure kind
  fMeasureKind = msrMeasureKind::kMeasureKindUnknown;

  // measure 'first in segment' kind
  fMeasureFirstInSegmentKind =
    msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindUnknown;

  // measure 'first in voice'
  fMeasureIsFirstInVoice = false; // default value

  // single-measure rest?
  fMeasureIsAMeasureRest = false;

  // fetch the staff
  S_msrStaff
    upLinkToStaff =
      upLinkToVoice->
        getVoiceUpLinkToStaff ();

  // set the full measure whole notes duration from time if relevant
  S_msrTimeSignature
    staffCurrentTimeSignature =
      upLinkToStaff->
        getStaffCurrentTimeSignature ();

  if (staffCurrentTimeSignature) {
    setFullMeasureWholeNotesDurationFromTimeSignature (
      staffCurrentTimeSignature);
  }

  // measure shortest note duration
//  fMeasureShortestNoteWholeNotes = msrWholeNotes (INT_MAX, 1);

  // measure shortest note tuplet factor
//  fMeasureShortestNoteTupletFactor = mfRational (1, 1);

  // measure doesn't contain sound yet
  fMeasureContainsSound = false;

  // regular measure ends detection
  fMeasureEndRegularKind =
    msrMeasureEndRegularKind::kMeasureEndRegularKindUnknown;

  // repeat context
  fMeasureRepeatContextKind =
    msrMeasureRepeatContextKind::kMeasureRepeatContext_UNKNOWN_;

  // measure whole notes duration
  // initialize measure whole notes
  setMeasureCurrentAccumulatedWholeNotesDuration (
    fInputStartLineNumber,
    msrWholeNotes (0, 1), // ready to receive the first note
    "initializeMeasure()");

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
  fMeasureKindHasBeenDetermined = false;
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
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrVoice
    containingSegmentUpLinkToVoice =
      containingSegment->
        getSegmentUpLinkToVoice ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of measure " <<
      this->asShortString () <<
      fMeasureDebugNumber <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
      containingSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // create newborn clone
  S_msrMeasure
    newbornClone =
      msrMeasure::create (
        fInputStartLineNumber,
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

    /* JMI v0.9.70 SHOULDE BE RE-COMPUTEDFOR THE MEASURE CLONE  d
  // measure ordinal number in voice
  newbornClone->fMeasureOrdinalNumberInVoice =
    fMeasureOrdinalNumberInVoice;

  // lengthes
  newbornClone->setFullMeasureWholeNotesDuration (
    fFullMeasureWholeNotesDuration);

  // measure purist number
  newbornClone->fMeasurePuristNumber = // JMI don't compute that again for clones ???
    fMeasurePuristNumber;

  // measure 'first in segment' kind
  newbornClone->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;
    / * * /

/ * JMI
  // single-measure rest?
  newbornClone->fMeasureIsAMeasureRest =
    fMeasureIsAMeasureRest;

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating a deep clone of measure " <<
      this->asShortString () <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
      containingSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // create deep clone
  S_msrMeasure
    deepClone =
      msrMeasure::create (
        fInputStartLineNumber,
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

//   // measure lengthes, in whole notes
//   deepClone->fFullMeasureWholeNotesDuration =
//     fFullMeasureWholeNotesDuration;

  deepClone->fMeasureCurrentAccumulatedWholeNotesDuration = // JMI ???
    fMeasureCurrentAccumulatedWholeNotesDuration;

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

    // multi-measure rest?

  deepClone->fMeasureIsAMeasureRest =
    fMeasureIsAMeasureRest;

  // elements

  int numberOfMeasureElements =
    fMeasureElementsList.size ();

  if (numberOfMeasureElements) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
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
        "\"";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    for (
      std::list <S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
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
            elementDeepClone,
            "createMeasureDeepClone()");
*/
      }

      else if (
        S_msrTimeSignature timeSignature = dynamic_cast<msrTimeSignature*>(&(*element))
        ) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTimeSignatures ()) {
          std::stringstream ss;

          ss <<
            "Sharing time " <<
            timeSignature->asShortString () <<
            " in measure " <<
            this->asShortString () <<
            "deep clone" <<
            ", line " << fInputStartLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

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
          elementDeepClone,
          "createMeasureDeepClone()");
    } // for
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "There are no elements in measure to be deep copied" <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
        containingSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating a copy with notes only of measure " <<
      this->asShortString () <<
      " in segment " <<
      containingSegment->asString () <<
      " in voice \"" <<
      containingSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingSegment != nullptr,
    "containingSegment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // create copy
  S_msrMeasure
    measureCopy =
      msrMeasure::create (
        fInputStartLineNumber,
        measureNumber,
        containingSegment);

  // set measureCopy's ordinal number JMI ???
  measureCopy->
    setMeasureOrdinalNumberInVoice (
      containingSegmentUpLinkToVoice->
        incrementVoiceCurrentMeasureOrdinalNumber ());

/* JMI v0.9.69
  // lengthes
  measureCopy->fFullMeasureWholeNotesDuration =
    fFullMeasureWholeNotesDuration;

  measureCopy->fMeasureCurrentAccumulatedWholeNotesDuration =
    fMeasureCurrentAccumulatedWholeNotesDuration;

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
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
        "\"";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    for (
      std::list <S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
          std::stringstream ss;

          ss <<
            "Sharing time " <<
            timeSignature->asShortString () <<
            " in measure " <<
            this->asShortString () <<
            "deep clone" <<
            ", line " << fInputStartLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

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
            elementToBeCopied,
            "createMeasureCopyWithNotesOnly()");
      }
    } // for
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "There are no elements in measure to be deep copied" <<
        " in segment " <<
        containingSegment->asString () <<
        " in voice \"" <<
        containingSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  return measureCopy;
}

/* JMI
void msrMeasure::setMeasureShortestNoteWholeNotes (
  const msrWholeNotes& duration)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotes ()
      ||
    gMsrOahGroup->getTraceMsrNotesDurations ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting the shortest note duration of measure " <<
      fMeasureNumber <<
      " to " <<
      duration.asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureShortestNoteWholeNotes = duration;
}

void msrMeasure::setMeasureShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotes ()
      ||
    gMsrOahGroup->getTraceMsrNotesDurations ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting the shortest note tuplet factor of measure " <<
      fMeasureNumber <<
      " to " <<
      noteTupletFactor;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureShortestNoteTupletFactor = noteTupletFactor;
}
*/

void msrMeasure::setMeasureEndRegularKind (
  msrMeasureEndRegularKind measureEndRegularKind)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "Setting end regular kind of measure '" <<
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
        ", line " << fInputStartLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  fMeasureEndRegularKind = measureEndRegularKind;
}

void msrMeasure::setMeasureRepeatContextKind (
  msrMeasureRepeatContextKind measureRepeatContextKind)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
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
        ", line " << fInputStartLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  fMeasureRepeatContextKind = measureRepeatContextKind;
}

void msrMeasure::setMeasureNumber (
  const std::string& measureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
      std::stringstream ss;

      ss <<
        "Setting the measure number of measure '" <<
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
        ", line " << fInputStartLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  fMeasureNumber = measureNumber;
}

void msrMeasure::setMeasureOrdinalNumberInVoice (
  int measureOrdinalNumber)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
      std::stringstream ss;

      ss <<
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
        ", line " << fInputStartLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  fMeasureOrdinalNumberInVoice = measureOrdinalNumber;
}

void msrMeasure::setMeasurePuristNumber (
  int measurePuristNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
    std::stringstream ss;

    ss <<
      "Setting the purist number of measure '" <<
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
      ", line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasurePuristNumber = measurePuristNumber;
}

// void msrMeasure::incrementMeasureVoicePosition (
// msrWholeNotes wholeNotesDelta)
// {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Incrementing the measure voice position of " <<
//         this->asShortString () <<
//         " by " <<
//         wholeNotesDelta.asString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//
//        gWaeHandler->waeTrace (
//          __FILE__, __LINE__,
//         ss.str (),
//      }
// #endif // MF_TRACE_IS_ENABLED
//
//   fMeasureVoicePosition += wholeNotesDelta;
// }

void msrMeasure::appendMeasureElementToMeasure (
  const S_msrMeasureElement& measureElement,
  const std::string&         context)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureElement != nullptr,
    "appendMeasureElementToMeasure(): measureElement is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Appending measure element " <<
      measureElement->asShortString () <<
      " to measure " <<
      asShortString () <<
//       " in voice \"" <<
//       fetchMeasureUpLinkToVoice ()-> JMI not yet set v0.9.66
//         getVoiceName () <<
//       "\"" <<
      ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", context: " << context <<
      ", line " << measureElement->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // populate measureElement uplink to measure
  S_msrMeasure upLinkToMeasure = this;

  measureElement->
    setMeasureElementUpLinkToMeasure (upLinkToMeasure);

//   // set measureElement's measure number JMI v0.9.66
//   upLinkToMeasure->
//     setMeasureNumber (
//       fMeasureNumber);

  // set measureElement's measure position
  measureElement->
    setMeasureElementMeasurePosition (
      this,
      fMeasureCurrentAccumulatedWholeNotesDuration,
      "appendMeasureElementToMeasure() 1");

  // append measureElement to the measure elements list
  fMeasureElementsList.push_back (measureElement);

//   DON'T account for tuplets sounding whole notes, // JMI v0.9.70
//   this is done one member note at a time

  msrWholeNotes
    measureElementWholeNotes =
      measureElement->getMeasureElementSoundingWholeNotes ();

  if (measureElementWholeNotes.getNumerator () != 0 ) {
    fMeasureContainsSound = true;

    incrementMeasureCurrentAccumulatedWholeNotesDuration (
      measureElement->getInputStartLineNumber (),
      measureElementWholeNotes,
      context + "appendMeasureElementToMeasure() 2: "
        +
      measureElement->asShortString ());
  }
}

void msrMeasure::insertElementInMeasureBeforeIterator (
  int                                      inputLineNumber,
  std::list <S_msrMeasureElement>::iterator iter,
  const S_msrMeasureElement&               elem)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
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
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   // set elem's measure number
  S_msrMeasure
    upLinkToMeasure =
      elem->
        getMeasureElementUpLinkToMeasure ();
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     upLinkToMeasure != nullptr,
//     "upLinkToMeasure is null");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   upLinkToMeasure->
//     setMeasureNumber (
//       fMeasureNumber);

  // set elem's measure position
  elem->
    setMeasureElementMeasurePosition (
      this,
      fMeasureCurrentAccumulatedWholeNotesDuration,
      "insertElementInMeasureBeforeIterator()");

  // insert elem in the measure elements list before (*iter)
  fMeasureElementsList.insert (
    iter, elem);

  // account for elem's duration in measure whole notes
  incrementMeasureCurrentAccumulatedWholeNotesDuration (
    inputLineNumber,
    elem->getMeasureElementSoundingWholeNotes (),
    "insertElementInMeasureBeforeIterator(): "
      +
    elem->asShortString ());
}

void msrMeasure::appendElementAtTheEndOfMeasure (
  const S_msrMeasureElement& elem)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceBarLines ()
  ) {
    std::stringstream ss;

    ss <<
      "Appending measure element " <<
      elem->asString () <<
      " at the end of measure " <<
      asString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()
          ->getVoiceName () <<
      "\", has measure position " <<
      elem->getMeasureElementMeasurePosition () <<
      ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", line " << elem->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    ss <<
      std::endl <<
      "==> appendElementAtTheEndOfMeasure() 1, fMeasureElementsList:" <<
      std::endl;

    ++gIndenter;
    for ( S_msrMeasureElement measureElement : fMeasureElementsList) {
      ss <<
        measureElement <<
        std::endl;
    } // for
    ss << std::endl;
    --gIndenter;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    displayMeasure (
      elem->getInputStartLineNumber (),
      "appendElementAtTheEndOfMeasure() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  if (fMeasureElementsList.size () == 0) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "fMeasureElementsList is empty " <<
        ", inserting elem at the end";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    appendMeasureElementToMeasure (
      elem,
      "appendElementAtTheEndOfMeasure() 3");

    // take elem's sounding whole notes duration into account
    // could be done elsewhere ??? JMI
    incrementMeasureCurrentAccumulatedWholeNotesDuration (
      elem->getInputStartLineNumber (),
      elem->getMeasureElementSoundingWholeNotes (),
      "appendElementAtTheEndOfMeasure() 3: "
        +
      elem->asShortString ());
  }

  else {
    S_msrMeasureElement
      lastMeasureElement =
        fMeasureElementsList.back ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "fMeasureElementsList is NOT empty " <<
        ", lastMeasureElement: " <<
        lastMeasureElement;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    if (
      // barLine?
      S_msrBarLine
        barLine =
          dynamic_cast<msrBarLine*>(&(*lastMeasureElement))
    ) {
      if (true || barLine->barLineIsAFinalBar ()) { // JMI v0.9.66
#ifdef MF_TRACE_IS_ENABLED
        if (
          gTraceOahGroup->getTraceMeasures ()
            ||
          gTraceOahGroup->getTraceBarLines ()
        ) {
          std::stringstream ss;

          ss <<
            "last measure element " <<
            lastMeasureElement->asString () <<
            " is a final barLine, inserting elem before it";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // fetch iterator to list end
        std::list <S_msrMeasureElement>::iterator it =
            fMeasureElementsList.end ();

        // fetch iterator to list last element
        --it;

        // insert elem before it in list // JMI 0.9.70 insert() right away ???
        // (will increment this measure's whole notes duration)
        insertElementInMeasureBeforeIterator (
          elem->getInputStartLineNumber (),
          it,
          elem);

        // register this measure as regular in case it was empty
        // should be done elsewhere JMI ???
      }

      else {
#ifdef MF_TRACE_IS_ENABLED
        if (
          gTraceOahGroup->getTraceMeasures ()
            ||
          gTraceOahGroup->getTraceBarLines ()
        ) {
          std::stringstream ss;

          ss <<
            "last measure element " <<
            lastMeasureElement->asString () <<
            " is a barLine, but not a final one, inserting elem after it";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        appendMeasureElementToMeasure (
          elem,
          "appendElementAtTheEndOfMeasure() 4");
      }
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceMeasures ()
          ||
        gTraceOahGroup->getTraceBarLines ()
      ) {
        std::stringstream ss;

        ss <<
          "last measure element " <<
          lastMeasureElement->asString () <<
          " is no barLine, inserting elem after it";

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      appendMeasureElementToMeasure (
        elem,
        "appendElementAtTheEndOfMeasure() 5");
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<== appendElementAtTheEndOfMeasure() 6, fMeasureElementsList:" <<
      std::endl;
    ++gIndenter;
    for ( S_msrMeasureElement measureElement : fMeasureElementsList) {
      ss <<
        measureElement <<
        std::endl;
    } // for
    ss << std::endl;
    --gIndenter;

    displayMeasure (
      elem->getInputStartLineNumber (),
      "appendElementAtTheEndOfMeasure() 7");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrMeasure::insertElementAtMeasurePosition (
  int                        inputLineNumber,
  const msrWholeNotes&       measurePosition,
  const S_msrMeasureElement& elem)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "Inserting element " <<
      elem->asShortString () <<
      " at position " <<
      measurePosition.asString () <<
      " in measure " <<
      asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()
          ->getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  size_t
    measureElementsListSize =
      fMeasureElementsList.size ();

  if (measureElementsListSize) {
    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement
        currentElement = (*i);

      msrWholeNotes
        currentMeasurePosition =
          currentElement->
            getMeasureElementMeasurePosition ();

      if (measurePosition == currentMeasurePosition) {
        // insert elem in the measure elements list before (*i)
        fMeasureElementsList.insert (
          i, elem);
      }
      else if (measurePosition >= currentMeasurePosition) {
        std::stringstream ss;

        ss <<
          "cannot insert element " <<
          elem->asShortString () <<
          " at position " <<
          measurePosition.asString () <<
          " in measure " <<
          asShortString () <<
          " in voice \"" <<
          fMeasureUpLinkToSegment->
            getSegmentUpLinkToVoice ()
              ->getVoiceName () <<
          "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
          fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
          " since there's no element at this exact position " <<
          ", line " << inputLineNumber;

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }

      if (++i == iEnd) break;
    } // for
  }

  else {
    std::stringstream ss;

    ss <<
      "cannot insert element " <<
      elem->asShortString () <<
      " at position " <<
      measurePosition.asString () <<
      " in measure " <<
      asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()
          ->getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      " since it is empty" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

//   // set elem's measure number
//     S_msrMeasure
//       tupletShortcutUpLinkToMeasure =
//         elem->
//           getMeasureElementUpLinkToMeasure ();
//
//   upLinkToMeasure->
//     setMeasureNumber (
//       fMeasureNumber);

  // set elem's measure position
  elem->
    setMeasureElementMeasurePosition (
      this,
      measurePosition,
      "insertElementAtMeasurePosition()");

  // account for elem's duration in measure whole notes
  incrementMeasureCurrentAccumulatedWholeNotesDuration (
    inputLineNumber,
    elem->getMeasureElementSoundingWholeNotes (),
    "insertElementAtMeasurePosition(): "
      +
    elem->asShortString ());
}

void msrMeasure::setNextMeasureNumber (const std::string& nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting next measure number for measure '" <<
      fMeasureNumber <<
      "' to '" <<
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
      ", line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNextMeasureNumber = nextMeasureNumber;
}

void msrMeasure::setMeasureIsFirstInVoice ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting measure " <<
      this->asShortString () <<
      " as first measure in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      " in segment " <<
      fMeasureUpLinkToSegment-> asString () <<
      ", line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureIsFirstInVoice = true;
}

// msrWholeNotes msrMeasure::fetchFullMeasureWholeNotesDuration_KEEP (
// int         inputLineNumber,
// std::string context) const
// {
//   msrWholeNotes result;
//
//   S_msrStaff
//     staff =
//       fetchMeasureUpLinkToStaff ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (
// //     true ||
//     gTraceOahGroup->getTraceMeasures ()
//       ||
//     gTraceOahGroup->getTraceNotesDurations ()
//       ||
//     gTraceOahGroup->getTraceMeasurePositions ()
//   ) {
//     std::stringstream ss;
//
//     ss <<
//       "--> getFullMeasureWholeNotesDuration() 1, staff: ";
//
//     if (staff) {
//       ss <<
//         staff->asShortString ();
//     }
//     else {
//       ss << "[NULL]";
//     }
//
//     ss <<
//       ", context: " << context <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (staff) {
//     S_msrTimeSignature
//       timeSignature =
//         staff->
//           getStaffCurrentTimeSignature ();
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (
// //     true ||
//     gTraceOahGroup->getTraceMeasures ()
//       ||
//     gTraceOahGroup->getTraceNotesDurations ()
//       ||
//     gTraceOahGroup->getTraceMeasurePositions ()
//   ) {
//       std::stringstream ss;
//
//       ss <<
//         "--> getFullMeasureWholeNotesDuration() 2, timeSignature: ";
//
//       if (timeSignature) {
//         ss <<
//           timeSignature->asShortString ();
//       }
//       else {
//         ss << "[NULL]";
//       }
//
//       ss <<
//         ", context: " << context <<
//         ", line " << inputLineNumber;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     if (timeSignature) {
//       result =
//         timeSignature->
//           timeSignatureWholeNotesPerMeasure ();
//     }
//   }
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (
// //     true ||
//     gTraceOahGroup->getTraceMeasures ()
//       ||
//     gTraceOahGroup->getTraceNotesDurations ()
//       ||
//     gTraceOahGroup->getTraceMeasurePositions ()
//   ) {
//     std::stringstream ss;
//
//     ss <<
//       "--> getFullMeasureWholeNotesDuration() 3, result: " <<
//       result;
//
//     if (staff) {
//       ss <<
//         staff->asShortString ();
//     }
//     else {
//       ss << "[NULL]";
//     }
//
//     ss <<
//       ", context: " << context <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return result;
// }

std::string msrMeasure::fullMeasureWholeNotesDurationpitchAndOctaveAsString ()
{
  return
    wholeNotesPitchAndOctaveAsString (
      fInputStartLineNumber,
      getFullMeasureWholeNotesDuration ());
}

void msrMeasure::setFullMeasureWholeNotesDuration (
  const msrWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
//     true ||
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting the full whole notes duration of measure " <<
      this->asShortString () <<
      " to "  <<
      wholeNotes.asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fFullMeasureWholeNotesDuration = wholeNotes;
}

void msrMeasure::setMeasureCurrentAccumulatedWholeNotesDuration (
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotes,
  std::string          context)
{
#ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasuresWholeNotesVectors ()) {
  if (
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting the accumulated measure whole notes duration of measure " <<
      this->asString () <<
      " to "  <<
      wholeNotes.asFractionString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", context: \"" << context << "\"" <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set measure whole notes
  fMeasureCurrentAccumulatedWholeNotesDuration = wholeNotes;
}

void msrMeasure::incrementMeasureCurrentAccumulatedWholeNotesDuration (
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotesDelta,
  std::string          context)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  //  sanity check
// if (false) // JMI v0.9.66
  mfAssert (
    __FILE__, __LINE__,
    wholeNotesDelta.getNumerator () != 0,
    "wholeNotesDelta.getNumerator () == 0");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // compute the new measure whole notes duration
  msrWholeNotes
    newMeasureCurrentAccumulatedWholeNotesDuration =
      fMeasureCurrentAccumulatedWholeNotesDuration + wholeNotesDelta;

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
  ) {
    std::stringstream ss;

    ss <<
      "Incrementing the accumulated measure whole notes duration of measure " <<
      this->asShortString () <<
      " from "  <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      " by "  <<
      wholeNotesDelta.asFractionString () <<
      " to " <<
      newMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", context: \"" << context << "\"" <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set new measure whole notes duration
  setMeasureCurrentAccumulatedWholeNotesDuration (
    inputLineNumber,
    newMeasureCurrentAccumulatedWholeNotesDuration,
    "incrementMeasureCurrentAccumulatedWholeNotesDuration(): "
      +
    context);
}

std::string msrMeasure::measureCurrentAccumulatedWholeNotesDurationpitchAndOctaveAsString ()
{
  return
    wholeNotesPitchAndOctaveAsString (
      fInputStartLineNumber,
      fMeasureCurrentAccumulatedWholeNotesDuration);
}

void msrMeasure::setMeasureKind (
  msrMeasureKind measureKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
//     true ||
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting the measure kind of measure " <<
      this->asShortString ()<<
      " to " <<
      measureKind <<
      " in segment " <<
      fMeasureUpLinkToSegment->asString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (measureKind) {
    case msrMeasureKind::kMeasureKindUnknown:
      break;
    case msrMeasureKind::kMeasureKindRegular:
      break;
    case msrMeasureKind::kMeasureKindAnacrusis:
      break;
    case msrMeasureKind::kMeasureKindIncompleteStandalone:
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastMeasure:
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

    case msrMeasureKind::kMeasureKindOverFlowing:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMeasuresDetails ()) {
        mfIndentedStringStream iss;

        iss <<
          "!!! Setting the measure kind of measure " <<
          this->asShortString ()<<
          " to " <<
          measureKind <<
          " in segment " <<
          fMeasureUpLinkToSegment->asString () <<
          " in voice \"" <<
          fMeasureUpLinkToSegment->
            getSegmentUpLinkToVoice ()->
              getVoiceName () <<
          "\"" <<
          ", line " << fInputStartLineNumber <<
          '\n';

        iss <<
          '\n' <<
          "--> this measure:" <<
          '\n';
        ++gIndenter;
        iss  <<
          asString () <<
          '\n';
        --gIndenter;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          iss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    case msrMeasureKind::kMeasureKindCadenza:
      break;

    case msrMeasureKind::kMeasureKindMusicallyEmpty:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMeasuresDetails ()) {
        mfIndentedStringStream iss;

        iss <<
          "??? Setting measure kind of measure " <<
          this->asShortString ()<<
          " to " <<
          measureKind <<
          " in segment " <<
          fMeasureUpLinkToSegment->asString () <<
          " in voice \"" <<
          fMeasureUpLinkToSegment->
            getSegmentUpLinkToVoice ()->
              getVoiceName () <<
          "\"" <<
          ", line " << fInputStartLineNumber <<
          '\n';

        iss <<
          '\n' <<
          "--> this measure:" <<
          '\n';
        ++gIndenter;
        iss  <<
          asString () <<
          '\n';
        --gIndenter;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          iss.str ());

        abort();
      }
#endif // MF_TRACE_IS_ENABLED

      break;
  } // switch

  fMeasureKind = measureKind;
}

void msrMeasure::appendMusicXMLPrintLayoutToMeasure (
  const S_msrMusicXMLPrintLayout& musicXMLPrintLayout)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
    std::stringstream ss;

    ss <<
      "Appending print layout " << musicXMLPrintLayout->asString () <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureUpLinkToVoice ()->
        getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append it to the measure elements list
//   prependOtherElementToMeasure (musicXMLPrintLayout); // JMI v0.9.67
  appendMeasureElementToMeasure (
    musicXMLPrintLayout,
    "appendMusicXMLPrintLayoutToMeasure()");

  // register it for MusicXML generation from MSR
  fMeasureMusicXMLPrintLayout = musicXMLPrintLayout;
}

void msrMeasure::appendClefKeyTimeSignatureGroupToMeasure (
  const S_msrClefKeyTimeSignatureGroup& clefKeyTimeSignatureGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending clefKeyTimeSignatureGroup " <<
      clefKeyTimeSignatureGroup->asString () <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureUpLinkToVoice ()->
        getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append it to the measure elements list
  appendMeasureElementToMeasure (
    clefKeyTimeSignatureGroup,
    "appendClefKeyTimeSignatureGroupToMeasure()");

  S_msrTimeSignature
    timeSignature =
      clefKeyTimeSignatureGroup->getTimeSignature ();

  if (timeSignature) { // JMI v0.9.70
    setFullMeasureWholeNotesDurationFromTimeSignature (
      timeSignature);
  }
}

void msrMeasure::appendClefToMeasure (
  const S_msrClef& clef)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefs ()) {
    std::stringstream ss;

    ss <<
      "Appending clef " <<
      clef <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureUpLinkToVoice ()->
        getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append it to the measure elements list
  appendMeasureElementToMeasure (
    clef,
    "appendClefToMeasure()");
}

void msrMeasure::appendKeyToMeasure (
  const S_msrKey& key)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Appending key " <<
      key <<
      " to measure " <<
      this->asShortString () <<
      ", in voice \"" <<
      fetchMeasureUpLinkToVoice ()->
        getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append it to the measure elements list
  appendMeasureElementToMeasure (
    key,
    "appendKeyToMeasure()");
}

void msrMeasure::appendTimeSignatureToMeasure (
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    timeSignature != nullptr,
    "timeSignature is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature " <<
      timeSignature <<
      " to measure " <<
      this->asShortString ()<<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED


  // append timeSignature to the measure elements list
  appendMeasureElementToMeasure (
    timeSignature,
    "appendTimeSignatureToMeasure()");

   // set the measure whole notes per full measure JMI v0.9.70
  setFullMeasureWholeNotesDurationFromTimeSignature (
    timeSignature);

  // register the full measure whole notes duration JMI v0.9.70
  setFullMeasureWholeNotesDuration (
    timeSignature->
      timeSignatureWholeNotesPerMeasure ());
}

void msrMeasure::setFullMeasureWholeNotesDurationFromTimeSignature (
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    timeSignature != nullptr,
    "timeSignature is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if ( // JMI
    gTraceOahGroup->getTraceWholeNoteDurations ()
      ||
    gTraceOahGroup->getTraceTimeSignatures ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting the full measure whole notes of measure " <<
      this->asShortString () <<
      " from time signature " <<
      timeSignature->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

        msrWholeNotes
          wholeNotesPerMeasure =
            timeSignature->
              timeSignatureWholeNotesPerMeasure ();

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTimeSignatures ()) {
          std::stringstream ss;

          ss <<
            "There are " <<
            wholeNotesPerMeasure.asFractionString () <<
            " whole note(s) per measure in time signature:" <<
            timeSignature->asString () <<
            "in measure " <<
            this->asShortString () <<
            " in voice \"" <<
            fMeasureUpLinkToSegment->
              getSegmentUpLinkToVoice ()->
                getVoiceName () <<
            "\"" <<
            ", line " << fInputStartLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // set full measure whole notes
        setFullMeasureWholeNotesDuration (
          wholeNotesPerMeasure);

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTimeSignatures ()) {
          std::stringstream ss;

          ss <<
            "Measure " <<
            this->asShortString () <<
            "'in voice \"" <<
            fMeasureUpLinkToSegment->
              getSegmentUpLinkToVoice ()->
                getVoiceName () <<
            "\"" <<
            " has " <<
            getFullMeasureWholeNotesDuration ().asString () <<
            " full measure whole notes ";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }
      break;

    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:

      // this measure is senza misura

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTimeSignatures ()) {
        std::stringstream ss;

        ss <<
          "Measure " <<
          this->asShortString () <<
           " in voice \"" <<
          fMeasureUpLinkToSegment->
            getSegmentUpLinkToVoice ()->
              getVoiceName () <<
          "\"" <<
          " is senza misura";

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      setMeasureKind (msrMeasureKind::kMeasureKindCadenza);

      setFullMeasureWholeNotesDuration (
        msrWholeNotes (INT_MAX, 1));
      break;
  } // switch

  --gIndenter;
}

void msrMeasure::appendTimeSignatureToMeasureClone (
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    timeSignature != nullptr,
    "timeSignature is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature:" <<
      std::endl;

    ++gIndenter;

    ss <<
      timeSignature;

    --gIndenter;

    S_msrVoice
      upLinkToVoice =
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ();

    ss <<      "to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      upLinkToVoice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append timeSignature to the measure elements list
  appendMeasureElementToMeasure (
    timeSignature,
    "appendTimeSignatureToMeasureClone()");

  // register the full measure whole notes duration JMI v0.9.70
  setFullMeasureWholeNotesDuration (
    timeSignature->
      timeSignatureWholeNotesPerMeasure ());
}

void msrMeasure::insertHiddenMeasureAndBarLineInMeasureClone (
  int                  inputLineNumber,
  const msrWholeNotes& measurePosition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Inserting hidden measure and barLine at position " <<
      measurePosition.asString () <<
      "' in measure clone \"" <<
      this->asShortString () <<
      "\" in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
// JMI      "' in voice \"" <<
//      upLinkToVoice->getVoiceName () <<
//      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

void msrMeasure::appendTranspositionToMeasure (
  const S_msrTransposition& transposition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTranspositions ()) {
    // fetch the voice
    S_msrVoice
      voice =
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ();

    std::stringstream ss;

    ss <<
      "Appending transposition " <<
      transposition->asShortString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      voice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append it to the measure elements list
  appendMeasureElementToMeasure (
    transposition,
    "appendTranspositionToMeasure()");
}

void msrMeasure::printMeasurePendingMeasureElementsList ()
{
  gLog <<
    std::endl <<
    "===> printMeasurePendingMeasureElementsList ()" <<
    std::endl;

  ++gIndenter;

  if (fMeasurePendingMeasureElementsList.size ()) {
    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fMeasurePendingMeasureElementsList.begin (),
      iEnd   = fMeasurePendingMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLog << (*i)->asShortString ();
      if (++i == iEnd) break;
      gLog << std::endl;
    } // for
  }
  else {
    gLog <<
      "[NONE]" <<
      std::endl;
  }

  --gIndenter;

  gLog <<
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Appending barLine " <<
      barLine->asString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      voice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  appendMeasureElementToMeasure (
    barLine,
    "appendBarLineToMeasure()");
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
  appendMeasureElementToMeasure (
    segno,
    "msrMeasure::appendSegnoToMeasure()");
}

void msrMeasure::appendCodaToMeasure (
  const S_msrCoda& coda)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (
    coda,
    "appendCodaToMeasure()");
}

void msrMeasure::appendEyeGlassesToMeasure (
  const S_msrEyeGlasses& eyeGlasses)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (
    eyeGlasses,
    "appendEyeGlassesToMeasure()");
}

void msrMeasure::appendPedalToMeasure (
  const S_msrPedal& pedal)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (
    pedal,
    "appendPedalToMeasure()");
}

void msrMeasure::appendDampToMeasure (
  const S_msrDamp& damp)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (
    damp,
    "appendDampToMeasure()");
}

void msrMeasure::appendDampAllToMeasure (
  const S_msrDampAll& dampAll)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (
    dampAll,
    "appendDampAllToMeasure()");
}

void msrMeasure::appendBarCheckToMeasure (
  const S_msrBarCheck& barCheck)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (
    barCheck,
    "appendBarCheckToMeasure()");
}

void msrMeasure::appendVoiceStaffChangeToMeasure (
  const S_msrVoiceStaffChange& voiceStaffChange)
{
  // append it to the measure elements list
  appendMeasureElementToMeasure (
    voiceStaffChange,
    "appendVoiceStaffChangeToMeasure()");
}

void msrMeasure::appendNoteToMeasure (
  const S_msrNote&     note,
  const msrWholeNotes& partCurrentDrawingMeasurePosition)
{
  // compute position delta
  msrWholeNotes
    positionsDelta =
      partCurrentDrawingMeasurePosition
        -
      fMeasureCurrentAccumulatedWholeNotesDuration;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    gLog <<
      std::endl << std::endl <<
      "this->print (gLog):" <<
      std::endl;
    this->print (gLog);
    gLog << std::endl << std::endl;

    std::stringstream ss;

    ss <<
      "Appending note " << note->asShortString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", partCurrentDrawingMeasurePosition: " <<
      partCurrentDrawingMeasurePosition.asString () <<
      ", getFullMeasureWholeNotesDuration (): " <<
      getFullMeasureWholeNotesDuration ().asString () <<
      ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", positionsDelta: " <<
      positionsDelta.asString () <<
      ", line " << note->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // should a skip note be appended before note?
  if (positionsDelta.getNumerator () > 0) {
    // fetch the voice
    S_msrVoice
      voice =
        fetchMeasureUpLinkToVoice ();

    // create a skip note of duration positionsDelta
    S_msrNote
      skipNote =
        msrNote::createSkipNote (
          note->getInputStartLineNumber (),
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
#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
    if (
      gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
    ) {
  #ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceParts ()
      ) {
        std::stringstream ss;

        ss <<
          "partCurrentDrawingMeasurePosition " <<
          partCurrentDrawingMeasurePosition.asString () <<
          " is smaller than fMeasureCurrentAccumulatedWholeNotesDuration " <<
          fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
          " in measure " <<
          this->asShortString () <<
          ", cannot padup in voice \"" <<
          fMeasureUpLinkToSegment->
            getSegmentUpLinkToVoice ()->
              getVoiceName () <<
          "\"" <<
          ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
          fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
          ", partCurrentDrawingMeasurePosition: " <<
          partCurrentDrawingMeasurePosition.asString () <<
          ", positionsDelta: " << positionsDelta <<
          ", line " << note->getInputStartLineNumber ();

    //     msrInternalError ( // JMI v0.9.68
        msrInternalWarning (
          gServiceRunData->getInputSourceName (),
          note->getInputStartLineNumber (),
    //      __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
  }

  else {
    // this measure is already at the part current measure position,
    // nothing to do
  }

  // append note to measure
  appendNoteOrPaddingToMeasure (note);

  // determine whether the note occupies a full measure JMI v0.9.69
//   if (note->getMeasureElementSoundingWholeNotes () == fMeasureCurrentAccumulatedWholeNotesDuration) {
  if (note->getMeasureElementSoundingWholeNotes () == getFullMeasureWholeNotesDuration ()) {
    note->
      setNoteOccupiesAFullMeasure ();
  }

  // is note a multi-measure rest? JMI v0.9.69
  if (note->fetchNoteIsAFullNoteRest ()) {
    setMeasureIsAMeasureRest ();
  }

  // set grace notes group 'before' elements measure position in relevant
  S_msrGraceNotesGroup
    graceNotesGroupBeforeNote =
      note->
        getGraceNotesGroupBeforeNote ();

  if (graceNotesGroupBeforeNote) {
    graceNotesGroupBeforeNote->
      setGraceNotesGroupElementsMeasurePositions (
        this,
        note->getMeasureElementMeasurePosition ());
  }

  // set grace notes group 'after' elements measure position in relevant
  S_msrGraceNotesGroup
    graceNotesGroupAfterNote =
      note->
        getGraceNotesGroupAfterNote ();

  if (graceNotesGroupAfterNote) {
    graceNotesGroupAfterNote->
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
  // fetch note sounding whole notes
  msrWholeNotes
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Appending note or padding " << note->asShortString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
       voice->getVoiceName () <<
      "\"" <<
      ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", noteSoundingWholeNotes: " << noteSoundingWholeNotes.asFractionString () <<
      ", line " << note->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  appendMeasureElementToMeasure (
    note,
    "appendNoteOrPaddingToMeasure()");

  // append it to the measure notes flat list if relevant
  Bool doAppendNoteToMeasureNotesFlatList (false);

  switch (note->getNoteKind ()) {
    case msrNoteKind::kNote_UNKNOWN_:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
      doAppendNoteToMeasureNotesFlatList = true;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // the chord itself will be appended to the list
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      doAppendNoteToMeasureNotesFlatList = true;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (note->getNoteShortcutUpLinkToTuplet ()) {
      /* JMI
        result =
          fNoteShortcutUpLinkToTuplet->
            fMeasureElementUpLinkToMeasure ()->
              fetchMeasureUpLinkToVoice ();
      */
      }

      doAppendNoteToMeasureNotesFlatList = true;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMI
      if (fNoteShortcutUpLinkToGraceNotesGroup) {
        result =
          fNoteShortcutUpLinkToGraceNotesGroup->
            getGraceNotesGroupUpLinkToVoice ();
              / * JMI
              getGraceNotesGroupUpLinkToNote ()->
              fetchNoteUpLinkToVoice ();
              * /
      }
    */
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      doAppendNoteToMeasureNotesFlatList = true;
      break;
  } // switch

  if (doAppendNoteToMeasureNotesFlatList) {
    appendNoteToMeasureNotesFlatList (note);
  }

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

  --gIndenter;
}

void msrMeasure::accountForTupletMemberNoteNotesDurationInMeasure ( // JMI v0.9.66 RENAME??? USELESS ACTUALLY
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
//     true ||
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
  ) {
    std::stringstream ss;

    ss <<
      "Accounting for the duration of tuplet member note " <<
      note->asShortString () <<
      " in measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set note's measure position JMI v0.9.66
  note->
    setMeasureElementMeasurePosition (
      this,
      fMeasureCurrentAccumulatedWholeNotesDuration,
      "accountForTupletMemberNoteNotesDurationInMeasure()");

  // fetch note sounding whole notes
  msrWholeNotes
    noteSoundingWholeNotes =
      note->getMeasureElementSoundingWholeNotes ();

  // append it to the measure notes flat list
  appendNoteToMeasureNotesFlatList (note);

  // account for note duration in measure whole notes
  incrementMeasureCurrentAccumulatedWholeNotesDuration (
    note->getInputStartLineNumber (),
    noteSoundingWholeNotes,
    "accountForTupletMemberNoteNotesDurationInMeasure(): "
      +
    note->asShortString ());
}

void msrMeasure::appendPaddingNoteAtTheEndOfMeasure (const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "Appending padding note " << note->asString () <<
      " at the end of measure " <<
      this->asString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", line " << note->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

  --gIndenter;
}

void msrMeasure::appendNoteToMeasureClone (const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Appending note '" << note->asShortString () <<
      "' to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

    /* JMI
  if (appendMeasureIfOverflow (note->getInputStartLineNumber ())) {
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
  // JMI  // only now to make it possible to remove it afterwards v0.9.69
    // if it happens to be the first note of a chord
    appendMeasureElementToMeasure (
      note,
      "appendNoteToMeasureClone()");

    // fetch note sounding whole notes
    msrWholeNotes
      noteSoundingWholeNotes =
        note->
          getMeasureElementSoundingWholeNotes ();

  //* JMI v0.9.69
//     // determine whether the note occupies a full measure
//     if (noteSoundingWholeNotes == fFullMeasureWholeNotesDuration)
//       note->
//         setNoteOccupiesAFullMeasure ();
    //  */

    // register note as the last one in this measure
    fMeasureLastHandledNote = note;

  --gIndenter;
}

void msrMeasure::appendDoubleTremoloToMeasure (
  const S_msrDoubleTremolo& doubleTremolo)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  appendMeasureElementToMeasure (
    doubleTremolo,
    "appendDoubleTremoloToMeasure()");

  --gIndenter;
}

void msrMeasure::appendChordToMeasure (const S_msrChord& chord)
{
  // populate uplink to measure
  chord->setMeasureElementUpLinkToMeasure (this);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Appending chord '" << chord <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set the chord kind
  chord->setChordKind (msrChordInKind::kChordInMeasure);

  // set the chord's measure position
//   chord->
//     setChordMeasurePosition (
//       this,
//       fMeasureCurrentAccumulatedWholeNotesDuration,
//       "msrMeasure::appendChordToMeasure (const S_msrChord& chord)");

  // append the chord to the measure elements list
  appendMeasureElementToMeasure (
    chord,
    "appendChordToMeasure()");

//   // fetch chord sounding whole notes
//   msrWholeNotes
//     chordSoundingWholeNotes =
//       chord->
//         getMeasureElementSoundingWholeNotes ();
//
  // determine if the chord occupies a full measure
// XXL  JMI  if (chordSoundingWholeNotes == fMeasureDivisionsPerWholeMeasure)
    // chord->setChordOccupiesAFullMeasure ();
}

void msrMeasure::appendTupletToMeasure (const S_msrTuplet& tuplet)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Appending tuplet " << tuplet->asString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      ", line " << tuplet->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set the tuplet kind
  tuplet->setTupletKind (msrTupletInKind::kTupletInMeasure);

  // set the tuplet's measure position
//   tuplet->
//     setTupletMeasurePosition (
//       this,
//       fMeasureCurrentAccumulatedWholeNotesDuration,
//       "msrMeasure::appendTupletToMeasure (const S_msrChord& chord)");

  // populate uplink to measure
//   tuplet->setMeasureElementUpLinkToMeasure (this);

  // append the tuplet to the measure elements list
  appendMeasureElementToMeasure (
    tuplet,
    "appendTupletToMeasure()");

  // the duration of the individual members of the tuplet
  // have already been accounted for in current measure's whole notes

  --gIndenter;
}

void msrMeasure::appendHarmonyToMeasureWithoutPadUp (
  int                  inputLineNumber,
  const S_msrHarmony&  harmony)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " << harmony->asString () <<
      " without padUp to measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append harmony to the measure elements list
  appendMeasureElementToMeasure (
    harmony,
    "appendHarmonyToMeasureWithoutPadUp()");

  --gIndenter;
}

void msrMeasure::appendHarmonyToMeasure (
  int                  inputLineNumber,
  const S_msrHarmony&  harmony,
  const msrWholeNotes& measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " << harmony->asString () <<
      " to measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // pad up to measurePositionToAppendAt
  padUpToMeasurePosition (
    inputLineNumber,
    measurePositionToAppendAt);

  // set harmony's measure position JMI v0.9.67 OK NOT HARMFUL
  harmony->
    setMeasureElementMeasurePosition (
      this,
      measurePositionToAppendAt,
      "appendHarmonyToMeasure()");

  // append harmony to the measure elements list
  appendHarmonyToMeasureWithoutPadUp (
    inputLineNumber,
    harmony);

  --gIndenter;
}

void msrMeasure::appendHarmoniesListToMeasure (
  int                            inputLineNumber,
  const std::list <S_msrHarmony>& harmoniesList,
  const msrWholeNotes&           measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmonies list to measure " << // JMI v0.9.67 HARMFUL
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // pad up to measurePositionToAppendAt only once
  padUpToMeasurePosition (
    inputLineNumber,
    measurePositionToAppendAt);

  // append the harmonies to the measure elements list without padUp
  for (S_msrHarmony harmony : harmoniesList) {
    appendHarmonyToMeasure (
      inputLineNumber,
      harmony,
      measurePositionToAppendAt);
  } // for

  --gIndenter;
}

void msrMeasure::appendHarmonyToMeasureClone (
  const S_msrHarmony& harmony)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " << harmony->asString () <<
      " to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", line " << harmony->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append harmony to the measure elements list
  appendMeasureElementToMeasure (
    harmony,
    "appendHarmonyToMeasureClone()");

  --gIndenter;
}

void msrMeasure::appendFiguredBassToMeasureWithoutPadUp (
  int                     inputLineNumber,
  const S_msrFiguredBass& figuredBass)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass " << figuredBass->asString () <<
      " to measure without padUp" <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append harmony to the measure elements list
  appendMeasureElementToMeasure (
    figuredBass,
    "appendFiguredBassToMeasureWithoutPadUp()");

  --gIndenter;
}

void msrMeasure::appendFiguredBassToMeasure (
  int                     inputLineNumber,
  const S_msrFiguredBass& figuredBass,
  const msrWholeNotes&    measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass " << figuredBass->asString () <<
      " to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // pad up to measurePositionToAppendAt
  padUpToMeasurePosition (
    inputLineNumber,
    measurePositionToAppendAt);

  // append harmony to the measure elements list
  appendFiguredBassToMeasureWithoutPadUp (
    inputLineNumber,
    figuredBass);

  --gIndenter;
}

void msrMeasure::appendFiguredBassesListToMeasure (
  int                                inputLineNumber,
  const std::list <S_msrFiguredBass>& figuredBasssesList,
  const msrWholeNotes&               measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured basses list " << // JMI v0.9.67 figuredBasssesList HARMFUL <<
      " to measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // pad up to measurePositionToAppendAt only once
  padUpToMeasurePosition (
    inputLineNumber,
    measurePositionToAppendAt);

  // append the figured basses to the measure elements list without padUp
  for (S_msrFiguredBass figuredBass : figuredBasssesList) {
    appendFiguredBassToMeasureWithoutPadUp (
      inputLineNumber,
      figuredBass);
  } // for

  --gIndenter;
}

void msrMeasure::appendFiguredBassToMeasureClone (
  const S_msrFiguredBass& figuredBass)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass " << figuredBass->asString () <<
      " to measure clone " <<
      this->asShortString () <<
      " in segment clone '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice clone \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", line " << figuredBass->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append the figuredBass to the measure elements list
  appendMeasureElementToMeasure (
    figuredBass,
    "appendFiguredBassToMeasureClone()");

  --gIndenter;
}

S_msrNote msrMeasure::createPaddingSkipNoteForVoice (
  int                  inputLineNumber,
  const msrWholeNotes& duration,
  const S_msrVoice&    voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasurePositions ()
      ||
    gTraceOahGroup->getTraceVoices ()
  ) {
    std::stringstream ss;

    ss <<
      "Creating a padding note for voice \"" <<
      voice->getVoiceName () <<
      "', duration: '" <<
      duration <<
      "\" in measure " <<
      this->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

// void msrMeasure::padUpToMeasurePositionInMeasure (
//   int             inputLineNumber,
//   const msrWholeNotes& measurePositionToPadUpTo)
// {
//   // fetch the measure voice
//   S_msrVoice
//     measureVoice =
//       fMeasureUpLinkToSegment->
//         getSegmentUpLinkToVoice ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasurePositions ()) {
//     this->print (gLog);
//
//       std::stringstream ss;
//
//     ss <<
//       "Padding from measure whole notes '" <<
//       fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
//       "' to '" <<measurePositionToPadUpTo.asString () <<
//       "' in measure " <<
//       this->asShortString () <<
//       " in segment " <<
//       fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
//       " in voice \"" <<
//       measureVoice->getVoiceName () <<
//       "\", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measurePositionToPadUpTo.getNumerator () >= 0,
//     "measurePositionToPadUpTo.getNumerator () is negative in padUpToMeasurePositionInMeasure()");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   if (fMeasureCurrentAccumulatedWholeNotesDuration < measurePositionToPadUpTo) {
//     ++gIndenter;
//
//     // appending a padding rest or skip to this measure to reach measurePositionToPadUpTo
//     msrWholeNotes
//       missingNotesDuration =
//         measurePositionToPadUpTo - fMeasureCurrentAccumulatedWholeNotesDuration;
//
//     // create a padding skip note
//     S_msrNote
//       paddingNote =
//         createPaddingSkipNoteForVoice (
//           inputLineNumber,
//           missingNotesDuration,
//           measureVoice);
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceNotes ()) {
//     std::stringstream ss;
//
//     ss <<
//         "Appending skip " << paddingNote->asString () <<
//         " (missingNotesDuration " << missingNotesDuration <<
//         " whole notes) to skip from measure position " <<
//         fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
//         " to measure position '" <<measurePositionToPadUpTo.asString () << "'"
//         " in measure " <<
//         this->asShortString () <<
//         " in voice \"" <<
//         measureVoice->getVoiceName () <<
//         std::endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     // append the paddingNote to the measure
//     appendNoteOrPaddingToMeasure (paddingNote);
//
// /* JMI
//     // set this measure as being padded // JMI
//     this->
//       setMeasureCreatedForARepeatKind (
//         msrMeasureKind::kMeasureKindCreatedForARepeatPadded);
//     */
//
//     // this measure contains music
//     fMeasureContainsSound = true;
//
//     --gIndenter;
//   }
//
//   else if (fMeasureCurrentAccumulatedWholeNotesDuration == measurePositionToPadUpTo) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceNotes ()) {
//     std::stringstream ss;
//
//     ss <<
//         "No need to pad from measure whole notes '" <<
//         fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
//         " to " <<
//         measurePositionToPadUpTo.asString () <<
//         " since they are equal in measure " <<
//         this->asShortString () <<
//         " in segment " <<
//         fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
//         " in voice \"" <<
//         measureVoice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
//
//   else {
//     measureVoice->print (gLog); // JMI
//     this->print (gLog);
//
//     std::stringstream ss;
//
//     ss <<
//         "Cannot pad from measure whole notes '" <<
//         fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
//         "' to '" <<
//         measurePositionToPadUpTo <<
//         "' since the former is larger than the latter in measure " <<
//         this->asShortString () <<
//         " in segment " <<
//         fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
//         " in voice \"" <<
//         measureVoice->getVoiceName () <<
//         "\", line " << inputLineNumber;
//
// //    msrError ( JMI
//     msrWarning (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//  //     __FILE__, __LINE__,
//       ss.str (),
//   }
// }
//
void msrMeasure::padUpToMeasurePosition (
  int                  inputLineNumber,
  const msrWholeNotes& measurePositionToPadUpTo)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePositionToPadUpTo != K_WHOLE_NOTES_UNKNOWN_,
    "measurePositionToPadUpTo == K_WHOLE_NOTES_UNKNOWN_");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // fetch the voice
  S_msrVoice
    measureVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasurePositions ()
      ||
    gTraceOahGroup->getTraceHarmonies ()
      ||
    gTraceOahGroup->getTraceFiguredBasses ()
  ) {
    std::stringstream ss;

    ss <<
      "Padding up to measure position '" <<
      measurePositionToPadUpTo.asString () <<
      "' in measure " <<
      this->asShortString () <<
      " in segment " <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "padUpToMeasurePosition() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fMeasureCurrentAccumulatedWholeNotesDuration < measurePositionToPadUpTo) {
    // appending a rest to this measure to reach measurePositionToPadUpTo
    msrWholeNotes
      missingNotesDuration =
        measurePositionToPadUpTo - fMeasureCurrentAccumulatedWholeNotesDuration;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasurePositions ()) {
      std::stringstream ss;

      ss <<
        "Creating a padding note for measure debug number " <<
        fMeasureDebugNumber <<
        ", missingNotesDuration: " << missingNotesDuration <<
        " in voice \"" << measureVoice->getVoiceName () <<
        "\", measure: " <<
        this->asShortString () <<
        ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
        fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
   }
#endif // MF_TRACE_IS_ENABLED

    // create a padding skip note
    S_msrNote
      paddingNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          missingNotesDuration,
          measureVoice);

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasurePositions ()) {
      std::stringstream ss;

      ss <<
        "Appending padding note " << paddingNote->asString () <<
        " (" << missingNotesDuration << " whole notes)" <<
        " to finalize \"" << measureVoice->getVoiceName () <<
        "\" measure: " <<
        this->asShortString () <<
        ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
        fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
   }
#endif // MF_TRACE_IS_ENABLED

    // append the paddingNote to the measure
    appendNoteOrPaddingToMeasure (paddingNote);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "padUpToMeasurePosition() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrMeasure::padUpToPositionAtTheEndOfTheMeasure (
  int                  inputLineNumber,
  const msrWholeNotes& measurePositionToPadUpTo,
  const std::string&   context)
{
  // fetch the voice
  S_msrVoice
    measureVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
      ||
    gTraceOahGroup->getTraceMeasures ()
  ) {
    std::stringstream ss;

    ss <<
      "Padding up from measure position " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      " to " <<
      measurePositionToPadUpTo.asFractionString () <<
      " at the end of measure " <<
      this->asString () <<
      " in segment " <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      ", , context: " << context <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionAtTheEndOfTheMeasure() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fMeasureCurrentAccumulatedWholeNotesDuration < measurePositionToPadUpTo) {
    // appending a skip note to this measure to reach measurePositionToPadUpTo
    msrWholeNotes
      missingNotesDuration =
        measurePositionToPadUpTo - fMeasureCurrentAccumulatedWholeNotesDuration;

#ifdef MF_TRACE_IS_ENABLED
    if (
      gTraceOahGroup->getTraceMeasurePositions ()
        ||
      gTraceOahGroup->getTraceMeasures ()
    ) {
      std::stringstream ss;

      ss <<
        "Appending a padding skip note" <<
        ", missingNotesDuration: " << missingNotesDuration <<
        ", at the end of measure " <<
        this->asString () <<
        " in voice \"" << measureVoice->getVoiceName () << "\",  " <<
        ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
        fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
   }
#endif // MF_TRACE_IS_ENABLED

//     gLog << JMI
//       std::endl << std::endl <<
//       "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" <<
//       std::endl << std::endl;
//     this->
//       print (gLog);
//     gLog <<
//       std::endl << std::endl <<
//       "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" <<
//       std::endl << std::endl;

    // create a padding skip note
    S_msrNote
      paddingSkipNote =
        createPaddingSkipNoteForVoice (
          inputLineNumber,
          missingNotesDuration,
          measureVoice);

#ifdef MF_TRACE_IS_ENABLED
    if (
      gTraceOahGroup->getTraceMeasurePositions ()
        ||
      gTraceOahGroup->getTraceMeasures ()
    ) {
      std::stringstream ss;

      ss <<
        "Appending padding skip note " << paddingSkipNote->asString () <<
        " (" << missingNotesDuration << " whole notes)" <<
        " to finalize \"" << measureVoice->getVoiceName () <<
        "\" measure: " <<
        this->asShortString () <<
        " measureCurrentAccumulatedWholeNotesDuration: " <<
        fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
   }
#endif // MF_TRACE_IS_ENABLED

    // append the skip note to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord JMI v0.9.67
    appendPaddingNoteAtTheEndOfMeasure (paddingSkipNote);
  }

  else if (fMeasureCurrentAccumulatedWholeNotesDuration > measurePositionToPadUpTo) {
#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
    if (
      gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
    ) {
#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceParts ()
      ) {
        std::stringstream ss;

        ss <<
          "Cannot padup measure " <<
          this->asShortString () <<
          " from " <<
          fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
          " to " <<
          measurePositionToPadUpTo.asString () <<
          " in voice \"" <<
          measureVoice->getVoiceName () <<
          "\"," <<
          " since the latter has already been overtaken";

    //     msrInternalError (
        msrInternalWarning ( // JMI v0.9.70
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
    //       __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
  }

  else {
    // this measure is already at the desired measure position
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasurePositions ()) {
      std::stringstream ss;

      ss <<
        "There is nothing to do to padup measure " <<
        this->asShortString () <<
        " from " <<
        fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
        " to " <<
        measurePositionToPadUpTo.asString () <<
        " in voice \"" <<
        measureVoice->getVoiceName () <<
        "\"," <<
        " since the latter is already at the desired measure position";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "padUpToPositionAtTheEndOfTheMeasure() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrMeasure::backupByWholeNotesStepLengthInMeasure ( // JMI USELESS ??? v0.9.66
  int                  inputLineNumber,
  const msrWholeNotes& backupTargetMeasureElementMeasurePosition)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    backupTargetMeasureElementMeasurePosition.getNumerator () >= 0,
    "backupTargetMeasureElementMeasurePosition.getNumerator () is negative");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasurePositions ()
      ||
    gTraceOahGroup->getTraceWholeNoteDurations ()
  ) {
    this->print (gLog);

    std::stringstream ss;

    ss <<
      "Backup by a '" <<
      backupTargetMeasureElementMeasurePosition.asString () <<
      "' whole notes step length in measure " <<
      this->asShortString () <<
      ", fMeasureCurrentAccumulatedWholeNotesDuration: '" <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", fullMeasureWholeNotesDuration: '" <<
      getFullMeasureWholeNotesDuration ().asString () <<
      "' in segment " <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      measureVoice->getVoiceName () <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // determine the measure position 'backupTargetMeasureElementMeasurePosition' backward
  msrWholeNotes
    measurePosition =
      getFullMeasureWholeNotesDuration () - backupTargetMeasureElementMeasurePosition;

  // pad up to it
  padUpToMeasurePosition (
    inputLineNumber,
    measurePosition);
}

void msrMeasure::appendPaddingSkipNoteToMeasure (
  int                  inputLineNumber,
  const msrWholeNotes& forwardStepLength)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "Appending a padding skip note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to measure " <<
      this->asShortString () <<
      " in segment " <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // fetch the measure voice
  S_msrVoice
    measureVoice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  // create a padding skip note
  S_msrNote
    paddingSkipNote =
      createPaddingSkipNoteForVoice (
        inputLineNumber,
        forwardStepLength,
        measureVoice);

  // append paddingSkipNote to the measure
  appendNoteOrPaddingToMeasure (paddingSkipNote);

  --gIndenter;
}

void msrMeasure::appendTempoToMeasure (
  const S_msrTempo& tempo)
{
  appendMeasureElementToMeasure (
    tempo,
    "appendTempoToMeasure()");
}

void msrMeasure::appendRehearsalMarkToMeasure (
  const S_msrRehearsalMark& rehearsalMark)
{
  appendMeasureElementToMeasure (
    rehearsalMark,
    "appendRehearsalMarkToMeasure()");
}

void msrMeasure::appendOctaveShiftToMeasure (
  const S_msrOctaveShift& octaveShift)
{
  appendMeasureElementToMeasure (
    octaveShift,
    "appendOctaveShiftToMeasure()");
}

void msrMeasure::appendScordaturaToMeasure (
  const S_msrScordatura& scordatura)
{
  appendMeasureElementToMeasure (
    scordatura,
    "appendScordaturaToMeasure()");
}

void msrMeasure::appendAccordionRegistrationToMeasure (
  const S_msrAccordionRegistration&
    accordionRegistration)
{
  appendMeasureElementToMeasure (
    accordionRegistration,
    "appendAccordionRegistrationToMeasure()");
}

void msrMeasure::appendHarpPedalsTuningToMeasure (
  const S_msrHarpPedalsTuning&
    harpPedalsTuning)
{
  appendMeasureElementToMeasure (
    harpPedalsTuning,
    "appendHarpPedalsTuningToMeasure()");
}

void msrMeasure::appendLineBreakToMeasure (
  const  S_msrLineBreak& lineBreak)
{
  appendMeasureElementToMeasure (
    lineBreak,
    "appendLineBreakToMeasure()");
}

void msrMeasure::appendPageBreakToMeasure (
  const S_msrPageBreak& pageBreak)
{
  appendMeasureElementToMeasure (
    pageBreak,
    "appendPageBreakToMeasure()");
}

void msrMeasure::appendStaffDetailsToMeasure (
  const S_msrStaffDetails& staffDetails)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Appending staff details '" << staffDetails->asShortString () <<
      "' to measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  appendMeasureElementToMeasure (
    staffDetails,
    "appendStaffDetailsToMeasure()");
}

void msrMeasure::appendBarNumberCheckToMeasure (
  const S_msrBarNumberCheck& barNumberCheck)
{
  appendMeasureElementToMeasure (
    barNumberCheck,
    "appendBarNumberCheckToMeasure()");
}

// void msrMeasure::prependOtherElementToMeasure (
//   const S_msrMeasureElement& elem)
// {
//   fMeasureElementsList.push_front (elem); // JMI
//
//   // this measure contains music
//   fMeasureContainsSound = true;
// }
//
// void msrMeasure::appendOtherElementToMeasure  (
//   const S_msrMeasureElement& elem)
// {
//   appendMeasureElementToMeasure (
//     elem,
//     "prependOtherElementToMeasure()");
//
//   // this measure contains music
//   fMeasureContainsSound = true;
// }

void msrMeasure::removeNoteFromMeasure (
  int              inputLineNumber,
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotes ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
  ) {
    std::stringstream ss;

    ss <<
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
      fMeasureLastHandledNote->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list <S_msrMeasureElement>::iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    ++i
  ) {
    if ((*i) == note) {
      // found note, erase it
      i = fMeasureElementsList.erase (i);

      // update measure whole notes
      setMeasureCurrentAccumulatedWholeNotesDuration (
        inputLineNumber,
        fMeasureCurrentAccumulatedWholeNotesDuration
          -
        fMeasureLastHandledNote->getMeasureElementSoundingWholeNotes (),
        "removeNoteFromMeasure(): "
          +
        note->asShortString ());

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      std::endl << std::endl <<
      "@@@@@@@@@@@@@@@@@ segmentVoicePart" <<
      std::endl <<
      segmentVoicePart <<
      std::endl <<
      "@@@@@@@@@@@@@@@@@" <<
      std::endl << std::endl;

    ss <<
      std::endl << std::endl <<
      "@@@@@@@@@@@@@@@@@ segmentUpLinkToVoice" <<
      std::endl <<
      segmentUpLinkToVoice <<
      std::endl <<
      "@@@@@@@@@@@@@@@@@" <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::stringstream ss;

  ss <<
    "cannot remove note " <<
    note <<
    " from measure " <<
    this->asShortString () <<
    " in voice \"" <<
    segmentUpLinkToVoice->getVoiceName () <<
    "\"," <<
    " since this note has not been found in fMeasureElementsList";

  msrInternalError (
    gServiceRunData->getInputSourceName (),
    inputLineNumber,
    __FILE__, __LINE__,
    ss.str ());
}

void msrMeasure::removeElementFromMeasure (
  int                 inputLineNumber,
  const S_msrElement& element)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
  ) {
    std::stringstream ss;

    ss <<
      "Removing element:" <<
      std::endl;

    ++gIndenter;
    ss <<
      " JMI ??? element->elementAsString ()" << std::endl;
    --gIndenter;

    ss <<
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
    ss <<
      "fMeasureLastHandledNote:" <<
      std::endl <<
      fMeasureLastHandledNote->asString () <<
      std::endl;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list <S_msrMeasureElement>::iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    ++i
  ) {
    if ((*i) == element) {
      // found element, erase it
      i = fMeasureElementsList.erase (i);

      // update measure whole notes
      setMeasureCurrentAccumulatedWholeNotesDuration (
        inputLineNumber,
        fMeasureCurrentAccumulatedWholeNotesDuration
          -
        fMeasureLastHandledNote->getMeasureElementSoundingWholeNotes (),
        "removeElementFromMeasure(): "
          +
        element->asShortString ());

      // return from function
      return;
    }
  } // for

  std::stringstream ss;

  ss <<
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

//   msrInternalError (
//     gServiceRunData->getInputSourceName (),
//     inputLineNumber,
//     __FILE__, __LINE__,
//     ss.str ());
  mxsr2msrInternalError (
    gServiceRunData->getInputSourceName (),
    inputLineNumber,
    __FILE__, __LINE__,
    ss.str ());
}

void msrMeasure::appendNoteToMeasureNotesFlatList (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesFlatView ()) {
    std::stringstream ss;

    ss <<
      "Appending note " <<
//       note->asStringForMeasuresSlices () << // JMI v0.9.70
      note->asShortString () <<
      " to the notes flat list of measure " <<
      this->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureNotesFlatList.push_back (note);
}

void msrMeasure::setMeasurePuristNumberFromVoice ()
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  // get the voice current purist number
  int
    voiceCurrentPuristNumber =
      voice->
        getVoiceCurrentMeasurePuristNumber ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
      std::stringstream ss;

      ss <<
        "Setting the measure purist number of measure '" <<
        fMeasureNumber <<
        "' from voice " <<
        voice->getVoiceName () <<
        " to '" <<
        voiceCurrentPuristNumber <<
        "' in segment " <<
        fMeasureUpLinkToSegment->asString () <<
        " in voice \"" <<
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        ", line " << fInputStartLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  // set measure purist number
  setMeasurePuristNumber (
    voiceCurrentPuristNumber);
}

void msrMeasure::determineMeasureKind (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind)
{
  if (fMeasureKindHasBeenDetermined) {
#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
    if (
      gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
    ) {
#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceParts ()
      ) {
        std::stringstream ss;

        ss <<
          "### MAINTAINANCE ### -- " <<
          "Attempting to determine measure kind for measure \"" <<
          asShortString () <<
          "\" more than once";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputStartLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
  }

  // register measureRepeatContextKind
  // JMI ??? v0.9.70
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
  msrWholeNotes
    wholeNotesSinceLastRegularMeasureEnd =
      voice->
        getWholeNotesSinceLastRegularMeasureEnd ();

  msrWholeNotes
    newWholeNotesSinceLastRegularMeasureEnd =
      wholeNotesSinceLastRegularMeasureEnd
        +
      fMeasureCurrentAccumulatedWholeNotesDuration;

  msrWholeNotes
    fullMeasureWholeNotesDuration =
      getFullMeasureWholeNotesDuration ();

#ifdef MF_TRACE_IS_ENABLED
  if (
//     true ||
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
  ) {
    std::stringstream ss;

    ss <<
      "Determining the measure kind of measure " <<
      this->asShortString () <<

      ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", fullMeasureWholeNotesDuration: " <<
      fullMeasureWholeNotesDuration.asFractionString () <<

      ", fMeasureCurrentAccumulatedWholeNotesDuration < fullMeasureWholeNotesDuration: " <<
      Bool (fMeasureCurrentAccumulatedWholeNotesDuration < fullMeasureWholeNotesDuration) <<
      ", fMeasureCurrentAccumulatedWholeNotesDuration == fullMeasureWholeNotesDuration: " <<
      Bool (fMeasureCurrentAccumulatedWholeNotesDuration == fullMeasureWholeNotesDuration) <<
      ", fMeasureCurrentAccumulatedWholeNotesDuration > fullMeasureWholeNotesDuration: " <<
      Bool (fMeasureCurrentAccumulatedWholeNotesDuration > fullMeasureWholeNotesDuration) <<

      ", wholeNotesSinceLastRegularMeasureEnd: " <<
      wholeNotesSinceLastRegularMeasureEnd <<
      ", newWholeNotesSinceLastRegularMeasureEnd: " <<
      newWholeNotesSinceLastRegularMeasureEnd <<
      ", currentVoiceRepeatPhaseKind: " <<
      currentVoiceRepeatPhaseKind <<
      "' in voice \"" << voice->getVoiceName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "determineMeasureKind() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // determine the measure kind
  if (false && ! fMeasureContainsSound) { // JMI v0.9.72 too early sometimes???
    // empty measure
      handleEmptyMeasure (
        inputLineNumber,
        voice);
  }

  else if (fMeasureCurrentAccumulatedWholeNotesDuration == fullMeasureWholeNotesDuration) {
    // this is a regular measure
      handleRegularMeasure (
        inputLineNumber,
        voice);
  }

  else {
    // this measure is incomplete or overflowing

    // increment voice whole notes since last regular measure end
    voice->
      setWholeNotesSinceLastRegularMeasureEnd (
        inputLineNumber,
        newWholeNotesSinceLastRegularMeasureEnd);

    // set measure kind
    if (fMeasureCurrentAccumulatedWholeNotesDuration < fullMeasureWholeNotesDuration) {
      //  this is an incomplete measure
      handleIncompleteMeasure (
        inputLineNumber,
        voice,
        measureRepeatContextKind,
        newWholeNotesSinceLastRegularMeasureEnd);
    }

    else if (fMeasureCurrentAccumulatedWholeNotesDuration > fullMeasureWholeNotesDuration) {
      // this is an overflowing measure
      handleOverflowingMeasure (
        inputLineNumber,
        voice);
    }

    else {
      // should not occur JMI v0.9.70 BABASSE
      abort();
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

  fMeasureKindHasBeenDetermined = true;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "determineMeasureKind() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrMeasure::handleEmptyMeasure (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "--> handleEmptyMeasure()" <<
        ", fMeasureEndRegularKind: " << fMeasureEndRegularKind;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

// mfAssertFalse (4444);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    voice->
      displayVoiceRepeatsStackMultiMeasureRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "handleEmptyMeasure () 1 measure has 0 measure whole notes");

    std::stringstream ss;

    ss <<
      "measure " <<
      this->asShortString () <<
      " has 0 measure whole notes" <<
      ", " <<
      asString () <<
      ", line " << inputLineNumber;

    if (false) // JMI v0.9.66
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    else
      msrInternalWarning (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set it's measure kind
  setMeasureKind (
    msrMeasureKind::kMeasureKindMusicallyEmpty);

  // increment voice's current measure purist number
  voice->
    incrementVoiceCurrentMeasurePuristNumber (
      inputLineNumber,
      "handleEmptyMeasure () 2 msrMeasureKind::kMeasureKindMusicallyEmpty");

  // reset voice whole notes since last regular measure end  // JMI ??? v0.9.71
  voice->
    setWholeNotesSinceLastRegularMeasureEnd (
      inputLineNumber,
      msrWholeNotes (0, 1));

//   // JMI v0.9.72
//   // append a skip to this measure to fill it
//   appendPaddingSkipNoteToMeasure (
//     inputLineNumber,
//     fFullMeasureWholeNotesDuration);
}

void msrMeasure::handleRegularMeasure (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "--> handleRegularMeasure() 1" <<
      ", fMeasureEndRegularKind: " << fMeasureEndRegularKind;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set it's measure kind
  setMeasureKind (msrMeasureKind::kMeasureKindRegular);

  // this is a regular measure end
  setMeasureEndRegularKind (
    msrMeasureEndRegularKind::kMeasureEndRegularKindYes);

  // increment voice's current measure purist number
  voice->
    incrementVoiceCurrentMeasurePuristNumber (
      inputLineNumber,
      "handleRegularMeasure() 2");

  // reset voice whole notes since last regular measure end
  voice->
    setWholeNotesSinceLastRegularMeasureEnd (
      inputLineNumber,
      msrWholeNotes (0, 1));
}

void msrMeasure::handleIncompleteMeasure (
  int                  inputLineNumber,
  const S_msrVoice&    voice,
  msrMeasureRepeatContextKind
                       measureRepeatContextKind,
  const msrWholeNotes& newWholeNotesSinceLastRegularMeasureEnd)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "--> handleIncompleteMeasure() 1" <<
        ", fMeasureEndRegularKind: " << fMeasureEndRegularKind;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  if (fMeasureIsFirstInVoice) {
    // is this is an anacrusis?

    // set it's measure kind
    // an incomplete first measure is an anacrusis
    // only if there are several measures in the score
    int
      scoreMeasuresNumber =
        fetchMeasureUpLinkToScore ()->getScoreMeasuresNumber ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

    gLog <<
      "=====> handleIncompleteMeasure() 2 -- scoreMeasuresNumber: " <<
      scoreMeasuresNumber <<
      std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    if (scoreMeasuresNumber == 1) {
      setMeasureKind (msrMeasureKind::kMeasureKindIncompleteLastMeasure);
    }
    else {
      setMeasureKind (msrMeasureKind::kMeasureKindAnacrusis);
    }

    // this is a regular measure end
    setMeasureEndRegularKind (
      msrMeasureEndRegularKind::kMeasureEndRegularKindYes);

    // set first incomplete measure purist number to 0
    setMeasurePuristNumber (0);
  }

  else {
    // this is an incomplete measure within the voice

    if (
      newWholeNotesSinceLastRegularMeasureEnd == getFullMeasureWholeNotesDuration ()
    ) {
      // this is a regular measure end
      setMeasureEndRegularKind (
        msrMeasureEndRegularKind::kMeasureEndRegularKindYes);

      // increment voice's current measure purist number
      voice->
        incrementVoiceCurrentMeasurePuristNumber (
          inputLineNumber,
          "handleIncompleteMeasure() 3 kMeasureEndRegularKindYes");

      // reset voice whole notes since last regular measure end
      voice->
        setWholeNotesSinceLastRegularMeasureEnd (
          inputLineNumber,
          msrWholeNotes (0, 1));
    }
    else {
      // this is no regular measure end
      setMeasureEndRegularKind (
        msrMeasureEndRegularKind::kMeasureEndRegularKindNo);
    }

    // set measure's kind according to measureRepeatContextKind
    switch (measureRepeatContextKind) {
      case msrMeasureRepeatContextKind::kMeasureRepeatContext_UNKNOWN_: // JMI ???
        {
          std::stringstream ss;

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceMeasuresDetails ()) {
            displayMeasure (
              inputLineNumber,
              "handleIncompleteMeasure() 4 kMeasureRepeatContext_UNKNOWN_");
          }
#endif // MF_TRACE_IS_ENABLED

          ss <<
            "measure " <<
            this->asShortString () <<
            " is kMeasureRepeatContext_UNKNOWN_ " << // JMI v0.9.66
            asShortString () <<
            ", line " << inputLineNumber;

          // JMI v0.9.71 ignore this
//     // JMI      msrInternalError (
//           msrInternalWarning (
//             gServiceRunData->getInputSourceName (),
//             inputLineNumber,
//      //       __FILE__, __LINE__,
//             ss.str ());
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

void msrMeasure::handleOverflowingMeasure (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
  ) {
    std::stringstream ss;

    ss <<
      "--> handleOverflowingMeasure() " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set it's measure kind
  setMeasureKind (msrMeasureKind::kMeasureKindOverFlowing);

  // this is a regular measure end
  setMeasureEndRegularKind (
    msrMeasureEndRegularKind::kMeasureEndRegularKindYes);

  // increment voice's current measure purist number
  voice->
    incrementVoiceCurrentMeasurePuristNumber (
      inputLineNumber,
      "handleOverflowingMeasure(), msrMeasureKind::kMeasureKindOverFlowing");

  // reset voice whole notes since last regular measure end
  voice->
    setWholeNotesSinceLastRegularMeasureEnd (
      inputLineNumber,
      msrWholeNotes (0, 1));
}

void msrMeasure::finalizeMeasureInRegularVoice (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  const std::string&          context)
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

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    regularPart != nullptr,
    "regularPart is null");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    gLog <<
      "---> regularPart: " <<
      std::endl;

    ++gIndenter;
    gLog <<
      regularPart <<
      std::endl;
    --gIndenter;
    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  msrWholeNotes
    fullMeasureWholeNotesDuration =
      getFullMeasureWholeNotesDuration ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Finalizing measure in regular voice, " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in regular voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", fMeasureOrdinalNumberInVoice: " <<
      fMeasureOrdinalNumberInVoice <<
      ", fullMeasureWholeNotesDuration: " <<
      fullMeasureWholeNotesDuration.asFractionString () <<
      ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//       if (fMeasureCurrentAccumulatedWholeNotesDuration.getNumerator () == 0) {

//   // fetch this measure's whole notes duration in the part measures vector // JMI v0.9.70 WHY???
//   msrWholeNotes
//     measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector =
//       regularPart->
//         fetchPartMeasuresWholeNotesVectorAt (
//           inputLineNumber,
//           fMeasureOrdinalNumberInVoice - 1);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "===> measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector: " <<
//       measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector <<
//       ", fMeasureOrdinalNumberInVoice: " << fMeasureOrdinalNumberInVoice;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED

  // determine the measure purist number from the voice
  setMeasurePuristNumberFromVoice ();

  // determine the measure kind v0.9.70
  determineMeasureKind (
    inputLineNumber,
    measureRepeatContextKind);

  // register this measures's whole notes duration in the part
  S_msrPart
    part =
      this->fetchMeasureUpLinkToPart ();

  part->
    registerOrdinalMeasureNumberWholeNotes (
      inputLineNumber,
      fMeasureOrdinalNumberInVoice,
      fMeasureCurrentAccumulatedWholeNotesDuration);

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureInRegularVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

//   // is there a single note or rest occupying the full measure?
//   if (fMeasureLongestNote) {
//     if (
//       fMeasureLongestNote-> getMeasureElementSoundingWholeNotes ()
//         ==
//       getFullMeasureWholeNotesDuration ()
//     ) {
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTraceMeasures ()) {
//         std::stringstream ss;
//
//         ss <<
//           "Note '" <<
//           fMeasureLongestNote->asShortString () <<
//           "' occupies measure " <<
//           this->asShortString () <<
//           " fully in segment '" <<
//           fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
//           "' in voice \"" <<
//           voice->getVoiceName () <<
//           "\", line " << inputLineNumber <<
//           std::endl;
//
//         gWaeHandler->waeTrace (
//           __FILE__, __LINE__,
//           ss.str ());
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       fMeasureLongestNote->
//         setNoteOccupiesAFullMeasure ();
//     }
//   }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureInRegularVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

// void msrMeasure::handleTheFirstHarmonyInAHarmoniesMeasure (
//   int                 inputLineNumber,
//   const S_msrVoice&   voice,
//   const S_msrHarmony& currentHarmony)
// {
//   // currentHarmony is the first harmony in the measure
//
//   // get currentHarmony's position in the measure
//   msrWholeNotes
//     currentHarmonyMeasurePosition =
//       currentHarmony->
//         getMeasureElementMeasurePosition ();
//
//   // get currentHarmony's note uplink
//   S_msrNote
//     currentHarmonyUpLinkToNote  =
//       currentHarmony->
//         getHarmonyUpLinkToNote ();
//
//   // set currentHarmony's sounding whole notes // HARMFUL
//   currentHarmony->
//     setMeasureElementSoundingWholeNotes (
//       currentHarmonyUpLinkToNote->
//         getMeasureElementSoundingWholeNotes (),
//       "msrMeasure::handleTheFirstHarmonyInAHarmoniesMeasure()");
//
//   // get the currentHarmony's note uplink position in the measure
//   msrWholeNotes
//     currentHarmonyUpLinkToNoteMeasurePosition =
//       currentHarmonyUpLinkToNote->
//         getMeasureElementMeasurePosition ();
//
//   // the position to pad up to is the minimum
//   // of those of the currentHarmony and currentHarmonyUpLinkToNote,
//   // to keep comparison points between the regular voice and its harmonies voice
//   msrWholeNotes
//     measurePositionToPadUpTo =
//       currentHarmonyUpLinkToNoteMeasurePosition;
// //      currentHarmonyMeasurePosition;
//
// /* JMI
//   if (
//     currentHarmonyMeasurePosition
//       <
//     currentHarmonyUpLinkToNoteMeasurePosition
//   ) {
//     measurePositionToPadUpTo =
//       currentHarmonyMeasurePosition;
//   }
// */
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceHarmonies ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> handleTheFirstHarmonyInAHarmoniesMeasure() 1" <<
//       ", currentHarmonyMeasurePosition: " <<
//       currentHarmonyMeasurePosition.asString () <<
//       ", measurePositionToPadUpTo: " <<
//      measurePositionToPadUpTo.asString () <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // is a padding skip note needed?
//   if (measurePositionToPadUpTo.getNumerator () != 0) {
//     // create a padding skip note
//     S_msrNote
//       skipNote =
//         createPaddingSkipNoteForVoice (
//           inputLineNumber,
//           measurePositionToPadUpTo,
//           voice);
//
//     // insert skipNote before currentHarmony in the measure's elements list
//     // this will result in a 'no chord' (N.C.) in LilyPond, for example
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Inserting first padding note " <<
//         skipNote->asString () <<
//         " before currentHarmony " <<
//         currentHarmony->asString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str (),
//     }
// #endif // MF_TRACE_IS_ENABLED
//
// //     // insert skipNote in the measure elements list before (*i)
// //     insertElementInMeasureBeforeIterator (
// //       inputLineNumber,
// //       i,
// //       skipNote);
//
// //   // set current harmony's element measure position
// //   currentHarmony->
// //     setMeasureElementMeasurePosition (
// //       this,
// //       measurePositionToPadUpTo,
// //       "first harmony in measure");
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
//       displayMeasure (
//         inputLineNumber,
//         "handleTheFirstHarmonyInAHarmoniesMeasure() 2");
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
// }
//
// void msrMeasure::handleASubsequentHarmonyInAHarmoniesMeasure (
//   int                 inputLineNumber,
//   const S_msrVoice&   voice,
//   const S_msrHarmony& previousHarmony,
//   const S_msrHarmony& currentHarmony)
// {
//   // currentHarmony is a subsequent harmony in the measure,
//   // occurring after previousHarmony
//
//   // get currentHarmony' position in the measure
//   msrWholeNotes
//     currentHarmonyMeasurePosition =
//       currentHarmony->
//         getMeasureElementMeasurePosition ();
//
//   // get previousHarmony's position in the measure
//   msrWholeNotes
//     previousHarmonyMeasurePosition =
//       previousHarmony->getMeasureElementMeasurePosition ();
//
//   // get previousHarmony's duration
//   msrWholeNotes
//     previousHarmonySoundingWholeNotes =
//       previousHarmony->getMeasureElementSoundingWholeNotes ();
//
//   // compute the measure position following previousHarmony
//   msrWholeNotes
//     measurePositionFollowingPreviousHarmony =
//       previousHarmonyMeasurePosition
//         +
//       previousHarmonySoundingWholeNotes;
//
//   // compute the measure positions delta
//   // between previousHarmony and currentHarmony
//   msrWholeNotes
//     measurePositionsDelta =
//       currentHarmonyMeasurePosition
//         -
//       measurePositionFollowingPreviousHarmony;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceHarmonies ()) {
//     mfIndentedStringStream iss;
//
//     iss <<
//       "---> handleASubsequentHarmonyInAHarmoniesMeasure() 1" <<
//       '\n';
//
//     ++gIndenter;
//
//     iss <<
//       "previousHarmony:" <<
//       '\n';
//
//     ++gIndenter;
//     if (previousHarmony) {
//       iss <<
//         previousHarmony->asString ();
//     }
//     else {
//       iss << "[NULL]";
//     }
//     --gIndenter;
//     iss << '\n';
//
//     iss <<
//       "currentHarmony: " <<
//       '\n';
//
//     ++gIndenter;
//     iss <<
//       currentHarmony->asString () <<
//       '\n';
//     --gIndenter;
//
//     iss <<
//       "previousHarmonyMeasurePosition: " <<
//       previousHarmonyMeasurePosition.asString () <<
//       '\n' <<
//       "currentHarmonyMeasurePosition: " <<
//       currentHarmonyMeasurePosition.asString () <<
//       '\n' <<
//       "measurePositionFollowingPreviousHarmony: " <<
//       measurePositionFollowingPreviousHarmony <<
//       '\n' <<
//       "measurePositionsDelta: " <<
//       measurePositionsDelta <<
//       '\n';
//
//     --gIndenter;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       iss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   msrWholeNotes
//     measurePositionToPadUpTo =
//       measurePositionFollowingPreviousHarmony;
//
//   // is a padding skip note needed?
//   if (measurePositionToPadUpTo.getNumerator () != 0) {
//     // create a padding skip note
//     S_msrNote
//       skipNote =
//         createPaddingSkipNoteForVoice (
//           inputLineNumber,
//           measurePositionToPadUpTo,
//           voice);
//
//     // insert skipNote before currentHarmony in the measure's elements list
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Inserting first padding note " <<
//         skipNote->asString () <<
//         " before currentHarmony " <<
//         currentHarmony->asString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str (),
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     // insert skipNote in the measure elements list before (*i)
// //     insertElementInMeasureBeforeIterator (
// //       inputLineNumber,
// //       i,
// //       skipNote);
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
//       displayMeasure (
//         inputLineNumber,
//         "handleTheFirstHarmonyInAHarmoniesMeasure() 2");
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
//
//   // update the previous harmony sounding whole notes duration if relevant
//   // to 'fill the gap' to the current harmony
//   if (previousHarmony) {
//     msrWholeNotes
//       newPreviousHarmonyWholeNotes =
//         currentHarmony->getMeasureElementMeasurePosition ()
//           -
//         previousHarmony->getMeasureElementMeasurePosition ();
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Setting the sounding whole notes duration of harmony " <<
//         previousHarmony->asString () <<
//         " to " <<
//         newPreviousHarmonyWholeNotes.asFractionString () <<
//         std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str (),
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     // set previousHarmony's sounding whole notes // HARMFUL
//     previousHarmony->
//       setMeasureElementSoundingWholeNotes (
//         newPreviousHarmonyWholeNotes,
//         "msrMeasure::handleASubsequentHarmonyInAHarmoniesMeasure()");
//   }
// }
//
void msrMeasure::handleTheLastHarmonyInAHarmoniesMeasure (
  int                 inputLineNumber,
  const S_msrVoice&   voice,
  const S_msrHarmony& currentHarmony)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Handling the last harmony " <<
      currentHarmony->asString () <<
      " while finalizing harmonies measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // does currentHarmony overflow the measure?

  // get the currentHarmony's position in the measure
  msrWholeNotes
    currentHarmonyMeasurePosition =
      currentHarmony->
        getMeasureElementMeasurePosition ();

  // get the currentHarmony's sounding whole notes
  msrWholeNotes
    currentHarmonySoundingWholeNotes =
      currentHarmony->
        getMeasureElementSoundingWholeNotes ();

  // compute the measure position following currentHarmony
  msrWholeNotes
    measurePositionFollowingCurrentHarmony =
      currentHarmonyMeasurePosition
        +
      currentHarmonySoundingWholeNotes;

//   // get the currentHarmony's note uplink // JMI v0.9.67
//   S_msrNote
//     currentHarmonyUpLinkToNote  =
//       currentHarmony->
//         getHarmonyUpLinkToNote ();
//
//   // compute the measure overflow whole notes
//   msrWholeNotes
//     measureOverflowWholeNotes =
//       measurePositionFollowingCurrentHarmony
//         -
//       getFullMeasureWholeNotesDuration ();

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

  // fetch the measure whole notes from the part measures fector
  msrWholeNotes
    measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector =
      harmoniesPart->
        fetchPartMeasuresWholeNotesVectorAt (
          inputLineNumber,
          fMeasureOrdinalNumberInVoice - 1);

  // compute the gap at the end of the measure if any
  msrWholeNotes
    gapAtTheEndOfTheMeasure =
      measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector
        -
      measurePositionFollowingCurrentHarmony;

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceHarmonies ()
      ||
    gTraceOahGroup->getTraceMeasurePositions ()
      ||
    gTraceOahGroup->getTraceNotesDurations ()
  ) {
    mfIndentedStringStream iss;

    iss <<
      "--> handleTheLastHarmonyInAHarmoniesMeasure() 1:" <<
      '\n';

    ++gIndenter;

    iss <<
      "currentHarmony:" <<
      '\n';

    ++gIndenter;
    if (currentHarmony) {
      iss <<
        currentHarmony->asString ();
    }
    else {
      iss << "[NULL]";
    }
    iss << '\n';
    --gIndenter;

//     iss <<
//       "currentHarmonyUpLinkToNote:" <<
//       '\n';
//
//     ++gIndenter;
//     iss <<
//       currentHarmonyUpLinkToNote->asString () <<
//       '\n';
//     --gIndenter;

    iss <<
      "currentHarmonyMeasurePosition: " <<
      currentHarmonyMeasurePosition.asString () <<
      '\n' <<

      "currentHarmonySoundingWholeNotes: " <<
      currentHarmonySoundingWholeNotes.asFractionString () <<
      '\n' <<

      "measurePositionFollowingCurrentHarmony: " <<
      measurePositionFollowingCurrentHarmony <<
      '\n' <<

      "getFullMeasureWholeNotesDuration (): " <<
      getFullMeasureWholeNotesDuration ().asString () <<
      '\n' <<

      "gapAtTheEndOfTheMeasure: " <<
      gapAtTheEndOfTheMeasure;

    --gIndenter;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      iss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (gapAtTheEndOfTheMeasure.getNumerator () > 0) {
    // a padding rest is needed at the end of the measure,
    // unless the latter is the only one in the voice v0.9.67 HARMFUL
//     if (
//       fMeasureNumber
//         !=
//       fetchMeasureUpLinkToScore ()->getScoreLastMeasureNumber ()
//     ) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Harmony " <<
        currentHarmony->asString () <<
        " does not reach the end of measure " <<
        this->asShortString () <<
        ", the gap is " <<
        gapAtTheEndOfTheMeasure.asString () <<
        ", in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

      // pad up to measurePositionToAppendAt
      padUpToMeasurePosition (
        inputLineNumber,
        getFullMeasureWholeNotesDuration ());
    }
//   }

  else if (gapAtTheEndOfTheMeasure.getNumerator () < 0) {
    // the currentHarmony overflows the measure
    std::stringstream ss;

    ss <<
      "The current harmony " <<
      currentHarmony->asString () <<
      " overflows the current measure " <<
      asString ();

    msrInternalWarning (
//     msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
//       __FILE__, __LINE__,
      ss.str ());

//     // compute currentHarmony's future sounding whole notes
//     msrWholeNotes
//       reducedSoundingWholeNotes =
//         currentHarmonySoundingWholeNotes
//           - // the delta is positive
//         measureOverflowWholeNotes;
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Reducing the sounding whole notes of harmony " <<
//         currentHarmony->asString () <<
//         " from " <<
//         currentHarmonySoundingWholeNotes.asFractionString () <<
//         " to " <<
//         reducedSoundingWholeNotes.asFractionString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str (),
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     if (reducedSoundingWholeNotes.getNumerator () == 0) {
//       std::stringstream ss;
//
//       ss <<
//         "Cannot reduce the duration of harmony " <<
//         currentHarmony->asShortString () <<
//         " to 0 : leaving it as it is";
//
//       msrInternalWarning
//         gServiceRunData->getInputSourceName (),
//         inputLineNumber,
// //  JMI             __FILE__, __LINE__,
//         ss.str (),
//     }
//     else {
//       // set currentHarmony's duration to the reduced value
//       currentHarmony->
//         setMeasureElementSoundingWholeNotes (
//           reducedSoundingWholeNotes,
//           "msrMeasure::handleTheLastHarmonyInAHarmoniesMeasure() 2");
//     }
  }

  else {
    // the currentHarmony reaches the end of the measure
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Harmony " <<
        currentHarmony->asString () <<
        " reaches the end of measure " <<
        this->asShortString () <<
        ", nothing more to do" <<
        ", in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

void msrMeasure::finalizeTheHarmoniesInAHarmoniesMeasure (
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Finalizing the harmonies in a harmonies measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fMeasureElementsList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeTheHarmoniesInAHarmoniesMeasure() 1");
    }
#endif // MF_TRACE_IS_ENABLED

    // in MusicXML, harmonies don't have their own duration
    // and may have a non-null offset:
    // we thus have to sort them in the measure by increasing position,
    // taking their offset into account
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Sorting the elements in harmonies measure " <<
        this->asShortString () <<
        " in segment '" <<
        fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
        "' in voice \"" <<
        voice->getVoiceName () <<
        "\" (" << context << ")" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fMeasureElementsList.sort (
      msrMeasureElement::compareMeasureElementsByIncreasingMeasurePosition);

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeTheHarmoniesInAHarmoniesMeasure() 2");
    }
#endif // MF_TRACE_IS_ENABLED

    // consider each measure element (a harmony in fact) in turn,
    // updating their duration and adding skips if needed
    S_msrHarmony
      previousHarmony = nullptr,
      currentHarmony  = nullptr;

    for (S_msrMeasureElement measureElement : fMeasureElementsList) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceHarmonies ()) {
        std::stringstream ss;

        ss <<
          "--> finalizeTheHarmoniesInAHarmoniesMeasure() 3" <<
          ", measureElement: " << measureElement <<
          this->asShortString () <<
          " in segment '" <<
          fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
          "' in voice \"" <<
          voice->getVoiceName () <<
          "\" (" << context << ")" <<
          ", line " << measureElement->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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
//         finalizeHarmonyInAHarmoniesMeasure ( // JMI HARMFULL
//           measureElement->getInputStartLineNumber (),
//           voice,
//           previousHarmony,
//           currentHarmony,
//           context);

        previousHarmony = currentHarmony;
      }

      else if (
        // note? JMI
        S_msrNote
          note =
            dynamic_cast<msrNote*>(&(*measureElement))
      ) {
        switch (note->getNoteKind ()) {
          case msrNoteKind::kNote_UNKNOWN_:
            break;

          // in measures
          case msrNoteKind::kNoteSkipInMeasure:
            // that's fine
            break;

          case msrNoteKind::kNoteRegularInMeasure:
          case msrNoteKind::kNoteRestInMeasure:
          case msrNoteKind::kNoteUnpitchedInMeasure:
          case msrNoteKind::kNoteRegularInChord:
          case msrNoteKind::kNoteRegularInTuplet:
          case msrNoteKind::kNoteRestInTuplet:
          case msrNoteKind::kNoteUnpitchedInTuplet:
          case msrNoteKind::kNoteRegularInGraceNotesGroup:
          case msrNoteKind::kNoteSkipInGraceNotesGroup:
          case msrNoteKind::kNoteInChordInGraceNotesGroup:
          case msrNoteKind::kNoteInTupletInGraceNotesGroup:
          case msrNoteKind::kNoteInDoubleTremolo:
            {
              std::stringstream ss;

              ss <<
                "Finalizing the harmonies in a harmonies measure containing note " <<
                note->asShortString () <<
                std::endl;

              msrInternalError (
                gServiceRunData->getInputSourceName (),
                measureElement->getInputStartLineNumber (),
                __FILE__, __LINE__,
                ss.str ());
            }
        } // switch
      }
    } // for

    // handle the last harmony in the measure if any,
    // otherwise padup to this empty measure's end
    if (currentHarmony) {
       handleTheLastHarmonyInAHarmoniesMeasure ( // JMI HARMFUL
         inputLineNumber,
         voice,
         currentHarmony);
    }
    else {
#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceMeasures ()
          ||
        gTraceOahGroup->getTraceHarmonies ()
      ) {
        std::stringstream ss;

        ss <<
          "*** the current harmonies measure is empty ***" <<
          ", line " << inputLineNumber;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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
        gLog <<
          "---> regularPart: " <<
          std::endl;

        ++gIndenter;
        gLog <<
          regularPart <<
          std::endl;
        --gIndenter;
        gLog << std::endl;
      }

      msrWholeNotes
        measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector =
          regularPart->
            fetchPartMeasuresWholeNotesVectorAt (
              inputLineNumber,
              fMeasureOrdinalNumberInVoice - 1);

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMeasures ()) {
        std::stringstream ss;

        ss <<
          "===> measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector: " <<
          measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector <<
          ", fMeasureOrdinalNumberInVoice: " << fMeasureOrdinalNumberInVoice;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      padUpToPositionAtTheEndOfTheMeasure ( // JMI ??? v0.9.67
        inputLineNumber,
        measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector,
        "finalizeTheHarmoniesInAHarmoniesMeasure()");
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeTheHarmoniesInAHarmoniesMeasure() 4");
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

// void msrMeasure::finalizeHarmonyInAHarmoniesMeasure (
//   int                 inputLineNumber,
//   const S_msrVoice&   voice,
//   const S_msrHarmony& previousHarmony,
//   const S_msrHarmony& currentHarmony,
//   const std::string&  context)
// {
//   // handle the currentHarmony
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceHarmonies ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Finalizing harmony " <<
//       currentHarmony->asString () <<
//       " while finalizing harmonies measure " <<
//       this->asShortString () <<
//       " in segment '" <<
//       fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
//       "' in voice \"" <<
//       voice->getVoiceName () <<
//       "\" (" << context << ")" <<
//       ", line " << inputLineNumber <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // get currentHarmony's note uplink
//   S_msrNote
//     currentHarmonyUpLinkToNote =
//       currentHarmony->
//         getHarmonyUpLinkToNote (); // JMI v0.9.66
//
//   // set currentHarmony's sounding whole notes
//   // JMI v0.9.67 NOT if there are several figured basses with offsets on the same note -- HARMFUL!
// //   currentHarmony->
// //     setMeasureElementSoundingWholeNotes (
// //       currentHarmonyUpLinkToNote->
// //         getMeasureElementSoundingWholeNotes (),
// //       "msrMeasure::finalizeHarmonyInAHarmoniesMeasure()");
//
// #ifdef MF_TRACE_IS_ENABLED
//   // get the currentHarmony's note uplink position in the measure
//   msrWholeNotes
//     currentHarmonyUpLinkToNoteMeasurePosition =
//       currentHarmonyUpLinkToNote->
//         getMeasureElementMeasurePosition ();
//
//   if (gTraceOahGroup->getTraceHarmonies ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> finalizeHarmonyInAHarmoniesMeasure() 2" <<
//       ", previousHarmony: ";
//
//     if (previousHarmony) {
//       ss <<
//         previousHarmony->asString ();
//     }
//     else {
//       ss << "[NULL]";
//     }
//
//     ss <<
//       ", currentHarmonyUpLinkToNoteMeasurePosition: " <<
//       currentHarmonyUpLinkToNoteMeasurePosition.asString () <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (! previousHarmony) {
//     handleTheFirstHarmonyInAHarmoniesMeasure (
//       inputLineNumber,
//       voice,
//       currentHarmony);
//   }
//
//   else {
//     handleASubsequentHarmonyInAHarmoniesMeasure (
//       inputLineNumber,
//       voice,
//       previousHarmony,
//       currentHarmony);
//   }
// }

void msrMeasure::handleFirstFiguredBassInFiguredBassMeasure (
  int                     inputLineNumber,
  const S_msrVoice&       voice,
  std::list <S_msrMeasureElement>::iterator&
                          i,
  const S_msrFiguredBass& previousFiguredBass,
  const S_msrFiguredBass& currentFiguredBass,
  const msrWholeNotes&    currentFiguredBassMeasurePosition)
{
  // currentFiguredBass is the first figured bass in the measure

  // the position to pad up to is the minimum
  // of those of the currentFiguredBass and currentFiguredBassUpLinkToNote,
  // to keep comparison points between the regular voice and its figured bass voice
  msrWholeNotes
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "handleFirstFiguredBassInFiguredBassMeasure() 5" <<
      ", previousFiguredBass is null, measurePositionToPadUpTo: " <<
      measurePositionToPadUpTo.asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    // this will result in a 'no chord' (N.C.) in LilyPond, for example
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Inserting first padding note " <<
        skipNote->asString () <<
        " before currentFiguredBass " <<
        currentFiguredBass->asString () <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // insert skipNote in the measure elements list before (*i)
    insertElementInMeasureBeforeIterator (
      inputLineNumber,
      i,
      skipNote);

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "handleFirstFiguredBassInFiguredBassMeasure() 6");
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

void msrMeasure::handleSubsequentFiguredBassInFiguredBassMeasure (
  int                     inputLineNumber,
  const S_msrVoice&       voice,
  std::list <S_msrMeasureElement>::iterator&
                          i,
  const S_msrFiguredBass& previousFiguredBass,
  const S_msrFiguredBass& currentFiguredBass,
  const msrWholeNotes&    currentFiguredBassMeasurePosition)
{
  // this is a subsequent figured bass in the measure

  // get the previousFiguredBass's position in the measure
  msrWholeNotes
    previousFiguredBassMeasurePosition =
      previousFiguredBass->getMeasureElementMeasurePosition ();

  // get the previousFiguredBass's duration
  msrWholeNotes
    previousFiguredBassSoundingWholeNotes =
      previousFiguredBass->getMeasureElementSoundingWholeNotes ();

  // compute the measure position following previousFiguredBass
  msrWholeNotes
    measurePositionFollowingPreviousFiguredBass =
      previousFiguredBassMeasurePosition
        +
      previousFiguredBassSoundingWholeNotes;

  // compute the measure positions delta
  msrWholeNotes
    measurePositionsDelta =
      currentFiguredBassMeasurePosition
        -
      measurePositionFollowingPreviousFiguredBass;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "handleSubsequentFiguredBassInFiguredBassMeasure() 7" <<
      ", previousFiguredBass: ";

    if (previousFiguredBass) {
      ss <<
        previousFiguredBass->asString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      ", currentFiguredBass: " <<
      currentFiguredBass->asString () <<
      ", previousFiguredBassMeasurePosition: " <<
      previousFiguredBassMeasurePosition.asString () <<
      ", currentFiguredBassMeasurePosition: " <<
      currentFiguredBassMeasurePosition.asString () <<
      ", measurePositionFollowingPreviousFiguredBass: " <<
      measurePositionFollowingPreviousFiguredBass <<
      ", measurePositionsDelta: " <<
      measurePositionsDelta;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
//         fMeasureCurrentAccumulatedWholeNotesDuration,
//         "handleSubsequentFiguredBassInFiguredBassMeasure() 8");

    // insert skipNote before currentFiguredBass in the measure's elements list
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Inserting subsequent padding note " <<
        skipNote->asString () <<
        " before currentFiguredBass " <<
        currentFiguredBass->asString () <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // insert skipNote in the measure elements list before (*i)
    insertElementInMeasureBeforeIterator (
      inputLineNumber,
      i,
      skipNote);
  }

  else if (measurePositionsDelta.getNumerator () < 0) {
    // the two harmonies overlap in time
    std::stringstream ss;

    ss <<
      "Previous figured bass " <<
      previousFiguredBass->asString () <<
      " overlaps current figured bass " <<
      currentFiguredBass->asString ();

    msrInternalWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str ());

    // compute previousFiguredBass's future sounding whole notes
    msrWholeNotes
      reducedSoundingWholeNotes =
        previousFiguredBassSoundingWholeNotes
          + // the delta is negative
        measurePositionsDelta;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Reducing the sounding whole notes of figured bass " << // JMI v0.9.67
        previousFiguredBass->asString () <<
        " from " <<
        previousFiguredBassSoundingWholeNotes.asFractionString () <<
        " to " <<
        reducedSoundingWholeNotes.asFractionString () <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    if (reducedSoundingWholeNotes.getNumerator () == 0) {
      std::stringstream ss;

      ss <<
        "Cannot reduce the duration of figured bass " <<
        previousFiguredBass->asShortString () <<
        " to 0 : leaving it as it is";

      msrInternalWarning (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
//  JMI             __FILE__, __LINE__,
        ss.str ());
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

void msrMeasure::handleTheLastFiguredBassInFiguredBassMeasure (
  int                     inputLineNumber,
  const S_msrVoice&       voice,
  const S_msrFiguredBass& currentFiguredBass)
{
  // does currentFiguredBass overflow the measure?

  // get the currentFiguredBass's position in the measure
  msrWholeNotes
    currentFiguredBassMeasurePosition =
      currentFiguredBass->getMeasureElementMeasurePosition ();

  // get the currentFiguredBass's sounding whole notes
  msrWholeNotes
    currentFiguredBassSoundingWholeNotes =
      currentFiguredBass->
        getMeasureElementSoundingWholeNotes ();

  // compute the measure position following currentFiguredBass
  msrWholeNotes
    measurePositionFollowingCurrentFiguredBass =
      currentFiguredBassMeasurePosition
        +
      currentFiguredBassSoundingWholeNotes;

  // get the currentFiguredBass's note uplink
  S_msrNote
    currentFiguredBassUpLinkToNote  =
      currentFiguredBass->
        getFiguredBassUpLinkToNote ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
    std::stringstream ss;

    ss <<
      "handleTheLastFiguredBassInFiguredBassMeasure() 1" <<
      ", currentFiguredBassUpLinkToNote:" <<
      std::endl;
    ++gIndenter;
    ss <<
      currentFiguredBassUpLinkToNote <<
      std::endl;
    --gIndenter;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // compute the measure overflow whole notes
  msrWholeNotes
    measureOverflowWholeNotes =
      measurePositionFollowingCurrentFiguredBass
        -
      getFullMeasureWholeNotesDuration ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "handleTheLastFiguredBassInFiguredBassMeasure() 2" <<
      ", currentFiguredBass: ";

    if (currentFiguredBass) {
      ss <<
        currentFiguredBass->asString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      ", currentFiguredBassMeasurePosition: " <<
      currentFiguredBassMeasurePosition.asString () <<
      ", currentFiguredBassSoundingWholeNotes: " <<
      currentFiguredBassSoundingWholeNotes.asFractionString () <<
      ", measurePositionFollowingCurrentFiguredBass: " <<
      measurePositionFollowingCurrentFiguredBass <<
      /* JMI
      ", measurePositionFollowingCurrentFiguredBassUpLinkToNote: " <<
      measurePositionFollowingCurrentFiguredBassUpLinkToNote <<
      ", currentFiguredBassUpLinkToNoteSoundingWholeNotes: " <<
      currentFiguredBassUpLinkToNoteSoundingWholeNotes.asFractionString () <<
      ", measurePositionFollowingCurrentFiguredBassUpLinkToNote: " <<
      measurePositionFollowingCurrentFiguredBassUpLinkToNote <<
      */
      ", currentFiguredBassSoundingWholeNotes: " <<
      currentFiguredBassSoundingWholeNotes.asFractionString () <<
      ", measureOverflowWholeNotes: " <<
      measureOverflowWholeNotes;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (measureOverflowWholeNotes.getNumerator () > 0) {
    // the currentFiguredBass's duration is too large
    std::stringstream ss;

    ss <<
      "The current figured bass " <<
      currentFiguredBass->asString () <<
      " overflows the current measure " <<
      asString ();

    msrInternalWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str ());

    // compute currentFiguredBass's future sounding whole notes
    msrWholeNotes
      reducedSoundingWholeNotes =
        currentFiguredBassSoundingWholeNotes
          - // the delta is positive
        measureOverflowWholeNotes;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Reducing the sounding whole notes of figured bass " <<
        currentFiguredBass->asString () <<
        " from " <<
        currentFiguredBassSoundingWholeNotes.asFractionString () <<
        " to " <<
        reducedSoundingWholeNotes.asFractionString () <<
        " in voice \"" <<
        voice->getVoiceName () <<
        "\", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    if (reducedSoundingWholeNotes.getNumerator () == 0) {
      std::stringstream ss;

      ss <<
        "Cannot reduce the duration of figured bass " <<
        currentFiguredBass->asShortString () <<
        " to 0 : leaving it as it is";

      msrInternalWarning (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
//  JMI             __FILE__, __LINE__,
        ss.str ());
    }
    else {
      // set currentFiguredBass's duration to the reduced value
      currentFiguredBass->
        setMeasureElementSoundingWholeNotes (
          reducedSoundingWholeNotes,
          "handleTheLastFiguredBassInFiguredBassMeasure () 3");
    }
  }
}

void msrMeasure::finalizeTheFiguredBassesInAFiguredBassMeasure (
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Finalizing the figured basses in figured bass measure " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
      voice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fMeasureElementsList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeTheFiguredBassesInAFiguredBassMeasure() 1");
    }
#endif // MF_TRACE_IS_ENABLED

    // in MusicXML, figured basses may have a duration,
    // used to indicate changes of figures under a note.
    // we thus have to distribute there durations over the notes'duration
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Sorting the elements in figured bass measure " <<
//         this->asShortString () <<
//         " in segment '" <<
//         fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
//         "' in voice \"" <<
//         voice->getVoiceName () <<
//         "\" (" << context << ")" <<
//         ", line " << inputLineNumber <<
//         std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str (),
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     fMeasureElementsList.sort ( // JMI v0.9.67
//       msrMeasureElement::compareMeasureElementsByIncreasingMeasurePosition);
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       displayMeasure (
//         inputLineNumber,
//         "finalizeTheFiguredBassesInAFiguredBassMeasure() 2");
//     }
// #endif // MF_TRACE_IS_ENABLED

    // consider each measure element (a figured bass in fact) in turn,
    // updating their duration and adding skips if needed
    std::list <S_msrMeasureElement>::iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;

    S_msrFiguredBass
      previousFiguredBass = nullptr,
      currentFiguredBass  = nullptr;

    while (true) {
      S_msrMeasureElement
        measureElement = (*i);

      if (++i == iEnd) break;

      if (false) {
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
  #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceHarmonies ()) {
            std::stringstream ss;

            ss <<
              "finalizeTheFiguredBassesInAFiguredBassMeasure() 3" <<
              ", currentFiguredBass: ";
              ++gIndenter;
              ss <<
                currentFiguredBass->asString () <<
                std::endl;
              --gIndenter;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
  #endif // MF_TRACE_IS_ENABLED

          // its position in the measure should take it's offset into account
          msrWholeNotes
            currentFiguredBassMeasurePosition =
              currentFiguredBass->
                getMeasureElementMeasurePosition ();

          // get the currentFiguredBass's note uplink
          S_msrNote
            currentFiguredBassUpLinkToNote  =
              currentFiguredBass->
                getFiguredBassUpLinkToNote ();

  #ifdef MF_TRACE_IS_ENABLED
          // get the currentFiguredBass's note uplink position in the measure
          msrWholeNotes
            currentFiguredBassUpLinkToNoteMeasurePosition =
              currentFiguredBassUpLinkToNote->
                getMeasureElementMeasurePosition ();

          if (gTraceOahGroup->getTraceHarmonies () ) {
            std::stringstream ss;

            ss <<
              "finalizeTheFiguredBassesInAFiguredBassMeasure() 4" <<
              ", previousFiguredBass: ";

            if (previousFiguredBass) {
              ss <<
                previousFiguredBass->asString ();
            }
            else {
              ss << "[NULL]";
            }

            ss <<
              ", currentFiguredBassMeasurePosition: " <<
              currentFiguredBassMeasurePosition.asString () <<
              ", currentFiguredBassUpLinkToNoteMeasurePosition: " <<
              currentFiguredBassUpLinkToNoteMeasurePosition.asString () <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
  #endif // MF_TRACE_IS_ENABLED

          if (! previousFiguredBass) {
            handleFirstFiguredBassInFiguredBassMeasure (
              measureElement->getInputStartLineNumber (),
              voice,
              i,
              previousFiguredBass,
              currentFiguredBass,
              currentFiguredBassMeasurePosition);
          }

          else {
            handleSubsequentFiguredBassInFiguredBassMeasure (
              measureElement->getInputStartLineNumber (),
              voice,
              i,
              previousFiguredBass,
              currentFiguredBass,
              currentFiguredBassMeasurePosition);
          }

          previousFiguredBass = currentFiguredBass;
        }
      } // while
    }

    if (currentFiguredBass) {
      handleTheLastFiguredBassInFiguredBassMeasure (
        inputLineNumber,
        voice,
        currentFiguredBass);
    }
/* JMI
    else {
      gLog <<
        "*** currentFiguredBass is null ***" << // JMI
        std::endl;
    }
    */

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeTheFiguredBassesInAFiguredBassMeasure() 5");
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

void msrMeasure::finalizeMeasureInHarmonyVoice (
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Finalizing measure in harmony voice, " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in harmonies voice \"" <<
      harmoniesVoice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureInHarmonyVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  // get the harmoniesPart number of measures
  int
    harmoniesPartNumberOfMeasures =
      harmoniesPart->
        getPartNumberOfMeasures ();

  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "--> fMeasureOrdinalNumberInVoice: " <<
      fMeasureOrdinalNumberInVoice <<
      ", harmoniesPartNumberOfMeasures: " <<
      harmoniesPartNumberOfMeasures;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the harmonies in this measure
  finalizeTheHarmoniesInAHarmoniesMeasure (
    inputLineNumber,
    context);

  // fetch the measure whole notes from the part measures fector
  msrWholeNotes
    measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector =
      harmoniesPart->
        fetchPartMeasuresWholeNotesVectorAt (
          inputLineNumber,
          fMeasureOrdinalNumberInVoice - 1);

  // pad the measure up to measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector
  padUpToPositionAtTheEndOfTheMeasure ( // JMI ??? v0.9.67
    inputLineNumber,
    measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector,
    "finalizeMeasureInHarmonyVoice()");

  // determine the measure purist number from the voice
  setMeasurePuristNumberFromVoice ();

  // determine the measure kind
  determineMeasureKind (
    inputLineNumber,
    measureRepeatContextKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureInHarmonyVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrMeasure::finalizeMeasureInFiguredBassVoice (
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Finalizing measure in figured bass voice, " <<
      this->asShortString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in figured bass voice \"" <<
      figuredBassVoice->getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBassesDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureInFiguredBassVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  // get the figuredBassPart number of measures
  int
    figuredBassPartNumberOfMeasures =
      figuredBassPart->
        getPartNumberOfMeasures ();

  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "fMeasureOrdinalNumberInVoice: " <<
      fMeasureOrdinalNumberInVoice <<
      ", figuredBassPartNumberOfMeasures: " <<
      figuredBassPartNumberOfMeasures;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the figured bass element in this measure
//   finalizeTheFiguredBassesInAFiguredBassMeasure ( // JMI HARMFULL
//     inputLineNumber,
//     context);

  // the measureCurrentAccumulatedWholeNotesDuration has to be computed
  // only now because finalizeTheHarmoniesInAHarmoniesMeasure()
  // may have incremented a harmony sounding whole notes duration
  msrWholeNotes
    measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector =
      figuredBassPart->
        fetchPartMeasuresWholeNotesVectorAt (
          inputLineNumber,
          fMeasureOrdinalNumberInVoice - 1);

  // pad the measure up to measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector
  // only if the measure doesn't contain any non-rest note,
  // otherwise the last figured bass element in the measure has already been extended to the end of the measure
  padUpToPositionAtTheEndOfTheMeasure ( // JMI ??? v0.9.67
    inputLineNumber,
    measureCurrentAccumulatedWholeNotesDurationFromPartMeasuresVector,
    "finalizeMeasureInFiguredBassVoice()");

  // determine the measure purist number from the voice
  setMeasurePuristNumberFromVoice ();

  // determine the measure kind
  determineMeasureKind (
    inputLineNumber,
    measureRepeatContextKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBassesDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureInFiguredBassVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrMeasure::finalizeMeasure (
  int                         inputLineNumber,
  msrMeasureRepeatContextKind measureRepeatContextKind,
  const std::string&          context)
{
  if (fMeasureHasBeenFinalized) {
#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
    if (
      gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
    ) {
#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceParts ()
      ) {
        std::stringstream ss;

        ss <<
          "### MAINTAINANCE ### -- " <<
          "Attempt at finalizing measure " <<
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

        if (gTraceOahGroup->getTraceMeasuresDetails ()) {
          std::stringstream ss;

          ss <<
            std::endl <<
            ss.str () <<
            std::endl << std::endl;

          S_msrStaff
            staff =
              fetchMeasureUpLinkToStaff ();

          ss <<
            "staff:" <<
            std::endl;

          ++gIndenter;
          ss << staff;
          ss << std::endl;
          --gIndenter;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }

  //     msrInternalError ( // JMI v0.9.70
        msrInternalWarning (
          gServiceRunData->getInputSourceName (),
          fInputStartLineNumber,
    //       __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
  }

  else {
    S_msrVoice
      voice =
        fMeasureUpLinkToSegment->
          getSegmentUpLinkToVoice ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "Finalizing measure 111 " <<
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

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

//     gLog <<
//       std::endl <<
//       this <<
//       std::endl;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      if (fMeasureCurrentAccumulatedWholeNotesDuration.getNumerator () == 0) {
        std::stringstream ss;

        ss <<
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
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          ss.str ());
      }
    }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresDetails ()) {
      displayMeasure (
        inputLineNumber,
        "finalizeMeasure() 1");
    }
#endif // MF_TRACE_IS_ENABLED

    // delegate finalization to voice kind specific methods
    switch (voice->getVoiceKind ()) {
      case msrVoiceKind::kVoiceKindRegular:
        finalizeMeasureInRegularVoice (
          inputLineNumber,
          measureRepeatContextKind,
          context);
        break;

      case msrVoiceKind::kVoiceKindDynamics:
        break;

      case msrVoiceKind::kVoiceKindHarmonies:
        finalizeMeasureInHarmonyVoice (
          inputLineNumber,
          measureRepeatContextKind,
          context);
        break;

      case msrVoiceKind::kVoiceKindFiguredBass:
        finalizeMeasureInFiguredBassVoice (
          inputLineNumber,
          measureRepeatContextKind,
          context);
        break;
    } // switch

    // fetch voice position
    msrWholeNotes
      voicePosition =
        fetchMeasureUpLinkToVoice ()->
          getCurrentVoicePosition ();

    // delegate position measure assignment to the elements in the measure
    for (S_msrMeasureElement measureElement : fMeasureElementsList) {
//       std::list <S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
//       i != fMeasureElementsList.end ();
//       ++i
//     ) {
//       S_msrMeasureElement measureElement = (*i);
//
  //       measureElement->
  //         setMeasureElementVoicePosition ( // JMI v0.9.66
  //           voicePosition,
  //           "finalizeMeasure()");
    } // for

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceVoicesFlatView ()) {
      // print measure notes flat list // JMI v0.9.70
      size_t
        measureNotesFlatListSize =
          fMeasureNotesFlatList.size ();

      gLog <<
        "Finalizing measure 222" <<
        ", fetchMeasureUpLinkToVoice: " <<
        fetchMeasureUpLinkToVoice ()->getVoiceName () <<
        ", fMeasureNumber: " <<
        fMeasureNumber <<
        ", fMeasureNotesFlatList: " <<
        mfSingularOrPlural (
          measureNotesFlatListSize, "note", "notes") <<
        ':' <<
        std::endl;

      if (measureNotesFlatListSize) {
        ++gIndenter;

        std::list <S_msrNote>::const_iterator
          iBegin = fMeasureNotesFlatList.begin (),
          iEnd   = fMeasureNotesFlatList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_msrNote note = (*i);

          gLog <<
            note->noteCoreAsString ();

          if (++i == iEnd) break;
          gLog << ' ';
        } // for
        gLog << std::endl << std::endl;

        --gIndenter;
      }
    }
#endif // MF_TRACE_IS_ENABLED

    // register finalization
    fMeasureHasBeenFinalized = true;
    fMeasureFinalizationContext = context;
  }
}

void msrMeasure::finalizeMeasureClone (
  int                 inputLineNumber,
  const S_msrMeasure& originalMeasure,
  const S_msrVoice&   voiceClone,
  const std::string&  context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Finalizing measure clone " <<
      this->asString () <<
      " in segment '" <<
      fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
      "' in voice \"" <<
// JMI      fMeasureUpLinkToSegment->getSegmentUpLinkToVoice ()->getVoiceName () <<
      voiceClone->getVoiceName () <<
        ", lines " << inputLineNumber << " .. " << fMeasureEndInputLineNumber << // JMI
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    if (fMeasureCurrentAccumulatedWholeNotesDuration.getNumerator () == 0) {
      std::stringstream ss;

      ss <<
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
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str ());
    }
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureClone() 1" + context);
  }
#endif // MF_TRACE_IS_ENABLED

  if (fMeasureHasBeenFinalized) {
#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
    if (
      gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
    ) {
#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceParts ()
      ) {
        std::stringstream ss;

        ss <<
          "### MAINTAINANCE ### -- " <<
          "Attempt at finalizing measure clone " <<
          asShortString () <<
          "\" more than once";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
  }

  ++gIndenter;

  // fetch the voiceClone's current time
  S_msrTimeSignature
    voiceCurrentTimeSignature =
      voiceClone->
        getVoiceCurrentTimeSignature ();

  if (! voiceCurrentTimeSignature) { // JMI v0.9.70
    // take the implicit 4/4 measure whole notes into account
    setFullMeasureWholeNotesDuration (
      msrWholeNotes (1, 1));
  }
  else {
    // set the full length from time
    setFullMeasureWholeNotesDurationFromTimeSignature (
      voiceCurrentTimeSignature);
  }

/* JMI
  // is this note the shortest one in this measure?
  msrWholeNotes
    voiceShortestNoteWholeNotes =
      voiceClone->
        getVoiceShortestNoteWholeNotes ();

  if (fMeasureShortestNoteWholeNotes < voiceShortestNoteWholeNotes) {
    voiceClone->
      registerShortestNoteInVoiceIfRelevant (
        note);

    // measure shortest note tuplet factor // JMI
    fMeasureShortestNoteTupletFactor = mfRational (1, 1);
  }
*/

  // register this measures's length in the part clone
  S_msrPart
    part =
      voiceClone->
        getVoiceUpLinkToStaff ()->
          getStaffUpLinkToPart ();

  part->
    registerOrdinalMeasureNumberWholeNotes (
      inputLineNumber,
      fMeasureOrdinalNumberInVoice,
      fMeasureCurrentAccumulatedWholeNotesDuration);

  // consistency check
  msrMeasureKind
    originalMeasureMeasureKind =
      originalMeasure->getMeasureKind ();

  if (fMeasureKind != originalMeasureMeasureKind) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresDetails ()) {
      this->
        displayMeasure (
          inputLineNumber,
          "finalizeMeasureClone() 2 - cloneMeasure");

      originalMeasure->
        displayMeasure (
          inputLineNumber,
          "finalizeMeasureClone() 3 - originalMeasure");

      std::stringstream ss;

      ss <<
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
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
   //      __FILE__, __LINE__,
        ss.str ());

      if (getFullMeasureWholeNotesDuration ().getNumerator () == 0) { // JMI
        std::stringstream ss;

        ss <<
          "*********>> measure clone '" <<
          fMeasureNumber <<
          "' full measure whole notes is 0" <<
          ", line " << inputLineNumber;

     //   msrInternalError ( // JMI
        msrInternalWarning (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
     //      __FILE__, __LINE__,
          ss.str ());
      }
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // delegate finalization to voice kind specific methods
  switch (voiceClone->getVoiceKind ()) { // JMI v0.9.66 NEEDED ???
    case msrVoiceKind::kVoiceKindRegular:
      finalizeMeasureInRegularVoice (
        inputLineNumber,
        fMeasureRepeatContextKind,
        context);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      finalizeMeasureInHarmonyVoice (
        inputLineNumber,
        fMeasureRepeatContextKind,
        context);
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      finalizeMeasureInFiguredBassVoice (
        inputLineNumber,
        fMeasureRepeatContextKind,
        context);
      break;
  } // switch

  // voice position
  msrWholeNotes
    voicePosition =
      fetchMeasureUpLinkToVoice ()->
        getCurrentVoicePosition ();

  // delegate position measure assignment to the elements in the measure
    for (S_msrMeasureElement measureElement : fMeasureElementsList) {
//   for (
//     std::list <S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
//     i != fMeasureElementsList.end ();
//     ++i
//   ) {
//     S_msrMeasureElement measureElement = (*i);

//     measureElement->
//       setMeasureElementVoicePosition ( // JMI v0.9.66
//         voicePosition,
//         "finalizeMeasure()");
  } // for

  // register finalization
  fMeasureHasBeenFinalized = true;
  fMeasureFinalizationContext = "finalizeMeasureClone() " + context;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayMeasure (
      inputLineNumber,
      "finalizeMeasureClone() 4");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrMeasure::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasure::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasure::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrMeasure::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasure::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasure::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrMeasure::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasure::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list <S_msrMeasureElement>::const_iterator i = fMeasureElementsList.begin ();
    i != fMeasureElementsList.end ();
    ++i
  ) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrMeasure::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
//     case msrMeasureRepeatContextKind::kMeasureRepeatContext_UNKNOWN_:
//       result = "kMeasureRepeatContext_UNKNOWN_";
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

  std::stringstream ss;

  ss <<
    "[Measure " <<
    fMeasureNumber <<
// JMI    ", fMeasureKind: " <<
    ", " <<
    fMeasureKind <<
// JMI    ", voice: " <<
    ", " <<
    voice->getVoiceName () <<
    ", fMeasureOrdinalNumberInVoice: " <<
    fMeasureOrdinalNumberInVoice <<
    ", fMeasurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", fMeasureDebugNumber: '" <<
    fMeasureDebugNumber <<
/* JMI
    ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
    fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
    ", getFullMeasureWholeNotesDuration (): " <<
    getFullMeasureWholeNotesDuration ().asFractionString () <<
    */
    "', " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

std::string msrMeasure::asStringForMeasuresSlices () const
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  std::stringstream ss;

  ss <<
    fMeasureNumber <<
    ": " << voice->getVoiceName () <<
    " [";
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
    "', fMeasureCurrentAccumulatedWholeNotesDuration: " <<
    fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
    ", getFullMeasureWholeNotesDuration (): " <<
    getFullMeasureWholeNotesDuration ().asFractionString () <<
    ", ";
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputStartLineNumber <<
    ", ";
*/

  size_t
    measureElementsListSize =
      fMeasureElementsList.size ();

/* JMI
  ss <<
    "fMeasureElementsList: " <<
    mfSingularOrPlural (
      measureElementsListSize, "element", "elements") <<
    std::endl;
*/

  if (measureElementsListSize) {
// JMI    ss << std::endl;

    ++gIndenter;

    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement measureElement = (*i);

      ss <<
        measureElement->asStringForMeasuresSlices ();
      if (++i == iEnd) break;
      ss << ", ";
    } // for

    --gIndenter;
  }

  ss << ']';

  return ss.str ();
}

std::string msrMeasure::asString () const
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  std::stringstream ss;

  ss <<
    "[Measure '" <<
    fMeasureNumber <<
    "', fMeasureKind: " <<
    fMeasureKind <<
    ", voice: " <<
    voice->getVoiceName () <<

    ", fMeasureContainsSound: " <<
    fMeasureContainsSound <<

   ", fMeasureOrdinalNumberInVoice: " <<
    fMeasureOrdinalNumberInVoice <<
    ", fMeasurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", fNextMeasureNumber: " <<
    fNextMeasureNumber <<
    ", fMeasureDebugNumber: " <<
    fMeasureDebugNumber <<

    "', fMeasureCurrentAccumulatedWholeNotesDuration: " <<
    fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<

    ", getFullMeasureWholeNotesDuration (): " <<
    getFullMeasureWholeNotesDuration ().asFractionString () <<
    ", " <<

    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrMeasure::displayMeasure (
  int                inputLineNumber,
  const std::string& context)
{
  gLog <<
    std::endl <<
    "*********>> Measure " <<
    fMeasureNumber <<
    /* JMI
    "', fMeasurePuristNumber: " <<
    fMeasurePuristNumber <<
    ", fMeasureDebugNumber: " <<
    fMeasureDebugNumber <<
    */
    "' (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    std::endl;

  ++gIndenter;
  print (gLog);
  --gIndenter;

  gLog <<
    " <<*********" <<
    std::endl << std::endl;
}

void msrMeasure::printFull (std::ostream& os) const
{
  os <<
    "[Measure FULL'" <<
    fMeasureNumber <<
    "', " << fMeasureKind <<
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
    fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 46;

  os << std::left <<
    std::setw (fieldWidth) <<
    "getFullMeasureWholeNotesDuration ()" << ": " <<
    getFullMeasureWholeNotesDuration ().asString () <<
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
    "fNextMeasureNumber" << ": " <<
    fNextMeasureNumber <<
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
    fMeasureEndRegularKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureRepeatContextKind" << ": " <<
    fMeasureRepeatContextKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureIsFirstInVoice" << ": " <<
    fMeasureIsFirstInVoice <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureFirstInSegmentKind" << ": " <<
    fMeasureFirstInSegmentKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureUpLinkToSegment" << ": " <<
    fMeasureUpLinkToSegment->asShortString () <<
    std::endl;


#ifdef MF_TRACE_IS_ENABLED
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
    os << "[NULL]" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "voiceCurrentKey" << ": ";
  if (voiceCurrentKey) {
    os <<
      voiceCurrentKey;
  }
  else {
    os << "[NULL]" << std::endl;
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
    os << "[NULL]" << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

    /* JMI

    std::setw (fieldWidth) <<
    "measureCurrentAccumulatedWholeNotesDurationpitchAndOctaveAsString" << ": " <<
    measureCurrentAccumulatedWholeNotesDurationpitchAndOctaveAsString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fullMeasureWholeNotesDurationpitchAndOctaveAsString" << ": " <<
    fullMeasureWholeNotesDurationpitchAndOctaveAsString () <<
    std::endl <<
      */

/* JMI
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureShortestNoteWholeNotes" << ": " <<
    fMeasureShortestNoteWholeNotes.asFractionString () <<
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
      "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureContainsSound" << ": " <<
    fMeasureContainsSound <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureKindHasBeenDetermined" << ": " <<
    fMeasureKindHasBeenDetermined <<
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
    "fMeasureIsAMeasureRest" << ": " <<
    fMeasureIsAMeasureRest <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fNextMeasureNumber" << ": \"" <<
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

    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrMeasureElement measureElement = (*i);

      measureElement->printFull (os);

      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  os << std::endl;

  // print measure notes flat list
  size_t
    measureNotesFlatListSize =
      fMeasureNotesFlatList.size ();

  os <<
    "fMeasureNotesFlatList: " <<
    mfSingularOrPlural (
      measureNotesFlatListSize, "note", "notes") <<
    ", fetchMeasureUpLinkToVoice: " << fetchMeasureUpLinkToVoice ()->getVoiceName () <<
    ", fMeasureNumber:" << fMeasureNumber <<
    std::endl;

  if (measureNotesFlatListSize) {
    ++gIndenter;

    std::list <S_msrNote>::const_iterator
      iBegin = fMeasureNotesFlatList.begin (),
      iEnd   = fMeasureNotesFlatList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote note = (*i);

      os <<
        note->noteCoreAsString ();

      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << std::endl;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrMeasure::print (std::ostream& os) const
{
  os <<
    "[Measure " <<
    fMeasureNumber <<
    "', " << fMeasureKind <<
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
    ", fMeasureCurrentAccumulatedWholeNotesDuration: " <<
    fMeasureCurrentAccumulatedWholeNotesDuration.asFractionString () <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 37;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureContainsSound" << ": " <<
    fMeasureContainsSound <<
    std::endl <<

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
    "fNextMeasureNumber" << ": " <<
    fNextMeasureNumber <<
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
    fMeasureEndRegularKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fullMeasureWholeNotesDuration" << ": " <<
    getFullMeasureWholeNotesDuration ().asString () <<
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

    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
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
    os << "[NULL]" << std::endl;
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
//         fMeasureCurrentAccumulatedWholeNotesDuration,
//         "handleASubsequentHarmonyInAHarmoniesMeasure() 2");
//
//     // insert skipNote before currentHarmony in the measure's elements list
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Inserting subsequent padding note " <<
//         skipNote->asString () <<
//         " before currentHarmony " <<
//         currentHarmony->asString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
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
//     std::stringstream ss;
//
//     ss <<
//       "Previous harmony " <<
//       previousHarmony->asString () <<
//       " overlaps current harmony " <<
//       currentHarmony->asString ();
//
//     msrInternalWarning (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       ss.str (),
//
//     // compute previousHarmony's future sounding whole notes
//     msrWholeNotes
//       reducedSoundingWholeNotes =
//         previousHarmonySoundingWholeNotes
//           + // the delta is negative
//         measurePositionsDelta;
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Reducing the sounding whole notes of harmony " <<
//         previousHarmony->asString () <<
//         " from " <<
//         previousHarmonySoundingWholeNotes.asFractionString () <<
//         " to " <<
//         reducedSoundingWholeNotes.asFractionString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     if (reducedSoundingWholeNotes.getNumerator () == 0) {
//       std::stringstream ss;
//
//       ss <<
//         "Cannot reduce the duration of harmony " <<
//         previousHarmony->asShortString () <<
//         " to 0 : leaving it as it is";
//
//       msrInternalWarning (
//         gServiceRunData->getInputSourceName (),
//         inputLineNumber,
// //  JMI             __FILE__, __LINE__,
//         ss.str (),
//     }
//     else {
//       // set previousHarmony's duration to the reduced value
//       previousHarmony->
//         setMeasureElementSoundingWholeNotes (
//           reducedSoundingWholeNotes);
//     }
//   }





*/
