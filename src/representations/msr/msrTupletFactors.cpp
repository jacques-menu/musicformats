/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // std::setw()), set::precision(), ...

#include "mfIndentedTextOutput.h"

#include "msrTupletFactors.h"


namespace MusicFormats
{

// tuplet factors
//______________________________________________________________________________
msrTupletFactor::msrTupletFactor ()
{
  fTupletActualNotes = -1;
  fTupletNormalNotes = -1;
}

msrTupletFactor::msrTupletFactor (
  int tupletActualNotes,
  int tupletNormalNotes)
{
  fTupletActualNotes = tupletActualNotes;
  fTupletNormalNotes = tupletNormalNotes;
}

msrTupletFactor::msrTupletFactor (
  const Rational& rationalTupletFactor)
{
  fTupletActualNotes = rationalTupletFactor.getNumerator ();
  fTupletNormalNotes = rationalTupletFactor.getDenominator ();
}

msrTupletFactor::~msrTupletFactor ()
{}

msrTupletFactor msrTupletFactor::inverse () const
{
  msrTupletFactor result (
    fTupletNormalNotes, fTupletActualNotes);

  return result;
}

std::string msrTupletFactor::asString () const
{
  std::stringstream s;

  s <<
    "[TupletFactor" <<
    ", tupletActualNotes: " << fTupletActualNotes <<
    ", tupletNormalNotes: " << fTupletNormalNotes <<
    ']';

  return s.str ();
}

std::string msrTupletFactor::asFractionString () const
{
  std::stringstream s;

  s << fTupletActualNotes << '/' << fTupletNormalNotes;

  return s.str ();
}

void msrTupletFactor::print (std::ostream& os) const
{
  os <<
    "TupletFactor" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 11;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletActualNotes" << " : " << fTupletActualNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletNormalNotes" << " : " << fTupletNormalNotes <<
    std::endl;

  --gIndenter;
};

std::ostream& operator << (std::ostream& os, const msrTupletFactor& elt)
{
  elt.print (os);
  return os;
}


}
