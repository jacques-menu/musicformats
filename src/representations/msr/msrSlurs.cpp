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

#include <stack>

#include "visitor.h"

#include "msrSlurs.h"

#include "oahOah.h"

#include "msrOah.h"




namespace MusicFormats
{

// slurs
// ------------------------------------------------------
std::string msrSlurTypeKindAsString (
  msrSlurTypeKind slurTypeKind)
{
  std::stringstream s;

  switch (slurTypeKind) {
    case msrSlurTypeKind::kSlurType_UNKNOWN:
      s << "kSlurType_UNKNOWN";
      break;

    case msrSlurTypeKind::kSlurTypeRegularStart:
      s << "kSlurTypeRegularStart";
      break;
    case msrSlurTypeKind::kSlurTypeRegularContinue:
      s << "kSlurTypeRegularContinue";
      break;
    case msrSlurTypeKind::kSlurTypeRegularStop:
      s << "kSlurTypeRegularStop";
      break;

    case msrSlurTypeKind::kSlurTypePhrasingStart:
      s << "kSlurTypePhrasingStart";
      break;
    case msrSlurTypeKind::kSlurTypePhrasingContinue:
      s << "kSlurTypePhrasingContinue";
      break;
    case msrSlurTypeKind::kSlurTypePhrasingStop:
      s << "kSlurTypePhrasingStop";
      break;
  } // switch

  return s.str ();
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
  msrSlur* o =
    new msrSlur (
      inputLineNumber,
      slurNumber,
      slurTypeKind,
      slurLineTypeKind,
      slurPlacementKind);
  assert (o != nullptr);
  return o;
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
      msrPlacementKind::kPlacement_UNKNOWN);
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

void msrSlur::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlur::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlur::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSlur::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSlur::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

std::string msrSlur::asString () const
{
  std::stringstream s;

  s <<
    "[Slur" <<
    ", slurNumber: " <<
    fSlurNumber <<

    ", slurTypeKind: " <<
    msrSlurTypeKindAsString (
      fSlurTypeKind) <<

    ", slurLineTypeKind: " <<
    msrLineTypeKindAsString (
      fSlurLineTypeKind) <<

    ", slurPlacementKind: " <<
    msrPlacementKindAsString (
      fSlurPlacementKind) <<

    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrSlur::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSlur& elt)
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
