/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cmath>
#include <iostream>
#include <regex>
#include <sstream>

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfServices.h"

#include "msrWholeNotes.h"

#include "oahEarlyOptions.h"

#include "msrWae.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrWholeNotes::msrWholeNotes (
  long int num,
  long int denom)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    denom > 0,
    "denom '" + std::to_string (denom) + "' is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fNumerator = num;
  fDenominator = denom;

  rationalise ();
}

msrWholeNotes::msrWholeNotes(const msrWholeNotes& wholeNotes)
{
  fNumerator   = wholeNotes.fNumerator;
  fDenominator = wholeNotes.fDenominator;
}

msrWholeNotes::msrWholeNotes (const std::string &theString)
{
  // decipher theString
  std::string regularExpression (
    "([-|+]?[[:digit:]]+)"  // numewholeNotesor
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
      numewholeNotesor   = sm [1],
      denominator = sm [2];

#ifdef MF_TRACE_IS_ENABLED
    if (false) {
	  	std::stringstream ss;

      ss <<
        "--> numewholeNotesor = \"" << numewholeNotesor << "\", " <<
        "--> denominator = \"" << denominator << "\"" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // extract the numewholeNotesor
    {
      std::stringstream ss;
      ss << numewholeNotesor;
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
      "msrWholeNotes std::string '" << theString <<
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

msrWholeNotes::~msrWholeNotes ()
{}

msrWholeNotes msrWholeNotes::inverse () const
{
  msrWholeNotes
  	result (
    	fDenominator, fNumerator);

  result.rationalise ();

  return result;
}

msrWholeNotes msrWholeNotes::opposite () const
{
  msrWholeNotes
  	result (
    	-fNumerator, fDenominator);

  result.rationalise ();

  return result;
}

msrWholeNotes msrWholeNotes::operator + (const msrWholeNotes &wholeNotes) const
{
  msrWholeNotes
  	result (
    	fNumerator * wholeNotes.fDenominator + wholeNotes.fNumerator * fDenominator,
    	fDenominator * wholeNotes.fDenominator);

  result.rationalise ();

  return result;
}

msrWholeNotes msrWholeNotes::operator - (const msrWholeNotes &wholeNotes) const
{
  msrWholeNotes
  	result (
    	fNumerator * wholeNotes.fDenominator - wholeNotes.fNumerator * fDenominator,
   		fDenominator * wholeNotes.fDenominator);

  result.rationalise ();

  return result;
}

msrWholeNotes msrWholeNotes::operator * (const mfRational &rat) const
{
  msrWholeNotes
  	result (
    	fNumerator * rat.getNumerator (),
    	fDenominator * rat.getDenominator ());

  result.rationalise ();

  return result;
}

msrWholeNotes msrWholeNotes::operator / (const mfRational &rat) const
{
  msrWholeNotes
  	result (
			fNumerator * rat.getDenominator (),
			fDenominator * rat.getNumerator ());

  result.rationalise ();

  return result;
}

msrWholeNotes msrWholeNotes::operator * (int num) const
{
  msrWholeNotes
  	result (
    	fNumerator * num,
    	fDenominator);

  result.rationalise ();

  return result;
}

msrWholeNotes msrWholeNotes::operator / (int num) const
{
  msrWholeNotes
  	result (
			fNumerator,
			fDenominator * num);

  result.rationalise ();

  return result;
}

mfRational msrWholeNotes::operator / (const msrWholeNotes &wholeNotes) const
{
	mfRational
		result (
			fNumerator * wholeNotes.fDenominator,
			fDenominator * wholeNotes.fNumerator);

  return result;
}

msrWholeNotes& msrWholeNotes::operator += (const msrWholeNotes &wholeNotes)
{
  if (fDenominator == wholeNotes.fDenominator) {
    fNumerator += wholeNotes.fNumerator;
  }
  else {
    fNumerator =
    	fNumerator * wholeNotes.fDenominator + wholeNotes.fNumerator * fDenominator;
    fDenominator *=
    	wholeNotes.fDenominator;
  }

  rationalise ();

  return (*this);
}

msrWholeNotes& msrWholeNotes::operator -= (const msrWholeNotes &wholeNotes)
{
  if (fDenominator == wholeNotes.fDenominator) {
    fNumerator -= wholeNotes.fNumerator;
  }
  else {
    fNumerator =
    	fNumerator * wholeNotes.fDenominator
    		-
    	wholeNotes.fNumerator * fDenominator;
    fDenominator *= wholeNotes.fDenominator;
  }

  rationalise ();

  return (*this);
}

msrWholeNotes& msrWholeNotes::operator *= (const mfRational &rat)
{
  fNumerator   *= rat.getNumerator ();
  fDenominator *= rat.getDenominator ();

  rationalise ();

  return (*this);
}

msrWholeNotes& msrWholeNotes::operator /= (const mfRational &rat)
{
  fNumerator   /= rat.getDenominator (); // JMI v0.9.67
  fDenominator /= rat.getNumerator ();

  rationalise ();

  return (*this);
}

msrWholeNotes& msrWholeNotes::operator = (const msrWholeNotes& wholeNotes) {
  fNumerator   = wholeNotes.fNumerator;
  fDenominator = wholeNotes.fDenominator;

  return (*this);
}

Bool msrWholeNotes::operator > (const msrWholeNotes &wholeNotes) const
{
  // a/b > c/d if and only if a * d > b * c.
  return
    ((fNumerator * wholeNotes.fDenominator) > (fDenominator * wholeNotes.fNumerator));
}

Bool msrWholeNotes::operator < (const msrWholeNotes &wholeNotes) const
{
  // a/b < c/d if and only if a * d < b * c.
  return
    ((fNumerator * wholeNotes.fDenominator) < (fDenominator * wholeNotes.fNumerator));
}

Bool msrWholeNotes::operator == (const msrWholeNotes &wholeNotes) const
{
  // a/b < c/d if and only if a * d < b * c.
  return
    ((fNumerator * wholeNotes.fDenominator) == (fDenominator * wholeNotes.fNumerator));
}

Bool msrWholeNotes::operator > (double num) const
{
  return (toDouble() > num);
}
Bool msrWholeNotes::operator >= (double num) const
{
  return (toDouble() >= num);
}
Bool msrWholeNotes::operator < (double num) const
{
  return (toDouble() < num);
}
Bool msrWholeNotes::operator <= (double num) const
{
  return (toDouble() <= num);
}
Bool msrWholeNotes::operator == (double num) const
{
  return (toDouble() == num); }

// gcd(a, b) calculates the gcd of a and b using Euclid's algorithm.
long int msrWholeNotes::gcd (long int a1, long int b1)
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

void msrWholeNotes::rationalise ()
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

double msrWholeNotes::toDouble () const
{
  return
    (fDenominator != 0) ? ((double)fNumerator/(double)fDenominator) : 0;
}

float msrWholeNotes::toFloat () const
{
  return
    (fDenominator != 0) ? ((float)fNumerator/(float)fDenominator) : 0;
}

std::string msrWholeNotes::toString () const
{
  std::ostringstream res;

  res << fNumerator << '/' << fDenominator;

  return res.str ();
}

msrWholeNotes::operator std::string () const
{
  return toString ();
}
msrWholeNotes::operator double () const
{
  return toDouble ();
}
msrWholeNotes::operator float () const
{
  return toFloat ();
}
msrWholeNotes::operator int () const
{
  const double x = toDouble ();

  return ((int) floor (x + 0.5f));
}

std::string msrWholeNotes::asString () const
{
  std::stringstream ss;

  ss << '[' << fNumerator << '/' << fDenominator << ']';

  return ss.str ();
}

std::string msrWholeNotes::asFractionString () const
{
  std::stringstream ss;

  ss << fNumerator << '/' << fDenominator;

  return ss.str ();
}

void msrWholeNotes::print (std::ostream& os) const
{
  os << "[msrWholeNotes " << fNumerator << '/' << fDenominator << ']';
}

std::ostream& operator << (std::ostream& os, const msrWholeNotes& wholeNotes)
{
  wholeNotes.print (os);
  return os;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const msrWholeNotes& wholeNotes)
{
  iss.getStringstream () <<
    wholeNotes.asString ();

  return iss;
}

//______________________________________________________________________________
EXP const msrWholeNotes K_WHOLE_NOTES_UNKNOWN (-444, 1);

//______________________________________________________________________________
void testWholeNotes ()
{
  msrWholeNotes wholeNotes1 ("33/55");
  std::cout << "wholeNotes1: " << wholeNotes1 << std::endl;

  msrWholeNotes wholeNotes2 (-1, 4);
  std::cout << "wholeNotes2: " << wholeNotes2 << std::endl;

  msrWholeNotes wholeNotes3 (3, 8);
  std::cout << "wholeNotes3: " << wholeNotes3 << std::endl;

  msrWholeNotes res1 = wholeNotes2 - wholeNotes3;
  std::cout << "res1: " << res1 << std::endl;
}


}

