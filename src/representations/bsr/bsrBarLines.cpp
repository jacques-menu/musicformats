/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "bsrBarLines.h"

#include "bsrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_bsrBarLine bsrBarLine::create (
  int            inputLineNumber,
  bsrBarLineKind barLineKind)
{
  bsrBarLine* o =
    new bsrBarLine (
      inputLineNumber, barLineKind);
  assert (o != nullptr);

  return o;
}

bsrBarLine::bsrBarLine (
  int            inputLineNumber,
  bsrBarLineKind barLineKind)
    : bsrLineContentsElement (inputLineNumber)
{
  fBarLineKind = barLineKind;

  fBarLineCellsList = buildCellsList ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Creating bsrBarLine '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrBarLine::~bsrBarLine ()
{}

S_bsrCellsList bsrBarLine::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber);

  switch (fBarLineKind) {
    case bsrBarLine::kBarLineKindNone:
      break;
    case bsrBarLine::kBarLineKindSpecial:
      result->
        appendCellKindToCellsList (
          bsrCellKind::kDots13);
      break;
    case bsrBarLine::kBarLineKindUnusual:
      result->
        appendCellKindToCellsList (
          bsrCellKind::kDots123);
      break;
    case bsrBarLine::kBarLineKindFinalDouble:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber,
            bsrCellKind::kDots126, bsrCellKind::kDots13));
      break;
    case bsrBarLine::kBarLineKindSectionalDouble:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber,
            bsrCellKind::kDots126, bsrCellKind::kDots13, bsrCellKind::kDots3));
      break;
  } // switch

  return result;
}

int bsrBarLine::fetchCellsNumber() const
{
  return fBarLineCellsList->fetchCellsNumber();
}

void bsrBarLine::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrBarLine::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrBarLine>*
    p =
      dynamic_cast<visitor<S_bsrBarLine>*> (v)) {
        S_bsrBarLine elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrBarLine::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrBarLine::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrBarLine::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrBarLine>*
    p =
      dynamic_cast<visitor<S_bsrBarLine>*> (v)) {
        S_bsrBarLine elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrBarLine::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrBarLine::browseData (basevisitor* v)
{}

string bsrBarLine::barLineKindAsString (
  bsrBarLineKind barLineKind)
{
  string result;

  switch (barLineKind) {
    case bsrBarLine::kBarLineKindNone:
      result = "barLineKindNone";
      break;
    case bsrBarLine::kBarLineKindSpecial:
      result = "barLineKindSpecial";
      break;
    case bsrBarLine::kBarLineKindUnusual:
      result = "barLineKindUnusual";
      break;
    case bsrBarLine::kBarLineKindFinalDouble:
      result = "barLineKindFinalDouble";
      break;
    case bsrBarLine::kBarLineKindSectionalDouble:
      result = "barLineKindSectionalDouble";
      break;
  } // switch

  return result;
}

string bsrBarLine::barLineKindAsDebugString (
  bsrBarLineKind barLineKind)
{
  string result;

  switch (barLineKind) {
    case bsrBarLine::kBarLineKindNone:
      result = "***None***";
      break;
    case bsrBarLine::kBarLineKindSpecial:
      result = "Special";
      break;
    case bsrBarLine::kBarLineKindUnusual:
      result = "Unusual";
      break;
    case bsrBarLine::kBarLineKindFinalDouble:
      result = "FinalDouble";
      break;
    case bsrBarLine::kBarLineKindSectionalDouble:
      result = "||"; // L'\u2016' ??? JMI
//      result = L'\u1D101';
      break;
  } // switch

  return result;
}

string bsrBarLine::asString () const
{
  stringstream s;

  s <<
    "BarLine" <<
    ", " << barLineKindAsString (fBarLineKind) <<
    ", barLineCellsList: " << fBarLineCellsList->asShortString () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrBarLine::asDebugString () const
{
  stringstream s;

  s <<
    "[BARLINE " <<
    barLineKindAsDebugString (fBarLineKind) <<
    "]";

  return s.str ();
}

void bsrBarLine::print (ostream& os) const
{
  os <<
    "BarLine" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os <<
    setw (fieldWidth) <<
    "barLineKind" << " : " << barLineKindAsString (fBarLineKind) <<
    endl <<
    setw (fieldWidth) <<
    "barLineCellsList" << " : " << fBarLineCellsList->asShortString () <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_bsrBarLine& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }
  
  return os;
}


}
