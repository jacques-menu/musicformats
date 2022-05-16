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

#include "msrMultipleFullBarRests.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrMultipleFullBarRests msrMultipleFullBarRests::create (
  int              inputLineNumber,
  const rational&  multipleFullBarRestsMeasureSoundingNotes,
  int              multipleFullBarRestsNumber,
  S_msrVoice       voiceUpLink)
{
  msrMultipleFullBarRests* o =
    new msrMultipleFullBarRests (
      inputLineNumber,
      multipleFullBarRestsMeasureSoundingNotes,
      multipleFullBarRestsNumber,
      voiceUpLink);
  assert (o != nullptr);
  return o;
}

S_msrMultipleFullBarRests msrMultipleFullBarRests::create (
  int          inputLineNumber,
  S_msrMeasure restMeasureClone,
  S_msrVoice   voiceUpLink)
{
  msrMultipleFullBarRests* o =
    new msrMultipleFullBarRests (
      inputLineNumber,
      restMeasureClone,
      voiceUpLink);
  assert (o != nullptr);
  return o;
}

msrMultipleFullBarRests::msrMultipleFullBarRests (
  int             inputLineNumber,
  const rational& multipleFullBarRestsMeasureSoundingNotes,
  int             multipleFullBarRestsNumber,
  S_msrVoice      voiceUpLink)
    : msrSegmentElement (inputLineNumber)
{
  fMultipleFullBarRestsVoiceUpLink = voiceUpLink;

  fMultipleFullBarRestsMeasureSoundingNotes =
    multipleFullBarRestsMeasureSoundingNotes;

  fMultipleFullBarRestsNumber = multipleFullBarRestsNumber;

  fMultipleFullBarRestsLastMeasurePuristNumber = -1;
}

msrMultipleFullBarRests::msrMultipleFullBarRests (
  int          inputLineNumber,
  S_msrMeasure restMeasureClone,
  S_msrVoice   voiceUpLink)
    : msrSegmentElement (inputLineNumber)
{
  fMultipleFullBarRestsVoiceUpLink = voiceUpLink;

  fMultipleFullBarRestsMeasureSoundingNotes =
    restMeasureClone->
      getFullMeasureWholeNotesDuration (); // JMI ???

  fMultipleFullBarRestsNumber = 1; // will evolve JMI

  fMultipleFullBarRestsLastMeasurePuristNumber = -1;

//   // create the multiple full-bar rests contents
//   fMultipleFullBarRestsContents =
//     msrMultipleFullBarRestsContents::create (
//       inputLineNumber,
//       this);
}

msrMultipleFullBarRests::~msrMultipleFullBarRests ()
{}

S_msrMultipleFullBarRests msrMultipleFullBarRests::createMultipleFullBarRestsNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Creating a newborn clone of multiple full-bar rests '" <<
      asString () <<
      "'" <<
      endl;
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
        fMultipleFullBarRestsMeasureSoundingNotes,
        fMultipleFullBarRestsNumber,
        containingVoice);

/* JMI
  newbornClone->fMultipleFullBarRestsNextMeasureNumber =
    fMultipleFullBarRestsNextMeasureNumber;
    */

  return newbornClone;
}

// void msrMultipleFullBarRests::setMultipleFullBarRestsContents (
//   S_msrMultipleFullBarRestsContents multipleFullBarRestsContents)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
//     gLogStream <<
//       "Setting multiple full-bar rests contents containing " <<
//       mfSingularOrPlural (
//         multipleFullBarRestsContents->
//           multipleFullBarRestsContentsMeasuresNumber (),
//         "measure",
//         "measures") <<
//       endl;
//   }
// #endif
//
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     multipleFullBarRestsContents != nullptr,
//     "multipleFullBarRestsContents is null");
//
//   fMultipleFullBarRestsContents = multipleFullBarRestsContents;
// }

