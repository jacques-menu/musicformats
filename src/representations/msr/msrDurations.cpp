/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw()), set::precision(), ...

#include "mfStaticSettings.h"

#include "mfServices.h"

#include "msrDurations.h"

#include "msrWae.h"


namespace MusicFormats
{

// durations
//______________________________________________________________________________
msrDurationKind msrDurationKindFromMusicXMLString (
  int           inputLineNumber,
  const std::string& durationString)
{
  msrDurationKind result = msrDurationKind::kDuration_UNKNOWN;

  if      (durationString == "maxima") {
    result = msrDurationKind::kDurationMaxima;
  }
  else if (durationString == "long") {
    result = msrDurationKind::kDurationLonga;
  }
  else if (durationString == "breve") {
    result = msrDurationKind::kDurationBreve;
  }
  else if (durationString == "whole") {
    result = msrDurationKind::kDurationWhole;
  }
  else if (durationString == "half") {
    result = msrDurationKind::kDurationHalf;
  }
  else if (durationString == "quarter") {
    result = msrDurationKind::kDurationQuarter;
  }
  else if (durationString == "eighth") {
    result = msrDurationKind::kDurationEighth;
  }
  else if (durationString == "16th") {
    result = msrDurationKind::kDuration16th;
  }
  else if (durationString == "32nd") {
    result = msrDurationKind::kDuration32nd;
  }
  else if (durationString == "64th") {
    result = msrDurationKind::kDuration64th;
  }
  else if (durationString == "128th") {
    result = msrDurationKind::kDuration128th;
  }
  else if (durationString == "256th") {
    result = msrDurationKind::kDuration256th;
  }
  else if (durationString == "512th") {
    result = msrDurationKind::kDuration512th;
  }
  else if (durationString == "1024th") {
    result = msrDurationKind::kDuration1024th;
  }
  else {
    std::stringstream s;

    s <<
      "MusicXML durationString \"" << durationString <<
      "\" is unknown";

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

EXP msrDurationKind msrDurationKindFromInteger (
  int inputLineNumber,
  int durationInteger)
{
  msrDurationKind result = msrDurationKind::kDuration_UNKNOWN;

  switch (durationInteger) {
    case 1:
      result = msrDurationKind::kDurationWhole;
      break;
    case 2:
      result = msrDurationKind::kDurationHalf;
      break;
    case 4:
      result = msrDurationKind::kDurationQuarter;
      break;
    case 8:
      result = msrDurationKind::kDurationEighth;
      break;
    case 16:
      result = msrDurationKind::kDuration16th;
      break;
    case 32:
      result = msrDurationKind::kDuration32nd;
      break;
    case 64:
      result = msrDurationKind::kDuration64th;
      break;
    case 128:
      result = msrDurationKind::kDuration128th;
      break;
    case 256:
      result = msrDurationKind::kDuration256th;
      break;
    case 512:
      result = msrDurationKind::kDuration512th;
      break;
    case 1024:
      result = msrDurationKind::kDuration1024th;
      break;
    default:
      ;
  } // switch

  return result;
}

msrDurationKind msrDurationKindFromString (
  int           inputLineNumber,
  const std::string& durationString)
{
  msrDurationKind result = msrDurationKind::kDuration_UNKNOWN;

  if      (durationString == "maxima") {
    result = msrDurationKind::kDurationMaxima;
  }
  else if (durationString == "long") {
    result = msrDurationKind::kDurationLonga;
  }
  else if (durationString == "breve") {
    result = msrDurationKind::kDurationBreve;
  }
  else if (durationString == "1") {
    result = msrDurationKind::kDurationWhole;
  }
  else if (durationString == "2") {
    result = msrDurationKind::kDurationHalf;
  }
  else if (durationString == "4") {
    result = msrDurationKind::kDurationQuarter;
  }
  else if (durationString == "8") {
    result = msrDurationKind::kDurationEighth;
  }
  else if (durationString == "16") {
    result = msrDurationKind::kDuration16th;
  }
  else if (durationString == "32") {
    result = msrDurationKind::kDuration32nd;
  }
  else if (durationString == "64") {
    result = msrDurationKind::kDuration64th;
  }
  else if (durationString == "128") {
    result = msrDurationKind::kDuration128th;
  }
  else if (durationString == "256") {
    result = msrDurationKind::kDuration256th;
  }
  else if (durationString == "512") {
    result = msrDurationKind::kDuration512th;
  }
  else if (durationString == "1024") {
    result = msrDurationKind::kDuration1024th;
  }
  else {
    std::stringstream s;

    s <<
      "MSR durationString \"" << durationString <<
      "\" is unknown";

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

EXP Rational rationalFromDurationKindAndDotsNumber (
  msrDurationKind durationKind,
  int             dotsNumber)
{
  // convert duration into whole notes
  Rational
    result =
      msrDurationKindAsWholeNotes (
        durationKind);

#ifdef MF_TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "=== rationalFromDurationKindAndDotsNumber()" <<
      ", (int) durationKind: " << (int) durationKind <<
      ", result: " << result <<
      std::endl;
  }
#endif

  // take dots into account if any
  if (dotsNumber > 0) {
    Rational
      increment = result * Rational (1,2);

    int dots = dotsNumber;

    while (dots > 0) {
      result += increment;

      increment *= Rational (1,2);

#ifdef MF_TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "=== rationalFromDurationKindAndDotsNumber()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      ", increment: " << increment <<
      std::endl;
  }
#endif

      --dots;
    } // while
  }

  return result;
}

Rational msrDurationKindAsWholeNotes (msrDurationKind durationKind)
{
  Rational result;

  switch (durationKind) {
    case msrDurationKind::kDuration_UNKNOWN:
      result = Rational (0, 1);
      break;

    case msrDurationKind::kDuration1024th:
      result = Rational (1, 1024);
      break;
    case msrDurationKind::kDuration512th:
      result = Rational (1, 512);
      break;
    case msrDurationKind::kDuration256th:
      result = Rational (1, 256);
      break;
    case msrDurationKind::kDuration128th:
      result = Rational (1, 128);
      break;
    case msrDurationKind::kDuration64th:
      result = Rational (1, 64);
      break;
    case msrDurationKind::kDuration32nd:
      result = Rational (1, 32);
      break;
    case msrDurationKind::kDuration16th:
      result = Rational (1, 16);
      break;
    case msrDurationKind::kDurationEighth:
      result = Rational (1, 8);
      break;
    case msrDurationKind::kDurationQuarter:
      result = Rational (1, 4);
      break;
    case msrDurationKind::kDurationHalf:
      result = Rational (1, 2);
      break;
    case msrDurationKind::kDurationWhole:
      result = Rational (1, 1);
      break;
    case msrDurationKind::kDurationBreve:
      result = Rational (2, 1);
      break;
    case msrDurationKind::kDurationLonga:
      result = Rational (4, 1);
      break;
    case msrDurationKind::kDurationMaxima:
      result = Rational (8, 1);
      break;
  } // switch

  return result;
}

msrDurationKind wholeNotesAsDurationKind (Rational wholeNotes)
{
  msrDurationKind result = msrDurationKind::kDuration_UNKNOWN;

  if (wholeNotes.getNumerator () == 1) {
    switch (wholeNotes.getDenominator ()) {
      case 1:
        result = msrDurationKind::kDurationWhole;
        break;
      case 2:
        result = msrDurationKind::kDurationHalf;
        break;
      case 4:
        result = msrDurationKind::kDurationQuarter;
        break;
      case 8:
        result = msrDurationKind::kDurationEighth;
        break;
      case 16:
        result = msrDurationKind::kDuration16th;
        break;
      case 32:
        result = msrDurationKind::kDuration32nd;
        break;
      case 64:
        result = msrDurationKind::kDuration64th;
        break;
      case 128:
        result = msrDurationKind::kDuration128th;
        break;
      case 256:
        result = msrDurationKind::kDuration256th;
        break;
      case 512:
        result = msrDurationKind::kDuration512th;
        break;
      case 1024:
        result = msrDurationKind::kDuration1024th;
        break;
      default:
        ;
    } // switch
  }

  else if (wholeNotes.getDenominator () == 1) {
    switch (wholeNotes.getNumerator ()) {
      case 2:
        result = msrDurationKind::kDurationBreve;
        break;
      case 4:
        result = msrDurationKind::kDurationLonga;
        break;
      case 8:
        result = msrDurationKind::kDurationMaxima;
        break;
      default:
        ;
    } // switch
  }

  return result;
}

std::string msrDurationKindAsMusicXMLType (msrDurationKind durationKind)
{
  std::string result;

  switch (durationKind) {
    case msrDurationKind::kDuration_UNKNOWN:
      result = "noDuration";
      break;

    case msrDurationKind::kDuration1024th:
      result = "1024th";
      break;
    case msrDurationKind::kDuration512th:
      result = "512th";
      break;
    case msrDurationKind::kDuration256th:
      result = "256th";
      break;
    case msrDurationKind::kDuration128th:
      result = "128th";
      break;
    case msrDurationKind::kDuration64th:
      result = "64th";
      break;
    case msrDurationKind::kDuration32nd:
      result = "32nd";
      break;
    case msrDurationKind::kDuration16th:
      result = "16th";
      break;
    case msrDurationKind::kDurationEighth:
      result = "eighth";
      break;
    case msrDurationKind::kDurationQuarter:
      result = "quarter";
      break;
    case msrDurationKind::kDurationHalf:
      result = "half";
      break;
    case msrDurationKind::kDurationWhole:
      result = "whole";
      break;
    case msrDurationKind::kDurationBreve:
      result = "breve";
      break;
    case msrDurationKind::kDurationLonga:
      result = "long";
      break;
    case msrDurationKind::kDurationMaxima:
      result = "maxima";
      break;
  } // switch

  return result;
}

std::string msrDurationKindAsString_INTERNAL (msrDurationKind durationKind) // JMI don't keep ???
{
  std::string result;

  switch (durationKind) {
    case msrDurationKind::kDuration_UNKNOWN:
      result = "kDuration_UNKNOWN";
      break;

    case msrDurationKind::kDuration1024th:
      result = "kDuration1024th";
      break;
    case msrDurationKind::kDuration512th:
      result = "kDuration512th";
      break;
    case msrDurationKind::kDuration256th:
      result = "kDuration256th";
      break;
    case msrDurationKind::kDuration128th:
      result = "kDuration128th";
      break;
    case msrDurationKind::kDuration64th:
      result = "kDuration64th";
      break;
    case msrDurationKind::kDuration32nd:
      result = "kDuration32nd";
      break;
    case msrDurationKind::kDuration16th:
      result = "kDuration16th";
      break;
    case msrDurationKind::kDurationEighth:
      result = "kDurationEighth";
      break;
    case msrDurationKind::kDurationQuarter:
      result = "kDurationQuarter";
      break;
    case msrDurationKind::kDurationHalf:
      result = "kDurationHalf";
      break;
    case msrDurationKind::kDurationWhole:
      result = "kDurationWhole";
      break;
    case msrDurationKind::kDurationBreve:
      result = "kDurationBreve";
      break;
    case msrDurationKind::kDurationLonga:
      result = "kDurationLonga";
      break;
    case msrDurationKind::kDurationMaxima:
      result = "kDurationMaxima";
      break;
  } // switch

  return result;
}

std::string msrDurationKindAsString (msrDurationKind durationKind)
{
  std::string result;

  switch (durationKind) {
    case msrDurationKind::kDuration_UNKNOWN:
      result = "???NoDuration???";
      break;

    case msrDurationKind::kDuration1024th:
      result = "1024";
      break;
    case msrDurationKind::kDuration512th:
      result = "512";
      break;
    case msrDurationKind::kDuration256th:
      result = "256";
      break;
    case msrDurationKind::kDuration128th:
      result = "128";
      break;
    case msrDurationKind::kDuration64th:
      result = "64";
      break;
    case msrDurationKind::kDuration32nd:
      result = "32nd";
      break;
    case msrDurationKind::kDuration16th:
      result = "16";
      break;
    case msrDurationKind::kDurationEighth:
      result = "8";
      break;
    case msrDurationKind::kDurationQuarter:
      result = "4";
      break;
    case msrDurationKind::kDurationHalf:
      result = "2";
      break;
    case msrDurationKind::kDurationWhole:
      result = "1";
      break;
    case msrDurationKind::kDurationBreve:
      result = "Breve";
      break;
    case msrDurationKind::kDurationLonga:
      result = "Longa";
      break;
    case msrDurationKind::kDurationMaxima:
      result = "Maxima";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrDurationKind& elt)
{
  os << msrDurationKindAsString (elt);
  return os;
}

// duration
//______________________________________________________________________________
msrDuration::msrDuration ()
{
  fDurationKind = msrDurationKind::kDuration_UNKNOWN;
  fDotsNumber   = 0;
}

msrDuration::msrDuration (
  msrDurationKind durationKind,
  int             dotsNumber)
{
  fDurationKind = durationKind;
  fDotsNumber   = dotsNumber;
}

msrDuration::~msrDuration ()
{}

Rational msrDuration::dottedDurationAsWholeNotes (
  int inputLineNumber) const
{
  // convert duration into whole notes
  Rational
    result =
      msrDurationKindAsWholeNotes (
        fDurationKind);

#ifdef MF_TRACING_IS_ENABLED
  if (false) { // JMI
    print (gLogStream);

    gLogStream <<
      "=== dottedDurationAsWholeNotes()" <<
      ", (int) fDurationKind: " << (int) fDurationKind <<
      ", result: " << result <<
      std::endl;
  }
#endif

  // take dots into account if any
  if (fDotsNumber > 0) {
    Rational
      increment = result * Rational (1,2);

    int dots = fDotsNumber;

    while (dots > 0) {
      result += increment;

      increment *= Rational (1,2);

#ifdef MF_TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "=== dottedDurationAsWholeNotes()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      ", increment: " << increment <<
      std::endl;
  }
#endif

      --dots;
    } // while
  }

  return result;
}

Rational msrDuration::dottedDurationAsWholeNotes_FOR_TEMPO (
  // used in lpsrEnumTypes, dottedDurationAsLilypondStringWithoutBackSlash(),
  // called in lpsr2lilypondTranslator.cpp, visitStart (S_msrTempo& elt)
  // JMI BUGGY, NEVER TESTED TEMP???
  int inputLineNumber) const
{
  // convert duration into whole notes
  Rational
    result =
      msrDurationKindAsWholeNotes (
        fDurationKind);

#ifdef MF_TRACING_IS_ENABLED
  if (false) {
    gLogStream <<
      "=== dottedDurationAsWholeNotes_FOR_TEMPO()" <<
      ", result: " << result <<
      "\"" <<
      std::endl;
  }
#endif

  // take dots into account if any
  if (fDotsNumber > 0) {
    int dots = fDotsNumber;

    while (dots > 0) {
      result *=
        Rational (3, 2);

#ifdef MF_TRACING_IS_ENABLED
  if (false) {
    gLogStream <<
      "=== dottedDurationAsWholeNotes_FOR_TEMPO()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      "\"" <<
      std::endl;
  }
#endif

      --dots;
    } // while
  }

  return result;
}

std::string msrDuration::asString () const
{
  std::stringstream s;

  s <<
     msrDurationKindAsString (fDurationKind);

  for (int i = 1; i <= fDotsNumber; ++i) {
    s << ".";
  } // for

  return s.str ();
}

void msrDuration::print (std::ostream& os) const
{
  const int fieldWidth = 11;

  os << std::left <<
    std::setw (fieldWidth) <<
    "durationKind" << ": " <<
    msrDurationKindAsString (fDurationKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "dotsNumber" << ": " << fDotsNumber <<
    std::endl;
};

std::ostream& operator << (std::ostream& os, const msrDuration& elt)
{
  os << elt.asString ();
  return os;
}

//_______________________________________________________________________________
int msrDurationBinaryLogarithm (int duration)
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
int msrNumberOfDots (int n)
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

// dotted durations
//______________________________________________________________________________
msrDottedDuration::msrDottedDuration ()
{
  fDurationKind = msrDurationKind::kDuration_UNKNOWN;
  fDotsNumber   = 0;
}

msrDottedDuration::msrDottedDuration (
  msrDurationKind durationKind,
  int             dotsNumber)
{
  fDurationKind = durationKind;
  fDotsNumber   = dotsNumber;
}

msrDottedDuration::~msrDottedDuration ()
{}

Rational msrDottedDuration::dottedDurationAsWholeNotes (
  int inputLineNumber) const
{
  // convert duration into whole notes
  Rational
    result =
      msrDurationKindAsWholeNotes (
        fDurationKind);

#ifdef MF_TRACING_IS_ENABLED
  if (false) { // JMI
    print (gLogStream);

    gLogStream <<
      "=== dottedDurationAsWholeNotes()" <<
      ", (int) fDurationKind: " << (int) fDurationKind <<
      ", result: " << result <<
      std::endl;
  }
#endif

  // take dots into account if any
  if (fDotsNumber > 0) {
    Rational
      increment = result * Rational (1,2);

    int dots = fDotsNumber;

    while (dots > 0) {
      result += increment;

      increment *= Rational (1,2);

#ifdef MF_TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "=== dottedDurationAsWholeNotes()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      ", increment: " << increment <<
      std::endl;
  }
#endif

      --dots;
    } // while
  }

  return result;
}

Rational msrDottedDuration::dottedDurationAsWholeNotes_FOR_TEMPO (
  // used in lpsrEnumTypes, dottedDurationAsLilypondStringWithoutBackSlash(),
  // called in lpsr2lilypondTranslator.cpp, visitStart (S_msrTempo& elt)
  // JMI BUGGY, NEVER TESTED TEMP???
  int inputLineNumber) const
{
  // convert duration into whole notes
  Rational
    result =
      msrDurationKindAsWholeNotes (
        fDurationKind);

#ifdef MF_TRACING_IS_ENABLED
  if (false) {
    gLogStream <<
      "=== dottedDurationAsWholeNotes_FOR_TEMPO()" <<
      ", result: " << result <<
      "\"" <<
      std::endl;
  }
#endif

  // take dots into account if any
  if (fDotsNumber > 0) {
    int dots = fDotsNumber;

    while (dots > 0) {
      result *=
        Rational (3, 2);

#ifdef MF_TRACING_IS_ENABLED
  if (false) {
    gLogStream <<
      "=== dottedDurationAsWholeNotes_FOR_TEMPO()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      "\"" <<
      std::endl;
  }
#endif

      --dots;
    } // while
  }

  return result;
}

std::string msrDottedDuration::asString () const
{
  std::stringstream s;

  s <<
     msrDurationKindAsString (fDurationKind);

  for (int i = 1; i <= fDotsNumber; ++i) {
    s << ".";
  } // for

  return s.str ();
}

void msrDottedDuration::print (std::ostream& os) const
{
  const int fieldWidth = 11;

  os << std::left <<
    std::setw (fieldWidth) <<
    "durationKind" << ": " <<
    msrDurationKindAsString (fDurationKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "dotsNumber" << ": " << fDotsNumber <<
    std::endl;
};

std::ostream& operator << (std::ostream& os, const msrDottedDuration& elt)
{
  os << elt.asString ();
  return os;
}

//_______________________________________________________________________________
std::string wholeNotesAsMsrString (
  int             inputLineNumber,
  const Rational& wholeNotes,
  int&            dotsNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> wholeNotesAsMsrString() 1 -------------------------------------" <<
      ", wholeNotes: " << wholeNotes <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numerator:   " << numerator <<
      std::endl <<
      "--> denominator: " << denominator <<
      std::endl << std::endl;
  }
#endif

  if (numerator == 0) { // JMI TEMP
    dotsNumber = 0;
    return "zero";
  }
  else if (numerator < 0) {
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
    std::stringstream s;

    s <<
      "numerator is not positive in wholeNotesAsMsrString()" <<
      ", wholeNotes: " << wholeNotes;

 //   msrError ( JMI
    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
  //    __FILE__, __LINE__,
      s.str ());

    return "???";
#endif
  }

  Bool
    integralNumberOfWholeNotes = denominator == 1;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> integralNumberOfWholeNotes: " <<
      integralNumberOfWholeNotes <<
      std::endl << std::endl;
  }
#endif

  /*
    augmentation dots add half the preceding duration or increment to the duration:
    they constitue a series of frations or the form '(2^n-1) / 2^n',
    starting with 3/2, 7/4, 15/8,
    that tends towards 2 while always remaining less than two.

    with MusicXML's limitation to 1024th of a whole note,
    with LilyPond's limitation to 128th of a whole note,
    valid numerators are:
  */

  int  numeratorDots = msrNumberOfDots (numerator);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numeratorDots " << ": " << numeratorDots <<
      std::endl << std::endl;
  }
#endif

