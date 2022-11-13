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
    if might be necessary for the the following two to be strictly in this order
    depending on the C++ compiler
*/
#include "msrMeasures.h"
#include "msrMeasureElementLambda.h"

#include "msrMoments.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrMeasureElementLambda::msrMeasureElementLambda (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
    : msrMeasureElement (inputLineNumber)
//       fMeasureElementMeasureMoment (
//         msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION),
//       fMeasureElementVoiceMoment (
//         msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION)
{
  fMeasureElementLambdaUpLinkToMeasure = upLinkToMeasure;
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

void msrMeasureElementLambda::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrMeasureElementLambda::browseData ()" <<
      std::endl;
  }
#endif
}

void msrMeasureElementLambda::setMeasureElementUpLinkToMeasure (
  S_msrMeasure measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

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

  fMeasureElementLambdaUpLinkToMeasure = measure;
}

S_msrMeasure msrMeasureElementLambda::fetchMeasureElementUpLinkToMeasure () const
{
  return fMeasureElementLambdaUpLinkToMeasure;
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
