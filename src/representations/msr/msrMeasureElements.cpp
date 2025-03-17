/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfConstants.h"


/*
  CAUTION: // JMI v0.9.66
    if might be necessary for the the following two included
    to be strictly in this order depending on the C++ compiler
*/
#include "msrMeasureElements.h"
#include "msrMeasures.h"

#include "msrMeasureConstants.h"

#include "mfMoments.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrMeasureElement::msrMeasureElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
//       fMeasureMoment ( JMI 0.9.72 ???
//         K_POSITION_IN_MEASURE_UNKNOWN_, K_POSITION_IN_MEASURE_UNKNOWN_),
//       fVoiceMoment (
//         K_POSITION_IN_MEASURE_UNKNOWN_, K_POSITION_IN_MEASURE_UNKNOWN_)
{
  fMeasureElementSoundingWholeNotes = K_WHOLE_NOTES_ZERO;

  fMeasureElementPositionInMeasure = K_POSITION_IN_MEASURE_UNKNOWN_;
  fMeasureElementVoicePosition = K_POSITION_IN_MEASURE_UNKNOWN_;
}

msrMeasureElement::~msrMeasureElement ()
{}

//______________________________________________________________________________
// set and get

void msrMeasureElement::setMeasureElementUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  fMeasureElementUpLinkToMeasure = measure;
}

S_msrMeasure msrMeasureElement::getMeasureElementUpLinkToMeasure () const
{
  return fMeasureElementUpLinkToMeasure;
}

void msrMeasureElement::setMeasureElementSoundingWholeNotes (
  const mfWholeNotes& wholeNotes,
  const std::string&   context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    ++gIndenter;

    S_msrMeasure
      upLinkToMeasure =
        getMeasureElementUpLinkToMeasure ();

    std::stringstream ss;

    ss <<
      "Setting the measure element sounding whole notes of " <<
      asString () <<
      " to " << wholeNotes.asString () << // JMI v0.9.66
//       " in measure '" << // JMI TOO EARLY v0.9.67
//       upLinkToMeasure->getMeasureNumber () <<
//       "', context: \"" <<
      ", context: \"" <<
      context <<
      "\"" <<
      std::endl;

//     if (wholeNotes == mfWholeNotes (114, 1)) abort (); // JMI v0.9.67 HARMFUL

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_SANITY_CHECKS_ARE_ENABLED // JMI v0.9.67
//   // sanity check // NO JMI v0.9.67
//   mfAssert (
//     __FILE__, __LINE__,
//     wholeNotes != K_WHOLE_NOTES_UNKNOWN_,
//     "wholeNotes == K_WHOLE_NOTES_UNKNOWN_");
//
//   mfWholeNotes
//     infinitelyDottesLonga (8, 1);
//
//   mfAssert (
//     __FILE__, __LINE__,
//     wholeNotes < infinitelyDottesLonga,
//     "wholeNotes >= infinitelyDottesLonga");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

  fMeasureElementSoundingWholeNotes = wholeNotes;
}

void msrMeasureElement::incrementMeasureElementSoundingWholeNotesBy (
  const mfWholeNotes& wholeNotes,
  const std::string&   context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    ++gIndenter;

    S_msrMeasure
      upLinkToMeasure =
        getMeasureElementUpLinkToMeasure ();

    std::stringstream ss;

    ss <<
      "Incrementing the measure element sounding whole notes of " <<
      asString () <<
      " by " << wholeNotes.asString () << // JMI v0.9.66
//       " in measure '" << // JMI TOO EARLY v0.9.67
//       upLinkToMeasure->getMeasureNumber () <<
//       "', context: \"" <<
      ", context: \"" <<
      context <<
      "\"" <<
      std::endl;

//     if (wholeNotes == mfWholeNotes (114, 1)) abort (); // JMI v0.9.67 HARMFUL

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_SANITY_CHECKS_ARE_ENABLED // JMI v0.9.67
//   // sanity check // NO JMI v0.9.67
//   mfAssert (
//     __FILE__, __LINE__,
//     wholeNotes != K_WHOLE_NOTES_UNKNOWN_,
//     "wholeNotes == K_WHOLE_NOTES_UNKNOWN_");
//
//   mfWholeNotes
//     infinitelyDottesLonga (8, 1);
//
//   mfAssert (
//     __FILE__, __LINE__,
//     wholeNotes < infinitelyDottesLonga,
//     "wholeNotes >= infinitelyDottesLonga");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

  fMeasureElementSoundingWholeNotes += wholeNotes;
}

