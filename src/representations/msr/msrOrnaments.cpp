/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "msrOrnaments.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrOrnament msrOrnament::create (
  int              inputLineNumber,
  msrOrnamentKind  ornamentKind,
  msrPlacementKind ornamentPlacementKind)
{
  msrOrnament* o =
    new msrOrnament (
      inputLineNumber, ornamentKind, ornamentPlacementKind);
  assert (o != nullptr);
  return o;
}

msrOrnament::msrOrnament (
  int              inputLineNumber,
  msrOrnamentKind  ornamentKind,
  msrPlacementKind ornamentPlacementKind)
    : msrElement (inputLineNumber)
{
  fOrnamentKind = ornamentKind;

  fOrnamentPlacementKind = ornamentPlacementKind;

  fOrnamentAccidentalKind = msrAccidentalKind::kAccidentalNone;
}

msrOrnament::~msrOrnament ()
{}

void msrOrnament::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrOrnament::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrOrnament::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOrnament::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrOrnament::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrOrnament::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrOrnament::browseData (basevisitor* v)
{}

string msrOrnament::ornamentKindAsString (
  msrOrnamentKind ornamentKind)
{
  string result;

  switch (ornamentKind) {
    case msrOrnament::kOrnamentTrill:
      result = "kOrnamentTrill";
      break;
      /* JMI
    case msrOrnament::kOrnamentDashes:
      result = "kOrnamentDashes";
      break;
      */
    case msrOrnament::kOrnamentTurn:
      result = "kOrnamentTurn";
      break;
    case msrOrnament::kOrnamentInvertedTurn:
      result = "kOrnamentInvertedTurn";
      break;
    case msrOrnament::kOrnamentDelayedTurn:
      result = "kOrnamentDelayedTurn";
      break;
    case msrOrnament::kOrnamentDelayedInvertedTurn:
      result = "kOrnamentDelayedInvertedTurn";
      break;
    case msrOrnament::kOrnamentVerticalTurn:
      result = "kOrnamentVerticalTurn";
      break;
    case msrOrnament::kOrnamentMordent:
      result = "kOrnamentMordent";
      break;
    case msrOrnament::kOrnamentInvertedMordent:
      result = "kOrnamentInvertedMordent";
      break;
    case msrOrnament::kOrnamentSchleifer:
      result = "kOrnamentSchleifer";
      break;
    case msrOrnament::kOrnamentShake:
      result = "kOrnamentShake";
      break;
    case msrOrnament::kOrnamentAccidentalKind:
      result = "kOrnamentAccidentalKind";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentKindAsString () const
{
  return ornamentKindAsString (fOrnamentKind);
}

string msrOrnament::ornamentPlacementKindAsString () const
{
   return
    msrPlacementKindAsString (
      fOrnamentPlacementKind);
}

string msrOrnament::ornamentAccidentalKindAsString () const
{
  return
    msrAccidentalKindAsString (
      fOrnamentAccidentalKind);
}

string msrOrnament::asString () const
{
  stringstream s;

  s <<
    "[Ornament" <<
    ", ornamentKind: " << ornamentKindAsString () <<
    ", ornamentPlacementKind: " << msrPlacementKindAsString (fOrnamentPlacementKind) <<
    ", ornamentAccidentalKind: " << msrAccidentalKindAsString (fOrnamentAccidentalKind) <<
    "]";

  return s.str ();
}


void msrOrnament::print (ostream& os) const
{
  os <<
    "[Ornament" <<
    ", " << ornamentKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 16;

  os <<
    setw (fieldWidth) <<
    "placement" << " : " <<
    ornamentPlacementKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "accidental mark" << " : " <<
    ornamentAccidentalKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "noteUpLink" << " : " <<
    fOrnamentNoteUpLink->asShortString () <<
    endl;

  --gIndenter;

  os << "]" << endl;
}

ostream& operator<< (ostream& os, const S_msrOrnament& elt)
{
  elt->print (os);
  return os;
}


}
