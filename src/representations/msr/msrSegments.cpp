/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrDoubleTremolos.h"
#include "msrLineBreaks.h"
#include "msrOctaveShifts.h"
#include "msrPageBreaks.h"
#include "msrRehearsalMarks.h"
#include "msrSegments.h"
#include "msrTempos.h"
#include "msrTuplets.h"
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
  const mfInputLineNumber& inputLineNumber,
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
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoice& segmentUpLinkToVoice)
    : msrVoiceElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    segmentUpLinkToVoice != nullptr,
    "segmentUpLinkToVoice is NULL");
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
  fSegmentDebugNumber = ++sSegmentDebugNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsBasics ()) {
    std::stringstream ss;

    ss <<
      "Initializing segment" <<
      ", fSegmentAbsoluteNumber: " <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "', in voice";
    if (fSegmentUpLinkToVoice) {
      ss <<
        "\"" <<
        fSegmentUpLinkToVoice->getVoiceName () <<
        "\"";
    }
    else {
      ss << "[NULL]";
    }
    ss <<
      "\", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// JMI 0.9.63
//  // segment shortest note wholeNotes
//  fSegmentShortestNoteWholeNotes = mfWholeNotes (INT_MAX, 1);

// segment shortest note tuplet factor
//  fSegmentShortestNoteTupletFactor = mfWholeNotes (1, 1);
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
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
    gTraceOahGroup->getTraceSegmentsBasics ()
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
      asShortString () << // avoid loop
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingVoice != nullptr,
    "containingVoice is NULL");
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
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingVoice != nullptr,
    "containingVoice is NULL");
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
        " measure elements in segment to be deep copied";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    for (S_msrSegmentElement measureElement : fSegmentElementsList) {
      // append a deep clone of the measure element to the deep clone
      // DEEP CLONING IS NOT YET FINALIZED JMI 0.9.63
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
        "There are no measures in segment to be deep copied";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
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
  const mfWholeNotes& wholeNotes)
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
      wholeNotes.asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      noteTupletFactor;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSegmentShortestNoteTupletFactor = noteTupletFactor;
}
*/

void msrSegment::assertSegmentLastMeasureIsNotNull (
  const mfInputLineNumber& inputLineNumber) const
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
      displayVoiceRepeatsStackMultipleMeasureRestsMeasureRepeatAndVoice (
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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
}

void msrSegment::assertSegmentElementsListIsNotEmpty (
  const mfInputLineNumber& inputLineNumber) const
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
      displayVoiceRepeatsStackMultipleMeasureRestsMeasureRepeatAndVoice (
        inputLineNumber,
        "assertSegmentElementsListIsNotEmpty()");
  }
#endif // MF_TRACE_IS_ENABLED

    std::stringstream ss;

    ss <<
      "assertSegmentElementsListIsNotEmpty()" <<
      ", fSegmentElementsList is empty in segment: " <<
      this->asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    gLog <<
      ss.str () <<
      std::endl;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
}

S_msrMeasure msrSegment::cascadeCreateAMeasureAndAppendItInSegment (
  const mfInputLineNumber& inputLineNumber,
  int                    previousMeasureEndInputLineNumber,
  const mfMeasureNumber& measureNumber,
  msrMeasureImplicitKind measureImplicitKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Cascading creating and appending a measure '" <<
      measureNumber <<
      "', to segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // determine new measure 'first in segment' kind
#ifdef MF_TRACE_IS_ENABLED
  msrMeasureFirstInSegmentKind
    measureFirstInSegmentKind;

  if (fSegmentElementsList.empty ()) {
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
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrMeasure
    result =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        this);

  // set measure end input line number JMI 0.9.66 to be done in appendMeasureToSegment() ???
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting end line number of measure number '" << // JMI
      measureNumber <<
      "' to " <<
      previousMeasureEndInputLineNumber;
//       " in segment '" << // JMI
//       fMeasureUpLinkToSegment->getSegmentAbsoluteNumber () <<
//       "' in voice \"" <<
//       fMeasureUpLinkToSegment->
//         getSegmentUpLinkToVoice ()->
//           getVoiceName () <<
//       "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   result-> JMI BEGINEND
//     setMeasureEndInputLineNumber (
//       previousMeasureEndInputLineNumber);

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
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber& nextMeasureNumber)
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
      "' line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (fSegmentElementsList.empty ()) {
    std::stringstream ss;

    ss <<
      ", fSegmentElementsList is empty in segment " <<
      this->asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", fSegmentAbsoluteNumber: " <<
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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // register print layout in segments's current measure
  fSegmentLastMeasure->
    appendMusicXMLPrintLayoutToMeasure (musicXMLPrintLayout);

  --gIndenter;
}

