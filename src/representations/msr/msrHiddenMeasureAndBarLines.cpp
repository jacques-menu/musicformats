/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfAssert.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrHiddenMeasureAndBarLines.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrHiddenMeasureAndBarLine msrHiddenMeasureAndBarLine::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating hiddenMeasureAndBarLine" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  msrHiddenMeasureAndBarLine* o =
    new msrHiddenMeasureAndBarLine (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrHiddenMeasureAndBarLine msrHiddenMeasureAndBarLine::create (
  int                 inputLineNumber)
{
  return
    msrHiddenMeasureAndBarLine::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
}

msrHiddenMeasureAndBarLine::msrHiddenMeasureAndBarLine (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{}

msrHiddenMeasureAndBarLine::~msrHiddenMeasureAndBarLine ()
{}

void msrHiddenMeasureAndBarLine::setHiddenMeasureAndBarLineUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of hidden measure and bar line " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fHiddenMeasureAndBarLineUpLinkToMeasure = measure;
}

void msrHiddenMeasureAndBarLine::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHiddenMeasureAndBarLine::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrHiddenMeasureAndBarLine>*
    p =
      dynamic_cast<visitor<S_msrHiddenMeasureAndBarLine>*> (v)) {
        S_msrHiddenMeasureAndBarLine elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHiddenMeasureAndBarLine::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrHiddenMeasureAndBarLine::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHiddenMeasureAndBarLine::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrHiddenMeasureAndBarLine>*
    p =
      dynamic_cast<visitor<S_msrHiddenMeasureAndBarLine>*> (v)) {
        S_msrHiddenMeasureAndBarLine elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHiddenMeasureAndBarLine::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrHiddenMeasureAndBarLine::browseData (basevisitor* v)
{}

std::string msrHiddenMeasureAndBarLine::asString () const
{
  std::stringstream s;

  s <<
    "HiddenMeasureAndBarLine" <<
    ", measurePosition: " << fMeasureElementMeasurePosition <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrHiddenMeasureAndBarLine::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrHiddenMeasureAndBarLine& elt)
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
