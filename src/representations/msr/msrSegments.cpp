/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrSegments.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
int msrSegment::gSegmentsCounter = 0;
int msrSegment::gSegmentDebugNumber = 0;

S_msrSegment msrSegment::create (
  int        inputLineNumber,
  S_msrVoice segmentVoicekUpLink)
{
  msrSegment* o =
    new msrSegment (
      inputLineNumber,
      segmentVoicekUpLink);
  assert (o != nullptr);

  return o;
}

msrSegment::msrSegment (
  int        inputLineNumber,
  S_msrVoice segmentVoicekUpLink)
    : msrVoiceElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segmentVoicekUpLink != nullptr,
    "segmentVoicekUpLink is null");

  // set segment's voice upLink
  fSegmentVoiceUpLink =
    segmentVoicekUpLink;

  // do other initializations
  initializeSegment ();
}

msrSegment::~msrSegment ()
{}

void msrSegment::initializeSegment ()
{
  fSegmentAbsoluteNumber = ++gSegmentsCounter;
  fSegmentDebugNumber    = ++gSegmentDebugNumber;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Initializing new segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "', in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\", line " << fInputLineNumber <<
      endl;
  }
#endif

//  // segment shortest note duration
//  fSegmentShortestNoteDuration = rational (INT_MAX, 1);

// segment shortest note tuplet factor
//  fSegmentShortestNoteTupletFactor = rational (1, 1);
}

S_msrStaff msrSegment::fetchSegmentStaffUpLink () const
{
  S_msrStaff result;

  if (fSegmentVoiceUpLink) {
    result =
      fSegmentVoiceUpLink->
        getVoiceStaffUpLink ();
  }

  return result;
}

S_msrPart msrSegment::fetchSegmentPartUpLink () const
{
  S_msrPart result;

  if (fSegmentVoiceUpLink) {
    result =
      fSegmentVoiceUpLink->
        fetchVoicePartUpLink ();
  }

  return result;
}

S_msrPartGroup msrSegment::fetchSegmentPartGroupUpLink () const
{
  S_msrPartGroup result;

  if (fSegmentVoiceUpLink) {
    result =
      fSegmentVoiceUpLink->
        fetchVoicePartGroupUpLink ();
  }

  return result;
}

S_msrScore msrSegment::fetchSegmentScoreUpLink () const
{
  S_msrScore result;

  if (fSegmentVoiceUpLink) {
    result =
      fSegmentVoiceUpLink->
        fetchVoiceScoreUpLink ();
  }

  return result;
}

S_msrSegment msrSegment::createSegmentNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a newborn clone of segment " <<
      asString () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrSegment
    newbornClone =
      msrSegment::create (
        fInputLineNumber,
        containingVoice);

  // absolute number, for coherency between passes
  newbornClone->fSegmentAbsoluteNumber =
    fSegmentAbsoluteNumber;

  // keep debug number fSegmentDebugNumber unchanged

  return newbornClone;
}

S_msrSegment msrSegment::createSegmentDeepClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a deep clone of segment " <<
      asString () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrSegment
    segmentDeepClone =
      msrSegment::create (
        fInputLineNumber,
        containingVoice);

  // absolute number
  segmentDeepClone->fSegmentAbsoluteNumber =
    fSegmentAbsoluteNumber;

  // keep debug number fSegmentDebugNumber unchanged

  // the measures in the segment contain the mmusic
  int numberOfSegmentMeasures =
   fSegmentMeasuresList.size () ;

  if (numberOfSegmentMeasures) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "There are " <<
        numberOfSegmentMeasures <<
        " measures in segment to be deep copied" <<
        endl;
    }
#endif

    for (
      list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin ();
      i != fSegmentMeasuresList.end ();
      ++i
  ) {
      // append a deep clone of the measure to the deep clone
      segmentDeepClone->
        appendMeasureToSegment (
          (*i)->
            createMeasureDeepClone (this));
    } // for
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegments ()) {
      gLogStream <<
        "There are no measures in segment to be deep copied" <<
        endl;
    }
#endif
  }

  // upLinks
  segmentDeepClone->fSegmentVoiceUpLink =
    containingVoice;

  return segmentDeepClone;
}

