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


using namespace std;

namespace MusicFormats
{

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
      msrPlacementKind::k_NoPlacement);
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
      endl;
  }

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTie::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTie::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTie::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTie::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTie::browseData (basevisitor* v)
{}

string msrTie::asString () const
{
  stringstream s;

  s <<
    "Tie" << ' ' << msrTieKindAsString (fTieKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrTie::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrTie& elt)
{
  elt->print (os);
  return os;
}


}
