/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrMeasureConstants.h"

#include "msrEyeGlasses.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrEyeGlasses msrEyeGlasses::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrEyeGlasses* obj =
    new msrEyeGlasses (
      inputLineNumber,
      upLinkToMeasure);
  assert (obj != nullptr);
  return obj;
}

S_msrEyeGlasses msrEyeGlasses::create (
  const mfInputLineNumber& inputLineNumber)
{
  return
    msrEyeGlasses::create (
      inputLineNumber,
      gNullMeasure); // set later in setEyeGlassesUpLinkToMeasure()
}

msrEyeGlasses::msrEyeGlasses (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{}

msrEyeGlasses::~msrEyeGlasses ()
{}

// void msrEyeGlasses::setEyeGlassesUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceEyeGlasses ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of eyeglasses " <<
//       asString () <<
//       " to measure " << measure->asString () <<
//       "' in measure '" <<
//       measure->asString () <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fEyeGlassesUpLinkToMeasure = measure;
// }

// void msrEyeGlasses::setEyeGlassesPositionInMeasure (
//   const S_msrMeasure& measure,
//   const mfRational&     positionInMeasure,
//   const std::string&  context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePositionInMeasures ()) {
//
//     gLog <<
//       "Setting the eyeglasses measure position of " <<
//       asString () <<
//       " to " << positionInMeasure.asString () <<
//       " (was '" <<
//       fEyeGlassesPositionInMeasure.asString () <<
//       "') in measure " <<
//       measure->asShortString () <<
//       " (EyeGlassesMeasureNumber: " <<
//       fBarLineUpLinkToMeasure->getMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     positionInMeasure != K_POSITION_IN_MEASURE_UNKNOWN_,
//     "positionInMeasure == K_POSITION_IN_MEASURE_UNKNOWN_");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // set measure element's measure position
//   fEyeGlassesPositionInMeasure = positionInMeasure;
//
//   // compute measure element's voice position
// // if (false) { // JMI 0.9.67 0.9.66
//   mfRational
//     voicePosition =
//       measure->
//         getMeasureVoicePosition ()
//         +
//       positionInMeasure;
//
//   // set figured bass's voice position
//   setEyeGlassesVoicePosition (
//     voicePosition,
//     context);
// // }
//
//   // update current voice position // JMI 0.9.66
// //   S_msrVoice
// //     voice =
// //       measure->
// //         fetchMeasureUpLinkToVoice ();
// //
// //   voice->
// //     incrementCurrentVoicePosition (
// //       fFiguredBassUpLinkToNote->
// //         getEyeGlassesSoundingWholeNotes ());
// }
//
void msrEyeGlasses::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrEyeGlasses::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrEyeGlasses::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrEyeGlasses::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrEyeGlasses::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrEyeGlasses::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrEyeGlasses::browseData (basevisitor* v)
{}

std::string msrEyeGlasses::asString () const
{
  std::stringstream ss;

  ss <<
    "EyeGlasses" <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void msrEyeGlasses::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrEyeGlasses& elt)
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
