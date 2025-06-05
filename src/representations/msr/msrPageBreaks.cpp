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

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"

#include "msrPageBreaks.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrUserSelectedPageBreakKindAsString (
  msrUserSelectedPageBreakKind userSelectedPageBreakKind)
{
  std::string result;

  switch (userSelectedPageBreakKind) {
    case msrUserSelectedPageBreakKind::kUserSelectedPageBreakYes:
      result = "kUserSelectedPageBreakYes";
      break;
    case msrUserSelectedPageBreakKind::kUserSelectedPageBreakNo:
      result = "kUserSelectedPageBreakNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrPageBreak msrPageBreak::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int                 nextBarPuristNumber,
  msrUserSelectedPageBreakKind
                      userSelectedPageBreakKind)
{
  msrPageBreak* obj =
    new msrPageBreak (
      inputLineNumber,
      upLinkToMeasure,
      nextBarPuristNumber,
      userSelectedPageBreakKind);
  assert (obj != nullptr);
  return obj;
}

S_msrPageBreak msrPageBreak::create (
  const mfInputLineNumber& inputLineNumber,
  int                 nextBarPuristNumber,
  msrUserSelectedPageBreakKind
                      userSelectedPageBreakKind)
{
  return
    msrPageBreak::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      nextBarPuristNumber,
      userSelectedPageBreakKind);
}

msrPageBreak::msrPageBreak (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int                 nextBarPuristNumber,
  msrUserSelectedPageBreakKind
                      userSelectedPageBreakKind)
    : msrMeasureElement (
        inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePageBreaks ()) {
    std::stringstream ss;

    ss <<
      "Constructing a page break" <<
      ", fUserSelectedPageBreakKind: " <<
      msrUserSelectedPageBreakKindAsString (
        fUserSelectedPageBreakKind) <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNextBarPuristNumber = nextBarPuristNumber;

  fUserSelectedPageBreakKind = userSelectedPageBreakKind;
}

msrPageBreak::~msrPageBreak ()
{}

// void msrPageBreak::setPageBreakUpLinkToMeasure (
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
//   if (gTraceOahGroup->getTracePageBreaks ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of page break " <<
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
//   fPageBreakUpLinkToMeasure = measure;
// }

void msrPageBreak::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPageBreak::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPageBreak::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrPageBreak::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPageBreak::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPageBreak::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrPageBreak::browseData (basevisitor* v)
{}

std::string msrPageBreak::asString () const
{
  std::stringstream ss;

  ss <<
    "[PageBreak" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"" <<
    ", fUserSelectedPageBreakKind: " <<
    msrUserSelectedPageBreakKindAsString (
      fUserSelectedPageBreakKind) <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrPageBreak::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrPageBreak& elt)
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
