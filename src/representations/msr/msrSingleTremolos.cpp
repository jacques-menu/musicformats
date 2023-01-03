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

#include "msrSingleTremolos.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrSingleTremolo msrSingleTremolo::create (
  int              inputLineNumber,
  int              singleTremoloMarksNumber,
  msrPlacementKind singleTremoloPlacementKind)
{
  msrSingleTremolo* o =
    new msrSingleTremolo (
      inputLineNumber,
      singleTremoloMarksNumber, singleTremoloPlacementKind);
  assert (o != nullptr);
  return o;
}

msrSingleTremolo::msrSingleTremolo (
  int              inputLineNumber,
  int              singleTremoloMarksNumber,
  msrPlacementKind singleTremoloPlacementKind)
    : msrElement (inputLineNumber)
{
  fSingleTremoloMarksNumber   = singleTremoloMarksNumber;
  fSingleTremoloPlacementKind = singleTremoloPlacementKind;
}

msrSingleTremolo::~msrSingleTremolo ()
{}

std::string msrSingleTremolo::singleTremoloPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fSingleTremoloPlacementKind);
}

void msrSingleTremolo::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSingleTremolo::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSingleTremolo::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrSingleTremolo::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSingleTremolo::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSingleTremolo::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrSingleTremolo::browseData (basevisitor* v)
{}

std::string msrSingleTremolo::asString () const
{
  std::stringstream s;

  s <<
    "SingleTremolo" <<
    ", line " << fInputLineNumber <<
    ", " << fSingleTremoloMarksNumber << " marks" <<
    ", placement" << ": " << singleTremoloPlacementKindAsString ();

  return s.str ();
}

void msrSingleTremolo::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSingleTremolo& elt)
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
