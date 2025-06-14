/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "msrOrnaments.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrOrnament msrOrnament::create (
  const mfInputLineNumber& inputLineNumber,
  msrOrnamentKind  ornamentKind,
  msrPlacementKind ornamentPlacementKind)
{
  msrOrnament* obj =
    new msrOrnament (
      inputLineNumber, ornamentKind, ornamentPlacementKind);
  assert (obj != nullptr);
  return obj;
}

msrOrnament::msrOrnament (
  const mfInputLineNumber& inputLineNumber,
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrOrnament::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrOrnament::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrOrnament::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrOrnament::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrOrnament::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrOrnament::browseData (basevisitor* v)
{}

std::string msrOrnamentKindAsString (
  msrOrnamentKind ornamentKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrOrnamentKind& elt)
{
  os << msrOrnamentKindAsString (elt);
  return os;
}

std::string msrOrnament::asString () const
{
  std::stringstream ss;

  ss <<
    "[Ornament" <<
    ", fOrnamentKind: " <<
    fOrnamentKind <<
    ", fOrnamentPlacementKind: " <<
    fOrnamentPlacementKind <<
    ", fOrnamentPlacementKind: " <<
    fOrnamentPlacementKind <<
    ", fOrnamentAccidentalKind: " <<
    fOrnamentAccidentalKind <<
    ']';

  return ss.str ();
}


void msrOrnament::print (std::ostream& os) const
{
  os <<
    "[Ornament" <<
    ", fOrnamentKind: " <<
    fOrnamentKind <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 24;

  os <<
    std::setw (fieldWidth) <<
    "fOrnamentPlacementKind" << ": " <<
    fOrnamentPlacementKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fOrnamentAccidentalKind" << ": " <<
    fOrnamentAccidentalKind <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fOrnamentUpLinkToNote" << ": " <<
    std::endl;

  ++gIndenter;
  os <<
//     gTab <<
    fOrnamentUpLinkToNote->asShortString () <<
    std::endl;
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrOrnament& elt)
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
