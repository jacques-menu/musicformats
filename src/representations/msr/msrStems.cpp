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

#include "msrStems.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrStem msrStem::create (
  int         inputLineNumber,
  msrStemKind stemKind)
{
  msrStem* o =
    new msrStem (
      inputLineNumber, stemKind);
  assert (o != nullptr);
  return o;
}

msrStem::msrStem (
  int         inputLineNumber,
  msrStemKind stemKind)
    : msrElement (inputLineNumber)
{
  fStemKind   = stemKind;
}

msrStem::~msrStem ()
{}

void msrStem::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStem::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStem::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStem::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStem::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStem::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStem::browseData (basevisitor* v)
{}

string msrStemKindAsString (
  msrStemKind stemKind)
{
  string result;

  switch (stemKind) {
    case msrStemKind::kStemNeutral:
      result = "kStemNeutral";
      break;
    case msrStemKind::kStemUp:
      result = "kStemUp";
      break;
    case msrStemKind::kStemDown:
      result = "kStemDown";
      break;
    case msrStemKind::kStemDouble:
      result = "kStemDouble";
      break;
  } // switch

  return result;
}

string msrStem::asString () const
{
  stringstream s;

  s <<
    "[Stem" <<
    ", fStemKind: " <<
    msrStemKindAsString (fStemKind) <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrStem::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrStem& elt)
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
