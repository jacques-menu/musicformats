/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "lpsrElements.h"

#include "oahOah.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
S_lpsrElement lpsrElement::create (
  int inputLineNumber)
{
  lpsrElement* o =
    new lpsrElement (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrElement::lpsrElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

lpsrElement::~lpsrElement ()
{}

void lpsrElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrElement>*
    p =
      dynamic_cast<visitor<S_lpsrElement>*> (v)) {
        S_lpsrElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrElement::browseData (basevisitor* v)
{}


}
