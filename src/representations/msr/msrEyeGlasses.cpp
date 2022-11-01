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
#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"

#include "msrMeasures.h"

#include "msrEyeGlasses.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrEyeGlasses msrEyeGlasses::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
  msrEyeGlasses* o =
    new msrEyeGlasses (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

msrEyeGlasses::msrEyeGlasses (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
    : msrMeasureElement (
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
      endl;
  }

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrEyeGlasses::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrEyeGlasses::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrEyeGlasses::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrEyeGlasses::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrEyeGlasses::browseData (basevisitor* v)
{}

string msrEyeGlasses::asString () const
{
  stringstream s;

  s <<
    "EyeGlasses" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrEyeGlasses::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrEyeGlasses& elt)
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
