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
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"



#include "mfAssert.h"

#include "msrLigatures.h"

#include "oahOah.h"

#include "msrOah.h"


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
  const S_msrLigature& sideLinkToOtherEnd)
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
      std::endl;
  }

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrLigature::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrLigature::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrLigature::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrLigature::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}


void msrLigature::browseData (basevisitor* v)
{}

std::string msrLigatureKindAsString (
  msrLigatureKind ligatureKind)
{
  std::string result;

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
    case msrLigatureKind::kLigatureNone:
      result = "kLigatureNone";
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrLigatureKind& elt)
{
  os << msrLigatureKindAsString (elt);
  return os;
}

std::string msrLigatureLineEndKindAsString (
  msrLigatureLineEndKind ligatureLineEndKind)
{
  std::stringstream s;

  switch (ligatureLineEndKind) {
    case msrLigatureLineEndKind::kLigatureLineEndUp:
      s << "ligatureLineEndUp";
      break;
    case msrLigatureLineEndKind::kLigatureLineEndDown:
      s << "ligatureLineEndDown";
      break;
    case msrLigatureLineEndKind::kLigatureLineEndBoth:
      s << "ligatureLineEndBoth";
      break;
    case msrLigatureLineEndKind::kLigatureLineEndArrow:
      s << "ligatureLineEndArrow";
      break;
    case msrLigatureLineEndKind::kLigatureLineEndNone:
      s << "ligatureLineEndNone";
      break;
  } // switch

  return s.str ();
}

std::ostream& operator << (std::ostream& os, const msrLigatureLineEndKind& elt)
{
  os << msrLigatureLineEndKindAsString (elt);
  return os;
}

std::string msrLigature::asString () const
{
  std::stringstream s;

  s <<
    "[Ligature" <<
    ", fLigatureKind: " << fLigatureKind <<
    ", fLigatureLineEndKind" << " : " << fLigatureLineEndKind <<
    ", fLigatureLineTypeKind" << " : " << fLigatureLineTypeKind <<
    ", fLigaturePlacementKind" << " : " << fLigaturePlacementKind <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrLigature::print (std::ostream& os) const
{
  os <<
    "[Ligature" <<
    ", fLigatureKind: " << fLigatureKind <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fLigatureLineEndKind" << " : " << fLigatureLineEndKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLigatureLineTypeKind" << " : " << fLigatureLineTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLigaturePlacementKind" << " : " << fLigaturePlacementKind <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrLigature& elt)
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
