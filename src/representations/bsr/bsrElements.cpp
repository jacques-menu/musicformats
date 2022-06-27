/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "bsrElements.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
S_bsrElement bsrElement::create (
  int inputLineNumber)
{
  bsrElement* o =
    new bsrElement (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

bsrElement::bsrElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

bsrElement::~bsrElement ()
{}

void bsrElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrElement>*
    p =
      dynamic_cast<visitor<S_bsrElement>*> (v)) {
        S_bsrElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrElement>*
    p =
      dynamic_cast<visitor<S_bsrElement>*> (v)) {
        S_bsrElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrElement::browseData (basevisitor* v)
{}


string bsrElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrElement::asString () ???";
}

string bsrElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void bsrElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_bsrElement& elt)
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
