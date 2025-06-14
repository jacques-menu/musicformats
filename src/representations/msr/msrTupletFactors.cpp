/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw()), set::precision(), ...

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

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI 0.9.70
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fTupletActualNotes > 0,
    "fTupletActualNotes is not positive");

  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fTupletNormalNotes > 0,
    "fTupletNormalNotes is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
}

msrTupletFactor::msrTupletFactor (
  const mfRational& rationalTupletFactor)
{
  fTupletActualNotes = rationalTupletFactor.getNumerator ();
  fTupletNormalNotes = rationalTupletFactor.getDenominator ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check JMI 0.9.70
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fTupletActualNotes > 0,
    "fTupletActualNotes is not positive");

  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fTupletNormalNotes > 0,
    "fTupletNormalNotes is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
}

msrTupletFactor::~msrTupletFactor ()
{}

msrTupletFactor msrTupletFactor::inverse () const
{
  msrTupletFactor
    result (
      fTupletNormalNotes, fTupletActualNotes);

  return result;
}

std::string msrTupletFactor::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[TupletFactor" <<
    ", tupletActualNotes: " << fTupletActualNotes <<
    ", tupletNormalNotes: " << fTupletNormalNotes <<
    ']';

  return ss.str ();
}

std::string msrTupletFactor::asFractionString () const
{
  std::stringstream ss;

  ss << fTupletActualNotes << '/' << fTupletNormalNotes;

  return ss.str ();
}

void msrTupletFactor::print (std::ostream& os) const
{
  os <<
    "[TupletFactor" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 11;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletActualNotes" << ": " << fTupletActualNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTupletNormalNotes" << ": " << fTupletNormalNotes <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
};

std::ostream& operator << (std::ostream& os, const msrTupletFactor& elt)
{
  os << elt.asShortString ();
  return os;
}


}
