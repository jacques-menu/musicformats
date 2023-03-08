/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrIntervals___
#define ___msrIntervals___

#include "msrPitches.h"


namespace MusicFormats
{

//______________________________________________________________________________
// intervals
enum class msrIntervalKind {
  kInterval_UNKNOWN_,

  kIntervalDiminishedUnisson, kIntervalPerfectUnisson,
  kIntervalAugmentedUnisson,

  kIntervalDiminishedSecond, kIntervalMinorSecond,
  kIntervalMajorSecond, kIntervalAugmentedSecond,

  kIntervalDiminishedThird, kIntervalMinorThird,
  kIntervalMajorThird, kIntervalAugmentedThird,

  kIntervalDiminishedFourth, kIntervalPerfectFourth,
  kIntervalAugmentedFourth,

  kIntervalDiminishedFifth, kIntervalPerfectFifth,
  kIntervalAugmentedFifth,

  kIntervalDiminishedSixth, kIntervalMinorSixth,
  kIntervalMajorSixth, kIntervalAugmentedSixth,

  kIntervalDiminishedSeventh, kIntervalMinorSeventh,
  kIntervalMajorSeventh, kIntervalAugmentedSeventh,

  kIntervalDiminishedOctave, kIntervalPerfectOctave,
  kIntervalAugmentedOctave,

  kIntervalDiminishedNinth, kIntervalMinorNinth,
  kIntervalMajorNinth, kIntervalAugmentedNinth,

  kIntervalDiminishedTenth, kIntervalMinorTenth,
  kIntervalMajorTenth, kIntervalAugmentedTenth,

  kIntervalDiminishedEleventh, kIntervalPerfectEleventh,
  kIntervalAugmentedEleventh,

  kIntervalDiminishedTwelfth, kIntervalPerfectTwelfth,
  kIntervalAugmentedTwelfth,

  kIntervalDiminishedThirteenth, kIntervalMinorThirteenth,
  kIntervalMajorThirteenth, kIntervalAugmentedThirteenth
};

std::string msrIntervalKindAsString (
  msrIntervalKind intervaKindl);

std::string msrIntervalKindAsShortString (
  msrIntervalKind intervalKind);

std::ostream& operator << (std::ostream& os, const msrIntervalKind& elt);

EXP int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind);

EXP int msrIntervalKindAsQuarterTones (
  msrIntervalKind intervalKind);

EXP msrIntervalKind invertIntervalKind (
  msrIntervalKind intervalKind);

EXP int intervalKindAsSemitones (
  msrIntervalKind intervalKind);

/* JMI
EXP msrSemiTonesPitchKind noteAtIntervalKindFromNote (
  msrIntervalKind             intervalKind,
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);
*/

EXP msrSemiTonesPitchKind noteAtIntervalFromSemiTonesPitch (
  int                   inputLineNumber,
  msrIntervalKind       intervalKind,
  msrSemiTonesPitchKind semiTonesPitchKind);

EXP msrQuarterTonesPitchKind noteAtIntervalFromQuarterTonesPitch (
  int                      inputLineNumber,
  msrIntervalKind          intervalKind,
  msrQuarterTonesPitchKind quarterTonesPitchKind);

EXP msrIntervalKind intervalBetweenSemiTonesPitches (
  msrSemiTonesPitchKind semiTonesPitch1,
  msrSemiTonesPitchKind semiTonesPitch2);

}

#endif // ___msrIntervals___
