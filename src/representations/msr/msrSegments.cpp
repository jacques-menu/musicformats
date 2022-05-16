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

  // the measure elements in the segment contain the mmusic
  size_t segmentElementsListSize =
   fSegmentElementsList.size ();

  if (segmentElementsListSize != 0) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "There are " <<
        segmentElementsListSize <<
        " measure elements in segment to be deep copied" <<
        endl;
    }
#endif

    for (S_msrSegmentElement measureElement : fSegmentElementsList) {
      // append a deep clone of the measure element to the deep clone
      // DEEP CLONING IS NOT YET FINALIZED JMI v0.9.63
//       if (
//         S_msrMeasure measure = dynamic_cast<msrRepeat*>(&(*(measureElement)))
//       ) {

//       segmentDeepClone->
//         appendMeasureToSegment (
//           measureElement->
//             createMeasureDeepClone (this));
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

void msrSegment::assertSegmentElementsListIsNotEmpty (
  int inputLineNumber) const
{
  if (! fSegmentElementsList.size ()) {
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasuresDetails ()
      ||
    gGlobalTracingOahGroup->getTraceSegmentsDetails ()
      ||
    gGlobalTracingOahGroup->getTraceRepeatsDetails ()
  ) {
    fSegmentVoiceUpLink->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "assertSegmentElementsListIsNotEmpty()");
  }
#endif

    stringstream s;

    s <<
      "assertSegmentElementsListIsNotEmpty()" <<
      ", fSegmentElementsList is empty" <<
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
  int           previousMeasureEndInputLineNumber,
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

  if (fSegmentElementsList.size () == 0) {
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

  // set measure end line number
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting measure end line number for " << // JMI
      measureNumber <<
      " to " <<
      previousMeasureEndInputLineNumber <<
//       " in segment '" << // JMI
//       fMeasureSegmentUpLink->getSegmentAbsoluteNumber () <<
//       "' in voice \"" <<
//       fMeasureSegmentUpLink->
//         getSegmentVoiceUpLink ()->
//           getVoiceName () <<
      endl;
  }
#endif

  result->
    setMeasureEndInputLineNumber (
      previousMeasureEndInputLineNumber);

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

  if (fSegmentElementsList.size ()) { // JMI ???
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

    fSegmentMeasuresFlatList.back ()->
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
  if (fSegmentElementsList.size () == 0) {
    stringstream s;

    s <<
      "fSegmentElementsList is empty"  << // JMI
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
  fSegmentMeasuresFlatList.back ()->
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
  if (fSegmentElementsList.size () == 0) {
    stringstream s;

    s <<
      "fSegmentElementsList is empty"  <<
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
  fSegmentMeasuresFlatList.back ()->
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
  if (fSegmentElementsList.size () == 0) {
    stringstream s;

    s <<
      "fSegmentElementsList is empty"  <<
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
  fSegmentMeasuresFlatList.front ()->
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
  assertSegmentElementsListIsNotEmpty (
    key->getInputLineNumber ());

  ++gIndenter;

  // register key in segments's current measure
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    timeSignature->getInputLineNumber ());

  ++gIndenter;

  // append timeSignature to segments's current measure
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    timeSignature->getInputLineNumber ());

  // append timeSignature to segments's current measure
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    inputLineNumber);

  // append time to segments's current measure
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    inputLineNumber);

  // append harmony to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    harmony->getInputLineNumber ());

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    inputLineNumber);

  // append figuredBassElement to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    figuredBassElement->getInputLineNumber ());

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    segno->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    coda->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    eyeGlasses->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    pedal->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    damp->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    dampAll->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
    appendDampAllToMeasure (dampAll);

  --gIndenter;
}

void msrSegment::appendTranspositionToSegment (
  S_msrTransposition transpose)
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
  assertSegmentElementsListIsNotEmpty (
    transpose->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
    appendTranspositionToMeasure (transpose);

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
  assertSegmentElementsListIsNotEmpty (
    staffDetails->getInputLineNumber ());

  // append staffDetails to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    lineBreak->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    pageBreak->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    barNumberCheck->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    tempo->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
    appendTempoToMeasure (tempo);

  --gIndenter;
}