/* JMI
void msrSegment::setSegmentShortestNoteDuration (
  const rational& duration)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalMsrOahGroup->getTraceMsrDurations ()
  ) {
    gLogStream <<
      "Setting the shortest note duration of segment " <<
      fSegmentAbsoluteNumber <<
      " to " <<
      duration <<
      endl;
  }
#endif

  fSegmentShortestNoteDuration = duration;
}

void msrSegment::setSegmentShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalMsrOahGroup->getTraceMsrDurations ()
  ) {
    gLogStream <<
      "Setting the shortest note tuplet factor of segment " <<
      fSegmentAbsoluteNumber <<
      " to " <<
      noteTupletFactor <<
      endl;
  }
#endif

  fSegmentShortestNoteTupletFactor = noteTupletFactor;
}
*/

void msrSegment::assertSegmentMeasuresListIsNotEmpty (
  int inputLineNumber) const
{
  if (! fSegmentMeasuresList.size ()) {
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasuresDetails ()
      ||
    gGlobalTracingOahGroup->getTraceSegmentsDetails ()
      ||
    gGlobalTracingOahGroup->getTraceRepeatsDetails ()
  ) {
    fSegmentVoiceUpLink->
      displayVoiceRepeatsStackFullMeasureRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "assertSegmentMeasuresListIsNotEmpty()");
  }
#endif

    stringstream s;

    s <<
      "assertSegmentMeasuresListIsNotEmpty()" <<
      ", fSegmentMeasuresList is empty" <<
      ", segment: " <<
      this->asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;

    gLogStream <<
      s.str () <<
      endl;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

S_msrMeasure msrSegment::createAMeasureAndAppendItToSegment (
  int           inputLineNumber,
  const string& measureNumber,
  msrMeasureImplicitKind
                measureImplicitKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating and appending a measure '" << measureNumber <<
      "', to segment '" << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // determine new measure 'first in segment' kind
  msrMeasure::msrMeasureFirstInSegmentKind
    measureFirstInSegmentKind;

  if (fSegmentMeasuresList.size () == 0) {
    // this is the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasure::kMeasureFirstInSegmentKindYes;
  }
  else {
    // this is not the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasure::kMeasureFirstInSegmentKindNo;
  }

  // create a measure
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" << measureNumber <<
      "' and appending it to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      ", measureFirstInSegmentKind: " <<
      msrMeasure::measureFirstInSegmentKindAsString (
        measureFirstInSegmentKind) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrMeasure
    result =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        this);

  // set result's ordinal number
  result->
    setMeasureOrdinalNumberInVoice (
      fSegmentVoiceUpLink->
        incrementVoiceCurrentMeasureOrdinalNumber ());

  // append result to the segment
  appendMeasureToSegment (result);

  --gIndenter;

  return result;
}

void msrSegment::setNextMeasureNumberInSegment (
  int           inputLineNumber,
  const string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" << nextMeasureNumber <<
      "' in segment '" << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  if (fSegmentMeasuresList.size ()) { // JMI ???
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Setting next measure number to '" << nextMeasureNumber <<
        "' in segment '" << asString () <<
        "'s last measure " <<
        "' in voice \"" <<
        fSegmentVoiceUpLink->getVoiceName () <<
        "\"" <<
        "', line " << inputLineNumber <<
        endl;
    }
#endif

    fSegmentMeasuresList.back ()->
      setNextMeasureNumber (
        nextMeasureNumber);
  }

  --gIndenter;
}

