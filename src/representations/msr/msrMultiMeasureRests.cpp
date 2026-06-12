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

#include "msrMultiMeasureRests.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrMultiMeasureRest msrMultiMeasureRest::create (
  const mfInputLineNumber& inputLineNumber,
  int               measuresNumber,
  int               slashesNumber,
  msrUseSymbolsKind useSymbolsKind)
{
  msrMultiMeasureRest* obj =
    new msrMultiMeasureRest (
      inputLineNumber,
      measuresNumber,
      slashesNumber,
      useSymbolsKind);
  assert (obj != nullptr);
  return obj;
}

msrMultiMeasureRest::msrMultiMeasureRest (
  const mfInputLineNumber& inputLineNumber,
  int               measuresNumber,
  int               slashesNumber,
  msrUseSymbolsKind useSymbolsKind)
    : msrSegmentElement (inputLineNumber)
{
  fMeasuresNumber = measuresNumber;
  fSlashesNumber = slashesNumber;
  fUseSymbolsKind = useSymbolsKind;

  fLastMeasurePuristNumber = -1;
}

msrMultiMeasureRest::~msrMultiMeasureRest ()
{}

S_msrMultiMeasureRest msrMultiMeasureRest::createMultiMeasureRestNewbornClone (
  const S_msrSegment& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of multi-measure rests '" <<
      asString () <<
      "'";

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

  S_msrMultiMeasureRest
    newbornClone =
//       msrMultiMeasureRest::create (
//         fInputLineNumber,
//         fMeasuresNumber,
//         containingVoice);
      msrMultiMeasureRest::create (
        fInputLineNumber,
        fMeasuresNumber,
        fSlashesNumber,
        fUseSymbolsKind);

/* JMI 0.9.63
  newbornClone->fNextMeasureNumber =
    fNextMeasureNumber;
    */

  return newbornClone;
}

mfWholeNotes msrMultiMeasureRest::fetchMultiMeasureRestMeasureSoundingNotes () const
{
  mfWholeNotes result;

  for (S_msrMeasure measure : fMeasuresList) {
    result +=
      measure->getMeasureNominalWholeNotesDuration ();
  } // for

  return result;
}

void msrMultiMeasureRest::setNextMeasureNumber (
  const mfMeasureNumber& nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Setting multi-measure rests next measure number to '" <<
      "' " <<
      nextMeasureNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNextMeasureNumber =
    nextMeasureNumber;
}

void msrMultiMeasureRest::setLastMeasurePuristMeasureNumber (
  const mfInputLineNumber& inputLineNumber,
  int puristMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Setting multi-measure rests last measure purist number to '" <<
      "' " <<
      puristMeasureNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fLastMeasurePuristNumber =
    puristMeasureNumber;
}

// void msrMultiMeasureRest::appendMeasureElementToSegmentElement (
//   const S_msrMeasureElement& elem)
// {
//   std::stringstream ss;
//
//   ss <<
//     "cannot append measure element " <<
//     elem->asShortString () <<
//     " to multi-measure rests " <<
//     asShortString ();
//
//   msrInternalError (
//     gServiceRunData->getInputSourceName (),
//     fInputLineNumber,
//     __FILE__, mfInputLineNumber (__LINE__),
//     ss.str ());
// }

void msrMultiMeasureRest::appendMeasureToMultiMeasureRest (
  const S_msrMeasure& measure)
{
//   fMultiMeasureRestContents->
//     getMultiMeasureRestContentsSegment ()->
//       appendMeasureToSegment (
//         measureClone);
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending measure "<<
      measure->asShortString () <<
      " to multi-measure rests " <<
      asShortString () <<
      "', line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasuresList.push_back (measure);

  // it measure the first one in the segment?
  if (! fMultiMeasureRestUpLinkToSegment->getSegmentFirstMeasure ()) {
    fMultiMeasureRestUpLinkToSegment->
      setSegmentFirstMeasure (measure);
  }

  // is measure the first one it the voice?
  // this is necessary for voice clones,
  // which don't go down the part-staff-voice-segment hierarchy
  S_msrVoice
    voice =
      fMultiMeasureRestUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  if (! voice->getVoiceFirstMeasure ()) {
    // YES, register it as such
    voice->
      setVoiceFirstMeasure (measure);

    measure->
      setMeasureIsFirstInVoice ();
  }

  // register measure as the last one in the segment
  fMultiMeasureRestUpLinkToSegment->
    setSegmentLastMeasure (measure);
}

