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
#include <cassert>

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, ...

#include <regex>

#include "mfRational.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"
#include "mfEnumAll.h"

#include "oahWae.h"
#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrPitchesNames.h"

#include "msrBasicTypes.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "lpsrOah.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

// input line numbers
//______________________________________________________________________________

const int K_NO_INPUT_LINE_NUMBER = 0;

// names lists max length
// ------------------------------------------------------

const int K_NAMES_LIST_MAX_LENGTH = 50;

// XMLLang
//______________________________________________________________________________
msrXMLLangKind msrXMLLangKindFromString (
  int           inputLineNumber,
  const string& XMLLangString)
{
  msrXMLLangKind
    result = msrXMLLangKind::kXMLLangIt; // default value

  if      (XMLLangString == "it")
    result = msrXMLLangKind::kXMLLangIt;
  else if (XMLLangString == "en")
    result = msrXMLLangKind::kXMLLangEn;
  else if (XMLLangString == "de")
    result = msrXMLLangKind::kXMLLangDe;
  else if (XMLLangString == "fr")
    result = msrXMLLangKind::kXMLLangFr;
  else if (XMLLangString == "ja")
    result = msrXMLLangKind::kXMLLangJa;
  else if (XMLLangString == "la")
    result = msrXMLLangKind::kXMLLangLa;
  else {
    if (XMLLangString.size ()) {
      stringstream s;

      s <<
        "xml:lang value '" << XMLLangString <<
        "' should be 'it', 'en', 'de' or 'fr'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrXMLLangKindAsString (
  msrXMLLangKind XMLLangKind)
{
  string result;

  switch (XMLLangKind) {
    case msrXMLLangKind::kXMLLangIt:
      result = "it";
      break;
    case msrXMLLangKind::kXMLLangEn:
      result = "en";
      break;
    case msrXMLLangKind::kXMLLangDe:
      result = "de";
      break;
    case msrXMLLangKind::kXMLLangFr:
      result = "fr";
      break;
    case msrXMLLangKind::kXMLLangJa:
      result = "ja";
      break;
    case msrXMLLangKind::kXMLLangLa:
      result = "la";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrXMLLangKind& elt)
{
  os << msrXMLLangKindAsString (elt);
  return os;
}

// duration
//______________________________________________________________________________
msrDuration::msrDuration ()
{
  fDurationKind = msrDurationKind::k_NoDuration;
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

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    print (gLogStream);

    gLogStream <<
      "=== dottedDurationAsWholeNotes()" <<
      ", (int) fDurationKind: " << (int) fDurationKind <<
      ", result: " << result <<
      endl;
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

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "=== dottedDurationAsWholeNotes()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      ", increment: " << increment <<
      endl;
  }
#endif

      --dots;
    } // while
  }

  return result;
}

Rational msrDuration::dottedDurationAsWholeNotes_FOR_TEMPO (
  // used in lpsrBasicTypes, dottedDurationAsLilypondStringWithoutBackSlash(),
  // called in lpsr2lilypondTranslator.cpp, visitStart (S_msrTempo& elt)
  // JMI BUGGY, NEVER TESTED TEMP???
  int inputLineNumber) const
{
  // convert duration into whole notes
  Rational
    result =
      msrDurationKindAsWholeNotes (
        fDurationKind);

#ifdef TRACING_IS_ENABLED
  if (false) {
    gLogStream <<
      "=== dottedDurationAsWholeNotes_FOR_TEMPO()" <<
      ", result: " << result <<
      "\"" <<
      endl;
  }
#endif

  // take dots into account if any
  if (fDotsNumber > 0) {
    int dots = fDotsNumber;

    while (dots > 0) {
      result *=
        Rational (3, 2);

#ifdef TRACING_IS_ENABLED
  if (false) {
    gLogStream <<
      "=== dottedDurationAsWholeNotes_FOR_TEMPO()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      "\"" <<
      endl;
  }
#endif

      --dots;
    } // while
  }

  return result;
}

string msrDuration::asString () const
{
  stringstream s;

  s <<
     msrDurationKindAsString (fDurationKind);

  for (int i = 1; i <= fDotsNumber; ++i) {
    s << ".";
  } // for

  return s.str ();
}

void msrDuration::print (ostream& os) const
{
  const int fieldWidth = 11;

  os << left <<
    setw (fieldWidth) <<
    "durationKind" << " : " <<
    msrDurationKindAsString (fDurationKind) <<
    endl <<
    setw (fieldWidth) <<
    "dotsNumber" << " : " << fDotsNumber <<
    endl;
};

ostream& operator << (ostream& os, const msrDuration& elt)
{
  os << elt.asString ();
  return os;
}

// dotted durations
//______________________________________________________________________________
msrDottedDuration::msrDottedDuration ()
{
  fDurationKind = msrDurationKind::k_NoDuration;
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

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    print (gLogStream);

    gLogStream <<
      "=== dottedDurationAsWholeNotes()" <<
      ", (int) fDurationKind: " << (int) fDurationKind <<
      ", result: " << result <<
      endl;
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

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "=== dottedDurationAsWholeNotes()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      ", increment: " << increment <<
      endl;
  }
#endif

      --dots;
    } // while
  }

  return result;
}

Rational msrDottedDuration::dottedDurationAsWholeNotes_FOR_TEMPO (
  // used in lpsrBasicTypes, dottedDurationAsLilypondStringWithoutBackSlash(),
  // called in lpsr2lilypondTranslator.cpp, visitStart (S_msrTempo& elt)
  // JMI BUGGY, NEVER TESTED TEMP???
  int inputLineNumber) const
{
  // convert duration into whole notes
  Rational
    result =
      msrDurationKindAsWholeNotes (
        fDurationKind);

#ifdef TRACING_IS_ENABLED
  if (false) {
    gLogStream <<
      "=== dottedDurationAsWholeNotes_FOR_TEMPO()" <<
      ", result: " << result <<
      "\"" <<
      endl;
  }
#endif

  // take dots into account if any
  if (fDotsNumber > 0) {
    int dots = fDotsNumber;

    while (dots > 0) {
      result *=
        Rational (3, 2);

#ifdef TRACING_IS_ENABLED
  if (false) {
    gLogStream <<
      "=== dottedDurationAsWholeNotes_FOR_TEMPO()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      "\"" <<
      endl;
  }
#endif

      --dots;
    } // while
  }

  return result;
}

string msrDottedDuration::asString () const
{
  stringstream s;

  s <<
     msrDurationKindAsString (fDurationKind);

  for (int i = 1; i <= fDotsNumber; ++i) {
    s << ".";
  } // for

  return s.str ();
}

void msrDottedDuration::print (ostream& os) const
{
  const int fieldWidth = 11;

  os << left <<
    setw (fieldWidth) <<
    "durationKind" << " : " <<
    msrDurationKindAsString (fDurationKind) <<
    endl <<
    setw (fieldWidth) <<
    "dotsNumber" << " : " << fDotsNumber <<
    endl;
};

ostream& operator << (ostream& os, const msrDottedDuration& elt)
{
  os << elt.asString ();
  return os;
}

// octaves
//______________________________________________________________________________
EXP int octaveNumberFromOctaveKind (msrOctaveKind octaveKind)
{
  int result = -2;

  switch (octaveKind) {
    case msrOctaveKind::k_NoOctave:
      result = -1;
      break;
    case msrOctaveKind::kOctave0:
      result = 0;
      break;
    case msrOctaveKind::kOctave1:
      result = 1;
      break;
    case msrOctaveKind::kOctave2:
      result = 2;
      break;
    case msrOctaveKind::kOctave3:
      result = 3;
      break;
    case msrOctaveKind::kOctave4:
      result = 4;
      break;
    case msrOctaveKind::kOctave5:
      result = 5;
      break;
    case msrOctaveKind::kOctave6:
      result = 6;
      break;
    case msrOctaveKind::kOctave7:
      result = 7;
      break;
    case msrOctaveKind::kOctave8:
      result = 8;
      break;
    case msrOctaveKind::kOctave9:
      result = 9;
      break;
  } // switch

  return result;
}

msrOctaveKind octaveSucc (msrOctaveKind octaveKind)
{
  msrOctaveKind result = msrOctaveKind::k_NoOctave;

  switch (octaveKind) {
    case msrOctaveKind::k_NoOctave:
      result = msrOctaveKind::k_NoOctave;
      break;
    case msrOctaveKind::kOctave0:
      result = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave1:
      result = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave2:
      result = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave3:
      result = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave4:
      result = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave5:
      result = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave6:
      result = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave7:
      result = msrOctaveKind::kOctave8;
      break;
    case msrOctaveKind::kOctave8:
      result = msrOctaveKind::kOctave9;
      break;
    case msrOctaveKind::kOctave9:
      result = msrOctaveKind::k_NoOctave;
      break;
  } // switch

  return result;
}

msrOctaveKind octavePred (msrOctaveKind octaveKind)
{
  msrOctaveKind result = msrOctaveKind::k_NoOctave;

  switch (octaveKind) {
    case msrOctaveKind::k_NoOctave:
      result = msrOctaveKind::k_NoOctave;
      break;
    case msrOctaveKind::kOctave0:
      result = msrOctaveKind::k_NoOctave;
      break;
    case msrOctaveKind::kOctave1:
      result = msrOctaveKind::kOctave0;
      break;
    case msrOctaveKind::kOctave2:
      result = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave3:
      result = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave4:
      result = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave5:
      result = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave6:
      result = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave7:
      result = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave8:
      result = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave9:
      result = msrOctaveKind::kOctave8;
      break;
  } // switch

  return result;
}

// prefix operators
msrOctaveKind& operator++ (msrOctaveKind& octaveKind)
{
  octaveKind = octaveSucc (octaveKind);

  return octaveKind;
}

msrOctaveKind& operator-- (msrOctaveKind& octaveKind)
{
  octaveKind = octavePred (octaveKind);

  return octaveKind;
}

// postfix operators
msrOctaveKind operator++ (msrOctaveKind& octaveKind, int)
{
  msrOctaveKind originalValue = octaveKind;

  octaveKind = octaveSucc (originalValue);

  return originalValue;
}

msrOctaveKind operator-- (msrOctaveKind& octaveKind, int)
{
  msrOctaveKind originalValue = octaveKind;

  octaveKind = octavePred (originalValue);

  return originalValue;
}

msrOctaveKind msrOctaveKindFromNumber (
  int inputLineNumber,
  int octaveNumber)
{
  msrOctaveKind result = msrOctaveKind::k_NoOctave;

  switch (octaveNumber) {
    case 0: result = msrOctaveKind::kOctave0; break;
    case 1: result = msrOctaveKind::kOctave1; break;
    case 2: result = msrOctaveKind::kOctave2; break;
    case 3: result = msrOctaveKind::kOctave3; break;
    case 4: result = msrOctaveKind::kOctave4; break;
    case 5: result = msrOctaveKind::kOctave5; break;
    case 6: result = msrOctaveKind::kOctave6; break;
    case 7: result = msrOctaveKind::kOctave7; break;
    case 8: result = msrOctaveKind::kOctave8; break;
    case 9: result = msrOctaveKind::kOctave9; break;
    default:
      {
        stringstream s;

        s <<
          "cannot create an octave kind from number '" <<
          octaveNumber <<
          "'";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
  } // switch

  return result;
}

msrOctaveKind msrOctaveKindFromCommasOrQuotes (
  int           inputLineNumber,
  const string& octaveIndication)
{
  /*
    octaveIndication should containt a possibly empty
    sequence of ','s or '\''s

    Middle C, LilyPond's c', starts octave 4,
    thus a pitch without any commas nor quotes is in octave 3
  */

  const msrOctaveKind
    octaveKindBelowMiddleC =
      msrOctaveKind::kOctave3;

  msrOctaveKind
    result =
      octaveKindBelowMiddleC;

  for (size_t i = 0; i < octaveIndication.size (); ++i) {
    switch (octaveIndication [i]) {
      case ',':
        if (result > octaveKindBelowMiddleC) {
          // a '\'' has been found previously
          stringstream s;

          s <<
            "octave indication \"" << octaveIndication <<
            "\" contains a ',' after a '\\'";

          oahError (s.str ());
        }

        --result;
        break;

      case '\'':
        if (result < octaveKindBelowMiddleC) {
          // a ',' has been found previously
          stringstream s;

          s <<
            "octave indication \"" << octaveIndication <<
            "\" contains a '\\'' after a ','";

          oahError (s.str ());
        }

        ++result;
        break;

      default:
        {
          stringstream s;

          s <<
            "octave indication \"" <<
            octaveIndication <<
            "\" should contain only commas and quotes" <<
            ", line = " << inputLineNumber;

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
    } // switch

//     gLogStream << JMI
//       "---> result: " <<
//       result <<
//       endl;
  } // for

  return result;
}

string msrOctaveKindAsString (msrOctaveKind octaveKind)
{
  string result;

  switch (octaveKind) {
    case msrOctaveKind::k_NoOctave:
      result = "*** noOctave ***]";
      break;
    case msrOctaveKind::kOctave0:
      result = "o0";
      break;
    case msrOctaveKind::kOctave1:
      result = "o1";
      break;
    case msrOctaveKind::kOctave2:
      result = "o2";
      break;
    case msrOctaveKind::kOctave3:
      result = "o3";
      break;
    case msrOctaveKind::kOctave4:
      result = "o4";
      break;
    case msrOctaveKind::kOctave5:
      result = "o5";
      break;
    case msrOctaveKind::kOctave6:
      result = "o6";
      break;
    case msrOctaveKind::kOctave7:
      result = "o7";
      break;
    case msrOctaveKind::kOctave8:
      result = "o8";
      break;
    case msrOctaveKind::kOctave9:
      result = "o9";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrOctaveKind& elt)
{
  os << msrOctaveKindAsString (elt);
  return os;
}

// octave entry kinds
//______________________________________________________________________________

map<string, msrOctaveEntryKind>
  gGlobalMsrOctaveEntryKindsMap;

string msrOctaveEntryKindAsString (
  msrOctaveEntryKind octaveEntryKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (octaveEntryKind) {
    case msrOctaveEntryKind::kOctaveEntryRelative: // default value
      result = "kOctaveEntryRelative";
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      result = "kOctaveEntryAbsolute";
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      result = "kOctaveEntryFixed";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrOctaveEntryKind& elt)
{
  os << msrOctaveEntryKindAsString (elt);
  return os;
}

void initializeMsrOctaveEntryKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalMsrOctaveEntryKindsMap ["relative"] = msrOctaveEntryKind::kOctaveEntryRelative;
  gGlobalMsrOctaveEntryKindsMap ["absolute"] = msrOctaveEntryKind::kOctaveEntryAbsolute;
  gGlobalMsrOctaveEntryKindsMap ["fixed"] = msrOctaveEntryKind::kOctaveEntryFixed;
}

string existingMsrOctaveEntryKinds (size_t namesListMaxLength)
{
  stringstream s;

  size_t
    msrOctaveEntryKindsMapSize =
      gGlobalMsrOctaveEntryKindsMap.size ();

  if (msrOctaveEntryKindsMapSize) {
    size_t
      nextToLast =
        msrOctaveEntryKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      map<string, msrOctaveEntryKind>::const_iterator i =
        gGlobalMsrOctaveEntryKindsMap.begin ();
      i != gGlobalMsrOctaveEntryKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != msrOctaveEntryKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// semitone pitches and absolute octave
//______________________________________________________________________________
S_msrSemiTonesPitchAndOctave msrSemiTonesPitchAndOctave::create (
  msrSemiTonesPitchKind semiTonesPitchKind,
  msrOctaveKind         octaveKind)
{
  msrSemiTonesPitchAndOctave* o =
    new msrSemiTonesPitchAndOctave (
      semiTonesPitchKind,
      octaveKind);
  assert (o != nullptr);

  return o;
}

msrSemiTonesPitchAndOctave::msrSemiTonesPitchAndOctave (
  msrSemiTonesPitchKind semiTonesPitchKind,
  msrOctaveKind         octaveKind)
{
  fSemiTonesPitchKind = semiTonesPitchKind;
  fOctaveKind         = octaveKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating pitch and octave '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrSemiTonesPitchAndOctave::~msrSemiTonesPitchAndOctave ()
{}

S_msrSemiTonesPitchAndOctave msrSemiTonesPitchAndOctave::createSemiTonesPitchAndOctaveNewbornClone ()
{
  S_msrSemiTonesPitchAndOctave
    newbornClone =
      msrSemiTonesPitchAndOctave::create (
        fSemiTonesPitchKind,
        fOctaveKind);

  return newbornClone;
}

S_msrSemiTonesPitchAndOctave msrSemiTonesPitchAndOctave::createFromString (
  int           inputLineNumber,
  const string& theString)
{
  S_msrSemiTonesPitchAndOctave result;

  // decipher theString
  string regularExpression (
    "([[:lower:]]+)" // pitch
    "([,\']*)"       // octaveIndication
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for semitones pitch and octave string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          '[' << sm [i] << "] ";
      } // for

      gLogStream << endl;
    }
#endif

  if (smSize == 3) {
    // found a well-formed specification,
    // need to check its ',' and '\'' contents
    string
      pitch            = sm [1],
      octaveIndication = sm [2];

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "--> pitch = \"" << pitch << "\", " <<
        "--> octaveIndication = \"" << octaveIndication << "\"" <<
        endl;
    }
#endif

    // fetch semitones pitch
    msrSemiTonesPitchKind
      semiTonesPitchKind =
        semiTonesPitchKindFromString (
          pitch);

    // compute the octave from octaveIndication
    msrOctaveKind
      octaveKind =
        msrOctaveKindFromCommasOrQuotes (
          inputLineNumber,
          octaveIndication);

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "--> semiTonesPitchKind = \"" <<
        msrSemiTonesPitchKindAsString (
          semiTonesPitchKind) << "\", " <<
        "--> octaveKind = " <<
        msrOctaveKindAsString (octaveKind) <<
        endl;
    }
#endif

    // create the semiTonesPitchAndOctave
    result =
      msrSemiTonesPitchAndOctave::create (
       semiTonesPitchKind,
       octaveKind);
  }

  else {
    stringstream s;

    s <<
      "semitones pitch and octave argument '" << theString <<
      "' is ill-formed";

    msrError (
//    msrWarning ( //  JMI
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void msrSemiTonesPitchAndOctave::incrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::k_NoOctave:
      fOctaveKind = msrOctaveKind::k_NoOctave;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave1:
      fOctaveKind = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave2:
      fOctaveKind = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave3:
      fOctaveKind = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave4:
      fOctaveKind = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave5:
      fOctaveKind = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave6:
      fOctaveKind = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave7:
      fOctaveKind = msrOctaveKind::kOctave8;
      break;
    case msrOctaveKind::kOctave8:
      fOctaveKind = msrOctaveKind::kOctave9;
      break;
    case msrOctaveKind::kOctave9:
      fOctaveKind = msrOctaveKind::k_NoOctave;
      break;
  } // switch
}

void msrSemiTonesPitchAndOctave::decrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::k_NoOctave:
      fOctaveKind = msrOctaveKind::k_NoOctave;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::k_NoOctave;
      break;
    case msrOctaveKind::kOctave1:
      fOctaveKind = msrOctaveKind::kOctave0;
      break;
    case msrOctaveKind::kOctave2:
      fOctaveKind = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave3:
      fOctaveKind = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave4:
      fOctaveKind = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave5:
      fOctaveKind = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave6:
      fOctaveKind = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave7:
      fOctaveKind = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave8:
      fOctaveKind = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave9:
      fOctaveKind = msrOctaveKind::kOctave8;
      break;
  } // switch
}

string msrSemiTonesPitchAndOctave::asString () const
{
  stringstream s;

  s << left <<
    "[SemiTonesPitchAndOctave" <<
    ": " <<
    "semiTonesPitchKind: " <<
    msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    ", octave: " <<
    msrOctaveKindAsString (fOctaveKind) <<
    ']';

  return s.str ();
}

void msrSemiTonesPitchAndOctave::print (ostream& os) const
{
  os <<
    "SemiTonesPitchAndOctave" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "fSemiTonesPitchKind" << " : " <<
      msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "fOctaveKind: " << " : " <<
    msrOctaveKindAsString (fOctaveKind) <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrSemiTonesPitchAndOctave& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

// semitone pitches and absolute octave
//______________________________________________________________________________
S_msrQuarterTonesPitchAndOctave msrQuarterTonesPitchAndOctave::create (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrOctaveKind            octaveKind)
{
  msrQuarterTonesPitchAndOctave* o =
    new msrQuarterTonesPitchAndOctave (
      quarterTonesPitchKind,
      octaveKind);
  assert (o != nullptr);

  return o;
}

msrQuarterTonesPitchAndOctave::msrQuarterTonesPitchAndOctave (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrOctaveKind            octaveKind)
{
  fQuarterTonesPitchKind = quarterTonesPitchKind;
  fOctaveKind            = octaveKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating pitch and octave '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrQuarterTonesPitchAndOctave::~msrQuarterTonesPitchAndOctave ()
{}

S_msrQuarterTonesPitchAndOctave msrQuarterTonesPitchAndOctave::createQuarterTonesPitchAndOctaveNewbornClone ()
{
  S_msrQuarterTonesPitchAndOctave
    newbornClone =
      msrQuarterTonesPitchAndOctave::create (
        fQuarterTonesPitchKind,
        fOctaveKind);

  return newbornClone;
}

S_msrQuarterTonesPitchAndOctave msrQuarterTonesPitchAndOctave::createFromString (
  int           inputLineNumber,
  const string& theString)
{
  S_msrQuarterTonesPitchAndOctave result;

  // decipher theString
  string regularExpression (
    "([[:lower:]]+)" // pitch
    "([,\']*)"       // octaveIndication
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for quartertones pitch and octave string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl <<
      smSize << " elements: ";

      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          '[' << sm [i] << "] ";
      } // for

      gLogStream << endl;
    }
#endif

  if (smSize == 3) {
    // found a well-formed specification,
    // need to check its ',' and '\'' contents
    string
      pitch            = sm [1],
      octaveIndication = sm [2];

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "--> pitch = \"" << pitch << "\", " <<
        "--> octaveIndication = \"" << octaveIndication << "\"" <<
        endl;
    }
#endif

    // fetch quartertones pitch
    msrQuarterTonesPitchKind
      quarterTonesPitchKind =
        quarterTonesPitchKindFromString (
          gGlobalMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind (),
          pitch);

    // compute the octave from octaveIndication
    msrOctaveKind
      octaveKind =
        msrOctaveKindFromCommasOrQuotes (
          inputLineNumber,
          octaveIndication);

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "--> quarterTonesPitchKind = \"" <<
          msrQuarterTonesPitchKindAsString (
            quarterTonesPitchKind) <<
        "\", " <<
        "--> octaveKind = " << msrOctaveKindAsString (octaveKind) <<
        endl;
    }
#endif

    // create the quarterTonesPitchAndOctave
    result =
      msrQuarterTonesPitchAndOctave::create (
       quarterTonesPitchKind,
       octaveKind);
  }

  else {
    stringstream s;

    s <<
      "quartertones pitch and octave argument '" << theString <<
      "' is ill-formed";

    msrError (
//    msrWarning ( //  JMI
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void msrQuarterTonesPitchAndOctave::incrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::k_NoOctave:
      fOctaveKind = msrOctaveKind::k_NoOctave;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave1:
      fOctaveKind = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave2:
      fOctaveKind = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave3:
      fOctaveKind = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave4:
      fOctaveKind = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave5:
      fOctaveKind = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave6:
      fOctaveKind = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave7:
      fOctaveKind = msrOctaveKind::kOctave8;
      break;
    case msrOctaveKind::kOctave8:
      fOctaveKind = msrOctaveKind::kOctave9;
      break;
    case msrOctaveKind::kOctave9:
      fOctaveKind = msrOctaveKind::k_NoOctave;
      break;
  } // switch
}

void msrQuarterTonesPitchAndOctave::decrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::k_NoOctave:
      fOctaveKind = msrOctaveKind::k_NoOctave;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::k_NoOctave;
      break;
    case msrOctaveKind::kOctave1:
      fOctaveKind = msrOctaveKind::kOctave0;
      break;
    case msrOctaveKind::kOctave2:
      fOctaveKind = msrOctaveKind::kOctave1;
      break;
    case msrOctaveKind::kOctave3:
      fOctaveKind = msrOctaveKind::kOctave2;
      break;
    case msrOctaveKind::kOctave4:
      fOctaveKind = msrOctaveKind::kOctave3;
      break;
    case msrOctaveKind::kOctave5:
      fOctaveKind = msrOctaveKind::kOctave4;
      break;
    case msrOctaveKind::kOctave6:
      fOctaveKind = msrOctaveKind::kOctave5;
      break;
    case msrOctaveKind::kOctave7:
      fOctaveKind = msrOctaveKind::kOctave6;
      break;
    case msrOctaveKind::kOctave8:
      fOctaveKind = msrOctaveKind::kOctave7;
      break;
    case msrOctaveKind::kOctave9:
      fOctaveKind = msrOctaveKind::kOctave8;
      break;
  } // switch
}

string msrQuarterTonesPitchAndOctave::asString () const
{
  stringstream s;

  s << left <<
    "[QuarterTonesPitchAndOctave" <<
    ": " <<
    "fQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsString (
      fQuarterTonesPitchKind) <<
    ", fOctaveKind: " << msrOctaveKindAsString (fOctaveKind) <<
    ']';

  return s.str ();
}

void msrQuarterTonesPitchAndOctave::print (ostream& os) const
{
  os <<
    "QuarterTonesPitchAndOctave" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "quarterTonesPitchKind" << " : " <<
      msrQuarterTonesPitchKindAsString (fQuarterTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "octave: " << " : " <<
    msrOctaveKindAsString (fOctaveKind) <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrQuarterTonesPitchAndOctave& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

// durations
//______________________________________________________________________________
msrDurationKind msrDurationKindFromMusicXMLString (
  int           inputLineNumber,
  const string& durationString)
{
  msrDurationKind result = msrDurationKind::k_NoDuration;

  if      (durationString == "maxima") {
    result = msrDurationKind::kMaxima;
  }
  else if (durationString == "long") {
    result = msrDurationKind::kLonga;
  }
  else if (durationString == "breve") {
    result = msrDurationKind::kBreve;
  }
  else if (durationString == "whole") {
    result = msrDurationKind::kWhole;
  }
  else if (durationString == "half") {
    result = msrDurationKind::kHalf;
  }
  else if (durationString == "quarter") {
    result = msrDurationKind::kQuarter;
  }
  else if (durationString == "eighth") {
    result = msrDurationKind::kEighth;
  }
  else if (durationString == "16th") {
    result = msrDurationKind::k16th;
  }
  else if (durationString == "32nd") {
    result = msrDurationKind::k32nd;
  }
  else if (durationString == "64th") {
    result = msrDurationKind::k64th;
  }
  else if (durationString == "128th") {
    result = msrDurationKind::k128th;
  }
  else if (durationString == "256th") {
    result = msrDurationKind::k256th;
  }
  else if (durationString == "512th") {
    result = msrDurationKind::k512th;
  }
  else if (durationString == "1024th") {
    result = msrDurationKind::k1024th;
  }
  else {
    stringstream s;

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
  msrDurationKind result = msrDurationKind::k_NoDuration;

  switch (durationInteger) {
    case 1:
      result = msrDurationKind::kWhole;
      break;
    case 2:
      result = msrDurationKind::kHalf;
      break;
    case 4:
      result = msrDurationKind::kQuarter;
      break;
    case 8:
      result = msrDurationKind::kEighth;
      break;
    case 16:
      result = msrDurationKind::k16th;
      break;
    case 32:
      result = msrDurationKind::k32nd;
      break;
    case 64:
      result = msrDurationKind::k64th;
      break;
    case 128:
      result = msrDurationKind::k128th;
      break;
    case 256:
      result = msrDurationKind::k256th;
      break;
    case 512:
      result = msrDurationKind::k512th;
      break;
    case 1024:
      result = msrDurationKind::k1024th;
      break;
    default:
      ;
  } // switch

  return result;
}

msrDurationKind msrDurationKindFromString (
  int           inputLineNumber,
  const string& durationString)
{
  msrDurationKind result = msrDurationKind::k_NoDuration;

  if      (durationString == "maxima") {
    result = msrDurationKind::kMaxima;
  }
  else if (durationString == "long") {
    result = msrDurationKind::kLonga;
  }
  else if (durationString == "breve") {
    result = msrDurationKind::kBreve;
  }
  else if (durationString == "1") {
    result = msrDurationKind::kWhole;
  }
  else if (durationString == "2") {
    result = msrDurationKind::kHalf;
  }
  else if (durationString == "4") {
    result = msrDurationKind::kQuarter;
  }
  else if (durationString == "8") {
    result = msrDurationKind::kEighth;
  }
  else if (durationString == "16") {
    result = msrDurationKind::k16th;
  }
  else if (durationString == "32") {
    result = msrDurationKind::k32nd;
  }
  else if (durationString == "64") {
    result = msrDurationKind::k64th;
  }
  else if (durationString == "128") {
    result = msrDurationKind::k128th;
  }
  else if (durationString == "256") {
    result = msrDurationKind::k256th;
  }
  else if (durationString == "512") {
    result = msrDurationKind::k512th;
  }
  else if (durationString == "1024") {
    result = msrDurationKind::k1024th;
  }
  else {
    stringstream s;

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

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "=== rationalFromDurationKindAndDotsNumber()" <<
      ", (int) durationKind: " << (int) durationKind <<
      ", result: " << result <<
      endl;
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

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "=== rationalFromDurationKindAndDotsNumber()" <<
      ", dots: " << dots <<
      ", result: " << result <<
      ", increment: " << increment <<
      endl;
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
    case msrDurationKind::k_NoDuration:
      result = Rational (0, 1);
      break;

    case msrDurationKind::k1024th:
      result = Rational (1, 1024);
      break;
    case msrDurationKind::k512th:
      result = Rational (1, 512);
      break;
    case msrDurationKind::k256th:
      result = Rational (1, 256);
      break;
    case msrDurationKind::k128th:
      result = Rational (1, 128);
      break;
    case msrDurationKind::k64th:
      result = Rational (1, 64);
      break;
    case msrDurationKind::k32nd:
      result = Rational (1, 32);
      break;
    case msrDurationKind::k16th:
      result = Rational (1, 16);
      break;
    case msrDurationKind::kEighth:
      result = Rational (1, 8);
      break;
    case msrDurationKind::kQuarter:
      result = Rational (1, 4);
      break;
    case msrDurationKind::kHalf:
      result = Rational (1, 2);
      break;
    case msrDurationKind::kWhole:
      result = Rational (1, 1);
      break;
    case msrDurationKind::kBreve:
      result = Rational (2, 1);
      break;
    case msrDurationKind::kLonga:
      result = Rational (4, 1);
      break;
    case msrDurationKind::kMaxima:
      result = Rational (8, 1);
      break;
  } // switch

  return result;
}

msrDurationKind wholeNotesAsDurationKind (Rational wholeNotes)
{
  msrDurationKind result = msrDurationKind::k_NoDuration;

  if (wholeNotes.getNumerator () == 1) {
    switch (wholeNotes.getDenominator ()) {
      case 1:
        result = msrDurationKind::kWhole;
        break;
      case 2:
        result = msrDurationKind::kHalf;
        break;
      case 4:
        result = msrDurationKind::kQuarter;
        break;
      case 8:
        result = msrDurationKind::kEighth;
        break;
      case 16:
        result = msrDurationKind::k16th;
        break;
      case 32:
        result = msrDurationKind::k32nd;
        break;
      case 64:
        result = msrDurationKind::k64th;
        break;
      case 128:
        result = msrDurationKind::k128th;
        break;
      case 256:
        result = msrDurationKind::k256th;
        break;
      case 512:
        result = msrDurationKind::k512th;
        break;
      case 1024:
        result = msrDurationKind::k1024th;
        break;
      default:
        ;
    } // switch
  }

  else if (wholeNotes.getDenominator () == 1) {
    switch (wholeNotes.getNumerator ()) {
      case 2:
        result = msrDurationKind::kBreve;
        break;
      case 4:
        result = msrDurationKind::kLonga;
        break;
      case 8:
        result = msrDurationKind::kMaxima;
        break;
      default:
        ;
    } // switch
  }

  return result;
}

string msrDurationKindAsMusicXMLType (msrDurationKind durationKind)
{
  string result;

  switch (durationKind) {
    case msrDurationKind::k_NoDuration:
      result = "noDuration";
      break;

    case msrDurationKind::k1024th:
      result = "1024th";
      break;
    case msrDurationKind::k512th:
      result = "512th";
      break;
    case msrDurationKind::k256th:
      result = "256th";
      break;
    case msrDurationKind::k128th:
      result = "128th";
      break;
    case msrDurationKind::k64th:
      result = "64th";
      break;
    case msrDurationKind::k32nd:
      result = "32nd";
      break;
    case msrDurationKind::k16th:
      result = "16th";
      break;
    case msrDurationKind::kEighth:
      result = "eighth";
      break;
    case msrDurationKind::kQuarter:
      result = "quarter";
      break;
    case msrDurationKind::kHalf:
      result = "half";
      break;
    case msrDurationKind::kWhole:
      result = "whole";
      break;
    case msrDurationKind::kBreve:
      result = "breve";
      break;
    case msrDurationKind::kLonga:
      result = "long";
      break;
    case msrDurationKind::kMaxima:
      result = "maxima";
      break;
  } // switch

  return result;
}

string msrDurationKindAsString_INTERNAL (msrDurationKind durationKind) // JMI don't keep ???
{
  string result;

  switch (durationKind) {
    case msrDurationKind::k_NoDuration:
      result = "k_NoDuration";
      break;

    case msrDurationKind::k1024th:
      result = "k1024th";
      break;
    case msrDurationKind::k512th:
      result = "k512th";
      break;
    case msrDurationKind::k256th:
      result = "k256th";
      break;
    case msrDurationKind::k128th:
      result = "k128th";
      break;
    case msrDurationKind::k64th:
      result = "k64th";
      break;
    case msrDurationKind::k32nd:
      result = "k32nd";
      break;
    case msrDurationKind::k16th:
      result = "k16th";
      break;
    case msrDurationKind::kEighth:
      result = "kEighth";
      break;
    case msrDurationKind::kQuarter:
      result = "kQuarter";
      break;
    case msrDurationKind::kHalf:
      result = "kHalf";
      break;
    case msrDurationKind::kWhole:
      result = "kWhole";
      break;
    case msrDurationKind::kBreve:
      result = "kBreve";
      break;
    case msrDurationKind::kLonga:
      result = "kLonga";
      break;
    case msrDurationKind::kMaxima:
      result = "kMaxima";
      break;
  } // switch

  return result;
}

string msrDurationKindAsString (msrDurationKind durationKind)
{
  string result;

  switch (durationKind) {
    case msrDurationKind::k_NoDuration:
      result = "???NoDuration???";
      break;

    case msrDurationKind::k1024th:
      result = "1024";
      break;
    case msrDurationKind::k512th:
      result = "512";
      break;
    case msrDurationKind::k256th:
      result = "256";
      break;
    case msrDurationKind::k128th:
      result = "128";
      break;
    case msrDurationKind::k64th:
      result = "64";
      break;
    case msrDurationKind::k32nd:
      result = "32nd";
      break;
    case msrDurationKind::k16th:
      result = "16";
      break;
    case msrDurationKind::kEighth:
      result = "8";
      break;
    case msrDurationKind::kQuarter:
      result = "4";
      break;
    case msrDurationKind::kHalf:
      result = "2";
      break;
    case msrDurationKind::kWhole:
      result = "1";
      break;
    case msrDurationKind::kBreve:
      result = "Breve";
      break;
    case msrDurationKind::kLonga:
      result = "Longa";
      break;
    case msrDurationKind::kMaxima:
      result = "Maxima";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrDurationKind& elt)
{
  os << msrDurationKindAsString (elt);
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

//_______________________________________________________________________________
string wholeNotesAsMsrString (
  int             inputLineNumber,
  const Rational& wholeNotes,
  int&            dotsNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> wholeNotesAsMsrString() 1 -------------------------------------" <<
      ", wholeNotes: " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  int
    numerator    = wholeNotes.getNumerator (),
    denominator  = wholeNotes.getDenominator ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numerator:   " << numerator <<
      endl <<
      "--> denominator: " << denominator <<
      endl << endl;
  }
#endif

  if (numerator == 0) { // JMI TEMP
    dotsNumber = 0;
    return "zero";
  }
  else if (numerator < 0) {
    // sanity check
    stringstream s;

    s <<
      "numerator is not positive in wholeNotesAsMsrString()" <<
      ", wholeNotes = " << wholeNotes;

 //   msrError ( JMI
    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
  //    __FILE__, __LINE__,
      s.str ());

    return "???";
  }

//  JMI ???
//
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
//     gLogStream <<
//       "--> wholeNotes rationalised: " << wholeNotes <<
//       endl;
//   }
// #endif
//
//   Bool
//     rationalHasBeenSimplified =
//       wholeNotes.getNumerator () != numerator; // denominators could be used too
//
//   if (rationalHasBeenSimplified) {
//     numerator    = wholeNotes.getNumerator (),
//     denominator  = wholeNotes.getDenominator ();
//   }

  Bool
    integralNumberOfWholeNotes = denominator == 1;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
//       "--> rationalHasBeenSimplified: " <<
//       rationalHasBeenSimplified <<
//       endl <<
      "--> integralNumberOfWholeNotes: " <<
      integralNumberOfWholeNotes <<
      endl << endl;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numeratorDots " << " : " << numeratorDots <<
      endl << endl;
  }
#endif

  /*
    valid denominators are powers of 2

    the Rational representing a dotted duration has to be brought
    to a value less than two, as explained above

    this is done by changing it denominator in the resulting string:

     whole notes        string
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
    string result;

    {
      stringstream s;

      s <<
        1 <<
        "*" <<
        numerator <<
        '/' <<
        denominator;

      result = s.str ();
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      stringstream s;

      s <<
        "denominator " << denominator <<
        " is no power of two between 1 and 128" <<
   //     " is no power of 2 between 1 and 1024" <<
        ", whole notes duration " <<
        numerator << '/' << denominator;
//
//       if (rationalHasBeenSimplified) {
//         s <<
//           " (" << numerator << '/' << denominator << ")" <<
//         endl;
//       }

      s <<
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> denominatorDurationLog" << " : " <<
      denominatorDurationLog <<
      endl << endl;
  }
#endif

  // bring the resulting fraction to be less that two if needed
  if (integralNumberOfWholeNotes) {
    // adapt the duration to avoid even numerators if can be,
    // since dotted durations cannot be recognized otherwise
    // 6/1 thus becomes 3 \breve, hence '\longa.'
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> integralNumberOfWholeNotes,"
        " bringing the faction to be less that 2" <<
        endl;
    }
#endif

    while (numerator % 2 == 0) {
      numerator /= 2;
      denominatorDurationLog -= 1;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
        gLogStream <<
          "--> numerator" << " : " <<
          numerator <<
          endl <<
          "--> denominatorDurationLog " << " : " <<
          denominatorDurationLog <<
          endl << endl;
      }
#endif
    } // while

    // update the number of dots
    numeratorDots = msrNumberOfDots (numerator);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numerator" << " : " <<
      numerator <<
      endl <<
      "--> denominatorDurationLog" << " : " <<
      denominatorDurationLog <<
      endl <<
      "--> numeratorDots " << " : " <<
      numeratorDots <<
      endl << endl;
  }
#endif

  // take care of the dots
  int multiplyingFactor = 1;

  if (numeratorDots >= 0 && denominatorDurationLog >= numeratorDots) {
    // take the dots into account
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> taking the dots into account" <<
        endl;
    }
#endif

    denominatorDurationLog -= numeratorDots;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> denominatorDurationLog" << " : " <<
        denominatorDurationLog <<
        endl <<
        "--> multiplyingFactor " << " : " <<
        multiplyingFactor <<
        endl << endl;
    }
#endif
  }
  else {
    // set the multiplying factor
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> setting the multiplying factor" <<
        endl;
    }
#endif

    // 5/8 becomes 8*5

    multiplyingFactor = numerator;
    numerator = 1;

    /* JMI
    multiplyingFactor = numerator;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
      gLogStream <<
        "--> denominatorDurationLog" << " : " <<
        denominatorDurationLog <<
        endl <<
        "--> multiplyingFactor " << " : " <<
        multiplyingFactor <<
        endl << endl;
    }
#endif

    while (multiplyingFactor >= 2) {
      // double duration
      --denominatorDurationLog;

      // adapt multiplying factor
      multiplyingFactor /= 2;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
        gLogStream <<
          "--> denominatorDurationLog" << " : " <<
          denominatorDurationLog <<
          endl <<
          "--> multiplyingFactor " << " : " <<
          multiplyingFactor <<
          endl << endl;
      }
#endif
    } // while
    */
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> numerator " << " : " <<
      numerator <<
      endl <<
      "--> numeratorDots " << " : " <<
      numeratorDots <<
      endl <<
      "--> denominatorDurationLog" << " : " <<
      denominatorDurationLog <<
      endl <<
      "--> multiplyingFactor " << " : " <<
      multiplyingFactor <<
      endl << endl;
  }
#endif

  // generate the code for the duration
  stringstream s;

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

  string result = s.str ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotesDetails ()) {
    gLogStream <<
      "--> wholeNotesAsMsrString() 2 -------------------------------------" <<
     ", result: \"" << result << "\"" <<
      ", numeratorDots" << " : " << numeratorDots <<
      endl;
  }
#endif

  // return the result
  dotsNumber = numeratorDots;

  return result;
}

