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
#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"



#include "mfAssert.h"

#include "msrLigatures.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrLigature msrLigature::create (
  int                    inputLineNumber,
  int                    ligatureNumber,
  msrLigatureKind        ligatureKind,
  msrLigatureLineEndKind ligatureLineEndKind,
  msrLineTypeKind        ligatureLineTypeKind,
  msrPlacementKind       ligaturePlacementKind)
{
  msrLigature* o =
    new msrLigature (
      inputLineNumber,
      ligatureNumber,
      ligatureKind,
      ligatureLineEndKind,
      ligatureLineTypeKind,
      ligaturePlacementKind);
  assert (o != nullptr);
  return o;
}

msrLigature::msrLigature (
  int                    inputLineNumber,
  int                    ligatureNumber,
  msrLigatureKind        ligatureKind,
  msrLigatureLineEndKind ligatureLineEndKind,
  msrLineTypeKind        ligatureLineTypeKind,
  msrPlacementKind       ligaturePlacementKind)
    : msrElement (inputLineNumber)
{
  fLigatureNumber        = ligatureNumber;
  fLigatureKind          = ligatureKind;
  fLigatureLineEndKind   = ligatureLineEndKind;
  fLigatureLineTypeKind  = ligatureLineTypeKind;
  fLigaturePlacementKind = ligaturePlacementKind;
}

msrLigature::~msrLigature ()
{}

void msrLigature::setLigatureSideLinkToOtherEnd (
  S_msrLigature sideLinkToOtherEnd)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    sideLinkToOtherEnd != nullptr,
    "sideLinkToOtherEnd is null");

  // set the two-way sideLink between both ends of the spanner
  fLigatureSideLinkToOtherEnd =
    sideLinkToOtherEnd;

  sideLinkToOtherEnd->fLigatureSideLinkToOtherEnd =
    this;
}

void msrLigature::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrLigature::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrLigature::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrLigature::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrLigature::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrLigature::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrLigature::browseData (basevisitor* v)
{}

string msrLigature::msrLigatureKindAsString (
  msrLigatureKind ligatureKind)
{
  string result;

  switch (ligatureKind) {
    case msrLigatureKind::kLigatureStart:
      result = "kLigatureStart";
      break;
    case msrLigatureKind::kLigatureContinue:
      result = "kLigatureContinue";
      break;
    case msrLigatureKind::kLigatureStop:
      result = "kLigatureStop";
      break;
    case kLigatureNone:
      result = "kLigatureNone";
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrLigatureKind& elt)
{
  os << msrLigatureKindAsString (elt);
  return os;
}

string msrLigature::msrLigatureLineEndKindAsString (
  msrLigatureLineEndKind ligatureLineEndKind)
{
  stringstream s;

  switch (ligatureLineEndKind) {
    case msrLigatureKind::kLigatureLineEndUp:
      s << "ligatureLineEndUp";
      break;
    case msrLigatureKind::kLigatureLineEndDown:
      s << "ligatureLineEndDown";
      break;
    case msrLigatureKind::kLigatureLineEndBoth:
      s << "ligatureLineEndBoth";
      break;
    case msrLigatureKind::kLigatureLineEndArrow:
      s << "ligatureLineEndArrow";
      break;
    case msrLigatureKind::kLigatureLineEndNone:
      s << "ligatureLineEndNone";
      break;
  } // switch

  return s.str ();
}

ostream& operator << (ostream& os, const msrLigatureLineEndKind& elt)
{
  os << msrLigatureLineEndKindAsString (elt);
  return os;
}

string msrLigature::msrLigatureKindAsString () const
{
  return msrLigatureKindAsString (fLigatureKind);
}

string msrLigature::asString () const
{
  stringstream s;

  s <<
    "[Ligature " << msrLigatureKindAsString () <<
    "fLigatureLineEndKind" << " : " << fLigatureLineEndKind <<
    "fLigatureLineTypeKind" << " : " << fLigatureLineTypeKind) <<
    "fLigaturePlacementKind" << " : " << fLigaturePlacementKind) <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrLigature::print (ostream& os) const
{
  os <<
    "[Ligature " << msrLigatureKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "ligatureLineEndKind" << " : " <<
    msrLigatureLineEndKindAsString (
      fLigatureLineEndKind) <<
    endl <<
    setw (fieldWidth) <<
    "ligatureLineTypeKind" << " : " <<
    msrLineTypeKindAsString (
      fLigatureLineTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "ligaturePlacementKind" << " : " <<
    msrPlacementKindAsString (
      fLigaturePlacementKind) <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrLigature& elt)
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
