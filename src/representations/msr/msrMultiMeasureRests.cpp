/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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
  int                 inputLineNumber,
  int                 measureRestsNumber,
  const S_msrSegment& upLinkToSegment)
{
  msrMultiMeasureRest* obj =
    new msrMultiMeasureRest (
      inputLineNumber,
      measureRestsNumber,
      upLinkToSegment);
  assert (obj != nullptr);
  return obj;
}

// S_msrMultiMeasureRest msrMultiMeasureRest::create (
//   int                 inputLineNumber,
//   const S_msrMeasure& measureRestClone,
//   const S_msrSegment& upLinkToSegment)
// {
//   msrMultiMeasureRest* obj =
//     new msrMultiMeasureRest (
//       inputLineNumber,
//       measureRestClone,
//       upLinkToSegment);
//   assert (obj != nullptr);
//   return obj;
// }

msrMultiMeasureRest::msrMultiMeasureRest (
  int                 inputLineNumber,
  int                 measureRestsNumber,
  const S_msrSegment& upLinkToSegment)
    : msrSegmentElement (inputLineNumber)
{
  fMultiMeasureRestUpLinkToSegment = upLinkToSegment;

  fMeasureRestsNumber = measureRestsNumber;

  fLastMeasureRestPuristNumber = -1;
}

// msrMultiMeasureRest::msrMultiMeasureRest (
//   int                 inputLineNumber,
//   const S_msrMeasure& measureRestClone,
//   const S_msrSegment& upLinkToSegment)
//     : msrSegmentElement (inputLineNumber)
// {
//   fMultiMeasureRestUpLinkToSegment = upLinkToSegment;
//
//   fMeasureRestsList.push_back (measureRestClone);
//   fMeasureRestsNumber = 1; // will evolve JMI v0.9.64
//
//   fLastMeasureRestPuristNumber = -1;
// }

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

  S_msrMultiMeasureRest
    newbornClone =
      msrMultiMeasureRest::create (
        fInputStartLineNumber,
        fMeasureRestsNumber,
        containingVoice);

/* JMI v0.9.63
  newbornClone->fNextMeasureNumber =
    fNextMeasureNumber;
    */

  return newbornClone;
}

msrWholeNotes msrMultiMeasureRest::fetchMultiMeasureRestMeasureSoundingNotes () const
{
  msrWholeNotes result;

  for (S_msrMeasure measure : fMeasureRestsList) {
    result +=
      measure->getFullMeasureWholeNotesDuration ();
  } // for

  return result;
}

void msrMultiMeasureRest::setNextMeasureNumber (
  const std::string& nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Setting multi-measure rests next measure number to '" <<
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

void msrMultiMeasureRest::setMultiMeasureRestLastMeasurePuristMeasureNumber (
  int inputLineNumber)
{
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fMultiMeasureRestContents != nullptr,
//     "fMultiMeasureRestContents is null");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   S_msrSegment
//     multiMeasureRestContentsSegment =
//       fMultiMeasureRestContents->
//         getMultiMeasureRestContentsSegment ();
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     multiMeasureRestContentsSegment != nullptr,
//     "multiMeasureRestContentsSegment is null");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

  // get multi-measure rests contents segment measure elements list
//   const std::list <S_msrMeasure>&
//     contentsSegmentElementsList =
//       multiMeasureRestContentsSegment->
//         getSegmentElementsList ();

  // get multi-measure rests contents last measure's purist number
  int lastMeasuresPuristNumber = -1;

//   if (contentsSegmentElementsList.size ()) {
//     lastMeasuresPuristNumber =
//       contentsSegmentElementsList.back ()->
//         getMeasurePuristNumber ();
//   }
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "cannot get multi-measure rests contents last measure purist number" <<
//       " because its measure elements list is empty" <<
//       " in voice clone '" <<
//       asShortString () <<
//       "' ";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputStartLineNumber,
//       __FILE__, __LINE__,
//       ss.str ());
//   }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Setting multi-measure rests last measure purist number to '" <<
      "' " <<
      lastMeasuresPuristNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fLastMeasureRestPuristNumber =
    lastMeasuresPuristNumber;
}

void msrMultiMeasureRest::appendMeasureElementToSegmentElement (
  const S_msrMeasureElement& elem)
{
  std::stringstream ss;

  ss <<
    "cannot append measure element " <<
    elem->asShortString () <<
    " to multi-measure rests " <<
    asShortString ();

  msrInternalError (
    gServiceRunData->getInputSourceName (),
    fInputStartLineNumber,
    __FILE__, __LINE__,
    ss.str ());
}

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
      "', line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureRestsList.push_back (measure);

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
    // yes, register it as such
    voice->
      setVoiceFirstMeasure (measure);

    measure->
      setMeasureIsFirstInVoice ();
  }

  // register measure as the last one in the segment
  fMultiMeasureRestUpLinkToSegment->
    setSegmentLastMeasure (
      measure);
}

void msrMultiMeasureRest::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultiMeasureRest::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // JMI   if (! inhibitMultiMeasureRestsBrowsing) { // JMI v0.9.67
  // browse the multi-measure rests measures
  for (S_msrMeasure measure : fMeasureRestsList) {
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
      __FILE__, __LINE__,
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
      fMeasureRestsNumber,
        "measure rest",
        "measure rests");

  ss <<
    ", fMultiMeasureRestUpLinkToSegment" << ": ";

  if (fMultiMeasureRestUpLinkToSegment) {
    ss <<
      "fMultiMeasureRestUpLinkToSegment->asString ()"; // JMI v0.9.67
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", fMeasureRestsList.size(): " <<
    fMeasureRestsList.size () <<

    ", fLastMeasureRestPuristNumber: '" <<
    fLastMeasureRestPuristNumber <<
    '\'' <<

    ", fNextMeasureNumber: '" <<
    fNextMeasureNumber <<
    '\'' <<

    ", multiMeasureRestMeasureSoundingNotes: " <<
    fetchMultiMeasureRestMeasureSoundingNotes () <<
    ", " <<

    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrMultiMeasureRest::displayMultiMeasureRest (
  int                inputLineNumber,
  const std::string& context)
{
  gLog <<
    std::endl <<
    "*********>> Multi-measure rest " << context << " \"" <<
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

void msrMultiMeasureRest::print (std::ostream& os) const
{
  os <<
    "[MultiMeasureRest" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 41;

  os <<
    "fMultiMeasureRestUpLinkToSegment" << ": ";

  if (fMultiMeasureRestUpLinkToSegment) {
    os <<
      "\"" <<
      fMultiMeasureRestUpLinkToSegment->asString () <<
      "\"";
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureRestsNumber" << ": " <<
    fMeasureRestsNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fLastMeasureRestPuristNumber" << ": " <<
    fLastMeasureRestPuristNumber <<
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
    std::setw (fieldWidth) <<
    "fMultiMeasureRestUpLinkToSegment" << ": ";
  if (fMultiMeasureRestUpLinkToSegment) {
    os << std::endl;
    ++gIndenter;

    os << "fMultiMeasureRestUpLinkToSegment" << std::endl; // JMI v0.9.67 v0.9.66

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os << std::endl;

  // print the measure rests measures list
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureRestsList" << ": ";

  if (fMeasureRestsList.size ()) {
    os << std::endl;
    ++gIndenter;

    for (S_msrMeasure measure : fMeasureRestsList) {
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


}