void msrSegment::appendPrintLayoutToSegment (
  S_msrPrintLayout printLayout)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePrintLayouts ()) {
    gLogStream <<
      "Appending print layout '" << printLayout->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  if (fSegmentMeasuresList.size () == 0) {
    stringstream s;

    s <<
      "fSegmentMeasuresList is empty"  << // JMI
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

    gLogStream <<
      "fSegmentVoiceUpLink:" <<
      endl;
    ++gIndenter;
    gLogStream <<
      fSegmentVoiceUpLink <<
      endl;
    --gIndenter;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      printLayout->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register print layout in segments's current measure
  fSegmentMeasuresList.back ()->
    appendPrintLayoutToMeasure (printLayout);

  --gIndenter;
}

void msrSegment::appendClefToSegment (S_msrClef clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  if (fSegmentMeasuresList.size () == 0) {
    stringstream s;

    s <<
      "fSegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

    gLogStream <<
      "fSegmentVoiceUpLink:" <<
      endl;
    ++gIndenter;
    gLogStream <<
      fSegmentVoiceUpLink <<
      endl;
    --gIndenter;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
       clef->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register clef in segments's current measure
  fSegmentMeasuresList.back ()->
    appendClefToMeasure (clef);

  --gIndenter;
}

void msrSegment::prependClefToSegment (S_msrClef clef) // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Prepending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  if (fSegmentMeasuresList.size () == 0) {
    stringstream s;

    s <<
      "fSegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", fSegmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

    gLogStream <<
      "fSegmentVoiceUpLink:" <<
      endl;
    ++gIndenter;
    gLogStream <<
      fSegmentVoiceUpLink <<
      endl;
    --gIndenter;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      clef->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register clef in segments's current measure
  fSegmentMeasuresList.front ()->
    appendClefToMeasure (clef);

  --gIndenter;
}

void msrSegment::appendKeyToSegment (S_msrKey key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " << key->asString () <<
      " to segment " << asString () <<
    ", in voice \"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    key->getInputLineNumber ());

  ++gIndenter;

  // register key in segments's current measure
  fSegmentMeasuresList.back ()->
    appendKeyToMeasure (key);

  --gIndenter;
}

void msrSegment::appendTimeSignatureToSegment (S_msrTimeSignature timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature:" <<
      endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    gLogStream <<
      "to segment " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    timeSignature->getInputLineNumber ());

  ++gIndenter;

  // append timeSignature to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeSignatureToMeasure (timeSignature);

  --gIndenter;
}

void msrSegment::appendTimeSignatureToSegmentClone (S_msrTimeSignature timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature:" <<
      endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    gLogStream <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    timeSignature->getInputLineNumber ());

  // append timeSignature to segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeSignatureToMeasureClone (timeSignature);

  --gIndenter;
}

void msrSegment::insertHiddenMeasureAndBarLineInSegmentClone (
  int             inputLineNumber,
  const rational& positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos () || gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      positionInMeasure <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    inputLineNumber);

  // append time to segments's current measure
  fSegmentMeasuresList.back ()->
    insertHiddenMeasureAndBarLineInMeasureClone (
      inputLineNumber,
      positionInMeasure);

  --gIndenter;
}

void msrSegment::appendHarmonyToSegment (S_msrHarmony harmony)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    harmony->getInputLineNumber ();

  ++gIndenter;

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    inputLineNumber);

  // append harmony to this segment
  fSegmentMeasuresList.back ()->
    appendHarmonyToMeasure (harmony);

  --gIndenter;
}

void msrSegment::appendHarmonyToSegmentClone (S_msrHarmony harmony)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    harmony->getInputLineNumber ());

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarmonyToMeasureClone (harmony);
}

void msrSegment::appendFiguredBassElementToSegment (
  S_msrFiguredBassElement figuredBassElement)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass " <<
      figuredBassElement->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  int inputLineNumber =
    figuredBassElement->getInputLineNumber ();

  ++gIndenter;

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    inputLineNumber);

  // append figuredBassElement to this segment
  fSegmentMeasuresList.back ()->
    appendFiguredBassElementToMeasure (figuredBassElement);

  --gIndenter;
}

void msrSegment::appendFiguredBassElementToSegmentClone (
  S_msrFiguredBassElement figuredBassElement)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass " <<
      figuredBassElement->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    figuredBassElement->getInputLineNumber ());

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendFiguredBassElementToMeasure (figuredBassElement);
}

void msrSegment::appendSegnoToSegment (S_msrSegno segno)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegnos ()) {
    gLogStream <<
      "Appending segno " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    segno->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendSegnoToMeasure (segno);

  --gIndenter;
}