string wholeNotesAsMsrString (
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

string multipleFullBarRestsWholeNotesAsMsrString (
  int             inputLineNumber, // JMI
  const Rational& wholeNotes)
{
  stringstream s;

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

// moments
//______________________________________________________________________________
// constants
const Rational  msrMoment::K_NO_POSITION (-987, 1);
const msrMoment msrMoment::K_NO_MOMENT   (K_NO_POSITION, K_NO_POSITION);

msrMoment::msrMoment ()
{
  fWrittenPositionInMeseasure = Rational (-1, 1);
  fSoundingRelativeOffset     = Rational (-1, 1);
}

msrMoment::msrMoment (
  const Rational& writtenPositionInMeseasure,
  const Rational& soundingRelativeOffset)
{
  fWrittenPositionInMeseasure = writtenPositionInMeseasure;
  fSoundingRelativeOffset     = soundingRelativeOffset;
}

msrMoment::msrMoment (
  const Rational& writtenPositionInMeseasure)
{
  fWrittenPositionInMeseasure = writtenPositionInMeseasure;
  fSoundingRelativeOffset     = Rational (0, 1);
}

msrMoment::~msrMoment ()
{}

void msrMoment::testMsrMomentComparisons (ostream& os)
{
  msrMoment m0 (Rational (3, 4));
  msrMoment m1 (Rational (3, 4), Rational (-1, 16));
  msrMoment m2 (Rational (3, 4), Rational (2, 16));

  os <<
    "m1: " << m1 << endl <<
    "m0: " << m0 << endl <<
    "m2: " << m2 << endl <<
    endl <<

    "m0 == m0: " << mfBooleanAsString (m0 == m0) << endl <<
    "m1 == m1: " << mfBooleanAsString (m1 == m1) << endl <<
    "m2 == m2: " << mfBooleanAsString (m2 == m2) << endl <<
    endl <<

    "m0 != m0: " << mfBooleanAsString (m0 != m0) << endl <<
    "m1 != m1: " << mfBooleanAsString (m1 != m1) << endl <<
    "m2 != m2: " << mfBooleanAsString (m2 != m2) << endl <<
    endl <<

    "m0 < m0: " << mfBooleanAsString (m0 < m0) << endl <<
    "m1 < m1: " << mfBooleanAsString (m1 < m1) << endl <<
    "m2 < m2: " << mfBooleanAsString (m2 < m2) << endl <<
    endl <<

    "m0 <= m0: " << mfBooleanAsString (m0 <= m0) << endl <<
    "m1 <= m1: " << mfBooleanAsString (m1 <= m1) << endl <<
    "m2 <= m2: " << mfBooleanAsString (m2 <= m2) << endl <<
    endl <<

    "m0 >= m0: " << mfBooleanAsString (m0 >= m0) << endl <<
    "m1 >= m1: " << mfBooleanAsString (m1 >= m1) << endl <<
    "m2 >= m2: " << mfBooleanAsString (m2 >= m2) << endl <<
    endl <<

    "m0 > m0: " << mfBooleanAsString (m0 > m0) << endl <<
    "m1 > m1: " << mfBooleanAsString (m1 > m1) << endl <<
    "m2 > m2: " << mfBooleanAsString (m2 > m2) << endl <<
    endl <<
    endl <<

    "m1 == m0: " << mfBooleanAsString (m1 == m0) << endl <<
    "m1 == m1: " << mfBooleanAsString (m1 == m1) << endl <<
    "m1 == m2: " << mfBooleanAsString (m1 == m2) << endl <<
    endl <<

    "m1 != m0: " << mfBooleanAsString (m1 != m0) << endl <<
    "m1 != m1: " << mfBooleanAsString (m1 != m1) << endl <<
    "m1 != m2: " << mfBooleanAsString (m1 != m2) << endl <<
    endl <<

    "m1 < m0: " << mfBooleanAsString (m1 < m0) << endl <<
    "m1 < m1: " << mfBooleanAsString (m1 < m1) << endl <<
    "m1 < m2: " << mfBooleanAsString (m1 < m2) << endl <<
    endl <<

    "m1 <= m0: " << mfBooleanAsString (m1 <= m0) << endl <<
    "m1 <= m1: " << mfBooleanAsString (m1 <= m1) << endl <<
    "m1 <= m2: " << mfBooleanAsString (m1 <= m2) << endl <<
    endl <<

    "m1 >= m0: " << mfBooleanAsString (m1 >= m0) << endl <<
    "m1 >= m1: " << mfBooleanAsString (m1 >= m1) << endl <<
    "m1 >= m2: " << mfBooleanAsString (m1 >= m2) << endl <<
    endl <<

    "m1 > m0: " << mfBooleanAsString (m1 > m0) << endl <<
    "m1 > m1: " << mfBooleanAsString (m1 > m1) << endl <<
    "m1 > m2: " << mfBooleanAsString (m1 > m2) << endl <<
    endl <<

    endl;

/* output: JMI
*/
}

Bool msrMoment::operator== (const msrMoment& other) const
{
  Bool result;

  if (fWrittenPositionInMeseasure != other.fWrittenPositionInMeseasure) {
    result = false;
  }
  else {
    result = fSoundingRelativeOffset == other.fSoundingRelativeOffset;
  }

  return result;
}

Bool msrMoment::operator< (const msrMoment& other) const
{
  Bool result;

  if (fWrittenPositionInMeseasure > other.fWrittenPositionInMeseasure) {
    result = false;
  }
  else if (fWrittenPositionInMeseasure == other.fWrittenPositionInMeseasure) {
    result = fSoundingRelativeOffset < other.fSoundingRelativeOffset;
  }
  else {
    result = true;
  }

  return result;
}

Bool msrMoment::operator> (const msrMoment& other) const
{
  Bool result;

  if (fWrittenPositionInMeseasure < other.fWrittenPositionInMeseasure) {
    result = false;
  }
  else if (fWrittenPositionInMeseasure == other.fWrittenPositionInMeseasure) {
    result = fSoundingRelativeOffset > other.fSoundingRelativeOffset;
  }
  else {
    result = true;
  }

  return result;
}

string msrMoment::asString () const
{
  stringstream s;

  s <<
    "[Moment" <<
    " writtenPositionInMeseasure: " <<
    fWrittenPositionInMeseasure <<
    ", soundingRelativeOffset: " <<
    fSoundingRelativeOffset <<
    ']';

  return s.str ();
}

void msrMoment::print (ostream& os) const
{
  os <<
    "[Moment" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "fWrittenPositionInMeseasure" << " : " << fWrittenPositionInMeseasure <<
    endl <<
    setw (fieldWidth) <<
    "fSoundingRelativeOffset" << " : " << fSoundingRelativeOffset <<
    endl;

  --gIndenter;

  os << ']' << endl;
};

ostream& operator << (ostream& os, const msrMoment& elt)
{
  elt.print (os);
  return os;
}

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
}

msrTupletFactor::msrTupletFactor (
  const Rational& rationalTupletFactor)
{
  fTupletActualNotes = rationalTupletFactor.getNumerator ();
  fTupletNormalNotes = rationalTupletFactor.getDenominator ();
}

msrTupletFactor::~msrTupletFactor ()
{}

msrTupletFactor msrTupletFactor::inverse () const
{
  msrTupletFactor result (
    fTupletNormalNotes, fTupletActualNotes);

  return result;
}

string msrTupletFactor::asString () const
{
  stringstream s;

  s <<
    "[TupletFactor" <<
    ", tupletActualNotes: " << fTupletActualNotes <<
    ", tupletNormalNotes: " << fTupletNormalNotes <<
    ']';

  return s.str ();
}

string msrTupletFactor::asFractionString () const
{
  stringstream s;

  s << fTupletActualNotes << '/' << fTupletNormalNotes;

  return s.str ();
}

void msrTupletFactor::print (ostream& os) const
{
  os <<
    "TupletFactor" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 11;

  os << left <<
    setw (fieldWidth) <<
    "fTupletActualNotes" << " : " << fTupletActualNotes <<
    endl <<
    setw (fieldWidth) <<
    "fTupletNormalNotes" << " : " << fTupletNormalNotes <<
    endl;

  --gIndenter;
};

ostream& operator << (ostream& os, const msrTupletFactor& elt)
{
  elt.print (os);
  return os;
}

// intervals
//______________________________________________________________________________
int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind)
{
  int result = INT_MIN;

  switch (intervalKind) {
    case msrIntervalKind::k_NoIntervalKind:
      break;

    case msrIntervalKind::kIntervalDiminishedUnisson:
      result = -1;
      break;
    case msrIntervalKind::kIntervalPerfectUnisson:
      result = 0;
      break;
    case msrIntervalKind::kIntervalAugmentedUnisson:
      result = 1;
      break;

    case msrIntervalKind::kIntervalDiminishedSecond:
      result = 0;
      break;
    case msrIntervalKind::kIntervalMinorSecond:
      result = 1;
      break;
    case msrIntervalKind::kIntervalMajorSecond:
      result = 2;
      break;
    case msrIntervalKind::kIntervalAugmentedSecond:
      result = 4;
      break;

    case msrIntervalKind::kIntervalDiminishedThird:
      result = 2;
      break;
    case msrIntervalKind::kIntervalMinorThird:
      result = 3;
      break;
    case msrIntervalKind::kIntervalMajorThird:
      result = 4;
      break;
    case msrIntervalKind::kIntervalAugmentedThird:
      result = 5;
      break;

    case msrIntervalKind::kIntervalDiminishedFourth:
      result = 4;
      break;
    case msrIntervalKind::kIntervalPerfectFourth:
      result = 5;
      break;
    case msrIntervalKind::kIntervalAugmentedFourth:
      result = 6;
      break;

    case msrIntervalKind::kIntervalDiminishedFifth:
      result = 6;
      break;
    case msrIntervalKind::kIntervalPerfectFifth:
      result = 7;
      break;
    case msrIntervalKind::kIntervalAugmentedFifth:
      result = 8;
      break;

    case msrIntervalKind::kIntervalDiminishedSixth:
      result = 7;
      break;
    case msrIntervalKind::kIntervalMinorSixth:
      result = 8;
      break;
    case msrIntervalKind::kIntervalMajorSixth:
      result = 9;
      break;
    case msrIntervalKind::kIntervalAugmentedSixth:
      result = 10;
      break;

    case msrIntervalKind::kIntervalDiminishedSeventh:
      result = 9;
      break;
    case msrIntervalKind::kIntervalMinorSeventh:
      result = 10;
      break;
    case msrIntervalKind::kIntervalMajorSeventh:
      result = 11;
      break;
    case msrIntervalKind::kIntervalAugmentedSeventh:
      result = 12;
      break;

    case msrIntervalKind::kIntervalDiminishedOctave:
      result = 11;
      break;
    case msrIntervalKind::kIntervalPerfectOctave:
      result = 12;
      break;
    case msrIntervalKind::kIntervalAugmentedOctave:
      result = 13;
      break;

    case msrIntervalKind::kIntervalDiminishedNinth:
      result = 12;
      break;
    case msrIntervalKind::kIntervalMinorNinth:
      result = 13;
      break;
    case msrIntervalKind::kIntervalMajorNinth:
      result = 14;
      break;
    case msrIntervalKind::kIntervalAugmentedNinth:
      result = 15;
      break;

    case msrIntervalKind::kIntervalDiminishedTenth:
      result = 14;
      break;
    case msrIntervalKind::kIntervalMinorTenth:
      result = 15;
      break;
    case msrIntervalKind::kIntervalMajorTenth:
      result = 16;
      break;
    case msrIntervalKind::kIntervalAugmentedTenth:
      result = 17;
      break;

    case msrIntervalKind::kIntervalDiminishedEleventh:
      result = 16;
      break;
    case msrIntervalKind::kIntervalPerfectEleventh:
      result = 17;
      break;
    case msrIntervalKind::kIntervalAugmentedEleventh:
      result = 18;
      break;

    case msrIntervalKind::kIntervalDiminishedTwelfth:
      result = 18;
      break;
    case msrIntervalKind::kIntervalPerfectTwelfth:
      result = 19;
      break;
    case msrIntervalKind::kIntervalAugmentedTwelfth:
      result = 20;
      break;

    case msrIntervalKind::kIntervalDiminishedThirteenth:
      result = 19;
      break;
    case msrIntervalKind::kIntervalMinorThirteenth:
      result = 20;
      break;
    case msrIntervalKind::kIntervalMajorThirteenth:
      result = 21;
      break;
    case msrIntervalKind::kIntervalAugmentedThirteenth:
      result = 22;
      break;
  } // switch

  return result;
}

int msrIntervalAsQuarterTones (
  msrIntervalKind intervalKind)
{
  return
    msrIntervalKindAsSemiTones (intervalKind) * 2;
}

