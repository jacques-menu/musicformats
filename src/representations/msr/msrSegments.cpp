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

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrBreaks.h"
#include "msrDoubleTremolos.h"
#include "msrOctaveShifts.h"
#include "msrRehearsalMarks.h"
#include "msrSegments.h"
#include "msrTempos.h"
#include "msrVoiceStaffChanges.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
int msrSegment::gSegmentsCounter = 0;
int msrSegment::gSegmentDebugNumber = 0;

S_msrSegment msrSegment::create (
  int        inputLineNumber,
  const S_msrVoice& segmentUpLinkToVoice)
{
  msrSegment* o =
    new msrSegment (
      inputLineNumber,
      segmentUpLinkToVoice);
  assert (o != nullptr);
  return o;
}

msrSegment::msrSegment (
  int        inputLineNumber,
  const S_msrVoice& segmentUpLinkToVoice)
    : msrVoiceElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segmentUpLinkToVoice != nullptr,
    "segmentUpLinkToVoice is null");

  // set segment's voice upLink
  fSegmentUpLinkToVoice =
    segmentUpLinkToVoice;

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
      "', segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "', in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\", line " << fInputLineNumber <<
      std::endl;
  }
#endif

// JMI v0.9.63
//  // segment shortest note duration
//  fSegmentShortestNoteDuration = Rational (INT_MAX, 1);

// segment shortest note tuplet factor
//  fSegmentShortestNoteTupletFactor = Rational (1, 1);
}

void msrSegment::setSegmentFirstMeasure (
  const S_msrMeasure& measure)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceSegments ()
      ||
    gGlobalTracingOahGroup->getTraceMeasures ()
  ) {
    gLogStream <<
      "Setting segment first measure to ";

    if (measure) {
      gLogStream <<
        measure->asString ();
    }
    else {
      gLogStream << "null";
    }

    gLogStream <<
      " in segment " <<
      asString () <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fSegmentFirstMeasure = measure;
}

void msrSegment::setSegmentLastMeasure (
  const S_msrMeasure& measure)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceSegments ()
      ||
    gGlobalTracingOahGroup->getTraceMeasures ()
  ) {
    gLogStream <<
      "Setting segment last measure to ";

    if (measure) {
      gLogStream <<
        measure->asString ();
    }
    else {
      gLogStream << "null";
    }

    gLogStream <<
      " in segment " <<
      asString () <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fSegmentLastMeasure = measure;
}

S_msrStaff msrSegment::fetchSegmentUpLinkToStaff () const
{
  S_msrStaff result;

  if (fSegmentUpLinkToVoice) {
    result =
      fSegmentUpLinkToVoice->
        getVoiceUpLinkToStaff ();
  }

  return result;
}

S_msrPart msrSegment::fetchSegmentUpLinkToPart () const
{
  S_msrPart result;

  if (fSegmentUpLinkToVoice) {
    result =
      fSegmentUpLinkToVoice->
        fetchVoiceUpLinkToPart ();
  }

  return result;
}

S_msrPartGroup msrSegment::fetchSegmentUpLinkToPartGroup () const
{
  S_msrPartGroup result;

  if (fSegmentUpLinkToVoice) {
    result =
      fSegmentUpLinkToVoice->
        fetchVoiceUpLinkToPartGroup ();
  }

  return result;
}

S_msrScore msrSegment::fetchSegmentUpLinkToScore () const
{
  S_msrScore result;

  if (fSegmentUpLinkToVoice) {
    result =
      fSegmentUpLinkToVoice->
        fetchVoiceUpLinkToScore ();
  }

  return result;
}

S_msrSegment msrSegment::createSegmentNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a newborn clone of segment " <<
      asString () <<
      std::endl;
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
  const S_msrVoice& containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a deep clone of segment " <<
      asString () <<
      std::endl;
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
        std::endl;
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
        std::endl;
    }
#endif
  }

  // upLinks
  segmentDeepClone->fSegmentUpLinkToVoice =
    containingVoice;

  return segmentDeepClone;
}

/* JMI
void msrSegment::setSegmentShortestNoteDuration (
  const Rational& duration)
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
      std::endl;
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
      std::endl;
  }
#endif

  fSegmentShortestNoteTupletFactor = noteTupletFactor;
}
*/

void msrSegment::assertSegmentLastMeasureIsNotNull (
  int inputLineNumber) const
{
  if (! fSegmentLastMeasure) {
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasuresDetails ()
      ||
    gGlobalTracingOahGroup->getTraceSegmentsDetails ()
      ||
    gGlobalTracingOahGroup->getTraceRepeatsDetails ()
  ) {
    fSegmentUpLinkToVoice->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "assertSegmentLastMeasureIsNotNull()");
  }
