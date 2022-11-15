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

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrBarNumberChecks.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrBarNumberCheck msrBarNumberCheck::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string&  nextBarOriginalNumber,
  int                 nextBarPuristNumber)
{
  msrBarNumberCheck* o =
    new msrBarNumberCheck (
      inputLineNumber,
      upLinkToMeasure,
      nextBarOriginalNumber,
      nextBarPuristNumber);
  assert (o != nullptr);
  return o;
}

S_msrBarNumberCheck msrBarNumberCheck::create (
  int                 inputLineNumber,
  const std::string&  nextBarOriginalNumber,
  int                 nextBarPuristNumber)
{
  return
    msrBarNumberCheck::create (
      inputLineNumber,
      gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      nextBarOriginalNumber,
      nextBarPuristNumber);
}

msrBarNumberCheck::msrBarNumberCheck (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string&  nextBarOriginalNumber,
  int                 nextBarPuristNumber)
    : msrMeasureElement (
        inputLineNumber)
{
  fNextBarOriginalNumber = nextBarOriginalNumber;
  fNextBarPuristNumber   = nextBarPuristNumber;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresNumbers ()) {
    gLogStream <<
      "Creating a bar number check" <<
      " with next bar original number '" <<
      nextBarOriginalNumber <<
      " and next bar purist number '" <<
      fNextBarPuristNumber <<
      "'" <<
      std::endl;
  }
#endif
}

msrBarNumberCheck::~msrBarNumberCheck ()
{}

void msrBarNumberCheck::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBarNumberCheck::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarNumberCheck::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrBarNumberCheck::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBarNumberCheck::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarNumberCheck::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarNumberCheck::browseData (basevisitor* v)
{}

std::string msrBarNumberCheck::asString () const
{
  std::stringstream s;

  s <<
    "[BarNumberCheck" <<
    ", nextBarOriginalNumber = \"" << fNextBarOriginalNumber << "\"" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"" <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrBarNumberCheck::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrBarNumberCheck& elt)
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
