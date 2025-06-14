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

#include "visitor.h"

#include "msrStems.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrStemKindAsString (
  msrStemKind stemKind)
{
  std::string result;

  switch (stemKind) {
    case msrStemKind::kStemKind_NONE:
      result = "kStemKind_NONE";
      break;
    case msrStemKind::kStemKindNeutral:
      result = "kStemKindNeutral";
      break;
    case msrStemKind::kStemKindUp:
      result = "kStemKindUp";
      break;
    case msrStemKind::kStemKindDown:
      result = "kStemKindDown";
      break;
    case msrStemKind::kStemKindDouble:
      result = "kStemKindDouble";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrStemKind& elt)
{
  os << msrStemKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrStem msrStem::create (
  const mfInputLineNumber& inputLineNumber,
  msrStemKind stemKind)
{
  msrStem* obj =
    new msrStem (
      inputLineNumber, stemKind);
  assert (obj != nullptr);
  return obj;
}

msrStem::msrStem (
  const mfInputLineNumber& inputLineNumber,
  msrStemKind stemKind)
    : msrElement (inputLineNumber)
{
  fStemKind = stemKind;
}

msrStem::~msrStem ()
{}

void msrStem::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStem::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStem::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrStem::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStem::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStem::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrStem::browseData (basevisitor* v)
{}

std::string msrStem::asString () const
{
  std::stringstream ss;

  ss <<
    "[Stem" <<
    ", fStemKind: " <<
    fStemKind <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrStem::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrStem& elt)
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