void msrMultiMeasureRest::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultiMeasureRest::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMultiMeasureRest>*
    p =
      dynamic_cast<visitor<S_msrMultiMeasureRest>*> (v)) {
        S_msrMultiMeasureRest elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMultiMeasureRest::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrMultiMeasureRest::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultiMeasureRest::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMultiMeasureRest>*
    p =
      dynamic_cast<visitor<S_msrMultiMeasureRest>*> (v)) {
        S_msrMultiMeasureRest elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMultiMeasureRest::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrMultiMeasureRest::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultiMeasureRest::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // JMI   if (! inhibitMultiMeasureRestsBrowsing) { // JMI 0.9.67
  // browse the multi-measure rests measures
  for (S_msrMeasure measure : fMeasuresList) {
    // browse the measure
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(measure));
  } // for

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrMultiMeasureRest::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrMultiMeasureRest::asString () const
{
  std::stringstream ss;

  ss <<
    "[MultiMeasureRest" <<
    ", for " <<
    mfSingularOrPlural (
      fMeasuresNumber,
        "measure rest",
        "measure rests");

  ss <<
    ", fSlashesNumber: " <<
    fSlashesNumber <<
    ", fUseSymbolsKind: " <<
    fUseSymbolsKind;

  ss <<
    ", fMultiMeasureRestUpLinkToSegment" << ": ";

  if (fMultiMeasureRestUpLinkToSegment) {
    ss <<
      fMultiMeasureRestUpLinkToSegment->asString (); // JMI 0.9.67
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", fMeasuresList.size(): " <<
    fMeasuresList.size () <<

    ", fLastMeasurePuristNumber: '" <<
    fLastMeasurePuristNumber <<
    '\'' <<

    ", fNextMeasureNumber: '" <<
    fNextMeasureNumber <<
    '\'' <<

    ", multiMeasureRestMeasureSoundingNotes: " <<
    fetchMultiMeasureRestMeasureSoundingNotes () <<
    ", " <<

    ", " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrMultiMeasureRest::displayMultiMeasureRest (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  gLog <<
    std::endl <<
    "*********>> Multi-measure rest " << context << " \"" <<
 // JMI   getVoiceName () <<
    "\"" <<
    ", " << inputLineNumber <<
    " contains:" <<
    std::endl;

  ++gIndenter;
  print (gLog);
  --gIndenter;

  gLog <<
    " <<*********" <<
    std::endl << std::endl;
}

void msrMultiMeasureRest::print (std::ostream& os) const
{
  os <<
    "[MultiMeasureRest" <<
    ", " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 41;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasuresNumber" << ": " <<
    fMeasuresNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSlashesNumber" << ": " <<
    fSlashesNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fUseSymbolsKind" << ": " <<
    fUseSymbolsKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fLastMeasurePuristNumber" << ": " <<
    fLastMeasurePuristNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNextMeasureNumber" << " : '" <<
    fNextMeasureNumber <<
    '\'' <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fetchMultiMeasureRestMeasureSoundingNotes" << ": " <<
    fetchMultiMeasureRestMeasureSoundingNotes () <<
    std::endl;

  // print the segment upLink
  os << std::left <<
    "fMultiMeasureRestUpLinkToSegment" << ": ";

  if (fMultiMeasureRestUpLinkToSegment) {
    os <<
      fMultiMeasureRestUpLinkToSegment->asString () <<
      "\"";
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  // print the measure rests measures list
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasuresList" << ": ";

  if (fMeasuresList.size ()) {
    os << std::endl;
    ++gIndenter;

    for (S_msrMeasure measure : fMeasuresList) {
      // print the measure
      os << measure;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << ']' << std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrMultiMeasureRest& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::string fetchMultiMeasureRestAsShortString (
  const S_msrMultiMeasureRest& multiMeasureRest)
{
  std::string result;

  if (multiMeasureRest) {
    result =
      multiMeasureRest->asString ();
  }
  else {
    result = "\"** MULTIPLE_MEASURE_REST IS NULL **\"";
  }

  return result;
}

std::string fetchMultiMeasureRestAsString (
  const S_msrMultiMeasureRest& multiMeasureRest)
{
  std::string result;

  if (multiMeasureRest) {
    result =
      multiMeasureRest->asShortString ();
  }
  else {
    result = "\"** MULTIPLE_MEASURE_REST IS NULL **\"";
  }

  return result;
}


}
