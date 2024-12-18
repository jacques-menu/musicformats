/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfServices.h"

#include "msrNotesDurations.h"

#include "oahEarlyOptions.h"

#include "msrWae.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// durations
//______________________________________________________________________________
msrNotesDurationKind msrNotesDurationKindFromMusicXMLString (
  int                inputLineNumber,
  const std::string& durationString)
{
  msrNotesDurationKind result = msrNotesDurationKind::kNotesDuration_UNKNOWN_;

  if      (durationString == "maxima") {
    result = msrNotesDurationKind::kNotesDurationMaxima;
  }
  else if (durationString == "long") {
    result = msrNotesDurationKind::kNotesDurationLonga;
  }
  else if (durationString == "breve") {
    result = msrNotesDurationKind::kNotesDurationBreve;
  }
  else if (durationString == "whole") {
    result = msrNotesDurationKind::kNotesDurationWhole;
  }
  else if (durationString == "half") {
    result = msrNotesDurationKind::kNotesDurationHalf;
  }
  else if (durationString == "quarter") {
    result = msrNotesDurationKind::kNotesDurationQuarter;
  }
  else if (durationString == "eighth") {
    result = msrNotesDurationKind::kNotesDurationEighth;
  }
  else if (durationString == "16th") {
    result = msrNotesDurationKind::kNotesDuration16th;
  }
  else if (durationString == "32nd") {
    result = msrNotesDurationKind::kNotesDuration32nd;
  }
  else if (durationString == "64th") {
    result = msrNotesDurationKind::kNotesDuration64th;
  }
  else if (durationString == "128th") {
    result = msrNotesDurationKind::kNotesDuration128th;
  }
  else if (durationString == "256th") {
    result = msrNotesDurationKind::kNotesDuration256th;
  }
  else if (durationString == "512th") {
    result = msrNotesDurationKind::kNotesDuration512th;
  }
  else if (durationString == "1024th") {
    result = msrNotesDurationKind::kNotesDuration1024th;
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

EXP msrNotesDurationKind msrNotesDurationKindFromInteger (
  int inputLineNumber,
  int durationInteger)
{
  msrNotesDurationKind result = msrNotesDurationKind::kNotesDuration_UNKNOWN_;

  switch (durationInteger) {
    case 1:
      result = msrNotesDurationKind::kNotesDurationWhole;
      break;
    case 2:
      result = msrNotesDurationKind::kNotesDurationHalf;
      break;
    case 4:
      result = msrNotesDurationKind::kNotesDurationQuarter;
      break;
    case 8:
      result = msrNotesDurationKind::kNotesDurationEighth;
      break;
    case 16:
      result = msrNotesDurationKind::kNotesDuration16th;
      break;
    case 32:
      result = msrNotesDurationKind::kNotesDuration32nd;
      break;
    case 64:
      result = msrNotesDurationKind::kNotesDuration64th;
      break;
    case 128:
      result = msrNotesDurationKind::kNotesDuration128th;
      break;
    case 256:
      result = msrNotesDurationKind::kNotesDuration256th;
      break;
    case 512:
      result = msrNotesDurationKind::kNotesDuration512th;
      break;
    case 1024:
      result = msrNotesDurationKind::kNotesDuration1024th;
      break;
    default:
      ;
  } // switch

  return result;
}

msrNotesDurationKind msrNotesDurationKindFromString (
  int                inputLineNumber,
  const std::string& durationString)
{
  msrNotesDurationKind result = msrNotesDurationKind::kNotesDuration_UNKNOWN_;

  if      (durationString == "maxima") {
    result = msrNotesDurationKind::kNotesDurationMaxima;
  }
  else if (durationString == "long") {
    result = msrNotesDurationKind::kNotesDurationLonga;
  }
  else if (durationString == "breve") {
    result = msrNotesDurationKind::kNotesDurationBreve;
  }
  else if (durationString == "1") {
    result = msrNotesDurationKind::kNotesDurationWhole;
  }
  else if (durationString == "2") {
    result = msrNotesDurationKind::kNotesDurationHalf;
  }
  else if (durationString == "4") {
    result = msrNotesDurationKind::kNotesDurationQuarter;
  }
  else if (durationString == "8") {
    result = msrNotesDurationKind::kNotesDurationEighth;
  }
  else if (durationString == "16") {
    result = msrNotesDurationKind::kNotesDuration16th;
  }
  else if (durationString == "32") {
    result = msrNotesDurationKind::kNotesDuration32nd;
  }
  else if (durationString == "64") {
    result = msrNotesDurationKind::kNotesDuration64th;
  }
  else if (durationString == "128") {
    result = msrNotesDurationKind::kNotesDuration128th;
  }
  else if (durationString == "256") {
    result = msrNotesDurationKind::kNotesDuration256th;
  }
  else if (durationString == "512") {
    result = msrNotesDurationKind::kNotesDuration512th;
  }
  else if (durationString == "1024") {
    result = msrNotesDurationKind::kNotesDuration1024th;
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

EXP msrWholeNotes wholeNotesFromNotesDurationKindAndDotsNumber (
  msrNotesDurationKind notesNotesDurationKind,
  int                  dotsNumber)
{
  // convert duration into whole notes
  msrWholeNotes
    result =
      msrNotesDurationKindAsWholeNotes (
        notesNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDurations ()) {
    std::stringstream ss;

    ss <<
      "=== wholeNotesFromNotesDurationKindAndDotsNumber()" <<
      ", (int) notesNotesDurationKind: " << (int) notesNotesDurationKind <<
      ", result: " << result;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take dots into account if any
  if (dotsNumber > 0) {
    msrWholeNotes
      increment = result * mfRational (1,2);

    int dots = dotsNumber;

    while (dots > 0) {
      result += increment;

      increment *= mfRational (1,2);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDurations ()) {
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

msrWholeNotes msrNotesDurationKindAsWholeNotes (msrNotesDurationKind notesNotesDurationKind)
{
  msrWholeNotes result;

  switch (notesNotesDurationKind) {
    case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
      result = msrWholeNotes (0, 1);
      break;

    case msrNotesDurationKind::kNotesDuration1024th:
      result = msrWholeNotes (1, 1024);
      break;
    case msrNotesDurationKind::kNotesDuration512th:
      result = msrWholeNotes (1, 512);
      break;
    case msrNotesDurationKind::kNotesDuration256th:
      result = msrWholeNotes (1, 256);
      break;
    case msrNotesDurationKind::kNotesDuration128th:
      result = msrWholeNotes (1, 128);
      break;
    case msrNotesDurationKind::kNotesDuration64th:
      result = msrWholeNotes (1, 64);
      break;
    case msrNotesDurationKind::kNotesDuration32nd:
      result = msrWholeNotes (1, 32);
      break;
    case msrNotesDurationKind::kNotesDuration16th:
      result = msrWholeNotes (1, 16);
      break;
    case msrNotesDurationKind::kNotesDurationEighth:
      result = msrWholeNotes (1, 8);
      break;
    case msrNotesDurationKind::kNotesDurationQuarter:
      result = msrWholeNotes (1, 4);
      break;
    case msrNotesDurationKind::kNotesDurationHalf:
      result = msrWholeNotes (1, 2);
      break;
    case msrNotesDurationKind::kNotesDurationWhole:
      result = msrWholeNotes (1, 1);
      break;
    case msrNotesDurationKind::kNotesDurationBreve:
      result = msrWholeNotes (2, 1);
      break;
    case msrNotesDurationKind::kNotesDurationLonga:
      result = msrWholeNotes (4, 1);
      break;
    case msrNotesDurationKind::kNotesDurationMaxima:
      result = msrWholeNotes (8, 1);
      break;
  } // switch

  return result;
}

msrNotesDurationKind wholeNotesAsNotesDurationKind (msrWholeNotes wholeNotes)
{
  msrNotesDurationKind result = msrNotesDurationKind::kNotesDuration_UNKNOWN_;

  if (wholeNotes.getNumerator () == 1) {
    switch (wholeNotes.getDenominator ()) {
      case 1:
        result = msrNotesDurationKind::kNotesDurationWhole;
        break;
      case 2:
        result = msrNotesDurationKind::kNotesDurationHalf;
        break;
      case 4:
        result = msrNotesDurationKind::kNotesDurationQuarter;
        break;
      case 8:
        result = msrNotesDurationKind::kNotesDurationEighth;
        break;
      case 16:
        result = msrNotesDurationKind::kNotesDuration16th;
        break;
      case 32:
        result = msrNotesDurationKind::kNotesDuration32nd;
        break;
      case 64:
        result = msrNotesDurationKind::kNotesDuration64th;
        break;
      case 128:
        result = msrNotesDurationKind::kNotesDuration128th;
        break;
      case 256:
        result = msrNotesDurationKind::kNotesDuration256th;
        break;
      case 512:
        result = msrNotesDurationKind::kNotesDuration512th;
        break;
      case 1024:
        result = msrNotesDurationKind::kNotesDuration1024th;
        break;
      default:
        ;
    } // switch
  }

  else if (wholeNotes.getDenominator () == 1) {
    switch (wholeNotes.getNumerator ()) {
      case 2:
        result = msrNotesDurationKind::kNotesDurationBreve;
        break;
      case 4:
        result = msrNotesDurationKind::kNotesDurationLonga;
        break;
      case 8:
        result = msrNotesDurationKind::kNotesDurationMaxima;
        break;
      default:
        ;
    } // switch
  }

  return result;
}

std::string msrNotesDurationKindAsMusicXMLType (msrNotesDurationKind notesNotesDurationKind)
{
  std::string result;

  switch (notesNotesDurationKind) {
    case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
      result = "noNotesDuration";
      break;

    case msrNotesDurationKind::kNotesDuration1024th:
      result = "1024th";
      break;
    case msrNotesDurationKind::kNotesDuration512th:
      result = "512th";
      break;
    case msrNotesDurationKind::kNotesDuration256th:
      result = "256th";
      break;
    case msrNotesDurationKind::kNotesDuration128th:
      result = "128th";
      break;
    case msrNotesDurationKind::kNotesDuration64th:
      result = "64th";
      break;
    case msrNotesDurationKind::kNotesDuration32nd:
      result = "32nd";
      break;
    case msrNotesDurationKind::kNotesDuration16th:
      result = "16th";
      break;
    case msrNotesDurationKind::kNotesDurationEighth:
      result = "eighth";
      break;
    case msrNotesDurationKind::kNotesDurationQuarter:
      result = "quarter";
      break;
    case msrNotesDurationKind::kNotesDurationHalf:
      result = "half";
      break;
    case msrNotesDurationKind::kNotesDurationWhole:
      result = "whole";
      break;
    case msrNotesDurationKind::kNotesDurationBreve:
      result = "breve";
      break;
    case msrNotesDurationKind::kNotesDurationLonga:
      result = "long";
      break;
    case msrNotesDurationKind::kNotesDurationMaxima:
      result = "maxima";
      break;
  } // switch

  return result;
}

std::string msrNotesDurationKindAsString_INTERNAL (
  msrNotesDurationKind notesNotesDurationKind) // JMI don't keep ??? v0.9.67
{
  std::string result;

  switch (notesNotesDurationKind) {
    case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
      result = "kNotesDuration_UNKNOWN_";
      break;

    case msrNotesDurationKind::kNotesDuration1024th:
      result = "kNotesDuration1024th";
      break;
    case msrNotesDurationKind::kNotesDuration512th:
      result = "kNotesDuration512th";
      break;
    case msrNotesDurationKind::kNotesDuration256th:
      result = "kNotesDuration256th";
      break;
    case msrNotesDurationKind::kNotesDuration128th:
      result = "kNotesDuration128th";
      break;
    case msrNotesDurationKind::kNotesDuration64th:
      result = "kNotesDuration64th";
      break;
    case msrNotesDurationKind::kNotesDuration32nd:
      result = "kNotesDuration32nd";
      break;
    case msrNotesDurationKind::kNotesDuration16th:
      result = "kNotesDuration16th";
      break;
    case msrNotesDurationKind::kNotesDurationEighth:
      result = "kNotesDurationEighth";
      break;
    case msrNotesDurationKind::kNotesDurationQuarter:
      result = "kNotesDurationQuarter";
      break;
    case msrNotesDurationKind::kNotesDurationHalf:
      result = "kNotesDurationHalf";
      break;
    case msrNotesDurationKind::kNotesDurationWhole:
      result = "kNotesDurationWhole";
      break;
    case msrNotesDurationKind::kNotesDurationBreve:
      result = "kNotesDurationBreve";
      break;
    case msrNotesDurationKind::kNotesDurationLonga:
      result = "kNotesDurationLonga";
      break;
    case msrNotesDurationKind::kNotesDurationMaxima:
      result = "kNotesDurationMaxima";
      break;
  } // switch

  return result;
}

std::string msrNotesDurationKindAsString (
  msrNotesDurationKind notesNotesDurationKind)
{
  std::string result;

  switch (notesNotesDurationKind) {
    case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
      result = "???NoNotesDuration???";
      break;

    case msrNotesDurationKind::kNotesDuration1024th:
      result = "1024";
      break;
    case msrNotesDurationKind::kNotesDuration512th:
      result = "512";
      break;
    case msrNotesDurationKind::kNotesDuration256th:
      result = "256";
      break;
    case msrNotesDurationKind::kNotesDuration128th:
      result = "128";
      break;
    case msrNotesDurationKind::kNotesDuration64th:
      result = "64";
      break;
    case msrNotesDurationKind::kNotesDuration32nd:
      result = "32";
      break;
    case msrNotesDurationKind::kNotesDuration16th:
      result = "16";
      break;
    case msrNotesDurationKind::kNotesDurationEighth:
      result = "8";
      break;
    case msrNotesDurationKind::kNotesDurationQuarter:
      result = "4";
      break;
    case msrNotesDurationKind::kNotesDurationHalf:
      result = "2";
      break;
    case msrNotesDurationKind::kNotesDurationWhole:
      result = "1";
      break;
    case msrNotesDurationKind::kNotesDurationBreve:
      result = "Breve";
      break;
    case msrNotesDurationKind::kNotesDurationLonga:
      result = "Longa";
      break;
    case msrNotesDurationKind::kNotesDurationMaxima:
      result = "Maxima";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrNotesDurationKind& elt)
{
  os << msrNotesDurationKindAsString (elt);
  return os;
}

// //______________________________________________________________________________
// msrNotesDuration::msrNotesDuration ()
// {
//   fNotesDurationKind = msrNotesDurationKind::kNotesDuration_UNKNOWN_;
//   fDotsNumber = 0;
// }
//
// msrNotesDuration::msrNotesDuration (
//   msrNotesDurationKind notesNotesDurationKind,
//   int                  dotsNumber)
// {
//   fNotesDurationKind = notesNotesDurationKind;
//   fDotsNumber        = dotsNumber;
// }
//
// msrNotesDuration::~msrNotesDuration ()
// {}
//
// msrWholeNotes msrNotesDuration::dottedNotesDurationAsWholeNotes (
//   int inputLineNumber) const
// {
//   // convert duration into whole notes
//   msrWholeNotes
//     result =
//       msrNotesDurationKindAsWholeNotes (
//         fNotesDurationKind);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceNotesDurations ()) {
//     print (gLog);
//
//     std::stringstream ss;
//
//     ss <<
//       "=== dottedNotesDurationAsWholeNotes()" <<
//       ", (int) fNotesDurationKind: " << (int) fNotesDurationKind <<
//       ", result: " << result;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // take dots into account if any
//   if (fDotsNumber > 0) {
//     msrWholeNotes
//       increment = result * mfRational (1,2);
//
//     int dots = fDotsNumber;
//
//     while (dots > 0) {
//       result += increment;
//
//       increment *= mfRational (1,2);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceNotesDurations ()) {
//     std::stringstream ss;
//
//     ss <<
//       "=== dottedNotesDurationAsWholeNotes()" <<
//       ", dots: " << dots <<
//       ", result: " << result <<
//       ", increment: " << increment;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//       --dots;
//     } // while
//   }
//
//   return result;
// }
//
// msrWholeNotes msrNotesDuration::dottedNotesDurationAsWholeNotes_FOR_TEMPO (
//   // used in lpsrEnumTypes, dottedNotesDurationAsLilypondStringWithoutBackSlash(),
//   // called in lpsr2lilypondTranslator.cpp, visitStart (S_msrTempo& elt)
//   // JMI BUGGY, NEVER TESTED TEMP??? v0.9.67
//   int inputLineNumber) const
// {
//   // convert duration into whole notes
//   msrWholeNotes
//     result =
//       msrNotesDurationKindAsWholeNotes (
//         fNotesDurationKind);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceNotesDurations ()) {
//     std::stringstream ss;
//
//     ss <<
//       "=== dottedNotesDurationAsWholeNotes_FOR_TEMPO()" <<
//       ", result: " << result <<
//       "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // take dots into account if any
//   if (fDotsNumber > 0) {
//     int dots = fDotsNumber;
//
//     while (dots > 0) {
//       result *=
//         mfRational (3, 2);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceNotesDurations ()) {
//     std::stringstream ss;
//
//     ss <<
//       "=== dottedNotesDurationAsWholeNotes_FOR_TEMPO()" <<
//       ", dots: " << dots <<
//       ", result: " << result <<
//       "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//       --dots;
//     } // while
//   }
//
//   return result;
// }
//
// std::string msrNotesDuration::asString () const
// {
//   std::stringstream ss;
//
//   ss <<
//      fNotesDurationKind;
//
//   for (int i = 1; i <= fDotsNumber; ++i) {
//     ss << ".";
//   } // for
//
//   return ss.str ();
// }
//
// void msrNotesDuration::print (std::ostream& os) const
// {
//   constexpr int fieldWidth = 11;
//
//   os << std::left <<
//     std::setw (fieldWidth) <<
//     "fNotesDurationKind" << ": " <<
//     fNotesDurationKind <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "dotsNumber" << ": " << fDotsNumber <<
//     std::endl;
// };
//
// std::ostream& operator << (std::ostream& os, const msrNotesDuration& elt)
// {
//   os << elt.asString ();
//   return os;
// }

//_______________________________________________________________________________
int msrNotesDurationBinaryLogarithm (int duration)
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
msrDottedNotesDuration::msrDottedNotesDuration ()
{
  fNotesDurationKind = msrNotesDurationKind::kNotesDuration_UNKNOWN_;
  fDotsNumber = 0;
}

msrDottedNotesDuration::msrDottedNotesDuration (
  msrNotesDurationKind notesNotesDurationKind,
  int                  dotsNumber)
{
  fNotesDurationKind = notesNotesDurationKind;
  fDotsNumber        = dotsNumber;
}

msrDottedNotesDuration::~msrDottedNotesDuration ()
{}

msrWholeNotes msrDottedNotesDuration::dottedNotesDurationAsWholeNotes (
  int inputLineNumber) const
{
  // convert duration into whole notes
  msrWholeNotes
    result =
      msrNotesDurationKindAsWholeNotes (
        fNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDurations ()) {
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
    msrWholeNotes
      increment = result * mfRational (1,2);

    int dots = fDotsNumber;

    while (dots > 0) {
      result += increment;

      increment *= mfRational (1,2);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDurations ()) {
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

msrWholeNotes msrDottedNotesDuration::dottedNotesDurationAsWholeNotes_FOR_TEMPO (
  // used in lpsrEnumTypes, dottedNotesDurationAsLilypondStringWithoutBackSlash(),
  // called in lpsr2lilypondTranslator.cpp, visitStart (S_msrTempo& elt)
  // JMI BUGGY, NEVER TESTED TEMP???
  int inputLineNumber) const
{
  // convert duration into whole notes
  msrWholeNotes
    result =
      msrNotesDurationKindAsWholeNotes (
        fNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDurations ()) {
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
  if (gTraceOahGroup->getTraceNotesDurations ()) {
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

std::string msrDottedNotesDuration::asString () const
{
  std::stringstream ss;

  ss <<
     fNotesDurationKind;

  for (int i = 1; i <= fDotsNumber; ++i) {
    ss << ".";
  } // for

  return ss.str ();
}

void msrDottedNotesDuration::print (std::ostream& os) const
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

std::ostream& operator << (std::ostream& os, const msrDottedNotesDuration& elt)
{
  os << elt.asString ();
  return os;
}

//______________________________________________________________________________
msrWholeNotes::msrWholeNotes ()
{
  fNumerator   = 0;
  fDenominator = 1;
}

msrWholeNotes::msrWholeNotes (
  long int num,
  long int denom)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    denom > 0,
    "msrWholeNotes denominator '" + std::to_string (denom) + "' shoule be positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fNumerator = num;
  fDenominator = denom;

  rationalise ();
}

msrWholeNotes::msrWholeNotes (const msrWholeNotes& wholeNotes)
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
  if (gTraceOahGroup->getTraceNotesDurations ()) {
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
      "msrWholeNotes string '" << theString <<
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
  // a/b == c/d if and only if a * d == b * c.
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

void msrWholeNotes::rationalise ()
{
  long int g = std::gcd (fNumerator, fDenominator);

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

std::string msrWholeNotes::asShortString () const
{
  std::stringstream ss;

  ss << "[msrWholeNotes ";

  if (fNumerator == K_WHOLE_NOTES_NUMERATOR_UNKNOWN_ ) {
    ss << "UNKNOWN_WHOLE_NOTES";
  }
  else {
    ss << fNumerator << '/' << fDenominator;
  }

  ss << ']';

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
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const msrWholeNotes& wholeNotes)
{
  os << wholeNotes.asShortString ();
  return os;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const msrWholeNotes& wholeNotes)
{
  iss.getStringstream () <<
    wholeNotes.asString ();

  return iss;
}

//_______________________________________________________________________________
std::string wholeNotesAndDotsNumberPitchAndOctaveAsString (
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotes,
  int&                 dotsNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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

  if (numerator == 0) { // JMI TEMP v0.9.70
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

   msrError ( // JMI v0.9.70
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
  if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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

  int  numeratorDots = msrNumberOfDots (numerator);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
    msrNotesDurationBinaryLogarithm (denominator);

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
    if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
  if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
    if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
      if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
    numeratorDots = msrNumberOfDots (numerator);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
    if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
    if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
    if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
    if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
      if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
  if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
  if (gTraceOahGroup->getTraceWholeNoteDurationsDetails ()) {
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
  const msrWholeNotes& wholeNotes)
{
  int dotsNumber; // not used JMI v0.9.70

  return
    wholeNotesAndDotsNumberPitchAndOctaveAsString (
      inputLineNumber,
      wholeNotes,
      dotsNumber);
}

std::string multiMeasureRestsWholeNotesPitchAndOctaveAsString (
  int                  inputLineNumber, // JMI
  const msrWholeNotes& wholeNotes)
{
  std::stringstream ss;

  msrWholeNotes
    wholeNotesUnit =
      msrWholeNotes (
        1,
        wholeNotes.getDenominator ());

  int numberOfWholeNotes =
    wholeNotes.getNumerator ();

  ss <<
    wholeNotesPitchAndOctaveAsString ( // JMI ??? v0.9.66
      inputLineNumber,
      wholeNotesUnit);

  if (numberOfWholeNotes != 1) {
    ss <<
      "*" << numberOfWholeNotes;
  }

  return ss.str ();
}

//______________________________________________________________________________
EXP const msrWholeNotes
  K_WHOLE_NOTES_UNKNOWN_ (
   K_WHOLE_NOTES_NUMERATOR_UNKNOWN_, 1);

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

