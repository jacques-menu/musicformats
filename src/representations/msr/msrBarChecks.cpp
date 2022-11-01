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

#include "msrMeasures.h"

#include "msrBarChecks.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrBarCheck msrBarCheck::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
  msrBarCheck* o =
    new msrBarCheck (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrBarCheck msrBarCheck::createWithNextBarPuristNumber (
  int           inputLineNumber,
  S_msrMeasure  upLinkToMeasure,
  const string& nextBarOriginalNumber,
  int           nextBarPuristNumber)
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
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresNumbers ()) {
    gLogStream <<
      "Creating a bar check without next bar number" <<
      endl;
  }
#endif
}

msrBarCheck::msrBarCheck (
  int           inputLineNumber,
  S_msrMeasure  upLinkToMeasure,
  const string& nextBarOriginalNumber,
  int           nextBarPuristNumber)
    : msrMeasureElement (
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
      endl;
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
      endl;
  }
#endif

  fNextBarPuristNumber = puristNumber;
}

void msrBarCheck::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBarCheck::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarCheck::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBarCheck::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBarCheck::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBarCheck::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarCheck::browseData (basevisitor* v)
{}

string msrBarCheck::asString () const
{
  stringstream s;

  s <<
    "[BarCheck" <<
    ", nextBarOriginalNumber = \"" << fNextBarOriginalNumber << "\"" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"" <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrBarCheck::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrBarCheck& elt)
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
