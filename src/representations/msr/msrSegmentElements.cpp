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

#include "msrSegmentElements.h"



#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// constants
const string   msrSegmentElement::K_NO_MEASURE_NUMBER = "K_NO_MEASURE_NUMBER";

const Rational msrSegmentElement::K_NO_WHOLE_NOTES (-444444, 1);

msrSegmentElement::msrSegmentElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
//       fSegmentElementMomentInMeasure (
//         msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION),
//       fSegmentElementMomentFromBeginningOfVoice (
//         msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION)
{
  fSegmentElementSoundingWholeNotes = Rational (0, 1);

//   fSegmentElementMeasureNumber = K_NO_MEASURE_NUMBER;

//   fSegmentElementMeasurePosition = msrMoment::K_NO_POSITION;
//   fSegmentElementPositionFromBeginningOfVoice   = msrMoment::K_NO_POSITION;
}

msrSegmentElement::~msrSegmentElement ()
{}

void msrSegmentElement::setSegmentElementSoundingWholeNotes (
  const Rational& wholeNotes,
  const string&   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting measure element sounding whole notes of " <<
      asString () <<
      " to '" << wholeNotes <<
//       "' in measure '" <<
//       fSegmentElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    wholeNotes != K_NO_WHOLE_NOTES,
    "wholeNotes == K_NO_WHOLE_NOTES");

  fSegmentElementSoundingWholeNotes = wholeNotes;
}

// void msrSegmentElement::setSegmentElementMeasurePosition (
//   const Rational& measurePosition,
//   const string&   context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
//     gLogStream <<
//       "Setting measure element position in measure of " <<
//       asString () <<
//       " to '" << measurePosition <<
//       "' (was '" <<
//       fSegmentElementMeasurePosition <<
//       "') " <<
// //       " in measure '" <<
// //       fSegmentElementMeasureNumber <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
// /* JMI
////   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measurePosition != msrMoment::K_NO_POSITION,
//     "measurePosition == msrMoment::K_NO_POSITION");
// */
//
//   fSegmentElementMeasurePosition = measurePosition;
// }
//
// void msrSegmentElement::setSegmentElementPositionFromBeginningOfVoice (
//   const Rational& positionFromBeginningOfVoice,
//   const string&   context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
//     gLogStream <<
//       "Setting measure element position in voice of " <<
//       asString () <<
//       " to '" << positionFromBeginningOfVoice <<
// //       "' in measure '" <<
// //       fSegmentElementMeasureNumber <<
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
//     positionFromBeginningOfVoice != msrMoment::K_NO_POSITION,
//     "positionFromBeginningOfVoice == msrMoment::K_NO_POSITION");
//
//   fSegmentElementPositionFromBeginningOfVoice = positionFromBeginningOfVoice;
// }
//
// void msrSegmentElement::setSegmentElementMomentInMeasure (
//   const msrMoment& momentInMeasure,
//   const string&    context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMomentsInMeasures ()) {
//     gLogStream <<
//       "Setting measure element moment in measure of " <<
//       asString () <<
//       " to '" << momentInMeasure <<
//       "' (was '" <<
//       fSegmentElementMomentInMeasure.asString () <<
// //       "') in measure '" <<
// //       fSegmentElementMeasureNumber <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
//
//   fSegmentElementMomentInMeasure = momentInMeasure;
// }
//
// void msrSegmentElement::setSegmentElementMomentFromBeginningOfVoice (
//   const msrMoment& momentFromBeginningOfVoice,
//   const string&    context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMomentsInMeasures ()) {
//     gLogStream <<
//       "Setting measure element moment in voice of " <<
//       asString () <<
//       " to '" << momentFromBeginningOfVoice <<
// //       "' in measure '" <<
// //       fSegmentElementMeasureNumber <<
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
//     momentFromBeginningOfVoice != msrMoment::K_NO_MOMENT,
//     "momentFromBeginningOfVoice == msrMoment::K_NO_MOMENT");
//
//   fSegmentElementMomentFromBeginningOfVoice = momentFromBeginningOfVoice;
// }
//
// void msrSegmentElement::assignSegmentElementPositionFromBeginningOfVoice (
//   Rational&     positionFromBeginningOfVoice,
//   const string& context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
//     gLogStream <<
//       "Assigning measure element position in voice of " <<
//       asString () <<
//       " to '" << positionFromBeginningOfVoice <<
// //       "' in measure '" <<
// //       fSegmentElementMeasureNumber <<
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
//     positionFromBeginningOfVoice != msrMoment::K_NO_POSITION,
//     "positionFromBeginningOfVoice == msrMoment::K_NO_POSITION");
//
//   // set measure element position in voice
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
//     gLogStream <<
//       "Setting measure element position in voice of " <<
//       asString () <<
//       " to '" << positionFromBeginningOfVoice <<
// //       "' in measure '" <<
// //       fSegmentElementMeasureNumber <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
//
//   fSegmentElementPositionFromBeginningOfVoice = positionFromBeginningOfVoice;
//
//   // account for it in positionFromBeginningOfVoice
//   positionFromBeginningOfVoice +=
//     fSegmentElementSoundingWholeNotes;
//
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
//     gLogStream <<
//       "Position in voice becomes " <<
//       positionFromBeginningOfVoice <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
// }
//
// bool msrSegmentElement::compareSegmentElementsByIncreasingMeasurePosition (
//   const SMARTP<msrSegmentElement>& first,
//   const SMARTP<msrSegmentElement>& second)
// {
//   return
//     first->getSegmentElementMeasurePosition ()
//       <
//     second->getSegmentElementMeasurePosition ();
// }

void msrSegmentElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegmentElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrSegmentElement>*
    p =
      dynamic_cast<visitor<S_msrSegmentElement>*> (v)) {
        S_msrSegmentElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegmentElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrSegmentElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegmentElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrSegmentElement>*
    p =
      dynamic_cast<visitor<S_msrSegmentElement>*> (v)) {
        S_msrSegmentElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegmentElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

string msrSegmentElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrSegmentElement::asString () ???";
}

string msrSegmentElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrSegmentElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrSegmentElement& elt)
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
