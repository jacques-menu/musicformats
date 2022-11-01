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

#include "msrMeasures.h"

#include "msrRehearsalMarks.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrRehearsalMark msrRehearsalMark::create (
  int                  inputLineNumber,
  S_msrMeasure         upLinkToMeasure,
  msrRehearsalMarkKind rehearsalKind,
  const string&        rehearsalText,
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

msrRehearsalMark::msrRehearsalMark (
  int                  inputLineNumber,
  S_msrMeasure         upLinkToMeasure,
  msrRehearsalMarkKind rehearsalKind,
  const string&        rehearsalText,
  msrPlacementKind     rehearsalPlacementKind)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{
  fRehearsalMarkKind = rehearsalKind;

  fRehearsalMarkText = rehearsalText;

  fRehearsalMarkPlacementKind = rehearsalPlacementKind;
}

msrRehearsalMark::~msrRehearsalMark ()
{}

void msrRehearsalMark::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRehearsalMark::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRehearsalMark>*
    p =
      dynamic_cast<visitor<S_msrRehearsalMark>*> (v)) {
        S_msrRehearsalMark elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRehearsalMark::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRehearsalMark::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRehearsalMark::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRehearsalMark>*
    p =
      dynamic_cast<visitor<S_msrRehearsalMark>*> (v)) {
        S_msrRehearsalMark elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRehearsalMark::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRehearsalMark::browseData (basevisitor* v)
{}

string msrRehearsalMark::rehearsalKindAsString (
  msrRehearsalMarkKind rehearsalKind)
{
  string result;

  switch (rehearsalKind) {
    case msrRehearsalMark::kNone:
      result = "kNone";
      break;
    case msrRehearsalMark::kRectangle:
      result = "kRectangle";
      break;
    case msrRehearsalMark::kOval:
      result = "kOval";
      break;
    case msrRehearsalMark::kCircle:
      result = "kCircle";
      break;
    case msrRehearsalMark::kBracket:
      result = "kBracket";
      break;
    case msrRehearsalMark::kTriangle:
      result = "kTriangle";
      break;
    case msrRehearsalMark::kDiamond:
      result = "kDiamond";
      break;
  } // switch

  return result;
}

void msrRehearsalMark::print (ostream& os) const
{
  os <<
    "RehearsalMark" << ' ' << fRehearsalMarkText <<
    " kind: " <<
    rehearsalKindAsString (fRehearsalMarkKind) <<
    " rehearsalPlacementKind: " <<
    placementKindAsString (fRehearsalMarkPlacementKind) <<
    endl;
}

ostream& operator << (ostream& os, const S_msrRehearsalMark& elt)
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