#endif

    std::stringstream s;

    s <<
      "fSegmentLastMeasure is null in segment " <<
      this->asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      std::endl;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

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
    fSegmentUpLinkToVoice->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "assertSegmentElementsListIsNotEmpty()");
  }
#endif

    std::stringstream s;

    s <<
      "assertSegmentElementsListIsNotEmpty()" <<
      ", fSegmentElementsList is empty" <<
      ", segment: " <<
      this->asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      std::endl;

    gLogStream <<
      s.str () <<
      std::endl;

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
  const std::string& measureNumber,
  msrMeasureImplicitKind
                measureImplicitKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating and appending a measure '" << measureNumber <<
      "', to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // determine new measure 'first in segment' kind
  msrMeasureFirstInSegmentKind
    measureFirstInSegmentKind;

  if (fSegmentElementsList.size () == 0) {
    // this is the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindYes;
  }
  else {
    // this is not the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindNo;
  }

  // create a measure
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" << measureNumber <<
      "' and appending it to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", measureFirstInSegmentKind: " <<
      msrMeasureFirstInSegmentKindAsString (
        measureFirstInSegmentKind) <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrMeasure
    result =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        this);

  // set measure end input line number JMI v0.9.66 to be done in appendMeasureToSegment() ???
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting end line number of measure number '" << // JMI
      measureNumber <<
      "' to " <<
      previousMeasureEndInputLineNumber <<
//       " in segment '" << // JMI
//       fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
//       "' in voice \"" <<
//       fMeasureUpLinkToSegment->
//         getSegmentUpLinkToVoice ()->
//           getVoiceName () <<
//       "\"" <<
      std::endl;
  }
#endif

  result->
    setMeasureEndInputLineNumber (
      previousMeasureEndInputLineNumber);

  // set result's ordinal number
  result->
    setMeasureOrdinalNumberInVoice (
      fSegmentUpLinkToVoice->
        incrementVoiceCurrentMeasureOrdinalNumber ());

  // append result to the segment
  appendMeasureToSegment (result);

  --gIndenter;

  return result;
}

void msrSegment::setNextMeasureNumberInSegment (
  int           inputLineNumber,
  const std::string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" << nextMeasureNumber <<
      "' in segment " << asString () <<
      "'in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "' line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  if (fSegmentElementsList.size ()) { // JMI ???
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Setting next measure number to '" << nextMeasureNumber <<
        "' in segment " << asString () <<
        "'s last measure " <<
        " in voice \"" <<
        fSegmentUpLinkToVoice->getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    fSegmentLastMeasure->
      setNextMeasureNumber (
        nextMeasureNumber);
  }

  --gIndenter;
}

