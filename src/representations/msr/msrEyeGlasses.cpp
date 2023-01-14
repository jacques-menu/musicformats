/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfEnableSanityChecksSetting.h"

#include "visitor.h"

#include "mfEnableTracingSetting.h"

#include "mfAssert.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrMeasureConstants.h"

#include "msrEyeGlasses.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrEyeGlasses msrEyeGlasses::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrEyeGlasses* o =
    new msrEyeGlasses (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrEyeGlasses msrEyeGlasses::create (
  int inputLineNumber)
{
  return
    msrEyeGlasses::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer); // set later in setEyeGlassesUpLinkToMeasure()
}

msrEyeGlasses::msrEyeGlasses (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{}

msrEyeGlasses::~msrEyeGlasses ()
{}

void msrEyeGlasses::setEyeGlassesUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of eyeglasses " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fEyeGlassesUpLinkToMeasure = measure;
}

// void msrEyeGlasses::setEyeGlassesMeasurePosition (
//   const S_msrMeasure& measure,
//   const Rational&     measurePosition,
//   const std::string&  context)
// {
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//
//     gLogStream <<
//       "Setting measure element measure position of " <<
//       asString () <<
//       " to '" << measurePosition <<
//       "' (was '" <<
//       fEyeGlassesMeasurePosition <<
//       "') in measure " <<
//       measure->asShortString () <<
//       " (EyeGlassesMeasureNumber: " <<
//       fBarLineUpLinkToMeasure->getMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//   }
// #endif
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measurePosition != msrMoment::K_MEASURE_POSITION_UNKNOWN,
//     "measurePosition == msrMoment::K_MEASURE_POSITION_UNKNOWN");
// #endif
//
//   // set measure element's measure position
//   fEyeGlassesMeasurePosition = measurePosition;
//
//   // compute measure element's voice position
// // if (false) { // JMI CAFE v0.9.66
//   Rational
//     voicePosition =
//       measure->
//         getMeasureVoicePosition ()
//         +
//       measurePosition;
//
//   // set figured bass's voice position
//   setEyeGlassesVoicePosition (
//     voicePosition,
//     context);
// // }
//
//   // update current voice position // JMI v0.9.66
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrEyeGlasses::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrEyeGlasses::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrEyeGlasses::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrEyeGlasses::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrEyeGlasses::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrEyeGlasses::browseData (basevisitor* v)
{}

std::string msrEyeGlasses::asString () const
{
  std::stringstream s;

  s <<
    "EyeGlasses" <<
    ", line " << fInputLineNumber;

  return s.str ();
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
