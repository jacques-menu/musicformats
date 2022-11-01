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

#include "msrHiddenMeasureAndBarLines.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrHiddenMeasureAndBarLine msrHiddenMeasureAndBarLine::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos () || gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Creating hiddenMeasureAndBarLine" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  msrHiddenMeasureAndBarLine* o =
    new msrHiddenMeasureAndBarLine (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

msrHiddenMeasureAndBarLine::msrHiddenMeasureAndBarLine (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{}

msrHiddenMeasureAndBarLine::~msrHiddenMeasureAndBarLine ()
{}

void msrHiddenMeasureAndBarLine::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHiddenMeasureAndBarLine::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHiddenMeasureAndBarLine>*
    p =
      dynamic_cast<visitor<S_msrHiddenMeasureAndBarLine>*> (v)) {
        S_msrHiddenMeasureAndBarLine elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHiddenMeasureAndBarLine::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHiddenMeasureAndBarLine::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHiddenMeasureAndBarLine::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHiddenMeasureAndBarLine>*
    p =
      dynamic_cast<visitor<S_msrHiddenMeasureAndBarLine>*> (v)) {
        S_msrHiddenMeasureAndBarLine elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHiddenMeasureAndBarLine::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHiddenMeasureAndBarLine::browseData (basevisitor* v)
{}

string msrHiddenMeasureAndBarLine::asString () const
{
  stringstream s;

  s <<
    "HiddenMeasureAndBarLine" <<
    ", measurePosition: " << fMeasureElementMeasurePosition <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrHiddenMeasureAndBarLine::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrHiddenMeasureAndBarLine& elt)
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
