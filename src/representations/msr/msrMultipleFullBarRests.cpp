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

#include "msrMultipleFullBarRests.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrMultipleFullBarRests msrMultipleFullBarRests::create (
  int                 inputLineNumber,
  int                 multipleFullBarRestsNumber,
  const S_msrSegment& upLinkToSegment)
{
  msrMultipleFullBarRests* obj =
    new msrMultipleFullBarRests (
      inputLineNumber,
      multipleFullBarRestsNumber,
      upLinkToSegment);
  assert (obj != nullptr);
  return obj;
}

S_msrMultipleFullBarRests msrMultipleFullBarRests::create (
  int                 inputLineNumber,
  const S_msrMeasure& restMeasureClone,
  const S_msrSegment& upLinkToSegment)
{
  msrMultipleFullBarRests* obj =
    new msrMultipleFullBarRests (
      inputLineNumber,
      restMeasureClone,
      upLinkToSegment);
  assert (obj != nullptr);
  return obj;
}

msrMultipleFullBarRests::msrMultipleFullBarRests (
  int                 inputLineNumber,
  int                 multipleFullBarRestsNumber,
  const S_msrSegment& upLinkToSegment)
    : msrSegmentElement (inputLineNumber)
{
  fMultipleFullBarRestsUpLinkToSegment = upLinkToSegment;

  fMultipleFullBarRestsNumber = multipleFullBarRestsNumber;

  fMultipleFullBarRestsLastMeasurePuristNumber = -1;
}

msrMultipleFullBarRests::msrMultipleFullBarRests (
  int                 inputLineNumber,
  const S_msrMeasure& restMeasureClone,
  const S_msrSegment& upLinkToSegment)
    : msrSegmentElement (inputLineNumber)
{
  fMultipleFullBarRestsUpLinkToSegment = upLinkToSegment;

  fMultipleFullBarRestsNumber = 1; // will evolve JMI v0.9.64

  fMultipleFullBarRestsLastMeasurePuristNumber = -1;
}

msrMultipleFullBarRests::~msrMultipleFullBarRests ()
{}

S_msrMultipleFullBarRests msrMultipleFullBarRests::createMultipleFullBarRestsNewbornClone (
  const S_msrSegment& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of multiple full-bar rests '" <<
      asString () <<
      "'" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrMultipleFullBarRests
    newbornClone =
      msrMultipleFullBarRests::create (
        fInputLineNumber,
        fMultipleFullBarRestsNumber,
        containingVoice);

/* JMI v0.9.63
  newbornClone->fMultipleFullBarRestsNextMeasureNumber =
    fMultipleFullBarRestsNextMeasureNumber;
    */

  return newbornClone;
}

msrWholeNotes msrMultipleFullBarRests::fetchMultipleFullBarRestsMeasureSoundingNotes () const
{
  msrWholeNotes result;

  for (S_msrMeasure measure : fFullBarRestsMeasuresList) {
    result +=
      measure->getFullMeasureWholeNotes ();
  } // for

  return result;
}

void msrMultipleFullBarRests::setMultipleFullBarRestsNextMeasureNumber (
  const std::string& nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    std::stringstream ss;

    ss <<
      "Setting multiple full-bar rests next measure number to '" <<
      "' " <<
      nextMeasureNumber <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMultipleFullBarRestsNextMeasureNumber =
    nextMeasureNumber;
}

void msrMultipleFullBarRests::setMultipleFullBarRestsLastMeasurePuristMeasureNumber (
  int inputLineNumber)
{
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fMultipleFullBarRestsContents != nullptr,
//     "fMultipleFullBarRestsContents is null");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   S_msrSegment
//     multipleFullBarRestsContentsSegment =
//       fMultipleFullBarRestsContents->
//         getMultipleFullBarRestsContentsSegment ();
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     multipleFullBarRestsContentsSegment != nullptr,
//     "multipleFullBarRestsContentsSegment is null");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

  // get multiple full-bar rests contents segment measure elements list
//   const std::list<S_msrMeasure>&
//     contentsSegmentElementsList =
//       multipleFullBarRestsContentsSegment->
//         getSegmentElementsList ();

  // get multiple full-bar rests contents last measure's purist number
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
//       "cannot get multiple full-bar rests contents last measure purist number" <<
//       " because its measure elements list is empty" <<
//       " in voice clone '" <<
//       asShortString () <<
//       "' ";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, __LINE__,
//       ss.str ());
//   }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    std::stringstream ss;

    ss <<
      "Setting multiple full-bar rests last measure purist number to '" <<
      "' " <<
      lastMeasuresPuristNumber <<
      "', line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMultipleFullBarRestsLastMeasurePuristNumber =
    lastMeasuresPuristNumber;
}

void msrMultipleFullBarRests:: appendMeasureElementToSegmentElement (
  const S_msrMeasureElement& elem)
{
  std::stringstream ss;

  ss <<
    "cannot append measure element " <<
    elem->asShortString () <<
    " to multiple full-bar rests " <<
    asShortString ();

  msrInternalError (
    gServiceRunData->getInputSourceName (),
    fInputLineNumber,
    __FILE__, __LINE__,
    ss.str ());
}

