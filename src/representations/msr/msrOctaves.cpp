/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw()), set::precision(), ...

#include <regex>

#include "mfIndentedTextOutput.h"
#include "mfServiceRunData.h"

#include "msrOctaves.h"

#include "msrOah.h"

#include "oahWae.h"
#include "msrWae.h"


namespace MusicFormats
{

// octaves
//______________________________________________________________________________
EXP int octaveNumberFromOctaveKind (msrOctaveKind octaveKind)
{
  int result = -2;

  switch (octaveKind) {
    case msrOctaveKind::kOctave_NO_:
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
  msrOctaveKind result = msrOctaveKind::kOctave_NO_;

  switch (octaveKind) {
    case msrOctaveKind::kOctave_NO_:
      result = msrOctaveKind::kOctave_NO_;
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
      result = msrOctaveKind::kOctave_NO_;
      break;
  } // switch

  return result;
}

msrOctaveKind octavePred (msrOctaveKind octaveKind)
{
  msrOctaveKind result = msrOctaveKind::kOctave_NO_;

  switch (octaveKind) {
    case msrOctaveKind::kOctave_NO_:
      result = msrOctaveKind::kOctave_NO_;
      break;
    case msrOctaveKind::kOctave0:
      result = msrOctaveKind::kOctave_NO_;
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
  msrOctaveKind result = msrOctaveKind::kOctave_NO_;

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
    case msrOctaveKind::kOctave_NO_:
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
    case msrOctaveKind::kOctave_NO_:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
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
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
  } // switch
}

void msrSemiTonesPitchAndOctave::decrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::kOctave_NO_:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
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

// quartertone pitches and absolute octave
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
    case msrOctaveKind::kOctave_NO_:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
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
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
  } // switch
}

void msrQuarterTonesPitchAndOctave::decrementOctaveKind ()
{
  switch (fOctaveKind) {
    case msrOctaveKind::kOctave_NO_:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
      break;
    case msrOctaveKind::kOctave0:
      fOctaveKind = msrOctaveKind::kOctave_NO_;
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


}

