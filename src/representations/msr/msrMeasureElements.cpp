/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "mfStaticSettings.h"

#include "visitor.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"

/*
  CAUTION: // JMI v0.9.66
    if might be necessary for the the following two included
    to be strictly in this order depending on the C++ compiler
*/
#include "msrMeasureElements.h"
#include "msrMeasures.h"

#include "msrMeasureConstants.h"

#include "msrMoments.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrMeasureElement::msrMeasureElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
//       fMeasureMoment (
//         msrMoment::K_MEASURE_POSITION_UNKNOWN, msrMoment::K_MEASURE_POSITION_UNKNOWN),
//       fVoiceMoment (
//         msrMoment::K_MEASURE_POSITION_UNKNOWN, msrMoment::K_MEASURE_POSITION_UNKNOWN)
{
  fSoundingWholeNotes = Rational (0, 1),

  fMeasurePosition = msrMoment::K_MEASURE_POSITION_UNKNOWN;
  fVoicePosition   = msrMoment::K_MEASURE_POSITION_UNKNOWN;
}

msrMeasureElement::~msrMeasureElement ()
{}

void msrMeasureElement::setSoundingWholeNotes (
  const Rational&    wholeNotes,
  const std::string& context)
{
  doSetSoundingWholeNotes (
    wholeNotes,
    context);
}

void msrMeasureElement::doSetSoundingWholeNotes (
  const Rational&    wholeNotes,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    S_msrMeasure upLinkToMeasure;

    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

    gLog <<
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
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    wholeNotes != K_WHOLE_NOTES_UNKNOWN,
    "wholeNotes == K_WHOLE_NOTES_UNKNOWN");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSoundingWholeNotes = wholeNotes;
}

void msrMeasureElement::setMeasurePosition (
  const S_msrMeasure& measure,
  const Rational&     measurePosition,
  const std::string&  context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasurePositions ()) {
    S_msrMeasure upLinkToMeasure;

    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

		std::stringstream ss;

    ss <<
      "Setting measure position of " <<
      asString () <<
      " to '" << measurePosition <<
      "' (was '" <<
      fMeasurePosition <<
      "') in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      upLinkToMeasure->getMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_MEASURE_POSITION_UNKNOWN,
    "measurePosition == msrMoment::K_MEASURE_POSITION_UNKNOWN");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set measure element's measure position
  fMeasurePosition = measurePosition;

//   // compute measure element's voice position
// // if (false) { // JMI CAFE v0.9.66
//   Rational
//     voicePosition =
//       measure->
//         getMeasureVoicePosition ()
//         +
//       measurePosition;

  // set figured bass's voice position
//   setVoicePosition ( // JMI v0.9.66
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
//         getSoundingWholeNotes ());
}

bool msrMeasureElement::compareMeasureElementsByIncreasingMeasurePosition (
  const SMARTP<msrMeasureElement>& first,
  const SMARTP<msrMeasureElement>& second)
{
  return
    bool (
      first->getMeasurePosition ()
        <
      second->getMeasurePosition ()
    );
}

void msrMeasureElement::setVoicePosition (
  const Rational&    voicePosition,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoicePositions ()) {
    S_msrMeasure upLinkToMeasure;

    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

		std::stringstream ss;

    ss <<
      "Setting voice position of " <<
      asString () <<
      " to '" << voicePosition <<
      "' in measure '" <<
      upLinkToMeasure->getMeasureNumber () <<
      "', context: \"" <<
      context <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voicePosition != msrMoment::K_MEASURE_POSITION_UNKNOWN,
    "voicePosition == msrMoment::K_MEASURE_POSITION_UNKNOWN");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set measure element voice position
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoicePositions ()) {
    S_msrMeasure upLinkToMeasure;

    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

		std::stringstream ss;

    ss <<
      "Setting measure element voice position of " <<
      asString () <<
      " to '" << voicePosition <<
      "' in measure '" <<
      upLinkToMeasure->getMeasureNumber () <<
      "', context: \"" <<
      context <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoicePosition = voicePosition;
}

// void msrMeasureElement::setMeasureMoment (
//   const msrMoment& measureMoment,
//   const std::string&    context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gGlobalTraceOahGroup->getTraceMeasureMoments ()) {
//     gLog <<
//       "Setting measure element measure moment of " <<
//       asString () <<
//       " to '" << measureMoment <<
//       "' (was '" <<
//       fMeasureMoment.asString () <<
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
// #endif // MF_TRACE_IS_ENABLED
//
//   fMeasureMoment = measureMoment;
// }
//
// void msrMeasureElement::setVoiceMoment (
//   const msrMoment& voiceMoment,
//   const std::string&    context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gGlobalTraceOahGroup->getTraceMeasureMoments ()) {
//     gLog <<
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
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     voiceMoment != msrMoment::K_MOMENT_UNKNOWN,
//     "voiceMoment == msrMoment::K_MOMENT_UNKNOWN");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   fVoiceMoment = voiceMoment;
// }

void msrMeasureElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrMeasureElement::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureElement::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrMeasureElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrMeasureElement::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureElement::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
