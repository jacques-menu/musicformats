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
#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfPreprocessorSettings.h"

#include "visitor.h"

#include "mfAssert.h"

#include "msrLigatures.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrLigature msrLigature::create (
  const mfInputLineNumber& inputLineNumber,
  int                    ligatureNumber,
  msrLigatureKind        ligatureKind,
  msrLigatureLineEndKind ligatureLineEndKind,
  msrLineTypeKind        ligatureLineTypeKind,
  msrPlacementKind       ligaturePlacementKind)
{
  msrLigature* obj =
    new msrLigature (
      inputLineNumber,
      ligatureNumber,
      ligatureKind,
      ligatureLineEndKind,
      ligatureLineTypeKind,
      ligaturePlacementKind);
  assert (obj != nullptr);
  return obj;
}

msrLigature::msrLigature (
  const mfInputLineNumber& inputLineNumber,
  int                    ligatureNumber,
  msrLigatureKind        ligatureKind,
  msrLigatureLineEndKind ligatureLineEndKind,
  msrLineTypeKind        ligatureLineTypeKind,
  msrPlacementKind       ligaturePlacementKind)
    : msrElement (inputLineNumber)
{
  fLigatureNumber = ligatureNumber;
  fLigatureKind = ligatureKind;
  fLigatureLineEndKind = ligatureLineEndKind;
  fLigatureLineTypeKind = ligatureLineTypeKind;
  fLigaturePlacementKind = ligaturePlacementKind;
}

msrLigature::~msrLigature ()
{}

void msrLigature::setLigatureSideLinkToOtherEnd (
  const S_msrLigature& sideLinkToOtherEnd)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    sideLinkToOtherEnd != nullptr,
    "sideLinkToOtherEnd is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set the two-way sideLink between both ends of the spanner
  fLigatureSideLinkToOtherEnd =
    sideLinkToOtherEnd;

  sideLinkToOtherEnd->fLigatureSideLinkToOtherEnd =
    this;
}

void msrLigature::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrLigature::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrLigature::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrLigature::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrLigature::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrLigature::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
  std::stringstream ss;

  switch (ligatureLineEndKind) {
    case msrLigatureLineEndKind::kLigatureLineEndUp:
      ss << "ligatureLineEndUp";
      break;
    case msrLigatureLineEndKind::kLigatureLineEndDown:
      ss << "ligatureLineEndDown";
      break;
    case msrLigatureLineEndKind::kLigatureLineEndBoth:
      ss << "ligatureLineEndBoth";
      break;
    case msrLigatureLineEndKind::kLigatureLineEndArrow:
      ss << "ligatureLineEndArrow";
      break;
    case msrLigatureLineEndKind::kLigatureLineEndNone:
      ss << "ligatureLineEndNone";
      break;
  } // switch

  return ss.str ();
}

std::ostream& operator << (std::ostream& os, const msrLigatureLineEndKind& elt)
{
  os << msrLigatureLineEndKindAsString (elt);
  return os;
}

std::string msrLigature::asString () const
{
  std::stringstream ss;

  ss <<
    "[Ligature" <<
    ", fLigatureKind: " << fLigatureKind <<
    ", fLigatureLineEndKind" << ": " << fLigatureLineEndKind <<
    ", fLigatureLineTypeKind" << ": " << fLigatureLineTypeKind <<
    ", fLigaturePlacementKind" << ": " << fLigaturePlacementKind <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrLigature::print (std::ostream& os) const
{
  os <<
    "[Ligature" <<
    ", fLigatureKind: " << fLigatureKind <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 22;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fLigatureLineEndKind" << ": " << fLigatureLineEndKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLigatureLineTypeKind" << ": " << fLigatureLineTypeKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLigaturePlacementKind" << ": " << fLigaturePlacementKind <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
