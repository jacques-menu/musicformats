/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iostream>
#include <sstream>

#include "mfStaticSettings.h"

#include "lpsrWae.h"

#include "mfStaticSettings.h"

#include "mfServices.h"
#include "mfStringsHandling.h"

#include "lpsrEnumTypes.h"

#include "oahOah.h"
#include "waeOah.h"

#include "lpsrOah.h"

#include "oahWae.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

//_______________________________________________________________________________
int lpsrDurationBinaryLogarithm (int duration)
{
  int result = INT_MIN;

/*
with LilyPond's limitation to 128th of a whole note,
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
    case 256: // JMI
      result = 8;
      break;

    default:
      ;
  } // switch

  return result;
}

//_______________________________________________________________________________
int lpsrNumberOfDots (int n)
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

    default:
      ;
    } // switch

  return result;
}

//_______________________________________________________________________________
std::string wholeNotesAsLilypondString (
  int             inputLineNumber,
  const Rational& wholeNotes,
  int&            dotsNumber)
{
  // this algorithm is inspired by musicxml2ly

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "--> wholeNotesAsLilypondString() 1 -------------------------------------" <<
      ", wholeNotes: " << wholeNotes <<
      ", dotsNumber: " << dotsNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "--> numerator:   " << numerator <<
      std::endl <<
      "--> denominator: " << denominator <<
      std::endl << std::endl;
  }
#endif

/* JMI
  if (false && numerator <= 0) { // JMI
    std::stringstream s;

    s <<
     "%{ZERO_LENGTH: " <<
     wholeNotes << // JMI
     ", line " <<
     inputLineNumber <<
     " %}";

    return s.str ();
  }
*/

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (numerator <= 0) {
    std::stringstream s1;

    s1 <<
      "numerator is not positive in wholeNotesAsLilypondString()" <<
      ", wholeNotes: " << wholeNotes;

    std::string message = s1.str ();

 //   lpsrError ( JMI
    lpsrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
  //    __FILE__, __LINE__,
      message);

    std::stringstream s2;

    s2 <<
      "%{ " <<
      message <<
      ", line: " << inputLineNumber <<
      " ???%}";

    return s2.str ();
  }
#endif

  Bool
    integralNumberOfWholeNotes = denominator == 1;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
//       "--> rationalHasBeenSimplified: " <<
//      rationalHasBeenSimplified <<
//       std::endl <<
      "--> integralNumberOfWholeNotes: " <<
      integralNumberOfWholeNotes <<
      std::endl << std::endl;
  }
#endif

  /*
    augmentation dots add half the preceding increment to the duration:
    they constitue a series of frations or the form '(2^n-1) / 2^n',
    starting with 3/2, 7/4, 15/8,
    that tends towards 2 while always remaining less than two.

    with MusicXML's limitation to 1024th of a whole note,
    with LilyPond's limitation to 128th of a whole note,
    valid numerators are:
  */

  int  numeratorDots = lpsrNumberOfDots (numerator);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numeratorDots " << ": " << numeratorDots <<
      std::endl << std::endl;
  }
#endif

/*
    default:
      if (! integralNumberOfWholeNotes) {
  //      multiplyingFactorIsToBeUsed = true;
      }
      else {
        // there can be integral numbers of whole notes up to 15,
        // corresponding to a \maxima...
          std::stringstream s;

          s <<
            "numerator " << numerator <<
            " is not one of 1, 3, 7, 15, 31, 63 or 127" <<
       //     " is not one of 1, 3, 7, 15, 31, 63, 127, 255, 511 or 1023" <<
            ", whole notes duration " <<
            numerator << '/' << denominator;

          if (rationalHasBeenSimplified) {
            s <<
              " (" << numerator << '/' << denominator << ")" <<
            std::endl;
          }

          s <<
            " it will be represented using a multiplying factor";

          lpsrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            s.str ());

  //      multiplyingFactorIsToBeUsed = true;
      }
  } // switch
    */

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
    lpsrDurationBinaryLogarithm (denominator);

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
        numerator << '/' << denominator;

