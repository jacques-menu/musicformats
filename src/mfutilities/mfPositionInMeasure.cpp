/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cmath>
#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw()), set::precision(), ...
#include <regex>
#include <numeric>      // gcd()

#include <iostream>
#include <sstream>
#include <regex>

#include "mfPreprocessorSettings.h"

#include "mfPositionInMeasure.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfServices.h"

#include "mfDurations.h"

#include "oahEarlyOptions.h"

#include "msrWae.h"

#include "waeHandlers.h"


namespace MusicFormats
{
//______________________________________________________________________________
mfPositionInMeasure mfPositionInMeasure::createFromWholeNotes (
  const mfWholeNotes& wholeNotes)
{
  return
    mfPositionInMeasure (
      wholeNotes.getNumerator (),
      wholeNotes.getDenominator ());
}

mfPositionInMeasure::mfPositionInMeasure ()
{
  fNumerator = 0;
  fDenominator = 1;
}

mfPositionInMeasure::mfPositionInMeasure (
  int numerator,
  int denominator)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     numerator >= 0,
//     "mfPositionInMeasure numerator '" + std::to_string (numerator) + "' should be positive or null");

  mfAssert (
    __FILE__, __LINE__,
    denominator > 0,
    "mfPositionInMeasure denominator '" + std::to_string (denominator) + "' should be positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fNumerator = numerator;
  fDenominator = denominator;

  rationalise ();
}

mfPositionInMeasure::mfPositionInMeasure (
  const mfPositionInMeasure& positionInMeasure)
{
  fNumerator = positionInMeasure.getNumerator ();
  fDenominator = positionInMeasure.getDenominator ();
}

mfPositionInMeasure::mfPositionInMeasure (const std::string& theString)
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
      numewholeNotesor = sm [1],
      denominator = sm [2];

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
      std::stringstream ss;

      ss <<
        "--> numewholeNotesor = \"" << numewholeNotesor << "\", " <<
        "--> denominator = \"" << denominator << "\"";

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
      "mfPositionInMeasure string '" << theString <<
      "' is ill-formed";

    msrError (
//    msrWarning ( //  JMI
      gServiceRunData->getInputSourceName (),
      0, // JMI inputLineNumber, ??? 0.9.66
      __FILE__, __LINE__,
      ss.str ());
  }

  rationalise ();
}

mfPositionInMeasure::~mfPositionInMeasure ()
{}

mfPositionInMeasure mfPositionInMeasure::operator + (
  const mfWholeNotes &wholeNotes) const
{
  mfPositionInMeasure
    result (
      fNumerator * wholeNotes.getDenominator ()
        +
      wholeNotes.getNumerator () * fDenominator,
      fDenominator * wholeNotes.getDenominator ());

  result.rationalise ();

  return result;
}

mfPositionInMeasure mfPositionInMeasure::operator - (
  const mfWholeNotes &wholeNotes) const
{
  mfPositionInMeasure
    result (
      fNumerator * wholeNotes.getDenominator ()
        -
      wholeNotes.getNumerator () * fDenominator,
      fDenominator * wholeNotes.getDenominator ());

  result.rationalise ();

  return result;
}

mfPositionInMeasure& mfPositionInMeasure::operator += (
  const mfWholeNotes &wholeNotes)
{
  if (fDenominator == wholeNotes.getDenominator ()) {
    fNumerator += wholeNotes.getNumerator ();
  }
  else {
    fNumerator =
      fNumerator * wholeNotes.getDenominator ()
        +
    wholeNotes.getNumerator () * fDenominator;

    fDenominator *=
      wholeNotes.getDenominator ();
  }

  rationalise ();

  return (*this);
}

mfPositionInMeasure& mfPositionInMeasure::operator -= (
  const mfWholeNotes &wholeNotes)
{
  if (fDenominator == wholeNotes.getDenominator ()) {
    fNumerator -= wholeNotes.getNumerator ();
  }
  else {
    fNumerator =
      fNumerator * wholeNotes.getDenominator ()
        -
      wholeNotes.getNumerator () * fDenominator;
    fDenominator *= wholeNotes.getDenominator ();
  }

  rationalise ();

  return (*this);
}