void msrSegment::appendCodaToSegment (S_msrCoda coda)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceCodas ()) {
    gLogStream <<
      "Appending coda " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    coda->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendCodaToMeasure (coda);

  --gIndenter;
}

void msrSegment::appendEyeGlassesToSegment (
  S_msrEyeGlasses eyeGlasses)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceEyeGlasses ()) {
    gLogStream <<
      "Appending eyeGlasses " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    eyeGlasses->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendEyeGlassesToMeasure (eyeGlasses);

  --gIndenter;
}

void msrSegment::appendPedalToSegment (S_msrPedal pedal)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePedals ()) {
    gLogStream <<
      "Appending pedal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }

#endif
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    pedal->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPedalToMeasure (pedal);

  --gIndenter;
}

void msrSegment::appendDampToSegment (S_msrDamp damp)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDamps ()) {
    gLogStream <<
      "Appending damp " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    damp->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampToMeasure (damp);

  --gIndenter;
}

void msrSegment::appendDampAllToSegment (S_msrDampAll dampAll)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDampAlls ()) {
    gLogStream <<
      "Appending damp all " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    dampAll->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendDampAllToMeasure (dampAll);

  --gIndenter;
}

void msrSegment::appendTransposeToSegment (
  S_msrTranspose transpose)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Appending transpose " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    transpose->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTransposeToMeasure (transpose);

  --gIndenter;
}

void msrSegment::appendStaffDetailsToSegment (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    staffDetails->getInputLineNumber ();

  if (
    gGlobalTracingOahGroup->getTraceStaffDetails ()
      ||
    gGlobalTracingOahGroup->getTraceSegments ()
  ) {
    gLogStream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\" line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceStaffDetails ()
      ||
    gGlobalTracingOahGroup->getTraceSegments ()
  ) {
    fSegmentVoiceUpLink->
      displayVoice (
        inputLineNumber,
        "appendStaffDetailsToSegment() 1");
  }
#endif

  ++gIndenter;

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    staffDetails->getInputLineNumber ());

  // append staffDetails to this segment
  fSegmentMeasuresList.back ()->
    appendStaffDetailsToMeasure (staffDetails);

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceStaffDetails ()
      ||
    gGlobalTracingOahGroup->getTraceSegments ()
  ) {
    fSegmentVoiceUpLink->
      displayVoice (
        inputLineNumber,
        "appendStaffDetailsToSegment() 2");
  }
#endif

  --gIndenter;
}

void msrSegment::appendLineBreakToSegment (S_msrLineBreak lineBreak)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
    gLogStream <<
      "Appending break '" <<
      lineBreak->asShortString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    lineBreak->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendLineBreakToMeasure (lineBreak);

  --gIndenter;
}

void msrSegment::appendPageBreakToSegment (S_msrPageBreak pageBreak)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
    gLogStream <<
      "Appending break " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    pageBreak->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPageBreakToMeasure (pageBreak);

  --gIndenter;
}

void msrSegment::appendBarNumberCheckToSegment (
  S_msrBarNumberCheck barNumberCheck)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarNumberChecks ()) {
    gLogStream <<
      "Appending bar number check " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    barNumberCheck->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendBarNumberCheckToMeasure (barNumberCheck);

  --gIndenter;
}

void msrSegment::appendTempoToSegment (
  S_msrTempo tempo)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Appending tempo " <<
      tempo->asShortString () <<
      " to segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    tempo->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTempoToMeasure (tempo);

  --gIndenter;
}

void msrSegment::appendRehearsalToSegment (
  S_msrRehearsal rehearsal)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRehearsals ()) {
    gLogStream <<
      "Appending rehearsal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    rehearsal->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendRehearsalToMeasure (rehearsal);

  --gIndenter;
}

void msrSegment::appendOctaveShiftToSegment (
  S_msrOctaveShift octaveShift)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceOctaveShifts ()) {
    gLogStream <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    octaveShift->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendOctaveShiftToMeasure (octaveShift);

  --gIndenter;
}

void msrSegment::appendScordaturaToSegment (
  S_msrScordatura scordatura)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceScordaturas ()) {
    gLogStream <<
      "Appending scordatura '" <<
  // JMI ???    scordatura->octaveShiftKindAsString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    scordatura->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendScordaturaToMeasure (scordatura);

  --gIndenter;
}

