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

#include "msrLineBreaks.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrUserSelectedLineBreakKindAsString (
  msrUserSelectedLineBreakKind userSelectedLineBreakKind)
{
  std::string result;

  switch (userSelectedLineBreakKind) {
    case msrUserSelectedLineBreakKind::kUserSelectedLineBreakYes:
      result = "kUserSelectedLineBreakYes";
      break;
    case msrUserSelectedLineBreakKind::kUserSelectedLineBreakNo:
      result = "kUserSelectedLineBreakNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrLineBreak msrLineBreak::create (
  const mfInputLineNumber& inputLineNumber,
  msrUserSelectedLineBreakKind
                     userSelectedLineBreakKind)
{
  // JMI 0.9.74 the next bar purist number will be set in msr2lpsr
  return
    msrLineBreak::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      0,
      userSelectedLineBreakKind);
}

S_msrLineBreak msrLineBreak::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int                 nextBarPuristNumber,
  msrUserSelectedLineBreakKind
                      userSelectedLineBreakKind)
{
  msrLineBreak* obj =
    new msrLineBreak (
      inputLineNumber,
      upLinkToMeasure,
      nextBarPuristNumber,
      userSelectedLineBreakKind);
  assert (obj != nullptr);
  return obj;
}

S_msrLineBreak msrLineBreak::create (
  const mfInputLineNumber& inputLineNumber,
  int                nextBarPuristNumber,
  msrUserSelectedLineBreakKind
                     userSelectedLineBreakKind)
{
  return
    msrLineBreak::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      nextBarPuristNumber,
      userSelectedLineBreakKind);
}

msrLineBreak::msrLineBreak (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int                 nextBarPuristNumber,
  msrUserSelectedLineBreakKind
                      userSelectedLineBreakKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fNextBarPuristNumber = nextBarPuristNumber;

  fUserSelectedLineBreakKind = userSelectedLineBreakKind;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLineBreaks ()) {
    std::stringstream ss;

    ss <<
      "Constructing a break before measure " << fNextBarPuristNumber <<
      ", fUserSelectedLineBreakKind: " <<
      msrUserSelectedLineBreakKindAsString (
        fUserSelectedLineBreakKind) <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrLineBreak::~msrLineBreak ()
{}

// void msrLineBreak::setLineBreakUpLinkToMeasure (
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
//   if (gTraceOahGroup->getTraceLineBreaks ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of line break " <<
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
//   fLineBreakUpLinkToMeasure = measure;
// }

void msrLineBreak::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrLineBreak::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrLineBreak::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrLineBreak::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrLineBreak::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrLineBreak::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrLineBreak::browseData (basevisitor* v)
{}

std::string msrLineBreak::asString () const
{
  std::stringstream ss;

  ss <<
    "[LineBreak" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"" <<
    ", fUserSelectedLineBreakKind: " <<
    msrUserSelectedLineBreakKindAsString (
      fUserSelectedLineBreakKind) <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrLineBreak::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrLineBreak& elt)
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
