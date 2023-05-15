/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "visitor.h"

#include "msrTies.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// ties
// ------------------------------------------------------
std::string msrTieKindAsString (msrTieKind tieKind)
{
  std::stringstream ss;

  switch (tieKind) {
    case msrTieKind::kTieNone:
      ss << "kTieNone***";
      break;
    case msrTieKind::kTieStart:
      ss << "kTieStart";
      break;
    case msrTieKind::kTieContinue:
      ss << "kTieContinue";
      break;
    case msrTieKind::kTieStop:
      ss << "kTieStop";
      break;
  } // switch

  return ss.str ();
}

std::ostream& operator << (std::ostream& os, const msrTieKind& elt)
{
  os << msrTieKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrTie msrTie::create (
  int              inputLineNumber,
  msrTieKind       tieKind,
  msrPlacementKind placementKind)
{
  msrTie* obj =
    new msrTie (
      inputLineNumber,
      tieKind,
      placementKind);
  assert (obj != nullptr);
  return obj;
}

S_msrTie msrTie::create (
  int              inputLineNumber,
  msrTieKind       tieKind)
{
  return
    msrTie::create (
      inputLineNumber,
      tieKind,
      msrPlacementKind::kPlacement_UNKNOWN_);
}

msrTie::msrTie (
  int              inputLineNumber,
  msrTieKind       tieKind,
  msrPlacementKind placementKind)
    : msrElement (inputLineNumber)
{
  fTieKind = tieKind;

  fTiePlacementKind = placementKind;
}

msrTie::~msrTie ()
{}

void msrTie::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTie::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTie::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTie::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTie::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTie::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrTie::browseData (basevisitor* v)
{}

std::string msrTie::asString () const
{
  std::stringstream ss;

  ss <<
    "Tie" << ' ' << msrTieKindAsString (fTieKind) <<
    ", line " << fInputStartLineNumber;

  return ss.str ();
}

void msrTie::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTie& elt)
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
