/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

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

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
int msrSegment::sSegmentsCounter = 0;
int msrSegment::sSegmentDebugNumber = 0;

S_msrSegment msrSegment::create (
  int        inputLineNumber,
  const S_msrVoice& segmentUpLinkToVoice)
{
  msrSegment* obj =
    new msrSegment (
      inputLineNumber,
      segmentUpLinkToVoice);
  assert (obj != nullptr);
  return obj;
}

msrSegment::msrSegment (
  int        inputLineNumber,
  const S_msrVoice& segmentUpLinkToVoice)
    : msrVoiceElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segmentUpLinkToVoice != nullptr,
    "segmentUpLinkToVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
  fSegmentAbsoluteNumber = ++sSegmentsCounter;
  fSegmentDebugNumber    = ++sSegmentDebugNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegments ()) {
    std::stringstream ss;

    ss <<
      "Initializing new segment '" <<
      fSegmentAbsoluteNumber <<
      "', segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "', in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// JMI v0.9.63
//  // segment shortest note wholeNotes
//  fSegmentShortestNoteWholeNotes = msrWholeNotes (INT_MAX, 1);

// segment shortest note tuplet factor
//  fSegmentShortestNoteTupletFactor = msrWholeNotes (1, 1);
}

void msrSegment::setSegmentFirstMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceSegments ()
      ||
    gTraceOahGroup->getTraceMeasures ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting segment first measure to ";

    if (measure) {
      ss <<
        measure->asString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      " in segment " <<
      asString () <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSegmentFirstMeasure = measure;
}

void msrSegment::setSegmentLastMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceSegments ()
      ||
    gTraceOahGroup->getTraceMeasures ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting segment last measure to ";

    if (measure) {
      ss <<
        measure->asString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      " in segment " <<
      asString () <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegments ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of segment " <<
      asString () <<
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
    containingVoice != nullptr,
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegments ()) {
    std::stringstream ss;

    ss <<
      "Creating a deep clone of segment " <<
      asString () <<
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
    containingVoice != nullptr,
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrSegment
    deepClone =
      msrSegment::create (
        fInputLineNumber,
        containingVoice);

  // absolute number
  deepClone->fSegmentAbsoluteNumber =
    fSegmentAbsoluteNumber;

  // keep debug number fSegmentDebugNumber unchanged

  // the measure elements in the segment contain the mmusic
  size_t segmentElementsListSize =
   fSegmentElementsList.size ();

  if (segmentElementsListSize != 0) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceVoices ()) {
      std::stringstream ss;

      ss <<
        "There are " <<
        segmentElementsListSize <<
        " measure elements in segment to be deep copied" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    for (S_msrSegmentElement measureElement : fSegmentElementsList) {
      // append a deep clone of the measure element to the deep clone
      // DEEP CLONING IS NOT YET FINALIZED JMI v0.9.63
//       if (
//         S_msrMeasure measure = dynamic_cast<msrRepeat*>(&(*(measureElement)))
//       ) {

//       deepClone->
//         appendMeasureToSegment (
//           measureElement->
//             createMeasureDeepClone (this));
    } // for
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSegments ()) {
      std::stringstream ss;

      ss <<
        "There are no measures in segment to be deep copied" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // upLinks
  deepClone->fSegmentUpLinkToVoice =
    containingVoice;

  return deepClone;
}

/* JMI
void msrSegment::setSegmentShortestNoteWholeNotes (
  const msrWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotes ()
      ||
    gMsrOahGroup->getTraceMsrNotesDurations ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting the shortest note wholeNotes of segment " <<
      fSegmentAbsoluteNumber <<
      " to " <<
      wholeNotes.asString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSegmentShortestNoteWholeNotes = wholeNotes;
}

void msrSegment::setSegmentShortestNoteTupletFactor (
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
      "Setting the shortest note tuplet factor of segment " <<
      fSegmentAbsoluteNumber <<
      " to " <<
      noteTupletFactor <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSegmentShortestNoteTupletFactor = noteTupletFactor;
}
*/

void msrSegment::assertSegmentLastMeasureIsNotNull (
  int inputLineNumber) const
{
  if (! fSegmentLastMeasure) {
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasuresDetails ()
      ||
    gTraceOahGroup->getTraceSegmentsDetails ()
      ||
    gTraceOahGroup->getTraceRepeatsDetails ()
  ) {
    fSegmentUpLinkToVoice->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "assertSegmentLastMeasureIsNotNull()");
  }
