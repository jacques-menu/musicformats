/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "bsrPageElements.h"

#include "oahOah.h"

#include "bsrOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
/* this class   is purely virtual
S_bsrPageElement bsrPageElement::create (
  int inputLineNumber)
{
  bsrPageElement* o =
    new bsrPageElement (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}
*/

bsrPageElement::bsrPageElement (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
  fSpacesBefore = 0;
//  fSpacesAfter  = 0;
}

bsrPageElement::~bsrPageElement ()
{}

/*
void bsrPageElement::setUpLinkToBsrLine (
S_bsrLine upLinkToBsrLine)
{
  fUpLinkToBsrLine = upLinkToBsrLine;
}

S_bsrLine bsrPageElement::getUpLinkToBsrLine () const
{
  S_msrVoice result;

  if (fMeasureElementUpLinkToMeasure) {
    result =
  return fUpLinkToBsrLine;
  }

  return result;
}
*/

void bsrPageElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrPageElement::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrPageElement>*
    p =
      dynamic_cast<visitor<S_bsrPageElement>*> (v)) {
        S_bsrPageElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrPageElement::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrPageElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrPageElement::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrPageElement>*
    p =
      dynamic_cast<visitor<S_bsrPageElement>*> (v)) {
        S_bsrPageElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrPageElement::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrPageElement::browseData (basevisitor* v)
{}


std::string bsrPageElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrPageElement::asString () ???";
}

std::string bsrPageElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void bsrPageElement::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_bsrPageElement& elt)
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