void msrSegment::appendAccordionRegistrationToSegment (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    accordionRegistration->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendAccordionRegistrationToMeasure (
      accordionRegistration);

  --gIndenter;
}

void msrSegment::appendHarpPedalsTuningToSegment (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending staff pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    harpPedalsTuning->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarpPedalsTuningToMeasure (
      harpPedalsTuning);

  --gIndenter;
}

void msrSegment::padUpToPositionInMeasureInSegment (
  int             inputLineNumber,
  const rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Padding up to current whole notes '" << wholeNotes <<
      "' in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  if (false && fSegmentMeasuresList.size () == 0) { // JMI
    stringstream s;

    s <<
      "fSegmentMeasuresList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

    gLogStream <<
      "fSegmentVoiceUpLink:" <<
      endl;
    ++gIndenter;
    gLogStream <<
      fSegmentVoiceUpLink <<
      endl;
    --gIndenter;

    gLogStream <<
      "Part:" <<
      endl;
    ++gIndenter;
    gLogStream <<
      fSegmentVoiceUpLink->fetchVoicePartUpLink () <<
      endl;
    --gIndenter;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentMeasuresList.back ()->
      padUpToPositionInMeasureInMeasure (
        inputLineNumber,
        wholeNotes);
  }
}

void msrSegment::backupByWholeNotesStepLengthInSegment (
  int             inputLineNumber,
  const rational& backupTargetMeasureElementPositionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceSegments ()
      ||
    gGlobalTracingOahGroup->getTraceMeasures ()
  ) {
    gLogStream <<
      "Backup by a '" <<
      backupTargetMeasureElementPositionInMeasure <<
      "' whole notes step length in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentMeasuresList.back ()->
      backupByWholeNotesStepLengthInMeasure (
        inputLineNumber,
        backupTargetMeasureElementPositionInMeasure);
  }
}

void msrSegment::appendPaddingNoteToSegment (
  int             inputLineNumber,
  const rational& forwardStepLength)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
    // append a padding note to the segment's last measure
    fSegmentMeasuresList.back ()->
      appendPaddingSkipNoteToMeasure (
        inputLineNumber,
        forwardStepLength);
  }

  --gIndenter;
}

void msrSegment::appendMeasureToSegment (S_msrMeasure measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();

  string measureNumber =
    measure->getMeasureElementMeasureNumber ();

  unsigned int segmentMeasuresListSize =
    fSegmentMeasuresList.size ();

  string currentMeasureNumber =
    segmentMeasuresListSize == 0
      ? ""
      : fSegmentMeasuresList.back ()->getMeasureElementMeasureNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending measure '" << measureNumber <<
      "' to segment " << asString ();

    if (fSegmentMeasuresList.size () == 0)
      gLogStream <<
        ", as first measure";
    else
      gLogStream <<
      ", after measure number '" << currentMeasureNumber << "'";

    gLogStream <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      ", line " << measure->getInputLineNumber () <<
      endl;
  }
#endif

  if (measureNumber == currentMeasureNumber) {
    stringstream s;

    s <<
      "appending measure number '" << measureNumber <<
      "' occurs twice in a row in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

//    msrInternalWarning ( // JMI
    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // is measure the first one this segment?
  if (segmentMeasuresListSize == 0) {
    measure->
      setMeasureFirstInSegmentKind (
        msrMeasure::kMeasureFirstInSegmentKindYes);
  }
  else {
    measure->
      setMeasureFirstInSegmentKind (
        msrMeasure::kMeasureFirstInSegmentKindNo);
  }

  // is measure the first one it the voice?
  // this is necessary for voice clones,
  // which don't go down the part-staff-voice-segment hierarchy
  if (! fSegmentVoiceUpLink->getVoiceFirstMeasure ()) {
    // yes, register it as such
    fSegmentVoiceUpLink->
      setVoiceFirstMeasure (measure);

    measure->
      setMeasureFirstInVoice ();
  }

  // append measure to the segment
  fSegmentMeasuresList.push_back (measure);
}

