/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "bsrClefs.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

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
      endl;
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
    case kClefKindNone:
      result =
        bsrCellsList::create (
          fInputLineNumber);
      break;

    case kClefKindGTreble:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots34, bsrCellKind::kDots123);
      break;
    case kClefKindFBass:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots3456, bsrCellKind::kDots123);
      break;
    case kClefKindCAlto:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots346, bsrCellKind::kDots123);
      break;

    case kClefKindGSoprano:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots34, bsrCellKind::kDots4, bsrCellKind::kDots123);
      break;
    case kClefKindCBaritone:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots3456, bsrCellKind::kDots456, bsrCellKind::kDots123);
      break;
    case kClefKindCTenor:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots346, bsrCellKind::kDots5, bsrCellKind::kDots123);
      break;

    case kClefKindGOttavaAlta:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots34, bsrCellKind::kDots123, bsrCellKind::kDots3456, bsrCellKind::kDots125);
      break;
    case kClefKindGOttavaBassa:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots34, bsrCellKind::kDots123, bsrCellKind::kDots3456, bsrCellKind::kDots236);
      break;

    case kClefKindModifiedBassForRightHandPart:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          bsrCellKind::kDots345, bsrCellKind::kDots3456, bsrCellKind::kDots13);
      break;
    case kClefKindModifiedTrebleForLeftHandPart:
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
      endl;
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
            endl;
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
      endl;
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
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrClef::browseData (basevisitor* v)
{}


string bsrClef::msrClefKindAsString (
  bsrClefKind clefKind)
{
  string result;

  switch (clefKind) {
    case bsrClef::kClefKindNone:
      result = "***clefNone***";
      break;
    case bsrClef::kClefKindGTreble:
      result = "clefGTreble";
      break;
    case bsrClef::kClefKindFBass:
      result = "clefFBass";
      break;
    case bsrClef::kClefKindCAlto:
      result = "clefCAlto";
      break;
    case bsrClef::kClefKindGSoprano:
      result = "clefGSoprano";
      break;
    case bsrClef::kClefKindCBaritone:
      result = "clefFBaritone";
      break;
    case bsrClef::kClefKindCTenor:
      result = "clefCTenor";
      break;
    case bsrClef::kClefKindGOttavaAlta:
      result = "clefGOttavaAlta";
      break;
    case bsrClef::kClefKindGOttavaBassa:
      result = "clefGOttavaBassa";
      break;
    case bsrClef::kClefKindModifiedBassForRightHandPart:
      result = "clefModifiedBassForRightHandPart";
      break;
    case bsrClef::kClefKindModifiedTrebleForLeftHandPart:
      result = "clefModifiedTrebleForLeftHandPart";
      break;
  } // switch

  return result;
}

string bsrClef::clefKindAsDebugString (
  bsrClefKind clefKind)
{
  string result;

  switch (clefKind) {
    case bsrClef::kClefKindNone:
      result = "_";
      break;
    case bsrClef::kClefKindGTreble:
      result = "g2";
      break;
    case bsrClef::kClefKindFBass:
      result = "f4";
      break;
    case bsrClef::kClefKindCAlto:
      result = "u3";
      break;
    case bsrClef::kClefKindGSoprano:
      result = "g1";
      break;
    case bsrClef::kClefKindCBaritone:
      result = "u5";
      break;
    case bsrClef::kClefKindCTenor:
      result = "u4";
      break;
    case bsrClef::kClefKindGOttavaAlta:
      result = "g8a";
      break;
    case bsrClef::kClefKindGOttavaBassa:
      result = "g8b";
      break;
    case bsrClef::kClefKindModifiedBassForRightHandPart:
      result = "rh";
      break;
    case bsrClef::kClefKindModifiedTrebleForLeftHandPart:
      result = "lh";
      break;
  } // switch

  return result;
}

string bsrClef::asString () const
{
  stringstream s;

  s <<
    "Clef" <<
    ", clefKind: " <<
    msrClefKindAsString (fClefKind) <<
    "clefCellsList: " << fClefCellsList <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrClef::asDebugString () const
{
  stringstream s;

  s <<
    "CLEF " <<
    clefKindAsDebugString (fClefKind) <<
    "]";

  return s.str ();
}

void bsrClef::print (ostream& os) const
{
  os <<
    "Clef" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 16;

  os <<
    setw (fieldWidth) <<
    "clefKind" << " : " <<
    msrClefKindAsString (fClefKind) <<
    endl <<
    setw (fieldWidth) <<
    "clefCellsList" << " : " << fClefCellsList <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrClef& elt)
{
  elt->print (os);
  return os;
}


}