#endif // MF_TRACE_IS_ENABLED

    std::stringstream ss;

    ss <<
      "fSegmentLastMeasure is null in segment " <<
      this->asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      std::endl;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msrSegment::assertSegmentElementsListIsNotEmpty (
  int inputLineNumber) const
{
  if (! fSegmentElementsList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasuresDetails ()
      ||
    gTraceOahGroup->getTraceSegmentsDetails ()
      ||
    gTraceOahGroup->getTraceRepeatsDetails ()
  ) {
    fSegmentUpLinkToVoice->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "assertSegmentElementsListIsNotEmpty()");
  }
#endif // MF_TRACE_IS_ENABLED

    std::stringstream ss;

    ss <<
      "assertSegmentElementsListIsNotEmpty()" <<
      ", fSegmentElementsList is empty" <<
      ", segment: " <<
      this->asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      std::endl;

    gLog <<
      ss.str () <<
      std::endl;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }
}

S_msrMeasure msrSegment::createAMeasureAndAppendItToSegment (
  int           inputLineNumber,
  int           previousMeasureEndInputLineNumber,
  const std::string& measureNumber,
  msrMeasureImplicitKind
                measureImplicitKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating and appending a measure '" << measureNumber <<
      "', to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // determine new measure 'first in segment' kind
#ifdef MF_TRACE_IS_ENABLED
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
#endif // MF_TRACE_IS_ENABLED

  // create a measure
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating measure '" << measureNumber <<
      "' and appending it to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", measureFirstInSegmentKind: " <<
      measureFirstInSegmentKind <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrMeasure
    result =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        this);

  // set measure end input line number JMI v0.9.66 to be done in appendMeasureToSegment() ???
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
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

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting next measure number to '" << nextMeasureNumber <<
      "' in segment " << asString () <<
      "'in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "' line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fSegmentElementsList.size ()) { // JMI ???
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "Setting next measure number to '" << nextMeasureNumber <<
        "' in segment " << asString () <<
        "'s last measure " <<
        " in voice \"" <<
        fSegmentUpLinkToVoice->getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fSegmentLastMeasure->
      setNextMeasureNumber (
        nextMeasureNumber);
  }

  --gIndenter;
}

void msrSegment::appendMusicXMLPrintLayoutToSegment (
  const S_msrMusicXMLPrintLayout& musicXMLPrintLayout)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
    std::stringstream ss;

    ss <<
      "Appending print layout " << musicXMLPrintLayout->asString () <<
      " to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (fSegmentElementsList.size () == 0) {
    std::stringstream ss;

    ss <<
      "fSegmentElementsList is empty"  << // JMI
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gLog <<
      "fSegmentUpLinkToVoice:" <<
      std::endl;
    ++gIndenter;
    gLog <<
      fSegmentUpLinkToVoice <<
      std::endl;
    --gIndenter;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      musicXMLPrintLayout->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // register print layout in segments's current measure
  fSegmentLastMeasure->
    appendMusicXMLPrintLayoutToMeasure (musicXMLPrintLayout);

  --gIndenter;
}

void msrSegment::appendClefToSegment  (
  const S_msrClef& clef)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefs ()) {
    std::stringstream ss;

    ss <<
      "Appending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (fSegmentElementsList.size () == 0) {
    std::stringstream ss;

    ss <<
      "fSegmentElementsList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gLog <<
      "fSegmentUpLinkToVoice:" <<
      std::endl;
    ++gIndenter;
    gLog <<
      fSegmentUpLinkToVoice <<
      std::endl;
    --gIndenter;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
       clef->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // register clef in segments's current measure
  fSegmentLastMeasure->
    appendClefToMeasure (clef);

  --gIndenter;
}

void msrSegment::prependClefToSegment  (
  const S_msrClef& clef) // JMI
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefs ()) {
    std::stringstream ss;

    ss <<
      "Prepending clef '" << clef->asString () <<
      "' to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (fSegmentElementsList.size () == 0) {
    std::stringstream ss;

    ss <<
      "fSegmentElementsList is empty"  <<
      " in segment '" <<
      fSegmentAbsoluteNumber <<
      ", fSegmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gLog <<
      "fSegmentUpLinkToVoice:" <<
      std::endl;
    ++gIndenter;
    gLog <<
      fSegmentUpLinkToVoice <<
      std::endl;
    --gIndenter;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      clef->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
      std::stringstream ss;

      ss <<
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
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }


