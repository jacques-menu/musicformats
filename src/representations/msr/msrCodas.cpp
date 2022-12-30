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
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      staffNumber,
      codaKind);
}

msrCoda::msrCoda (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int                 staffNumber,
  msrCodaKind         codaKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fStaffNumber = staffNumber;

  fCodaKind = codaKind;
}

msrCoda::~msrCoda ()
{}

void msrCoda::setCodaUpLinkToMeasure (
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
      "==> Setting the uplink to measure of coda " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fCodaUpLinkToMeasure = measure;
}


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