void msrSegment::appendMusicXMLPrintLayoutToSegment (
  const S_msrMusicXMLPrintLayout& musicXMLPrintLayout)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMusicXMLPrintLayouts ()) {
    gLogStream <<
      "Appending print layout " << musicXMLPrintLayout->asString () <<
      " to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // sanity check
  if (fSegmentElementsList.size () == 0) {
    std::stringstream s;

    s <<
      "fSegmentElementsList is empty"  << // JMI
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gLogStream <<
      "fSegmentUpLinkToVoice:" <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      fSegmentUpLinkToVoice <<
      std::endl;
    --gIndenter;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      musicXMLPrintLayout->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register print layout in segments's current measure
  fSegmentLastMeasure->
    appendMusicXMLPrintLayoutToMeasure (musicXMLPrintLayout);

  --gIndenter;
}

void msrSegment::appendClefToSegment  (
  const S_msrClef& clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // sanity check
  if (fSegmentElementsList.size () == 0) {
    std::stringstream s;

    s <<
      "fSegmentElementsList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gLogStream <<
      "fSegmentUpLinkToVoice:" <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      fSegmentUpLinkToVoice <<
      std::endl;
    --gIndenter;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
       clef->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register clef in segments's current measure
  fSegmentLastMeasure->
    appendClefToMeasure (clef);

  --gIndenter;
}

void msrSegment::prependClefToSegment  (
  const S_msrClef& clef) // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Prepending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // sanity check
  if (fSegmentElementsList.size () == 0) {
    std::stringstream s;

    s <<
      "fSegmentElementsList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", fSegmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gLogStream <<
      "fSegmentUpLinkToVoice:" <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      fSegmentUpLinkToVoice <<
      std::endl;
    --gIndenter;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      clef->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register clef in segments's current measure

  S_msrSegmentElement
    segmentElementsListFirstElement =
      fSegmentElementsList.front ();

  if (
    // measure?

    S_msrMeasure
      measure =
        dynamic_cast<msrMeasure*>(&(*segmentElementsListFirstElement))
  ) {
    // prepend the clef to the first measure
    fSegmentFirstMeasure->
      appendClefToMeasure (clef);
  }

  else if (
    // full-bar rest?

    S_msrMultipleFullBarRests
      multipleFullBarRests =
        dynamic_cast<msrMultipleFullBarRests*>(&(*segmentElementsListFirstElement))
  ) {
    const std::list<S_msrMeasure>&
      fullBarRestsMeasuresList =
        multipleFullBarRests->
          getFullBarRestsMeasuresList ();

    if (fullBarRestsMeasuresList.size ()) {
      S_msrMeasure
        fullBarRestsMeasuresListFirstMeasure = // JMI v0.9.64 ???
          fullBarRestsMeasuresList.front ();

      // prepend the clef to the full-bar rest first measure
      fullBarRestsMeasuresListFirstMeasure->
        appendClefToMeasure (clef);
    }

    else {
      std::stringstream s;

      s <<
        "attempt at prepending clef " <<
        clef->asShortString () <<
        " to segment " <<
        this->asString () <<
        " which is not a measure nor a full-bar rest" <<
        ", in voice \"" <<
        fSegmentUpLinkToVoice->getVoiceName () <<
        "\"" <<
        "', line " << fInputLineNumber; // JMI v0.9.64

      msrInternalError ( // JMI v0.9.64 ???
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }


//   if (segmentElementsListFirstElement == fSegmentFirstMeasure) {
//   }
  else {
    std::stringstream s;

    s <<
      "attempt at prepending clef " <<
      clef->asShortString () <<
      " to segment " <<
      this->asString () <<
      " which is not a measure nor a full-bar rest" <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "', line " << fInputLineNumber; // JMI v0.9.64

    gLogStream <<
      std::endl << std::endl <<
      s.str () <<
      std::endl << std::endl;

    gLogStream << "THIS:" << std::endl;
    gLogStream << "----------------------------" << std::endl;
    ++gIndenter;
    this->printShort (gLogStream);
    --gIndenter;

    gLogStream << std::endl;

    gLogStream << "fSegmentFirstMeasure:" << std::endl;
    gLogStream << "----------------------------" << std::endl;
    ++gIndenter;
    fSegmentFirstMeasure->printShort (gLogStream);
    --gIndenter;

    gLogStream << std::endl;

    gLogStream << "fSegmentLastMeasure:" << std::endl;
    gLogStream << "----------------------------" << std::endl;
    ++gIndenter;
    fSegmentLastMeasure->printShort (gLogStream);
    --gIndenter;

    gLogStream << std::endl;

    gLogStream << "segmentElementsListLastElement:" << std::endl;
    gLogStream << "----------------------------" << std::endl;
    ++gIndenter;
    segmentElementsListFirstElement->printShort (gLogStream);
    --gIndenter;

    abort (); // JMI

    msrInternalError ( // JMI v0.9.64 ???
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

//   fSegmentMeasuresFlatList.front ()-> JMI v0.9.64
//     appendClefToMeasure (clef);

  --gIndenter;
}

void msrSegment::appendKeyToSegment (
  const S_msrKey& key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " << key->asString () <<
      " to segment " << asString () <<
    ", in voice \"" <<
    fSegmentUpLinkToVoice->getVoiceName () <<
    "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    key->getInputLineNumber ());

  ++gIndenter;

  // register key in segments's current measure
  fSegmentLastMeasure->
    appendKeyToMeasure (key);

  --gIndenter;
}

void msrSegment::appendTimeSignatureToSegment (
  const S_msrTimeSignature& timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    gLogStream <<
      "to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    timeSignature->getInputLineNumber ());

  ++gIndenter;

  // append timeSignature to segments's current measure
  fSegmentLastMeasure->
    appendTimeSignatureToMeasure (timeSignature);

  --gIndenter;
}

void msrSegment::appendTimeSignatureToSegmentClone (
  const S_msrTimeSignature& timeSignature){
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      timeSignature;

    --gIndenter;

    gLogStream <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    timeSignature->getInputLineNumber ());

  // append timeSignature to segments's current measure
  fSegmentLastMeasure->
    appendTimeSignatureToMeasureClone (timeSignature);

  --gIndenter;
}

void msrSegment::insertHiddenMeasureAndBarLineInSegmentClone (
  int             inputLineNumber,
  const Rational& measurePosition)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      measurePosition <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    inputLineNumber);

  // append time to segments's current measure
  fSegmentLastMeasure->
    insertHiddenMeasureAndBarLineInMeasureClone (
      inputLineNumber,
      measurePosition);

  --gIndenter;
}

void msrSegment::appendHarmonyToSegment (const S_msrHarmony& harmony)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  int inputLineNumber =
    harmony->getInputLineNumber ();

  ++gIndenter;

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    inputLineNumber);

  // append harmony to this segment
  fSegmentLastMeasure->
    appendHarmonyToMeasure (harmony);

  --gIndenter;
}

void msrSegment::appendHarmonyToSegmentClone (const S_msrHarmony& harmony)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    harmony->getInputLineNumber ());

  // append it to this segment
  fSegmentLastMeasure->
    appendHarmonyToMeasureClone (harmony);
}

