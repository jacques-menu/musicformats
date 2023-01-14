/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <sstream>

#include "mfServices.h"

#include "msrIntervals.h"

#include "msrWae.h"

#include "msrOah.h"


namespace MusicFormats
{

// intervals
//______________________________________________________________________________
int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind)
{
  int result = INT_MIN;

  switch (intervalKind) {
    case msrIntervalKind::kInterval_UNKNOWN:
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

std::string msrIntervalKindAsString (
  msrIntervalKind intervalKind)
{
  std::string result;

  switch (intervalKind) {
    case msrIntervalKind::kInterval_UNKNOWN:
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

std::ostream& operator << (std::ostream& os, const msrIntervalKind& elt)
{
  os << msrIntervalKindAsString (elt);
  return os;
}

std::string msrIntervalAsShortString (
  msrIntervalKind intervalKind)
{
  std::string result;

  switch (intervalKind) {
    case msrIntervalKind::kInterval_UNKNOWN:
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
  msrIntervalKind result = msrIntervalKind::kInterval_UNKNOWN;

  switch (intervalKind) {
    case msrIntervalKind::kInterval_UNKNOWN:
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
    case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        std::stringstream s;

        s <<
          "Sorry, computing intervals from semitones pitch '" <<
          msrSemiTonesPitchKindAsString (semiTonesPitchKind) <<
          '(' << msrSemiTonesPitchKindAsString (semiTonesPitchKind) << ")" <<
          "' is not supported"
          ", line: " << inputLineNumber;

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
  msrQuarterTonesPitchKind result = msrQuarterTonesPitchKind::kQTP_UNKNOWN;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_UNKNOWN:
      result = msrQuarterTonesPitchKind::kQTP_UNKNOWN;
      break;

    case msrQuarterTonesPitchKind::kQTP_C_Flat:
      break;

    case msrQuarterTonesPitchKind::kQTP_C_Natural:
      switch (intervalKind) {
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        case msrIntervalKind::kInterval_UNKNOWN:
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
        std::stringstream s;

        s <<
          "Sorry, computing intervals from quartertones pitch '" <<
          msrQuarterTonesPitchKindAsStringInLanguage (
            quarterTonesPitchKind,
            gGlobalMsrOahGroup->
              getMsrQuarterTonesPitchesLanguageKind ()) <<
          '(' << msrQuarterTonesPitchKindAsString (quarterTonesPitchKind) << ")" <<
          "' is not supported"
          ", line: " << inputLineNumber;

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
  msrIntervalKind result = msrIntervalKind::kInterval_UNKNOWN;

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


}