//   if (segmentElementsListFirstElement == fSegmentFirstMeasure) {
//   }
  else {
    std::stringstream ss;

    ss <<
      "attempt at prepending clef " <<
      clef->asShortString () <<
      " to segment " <<
      this->asString () <<
      " which is not a measure nor a full-bar rest" <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "', line " << fInputLineNumber; // JMI v0.9.64

    gLog <<
      std::endl << std::endl <<
      ss.str () <<
      std::endl << std::endl;

    gLog << "THIS:" << std::endl;
    gLog << "----------------------------" << std::endl;
    ++gIndenter;
    gLog << this;
    --gIndenter;

    gLog << std::endl;

    gLog << "fSegmentFirstMeasure:" << std::endl;
    gLog << "----------------------------" << std::endl;
    ++gIndenter;
    gLog << fSegmentFirstMeasure;
    --gIndenter;

    gLog << std::endl;

    gLog << "fSegmentLastMeasure:" << std::endl;
    gLog << "----------------------------" << std::endl;
    ++gIndenter;
    gLog << fSegmentLastMeasure;
    --gIndenter;

    gLog << std::endl;

    gLog << "segmentElementsListLastElement:" << std::endl;
    gLog << "----------------------------" << std::endl;
    ++gIndenter;
    gLog << segmentElementsListFirstElement;
    --gIndenter;

    abort (); // JMI

    msrInternalError ( // JMI v0.9.64 ???
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

//   fSegmentMeasuresFlatList.front ()-> JMI v0.9.64
//     appendClefToMeasure (clef);

  --gIndenter;
}

void msrSegment::appendKeyToSegment (
  const S_msrKey& key)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Appending key " << key->asString () <<
      " to segment " << asString () <<
    ", in voice \"" <<
    fSegmentUpLinkToVoice->getVoiceName () <<
    "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    key->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // register key in segments's current measure
  fSegmentLastMeasure->
    appendKeyToMeasure (key);

  --gIndenter;
}

void msrSegment::appendTimeSignatureToSegment (
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      timeSignature;

    --gIndenter;

    gLog <<
      "to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    timeSignature->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append timeSignature to segments's current measure
  fSegmentLastMeasure->
    appendTimeSignatureToMeasure (timeSignature);

  --gIndenter;
}

void msrSegment::appendTimeSignatureToSegmentClone (
  const S_msrTimeSignature& timeSignature){
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      timeSignature;

    --gIndenter;

    gLog <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    timeSignature->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // append timeSignature to segments's current measure
  fSegmentLastMeasure->
    appendTimeSignatureToMeasureClone (timeSignature);

  --gIndenter;
}

void msrSegment::insertHiddenMeasureAndBarLineInSegmentClone (
  int             inputLineNumber,
  const msrWholeNotes& measurePosition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Inserting hidden measure and barLine at position " <<
      measurePosition.asString () <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    inputLineNumber);
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // append time to segments's current measure
  fSegmentLastMeasure->
    insertHiddenMeasureAndBarLineInMeasureClone (
      inputLineNumber,
      measurePosition);

  --gIndenter;
}

void msrSegment::appendHarmonyToSegment (
  int                  inputLineNumber,
  const S_msrHarmony&  harmony,
  const msrWholeNotes& measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " << harmony->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    inputLineNumber);
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append harmony to this segment
  fSegmentLastMeasure->
    appendHarmonyToMeasure (
      inputLineNumber,
      harmony,
      measurePositionToAppendAt);

  --gIndenter;
}

void msrSegment::appendHarmoniesListToSegment (
  int                            inputLineNumber,
  const std::list<S_msrHarmony>& harmoniesList,
  const msrWholeNotes&           measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmonies list to segment " << // JMI v0.9.67 HARMFUL
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the harmonies to this segment
  fSegmentLastMeasure->
    appendHarmoniesListToMeasure(
      inputLineNumber,
      harmoniesList,
      measurePositionToAppendAt);
}

void msrSegment::appendHarmonyToSegmentClone (const S_msrHarmony& harmony)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " << harmony->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    harmony->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // append harmony to this segment
  fSegmentLastMeasure->
    appendHarmonyToMeasureClone (harmony);
}