void msrMultipleFullBarRests::appendMeasureToMultipleFullBarRests (
  const S_msrMeasure& measure)
{
//   fMultipleFullBarRestsContents->
//     getMultipleFullBarRestsContentsSegment ()->
//       appendMeasureToSegment (
//         measureClone);
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    std::stringstream ss;

    ss <<
      "Appending measure "<<
      measure->asShortString () <<
      " to multiple full-bar rests " <<
      asShortString () <<
      "', line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  fFullBarRestsMeasuresList.push_back (measure);

  // it measure the first one in the segment?
  if (! fMultipleFullBarRestsUpLinkToSegment->getSegmentFirstMeasure ()) {
    fMultipleFullBarRestsUpLinkToSegment->
      setSegmentFirstMeasure (measure);
  }

  // is measure the first one it the voice?
  // this is necessary for voice clones,
  // which don't go down the part-staff-voice-segment hierarchy
  S_msrVoice
    voice =
      fMultipleFullBarRestsUpLinkToSegment->
        getSegmentUpLinkToVoice ();

  if (! voice->getVoiceFirstMeasure ()) {
    // yes, register it as such
    voice->
      setVoiceFirstMeasure (measure);

    measure->
      setMeasureIsFirstInVoice ();
  }

  // register measure as the last one in the segment
  fMultipleFullBarRestsUpLinkToSegment->
    setSegmentLastMeasure (
      measure);
}

void msrMultipleFullBarRests::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultipleFullBarRests::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMultipleFullBarRests>*
    p =
      dynamic_cast<visitor<S_msrMultipleFullBarRests>*> (v)) {
        S_msrMultipleFullBarRests elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMultipleFullBarRests::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrMultipleFullBarRests::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultipleFullBarRests::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMultipleFullBarRests>*
    p =
      dynamic_cast<visitor<S_msrMultipleFullBarRests>*> (v)) {
        S_msrMultipleFullBarRests elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMultipleFullBarRests::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrMultipleFullBarRests::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMultipleFullBarRests::browseData ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

 // JMI   if (! inhibitMultipleFullBarRestsBrowsing) { // JMI v0.9.67
  // browse the multiple full-bar rests measures
  for (S_msrMeasure measure : fFullBarRestsMeasuresList) {
    // browse the measure
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(measure));
  } // for

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrMultipleFullBarRests::browseData ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrMultipleFullBarRests::asString () const
{
  std::stringstream ss;

  ss <<
    "[MultipleFullBarRests" <<
    ", for " <<
    mfSingularOrPlural (
      fMultipleFullBarRestsNumber,
        "full-bar rest",
        "full-bar rests");

  ss <<
    ", fMultipleFullBarRestsUpLinkToSegment" << ": ";

  if (fMultipleFullBarRestsUpLinkToSegment) {
    ss <<
      "fMultipleFullBarRestsUpLinkToSegment->asString ()"; // JMI v0.9.67
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", fFullBarRestsMeasuresList.size(): " <<
    fFullBarRestsMeasuresList.size () <<

    ", fMultipleFullBarRestsLastMeasurePuristNumber: '" <<
    fMultipleFullBarRestsLastMeasurePuristNumber <<
    "'" <<

    ", multipleFullBarRestsNextMeasureNumber: '" <<
    fMultipleFullBarRestsNextMeasureNumber <<
    "'" <<

    ", multipleFullBarRestsMeasureSoundingNotes: " <<
    fetchMultipleFullBarRestsMeasureSoundingNotes () <<
    ", " <<

    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrMultipleFullBarRests::displayMultipleFullBarRests (
  int           inputLineNumber,
  const std::string& context)
{
  gLog <<
    std::endl <<
    "*********>> Full-bar rests " << context << " \"" <<
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

void msrMultipleFullBarRests::print (std::ostream& os) const
{
  os <<
    "[MultipleFullBarRests" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 41;

  os <<
    "fMultipleFullBarRestsUpLinkToSegment" << ": ";

  if (fMultipleFullBarRestsUpLinkToSegment) {
    os <<
      "\"" <<
      fMultipleFullBarRestsUpLinkToSegment->asString () <<
      "\"";
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMultipleFullBarRestsNumber" << ": " <<
    fMultipleFullBarRestsNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMultipleFullBarRestsLastMeasurePuristNumber" << ": " <<
    fMultipleFullBarRestsLastMeasurePuristNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMultipleFullBarRestsNextMeasureNumber" << " : '" <<
    fMultipleFullBarRestsNextMeasureNumber <<
    "'" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fetchMultipleFullBarRestsMeasureSoundingNotes" << ": " <<
    fetchMultipleFullBarRestsMeasureSoundingNotes () <<
    std::endl;

  // print the segment upLink
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMultipleFullBarRestsUpLinkToSegment" << ": ";
  if (fMultipleFullBarRestsUpLinkToSegment) {
    os << std::endl;
    ++gIndenter;

    os << "fMultipleFullBarRestsUpLinkToSegment" << std::endl; // JMI v0.9.67 v0.9.66

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os << std::endl;

  // print the full-bar rests measures list
  os << std::left <<
    std::setw (fieldWidth) <<
    "fFullBarRestsMeasuresList" << ": ";

  if (fFullBarRestsMeasuresList.size ()) {
    os << std::endl;
    ++gIndenter;

    for (S_msrMeasure measure : fFullBarRestsMeasuresList) {
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

std::ostream& operator << (std::ostream& os, const S_msrMultipleFullBarRests& elt)
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
