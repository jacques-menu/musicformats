/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cmath>
#include <iostream>
#include <regex>
#include <sstream>
#include <numeric>      // gcd()

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfRational.h"
#include "mfServices.h"

#include "oahEarlyOptions.h"

#include "msrWae.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
mfRational::mfRational (
  int num,
  int denom)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
if (false) // JMI v0.9.70
  mfAssert (
    __FILE__, __LINE__,
    denom > 0,
    "denom '" + std::to_string (denom) + "' is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fNumerator = num;
  fDenominator = denom;

  rationalise ();
}

mfRational::mfRational(const mfRational& rat)
{
  fNumerator = rat.fNumerator;
  fDenominator = rat.fDenominator;
}

mfRational::mfRational (const std::string& theString)
{
  // decipher theString
  std::string regularExpression (
    "([-|+]?[[:digit:]]+)"  // numerator
    "/"
    "([[:digit:]]+)"        // denominator
    );

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

  if (smSize == 3) {
    // found a well-formed specification,
    // need to check its contents
    std::string
      numerator = sm [1],
      denominator = sm [2];

#ifdef MF_TRACE_IS_ENABLED
    if (false) {
      std::stringstream ss;

      ss <<
        "--> numerator = \"" << numerator << "\", " <<
        "--> denominator = \"" << denominator << "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // extract the numerator
    {
      std::stringstream ss;
      ss << numerator;
      ss >> fNumerator;
    }

    // extract the denominator
    {
      std::stringstream ss;
      ss << denominator;
      ss >> fDenominator;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
      mfAssert (
        __FILE__, __LINE__,
        fDenominator > 0,
        "fDenominator '" + denominator + "' is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
    }
  }

  else {
    std::stringstream ss;

    ss <<
      "mfRational std::string '" << theString <<
      "' is ill-formed";

    msrError (
//    msrWarning ( //  JMI
      gServiceRunData->getInputSourceName (),
      0, // JMI inputLineNumber, ??? v0.9.66
      __FILE__, __LINE__,
      ss.str ());
  }

  rationalise ();
}

mfRational::~mfRational ()
{}

mfRational mfRational::inverse () const
{
  mfRational
    result (
      fDenominator, fNumerator);

  result.rationalise ();

  return result;
}

mfRational mfRational::opposite () const
{
  mfRational
    result (
      -fNumerator, fDenominator);

  result.rationalise ();

  return result;
}

mfRational mfRational::operator + (const mfRational &rat) const
{
  mfRational
    result (
      fNumerator * rat.fDenominator + rat.fNumerator * fDenominator,
       fDenominator * rat.fDenominator);

  result.rationalise ();

  return result;
}

mfRational mfRational::operator - (const mfRational &rat) const
{
  mfRational
    result (
      fNumerator * rat.fDenominator - rat.fNumerator * fDenominator,
      fDenominator * rat.fDenominator);

  result.rationalise ();

  return result;
}

mfRational mfRational::operator * (const mfRational &rat) const
{
  mfRational
    result (
      fNumerator * rat.fNumerator,
      fDenominator * rat.fDenominator);

  result.rationalise ();

  return result;
}

mfRational mfRational::operator / (const mfRational &rat) const
{
  mfRational
    result (
      fNumerator * rat.fDenominator,
      fDenominator * rat.fNumerator);

  result.rationalise ();

  return result;
}

mfRational mfRational::operator * (int num) const
{
  mfRational
    result (
      fNumerator * num,
      fDenominator);

  result.rationalise ();

  return result;
}

mfRational mfRational::operator / (int num) const
{
  mfRational
    result (
      fNumerator,
      fDenominator * num);

  result.rationalise ();

  return result;
}

mfRational& mfRational::operator += (const mfRational &rat)
{
  if (fDenominator == rat.fDenominator) {
    fNumerator += rat.fNumerator;
  }
  else {
    fNumerator = fNumerator * rat.fDenominator + rat.fNumerator * fDenominator;
    fDenominator *= rat.fDenominator;
  }

  rationalise ();

  return (*this);
}

mfRational& mfRational::operator -= (const mfRational &rat)
{
  if (fDenominator == rat.fDenominator) {
    fNumerator -= rat.fNumerator;
  }
  else {
    fNumerator = fNumerator * rat.fDenominator - rat.fNumerator * fDenominator;
    fDenominator *= rat.fDenominator;
  }

  rationalise ();

  return (*this);
}

mfRational& mfRational::operator *= (const mfRational &rat)
{
  fNumerator   *= rat.fNumerator;
  fDenominator *= rat.fDenominator;

  rationalise ();

  return (*this);
}

mfRational& mfRational::operator /= (const mfRational &rat)
{
  fNumerator   *= rat.fDenominator;
  fDenominator *= rat.fNumerator;

  rationalise ();

  return (*this);
}

mfRational& mfRational::operator = (const mfRational& rat) {
  fNumerator = rat.fNumerator;
  fDenominator = rat.fDenominator;

  return (*this);
}

Bool mfRational::operator > (const mfRational &rat) const
{
  // a/b > c/d if and only if a * d > b * c.
  return
    ((fNumerator * rat.fDenominator) > (fDenominator * rat.fNumerator));
}

Bool mfRational::operator < (const mfRational &rat) const
{
  // a/b < c/d if and only if a * d < b * c.
  return
    ((fNumerator * rat.fDenominator) < (fDenominator * rat.fNumerator));
}

Bool mfRational::operator == (const mfRational &rat) const
{
  // a/b < c/d if and only if a * d < b * c.
  return
    ((fNumerator * rat.fDenominator) == (fDenominator * rat.fNumerator));
}

Bool mfRational::operator > (double num) const
{
  return (toDouble() > num);
}
Bool mfRational::operator >= (double num) const
{
  return (toDouble() >= num);
}
Bool mfRational::operator < (double num) const
{
  return (toDouble() < num);
}
Bool mfRational::operator <= (double num) const
{
  return (toDouble() <= num);
}
Bool mfRational::operator == (double num) const
{
  return (toDouble() == num); }

void mfRational::rationalise ()
{
  int g = std::gcd (fNumerator, fDenominator);

  fNumerator   /= g;
  fDenominator /= g;

  if (fNumerator == 0) {
    fDenominator = 1;
  }
  else if (fDenominator < 0) {
    fNumerator = -fNumerator;
    fDenominator = -fDenominator;
  }
}

double mfRational::toDouble () const
{
  return
    (fDenominator != 0) ? ((double)fNumerator/(double)fDenominator) : 0;
}

float mfRational::toFloat () const
{
  return
    (fDenominator != 0) ? ((float)fNumerator/(float)fDenominator) : 0;
}

std::string mfRational::toString () const
{
  std::ostringstream res;

  res << fNumerator << '/' << fDenominator;

  return res.str ();
}

mfRational::operator std::string () const
{
  return toString ();
}
mfRational::operator double () const
{
  return toDouble ();
}
mfRational::operator float () const
{
  return toFloat ();
}
mfRational::operator int () const
{
  const double x = toDouble ();

  return ((int) floor (x + 0.5f));
}

std::string mfRational::asString () const
{
  std::stringstream ss;

  ss << '[' << fNumerator << '/' << fDenominator << ']';

  return ss.str ();
}

std::string mfRational::asFractionString () const
{
  std::stringstream ss;

  ss << fNumerator << '/' << fDenominator;

  return ss.str ();
}

void mfRational::print (std::ostream& os) const
{
  os << "[mfRational " << fNumerator << '/' << fDenominator << ']';
}

std::ostream& operator << (std::ostream& os, const mfRational& rat)
{
  rat.print (os);
  return os;
}

void testRational ()
{
  mfRational rat1 ("33/55");
  std::cout << "rat1: " << rat1 << std::endl;

  mfRational rat2 (-1, 4);
  std::cout << "rat2: " << rat2 << std::endl;

  mfRational rat3 (3, 8);
  std::cout << "rat3: " << rat3 << std::endl;

  mfRational res1 = rat2 - rat3;
  std::cout << "res1: " << res1 << std::endl;
}


}