void msrSegment::appendFiguredBassToSegment (
  int                     inputLineNumber,
  const S_msrFiguredBass& figuredBass,
  const msrWholeNotes&    measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass " <<
      figuredBass->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    inputLineNumber);
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append figuredBass to this segment
  fSegmentLastMeasure->
    appendFiguredBassToMeasure (
      inputLineNumber,
      figuredBass,
      measurePositionToAppendAt);

  --gIndenter;
}

void msrSegment::appendFiguredBassesListToSegment (
  int                                inputLineNumber,
  const std::list<S_msrFiguredBass>& figuredBasssesList,
  const msrWholeNotes&               measurePositionToAppendAt)
{

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured basses list \"" <<
//       figuredBasssesList->asString () << // JMI v0.9.67 HARMFUL
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the figured basses to this segment
  fSegmentLastMeasure->
    appendFiguredBassesListToMeasure (
      inputLineNumber,
      figuredBasssesList,
      measurePositionToAppendAt);
}

void msrSegment::appendFiguredBassToSegmentClone (
  const S_msrFiguredBass& figuredBass)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass " <<
      figuredBass->asString () <<
      " to segment clone " << asString () <<
      "' in voice clone \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    figuredBass->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // append figuredBass to this segment
  fSegmentLastMeasure->
    appendFiguredBassToMeasureClone (figuredBass);
}

void msrSegment::appendSegnoToSegment (const S_msrSegno& segno)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegnos ()) {
    std::stringstream ss;

    ss <<
      "Appending segno " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    segno->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendSegnoToMeasure (segno);

  --gIndenter;
}

void msrSegment::appendCodaToSegment (const S_msrCoda& coda)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCodas ()) {
    std::stringstream ss;

    ss <<
      "Appending coda " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    coda->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendCodaToMeasure (coda);

  --gIndenter;
}

void msrSegment::appendEyeGlassesToSegment (
  const S_msrEyeGlasses& eyeGlasses)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceEyeGlasses ()) {
    std::stringstream ss;

    ss <<
      "Appending eyeGlasses " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    eyeGlasses->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendEyeGlassesToMeasure (eyeGlasses);

  --gIndenter;
}

void msrSegment::appendPedalToSegment (const S_msrPedal& pedal)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePedals ()) {
    std::stringstream ss;

    ss <<
      "Appending pedal " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    pedal->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendPedalToMeasure (pedal);

  --gIndenter;
}

void msrSegment::appendDampToSegment (
  const S_msrDamp& damp)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDamps ()) {
    std::stringstream ss;

    ss <<
      "Appending damp " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    damp->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendDampToMeasure (damp);

  --gIndenter;
}

void msrSegment::appendDampAllToSegment (
  const S_msrDampAll& dampAll)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDampAlls ()) {
    std::stringstream ss;

    ss <<
      "Appending damp all " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    dampAll->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendDampAllToMeasure (dampAll);

  --gIndenter;
}

void msrSegment::appendTranspositionToSegment (
  const S_msrTransposition& transposition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTranspositions ()) {
    std::stringstream ss;

    ss <<
      "Appending transposition " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    transposition->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendTranspositionToMeasure (transposition);

  --gIndenter;
}

void msrSegment::appendStaffDetailsToSegment (
  const S_msrStaffDetails& staffDetails)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    staffDetails->getInputLineNumber ();

  if (
    gTraceOahGroup->getTraceStavesDetails ()
      ||
    gTraceOahGroup->getTraceSegments ()
  ) {
    std::stringstream ss;

    ss <<
      "Appending staff details " <<
      staffDetails->asShortString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\" line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceStavesDetails ()
      ||
    gTraceOahGroup->getTraceSegments ()
  ) {
    fSegmentUpLinkToVoice->
      displayVoice (
        inputLineNumber,
        "appendStaffDetailsToSegment() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    staffDetails->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // append staffDetails to this segment
  fSegmentLastMeasure->
    appendStaffDetailsToMeasure (staffDetails);

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceStavesDetails ()
      ||
    gTraceOahGroup->getTraceSegments ()
  ) {
    fSegmentUpLinkToVoice->
      displayVoice (
        inputLineNumber,
        "appendStaffDetailsToSegment() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrSegment::appendLineBreakToSegment (
  const S_msrLineBreak& lineBreak)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLineBreaks ()) {
    std::stringstream ss;

    ss <<
      "Appending break " <<
      lineBreak->asShortString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    lineBreak->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendLineBreakToMeasure (lineBreak);

  --gIndenter;
}

void msrSegment::appendPageBreakToSegment (
  const S_msrPageBreak& pageBreak)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePageBreaks ()) {
    std::stringstream ss;

    ss <<
      "Appending break " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    pageBreak->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendPageBreakToMeasure (pageBreak);

  --gIndenter;
}