void msrSegment::prependMeasureToSegment (S_msrMeasure measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();

  string measureNumber =
    measure->getMeasureElementMeasureNumber ();

  unsigned int segmentMeasuresListSize =
    fSegmentMeasuresList.size ();

  string currentMeasureNumber =
    segmentMeasuresListSize == 0
      ? ""
      : fSegmentMeasuresList.back ()->getMeasureElementMeasureNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Prepending measure " << measureNumber <<
      " to segment " << asString ();

    if (segmentMeasuresListSize == 0) {
      gLogStream <<
        ", as first measure FOO"; // JMI
    }

/* JMI
    else
      gLogStream <<
      ", after measure number '" << currentMeasureNumber << "'";
*/

    gLogStream <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      ", line " << measure->getInputLineNumber () <<
      endl;
  }
#endif

  if (measureNumber == currentMeasureNumber) {
    stringstream s;

    s <<
      "prepending measure number '" << measureNumber <<
      "' occurs twice in a row in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

    msrInternalError (
// JMI    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  measure->
    setMeasureFirstInVoice ();

  // prepend measure to the segment
  fSegmentMeasuresList.push_front (measure);
}

void msrSegment::prependBarLineToSegment (S_msrBarLine barLine)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Prepending barLine '" << barLine->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    barLine->getInputLineNumber ());

  // prepend barLine to this segment
  ++gIndenter;

  fSegmentMeasuresList.front ()->
    prependBarLineToMeasure (barLine);

  --gIndenter;
}

void msrSegment::appendBarLineToSegment (
  S_msrBarLine barLine)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " << barLine->asString () <<
      " to segment " << asString () << "'" <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    barLine->getInputLineNumber ());

  ++gIndenter;

  // append barLine to this segment
  fSegmentMeasuresList.back ()->
    appendBarLineToMeasure (barLine);

  --gIndenter;
}

void msrSegment::appendBarCheckToSegment (S_msrBarCheck barCheck)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarChecks ()) {
    gLogStream <<
      "Appending bar check '" << barCheck->asString () <<
      "' to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    barCheck->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendBarCheckToMeasure (barCheck);
}

void msrSegment::appendVoiceStaffChangeToSegment (
  S_msrVoiceStaffChange voiceStaffChange)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "Appending voice staff change '" <<
      voiceStaffChange->asString () <<
      "' to segment \"" << asString () << "\"" <<
      ", line " << voiceStaffChange->getInputLineNumber () <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    voiceStaffChange->getInputLineNumber ());

  ++gIndenter;

  fSegmentMeasuresList.back ()->
    appendVoiceStaffChangeToMeasure (
      voiceStaffChange);

  --gIndenter;
}

void msrSegment::appendNoteToSegment (
  S_msrNote       note,
  const rational& partCurrentPositionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    if (! fSegmentMeasuresList.size ()) { // JMI
      displaySegment (
        note->getInputLineNumber (),
        "appendNoteToSegment()");
    }
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    note->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendNoteToMeasure (
      note,
      partCurrentPositionInMeasure);
}

void msrSegment::appendNoteToSegmentClone (S_msrNote note)
{
  fSegmentMeasuresList.back ()->
    appendNoteToMeasureClone (note);
}

void msrSegment::appendDoubleTremoloToSegment ( // JMI
  S_msrDoubleTremolo doubleTremolo)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Appending double tremolo " <<
      doubleTremolo->asShortString () <<
      " to segment '" << asString () << "'" <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    doubleTremolo->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendDoubleTremoloToMeasure (doubleTremolo);
}

void msrSegment::appendChordToSegment (S_msrChord chord) // JMI
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    chord->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendChordToMeasure (chord);
}

void msrSegment::appendTupletToSegment (S_msrTuplet tuplet) // JMI
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    tuplet->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendTupletToMeasure (tuplet);
}

