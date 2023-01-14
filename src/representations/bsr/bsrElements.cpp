/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "bsrElements.h"

#include "oahOah.h"

#include "bsrOah.h"


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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrElement::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrElement>*
    p =
      dynamic_cast<visitor<S_bsrElement>*> (v)) {
        S_bsrElement elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrElement::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrElement::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrElement>*
    p =
      dynamic_cast<visitor<S_bsrElement>*> (v)) {
        S_bsrElement elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrElement::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrElement::browseData (basevisitor* v)
{}


std::string bsrElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrElement::asString () ???";
}

std::string bsrElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void bsrElement::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_bsrElement& elt)
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