//       if (rationalHasBeenSimplified) {
//         s <<
//           " (" << numerator << '/' << denominator << ")" <<
//         std::endl;
//       }

      s <<
        " cannot be represented as a dotted power of 2" <<
        ", " <<
        result <<
        " will be used";

   //   lpsrError ( JMI
      lpsrWarning (
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
    // 6/1 thus becomes 3\breve, hence \longa.
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
    numeratorDots = lpsrNumberOfDots (numerator);
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numerator" << ": " <<
      numerator <<
      std::endl <<
      "--> denominatorDurationLog" << ": " <<
      denominatorDurationLog <<
      std::endl <<
      "--> numeratorDots " << ": " <<
      numeratorDots <<
      std::endl << std::endl;
  }
#endif

  // take care of the dots
  int multiplyingFactor = 1;

  if (numeratorDots >= 0 && denominatorDurationLog >= numeratorDots) {
    // take the dots into account
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
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
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
      "--> wholeNotesAsLilypondString() 2 -------------------------------------" <<
     ", result: \"" << result << "\"" <<
      ", numeratorDots" << ": " << numeratorDots <<
      std::endl;
  }
#endif

  // return the result
  dotsNumber = numeratorDots;

  return result;
}

std::string wholeNotesAsLilypondString (
  int             inputLineNumber,
  const Rational& wholeNotes)
{
  int dotsNumber; // not used

  return
    wholeNotesAsLilypondString (
      inputLineNumber,
      wholeNotes,
      dotsNumber);
}

//_______________________________________________________________________________
std::string dottedDurationAsLilypondString (
  int               inputLineNumber,
  msrDottedDuration dottedDuration)
{
  return
    wholeNotesAsLilypondString (
      inputLineNumber,
      dottedDuration.dottedDurationAsWholeNotes_FOR_TEMPO (
        inputLineNumber));
}

std::string dottedDurationAsLilypondStringWithoutBackSlash (
  int               inputLineNumber,
  msrDottedDuration dottedDuration)
{
  std::string result =
    wholeNotesAsLilypondString (
      inputLineNumber,
      dottedDuration.dottedDurationAsWholeNotes_FOR_TEMPO (
        inputLineNumber));

  if (result [0] == '\\') {
    result = result.substr (1);
  }

  return result;
}

//_______________________________________________________________________________
std::string multipleFullBarRestsWholeNoteAsLilypondString (
  int             inputLineNumber,
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
    wholeNotesAsLilypondString (
      inputLineNumber,
      denominatorAsFraction);

  if (numberOfWholeNotes != 1) {
    s <<
      "*" << numberOfWholeNotes;
  }

  return s.str ();
}

//_______________________________________________________________________________
void writeTextsListAsLilypondString (
  const std::list<std::string>& textsList,
  std::ostream&            os)
{
  std::string contents;

  if (textsList.size ()) {
    std::list<std::string>::const_iterator
      iBegin = textsList.begin (),
      iEnd   = textsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      contents += (*i);
      if (++i == iEnd) break;
  // JMI    os << ", ";
    } // for
  }

  size_t found = contents.find("\"");
  if (found != std::string::npos) {
    os << mfDoubleQuoteString (contents);
  }
  else {
    os << mfDoubleQuoteStringIfNonAlpha (contents); // JMI
  }
}

// score output kinds
//______________________________________________________________________________

std::map<std::string, lpsrScoreOutputKind>
  gGlobalLpsrScoreOutputKindsMap;