mfWholeNotes msrMeasureElement:: getMeasureElementSoundingWholeNotes () const
{
  return fMeasureElementSoundingWholeNotes;
}

mfPositionInMeasure msrMeasureElement::getMeasureElementPositionInMeasure () const
{
  return fMeasureElementPositionInMeasure;
}

mfPositionInMeasure msrMeasureElement::getMeasureElementVoicePosition () const
{
  return fMeasureElementVoicePosition;
}

void msrMeasureElement::setMeasureElementPositionInMeasure (
  const S_msrMeasure&        measure,
  const mfPositionInMeasure& positionInMeasure,
  const std::string&         context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePositionInMeasures ()) {
    S_msrMeasure
      upLinkToMeasure =
        getMeasureElementUpLinkToMeasure ();

    std::stringstream ss;

    ss <<
      "Setting the measure position of measure element " <<
      asString () <<
      " to " <<
      positionInMeasure.asString () <<
      " (was '" <<
      fMeasureElementPositionInMeasure.asString () <<
      "') in measure " <<
      measure->asShortString ();

    ss <<
      " (measureElementMeasureNumber: ";
    if (upLinkToMeasure) {
      ss <<
        upLinkToMeasure->getMeasureNumber ();
    }
    else {
      ss << "UNKNOWN_MEASURE_NUMBER";
    }

    ss <<
      "), context: \"" <<
      context <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    positionInMeasure != K_POSITION_IN_MEASURE_UNKNOWN_,
    "positionInMeasure == K_POSITION_IN_MEASURE_UNKNOWN_");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set measure element's measure position
  fMeasureElementPositionInMeasure = positionInMeasure;

//   // compute measure element's voice position
// // if (false) { // JMI v0.9.67
//   mfWholeNotes
//     voicePosition =
//       measure->
//         getMeasureVoicePosition ()
//         +
//       positionInMeasure;

  // set figured bass's voice position
//   setMeasureElementVoicePosition ( // JMI v0.9.66
//     voicePosition,
//     context);
// }

  // update current voice position // JMI v0.9.67
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

//______________________________________________________________________________
bool msrMeasureElement::compareMeasureElementsByIncreasingPositionInMeasure (
  const SMARTP<msrMeasureElement>& first,
  const SMARTP<msrMeasureElement>& second)
{
  return
    bool (
      first->fMeasureElementPositionInMeasure
        <
      second->fMeasureElementPositionInMeasure
    );
}

void msrMeasureElement::setMeasureElementVoicePosition (
  const mfPositionInMeasure& voicePosition,
  const std::string&         context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicePositions ()) {
    S_msrMeasure
      upLinkToMeasure =
        getMeasureElementUpLinkToMeasure ();

    std::stringstream ss;

    ss <<
      "Setting the voice position of " <<
      asString () <<
      " to " << voicePosition <<
      " in measure '" <<
      upLinkToMeasure->getMeasureNumber () <<
      "', context: \"" <<
      context <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voicePosition != K_POSITION_IN_MEASURE_UNKNOWN_,
    "voicePosition == K_POSITION_IN_MEASURE_UNKNOWN_");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set measure element voice position
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicePositions ()) {
    S_msrMeasure
      upLinkToMeasure =
        getMeasureElementUpLinkToMeasure ();

    std::stringstream ss;

    ss <<
      "Setting the measure element voice position of " <<
      asString () <<
      " to " << voicePosition <<
      " in measure '" <<
      upLinkToMeasure->getMeasureNumber () <<
      "', context: \"" <<
      context <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureElementVoicePosition = voicePosition;
}

// void msrMeasureElement::setMeasureMoment (
//   const mfMoment& measureMoment,
//   const std::string&    context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureMoments ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Setting the measure moment of " <<
//       asString () <<
//       " to " << measureMoment <<
//       " (was '" <<
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
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fMeasureMoment = measureMoment;
// }
//
// void msrMeasureElement::setVoiceMoment (
//   const mfMoment& voiceMoment,
//   const std::string&    context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureMoments ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Setting the measure element voice moment of " <<
//       asString () <<
//       " to " << voiceMoment <<
//       " in measure '" <<
//       getMeasureElementUpLinkToMeasure ()->getMeasureNumber () <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     voiceMoment != mfMoment::K_MOMENT_UNKNOWN_,
//     "voiceMoment == mfMoment::K_MOMENT_UNKNOWN_");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   fVoiceMoment = voiceMoment;
// }

void msrMeasureElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureElement::acceptIn ()";

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
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureElement::visitStart ()";

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
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureElement::acceptOut ()";

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
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureElement::visitEnd ()";

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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