void msrSegment::appendBarNumberCheckToSegment (
  const S_msrBarNumberCheck& barNumberCheck)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarNumberChecks ()) {
    std::stringstream ss;

    ss <<
      "Appending bar number check " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    barNumberCheck->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendBarNumberCheckToMeasure (barNumberCheck);

  --gIndenter;
}

void msrSegment::appendTempoToSegment (
  const S_msrTempo& tempo)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Appending tempo " <<
      tempo->asShortString () <<
      " to segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    tempo->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendTempoToMeasure (tempo);

  --gIndenter;
}

void msrSegment::appendRehearsalMarkToSegment (
  const S_msrRehearsalMark& rehearsalMark)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRehearsalMarks ()) {
    std::stringstream ss;

    ss <<
      "Appending rehearsalMark " <<
      " to segment " << asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    rehearsalMark->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendRehearsalMarkToMeasure (rehearsalMark);

  --gIndenter;
}

void msrSegment::appendOctaveShiftToSegment (
  const S_msrOctaveShift& octaveShift)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceOctaveShifts ()) {
    std::stringstream ss;

    ss <<
      "Appending octave shift '" <<
      octaveShift->getOctaveShiftKind () <<
      "' to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    octaveShift->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendOctaveShiftToMeasure (octaveShift);

  --gIndenter;
}

void msrSegment::appendScordaturaToSegment (
  const S_msrScordatura& scordatura)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceScordaturas ()) {
    std::stringstream ss;

    ss <<
      "Appending scordatura XXX " <<
  // JMI ???    scordatura->getOctaveShiftKind () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    scordatura->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegments ()) {
    std::stringstream ss;

    ss <<
      "Appending accordion registration " <<
      accordionRegistration->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    accordionRegistration->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegments ()) {
    std::stringstream ss;

    ss <<
      "Appending staff pedals tuning " <<
      harpPedalsTuning->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    harpPedalsTuning->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append it to this segment
  fSegmentLastMeasure->
    appendHarpPedalsTuningToMeasure (
      harpPedalsTuning);

  --gIndenter;
}

// void msrSegment::padUpToMeasurePositionInSegment (
//   int             inputLineNumber,
//   const msrWholeNotes& wholeNotes)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasurePositions ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Padding up to whole notes '" << wholeNotes.asString () <<
//       "' in segment '" <<
//       fSegmentAbsoluteNumber <<
//       ", segmentDebugNumber: '" <<
//       fSegmentDebugNumber <<
//       " in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\", line " << inputLineNumber <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (false && fSegmentMeasuresFlatList.size () == 0) { // JMI
//     std::stringstream ss;
//
//     ss <<
//       "fSegmentMeasuresFlatList is empty"  <<
//       " in segment '" <<
//       fSegmentAbsoluteNumber <<
//       ", segmentDebugNumber: '" <<
//       fSegmentDebugNumber <<
//       " in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"";
//
//     gLog <<
//       "fSegmentUpLinkToVoice:" <<
//       std::endl;
//     ++gIndenter;
//     gLog <<
//       fSegmentUpLinkToVoice <<
//       std::endl;
//     --gIndenter;
//
//     gLog <<
//       "Part:" <<
//       std::endl;
//     ++gIndenter;
//     gLog <<
//       fSegmentUpLinkToVoice->fetchVoiceUpLinkToPart () <<
//       std::endl;
//     --gIndenter;
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, __LINE__,
//       ss.str ());
//   }
//
//   if (fSegmentMeasuresFlatList.size ()) { // JMI BOFBOF v0.9.67
//     // pad last measure up to to this actual wholes notes
//     fSegmentLastMeasure->
//       padUpToMeasurePositionInMeasure (
//         inputLineNumber,
//         wholeNotes);
//   }
// }