void msrSegment::appendFiguredBassToSegment (
  const S_msrFiguredBass& figuredBass)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass " <<
      figuredBass->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  ++gIndenter;

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    inputLineNumber);

  // append figuredBass to this segment
  fSegmentLastMeasure->
    appendFiguredBassToMeasure (
      figuredBass);

  --gIndenter;
}

void msrSegment::appendFiguredBassToSegmentClone (
  const S_msrFiguredBass& figuredBass)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass " <<
      figuredBass->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    figuredBass->getInputLineNumber ());

  // append it to this segment
  fSegmentLastMeasure->
    appendFiguredBassToMeasure (figuredBass);
}

void msrSegment::appendSegnoToSegment (const S_msrSegno& segno)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegnos ()) {
    gLogStream <<
      "Appending segno " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    segno->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendSegnoToMeasure (segno);

  --gIndenter;
}

void msrSegment::appendCodaToSegment (const S_msrCoda& coda)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceCodas ()) {
    gLogStream <<
      "Appending coda " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    coda->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendCodaToMeasure (coda);

  --gIndenter;
}

void msrSegment::appendEyeGlassesToSegment (
  const S_msrEyeGlasses& eyeGlasses)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceEyeGlasses ()) {
    gLogStream <<
      "Appending eyeGlasses " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    eyeGlasses->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendEyeGlassesToMeasure (eyeGlasses);

  --gIndenter;
}

void msrSegment::appendPedalToSegment (const S_msrPedal& pedal)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePedals ()) {
    gLogStream <<
      "Appending pedal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }

#endif
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    pedal->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendPedalToMeasure (pedal);

  --gIndenter;
}

void msrSegment::appendDampToSegment (
  const S_msrDamp& damp)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDamps ()) {
    gLogStream <<
      "Appending damp " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    damp->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendDampToMeasure (damp);

  --gIndenter;
}

void msrSegment::appendDampAllToSegment (
  const S_msrDampAll& dampAll)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDampAlls ()) {
    gLogStream <<
      "Appending damp all " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    dampAll->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendDampAllToMeasure (dampAll);

  --gIndenter;
}

void msrSegment::appendTranspositionToSegment (
  const S_msrTransposition& transposition)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Appending transposition " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    transposition->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendTranspositionToMeasure (transposition);

  --gIndenter;
}

void msrSegment::appendStaffDetailsToSegment (
  const S_msrStaffDetails& staffDetails)
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
      "Appending staff details " <<
      staffDetails->asShortString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\" line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceStaffDetails ()
      ||
    gGlobalTracingOahGroup->getTraceSegments ()
  ) {
    fSegmentUpLinkToVoice->
      displayVoice (
        inputLineNumber,
        "appendStaffDetailsToSegment() 1");
  }
#endif

  ++gIndenter;

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    staffDetails->getInputLineNumber ());

  // append staffDetails to this segment
  fSegmentLastMeasure->
    appendStaffDetailsToMeasure (staffDetails);

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceStaffDetails ()
      ||
    gGlobalTracingOahGroup->getTraceSegments ()
  ) {
    fSegmentUpLinkToVoice->
      displayVoice (
        inputLineNumber,
        "appendStaffDetailsToSegment() 2");
  }
#endif

  --gIndenter;
}

void msrSegment::appendLineBreakToSegment (
  const S_msrLineBreak& lineBreak)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
    gLogStream <<
      "Appending break " <<
      lineBreak->asShortString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    lineBreak->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendLineBreakToMeasure (lineBreak);

  --gIndenter;
}

void msrSegment::appendPageBreakToSegment (
  const S_msrPageBreak& pageBreak)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
    gLogStream <<
      "Appending break " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    pageBreak->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendPageBreakToMeasure (pageBreak);

  --gIndenter;
}

void msrSegment::appendBarNumberCheckToSegment (
  const S_msrBarNumberCheck& barNumberCheck)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarNumberChecks ()) {
    gLogStream <<
      "Appending bar number check " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    barNumberCheck->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendBarNumberCheckToMeasure (barNumberCheck);

  --gIndenter;
}

void msrSegment::appendTempoToSegment (
  const S_msrTempo& tempo)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Appending tempo " <<
      tempo->asShortString () <<
      " to segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    tempo->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendTempoToMeasure (tempo);

  --gIndenter;
}

