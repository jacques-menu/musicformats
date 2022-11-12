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

/*
  CAUTION: // JMI v0.9.66
    the following two should be in this order
    due to mutual dependencies
*/
#include "msrMeasures.h"
#include "msrMeasureElements.h"

#include "msrMoments.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
// constants
const std::string
  msrMeasureElement::K_NO_MEASURE_NUMBER = "K_NO_MEASURE_NUMBER";

const Rational
  msrMeasureElement::K_NO_WHOLE_NOTES (-444444, 1);

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

void msrMeasureElement::setMeasureElementSoundingWholeNotes (
  const Rational& wholeNotes,
  const std::string&   context)
{
  doSetMeasureElementSoundingWholeNotes (
    wholeNotes,
    context);
}

void msrMeasureElement::doSetMeasureElementSoundingWholeNotes (
  const Rational& wholeNotes,
  const std::string&   context)
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
      std::endl;

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
  const std::string& context)
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
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set measure element's position in measure
  fMeasureElementMeasurePosition = measurePosition;

  // compute measure element's position in voice
// if (false) { // JMI CAFE v0.9.66
  Rational
    voicePosition =
      measure->
        getMeasureVoicePosition ()
        +
      measurePosition;

  // set figured bass's position in voice
  setMeasureElementVoicePosition (
    voicePosition,
    context);
// }

  // update current position in voice // JMI v0.9.66
//   S_msrVoice
//     voice =
//       measure->
//         fetchMeasureUpLinkToVoice ();
//
//   voice->
//     incrementCurrentVoicePosition (
//       fFiguredBassUpLinkToNote->
//         getMeasureElementSoundingWholeNotes ());
}

void msrMeasureElement::setMeasureElementVoicePosition (
  const Rational& voicePosition,
  const std::string&   context)
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
      std::endl;
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
      std::endl;
  }
#endif

  fMeasureElementVoicePosition = voicePosition;
}

// void msrMeasureElement::setMeasureElementMeasureMoment (
//   const msrMoment& measureMoment,
//   const std::string&    context)
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
//       std::endl;
//   }
// #endif
//
//   fMeasureElementMeasureMoment = measureMoment;
// }
//
// void msrMeasureElement::setMeasureElementVoiceMoment (
//   const msrMoment& voiceMoment,
//   const std::string&    context)
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
//       std::endl;
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

std::string msrMeasureElement::fetchMeasureElementMeasureNumber () const
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
      std::endl;
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
            std::endl;
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
      std::endl;
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
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

std::string msrMeasureElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrMeasureElement::asString () ???";
}

std::string msrMeasureElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrMeasureElement::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrMeasureElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
msrMeasureElementLambda::msrMeasureElementLambda (
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

msrMeasureElementLambda::~msrMeasureElementLambda ()
{}

void msrMeasureElementLambda::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureElementLambda::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureElementLambda::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureElementLambda::setMeasureElementLambdaUpLinkToMeasure (
  S_msrMeasure measure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of measure element lambda " <<
      asString () <<
      " to measure " << measure->asString () << // JMI v0.9.66
      "' in measure '" <<
      fetchMeasureElementMeasureNumber () <<
      std::endl;

    --gIndenter;
  }
#endif

  fMeasureElementLambaUpLinkToMeasure = measure;
}

S_msrMeasure msrMeasureElementLambda::getMeasureElementLambdaUpLinkToMeasure () const
{
  return fMeasureElementLambaUpLinkToMeasure;
}

void msrMeasureElementLambda::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureElementLambda::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureElementLambda::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

std::string msrMeasureElementLambda::asString () const
{
  // this is overriden all in actual elements
  return "??? msrMeasureElementLambda::asString () ???";
}

std::string msrMeasureElementLambda::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrMeasureElementLambda::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrMeasureElementLambda& elt)
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
