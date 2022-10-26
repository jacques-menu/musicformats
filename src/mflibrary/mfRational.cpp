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


using namespace std;

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
    "denom '" + to_string (denom) + "' is not positive");

  fNumerator = num;
  fDenominator = denom;

  rationalise ();
}

Rational::Rational(const Rational& pos)
{
  fNumerator   = pos.fNumerator;
  fDenominator = pos.fDenominator;
}

Rational::Rational (const string &theString)
{
  // decipher theString
  string regularExpression (
    "([-|+]?[[:digit:]]+)"  // numerator
    "/"
    "([[:digit:]]+)"        // denominator
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

  if (smSize == 3) {
    // found a well-formed specification,
    // need to check its contents
    string
      numerator   = sm [1],
      denominator = sm [2];

#ifdef TRACING_IS_ENABLED
    if (false) {
      gLogStream <<
        "--> numerator = \"" << numerator << "\", " <<
        "--> denominator = \"" << denominator << "\"" <<
        endl;
    }
#endif

    // extract the numerator
    {
      stringstream s;
      s << numerator;
      s >> fNumerator;
    }

    // extract the denominator
    {
      stringstream s;
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
    stringstream s;

    s <<
      "Rational string '" << theString <<
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

Rational Rational::operator + (const Rational &pos) const
{
  Rational result (
    fNumerator * pos.fDenominator + pos.fNumerator * fDenominator,
    fDenominator * pos.fDenominator);

  result.rationalise ();

  return result;
}

Rational Rational::operator - (const Rational &pos) const
{
  Rational result (
    fNumerator * pos.fDenominator - pos.fNumerator * fDenominator,
    fDenominator * pos.fDenominator);

  result.rationalise ();

  return result;
}

Rational Rational::operator * (const Rational &pos) const
{
  Rational result (
    fNumerator * pos.fNumerator,
    fDenominator * pos.fDenominator);

  result.rationalise ();

  return result;
}

Rational Rational::operator / (const Rational &pos) const
{
  Rational result (
  fNumerator * pos.fDenominator,
  fDenominator * pos.fNumerator);

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

Rational& Rational::operator += (const Rational &pos)
{
  if (fDenominator == pos.fDenominator) {
		fNumerator += pos.fNumerator;
	}
	else {
		fNumerator = fNumerator * pos.fDenominator + pos.fNumerator * fDenominator;
		fDenominator *= pos.fDenominator;
	}

  return (*this);
}

Rational& Rational::operator -= (const Rational &pos)
{
	if (fDenominator == pos.fDenominator) {
		fNumerator -= pos.fNumerator;
	}
	else {
		fNumerator = fNumerator * pos.fDenominator - pos.fNumerator * fDenominator;
		fDenominator *= pos.fDenominator;
  }

	return (*this);
}

Rational& Rational::operator *= (const Rational &pos)
{
  fNumerator   *= pos.fNumerator;
  fDenominator *= pos.fDenominator;

  return (*this);
}

Rational& Rational::operator /= (const Rational &pos)
{
  fNumerator   *= pos.fDenominator;
  fDenominator *= pos.fNumerator;

  return (*this);
}

Rational& Rational::operator = (const Rational& pos) {
  fNumerator   = pos.fNumerator;
  fDenominator = pos.fDenominator;

  return (*this);
}

Bool Rational::operator > (const Rational &pos) const
{
  // a/b > c/d if and only if a * d > b * c.
  return
    ((fNumerator * pos.fDenominator) > (fDenominator * pos.fNumerator));
}

Bool Rational::operator < (const Rational &pos) const
{
  // a/b < c/d if and only if a * d < b * c.
  return
    ((fNumerator * pos.fDenominator) < (fDenominator * pos.fNumerator));
}

Bool Rational::operator == (const Rational &pos) const
{
  // a/b < c/d if and only if a * d < b * c.
	return
	  ((fNumerator * pos.fDenominator) == (fDenominator * pos.fNumerator));
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

string Rational::toString () const
{
  ostringstream res;

  res << fNumerator << '/' << fDenominator;

  return res.str ();
}

Rational::operator string () const
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

string Rational::asString () const
{
  stringstream s;

  s << '[' << fNumerator << '/' << fDenominator << ']';

  return s.str ();
}

string Rational::asFractionString () const
{
  stringstream s;

  s << fNumerator << '/' << fDenominator;

  return s.str ();
}

void Rational::print (ostream& os) const
{
  os << "[Rational " << fNumerator << '/' << fDenominator << ']';
}

ostream& operator << (ostream& os, const Rational& pos)
{
  pos.print (os);
  return os;
}

void testRational ()
{
  Rational rat1 ("33/55");
  cout << "rat1: " << rat1 << endl;

  Rational rat2 (-1, 4);
  cout << "rat2: " << rat2 << endl;

  Rational rat3 (3, 8);
  cout << "rat3: " << rat3 << endl;

  Rational res1 = rat2 - rat3;
  cout << "res1: " << res1 << endl;
}


}