/* JMI
void msrSegment::addGraceNotesGroupAheadOfSegmentIfNeeded (
  S_msrGraceNotesGroup graceNotesGroup)

{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    graceNotesGroup->getInputLineNumber ());

  fSegmentMeasuresList.front ()->
    addGraceNotesGroupAheadOfMeasure (graceNotesGroup);
}

void msrSegment::appendGraceNotesToSegment (
  S_msrGraceNotes graceNotes)
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    graceNotes->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendGraceNotesToMeasure (graceNotes);
}

void msrSegment::appendAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    afterGraceNotes->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendAfterGraceNotesToMeasure (afterGraceNotes);
}

void msrSegment::prependAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)

{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    afterGraceNotes->getInputLineNumber ());

  fSegmentMeasuresList.front ()->
    prependAfterGraceNotesToMeasure (afterGraceNotes); // JMI
}
*/

void msrSegment::prependOtherElementToSegment (
  S_msrMeasureElement elem)
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    elem->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    prependOtherElementToMeasure (elem);
}

void msrSegment::appendOtherElementToSegment (
  S_msrMeasureElement elem)
{
  // sanity check
  assertSegmentMeasuresListIsNotEmpty (
    elem->getInputLineNumber ());

  fSegmentMeasuresList.back ()->
    appendOtherElementToMeasure (elem);
}

/* JMI
S_msrElement msrSegment::removeLastElementFromSegment (
  int inputLineNumber)
{
  // this last element can be a note or a tuplet,
  // this method is used when the seconde note of a chord is mest

  if (fSegmentMeasuresList.size ()) {
    return
      fSegmentMeasuresList.back ()->
        removeLastElementFromMeasure (
          inputLineNumber);
  }

  else {
    msrInternalError (
      inputLineNumber,
      "cannot removeLastElementFromSegment () " <<
      asString () <<
      " since it is empty");
  }
}
*/

void msrSegment::removeNoteFromSegment (
  int       inputLineNumber,
  S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Removing note '" <<
      note->asString () <<
      "'from segment '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  ++gIndenter;

  if (fSegmentMeasuresList.size ()) {
    fSegmentMeasuresList.back ()->
      removeNoteFromMeasure (
        inputLineNumber,
        note);
  }

  else {
    stringstream s;

    s <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      " since it is empty";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  --gIndenter;
}

void msrSegment::removeElementFromSegment (
  int          inputLineNumber,
  S_msrElement element)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Removing element '" <<
      element->asString () <<
      "'from segment '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

 if (fSegmentMeasuresList.size ()) {
    fSegmentMeasuresList.back ()->
      removeElementFromMeasure (
        inputLineNumber,
        element);
  }

  else {
    stringstream s;

    s <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      " since it is empty";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

S_msrMeasure msrSegment::fetchLastMeasureFromSegment (
  int           inputLineNumber,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Fetching last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      " (" << context << ")" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  if (! fSegmentMeasuresList.size ()) {
    stringstream s;

    s <<
      "cannot fetch last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      "' since it is empty";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      endl <<
      "The fetched measure contains:" <<
      endl;

    ++gIndenter;

    gLogStream <<
      result <<
      endl;

    --gIndenter;
  }
#endif

  return result;
}

S_msrMeasure msrSegment::removeLastMeasureFromSegment (
  int           inputLineNumber,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Removing last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      " (" << context << ")" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    fSegmentVoiceUpLink->
      displayVoiceRepeatsStackFullMeasureRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "removeLastMeasureFromSegment() 1");
  }
#endif

  if (! fSegmentMeasuresList.size ()) {
    stringstream s;

    s <<
      "cannot remove last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      "' since it is empty";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      endl <<
      "The removed measure contains:" <<
      endl;

    ++gIndenter;

    gLogStream <<
      result <<
      endl;

    --gIndenter;
  }
#endif

  fSegmentMeasuresList.pop_back ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    fSegmentVoiceUpLink->
      displayVoiceRepeatsStackFullMeasureRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "removeLastMeasureFromSegment() 2");
  }
#endif

  return result;
}

void msrSegment::finalizeAllTheMeasuresOfSegment ( // superflous JMI ???
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing all the measures of segment '" <<
      fSegmentAbsoluteNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin ();
    i != fSegmentMeasuresList.end ();
    ++i
  ) {
    S_msrMeasure measure = (*i);

    measure->
      finalizeMeasure (
        inputLineNumber,
        msrMeasure::kMeasureRepeatContextKindNone,
        "finalizeAllTheMeasuresOfSegment()");
  } // for
}

