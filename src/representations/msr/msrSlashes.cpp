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

#include "msrSlashes.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrSlash msrSlash::create (
  int                  inputLineNumber,
  msrSlashTypeKind     slashTypeKind,
  msrUseDotsKind  useDotsKind,
  msrSlashUseStemsKind slashUseStemsKind)
{
  msrSlash* o =
    new msrSlash (
      inputLineNumber,
      slashTypeKind,
      useDotsKind,
      slashUseStemsKind);
  assert (o != nullptr);
  return o;
}

msrSlash::msrSlash (
  int                  inputLineNumber,
  msrSlashTypeKind     slashTypeKind,
  msrUseDotsKind  useDotsKind,
  msrSlashUseStemsKind slashUseStemsKind)
    : msrElement (inputLineNumber)
{
  fSlashTypeKind     = slashTypeKind;
  fUseDotsKind  = useDotsKind;
  fSlashUseStemsKind = slashUseStemsKind;
}

msrSlash::~msrSlash ()
{}

void msrSlash::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlash::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSlash>*
    p =
      dynamic_cast<visitor<S_msrSlash>*> (v)) {
        S_msrSlash elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlash::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSlash::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlash::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSlash>*
    p =
      dynamic_cast<visitor<S_msrSlash>*> (v)) {
        S_msrSlash elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlash::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSlash::browseData (basevisitor* v)
{}

string msrSlash::asString () const
{
  stringstream s;

  s <<
    "Slash" <<
    ", line " << fInputLineNumber <<
    ", slashTypeKind:" <<
    msrSlashTypeKindAsString (
      fSlashTypeKind) <<
    ", useDotsKind:" <<
    msrUseDotsKindAsString (
      fUseDotsKind) <<
    ", slashUseStemsKind:" <<
    msrSlashUseStemsKindAsString (
      fSlashUseStemsKind);

  return s.str ();
}

void msrSlash::print (ostream& os) const
{
  os <<
    "[Slash" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 18;

  // print the voice upLink
  os << left <<
    setw (fieldWidth) <<
    "slashTypeKind" << " : " <<
    msrSlashTypeKindAsString (
      fSlashTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "useDotsKind" << " : " <<
    msrUseDotsKindAsString (
      fUseDotsKind) <<
    endl <<
    setw (fieldWidth) <<
    "slashUseStemsKind" << " : " <<
    msrSlashUseStemsKindAsString (
      fSlashUseStemsKind) <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrSlash& elt)
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
