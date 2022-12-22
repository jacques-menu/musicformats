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

#include "msrTies.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

// ties
// ------------------------------------------------------
std::string msrTieKindAsString (msrTieKind tieKind)
{
  std::stringstream s;

  switch (tieKind) {
    case msrTieKind::kTieNone:
      s << "kTieNone***";
      break;
    case msrTieKind::kTieStart:
      s << "kTieStart";
      break;
    case msrTieKind::kTieContinue:
      s << "kTieContinue";
      break;
    case msrTieKind::kTieStop:
      s << "kTieStop";
      break;
  } // switch

  return s.str ();
}

std::ostream& operator << (std::ostream& os, const msrTieKind& elt)
{
  os << msrTieKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrTie msrTie::create (
  int              inputLineNumber,
  msrTieKind       tieKind,
  msrPlacementKind placementKind)
{
  msrTie* o =
    new msrTie (
      inputLineNumber,
      tieKind,
      placementKind);
  assert (o != nullptr);
  return o;
}

S_msrTie msrTie::create (
  int              inputLineNumber,
  msrTieKind       tieKind)
{
  return
    msrTie::create (
      inputLineNumber,
      tieKind,
      msrPlacementKind::kPlacement_UNKNOWN);
}

msrTie::msrTie (
  int              inputLineNumber,
  msrTieKind       tieKind,
  msrPlacementKind placementKind)
    : msrElement (inputLineNumber)
{
  fTieKind = tieKind;

  fTiePlacementKind = placementKind;
}

msrTie::~msrTie ()
{}

void msrTie::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTie::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTie::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrTie::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTie::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTie::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrTie::browseData (basevisitor* v)
{}

std::string msrTie::asString () const
{
  std::stringstream s;

  s <<
    "Tie" << ' ' << msrTieKindAsString (fTieKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrTie::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTie& elt)
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