void msrSegment::backupByWholeNotesStepLengthInSegment (
  int     inputLineNumber,
  const msrWholeNotes&
          backupTargetMeasurePosition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceSegments ()
      ||
    gTraceOahGroup->getTraceMeasures ()
  ) {
    std::stringstream ss;

    ss <<
      "Backup by a '" <<
      backupTargetMeasurePosition.asString () <<
      "' whole notes step length in segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fSegmentMeasuresFlatList.size ()) { // JMI BOFBOF
    // pad last measure up to to this actual wholes notes
    fSegmentLastMeasure->
      backupByWholeNotesStepLengthInMeasure (
        inputLineNumber,
        backupTargetMeasurePosition);
  }
}

void msrSegment::appendPaddingNoteToSegment (
  int             inputLineNumber,
  const msrWholeNotes& forwardStepLength)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
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

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fSegmentMeasuresFlatList.size ()) { // JMI BOFBOF v0.9.67
    // append a padding note to the segment's last measure
    fSegmentLastMeasure->
      appendPaddingSkipNoteToMeasure ( // JMI why not a rest??? v0.9.67
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Appending measure '" << measureNumber <<
      "' to segment " << asString ();

    if (fSegmentMeasuresFlatList.size () == 0)
      gLog <<
        ", as first measure";
    else
      gLog <<
      ", after measure number '" << currentMeasureNumber << "'";

    gLog <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << measure->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (measureNumber == currentMeasureNumber) {
    std::stringstream ss;

    ss <<
      "appending measure number '" << measureNumber <<
      "' occurs twice in a row in segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    msrInternalWarning ( // JMI v0.9.67 v0.9.63
//     msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
//       __FILE__, __LINE__,
      ss.str ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Prepending measure " << measureNumber <<
      " to segment " << asString ();

    if (segmentElementsListSize == 0) {
      gLog <<
        ", as first measure";
    }

/* JMI v0.9.66
    else
      gLog <<
      ", after measure number '" << currentMeasureNumber << "'";
*/

    gLog <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"," <<
      ", line " << measure->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (measureNumber == currentMeasureNumber) {
    std::stringstream ss;

    ss <<
      "prepending measure number '" << measureNumber <<
      "' occurs twice in a row in segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    msrInternalError (
// JMI    msrInternalWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Appending multiple full-bar rests " << multipleFullBarRests->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append multipleFullBarRests to the segment
  fSegmentElementsList.push_back (multipleFullBarRests);
}

void msrSegment::prependBarLineToSegment (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Prepending barLine " << barLine->asString () <<
      "' to segment '" << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentElementsListIsNotEmpty (
    barLine->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // prepend barLine to this segment
  ++gIndenter;

  fSegmentMeasuresFlatList.front ()-> // JMI ??? v0.9.63
    prependBarLineToMeasure (barLine);

  --gIndenter;
}

void msrSegment::appendBarLineToSegment (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Appending barLine " << barLine->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    barLine->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append barLine to this segment
  fSegmentLastMeasure->
    appendBarLineToMeasure (barLine);

  --gIndenter;
}

void msrSegment::appendBarCheckToSegment (
  const S_msrBarCheck& barCheck)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarChecks ()) {
    std::stringstream ss;

    ss <<
      "Appending bar check " << barCheck->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"," <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    barCheck->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSegmentLastMeasure->
    appendBarCheckToMeasure (barCheck);
}

void msrSegment::appendVoiceStaffChangeToSegment (
  const S_msrVoiceStaffChange& voiceStaffChange)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChanges ()) {
    std::stringstream ss;

    ss <<
      "Appending voice staff change " <<
      voiceStaffChange->asString () <<
      " to segment " << asString () <<
      ", line " << voiceStaffChange->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    voiceStaffChange->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  fSegmentLastMeasure->
    appendVoiceStaffChangeToMeasure (
      voiceStaffChange);

  --gIndenter;
}

void msrSegment::appendNoteToSegment (
  const S_msrNote&       note,
  const msrWholeNotes& partDrawingMeasurePosition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    if (! fSegmentElementsList.size ()) { // JMI
      displaySegment (
        note->getInputLineNumber (),
        "appendNoteToSegment()");
    }
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    note->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSegmentLastMeasure->
    appendNoteToMeasure (
      note,
      partDrawingMeasurePosition);
}

void msrSegment::appendNoteToSegmentClone (const S_msrNote& note)
{
  fSegmentLastMeasure->
    appendNoteToMeasureClone (note);
}

void msrSegment::appendDoubleTremoloToSegment ( // JMI
  const S_msrDoubleTremolo& doubleTremolo)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Appending double tremolo " <<
      doubleTremolo->asShortString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    doubleTremolo->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSegmentLastMeasure->
    appendDoubleTremoloToMeasure (doubleTremolo);
}

void msrSegment::appendChordToSegment (const S_msrChord& chord) // JMI
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    chord->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSegmentLastMeasure->
    appendChordToMeasure (chord);
}

