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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrSpaces::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrSpaces::visitStart ()" <<
            std::endl;
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrSpaces::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrSpaces>*
    p =
      dynamic_cast<visitor<S_bsrSpaces>*> (v)) {
        S_bsrSpaces elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrSpaces::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrSpaces::browseData (basevisitor* v)
{}

std::string bsrSpaces::asString () const
{
  std::stringstream s;

  s <<
    "Spaces" <<
    ", numberOfSpaces: " << fNumberOfSpaces <<
    ", spacesCellsList: " << fSpacesCellsList <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

std::string bsrSpaces::asDebugString () const
{
  std::string result;

  for (int i = 1; i <= fNumberOfSpaces; ++i) {
    result += ' ';
  } // for

  return result;
}

void bsrSpaces::print (std::ostream& os) const
{
  os <<
    "Spaces" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 16;

  os <<
    std::setw (fieldWidth) <<
    "numberOfSpaces" << ": " << fNumberOfSpaces <<
    std::endl <<
    std::setw (fieldWidth) <<
    "spacesCellsList" << ": " << fSpacesCellsList <<
    std::endl <<
    std::setw (fieldWidth) <<
    "spacesBefore" << ": " << fSpacesBefore <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrSpaces& elt)
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
