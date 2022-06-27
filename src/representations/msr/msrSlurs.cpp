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

#include <stack>

#include "visitor.h"

#include "msrSlurs.h"

#include "oahOah.h"

#include "msrOah.h"




using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrSlur msrSlur::create (
  int              inputLineNumber,
  int              slurNumber,
  msrSlurTypeKind  slurTypeKind,
  msrLineTypeKind  slurLineTypeKind,
  msrPlacementKind slurPlacementKind)
{
  msrSlur* o =
    new msrSlur (
      inputLineNumber,
      slurNumber,
      slurTypeKind,
      slurLineTypeKind,
      slurPlacementKind);
  assert (o != nullptr);
  return o;
}

S_msrSlur msrSlur::create (
  int              inputLineNumber,
  int              slurNumber,
  msrSlurTypeKind  slurTypeKind,
  msrLineTypeKind  slurLineTypeKind)
{
  return
    msrSlur::create (
      inputLineNumber,
      slurNumber,
      slurTypeKind,
      slurLineTypeKind,
      msrPlacementKind::k_NoPlacement);
}

msrSlur::msrSlur (
  int              inputLineNumber,
  int              slurNumber,
  msrSlurTypeKind  slurTypeKind,
  msrLineTypeKind  slurLineTypeKind,
  msrPlacementKind slurPlacementKind)
    : msrElement (inputLineNumber)
{
  fSlurNumber        = slurNumber;
  fSlurTypeKind      = slurTypeKind;
  fSlurLineTypeKind  = slurLineTypeKind;
  fSlurPlacementKind = slurPlacementKind;
}

msrSlur::~msrSlur ()
{}

void msrSlur::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlur::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlur::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlur::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlur::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

string msrSlur::asString () const
{
  stringstream s;

  s <<
    "[Slur" <<
    ", slurNumber: " <<
    fSlurNumber <<

    ", slurTypeKind: " <<
    msrSlurTypeKindAsString (
      fSlurTypeKind) <<

    ", slurLineTypeKind: " <<
    msrLineTypeKindAsString (
      fSlurLineTypeKind) <<

    ", slurPlacementKind: " <<
    msrPlacementKindAsString (
      fSlurPlacementKind) <<

    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrSlur::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrSlur& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}


}