  /*
    valid denominators are powers of 2

    the Rational representing a dotted duration has to be brought
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

  int denominatorDurationLog =
    msrDurationBinaryLogarithm (denominator);

  if (denominatorDurationLog == INT_MIN) {
    std::string result;

    {
      std::stringstream s;

      s <<
        1 <<
        "*" <<
        numerator <<
        '/' <<
        denominator;

      result = s.str ();
    }

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      std::stringstream s;

      s <<
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
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
    //    __FILE__, __LINE__,
        s.str ());
    }
#endif

    return result;
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> denominatorDurationLog" << ": " <<
      denominatorDurationLog <<
      std::endl << std::endl;
  }
#endif

  // bring the resulting fraction to be less that two if needed
  if (integralNumberOfWholeNotes) {
    // adapt the duration to avoid even numerators if can be,
    // since dotted durations cannot be recognized otherwise
    // 6/1 thus becomes 3 \breve, hence '\longa.'
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> integralNumberOfWholeNotes,"
        " bringing the faction to be less that 2" <<
        std::endl;
    }
#endif

    while (numerator % 2 == 0) {
      numerator /= 2;
      denominatorDurationLog -= 1;

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
        gLogStream <<
          "--> numerator" << ": " <<
          numerator <<
          std::endl <<
          "--> denominatorDurationLog " << ": " <<
          denominatorDurationLog <<
          std::endl << std::endl;
      }
#endif
    } // while

    // update the number of dots
    numeratorDots = msrNumberOfDots (numerator);
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numerator: " <<
      numerator <<
      std::endl <<
      "--> denominatorDurationLog: " <<
      denominatorDurationLog <<
      std::endl <<
      "--> numeratorDots: " <<
      numeratorDots <<
      std::endl << std::endl;
  }
#endif

  // take care of the dots
  int multiplyingFactor = 1;

  if (numeratorDots >= 0 && denominatorDurationLog >= numeratorDots) {
    // take the dots into account
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> taking the dots into account" <<
        std::endl;
    }
#endif

    denominatorDurationLog -= numeratorDots;

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> denominatorDurationLog" << ": " <<
        denominatorDurationLog <<
        std::endl <<
        "--> multiplyingFactor " << ": " <<
        multiplyingFactor <<
        std::endl << std::endl;
    }
#endif
  }
  else {
    // set the multiplying factor
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> setting the multiplying factor" <<
        std::endl;
    }
#endif

    // 5/8 becomes 8*5

    multiplyingFactor = numerator;
    numerator = 1;

    /* JMI
    multiplyingFactor = numerator;

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> denominatorDurationLog" << ": " <<
        denominatorDurationLog <<
        std::endl <<
        "--> multiplyingFactor " << ": " <<
        multiplyingFactor <<
        std::endl << std::endl;
    }
#endif

    while (multiplyingFactor >= 2) {
      // double duration
      --denominatorDurationLog;

      // adapt multiplying factor
      multiplyingFactor /= 2;

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
        gLogStream <<
          "--> denominatorDurationLog" << ": " <<
          denominatorDurationLog <<
          std::endl <<
          "--> multiplyingFactor " << ": " <<
          multiplyingFactor <<
          std::endl << std::endl;
      }
#endif
    } // while
    */
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numerator " << ": " <<
      numerator <<
      std::endl <<
      "--> numeratorDots " << ": " <<
      numeratorDots <<
      std::endl <<
      "--> denominatorDurationLog" << ": " <<
      denominatorDurationLog <<
      std::endl <<
      "--> multiplyingFactor " << ": " <<
      multiplyingFactor <<
      std::endl << std::endl;
  }
