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

#include "msrMultipleFullBarRests.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrMultipleFullBarRests msrMultipleFullBarRests::create (
  int              inputLineNumber,
  int              multipleFullBarRestsNumber,
  const S_msrSegment&     upLinkToSegment)
{
  msrMultipleFullBarRests* o =
    new msrMultipleFullBarRests (
      inputLineNumber,
      multipleFullBarRestsNumber,
      upLinkToSegment);
  assert (o != nullptr);
  return o;
}

S_msrMultipleFullBarRests msrMultipleFullBarRests::create (
  int          inputLineNumber,
  const S_msrMeasure& restMeasureClone,
  const S_msrSegment& upLinkToSegment)
{
  msrMultipleFullBarRests* o =
    new msrMultipleFullBarRests (
      inputLineNumber,
      restMeasureClone,
      upLinkToSegment);
  assert (o != nullptr);
  return o;
}

msrMultipleFullBarRests::msrMultipleFullBarRests (
  int             inputLineNumber,
  int             multipleFullBarRestsNumber,
  const S_msrSegment&    upLinkToSegment)
    : msrSegmentElement (inputLineNumber)
{
  fMultipleFullBarRestsUpLinkToSegment = upLinkToSegment;

  fMultipleFullBarRestsNumber = multipleFullBarRestsNumber;

  fMultipleFullBarRestsLastMeasurePuristNumber = -1;
}

msrMultipleFullBarRests::msrMultipleFullBarRests (
  int          inputLineNumber,
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Creating a newborn clone of multiple full-bar rests '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

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

Rational msrMultipleFullBarRests::fetchMultipleFullBarRestsMeasureSoundingNotes () const
{
  Rational result;

  for (S_msrMeasure measure : fFullBarRestsMeasuresList) {
    result +=
      measure->getFullMeasureWholeNotesDuration ();
  } // for

  return result;
}

void msrMultipleFullBarRests::setMultipleFullBarRestsNextMeasureNumber (
  const std::string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Setting multiple full-bar rests next measure number to '" <<
      "' " <<
      nextMeasureNumber <<
      std::endl;
  }
#endif

  fMultipleFullBarRestsNextMeasureNumber =
    nextMeasureNumber;
}

void msrMultipleFullBarRests::setMultipleFullBarRestsLastMeasurePuristMeasureNumber (
  int inputLineNumber)
{
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fMultipleFullBarRestsContents != nullptr,
//     "fMultipleFullBarRestsContents is null");
//
//   S_msrSegment
//     multipleFullBarRestsContentsSegment =
//       fMultipleFullBarRestsContents->
//         getMultipleFullBarRestsContentsSegment ();
//
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     multipleFullBarRestsContentsSegment != nullptr,
//     "multipleFullBarRestsContentsSegment is null");

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
//     std::stringstream s;
//
//     s <<
//       "cannot get multiple full-bar rests contents last measure purist number" <<
//       " because its measure elements list is empty" <<
//       " in voice clone '" <<
//       asShortString () <<
//       "' ";
//
//     msrInternalError (
//       gGlobalServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, __LINE__,
//       s.str ());
//   }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Setting multiple full-bar rests last measure purist number to '" <<
      "' " <<
      lastMeasuresPuristNumber <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  fMultipleFullBarRestsLastMeasurePuristNumber =
    lastMeasuresPuristNumber;
}

void msrMultipleFullBarRests:: appendMeasureElementToSegmentElement (
  const S_msrMeasureElement& elem)
{
  std::stringstream s;

  s <<
    "cannot append measure element " <<
    elem->asShortString () <<
    " to multiple full-bar rests " <<
    asShortString ();

  msrInternalError (
    gGlobalServiceRunData->getInputSourceName (),
    fInputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

void msrMultipleFullBarRests::appendMeasureToMultipleFullBarRests (
  const S_msrMeasure& measure)
{
//   fMultipleFullBarRestsContents->
//     getMultipleFullBarRestsContentsSegment ()->
//       appendMeasureToSegment (
//         measureClone);
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending measure "<<
      measure->asShortString () <<
      " to multiple full-bar rests " <<
      asShortString () <<
      "', line " << fInputLineNumber <<
      std::endl;
  }
#endif

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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMultipleFullBarRests::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrMultipleFullBarRests>*
    p =
      dynamic_cast<visitor<S_msrMultipleFullBarRests>*> (v)) {
        S_msrMultipleFullBarRests elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMultipleFullBarRests::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleFullBarRests::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMultipleFullBarRests::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrMultipleFullBarRests>*
    p =
      dynamic_cast<visitor<S_msrMultipleFullBarRests>*> (v)) {
        S_msrMultipleFullBarRests elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMultipleFullBarRests::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleFullBarRests::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMultipleFullBarRests::browseData ()" <<
      std::endl;
  }

 // JMI   if (! inhibitMultipleFullBarRestsBrowsing) { // KAKA
  // browse the multiple full-bar rests measures
  for (S_msrMeasure measure : fFullBarRestsMeasuresList) {
    // browse the measure
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(measure));
  } // for

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrMultipleFullBarRests::browseData ()" <<
      std::endl;
  }
//  }
}

std::string msrMultipleFullBarRests::asString () const
{
  std::stringstream s;

  s <<
    "[MultipleFullBarRests" <<
    ", for " <<
    mfSingularOrPlural (
      fMultipleFullBarRestsNumber,
        "full-bar rest",
        "full-bar rests");

  s <<
    ", fMultipleFullBarRestsUpLinkToSegment" << ": ";

  if (fMultipleFullBarRestsUpLinkToSegment) {
    s <<
      "fMultipleFullBarRestsUpLinkToSegment->asString ()"; // KAKA
  }
  else {
    s << "[NONE]";
  }

  s <<
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

  return s.str ();
}

void msrMultipleFullBarRests::displayMultipleFullBarRests (
  int           inputLineNumber,
  const std::string& context)
{
  gLogStream <<
    std::endl <<
    "*********>> Full-bar rests " << context << " \"" <<
 // JMI   getVoiceName () <<
    "\"" <<
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
    os << "[NONE]";
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

    os << "fMultipleFullBarRestsUpLinkToSegment" << std::endl; // KAKA v0.9.66

    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