void msrSegment::appendRehearsalMarkToSegment (
  S_msrRehearsalMark rehearsalMark)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRehearsalMarks ()) {
    gLogStream <<
      "Appending rehearsalMark " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  assertSegmentElementsListIsNotEmpty (
    rehearsalMark->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
    appendRehearsalMarkToMeasure (rehearsalMark);

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
  assertSegmentElementsListIsNotEmpty (
    octaveShift->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    scordatura->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    accordionRegistration->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    harpPedalsTuning->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentMeasuresFlatList.back ()->
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

  if (false && fSegmentMeasuresFlatList.size () == 0) { // JMI
    stringstream s;

    s <<
      "fSegmentMeasuresFlatList is empty"  <<
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

  if (fSegmentMeasuresFlatList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentMeasuresFlatList.back ()->
      padUpToPositionInMeasureInMeasure (
        inputLineNumber,
        wholeNotes);
  }
}

void msrSegment::backupByWholeNotesStepLengthInSegment (
  int     inputLineNumber,
  const rational&
          backupTargetMeasureElementPositionInMeasure)
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

  if (fSegmentMeasuresFlatList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentMeasuresFlatList.back ()->
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

  if (fSegmentMeasuresFlatList.size ()) { // JMI BOFBOF
    // append a padding note to the segment's last measure
    fSegmentMeasuresFlatList.back ()->
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

  size_t segmentElementsListSize =
    fSegmentMeasuresFlatList.size ();

  string currentMeasureNumber =
    segmentElementsListSize == 0
      ? ""
      : fSegmentMeasuresFlatList.back ()->getMeasureElementMeasureNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending measure '" << measureNumber <<
      "' to segment " << asString ();

    if (fSegmentMeasuresFlatList.size () == 0)
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
      "' occurs twice in a row in segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"";

    msrInternalWarning ( // JMI KAKA v0.9.63
//     msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
//       __FILE__, __LINE__,
      s.str ());
  }

  // is measure the first one this segment?
  if (segmentElementsListSize == 0) {
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
  fSegmentElementsList.push_back (measure);

  fSegmentMeasuresFlatList.push_back (measure);

  // register measure as the last appended one in the segment
  fSegmentLastAppendedMeasure = measure;
}

void msrSegment::prependMeasureToSegment (S_msrMeasure measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();

  string measureNumber =
    measure->getMeasureElementMeasureNumber ();

  size_t segmentElementsListSize =
    fSegmentElementsList.size ();

  string currentMeasureNumber =
    segmentElementsListSize == 0
      ? ""
      : fSegmentMeasuresFlatList.back ()->getMeasureElementMeasureNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Prepending measure " << measureNumber <<
      " to segment " << asString ();

    if (segmentElementsListSize == 0) {
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
      "' occurs twice in a row in segment " <<
      asString () <<
      " in voice \"" <<
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
  fSegmentElementsList.push_front (measure);

  fSegmentMeasuresFlatList.push_front (measure);
}

S_msrMeasure msrSegment::fetchSegmentFirstMeasure () const
{
  S_msrMeasure result;

  if (fSegmentMeasuresFlatList.size ()) {
    result = fSegmentMeasuresFlatList.front ();
  }

  return result;
}

S_msrMeasure msrSegment::fetchSegmentLastMeasure () const
{
  S_msrMeasure result;

  if (fSegmentMeasuresFlatList.size ()) {
    result = fSegmentMeasuresFlatList.back ();
  }

  return result;
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
  assertSegmentElementsListIsNotEmpty (
    barLine->getInputLineNumber ());

  // prepend barLine to this segment
  ++gIndenter;

  fSegmentMeasuresFlatList.front ()->
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
  assertSegmentElementsListIsNotEmpty (
    barLine->getInputLineNumber ());

  ++gIndenter;

  // append barLine to this segment
  fSegmentMeasuresFlatList.back ()->
    appendBarLineToMeasure (barLine);

  --gIndenter;
}

void msrSegment::appendBarCheckToSegment (S_msrBarCheck barCheck)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarChecks ()) {
    gLogStream <<
      "Appending bar check " << barCheck->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentVoiceUpLink->getVoiceName () <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  assertSegmentElementsListIsNotEmpty (
    barCheck->getInputLineNumber ());

  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    voiceStaffChange->getInputLineNumber ());

  ++gIndenter;

  fSegmentMeasuresFlatList.back ()->
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
    if (! fSegmentElementsList.size ()) { // JMI
      displaySegment (
        note->getInputLineNumber (),
        "appendNoteToSegment()");
    }
  }
#endif

  // sanity check
  assertSegmentElementsListIsNotEmpty (
    note->getInputLineNumber ());

  fSegmentMeasuresFlatList.back ()->
    appendNoteToMeasure (
      note,
      partCurrentPositionInMeasure);
}

void msrSegment::appendNoteToSegmentClone (S_msrNote note)
{
  fSegmentMeasuresFlatList.back ()->
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
  assertSegmentElementsListIsNotEmpty (
    doubleTremolo->getInputLineNumber ());

  fSegmentMeasuresFlatList.back ()->
    appendDoubleTremoloToMeasure (doubleTremolo);
}

void msrSegment::appendChordToSegment (S_msrChord chord) // JMI
{
  // sanity check
  assertSegmentElementsListIsNotEmpty (
    chord->getInputLineNumber ());

  fSegmentMeasuresFlatList.back ()->
    appendChordToMeasure (chord);
}

void msrSegment::appendTupletToSegment (S_msrTuplet tuplet) // JMI
{
  // sanity check
  assertSegmentElementsListIsNotEmpty (
    tuplet->getInputLineNumber ());

  fSegmentMeasuresFlatList.back ()->
    appendTupletToMeasure (tuplet);
}

/* JMI
void msrSegment::addGraceNotesGroupAheadOfSegmentIfNeeded (
  S_msrGraceNotesGroup graceNotesGroup)

{
  // sanity check
  assertSegmentElementsListIsNotEmpty (
    graceNotesGroup->getInputLineNumber ());

  fSegmentMeasuresFlatList.front ()->
    addGraceNotesGroupAheadOfMeasure (graceNotesGroup);
}

void msrSegment::appendGraceNotesToSegment (
  S_msrGraceNotes graceNotes)
{
  // sanity check
  assertSegmentElementsListIsNotEmpty (
    graceNotes->getInputLineNumber ());

  fSegmentMeasuresFlatList.back ()->
    appendGraceNotesToMeasure (graceNotes);
}

void msrSegment::appendAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)
{
  // sanity check
  assertSegmentElementsListIsNotEmpty (
    afterGraceNotes->getInputLineNumber ());

  fSegmentMeasuresFlatList.back ()->
    appendAfterGraceNotesToMeasure (afterGraceNotes);
}

void msrSegment::prependAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)

{
  // sanity check
  assertSegmentElementsListIsNotEmpty (
    afterGraceNotes->getInputLineNumber ());

  fSegmentMeasuresFlatList.front ()->
    prependAfterGraceNotesToMeasure (afterGraceNotes); // JMI
}
*/

void msrSegment::prependOtherElementToSegment (
  S_msrMeasureElement elem)
{
  // sanity check
  assertSegmentElementsListIsNotEmpty (
    elem->getInputLineNumber ());

  fSegmentMeasuresFlatList.back ()->
    prependOtherElementToMeasure (elem);
}

void msrSegment::appendOtherElementToSegment (
  S_msrMeasureElement elem)
{
  // sanity check
  assertSegmentElementsListIsNotEmpty (
    elem->getInputLineNumber ());

  fSegmentMeasuresFlatList.back ()->
    appendOtherElementToMeasure (elem);
}

/* JMI
S_msrElement msrSegment::removeLastElementFromSegment (
  int inputLineNumber)
{
  // this last element can be a note or a tuplet,
  // this method is used when the seconde note of a chord is mest

  if (fSegmentMeasuresFlatList.size ()) {
    return
      fSegmentMeasuresFlatList.back ()->
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

  if (fSegmentMeasuresFlatList.size ()) {
    fSegmentMeasuresFlatList.back ()->
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

 if (fSegmentMeasuresFlatList.size ()) {
    fSegmentMeasuresFlatList.back ()->
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

  if (! fSegmentMeasuresFlatList.size ()) {
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
      fSegmentMeasuresFlatList.back ();

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
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "removeLastMeasureFromSegment() 1");
  }
#endif

  if (! fSegmentMeasuresFlatList.size ()) {
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
      fSegmentMeasuresFlatList.back ();

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

  fSegmentMeasuresFlatList.pop_back ();

  // remove it from segment elements list too JMI v0.9.63 KAKA

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    fSegmentVoiceUpLink->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
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

  for (S_msrMeasure measure : fSegmentMeasuresFlatList) {
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

  for (S_msrSegmentElement segmentElement : fSegmentElementsList) {
    // browse the element
    msrBrowser<msrSegmentElement> browser (v);
    browser.browse (*segmentElement);
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
    '[' <<
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
    '[' <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
      ", fSegmentDebugNumber: '" <<
      fSegmentDebugNumber <<
    "' in voice \"" <<
    fSegmentVoiceUpLink->getVoiceName () <<
    "\"";

  if (! fSegmentElementsList.size ()) {
    s <<
      " (0 measures)";
  }
  else {
    s <<
      " (" <<
      mfSingularOrPlural (
        fSegmentElementsList.size (), "measure", " measures") <<
      "), i.e. [";

    list<S_msrSegmentElement>::const_iterator
      iBegin = fSegmentElementsList.begin (),
      iEnd   = fSegmentElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asString ();
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
      fSegmentElementsList.size (), "measure", "measures") <<
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

  if (! fSegmentElementsList.size ()) {
    os <<
      setw (fieldWidth) <<
      "fSegmentElementsList" << " : " << "empty" <<
      endl;
  }

  else {
    os << endl;

    list<S_msrSegmentElement>::const_iterator
      iBegin = fSegmentElementsList.begin (),
      iEnd   = fSegmentElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;

  os << ']' << endl;
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
      fSegmentElementsList.size (), "measure", "measures") <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 20;

  if (! fSegmentElementsList.size ()) {
    os <<
      setw (fieldWidth) <<
      "fSegmentElementsList" << " : " << "empty" <<
      endl;
  }

  else {
    os << endl;

    list<S_msrSegmentElement>::const_iterator
      iBegin = fSegmentElementsList.begin (),
      iEnd   = fSegmentElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrSegment& elt)
{
  elt->print (os);
  return os;
}


}
