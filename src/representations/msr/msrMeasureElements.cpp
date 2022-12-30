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

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfAssert.h"

/*
  CAUTION: // JMI v0.9.66
    if might be necessary for the the following two to be strictly in this order
    depending on the C++ compiler
*/
#include "msrMeasureElements.h"
#include "msrMeasures.h"

#include "msrMeasureConstants.h"

#include "msrMoments.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrMeasureElement::msrMeasureElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
//       fMeasureElementMeasureMoment (
//         msrMoment::K_MEASURE_POSITION_UNKNOWN, msrMoment::K_MEASURE_POSITION_UNKNOWN),
//       fMeasureElementVoiceMoment (
//         msrMoment::K_MEASURE_POSITION_UNKNOWN, msrMoment::K_MEASURE_POSITION_UNKNOWN)
{
  fMeasureElementSoundingWholeNotes = Rational (0, 1),

  fMeasureElementMeasurePosition = msrMoment::K_MEASURE_POSITION_UNKNOWN;
  fMeasureElementVoicePosition   = msrMoment::K_MEASURE_POSITION_UNKNOWN;
}

msrMeasureElement::~msrMeasureElement ()
{}

void msrMeasureElement::setMeasureElementSoundingWholeNotes (
  const Rational&    wholeNotes,
  const std::string& context)
{
  doSetMeasureElementSoundingWholeNotes (
    wholeNotes,
    context);
}

void msrMeasureElement::doSetMeasureElementSoundingWholeNotes (
  const Rational&    wholeNotes,
  const std::string& context)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    S_msrMeasure upLinkToMeasure;

    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

    gLogStream <<
      "==> Setting measure element sounding whole notes of " <<
      asString () <<
      " to 'WHOLE_NOTES " << wholeNotes << // JMI v0.9.66
      "' in measure '" <<
      upLinkToMeasure->getMeasureNumber () <<
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
    wholeNotes != K_WHOLE_NOTES_UNKNOWN,
    "wholeNotes == K_WHOLE_NOTES_UNKNOWN");

  fMeasureElementSoundingWholeNotes = wholeNotes;
}

void msrMeasureElement::setMeasureElementMeasurePosition (
  const S_msrMeasure& measure,
  const Rational&     measurePosition,
  const std::string&  context)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    S_msrMeasure upLinkToMeasure;

    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

    gLogStream <<
      "Setting measure position of " <<
      asString () <<
      " to '" << measurePosition <<
      "' (was '" <<
      fMeasureElementMeasurePosition <<
      "') in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      upLinkToMeasure->getMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_MEASURE_POSITION_UNKNOWN,
    "measurePosition == msrMoment::K_MEASURE_POSITION_UNKNOWN");

  // set measure element's measure position
  fMeasureElementMeasurePosition = measurePosition;

//   // compute measure element's voice position
// // if (false) { // JMI CAFE v0.9.66
//   Rational
//     voicePosition =
//       measure->
//         getMeasureVoicePosition ()
//         +
//       measurePosition;

  // set figured bass's voice position
//   setMeasureElementVoicePosition ( // JMI v0.9.66
//     voicePosition,
//     context);
// }

  // update current voice position // JMI v0.9.66
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

void msrMeasureElement::setMeasureElementVoicePosition (
  const Rational&    voicePosition,
  const std::string& context)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicePositions ()) {
    S_msrMeasure upLinkToMeasure;

    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

    gLogStream <<
      "Setting voice position of " <<
      asString () <<
      " to '" << voicePosition <<
      "' in measure '" <<
      upLinkToMeasure->getMeasureNumber () <<
      "', context: \"" <<
      context <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voicePosition != msrMoment::K_MEASURE_POSITION_UNKNOWN,
    "voicePosition == msrMoment::K_MEASURE_POSITION_UNKNOWN");

  // set measure element voice position
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicePositions ()) {
    S_msrMeasure upLinkToMeasure;

    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

    gLogStream <<
      "Setting measure element voice position of " <<
      asString () <<
      " to '" << voicePosition <<
      "' in measure '" <<
      upLinkToMeasure->getMeasureNumber () <<
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
// #ifdef OAH_TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasureMoments ()) {
//     gLogStream <<
//       "Setting measure element measure moment of " <<
//       asString () <<
//       " to '" << measureMoment <<
//       "' (was '" <<
//       fMeasureElementMeasureMoment.asString () <<
// //       "') " <<
// //       in measure " << JMI v0.9.66
// //       measure->asShortString () <<
//       "), measureElementMeasureNumber: " <<
//       getMeasureElementUpLinkToMeasure ()->getMeasureNumber () <<
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
// #ifdef OAH_TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasureMoments ()) {
//     gLogStream <<
//       "Setting measure element voice moment of " <<
//       asString () <<
//       " to '" << voiceMoment <<
//       "' in measure '" <<
//       getMeasureElementUpLinkToMeasure ()->getMeasureNumber () <<
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
//     voiceMoment != msrMoment::K_MOMENT_UNKNOWN,
//     "voiceMoment == msrMoment::K_MOMENT_UNKNOWN");
//
//   fMeasureElementVoiceMoment = voiceMoment;
// }

void msrMeasureElement::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
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

#ifdef OAH_TRACING_IS_ENABLED
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
#ifdef OAH_TRACING_IS_ENABLED
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

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureElement::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrMeasureElement::browseData (basevisitor* v)
{}

std::string msrMeasureElement::asString () const
{
  // this is overriden all in actual elements
  return "[??? msrMeasureElement ???]";
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

void msrMeasureElement::printSummary (std::ostream& os) const
{}

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


}
