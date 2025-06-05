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

#include "msrMultipleMeasureRests.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrMultipleMeasureRest msrMultipleMeasureRest::create (
  const mfInputLineNumber& inputLineNumber,
  int               measuresNumber,
  int               slashesNumber,
  msrUseSymbolsKind useSymbolsKind)
{
  msrMultipleMeasureRest* obj =
    new msrMultipleMeasureRest (
      inputLineNumber,
      measuresNumber,
      slashesNumber,
      useSymbolsKind);
  assert (obj != nullptr);
  return obj;
}

msrMultipleMeasureRest::msrMultipleMeasureRest (
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

msrMultipleMeasureRest::~msrMultipleMeasureRest ()
{}

S_msrMultipleMeasureRest msrMultipleMeasureRest::createMultipleMeasureRestNewbornClone (
  const S_msrSegment& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of multiple measure rests '" <<
      asString () <<
      "'";

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
    "containingVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrMultipleMeasureRest
    newbornClone =
//       msrMultipleMeasureRest::create (
//         fInputLineNumber,
//         fMeasuresNumber,
//         containingVoice);
      msrMultipleMeasureRest::create (
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

mfWholeNotes msrMultipleMeasureRest::fetchMultipleMeasureRestMeasureSoundingNotes () const
{
  mfWholeNotes result;

  for (S_msrMeasure measure : fMeasuresList) {
    result +=
      measure->getFullMeasureWholeNotesDuration ();
  } // for

  return result;
}

void msrMultipleMeasureRest::setNextMeasureNumber (
  const std::string& nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Setting multiple measure rests next measure number to '" <<
      "' " <<
      nextMeasureNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNextMeasureNumber =
    nextMeasureNumber;
}

void msrMultipleMeasureRest::setLastMeasurePuristMeasureNumber (
  const mfInputLineNumber& inputLineNumber,
  int puristMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Setting multiple measure rests last measure purist number to '" <<
      "' " <<
      puristMeasureNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fLastMeasurePuristNumber =
    puristMeasureNumber;
}

void msrMultipleMeasureRest::appendMeasureElementToSegmentElement (
  const S_msrMeasureElement& elem)
{
  std::stringstream ss;

  ss <<
    "cannot append measure element " <<
    elem->asShortString () <<
    " to multiple measure rests " <<
    asShortString ();

  msrInternalError (
    gServiceRunData->getInputSourceName (),
    fInputLineNumber,
    __FILE__, __LINE__,
    ss.str ());
}

void msrMultipleMeasureRest::appendMeasureToMultipleMeasureRest (
  const S_msrMeasure& measure)
{
//   fMultipleMeasureRestContents->
//     getMultipleMeasureRestContentsSegment ()->
//       appendMeasureToSegment (
//         measureClone);
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending measure "<<
      measure->asShortString () <<
      " to multiple measure rests " <<
      asShortString () <<
      "', line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasuresList.push_back (measure);

  // it measure the first one in the segment?
  if (! fMultipleMeasureRestUpLinkToSegment->getSegmentFirstMeasure ()) {
    fMultipleMeasureRestUpLinkToSegment->
      setSegmentFirstMeasure (measure);
  }

  // is measure the first one it the voice?
  // this is necessary for voice clones,
  // which don't go down the part-staff-voice-segment hierarchy
  S_msrVoice
    voice =
      fMultipleMeasureRestUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  if (! voice->getVoiceFirstMeasure ()) {
    // yes, register it as such
    voice->
      setVoiceFirstMeasure (measure);

    measure->
      setMeasureIsFirstInVoice ();
  }

  // register measure as the last one in the segment
  fMultipleMeasureRestUpLinkToSegment->
    setSegmentLastMeasure (
      measure);
}

void msrMultipleMeasureRest::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultipleMeasureRest::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMultipleMeasureRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleMeasureRest>*> (v)) {
        S_msrMultipleMeasureRest elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMultipleMeasureRest::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrMultipleMeasureRest::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultipleMeasureRest::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMultipleMeasureRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleMeasureRest>*> (v)) {
        S_msrMultipleMeasureRest elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMultipleMeasureRest::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrMultipleMeasureRest::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultipleMeasureRest::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // JMI   if (! inhibitMultipleMeasureRestsBrowsing) { // JMI 0.9.67
  // browse the multiple measure rests measures
  for (S_msrMeasure measure : fMeasuresList) {
    // browse the measure
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(measure));
  } // for

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrMultipleMeasureRest::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrMultipleMeasureRest::asString () const
{
  std::stringstream ss;

  ss <<
    "[MultipleMeasureRest" <<
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
    ", fMultipleMeasureRestUpLinkToSegment" << ": ";

  if (fMultipleMeasureRestUpLinkToSegment) {
    ss <<
      "fMultipleMeasureRestUpLinkToSegment->asString ()"; // JMI 0.9.67
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

    ", multipleMeasureRestMeasureSoundingNotes: " <<
    fetchMultipleMeasureRestMeasureSoundingNotes () <<
    ", " <<

    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrMultipleMeasureRest::displayMultipleMeasureRest (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  gLog <<
    std::endl <<
    "*********>> Multiple measure rest " << context << " \"" <<
 // JMI   getVoiceName () <<
    "\"" <<
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

void msrMultipleMeasureRest::print (std::ostream& os) const
{
  os <<
    "[MultipleMeasureRest" <<
    ", line " << fInputLineNumber <<
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
    "fetchMultipleMeasureRestMeasureSoundingNotes" << ": " <<
    fetchMultipleMeasureRestMeasureSoundingNotes () <<
    std::endl;

  // print the segment upLink
  os << std::left <<
    "fMultipleMeasureRestUpLinkToSegment" << ": ";

  if (fMultipleMeasureRestUpLinkToSegment) {
    os <<
      "\"" <<
      fMultipleMeasureRestUpLinkToSegment->asString () <<
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

std::ostream& operator << (std::ostream& os, const S_msrMultipleMeasureRest& elt)
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