void msrSegment::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegment::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegment::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSegment::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegment::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegment::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSegment::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegment::browseData ()" <<
      endl;
  }

/* JMI
  gLogStream <<
    gTab <<
    "==========>>> " <<
    fSegmentMeasuresList.size () << " measures" << endl;
    */

  for (
    list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin ();
    i != fSegmentMeasuresList.end ();
    ++i
  ) {
    // browse the element
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(*i));
  } // for

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrSegment::browseData ()" <<
      endl;
  }
}

string msrSegment::asShortString () const
{
  stringstream s;

  s <<
    "[" <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
      ", fSegmentDebugNumber: '" <<
      fSegmentDebugNumber <<
    "', voice: \"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"" <<
    "]";

  return s.str ();
}

string msrSegment::asString () const
{
  stringstream s;

  s <<
    "[" <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
      ", fSegmentDebugNumber: '" <<
      fSegmentDebugNumber <<
    "' in voice \"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"";

  if (! fSegmentMeasuresList.size ()) {
    s <<
      " (0 measures)";
  }
  else {
    s <<
      " (" <<
      mfSingularOrPlural (
        fSegmentMeasuresList.size (), "measure", " measures") <<
      "), i.e. [";

    list<S_msrMeasure>::const_iterator
      iBegin = fSegmentMeasuresList.begin (),
      iEnd   = fSegmentMeasuresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->getMeasureElementMeasureNumber ();
      if (++i == iEnd) break;
      s << ", ";
    } // for

    s << "]";
  }

  s << "]";

  return s.str ();
}

void msrSegment::displaySegment (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    "*********>> Segment '" <<
    fSegmentAbsoluteNumber <<
    ", segmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    /* JMI
    "', score:" <<
    endl <<
    fSegmentVoiceUpLink->
      fetchVoiceGroupScoreUpLink () <<
            */
    "', voice:" <<
    endl <<
    fSegmentVoiceUpLink->getVoiceName () <<
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

void msrSegment::print (ostream& os) const
{
  os <<
    "[Segment '" <<
    fSegmentAbsoluteNumber <<
    ", fSegmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "', " <<
    mfSingularOrPlural (
      fSegmentMeasuresList.size (), "measure", "measures") <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 20;

  os <<
    setw (fieldWidth) <<
    "fSegmentVoiceUpLink" << " : " <<
    "\"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"" <<
    endl;

/* JMI
  os << left <<
    setw (fieldWidth) <<
    "fSegmentShortestNoteDuration" << " : " <<
    fSegmentShortestNoteDuration <<
    endl <<
    setw (fieldWidth) <<
    "fSegmentShortestNoteTupletFactor" << " : " <<
    fSegmentShortestNoteTupletFactor <<
    endl;
*/

  if (! fSegmentMeasuresList.size ()) {
    os <<
      setw (fieldWidth) <<
      "fSegmentMeasuresList" << " : " << "empty" <<
      endl;
  }

  else {
    os << endl;

    list<S_msrMeasure>::const_iterator
      iBegin = fSegmentMeasuresList.begin (),
      iEnd   = fSegmentMeasuresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;

  os << "]" << endl;
}

void msrSegment::printShort (ostream& os) const
{
  os <<
    "[Segment '" <<
    fSegmentAbsoluteNumber <<
    ", fSegmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "', " <<
    mfSingularOrPlural (
      fSegmentMeasuresList.size (), "measure", "measures") <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 20;

  if (! fSegmentMeasuresList.size ()) {
    os <<
      setw (fieldWidth) <<
      "fSegmentMeasuresList" << " : " << "empty" <<
      endl;
  }

  else {
    os << endl;

    list<S_msrMeasure>::const_iterator
      iBegin = fSegmentMeasuresList.begin (),
      iEnd   = fSegmentMeasuresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i)->printShort (os);
      if (++i == iEnd) break;
//      os << endl;
    } // for
  }

  --gIndenter;

  os << "]" << endl;
}

ostream& operator<< (ostream& os, const S_msrSegment& elt)
{
  elt->print (os);
  return os;
}


}
