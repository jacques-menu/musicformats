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
S_msrTransposition msrTransposition::create (
  int  inputLineNumber,
  int  transposeDiatonic,
  int  transposeChromatic,
  int  transposeOctaveChange,
  Bool transposeDouble)
{
  msrTransposition* o =
    new msrTransposition (
      inputLineNumber,
      transposeDiatonic,
      transposeChromatic,
      transposeOctaveChange,
      transposeDouble);
  assert (o != nullptr);
  return o;
}

msrTransposition::msrTransposition (
  int  inputLineNumber,
  int  transposeDiatonic,
  int  transposeChromatic,
  int  transposeOctaveChange,
  Bool transposeDouble)
    : msrMeasureElement (inputLineNumber)
{
  fTranspositionDiatonic     = transposeDiatonic;
  fTranspositionChromatic    = transposeChromatic;
  fTranspositionOctaveChange = transposeOctaveChange;
  fTranspositionDouble       = transposeDouble;

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

msrTransposition::~msrTransposition ()
{}

Bool msrTransposition::isEqualTo (S_msrTransposition otherTranspose) const
{
  if (! otherTranspose) {
    return false;
  }

  return
    fTranspositionDiatonic ==
      otherTranspose->fTranspositionDiatonic
      &&
    fTranspositionChromatic ==
      otherTranspose->fTranspositionChromatic
      &&
    fTranspositionOctaveChange ==
      otherTranspose->fTranspositionOctaveChange
      &&
    fTranspositionDouble ==
      otherTranspose->fTranspositionDouble;
}

void msrTransposition::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTransposition::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTransposition>*
    p =
      dynamic_cast<visitor<S_msrTransposition>*> (v)) {
        S_msrTransposition elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTransposition::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTransposition::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTransposition::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTransposition>*
    p =
      dynamic_cast<visitor<S_msrTransposition>*> (v)) {
        S_msrTransposition elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTransposition::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTransposition::browseData (basevisitor* v)
{}

string msrTransposition::asString () const
{
  stringstream s;

  s <<
    "[Transpose" <<
    ", fTranspositionDiatonic = " << fTranspositionDiatonic <<
    ", fTranspositionChromatic = " << fTranspositionChromatic <<
    ", fTranspositionOctaveChange = " << fTranspositionOctaveChange <<
    ", fTranspositionDouble = " << fTranspositionDouble <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrTransposition::print (ostream& os) const
{
  const int fieldWidth = 22;

  os <<
    "[Transpose" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "fTranspositionDiatonic" << " = " << fTranspositionDiatonic <<
    endl <<
    setw (fieldWidth) <<
    "fTranspositionChromatic" << " = " << fTranspositionChromatic <<
    endl <<
    setw (fieldWidth) <<
    "fTranspositionOctaveChange" << " = " << fTranspositionOctaveChange <<
    endl <<
    setw (fieldWidth) <<
    "fTranspositionDouble" << " = " << fTranspositionDouble <<
    endl << endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrTransposition& elt)
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
