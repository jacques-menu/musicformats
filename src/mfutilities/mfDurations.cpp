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

#include "mfDurations.h"

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
mfWholeNotes::mfWholeNotes ()
{
  fNumerator = 0;
  fDenominator = 1;
}

mfWholeNotes::mfWholeNotes (
  int numerator,
  int denominator)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     numerator >= 0,
//     "mfWholeNotes numerator '" + std::to_string (numerator) + "' should be positive or null");

  mfAssert (
    __FILE__, __LINE__,
    denominator > 0,
    "mfWholeNotes denominator '" + std::to_string (denominator) + "' should be positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fNumerator = numerator;
  fDenominator = denominator;

  rationalise ();
}

mfWholeNotes::mfWholeNotes (const mfWholeNotes& wholeNotes)
{
  fNumerator = wholeNotes.fNumerator;
  fDenominator = wholeNotes.fDenominator;
}

mfWholeNotes::mfWholeNotes (const std::string& theString)
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
      "mfWholeNotes string '" << theString <<
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

mfWholeNotes::~mfWholeNotes ()
{}

mfWholeNotes mfWholeNotes::inverse () const
{
  mfWholeNotes
    result (
      fDenominator, fNumerator);

  result.rationalise ();

  return result;
}

mfWholeNotes mfWholeNotes::opposite () const
{
  mfWholeNotes
    result (
      -fNumerator, fDenominator);

  result.rationalise ();

  return result;
}

mfWholeNotes mfWholeNotes::operator + (const mfWholeNotes &wholeNotes) const
{
  mfWholeNotes
    result (
      fNumerator * wholeNotes.fDenominator + wholeNotes.fNumerator * fDenominator,
      fDenominator * wholeNotes.fDenominator);

  result.rationalise ();

  return result;
}

mfWholeNotes mfWholeNotes::operator - (const mfWholeNotes &wholeNotes) const
{
  mfWholeNotes
    result (
      fNumerator * wholeNotes.fDenominator - wholeNotes.fNumerator * fDenominator,
      fDenominator * wholeNotes.fDenominator);

  result.rationalise ();

  return result;
}

mfWholeNotes mfWholeNotes::operator * (const mfRational &rat) const
{
  mfWholeNotes
    result (
      fNumerator * rat.getNumerator (),
      fDenominator * rat.getDenominator ());

  result.rationalise ();

  return result;
}

mfWholeNotes mfWholeNotes::operator / (const mfRational &rat) const
{
  mfWholeNotes
    result (
      fNumerator * rat.getDenominator (),
      fDenominator * rat.getNumerator ());

  result.rationalise ();

  return result;
}

mfWholeNotes mfWholeNotes::operator * (int num) const
{
  mfWholeNotes
    result (
      fNumerator * num,
      fDenominator);

  result.rationalise ();

  return result;
}

mfWholeNotes mfWholeNotes::operator / (int num) const
{
  mfWholeNotes
    result (
      fNumerator,
      fDenominator * num);

  result.rationalise ();

  return result;
}

mfRational mfWholeNotes::operator / (const mfWholeNotes &wholeNotes) const
{
  mfRational
    result (
      fNumerator * wholeNotes.fDenominator,
      fDenominator * wholeNotes.fNumerator);

  return result;
}

mfWholeNotes& mfWholeNotes::operator += (const mfWholeNotes &wholeNotes)
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

mfWholeNotes& mfWholeNotes::operator -= (const mfWholeNotes &wholeNotes)
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

mfWholeNotes& mfWholeNotes::operator *= (const mfRational &rat)
{
  fNumerator   *= rat.getNumerator ();
  fDenominator *= rat.getDenominator ();

  rationalise ();

  return (*this);
}

mfWholeNotes& mfWholeNotes::operator /= (const mfRational &rat)
{
  fNumerator   /= rat.getDenominator (); // JMI 0.9.67
  fDenominator /= rat.getNumerator ();

  rationalise ();

  return (*this);
}

mfWholeNotes& mfWholeNotes::operator = (const mfWholeNotes& wholeNotes) {
  fNumerator = wholeNotes.fNumerator;
  fDenominator = wholeNotes.fDenominator;

  return (*this);
}

Bool mfWholeNotes::operator > (const mfWholeNotes &wholeNotes) const
{
  // a/b > c/d if and only if a * d > b * c.
  return
    ((fNumerator * wholeNotes.fDenominator) > (fDenominator * wholeNotes.fNumerator));
}

Bool mfWholeNotes::operator < (const mfWholeNotes &wholeNotes) const
{
  // a/b < c/d if and only if a * d < b * c.
  return
    ((fNumerator * wholeNotes.fDenominator) < (fDenominator * wholeNotes.fNumerator));
}

Bool mfWholeNotes::operator == (const mfWholeNotes &wholeNotes) const
{
  // a/b == c/d if and only if a * d == b * c.
  return
    ((fNumerator * wholeNotes.fDenominator) == (fDenominator * wholeNotes.fNumerator));
}