std::string lpsrScoreOutputKindAsString (
  lpsrScoreOutputKind scoreOutputKind)
{
  std::string result;

  // no CamelCase here, these strings are used in the command line options

  switch (scoreOutputKind) {
    case lpsrScoreOutputKind::kScoreOutputScoreOnly: // default value
      result = "score-only";
      break;
    case lpsrScoreOutputKind::kScoreOutputScoreAndParts:
      result = "score-and-parts";
      break;
    case lpsrScoreOutputKind::kScoreOutputPartsAndScore:
      result = "parts-and-score";
      break;
    case lpsrScoreOutputKind::kScoreOutputPartsOnly:
      result = "parts-only";
      break;
    case lpsrScoreOutputKind::kScoreOutputScoreAndPartsOneFile:
      result = "score-and-parts-one-file";
      break;
    case lpsrScoreOutputKind::kScoreOutputPartsAndScoreOneFile:
      result = "parts-and-score-one-file";
      break;
    case lpsrScoreOutputKind::kScoreOutputPartsOnlyOneFile:
      result = "parts-only-one-file";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrScoreOutputKind& elt)
{
  os << lpsrScoreOutputKindAsString (elt);
  return os;
}

void initializeLpsrScoreOutputKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalLpsrScoreOutputKindsMap ["score-only"] =
    lpsrScoreOutputKind::kScoreOutputScoreOnly;
  gGlobalLpsrScoreOutputKindsMap ["score-and-parts"] =
    lpsrScoreOutputKind::kScoreOutputScoreAndParts;
  gGlobalLpsrScoreOutputKindsMap ["parts-and-score"] =
    lpsrScoreOutputKind::kScoreOutputPartsAndScore;
  gGlobalLpsrScoreOutputKindsMap ["parts-only"] =
    lpsrScoreOutputKind::kScoreOutputPartsOnly;
  gGlobalLpsrScoreOutputKindsMap ["score-and-parts-one-file"] =
    lpsrScoreOutputKind::kScoreOutputScoreAndPartsOneFile;
  gGlobalLpsrScoreOutputKindsMap ["parts-and-score-one-file"] =
    lpsrScoreOutputKind::kScoreOutputPartsAndScoreOneFile;
  gGlobalLpsrScoreOutputKindsMap ["parts-only-one-file"] =
    lpsrScoreOutputKind::kScoreOutputPartsOnlyOneFile;
}

