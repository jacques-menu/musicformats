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

#include "mfAssert.h"
#include "mfPreprocessorSettings.h"


#include "mfDurationsAndPositionInMeasures.h"
#include "msrSegmentElements.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrSegmentElement::msrSegmentElement (
  const mfInputLineNumber& inputLineNumber)
    : msrElement (inputLineNumber)
//       fSegmentElementMeasureMoment (
//         K_POSITION_IN_MEASURE_UNKNOWN_, K_POSITION_IN_MEASURE_UNKNOWN_),
//       fSegmentElementVoiceMoment (
//         K_POSITION_IN_MEASURE_UNKNOWN_, K_POSITION_IN_MEASURE_UNKNOWN_)
{
//   fSegmentElementSoundingWholeNotes = mfRational (0, 1);
}

msrSegmentElement::~msrSegmentElement ()
{}

// void msrSegmentElement::setSegmentElementSoundingWholeNotes (
//   const mfRational& wholeNotes,
//   const std::string&   context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceDurations ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Setting segment element sounding whole notes of " <<
//       asString () <<
//       " to " << wholeNotes.asString () <<
// //       "' in measure '" <<
// //       fSegmentElementMeasureNumber <<
//       ", context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, mfInputLineNumber (__LINE__),
//         ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     wholeNotes != K_WHOLE_NOTES_UNKNOWN_,
//     "wholeNotes == K_WHOLE_NOTES_UNKNOWN_");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   fSegmentElementSoundingWholeNotes = wholeNotes;
// }

void msrSegmentElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSegmentElement::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSegmentElement>*
    p =
      dynamic_cast<visitor<S_msrSegmentElement>*> (v)) {
        S_msrSegmentElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSegmentElement::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrSegmentElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSegmentElement::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSegmentElement>*
    p =
      dynamic_cast<visitor<S_msrSegmentElement>*> (v)) {
        S_msrSegmentElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSegmentElement::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

std::string msrSegmentElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrSegmentElement::asString () ???";
}

std::string msrSegmentElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrSegmentElement::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

void msrSegmentElement::printFull (std::ostream& os) const
{
  print (os); // JMI 0.9.70
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSegmentElement& elt)
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