void msrSegment::appendClefKeyTimeSignatureGroupToSegment  (
  const S_msrClefKeyTimeSignatureGroup& clefKeyTimeSignatureGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending clefKeyTimeSignatureGroup " <<
      clefKeyTimeSignatureGroup->asString () <<
      " to segment " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   if (false && fSegmentElementsList.empty ()) { // JMI 0.9.73
  if (fSegmentElementsList.empty ()) { // JMI 0.9.73
    std::stringstream ss;

    ss <<
      ", fSegmentElementsList is empty in segment " <<
      this->asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
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
       clefKeyTimeSignatureGroup->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // register clefKeyTimeSignatureGroup in segments's current measure
  fSegmentLastMeasure->
    appendClefKeyTimeSignatureGroupToMeasure (
      clefKeyTimeSignatureGroup);

  --gIndenter;
}

// void msrSegment::appendClefToSegment  (
//   const S_msrClef& clef)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceClefs ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending clef " << clef
//       " to segment " << asString () <<
//       ", in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"" <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   if (fSegmentElementsList.empty ()) {
//     std::stringstream ss;
//
//     ss <<
//       ", fSegmentElementsList is empty in segment " <<
//       this->asString () <<
//       ", in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"" <<
//       fSegmentAbsoluteNumber <<
//       ", segmentDebugNumber: '" <<
//       fSegmentDebugNumber <<
//       "' in voice \"" <<
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
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//        clef->getInputLineNumber (),
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // register clef in segments's current measure
//   fSegmentLastMeasure->
//     appendClefToMeasure (clef);
//
//   --gIndenter;
// }
//
// void msrSegment::prependClefToSegment  (
//   const S_msrClef& clef) // JMI
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceClefs ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Prepending clef " << clef
//       " to segment " << asString () <<
//       ", in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"" <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//
//     abort ();
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   if (fSegmentElementsList.empty ()) {
//     std::stringstream ss;
//
//     ss <<
//       ", fSegmentElementsList is empty in segment " <<
//       this->asString () <<
//       ", in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"" <<
//       fSegmentAbsoluteNumber <<
//       ", fSegmentDebugNumber: '" <<
//       fSegmentDebugNumber <<
//       "' in voice \"" <<
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
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       clef->getInputLineNumber (),
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // register clef in segments's current measure
//
//   S_msrSegmentElement
//     segmentElementsListFirstElement =
//       fSegmentElementsList.front ();
//
//   if (
//     // measure?
//
//     S_msrMeasure
//       measure =
//         dynamic_cast<msrMeasure*>(&(*segmentElementsListFirstElement))
//   ) {
//     // prepend the clef to the first measure
//     fSegmentFirstMeasure->
//       appendClefToMeasure (clef);
//   }
//
//   else if (
//     // measure rest?
//
//     S_msrMultipleMeasureRest
//       multipleMeasureRest =
//         dynamic_cast<msrMultipleMeasureRest*>(&(*segmentElementsListFirstElement))
//   ) {
//     const std::list <S_msrMeasure>&
//       measureRestsList =
//         multipleMeasureRest->
//           getMeasureRestsList ();
//
//     if (measureRestsList.size ()) {
//       S_msrMeasure
//         measureRestsListFirstMeasure = // JMI 0.9.64 ???
//           measureRestsList.front ();
//
//       // prepend the clef to the measure rest first measure
//       measureRestsListFirstMeasure->
//         appendClefToMeasure (clef);
//     }
//
//     else {
//       std::stringstream ss;
//
//       ss <<
//         "attempt at prepending clef " <<
//         clef->asShortString () <<
//         " to segment " <<
//         this->asString () <<
//         " which is not a measure nor a measure rest" <<
//         ", in voice \"" <<
//         fSegmentUpLinkToVoice->getVoiceName () <<
//         "\"" <<
//         "', line " << fInputLineNumber; // JMI 0.9.64
//
//       msrInternalError ( // JMI 0.9.64 ???
//         gServiceRunData->getInputSourceName (),
//         fInputLineNumber,
//         __FILE__, mfInputLineNumber (__LINE__),
//         ss.str ());
//     }
//   }
//
// //   if (segmentElementsListFirstElement == fSegmentFirstMeasure) {
// //   }
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "attempt at prepending clef " <<
//       clef->asShortString () <<
//       " to segment " <<
//       this->asString () <<
//       " which is not a measure nor a measure rest" <<
//       ", in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"" <<
//       "', line " << fInputLineNumber; // JMI 0.9.64
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//
//     gLog << "THIS:" << std::endl;
//     gLog << "----------------------------" << std::endl;
//     ++gIndenter;
//     gLog << std::hex << std::showbase << this << std::dec; // JMI 0.9.69
//     --gIndenter;
//
//     gLog << std::endl;
//
//     gLog << "fSegmentFirstMeasure:" << std::endl;
//     gLog << "----------------------------" << std::endl;
//     ++gIndenter;
//     gLog << fSegmentFirstMeasure;
//     --gIndenter;
//
//     gLog << std::endl;
//
//     gLog << "fSegmentLastMeasure:" << std::endl;
//     gLog << "----------------------------" << std::endl;
//     ++gIndenter;
//     gLog << fSegmentLastMeasure;
//     --gIndenter;
//
//     gLog << std::endl;
//
//     gLog << "segmentElementsListLastElement:" << std::endl;
//     gLog << "----------------------------" << std::endl;
//     ++gIndenter;
//     gLog << segmentElementsListFirstElement;
//     --gIndenter;
//
// //     abort (); // JMI 0.9.67 HARMFUL
//
//     msrInternalError ( // JMI 0.9.64 ???
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
// //   fSegmentMeasuresList.front ()-> JMI 0.9.64
// //     appendClefToMeasure (clef);
//
//   --gIndenter;
// }
//
// void msrSegment::appendKeyToSegment (
//   const S_msrKey& key)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceKeys ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending key " << key
//       " to segment " << asString () <<
//     ", in voice \"" <<
//     fSegmentUpLinkToVoice->getVoiceName () <<
//     "\"" <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   assertSegmentLastMeasureIsNotNull (
//     key->getInputLineNumber ());
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   ++gIndenter;
//
//   // register key in segments's current measure
//   fSegmentLastMeasure->
//     appendKeyToMeasure (key);
//
//   --gIndenter;
// }

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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  const mfInputLineNumber& inputLineNumber,
  const mfPositionInMeasure& positionInMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Inserting hidden measure and barLine at position " <<
      positionInMeasure.asString () <<
      "to segment clone " << asString () <<
      ", in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      positionInMeasure);

  --gIndenter;
}

void msrSegment::appendHarmonyToSegment (
  const mfInputLineNumber& inputLineNumber,
  const S_msrHarmony&        harmony,
  const mfPositionInMeasure& positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmoniesBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " << harmony->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      positionInMeasureToAppendAt);

  --gIndenter;
}

void msrSegment::appendHarmoniesListToSegment (
  const mfInputLineNumber&        inputLineNumber,
  const std::list <S_msrHarmony>& harmoniesList,
  const mfPositionInMeasure&      positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmoniesBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending harmonies list to segment " << // JMI 0.9.67 HARMFUL
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the harmonies to this segment
  fSegmentLastMeasure->
    appendHarmoniesListToMeasure(
      inputLineNumber,
      harmoniesList,
      positionInMeasureToAppendAt);
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  const mfInputLineNumber& inputLineNumber,
  const S_msrFiguredBass&    figuredBass,
  const mfPositionInMeasure& positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBassesBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass " <<
      figuredBass->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      positionInMeasureToAppendAt);

  --gIndenter;
}

void msrSegment::cascadeAppendFiguredBassesListToSegment (
  const mfInputLineNumber&            inputLineNumber,
  const std::list <S_msrFiguredBass>& figuredBasssesList,
  const mfPositionInMeasure&          positionInMeasureToAppendAt)
{

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBassesBasics ()) {
    std::stringstream ss;

    ss <<
      "Cascading appending figured basses list \"" <<
//       figuredBasssesList->asString () << // JMI 0.9.67 HARMFUL
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the figured basses to this segment
  fSegmentLastMeasure->
    cascadeAppendFiguredBassesListToMeasure (
      inputLineNumber,
      figuredBasssesList,
      positionInMeasureToAppendAt);
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\" line " << staffDetails->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
        staffDetails->getInputLineNumber (),
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
        staffDetails->getInputLineNumber (),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

// void msrSegment::padUpToPositionInMeasureInSegment (
//   const mfInputLineNumber& inputLineNumber,
//   const mfWholeNotes& wholeNotes)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePositionInMeasures ()) {
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
//       gWaeHandler->waeTrace (
//         __FILE__, mfInputLineNumber (__LINE__),
//         ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (false && fSegmentMeasuresList.empty ()) { // JMI
//     std::stringstream ss;
//
//     ss <<
//       "fSegmentMeasuresList is empty"  <<
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
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   if (fSegmentMeasuresList.size ()) { // JMI BOFBOF 0.9.67
//     // pad last measure up to to this actual wholes notes
//     fSegmentLastMeasure->
//       padUpToPositionInMeasureInMeasure (
//         inputLineNumber,
//         wholeNotes);
//   }
// }

// void msrSegment::casadeBackupByWholeNotesStepLengthInSegment (
//   const mfInputLineNumber& inputLineNumber,
//   const mfWholeNotes&
//           backupTargetMeasureElementPositionInMeasure)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (
//     gTraceOahGroup->getTraceSegments ()
//       ||
//     gTraceOahGroup->getTraceMeasures ()
//   ) {
//     std::stringstream ss;
//
//     ss <<
//       "Backup by a '" <<
//       backupTargetMeasureElementPositionInMeasure.asString () <<
//       "' whole notes step length in segment '" <<
//       fSegmentAbsoluteNumber <<
//       ", segmentDebugNumber: '" <<
//       fSegmentDebugNumber <<
//       "' in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (fSegmentMeasuresList.size ()) { // JMI BOFBOF
//     // pad last measure up to to this actual wholes notes
//     fSegmentLastMeasure->
//       casadeBackupByWholeNotesStepLengthInMeasure (
//         inputLineNumber,
//         backupTargetMeasureElementPositionInMeasure);
//   }
// }

void msrSegment::cascadeAppendPaddingNoteToSegment (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& forwardStepLength)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Cascading appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to segment '" <<
      fSegmentAbsoluteNumber <<
      ", segmentDebugNumber: '" <<
      fSegmentDebugNumber <<
      "' in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fSegmentMeasuresList.size ()) { // JMI BOFBOF 0.9.67
    // append a padding note to the segment's last measure
    fSegmentLastMeasure->
      appendPaddingSkipNoteToMeasure ( // JMI why not a rest??? 0.9.67
        inputLineNumber,
        forwardStepLength);
  }

  --gIndenter;
}

void msrSegment::cascadeAppendMultipleMeasureRestToSegment (
  const S_msrMultipleMeasureRest& multipleMeasureRest)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Cascading appending multiple measure rest " << multipleMeasureRest->asString () <<
      " to segment " << asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append multipleMeasureRest to the segment
  fSegmentElementsList.push_back (multipleMeasureRest);

  fCurrentMultipleMeasureRest = multipleMeasureRest;

  fOnGoingMultipleMeasureRest = true;
}

