/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

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

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrElement::msrElement (
  const mfInputLineNumber& inputLineNumber)
{
  fInputLineNumber = inputLineNumber;
}

msrElement::~msrElement ()
{}

SMARTP<msrElement> msrElement::createNewbornClone ()
{
  return this;
}

SMARTP<msrElement> msrElement::createDeepClone ()
{
  return createNewbornClone ();
}

//______________________________________________________________________________
// set and get
// ------------------------------------------------------

mfInputLineNumber msrElement::getInputLineNumber () const
{
  return fInputLineNumber;
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

std::string msrElement::asStringForMeasuresSlices () const
{
  // this can be overriden in actual elements
  std::stringstream ss;

  ss <<
    "[Element " <<
    asShortString () <<
    ']';

  return ss.str ();
}

std::string msrElement::asStringForMeasuresSlices () const
{
  // this can be overriden in actual elements
  std::stringstream ss;

  ss <<
    '[' <<
    asShortString () <<
    ']';

  return ss.str ();
}

std::ostream& operator << (std::ostream& os, const S_msrElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const msrElement& elt)
{
  elt.print (os);

  return os;
}

std::string msrElementAsStringOrNULL(
  S_msrElement element)
{
  if (element) {
    return
      element->asString ();
  }
  else {
    return "[NULL]";
  }
}


}
