/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "bsrClefs.h"

#include "oahOah.h"

#include "bsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrClef bsrClef::create (
  int         inputLineNumber,
  bsrClefKind clefKind)
{
  bsrClef* o =
    new bsrClef (
      inputLineNumber, clefKind);
  assert (o != nullptr);
  return o;
}

bsrClef::bsrClef (
  int         inputLineNumber,
  bsrClefKind clefKind)
    : bsrLineContentsElement (inputLineNumber)
{
  fClefKind = clefKind;

  fClefCellsList = buildCellsList ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Creating bsrClef '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      std::endl;
  }
#endif
}

bsrClef::~bsrClef ()
{}

S_bsrCellsList bsrClef::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber);

  switch (fClefKind) {
    case bsrClefKind::kClefKindNone:
      result =
        bsrCellsList::create (
          fInputLineNumber);
      break;

    case bsrClefKind::kClefKindGTreble:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots34, bsrCellKind::kDots123);
      break;
    case bsrClefKind::kClefKindFBass:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots3456, bsrCellKind::kDots123);
      break;
    case bsrClefKind::kClefKindCAlto:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots346, bsrCellKind::kDots123);
      break;

    case bsrClefKind::kClefKindGSoprano:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots34, bsrCellKind::kDots4, bsrCellKind::kDots123);
      break;
    case bsrClefKind::kClefKindCBaritone:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots3456, bsrCellKind::kDots456, bsrCellKind::kDots123);
      break;
    case bsrClefKind::kClefKindCTenor:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots346, bsrCellKind::kDots5, bsrCellKind::kDots123);
      break;

    case bsrClefKind::kClefKindGOttavaAlta:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots34, bsrCellKind::kDots123, bsrCellKind::kDots3456, bsrCellKind::kDots125);
      break;
    case bsrClefKind::kClefKindGOttavaBassa:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots34, bsrCellKind::kDots123, bsrCellKind::kDots3456, bsrCellKind::kDots236);
      break;

    case bsrClefKind::kClefKindModifiedBassForRightHandPart:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots3456, bsrCellKind::kDots13);
      break;
    case bsrClefKind::kClefKindModifiedTrebleForLeftHandPart:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots34, bsrCellKind::kDots13);
      break;
  } // switch

  return result;
}

int bsrClef::fetchCellsNumber() const
{
  return fClefCellsList->fetchCellsNumber();
}

void bsrClef::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrClef::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrClef>*
    p =
      dynamic_cast<visitor<S_bsrClef>*> (v)) {
        S_bsrClef elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrClef::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrClef::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrClef::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrClef>*
    p =
      dynamic_cast<visitor<S_bsrClef>*> (v)) {
        S_bsrClef elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrClef::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrClef::browseData (basevisitor* v)
{}


std::string bsrClefKindAsString (
  bsrClefKind clefKind)
{
  std::string result;

  switch (clefKind) {
    case bsrClefKind::kClefKindNone:
      result = "kClefKindNone***";
      break;
    case bsrClefKind::kClefKindGTreble:
      result = "kClefKindGTreble";
      break;
    case bsrClefKind::kClefKindFBass:
      result = "kClefKindFBass";
      break;
    case bsrClefKind::kClefKindCAlto:
      result = "kClefKindCAlto";
      break;
    case bsrClefKind::kClefKindGSoprano:
      result = "kClefKindGSoprano";
      break;
    case bsrClefKind::kClefKindCBaritone:
      result = "kClefKindCBaritone";
      break;
    case bsrClefKind::kClefKindCTenor:
      result = "kClefKindCTenor";
      break;
    case bsrClefKind::kClefKindGOttavaAlta:
      result = "kClefKindGOttavaAlta";
      break;
    case bsrClefKind::kClefKindGOttavaBassa:
      result = "kClefKindGOttavaBassa";
      break;
    case bsrClefKind::kClefKindModifiedBassForRightHandPart:
      result = "kClefKindModifiedBassForRightHandPart";
      break;
    case bsrClefKind::kClefKindModifiedTrebleForLeftHandPart:
      result = "kClefKindModifiedTrebleForLeftHandPart";
      break;
  } // switch

  return result;
}

std::string bsrClefKindAsDebugString (
  bsrClefKind clefKind)
{
  std::string result;

  switch (clefKind) {
    case bsrClefKind::kClefKindNone:
      result = "_";
      break;
    case bsrClefKind::kClefKindGTreble:
      result = "g2";
      break;
    case bsrClefKind::kClefKindFBass:
      result = "f4";
      break;
    case bsrClefKind::kClefKindCAlto:
      result = "u3";
      break;
    case bsrClefKind::kClefKindGSoprano:
      result = "g1";
      break;
    case bsrClefKind::kClefKindCBaritone:
      result = "u5";
      break;
    case bsrClefKind::kClefKindCTenor:
      result = "u4";
      break;
    case bsrClefKind::kClefKindGOttavaAlta:
      result = "g8a";
      break;
    case bsrClefKind::kClefKindGOttavaBassa:
      result = "g8b";
      break;
    case bsrClefKind::kClefKindModifiedBassForRightHandPart:
      result = "rh";
      break;
    case bsrClefKind::kClefKindModifiedTrebleForLeftHandPart:
      result = "lh";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrClefKind& elt)
{
  os << bsrClefKindAsString (elt);
  return os;
}

std::string bsrClef::asString () const
{
  std::stringstream s;

  s <<
    "[Clef" <<
    ", fClefKind: " << fClefKind <<
    "fClefCellsList: " << fClefCellsList <<
    ", fSpacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string bsrClef::asDebugString () const
{
  std::stringstream s;

  s <<
    "CLEF " <<
    bsrClefKindAsDebugString (fClefKind) <<
    ']';

  return s.str ();
}

void bsrClef::print (std::ostream& os) const
{
  os <<
    "Clef" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 16;

  os <<
    std::setw (fieldWidth) <<
    "fClefKind" << ": " << fClefKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fClefCellsList" << ": " << fClefCellsList <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSpacesBefore" << ": " << fSpacesBefore <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrClef& elt)
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