void msrSegment::appendMeasureToSegment (const S_msrMeasure& measure)
{
  mfMeasureNumber
    measureNumber =
      measure->getMeasureNumber ();

  size_t segmentElementsListSize =
    fSegmentMeasuresList.size ();

  mfMeasureNumber
    segmentLastMeasureNumber =
        segmentElementsListSize == 0
          ? mfMeasureNumber ("???")
          : fSegmentLastMeasure->getMeasureNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresBasics ()) {
    std::stringstream ss;

    if (fSegmentMeasuresList.empty ())
      ss <<
        "Appending first measure";
    else
      ss <<
      " Appending new last measure after measure number '" <<
      segmentLastMeasureNumber <<
      '\'';

    ss <<
      ", " <<
      measure->asString () <<
      " to segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"" <<
      ", line " << measure->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   if (fSegmentAbsoluteNumber == 2) abort();

  if (measureNumber == segmentLastMeasureNumber) {
    std::stringstream ss;

    ss <<
      "measure number '" << measureNumber <<
      "' occurs more that once in segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    if (false) {
    msrInternalWarning ( // JMI 0.9.73
//     msrInternalError (
      gServiceRunData->getInputSourceName (),
      measure->getInputLineNumber (),
//       __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
    } else {
//     msrInternalWarning ( // JMI 0.9.73
    msrInternalError (
      gServiceRunData->getInputSourceName (),
      measure->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
    }
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

  if (fOnGoingMultipleMeasureRest) {
    // append measure to the current multiple measure rests
    fCurrentMultipleMeasureRest->
      appendMeasureToMultipleMeasureRest (
        measure);
  }
  else {
    // append measure to the segment
    fSegmentElementsList.push_back (measure);

//     // set the measure full measure whole notes duration JMI 0.9.70
//     measure->
//       setFullMeasureWholeNotesDuration (
//         fetchSegmentUpLinkToStaff ()->
//           getStaffCurrentTimeSignature ()->
//             timeSignatureWholeNotesPerMeasure ());
  }

  // append measure to the segment measures list
  fSegmentMeasuresList.push_back (measure);

  // register measure as the last one in the segment
  setSegmentLastMeasure (
    measure);
}

void msrSegment::prependMeasureToSegment (const S_msrMeasure& measure)
{
  mfMeasureNumber
    measureNumber =
      measure->getMeasureNumber ();

  size_t segmentElementsListSize =
    fSegmentElementsList.size ();

  mfMeasureNumber
     segmentLastMeasureNumber =
        segmentElementsListSize == 0
          ? mfMeasureNumber ("")
          : fSegmentLastMeasure->getMeasureNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresBasics ()) {
    std::stringstream ss;

    ss <<
      "Prepending measure " <<
      measure->asString () <<
      " to segment " << asString ();

    if (segmentElementsListSize == 0) {
      gLog <<
        ", as first measure";
    }

/* JMI 0.9.66
    else
      gLog <<
      ", after measure number '" << segmentLastMeasureNumber << "'";
*/

    gLog <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"," <<
      ", line " << measure->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (measureNumber == segmentLastMeasureNumber) {
    std::stringstream ss;

    ss <<
      "prepending measure number '" << measureNumber <<
      "' occurs more that once in segment " <<
      asString () <<
      " in voice \"" <<
      fSegmentUpLinkToVoice->getVoiceName () <<
      "\"";

    msrInternalError (
// JMI    msrInternalWarning (
      gServiceRunData->getInputSourceName (),
      measure->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  measure->
    setMeasureIsFirstInVoice ();

  // prepend measure to the segment
  fSegmentElementsList.push_front (measure);

  fSegmentMeasuresList.push_front (measure);
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

  fSegmentMeasuresList.front ()-> // JMI ??? 0.9.63
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "\",";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", line " << voiceStaffChange->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  const S_msrNote&  note,
  const mfPositionInMeasure&
                    partCurrentDrawingPositionInMeasure)
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
    appendNoteToMeasureAtPosition (
      note,
      partCurrentDrawingPositionInMeasure);
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
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

  fSegmentMeasuresList.front ()->
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

  fSegmentMeasuresList.front ()->
    prependAfterGraceNotesToMeasure (afterGraceNotes); // JMI
}
*/

// void msrSegment::prependOtherElementToSegment (
//   const S_msrMeasureElement& elem)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   assertSegmentLastMeasureIsNotNull (
//     elem->getInputLineNumber ());
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   fSegmentLastMeasure->
//     prependOtherElementToMeasure (elem);
// }

// void msrSegment::appendOtherElementToSegment (
//   const S_msrMeasureElement& elem)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   assertSegmentLastMeasureIsNotNull (
//     elem->getInputLineNumber ());
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   fSegmentLastMeasure->
//     appendOtherElementToMeasure (elem);
// }

/* JMI
S_msrElement msrSegment::removeLastElementFromSegment (
  const mfInputLineNumber& inputLineNumber)
{
  // this last element can be a note or a tuplet,
  // this method is used when the seconde note of a chord is mest

  if (fSegmentMeasuresList.size ()) {
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

// void msrSegment::removeNoteFromSegment (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrNote& note)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceNotes ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing note " <<
//       note->asString () <<
//       " from segment " <<
//       asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   if (fSegmentMeasuresList.size ()) {
//     fSegmentLastMeasure->
//       removeNoteFromMeasure (
//         inputLineNumber,
//         note);
//   }
//
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "cannot remove note from segment " <<
//       asString () <<
//       "' in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"," <<
//       " since it is empty";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   --gIndenter;
// }

// void msrSegment::removeElementFromSegment (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrElement& element)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceSegments ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing element " <<
//       element->asString () <<
//       " from segment '" <<
//       asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//  if (fSegmentMeasuresList.size ()) {
//     fSegmentLastMeasure->
//       removeElementFromMeasure (
//         inputLineNumber,
//         element);
//   }
//
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "cannot remove note from segment " <<
//       asString () <<
//       "' in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"," <<
//       " since it is empty";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// }

S_msrMeasure msrSegment::fetchLastMeasureFromSegment (
  const mfInputLineNumber& inputLineNumber,
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
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fSegmentMeasuresList.size ()) {
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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

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

// S_msrMeasure msrSegment::removeLastMeasureFromSegment (
//   const mfInputLineNumber& inputLineNumber,
//   const std::string& context)
// {
//   S_msrMeasure result;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing last measure from segment '" <<
//       fSegmentAbsoluteNumber <<
//       "', segmentDebugNumber: '" <<
//       fSegmentDebugNumber <<
//       "' (" << context << ")" <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasuresDetails ()) {
//     fSegmentUpLinkToVoice->
//       displayVoiceRepeatsStackMultipleMeasureRestsMeasureRepeatAndVoice (
//         inputLineNumber,
//         "removeLastMeasureFromSegment() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// //   if (! fSegmentMeasuresList.size ()) {
// //     std::stringstream ss;
// //
// //     ss <<
// //       "cannot remove last measure from segment '" <<
// //       fSegmentAbsoluteNumber <<
// //       ", segmentDebugNumber: '" <<
// //       fSegmentDebugNumber <<
// //       "' in voice \"" <<
// //       fSegmentUpLinkToVoice->getVoiceName () <<
// //       "\"" <<
// //       ", since it is empty";
// //
// //     msrInternalError (
// //       gServiceRunData->getInputSourceName (),
// //       inputLineNumber,
// //       __FILE__, mfInputLineNumber (__LINE__),
// //       ss.str ());
// //   }
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   if (! fSegmentLastMeasure) {
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
//       ", since that segment does not contain any";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // we've got the result
//   result = fSegmentLastMeasure;
//
//   // remove it from segment elements list too ??? JMI 0.9.63 JMI 0.9.67
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasuresDetails ()) {
//     fSegmentUpLinkToVoice->
//       displayVoiceRepeatsStackMultipleMeasureRestsMeasureRepeatAndVoice (
//         inputLineNumber,
//         "removeLastMeasureFromSegment() 2");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   S_msrSegmentElement
//     segmentElementsListLastElement =
//       fSegmentElementsList.back ();
//
//   if (segmentElementsListLastElement == fSegmentLastMeasure) {
//     // remove it from the elements list
//     fSegmentElementsList.pop_back ();
//   }
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "attempt at removing the last measure of segment " <<
//       this->asString () <<
//       " which is not at the end of fSegmentElementsList" <<
//       ", in voice \"" <<
//       fSegmentUpLinkToVoice->getVoiceName () <<
//       "\"" <<
//       "', line " << inputLineNumber;
//
//     gLog <<
//       std::endl << std::endl <<
//       ss.str () <<
//       std::endl << std::endl;
//
//     gLog << "THIS:" << std::endl;
//     gLog << "----------------------------" << std::endl;
//     ++gIndenter;
//     gLog << std::hex << std::showbase << this << std::dec; // JMI 0.9.69
//     --gIndenter;
//
//     gLog << std::endl;
//
//     gLog << "fSegmentLastMeasure:" << std::endl;
//     gLog << "----------------------------" << std::endl;
//     ++gIndenter;
//     gLog << fSegmentLastMeasure;
//     --gIndenter;
//
//     gLog << std::endl;
//
//     gLog << "segmentElementsListLastElement:" << std::endl;
//     gLog << "----------------------------" << std::endl;
//     ++gIndenter;
//     gLog << segmentElementsListLastElement;
//     --gIndenter;
//
// //     abort (); // JMI 0.9.67 HARMFUL
//
//     msrInternalError ( // JMI 0.9.64 ???
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "The removed measure contains:" <<
//       std::endl;
//
//     ++gIndenter;
//
//     gLog <<
//       result->asString () <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // remove the last measure from the segments measures list
//   fSegmentMeasuresList.pop_back ();
//
//   // don't forget about fSegmentLastMeasure now,
//   // since it may be used and/or re-appended soon JMI 0.9.63
// //   setSegmentLastMeasure ( // JMI 0.9.67
// //     nullptr);
//
//   return result;
// }

void msrSegment::finalizeAllTheMeasuresOfSegment ( // superflous JMI ???
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Finalizing all the measures of segment '" <<
      fSegmentAbsoluteNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrMeasure measure : fSegmentMeasuresList) {
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
      "% ==> msrSegment::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
            "% ==> Launching msrSegment::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
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
      "% ==> msrSegment::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
            "% ==> Launching msrSegment::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
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
      "% ==> msrSegment::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      "% <== msrSegment::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
    "[Segment" <<
    ", fSegmentAbsoluteNumber: " <<
    fSegmentAbsoluteNumber <<
    ", fSegmentDebugNumber: " <<
    fSegmentDebugNumber <<
    " in voice \"" <<
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
      std::list <S_msrSegmentElement>::const_iterator
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
  const mfInputLineNumber& inputLineNumber,
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

void msrSegment::print (std::ostream& os) const
{
  os <<
    "[Segment" <<
    ", fSegmentAbsoluteNumber: " <<
    fSegmentAbsoluteNumber <<
    ", fSegmentDebugNumber: " <<
    fSegmentDebugNumber <<
    ", " <<
    mfSingularOrPlural (
      fSegmentElementsList.size (), "measure", "measures") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 20;

  if (! fSegmentElementsList.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fSegmentElementsList" << ": " << "[EMPTY]" <<
      std::endl;
  }

  else {
    os << std::endl;

    std::list <S_msrSegmentElement>::const_iterator
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

void msrSegment::printFull (std::ostream& os) const
{
  os <<
    "[Segment FULL" <<
    ", fSegmentAbsoluteNumber: " <<
    fSegmentAbsoluteNumber <<
    ", fSegmentDebugNumber: " <<
    fSegmentDebugNumber <<
    ", " <<
    mfSingularOrPlural (
      fSegmentElementsList.size (), "measure", "measures") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 20;

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
    fSegmentShortestNoteWholeNotes <<
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
    fSegmentFirstMeasure->printFull (os);
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
    fSegmentLastMeasure->printFull (os);
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

    std::list <S_msrSegmentElement>::const_iterator
      iBegin = fSegmentElementsList.begin (),
      iEnd   = fSegmentElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrSegmentElement
        segmentElement = (*i);

      segmentElement->printFull (os);

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