//______________________________________________________________________________
string msrIntervalKindAsString (
  msrIntervalKind intervalKind)
{
  string result;

  switch (intervalKind) {
    case msrIntervalKind::k_NoIntervalKind:
      result = "noInterval";
      break;

    case msrIntervalKind::kIntervalDiminishedUnisson:
      result = "diminishedUnisson";
      break;
    case msrIntervalKind::kIntervalPerfectUnisson:
      result = "perfectUnison";
      break;
    case msrIntervalKind::kIntervalAugmentedUnisson:
      result = "augmentedUnison";
      break;

    case msrIntervalKind::kIntervalDiminishedSecond:
      result = "diminishedSecond";
      break;
    case msrIntervalKind::kIntervalMinorSecond:
      result = "minorSecond";
      break;
    case msrIntervalKind::kIntervalMajorSecond:
      result = "majorSecond";
      break;
    case msrIntervalKind::kIntervalAugmentedSecond:
      result = "augmentedSecond";
      break;

    case msrIntervalKind::kIntervalDiminishedThird:
      result = "diminishedThird";
      break;
    case msrIntervalKind::kIntervalMinorThird:
      result = "minorThird";
      break;
    case msrIntervalKind::kIntervalMajorThird:
      result = "majorThird";
      break;
    case msrIntervalKind::kIntervalAugmentedThird:
      result = "augmentedThird";
      break;

    case msrIntervalKind::kIntervalDiminishedFourth:
      result = "diminishedFourth";
      break;
    case msrIntervalKind::kIntervalPerfectFourth:
      result = "perfectFourth";
      break;
    case msrIntervalKind::kIntervalAugmentedFourth:
      result = "augmentedFourth";
      break;

    case msrIntervalKind::kIntervalDiminishedFifth:
      result = "diminishedFifth";
      break;
    case msrIntervalKind::kIntervalPerfectFifth:
      result = "perfectFifth";
      break;
    case msrIntervalKind::kIntervalAugmentedFifth:
      result = "augmentedFifth";
      break;

    case msrIntervalKind::kIntervalDiminishedSixth:
      result = "diminishedSixth";
      break;
    case msrIntervalKind::kIntervalMinorSixth:
      result = "minorSixth";
      break;
    case msrIntervalKind::kIntervalMajorSixth:
      result = "majorSixth";
      break;
    case msrIntervalKind::kIntervalAugmentedSixth:
      result = "augmentedSixth";
      break;

    case msrIntervalKind::kIntervalDiminishedSeventh:
      result = "diminishedSeventh";
      break;
    case msrIntervalKind::kIntervalMinorSeventh:
      result = "minorSeventh";
      break;
    case msrIntervalKind::kIntervalMajorSeventh:
      result = "majorSeventh";
      break;
    case msrIntervalKind::kIntervalAugmentedSeventh:
      result = "augmentedSeventh";
      break;

    case msrIntervalKind::kIntervalDiminishedOctave:
      result = "diminishedOctave";
      break;
    case msrIntervalKind::kIntervalPerfectOctave:
      result = "perfectOctave";
      break;
    case msrIntervalKind::kIntervalAugmentedOctave:
      result = "augmentedOctave";
      break;

    case msrIntervalKind::kIntervalDiminishedNinth:
      result = "diminishedNinth";
      break;
    case msrIntervalKind::kIntervalMinorNinth:
      result = "minorNinth";
      break;
    case msrIntervalKind::kIntervalMajorNinth:
      result = "majorNinth";
      break;
    case msrIntervalKind::kIntervalAugmentedNinth:
      result = "sugmentedNinth";
      break;

    case msrIntervalKind::kIntervalDiminishedTenth:
      result = "diminishedTenth";
      break;
    case msrIntervalKind::kIntervalMinorTenth:
      result = "minorTenth";
      break;
    case msrIntervalKind::kIntervalMajorTenth:
      result = "majorTenth";
      break;
    case msrIntervalKind::kIntervalAugmentedTenth:
      result = "augmentedTenth";
      break;

    case msrIntervalKind::kIntervalDiminishedEleventh:
      result = "diminishedEleventh";
      break;
    case msrIntervalKind::kIntervalPerfectEleventh:
      result = "perfectEleventh";
      break;
    case msrIntervalKind::kIntervalAugmentedEleventh:
      result = "augmentedEleventh";
      break;

    case msrIntervalKind::kIntervalDiminishedTwelfth:
      result = "diminishedTwelfth";
      break;
    case msrIntervalKind::kIntervalPerfectTwelfth:
      result = "perfectTwelfth";
      break;
    case msrIntervalKind::kIntervalAugmentedTwelfth:
      result = "augmentedTwelfth";
      break;

    case msrIntervalKind::kIntervalDiminishedThirteenth:
      result = "diminishedThirteenth";
      break;
    case msrIntervalKind::kIntervalMinorThirteenth:
      result = "minorThirteenth";
      break;
    case msrIntervalKind::kIntervalMajorThirteenth:
      result = "majorThirteenth";
      break;
    case msrIntervalKind::kIntervalAugmentedThirteenth:
      result = "augmentedThirteenth";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrIntervalKind& elt)
{
  os << msrIntervalKindAsString (elt);
  return os;
}

string msrIntervalAsShortString (
  msrIntervalKind intervalKind)
{
  string result;

  switch (intervalKind) {
    case msrIntervalKind::k_NoIntervalKind:
      result = "noInterval";
      break;

    case msrIntervalKind::kIntervalDiminishedUnisson:
      result = "bu";
      break;
    case msrIntervalKind::kIntervalPerfectUnisson:
      result = "u";
      break;
    case msrIntervalKind::kIntervalAugmentedUnisson:
      result = "#u";
      break;

    case msrIntervalKind::kIntervalDiminishedSecond:
      result = "bb2";
      break;
    case msrIntervalKind::kIntervalMinorSecond:
      result = "b2";
      break;
    case msrIntervalKind::kIntervalMajorSecond:
      result = "2";
      break;
    case msrIntervalKind::kIntervalAugmentedSecond:
      result = "#2";
      break;

    case msrIntervalKind::kIntervalDiminishedThird:
      result = "bb3";
      break;
    case msrIntervalKind::kIntervalMinorThird:
      result = "b3";
      break;
    case msrIntervalKind::kIntervalMajorThird:
      result = "3";
      break;
    case msrIntervalKind::kIntervalAugmentedThird:
      result = "#3";
      break;

    case msrIntervalKind::kIntervalDiminishedFourth:
      result = "b4";
      break;
    case msrIntervalKind::kIntervalPerfectFourth:
      result = "4";
      break;
    case msrIntervalKind::kIntervalAugmentedFourth:
      result = "#4";
      break;

    case msrIntervalKind::kIntervalDiminishedFifth:
      result = "b5";
      break;
    case msrIntervalKind::kIntervalPerfectFifth:
      result = "5";
      break;
    case msrIntervalKind::kIntervalAugmentedFifth:
      result = "#4";
      break;

    case msrIntervalKind::kIntervalDiminishedSixth:
      result = "bb6";
      break;
    case msrIntervalKind::kIntervalMinorSixth:
      result = "b6";
      break;
    case msrIntervalKind::kIntervalMajorSixth:
      result = "6";
      break;
    case msrIntervalKind::kIntervalAugmentedSixth:
      result = "#6";
      break;

    case msrIntervalKind::kIntervalDiminishedSeventh:
      result = "b7";
      break;
    case msrIntervalKind::kIntervalMinorSeventh:
      result = "7";
      break;
    case msrIntervalKind::kIntervalMajorSeventh:
      result = "∆7";
      break;
    case msrIntervalKind::kIntervalAugmentedSeventh:
      result = "∆∆7";
      break;

    case msrIntervalKind::kIntervalDiminishedOctave:
      result = "b8";
      break;
    case msrIntervalKind::kIntervalPerfectOctave:
      result = "8";
      break;
    case msrIntervalKind::kIntervalAugmentedOctave:
      result = "#8";
      break;

    case msrIntervalKind::kIntervalDiminishedNinth:
      result = "bb9";
      break;
    case msrIntervalKind::kIntervalMinorNinth:
      result = "b9";
      break;
    case msrIntervalKind::kIntervalMajorNinth:
      result = "9";
      break;
    case msrIntervalKind::kIntervalAugmentedNinth:
      result = "#9";
      break;

    case msrIntervalKind::kIntervalDiminishedTenth:
      result = "bb10";
      break;
    case msrIntervalKind::kIntervalMinorTenth:
      result = "b10";
      break;
    case msrIntervalKind::kIntervalMajorTenth:
      result = "10";
      break;
    case msrIntervalKind::kIntervalAugmentedTenth:
      result = "#10";
      break;

    case msrIntervalKind::kIntervalDiminishedEleventh:
      result = "b11";
      break;
    case msrIntervalKind::kIntervalPerfectEleventh:
      result = "11";
      break;
    case msrIntervalKind::kIntervalAugmentedEleventh:
      result = "#11";
      break;

    case msrIntervalKind::kIntervalDiminishedTwelfth:
      result = "b12";
      break;
    case msrIntervalKind::kIntervalPerfectTwelfth:
      result = "12";
      break;
    case msrIntervalKind::kIntervalAugmentedTwelfth:
      result = "#12";
      break;

    case msrIntervalKind::kIntervalDiminishedThirteenth:
      result = "bb13";
      break;
    case msrIntervalKind::kIntervalMinorThirteenth:
      result = "b13";
      break;
    case msrIntervalKind::kIntervalMajorThirteenth:
      result = "13";
      break;
    case msrIntervalKind::kIntervalAugmentedThirteenth:
      result = "#13";
      break;
  } // switch

  return result;
}

msrIntervalKind invertIntervalKind (
  msrIntervalKind intervalKind)
{
  msrIntervalKind result = msrIntervalKind::k_NoIntervalKind;

  switch (intervalKind) {
    case msrIntervalKind::k_NoIntervalKind:
      break;

    case msrIntervalKind::kIntervalDiminishedUnisson:
      result = msrIntervalKind::kIntervalAugmentedUnisson;
      break;
    case msrIntervalKind::kIntervalPerfectUnisson:
      result = msrIntervalKind::kIntervalPerfectUnisson;
      break;
    case msrIntervalKind::kIntervalAugmentedUnisson:
      result = msrIntervalKind::kIntervalDiminishedOctave;
      break;

    case msrIntervalKind::kIntervalDiminishedSecond:
      result = msrIntervalKind::kIntervalAugmentedSeventh;
      break;
    case msrIntervalKind::kIntervalMinorSecond:
      result = msrIntervalKind::kIntervalMajorSeventh;
      break;
    case msrIntervalKind::kIntervalMajorSecond:
      result = msrIntervalKind::kIntervalMinorSeventh;
      break;
    case msrIntervalKind::kIntervalAugmentedSecond:
      result = msrIntervalKind::kIntervalDiminishedSeventh;
      break;

    case msrIntervalKind::kIntervalDiminishedThird:
      result = msrIntervalKind::kIntervalAugmentedSixth;
      break;
    case msrIntervalKind::kIntervalMinorThird:
      result = msrIntervalKind::kIntervalMajorSixth;
      break;
    case msrIntervalKind::kIntervalMajorThird:
      result = msrIntervalKind::kIntervalMinorSixth;
      break;
    case msrIntervalKind::kIntervalAugmentedThird:
      result = msrIntervalKind::kIntervalDiminishedSixth;
      break;

    case msrIntervalKind::kIntervalDiminishedFourth:
      result = msrIntervalKind::kIntervalAugmentedFifth;
      break;
    case msrIntervalKind::kIntervalPerfectFourth:
      result = msrIntervalKind::kIntervalPerfectFifth;
      break;
    case msrIntervalKind::kIntervalAugmentedFourth:
      result = msrIntervalKind::kIntervalDiminishedFifth;
      break;

    case msrIntervalKind::kIntervalDiminishedFifth:
      result = msrIntervalKind::kIntervalAugmentedFourth;
      break;
    case msrIntervalKind::kIntervalPerfectFifth:
      result = msrIntervalKind::kIntervalPerfectFourth;
      break;
    case msrIntervalKind::kIntervalAugmentedFifth:
      result = msrIntervalKind::kIntervalDiminishedFourth;
      break;

    case msrIntervalKind::kIntervalDiminishedSixth:
      result = msrIntervalKind::kIntervalAugmentedThird;
      break;
    case msrIntervalKind::kIntervalMinorSixth:
      result = msrIntervalKind::kIntervalMajorThird;
      break;
    case msrIntervalKind::kIntervalMajorSixth:
      result = msrIntervalKind::kIntervalMinorThird;
      break;
    case msrIntervalKind::kIntervalAugmentedSixth:
      result = msrIntervalKind::kIntervalDiminishedThird;
      break;

    case msrIntervalKind::kIntervalDiminishedSeventh:
      result = msrIntervalKind::kIntervalAugmentedSecond;
      break;
    case msrIntervalKind::kIntervalMinorSeventh:
      result = msrIntervalKind::kIntervalMajorSecond;
      break;
    case msrIntervalKind::kIntervalMajorSeventh:
      result = msrIntervalKind::kIntervalMinorSecond;
      break;
    case msrIntervalKind::kIntervalAugmentedSeventh:
      result = msrIntervalKind::kIntervalDiminishedSecond;
      break;

    case msrIntervalKind::kIntervalDiminishedOctave:
      result = msrIntervalKind::kIntervalAugmentedUnisson;
      break;
    case msrIntervalKind::kIntervalPerfectOctave:
      result = msrIntervalKind::kIntervalPerfectOctave;
      break;
    case msrIntervalKind::kIntervalAugmentedOctave:
      result = msrIntervalKind::kIntervalDiminishedUnisson;
      break;

    case msrIntervalKind::kIntervalDiminishedNinth:
      result = msrIntervalKind::kIntervalAugmentedSeventh;
      break;
    case msrIntervalKind::kIntervalMinorNinth:
      result = msrIntervalKind::kIntervalMajorSeventh;
      break;
    case msrIntervalKind::kIntervalMajorNinth:
      result = msrIntervalKind::kIntervalMinorSeventh;
      break;
    case msrIntervalKind::kIntervalAugmentedNinth:
      result = msrIntervalKind::kIntervalDiminishedSeventh;
      break;

    case msrIntervalKind::kIntervalDiminishedTenth:
      result = msrIntervalKind::kIntervalAugmentedSixth;
      break;
    case msrIntervalKind::kIntervalMinorTenth:
      result = msrIntervalKind::kIntervalMajorSixth;
      break;
    case msrIntervalKind::kIntervalMajorTenth:
      result = msrIntervalKind::kIntervalMinorSixth;
      break;
    case msrIntervalKind::kIntervalAugmentedTenth:
      result = msrIntervalKind::kIntervalDiminishedSixth;
      break;

    case msrIntervalKind::kIntervalDiminishedEleventh:
      result = msrIntervalKind::kIntervalAugmentedFifth;
      break;
    case msrIntervalKind::kIntervalPerfectEleventh:
      result = msrIntervalKind::kIntervalPerfectFifth;
      break;
    case msrIntervalKind::kIntervalAugmentedEleventh:
      result = msrIntervalKind::kIntervalDiminishedFifth;
      break;

    case msrIntervalKind::kIntervalDiminishedTwelfth:
      result = msrIntervalKind::kIntervalAugmentedFourth;
      break;
    case msrIntervalKind::kIntervalPerfectTwelfth:
      result = msrIntervalKind::kIntervalPerfectFourth;
      break;
    case msrIntervalKind::kIntervalAugmentedTwelfth:
      result = msrIntervalKind::kIntervalDiminishedFourth;
      break;

    case msrIntervalKind::kIntervalDiminishedThirteenth:
      result = msrIntervalKind::kIntervalAugmentedThird;
      break;
    case msrIntervalKind::kIntervalMinorThirteenth:
      result = msrIntervalKind::kIntervalMajorThird;
      break;
    case msrIntervalKind::kIntervalMajorThirteenth:
      result = msrIntervalKind::kIntervalMinorThird;
      break;
    case msrIntervalKind::kIntervalAugmentedThirteenth:
      result = msrIntervalKind::kIntervalDiminishedThird;
      break;
  } // switch

  return result;
}

int intervalKindAsSemitones (
  msrIntervalKind intervalKind)
{
  int result = INT_MIN;

  switch (intervalKind) {
    case msrIntervalKind::k_NoIntervalKind:
      break;

    case msrIntervalKind::kIntervalDiminishedUnisson:
      result = -1;
      break;
    case msrIntervalKind::kIntervalPerfectUnisson:
      result = 0;
      break;
    case msrIntervalKind::kIntervalAugmentedUnisson:
      result = 1;
      break;

    case msrIntervalKind::kIntervalDiminishedSecond:
      result = 0;
      break;
    case msrIntervalKind::kIntervalMinorSecond:
      result = 1;
      break;
    case msrIntervalKind::kIntervalMajorSecond:
      result = 2;
      break;
    case msrIntervalKind::kIntervalAugmentedSecond:
      result = 3;
      break;

    case msrIntervalKind::kIntervalDiminishedThird:
      result = 2;
      break;
    case msrIntervalKind::kIntervalMinorThird:
      result = 3;
      break;
    case msrIntervalKind::kIntervalMajorThird:
      result = 4;
      break;
    case msrIntervalKind::kIntervalAugmentedThird:
      result = 5;
      break;

    case msrIntervalKind::kIntervalDiminishedFourth:
      result = 4;
      break;
    case msrIntervalKind::kIntervalPerfectFourth:
      result = 5;
      break;
    case msrIntervalKind::kIntervalAugmentedFourth:
      result = 6;
      break;

    case msrIntervalKind::kIntervalDiminishedFifth:
      result = 7;
      break;
    case msrIntervalKind::kIntervalPerfectFifth:
      result = 7;
      break;
    case msrIntervalKind::kIntervalAugmentedFifth:
      result = 8;
      break;

    case msrIntervalKind::kIntervalDiminishedSixth:
      result = 7;
      break;
    case msrIntervalKind::kIntervalMinorSixth:
      result = 8;
      break;
    case msrIntervalKind::kIntervalMajorSixth:
      result = 9;
      break;
    case msrIntervalKind::kIntervalAugmentedSixth:
      result = 10;
      break;

    case msrIntervalKind::kIntervalDiminishedSeventh:
      result = 9;
      break;
    case msrIntervalKind::kIntervalMinorSeventh:
      result = 10;
      break;
    case msrIntervalKind::kIntervalMajorSeventh:
      result = 11;
      break;
    case msrIntervalKind::kIntervalAugmentedSeventh:
      result = 12;
      break;

    case msrIntervalKind::kIntervalDiminishedOctave:
      result = 11;
      break;
    case msrIntervalKind::kIntervalPerfectOctave:
      result = 12;
      break;
    case msrIntervalKind::kIntervalAugmentedOctave:
      result = 13;
      break;

    case msrIntervalKind::kIntervalDiminishedNinth:
      result = 12;
      break;
    case msrIntervalKind::kIntervalMinorNinth:
      result = 13;
      break;
    case msrIntervalKind::kIntervalMajorNinth:
      result = 14;
      break;
    case msrIntervalKind::kIntervalAugmentedNinth:
      result = 15;
      break;

    case msrIntervalKind::kIntervalDiminishedTenth:
      result = 14;
      break;
    case msrIntervalKind::kIntervalMinorTenth:
      result = 15;
      break;
    case msrIntervalKind::kIntervalMajorTenth:
      result = 16;
      break;
    case msrIntervalKind::kIntervalAugmentedTenth:
      result = 17;
      break;

    case msrIntervalKind::kIntervalDiminishedEleventh:
      result = 16;
      break;
    case msrIntervalKind::kIntervalPerfectEleventh:
      result = 17;
      break;
    case msrIntervalKind::kIntervalAugmentedEleventh:
      result = 18;
      break;

    case msrIntervalKind::kIntervalDiminishedTwelfth:
      result = 18;
      break;
    case msrIntervalKind::kIntervalPerfectTwelfth:
      result = 19;
      break;
    case msrIntervalKind::kIntervalAugmentedTwelfth:
      result = 20;
      break;

    case msrIntervalKind::kIntervalDiminishedThirteenth:
      result = 19;
      break;
    case msrIntervalKind::kIntervalMinorThirteenth:
      result = 20;
      break;
    case msrIntervalKind::kIntervalMajorThirteenth:
      result = 21;
      break;
    case msrIntervalKind::kIntervalAugmentedThirteenth:
      result = 22;
      break;
  } // switch

  return result;
}


/* JMI
msrSemiTonesPitchKind noteAtIntervalKindFromNote (
  msrIntervalKind             intervalKind,
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  msrSemiTonesPitchKind result = msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch;

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch;
      break;

    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          switch (alterationPreferenceKind) {
            case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
              result = msrSemiTonesPitchKind::kSTP_C_Sharp;
              break;
            case msrAlterationPreferenceKind::kAlterationPreferenceNatural:
              result = msrSemiTonesPitchKind::kSTP_C_Sharp;
              break;
            case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
              result = msrSemiTonesPitchKind::kSTP_C_Sharp;
              break;
          } // switch
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          break;
        case msrIntervalKind::kIntervalMinorThird:
          break;
        case msrIntervalKind::kIntervalMajorThird:
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_C_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
      break;

    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      break;

    case msrSemiTonesPitchKind::kSTP_D_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
      break;

    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      break;

    case msrSemiTonesPitchKind::kSTP_E_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_F_Flat:
      break;

    case msrSemiTonesPitchKind::kSTP_F_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_E_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
      break;

    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      break;

    case msrSemiTonesPitchKind::kSTP_G_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
      break;

    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      break;

    case msrSemiTonesPitchKind::kSTP_A_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
      break;

    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      break;

    case msrSemiTonesPitchKind::kSTP_B_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      break;
  } // switch

  return result;
}
*/

msrSemiTonesPitchKind noteAtIntervalFromSemiTonesPitch (
  int                   inputLineNumber,
  msrIntervalKind       intervalKind,
  msrSemiTonesPitchKind semiTonesPitchKind)
{
  msrSemiTonesPitchKind result = msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch;

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_D_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_A_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_D_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_C_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_D_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_D_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_E_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_E_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_E_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_G_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_D_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_G_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_C_TripleSharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_F_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_G_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_A_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_D_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_G_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_A_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_F_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_B_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_B_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_G_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_A_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_A_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_G_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_A_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_A_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_G_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_F_TripleSharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_B_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_B_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrSemiTonesPitchKind::kSTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrSemiTonesPitchKind::kSTP_D_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrSemiTonesPitchKind::kSTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrSemiTonesPitchKind::kSTP_G_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrSemiTonesPitchKind::kSTP_A_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrSemiTonesPitchKind::kSTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrSemiTonesPitchKind::kSTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrSemiTonesPitchKind::kSTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrSemiTonesPitchKind::kSTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrSemiTonesPitchKind::kSTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrSemiTonesPitchKind::kSTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrSemiTonesPitchKind::kSTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrSemiTonesPitchKind::kSTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrSemiTonesPitchKind::kSTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrSemiTonesPitchKind::kSTP_D_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrSemiTonesPitchKind::kSTP_D_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrSemiTonesPitchKind::kSTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrSemiTonesPitchKind::kSTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrSemiTonesPitchKind::kSTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrSemiTonesPitchKind::kSTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrSemiTonesPitchKind::kSTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrSemiTonesPitchKind::kSTP_G_TripleSharp;
          break;
      } // switch
      break;

      default:
      {
        // computing intervals on double or triple flats/sharps
        // is not considered useful, hence not supported
        stringstream s;

        s <<
          "Sorry, computing intervals from semitones pitch '" <<
          msrSemiTonesPitchKindAsString (semiTonesPitchKind) <<
          '(' << msrSemiTonesPitchKindAsString (semiTonesPitchKind) << ")" <<
          "' is not supported"
          ", line = " << inputLineNumber;

        msrUnsupported (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
  } // switch

  return result;
}

msrQuarterTonesPitchKind noteAtIntervalFromQuarterTonesPitch (
  int                      inputLineNumber,
  msrIntervalKind          intervalKind,
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrQuarterTonesPitchKind result = msrQuarterTonesPitchKind::k_NoQuarterTonesPitch;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::k_NoQuarterTonesPitch:
      result = msrQuarterTonesPitchKind::k_NoQuarterTonesPitch;
      break;

    case msrQuarterTonesPitchKind::kQTP_C_Flat:
      break;

    case msrQuarterTonesPitchKind::kQTP_C_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_C_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_D_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_D_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_D_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_E_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_E_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_E_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_G_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_D_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_G_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_C_TripleSharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_F_Flat:
      break;

    case msrQuarterTonesPitchKind::kQTP_F_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_F_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_G_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_G_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_G_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_A_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_A_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_A_Sharp:
      break;

    case msrQuarterTonesPitchKind::kQTP_B_Flat:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_B_Natural:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;
      } // switch
      break;

    case msrQuarterTonesPitchKind::kQTP_B_Sharp:
      switch (intervalKind) {
        case msrIntervalKind::k_NoIntervalKind:
          break;

        case msrIntervalKind::kIntervalDiminishedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          result = msrQuarterTonesPitchKind::kQTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThird:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          result = msrQuarterTonesPitchKind::kQTP_D_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFourth:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedFifth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          result = msrQuarterTonesPitchKind::kQTP_G_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          result = msrQuarterTonesPitchKind::kQTP_A_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedOctave:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectOctave:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedOctave:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrIntervalKind::kIntervalMinorNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedNinth:
          result = msrQuarterTonesPitchKind::kQTP_C_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrIntervalKind::kIntervalMinorTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTenth:
          result = msrQuarterTonesPitchKind::kQTP_D_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrIntervalKind::kIntervalPerfectEleventh:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrIntervalKind::kIntervalAugmentedEleventh:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrIntervalKind::kIntervalPerfectTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedTwelfth:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleSharp;
          break;

        case msrIntervalKind::kIntervalDiminishedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrIntervalKind::kIntervalMinorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrIntervalKind::kIntervalMajorThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;
        case msrIntervalKind::kIntervalAugmentedThirteenth:
          result = msrQuarterTonesPitchKind::kQTP_G_TripleSharp;
          break;
      } // switch
      break;

      default:
      {
        // computing intervals on double or triple flats/sharps
        // is not considered useful, hence not supported
        stringstream s;

        s <<
          "Sorry, computing intervals from quartertones pitch '" <<
          msrQuarterTonesPitchKindAsStringInLanguage (
            quarterTonesPitchKind,
            gGlobalMsrOahGroup->
              getMsrQuarterTonesPitchesLanguageKind ()) <<
          '(' << msrQuarterTonesPitchKindAsString (quarterTonesPitchKind) << ")" <<
          "' is not supported"
          ", line = " << inputLineNumber;

        msrUnsupported (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
  } // switch

  return result;
}

msrIntervalKind intervalBetweenSemiTonesPitches (
  msrSemiTonesPitchKind semiTonesPitch1,
  msrSemiTonesPitchKind semiTonesPitch2)
{
  msrIntervalKind result = msrIntervalKind::k_NoIntervalKind;

  msrSemiTonesPitchKind
    workSemiTonesPitch1 = semiTonesPitch1,
    workSemiTonesPitch2 = semiTonesPitch2;

  // order the semitones pitches so that
  // workSemiTonesPitch1 is greater or equal to workSemiTonesPitch2
  // according to the enum type
  Bool invertInterval (false);

  if (semiTonesPitch1 < semiTonesPitch2) {
    workSemiTonesPitch1 = semiTonesPitch2;
    workSemiTonesPitch2 = semiTonesPitch1;
    invertInterval = true;
  }

  switch (workSemiTonesPitch1) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      break;

    case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
      switch (workSemiTonesPitch2) {
        case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
          break;

 /*
     msrIntervalKind::kIntervalDiminishedUnisson, msrIntervalKind::kIntervalPerfectUnisson, msrIntervalKind::kIntervalAugmentedUnisson,

  msrIntervalKind::kIntervalDiminishedSecond, msrIntervalKind::kIntervalMinorSecond, msrIntervalKind::kIntervalMajorSecond, msrIntervalKind::kIntervalAugmentedSecond,

  msrIntervalKind::kIntervalDiminishedThird, msrIntervalKind::kIntervalMinorThird, msrIntervalKind::kIntervalMajorThird, msrIntervalKind::kIntervalAugmentedThird,

  msrIntervalKind::kIntervalDiminishedFourth, msrIntervalKind::kIntervalPerfectFourth, msrIntervalKind::kIntervalAugmentedFourth,

  msrIntervalKind::kIntervalDiminishedFifth, msrIntervalKind::kIntervalPerfectFifth, msrIntervalKind::kIntervalAugmentedFifth,

  msrIntervalKind::kIntervalDiminishedSixth, msrIntervalKind::kIntervalMinorSixth, msrIntervalKind::kIntervalMajorSixth, msrIntervalKind::kIntervalAugmentedSixth,

  msrIntervalKind::kIntervalDiminishedSeventh, msrIntervalKind::kIntervalMinorSeventh, msrIntervalKind::kIntervalMajorSeventh, msrIntervalKind::kIntervalAugmentedSeventh,

  msrIntervalKind::kIntervalDiminishedOctave, msrIntervalKind::kIntervalPerfectOctave, msrIntervalKind::kIntervalAugmentedOctave,

  msrIntervalKind::kIntervalDiminishedNinth, msrIntervalKind::kIntervalMinorNinth, msrIntervalKind::kIntervalMajorNinth, msrIntervalKind::kIntervalAugmentedNinth,

  msrIntervalKind::kIntervalDiminishedTenth, msrIntervalKind::kIntervalMinorTenth, msrIntervalKind::kIntervalMajorTenth, msrIntervalKind::kIntervalAugmentedTenth,

  msrIntervalKind::kIntervalDiminishedEleventh, msrIntervalKind::kIntervalPerfectEleventh, msrIntervalKind::kIntervalAugmentedEleventh,

  msrIntervalKind::kIntervalDiminishedTwelfth, msrIntervalKind::kIntervalPerfectTwelfth, msrIntervalKind::kIntervalAugmentedTwelfth,

  msrIntervalKind::kIntervalDiminishedThirteenth, msrIntervalKind::kIntervalMinorThirteenth, msrIntervalKind::kIntervalMajorThirteenth, msrIntervalKind::kIntervalAugmentedThirteenth };

*/

        case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_D_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_E_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_A_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_B_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      break;
    case msrSemiTonesPitchKind::kSTP_C_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
      break;

    case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      break;
    case msrSemiTonesPitchKind::kSTP_D_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
      break;

    case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      break;
    case msrSemiTonesPitchKind::kSTP_E_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_E_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
      break;

    case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_F_Flat:
      break;
    case msrSemiTonesPitchKind::kSTP_F_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
      break;

    case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      break;
    case msrSemiTonesPitchKind::kSTP_G_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
      break;

    case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      break;
    case msrSemiTonesPitchKind::kSTP_A_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
      break;
    case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
      break;

    case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      break;
    case msrSemiTonesPitchKind::kSTP_B_Natural:
      break;
    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      break;

    case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
      switch (workSemiTonesPitch2) {
        case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
          break;

 /*
     msrIntervalKind::kIntervalDiminishedUnisson, msrIntervalKind::kIntervalPerfectUnisson, msrIntervalKind::kIntervalAugmentedUnisson,

  msrIntervalKind::kIntervalDiminishedSecond, msrIntervalKind::kIntervalMinorSecond, msrIntervalKind::kIntervalMajorSecond, msrIntervalKind::kIntervalAugmentedSecond,

  msrIntervalKind::kIntervalDiminishedThird, msrIntervalKind::kIntervalMinorThird, msrIntervalKind::kIntervalMajorThird, msrIntervalKind::kIntervalAugmentedThird,

  msrIntervalKind::kIntervalDiminishedFourth, msrIntervalKind::kIntervalPerfectFourth, msrIntervalKind::kIntervalAugmentedFourth,

  msrIntervalKind::kIntervalDiminishedFifth, msrIntervalKind::kIntervalPerfectFifth, msrIntervalKind::kIntervalAugmentedFifth,

  msrIntervalKind::kIntervalDiminishedSixth, msrIntervalKind::kIntervalMinorSixth, msrIntervalKind::kIntervalMajorSixth, msrIntervalKind::kIntervalAugmentedSixth,

  msrIntervalKind::kIntervalDiminishedSeventh, msrIntervalKind::kIntervalMinorSeventh, msrIntervalKind::kIntervalMajorSeventh, msrIntervalKind::kIntervalAugmentedSeventh,

  msrIntervalKind::kIntervalDiminishedOctave, msrIntervalKind::kIntervalPerfectOctave, msrIntervalKind::kIntervalAugmentedOctave,

  msrIntervalKind::kIntervalDiminishedNinth, msrIntervalKind::kIntervalMinorNinth, msrIntervalKind::kIntervalMajorNinth, msrIntervalKind::kIntervalAugmentedNinth,

  msrIntervalKind::kIntervalDiminishedTenth, msrIntervalKind::kIntervalMinorTenth, msrIntervalKind::kIntervalMajorTenth, msrIntervalKind::kIntervalAugmentedTenth,

  msrIntervalKind::kIntervalDiminishedEleventh, msrIntervalKind::kIntervalPerfectEleventh, msrIntervalKind::kIntervalAugmentedEleventh,

  msrIntervalKind::kIntervalDiminishedTwelfth, msrIntervalKind::kIntervalPerfectTwelfth, msrIntervalKind::kIntervalAugmentedTwelfth,

  msrIntervalKind::kIntervalDiminishedThirteenth, msrIntervalKind::kIntervalMinorThirteenth, msrIntervalKind::kIntervalMajorThirteenth, msrIntervalKind::kIntervalAugmentedThirteenth };

*/

        case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_D_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_E_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_A_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_B_Sharp:
          result = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
          result = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
          result = msrIntervalKind::kIntervalPerfectUnisson;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
      switch (workSemiTonesPitch2) {
        case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
          break;

 /*
     msrIntervalKind::kIntervalDiminishedUnisson, msrIntervalKind::kIntervalPerfectUnisson, msrIntervalKind::kIntervalAugmentedUnisson,

  msrIntervalKind::kIntervalDiminishedSecond, msrIntervalKind::kIntervalMinorSecond, msrIntervalKind::kIntervalMajorSecond, msrIntervalKind::kIntervalAugmentedSecond,

  msrIntervalKind::kIntervalDiminishedThird, msrIntervalKind::kIntervalMinorThird, msrIntervalKind::kIntervalMajorThird, msrIntervalKind::kIntervalAugmentedThird,

  msrIntervalKind::kIntervalDiminishedFourth, msrIntervalKind::kIntervalPerfectFourth, msrIntervalKind::kIntervalAugmentedFourth,

  msrIntervalKind::kIntervalDiminishedFifth, msrIntervalKind::kIntervalPerfectFifth, msrIntervalKind::kIntervalAugmentedFifth,

  msrIntervalKind::kIntervalDiminishedSixth, msrIntervalKind::kIntervalMinorSixth, msrIntervalKind::kIntervalMajorSixth, msrIntervalKind::kIntervalAugmentedSixth,

  msrIntervalKind::kIntervalDiminishedSeventh, msrIntervalKind::kIntervalMinorSeventh, msrIntervalKind::kIntervalMajorSeventh, msrIntervalKind::kIntervalAugmentedSeventh,

  msrIntervalKind::kIntervalDiminishedOctave, msrIntervalKind::kIntervalPerfectOctave, msrIntervalKind::kIntervalAugmentedOctave,

  msrIntervalKind::kIntervalDiminishedNinth, msrIntervalKind::kIntervalMinorNinth, msrIntervalKind::kIntervalMajorNinth, msrIntervalKind::kIntervalAugmentedNinth,

  msrIntervalKind::kIntervalDiminishedTenth, msrIntervalKind::kIntervalMinorTenth, msrIntervalKind::kIntervalMajorTenth, msrIntervalKind::kIntervalAugmentedTenth,

  msrIntervalKind::kIntervalDiminishedEleventh, msrIntervalKind::kIntervalPerfectEleventh, msrIntervalKind::kIntervalAugmentedEleventh,

  msrIntervalKind::kIntervalDiminishedTwelfth, msrIntervalKind::kIntervalPerfectTwelfth, msrIntervalKind::kIntervalAugmentedTwelfth,

  msrIntervalKind::kIntervalDiminishedThirteenth, msrIntervalKind::kIntervalMinorThirteenth, msrIntervalKind::kIntervalMajorThirteenth, msrIntervalKind::kIntervalAugmentedThirteenth };

*/

        case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_D_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_E_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_A_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_Flat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_Natural:
          break;
        case msrSemiTonesPitchKind::kSTP_B_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
          result = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
          result = msrIntervalKind::kIntervalPerfectUnisson;
          break;
      } // switch
      break;
  } // switch

  if (invertInterval) {
    result = invertIntervalKind (result);
  }

  return result;
}

// staves
//______________________________________________________________________________
string msrStaffKindAsString (
  msrStaffKind staffKind)
{
  string result;

  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
      result = "kStaffKindRegular";
      break;
    case msrStaffKind::kStaffKindTablature:
      result = "kStaffKindTablature";
      break;
    case msrStaffKind::kStaffKindHarmonies:
      result = "kStaffKindHarmonies";
      break;
    case msrStaffKind::kStaffKindFiguredBass:
      result = "kStaffKindFiguredBass";
      break;
    case msrStaffKind::kStaffKindDrum:
      result = "kStaffKindDrum";
      break;
    case msrStaffKind::kStaffKindRythmic:
      result = "kStaffKindRythmic";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrStaffKind& elt)
{
  os << msrStaffKindAsString (elt);
  return os;
}

string msrStaffKindAsStringForPrint (
  msrStaffKind staffKind)
{
  string result;

  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
      result = "kStaffKindRegular";
      break;
    case msrStaffKind::kStaffKindTablature:
      result = "kStaffKindTablature";
      break;
    case msrStaffKind::kStaffKindHarmonies:
      result = "kStaffKindHarmonies";
      break;
    case msrStaffKind::kStaffKindFiguredBass:
      result = "kStaffKindFiguredBass";
      break;
    case msrStaffKind::kStaffKindDrum:
      result = "kStaffKindDrum";
      break;
    case msrStaffKind::kStaffKindRythmic:
      result = "kStaffKindRythmic";
      break;
  } // switch

  return result;
}

// voices
//______________________________________________________________________________
string msrVoiceKindAsString (
  msrVoiceKind voiceKind)
{
  string result;

  switch (voiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      result = "kVoiceKindRegular";
      break;
    case msrVoiceKind::kVoiceKindDynamics:
      result = "kVoiceKindDynamics";
      break;
    case msrVoiceKind::kVoiceKindHarmonies:
      result = "kVoiceKindHarmonies";
      break;
    case msrVoiceKind::kVoiceKindFiguredBass:
      result = "kVoiceKindFiguredBass";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrVoiceKind& elt)
{
  os << msrVoiceKindAsString (elt);
  return os;
}

string msrVoiceKindAsStringForPrint (
  msrVoiceKind voiceKind)
{
  string result;

  switch (voiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      result = "Regular";
      break;
    case msrVoiceKind::kVoiceKindDynamics:
      result = "Dynamics";
      break;
    case msrVoiceKind::kVoiceKindHarmonies:
      result = "Harmonies";
      break;
    case msrVoiceKind::kVoiceKindFiguredBass:
      result = "FiguredBass";
      break;
  } // switch

  return result;
}

// measures
//______________________________________________________________________________
string msrMeasureKindAsString (
  msrMeasureKind measureKind)
{
  string result;

  switch (measureKind) {
    case msrMeasureKind::kMeasureKindUnknown:
      result = "***kMeasureKindUnknown***";
      break;
    case msrMeasureKind::kMeasureKindRegular:
      result = "kMeasureKindRegular";
      break;
    case msrMeasureKind::kMeasureKindAnacrusis:
      result = "kMeasureKindAnacrusis";
      break;
    case msrMeasureKind::kMeasureKindIncompleteStandalone:
      result = "kMeasureKindIncompleteStandalone";
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
      result = "kMeasureKindIncompleteLastInRepeatCommonPart";
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
      result = "kMeasureKindIncompleteLastInRepeatHookedEnding";
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
      result = "kMeasureKindIncompleteLastInRepeatHooklessEnding";
      break;
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
      result = "kMeasureKindIncompleteNextMeasureAfterCommonPart";
      break;
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
      result = "kMeasureKindIncompleteNextMeasureAfterHookedEnding";
      break;
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      result = "kMeasureKindIncompleteNextMeasureAfterHooklessEnding";
      break;
    case msrMeasureKind::kMeasureKindOvercomplete:
      result = "kMeasureKindOvercomplete";
      break;
    case msrMeasureKind::kMeasureKindCadenza:
      result = "kMeasureKindCadenza";
      break;
    case msrMeasureKind::kMeasureKindMusicallyEmpty:
      result = "kMeasureKindMusicallyEmpty";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrMeasureKind& elt)
{
  os << msrMeasureKindAsString (elt);
  return os;
}

string msrMeasureKindasShortStringForMeasuresSlices (
  msrMeasureKind measureKind)
{
  string result;

  switch (measureKind) {
    case msrMeasureKind::kMeasureKindUnknown:
      result = "***kMeasureKindUnknown***";
      break;
    case msrMeasureKind::kMeasureKindRegular:
      result = "kMeasureKindRegular";
      break;
    case msrMeasureKind::kMeasureKindAnacrusis:
      result = "kMeasureKindAnacrusis";
      break;
    case msrMeasureKind::kMeasureKindIncompleteStandalone:
      result = "kMeasureKindIncompleteStandalone";
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
      result = "kMeasureKindIncompleteLastInRepeatCommonPart";
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
      result = "kMeasureKindIncompleteLastInRepeatHookedEnding";
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
      result = "kMeasureKindIncompleteLastInRepeatHooklessEnding";
      break;
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
      result = "kMeasureKindIncompleteNextMeasureAfterCommonPart";
      break;
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
      result = "kMeasureKindIncompleteNextMeasureAfterHookedEnding";
      break;
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      result = "kMeasureKindIncompleteNextMeasureAfterHooklessEnding";
      break;
    case msrMeasureKind::kMeasureKindOvercomplete:
      result = "kMeasureKindOvercomplete";
      break;
    case msrMeasureKind::kMeasureKindCadenza:
      result = "kMeasureKindCadenza";
      break;
    case msrMeasureKind::kMeasureKindMusicallyEmpty:
      result = "kMeasureKindMusicallyEmpty";
      break;
  } // switch

  return result;
}

string msrMeasureImplicitKindAsString (
  msrMeasureImplicitKind measureImplicitKind)
{
  string result;

  switch (measureImplicitKind) {
    case msrMeasureImplicitKind::kMeasureImplicitKindYes:
      result = "kMeasureImplicitKindYes";
      break;
    case msrMeasureImplicitKind::kMeasureImplicitKindNo:
      result = "kMeasureImplicitKindNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrMeasureImplicitKind& elt)
{
  os << msrMeasureImplicitKindAsString (elt);
  return os;
}

// clefs
//______________________________________________________________________________
map<string, msrClefKind>
  gGlobalClefKindsMap;

list<string>
  gClefKindsNamesList;

string msrClefKindAsString (
  msrClefKind clefKind)
{
  string result;

  switch (clefKind) {
    case msrClefKind::k_NoClef:
      result = "***k_NoClef***";
      break;
    case msrClefKind::kClefTreble:
      result = "kClefTreble";
      break;
    case msrClefKind::kClefSoprano:
      result = "kClefSoprano";
      break;
    case msrClefKind::kClefMezzoSoprano:
      result = "kClefMezzoSoprano";
      break;
    case msrClefKind::kClefAlto:
      result = "kClefAlto";
      break;
    case msrClefKind::kClefTenor:
      result = "kClefTenor";
      break;
    case msrClefKind::kClefBaritone:
      result = "kClefBaritone";
      break;
    case msrClefKind::kClefBass:
      result = "kClefBass";
      break;
    case msrClefKind::kClefTrebleLine1:
      result = "kClefTrebleLine1";
      break;
    case msrClefKind::kClefTrebleMinus15:
      result = "kClefTrebleMinus15";
      break;
    case msrClefKind::kClefTrebleMinus8:
      result = "kClefTrebleMinus8";
      break;
    case msrClefKind::kClefTreblePlus8:
      result = "kClefTreblePlus8";
      break;
    case msrClefKind::kClefTreblePlus15:
      result = "kClefTreblePlus15";
      break;
    case msrClefKind::kClefBassMinus15:
      result = "kClefBassMinus15";
      break;
    case msrClefKind::kClefBassMinus8:
      result = "kClefBassMinus8";
      break;
    case msrClefKind::kClefBassPlus8:
      result = "kClefBassPlus8";
      break;
    case msrClefKind::kClefBassPlus15:
      result = "kClefBassPlus15";
      break;
    case msrClefKind::kClefVarbaritone:
      result = "kClefVarbaritone";
      break;
    case msrClefKind::kClefTablature4:
      result = "kClefTablature4";
      break;
    case msrClefKind::kClefTablature5:
      result = "kClefTablature5";
      break;
    case msrClefKind::kClefTablature6:
      result = "kClefTablature6";
      break;
    case msrClefKind::kClefTablature7:
      result = "kClefTablature7";
      break;
    case msrClefKind::kClefPercussion:
      result = "kClefPercussion";
      break;
    case msrClefKind::kClefJianpu:
      result = "kClefJianpu";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrClefKind& elt)
{
  os << msrClefKindAsString (elt);
  return os;
}

msrClefKind clefKindFromString (
  int           inputLineNumber,
  const string& clefString)
{
  msrClefKind result = msrClefKind::k_NoClef;

  if      (clefString == "treble")
    result = msrClefKind::kClefTreble;
  else if (clefString == "soprano")
    result = msrClefKind::kClefSoprano;
  else if (clefString == "mezzosoprano")
    result = msrClefKind::kClefMezzoSoprano;
  else if (clefString == "alto")
    result = msrClefKind::kClefAlto;
  else if (clefString == "tenor")
    result = msrClefKind::kClefTenor;
  else if (clefString == "baritone")
    result = msrClefKind::kClefBaritone;
  else if (clefString == "bass")
    result = msrClefKind::kClefBass;
  else if (clefString == "treble1")
    result = msrClefKind::kClefTrebleLine1;
  else if (clefString == "treble-15")
    result = msrClefKind::kClefTrebleMinus15;
  else if (clefString == "treble-8")
    result = msrClefKind::kClefTrebleMinus8;
  else if (clefString == "treble+8")
    result = msrClefKind::kClefTreblePlus8;
  else if (clefString == "treble+15")
    result = msrClefKind::kClefTreblePlus15;
  else if (clefString == "bass-15")
    result = msrClefKind::kClefBassMinus15;
  else if (clefString == "bass-8")
    result = msrClefKind::kClefBassMinus8;
  else if (clefString == "bass+8")
    result = msrClefKind::kClefBassPlus8;
  else if (clefString == "bass+15")
    result = msrClefKind::kClefBassPlus15;
  else if (clefString == "varbaritone")
    result = msrClefKind::kClefVarbaritone;
  else if (clefString == "tab4")
    result = msrClefKind::kClefTablature4;
  else if (clefString == "tab5")
    result = msrClefKind::kClefTablature5;
  else if (clefString == "tab6")
    result = msrClefKind::kClefTablature6;
  else if (clefString == "tab7")
    result = msrClefKind::kClefTablature7;
  else if (clefString == "percussion")
    result = msrClefKind::kClefPercussion;
  else if (clefString == "jianpu")
    result = msrClefKind::kClefJianpu;
  else {
    stringstream s;

    s <<
      "clef string \"" <<
      clefString <<
      "\" is unknown" <<
      ", line = " << inputLineNumber;

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void initializeClefKinds ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
  //  gGlobalClefKindsMap [gClefKindsNamesList.back ()] = kMajorClef;

    gClefKindsNamesList.push_back ("treble");
    gGlobalClefKindsMap ["treble"] = msrClefKind::kClefTreble;
    gClefKindsNamesList.push_back ("soprano");
    gGlobalClefKindsMap ["soprano"] = msrClefKind::kClefSoprano;
    gClefKindsNamesList.push_back ("mezzosoprano");
    gGlobalClefKindsMap ["mezzosoprano"] = msrClefKind::kClefMezzoSoprano;
    gClefKindsNamesList.push_back ("alto");
    gGlobalClefKindsMap ["alto"] = msrClefKind::kClefAlto;
    gClefKindsNamesList.push_back ("tenor");
    gGlobalClefKindsMap ["tenor"] = msrClefKind::kClefTenor;
    gClefKindsNamesList.push_back ("baritone");
    gGlobalClefKindsMap ["baritone"] = msrClefKind::kClefBaritone;
    gClefKindsNamesList.push_back ("bass");
    gGlobalClefKindsMap ["bass"] = msrClefKind::kClefBass;
    gClefKindsNamesList.push_back ("treble1");
    gGlobalClefKindsMap ["treble1"] = msrClefKind::kClefTrebleLine1;
    gClefKindsNamesList.push_back ("treble-15");
    gGlobalClefKindsMap ["treble-15"] = msrClefKind::kClefTrebleMinus15;
    gClefKindsNamesList.push_back ("treble-8");
    gGlobalClefKindsMap ["treble-8"] = msrClefKind::kClefTrebleMinus8;
    gClefKindsNamesList.push_back ("treble+8");
    gGlobalClefKindsMap ["treble+8"] = msrClefKind::kClefTreblePlus8;
    gClefKindsNamesList.push_back ("treble+15");
    gGlobalClefKindsMap ["treble+15"] = msrClefKind::kClefTreblePlus15;
    gClefKindsNamesList.push_back ("bass-15");
    gGlobalClefKindsMap ["bass-15"] = msrClefKind::kClefBassMinus15;
    gClefKindsNamesList.push_back ("bass-8");
    gGlobalClefKindsMap ["bass-8"] = msrClefKind::kClefBassMinus8;
    gClefKindsNamesList.push_back ("bass+8");
    gGlobalClefKindsMap ["bass+8"] = msrClefKind::kClefBassPlus8;
    gClefKindsNamesList.push_back ("bass+15");
    gGlobalClefKindsMap ["bass+15"] = msrClefKind::kClefBassPlus15;
    gClefKindsNamesList.push_back ("varbaritone");
    gGlobalClefKindsMap ["varbaritone"] = msrClefKind::kClefVarbaritone;
    gClefKindsNamesList.push_back ("tab4");
    gGlobalClefKindsMap ["tab4"] = msrClefKind::kClefTablature4;
    gClefKindsNamesList.push_back ("tab5");
    gGlobalClefKindsMap ["tab5"] = msrClefKind::kClefTablature5;
    gClefKindsNamesList.push_back ("tab6");
    gGlobalClefKindsMap ["tab6"] = msrClefKind::kClefTablature6;
    gClefKindsNamesList.push_back ("tab7");
    gGlobalClefKindsMap ["tab7"] = msrClefKind::kClefTablature7;
    gClefKindsNamesList.push_back ("percussion");
    gGlobalClefKindsMap ["percussion"] = msrClefKind::kClefPercussion;
    gClefKindsNamesList.push_back ("jianpu");
    gGlobalClefKindsMap ["jianpu"] = msrClefKind::kClefJianpu;

    pPrivateThisMethodHasBeenRun = true;
  }
}

string existingClefKinds (size_t namesListMaxLength)
{
  stringstream s;

  size_t clefKindsMapSize =
    gGlobalClefKindsMap.size ();

  if (clefKindsMapSize) {
    size_t nextToLast =
      clefKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      map<string, msrClefKind>::const_iterator i =
        gGlobalClefKindsMap.begin ();
      i != gGlobalClefKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != clefKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

string existingClefKindsNames (size_t namesListMaxLength)
{
  stringstream s;

  size_t clefKindsNamesMapSize =
    gClefKindsNamesList.size ();

  if (clefKindsNamesMapSize) {
    size_t nextToLast =
      clefKindsNamesMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      list<string>::const_iterator i =
        gClefKindsNamesList.begin ();
      i != gClefKindsNamesList.end ();
      ++i
    ) {
      string theString = (*i);

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != clefKindsNamesMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// keys
// ------------------------------------------------------

string msrKeyKindAsString (
  msrKeyKind keyKind)
{
  string result;

  switch (keyKind) {
    case msrKeyKind::kKeyTraditional:
      result = "kKeyTraditional";
      break;
    case msrKeyKind::kKeyHumdrumScot:
      result = "kKeyHumdrumScot";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrKeyKind& elt)
{
  os << msrKeyKindAsString (elt);
  return os;
}

string msrModeKindAsString (
  msrModeKind modeKind)
{
  string result;

  switch (modeKind) {
    case msrModeKind::k_NoMode:
      result = "***k_NoMode***";
      break;
    case msrModeKind::kModeMajor:
      result = "kModeMajor";
      break;
    case msrModeKind::kModeMinor:
      result = "kModeMinor";
      break;
    case msrModeKind::kModeIonian:
      result = "kModeIonian";
      break;
    case msrModeKind::kModeDorian:
      result = "kModeDorian";
      break;
    case msrModeKind::kModePhrygian:
      result = "kModePhrygian";
      break;
    case msrModeKind::kModeLydian:
      result = "kModeLydian";
      break;
    case msrModeKind::kModeMixolydian:
      result = "mixolkModeMixolydianydian";
      break;
    case msrModeKind::kModeAeolian:
      result = "kModeAeolian";
      break;
    case msrModeKind::kModeLocrian:
      result = "kModeLocrian";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrModeKind& elt)
{
  os << msrModeKindAsString (elt);
  return os;
}

msrModeKind modeKindFromString (
  int           inputLineNumber,
  const string& modeString)
{
  // no CamelCase here, these strings are used in the command line options
  msrModeKind result = msrModeKind::k_NoMode;

  if      (modeString == "major")
    result = msrModeKind::kModeMajor;
  else if (modeString == "minor")
    result = msrModeKind::kModeMinor;
  else if (modeString == "ionian")
    result = msrModeKind::kModeIonian;
  else if (modeString == "dorian")
    result = msrModeKind::kModeDorian;
  else if (modeString == "phrygian")
    result = msrModeKind::kModePhrygian;
  else if (modeString == "lydian")
    result = msrModeKind::kModeLydian;
  else if (modeString == "mixolydian")
    result = msrModeKind::kModeMixolydian;
  else if (modeString == "aeolian")
    result = msrModeKind::kModeAeolian;
  else if (modeString == "locrian")
    result = msrModeKind::kModeLocrian;
  else {
    stringstream s;

    s <<
      "mode string \"" <<
      modeString <<
      "\" is unknown" <<
      ", line = " << inputLineNumber;

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

// time signatures
//______________________________________________________________________________
string msrTimeSignatureSymbolKindAsString (
  msrTimeSignatureSymbolKind timeSignatureSymbolKind)
{
  string result;

  switch (timeSignatureSymbolKind) {
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon:
      result = "kTimeSignatureSymbolCommon";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut:
      result = "kTimeSignatureSymbolCut";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote:
      result = "kTimeSignatureSymbolNote";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote:
      result = "kTimeSignatureSymbolDottedNote";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber:
      result = "kTimeSignatureSymbolSingleNumber";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
      result = "kTimeSignatureSymbolSenzaMisura";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone:
      result = "kTimeSignatureSymbolNone";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTimeSignatureSymbolKind& elt)
{
  os << msrTimeSignatureSymbolKindAsString (elt);
  return os;
}

string msrTimeSignatureSeparatorKindAsString (
  msrTimeSignatureSeparatorKind timeSignatureSeparatorKind)
{
  string result;

  switch (timeSignatureSeparatorKind) {
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorNone:
      result = "kTimeSignatureSeparatorNone";
      break;
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorHorizontal:
      result = "kTimeSignatureSeparatorHorizontal";
      break;
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorDiagonal:
      result = "kTimeSignatureSeparatorDiagonal";
      break;
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorVertical:
      result = "kTimeSignatureSeparatorVertical";
      break;
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorAdjacent:
      result = "kTimeSignatureSeparatorAdjacent";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTimeSignatureSeparatorKind& elt)
{
  os << msrTimeSignatureSeparatorKindAsString (elt);
  return os;
}

string msrTimeSignatureRelationKindAsString (
  msrTimeSignatureRelationKind timeSignatureRelationKind)
{
  string result;

  switch (timeSignatureRelationKind) {
    case msrTimeSignatureRelationKind::kTimeSignatureRelationNone:
      result = "kTimeSignatureRelationNone";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationParentheses:
      result = "kTimeSignatureRelationParentheses";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationBracket:
      result = "kTimeSignatureRelationBracket";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationEquals:
      result = "kTimeSignatureRelationEquals";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationSlash:
      result = "kTimeSignatureRelationSlash";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationSpace:
      result = "kTimeSignatureRelationSpace";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationHyphen:
      result = "kTimeSignatureRelationHyphen";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTimeSignatureRelationKind& elt)
{
  os << msrTimeSignatureRelationKindAsString (elt);
  return os;
}

// repeats
//______________________________________________________________________________
string msrRepeatEndingKindAsString (
  msrRepeatEndingKind repeatEndingKind)
{
  string result;

  switch (repeatEndingKind) {
    case msrRepeatEndingKind::kRepeatEndingHooked:
      result = "kRepeatEndingHooked";
      break;
    case msrRepeatEndingKind::kRepeatEndingHookless:
      result = "kRepeatEndingHookless";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrRepeatEndingKind& elt)
{
  os << msrRepeatEndingKindAsString (elt);
  return os;
}

// harmonies
//______________________________________________________________________________
// constant
const int K_HARMONY_NO_INVERSION = -1;

string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind)
{
  string result;

  switch (harmonyKind) {
    case msrHarmonyKind::k_NoHarmony:
      result = "***k_NoHarmony***";
      break;

    // MusicXML harmonies

    case msrHarmonyKind::kHarmonyMajor:
      result = "kHarmonyMajor";
      break;
    case msrHarmonyKind::kHarmonyMinor:
      result = "kHarmonyMinor";
      break;
    case msrHarmonyKind::kHarmonyAugmented:
      result = "kHarmonyAugmented";
      break;
    case msrHarmonyKind::kHarmonyDiminished:
      result = "kHarmonyDiminished";
      break;

    case msrHarmonyKind::kHarmonyDominant:
      result = "kHarmonyDominant";
      break;
    case msrHarmonyKind::kHarmonyMajorSeventh:
      result = "kHarmonyMajorSeventh";
      break;
    case msrHarmonyKind::kHarmonyMinorSeventh:
      result = "kHarmonyMinorSeventh";
      break;
    case msrHarmonyKind::kHarmonyDiminishedSeventh:
      result = "kHarmonyDiminishedSeventh";
      break;
    case msrHarmonyKind::kHarmonyAugmentedSeventh:
      result = "kHarmonyAugmentedSeventh";
      break;
    case msrHarmonyKind::kHarmonyHalfDiminished:
      result = "kHarmonyHalfDiminished";
      break;
    case msrHarmonyKind::kHarmonyMinorMajorSeventh:
      result = "kHarmonyMinorMajorSeventh";
      break;

    case msrHarmonyKind::kHarmonyMajorSixth:
      result = "kHarmonyMajorSixth";
      break;
    case msrHarmonyKind::kHarmonyMinorSixth:
      result = "kHarmonyMinorSixth";
      break;

    case msrHarmonyKind::kHarmonyDominantNinth:
      result = "kHarmonyDominantNinth";
      break;
    case msrHarmonyKind::kHarmonyMajorNinth:
      result = "kHarmonyMajorNinth";
      break;
    case msrHarmonyKind::kHarmonyMinorNinth:
      result = "kHarmonyMinorNinth";
      break;

    case msrHarmonyKind::kHarmonyDominantEleventh:
      result = "kHarmonyDominantEleventh";
      break;
    case msrHarmonyKind::kHarmonyMajorEleventh:
      result = "kHarmonyMajorEleventh";
      break;
    case msrHarmonyKind::kHarmonyMinorEleventh:
      result = "kHarmonyMinorEleventh";
      break;

    case msrHarmonyKind::kHarmonyDominantThirteenth:
      result = "kHarmonyDominantThirteenth";
      break;
    case msrHarmonyKind::kHarmonyMajorThirteenth:
      result = "kHarmonyMajorThirteenth";
      break;
    case msrHarmonyKind::kHarmonyMinorThirteenth:
      result = "kHarmonyMinorThirteenth";
      break;

    case msrHarmonyKind::kHarmonySuspendedSecond:
      result = "kHarmonySuspendedSecond";
      break;
    case msrHarmonyKind::kHarmonySuspendedFourth:
      result = "kHarmonySuspendedFourth";
      break;

    case msrHarmonyKind::kHarmonyNeapolitan:
      result = "kHarmonyNeapolitan";
      break;
    case msrHarmonyKind::kHarmonyItalian:
      result = "kHarmonyItalian";
      break;
    case msrHarmonyKind::kHarmonyFrench:
      result = "kHarmonyFrench";
      break;
    case msrHarmonyKind::kHarmonyGerman:
      result = "kHarmonyGerman";
      break;

    case msrHarmonyKind::kHarmonyPedal:
      result = "kHarmonyPedal";
      break;
    case msrHarmonyKind::kHarmonyPower:
      result = "kHarmonyPower";
      break;
    case msrHarmonyKind::kHarmonyTristan:
      result = "kHarmonyTristan";
      break;

    // jazz-specific Harmonies

    case msrHarmonyKind::kHarmonyMinorMajorNinth: // -maj9, minmaj9
      result = "kHarmonyMinorMajorNinth";
      break;

    case msrHarmonyKind::kHarmonyDominantSuspendedFourth: // 7sus4, domsus4
      result = "kHarmonyDominantSuspendedFourth";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedFifth: // 7#5, domaug5
      result = "kHarmonyDominantAugmentedFifth";
      break;
    case msrHarmonyKind::kHarmonyDominantMinorNinth: // 7b9, dommin9
      result = "kHarmonyDominantMinorNinth";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedNinthDiminishedFifth: // 7#9b5, domaug9dim5
      result = "kHarmonyDominantAugmentedNinthDiminishedFifth";
      break;
    case msrHarmonyKind::kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth: // 7#9#5, domaug9aug5
      result = "kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedEleventh: // 7#11, domaug11
      result = "kHarmonyDominantAugmentedEleventh";
      break;

    case msrHarmonyKind::kHarmonyMajorSeventhAugmentedEleventh: // maj7#11, maj7aug11
      result = "kHarmonyMajorSeventhAugmentedEleventh";
      break;

    // other

    case msrHarmonyKind::kHarmonyOther:
      result = "kHarmonyOther";
      break;
    case msrHarmonyKind::kHarmonyNone:
      result = "kHarmonyNone";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrHarmonyKind& elt)
{
  os << msrHarmonyKindAsString (elt);
  return os;
}

string msrHarmonyKindAsShortString (
  msrHarmonyKind harmonyKind)
{
  string result;

  switch (harmonyKind) {
    case msrHarmonyKind::k_NoHarmony:
      result = "noHarmony";
      break;

    // MusicXML harmonies

    case msrHarmonyKind::kHarmonyMajor:
      result = "M";
      break;
    case msrHarmonyKind::kHarmonyMinor:
      result = "m";
      break;
    case msrHarmonyKind::kHarmonyAugmented:
      result = "+";
      break;
    case msrHarmonyKind::kHarmonyDiminished:
      result = "dim";
      break;

    case msrHarmonyKind::kHarmonyDominant:
      result = "7";
      break;
    case msrHarmonyKind::kHarmonyMajorSeventh:
      result = "∆7";
      break;
    case msrHarmonyKind::kHarmonyMinorSeventh:
      result = "m7";
      break;
    case msrHarmonyKind::kHarmonyDiminishedSeventh:
      result = "7/5-";
      break;
    case msrHarmonyKind::kHarmonyAugmentedSeventh:
      result = "7/5+";
      break;
    case msrHarmonyKind::kHarmonyHalfDiminished:
      result = "ø7";
      break;
    case msrHarmonyKind::kHarmonyMinorMajorSeventh:
      result = "m∆7";
      break;

    case msrHarmonyKind::kHarmonyMajorSixth:
      result = "6";
      break;
    case msrHarmonyKind::kHarmonyMinorSixth:
      result = "m6";
      break;

    case msrHarmonyKind::kHarmonyDominantNinth:
      result = "9";
      break;
    case msrHarmonyKind::kHarmonyMajorNinth:
      result = "∆7/9";
      break;
    case msrHarmonyKind::kHarmonyMinorNinth:
      result = "m∆7/9";
      break;

    case msrHarmonyKind::kHarmonyDominantEleventh:
      result = "11";
      break;
    case msrHarmonyKind::kHarmonyMajorEleventh:
      result = "∆7/11";
      break;
    case msrHarmonyKind::kHarmonyMinorEleventh:
      result = "m∆7/11";
      break;

    case msrHarmonyKind::kHarmonyDominantThirteenth:
      result = "13";
      break;
    case msrHarmonyKind::kHarmonyMajorThirteenth:
      result = "∆7/13";
      break;
    case msrHarmonyKind::kHarmonyMinorThirteenth:
      result = "m∆7/13";
      break;

    case msrHarmonyKind::kHarmonySuspendedSecond:
      result = "sus2";
      break;
    case msrHarmonyKind::kHarmonySuspendedFourth:
      result = "sus4";
      break;

    case msrHarmonyKind::kHarmonyNeapolitan:
      result = "Neapolitan";
      break;
    case msrHarmonyKind::kHarmonyItalian:
      result = "Italian";
      break;
    case msrHarmonyKind::kHarmonyFrench:
      result = "French";
      break;
    case msrHarmonyKind::kHarmonyGerman:
      result = "German";
      break;

    case msrHarmonyKind::kHarmonyPedal:
      result = "Pedal";
      break;
    case msrHarmonyKind::kHarmonyPower:
      result = "Power";
      break;
    case msrHarmonyKind::kHarmonyTristan:
      result = "Tristan";
      break;

    // jazz-specific Harmonies

    case msrHarmonyKind::kHarmonyMinorMajorNinth: // -maj9, minmaj9
      result = "m∆9";
      break;

    case msrHarmonyKind::kHarmonyDominantSuspendedFourth: // 7sus4, domsus4
      result = "7sus4";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedFifth: // 7#5, domaug5
      result = "7#5";
      break;
    case msrHarmonyKind::kHarmonyDominantMinorNinth: // 7b9, dommin9
      result = "7b9";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedNinthDiminishedFifth: // 7#9b5, domaug9dim5
      result = "7#9b5";
      break;
    case msrHarmonyKind::kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth: // 7#9#5, domaug9aug5
      result = "7#9#5";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedEleventh: // 7#11, domaug11
      result = "7#11";
      break;

    case msrHarmonyKind::kHarmonyMajorSeventhAugmentedEleventh: // maj7#11, maj7aug11
      result = "maj7#11";
      break;

    // other

    case msrHarmonyKind::kHarmonyOther:
      result = "kHarmonyOther";
      break;
    case msrHarmonyKind::kHarmonyNone:
      result = "kHarmonyNone";
      break;
  } // switch

  return result;
}

string msrHarmonyKindShortName (
  msrHarmonyKind harmonyKind)
{
  string result;

  switch (harmonyKind) {
    case msrHarmonyKind::k_NoHarmony:
      result = "noHarmony";
      break;

    // MusicXML harmonies

    case msrHarmonyKind::kHarmonyMajor:
      result = "maj";
      break;
    case msrHarmonyKind::kHarmonyMinor:
      result = "min";
      break;
    case msrHarmonyKind::kHarmonyAugmented:
      result = "aug";
      break;
    case msrHarmonyKind::kHarmonyDiminished:
      result = "dim";
      break;

    case msrHarmonyKind::kHarmonyDominant:
      result = "dom";
      break;
    case msrHarmonyKind::kHarmonyMajorSeventh:
      result = "maj";
      break;
    case msrHarmonyKind::kHarmonyMinorSeventh:
      result = "min7";
      break;
    case msrHarmonyKind::kHarmonyDiminishedSeventh:
      result = "dim7";
      break;
    case msrHarmonyKind::kHarmonyAugmentedSeventh:
      result = "aug7";
      break;
    case msrHarmonyKind::kHarmonyHalfDiminished:
      result = "halfdim";
      break;
    case msrHarmonyKind::kHarmonyMinorMajorSeventh:
      result = "minmaj7";
      break;

    case msrHarmonyKind::kHarmonyMajorSixth:
      result = "maj6";
      break;
    case msrHarmonyKind::kHarmonyMinorSixth:
      result = "min6";
      break;

    case msrHarmonyKind::kHarmonyDominantNinth:
      result = "dom9";
      break;
    case msrHarmonyKind::kHarmonyMajorNinth:
      result = "maj9";
      break;
    case msrHarmonyKind::kHarmonyMinorNinth:
      result = "min9";
      break;

    case msrHarmonyKind::kHarmonyDominantEleventh:
      result = "dom11";
      break;
    case msrHarmonyKind::kHarmonyMajorEleventh:
      result = "maj11";
      break;
    case msrHarmonyKind::kHarmonyMinorEleventh:
      result = "min11";
      break;

    case msrHarmonyKind::kHarmonyDominantThirteenth:
      result = "dom13";
      break;
    case msrHarmonyKind::kHarmonyMajorThirteenth:
      result = "maj13";
      break;
    case msrHarmonyKind::kHarmonyMinorThirteenth:
      result = "min13";
      break;

    case msrHarmonyKind::kHarmonySuspendedSecond:
      result = "sus2";
      break;
    case msrHarmonyKind::kHarmonySuspendedFourth:
      result = "sus4";
      break;

    case msrHarmonyKind::kHarmonyNeapolitan:
      result = "neapolitan";
      break;
    case msrHarmonyKind::kHarmonyItalian:
      result = "ttalian";
      break;
    case msrHarmonyKind::kHarmonyFrench:
      result = "french";
      break;
    case msrHarmonyKind::kHarmonyGerman:
      result = "german";
      break;

    case msrHarmonyKind::kHarmonyPedal:
      result = "pedal";
      break;
    case msrHarmonyKind::kHarmonyPower:
      result = "power";
      break;
    case msrHarmonyKind::kHarmonyTristan:
      result = "tristan";
      break;

    // jazz-specific Harmonies

    case msrHarmonyKind::kHarmonyMinorMajorNinth: // -maj9, minmaj9
      result = "minmaj9";
      break;

    case msrHarmonyKind::kHarmonyDominantSuspendedFourth: // 7sus4, domsus4
      result = "domsus4";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedFifth: // 7#5, domaug5
      result = "domaug5";
      break;
    case msrHarmonyKind::kHarmonyDominantMinorNinth: // 7b9, dommin9
      result = "dommin9";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedNinthDiminishedFifth: // 7#9b5, domaug9dim5
      result = "domaug9dim5";
      break;
    case msrHarmonyKind::kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth: // 7#9#5, domaug9aug5
      result = "domaug9aug5";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedEleventh: // 7#11, domaug11
      result = "domaug11";
      break;

    case msrHarmonyKind::kHarmonyMajorSeventhAugmentedEleventh: // maj7#11, maj7aug11
      result = "maj7aug11";
      break;

    // other

    case msrHarmonyKind::kHarmonyOther:
      result = "other";
      break;
    case msrHarmonyKind::kHarmonyNone:
      result = "[NONE]";
      break;
  } // switch

  return result;
}

msrHarmonyKind msrHarmonyKindFromString (
  const string& theString)
{
  msrHarmonyKind result = msrHarmonyKind::k_NoHarmony;

  // MusicXML harmonies

  if (theString == "maj") {
    result = msrHarmonyKind::kHarmonyMajor;
  }
  else if (theString == "min") {
    result = msrHarmonyKind::kHarmonyMinor;
  }
  else if (theString == "aug") {
    result = msrHarmonyKind::kHarmonyAugmented;
  }
  else if (theString == "dim") {
    result = msrHarmonyKind::kHarmonyDiminished;
  }
  else if (theString == "dom") {
    result = msrHarmonyKind::kHarmonyDominant;
  }
  else if (theString == "maj7") {
    result = msrHarmonyKind::kHarmonyMajorSeventh;
  }
  else if (theString == "min7") {
    result = msrHarmonyKind::kHarmonyMinorSeventh;
  }
  else if (theString == "dim7") {
    result = msrHarmonyKind::kHarmonyDiminishedSeventh;
  }
  else if (theString == "aug7") {
    result = msrHarmonyKind::kHarmonyAugmentedSeventh;
  }
  else if (theString == "halfdim") {
    result = msrHarmonyKind::kHarmonyHalfDiminished;
  }
  else if (theString == "minmaj7") {
    result = msrHarmonyKind::kHarmonyMinorMajorSeventh;
  }
  else if (theString == "maj6") {
    result = msrHarmonyKind::kHarmonyMajorSixth;
  }
  else if (theString == "min6") {
    result = msrHarmonyKind::kHarmonyMinorSixth;
  }
  else if (theString == "dom9") {
    result = msrHarmonyKind::kHarmonyDominantNinth;
  }
  else if (theString == "maj9") {
    result = msrHarmonyKind::kHarmonyMajorNinth;
  }
  else if (theString == "min9") {
    result = msrHarmonyKind::kHarmonyMinorNinth;
  }
  else if (theString == "dom11") {
    result = msrHarmonyKind::kHarmonyDominantEleventh;
  }
  else if (theString == "maj11") {
    result = msrHarmonyKind::kHarmonyMajorEleventh;
  }
  else if (theString == "min11") {
    result = msrHarmonyKind::kHarmonyMinorEleventh;
  }
  else if (theString == "dom13") {
    result = msrHarmonyKind::kHarmonyDominantThirteenth;
  }
  else if (theString == "maj13") {
    result = msrHarmonyKind::kHarmonyMajorThirteenth;
  }
  else if (theString == "min13") {
    result = msrHarmonyKind::kHarmonyMinorThirteenth;
  }
  else if (theString == "sus2") {
    result = msrHarmonyKind::kHarmonySuspendedSecond;
  }
  else if (theString == "sus4") {
    result = msrHarmonyKind::kHarmonySuspendedFourth;
  }
  else if (theString == "neapolitan") {
    result = msrHarmonyKind::kHarmonyNeapolitan;
  }
  else if (theString == "italian") {
    result = msrHarmonyKind::kHarmonyItalian;
  }
  else if (theString == "french") {
    result = msrHarmonyKind::kHarmonyFrench;
  }
  else if (theString == "german") {
    result = msrHarmonyKind::kHarmonyGerman;
  }
  else if (theString == "pedal") {
    result = msrHarmonyKind::kHarmonyPedal;
  }
  else if (theString == "power") {
    result = msrHarmonyKind::kHarmonyPower;
  }
  else if (theString == "tristan") {
    result = msrHarmonyKind::kHarmonyTristan;
  }

  // jazz-specific Harmonies

  else if (theString == "minmaj9") {
    result = msrHarmonyKind::kHarmonyMinorMajorNinth;                              // -maj9, minmaj9
  }
  else if (theString == "domsus4") {
    result = msrHarmonyKind::kHarmonyDominantSuspendedFourth;               // 7sus4, domsus4
  }
  else if (theString == "domaug5") {
    result = msrHarmonyKind::kHarmonyDominantAugmentedFifth;                // 7#5, domaug5
  }
  else if (theString == "dommin9") {
    result = msrHarmonyKind::kHarmonyDominantMinorNinth;                    // 7b9, dommin9
  }
  else if (theString == "domaug9dim5") {
    result = msrHarmonyKind::kHarmonyDominantAugmentedNinthDiminishedFifth; // 7#9b5, domaug9dim5
  }
  else if (theString == "domaug9aug5") {
    result = msrHarmonyKind::kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth;  // 7#9#5, domaug9aug5
  }
  else if (theString == "domaug11") {
    result = msrHarmonyKind::kHarmonyDominantAugmentedEleventh;             // 7#11, domaug11
  }
  else if (theString == "maj7aug11") {
    result = msrHarmonyKind::kHarmonyMajorSeventhAugmentedEleventh;         // maj7#11, maj7aug11
  }

  return result;
}

map<string, msrHarmonyKind>
  gGlobalHarmonyKindsMap;

list<string>
  gHarmonyKindsNamesList;

void initializeHarmonyKinds ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
    // MusicXML harmonies

    gHarmonyKindsNamesList.push_back ("maj"); // JMI
    gGlobalHarmonyKindsMap [gHarmonyKindsNamesList.back ()] = msrHarmonyKind::kHarmonyMajor;
    gGlobalHarmonyKindsMap ["min"] = msrHarmonyKind::kHarmonyMinor;
    gGlobalHarmonyKindsMap ["aug"] = msrHarmonyKind::kHarmonyAugmented;
    gGlobalHarmonyKindsMap ["dim"] = msrHarmonyKind::kHarmonyDiminished;
    gGlobalHarmonyKindsMap ["dom"] = msrHarmonyKind::kHarmonyDominant;
    gGlobalHarmonyKindsMap ["maj7"] = msrHarmonyKind::kHarmonyMajorSeventh;
    gGlobalHarmonyKindsMap ["min7"] = msrHarmonyKind::kHarmonyMinorSeventh;
    gGlobalHarmonyKindsMap ["dim7"] = msrHarmonyKind::kHarmonyDiminishedSeventh;
    gGlobalHarmonyKindsMap ["aug7"] = msrHarmonyKind::kHarmonyAugmentedSeventh;
    gGlobalHarmonyKindsMap ["halfdim"] = msrHarmonyKind::kHarmonyHalfDiminished;
    gGlobalHarmonyKindsMap ["minmaj7"] = msrHarmonyKind::kHarmonyMinorMajorSeventh;
    gGlobalHarmonyKindsMap ["maj6"] = msrHarmonyKind::kHarmonyMajorSixth;
    gGlobalHarmonyKindsMap ["min6"] = msrHarmonyKind::kHarmonyMinorSixth;
    gGlobalHarmonyKindsMap ["dom9"] = msrHarmonyKind::kHarmonyDominantNinth;
    gGlobalHarmonyKindsMap ["maj9"] = msrHarmonyKind::kHarmonyMajorNinth;
    gGlobalHarmonyKindsMap ["min9"] = msrHarmonyKind::kHarmonyMinorNinth;
    gGlobalHarmonyKindsMap ["dom11"] = msrHarmonyKind::kHarmonyDominantEleventh;
    gGlobalHarmonyKindsMap ["maj11"] = msrHarmonyKind::kHarmonyMajorEleventh;
    gGlobalHarmonyKindsMap ["min11"] = msrHarmonyKind::kHarmonyMinorEleventh;
    gGlobalHarmonyKindsMap ["dom13"] = msrHarmonyKind::kHarmonyDominantThirteenth;
    gGlobalHarmonyKindsMap ["maj13"] = msrHarmonyKind::kHarmonyMajorThirteenth;
    gGlobalHarmonyKindsMap ["min13"] = msrHarmonyKind::kHarmonyMinorThirteenth;
    gGlobalHarmonyKindsMap ["sus2"] = msrHarmonyKind::kHarmonySuspendedSecond;
    gGlobalHarmonyKindsMap ["sus4"] = msrHarmonyKind::kHarmonySuspendedFourth;
    gGlobalHarmonyKindsMap ["neapolitan"] = msrHarmonyKind::kHarmonyNeapolitan;
    gGlobalHarmonyKindsMap ["italian"] = msrHarmonyKind::kHarmonyItalian;
    gGlobalHarmonyKindsMap ["french"] = msrHarmonyKind::kHarmonyFrench;
    gGlobalHarmonyKindsMap ["german"] = msrHarmonyKind::kHarmonyGerman;
    gGlobalHarmonyKindsMap ["pedal"] = msrHarmonyKind::kHarmonyPedal;
    gGlobalHarmonyKindsMap ["power"] = msrHarmonyKind::kHarmonyPower;
    gGlobalHarmonyKindsMap ["tristan"] = msrHarmonyKind::kHarmonyTristan;

    // jazz-specific Harmonies

    gGlobalHarmonyKindsMap ["minmaj9"] = msrHarmonyKind::kHarmonyMinorMajorNinth;
    gGlobalHarmonyKindsMap ["domsus4"] = msrHarmonyKind::kHarmonyDominantSuspendedFourth;
    gGlobalHarmonyKindsMap ["domaug5"] = msrHarmonyKind::kHarmonyDominantAugmentedFifth;
    gGlobalHarmonyKindsMap ["dommin9"] = msrHarmonyKind::kHarmonyDominantMinorNinth;
    gGlobalHarmonyKindsMap ["domaug9dim5"] = msrHarmonyKind::kHarmonyDominantAugmentedNinthDiminishedFifth;
    gGlobalHarmonyKindsMap ["domaug9aug5"] = msrHarmonyKind::kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth;
    gGlobalHarmonyKindsMap ["domaug11"] = msrHarmonyKind::kHarmonyDominantAugmentedEleventh;
    gGlobalHarmonyKindsMap ["maj7aug11"] = msrHarmonyKind::kHarmonyMajorSeventhAugmentedEleventh;

    pPrivateThisMethodHasBeenRun = true;
  }
}

string existingHarmonyKinds (size_t namesListMaxLength)
{
  stringstream s;

  size_t harmonyKindsMapSize =
    gGlobalHarmonyKindsMap.size ();

  if (harmonyKindsMapSize) {
    size_t nextToLast =
      harmonyKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      map<string, msrHarmonyKind>::const_iterator i =
        gGlobalHarmonyKindsMap.begin ();
      i != gGlobalHarmonyKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != harmonyKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

string existingHarmonyKindsNames (size_t namesListMaxLength)
{
  stringstream s;

  size_t harmonyKindsNamesListSize =
    gHarmonyKindsNamesList.size ();

  if (harmonyKindsNamesListSize) {
    size_t nextToLast =
      harmonyKindsNamesListSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      list<string>::const_iterator i =
        gHarmonyKindsNamesList.begin ();
      i != gHarmonyKindsNamesList.end ();
      ++i
    ) {
      string theString = (*i);

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != harmonyKindsNamesListSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

void initializeHarmonyStructuresMap ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
    for (auto e : mfEnumAll<msrHarmonyKind> ()) {
      // create the harmony structure
      S_msrHarmonyStructure
        harmonyStructure =
          msrHarmonyStructure::create (
            e);

      // register it in the map
      gGlobalHarmonyStructuresMap [e] =
        harmonyStructure;
    } // for

    pPrivateThisMethodHasBeenRun = true;
  }
}

void printHarmonyStructuresMap ()
{
  gLogStream <<
    "Harmonies harmonies structures:" <<
    " (" << gGlobalHarmonyStructuresMap.size () << ")" <<
    endl;

  ++gIndenter;

  for (auto e : mfEnumAll<msrHarmonyKind> ()) {
    gLogStream <<
      msrHarmonyKindAsString (e) << ":" <<
      endl;

    ++gIndenter;

    S_msrHarmonyStructure
      harmonyStructure =
        gGlobalHarmonyStructuresMap [e];

    if (harmonyStructure) {
      gLogStream <<
        harmonyStructure <<
        endl;
    }
    else {
      gLogStream <<
        "no intervals" <<
        endl;
    }

    --gIndenter;

    gLogStream << endl;
  } // for

  --gIndenter;

  gLogStream << endl;
}

string msrDiatonicPitchKindAsString (
  msrDiatonicPitchKind diatonicPitchKind)
{
  string result;

  switch (diatonicPitchKind) {
    case msrDiatonicPitchKind::k_NoDiatonicPitch:
      result = "***k_NoDiatonicPitch***";
      break;

    case msrDiatonicPitchKind::kDiatonicPitchA:
      result = "A";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchB:
      result = "B";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchC:
      result = "C";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchD:
      result = "D";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchE:
      result = "E";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchF:
      result = "F";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchG:
      result = "G";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrDiatonicPitchKind& elt)
{
  os << msrDiatonicPitchKindAsString (elt);
  return os;
}

msrDiatonicPitchKind msrDiatonicPitchKindFromChar (
  char diatonicNoteName)
{
  msrDiatonicPitchKind result = msrDiatonicPitchKind::k_NoDiatonicPitch;

  switch (diatonicNoteName) {
    case 'a':
    case 'A':
      result = msrDiatonicPitchKind::kDiatonicPitchA;
       break;
    case 'b':
    case 'B':
      result = msrDiatonicPitchKind::kDiatonicPitchB;
      break;
    case 'c':
    case 'C':
      result = msrDiatonicPitchKind::kDiatonicPitchC;
      break;
    case 'd':
    case 'D':
      result = msrDiatonicPitchKind::kDiatonicPitchD;
      break;
    case 'e':
    case 'E':
      result = msrDiatonicPitchKind::kDiatonicPitchE;
      break;
    case 'f':
    case 'F':
      result = msrDiatonicPitchKind::kDiatonicPitchF;
      break;
    case 'g':
    case 'G':
      result = msrDiatonicPitchKind::kDiatonicPitchG;
      break;
    default: {}
  } // switch

  return result;
}

// alterations
//______________________________________________________________________________
msrAlterationKind msrAlterationKindFromMusicXMLAlter (
  float alter)
{

/*
  The alter element represents chromatic alteration
  in number of semitones (e.g., -1 for flat, 1 for sharp).
  Decimal values like 0.5 (quarter tone sharp) are used for microtones:

  semi-sharp semi-flat sesqui-sharp sesqui-flat double-sharp double-flat
      +0.5      -0.5        +1.5       -1.5         +2.0        -2.0
*/

  msrAlterationKind result = msrAlterationKind::k_NoAlteration;

  if      (alter == 0 ) {
    result = msrAlterationKind::kAlterationNatural;
  }

  else if (alter == -1 ) {
    result = msrAlterationKind::kAlterationFlat;
  }

  else if (alter == 1 ) {
    result = msrAlterationKind::kAlterationSharp;
  }

  else if (alter == -0.5 ) {
    result = msrAlterationKind::kAlterationSemiFlat;
  }

  else if (alter == +0.5 ) {
    result = msrAlterationKind::kAlterationSemiSharp;
  }

  else if (alter == -1.5 ) {
    result = msrAlterationKind::kAlterationSesquiFlat;
  }

  else if (alter == +1.5 ) {
    result = msrAlterationKind::kAlterationSesquiSharp;
  }

  else if (alter == -2 ) {
    result = msrAlterationKind::kAlterationDoubleFlat;
  }

  else if (alter == +2 ) {
    result = msrAlterationKind::kAlterationDoubleSharp;
  }

  else if (alter == -3 ) {
    result = msrAlterationKind::kAlterationTripleFlat;
  }

  else if (alter == +3 ) {
    result = msrAlterationKind::kAlterationTripleSharp;
  }

  return result;
}

float msrMusicXMLAlterFromAlterationKind (
  msrAlterationKind alterationKind)
{
  float result = -111.0;

  switch (alterationKind) {
    case msrAlterationKind::k_NoAlteration:
      break;

    case msrAlterationKind::kAlterationTripleFlat:
      result = -3;
      break;
    case msrAlterationKind::kAlterationDoubleFlat:
      result = -2;
      break;
    case msrAlterationKind::kAlterationSesquiFlat:
      result = -1.5;
      break;
    case msrAlterationKind::kAlterationFlat:
      result = -1;
      break;
    case msrAlterationKind::kAlterationSemiFlat:
      result = -0.5;
      break;
    case msrAlterationKind::kAlterationNatural:
      result = 0;
      break;
    case msrAlterationKind::kAlterationSemiSharp:
      result = +0.5;
      break;
    case msrAlterationKind::kAlterationSharp:
      result = 1;
      break;
    case msrAlterationKind::kAlterationSesquiSharp:
      result = +1.5;
      break;
    case msrAlterationKind::kAlterationDoubleSharp:
      result = +2;
      break;
    case msrAlterationKind::kAlterationTripleSharp:
      result = +3;
      break;
  } // switch

  return result;
}

string msrAlterationKindAsString (
  msrAlterationKind alterationKind)
{
  string result;

  switch (alterationKind) {
    case msrAlterationKind::k_NoAlteration:
      result = "***noAlteration***";
      break;

    case msrAlterationKind::kAlterationTripleFlat:
      result = "kAlterationTripleFlat";
      break;
    case msrAlterationKind::kAlterationDoubleFlat:
      result = "kAlterationDoubleFlat";
      break;
    case msrAlterationKind::kAlterationSesquiFlat:
      result = "kAlterationSesquiFlat";
      break;
    case msrAlterationKind::kAlterationFlat:
      result = "kAlterationFlat";
      break;
    case msrAlterationKind::kAlterationSemiFlat:
      result = "kAlterationSemiFlat";
      break;
    case msrAlterationKind::kAlterationNatural:
      result = "kAlterationNatural";
      break;
    case msrAlterationKind::kAlterationSemiSharp:
      result = "kAlterationSemiSharp";
      break;
    case msrAlterationKind::kAlterationSharp:
      result = "kAlterationSharp";
      break;
    case msrAlterationKind::kAlterationSesquiSharp:
      result = "kAlterationSesquiSharp";
      break;
    case msrAlterationKind::kAlterationDoubleSharp:
      result = "kAlterationDoubleSharp";
      break;
    case msrAlterationKind::kAlterationTripleSharp:
      result = "kAlterationTripleSharp";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrAlterationKind& elt)
{
  os << msrAlterationKindAsString (elt);
  return os;
}

// accidentals
//______________________________________________________________________________
string msrAccidentalKindAsString (
  msrAccidentalKind accidentalKind)
{
  string result;

  switch (accidentalKind) {
    case msrAccidentalKind::kAccidentalNone:
      result = "accidentalNone";
      break;

    case msrAccidentalKind::kAccidentalSharp:
      result = "kAccidentalSharp";
      break;
    case msrAccidentalKind::kAccidentalNatural:
      result = "kAccidentalNatural";
      break;
    case msrAccidentalKind::kAccidentalFlat:
      result = "kAccidentalFlat";
      break;
    case msrAccidentalKind::kAccidentalDoubleSharp:
      result = "kAccidentalDoubleSharp";
      break;
    case msrAccidentalKind::kAccidentalSharpSharp:
      result = "kAccidentalSharpSharp";
      break;
    case msrAccidentalKind::kAccidentalFlatFlat:
      result = "kAccidentalFlatFlat";
      break;
    case msrAccidentalKind::kAccidentalNaturalSharp:
      result = "kAccidentalNaturalSharp";
      break;
    case msrAccidentalKind::kAccidentalNaturalFlat:
      result = "kAccidentalNaturalFlat";
      break;
    case msrAccidentalKind::kAccidentalQuarterFlat:
      result = "kAccidentalQuarterFlat";
      break;
    case msrAccidentalKind::kAccidentalQuarterSharp:
      result = "kAccidentalQuarterSharp";
      break;
    case msrAccidentalKind::kAccidentalThreeQuartersFlat:
      result = "kAccidentalThreeQuartersFlat";
      break;
    case msrAccidentalKind::kAccidentalThreeQuartersSharp:
      result = "kAccidentalThreeQuartersSharp";
      break;

    case msrAccidentalKind::kAccidentalSharpDown:
      result = "kAccidentalSharpDown";
      break;
    case msrAccidentalKind::kAccidentalSharpUp:
      result = "kAccidentalSharpUp";
      break;
    case msrAccidentalKind::kAccidentalNaturalDown:
      result = "kAccidentalNaturalDown";
      break;
    case msrAccidentalKind::kAccidentalNaturalUp:
      result = "kAccidentalNaturalUp";
      break;
    case msrAccidentalKind::kAccidentalFlatDown:
      result = "kAccidentalFlatDown";
      break;
    case msrAccidentalKind::kAccidentalFlatUp:
      result = "kAccidentalFlatUp";
      break;
    case msrAccidentalKind::kAccidentalTripleSharp:
      result = "kAccidentalTripleSharp";
      break;
    case msrAccidentalKind::kAccidentalTripleFlat:
      result = "kAccidentalTripleFlat";
      break;
    case msrAccidentalKind::kAccidentalSlashQuarterSharp:
      result = "kAccidentalSlashQuarterSharp";
      break;
    case msrAccidentalKind::kAccidentalSlashSharp:
      result = "kAccidentalSlashSharp";
      break;
    case msrAccidentalKind::kAccidentalSlashFlat:
      result = "kAccidentalSlashFlat";
      break;
    case msrAccidentalKind::kAccidentalDoubleSlashFlat:
      result = "kAccidentaldoubleSlashFlat";
      break;
    case msrAccidentalKind::kAccidentalSharp_1:
      result = "kAccidentalSharp_1";
      break;
    case msrAccidentalKind::kAccidentalSharp_2:
      result = "kAccidentalSharp_2";
      break;
    case msrAccidentalKind::kAccidentalSharp_3:
      result = "kAccidentalSharp_3";
      break;
    case msrAccidentalKind::kAccidentalSharp_5:
      result = "kAccidentalSharp_5";
      break;
    case msrAccidentalKind::kAccidentalFlat_1:
      result = "kAccidentalFlat_1";
      break;
    case msrAccidentalKind::kAccidentalFlat_2:
      result = "kAccidentalFlat_2";
      break;
    case msrAccidentalKind::kAccidentalFlat_3:
      result = "kAccidentalFlat_3";
      break;
    case msrAccidentalKind::kAccidentalFlat_4:
      result = "kAccidentalFlat_4";
      break;
    case msrAccidentalKind::kAccidentalSori:
      result = "kAccidentalSori";
      break;
    case msrAccidentalKind::kAccidentalKoron:
      result = "kAccidentalKoron";
      break;

    case msrAccidentalKind::kAccidentalOther:
      result = "kAccidentalOther";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrAccidentalKind& elt)
{
  os <<msrAccidentalKindAsString (elt);
  return os;
}

string msrAccidentalKindAsMusicXMLString (
  msrAccidentalKind accidentalKind)
{
  string result;

  switch (accidentalKind) {
    case msrAccidentalKind::kAccidentalNone:
      break;

    case msrAccidentalKind::kAccidentalSharp:
      result = "sharp";
      break;
    case msrAccidentalKind::kAccidentalNatural:
      result = "natural";
      break;
    case msrAccidentalKind::kAccidentalFlat:
      result = "flat";
      break;
    case msrAccidentalKind::kAccidentalDoubleSharp:
      result = "double-sharp";
      break;
    case msrAccidentalKind::kAccidentalSharpSharp:
      result = "sharp-sharp";
      break;
    case msrAccidentalKind::kAccidentalFlatFlat:
      result = "flat-flat";
      break;
    case msrAccidentalKind::kAccidentalNaturalSharp:
      result = "natural-sharp";
      break;
    case msrAccidentalKind::kAccidentalNaturalFlat:
      result = "natural-flat";
      break;
    case msrAccidentalKind::kAccidentalQuarterFlat:
      result = "quarter-flat";
      break;
    case msrAccidentalKind::kAccidentalQuarterSharp:
      result = "quarter-sharp";
      break;
    case msrAccidentalKind::kAccidentalThreeQuartersFlat:
      result = "three-quarters-flat";
      break;
    case msrAccidentalKind::kAccidentalThreeQuartersSharp:
      result = "three-quarters-sharp";
      break;

    case msrAccidentalKind::kAccidentalSharpDown:
      result = "sharp-down";
      break;
    case msrAccidentalKind::kAccidentalSharpUp:
      result = "sharp-up";
      break;
    case msrAccidentalKind::kAccidentalNaturalDown:
      result = "natural-down";
      break;
    case msrAccidentalKind::kAccidentalNaturalUp:
      result = "natural-up";
      break;
    case msrAccidentalKind::kAccidentalFlatDown:
      result = "flat-down";
      break;
    case msrAccidentalKind::kAccidentalFlatUp:
      result = "flat-up";
      break;
    case msrAccidentalKind::kAccidentalTripleSharp:
      result = "triple-sharp";
      break;
    case msrAccidentalKind::kAccidentalTripleFlat:
      result = "triple-flat";
      break;
    case msrAccidentalKind::kAccidentalSlashQuarterSharp:
      result = "slash-quarter-sharp";
      break;
    case msrAccidentalKind::kAccidentalSlashSharp:
      result = "slash-sharp";
      break;
    case msrAccidentalKind::kAccidentalSlashFlat:
      result = "slash-flat";
      break;
    case msrAccidentalKind::kAccidentalDoubleSlashFlat:
      result = "double-slash-flat";
      break;
    case msrAccidentalKind::kAccidentalSharp_1:
      result = "sharp-1";
      break;
    case msrAccidentalKind::kAccidentalSharp_2:
      result = "sharp-2";
      break;
    case msrAccidentalKind::kAccidentalSharp_3:
      result = "sharp-3";
      break;
    case msrAccidentalKind::kAccidentalSharp_5:
      result = "sharp-5";
      break;
    case msrAccidentalKind::kAccidentalFlat_1:
      result = "flat-1";
      break;
    case msrAccidentalKind::kAccidentalFlat_2:
      result = "flat-2";
      break;
    case msrAccidentalKind::kAccidentalFlat_3:
      result = "flat-3";
      break;
    case msrAccidentalKind::kAccidentalFlat_4:
      result = "flat-4";
      break;
    case msrAccidentalKind::kAccidentalSori:
      result = "sori";
      break;
    case msrAccidentalKind::kAccidentalKoron:
      result = "koron";
      break;

    case msrAccidentalKind::kAccidentalOther:
      result = "other";
      break;
  } // switch

  return result;
}

// editorial accidentals
//______________________________________________________________________________

string msrEditorialAccidentalKindAsString (
  msrEditorialAccidentalKind noteEditorialAccidentalKind)
{
  string result;

  switch (noteEditorialAccidentalKind) {
    case msrEditorialAccidentalKind::kEditorialAccidentalYes:
      result = "kEditorialAccidentalYes";
      break;
    case msrEditorialAccidentalKind::kEditorialAccidentalNo:
      result = "kEditorialAccidentalNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrEditorialAccidentalKind& elt)
{
  os <<msrEditorialAccidentalKindAsString (elt);
  return os;
}

// cautionary accidentals
//______________________________________________________________________________

string msrCautionaryAccidentalKindAsString (
  msrCautionaryAccidentalKind noteCautionaryAccidentalKind)
{
  string result;

  switch (noteCautionaryAccidentalKind) {
    case msrCautionaryAccidentalKind::kCautionaryAccidentalYes:
      result = "kCautionaryAccidentalYes";
      break;
    case msrCautionaryAccidentalKind::kCautionaryAccidentalNo:
      result = "kCautionaryAccidentalNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrCautionaryAccidentalKind& elt)
{
  os <<msrCautionaryAccidentalKindAsString (elt);
  return os;
}

//______________________________________________________________________________
void setDiatonicPitchAndAlterationKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind)
{
  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::k_NoQuarterTonesPitch:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA; // any value would fit
      alterationKind    = msrAlterationKind::k_NoAlteration;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_B_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_C_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_D_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_E_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_F_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_G_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_Rest:
    case msrQuarterTonesPitchKind::kQTP_Skip:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA; // any value would fit JMI
      alterationKind    = msrAlterationKind::k_NoAlteration;

      break;
  } // switch
}

string msrQuarterTonesPitchKindAsString (
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  string result;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::k_NoQuarterTonesPitch:
      result = "***noQuarterTonesPitch***";
      break;

    case msrQuarterTonesPitchKind::kQTP_A_TripleFlat:
      result = "kQTP_A_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_DoubleFlat:
      result = "kQTP_A_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SesquiFlat:
      result = "kQTP_A_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Flat:
      result = "kQTP_A_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SemiFlat:
      result = "kQTP_A_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Natural:
      result = "kQTP_A_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SemiSharp:
      result = "kQTP_A_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Sharp:
      result = "kQTP_A_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SesquiSharp:
      result = "kQTP_A_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_DoubleSharp:
      result = "kQTP_A_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_TripleSharp:
      result = "kQTP_A_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_B_TripleFlat:
      result = "kQTP_B_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_DoubleFlat:
      result = "kQTP_B_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SesquiFlat:
      result = "kQTP_B_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Flat:
      result = "kQTP_B_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SemiFlat:
      result = "kQTP_B_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Natural:
      result = "kQTP_B_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SemiSharp:
      result = "kQTP_B_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Sharp:
      result = "kQTP_B_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SesquiSharp:
      result = "kQTP_B_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_DoubleSharp:
      result = "kQTP_B_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_TripleSharp:
      result = "kQTP_B_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_C_TripleFlat:
      result = "kQTP_C_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_DoubleFlat:
      result = "kQTP_C_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SesquiFlat:
      result = "kQTP_C_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Flat:
      result = "kQTP_C_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SemiFlat:
      result = "kQTP_C_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Natural:
      result = "kQTP_C_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SemiSharp:
      result = "kQTP_C_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Sharp:
      result = "kQTP_C_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SesquiSharp:
      result = "kQTP_C_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_DoubleSharp:
      result = "kQTP_C_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_TripleSharp:
      result = "kQTP_C_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_D_TripleFlat:
      result = "kQTP_D_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_DoubleFlat:
      result = "kQTP_D_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SesquiFlat:
      result = "kQTP_D_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Flat:
      result = "kQTP_D_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SemiFlat:
      result = "kQTP_D_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Natural:
      result = "kQTP_D_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SemiSharp:
      result = "kQTP_D_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Sharp:
      result = "kQTP_D_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SesquiSharp:
      result = "kQTP_D_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_DoubleSharp:
      result = "kQTP_D_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_TripleSharp:
      result = "kQTP_D_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_E_TripleFlat:
      result = "kQTP_E_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_DoubleFlat:
      result = "kQTP_E_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SesquiFlat:
      result = "kQTP_E_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Flat:
      result = "kQTP_E_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SemiFlat:
      result = "kQTP_E_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Natural:
      result = "kQTP_E_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SemiSharp:
      result = "kQTP_E_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Sharp:
      result = "kQTP_E_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SesquiSharp:
      result = "kQTP_E_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_DoubleSharp:
      result = "kQTP_E_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_TripleSharp:
      result = "kQTP_E_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_F_TripleFlat:
      result = "kQTP_F_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_DoubleFlat:
      result = "kQTP_F_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SesquiFlat:
      result = "kQTP_F_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Flat:
      result = "kQTP_F_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SemiFlat:
      result = "kQTP_F_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Natural:
      result = "kQTP_F_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SemiSharp:
      result = "kQTP_F_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Sharp:
      result = "kQTP_F_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SesquiSharp:
      result = "kQTP_F_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_DoubleSharp:
      result = "kQTP_F_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_TripleSharp:
      result = "kQTP_F_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_G_TripleFlat:
      result = "kQTP_G_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_DoubleFlat:
      result = "kQTP_G_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SesquiFlat:
      result = "kQTP_G_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Flat:
      result = "kQTP_G_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SemiFlat:
      result = "kQTP_G_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Natural:
      result = "kQTP_G_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SemiSharp:
      result = "kQTP_G_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Sharp:
      result = "kQTP_G_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SesquiSharp:
      result = "kQTP_G_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_DoubleSharp:
      result = "kQTP_G_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_TripleSharp:
      result = "kQTP_G_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_Rest:
      result = "kQTP_Rest";
      break;
    case msrQuarterTonesPitchKind::kQTP_Skip:
      result = "kQTP_Skip";
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrQuarterTonesPitchKind& elt)
{
  os << msrQuarterTonesPitchKindAsString (elt);
  return os;
}

void fetchDiatonicPitchKindAndAlterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind)
{
  diatonicPitchKind =
    diatonicPitchKindFromQuarterTonesPitchKind (
      inputLineNumber,
      quarterTonesPitchKind);

  alterationKind  =
    alterationKindFromQuarterTonesPitchKind (
      inputLineNumber,
      quarterTonesPitchKind);
}

msrQuarterTonesPitchKind quarterTonesPitchKindFromDiatonicPitchAndAlteration (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind)
{
  msrQuarterTonesPitchKind
    result =
      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch;

  switch (diatonicPitchKind) {
    case msrDiatonicPitchKind::k_NoDiatonicPitch:
      {
        result = msrQuarterTonesPitchKind::k_NoQuarterTonesPitch;
        /* JMI
        stringstream s;

        s <<
          "cannot convert msrDiatonicPitchKind::k_NoDiatonicPitch to a quarter tones pitch"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;

    case msrDiatonicPitchKind::kDiatonicPitchA:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_TripleSharp;
          break;
        case msrAlterationKind::k_NoAlteration:
          {
            stringstream s;

            s <<
              "'A' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchB:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleSharp;
          break;
        case msrAlterationKind::k_NoAlteration:
          {
            stringstream s;

            s <<
              "'B' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

      break;

    case msrDiatonicPitchKind::kDiatonicPitchC:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_TripleSharp;
          break;
        case msrAlterationKind::k_NoAlteration:
          {
            stringstream s;

            s <<
              "'C' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchD:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_TripleSharp;
          break;
        case msrAlterationKind::k_NoAlteration:
          {
            stringstream s;

            s <<
              "'D' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchE:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_TripleSharp;
          break;
        case msrAlterationKind::k_NoAlteration:
          {
            stringstream s;

            s <<
              "'E' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchF:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleSharp;
          break;
        case msrAlterationKind::k_NoAlteration:
          {
            stringstream s;

            s <<
              "'F' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchG:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_TripleSharp;
          break;
        case msrAlterationKind::k_NoAlteration:
          {
            stringstream s;

            s <<
              "'G' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;
  } // switch

  return result;
}

msrDiatonicPitchKind diatonicPitchKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrDiatonicPitchKind result = msrDiatonicPitchKind::k_NoDiatonicPitch;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_A_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_A_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_A_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_A_Flat:
    case msrQuarterTonesPitchKind::kQTP_A_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_A_Natural:
    case msrQuarterTonesPitchKind::kQTP_A_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_A_Sharp:
    case msrQuarterTonesPitchKind::kQTP_A_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_A_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_A_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchA;
      break;

    case msrQuarterTonesPitchKind::kQTP_B_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_B_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_B_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_B_Flat:
    case msrQuarterTonesPitchKind::kQTP_B_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_B_Natural:
    case msrQuarterTonesPitchKind::kQTP_B_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_B_Sharp:
    case msrQuarterTonesPitchKind::kQTP_B_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_B_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_B_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchB;
      break;

    case msrQuarterTonesPitchKind::kQTP_C_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_C_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_C_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_C_Flat:
    case msrQuarterTonesPitchKind::kQTP_C_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_C_Natural:
    case msrQuarterTonesPitchKind::kQTP_C_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_C_Sharp:
    case msrQuarterTonesPitchKind::kQTP_C_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_C_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_C_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchC;
      break;

    case msrQuarterTonesPitchKind::kQTP_D_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_D_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_D_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_D_Flat:
    case msrQuarterTonesPitchKind::kQTP_D_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_D_Natural:
    case msrQuarterTonesPitchKind::kQTP_D_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_D_Sharp:
    case msrQuarterTonesPitchKind::kQTP_D_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_D_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_D_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchD;
      break;

    case msrQuarterTonesPitchKind::kQTP_E_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_E_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_E_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_E_Flat:
    case msrQuarterTonesPitchKind::kQTP_E_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_E_Natural:
    case msrQuarterTonesPitchKind::kQTP_E_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_E_Sharp:
    case msrQuarterTonesPitchKind::kQTP_E_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_E_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_E_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchE;
      break;

    case msrQuarterTonesPitchKind::kQTP_F_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_F_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_F_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_F_Flat:
    case msrQuarterTonesPitchKind::kQTP_F_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_F_Natural:
    case msrQuarterTonesPitchKind::kQTP_F_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_F_Sharp:
    case msrQuarterTonesPitchKind::kQTP_F_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_F_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_F_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchF;
      break;

    case msrQuarterTonesPitchKind::kQTP_G_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_G_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_G_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_G_Flat:
    case msrQuarterTonesPitchKind::kQTP_G_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_G_Natural:
    case msrQuarterTonesPitchKind::kQTP_G_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_G_Sharp:
    case msrQuarterTonesPitchKind::kQTP_G_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_G_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_G_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchG;
      break;

    case msrQuarterTonesPitchKind::kQTP_Rest:
      {
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a rest"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

    case msrQuarterTonesPitchKind::kQTP_Skip:
      {
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a skip"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

    case msrQuarterTonesPitchKind::k_NoQuarterTonesPitch:
      {
        result = msrDiatonicPitchKind::k_NoDiatonicPitch;

        /* JMI
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a msrQuarterTonesPitchKind::k_NoQuarterTonesPitch"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
  } // switch

  return result;
}

msrAlterationKind alterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrAlterationKind result = msrAlterationKind::k_NoAlteration;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_A_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_B_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_C_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_D_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_E_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_F_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_G_TripleFlat:
      result = msrAlterationKind::kAlterationTripleFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_B_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_C_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_D_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_E_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_F_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_G_DoubleFlat:
      result = msrAlterationKind::kAlterationDoubleFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_B_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_C_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_D_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_E_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_F_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_G_SesquiFlat:
      result = msrAlterationKind::kAlterationSesquiFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_Flat:
    case msrQuarterTonesPitchKind::kQTP_B_Flat:
    case msrQuarterTonesPitchKind::kQTP_C_Flat:
    case msrQuarterTonesPitchKind::kQTP_D_Flat:
    case msrQuarterTonesPitchKind::kQTP_E_Flat:
    case msrQuarterTonesPitchKind::kQTP_F_Flat:
    case msrQuarterTonesPitchKind::kQTP_G_Flat:
      result = msrAlterationKind::kAlterationFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_B_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_C_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_D_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_E_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_F_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_G_SemiFlat:
      result = msrAlterationKind::kAlterationSemiFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_Natural:
    case msrQuarterTonesPitchKind::kQTP_B_Natural:
    case msrQuarterTonesPitchKind::kQTP_C_Natural:
    case msrQuarterTonesPitchKind::kQTP_D_Natural:
    case msrQuarterTonesPitchKind::kQTP_E_Natural:
    case msrQuarterTonesPitchKind::kQTP_F_Natural:
    case msrQuarterTonesPitchKind::kQTP_G_Natural:
      result = msrAlterationKind::kAlterationNatural;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_B_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_C_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_D_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_E_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_F_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_G_SemiSharp:
      result = msrAlterationKind::kAlterationSemiSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_Sharp:
    case msrQuarterTonesPitchKind::kQTP_B_Sharp:
    case msrQuarterTonesPitchKind::kQTP_C_Sharp:
    case msrQuarterTonesPitchKind::kQTP_D_Sharp:
    case msrQuarterTonesPitchKind::kQTP_E_Sharp:
    case msrQuarterTonesPitchKind::kQTP_F_Sharp:
    case msrQuarterTonesPitchKind::kQTP_G_Sharp:
      result = msrAlterationKind::kAlterationSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_B_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_C_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_D_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_E_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_F_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_G_SesquiSharp:
      result = msrAlterationKind::kAlterationSesquiSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_B_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_C_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_D_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_E_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_F_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_G_DoubleSharp:
      result = msrAlterationKind::kAlterationDoubleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_B_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_C_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_D_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_E_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_F_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_G_TripleSharp:
      result = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_Rest:
      {
        stringstream s;

        s <<
          "cannot get the alteration kind of a rest"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

    case msrQuarterTonesPitchKind::kQTP_Skip:
      {
        stringstream s;

        s <<
          "cannot get the alteration kind of a skip"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

    case msrQuarterTonesPitchKind::k_NoQuarterTonesPitch:
      {
        result = msrAlterationKind::k_NoAlteration;

        /* JMI
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a msrQuarterTonesPitchKind::k_NoQuarterTonesPitch"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
  } // switch

  return result;
}

msrQuarterTonesPitchKind quarterTonesPitchKindFromSemiTonesPitchKind (
  msrSemiTonesPitchKind semiTonesPitchKind)
{
  msrQuarterTonesPitchKind result = msrQuarterTonesPitchKind::k_NoQuarterTonesPitch;

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      break;

    case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_C_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      result = msrQuarterTonesPitchKind::kQTP_C_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_C_Natural:
      result = msrQuarterTonesPitchKind::kQTP_C_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_C_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_D_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      result = msrQuarterTonesPitchKind::kQTP_D_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_D_Natural:
      result = msrQuarterTonesPitchKind::kQTP_D_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_D_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_E_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      result = msrQuarterTonesPitchKind::kQTP_E_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_E_Natural:
      result = msrQuarterTonesPitchKind::kQTP_E_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_E_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_E_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_F_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
      result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_F_Flat:
      result = msrQuarterTonesPitchKind::kQTP_F_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_F_Natural:
      result = msrQuarterTonesPitchKind::kQTP_F_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_F_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_G_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      result = msrQuarterTonesPitchKind::kQTP_G_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_G_Natural:
      result = msrQuarterTonesPitchKind::kQTP_G_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_G_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_A_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      result = msrQuarterTonesPitchKind::kQTP_A_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_A_Natural:
      result = msrQuarterTonesPitchKind::kQTP_A_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_A_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      result = msrQuarterTonesPitchKind::kQTP_B_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_B_Natural:
      result = msrQuarterTonesPitchKind::kQTP_B_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_B_TripleSharp;
      break;
  } // switch

  return result;
}

msrSemiTonesPitchKind semiTonesPitchKindFromQuarterTonesPitchKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrSemiTonesPitchKind result = msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch;

    switch (quarterTonesPitchKind) {
      case msrQuarterTonesPitchKind::k_NoQuarterTonesPitch:
        break;

      case msrQuarterTonesPitchKind::kQTP_Rest:
      case msrQuarterTonesPitchKind::kQTP_Skip:
        break;

      case msrQuarterTonesPitchKind::kQTP_A_Flat:
        result = msrSemiTonesPitchKind::kSTP_A_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_A_Natural:
        result = msrSemiTonesPitchKind::kSTP_A_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_A_Sharp:
        result = msrSemiTonesPitchKind::kSTP_A_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_B_Flat:
        result = msrSemiTonesPitchKind::kSTP_B_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_B_Natural:
        result = msrSemiTonesPitchKind::kSTP_B_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_B_Sharp:
        result = msrSemiTonesPitchKind::kSTP_B_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_C_Flat:
        result = msrSemiTonesPitchKind::kSTP_C_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_C_Natural:
        result = msrSemiTonesPitchKind::kSTP_C_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_C_Sharp:
        result = msrSemiTonesPitchKind::kSTP_C_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_D_Flat:
        result = msrSemiTonesPitchKind::kSTP_D_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_D_Natural:
        result = msrSemiTonesPitchKind::kSTP_D_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_D_Sharp:
        result = msrSemiTonesPitchKind::kSTP_D_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_E_Flat:
        result = msrSemiTonesPitchKind::kSTP_E_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_E_Natural:
        result = msrSemiTonesPitchKind::kSTP_E_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_E_Sharp:
        result = msrSemiTonesPitchKind::kSTP_E_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_F_Flat:
        result = msrSemiTonesPitchKind::kSTP_F_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_F_Natural:
        result = msrSemiTonesPitchKind::kSTP_F_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_F_Sharp:
        result = msrSemiTonesPitchKind::kSTP_F_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_G_Flat:
        result = msrSemiTonesPitchKind::kSTP_G_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_G_Natural:
        result = msrSemiTonesPitchKind::kSTP_G_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_G_Sharp:
        result = msrSemiTonesPitchKind::kSTP_G_Sharp;
        break;

      default:
        ;
    } // switch

  return result;
}

msrSemiTonesPitchKind semiTonesPitchKindFromString (
  const string& theString)
{
  msrSemiTonesPitchKind result;

  // fetch the quarternotes pitches kind
  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromString (
        gGlobalMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind (),
        theString);

  // fetch the semitones pitches kind
  result =
    semiTonesPitchKindFromQuarterTonesPitchKind (
      quarterTonesPitchKind);

  return result;
}

string existingQuarterTonesPitchesLanguageKinds (size_t namesListMaxLength)
{
  stringstream s;

  size_t
    quarterTonesPitchesLanguageKindsMapSize =
      getQuarterTonesPitchesLanguageKindsMap ().size ();

  if (quarterTonesPitchesLanguageKindsMapSize) {
    size_t
      nextToLast =
        quarterTonesPitchesLanguageKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator i =
        getQuarterTonesPitchesLanguageKindsMap ().begin ();
      i != getQuarterTonesPitchesLanguageKindsMap ().end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != quarterTonesPitchesLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// semi tones pitches
//______________________________________________________________________________

string msrSemiTonesPitchKindAsString (
  msrSemiTonesPitchKind semiTonesPitchKind)
{
  string result;

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = "***kSTP_NoSemiTonesPitch***";
      break;

    case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
      result = "kSTP_C_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
      result = "kSTP_C_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      result = "kSTP_C_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Natural:
      result = "kSTP_C_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      result = "kSTP_C_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
      result = "kSTP_C_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
      result = "kSTP_C_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
      result = "kSTP_D_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
      result = "kSTP_D_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      result = "kSTP_D_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Natural:
      result = "kSTP_D_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      result = "kSTP_D_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
      result = "kSTP_D_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
      result = "kSTP_D_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
      result = "kSTP_E_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
      result = "kSTP_E_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      result = "kSTP_E_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Natural:
      result = "kSTP_E_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Sharp:
      result = "kSTP_E_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
      result = "kSTP_E_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
      result = "kSTP_E_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
      result = "kSTP_F_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
      result = "kSTP_F_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Flat:
      result = "kSTP_F_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Natural:
      result = "kSTP_F_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      result = "kSTP_F_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
      result = "kSTP_F_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
      result = "kSTP_F_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
      result = "kSTP_G_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
      result = "kSTP_G_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      result = "kSTP_G_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Natural:
      result = "kSTP_G_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      result = "kSTP_G_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
      result = "kSTP_G_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
      result = "kSTP_G_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
      result = "kSTP_A_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
      result = "kSTP_A_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      result = "kSTP_A_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Natural:
      result = "kSTP_A_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      result = "kSTP_A_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
      result = "kSTP_A_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
      result = "kSTP_A_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
      result = "kSTP_B_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
      result = "kSTP_B_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      result = "kSTP_B_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Natural:
      result = "kSTP_B_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      result = "kSTP_B_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
      result = "kSTP_B_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
      result = "kSTP_B_TripleSharp";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSemiTonesPitchKind& elt)
{
  os << msrSemiTonesPitchKindAsString (elt);
  return os;
}

/* JMI
string semiTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind)
{
  string result;

/ * JMI
  msrDiatonicPitchKind
    diatonicPitchKind =
      diatonicPitchKindFromQuarterTonesPitchKind (
        inputLineNumber,
        quarterTonesPitchKind);
        * /

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = "***kSTP_NoSemiTonesPitch***";
      break;

    case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
      result = "C_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
      result = "C_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      result = "C_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Natural:
      result = "C_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      result = "C_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
      result = "C_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
      result = "C_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
      result = "D_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
      result = "D_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      result = "D_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Natural:
      result = "D_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      result = "D_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
      result = "D_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
      result = "D_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
      result = "E_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
      result = "E_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      result = "E_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Natural:
      result = "E_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Sharp:
      result = "E_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
      result = "E_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
      result = "E_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
      result = "F_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
      result = "F_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Flat:
      result = "F_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Natural:
      result = "F_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      result = "F_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
      result = "F_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
      result = "F_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
      result = "G_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
      result = "G_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      result = "G_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Natural:
      result = "G_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      result = "G_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
      result = "G_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
      result = "G_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
      result = "A_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
      result = "A_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      result = "A_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Natural:
      result = "A_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      result = "A_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
      result = "A_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
      result = "A_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
      result = "B_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
      result = "B_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      result = "B_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Natural:
      result = "B_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      result = "B_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
      result = "B_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
      result = "B_TripleSharp";
      break;
  } // switch

  return result;
}
*/

string msrAlterationPreferenceKindAsString (
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  string result;

  switch (alterationPreferenceKind) {
    case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
      result = "kAlterationPreferenceFlat";
      break;
    case msrAlterationPreferenceKind::kAlterationPreferenceNatural:
      result = "kAlterationPreferenceNatural";
      break;
    case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
      result = "kAlterationPreferenceSharp";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrAlterationPreferenceKind& elt)
{
  os << msrAlterationPreferenceKindAsString (elt);
  return os;
}

msrQuarterTonesPitchKind msrSemiTonesPitchKindAsQuarterTonesPitchKind (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  msrQuarterTonesPitchKind
    result =
      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch;

  /* JMI
  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = msrQuarterTonesPitchKind::k_NoQuarterTonesPitch;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Natural: // msrSemiTonesPitchKind::kSTP_B_Sharp, msrSemiTonesPitchKind::kSTP_D_DoubleFlat
      result = kC_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Sharp: // msrSemiTonesPitchKind::kSTP_B_DoubleSharp, msrSemiTonesPitchKind::kSTP_D_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = kC_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = kD_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_D_Natural: // msrSemiTonesPitchKind::kSTP_C_DoubleSharp, msrSemiTonesPitchKind::kSTP_E_DoubleFlat
      result = kD_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_D_Sharp: // msrSemiTonesPitchKind::kSTP_E_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = kD_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = kE_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_E_Natural: // msrSemiTonesPitchKind::kSTP_D_DoubleSharp, msrSemiTonesPitchKind::kSTP_F_Flat
      result = kE_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_F_Natural: // msrSemiTonesPitchKind::kSTP_E_Sharp, msrSemiTonesPitchKind::kSTP_G_DoubleFlat
      result = kF_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_F_Sharp: // msrSemiTonesPitchKind::kSTP_E_DoubleSharp, msrSemiTonesPitchKind::kSTP_G_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = kF_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = kG_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_G_Natural: // msrSemiTonesPitchKind::kSTP_F_DoubleSharp, msrSemiTonesPitchKind::kSTP_A_DoubleFlat
      result = kG_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_G_Sharp: // msrSemiTonesPitchKind::kSTP_A_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = kG_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_A_Natural: // msrSemiTonesPitchKind::kSTP_G_DoubleSharp, msrSemiTonesPitchKind::kSTP_B_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_A_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_A_Sharp: // msrSemiTonesPitchKind::kSTP_B_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = kB_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_B_Natural: // msrSemiTonesPitchKind::kSTP_A_DoubleSharp, msrSemiTonesPitchKind::kSTP_C_Flat
      result = kB_Natural;
      break;
  } // switch
  */


  /* JMI
  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = msrQuarterTonesPitchKind::k_NoQuarterTonesPitch;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Natural: // msrSemiTonesPitchKind::kSTP_B_Sharp, msrSemiTonesPitchKind::kSTP_D_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_C_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Sharp: // msrSemiTonesPitchKind::kSTP_B_DoubleSharp, msrSemiTonesPitchKind::kSTP_D_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_D_Natural: // msrSemiTonesPitchKind::kSTP_C_DoubleSharp, msrSemiTonesPitchKind::kSTP_E_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_D_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_D_Sharp: // msrSemiTonesPitchKind::kSTP_E_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_E_Natural: // msrSemiTonesPitchKind::kSTP_D_DoubleSharp, msrSemiTonesPitchKind::kSTP_F_Flat
      result = msrQuarterTonesPitchKind::kQTP_E_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_F_Natural: // msrSemiTonesPitchKind::kSTP_E_Sharp, msrSemiTonesPitchKind::kSTP_G_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_F_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_F_Sharp: // msrSemiTonesPitchKind::kSTP_E_DoubleSharp, msrSemiTonesPitchKind::kSTP_G_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_G_Natural: // msrSemiTonesPitchKind::kSTP_F_DoubleSharp, msrSemiTonesPitchKind::kSTP_A_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_G_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_G_Sharp: // msrSemiTonesPitchKind::kSTP_A_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_A_Natural: // msrSemiTonesPitchKind::kSTP_G_DoubleSharp, msrSemiTonesPitchKind::kSTP_B_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_A_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_A_Sharp: // msrSemiTonesPitchKind::kSTP_B_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_B_Natural: // msrSemiTonesPitchKind::kSTP_A_DoubleSharp, msrSemiTonesPitchKind::kSTP_C_Flat
      result = msrQuarterTonesPitchKind::kQTP_B_Natural;
      break;
  } // switch
  */

  return result;
}

string existingMsrQuarterTonesPitchesLanguageKinds (
  size_t namesListMaxLength)
{
  stringstream s;

  size_t
    quarterTonesPitchesLanguageKindsMapSize =
      getQuarterTonesPitchesLanguageKindsMap ().size ();

  if (quarterTonesPitchesLanguageKindsMapSize) {
    size_t nextToLast =
      quarterTonesPitchesLanguageKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator i =
        getQuarterTonesPitchesLanguageKindsMap ().begin ();
      i != getQuarterTonesPitchesLanguageKindsMap ().end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
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
      else if (count != quarterTonesPitchesLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// notes
//______________________________________________________________________________
string msrNoteKindAsString (
  msrNoteKind noteKind)
{
  string result;

  switch (noteKind) {
    case msrNoteKind::k_NoNote:
      result = "***k_NoNote***";
      break;

    // in measures
    case msrNoteKind::kNoteRegularInMeasure:
      result = "kNoteRegularInMeasure";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      result = "kNoteRestInMeasure";
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      result = "kNoteSkipInMeasure";
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = "kNoteUnpitchedInMeasure";
      break;

    // in chords
    case msrNoteKind::kNoteRegularInChord:
      result = "kNoteRegularInChord";
      break;

    // in tuplets
    case msrNoteKind::kNoteRegularInTuplet:
      result = "kNoteRegularInTuplet";
      break;

    case msrNoteKind::kNoteRestInTuplet:
      result = "kNoteRestInTuplet";
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      result = "kNoteUnpitchedInTuplet";
      break;

    // in grace notes groups
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      result = "kNoteRegularInGraceNotesGroup";
      break;
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      result = "kNoteSkipInGraceNotesGroup";
      break;

    // in chords in grace notes groups
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      result = "kNoteInChordInGraceNotesGroup";
      break;

    // in tuplets in grace notes groups
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      result = "kNoteInTupletInGraceNotesGroup";
      break;

    // in double-tremolos
    case msrNoteKind::kNoteInDoubleTremolo:
      result = "kNoteInDoubleTremolo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrNoteKind& elt)
{
  os << msrNoteKindAsString (elt);
  return os;
}

// solo notes or rests
//______________________________________________________________________________

string msrSoloNoteOrRestInVoiceKindAsString (
  msrSoloNoteOrRestInVoiceKind soloNoteOrRestInVoiceKind)
{
  string result;

  switch (soloNoteOrRestInVoiceKind) {
    case msrSoloNoteOrRestInVoiceKind::kSoloNoteOrRestInVoiceYes:
      result = "kSoloNoteOrRestInVoiceYes";
      break;
    case msrSoloNoteOrRestInVoiceKind::kSoloNoteOrRestInVoiceNo:
      result = "kSoloNoteOrRestInVoiceNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSoloNoteOrRestInVoiceKind& elt)
{
  os << msrSoloNoteOrRestInVoiceKindAsString (elt);
  return os;
}

string msrSoloNoteOrRestInStaffKindAsString (
  msrSoloNoteOrRestInStaffKind soloNoteOrRestInStaffKind)
{
  string result;

  switch (soloNoteOrRestInStaffKind) {
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffYes:
      result = "kSoloNoteOrRestInStaffYes";
      break;
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffPartially:
      result = "kSoloNoteOrRestInStaffPartially";
      break;
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffNo:
      result = "kSoloNoteOrRestInStaffNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSoloNoteOrRestInStaffKind& elt)
{
  os << msrSoloNoteOrRestInStaffKindAsString (elt);
  return os;
}

// chords
//______________________________________________________________________________

string msrChordInKindAsString (
  msrChordInKind chordInKind)
{
  string result;

  switch (chordInKind) {
    case msrChordInKind::k_NoChordIn:
      result = "***k_NoChordIn***";
      break;
    case msrChordInKind::kChordInMeasure:
      result = "kChordInMeasure";
      break;
    case msrChordInKind::kChordInTuplet:
      result = "kChordInTuplet";
      break;
    case msrChordInKind::kChordInGraceNotesGroup:
      result = "kChordInGraceNotesGroup";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrChordInKind& elt)
{
  os << msrChordInKindAsString (elt);
  return os;
}

// tuplets
//______________________________________________________________________________

EXP string msrTupletInKindAsString (
  msrTupletInKind tupletInKind)
{
  string result;

  switch (tupletInKind) {
    case msrTupletInKind::k_NoTupletIn:
      result = "***k_NoTupletIn***";
      break;
    case msrTupletInKind::kTupletInMeasure:
      result = "kTupletInMeasure";
      break;
    case msrTupletInKind::kTupletInTuplet:
      result = "kTupletInTuplet";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTupletInKind& elt)
{
  os << msrTupletInKindAsString (elt);
  return os;
}

// beams
//______________________________________________________________________________
string msrBeamKindAsString (
  msrBeamKind beamKind)
{
  string result;

  switch (beamKind) {
    case msrBeamKind::k_NoBeam:
      result = "***k_NoBeam***";
      break;
    case msrBeamKind::kBeamBegin:
      result = "kBeamBegin";
      break;
    case msrBeamKind::kBeamContinue:
      result = "kBeamContinue";
      break;
    case msrBeamKind::kBeamEnd:
      result = "kBeamEnd";
      break;
    case msrBeamKind::kBeamForwardHook:
      result = "kBeamForwardHook";
      break;
    case msrBeamKind::kBeamBackwardHook:
      result = "kBeamBackwardHook";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrBeamKind& elt)
{
  os << msrBeamKindAsString (elt);
  return os;
}

// ties
// ------------------------------------------------------
string msrTieKindAsString (msrTieKind tieKind)
{
  stringstream s;

  switch (tieKind) {
    case msrTieKind::kTieNone:
      s << "***kTieNone***";
      break;
    case msrTieKind::kTieStart:
      s << "kTieStart";
      break;
    case msrTieKind::kTieContinue:
      s << "kTieContinue";
      break;
    case msrTieKind::kTieStop:
      s << "kTieStop";
      break;
  } // switch

  return s.str ();
}

ostream& operator << (ostream& os, const msrTieKind& elt)
{
  os << msrTieKindAsString (elt);
  return os;
}

// slurs
// ------------------------------------------------------
string msrSlurTypeKindAsString (
  msrSlurTypeKind slurTypeKind)
{
  stringstream s;

  switch (slurTypeKind) {
    case msrSlurTypeKind::k_NoSlur:
      s << "***k_NoSlur***";
      break;

    case msrSlurTypeKind::kSlurTypeRegularStart:
      s << "kSlurTypeRegularStart";
      break;
    case msrSlurTypeKind::kSlurTypeRegularContinue:
      s << "kSlurTypeRegularContinue";
      break;
    case msrSlurTypeKind::kSlurTypeRegularStop:
      s << "kSlurTypeRegularStop";
      break;

    case msrSlurTypeKind::kSlurTypePhrasingStart:
      s << "kSlurTypePhrasingStart";
      break;
    case msrSlurTypeKind::kSlurTypePhrasingContinue:
      s << "kSlurTypePhrasingContinue";
      break;
    case msrSlurTypeKind::kSlurTypePhrasingStop:
      s << "kSlurTypePhrasingStop";
      break;
  } // switch

  return s.str ();
}

ostream& operator << (ostream& os, const msrSlurTypeKind& elt)
{
  os << msrSlurTypeKindAsString (elt);
  return os;
}

// enharmonies
//______________________________________________________________________________
msrSemiTonesPitchKind enharmonicSemiTonesPitch (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  msrSemiTonesPitchKind result = semiTonesPitchKind;

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      result = msrSemiTonesPitchKind::kSTP_B_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      result = msrSemiTonesPitchKind::kSTP_D_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      result = msrSemiTonesPitchKind::kSTP_C_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_D_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      result = msrSemiTonesPitchKind::kSTP_E_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      result = msrSemiTonesPitchKind::kSTP_D_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_E_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_F_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      result = msrSemiTonesPitchKind::kSTP_G_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      result = msrSemiTonesPitchKind::kSTP_F_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_G_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      result = msrSemiTonesPitchKind::kSTP_A_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      result = msrSemiTonesPitchKind::kSTP_G_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_A_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      result = msrSemiTonesPitchKind::kSTP_B_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      result = msrSemiTonesPitchKind::kSTP_A_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_B_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      result = msrSemiTonesPitchKind::kSTP_C_Natural;
      break;

    default:
      ;
  } // switch

  return result;
}

// length units
//______________________________________________________________________________

map<string, msrLengthUnitKind>
  gGlobalMsrLengthUnitKindsMap;

string msrLengthUnitKindAsString (
  msrLengthUnitKind lengthUnitKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (lengthUnitKind) {
    case msrLengthUnitKind::kUnitInch:
      result = "kUnitInch";
      break;
    case msrLengthUnitKind::kUnitCentimeter:
      result = "kUnitCentimeter";
      break;
    case msrLengthUnitKind::kUnitMillimeter: // default value
      result = "kUnitMillimeter";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrLengthUnitKind& elt)
{
  os << msrLengthUnitKindAsString (elt);
  return os;
}

void initializeMsrLengthUnitKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalMsrLengthUnitKindsMap ["in"] = msrLengthUnitKind::kUnitInch;
  gGlobalMsrLengthUnitKindsMap ["cm"] = msrLengthUnitKind::kUnitCentimeter;
  gGlobalMsrLengthUnitKindsMap ["mm"] = msrLengthUnitKind::kUnitMillimeter;
}

string existingMsrLengthUnitKinds (size_t namesListMaxLength)
{
  stringstream s;

  size_t msrLengthUnitKindsMapSize =
    gGlobalMsrLengthUnitKindsMap.size ();

  if (msrLengthUnitKindsMapSize) {
    size_t nextToLast =
      msrLengthUnitKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      map<string, msrLengthUnitKind>::const_iterator i =
        gGlobalMsrLengthUnitKindsMap.begin ();
      i != gGlobalMsrLengthUnitKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != msrLengthUnitKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// lengths
//______________________________________________________________________________
S_msrLength msrLength::create (
  msrLengthUnitKind lengthUnitKind,
  float             lengthValue)
{
  msrLength * o =
    new msrLength (
      lengthUnitKind,
      lengthValue);
  assert (o != nullptr);

  return o;
}

msrLength::msrLength (
  msrLengthUnitKind lengthUnitKind,
  float             lengthValue)
{
  fLengthUnitKind = lengthUnitKind;
  fLengthValue    = lengthValue;
}

msrLength::msrLength ()
{
  fLengthUnitKind = msrLengthUnitKind::kUnitMillimeter;
  fLengthValue    = 0.0;
}

msrLength::~msrLength ()
{}

void msrLength::convertToLengthUnit (
  msrLengthUnitKind lengthUnitKind)
{
  if (fLengthUnitKind != lengthUnitKind) {
    switch (lengthUnitKind) {
      case msrLengthUnitKind::kUnitInch:
        switch (fLengthUnitKind) {
          case msrLengthUnitKind::kUnitInch:
            break;
          case msrLengthUnitKind::kUnitCentimeter:
            fLengthValue /= 2.54f;
            break;
          case msrLengthUnitKind::kUnitMillimeter:
            fLengthValue /= 25.4f;
            break;
        } // switch
        break;

      case msrLengthUnitKind::kUnitCentimeter:
        switch (fLengthUnitKind) {
          case msrLengthUnitKind::kUnitInch:
            fLengthValue *= 2.54f;
            break;
          case msrLengthUnitKind::kUnitCentimeter:
            break;
          case msrLengthUnitKind::kUnitMillimeter:
            fLengthValue /= 10;
            break;
        } // switch
        break;

      case msrLengthUnitKind::kUnitMillimeter:
        switch (fLengthUnitKind) {
          case msrLengthUnitKind::kUnitInch:
            fLengthValue *= 25.4f;
            break;
          case msrLengthUnitKind::kUnitCentimeter:
            fLengthValue *= 10;
            break;
          case msrLengthUnitKind::kUnitMillimeter:
            break;
        } // switch
        break;
    } // switch

    fLengthUnitKind = lengthUnitKind;
  }
}

string msrLength::asString () const
{
  stringstream s;

  s <<
    "[Length " <<
    setprecision (4) <<
    fLengthValue <<
    ' ' <<
    msrLengthUnitKindAsString (fLengthUnitKind) <<
    ']';

  return s.str ();
}

void msrLength::print (ostream& os) const
{
  os <<
    asString (); // JMI
};

ostream& operator << (ostream& os, const msrLength& elt)
{
  elt.print (os);
  return os;
}

ostream& operator << (ostream& os, const S_msrLength& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

// margins types
//______________________________________________________________________________

map<string, msrMarginTypeKind>
  gGlobalMsrMarginTypeKindsMap;

string msrMarginTypeKindAsString (
  msrMarginTypeKind marginTypeKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (marginTypeKind) {
    case msrMarginTypeKind::kMarginOdd:
      result = "odd";
      break;
    case msrMarginTypeKind::kMarginEven:
      result = "even";
      break;
    case msrMarginTypeKind::kMarginBoth: // default value
      result = "both";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrMarginTypeKind& elt)
{
  os << msrMarginTypeKindAsString (elt);
  return os;
}

void initializeMsrMarginTypeKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalMsrMarginTypeKindsMap ["odd"] = msrMarginTypeKind::kMarginOdd;
  gGlobalMsrMarginTypeKindsMap ["even"] = msrMarginTypeKind::kMarginEven;
  gGlobalMsrMarginTypeKindsMap ["both"] = msrMarginTypeKind::kMarginBoth;
}

string existingMsrMarginTypeKinds (size_t namesListMaxLength)
{
  stringstream s;

  size_t msrMarginTypeKindsMapSize =
    gGlobalMsrMarginTypeKindsMap.size ();

  if (msrMarginTypeKindsMapSize) {
    size_t nextToLast =
      msrMarginTypeKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      map<string, msrMarginTypeKind>::const_iterator i =
        gGlobalMsrMarginTypeKindsMap.begin ();
      i != gGlobalMsrMarginTypeKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != msrMarginTypeKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// margins
//______________________________________________________________________________
S_msrMargin msrMargin::create (
  msrMarginTypeKind marginTypeKind,
  msrLength         marginLength)
{
  msrMargin * o =
    new msrMargin (
      marginTypeKind,
      marginLength);
  assert (o != nullptr);

  return o;
}

msrMargin::msrMargin (
  msrMarginTypeKind marginTypeKind,
  msrLength         marginLength)
{
  fMarginTypeKind = marginTypeKind;
  fMarginLength   = marginLength;
}

msrMargin::~msrMargin ()
{}

string msrMargin::asString () const
{
  stringstream s;

  s <<
    "[Margin, " <<
    setprecision (4) <<
    fMarginLength.asString () <<
    ' ' <<
    msrMarginTypeKindAsString (fMarginTypeKind) <<
    ']';

  return s.str ();
}

void msrMargin::print (ostream& os) const
{
  os <<
    asString ();
};

ostream& operator << (ostream& os, const S_msrMargin& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

// margins groups
//______________________________________________________________________________
S_msrMarginsGroup msrMarginsGroup::create (
  msrMarginTypeKind marginTypeKind)
{
  msrMarginsGroup * o =
    new msrMarginsGroup (
      marginTypeKind);
  assert (o != nullptr);

  return o;
}

msrMarginsGroup::msrMarginsGroup (
  msrMarginTypeKind marginTypeKind)
{
  fMarginsGroupTypeKind = marginTypeKind;
}

msrMarginsGroup::~msrMarginsGroup ()
{}

void msrMarginsGroup::setLeftMargin (
  int         inputLineNumber,
  S_msrMargin val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " left margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fLeftMargin = val;
}

void msrMarginsGroup::setRightMargin (
  int         inputLineNumber,
  S_msrMargin val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " right margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fRightMargin = val;
}

void msrMarginsGroup::setTopMargin (
  int         inputLineNumber,
  S_msrMargin val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " top margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fTopMargin = val;
}

void msrMarginsGroup::setBottomMargin (
  int         inputLineNumber,
  S_msrMargin val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " bottom margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fBottomMargin = val;
}

/* JMI
void msrMarginsGroup::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMarginsGroup::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMarginsGroup>*
    p =
      dynamic_cast<visitor<S_msrMarginsGroup>*> (v)) {
        S_msrMarginsGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMarginsGroup::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMarginsGroup::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMarginsGroup::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMarginsGroup>*
    p =
      dynamic_cast<visitor<S_msrMarginsGroup>*> (v)) {
        S_msrMarginsGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMarginsGroup::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMarginsGroup::browseData (basevisitor* v)
{}
*/

string msrMarginsGroup::asString () const
{
  stringstream s;

  s <<
    "[MarginsGroup, " <<
    msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
    ", leftMargin: " << fLeftMargin->asString () <<
    ", rightMargin: " << fRightMargin->asString () <<
    ", topMargin: " << fTopMargin->asString () <<
    ", bottomMargin: " << fBottomMargin->asString () <<
    ']';

  return s.str ();
}

void msrMarginsGroup::print (ostream& os) const
{

  os <<
    "MarginsGroup" <<
    ", marginsGroupTypeKind: " <<
    msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
    endl;

  const int fieldWidth = 13;

  ++gIndenter;

  // margins
  os << left <<
    setw (fieldWidth) <<
    "leftMargin" << " : ";
    if (fLeftMargin) {
      os << fLeftMargin;
    }
    else {
      os << "[NONE]";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "rightMargin" << " : ";
    if (fRightMargin) {
      os << fRightMargin;
    }
    else {
      os << "[NONE]";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "topMargin" << " : ";
    if (fTopMargin) {
      os << fTopMargin;
    }
    else {
      os << "[NONE]";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "bottomMargin" << " : ";
    if (fBottomMargin) {
      os << fBottomMargin;
    }
    else {
      os << "[NONE]";
    }
  os << endl;

  --gIndenter;
};

ostream& operator << (ostream& os, const S_msrMarginsGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

// font size
//______________________________________________________________________________
string msrFontSizeKindAsString (
  msrFontSizeKind fontSizeKind)
{
  string result;

  switch (fontSizeKind) {
    case msrFontSizeKind::kFontSizeNone:
      result = "kFontSizeNone";
      break;
    case msrFontSizeKind::kFontSizeXXSmall:
      result = "kFontSizeXXSmall";
      break;
    case msrFontSizeKind::kFontSizeXSmall:
      result = "kFontSizeXSmall";
      break;
    case msrFontSizeKind::kFontSizeSmall:
      result = "kFontSizeSmall";
      break;
    case msrFontSizeKind::kFontSizeMedium:
      result = "kFontSizeMedium";
      break;
    case msrFontSizeKind::kFontSizeLarge:
      result = "kFontSizeLarge";
      break;
    case msrFontSizeKind::kFontSizeXLarge:
      result = "kFontSizeXLarge";
      break;
    case msrFontSizeKind::kFontSizeXXLarge:
      result = "kFontSizeXXLarge";
      break;
    case msrFontSizeKind::kFontSizeNumeric:
      result = "kFontSizeNumeric";
      break;
    } // switch

  return result;
}

ostream& operator << (ostream& os, const  msrFontSizeKind& elt)
{
  os <<  msrFontSizeKindAsString (elt);
  return os;
}

S_msrFontSize msrFontSize::create (
  msrFontSizeKind fontSizeKind)
{
  msrFontSize * o =
    new msrFontSize (
      fontSizeKind);
  assert (o != nullptr);

  return o;
}

S_msrFontSize msrFontSize::create (
  float numericFontSize)
{
  msrFontSize * o =
    new msrFontSize (
      numericFontSize);
  assert (o != nullptr);

  return o;
}

msrFontSize::msrFontSize (
  msrFontSizeKind fontSizeKind)
{
  fFontSizeKind = fontSizeKind;
}

msrFontSize::msrFontSize (
  float fontNumericSize)
{
  fFontSizeKind    = msrFontSizeKind::kFontSizeNumeric;
  fFontNumericSize = fontNumericSize;
}

msrFontSize::~msrFontSize ()
{}

string msrFontSize::fontSizeAsString () const
{
  string result;

  switch (fFontSizeKind) {
    case msrFontSizeKind::kFontSizeNone:
    case msrFontSizeKind::kFontSizeXXSmall:
    case msrFontSizeKind::kFontSizeXSmall:
    case msrFontSizeKind::kFontSizeSmall:
    case msrFontSizeKind::kFontSizeMedium:
    case msrFontSizeKind::kFontSizeLarge:
    case msrFontSizeKind::kFontSizeXLarge:
    case msrFontSizeKind::kFontSizeXXLarge:
      result = msrFontSizeKindAsString (fFontSizeKind);
      break;

    case msrFontSizeKind::kFontSizeNumeric:
      result =
        to_string (fFontNumericSize);
      break;
    } // switch

  return result;
}

float msrFontSize::getFontNumericSize () const
{
  float result = 12; // JMI

  switch (fFontSizeKind) {
    case msrFontSizeKind::kFontSizeNone:
    case msrFontSizeKind::kFontSizeXXSmall:
    case msrFontSizeKind::kFontSizeXSmall:
    case msrFontSizeKind::kFontSizeSmall:
    case msrFontSizeKind::kFontSizeMedium:
    case msrFontSizeKind::kFontSizeLarge:
    case msrFontSizeKind::kFontSizeXLarge:
    case msrFontSizeKind::kFontSizeXXLarge:
      {
        stringstream s;

        s <<
          "attempting to get font numeric size for a " <<
          msrFontSizeKindAsString (fFontSizeKind);

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          K_NO_INPUT_LINE_NUMBER, // JMI
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrFontSizeKind::kFontSizeNumeric:
      result = fFontNumericSize;
      break;
    } // switch

  return result;
}

void msrFontSize::print (ostream& os) const
{
  switch (fFontSizeKind) {
    case msrFontSizeKind::kFontSizeNone:
    case msrFontSizeKind::kFontSizeXXSmall:
    case msrFontSizeKind::kFontSizeXSmall:
    case msrFontSizeKind::kFontSizeSmall:
    case msrFontSizeKind::kFontSizeMedium:
    case msrFontSizeKind::kFontSizeLarge:
    case msrFontSizeKind::kFontSizeXLarge:
    case msrFontSizeKind::kFontSizeXXLarge:
      os <<
        msrFontSizeKindAsString (fFontSizeKind);
      break;

    case msrFontSizeKind::kFontSizeNumeric:
      os <<
        fFontNumericSize;
      break;
    } // switch
}

// font style
//______________________________________________________________________________
msrFontStyleKind msrFontStyleKindFromString (
  int           inputLineNumber,
  const string& fontStyleString)
{
  msrFontStyleKind result = msrFontStyleKind::kFontStyleNone; // default value

  if      (fontStyleString == "normal")
    result = msrFontStyleKind::kFontStyleNormal;
  else if (fontStyleString == "italic")
    result = msrFontStyleKind::KFontStyleItalic;
  else {
    if (fontStyleString.size ()) {
      stringstream s;

      s <<
        "style value " << fontStyleString <<
        " should be 'normal' or 'italic'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrFontStyleKindAsString (
  msrFontStyleKind fontStyleKind)
{
  string result;

  switch (fontStyleKind) {
    case msrFontStyleKind::kFontStyleNone:
      result = "***kFontStyleNone***";
      break;
    case msrFontStyleKind::kFontStyleNormal:
      result = "kFontStyleNormal";
      break;
    case msrFontStyleKind::KFontStyleItalic:
      result = "KFontStyleItalic";
      break;
    } // switch

  return result;
}

ostream& operator << (ostream& os, const msrFontStyleKind& elt)
{
  os << msrFontStyleKindAsString (elt);
  return os;
}

// font weight
//______________________________________________________________________________
msrFontWeightKind msrFontWeightKindFromString (
  int           inputLineNumber,
  const string& fontWeightString)
{
  msrFontWeightKind result = msrFontWeightKind::kFontWeightNone; // default value

  if      (fontWeightString == "normal")
    result = msrFontWeightKind::kFontWeightNormal;
  else if (fontWeightString == "bold")
    result = msrFontWeightKind::kFontWeightBold;
  else {
    if (fontWeightString.size ()) {
      stringstream s;

      s <<
        "font weight value " << fontWeightString <<
        " should be 'normal' or 'bold'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrFontWeightKindAsString (
  msrFontWeightKind fontWeightKind)
{
  string result;

  switch (fontWeightKind) {
    case msrFontWeightKind::kFontWeightNone:
      result = "kFontWeightNone";
      break;
    case msrFontWeightKind::kFontWeightNormal:
      result = "kFontWeightNormal";
      break;
    case msrFontWeightKind::kFontWeightBold:
      result = "kFontWeightBold";
      break;
    } // switch

  return result;
}

ostream& operator << (ostream& os, const msrFontWeightKind& elt)
{
  os << msrFontWeightKindAsString (elt);
  return os;
}

// alignement
//______________________________________________________________________________
msrJustifyKind msrJustifyKindFromString (
  int           inputLineNumber,
  const string& justifyString)
{
  msrJustifyKind result = msrJustifyKind::kJustifyNone; // default value

  if      (justifyString == "left")
    result = msrJustifyKind::kJustifyLeft;
  else if (justifyString == "center")
    result = msrJustifyKind::kJustifyCenter;
  else if (justifyString == "right")
    result = msrJustifyKind::kJustifyRight;
  else {
    if (justifyString.size ()) {
      stringstream s;

      s <<
        "justify value " << justifyString <<
        " should be 'left', 'center' or 'right'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrJustifyKindAsString (
  msrJustifyKind justifyKind)
{
  string result;

  switch (justifyKind) {
    case msrJustifyKind::kJustifyNone:
      result = "***kJustifyNone***";
      break;
    case msrJustifyKind::kJustifyLeft:
      result = "kJustifyLeft";
      break;
    case msrJustifyKind::kJustifyCenter:
      result = "kJustifyCenter";
      break;
    case msrJustifyKind::kJustifyRight:
      result = "kJustifyRight";
      break;
    } // switch

  return result;
}

ostream& operator << (ostream& os, const msrJustifyKind& elt)
{
  os << msrJustifyKindAsString (elt);
  return os;
}

msrHorizontalAlignmentKind msrHorizontalAlignmentKindFromString (
  int           inputLineNumber,
  const string& horizontalAlignmentString)
{
  msrHorizontalAlignmentKind
    result = msrHorizontalAlignmentKind::kHorizontalAlignmentNone; // default value

  if      (horizontalAlignmentString == "left")
    result = msrHorizontalAlignmentKind::kHorizontalAlignmentLeft;
  else if (horizontalAlignmentString == "center")
    result = msrHorizontalAlignmentKind::kHorizontalAlignmentCenter;
  else if (horizontalAlignmentString == "right")
    result = msrHorizontalAlignmentKind::kHorizontalAlignmentRight;
  else {
    if (horizontalAlignmentString.size ()) {
      stringstream s;

      s <<
        "halign value " << horizontalAlignmentString <<
        " should be 'left', 'center' or 'right'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrHorizontalAlignmentKindAsString (
  msrHorizontalAlignmentKind horizontalAlignmentKind)
{
  string result;

  switch (horizontalAlignmentKind) {
    case msrHorizontalAlignmentKind::kHorizontalAlignmentNone:
      result = "***kHorizontalAlignmentNone***";
      break;
    case msrHorizontalAlignmentKind::kHorizontalAlignmentLeft:
      result = "kHorizontalAlignmentLeft";
      break;
    case msrHorizontalAlignmentKind::kHorizontalAlignmentCenter:
      result = "kHorizontalAlignmentCenter";
      break;
    case msrHorizontalAlignmentKind::kHorizontalAlignmentRight:
      result = "kHorizontalAlignmentRight";
      break;
    } // switch

  return result;
}

ostream& operator << (ostream& os, const msrHorizontalAlignmentKind& elt)
{
  os << msrHorizontalAlignmentKindAsString (elt);
  return os;
}

msrVerticalAlignmentKind msrVerticalAlignmentKindFromString (
  int           inputLineNumber,
  const string& verticalAlignmentString)
{
  msrVerticalAlignmentKind
    result = msrVerticalAlignmentKind::kVerticalAlignmentNone; // default value

  if      (verticalAlignmentString == "top")
    result = msrVerticalAlignmentKind::kVerticalAlignmentTop;
  else if (verticalAlignmentString == "middle")
    result = msrVerticalAlignmentKind::kVerticalAlignmentMiddle;
  else if (verticalAlignmentString == "bottom")
    result = msrVerticalAlignmentKind::kVerticalAlignmentBottom;
  else {
    if (verticalAlignmentString.size ()) {
      stringstream s;

      s <<
        "valign value " << verticalAlignmentString <<
        " should be 'top', 'middle' or 'bottom'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind)
{
  string result;

  switch (verticalAlignmentKind) {
    case msrVerticalAlignmentKind::kVerticalAlignmentNone:
      result = "***kVerticalAlignmentNone***";
      break;
    case msrVerticalAlignmentKind::kVerticalAlignmentTop:
      result = "kVerticalAlignmentTop";
      break;
    case msrVerticalAlignmentKind::kVerticalAlignmentMiddle:
      result = "kVerticalAlignmentMiddle";
      break;
    case msrVerticalAlignmentKind::kVerticalAlignmentBottom:
      result = "kVerticalAlignmentBottom";
      break;
    } // switch

  return result;
}

ostream& operator << (ostream& os, const msrVerticalAlignmentKind& elt)
{
  os << msrVerticalAlignmentKindAsString (elt);
  return os;
}

// direction
//______________________________________________________________________________
string msrDirectionKindAsString (
  msrDirectionKind directionKind)
{
  string result;

  switch (directionKind) {
    case msrDirectionKind::kDirectionNone:
      result = "***kDirectionNone***";
      break;

    case msrDirectionKind::kDirectionUp:
      result = "kDirectionUp";
      break;
    case msrDirectionKind::kDirectionDown:
      result = "kDirectionDown";
      break;
    } // switch

  return result;
}

ostream& operator << (ostream& os, const msrDirectionKind& elt)
{
  os << msrDirectionKindAsString (elt);
  return os;
}

// print object
//______________________________________________________________________________
msrPrintObjectKind msrPrintObjectKindFromString (
  int           inputLineNumber,
  const string& printObjectString)
{
  msrPrintObjectKind
    result =
      msrPrintObjectKind::kPrintObjectNone; // default value JMI v0.9.65

//   gLogStream << // JMI TEST JMI v0.9.65
//     "printObjectString = " <<
//     printObjectString <<
//     endl;

  if      (printObjectString == "yes")
    result = msrPrintObjectKind::kPrintObjectYes;
  else if (printObjectString == "no")
    result = msrPrintObjectKind::kPrintObjectNo;
  else {
    if (printObjectString.size ()) {
      stringstream s;

      s <<
        "printObject \"" << printObjectString <<
        "\" should be 'yes' or 'no'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrPrintObjectKindAsString (
  msrPrintObjectKind printObjectKind)
{
  string result;

  switch (printObjectKind) {
    case msrPrintObjectKind::kPrintObjectNone:
      result = "***kPrintObjectNone***";
      break;
    case msrPrintObjectKind::kPrintObjectYes:
      result = "kPrintObjectYes";
      break;
    case msrPrintObjectKind::kPrintObjectNo:
      result = "kPrintObjectNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrPrintObjectKind& elt)
{
  os << msrPrintObjectKindAsString (elt);
  return os;
}

// placement
//______________________________________________________________________________
msrPlacementKind msrPlacementKindFromString (
  int           inputLineNumber,
  const string& placementString)
{
  msrPlacementKind result = msrPlacementKind::k_NoPlacement; // default value

  if      (placementString == "above")
    result = msrPlacementKind::kPlacementAbove;
  else if (placementString == "below")
    result = msrPlacementKind::kPlacementBelow;
  else {
    if (placementString.size ()) {
      stringstream s;

      s <<
        "placement \"" << placementString <<
        "\" should be 'above' or 'below'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string placementKindAsString (
  msrPlacementKind placementKind)
{
  string result;

  switch (placementKind) {
    case msrPlacementKind::k_NoPlacement:
      result = "k_NoPlacement";
      break;
    case msrPlacementKind::kPlacementAbove:
      result = "kPlacementAbove";
      break;
    case msrPlacementKind::kPlacementBelow:
      result = "kPlacementBelow";
      break;
    default:
      ;
      // cannot occur since 'enum class' variables are initialized to 0
      // and the C++ compilers check for missing, unspecified switch values
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrPlacementKind& elt)
{
  os << placementKindAsString (elt);
  return os;
}

// dynamics
//______________________________________________________________________________
msrDynamicKind dynamicFromString (const string& theString)
{
  msrDynamicKind result = msrDynamicKind::k_NoDynamics;

  if (theString == "f") {
    result = msrDynamicKind::kDynamicF;
  }
  else if (theString == "f") {
    result = msrDynamicKind::kDynamicF;
  }
  else if (theString == "fff") {
    result = msrDynamicKind::kDynamicFFF;
  }
  else if (theString == "ffff") {
    result = msrDynamicKind::kDynamicFFFF;
  }
  else if (theString == "fffff") {
    result = msrDynamicKind::kDynamicFFFFF;
  }
  else if (theString == "ffffff") {
    result = msrDynamicKind::kDynamicFFFFFF;
  }

  else if (theString == "p") {
    result = msrDynamicKind::kDynamicP;
  }
  else if (theString == "pp") {
    result = msrDynamicKind::kDynamicPP;
  }
  else if (theString == "ppp") {
    result = msrDynamicKind::kDynamicPPP;
  }
  else if (theString == "pppp") {
    result = msrDynamicKind::kDynamicPPPP;
  }
  else if (theString == "ppppp") {
    result = msrDynamicKind::kDynamicPPPPP;
  }
  else if (theString == "pppppp") {
    result = msrDynamicKind::kDynamicPPPPPP;
  }

  else if (theString == "mf") {
    result = msrDynamicKind::kDynamicMF;
  }
  else if (theString == "mp") {
    result = msrDynamicKind::kDynamicMP;
  }
  else if (theString == "fp") {
    result = msrDynamicKind::kDynamicFP;
  }
  else if (theString == "fz") {
    result = msrDynamicKind::kDynamicFZ;
  }
  else if (theString == "pf") {
    result = msrDynamicKind::kDynamicPF;
  }
  else if (theString == "rf") {
    result = msrDynamicKind::kDynamicRF;
  }
  else if (theString == "sf") {
    result = msrDynamicKind::kDynamicSF;
  }

  else if (theString == "rfz") {
    result = msrDynamicKind::kDynamicRFZ;
  }
  else if (theString == "sfz") {
    result = msrDynamicKind::kDynamicSFZ;
  }
  else if (theString == "sfp") {
    result = msrDynamicKind::kDynamicSFP;
  }
  else if (theString == "sfpp") {
    result = msrDynamicKind::kDynamicSFPP;
  }
  else if (theString == "sffz") {
    result = msrDynamicKind::kDynamicSFFZ;
  }
  else if (theString == "sfzp") {
    result = msrDynamicKind::kDynamicSFZP;
  }
  else if (theString == "n") {
    result = msrDynamicKind::kDynamicN;
  }

  return result;
}

string msrDynamicKindAsString (
  msrDynamicKind dynamicKind)
{
  string result;

  switch (dynamicKind) {
    case msrDynamicKind::k_NoDynamics:
      result = "*msrDynamicKind::k_NoDynamics*";
      break;

    case msrDynamicKind::kDynamicF:
      result = "f";
      break;
    case msrDynamicKind::kDynamicFF:
      result = "ff";
      break;
    case msrDynamicKind::kDynamicFFF:
      result = "fff";
      break;
    case msrDynamicKind::kDynamicFFFF:
      result = "ffff";
      break;
    case msrDynamicKind::kDynamicFFFFF:
      result = "fffff";
      break;
    case msrDynamicKind::kDynamicFFFFFF:
      result = "ffffff";
      break;

    case msrDynamicKind::kDynamicP:
      result = "p";
      break;
    case msrDynamicKind::kDynamicPP:
      result = "pp";
      break;
    case msrDynamicKind::kDynamicPPP:
      result = "ppp";
      break;
    case msrDynamicKind::kDynamicPPPP:
      result = "pppp";
      break;
    case msrDynamicKind::kDynamicPPPPP:
      result = "ppppp";
      break;
    case msrDynamicKind::kDynamicPPPPPP:
      result = "pppppp";
      break;

    case msrDynamicKind::kDynamicMF:
      result = "mf";
      break;
    case msrDynamicKind::kDynamicMP:
      result = "mp";
      break;
    case msrDynamicKind::kDynamicFP:
      result = "fp";
      break;
    case msrDynamicKind::kDynamicFZ:
      result = "fz";
      break;
    case msrDynamicKind::kDynamicPF:
      result = "pf";
      break;
    case msrDynamicKind::kDynamicRF:
      result = "rf";
      break;
    case msrDynamicKind::kDynamicSF:
      result = "sf";
      break;

    case msrDynamicKind::kDynamicRFZ:
      result = "rfz";
      break;
    case msrDynamicKind::kDynamicSFZ:
      result = "sfz";
      break;
    case msrDynamicKind::kDynamicSFP:
      result = "sfp";
      break;
    case msrDynamicKind::kDynamicSFPP:
      result = "sfpp";
      break;
    case msrDynamicKind::kDynamicSFFZ:
      result = "sffz";
      break;
    case msrDynamicKind::kDynamicSFZP:
      result = "sfzp";
      break;
    case msrDynamicKind::kDynamicN:
      result = "n";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrDynamicKind& elt)
{
  os << msrDynamicKindAsString (elt);
  return os;
}

// measure style
//______________________________________________________________________________
msrUseDotsKind msrUseDotsFromString (
  int           inputLineNumber,
  const string& useDotsString)
{
  msrUseDotsKind result = msrUseDotsKind::kUseDotsNo; // default value

  if      (useDotsString == "yes")
    result = msrUseDotsKind::kUseDotsYes;
  else if (useDotsString == "no")
    result = msrUseDotsKind::kUseDotsNo;
  else {
    if (useDotsString.size ()) {
      stringstream s;

      s <<
        "use-dots \"" << useDotsString <<
        "\" is unknown";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrSlashTypeKindAsString (
  msrSlashTypeKind slashTypeKind)
{
  string result;

  switch (slashTypeKind) {
    case msrSlashTypeKind::k_NoSlashType:
      result = "k_NoSlashType";
      break;
    case msrSlashTypeKind::kSlashTypeStart:
      result = "kSlashTypeStart";
      break;
    case msrSlashTypeKind::kSlashTypeStop:
      result = "kSlashTypeStop";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSlashTypeKind& elt)
{
  os << msrSlashTypeKindAsString (elt);
  return os;
}

string msrUseDotsKindAsString (
  msrUseDotsKind useDotsKind)
{
  string result;

  switch (useDotsKind) {
    case msrUseDotsKind::k_NoUseDots:
      result = "k_NoUseDots";
      break;
    case msrUseDotsKind::kUseDotsYes:
      result = "kUseDotsYes";
      break;
    case msrUseDotsKind::kUseDotsNo:
      result = "kUseDotsNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrUseDotsKind& elt)
{
  os << msrUseDotsKindAsString (elt);
  return os;
}

string msrSlashUseStemsKindAsString (
  msrSlashUseStemsKind slashUseStemsKind)
{
  string result;

  switch (slashUseStemsKind) {
    case msrSlashUseStemsKind::k_NoSlashUseStems:
      result = "k_NoSlashUseStems";
      break;
    case msrSlashUseStemsKind::kSlashUseStemsYes:
      result = "kSlashUseStemsYes";
      break;
    case msrSlashUseStemsKind::kSlashUseStemsNo:
      result = "kSlashUseStemsNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSlashUseStemsKind& elt)
{
  os << msrSlashUseStemsKindAsString (elt);
  return os;
}

//______________________________________________________________________________
string msrLineTypeKindAsString (
  msrLineTypeKind lineTypeKind)
{
  string result;

  switch (lineTypeKind) {
    case msrLineTypeKind::kLineTypeSolid:
      result = "kLineTypeSolid";
      break;
    case msrLineTypeKind::kLineTypeDashed:
      result = "kLineTypeDashed";
      break;
    case msrLineTypeKind::kLineTypeDotted:
      result = "kLineTypeDotted";
      break;
    case msrLineTypeKind::kLineTypeWavy:
      result = "kLineTypeWavy";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrLineTypeKind& elt)
{
  os << msrLineTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
string msrTremoloTypeKindAsString (
  msrTremoloTypeKind tremoloTypeKind)
{
  string result;

  switch (tremoloTypeKind) {
    case msrTremoloTypeKind::k_NoTremoloType:
      result = "***k_NoTremoloType***";
      break;
    case msrTremoloTypeKind::kTremoloTypeSingle:
      result = "kTremoloTypeSingle";
      break;
    case msrTremoloTypeKind::kTremoloTypeStart:
      result = "kTremoloTypeStart";
      break;
    case msrTremoloTypeKind::kTremoloTypeStop:
      result = "kTremoloTypeStop";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTremoloTypeKind& elt)
{
  os << msrTremoloTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
string msrTechnicalTypeKindAsString (
  msrTechnicalTypeKind technicalTypeKind)
{
  string result;

  switch (technicalTypeKind) {
    case msrTechnicalTypeKind::k_NoTechnicalType:
      result = "k_NoTechnicalType";
      break;
    case msrTechnicalTypeKind::kTechnicalTypeStart:
      result = "kTechnicalTypeStart";
      break;
    case msrTechnicalTypeKind::kTechnicalTypeStop:
      result = "kTechnicalTypeStop";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTechnicalTypeKind& elt)
{
  os << msrTechnicalTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
string msrSpannerTypeKindAsString (
  msrSpannerTypeKind spannerTypeKind)
{
  string result;

  switch (spannerTypeKind) {
    case msrSpannerTypeKind::k_NoSpannerType:
      result = "k_NoSpannerType";
      break;
    case msrSpannerTypeKind::kSpannerTypeStart:
      result = "kSpannerTypeStart";
      break;
    case msrSpannerTypeKind::kSpannerTypeStop:
      result = "kSpannerTypeStop";
      break;
    case msrSpannerTypeKind::kSpannerTypeContinue:
      result = "kSpannerTypeContinue";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSpannerTypeKind& elt)
{
  os << msrSpannerTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrHarmonyInterval msrHarmonyInterval::create (
// JMI  int             inputLineNumber,
  msrIntervalKind harmonyIntervalIntervalKind,
  int             harmonyIntervalRelativeOctave)
{
  msrHarmonyInterval* o =
    new msrHarmonyInterval (
 //     inputLineNumber,
 //     harmonyIntervalNumber,
      harmonyIntervalIntervalKind,
      harmonyIntervalRelativeOctave);
  assert (o != nullptr);

  return o;
}

msrHarmonyInterval::msrHarmonyInterval (
// JMI  int             inputLineNumber,
  msrIntervalKind harmonyIntervalIntervalKind,
  int             harmonyIntervalRelativeOctave)
  // JMI  : msrElement (inputLineNumber)
{
  fHarmonyIntervalIntervalKind = harmonyIntervalIntervalKind;

  fHarmonyIntervalRelativeOctave = harmonyIntervalRelativeOctave;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony item '" <<
      harmonyIntervalAsString () <<
      "'" <<
      endl;
  }
#endif
}

msrHarmonyInterval::~msrHarmonyInterval ()
{}

S_msrHarmonyInterval msrHarmonyInterval::createHarmonyIntervalNewbornClone ()
{
  S_msrHarmonyInterval
    newbornClone =
      msrHarmonyInterval::create (
        fHarmonyIntervalIntervalKind,
        fHarmonyIntervalRelativeOctave);

  return newbornClone;
}

string msrHarmonyInterval::harmonyIntervalAsString () const
{
  stringstream s;

  const int fieldWidth = 19;

  s << left <<
    "HarmonyInterval" <<
    ": " <<
    setw (fieldWidth) <<
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    "harmonyIntervalRelativeOctave: " << fHarmonyIntervalRelativeOctave;

  return s.str ();
}

string msrHarmonyInterval::harmonyIntervalAsShortString () const
{
  return
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind);
}


void msrHarmonyInterval::normalizeInterval ()
{
  // bring the interval below an octave
  if (fHarmonyIntervalIntervalKind > msrIntervalKind::kIntervalAugmentedSeventh) {
    fHarmonyIntervalIntervalKind =
      msrIntervalKind (
        (int) fHarmonyIntervalIntervalKind
          -
        (int) msrIntervalKind::kIntervalAugmentedSeventh);

    fHarmonyIntervalRelativeOctave += 1;
  }
}

void msrHarmonyInterval::deNormalizeInterval ()
{
  // bring the interval above the octave if its relative octave is 1
  if (
    fHarmonyIntervalRelativeOctave == 1
      &&
    fHarmonyIntervalIntervalKind <= msrIntervalKind::kIntervalAugmentedSeventh) {
    fHarmonyIntervalIntervalKind =
      msrIntervalKind (
        (int) fHarmonyIntervalIntervalKind
          +
        (int) msrIntervalKind::kIntervalAugmentedSeventh);

    fHarmonyIntervalRelativeOctave -= 1;
  }
}

S_msrHarmonyInterval msrHarmonyInterval::intervalDifference (
  S_msrHarmonyInterval otherHarmonyInterval)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      endl <<
      "--> computing intervalDifference betwwen '" <<
      asShortString () <<
      "' and '" <<
      otherHarmonyInterval->asShortString () <<
      "'" <<
      endl;
  }
#endif

  msrIntervalKind resultIntervalKind   = msrIntervalKind::k_NoIntervalKind;

  S_msrHarmonyInterval
    operand1 =
      this->createHarmonyIntervalNewbornClone (),
    operand2 =
      otherHarmonyInterval->createHarmonyIntervalNewbornClone ();

  // normalize both intervals
  operand1->
    normalizeInterval ();
  operand2->
    normalizeInterval ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      "--> normalized operands are '" <<
      operand1->asShortString () <<
      "' and '" <<
      operand2->asShortString () <<
      "'" <<
      endl;
  }
#endif

  // fetch the relative octaves
  int
    relativeOctave1 = operand1->fHarmonyIntervalRelativeOctave,
    relativeOctave2 = operand2->fHarmonyIntervalRelativeOctave;

  // fetch the interval kind
  msrIntervalKind
    intervalKind1 =
      operand1->fHarmonyIntervalIntervalKind,
    intervalKind2 =
      operand2->fHarmonyIntervalIntervalKind;

  // order the operands so that
  // relativeOctave1 is greater or equal to relativeOctave2
  Bool permuteRelativeOctaves (false);

  if (relativeOctave1 < relativeOctave2) {
    int saveRelativeOctave1 = relativeOctave1;

    relativeOctave1 = relativeOctave2;
    relativeOctave2 = saveRelativeOctave1;

    permuteRelativeOctaves = true;
  }

  // order the intervals so that
  // intervalKind1 is greater or equal to intervalKind2
  // according to the enum type
  Bool invertInterval (false);

  if (intervalKind1 < intervalKind2) {
    msrIntervalKind saveIntervalKind1 = intervalKind1;

    intervalKind1 = intervalKind2;
    intervalKind2 = saveIntervalKind1;

    invertInterval = true;
  }

  // compute the resulting relative octaves difference
  int
    resultRelativeOctave =
      relativeOctave1 - relativeOctave2;
  if (invertInterval) {
    --resultRelativeOctave;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      "--> permuteRelativeOctaves = " <<
      permuteRelativeOctaves <<
      ", invertInterval = " <<
      invertInterval <<
      ", resultRelativeOctave = " <<
      resultRelativeOctave <<
      endl;
  }
#endif

  // compute resulting interval Kind
  switch (intervalKind1) {
    case msrIntervalKind::k_NoIntervalKind:
      break;

    case msrIntervalKind::kIntervalDiminishedUnisson:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalPerfectUnisson:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedUnisson:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;


    case msrIntervalKind::kIntervalDiminishedSecond:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMinorSecond:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMajorSecond:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedSecond:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;


    case msrIntervalKind::kIntervalDiminishedThird:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedThird;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMinorThird:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedThird;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMajorThird:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedThird:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
    //      resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
   // JMI      resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;


    case msrIntervalKind::kIntervalDiminishedFourth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalPerfectFourth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedThird;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedFourth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;


    case msrIntervalKind::kIntervalDiminishedFifth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
 // JMI         resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
      // JMI    resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalPerfectFifth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFifth;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedFifth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
 // JMI         resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;


    case msrIntervalKind::kIntervalDiminishedSixth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSixth;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSixth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalMinorSixth;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalMajorThird:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalDiminishedThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
         resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
         resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMinorSixth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSixth;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSixth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSixth;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFifth;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMajorSixth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSixth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSixth;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSixth;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFifth;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedSixth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
   //       resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSixth;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;


    case msrIntervalKind::kIntervalDiminishedSeventh:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSixth;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSixth;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSixth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalMajorThird:
     //     resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedThird;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMinorSeventh:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSixth;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSixth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSixth;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFifth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedThird;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMajorSeventh:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSeventh;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSixth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSixth;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFifth;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
 // JMI         resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSecond;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedSeventh:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
 // JMI         resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalAugmentedSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
     // JMI    resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSixth;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
      // JMI    resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFifth;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFifth;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalDiminishedFifth;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
     // JMI     resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedFourth;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectFourth;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
  // JMI        resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
 // JMI         resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSecond;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSecond;
          break;
        case msrIntervalKind::kIntervalDiminishedSeventh:
   // JMI       resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
    // JMI      resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    default:
      ;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      "--> base resultIntervalKind = '" <<
      msrIntervalKindAsString (resultIntervalKind) <<
      "'" <<
      endl;
  }
#endif

  // take interval inversion into account if relevant
  if (invertInterval) {
    resultIntervalKind =
      invertIntervalKind (resultIntervalKind);
  }

  // take relative octabes permutation into account if relevant
  if (permuteRelativeOctaves) {
    resultIntervalKind =
      invertIntervalKind (resultIntervalKind);
  }

  // create the result
  S_msrHarmonyInterval
    result =
      msrHarmonyInterval::create (
        resultIntervalKind,
        resultRelativeOctave);

  // denormalize it, in order to get intervals
  // greater than an augmented seventh if applicable
  result->deNormalizeInterval ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      "--> result = '" <<
      result->asShortString () <<
      "'" <<
      endl << endl;
  }
#endif

  // return it;
  return result;
}

S_msrHarmonyInterval msrHarmonyInterval::intervalSum (
  S_msrHarmonyInterval otherHarmonyInterval)
{
  msrIntervalKind resultIntervalKind   = msrIntervalKind::k_NoIntervalKind;
  int             resultRelativeOctave = 0;

  msrIntervalKind
    intervalKind1 = fHarmonyIntervalIntervalKind,
    intervalKind2 = otherHarmonyInterval->fHarmonyIntervalIntervalKind;

  int
    relativeOctave1 = fHarmonyIntervalRelativeOctave,
    relativeOctave2 = otherHarmonyInterval->fHarmonyIntervalRelativeOctave;

  relativeOctave1 = relativeOctave2; // TEMP, JMI
  relativeOctave2 = relativeOctave1; // TEMP, JMI

  // order the intervals so that
  // intervalKind1 is greater or equal to intervalKind2
  // according to the enum type
  Bool invertInterval (false);

  if (intervalKind1 < intervalKind2) {
    intervalKind1 = intervalKind2;
    intervalKind2 = intervalKind1;
    invertInterval = true;
  }

  switch (intervalKind1) {
    case msrIntervalKind::k_NoIntervalKind:
      break;

    case msrIntervalKind::kIntervalDiminishedUnisson:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalPerfectUnisson:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedUnisson:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalDiminishedSecond:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalDiminishedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMinorSecond:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMajorSecond:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedSecond:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalDiminishedThird:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMinorThird:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMajorThird:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedThird:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalDiminishedFourth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalPerfectFourth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedFourth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalDiminishedFifth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalPerfectFifth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedFifth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalDiminishedSixth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMinorSixth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMajorSixth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedSixth:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalDiminishedSeventh:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMinorSeventh:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalMajorSeventh:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    case msrIntervalKind::kIntervalAugmentedSeventh:
      switch (intervalKind2) {
        case msrIntervalKind::kIntervalDiminishedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedUnisson;
          break;
        case msrIntervalKind::kIntervalPerfectUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedUnisson:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedOctave;
          break;
        case msrIntervalKind::kIntervalMinorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMajorSeventh;
          break;
        case msrIntervalKind::kIntervalMajorSecond:
          resultIntervalKind = msrIntervalKind::kIntervalMinorSeventh;
          break;
        case msrIntervalKind::kIntervalAugmentedSecond:
          resultIntervalKind = msrIntervalKind::kIntervalDiminishedSeventh;
          break;
        case msrIntervalKind::kIntervalDiminishedThird:
          resultIntervalKind = msrIntervalKind::kIntervalAugmentedSixth;
          break;
        case msrIntervalKind::kIntervalMinorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalMajorThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedThird:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFourth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalDiminishedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalPerfectFifth:
          resultIntervalKind = msrIntervalKind::kIntervalMinorThird;
          break;
        case msrIntervalKind::kIntervalAugmentedFifth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedSixth:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalDiminishedSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMinorSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalMajorSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        case msrIntervalKind::kIntervalAugmentedSeventh:
          resultIntervalKind = msrIntervalKind::kIntervalPerfectUnisson;
          break;
        default:
          ;
      } // switch
      break;

    default:
      ;
  } // switch


  if (invertInterval) {
    resultIntervalKind =
      invertIntervalKind (resultIntervalKind);
  }

  return
    msrHarmonyInterval::create (
      resultIntervalKind,
      resultRelativeOctave);
}

/* JMI
void msrHarmonyInterval::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyInterval::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyInterval>*
    p =
      dynamic_cast<visitor<S_msrHarmonyInterval>*> (v)) {
        S_msrHarmonyInterval elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyInterval::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarmonyInterval::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyInterval::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyInterval>*
    p =
      dynamic_cast<visitor<S_msrHarmonyInterval>*> (v)) {
        S_msrHarmonyInterval elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyInterval::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrHarmonyInterval::browseData (basevisitor* v)
{}
*/

string msrHarmonyInterval::asString () const
{
  stringstream s;

  s <<
    "[HarmonyInterval " <<
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    ", fHarmonyIntervalRelativeOctave: " <<
    fHarmonyIntervalRelativeOctave <<
    ']';

  return s.str ();
}

string msrHarmonyInterval::asShortString () const
{
  stringstream s;

  s <<
    '[' <<
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    ", rel.oct. " <<
    fHarmonyIntervalRelativeOctave <<
    ']';

  return s.str ();
}

void msrHarmonyInterval::print (ostream& os) const
{
  os <<
    "HarmonyInterval" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
  /* JMI
    setw (fieldWidth) <<
    "harmonyIntervalNumber" << " : " << fHarmonyIntervalNumber <<
    endl <<
    */
    setw (fieldWidth) <<
    "harmonyIntervalIntervalKind" << " : " <<
      msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    endl <<
    setw (fieldWidth) <<
    "harmonyIntervalRelativeOctave" << " : " << fHarmonyIntervalRelativeOctave <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrHarmonyInterval& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
map<msrHarmonyKind, S_msrHarmonyStructure>
  gGlobalHarmonyStructuresMap;

S_msrHarmonyStructure msrHarmonyStructure::createBare (
  msrHarmonyKind harmonyStructureHarmonyKind)
{
  msrHarmonyStructure* o =
    new msrHarmonyStructure (
      harmonyStructureHarmonyKind);
  assert (o != nullptr);

  return o;
}

S_msrHarmonyStructure msrHarmonyStructure::create (
  msrHarmonyKind harmonyStructureHarmonyKind)
{
  S_msrHarmonyStructure o =
    createBare (
      harmonyStructureHarmonyKind);

  o->
    populateHarmonyStructure ();

  return o;
}

msrHarmonyStructure::msrHarmonyStructure (
  msrHarmonyKind harmonyStructureHarmonyKind)
{
  fHarmonyStructureHarmonyKind = harmonyStructureHarmonyKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony intervals '" <<
      harmonyStructureAsString () <<
      "'" <<
      endl;
  }
#endif
}

S_msrHarmonyStructure msrHarmonyStructure::createHarmonyStructureNewbornClone ()
{
  S_msrHarmonyStructure
    newbornClone =
      createBare (
        fHarmonyStructureHarmonyKind);

  return newbornClone;
}

void msrHarmonyStructure::populateHarmonyStructure ()
{
  // append harmony items to harmony intervals
  switch (fHarmonyStructureHarmonyKind) {
    case msrHarmonyKind::k_NoHarmony:
      break;

    // MusicXML harmonies

    case msrHarmonyKind::kHarmonyMajor:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMinor:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyAugmented:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedFifth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDiminished:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalDiminishedFifth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDominant:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMajorSeventh:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSeventh)
          );
      }
      break;

     case msrHarmonyKind::kHarmonyMinorSeventh:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDiminishedSeventh:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalDiminishedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalDiminishedSeventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyAugmentedSeventh:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyHalfDiminished:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalDiminishedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
      }
      break;

     case msrHarmonyKind::kHarmonyMinorMajorSeventh:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSeventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMajorSixth:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSixth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMinorSixth:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSixth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDominantNinth:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorNinth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMajorNinth:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorNinth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMinorNinth:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorNinth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDominantEleventh:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectEleventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMajorEleventh:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectEleventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMinorEleventh:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectEleventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDominantThirteenth:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThirteenth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMajorThirteenth:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThirteenth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMinorThirteenth:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThirteenth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonySuspendedSecond:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSecond)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonySuspendedFourth:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
      }
      break;

/*
* kNeapolitan f aes des' in:
*
* c e g c' -> f f aes des' -> d g d b -> c e g c'

they are three different pre-dominant harmonies that are taught to American undergrads in a sophomore theory course.

in E major:
Italian = C E A#
French = C E F# A#
German = C E G A#
Tristan = C D# F# A#

in all of them, the C and A# in theory want to fan out to B (the dominant).  This is, of course, in theory - Wagner’s use of the Tristan harmony, which he clearly named his opera after, has the A# moving down to A, or the 7th of the dominant (I’m transposing to fit w/ the example above).  Wagner obviously did not pay much attention during his sophomore music theory course…
*/

    case msrHarmonyKind::kHarmonyNeapolitan:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalDiminishedSixth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyItalian:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedSixth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyFrench:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedSixth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyGerman:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedSixth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyPedal:
      break;

    case msrHarmonyKind::kHarmonyPower:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyTristan:
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedSecond)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedSixth)
          );
      }
      break;

    // jazz-specific harmonies

    case msrHarmonyKind::kHarmonyMinorMajorNinth: // -maj9, minmaj9
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorNinth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDominantSuspendedFourth: // 7sus4, domsus4
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDominantAugmentedFifth: // 7#5, domaug5
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDominantMinorNinth: // 7b9, dommin9
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorNinth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDominantAugmentedNinthDiminishedFifth: // 7#9b5, domaug9dim5
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalDiminishedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedNinth)
          );
      }
      break;

    case msrHarmonyKind::kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth: // 7#9#5, domaug9aug5
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorThird)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedNinth)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyDominantAugmentedEleventh: // 7#11, domaug11
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMinorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedEleventh)
          );
      }
      break;

    case msrHarmonyKind::kHarmonyMajorSeventhAugmentedEleventh: // maj7#11, maj7aug11
      {
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectUnisson)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFourth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalPerfectFifth)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalMajorSeventh)
          );
        appendHarmonyIntervalToHarmonyStructure (
          msrHarmonyInterval::create (
            msrIntervalKind::kIntervalAugmentedEleventh)
          );
      }
      break;

    // other

    case msrHarmonyKind::kHarmonyOther:
      break;

    case msrHarmonyKind::kHarmonyNone:
      break;
  } // switch

/* JMI
  // register harmony intervals in map
  gGlobalHarmonyStructuresMap [fHarmonyStructureHarmonyKind] = this;
  */
}

msrHarmonyStructure::~msrHarmonyStructure ()
{}

void msrHarmonyStructure::appendHarmonyIntervalToHarmonyStructure (
  S_msrHarmonyInterval harmonyInterval)
{
  // set the input line number and harmony item number // JMI

  // append the harmony item
  fHarmonyStructureIntervals.push_back (
    harmonyInterval);
}

/* JMI
void msrHarmonyStructure::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyStructure::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyStructure>*
    p =
      dynamic_cast<visitor<S_msrHarmonyStructure>*> (v)) {
        S_msrHarmonyStructure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyStructure::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarmonyStructure::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyStructure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyStructure>*
    p =
      dynamic_cast<visitor<S_msrHarmonyStructure>*> (v)) {
        S_msrHarmonyStructure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyStructure::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrHarmonyStructure::browseData (basevisitor* v)
{}
*/

S_msrHarmonyInterval msrHarmonyStructure::bassHarmonyIntervalForHarmonyInversion (
  int inputLineNumber,
  int inversionNumber)
{
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */

  S_msrHarmonyInterval result;

  if (
    inversionNumber < 0
      ||
    inversionNumber > int (fHarmonyStructureIntervals.size ()) - 1 ) {
    stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for harmony intervals '" <<
      msrHarmonyKindAsString (fHarmonyStructureHarmonyKind) <<
      "', line " << inputLineNumber;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return fHarmonyStructureIntervals [inversionNumber];
}

S_msrHarmonyStructure msrHarmonyStructure::invertHarmonyStructure (int inversion)
{
  if (inversion == 0) {
    return this;
  }

  // create an empty object
  S_msrHarmonyStructure
    result =
      this->
        createHarmonyStructureNewbornClone ();

  size_t
    harmonyStructureIntervalsSize =
      fHarmonyStructureIntervals.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> invertHarmonyStructure (), inversion = " <<
      inversion <<
      ", original harmonyStructureIntervalsSize = " <<
      harmonyStructureIntervalsSize <<
      endl;
  }
#endif

  if (harmonyStructureIntervalsSize) {
    // add the first items
    for (size_t i = inversion; i < harmonyStructureIntervalsSize; ++i) {
      S_msrHarmonyInterval
        harmonyIntervalClone =
          fHarmonyStructureIntervals [i]->
            createHarmonyIntervalNewbornClone ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "--> adding first item to result:" <<
          endl;
        ++gIndenter;
        gLogStream <<
          harmonyIntervalClone <<
          endl;
        --gIndenter;
      }
#endif

      result->
        appendHarmonyIntervalToHarmonyStructure (
          harmonyIntervalClone);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "==> result harmony structure after adding first item :" <<
          endl;

        ++gIndenter;
        gLogStream <<
          result <<
          endl;
        --gIndenter;
      }
#endif
    } // for

    // add  the octaviate last items
    for (int i = 0; i < inversion; ++i) {
      S_msrHarmonyInterval
        harmonyIntervalClone =
          fHarmonyStructureIntervals [i]->
            createHarmonyIntervalNewbornClone ();

      harmonyIntervalClone->
        incrementHarmonyIntervalRelativeOctave ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "--> adding last item to resultlast item :" <<
          endl;
        ++gIndenter;
        gLogStream <<
          harmonyIntervalClone <<
          endl;
        --gIndenter;
      }
#endif

      result->
        appendHarmonyIntervalToHarmonyStructure (
          harmonyIntervalClone);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "==> result harmony structure after  after adding last item:" <<
          endl;

        ++gIndenter;
        gLogStream <<
          result <<
          endl;
        --gIndenter;
      }
#endif
    } // for
  }

  return result;
}

list<msrSemiTonesPitchKind> buildSemiTonesChord (
  msrHarmonyKind        harmonyKind,
  msrSemiTonesPitchKind rootNote)
{
  list<msrSemiTonesPitchKind> result;

  // create the harmony intervals
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        harmonyKind);

  // add the root to the harmony
  result.push_back (rootNote);

  // add the other notes to the harmony
  const vector<S_msrHarmonyInterval>&
    harmonyStructureIntervals =
      harmonyStructure->
        getHarmonyStructureIntervals ();

  for (size_t i = 1; i << harmonyStructureIntervals.size (); ++i) {
    result.push_back (rootNote);
  } // for

  return result;
}

string msrHarmonyStructure::harmonyStructureAsString () const
{
  stringstream s;

  s <<
    "HarmonyStructure" <<
    ", " <<
    msrHarmonyKindAsString (fHarmonyStructureHarmonyKind) <<
    ", " <<
    mfSingularOrPlural (
      fHarmonyStructureIntervals.size (), "item", "items");

  return s.str ();
}

void msrHarmonyStructure::print (ostream& os) const
{
  os <<
    "HarmonyStructure" <<
    ", harmonyStructureHarmonyKind: " <<
    msrHarmonyKindAsString (fHarmonyStructureHarmonyKind) <<
    ", " <<
    mfSingularOrPlural (
      fHarmonyStructureIntervals.size (), "interval", "intervals") <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  ++gIndenter;

  if (fHarmonyStructureIntervals.size ()) {
    vector<S_msrHarmonyInterval>::const_reverse_iterator
      iBegin = fHarmonyStructureIntervals.crbegin (),
      iEnd   = fHarmonyStructureIntervals.crend (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHarmonyInterval
        harmonyInterval = (*i);

      gLogStream <<
        harmonyInterval->harmonyIntervalAsShortString () <<
        endl;

      if (++i == iEnd) break;
    } // for
  }
  else {
    gLogStream <<
      "no intervals" <<
      endl;
  }

  --gIndenter;
}

void msrHarmonyStructure::printAllHarmoniesStructures (ostream& os)
{
  os <<
    "All the known harmonies structures are:" <<
    endl << endl;

  ++gIndenter;

  for (auto e : EnumTrueHarmonies<msrHarmonyKind> ()) {
    // create the harmony intervals
    S_msrHarmonyStructure
      harmonyStructure =
        msrHarmonyStructure::create (
          e);

    // print it
    os <<
      harmonyStructure <<
      endl;
  } // for

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrHarmonyStructure& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

/* JMI
//______________________________________________________________________________
S_msrSemiTonesPitchAndAbsoluteOctave msrSemiTonesPitchAndAbsoluteOctave::create (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   absoluteOctave)
{
  msrSemiTonesPitchAndAbsoluteOctave* o =
    new msrSemiTonesPitchAndAbsoluteOctave (
      semiTonesPitchKind,
      absoluteOctave);
  assert (o != nullptr);

  return o;
}

msrSemiTonesPitchAndAbsoluteOctave::msrSemiTonesPitchAndAbsoluteOctave (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   absoluteOctave)
{
  fSemiTonesPitchKind = semiTonesPitchKind;

  fAbsoluteOctave = absoluteOctave;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony item '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrSemiTonesPitchAndAbsoluteOctave::~msrSemiTonesPitchAndAbsoluteOctave ()
{}

S_msrSemiTonesPitchAndAbsoluteOctave msrSemiTonesPitchAndAbsoluteOctave::createSemiTonesPitchAndAbsoluteOctaveNewbornClone ()
{
  S_msrSemiTonesPitchAndAbsoluteOctave
    newbornClone =
      msrSemiTonesPitchAndAbsoluteOctave::create (
        fSemiTonesPitchKind,
        fAbsoluteOctave);

  return newbornClone;
}

string msrSemiTonesPitchAndAbsoluteOctave::asString () const
{
  stringstream s;

  const int fieldWidth = 19;

  s << left <<
    "SemiTonesPitchAndAbsoluteOctave" <<
    ": " <<
    setw (fieldWidth) <<
    msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    ", absoluteOctave: " << fAbsoluteOctave;

  return s.str ();
}

void msrSemiTonesPitchAndAbsoluteOctave::print (ostream& os) const
{
  os <<
    "SemiTonesPitchAndAbsoluteOctave" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "semiTonesPitchKind" << " : " <<
      msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "absoluteOctave" << " : " << fAbsoluteOctave <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrSemiTonesPitchAndAbsoluteOctave& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrSemiTonesPitchAndRelativeOctave msrSemiTonesPitchAndRelativeOctave::create (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   relativeOctave)
{
  msrSemiTonesPitchAndRelativeOctave* o =
    new msrSemiTonesPitchAndRelativeOctave (
      semiTonesPitchKind,
      relativeOctave);
  assert (o != nullptr);

  return o;
}

msrSemiTonesPitchAndRelativeOctave::msrSemiTonesPitchAndRelativeOctave (
  msrSemiTonesPitchKind semiTonesPitchKind,
  int                   relativeOctave)
{
  fSemiTonesPitchKind = semiTonesPitchKind;

  fRelativeOctave = relativeOctave;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony item '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrSemiTonesPitchAndRelativeOctave::~msrSemiTonesPitchAndRelativeOctave ()
{}

S_msrSemiTonesPitchAndRelativeOctave msrSemiTonesPitchAndRelativeOctave::createSemiTonesPitchAndRelativeOctaveNewbornClone ()
{
  S_msrSemiTonesPitchAndRelativeOctave
    newbornClone =
      msrSemiTonesPitchAndRelativeOctave::create (
        fSemiTonesPitchKind,
        fRelativeOctave);

  return newbornClone;
}

string msrSemiTonesPitchAndRelativeOctave::asString () const
{
  stringstream s;

  const int fieldWidth = 19;

  s << left <<
    "SemiTonesPitchAndRelativeOctave" <<
    ": " <<
    setw (fieldWidth) <<
    msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    ", relativeOctave: " << fRelativeOctave;

  return s.str ();
}

void msrSemiTonesPitchAndRelativeOctave::print (ostream& os) const
{
  os <<
    "SemiTonesPitchAndRelativeOctave" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "semiTonesPitchKind" << " : " <<
      msrSemiTonesPitchKindAsString (fSemiTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "relativeOctave" << " : " << fRelativeOctave <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrSemiTonesPitchAndRelativeOctave& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}
*/

//______________________________________________________________________________
S_msrHarmonyContents msrHarmonyContents::create (
// JMI  int                   inputLineNumber,
  msrSemiTonesPitchKind harmonyContentsRootNote,
  msrHarmonyKind        harmonyContentsHarmonyKind)
{
  msrHarmonyContents* o =
    new msrHarmonyContents (
 //     inputLineNumber,
      harmonyContentsRootNote,
      harmonyContentsHarmonyKind);
  assert (o != nullptr);

  return o;
}

msrHarmonyContents::msrHarmonyContents (
// JMI  int                   inputLineNumber,
  msrSemiTonesPitchKind harmonyContentsRootNote,
  msrHarmonyKind        harmonyContentsHarmonyKind)
{
  fHarmonyContentsRootNote    = harmonyContentsRootNote;
  fHarmonyContentsHarmonyKind = harmonyContentsHarmonyKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmonyContents '" <<
      harmonyContentsAsString () <<
      "'" <<
      endl;
  }
#endif

  // create the root harmony element
  S_msrSemiTonesPitchAndOctave
    rootChordElement =
      msrSemiTonesPitchAndOctave::create (
        fHarmonyContentsRootNote,
        msrOctaveKind::kOctave0); // relative octave JMI ???

  // add it to the harmony elements
  fHarmonyElementsVector.push_back (rootChordElement);

  // add the other notes to the harmony elements
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        fHarmonyContentsHarmonyKind);

  const vector<S_msrHarmonyInterval>&
    harmonyIntervals =
      harmonyStructure->
        getHarmonyStructureIntervals ();

  for (size_t i = 1; i < harmonyIntervals.size (); ++i) {
    // get the interval
    msrIntervalKind
      intervalKind =
        harmonyIntervals [i]->
          getHarmonyIntervalIntervalKind ();

    // fetch the semitone pitch
    msrSemiTonesPitchKind
      semiTonePitch =
        noteAtIntervalFromSemiTonesPitch (
          0, // ??? JM                   inputLineNumber,
          intervalKind,
          fHarmonyContentsRootNote);

    // create the harmony element
    S_msrSemiTonesPitchAndOctave
      harmonyElement =
        msrSemiTonesPitchAndOctave::create (
          semiTonePitch,
          msrOctaveKind::kOctave0); // relative octave JMI ???

    // add it to the harmony elements
    fHarmonyElementsVector.push_back (harmonyElement);
  } // for
}

msrHarmonyContents::~msrHarmonyContents ()
{}

string msrHarmonyContents::harmonyContentsAsString () const
{
  stringstream s;

  s <<
    "HarmonyContents" <<
    ", " <<
    msrHarmonyKindAsString (fHarmonyContentsHarmonyKind) <<
    ", " <<
    mfSingularOrPlural (
      fHarmonyElementsVector.size (), "harmony element", "harmony elements");

  return s.str ();
}

msrSemiTonesPitchKind msrHarmonyContents::bassSemiTonesPitchKindForHarmonyInversion (
  int inputLineNumber,
  int inversionNumber)
{
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */

  if (
    inversionNumber < 0
      ||
    inversionNumber > int (fHarmonyElementsVector.size ()) - 1 ) {
    stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for harmony notes '" <<
      msrHarmonyKindAsString (fHarmonyContentsHarmonyKind) <<
      "', line " << inputLineNumber;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return
    fHarmonyElementsVector [inversionNumber]->
      getSemiTonesPitchKind ();
}

void msrHarmonyContents::printAllHarmoniesContents (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind)
{
  // fetch the root quartertones pitch kind
  msrQuarterTonesPitchKind
    rootQuarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        rootSemiTonesPitchKind);

  os <<
    "All the known harmonies contents with diatonic root '" <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      rootQuarterTonesPitchKind,
      gGlobalLpsrOahGroup->
        getLpsrQuarterTonesPitchesLanguageKind ()) <<
      /* JMI
    "' (" <<
    msrSemiTonesPitchKindAsString (
      rootSemiTonesPitchKind) <<
    ")" <<
    */
    "' in language '" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      gGlobalLpsrOahGroup->
        getLpsrQuarterTonesPitchesLanguageKind ()) <<
    "' 'are:" <<
    endl << endl;

  ++gIndenter;

  for (auto e : EnumTrueHarmonies<msrHarmonyKind> ()) {
    os <<
      msrHarmonyKindAsString (e) <<
      ":" <<
      endl;

    ++gIndenter;

    // create the harmony intervals
    S_msrHarmonyStructure
      harmonyStructure =
        msrHarmonyStructure::create (
          e);

    // fetch the intervals items for these intervals
    // with rootSemiTonesPitchKind as root
    const vector <S_msrHarmonyInterval>&
      harmonyStructureIntervals =
        harmonyStructure->
          getHarmonyStructureIntervals ();

    if (harmonyStructureIntervals.size ()) {
      // fetch the notes for these intervals
      vector<S_msrHarmonyInterval>::const_reverse_iterator
        iBegin = harmonyStructureIntervals.crbegin (),
        iEnd   = harmonyStructureIntervals.crend (),
        i      = iBegin;

      for ( ; ; ) {
        S_msrHarmonyInterval
          harmonyInterval = (*i);

        msrIntervalKind
          intervalKind =
            harmonyInterval->
              getHarmonyIntervalIntervalKind ();

        // fetch the semitones pitch kind
        msrSemiTonesPitchKind
          noteSemiTonesPitchKind =
            noteAtIntervalFromSemiTonesPitch (
              K_NO_INPUT_LINE_NUMBER,
              intervalKind,
              rootSemiTonesPitchKind);

        // fetch the quartertones pitch kind
        msrQuarterTonesPitchKind
          noteQuarterTonesPitchKind =
            quarterTonesPitchKindFromSemiTonesPitchKind (
              noteSemiTonesPitchKind);

        // print it
        const int fieldWidth2 = 8;

        os << left <<
          setw (fieldWidth2) <<
          msrQuarterTonesPitchKindAsStringInLanguage (
            noteQuarterTonesPitchKind,
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
          " : " <<
          msrIntervalKindAsString (intervalKind) <<
          endl;

        if (++i == iEnd) break;

        // no endl here
      } // for
    }

    os << endl;

    --gIndenter;
  } // for

  --gIndenter;
}

/* JMI
void msrHarmonyContents::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyContents::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyContents>*
    p =
      dynamic_cast<visitor<S_msrHarmonyContents>*> (v)) {
        S_msrHarmonyContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyContents::visitStart ()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarmonyContents::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyContents>*
    p =
      dynamic_cast<visitor<S_msrHarmonyContents>*> (v)) {
        S_msrHarmonyContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyContents::visitEnd ()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrHarmonyContents::browseData (basevisitor* v)
{}
*/

void msrHarmonyContents::print (ostream& os) const
{
  os <<
    "HarmonyContents" <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os << left <<
    setw (fieldWidth) <<
    "harmonyContentsRootNote" << " : " <<
    msrSemiTonesPitchKindAsString (fHarmonyContentsRootNote) <<
    endl <<
    setw (fieldWidth) <<
    "harmonyContentsHarmonyKind" << " : " <<
    msrHarmonyKindAsString (fHarmonyContentsHarmonyKind) <<
    endl;

  if (fHarmonyElementsVector.size ()) {
    os <<
    mfSingularOrPlural (
      fHarmonyElementsVector.size (), "harmony element", "harmony elements") <<
    ":" <<
    endl;

    ++gIndenter;

    for (size_t i = 0; i < fHarmonyElementsVector.size (); ++i) {
      S_msrSemiTonesPitchAndOctave
        harmonyElement =
          fHarmonyElementsVector [i];

      os <<
        harmonyElement <<
        endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "no notes" <<
      endl;
  }

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrHarmonyContents& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
void printHarmonyDetails (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind)
{
  // fetch the quartertones pitch kind
  msrQuarterTonesPitchKind
    rootQuarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        rootSemiTonesPitchKind);

  string
    rootQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        rootQuarterTonesPitchKind,
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  string
    harmonyKindShortName =
      msrHarmonyKindShortName (
        harmonyKind);

  // print the deails
  os <<
    "The details of harmony '" <<
    rootQuarterTonesPitchKindAsString <<
    ' ' <<
    harmonyKindShortName <<
    "' are:" <<
    endl << endl;

  ++gIndenter;

  // create the harmony intervals
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        harmonyKind);

  // fetch the intervals items for these intervals
  // with rootSemiTonesPitchKind as root
  const vector <S_msrHarmonyInterval>&
    harmonyStructureIntervals =
      harmonyStructure->
        getHarmonyStructureIntervals ();

  // loop on all the inversion
  int harmonyStructureIntervalsNumber =
    harmonyStructureIntervals.size ();

  if (harmonyStructureIntervalsNumber) {
    for (int inversion = 0; inversion < harmonyStructureIntervalsNumber; ++inversion) {
      // invert the harmony structure
      S_msrHarmonyStructure
        invertedHarmonyStructure =
          harmonyStructure->
            invertHarmonyStructure (inversion);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        os <<
          "==> inversion = " << inversion <<
          ", initial invertedHarmonyStructure:" <<
          endl;

        ++gIndenter;
        os <<
          invertedHarmonyStructure <<
          endl;
        --gIndenter;
      }
#endif

      // get the inverted harmony structure intervals
      const vector <S_msrHarmonyInterval>&
        invertedHarmonyStructureIntervals =
          invertedHarmonyStructure->
            getHarmonyStructureIntervals ();

      // fetch the notes for these intervals
      /* JMI
      vector<S_msrHarmonyInterval>::const_iterator
        iBegin = invertedHarmonyStructureIntervals.begin (),
        iEnd   = invertedHarmonyStructureIntervals.end (),
        i      = iBegin;
        */
      vector<S_msrHarmonyInterval>::const_reverse_iterator
        iBegin = invertedHarmonyStructureIntervals.crbegin (),
        iEnd   = invertedHarmonyStructureIntervals.crend (),
        i      = iBegin;

      os <<
        "Chord '" <<
        rootQuarterTonesPitchKindAsString <<
        ' ' <<
        harmonyKindShortName <<
        "'";

      if (inversion == 0) {
        os <<
          " fundamental state";
      }
      else {
        os <<
          " inversion " << inversion;
      }

      os <<
        " contents, " <<
        invertedHarmonyStructureIntervals.size () <<
        " intervals:" <<
        endl;

      ++gIndenter;

      for ( ; ; ) {
        S_msrHarmonyInterval
          harmonyInterval = (*i);

        msrIntervalKind
          intervalKind =
            harmonyInterval->
              getHarmonyIntervalIntervalKind ();

        const int fieldWidth1 = 17;

        os << left <<
          setw (fieldWidth1) <<
          msrIntervalKindAsString (intervalKind) <<
          ": ";

        // fetch the semitones pitch kind
        msrSemiTonesPitchKind
          noteSemiTonesPitchKind =
            noteAtIntervalFromSemiTonesPitch (
              K_NO_INPUT_LINE_NUMBER,
              intervalKind,
              rootSemiTonesPitchKind);

        // fetch the quartertones pitch kind
        msrQuarterTonesPitchKind
          noteQuarterTonesPitchKind =
            quarterTonesPitchKindFromSemiTonesPitchKind (
              noteSemiTonesPitchKind);

        // print it
        const int fieldWidth2 = 8;

        os << left <<
          setw (fieldWidth2) <<
          msrQuarterTonesPitchKindAsStringInLanguage (
            noteQuarterTonesPitchKind,
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
            /* JMI
          ", octave " << relativeOctave <<
          " (" <<
          msrSemiTonesPitchKindAsString (
            noteSemiTonesPitchKind) <<
          ")" <<
          */
          endl;

        if (++i == iEnd) break;

        // no endl here
      } // for

      --gIndenter;

      os << endl;
    } // for
  }

  --gIndenter;
}

//______________________________________________________________________________
void printHarmonyAnalysis (
  ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind,
  int                   inversion)
{
  // fetch the quartertones pitch kind
  msrQuarterTonesPitchKind
    rootQuarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        rootSemiTonesPitchKind);

  string
    rootQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        rootQuarterTonesPitchKind,
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  string
    harmonyKindShortName =
      msrHarmonyKindShortName (
        harmonyKind);

  // print the deails
  os <<
    "The analysis of harmony '" <<
    rootQuarterTonesPitchKindAsString <<
    ' ' <<
    harmonyKindShortName <<
    "' inversion " <<
    inversion <<
    " is:" <<
    endl << endl;

  ++gIndenter;

  // create the harmony intervals
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        harmonyKind);

  // fetch the intervals items for these intervals
  // with rootSemiTonesPitchKind as root
  const vector <S_msrHarmonyInterval>&
    harmonyStructureIntervals =
      harmonyStructure->
        getHarmonyStructureIntervals ();

  // loop on all the inversion
  int harmonyStructureIntervalsNumber =
    harmonyStructureIntervals.size ();

  if (harmonyStructureIntervalsNumber) {
    if (inversion < harmonyStructureIntervalsNumber) {
      // invert the harmony structure
      S_msrHarmonyStructure
        invertedHarmonyStructure =
          harmonyStructure->
            invertHarmonyStructure (inversion);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        os <<
          "==> inversion = " << inversion <<
          ", initial invertedHarmonyStructure:" <<
          endl;

        ++gIndenter;
        os <<
          invertedHarmonyStructure <<
          endl;
        --gIndenter;
      }
#endif

      // get the inverted harmony structure intervals
      const vector <S_msrHarmonyInterval>&
        invertedHarmonyStructureIntervals =
          invertedHarmonyStructure->
            getHarmonyStructureIntervals ();

      // print the harmony contents
      {
        os <<
          "Harmony '" <<
          rootQuarterTonesPitchKindAsString <<
          ' ' <<
          harmonyKindShortName <<
          "'";

        if (inversion == 0) {
          os <<
            " fundamental state";
        }
        else {
          os <<
            " inversion " << inversion;
        }

        os <<
          " contents, " <<
          invertedHarmonyStructureIntervals.size () <<
          " intervals:" <<
          endl;

        ++gIndenter;

        vector<S_msrHarmonyInterval>::const_reverse_iterator
          iBegin = invertedHarmonyStructureIntervals.crbegin (),
          iEnd   = invertedHarmonyStructureIntervals.crend (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrHarmonyInterval
            harmonyInterval = (*i);

          msrIntervalKind
            intervalKind =
              harmonyInterval->
                getHarmonyIntervalIntervalKind ();

          // fetch the semitones pitch kind
          msrSemiTonesPitchKind
            noteSemiTonesPitchKind =
              noteAtIntervalFromSemiTonesPitch (
                K_NO_INPUT_LINE_NUMBER,
                intervalKind,
                rootSemiTonesPitchKind);

          // fetch the quartertones pitch kind
          msrQuarterTonesPitchKind
            noteQuarterTonesPitchKind =
              quarterTonesPitchKindFromSemiTonesPitchKind (
                noteSemiTonesPitchKind);

          // print it
          const int fieldWidth2 = 5;

          os << left <<
            setw (fieldWidth2) <<
            msrQuarterTonesPitchKindAsStringInLanguage (
              noteQuarterTonesPitchKind,
              gGlobalLpsrOahGroup->
                getLpsrQuarterTonesPitchesLanguageKind ()) <<
            " : " <<
            msrIntervalKindAsString (intervalKind) <<
            endl;

          if (++i == iEnd) break;

          // no endl here
        } // for

        --gIndenter;

        os << endl;
      }

      // print the harmony's inner intervals
      {
        os <<
          "Chord '" <<
          rootQuarterTonesPitchKindAsString <<
          ' ' <<
          harmonyKindShortName <<
          "'";

        if (inversion == 0) {
          os <<
            " fundamental state";
        }
        else {
          os <<
            " inversion " << inversion;
        }

        os <<
          " inner intervals:" <<
          endl;

        ++gIndenter;

        int tritonsCounter = 0;

        vector<S_msrHarmonyInterval>::const_iterator
          iBegin1 = invertedHarmonyStructureIntervals.begin (),
          iEnd1   = invertedHarmonyStructureIntervals.end () - 1,
          i1      = iBegin1;

        for ( ; ; ) {
          S_msrHarmonyInterval
            harmonyInterval1 = (*i1);

          msrIntervalKind
            intervalKind1 =
              harmonyInterval1->
                getHarmonyIntervalIntervalKind ();

          // fetch the semitones pitch kind
          msrSemiTonesPitchKind
            noteSemiTonesPitchKind1 =
              noteAtIntervalFromSemiTonesPitch (
                K_NO_INPUT_LINE_NUMBER,
                intervalKind1,
                rootSemiTonesPitchKind);

          // fetch the quartertones pitch kind
          msrQuarterTonesPitchKind
            noteQuarterTonesPitchKind1 =
              quarterTonesPitchKindFromSemiTonesPitchKind (
                noteSemiTonesPitchKind1);

          // print the invervals
          vector<S_msrHarmonyInterval>::const_iterator
            iBegin2 = i1 + 1,
            iEnd2   = invertedHarmonyStructureIntervals.end (),
            i2      = iBegin2;

          for ( ; ; ) {
            S_msrHarmonyInterval
              harmonyInterval2 = (*i2);

            msrIntervalKind
              intervalKind2 =
                harmonyInterval2->
                  getHarmonyIntervalIntervalKind ();

            const int fieldWidth1 = 5;

            // fetch the semitones pitch kind
            msrSemiTonesPitchKind
              noteSemiTonesPitchKind2 =
                noteAtIntervalFromSemiTonesPitch (
                  K_NO_INPUT_LINE_NUMBER,
                  intervalKind2,
                  rootSemiTonesPitchKind);

            // fetch the quartertones pitch kind
            msrQuarterTonesPitchKind
              noteQuarterTonesPitchKind2 =
                quarterTonesPitchKindFromSemiTonesPitchKind (
                  noteSemiTonesPitchKind2);

            // compute the inner interval
            S_msrHarmonyInterval
              interInterval =
                harmonyInterval2->
                  intervalDifference (
                    harmonyInterval1);

            msrIntervalKind
              innerIntervalKind =
                interInterval->
                  getHarmonyIntervalIntervalKind ();

            // is this interval a triton?
            switch (innerIntervalKind) {
              case msrIntervalKind::kIntervalAugmentedFourth:
              case msrIntervalKind::kIntervalDiminishedFifth:
                ++tritonsCounter;
                break;
              default:
                ;
            } // switch

            // print it
            ++gIndenter;

            const int fieldWidth2 = 20;

            os << left <<
              setw (fieldWidth1) <<
              msrQuarterTonesPitchKindAsStringInLanguage (
                noteQuarterTonesPitchKind1,
                gGlobalLpsrOahGroup->
                  getLpsrQuarterTonesPitchesLanguageKind ()) <<

              " -> " <<

              setw (fieldWidth1) <<
              msrQuarterTonesPitchKindAsStringInLanguage (
                noteQuarterTonesPitchKind2,
                gGlobalLpsrOahGroup->
                  getLpsrQuarterTonesPitchesLanguageKind ()) <<

              " : " <<

              setw (fieldWidth2) << // JMI
              msrIntervalKindAsString (innerIntervalKind) <<

              '(' <<
              setw (fieldWidth2) <<
              msrIntervalKindAsString (intervalKind1) <<
              " -> " <<
              msrIntervalKindAsString (intervalKind2) <<
              ")" <<

              endl;

            --gIndenter;

            if (++i2 == iEnd2) break;
          } // for

          if (++i1 == iEnd1) break;

          os << endl;
        } // for

        --gIndenter;


        if (tritonsCounter > 0) {
          os <<
            "This harmony contains " <<
            mfSingularOrPlural (
              tritonsCounter, "triton", "tritons") <<
            endl;
        }

        os << endl;
      }
    }

    else {
      os <<
        "Chord '" <<
        rootQuarterTonesPitchKindAsString <<
        ' ' <<
        harmonyKindShortName <<
        "' has only " <<
        harmonyStructureIntervalsNumber <<
        " intervals, inversion " <<
        inversion <<
        " does not exist" <<
        endl << endl;
    }
  }

  --gIndenter;
}

// RGB colors
//______________________________________________________________________________
msrRGBColor::msrRGBColor ()
{
  // initializing to negative values for isEmpty()
  fR = -1.0;
  fG = -1.0;
  fB = -1.0;
}

msrRGBColor::msrRGBColor (
  float theR,
  float theG,
  float theB)
{
  fR = theR;
  fG = theG;
  fB = theB;
}

msrRGBColor::msrRGBColor (
  const string& theString)
{
  string regularExpression (
    "([[:digit:]]*.[[:digit:]]*)" // RString
    ","
    "([[:digit:]]*.[[:digit:]]*)" // GString
    ","
    "([[:digit:]]*.[[:digit:]]*)" // BString
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for RGB color string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 4) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          '[' << sm [i] << "] ";
      } // for
      gLogStream << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "msrRGBColor string '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    RString = sm [1],
    GString = sm [2],
    BString = sm [3];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "--> RString = \"" << RString << "\", " <<
      "--> GString = \"" << GString << "\"" <<
      "--> BString = \"" << BString << "\"" <<
      endl;
  }
#endif

  // are these strings alright?
  {
    stringstream s;

    s << RString;
    s >> fR;

    if (fR < 0.0 || fR > 1.0) {
      gLogStream <<
        "### ERROR: the R component " << fR <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        endl;
    }
  }
  {
    stringstream s;

    s << GString;
    s >> fG;

    if (fG < 0.0 || fG > 1.0) {
      gLogStream <<
        "### ERROR: the G component " << fG <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        endl;
    }
  }
  {
    stringstream s;

    s << BString;
    s >> fB;

    if (fB < 0.0 || fB > 1.0) {
      gLogStream <<
        "### ERROR: the B component " << fB <<
        " is not in the [0.0..1.0] interval in RGB color '" << theString << "'" <<
        endl;
    }
  }

}

string msrRGBColor::asString (int precision) const
{
  stringstream s;

  s <<
    "[RGBColor " <<
    setprecision (precision) <<
    '[' <<
    fR <<
    "," <<
    fG <<
    "," <<
    fB <<
    "]]";

  return s.str ();
}

string msrRGBColor::asSpaceSeparatedString (int precision) const
{
  stringstream s;

  s <<
    setprecision (precision) <<
    fR <<
    ' ' <<
    fG <<
    ' ' <<
    fB;

  return s.str ();
}

void msrRGBColor::print (ostream& os) const
{
  os << asString () << endl;
};

ostream& operator << (ostream& os, const msrRGBColor& elt)
{
  elt.print (os);
  return os;
}

// AlphaRGB colors
//______________________________________________________________________________
msrAlphaRGBColor::msrAlphaRGBColor (
  const string& colorRGB,
  const string& colorAlpha)
{
  fColorRGB   = colorRGB;
  fColorAlpha = colorAlpha;
}

msrAlphaRGBColor::msrAlphaRGBColor (
  const string& colorRGB)
{
  fColorRGB   = colorRGB;
  fColorAlpha = "FF";
}

msrAlphaRGBColor::~msrAlphaRGBColor ()
{}

string msrAlphaRGBColor::asString () const
{
  stringstream s;

  s << "[AlphaRGBColor: " <<
    "colorRGB: \"" << fColorRGB <<
    "\", colorAlpha: \"" << fColorAlpha <<
    "\"]";

  return s.str ();
}

void msrAlphaRGBColor::print (ostream& os) const
{
  os << asString () << endl;
};

ostream& operator << (ostream& os, const msrAlphaRGBColor& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
string msrScoreNotationKindAsString (
  msrScoreNotationKind lilypondScoreNotationKind)
{
  string result;

  switch (lilypondScoreNotationKind) {
    case msrScoreNotationKind::kScoreNotationWestern:
      result = "kScoreNotationWestern";
    case msrScoreNotationKind::kScoreNotationJianpu:
      result = "kScoreNotationWestern";
    case msrScoreNotationKind::kScoreNotationDiatonicAccordion:
      result = "kScoreNotationDiatonicAccordion";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrScoreNotationKind& elt)
{
  os << msrScoreNotationKindAsString (elt);
  return os;
}

//______________________________________________________________________________
void initializeMsrBasicTypes ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (
      gGlobalOahEarlyOptions.getEarlyTracingOah ()
        &&
      ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
    ) {
      gLogStream <<
        "Initializing MSR basic types handling" <<
        endl;
  }
#endif

    // languages handling
    // ------------------------------------------------------

    initializeQuarterTonesPitchesLanguageKinds ();

    // clefs handling
    // ------------------------------------------------------

    initializeClefKinds ();

    // harmonies handling
    // ------------------------------------------------------

    initializeHarmonyKinds ();

    // harmony structures handling
    // ------------------------------------------------------

    initializeHarmonyStructuresMap ();

    // MSR lengths handling
    // ------------------------------------------------------

    initializeMsrLengthUnitKindsMap ();

    // MSR margins types handling
    // ------------------------------------------------------

    initializeMsrMarginTypeKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}


}

/* JMI
#include <algorithm>    // for_each

    // Iterate over a map using std::for_each and Lambda function
    for_each (
      getQuarterTonesPitchesLanguageKindsMap ().begin(),
      getQuarterTonesPitchesLanguageKindsMap ().end(),
        [] (pair<string, getQuarterTonesPitchesLanguageKindsMap ()> element) {
          string theString = (element).first;

          ++count;

          cumulatedLength += theString.size ();
          if (cumulatedLength >= namesListMaxLength) {
            s << endl << gIndenter.getSpacer ();
            cumulatedLength = 0;
          }

          s << theString;

          if (count == nextToLast) {
            s << " and ";
          }
          else {
            s << ", ";
          }
        }
    ); // foreach
*/