void msrSegment::appendRehearsalMarkToSegment (
  const S_msrRehearsalMark& rehearsalMark)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRehearsalMarks ()) {
    gLogStream <<
      "Appending rehearsalMark " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    rehearsalMark->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendRehearsalMarkToMeasure (rehearsalMark);

  --gIndenter;
}

void msrSegment::appendOctaveShiftToSegment (
  const S_msrOctaveShift& octaveShift)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceOctaveShifts ()) {
    gLogStream <<
      "Appending octave shift '" <<
      octaveShift->getOctaveShiftKind () <<
      "' to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    octaveShift->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendOctaveShiftToMeasure (octaveShift);

  --gIndenter;
}

void msrSegment::appendScordaturaToSegment (
  const S_msrScordatura& scordatura)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceScordaturas ()) {
    gLogStream <<
      "Appending scordatura XXX " <<
  // JMI ???    scordatura->getOctaveShiftKind () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    scordatura->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendScordaturaToMeasure (scordatura);

  --gIndenter;
}

void msrSegment::appendAccordionRegistrationToSegment (
  const S_msrAccordionRegistration&
    accordionRegistration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending accordion registration " <<
      accordionRegistration->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    accordionRegistration->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendAccordionRegistrationToMeasure (
      accordionRegistration);

  --gIndenter;
}

void msrSegment::appendHarpPedalsTuningToSegment (
  const S_msrHarpPedalsTuning&
    harpPedalsTuning)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending staff pedals tuning " <<
      harpPedalsTuning->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    harpPedalsTuning->getInputLineNumber ());

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendHarpPedalsTuningToMeasure (
      harpPedalsTuning);

  --gIndenter;
}

void msrSegment::padUpToMeasurePositionInSegment (
  int             inputLineNumber,
  const Rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Padding up to whole notes '" << wholeNotes <<
      "' in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (false && fSegmentMeasuresFlatList.size () == 0) { // JMI
    std::stringstream s;

    s <<
      "fSegmentMeasuresFlatList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gLogStream <<
      "fSegmentUpLinkToVoice:" <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      fSegmentUpLinkToVoice <<
      std::endl;
    --gIndenter;

    gLogStream <<
      "Part:" <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      fSegmentUpLinkToVoice->fetchVoiceUpLinkToPart () <<
      std::endl;
    --gIndenter;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (fSegmentMeasuresFlatList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentLastMeasure->
      padUpToMeasurePosition (
        inputLineNumber,
        wholeNotes);
  }
}

void msrSegment::backupByWholeNotesStepLengthInSegment (
  int     inputLineNumber,
  const Rational&
          backupTargetMeasureElementMeasurePosition)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceSegments ()
      ||
    gGlobalTracingOahGroup->getTraceMeasures ()
  ) {
    gLogStream <<
      "Backup by a '" <<
      backupTargetMeasureElementMeasurePosition <<
      "' whole notes step length in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fSegmentMeasuresFlatList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentLastMeasure->
      backupByWholeNotesStepLengthInMeasure (
        inputLineNumber,
        backupTargetMeasureElementMeasurePosition);
  }
}

void msrSegment::appendPaddingNoteToSegment (
  int             inputLineNumber,
  const Rational& forwardStepLength)
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
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  if (fSegmentMeasuresFlatList.size ()) { // JMI BOFBOF
    // append a padding note to the segment's last measure
    fSegmentLastMeasure->
      appendPaddingSkipNoteToMeasure (
        inputLineNumber,
        forwardStepLength);
  }

  --gIndenter;
}

void msrSegment::appendMeasureToSegment (const S_msrMeasure& measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();

  std::string measureNumber =
    measure->getMeasureNumber ();

  size_t segmentElementsListSize =
    fSegmentMeasuresFlatList.size ();

  std::string currentMeasureNumber =
    segmentElementsListSize == 0
      ? ""
      : fSegmentLastMeasure->getMeasureNumber ();

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
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << measure->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (measureNumber == currentMeasureNumber) {
    std::stringstream s;

    s <<
      "appending measure number '" << measureNumber <<
      "' occurs twice in a row in segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    msrInternalWarning ( // JMI KAKA v0.9.63
//     msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
//       __FILE__, __LINE__,
      s.str ());
  }

  // is measure the first one this segment?
  if (! fSegmentFirstMeasure) {
    measure->
      setMeasureFirstInSegmentKind (
        msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindYes);

    setSegmentFirstMeasure (measure);
  }
  else {
    measure->
      setMeasureFirstInSegmentKind (
        msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindNo);
  }

  // is measure the first one it the voice?
  // this is necessary for voice clones,
  // which don't go down the part-staff-voice-segment hierarchy
  if (! fSegmentUpLinkToVoice->getVoiceFirstMeasure ()) {
    // yes, register it as such
    fSegmentUpLinkToVoice->
      setVoiceFirstMeasure (measure);

    measure->
      setMeasureIsFirstInVoice ();
  }

  // append measure to the segment
  fSegmentElementsList.push_back (measure);

  fSegmentMeasuresFlatList.push_back (measure);

  // register measure as the last one in the segment
  setSegmentLastMeasure (
    measure);
}

