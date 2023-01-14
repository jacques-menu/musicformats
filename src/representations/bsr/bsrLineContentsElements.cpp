/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "bsrLineContentsElements.h"

#include "oahOah.h"

#include "bsrOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
/* this class is purely virtual
S_bsrLineContentsElement bsrLineContentsElement::create (
  int inputLineNumber)
{
  bsrLineContentsElement* o =
    new bsrLineContentsElement (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}
*/

bsrLineContentsElement::bsrLineContentsElement (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
  fSpacesBefore = 0;
}

bsrLineContentsElement::~bsrLineContentsElement ()
{}

/*
void bsrLineContentsElement::setUpLinkToBsrLine (
  S_bsrLine upLinkToBsrLine)
{
  fUpLinkToBsrLine = upLinkToBsrLine;
}

S_bsrLine bsrLineContentsElement::getUpLinkToBsrLine () const
{
  S_msrVoice result;

  if (fMeasureElementUpLinkToMeasure) {
    result =
  return fUpLinkToBsrLine;
  }

  return result;
}
*/

void bsrLineContentsElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContentsElement::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrLineContentsElement>*
    p =
      dynamic_cast<visitor<S_bsrLineContentsElement>*> (v)) {
        S_bsrLineContentsElement elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLineContentsElement::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrLineContentsElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrLineContentsElement::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrLineContentsElement>*
    p =
      dynamic_cast<visitor<S_bsrLineContentsElement>*> (v)) {
        S_bsrLineContentsElement elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrLineContentsElement::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrLineContentsElement::browseData (basevisitor* v)
{}


std::string bsrLineContentsElement::asString () const
{
  // this is overriden all in actual elements
  return "??? bsrLineContentsElement::asString () ???";
}

std::string bsrLineContentsElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

/* JMI
std::string bsrLineContentsElement::asDebugString () const
{
  // this can be overriden in actual elements
  return asShortString ();
}
*/

void bsrLineContentsElement::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_bsrLineContentsElement& elt)
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
