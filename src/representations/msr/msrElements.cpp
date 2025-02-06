/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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
  int inputLineNumber)
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

int msrElement::getInputLineNumber () const
{
  return fInputLineNumber;
}

//______________________________________________________________________________
void msrElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrElement::msrElement ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrElement::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrElement::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrElement::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
  return "[??? msrElement::asString () ???]";
}

std::string msrElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
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

void msrElement::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

void msrElement::printFull (std::ostream& os) const
{
   print (os);
}

void msrElement::printSummary (std::ostream& os) const
{
  print (os);
}

msrElement::operator std::string() const
{
  return asString ();
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


}
