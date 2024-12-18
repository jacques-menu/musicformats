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
#include <iomanip>      // std::setw, std::setprecision, ...

#include <stack>

#include "visitor.h"

#include "msrSlurs.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// slurs
// ------------------------------------------------------
std::string msrSlurTypeKindAsString (
  msrSlurTypeKind slurTypeKind)
{
  std::stringstream ss;

  switch (slurTypeKind) {
    case msrSlurTypeKind::kSlurType_UNKNOWN_:
      ss << "kSlurType_UNKNOWN_";
      break;

    case msrSlurTypeKind::kSlurTypeRegularStart:
      ss << "kSlurTypeRegularStart";
      break;
    case msrSlurTypeKind::kSlurTypeRegularContinue:
      ss << "kSlurTypeRegularContinue";
      break;
    case msrSlurTypeKind::kSlurTypeRegularStop:
      ss << "kSlurTypeRegularStop";
      break;

    case msrSlurTypeKind::kSlurTypePhrasingStart:
      ss << "kSlurTypePhrasingStart";
      break;
    case msrSlurTypeKind::kSlurTypePhrasingContinue:
      ss << "kSlurTypePhrasingContinue";
      break;
    case msrSlurTypeKind::kSlurTypePhrasingStop:
      ss << "kSlurTypePhrasingStop";
      break;
  } // switch

  return ss.str ();
}

std::ostream& operator << (std::ostream& os, const msrSlurTypeKind& elt)
{
  os << msrSlurTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrSlur msrSlur::create (
  int              inputLineNumber,
  int              slurNumber,
  msrSlurTypeKind  slurTypeKind,
  msrLineTypeKind  slurLineTypeKind,
  msrPlacementKind slurPlacementKind)
{
  msrSlur* obj =
    new msrSlur (
      inputLineNumber,
      slurNumber,
      slurTypeKind,
      slurLineTypeKind,
      slurPlacementKind);
  assert (obj != nullptr);
  return obj;
}

S_msrSlur msrSlur::create (
  int              inputLineNumber,
  int              slurNumber,
  msrSlurTypeKind  slurTypeKind,
  msrLineTypeKind  slurLineTypeKind)
{
  return
    msrSlur::create (
      inputLineNumber,
      slurNumber,
      slurTypeKind,
      slurLineTypeKind,
      msrPlacementKind::kPlacement_UNKNOWN_);
}

msrSlur::msrSlur (
  int              inputLineNumber,
  int              slurNumber,
  msrSlurTypeKind  slurTypeKind,
  msrLineTypeKind  slurLineTypeKind,
  msrPlacementKind slurPlacementKind)
    : msrElement (inputLineNumber)
{
  fSlurNumber        = slurNumber;
  fSlurTypeKind      = slurTypeKind;
  fSlurLineTypeKind  = slurLineTypeKind;
  fSlurPlacementKind = slurPlacementKind;
}

msrSlur::~msrSlur ()
{}

bool msrSlur::compareSlursStopsBeforeStarts (
  const SMARTP<msrSlur>& first,
  const SMARTP<msrSlur>& second)
{
  return
    bool ( // JMI switch() here? v0.9.70
      first->fSlurTypeKind
        >
      second->fSlurTypeKind
    );
}

void msrSlur::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSlur::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSlur::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSlur::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSlur::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

std::string msrSlur::asString () const
{
  std::stringstream ss;

  ss <<
    "[Slur" <<
    ", slurNumber: " <<
    fSlurNumber <<

    ", slurTypeKind: " <<
    fSlurTypeKind <<

    ", slurLineTypeKind: " <<
    fSlurLineTypeKind <<

    ", slurPlacementKind: " <<
    fSlurPlacementKind <<

    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrSlur::print (std::ostream& os) const
{
  os <<
    "[Slur" <<
    ", fSlurTypeKind: " << fSlurTypeKind <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 19;

  os <<
    std::setw(fieldWidth) <<
    "fSlurNumber" << ": " <<
    fSlurNumber <<
    std::endl <<

    std::setw(fieldWidth) <<
    "fSlurTypeKind" << ": " <<
    fSlurTypeKind <<
    std::endl <<

    std::setw(fieldWidth) <<
    "fSlurLineTypeKind" << ": " <<
    fSlurLineTypeKind <<
    std::endl <<

    std::setw(fieldWidth) <<
    "fSlurPlacementKind" << ": " <<
    fSlurPlacementKind <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSlur& elt)
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