Bool mfWholeNotes::operator > (double num) const
{
  return (toDouble() > num);
}
Bool mfWholeNotes::operator >= (double num) const
{
  return (toDouble() >= num);
}
Bool mfWholeNotes::operator < (double num) const
{
  return (toDouble() < num);
}
Bool mfWholeNotes::operator <= (double num) const
{
  return (toDouble() <= num);
}
Bool mfWholeNotes::operator == (double num) const
{
  return (toDouble() == num); }

void mfWholeNotes::rationalise ()
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

double mfWholeNotes::toDouble () const
{
  return
    (fDenominator != 0) ? ((double)fNumerator/(double)fDenominator) : 0;
}

float mfWholeNotes::toFloat () const
{
  return
    (fDenominator != 0) ? ((float)fNumerator/(float)fDenominator) : 0;
}

std::string mfWholeNotes::toString () const
{
  std::ostringstream res;

  res << fNumerator << '/' << fDenominator;

  return res.str ();
}

mfWholeNotes::operator std::string () const
{
  return toString ();
}
mfWholeNotes::operator double () const
{
  return toDouble ();
}
mfWholeNotes::operator float () const
{
  return toFloat ();
}
mfWholeNotes::operator int () const
{
  const double x = toDouble ();

  return ((int) floor (x + 0.5f));
}

std::string mfWholeNotes::asShortString () const
{
  std::stringstream ss;

  if (fNumerator == K_WHOLE_NOTES_NUMERATOR_UNKNOWN_ ) {
    ss << "UNKNOWN_WHOLE_NOTES_";
  }
  else {
    ss << fNumerator << '/' << fDenominator << " whn";
  }

  return ss.str ();
}

std::string mfWholeNotes::asFractionString () const
{
  std::stringstream ss;

  ss << fNumerator << '/' << fDenominator << " whn";

  return ss.str ();
}

void mfWholeNotes::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const mfWholeNotes& wholeNotes)
{
  os << wholeNotes.asShortString ();
  return os;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const mfWholeNotes& wholeNotes)
{
  iss.getStringstream () <<
    wholeNotes.asString ();

  return iss;
}