#endif

  // generate the code for the duration
  std::stringstream s;

  switch (denominatorDurationLog) {
    case -3:
      s << "\\maxima";
      break;
    case -2:
      s << "\\longa";
      break;
    case -1:
      s << "\\breve";
      break;

    default:
      s << (1 << denominatorDurationLog);
  } // switch

  // append the dots if any
  if (numeratorDots > 0) {
    for (int i = 0; i < numeratorDots; ++i) {
      s << ".";
    } // for
  }

  if (multiplyingFactor != 1) {
    // append the multiplying factor
    s <<
      "*" << multiplyingFactor;

    /* JMI
    if (integralNumberOfWholeNotes) {
      s <<
        "*" << multiplyingFactor;
    }
    else {
      s <<
        "*" << multiplyingFactor << '/' << 1; // ??? denominator;
    }
    */
  }

  std::string result = s.str ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> wholeNotesAsMsrString() 2 -------------------------------------" <<
     ", result: \"" << result << "\"" <<
      ", numeratorDots" << ": " << numeratorDots <<
      std::endl;
  }
#endif

  // return the result
  dotsNumber = numeratorDots;

  return result;
}

std::string wholeNotesAsMsrString (
  int             inputLineNumber,
  const Rational& wholeNotes)
{
  int dotsNumber; // not used

  return
    wholeNotesAsMsrString (
      inputLineNumber,
      wholeNotes,
      dotsNumber);
}

std::string multipleFullBarRestsWholeNotesAsMsrString (
  int             inputLineNumber, // JMI
  const Rational& wholeNotes)
{
  std::stringstream s;

  Rational
    denominatorAsFraction =
      Rational (
        1,
        wholeNotes.getDenominator ());

  int numberOfWholeNotes =
    wholeNotes.getNumerator ();

  s <<
    wholeNotesAsMsrString ( // JMI ??? v0.9.66
      inputLineNumber,
      denominatorAsFraction);

  if (numberOfWholeNotes != 1) {
    s <<
      "*" << numberOfWholeNotes;
  }

  return s.str ();
}


}