std::string availableLpsrScoreOutputKinds (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    lpsrScoreOutputKindsMapSize =
      gGlobalLpsrScoreOutputKindsMap.size ();

  if (lpsrScoreOutputKindsMapSize) {
    size_t
      nextToLast =
        lpsrScoreOutputKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, lpsrScoreOutputKind>::const_iterator i =
        gGlobalLpsrScoreOutputKindsMap.begin ();
      i != gGlobalLpsrScoreOutputKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != lpsrScoreOutputKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// accidental styles
//______________________________________________________________________________

std::map<std::string, lpsrAccidentalStyleKind>
  gGlobalLpsrAccidentalStyleKindsMap;

std::string lpsrAccidentalStyleKindAsString (
  lpsrAccidentalStyleKind accidentalStyleKind)
{
  std::string result;

  switch (accidentalStyleKind) {
    case lpsrAccidentalStyleKind::kAccidentalStyleDefault: // default value
      result = "kAccidentalStyleDefault";
      break;
    case lpsrAccidentalStyleKind::kVoice:
      result = "kVoice";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleModern:
      result = "kAccidentalStyleModern";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleModernCautionary:
      result = "kAccidentalStyleModernCautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleModernVoice:
      result = "kAccidentalStyleModernVoice";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleModernVoiceCautionary:
      result = "kAccidentalStyleModernVoiceCautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStylePiano:
      result = "kAccidentalStylePiano";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStylePianoCautionary:
      result = "kAccidentalStylePianoCautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNeoModern:
      result = "kAccidentalStyleNeoModern";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNeoModernCautionary:
      result = "kAccidentalStyleNeoModernCautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNeoModernVoice:
      result = "kAccidentalStyleNeoModernVoice";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNeoModernVoiceCautionary:
      result = "kAccidentalStyleNeoModernVoiceCautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleDodecaphonic:
      result = "kAccidentalStyleDodecaphonic";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleDodecaphonicNoRepeat:
      result = "kAccidentalStyleDodecaphonicNoRepeat";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleDodecaphonicFirst:
      result = "kAccidentalStyleDodecaphonicFirst";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleTeaching:
      result = "kAccidentalStyleTeaching";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNoReset:
      result = "kAccidentalStyleNoReset";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleForget:
      result = "kAccidentalStyleForget";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrAccidentalStyleKind& elt)
{
  os << lpsrAccidentalStyleKindAsString (elt);
  return os;
}

std::string lpsrAccidentalStyleKindAsLilypondString (
  lpsrAccidentalStyleKind accidentalStyleKind)
{
  std::string result;

  switch (accidentalStyleKind) {
    case lpsrAccidentalStyleKind::kAccidentalStyleDefault: // default value
      result = "default";
      break;
    case lpsrAccidentalStyleKind::kVoice:
      result = "voice";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleModern:
      result = "modern";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleModernCautionary:
      result = "modern-cautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleModernVoice:
      result = "modern-voice";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleModernVoiceCautionary:
      result = "modern-voice-cautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStylePiano:
      result = "piano";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStylePianoCautionary:
      result = "piano-cautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNeoModern:
      result = "neo-modern";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNeoModernCautionary:
      result = "neo-modern-cautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNeoModernVoice:
      result = "neo-modern-voice";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNeoModernVoiceCautionary:
      result = "neo-modern--voice-cautionary";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleDodecaphonic:
      result = "dodecaphonic";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleDodecaphonicNoRepeat:
      result = "dodecaphonic-no-repeat";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleDodecaphonicFirst:
      result = "dodecaphonic-first";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleTeaching:
      result = "teaching";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleNoReset:
      result = "no-reset";
      break;
    case lpsrAccidentalStyleKind::kAccidentalStyleForget:
      result = "forget";
      break;
  } // switch

  return result;
} // switch

void initializeLpsrAccidentalStyleKindsMap ()
{
  // register the LilyPond accidental styles
  // --------------------------------------

  gGlobalLpsrAccidentalStyleKindsMap ["default"] =
    lpsrAccidentalStyleKind::kAccidentalStyleDefault;
  gGlobalLpsrAccidentalStyleKindsMap ["voice"] =
    lpsrAccidentalStyleKind::kVoice;
  gGlobalLpsrAccidentalStyleKindsMap ["modern"] =
    lpsrAccidentalStyleKind::kAccidentalStyleModern;
  gGlobalLpsrAccidentalStyleKindsMap ["modern-cautionary"] =
    lpsrAccidentalStyleKind::kAccidentalStyleModernCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["modern-voice"] =
    lpsrAccidentalStyleKind::kAccidentalStyleModernVoice;
  gGlobalLpsrAccidentalStyleKindsMap ["modern-voice-cautionary"] =
    lpsrAccidentalStyleKind::kAccidentalStyleModernVoiceCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["piano"] =
    lpsrAccidentalStyleKind::kAccidentalStylePiano;
  gGlobalLpsrAccidentalStyleKindsMap ["piano-cautionary"] =
    lpsrAccidentalStyleKind::kAccidentalStylePianoCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["neo-modern"] =
    lpsrAccidentalStyleKind::kAccidentalStyleNeoModern;
  gGlobalLpsrAccidentalStyleKindsMap ["neo-modern-cautionary"] =
    lpsrAccidentalStyleKind::kAccidentalStyleNeoModernCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["neo-modern-voice"] =
    lpsrAccidentalStyleKind::kAccidentalStyleNeoModernVoice;
  gGlobalLpsrAccidentalStyleKindsMap ["neo-modern-voice-cautionary"] =
    lpsrAccidentalStyleKind::kAccidentalStyleNeoModernVoiceCautionary;
  gGlobalLpsrAccidentalStyleKindsMap ["dodecaphonic"] =
    lpsrAccidentalStyleKind::kAccidentalStyleDodecaphonic;
  gGlobalLpsrAccidentalStyleKindsMap ["dodecaphonic-no-repeat"] =
    lpsrAccidentalStyleKind::kAccidentalStyleDodecaphonicNoRepeat;
  gGlobalLpsrAccidentalStyleKindsMap ["dodecaphonic-first"] =
    lpsrAccidentalStyleKind::kAccidentalStyleDodecaphonicFirst;
  gGlobalLpsrAccidentalStyleKindsMap ["teaching"] =
    lpsrAccidentalStyleKind::kAccidentalStyleTeaching;
  gGlobalLpsrAccidentalStyleKindsMap ["no-reset"] =
    lpsrAccidentalStyleKind::kAccidentalStyleNoReset;
  gGlobalLpsrAccidentalStyleKindsMap ["forget"] =
    lpsrAccidentalStyleKind::kAccidentalStyleForget;
}

std::string availableLpsrAccidentalStyleKinds (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    accidentalStyleKindsMapSize =
      gGlobalLpsrAccidentalStyleKindsMap.size ();

  if (accidentalStyleKindsMapSize) {
    size_t
      nextToLast =
        accidentalStyleKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, lpsrAccidentalStyleKind>::const_iterator i =
        gGlobalLpsrAccidentalStyleKindsMap.begin ();
      i != gGlobalLpsrAccidentalStyleKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != accidentalStyleKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// chords languages
//______________________________________________________________________________

std::map<std::string, lpsrChordsLanguageKind>
  gGlobalLpsrChordsLanguageKindsMap;

void initializeLpsrChordsLanguageKindsMap ()
{
  gGlobalLpsrChordsLanguageKindsMap ["ignatzek"] =
    lpsrChordsLanguageKind::kChordsIgnatzek; // default
  gGlobalLpsrChordsLanguageKindsMap ["german"] =
    lpsrChordsLanguageKind::kChordsGerman;
  gGlobalLpsrChordsLanguageKindsMap ["semiGerman"] =
    lpsrChordsLanguageKind::kChordsSemiGerman;
  gGlobalLpsrChordsLanguageKindsMap ["italian"] =
    lpsrChordsLanguageKind::kChordsItalian;
  gGlobalLpsrChordsLanguageKindsMap ["french"] =
    lpsrChordsLanguageKind::kChordsFrench;
}

std::string lpsrChordsLanguageKindAsString (
  lpsrChordsLanguageKind languageKind)
{
  std::string result;

  switch (languageKind) {
    case lpsrChordsLanguageKind::kChordsIgnatzek: // default value
      result = "kChordsIgnatzek";
      break;
    case lpsrChordsLanguageKind::kChordsGerman:
      result = "kChordsGerman";
      break;
    case lpsrChordsLanguageKind::kChordsSemiGerman:
      result = "kChordsSemiGerman";
      break;
    case lpsrChordsLanguageKind::kChordsItalian:
      result = "kChordsItalian";
      break;
    case lpsrChordsLanguageKind::kChordsFrench:
      result = "kChordsFrench";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrChordsLanguageKind& elt)
{
  os << lpsrChordsLanguageKindAsString (elt);
  return os;
}

std::string availableLpsrChordsLanguageKinds (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    lpsrChordsLanguageKindsMapSize =
      gGlobalLpsrChordsLanguageKindsMap.size ();

  if (lpsrChordsLanguageKindsMapSize) {
    size_t nextToLast =
      lpsrChordsLanguageKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, lpsrChordsLanguageKind>::const_iterator i =
        gGlobalLpsrChordsLanguageKindsMap.begin ();
      i != gGlobalLpsrChordsLanguageKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
        break;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != lpsrChordsLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
std::string msrSemiTonesPitchAndOctaveAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  S_msrSemiTonesPitchAndOctave       semiTonesPitchAndOctave)
{
  std::stringstream s;

  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchAndOctave->
          getSemiTonesPitchKind ());

  std::map<msrQuarterTonesPitchKind, std::string>*
    pitchesNamesMapPTR =
      &getNederlandsPitchesNamesMap (); // default LilyPond value

  // is quarterTonesPitchName in the part renaming map?
  switch (languageKind) {
    case msrQuarterTonesPitchesLanguageKind::kQTPNederlands:
      pitchesNamesMapPTR = &getNederlandsPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPCatalan:
      pitchesNamesMapPTR = &getCatalanPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPDeutsch:
      pitchesNamesMapPTR = &getDeutschPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEnglish:
      pitchesNamesMapPTR = &getEnglishPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEspanol:
      pitchesNamesMapPTR = &getEspanolPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPFrancais:
      pitchesNamesMapPTR = &getFrancaisPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPItaliano:
      pitchesNamesMapPTR = &getItalianoPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPNorsk:
      pitchesNamesMapPTR = &getNorskPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPPortugues:
      pitchesNamesMapPTR = &getPortuguesPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSuomi:
      pitchesNamesMapPTR = &getSuomiPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSvenska:
      pitchesNamesMapPTR = &getSvenskaPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPVlaams:
      pitchesNamesMapPTR = &getVlaamsPitchesNamesMap ();
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPArabic:
      pitchesNamesMapPTR = &getArabicPitchesNamesMap ();
      break;
  } // switch

  s << (*pitchesNamesMapPTR) [quarterTonesPitchKind];

  switch (semiTonesPitchAndOctave->getOctaveKind ()) {
    case msrOctaveKind::kOctave_UNKNOWN:
      s << "*** noOctave ***";
      break;
    case msrOctaveKind::kOctave0:
      s << ",,";
      break;
    case msrOctaveKind::kOctave1:
      s << ",";
      break;
    case msrOctaveKind::kOctave2:
      s << ",";
      break;
    case msrOctaveKind::kOctave3:
      s << "";
      break;
    case msrOctaveKind::kOctave4:
      s << "'";
      break;
    case msrOctaveKind::kOctave5:
      s << "''";
      break;
    case msrOctaveKind::kOctave6:
      s << "'''";
      break;
    case msrOctaveKind::kOctave7:
      s << "''''";
      break;
    case msrOctaveKind::kOctave8:
      s << "'''''";
      break;
    case msrOctaveKind::kOctave9:
      s << "''''''";
      break;
  } // switch

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    s <<
      " %{ " <<
      semiTonesPitchAndOctave->asString () <<
      " %}";
  }
#endif

  return s.str ();
}

/* JMI
std::string msrSemiTonesPitchAndOctaveAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  S_msrSemiTonesPitchAndOctave       semiTonesPitchAndOctave)
{
  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchKind);

  return
    msrQuarterTonesPitchAndOctaveAsLilypondString (
      languageKind,
      quarterTonesPitchKind);

  return "??????";
}
*/

// dynamics
//______________________________________________________________________________
std::map<std::string, lpsrDynamicsTextSpannersStyleKind>
  pLpsrDynamicsTextSpannersStyleKindsMap;

std::map<std::string, lpsrDynamicsTextSpannersStyleKind>&
  getLpsrDynamicsTextSpannersStyleKindsMap ()
{
  return pLpsrDynamicsTextSpannersStyleKindsMap;
}

std::string lpsrDynamicsTextSpannersStyleKindAsString (
  lpsrDynamicsTextSpannersStyleKind dynamicsTextSpannersStyleKind)
{
  std::string result;

  // no CamelCase here, these strings are used in the command line options

  switch (dynamicsTextSpannersStyleKind) {
    case lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleNone:
      result = "kDynamicsTextSpannersStyleNone";
      break;
    case lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleDashedLine:
      result = "kDynamicsTextSpannersStyleDashedLine";
      break;
    case lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleZigzag:
      result = "kDynamicsTextSpannersStyleZigzag";
      break;
    case lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleTrill:
      result = "kDynamicsTextSpannersStyleTrill";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrDynamicsTextSpannersStyleKind& elt)
{
  os << lpsrDynamicsTextSpannersStyleKindAsString (elt);
  return os;
}

lpsrDynamicsTextSpannersStyleKind lpsrDynamicsTextSpannersStyleKindFromString (
  int           inputLineNumber,
  const std::string& dynamicsTextSpannersStyleKindString)
{
  lpsrDynamicsTextSpannersStyleKind result =
    lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleNone;

  if      (dynamicsTextSpannersStyleKindString == "[NONE]")
    result =
      lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleNone;
  else if (dynamicsTextSpannersStyleKindString == "dashed-line")
    result =
      lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleDashedLine;
  else if (dynamicsTextSpannersStyleKindString == "zigzag")
    result =
      lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleZigzag;
  else if (dynamicsTextSpannersStyleKindString == "trill")
    result =
      lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleTrill;
  else {
    std::stringstream s;

    s <<
      "dynamics text spanners style string \"" <<
      dynamicsTextSpannersStyleKindString <<
      "\" is unknown" <<
      ", line: " << inputLineNumber;

    lpsrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void initializeLpsrDynamicsTextSpannersStyleKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  pLpsrDynamicsTextSpannersStyleKindsMap ["[NONE]"] =
    lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleNone;
  pLpsrDynamicsTextSpannersStyleKindsMap ["dashed-line"] =
    lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleDashedLine;
  pLpsrDynamicsTextSpannersStyleKindsMap ["zigzag"] =
    lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleZigzag;
  pLpsrDynamicsTextSpannersStyleKindsMap ["trill"] =
    lpsrDynamicsTextSpannersStyleKind::kDynamicsTextSpannersStyleTrill;
}

std::string availableLpsrDynamicsTextSpannersStyleKinds (
  size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    lpsrDynamicsTextSpannersStyleKindsMapSize =
      pLpsrDynamicsTextSpannersStyleKindsMap.size ();

  if (lpsrDynamicsTextSpannersStyleKindsMapSize) {
    size_t
      nextToLast =
        lpsrDynamicsTextSpannersStyleKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, lpsrDynamicsTextSpannersStyleKind>::const_iterator i =
        pLpsrDynamicsTextSpannersStyleKindsMap.begin ();
      i != pLpsrDynamicsTextSpannersStyleKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
        break;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != lpsrDynamicsTextSpannersStyleKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// lyrics durations
//______________________________________________________________________________

std::map<std::string, lpsrLyricsDurationsKind>
  gGlobalLpsrLyricsDurationsKindsMap;

std::string lpsrLyricsDurationsKindAsString (
  lpsrLyricsDurationsKind lyricsDurationsKind)
{
  std::string result;

  // no CamelCase here, these strings are used in the command line options

  switch (lyricsDurationsKind) {
    case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
      result = "implicit";
      break;
    case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
      result = "explicit";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrLyricsDurationsKind& elt)
{
  os << lpsrLyricsDurationsKindAsString (elt);
  return os;
}

void initializeLpsrLyricsDurationsKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalLpsrLyricsDurationsKindsMap ["implicit"] =
    lpsrLyricsDurationsKind::kLyricsDurationsImplicit;
  gGlobalLpsrLyricsDurationsKindsMap ["explicit"] =
    lpsrLyricsDurationsKind::kLyricsDurationsExplicit;
}

std::string availableLpsrLyricsDurationsKinds (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    lpsrLyricsDurationsKindsMapSize =
      gGlobalLpsrLyricsDurationsKindsMap.size ();

  if (lpsrLyricsDurationsKindsMapSize) {
    size_t
      nextToLast =
        lpsrLyricsDurationsKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, lpsrLyricsDurationsKind>::const_iterator i =
        gGlobalLpsrLyricsDurationsKindsMap.begin ();
      i != gGlobalLpsrLyricsDurationsKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
        break;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != lpsrLyricsDurationsKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}


}