//_______________________________________________________________________________
std::string wholeNotesAndDotsNumberPitchAndOctaveAsString (
  int                  inputLineNumber,
  const mfWholeNotes& wholeNotes,
  int&                 dotsNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurationsDetails ()) {
    std::stringstream ss;

    ss <<
      "--> wholeNotesAndDotsNumberPitchAndOctaveAsString() 1 -------------------------------------" <<
      ", wholeNotes: " << wholeNotes.asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int
    numerator = wholeNotes.getNumerator (),
    denominator = wholeNotes.getDenominator ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurationsDetails ()) {
    std::stringstream ss;

    ss <<
      "--> numerator:   " << numerator <<
      std::endl <<
      "--> denominator: " << denominator <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (numerator == 0) { // JMI TEMP 0.9.70
    dotsNumber = 0;
    return "ZERO";
  }

  else if (numerator == K_WHOLE_NOTES_NUMERATOR_UNKNOWN_ ) {
    return "K_WHOLE_NOTES_NUMERATOR_UNKNOWN_";
  }

  else if (numerator < 0) {
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
    std::stringstream ss;

    ss <<
      "numerator " <<
      numerator <<
      " is not positive in wholeNotesAndDotsNumberPitchAndOctaveAsString()" <<
      ", wholeNotes: " << wholeNotes.asFractionString ();

   msrError ( // JMI 0.9.70
//     msrWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
     __FILE__, __LINE__,
      ss.str ());

    return "WholeNotesAndDotsNumber_???";
#endif // MF_SANITY_CHECKS_ARE_ENABLED
  }

  // here, numerator > 0

  Bool
    integralNumberOfWholeNotes = denominator == 1;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurationsDetails ()) {
    std::stringstream ss;

    ss <<
      "--> integralNumberOfWholeNotes: " <<
      integralNumberOfWholeNotes <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  /*
    augmentation dots add half the preceding duration or increment to the duration:
    they constitue a series of frations or the form '(2^n-1) / 2^n',
    starting with 3/2, 7/4, 15/8,
    that tends towards 2 while always remaining less than two.

    with MusicXML's limitation to 1024th of a whole note,
    with LilyPond's limitation to 128th of a whole note,
    valid numerators are:
  */

  int  numeratorDots = mfNumberOfDots (numerator);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurationsDetails ()) {
    std::stringstream ss;

    ss <<
      "--> numeratorDots " << ": " << numeratorDots <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  /*
    valid denominators are powers of 2

    the mfRational representing a dotted duration has to be brought
    to a value less than two, as explained above

    this is done by changing it denominator in the resulting std::string:

     whole notes        std::string
         3/1              \breve.
         3/2              1.
         3/4              2.
         3/8              4.

         7/1              \longa..
         7/2              \breve..
         7/4              1..
         7/8              2..

    since such resulting denominators can be fractions of wholes notes
    as well as multiple thereof,
    we'll be better of using binary logarithms for the computations
  */

  int denominatorNotesDurationLog =
    mfDurationBinaryLogarithm (denominator);

  if (denominatorNotesDurationLog == INT_MIN) {
    std::string result;

    {
      std::stringstream ss;

      ss <<
        1 <<
        "*" <<
        numerator <<
        '/' <<
        denominator;

      result = ss.str ();
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDurationsDetails ()) {
      std::stringstream ss;

      ss <<
        "denominator " << denominator <<
        " is no power of two between 1 and 128" <<
   //     " is no power of 2 between 1 and 1024" <<
        ", whole notes duration " <<
        numerator << '/' << denominator <<
        " cannot be represented as a dotted power of 2" <<
        ", " <<
        result <<
        " will be used";

   //   msrError ( JMI
      msrWarning (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
    //    __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    return result;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurationsDetails ()) {
    std::stringstream ss;

    ss <<
      "--> denominatorNotesDurationLog" << ": " <<
      denominatorNotesDurationLog <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // bring the resulting fraction to be less that two if needed
  if (integralNumberOfWholeNotes) {
    // adapt the duration to avoid even numerators if can be,
    // since dotted durations cannot be recognized otherwise
    // 6/1 thus becomes 3 \breve, hence '\longa.'
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDurationsDetails ()) {
      std::stringstream ss;

      ss <<
        "--> integralNumberOfWholeNotes,"
        " bringing the faction to be less that 2";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    while (numerator % 2 == 0) {
      numerator /= 2;
      denominatorNotesDurationLog -= 1;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceDurationsDetails ()) {
        gLog <<
          "--> numerator" << ": " <<
          numerator <<
          std::endl <<
          "--> denominatorNotesDurationLog " << ": " <<
          denominatorNotesDurationLog <<
          std::endl << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
    } // while

    // update the number of dots
    numeratorDots = mfNumberOfDots (numerator);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurationsDetails ()) {
    std::stringstream ss;

    ss <<
      "--> numerator: " <<
      numerator <<
      std::endl <<
      "--> denominatorNotesDurationLog: " <<
      denominatorNotesDurationLog <<
      std::endl <<
      "--> numeratorDots: " <<
      numeratorDots <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take care of the dots
  int multiplyingFactor = 1;

  if (numeratorDots >= 0 && denominatorNotesDurationLog >= numeratorDots) {
    // take the dots into account
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDurationsDetails ()) {
      std::stringstream ss;

      ss <<
        "--> taking the dots into account";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    denominatorNotesDurationLog -= numeratorDots;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDurationsDetails ()) {
      std::stringstream ss;

      ss <<
        "--> denominatorNotesDurationLog" << ": " <<
        denominatorNotesDurationLog <<
        std::endl <<
        "--> multiplyingFactor " << ": " <<
        multiplyingFactor <<
        std::endl << std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }
  else {
    // set the multiplying factor
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDurationsDetails ()) {
      std::stringstream ss;

      ss <<
        "--> setting the multiplying factor";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // 5/8 becomes 8*5

    multiplyingFactor = numerator;
    numerator = 1;

    /* JMI
    multiplyingFactor = numerator;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDurationsDetails ()) {
      std::stringstream ss;

      ss <<
        "--> denominatorNotesDurationLog" << ": " <<
        denominatorNotesDurationLog <<
        std::endl <<
        "--> multiplyingFactor " << ": " <<
        multiplyingFactor <<
        std::endl << std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    while (multiplyingFactor >= 2) {
      // double duration
      --denominatorNotesDurationLog;

      // adapt multiplying factor
      multiplyingFactor /= 2;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceDurationsDetails ()) {
        gLog <<
          "--> denominatorNotesDurationLog" << ": " <<
          denominatorNotesDurationLog <<
          std::endl <<
          "--> multiplyingFactor " << ": " <<
          multiplyingFactor <<
          std::endl << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
    } // while
    */
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurationsDetails ()) {
    std::stringstream ss;

    ss <<
      "--> numerator " << ": " <<
      numerator <<
      std::endl <<
      "--> numeratorDots " << ": " <<
      numeratorDots <<
      std::endl <<
      "--> denominatorNotesDurationLog" << ": " <<
      denominatorNotesDurationLog <<
      std::endl <<
      "--> multiplyingFactor " << ": " <<
      multiplyingFactor <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the code for the duration
  std::stringstream ss;

  switch (denominatorNotesDurationLog) {
    case -3:
      ss << "\\maxima";
      break;
    case -2:
      ss << "\\longa";
      break;
    case -1:
      ss << "\\breve";
      break;

    default:
      ss << (1 << denominatorNotesDurationLog);
  } // switch

  // append the dots if any
  if (numeratorDots > 0) {
    for (int i = 0; i < numeratorDots; ++i) {
      ss << ".";
    } // for
  }

  if (multiplyingFactor != 1) {
    // append the multiplying factor
    ss <<
      "*" << multiplyingFactor;

    /* JMI
    if (integralNumberOfWholeNotes) {
      ss <<
        "*" << multiplyingFactor;
    }
    else {
      ss <<
        "*" << multiplyingFactor << '/' << 1; // ??? denominator;
    }
    */
  }

  std::string result = ss.str ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurationsDetails ()) {
    std::stringstream ss;

    ss <<
      "--> wholeNotesAndDotsNumberPitchAndOctaveAsString() 2 -------------------------------------" <<
     ", result: \"" << result << "\"" <<
      ", numeratorDots" << ": " << numeratorDots;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // return the result
  dotsNumber = numeratorDots;

  return result;
}

std::string wholeNotesPitchAndOctaveAsString (
  int                  inputLineNumber,
  const mfWholeNotes& wholeNotes)
{
  int dotsNumber; // not used JMI 0.9.70

  return
    wholeNotesAndDotsNumberPitchAndOctaveAsString (
      inputLineNumber,
      wholeNotes,
      dotsNumber);
}

std::string multipleMeasureRestsWholeNotesPitchAndOctaveAsString (
  int                  inputLineNumber, // JMI
  const mfWholeNotes& wholeNotes)
{
  std::stringstream ss;

  mfWholeNotes
    wholeNotesUnit =
      mfWholeNotes (
        1,
        wholeNotes.getDenominator ());

  int numberOfWholeNotes =
    wholeNotes.getNumerator ();

  ss <<
    wholeNotesPitchAndOctaveAsString ( // JMI ??? 0.9.66
      inputLineNumber,
      wholeNotesUnit);

  if (numberOfWholeNotes != 1) {
    ss <<
      "*" << numberOfWholeNotes;
  }

  return ss.str ();
}

//______________________________________________________________________________
void testWholeNotes ()
{
  mfWholeNotes wholeNotes1 ("33/55");
  std::cout << "wholeNotes1: " << wholeNotes1 << std::endl;

  mfWholeNotes wholeNotes2 (-1, 4);
  std::cout << "wholeNotes2: " << wholeNotes2 << std::endl;

  mfWholeNotes wholeNotes3 (3, 8);
  std::cout << "wholeNotes3: " << wholeNotes3 << std::endl;

  mfWholeNotes res1 = wholeNotes2 - wholeNotes3;
  std::cout << "res1: " << res1 << std::endl;
}


//______________________________________________________________________________
mfDurationKind mfDurationKindFromMusicXMLGraphicNoteType (
  int                inputLineNumber,
  const std::string& durationString)
{
  mfDurationKind result = mfDurationKind::kDuration_UNKNOWN_;

  if      (durationString == "maxima") {
    result = mfDurationKind::kDurationMaxima;
  }
  else if (durationString == "long") {
    result = mfDurationKind::kDurationLonga;
  }
  else if (durationString == "breve") {
    result = mfDurationKind::kDurationBreve;
  }
  else if (durationString == "whole") {
    result = mfDurationKind::kDurationWhole;
  }
  else if (durationString == "half") {
    result = mfDurationKind::kDurationHalf;
  }
  else if (durationString == "quarter") {
    result = mfDurationKind::kDurationQuarter;
  }
  else if (durationString == "eighth") {
    result = mfDurationKind::kDurationEighth;
  }
  else if (durationString == "16th") {
    result = mfDurationKind::kDuration16th;
  }
  else if (durationString == "32nd") {
    result = mfDurationKind::kDuration32nd;
  }
  else if (durationString == "64th") {
    result = mfDurationKind::kDuration64th;
  }
  else if (durationString == "128th") {
    result = mfDurationKind::kDuration128th;
  }
  else if (durationString == "256th") {
    result = mfDurationKind::kDuration256th;
  }
  else if (durationString == "512th") {
    result = mfDurationKind::kDuration512th;
  }
  else if (durationString == "1024th") {
    result = mfDurationKind::kDuration1024th;
  }
  else {
    std::stringstream ss;

    ss <<
      "MusicXML durationString \"" << durationString <<
      "\" is unknown";

    msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  return result;
}

EXP mfDurationKind mfDurationKindFromInteger (
  int inputLineNumber,
  int durationInteger)
{
  mfDurationKind result = mfDurationKind::kDuration_UNKNOWN_;

  switch (durationInteger) {
    case 1:
      result = mfDurationKind::kDurationWhole;
      break;
    case 2:
      result = mfDurationKind::kDurationHalf;
      break;
    case 4:
      result = mfDurationKind::kDurationQuarter;
      break;
    case 8:
      result = mfDurationKind::kDurationEighth;
      break;
    case 16:
      result = mfDurationKind::kDuration16th;
      break;
    case 32:
      result = mfDurationKind::kDuration32nd;
      break;
    case 64:
      result = mfDurationKind::kDuration64th;
      break;
    case 128:
      result = mfDurationKind::kDuration128th;
      break;
    case 256:
      result = mfDurationKind::kDuration256th;
      break;
    case 512:
      result = mfDurationKind::kDuration512th;
      break;
    case 1024:
      result = mfDurationKind::kDuration1024th;
      break;
    default:
      ;
  } // switch

  return result;
}

mfDurationKind mfDurationKindFromString (
  int                inputLineNumber,
  const std::string& durationString)
{
  mfDurationKind result = mfDurationKind::kDuration_UNKNOWN_;

  if      (durationString == "maxima") {
    result = mfDurationKind::kDurationMaxima;
  }
  else if (durationString == "long") {
    result = mfDurationKind::kDurationLonga;
  }
  else if (durationString == "breve") {
    result = mfDurationKind::kDurationBreve;
  }
  else if (durationString == "1") {
    result = mfDurationKind::kDurationWhole;
  }
  else if (durationString == "2") {
    result = mfDurationKind::kDurationHalf;
  }
  else if (durationString == "4") {
    result = mfDurationKind::kDurationQuarter;
  }
  else if (durationString == "8") {
    result = mfDurationKind::kDurationEighth;
  }
  else if (durationString == "16") {
    result = mfDurationKind::kDuration16th;
  }
  else if (durationString == "32") {
    result = mfDurationKind::kDuration32nd;
  }
  else if (durationString == "64") {
    result = mfDurationKind::kDuration64th;
  }
  else if (durationString == "128") {
    result = mfDurationKind::kDuration128th;
  }
  else if (durationString == "256") {
    result = mfDurationKind::kDuration256th;
  }
  else if (durationString == "512") {
    result = mfDurationKind::kDuration512th;
  }
  else if (durationString == "1024") {
    result = mfDurationKind::kDuration1024th;
  }
  else {
    std::stringstream ss;

    ss <<
      "MSR durationString \"" << durationString <<
      "\" is unknown";

    msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  return result;
}

EXP mfWholeNotes wholeNotesFromNotesDurationKindAndDotsNumber (
  mfDurationKind notesDurationKind,
  int                  dotsNumber)
{
  // convert duration into whole notes
  mfWholeNotes
    result =
      mfDurationKindAsWholeNotes (
        notesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    std::stringstream ss;

    ss <<
      "=== wholeNotesFromNotesDurationKindAndDotsNumber()" <<
      ", (int) notesDurationKind: " << (int) notesDurationKind <<
      ", result: " << result;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take dots into account if any
  if (dotsNumber > 0) {
    mfWholeNotes
      increment = result * mfRational (1,2);

    int dots = dotsNumber;

    while (dots > 0) {
      result += increment;

      increment *= mfRational (1,2);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    std::stringstream ss;

    ss <<
      "=== wholeNotesFromNotesDurationKindAndDotsNumber()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      ", increment: " << increment;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

      --dots;
    } // while
  }

  return result;
}

mfWholeNotes mfDurationKindAsWholeNotes (mfDurationKind notesDurationKind)
{
  mfWholeNotes result;

  switch (notesDurationKind) {
    case mfDurationKind::kDuration_UNKNOWN_:
      result = K_WHOLE_NOTES_ZERO;
      break;

    case mfDurationKind::kDuration1024th:
      result = mfWholeNotes (1, 1024);
      break;
    case mfDurationKind::kDuration512th:
      result = mfWholeNotes (1, 512);
      break;
    case mfDurationKind::kDuration256th:
      result = mfWholeNotes (1, 256);
      break;
    case mfDurationKind::kDuration128th:
      result = mfWholeNotes (1, 128);
      break;
    case mfDurationKind::kDuration64th:
      result = mfWholeNotes (1, 64);
      break;
    case mfDurationKind::kDuration32nd:
      result = mfWholeNotes (1, 32);
      break;
    case mfDurationKind::kDuration16th:
      result = mfWholeNotes (1, 16);
      break;
    case mfDurationKind::kDurationEighth:
      result = mfWholeNotes (1, 8);
      break;
    case mfDurationKind::kDurationQuarter:
      result = mfWholeNotes (1, 4);
      break;
    case mfDurationKind::kDurationHalf:
      result = mfWholeNotes (1, 2);
      break;
    case mfDurationKind::kDurationWhole:
      result = mfWholeNotes (1, 1);
      break;
    case mfDurationKind::kDurationBreve:
      result = mfWholeNotes (2, 1);
      break;
    case mfDurationKind::kDurationLonga:
      result = mfWholeNotes (4, 1);
      break;
    case mfDurationKind::kDurationMaxima:
      result = mfWholeNotes (8, 1);
      break;
  } // switch

  return result;
}

mfDurationKind wholeNotesAsNotesDurationKind (mfWholeNotes wholeNotes)
{
  mfDurationKind result = mfDurationKind::kDuration_UNKNOWN_;

  if (wholeNotes.getNumerator () == 1) {
    switch (wholeNotes.getDenominator ()) {
      case 1:
        result = mfDurationKind::kDurationWhole;
        break;
      case 2:
        result = mfDurationKind::kDurationHalf;
        break;
      case 4:
        result = mfDurationKind::kDurationQuarter;
        break;
      case 8:
        result = mfDurationKind::kDurationEighth;
        break;
      case 16:
        result = mfDurationKind::kDuration16th;
        break;
      case 32:
        result = mfDurationKind::kDuration32nd;
        break;
      case 64:
        result = mfDurationKind::kDuration64th;
        break;
      case 128:
        result = mfDurationKind::kDuration128th;
        break;
      case 256:
        result = mfDurationKind::kDuration256th;
        break;
      case 512:
        result = mfDurationKind::kDuration512th;
        break;
      case 1024:
        result = mfDurationKind::kDuration1024th;
        break;
      default:
        ;
    } // switch
  }

  else if (wholeNotes.getDenominator () == 1) {
    switch (wholeNotes.getNumerator ()) {
      case 2:
        result = mfDurationKind::kDurationBreve;
        break;
      case 4:
        result = mfDurationKind::kDurationLonga;
        break;
      case 8:
        result = mfDurationKind::kDurationMaxima;
        break;
      default:
        ;
    } // switch
  }

  return result;
}

std::string mfDurationKindAsMusicXMLType (mfDurationKind notesDurationKind)
{
  std::string result;

  switch (notesDurationKind) {
    case mfDurationKind::kDuration_UNKNOWN_:
      result = "noNotesDuration";
      break;

    case mfDurationKind::kDuration1024th:
      result = "1024th";
      break;
    case mfDurationKind::kDuration512th:
      result = "512th";
      break;
    case mfDurationKind::kDuration256th:
      result = "256th";
      break;
    case mfDurationKind::kDuration128th:
      result = "128th";
      break;
    case mfDurationKind::kDuration64th:
      result = "64th";
      break;
    case mfDurationKind::kDuration32nd:
      result = "32nd";
      break;
    case mfDurationKind::kDuration16th:
      result = "16th";
      break;
    case mfDurationKind::kDurationEighth:
      result = "eighth";
      break;
    case mfDurationKind::kDurationQuarter:
      result = "quarter";
      break;
    case mfDurationKind::kDurationHalf:
      result = "half";
      break;
    case mfDurationKind::kDurationWhole:
      result = "whole";
      break;
    case mfDurationKind::kDurationBreve:
      result = "breve";
      break;
    case mfDurationKind::kDurationLonga:
      result = "long";
      break;
    case mfDurationKind::kDurationMaxima:
      result = "maxima";
      break;
  } // switch

  return result;
}

std::string mfDurationKindAsString_INTERNAL (
  mfDurationKind notesDurationKind) // JMI don't keep ??? 0.9.67
{
  std::string result;

  switch (notesDurationKind) {
    case mfDurationKind::kDuration_UNKNOWN_:
      result = "kDuration_UNKNOWN_";
      break;

    case mfDurationKind::kDuration1024th:
      result = "kDuration1024th";
      break;
    case mfDurationKind::kDuration512th:
      result = "kDuration512th";
      break;
    case mfDurationKind::kDuration256th:
      result = "kDuration256th";
      break;
    case mfDurationKind::kDuration128th:
      result = "kDuration128th";
      break;
    case mfDurationKind::kDuration64th:
      result = "kDuration64th";
      break;
    case mfDurationKind::kDuration32nd:
      result = "kDuration32nd";
      break;
    case mfDurationKind::kDuration16th:
      result = "kDuration16th";
      break;
    case mfDurationKind::kDurationEighth:
      result = "kDurationEighth";
      break;
    case mfDurationKind::kDurationQuarter:
      result = "kDurationQuarter";
      break;
    case mfDurationKind::kDurationHalf:
      result = "kDurationHalf";
      break;
    case mfDurationKind::kDurationWhole:
      result = "kDurationWhole";
      break;
    case mfDurationKind::kDurationBreve:
      result = "kDurationBreve";
      break;
    case mfDurationKind::kDurationLonga:
      result = "kDurationLonga";
      break;
    case mfDurationKind::kDurationMaxima:
      result = "kDurationMaxima";
      break;
  } // switch

  return result;
}

std::string mfDurationKindAsString (
  mfDurationKind notesDurationKind)
{
  std::string result;

  switch (notesDurationKind) {
    case mfDurationKind::kDuration_UNKNOWN_:
      result = "kDuration_UNKNOWN_";
      break;

    case mfDurationKind::kDuration1024th:
      result = "1024";
      break;
    case mfDurationKind::kDuration512th:
      result = "512";
      break;
    case mfDurationKind::kDuration256th:
      result = "256";
      break;
    case mfDurationKind::kDuration128th:
      result = "128";
      break;
    case mfDurationKind::kDuration64th:
      result = "64";
      break;
    case mfDurationKind::kDuration32nd:
      result = "32";
      break;
    case mfDurationKind::kDuration16th:
      result = "16";
      break;
    case mfDurationKind::kDurationEighth:
      result = "8";
      break;
    case mfDurationKind::kDurationQuarter:
      result = "4";
      break;
    case mfDurationKind::kDurationHalf:
      result = "2";
      break;
    case mfDurationKind::kDurationWhole:
      result = "1";
      break;
    case mfDurationKind::kDurationBreve:
      result = "Breve";
      break;
    case mfDurationKind::kDurationLonga:
      result = "Longa";
      break;
    case mfDurationKind::kDurationMaxima:
      result = "Maxima";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mfDurationKind& elt)
{
  os << mfDurationKindAsString (elt);
  return os;
}

//______________________________________________________________________________
EXP void checkNoteDurationKindAndWholeNotesDurationConsistency (
  int                  inputLineNumber,
  mfDurationKind notesDurationKind,
  mfWholeNotes        wholeNotesDuration)
{
  Bool consistency (false);

  switch (notesDurationKind) {
    case mfDurationKind::kDuration_UNKNOWN_:
      consistency = wholeNotesDuration == mfWholeNotes (0, 1024);
      break;

    case mfDurationKind::kDuration1024th:
      consistency = wholeNotesDuration == mfWholeNotes (1, 1024);
      break;
    case mfDurationKind::kDuration512th:
      consistency = wholeNotesDuration == mfWholeNotes (1, 512);
      break;
    case mfDurationKind::kDuration256th:
      consistency = wholeNotesDuration == mfWholeNotes (1, 256);
      break;
    case mfDurationKind::kDuration128th:
      consistency = wholeNotesDuration == mfWholeNotes (1, 128);
      break;
    case mfDurationKind::kDuration64th:
      consistency = wholeNotesDuration == mfWholeNotes (1, 64);
      break;
    case mfDurationKind::kDuration32nd:
      consistency = wholeNotesDuration == mfWholeNotes (1, 32);
      break;
    case mfDurationKind::kDuration16th:
      consistency = wholeNotesDuration == mfWholeNotes (1, 16);
      break;
    case mfDurationKind::kDurationEighth:
      consistency = wholeNotesDuration == mfWholeNotes (1, 8);
      break;
    case mfDurationKind::kDurationQuarter:
      consistency = wholeNotesDuration == mfWholeNotes (1, 4);
      break;
    case mfDurationKind::kDurationHalf:
      consistency = wholeNotesDuration == mfWholeNotes (1, 2);
      break;
    case mfDurationKind::kDurationWhole:
      consistency = wholeNotesDuration == mfWholeNotes (1, 1);
      break;
    case mfDurationKind::kDurationBreve:
      consistency = wholeNotesDuration == mfWholeNotes (2, 1);
      break;
    case mfDurationKind::kDurationLonga:
      consistency = wholeNotesDuration == mfWholeNotes (4, 1);
      break;
    case mfDurationKind::kDurationMaxima:
      consistency = wholeNotesDuration == mfWholeNotes (8, 1);
      break;
  } // switch

  if (! consistency) {
    std::stringstream ss;

    ss <<
      "notesDurationKind " <<
      notesDurationKind <<
      " and wholeNotesDuration " <<
      wholeNotesDuration <<
      " are inconsistent";

// //     msrError ( // JMI VITAL
//     msrWarning (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
// //       __FILE__, __LINE__,
//       ss.str ());
  }
}

// ______________________________________________________________________________
msrNotesDuration::msrNotesDuration ()
{
  fNotesDurationKind = mfDurationKind::kDuration_UNKNOWN_;
  fDotsNumber = 0;
}

msrNotesDuration::msrNotesDuration (
  mfDurationKind notesDurationKind,
  int                  dotsNumber)
{
  fNotesDurationKind = notesDurationKind;
  fDotsNumber = dotsNumber;
}

msrNotesDuration::~msrNotesDuration ()
{}

mfWholeNotes msrNotesDuration::dottedNotesDurationAsWholeNotes (
  int inputLineNumber) const
{
  // convert duration into whole notes
  mfWholeNotes
    result =
      mfDurationKindAsWholeNotes (
        fNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    print (gLog);

    std::stringstream ss;

    ss <<
      "=== dottedNotesDurationAsWholeNotes()" <<
      ", (int) fNotesDurationKind: " << (int) fNotesDurationKind <<
      ", result: " << result;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take dots into account if any
  if (fDotsNumber > 0) {
    mfWholeNotes
      increment = result * mfRational (1,2);

    int dots = fDotsNumber;

    while (dots > 0) {
      result += increment;

      increment *= mfRational (1,2);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    std::stringstream ss;

    ss <<
      "=== dottedNotesDurationAsWholeNotes()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      ", increment: " << increment;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

      --dots;
    } // while
  }

  return result;
}

mfWholeNotes msrNotesDuration::dottedNotesDurationAsWholeNotes_FOR_TEMPO (
  // used in lpsrEnumTypes, dottedNotesDurationAsLilypondStringWithoutBackSlash(),
  // called in lpsr2lilypondTranslator.cpp, visitStart (S_msrTempo& elt)
  // JMI BUGGY, NEVER TESTED TEMP??? 0.9.67
  int inputLineNumber) const
{
  // convert duration into whole notes
  mfWholeNotes
    result =
      mfDurationKindAsWholeNotes (
        fNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    std::stringstream ss;

    ss <<
      "=== dottedNotesDurationAsWholeNotes_FOR_TEMPO()" <<
      ", result: " << result <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take dots into account if any
  if (fDotsNumber > 0) {
    int dots = fDotsNumber;

    while (dots > 0) {
      result *=
        mfRational (3, 2);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    std::stringstream ss;

    ss <<
      "=== dottedNotesDurationAsWholeNotes_FOR_TEMPO()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

      --dots;
    } // while
  }

  return result;
}

std::string msrNotesDuration::asString () const
{
  std::stringstream ss;

  ss <<
     fNotesDurationKind;

  for (int i = 1; i <= fDotsNumber; ++i) {
    ss << ".";
  } // for

  return ss.str ();
}

void msrNotesDuration::print (std::ostream& os) const
{
  constexpr int fieldWidth = 11;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNotesDurationKind" << ": " <<
    fNotesDurationKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "dotsNumber" << ": " << fDotsNumber <<
    std::endl;
};

std::ostream& operator << (std::ostream& os, const msrNotesDuration& elt)
{
  os << elt.asString ();
  return os;
}

//_______________________________________________________________________________
int mfNumberOfDots (int n)
{
  int  result = INT_MIN;

   switch (n) {
    case 1:
      result = 0;
      break;
    case 3:
      result = 1;
      break;
    case 7:
      result = 2;
      break;
    case 15:
      result = 3;
      break;
    case 31:
      result = 4;
      break;
    case 63:
      result = 5;
      break;
    case 127:
      result = 6;
      break;
    case 255:
      result = 7;
      break;
    case 511:
      result = 8;
      break;
    case 1023:
      result = 9;
      break;

    default:
      ;
    } // switch

  return result;
}

int mfDurationBinaryLogarithm (int duration)
{
  int result = INT_MIN;

/*
with MusicXML's limitation to 1024th of a whole note,
valid denominators binary logarithms, i.e. their exponent, are:
*/

  switch (duration) {
    case 1:
      result = 0;
      break;
    case 2:
      result = 1;
      break;
    case 4:
      result = 2;
      break;
    case 8:
      result = 3;
      break;
    case 16:
      result = 4;
      break;
    case 32:
      result = 5;
      break;
    case 64:
      result = 6;
      break;
    case 128:
      result = 7;
      break;
    case 256:
      result = 8;
      break;
    case 512:
      result = 9;
      break;
    case 1024:
      result = 10;
      break;

    default:
      ;
  } // switch

  return result;
}

//_______________________________________________________________________________
// dotted durations
//______________________________________________________________________________
mfDottedNotesDuration::mfDottedNotesDuration ()
{
  fNotesDurationKind = mfDurationKind::kDuration_UNKNOWN_;
  fDotsNumber = 0;
}

mfDottedNotesDuration::mfDottedNotesDuration (
  mfDurationKind notesDurationKind,
  int                  dotsNumber)
{
  fNotesDurationKind = notesDurationKind;
  fDotsNumber = dotsNumber;
}

mfDottedNotesDuration::~mfDottedNotesDuration ()
{}

mfWholeNotes mfDottedNotesDuration::dottedNotesDurationAsWholeNotes (
  int inputLineNumber) const
{
  // convert duration into whole notes
  mfWholeNotes
    result =
      mfDurationKindAsWholeNotes (
        fNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    print (gLog);

    std::stringstream ss;

    ss <<
      "=== dottedNotesDurationAsWholeNotes()" <<
      ", (int) fNotesDurationKind: " << (int) fNotesDurationKind <<
      ", result: " << result;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take dots into account if any
  if (fDotsNumber > 0) {
    mfWholeNotes
      increment = result * mfRational (1,2);

    int dots = fDotsNumber;

    while (dots > 0) {
      result += increment;

      increment *= mfRational (1,2);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    std::stringstream ss;

    ss <<
      "=== dottedNotesDurationAsWholeNotes()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      ", increment: " << increment;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

      --dots;
    } // while
  }

  return result;
}

mfWholeNotes mfDottedNotesDuration::dottedNotesDurationAsWholeNotes_FOR_TEMPO (
  // used in lpsrEnumTypes, dottedNotesDurationAsLilypondStringWithoutBackSlash(),
  // called in lpsr2lilypondTranslator.cpp, visitStart (S_msrTempo& elt)
  // JMI BUGGY, NEVER TESTED TEMP???
  int inputLineNumber) const
{
  // convert duration into whole notes
  mfWholeNotes
    result =
      mfDurationKindAsWholeNotes (
        fNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    std::stringstream ss;

    ss <<
      "=== dottedNotesDurationAsWholeNotes_FOR_TEMPO()" <<
      ", result: " << result <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take dots into account if any
  if (fDotsNumber > 0) {
    int dots = fDotsNumber;

    while (dots > 0) {
      result *=
        mfRational (3, 2);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDurations ()) {
    std::stringstream ss;

    ss <<
      "=== dottedNotesDurationAsWholeNotes_FOR_TEMPO()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

      --dots;
    } // while
  }

  return result;
}

std::string mfDottedNotesDuration::asString () const
{
  std::stringstream ss;

  ss <<
     fNotesDurationKind;

  for (int i = 1; i <= fDotsNumber; ++i) {
    ss << ".";
  } // for

  return ss.str ();
}

void mfDottedNotesDuration::print (std::ostream& os) const
{
  constexpr int fieldWidth = 11;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNotesDurationKind" << ": " <<
    fNotesDurationKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fDotsNumber" << ": " << fDotsNumber <<
    std::endl;
};

std::ostream& operator << (std::ostream& os, const mfDottedNotesDuration& elt)
{
  os << elt.asString ();
  return os;
}


}

