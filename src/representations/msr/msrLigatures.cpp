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

void msrLigature::setLigatureOtherEndSideLink (
  S_msrLigature otherEndSideLink)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    otherEndSideLink != nullptr,
    "otherEndSideLink is null");

  // set the two-way sideLink between both ends of the spanner
  fLigatureOtherEndSideLink =
    otherEndSideLink;

  otherEndSideLink->fLigatureOtherEndSideLink =
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

string msrLigature::ligatureKindAsString (
  msrLigatureKind ligatureKind)
{
  stringstream s;

  switch (ligatureKind) {
    case msrLigature::kLigatureStart:
      s << "ligatureStart";
      break;
    case msrLigature::kLigatureContinue:
      s << "ligatureContinue";
      break;
    case msrLigature::kLigatureStop:
      s << "ligatureStop";
      break;
    case kLigatureNone:
      s << "ligatureNone";
  } // switch

  return s.str ();

}

string msrLigature::ligatureLineEndKindAsString (
  msrLigatureLineEndKind ligatureLineEndKind)
{
  stringstream s;

  switch (ligatureLineEndKind) {
    case msrLigature::kLigatureLineEndUp:
      s << "ligatureLineEndUp";
      break;
    case msrLigature::kLigatureLineEndDown:
      s << "ligatureLineEndDown";
      break;
    case msrLigature::kLigatureLineEndBoth:
      s << "ligatureLineEndBoth";
      break;
    case msrLigature::kLigatureLineEndArrow:
      s << "ligatureLineEndArrow";
      break;
    case msrLigature::kLigatureLineEndNone:
      s << "ligatureLineEndNone";
      break;
  } // switch

  return s.str ();

}

string msrLigature::ligatureKindAsString () const
{
  return ligatureKindAsString (fLigatureKind);
}

void msrLigature::print (ostream& os) const
{
  os <<
    "[Ligature " << ligatureKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "ligatureLineEndKind" << " : " <<
    ligatureLineEndKindAsString (
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

ostream& operator<< (ostream& os, const S_msrLigature& elt)
{
  elt->print (os);
  return os;
}


}
