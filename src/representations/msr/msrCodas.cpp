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

#include "msrCodas.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string codaKindAsString (
  msrCodaKind codaKind)
{
  string result;

  switch (codaKind) {
    case msrCodaKind::kCodaFirst:
      result = "kCodaFirst";
      break;
    case msrCodaKind::kCodaSecond:
      result = "kCodaSecond";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrCodaKind& elt)
{
  os << codaKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrCoda msrCoda::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure,
  int          staffNumber,
  msrCodaKind  codaKind)
{
  msrCoda* o =
    new msrCoda (
      inputLineNumber,
      upLinkToMeasure,
      staffNumber,
      codaKind);
  assert (o != nullptr);
  return o;
}

msrCoda::msrCoda (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure,
  int          staffNumber,
  msrCodaKind  codaKind)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{
  fStaffNumber = staffNumber;

  fCodaKind = codaKind;
}

msrCoda::~msrCoda ()
{}

void msrCoda::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrCoda::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCoda::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrCoda::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrCoda::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCoda::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrCoda::browseData (basevisitor* v)
{}

string msrCoda::asString () const
{
  stringstream s;

  s <<
    "Coda" <<
    ", staffNumber: " << fStaffNumber <<
    ", codaKind: " << codaKindAsString (fCodaKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrCoda::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrCoda& elt)
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
