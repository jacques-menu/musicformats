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
#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "msrElements.h"

#include "msrPathToVoice.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrElement::msrElement (
  int inputLineNumber)
{
  fInputLineNumber = inputLineNumber;
}

msrElement::~msrElement ()
{}

void msrElement::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrElement::msrElement ()" <<
      std::endl;
  }

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrElement::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrElement::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrElement::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrElement::browseDataAlongPathToVoice (
  basevisitor*            v,
  const S_msrPathToVoice& pathToVoice)
{
  /*
    This defaults to browseData (),
    and can be used to browse only the
    msrBook, msrScore, msrPartGroup, msrPart, msrStaff and msrVoice
    instances in the path
  */

  browseData (v);
}

std::string msrElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrElement::asString () ???";
}

std::string msrElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

std::string msrElement::asShortStringForMeasuresSlices () const
{
  // this can be overriden in actual elements
  return asShortString ();
}

void msrElement::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

void msrElement::printShort (std::ostream& os) const
{
  print (os);
}

void msrElement::printSummary (std::ostream& os) const
{
  print (os);
}

std::ostream& operator << (std::ostream& os, const S_msrElement& elt)
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
