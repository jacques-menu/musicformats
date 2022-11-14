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

#include "oahOah.h"

#include "msrOah.h"

#include "msrMeasures.h"

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
      gNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
}

msrEyeGlasses::msrEyeGlasses (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElementLambda (
        inputLineNumber,
        upLinkToMeasure)
{}

msrEyeGlasses::~msrEyeGlasses ()
{}

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
