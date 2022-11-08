/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "msrMeasureElements.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// constants
const string   msrMeasureElement::K_NO_MEASURE_NUMBER = "K_NO_MEASURE_NUMBER";

const Rational msrMeasureElement::K_NO_WHOLE_NOTES (-444444, 1);

msrMeasureElement::msrMeasureElement (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
    : msrElement (inputLineNumber)
//       fMeasureElementMeasureMoment (
//         msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION),
//       fMeasureElementVoiceMoment (
//         msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION)
{
  fMeasureElementUpLinkToMeasure = upLinkToMeasure;

  fMeasureElementSoundingWholeNotes = Rational (0, 1),

  fMeasureElementMeasurePosition = msrMoment::K_NO_POSITION;
  fMeasureElementVoicePosition   = msrMoment::K_NO_POSITION;
}

msrMeasureElement::~msrMeasureElement ()
{}

void msrMeasureElement::setMeasureElementUpLinkToMeasure (
  S_msrMeasure measure)
{
  fMeasureElementUpLinkToMeasure = measure;
}

S_msrMeasure msrMeasureElement::getMeasureElementUpLinkToMeasure () const
{
  return fMeasureElementUpLinkToMeasure;
}

void msrMeasureElement::setMeasureElementSoundingWholeNotes (
  const Rational& wholeNotes,
  const string&   context)
{
  doSetMeasureElementSoundingWholeNotes (
    wholeNotes,
    context);
}

void msrMeasureElement::doSetMeasureElementSoundingWholeNotes (
  const Rational& wholeNotes,
  const string&   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting measure element sounding whole notes of " <<
      asString () <<
      " to 'WHOLE_NOTES " << wholeNotes << // JMI v0.9.66
      "' in measure '" <<
      fetchMeasureElementMeasureNumber () <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;

    --gIndenter;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    wholeNotes != K_NO_WHOLE_NOTES,
    "wholeNotes == K_NO_WHOLE_NOTES");

  fMeasureElementSoundingWholeNotes = wholeNotes;
}

void msrMeasureElement::setMeasureElementMeasurePosition (
  const S_msrMeasure measure,
  const Rational&    measurePosition,
  const string&      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {

    gLogStream <<
      "Setting measure element position in measure of " <<
      asString () <<
      " to '" << measurePosition <<
      "' (was '" <<
      fMeasureElementMeasurePosition <<
      "') in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      fetchMeasureElementMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set measure element's position in measure
  fMeasureElementMeasurePosition = measurePosition;
}

void msrMeasureElement::setMeasureElementVoicePosition (
  const Rational& voicePosition,
  const string&   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting measure element position in voice of " <<
      asString () <<
      " to '" << voicePosition <<
      "' in measure '" <<
      fetchMeasureElementMeasureNumber () <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voicePosition != msrMoment::K_NO_POSITION,
    "voicePosition == msrMoment::K_NO_POSITION");

  // set measure element position in voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting measure element position in voice of " <<
      asString () <<
      " to '" << voicePosition <<
      "' in measure '" <<
      fetchMeasureElementMeasureNumber () <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  fMeasureElementVoicePosition = voicePosition;
}

// void msrMeasureElement::setMeasureElementMeasureMoment (
//   const msrMoment& measureMoment,
//   const string&    context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMomentsInMeasures ()) {
//     gLogStream <<
//       "Setting measure element moment in measure of " <<
//       asString () <<
//       " to '" << measureMoment <<
//       "' (was '" <<
//       fMeasureElementMeasureMoment.asString () <<
// //       "') " <<
// //       in measure " << JMI v0.9.66
// //       measure->asShortString () <<
//       "), measureElementMeasureNumber: " <<
//       fetchMeasureElementMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
//
//   fMeasureElementMeasureMoment = measureMoment;
// }
//
// void msrMeasureElement::setMeasureElementVoiceMoment (
//   const msrMoment& voiceMoment,
//   const string&    context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMomentsInMeasures ()) {
//     gLogStream <<
//       "Setting measure element moment in voice of " <<
//       asString () <<
//       " to '" << voiceMoment <<
//       "' in measure '" <<
//       fetchMeasureElementMeasureNumber () <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
//
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     voiceMoment != msrMoment::K_NO_MOMENT,
//     "voiceMoment == msrMoment::K_NO_MOMENT");
//
//   fMeasureElementVoiceMoment = voiceMoment;
// }

string msrMeasureElement::fetchMeasureElementMeasureNumber () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fMeasureElementUpLinkToMeasure != nullptr,
    "fMeasureElementUpLinkToMeasure is null");

  return
    fMeasureElementUpLinkToMeasure->
      getMeasureNumber ();
}

bool msrMeasureElement::compareMeasureElementsByIncreasingMeasurePosition (
  const SMARTP<msrMeasureElement>& first,
  const SMARTP<msrMeasureElement>& second)
{
  return
    bool (
      first->getMeasureElementMeasurePosition ()
        <
      second->getMeasureElementMeasurePosition ()
    );
}

void msrMeasureElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

string msrMeasureElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrMeasureElement::asString () ???";
}

string msrMeasureElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrMeasureElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrMeasureElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
