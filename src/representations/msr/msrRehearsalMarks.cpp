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

#include "visitor.h"

#include "mfAssert.h"

#include "msrMeasureConstants.h"

#include "msrRehearsalMarks.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrRehearsalMark msrRehearsalMark::create (
  int                  inputLineNumber,
  S_msrMeasure&        upLinkToMeasure,
  msrRehearsalMarkKind rehearsalKind,
  const std::string&   rehearsalText,
  msrPlacementKind     rehearsalPlacementKind)
{
  msrRehearsalMark* o =
    new msrRehearsalMark (
      inputLineNumber,
      upLinkToMeasure,
      rehearsalKind,
      rehearsalText,
      rehearsalPlacementKind);
  assert (o != nullptr);
  return o;
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
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      rehearsalKind,
      rehearsalText,
      rehearsalPlacementKind);
}

msrRehearsalMark::msrRehearsalMark (
  int                  inputLineNumber,
  S_msrMeasure&        upLinkToMeasure,
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

void msrRehearsalMark::setRehearsalMarkUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of rehearsal mark " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fRehearsalMarkUpLinkToMeasure = measure;
}

void msrRehearsalMark::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRehearsalMark::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrRehearsalMark>*
    p =
      dynamic_cast<visitor<S_msrRehearsalMark>*> (v)) {
        S_msrRehearsalMark elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRehearsalMark::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrRehearsalMark::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRehearsalMark::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrRehearsalMark>*
    p =
      dynamic_cast<visitor<S_msrRehearsalMark>*> (v)) {
        S_msrRehearsalMark elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRehearsalMark::visitEnd ()" <<
            std::endl;
        }
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
      result = "kRectangle";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkOval:
      result = "kOval";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkCircle:
      result = "kCircle";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkBracket:
      result = "kBracket";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkTriangle:
      result = "kTriangle";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkDiamond:
      result = "kDiamond";
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
  std::stringstream s;

  s <<
    "[RehearsalMark" <<
    ", fRehearsalMarkText:" << fRehearsalMarkText <<
    " fRehearsalMarkKind: " << fRehearsalMarkKind <<
    " rehearsalPlacementKind: " << fRehearsalMarkPlacementKind <<
    ']';

  return s.str ();
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
