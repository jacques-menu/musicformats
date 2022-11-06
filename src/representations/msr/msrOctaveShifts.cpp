/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"

#include "msrMeasures.h"

#include "msrOctaveShifts.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrOctaveShift msrOctaveShift::create (
  int                inputLineNumber,
  S_msrMeasure       upLinkToMeasure,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
{
  msrOctaveShift* o =
    new msrOctaveShift (
      inputLineNumber,
      upLinkToMeasure,
      octaveShiftKind,
      octaveShiftSize);
  assert (o != nullptr);
  return o;
}

msrOctaveShift::msrOctaveShift (
  int                inputLineNumber,
  S_msrMeasure       upLinkToMeasure,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{
  fOctaveShiftKind = octaveShiftKind;

  fOctaveShiftSize = octaveShiftSize;
}

msrOctaveShift::~msrOctaveShift ()
{}

void msrOctaveShift::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrOctaveShift::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrOctaveShift::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOctaveShift::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrOctaveShift::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrOctaveShift::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrOctaveShift::browseData (basevisitor* v)
{}

string msrOctaveShiftKindAsString (
  msrOctaveShiftKind octaveShiftKind) const
{
  string result;

  switch (octaveShiftKind) {
    case msrOctaveShiftKind::kOctaveShiftNone:
      result = "kOctaveShiftNone";
      break;
    case msrOctaveShiftKind::kOctaveShiftUp:
      result = "kOctaveShiftUp";
      break;
    case msrOctaveShiftKind::kOctaveShiftDown:
      result = "kOctaveShiftDown";
      break;
    case msrOctaveShiftKind::kOctaveShiftStop:
      result = "kOctaveShiftStop";
      break;
    case msrOctaveShiftKind::kOctaveShiftContinue:
      result = "kOctaveShiftContinue";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrOctaveShiftKind& elt)
{
  os << msrOctaveShiftKindAsString (elt);
  return os;
}

string msrOctaveShift::asString () const
{
  stringstream s;

  s <<
    "[OctaveShift" <<
    ", fOctaveShiftKind: " << fOctaveShiftKind <<
    ", fOctaveShiftSize: " << fOctaveShiftSize <<
    ']';

  return s.str ();
}

void msrOctaveShift::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrOctaveShift& elt)
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