void msrSegment::appendTupletToSegment (const S_msrTuplet& tuplet) // JMI
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    tuplet->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSegmentLastMeasure->
    appendTupletToMeasure (tuplet);
}

/* JMI
void msrSegment::addGraceNotesGroupAheadOfSegmentIfNeeded (
  const S_msrGraceNotesGroup& graceNotesGroup)

{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    graceNotesGroup->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSegmentMeasuresFlatList.front ()->
    addGraceNotesGroupAheadOfMeasure (graceNotesGroup);
}

void msrSegment::appendAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    afterGraceNotes->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSegmentLastMeasure->
    appendAfterGraceNotesToMeasure (afterGraceNotes);
}

void msrSegment::prependAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)

{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    afterGraceNotes->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSegmentMeasuresFlatList.front ()->
    prependAfterGraceNotesToMeasure (afterGraceNotes); // JMI
}
*/

void msrSegment::prependOtherElementToSegment (
  const S_msrMeasureElement& elem)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    elem->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSegmentLastMeasure->
    prependOtherElementToMeasure (elem);
}

void msrSegment::appendOtherElementToSegment (
  const S_msrMeasureElement& elem)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  assertSegmentLastMeasureIsNotNull (
    elem->getInputLineNumber ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Removing note " <<
      note->asString () <<
      " from segment " <<
      asString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fSegmentMeasuresFlatList.size ()) {
    fSegmentLastMeasure->
      removeNoteFromMeasure (
        inputLineNumber,
        note);
  }

  else {
    std::stringstream ss;

    ss <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"," <<
      " since it is empty";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  --gIndenter;
}

void msrSegment::removeElementFromSegment (
  int          inputLineNumber,
  const S_msrElement& element)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegments ()) {
    std::stringstream ss;

    ss <<
      "Removing element " <<
      element->asString () <<
      " from segment '" <<
      asString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 if (fSegmentMeasuresFlatList.size ()) {
    fSegmentLastMeasure->
      removeElementFromMeasure (
        inputLineNumber,
        element);
  }

  else {
    std::stringstream ss;

    ss <<
      "cannot remove note from segment " <<
      asString () <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"," <<
      " since it is empty";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }
}