void msrSegment::prependMeasureToSegment (const S_msrMeasure& measure)
{
  int inputLineNumber =
    measure->getInputLineNumber ();

  std::string measureNumber =
    measure->getMeasureNumber ();

  size_t segmentElementsListSize =
    fSegmentElementsList.size ();

  std::string currentMeasureNumber =
    segmentElementsListSize == 0
      ? ""
      : fSegmentLastMeasure->getMeasureNumber ();

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
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"," <<
      ", line " << measure->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (measureNumber == currentMeasureNumber) {
    std::stringstream s;

    s <<
      "prepending measure number '" << measureNumber <<
      "' occurs twice in a row in segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    msrInternalError (
// JMI    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  measure->
    setMeasureIsFirstInVoice ();

  // prepend measure to the segment
  fSegmentElementsList.push_front (measure);

  fSegmentMeasuresFlatList.push_front (measure);
}

void msrSegment::appendMultipleFullBarRestsToSegment (
  const S_msrMultipleFullBarRests& multipleFullBarRests)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending multiple full-bar rests " << multipleFullBarRests->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // append multipleFullBarRests to the segment
  fSegmentElementsList.push_back (multipleFullBarRests);
}

void msrSegment::prependBarLineToSegment (
  const S_msrBarLine& barLine)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Prepending barLine " << barLine->asString () <<
      "' to segment '" << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentElementsListIsNotEmpty (
    barLine->getInputLineNumber ());

  // prepend barLine to this segment
  ++gIndenter;

  fSegmentMeasuresFlatList.front ()-> // JMI ??? v0.9.63
    prependBarLineToMeasure (barLine);

  --gIndenter;
}

void msrSegment::appendBarLineToSegment (
  const S_msrBarLine& barLine)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " << barLine->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    barLine->getInputLineNumber ());

  ++gIndenter;

  // append barLine to this segment
  fSegmentLastMeasure->
    appendBarLineToMeasure (barLine);

  --gIndenter;
}

void msrSegment::appendBarCheckToSegment (
  const S_msrBarCheck& barCheck)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarChecks ()) {
    gLogStream <<
      "Appending bar check " << barCheck->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"," <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    barCheck->getInputLineNumber ());

  fSegmentLastMeasure->
    appendBarCheckToMeasure (barCheck);
}

void msrSegment::appendVoiceStaffChangeToSegment (
  const S_msrVoiceStaffChange& voiceStaffChange)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "Appending voice staff change " <<
      voiceStaffChange->asString () <<
      " to segment " << asString () <<
      ", line " << voiceStaffChange->getInputLineNumber () <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    voiceStaffChange->getInputLineNumber ());

  ++gIndenter;

  fSegmentLastMeasure->
    appendVoiceStaffChangeToMeasure (
      voiceStaffChange);

  --gIndenter;
}

void msrSegment::appendNoteToSegment (
  const S_msrNote&       note,
  const Rational& partMeasurePosition)
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
  assertSegmentLastMeasureIsNotNull (
    note->getInputLineNumber ());

  fSegmentLastMeasure->
    appendNoteToMeasure (
      note,
      partMeasurePosition);
}

void msrSegment::appendNoteToSegmentClone (const S_msrNote& note)
{
  fSegmentLastMeasure->
    appendNoteToMeasureClone (note);
}

void msrSegment::appendDoubleTremoloToSegment ( // JMI
  const S_msrDoubleTremolo& doubleTremolo)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Appending double tremolo " <<
      doubleTremolo->asShortString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  assertSegmentLastMeasureIsNotNull (
    doubleTremolo->getInputLineNumber ());

  fSegmentLastMeasure->
    appendDoubleTremoloToMeasure (doubleTremolo);
}

void msrSegment::appendChordToSegment (const S_msrChord& chord) // JMI
{
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    chord->getInputLineNumber ());

  fSegmentLastMeasure->
    appendChordToMeasure (chord);
}

void msrSegment::appendTupletToSegment (const S_msrTuplet& tuplet) // JMI
{
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    tuplet->getInputLineNumber ());

  fSegmentLastMeasure->
    appendTupletToMeasure (tuplet);
}