void msrMultipleFullBarRests::setMultipleFullBarRestsNextMeasureNumber (
  const string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Setting multiple full-bar rests next measure number to '" <<
      "' " <<
      nextMeasureNumber <<
      endl;
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
//   const list<S_msrMeasure>&
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
//     stringstream s;
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
      endl;
  }
#endif

  fMultipleFullBarRestsLastMeasurePuristNumber =
    lastMeasuresPuristNumber;
}

void msrMultipleFullBarRests:: appendMeasureElementToSegmentElement (
  S_msrMeasureElement elem)
{
  stringstream s;

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
  S_msrMeasure measure)
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
      endl;
  }
#endif

  fFullBarRestsMeasuresList.push_back (measure);
}

void msrMultipleFullBarRests::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMultipleFullBarRests::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMultipleFullBarRests>*
    p =
      dynamic_cast<visitor<S_msrMultipleFullBarRests>*> (v)) {
        S_msrMultipleFullBarRests elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMultipleFullBarRests::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleFullBarRests::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMultipleFullBarRests::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMultipleFullBarRests>*
    p =
      dynamic_cast<visitor<S_msrMultipleFullBarRests>*> (v)) {
        S_msrMultipleFullBarRests elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMultipleFullBarRests::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleFullBarRests::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMultipleFullBarRests::browseData ()" <<
      endl;
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
      endl;
  }
//  }
}

string msrMultipleFullBarRests::asString () const
{
  stringstream s;

  s <<
    "MultipleFullBarRests" <<
    ", multipleFullBarRestsLastMeasurePuristNumber: '" <<
    fMultipleFullBarRestsLastMeasurePuristNumber <<
    "'" <<
    ", multipleFullBarRestsMeasureSoundingNotes: " <<
    fMultipleFullBarRestsMeasureSoundingNotes <<
    ", " <<
    mfSingularOrPlural (
      fMultipleFullBarRestsNumber,
        "rest measure",
        "multiple full-bar rests") <<
    ", multipleFullBarRestsNextMeasureNumber: '" <<
    fMultipleFullBarRestsNextMeasureNumber <<
    "'" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrMultipleFullBarRests::displayMultipleFullBarRests (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    "*********>> Full-bar rests " << context << " \"" <<
 // JMI   getVoiceName () <<
    "\"" <<
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

void msrMultipleFullBarRests::print (ostream& os) const
{
  os <<
    "MultipleFullBarRests" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 36;

  os << left <<
    setw (fieldWidth) <<
    "multipleFullBarRestsLastMeasurePuristNumber" << " : " <<
    fMultipleFullBarRestsLastMeasurePuristNumber <<
    endl <<
    setw (fieldWidth) <<
    "multipleFullBarRestsMeasureSoundingNotes" << " : " <<
    fMultipleFullBarRestsMeasureSoundingNotes <<
    endl <<
    setw (fieldWidth) <<
    "multipleFullBarRestsNumber" << " : " <<
    fMultipleFullBarRestsNumber <<
    endl <<
    setw (fieldWidth) <<
    "multipleFullBarRestsNextMeasureNumber" << " : '" <<
    fMultipleFullBarRestsNextMeasureNumber <<
    "'" <<
    endl;

  // print the voice upLink
  os << left <<
    setw (fieldWidth) <<
    "multipleFullBarRestsVoiceUpLink" << " : " <<
    "\"" <<
    fMultipleFullBarRestsVoiceUpLink->getVoiceName () <<
    "\"" <<
    endl;

  // print the rests contents
//   if (! fMultipleFullBarRestsContents) {
//     os << left <<
//       setw (fieldWidth) <<
//       "multipleFullBarRestsContents" << " : " << "none" <<
//       endl;
//   }
//
//   else {
// //     os <<
// //       fMultipleFullBarRestsContents;
//   }

  --gIndenter;
}

void msrMultipleFullBarRests::printShort (ostream& os) const
{
  print (os);
}

ostream& operator<< (ostream& os, const S_msrMultipleFullBarRests& elt)
{
  elt->print (os);
  return os;
}


}
