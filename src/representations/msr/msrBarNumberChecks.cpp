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

#include "msrBarNumberChecks.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrBarNumberCheck msrBarNumberCheck::create (
  int           inputLineNumber,
  const string& nextBarOriginalNumber,
  int           nextBarPuristNumber)
{
  msrBarNumberCheck* o =
    new msrBarNumberCheck (
      inputLineNumber,
      nextBarOriginalNumber,
      nextBarPuristNumber);
  assert (o != nullptr);
  return o;
}

msrBarNumberCheck::msrBarNumberCheck (
  int           inputLineNumber,
  const string& nextBarOriginalNumber,
  int           nextBarPuristNumber)
    : msrMeasureElement (inputLineNumber)
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
      endl;
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
      endl;
  }

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarNumberCheck::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBarNumberCheck::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBarNumberCheck::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarNumberCheck::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarNumberCheck::browseData (basevisitor* v)
{}

string msrBarNumberCheck::asString () const
{
  stringstream s;

  s <<
    "[BarNumberCheck" <<
    ", nextBarOriginalNumber = \"" << fNextBarOriginalNumber << "\"" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"" <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrBarNumberCheck::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrBarNumberCheck& elt)
{
  elt->print (os);
  return os;
}


}
