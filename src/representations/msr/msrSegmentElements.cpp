/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "mfEnableSanityChecksSetting.h"

#include "visitor.h"

#include "mfEnableTracingSetting.h"

#include "mfAssert.h"

#include "msrSegmentElements.h"



#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
// constants
const std::string   msrSegmentElement::K_MEASURE_NUMBER_UNKNOWN = "K_MEASURE_NUMBER_UNKNOWN";

const Rational msrSegmentElement::K_WHOLE_NOTES_UNKNOWN (-444444, 1);

msrSegmentElement::msrSegmentElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
//       fSegmentElementMeasureMoment (
//         msrMoment::K_MEASURE_POSITION_UNKNOWN, msrMoment::K_MEASURE_POSITION_UNKNOWN),
//       fSegmentElementVoiceMoment (
//         msrMoment::K_MEASURE_POSITION_UNKNOWN, msrMoment::K_MEASURE_POSITION_UNKNOWN)
{
  fSegmentElementSoundingWholeNotes = Rational (0, 1);

//   fSegmentElementMeasureNumber = K_MEASURE_NUMBER_UNKNOWN;

//   fSegmentElementMeasurePosition = msrMoment::K_MEASURE_POSITION_UNKNOWN;
//   fSegmentElementVoicePosition   = msrMoment::K_MEASURE_POSITION_UNKNOWN;
}

msrSegmentElement::~msrSegmentElement ()
{}

void msrSegmentElement::setSegmentElementSoundingWholeNotes (
  const Rational& wholeNotes,
  const std::string&   context)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "Setting measure element sounding whole notes of " <<
      asString () <<
      " to '" << wholeNotes <<
//       "' in measure '" <<
//       fSegmentElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    wholeNotes != K_WHOLE_NOTES_UNKNOWN,
    "wholeNotes == K_WHOLE_NOTES_UNKNOWN");
#endif

  fSegmentElementSoundingWholeNotes = wholeNotes;
}

void msrSegmentElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegmentElement::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msrSegmentElement>*
    p =
      dynamic_cast<visitor<S_msrSegmentElement>*> (v)) {
        S_msrSegmentElement elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegmentElement::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrSegmentElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegmentElement::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msrSegmentElement>*
    p =
      dynamic_cast<visitor<S_msrSegmentElement>*> (v)) {
        S_msrSegmentElement elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegmentElement::visitEnd ()" <<
            std::endl;
        }
#endif
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

std::ostream& operator << (std::ostream& os, const S_msrSegmentElement& elt)
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
