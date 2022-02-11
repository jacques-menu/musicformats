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

#include "msrRehearsals.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrRehearsal msrRehearsal::create (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  const string&    rehearsalText,
  msrPlacementKind rehearsalPlacementKind)
{
  msrRehearsal* o =
    new msrRehearsal (
      inputLineNumber,
      rehearsalKind,
      rehearsalText,
      rehearsalPlacementKind);
  assert (o != nullptr);
  return o;
}

msrRehearsal::msrRehearsal (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  const string&    rehearsalText,
  msrPlacementKind rehearsalPlacementKind)
    : msrMeasureElement (inputLineNumber)
{
  fRehearsalKind = rehearsalKind;

  fRehearsalText = rehearsalText;

  fRehearsalPlacementKind = rehearsalPlacementKind;
}

msrRehearsal::~msrRehearsal ()
{}

void msrRehearsal::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRehearsal::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRehearsal::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRehearsal::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRehearsal::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRehearsal::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRehearsal::browseData (basevisitor* v)
{}

string msrRehearsal::rehearsalKindAsString (
  msrRehearsalKind rehearsalKind)
{
  string result;

  switch (rehearsalKind) {
    case msrRehearsal::kNone:
      result = "kNone";
      break;
    case msrRehearsal::kRectangle:
      result = "kRectangle";
      break;
    case msrRehearsal::kOval:
      result = "kOval";
      break;
    case msrRehearsal::kCircle:
      result = "kCircle";
      break;
    case msrRehearsal::kBracket:
      result = "kBracket";
      break;
    case msrRehearsal::kTriangle:
      result = "kTriangle";
      break;
    case msrRehearsal::kDiamond:
      result = "kDiamond";
      break;
  } // switch

  return result;
}

void msrRehearsal::print (ostream& os) const
{
  os <<
    "Rehearsal" << " " << fRehearsalText <<
    " kind: " <<
    rehearsalKindAsString (fRehearsalKind) <<
    " rehearsalPlacementKind: " <<
    msrPlacementKindAsString (fRehearsalPlacementKind) <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrRehearsal& elt)
{
  elt->print (os);
  return os;
}


}
