/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "bsrSpaces.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_bsrSpaces bsrSpaces::create (
  int inputLineNumber,
  int numberOfSpaces)
{
  bsrSpaces* o =
    new bsrSpaces (
      inputLineNumber, numberOfSpaces);
  assert (o != nullptr);
  return o;
}

bsrSpaces::bsrSpaces (
  int inputLineNumber,
  int numberOfSpaces)
    : bsrLineContentsElement (inputLineNumber)
{
  fNumberOfSpaces = numberOfSpaces;

  fSpacesCellsList =
    bsrCellsList::create (inputLineNumber);

  for (int i = 1; i <= fNumberOfSpaces; ++i) {
    fSpacesCellsList->
      appendCellKindToCellsList (kCellSpace);
  } // for
}

bsrSpaces::~bsrSpaces ()
{}

void bsrSpaces::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrSpaces::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrSpaces::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

int bsrSpaces::fetchCellsNumber () const
{
  return fNumberOfSpaces;
}

void bsrSpaces::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrSpaces::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrSpaces::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrSpaces::browseData (basevisitor* v)
{}

string bsrSpaces::asString () const
{
  stringstream s;

  s <<
    "Spaces" <<
    ", numberOfSpaces: " << fNumberOfSpaces <<
    ", spacesCellsList: " << fSpacesCellsList <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrSpaces::asDebugString () const
{
  string result;

  for (int i = 1; i <= fNumberOfSpaces; ++i) {
    result += " ";
  } // for

  return result;
}

void bsrSpaces::print (ostream& os) const
{
  os <<
    "Spaces" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 16;

  os <<
    setw (fieldWidth) <<
    "numberOfSpaces" << " : " << fNumberOfSpaces <<
    endl <<
    setw (fieldWidth) <<
    "spacesCellsList" << " : " << fSpacesCellsList <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrSpaces& elt)
{
  elt->print (os);
  return os;
}


}
