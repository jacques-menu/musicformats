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

#include <cmath>

#include <regex>

#include "mfAssert.h"

#include "mfRational.h"

#include "mfServiceRunData.h"

#include "msrWae.h"


namespace MusicFormats
{

// Rational
//______________________________________________________________________________
Rational::Rational (
  long int num,
  long int denom)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    denom > 0,
    "denom '" + std::to_string (denom) + "' is not positive");

  fNumerator = num;
  fDenominator = denom;

  rationalise ();
}

Rational::Rational(const Rational& rat)
{
  fNumerator   = rat.fNumerator;
  fDenominator = rat.fDenominator;
}

Rational::Rational (const std::string &theString)
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
      numerator   = sm [1],
      denominator = sm [2];

#ifdef OAH_TRACING_IS_ENABLED
    if (false) {
      gLogStream <<
        "--> numerator = \"" << numerator << "\", " <<
        "--> denominator = \"" << denominator << "\"" <<
        std::endl;
    }
#endif

    // extract the numerator
    {
      std::stringstream s;
      s << numerator;
      s >> fNumerator;
    }

    // extract the denominator
    {
      std::stringstream s;
      s << denominator;
      s >> fDenominator;

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        fDenominator > 0,
        "fDenominator '" + denominator + "' is not positive");
    }
  }

  else {
    std::stringstream s;

    s <<
      "Rational std::string '" << theString <<
      "' is ill-formed";

    msrError (
//    msrWarning ( //  JMI
      gGlobalServiceRunData->getInputSourceName (),
      0, // JMI inputLineNumber, ??? v0.9.66
      __FILE__, __LINE__,
      s.str ());
  }

  rationalise ();
}

Rational::~Rational ()
{}

Rational Rational::inverse () const
{
  Rational result (
    fDenominator, fNumerator);

  result.rationalise ();

  return result;
}

Rational Rational::opposite () const
{
  Rational result (
    -fNumerator, fDenominator);

  result.rationalise ();

  return result;
}

Rational Rational::operator + (const Rational &rat) const
{
  Rational result (
    fNumerator * rat.fDenominator + rat.fNumerator * fDenominator,
    fDenominator * rat.fDenominator);

  result.rationalise ();

  return result;
}

Rational Rational::operator - (const Rational &rat) const
{
  Rational result (
    fNumerator * rat.fDenominator - rat.fNumerator * fDenominator,
    fDenominator * rat.fDenominator);

  result.rationalise ();

  return result;
}

Rational Rational::operator * (const Rational &rat) const
{
  Rational result (
    fNumerator * rat.fNumerator,
    fDenominator * rat.fDenominator);

  result.rationalise ();

  return result;
}

Rational Rational::operator / (const Rational &rat) const
{
  Rational result (
  fNumerator * rat.fDenominator,
  fDenominator * rat.fNumerator);

  result.rationalise ();

  return result;
}

Rational Rational::operator * (int num) const
{
  Rational result (
    fNumerator * num,
    fDenominator);

  result.rationalise ();

  return result;
}

Rational Rational::operator / (int num) const
{
  Rational result (
      fNumerator,
      fDenominator * num);

  result.rationalise ();

  return result;
}

Rational& Rational::operator += (const Rational &rat)
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

Rational& Rational::operator -= (const Rational &rat)
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

Rational& Rational::operator *= (const Rational &rat)
{
  fNumerator   *= rat.fNumerator;
  fDenominator *= rat.fDenominator;

  rationalise ();

  return (*this);
}

Rational& Rational::operator /= (const Rational &rat)
{
  fNumerator   *= rat.fDenominator;
  fDenominator *= rat.fNumerator;

  rationalise ();

  return (*this);
}

Rational& Rational::operator = (const Rational& rat) {
  fNumerator   = rat.fNumerator;
  fDenominator = rat.fDenominator;

  return (*this);
}

Bool Rational::operator > (const Rational &rat) const
{
  // a/b > c/d if and only if a * d > b * c.
  return
    ((fNumerator * rat.fDenominator) > (fDenominator * rat.fNumerator));
}

Bool Rational::operator < (const Rational &rat) const
{
  // a/b < c/d if and only if a * d < b * c.
  return
    ((fNumerator * rat.fDenominator) < (fDenominator * rat.fNumerator));
}

Bool Rational::operator == (const Rational &rat) const
{
  // a/b < c/d if and only if a * d < b * c.
  return
    ((fNumerator * rat.fDenominator) == (fDenominator * rat.fNumerator));
}

Bool Rational::operator > (double num) const
{
  return (toDouble() > num);
}
Bool Rational::operator >= (double num) const
{
  return (toDouble() >= num);
}
Bool Rational::operator < (double num) const
{
  return (toDouble() < num);
}
Bool Rational::operator <= (double num) const
{
  return (toDouble() <= num);
}
Bool Rational::operator == (double num) const
{
  return (toDouble() == num); }

// gcd(a, b) calculates the gcd of a and b using Euclid's algorithm.
long int Rational::gcd (long int a1, long int b1)
{
  long int r;

  long int a = labs (a1);
  long int b = labs (b1);

  if (! (a == 0) || (b == 0)) {
    while (b > 0){
      r = a % b;
      a = b;
      b = r;
    } // while

    return a;
  }

  return 1;
}

void Rational::rationalise ()
{
  long int g = gcd (fNumerator, fDenominator);

  fNumerator   /= g;
  fDenominator /= g;

  if (fNumerator == 0) {
    fDenominator = 1;
  }
  else if (fDenominator < 0) {
    fNumerator   = -fNumerator;
    fDenominator = -fDenominator;
  }
}

double Rational::toDouble () const
{
  return
    (fDenominator != 0) ? ((double)fNumerator/(double)fDenominator) : 0;
}

float Rational::toFloat () const
{
  return
    (fDenominator != 0) ? ((float)fNumerator/(float)fDenominator) : 0;
}

std::string Rational::toString () const
{
  std::ostringstream res;

  res << fNumerator << '/' << fDenominator;

  return res.str ();
}

Rational::operator std::string () const
{
  return toString ();
}
Rational::operator double () const
{
  return toDouble ();
}
Rational::operator float () const
{
  return toFloat ();
}
Rational::operator int () const
{
  const double x = toDouble ();

  return ((int) floor (x + 0.5f));
}

std::string Rational::asString () const
{
  std::stringstream s;

  s << '[' << fNumerator << '/' << fDenominator << ']';

  return s.str ();
}

std::string Rational::asFractionString () const
{
  std::stringstream s;

  s << fNumerator << '/' << fDenominator;

  return s.str ();
}

void Rational::print (std::ostream& os) const
{
  os << "[Rational " << fNumerator << '/' << fDenominator << ']';
}

std::ostream& operator << (std::ostream& os, const Rational& rat)
{
  rat.print (os);
  return os;
}

void testRational ()
{
  Rational rat1 ("33/55");
  std::cout << "rat1: " << rat1 << std::endl;

  Rational rat2 (-1, 4);
  std::cout << "rat2: " << rat2 << std::endl;

  Rational rat3 (3, 8);
  std::cout << "rat3: " << rat3 << std::endl;

  Rational res1 = rat2 - rat3;
  std::cout << "res1: " << res1 << std::endl;
}


}