/* JMI
void msrSegment::addGraceNotesGroupAheadOfSegmentIfNeeded (
  const S_msrGraceNotesGroup& graceNotesGroup)

{
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    graceNotesGroup->getInputLineNumber ());

  fSegmentMeasuresFlatList.front ()->
    addGraceNotesGroupAheadOfMeasure (graceNotesGroup);
}

void msrSegment::appendAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)
{
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    afterGraceNotes->getInputLineNumber ());

  fSegmentLastMeasure->
    appendAfterGraceNotesToMeasure (afterGraceNotes);
}

void msrSegment::prependAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)

{
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    afterGraceNotes->getInputLineNumber ());

  fSegmentMeasuresFlatList.front ()->
    prependAfterGraceNotesToMeasure (afterGraceNotes); // JMI
}
*/

void msrSegment::prependOtherElementToSegment (
  const S_msrMeasureElement& elem)
{
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    elem->getInputLineNumber ());

  fSegmentLastMeasure->
    prependOtherElementToMeasure (elem);
}

void msrSegment::appendOtherElementToSegment (
  const S_msrMeasureElement& elem)
{
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    elem->getInputLineNumber ());

  fSegmentLastMeasure->
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
      fSegmentLastMeasure->
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
  const S_msrNote& note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Removing note " <<
      note->asString () <<
      " from segment " <<
      asString () <<
      std::endl;
  }
#endif

  ++gIndenter;

  if (fSegmentMeasuresFlatList.size ()) {
    fSegmentLastMeasure->
      removeNoteFromMeasure (
        inputLineNumber,
        note);
  }

  else {
    std::stringstream s;

    s <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
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
  const S_msrElement& element)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Removing element " <<
      element->asString () <<
      " from segment '" <<
      asString () <<
      std::endl;
  }
#endif

 if (fSegmentMeasuresFlatList.size ()) {
    fSegmentLastMeasure->
      removeElementFromMeasure (
        inputLineNumber,
        element);
  }

  else {
    std::stringstream s;

    s <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
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
  const std::string& context)
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
      std::endl;
  }
#endif

  if (! fSegmentMeasuresFlatList.size ()) {
    std::stringstream s;

    s <<
      "cannot fetch last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", since it is empty";

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
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    gLogStream <<
      std::endl <<
      "The fetched measure contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      result <<
      std::endl;

    --gIndenter;
  }
#endif

  return result;
}

S_msrMeasure msrSegment::removeLastMeasureFromSegment (
  int           inputLineNumber,
  const std::string& context)
{
  S_msrMeasure result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Removing last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      "', segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    fSegmentUpLinkToVoice->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "removeLastMeasureFromSegment() 1");
  }
#endif

//   if (! fSegmentMeasuresFlatList.size ()) {
//     std::stringstream s;
//
//     s <<
//       "cannot remove last measure from segment '" <<
//       fSegmentAbsoluteNumber <<
//       ", segmentDebugNumber: '" <<
//       fSegmentDebugNumber <<
//       "' in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"" <<
//       ", since it is empty";
//
//     msrInternalError (
//       gGlobalServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, __LINE__,
//       s.str ());
//   }

  // sanity check
  if (! fSegmentLastMeasure) {
    std::stringstream s;

    s <<
      "cannot remove last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", since that segment does not contain any";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // we've got the result
  result = fSegmentLastMeasure;

  // remove it from segment elements list too ??? JMI v0.9.63 KAKA

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    fSegmentUpLinkToVoice->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "removeLastMeasureFromSegment() 2");
  }
#endif

  S_msrSegmentElement
    segmentElementsListLastElement =
      fSegmentElementsList.back ();

  if (segmentElementsListLastElement == fSegmentLastMeasure) {
    // remove it from the elements list
    fSegmentElementsList.pop_back ();
  }
  else {
    std::stringstream s;

    s <<
      "attempt at removing the last measure of segment " <<
      this->asString () <<
      " which is not at the end of fSegmentElementsList" <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber;

    gLogStream <<
      std::endl << std::endl <<
      s.str () <<
      std::endl << std::endl;

    gLogStream << "THIS:" << std::endl;
    gLogStream << "----------------------------" << std::endl;
    ++gIndenter;
    this->printShort (gLogStream);
    --gIndenter;

    gLogStream << std::endl;

    gLogStream << "fSegmentLastMeasure:" << std::endl;
    gLogStream << "----------------------------" << std::endl;
    ++gIndenter;
    fSegmentLastMeasure->printShort (gLogStream);
    --gIndenter;

    gLogStream << std::endl;

    gLogStream << "segmentElementsListLastElement:" << std::endl;
    gLogStream << "----------------------------" << std::endl;
    ++gIndenter;
    segmentElementsListLastElement->printShort (gLogStream);
    --gIndenter;

    abort (); // JMI

    msrInternalError ( // JMI v0.9.64 ???
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "The removed measure contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      result->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  // remove the last measure from the segments measures flat std::list
  fSegmentMeasuresFlatList.pop_back ();

  // don't forget about fSegmentLastMeasure now,
  // since it may be used and/or re-appended soon JMI v0.9.63
//   setSegmentLastMeasure ( // KAKA
//     nullptr);

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
      std::endl;
  }
#endif

  for (S_msrMeasure measure : fSegmentMeasuresFlatList) {
    measure->
      finalizeMeasure (
        inputLineNumber,
        msrMeasureRepeatContextKind::kMeasureRepeatContextNone,
        "finalizeAllTheMeasuresOfSegment()");
  } // for
}