S_msrMeasure msrSegment::fetchLastMeasureFromSegment (
  int           inputLineNumber,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Fetching last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      " (" << context << ")" <<
      "', line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fSegmentMeasuresFlatList.size ()) {
    std::stringstream ss;

    ss <<
      "cannot fetch last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", since it is empty";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresFlatList.back ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    gLog <<
      std::endl <<
      "The fetched measure contains:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      result <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

S_msrMeasure msrSegment::removeLastMeasureFromSegment (
  int           inputLineNumber,
  const std::string& context)
{
  S_msrMeasure result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Removing last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      "', segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    fSegmentUpLinkToVoice->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "removeLastMeasureFromSegment() 1");
  }
#endif // MF_TRACE_IS_ENABLED

//   if (! fSegmentMeasuresFlatList.size ()) {
//     std::stringstream ss;
//
//     ss <<
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
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, __LINE__,
//       ss.str ());
//   }

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (! fSegmentLastMeasure) {
    std::stringstream ss;

    ss <<
      "cannot remove last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", since that segment does not contain any";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // we've got the result
  result = fSegmentLastMeasure;

  // remove it from segment elements list too ??? JMI v0.9.63 JMI v0.9.67

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    fSegmentUpLinkToVoice->
      displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "removeLastMeasureFromSegment() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrSegmentElement
    segmentElementsListLastElement =
      fSegmentElementsList.back ();

  if (segmentElementsListLastElement == fSegmentLastMeasure) {
    // remove it from the elements list
    fSegmentElementsList.pop_back ();
  }
  else {
    std::stringstream ss;

    ss <<
      "attempt at removing the last measure of segment " <<
      this->asString () <<
      " which is not at the end of fSegmentElementsList" <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber;

    gLog <<
      std::endl << std::endl <<
      ss.str () <<
      std::endl << std::endl;

    gLog << "THIS:" << std::endl;
    gLog << "----------------------------" << std::endl;
    ++gIndenter;
    gLog << this;
    --gIndenter;

    gLog << std::endl;

    gLog << "fSegmentLastMeasure:" << std::endl;
    gLog << "----------------------------" << std::endl;
    ++gIndenter;
    gLog << fSegmentLastMeasure;
    --gIndenter;

    gLog << std::endl;

    gLog << "segmentElementsListLastElement:" << std::endl;
    gLog << "----------------------------" << std::endl;
    ++gIndenter;
    gLog << segmentElementsListLastElement;
    --gIndenter;

    abort (); // JMI

    msrInternalError ( // JMI v0.9.64 ???
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "The removed measure contains:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      result->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // remove the last measure from the segments measures flat list
  fSegmentMeasuresFlatList.pop_back ();

  // don't forget about fSegmentLastMeasure now,
  // since it may be used and/or re-appended soon JMI v0.9.63
//   setSegmentLastMeasure ( // JMI v0.9.67
//     nullptr);

  return result;
}

void msrSegment::finalizeAllTheMeasuresOfSegment ( // superflous JMI ???
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Finalizing all the measures of segment '" <<
      fSegmentAbsoluteNumber <<
      "', line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSegment::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSegment::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrSegment::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSegment::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSegment::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrSegment::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSegment::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrSegmentElement segmentElement : fSegmentElementsList) {
    // browse the element
    msrBrowser<msrSegmentElement> browser (v);
    browser.browse (*segmentElement);
  } // for

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrSegment::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrSegment::asShortString () const
{
  std::stringstream ss;

  ss <<
    '[' <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
    ", fSegmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "', voice: \"" <<
    fSegmentUpLinkToVoice->getVoiceName () <<
    "\"" <<
    ']';

  return ss.str ();
}

std::string msrSegment::asString () const
{
  std::stringstream ss;

  ss <<
    '[' <<
    "Segment '" <<
    fSegmentAbsoluteNumber <<
    "', fSegmentDebugNumber: '" <<
    fSegmentDebugNumber <<
    "' in voice \"" <<
    fSegmentUpLinkToVoice->getVoiceName () <<
    "\"";

  if (! fSegmentElementsList.size ()) {
    ss <<
      " (0 measures)";
  }
  else {
    ss <<
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
        ss << (*i)->asString ();
        if (++i == iEnd) break;
        ss << ", ";
      } // for
    }

    ss << ']';
  }

  ss << ']';

  return ss.str ();
}

void msrSegment::displaySegment (
  int           inputLineNumber,
  const std::string& context)
{
  gLog <<
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
  print (gLog);
  --gIndenter;

  gLog <<
    " <<*********" <<
    std::endl << std::endl;
}

void msrSegment::printFull (std::ostream& os) const
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
    "fSegmentUpLinkToVoice" << ": " <<
    "\"" <<
    fSegmentUpLinkToVoice->getVoiceName () <<
    "\"" <<
    std::endl;

/* JMI
  os << std::left <<
    std::setw (fieldWidth) <<
    "fSegmentShortestNoteWholeNotes" << ": " <<
    fSegmentShortestNoteWholeNotes.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSegmentShortestNoteTupletFactor" << ": " <<
    fSegmentShortestNoteTupletFactor <<
    std::endl;
*/

  os <<
    "fSegmentFirstMeasure" << ": ";
  if (fSegmentFirstMeasure) {
    os << std::endl;
    ++gIndenter;
    os << fSegmentFirstMeasure;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os <<
    "fSegmentLastMeasure" << ": ";
  if (fSegmentLastMeasure) {
    os << std::endl;
    ++gIndenter;
    os << fSegmentLastMeasure;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  if (! fSegmentElementsList.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fSegmentElementsList" << ": " << "[EMPTY]" <<
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

  if (! fSegmentElementsList.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fSegmentElementsList" << ": " << "[EMPTY]" <<
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

std::ostream& operator << (std::ostream& os, const S_msrSegment& elt)
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
