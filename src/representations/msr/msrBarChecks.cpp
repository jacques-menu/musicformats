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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrBarChecks.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrBarCheck msrBarCheck::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrBarCheck* o =
    new msrBarCheck (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrBarCheck msrBarCheck::create (
  int                 inputLineNumber)
{
  return
    msrBarCheck::create (
      inputLineNumber,
      gNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
}

S_msrBarCheck msrBarCheck::createWithNextBarPuristNumber ( // JMI superflous??? v0.9.66
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string&  nextBarOriginalNumber,
  int                 nextBarPuristNumber)
{
  msrBarCheck* o =
    new msrBarCheck (
      inputLineNumber,
      upLinkToMeasure,
      nextBarOriginalNumber,
      nextBarPuristNumber);
  assert (o != nullptr);
  return o;
}

msrBarCheck::msrBarCheck (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElementLambda (
        inputLineNumber,
        upLinkToMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresNumbers ()) {
    gLogStream <<
      "Creating a bar check without next bar number" <<
      std::endl;
  }
#endif
}

msrBarCheck::msrBarCheck (
  int                inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string& nextBarOriginalNumber,
  int                nextBarPuristNumber)
    : msrMeasureElementLambda (
        inputLineNumber,
        upLinkToMeasure)
{
  fNextBarOriginalNumber = nextBarOriginalNumber;
  fNextBarPuristNumber   = nextBarPuristNumber;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresNumbers ()) {
    gLogStream <<
      "Creating a bar check" <<
      " with next bar original number '" <<
      nextBarOriginalNumber <<
      " and next bar purist number '" <<
      fNextBarPuristNumber <<
      "'" <<
      std::endl;
  }
#endif
}

msrBarCheck::~msrBarCheck ()
{}

void msrBarCheck::setNextBarPuristNumber (int puristNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresNumbers ()) {
    gLogStream <<
      "Setting bar check next bar number to '" <<
      puristNumber <<
      "'" <<
      std::endl;
  }
#endif

  fNextBarPuristNumber = puristNumber;
}

void msrBarCheck::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBarCheck::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarCheck::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrBarCheck::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBarCheck::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarCheck::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarCheck::browseData (basevisitor* v)
{}

std::string msrBarCheck::asString () const
{
  std::stringstream s;

  s <<
    "[BarCheck" <<
    ", nextBarOriginalNumber = \"" << fNextBarOriginalNumber << "\"" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"" <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrBarCheck::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrBarCheck& elt)
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