void msrSegment::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegment::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegment::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrSegment::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegment::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegment::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrSegment::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegment::browseData ()" <<
      std::endl;
  }

  for (S_msrSegmentElement segmentElement : fSegmentElementsList) {
    // browse the element
    msrBrowser<msrSegmentElement> browser (v);
    browser.browse (*segmentElement);
  } // for

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrSegment::browseData ()" <<
      std::endl;
  }
}

std::string msrSegment::asShortString () const
{
  std::stringstream s;

  s <<
    '[' <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
    ", fSegmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "', voice: \"" <<
    fSegmentUpLinkToVoice->getVoiceName () <<
    "\"" <<
    ']';

  return s.str ();
}

std::string msrSegment::asString () const
{
  std::stringstream s;

  s <<
    '[' <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
    "', fSegmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "' in voice \"" <<
    fSegmentUpLinkToVoice->getVoiceName () <<
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

    if (fSegmentElementsList.size ()) {
      std::list<S_msrSegmentElement>::const_iterator
        iBegin = fSegmentElementsList.begin (),
        iEnd   = fSegmentElementsList.end (),
        i      = iBegin;

      for ( ; ; ) {
        s << (*i)->asString ();
        if (++i == iEnd) break;
        s << ", ";
      } // for
    }

    s << ']';
  }

  s << ']';

  return s.str ();
}

void msrSegment::displaySegment (
  int           inputLineNumber,
  const std::string& context)
{
  gLogStream <<
    std::endl <<
    "*********>> Segment '" <<
    fSegmentAbsoluteNumber <<
    ", segmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    /* JMI
    "', score:" <<
    std::endl <<
    fSegmentUpLinkToVoice->
      fetchVoiceGroupUpLinkToScore () <<
            */
    "', voice:" <<
    std::endl <<
    fSegmentUpLinkToVoice->getVoiceName () <<
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

void msrSegment::print (std::ostream& os) const
{
  os <<
    "[Segment '" <<
    fSegmentAbsoluteNumber <<
    "', fSegmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "', " <<
    mfSingularOrPlural (
      fSegmentElementsList.size (), "measure", "measures") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 20;

  os <<
    std::setw (fieldWidth) <<
    "fSegmentUpLinkToVoice" << " : " <<
    "\"" <<
    fSegmentUpLinkToVoice->getVoiceName () <<
    "\"" <<
    std::endl;

/* JMI
  os << std::left <<
    std::setw (fieldWidth) <<
    "fSegmentShortestNoteDuration" << " : " <<
    fSegmentShortestNoteDuration <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSegmentShortestNoteTupletFactor" << " : " <<
    fSegmentShortestNoteTupletFactor <<
    std::endl;
*/

  os <<
    "fSegmentFirstMeasure" << " : ";
  if (fSegmentFirstMeasure) {
    os << std::endl;
    ++gIndenter;
    os << fSegmentFirstMeasure;
    --gIndenter;
  }
  else {
    os << "null" << std::endl;
  }

  os <<
    "fSegmentLastMeasure" << " : ";
  if (fSegmentLastMeasure) {
    os << std::endl;
    ++gIndenter;
    os << fSegmentLastMeasure;
    --gIndenter;
  }
  else {
    os << "null" << std::endl;
  }

  if (! fSegmentElementsList.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fSegmentElementsList" << " : " << "[EMPTY]" <<
      std::endl;
  }

  else {
    os << std::endl;

    std::list<S_msrSegmentElement>::const_iterator
      iBegin = fSegmentElementsList.begin (),
      iEnd   = fSegmentElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrSegment::printShort (std::ostream& os) const
{
  os <<
    "[Segment '" <<
    fSegmentAbsoluteNumber <<
    "', fSegmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "', " <<
    mfSingularOrPlural (
      fSegmentElementsList.size (), "measure", "measures") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 20;

  if (! fSegmentElementsList.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fSegmentElementsList" << " : " << "[EMPTY]" <<
      std::endl;
  }

  else {
    os << std::endl;

    std::list<S_msrSegmentElement>::const_iterator
      iBegin = fSegmentElementsList.begin (),
      iEnd   = fSegmentElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSegment& elt)
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
