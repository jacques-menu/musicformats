/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrCodas.h"

#include "waeHandlers.h"


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
  msrCoda* obj =
    new msrCoda (
      inputLineNumber,
      upLinkToMeasure,
      staffNumber,
      codaKind);
  assert (obj != nullptr);
  return obj;
}

S_msrCoda msrCoda::create (
  int                 inputLineNumber,
  int                 staffNumber,
  msrCodaKind         codaKind)
{
  return
    msrCoda::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCodas ()) {
    ++gIndenter;

    gLog <<
      "Setting the uplink to measure of coda " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fCodaUpLinkToMeasure = measure;
}


void msrCoda::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrCoda::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrCoda::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrCoda::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrCoda::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrCoda::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrCoda::browseData (basevisitor* v)
{}

std::string msrCoda::asString () const
{
  std::stringstream ss;

  ss <<
    "Coda" <<
    ", staffNumber: " << fStaffNumber <<
    ", codaKind: " << msrCodaKindAsString (fCodaKind) <<
    ", line " << fInputLineNumber;

  return ss.str ();
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
