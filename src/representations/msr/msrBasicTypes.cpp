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

// #include "msrBasicTypes.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "lpsrOah.h"

#include "oahEarlyOptions.h"


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

// intervals
//______________________________________________________________________________
int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind)
{
  int result = INT_MIN;

  switch (intervalKind) {
    case msrIntervalKind::kInterval_NO_:
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
    case msrIntervalKind::kInterval_NO_:
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
    case msrIntervalKind::kInterval_NO_:
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
  msrIntervalKind result = msrIntervalKind::kInterval_NO_;

  switch (intervalKind) {
    case msrIntervalKind::kInterval_NO_:
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
    case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
  msrQuarterTonesPitchKind result = msrQuarterTonesPitchKind::kQTP_NO_;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_NO_:
      result = msrQuarterTonesPitchKind::kQTP_NO_;
      break;

    case msrQuarterTonesPitchKind::kQTP_C_Flat:
      break;

    case msrQuarterTonesPitchKind::kQTP_C_Natural:
      switch (intervalKind) {
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
        case msrIntervalKind::kInterval_NO_:
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
  msrIntervalKind result = msrIntervalKind::kInterval_NO_;

  msrSemiTonesPitchKind
    workSemiTonesPitch1 = semiTonesPitch1,
    workSemiTonesPitch2 = semiTonesPitch2;

  // order the semitones pitches so that
  // workSemiTonesPitch1 is greater or equal to workSemiTonesPitch2
  // according to the enum class type
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
    case msrModeKind::kMode_NO_:
      result = "***kMode_NO_***";
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
  msrModeKind result = msrModeKind::kMode_NO_;

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

//______________________________________________________________________________
void setDiatonicPitchAndAlterationKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind)
{
  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_NO_:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA; // any value would fit
      alterationKind    = msrAlterationKind::kAlteration_NO_;
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
      alterationKind    = msrAlterationKind::kAlteration_NO_;

      break;
  } // switch
}

string msrQuarterTonesPitchKindAsString (
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  string result;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_NO_:
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
      msrQuarterTonesPitchKind::kQTP_NO_;

  switch (diatonicPitchKind) {
    case msrDiatonicPitchKind::kDiatonicPitch_NO_:
      {
        result = msrQuarterTonesPitchKind::kQTP_NO_;
        /* JMI
        stringstream s;

        s <<
          "cannot convert msrDiatonicPitchKind::kDiatonicPitch_NO_ to a quarter tones pitch"
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
        case msrAlterationKind::kAlteration_NO_:
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
        case msrAlterationKind::kAlteration_NO_:
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
        case msrAlterationKind::kAlteration_NO_:
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
        case msrAlterationKind::kAlteration_NO_:
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
        case msrAlterationKind::kAlteration_NO_:
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
        case msrAlterationKind::kAlteration_NO_:
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
        case msrAlterationKind::kAlteration_NO_:
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
  msrDiatonicPitchKind result = msrDiatonicPitchKind::kDiatonicPitch_NO_;

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

    case msrQuarterTonesPitchKind::kQTP_NO_:
      {
        result = msrDiatonicPitchKind::kDiatonicPitch_NO_;

        /* JMI
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a msrQuarterTonesPitchKind::kQTP_NO_"
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
  msrAlterationKind result = msrAlterationKind::kAlteration_NO_;

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

    case msrQuarterTonesPitchKind::kQTP_NO_:
      {
        result = msrAlterationKind::kAlteration_NO_;

        /* JMI
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a msrQuarterTonesPitchKind::kQTP_NO_"
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
  msrQuarterTonesPitchKind result = msrQuarterTonesPitchKind::kQTP_NO_;

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
      case msrQuarterTonesPitchKind::kQTP_NO_:
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

// tuplets
//______________________________________________________________________________

EXP string msrTupletInKindAsString (
  msrTupletInKind tupletInKind)
{
  string result;

  switch (tupletInKind) {
    case msrTupletInKind::kTupletIn_NO_:
      result = "***kTupletIn_NO_***";
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

  msrIntervalKind resultIntervalKind   = msrIntervalKind::kInterval_NO_;

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
  // according to the enum class type
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
    case msrIntervalKind::kInterval_NO_:
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
  msrIntervalKind resultIntervalKind   = msrIntervalKind::kInterval_NO_;
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
  // according to the enum class type
  Bool invertInterval (false);

  if (intervalKind1 < intervalKind2) {
    intervalKind1 = intervalKind2;
    intervalKind2 = intervalKind1;
    invertInterval = true;
  }

  switch (intervalKind1) {
    case msrIntervalKind::kInterval_NO_:
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
    case msrHarmonyKind::kHarmony_NO_:
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
              K_MF_NO_INPUT_LINE_NUMBER,
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

