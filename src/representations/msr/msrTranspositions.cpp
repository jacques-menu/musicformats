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

#include "msrTranspositions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrOctaveShift msrOctaveShift::create (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
{
  msrOctaveShift* o =
    new msrOctaveShift (
      inputLineNumber, octaveShiftKind, octaveShiftSize);
  assert (o != nullptr);
  return o;
}

msrOctaveShift::msrOctaveShift (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
    : msrMeasureElement (inputLineNumber)
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

string msrOctaveShift::octaveShiftKindAsString () const
{
  string result;

  switch (fOctaveShiftKind) {
    case msrOctaveShift::kOctaveShiftNone:
      result = "octaveShiftNone";
      break;
    case msrOctaveShift::kOctaveShiftUp:
      result = "octaveShiftUp";
      break;
    case msrOctaveShift::kOctaveShiftDown:
      result = "octaveShiftDown";
      break;
    case msrOctaveShift::kOctaveShiftStop:
      result = "octaveShiftStop";
      break;
    case msrOctaveShift::kOctaveShiftContinue:
      result = "octaveShiftContinue";
      break;
  } // switch

  return result;
}

string msrOctaveShift::asString () const
{
  string result;

  stringstream s;

  s <<
    "OctaveShift" <<
    ", kind: " << octaveShiftKindAsString () <<
    ", size: " << fOctaveShiftSize;

  return s.str ();
}

void msrOctaveShift::print (ostream& os) const
{
  ++gIndenter;

  os <<
    "OctaveShift" <<
    ", kind: " << octaveShiftKindAsString () <<
    ", size: " << fOctaveShiftSize <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrOctaveShift& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrTranspose msrTranspose::create (
  int  inputLineNumber,
  int  transposeDiatonic,
  int  transposeChromatic,
  int  transposeOctaveChange,
  Bool transposeDouble)
{
  msrTranspose* o =
    new msrTranspose (
      inputLineNumber,
      transposeDiatonic,
      transposeChromatic,
      transposeOctaveChange,
      transposeDouble);
  assert (o != nullptr);
  return o;
}

msrTranspose::msrTranspose (
  int  inputLineNumber,
  int  transposeDiatonic,
  int  transposeChromatic,
  int  transposeOctaveChange,
  Bool transposeDouble)
    : msrMeasureElement (inputLineNumber)
{
  fTransposeDiatonic     = transposeDiatonic;
  fTransposeChromatic    = transposeChromatic;
  fTransposeOctaveChange = transposeOctaveChange;
  fTransposeDouble       = transposeDouble;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Creating transpose '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrTranspose::~msrTranspose ()
{}

Bool msrTranspose::isEqualTo (S_msrTranspose otherTranspose) const
{
  if (! otherTranspose) {
    return false;
  }

  return
    fTransposeDiatonic ==
      otherTranspose->fTransposeDiatonic
      &&
    fTransposeChromatic ==
      otherTranspose->fTransposeChromatic
      &&
    fTransposeOctaveChange ==
      otherTranspose->fTransposeOctaveChange
      &&
    fTransposeDouble ==
      otherTranspose->fTransposeDouble;
}

void msrTranspose::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTranspose::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTranspose::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTranspose::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTranspose::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTranspose::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTranspose::browseData (basevisitor* v)
{}

string msrTranspose::asString () const
{
  stringstream s;

  s <<
    "[Transpose" <<
    ", diatonic = " << fTransposeDiatonic <<
    ", chromatic = " << fTransposeChromatic <<
    ", transposeOctaveChange = " << fTransposeOctaveChange <<
    ", transposeDouble = " << fTransposeDouble <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrTranspose::print (ostream& os) const
{
  const int fieldWidth = 22;

  os <<
    "[Transpose" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "transposeDiatonic" << " = " << fTransposeDiatonic <<
    endl <<
    setw (fieldWidth) <<
    "transposeChromatic" << " = " << fTransposeChromatic <<
    endl <<
    setw (fieldWidth) <<
    "transposeOctaveChange" << " = " << fTransposeOctaveChange <<
    endl <<
    setw (fieldWidth) <<
    "transposeDouble" << " = " << fTransposeDouble <<
    endl << endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrTranspose& elt)
{
  elt->print (os);
  return os;
}


}
