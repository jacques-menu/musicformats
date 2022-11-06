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

string msrOrnamentKindAsString (
  msrOrnamentKind ornamentKind)
{
  string result;

  switch (ornamentKind) {
    case msrOrnamentKind::kOrnamentTrill:
      result = "kOrnamentTrill";
      break;
      /* JMI
    case msrOrnamentKind::kOrnamentDashes:
      result = "kOrnamentDashes";
      break;
      */
    case msrOrnamentKind::kOrnamentTurn:
      result = "kOrnamentTurn";
      break;
    case msrOrnamentKind::kOrnamentInvertedTurn:
      result = "kOrnamentInvertedTurn";
      break;
    case msrOrnamentKind::kOrnamentDelayedTurn:
      result = "kOrnamentDelayedTurn";
      break;
    case msrOrnamentKind::kOrnamentDelayedInvertedTurn:
      result = "kOrnamentDelayedInvertedTurn";
      break;
    case msrOrnamentKind::kOrnamentVerticalTurn:
      result = "kOrnamentVerticalTurn";
      break;
    case msrOrnamentKind::kOrnamentMordent:
      result = "kOrnamentMordent";
      break;
    case msrOrnamentKind::kOrnamentInvertedMordent:
      result = "kOrnamentInvertedMordent";
      break;
    case msrOrnamentKind::kOrnamentSchleifer:
      result = "kOrnamentSchleifer";
      break;
    case msrOrnamentKind::kOrnamentShake:
      result = "kOrnamentShake";
      break;
    case msrOrnamentKind::kOrnamentAccidentalKind:
      result = "kOrnamentAccidentalKind";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrOrnamentKind& elt)
{
  os << msrOrnamentKindAsString (elt);
  return os;
}

string msrOrnament::asString () const
{
  stringstream s;

  s <<
    "[Ornament" <<
    ", fOrnamentPlacementKind: " << msrPlacementKindAsString (fOrnamentPlacementKind)  <<
    ", fOrnamentPlacementKind: " <<
    msrPlacementKindAsString (fOrnamentPlacementKind) <<
    ", fOrnamentAccidentalKind: " <<
    msrAccidentalKindAsString (fOrnamentAccidentalKind) <<
    ']';

  return s.str ();
}


void msrOrnament::print (ostream& os) const
{
  os <<
    "[Ornament" <<
    ", " << msrPlacementKindAsString (fOrnamentPlacementKind)  <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 16;

  os <<
    setw (fieldWidth) <<
    "fOrnamentPlacementKind" << " : " <<
    msrPlacementKindAsString (fOrnamentPlacementKind) <<
    endl <<
    setw (fieldWidth) <<
    "fOrnamentAccidentalKind" << " : " <<
    msrAccidentalKindAsString (fOrnamentAccidentalKind) <<
    endl <<
    setw (fieldWidth) <<
    "fOrnamentUpLinkToNote" << " : " <<
    fOrnamentUpLinkToNote->asShortString () <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrOrnament& elt)
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
