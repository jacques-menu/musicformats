/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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

#include "msrBarNumberChecks.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrBarNumberCheck msrBarNumberCheck::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string&  nextBarOriginalNumber,
  int                 nextBarPuristNumber)
{
  msrBarNumberCheck* obj =
    new msrBarNumberCheck (
      inputLineNumber,
      upLinkToMeasure,
      nextBarOriginalNumber,
      nextBarPuristNumber);
  assert (obj != nullptr);
  return obj;
}

S_msrBarNumberCheck msrBarNumberCheck::create (
  int                inputLineNumber,
  const std::string& nextBarOriginalNumber,
  int                nextBarPuristNumber)
{
  return
    msrBarNumberCheck::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  fNextBarPuristNumber = nextBarPuristNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
    std::stringstream ss;

    ss <<
      "Creating a bar number check" <<
      " with next bar original number '" <<
      nextBarOriginalNumber <<
      " and next bar purist number '" <<
      fNextBarPuristNumber <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrBarNumberCheck::~msrBarNumberCheck ()
{}

// void msrBarNumberCheck::setBarNumberCheckUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceBarNumberChecks ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of bar number check " <<
//       asString () <<
//       " to measure " << measure->asString () <<
//       "' in measure '" <<
//       measure->asString () <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fBarNumberCheckUpLinkToMeasure = measure;
// }

void msrBarNumberCheck::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBarNumberCheck::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBarNumberCheck::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrBarNumberCheck::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBarNumberCheck::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBarNumberCheck::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrBarNumberCheck::browseData (basevisitor* v)
{}

std::string msrBarNumberCheck::asString () const
{
  std::stringstream ss;

  ss <<
    "[BarNumberCheck" <<
    ", nextBarOriginalNumber = \"" << fNextBarOriginalNumber << "\"" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"" <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