mfWholeNotes mfPositionInMeasure::operator - (
  const mfPositionInMeasure &positionInMeasure) const
{
  mfWholeNotes
    result (
      fNumerator * positionInMeasure.getDenominator ()
        -
      positionInMeasure.getNumerator () * fDenominator,
      fDenominator * positionInMeasure.getDenominator ());

  result.rationalise ();

  return result;
}

mfPositionInMeasure& mfPositionInMeasure::operator = (
  const mfPositionInMeasure& positionInMeasure)
{
  fNumerator = positionInMeasure.fNumerator;
  fDenominator = positionInMeasure.fDenominator;

  return (*this);
}

Bool mfPositionInMeasure::operator > (
  const mfPositionInMeasure& positionInMeasure) const
{
  // a/b > c/d if and only if a * d > b * c.
  return
    (
      (fNumerator * positionInMeasure.fDenominator)
        >
      (fDenominator * positionInMeasure.fNumerator)
    );
}

Bool mfPositionInMeasure::operator < (
  const mfPositionInMeasure& positionInMeasure) const
{
  // a/b < c/d if and only if a * d < b * c.
  return
    (
      (fNumerator * positionInMeasure.fDenominator)
        <
      (fDenominator * positionInMeasure.fNumerator)
    );
}

Bool mfPositionInMeasure::operator == (
  const mfPositionInMeasure& positionInMeasure) const
{
  // a/b == c/d if and only if a * d == b * c.
  return
    (
      (fNumerator * positionInMeasure.fDenominator)
        ==
      (fDenominator * positionInMeasure.fNumerator)
    );
}

Bool mfPositionInMeasure::operator > (double num) const
{
  return (toDouble() > num);
}
Bool mfPositionInMeasure::operator >= (double num) const
{
  return (toDouble() >= num);
}
Bool mfPositionInMeasure::operator < (double num) const
{
  return (toDouble() < num);
}
Bool mfPositionInMeasure::operator <= (double num) const
{
  return (toDouble() <= num);
}
Bool mfPositionInMeasure::operator == (double num) const
{
  return (toDouble() == num); }

void mfPositionInMeasure::rationalise ()
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

double mfPositionInMeasure::toDouble () const
{
  return
    (fDenominator != 0) ? ((double)fNumerator/(double)fDenominator) : 0;
}

float mfPositionInMeasure::toFloat () const
{
  return
    (fDenominator != 0) ? ((float)fNumerator/(float)fDenominator) : 0;
}

std::string mfPositionInMeasure::toString () const
{
  std::ostringstream res;

  res << fNumerator << '/' << fDenominator;

  return res.str ();
}

mfPositionInMeasure::operator std::string () const
{
  return toString ();
}
mfPositionInMeasure::operator double () const
{
  return toDouble ();
}
mfPositionInMeasure::operator float () const
{
  return toFloat ();
}
mfPositionInMeasure::operator int () const
{
  const double x = toDouble ();

  return ((int) floor (x + 0.5f));
}

mfWholeNotes mfPositionInMeasure::asWholeNotes () const
{
  return
    mfWholeNotes (fNumerator, fDenominator);
}

std::string mfPositionInMeasure::asShortString () const
{
  std::stringstream ss;

  if (fNumerator == K_POSITION_IN_MEASURE_NUMERATOR_UNKNOWN_ ) {
    ss << "K_POSITION_IN_MEASURE_UNKNOWN_";
  }
  else {
    ss << "pim " << fNumerator << '/' << fDenominator;
  }

  return ss.str ();
}

std::string mfPositionInMeasure::asFractionString () const
{
  std::stringstream ss;

  ss << "pim " << fNumerator << '/' << fDenominator;

  return ss.str ();
}

void mfPositionInMeasure::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (
  std::ostream&              os,
  const mfPositionInMeasure& positionInMeasure)
{
  os << positionInMeasure.asShortString ();
  return os;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream&    iss,
  const mfPositionInMeasure& positionInMeasure)
{
  iss.getStringstream () <<
    positionInMeasure.asString ();

  return iss;
}


}

