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

#include "msrCodas.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrCodaKindAsString (
  msrCodaKind codaKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrCodaKind& elt)
{
  os << msrCodaKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrCoda msrCoda::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int                 staffNumber,
  msrCodaKind         codaKind)
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

S_msrCoda msrCoda::create (
  int                 inputLineNumber,
  int                 staffNumber,
  msrCodaKind         codaKind)
{
  return
    msrCoda::create (
      inputLineNumber,
      gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      staffNumber,
      codaKind);
}

msrCoda::msrCoda (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int           staffNumber,
  msrCodaKind   codaKind)
    : msrMeasureElementLambda (
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
      std::endl;
  }

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCoda::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrCoda::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrCoda::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrCoda::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrCoda::browseData (basevisitor* v)
{}

std::string msrCoda::asString () const
{
  std::stringstream s;

  s <<
    "Coda" <<
    ", staffNumber: " << fStaffNumber <<
    ", codaKind: " << msrCodaKindAsString (fCodaKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrCoda::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrCoda& elt)
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
