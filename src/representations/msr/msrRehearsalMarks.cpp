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

#include "mfPreprocessorSettings.h"

#include "visitor.h"

#include "mfAssert.h"

#include "msrMeasureConstants.h"

#include "msrRehearsalMarks.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrRehearsalMark msrRehearsalMark::create (
  int                  inputLineNumber,
  const S_msrMeasure&  upLinkToMeasure,
  msrRehearsalMarkKind rehearsalKind,
  const std::string&   rehearsalText,
  msrPlacementKind     rehearsalPlacementKind)
{
  msrRehearsalMark* obj =
    new msrRehearsalMark (
      inputLineNumber,
      upLinkToMeasure,
      rehearsalKind,
      rehearsalText,
      rehearsalPlacementKind);
  assert (obj != nullptr);
  return obj;
}

S_msrRehearsalMark msrRehearsalMark::create (
  int                  inputLineNumber,
  msrRehearsalMarkKind rehearsalKind,
  const std::string&   rehearsalText,
  msrPlacementKind     rehearsalPlacementKind)
{
  return
    msrRehearsalMark::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      rehearsalKind,
      rehearsalText,
      rehearsalPlacementKind);
}

msrRehearsalMark::msrRehearsalMark (
  int                  inputLineNumber,
  const S_msrMeasure&  upLinkToMeasure,
  msrRehearsalMarkKind rehearsalKind,
  const std::string&   rehearsalText,
  msrPlacementKind     rehearsalPlacementKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fRehearsalMarkKind = rehearsalKind;

  fRehearsalMarkText = rehearsalText;

  fRehearsalMarkPlacementKind = rehearsalPlacementKind;
}

msrRehearsalMark::~msrRehearsalMark ()
{}

// void msrRehearsalMark::setRehearsalMarkUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measure != nullptr,
//     "measure is null");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRehearsalMarks ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of rehearsal mark " <<
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
//   fRehearsalMarkUpLinkToMeasure = measure;
// }

void msrRehearsalMark::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRehearsalMark::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRehearsalMark>*
    p =
      dynamic_cast<visitor<S_msrRehearsalMark>*> (v)) {
        S_msrRehearsalMark elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRehearsalMark::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrRehearsalMark::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRehearsalMark::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRehearsalMark>*
    p =
      dynamic_cast<visitor<S_msrRehearsalMark>*> (v)) {
        S_msrRehearsalMark elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRehearsalMark::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrRehearsalMark::browseData (basevisitor* v)
{}

std::string msrRrehearsalKindAsString (
  msrRehearsalMarkKind rehearsalKind)
{
  std::string result;

  switch (rehearsalKind) {
    case msrRehearsalMarkKind::kRehearsalMarkNone:
      result = "kNone";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkRectangle:
      result = "kRehearsalMarkRectangle";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkSquare:
      result = "kRehearsalMarkSquare";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkOval:
      result = "kRehearsalMarkOval";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkCircle:
      result = "kRehearsalMarkCircle";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkBracket:
      result = "kRehearsalMarkBracket";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkTriangle:
      result = "kRehearsalMarkTriangle";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkDiamond:
      result = "kRehearsalMarkDiamond";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkPentagon:
      result = "kRehearsalMarkPentagon";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkHexagon:
      result = "kRehearsalMarkHexagon";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkHeptagon:
      result = "kRehearsalMarkHeptagon";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkOctagon:
      result = "kRehearsalMarkOctagon";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkNonagon:
      result = "kRehearsalMarkNonagon";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkDecagon:
      result = "kRehearsalMarkDecagon";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrRehearsalMarkKind& elt)
{
  os << msrRrehearsalKindAsString (elt);
  return os;
}

std::string msrRehearsalMark::asString () const
{
  std::stringstream ss;

  ss <<
    "[RehearsalMark" <<
    ", fRehearsalMarkText:" << fRehearsalMarkText <<
    " fRehearsalMarkKind: " << fRehearsalMarkKind <<
    " rehearsalPlacementKind: " << fRehearsalMarkPlacementKind <<
    ']';

  return ss.str ();
}

void msrRehearsalMark::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrRehearsalMark& elt)
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
