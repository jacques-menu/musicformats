/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "mfEnableSanityChecksSetting.h"

#include "visitor.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfEnumAll.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "mfEnableTracingSetting.h"

#include "msrMeasureConstants.h"

#include "msrPitchesNames.h"

#include "msrHarmonies.h"
#include "msrTablatures.h"

#include "oahOah.h"
#include "msrOah.h"
#include "lpsrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony item '" <<
      harmonyIntervalAsString () <<
      "'" <<
      std::endl;
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

std::string msrHarmonyInterval::harmonyIntervalAsString () const
{
  std::stringstream s;

  const int fieldWidth = 19;

  s << std::left <<
    "HarmonyInterval" <<
    ": " <<
    std::setw (fieldWidth) <<
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    "harmonyIntervalRelativeOctave: " << fHarmonyIntervalRelativeOctave;

  return s.str ();
}

std::string msrHarmonyInterval::harmonyIntervalAsShortString () const
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
  const S_msrHarmonyInterval& otherHarmonyInterval)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      std::endl <<
      "--> computing intervalDifference betwwen '" <<
      asShortString () <<
      "' and '" <<
      otherHarmonyInterval->asShortString () <<
      "'" <<
      std::endl;
  }
#endif

  msrIntervalKind resultIntervalKind   = msrIntervalKind::kInterval_UNKNOWN;

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      "--> normalized operands are '" <<
      operand1->asShortString () <<
      "' and '" <<
      operand2->asShortString () <<
      "'" <<
      std::endl;
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      "--> permuteRelativeOctaves = " <<
      permuteRelativeOctaves <<
      ", invertInterval = " <<
      invertInterval <<
      ", resultRelativeOctave = " <<
      resultRelativeOctave <<
      std::endl;
  }
#endif

  // compute resulting interval Kind
  switch (intervalKind1) {
    case msrIntervalKind::kInterval_UNKNOWN:
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      "--> base resultIntervalKind = '" <<
      msrIntervalKindAsString (resultIntervalKind) <<
      "'" <<
      std::endl;
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceExtraHarmonies ()) {
    gLogStream <<
      "--> result = '" <<
      result->asShortString () <<
      "'" <<
      std::endl << std::endl;
  }
#endif

  // return it;
  return result;
}

S_msrHarmonyInterval msrHarmonyInterval::intervalSum (
  const S_msrHarmonyInterval& otherHarmonyInterval)
{
  msrIntervalKind resultIntervalKind   = msrIntervalKind::kInterval_UNKNOWN;
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
    case msrIntervalKind::kInterval_UNKNOWN:
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
      std::endl;
  }

  if (visitor<S_msrHarmonyInterval>*
    p =
      dynamic_cast<visitor<S_msrHarmonyInterval>*> (v)) {
        S_msrHarmonyInterval elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyInterval::visitStart ()" <<
             std::endl;
        p->visitStart (elem);
  }
}

void msrHarmonyInterval::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyInterval::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrHarmonyInterval>*
    p =
      dynamic_cast<visitor<S_msrHarmonyInterval>*> (v)) {
        S_msrHarmonyInterval elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyInterval::visitEnd ()" <<
            std::endl;
        p->visitEnd (elem);
  }
}

void msrHarmonyInterval::browseData (basevisitor* v)
{}
*/

std::string msrHarmonyInterval::asString () const
{
  std::stringstream s;

  s <<
    "[HarmonyInterval " <<
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    ", fHarmonyIntervalRelativeOctave: " <<
    fHarmonyIntervalRelativeOctave <<
    ']';

  return s.str ();
}

std::string msrHarmonyInterval::asShortString () const
{
  std::stringstream s;

  s <<
    '[' <<
    msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    ", rel.oct. " <<
    fHarmonyIntervalRelativeOctave <<
    ']';

  return s.str ();
}

void msrHarmonyInterval::print (std::ostream& os) const
{
  os <<
    "HarmonyInterval" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 22;

  os << std::left <<
  /* JMI
    std::setw (fieldWidth) <<
    "harmonyIntervalNumber" << ": " << fHarmonyIntervalNumber <<
    std::endl <<
    */
    std::setw (fieldWidth) <<
    "harmonyIntervalIntervalKind" << ": " <<
      msrIntervalKindAsString (fHarmonyIntervalIntervalKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "harmonyIntervalRelativeOctave" << ": " << fHarmonyIntervalRelativeOctave <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrHarmonyInterval& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

// modes
//______________________________________________________________________________
std::string msrModeKindAsString (
  msrModeKind modeKind)
{
  std::string result;

  switch (modeKind) {
    case msrModeKind::kMode_UNKNOWN:
      result = "kMode_UNKNOWN";
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

std::ostream& operator << (std::ostream& os, const msrModeKind& elt)
{
  os << msrModeKindAsString (elt);
  return os;
}

msrModeKind modeKindFromString (
  int           inputLineNumber,
  const std::string& modeString)
{
  // no CamelCase here, these strings are used in the command line options
  msrModeKind result = msrModeKind::kMode_UNKNOWN;

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
    std::stringstream s;

    s <<
      "mode std::string \"" <<
      modeString <<
      "\" is unknown" <<
      ", line: " << inputLineNumber;

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

// harmonies
//______________________________________________________________________________
// constant
const int K_HARMONY_INVERSION_NONE = -1;

std::string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind)
{
  std::string result;

  switch (harmonyKind) {
    case msrHarmonyKind::kHarmony_UNKNOWN:
      result = "kHarmony_UNKNOWN";
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

std::ostream& operator << (std::ostream& os, const msrHarmonyKind& elt)
{
  os << msrHarmonyKindAsString (elt);
  return os;
}

std::string msrHarmonyKindAsShortString (
  msrHarmonyKind harmonyKind)
{
  std::string result;

  switch (harmonyKind) {
    case msrHarmonyKind::kHarmony_UNKNOWN:
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

std::string msrHarmonyKindShortName (
  msrHarmonyKind harmonyKind)
{
  std::string result;

  switch (harmonyKind) {
    case msrHarmonyKind::kHarmony_UNKNOWN:
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
  const std::string& theString)
{
  msrHarmonyKind result = msrHarmonyKind::kHarmony_UNKNOWN;

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

std::map<std::string, msrHarmonyKind>
  gGlobalHarmonyKindsMap;

std::list<std::string>
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

std::string existingHarmonyKinds (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t harmonyKindsMapSize =
    gGlobalHarmonyKindsMap.size ();

  if (harmonyKindsMapSize) {
    size_t nextToLast =
      harmonyKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, msrHarmonyKind>::const_iterator i =
        gGlobalHarmonyKindsMap.begin ();
      i != gGlobalHarmonyKindsMap.end ();
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
      else if (count != harmonyKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

std::string existingHarmonyKindsNames (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t harmonyKindsNamesListSize =
    gHarmonyKindsNamesList.size ();

  if (harmonyKindsNamesListSize) {
    size_t nextToLast =
      harmonyKindsNamesListSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::list<std::string>::const_iterator i =
        gHarmonyKindsNamesList.begin ();
      i != gHarmonyKindsNamesList.end ();
      ++i
    ) {
      std::string theString = (*i);

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
      else if (count != harmonyKindsNamesListSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
S_msrHarmonyDegree msrHarmonyDegree::create (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlterationKind        harmonyDegreeAlterationKind,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  msrHarmonyDegree* o =
    new msrHarmonyDegree (
      inputLineNumber,
      harmonyDegreeValue,
      harmonyDegreeAlterationKind,
      harmonyDegreeTypeKind);
  assert (o != nullptr);
  return o;
}

msrHarmonyDegree::msrHarmonyDegree (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlterationKind        harmonyDegreeAlterationKind,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
    : msrElement (inputLineNumber)
{
  fHarmonyDegreeValue          = harmonyDegreeValue;
  fHarmonyDegreeAlterationKind = harmonyDegreeAlterationKind;
  fHarmonyDegreeTypeKind       = harmonyDegreeTypeKind;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmony degree '" <<
      asString () <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif
}

msrHarmonyDegree::~msrHarmonyDegree ()
{}

void msrHarmonyDegree::setHarmonyDegreeUpLinkToHarmony (
  const S_msrHarmony& UpLinkToHarmony)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    UpLinkToHarmony != nullptr,
     "UpLinkToHarmony is null");
#endif

  fHarmonyDegreeUpLinkToHarmony =
    UpLinkToHarmony;
}

/* JMI ???
int msrHarmonyDegree::harmonyDegreeAsSemitones () const
{
/ *
  Kind indicates the type of chord. Degree elements
  can then add, subtract, or alter from these
  starting points.
* /

  int result = -1;

  // determine the value for an unaltered degree value
  switch (fHarmonyDegreeValue) {
    case 0:
      result = 0;
      break;
    case 1:
      result = 0;
      break;
    case 2:
      result = 0;
      break;
    case 3:
      result = 0;
      break;
    case 4:
      result = 0;
      break;
    case 5:
      result = 0;
      break;
    case 6:
      result = 0;
      break;
    case 7:
      result = 0;
      break;
    case 8:
      result = 0;
      break;
    case 9:
      result = 0;
      break;
    case 10:
      result = 0;
      break;
    case 11:
      result = 0;
      break;
    case 12:
      result = 0;
      break;
    case 13:
      result = 0;
      break;
  } // switch

  return result;
}
*/

void msrHarmonyDegree::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyDegree::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyDegree::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmonyDegree::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyDegree::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyDegree::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmonyDegree::browseData (basevisitor* v)
{}

std::string msrHarmonyDegreeTypeKindAsString (
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  std::string result;

  switch (harmonyDegreeTypeKind) {
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAdd:
      result = "kHarmonyDegreeTypeAdd";
      break;
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAlter:
      result = "kHarmonyDegreeTypeAlter";
      break;
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeSubstract:
      result = "kHarmonyDegreeTypeSubstract";
      break;
  } // switch

  return result;
}

std::string msrHarmonyDegree::harmonyDegreeKindAsShortString () const
{
  std::string result;

  switch (fHarmonyDegreeTypeKind) {
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAdd:
      result = "kHarmonyDegreeTypeAdd";
      break;
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAlter:
      result = "kHarmonyDegreeTypeAlter";
      break;
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeSubstract:
      result = "kHarmonyDegreeTypeSubstract";
      break;
  } // switch

  return result;
}

std::string msrHarmonyDegree::asString () const
{
  std::stringstream s;

  s <<
    "[HarmonyDegree" <<
    ", type: " << harmonyDegreeKindAsShortString () <<
    ", value: " << fHarmonyDegreeValue <<
    ", alteration: " <<
    msrAlterationKindAsString (
      fHarmonyDegreeAlterationKind) <<
    ", line: " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrHarmonyDegree::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrHarmonyDegree& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

// harmonies contents
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmonyContents '" <<
      harmonyContentsAsString () <<
      "'" <<
      std::endl;
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

  const std::vector<S_msrHarmonyInterval>&
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

std::string msrHarmonyContents::harmonyContentsAsString () const
{
  std::stringstream s;

  s <<
    "HarmonyContents" <<
    ", " <<
    fHarmonyContentsHarmonyKind <<
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
    std::stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for harmony notes '" <<
      fHarmonyContentsHarmonyKind <<
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
  std::ostream&              os,
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
    rootSemiTonesPitchKind <<
    ")" <<
    */
    "' in language '" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      gGlobalLpsrOahGroup->
        getLpsrQuarterTonesPitchesLanguageKind ()) <<
    "' 'are:" <<
    std::endl << std::endl;

  ++gIndenter;

  for (auto e : EnumTrueHarmonies<msrHarmonyKind> ()) {
    os <<
      msrHarmonyKindAsString (e) <<
      ":" <<
      std::endl;

    ++gIndenter;

    // create the harmony intervals
    S_msrHarmonyStructure
      harmonyStructure =
        msrHarmonyStructure::create (
          e);

    // fetch the intervals items for these intervals
    // with rootSemiTonesPitchKind as root
    const std::vector <S_msrHarmonyInterval>&
      harmonyStructureIntervals =
        harmonyStructure->
          getHarmonyStructureIntervals ();

    if (harmonyStructureIntervals.size ()) {
      // fetch the notes for these intervals
      std::vector<S_msrHarmonyInterval>::const_reverse_iterator
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
              K_MF_INPUT_LINE_UNKNOWN,
              intervalKind,
              rootSemiTonesPitchKind);

        // fetch the quartertones pitch kind
        msrQuarterTonesPitchKind
          noteQuarterTonesPitchKind =
            quarterTonesPitchKindFromSemiTonesPitchKind (
              noteSemiTonesPitchKind);

        // print it
        const int fieldWidth2 = 8;

        os << std::left <<
          std::setw (fieldWidth2) <<
          msrQuarterTonesPitchKindAsStringInLanguage (
            noteQuarterTonesPitchKind,
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
          ": " <<
          msrIntervalKindAsString (intervalKind) <<
          std::endl;

        if (++i == iEnd) break;

        // no std::endl here
      } // for
    }

    os << std::endl;

    --gIndenter;
  } // for

  --gIndenter;
}

/* JMI
void msrHarmonyContents::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyContents::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrHarmonyContents>*
    p =
      dynamic_cast<visitor<S_msrHarmonyContents>*> (v)) {
        S_msrHarmonyContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyContents::visitStart ()" <<
             std::endl;
        p->visitStart (elem);
  }
}

void msrHarmonyContents::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyContents::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrHarmonyContents>*
    p =
      dynamic_cast<visitor<S_msrHarmonyContents>*> (v)) {
        S_msrHarmonyContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyContents::visitEnd ()" <<
            std::endl;
        p->visitEnd (elem);
  }
}

void msrHarmonyContents::browseData (basevisitor* v)
{}
*/

void msrHarmonyContents::print (std::ostream& os) const
{
  os <<
    "HarmonyContents" <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    std::endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fHarmonyContentsRootNote" << ": " <<
    fHarmonyContentsRootNote <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fHarmonyContentsHarmonyKind" << ": " <<
    fHarmonyContentsHarmonyKind <<
    std::endl;

  if (fHarmonyElementsVector.size ()) {
    os <<
    mfSingularOrPlural (
      fHarmonyElementsVector.size (), "harmony element", "harmony elements") <<
    ":" <<
    std::endl;

    ++gIndenter;

    for (size_t i = 0; i < fHarmonyElementsVector.size (); ++i) {
      S_msrSemiTonesPitchAndOctave
        harmonyElement =
          fHarmonyElementsVector [i];

      os <<
        harmonyElement <<
        std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "no notes" <<
      std::endl;
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrHarmonyContents& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrHarmony msrHarmony::create (
  int                      inputLineNumber,
  const S_msrMeasure&      upLinkToMeasure,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  const std::string&       harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  const Rational&          harmonySoundingWholeNotes,
  const Rational&          harmonyDisplayWholeNotes,
  int                      harmoniesStaffNumber,
  const msrTupletFactor&   harmonyTupletFactor,
  const Rational&          harmonyWholeNotesOffset)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      upLinkToMeasure,
      harmonyRootQuarterTonesPitchKind,
      harmonyKind,
      harmonyKindText,
      harmonyInversion,
      harmonyBassQuarterTonesPitchKind,
      harmonySoundingWholeNotes,
      harmonyDisplayWholeNotes,
      harmoniesStaffNumber,
      harmonyTupletFactor,
      harmonyWholeNotesOffset);
  assert (o != nullptr);
  return o;
}

msrHarmony::msrHarmony (
  int                      inputLineNumber,
  const S_msrMeasure&      upLinkToMeasure,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  const std::string&       harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  const Rational&          harmonySoundingWholeNotes,
  const Rational&          harmonyDisplayWholeNotes,
  int                      harmoniesStaffNumber,
  const msrTupletFactor&   harmonyTupletFactor,
  const Rational&          harmonyWholeNotesOffset)
    : msrMeasureElement (
        inputLineNumber),
      fHarmonyTupletFactor (
        harmonyTupletFactor)
{
  fHarmonyRootQuarterTonesPitchKind =
    harmonyRootQuarterTonesPitchKind;

  fHarmonyKind     = harmonyKind;
  fHarmonyKindText = harmonyKindText;

  fHarmonyInversion = harmonyInversion;

  fHarmonyBassQuarterTonesPitchKind =
    harmonyBassQuarterTonesPitchKind;

  doSetMeasureElementSoundingWholeNotes (
    harmonySoundingWholeNotes,
    "msrHarmony::msrHarmony()");

  fHarmonyDisplayWholeNotes =
    harmonyDisplayWholeNotes;

  // a harmony is considered to be at the beginning of the measure
  // until this is computed in msrMeasure::finalizeHarmonyInHarmoniesMeasure()
  fMeasureElementMeasurePosition = Rational (0, 1);

  fHarmoniesStaffNumber = harmoniesStaffNumber;

  fHarmonyWholeNotesOffset = harmonyWholeNotesOffset;

  // handle harmony inversion if any
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */
  if (fHarmonyInversion > 0) {
    // fetch the harmony intervals
    S_msrHarmonyStructure
      harmonyStructure =
        msrHarmonyStructure::create (
  // JMI        inputLineNumber,
          fHarmonyKind);

    // fetch the bass harmony item for the inversion
    S_msrHarmonyInterval
      bassHarmonyInterval =
        harmonyStructure->
          bassHarmonyIntervalForHarmonyInversion (
            inputLineNumber,
            fHarmonyInversion);

    // fetch the inverted harmony bass semitones pitch
    msrQuarterTonesPitchKind
      invertedHarmonyBassQuarterTonesPitchKind =
        noteAtIntervalFromQuarterTonesPitch (
          inputLineNumber,
          bassHarmonyInterval->getHarmonyIntervalIntervalKind (),
          fHarmonyRootQuarterTonesPitchKind);

    // is this compatible with bass quartertones pitch if specified?
    if (fHarmonyBassQuarterTonesPitchKind != msrQuarterTonesPitchKind::kQTP_UNKNOWN) {
      if (
        invertedHarmonyBassQuarterTonesPitchKind
          !=
        fHarmonyBassQuarterTonesPitchKind
        ) {
        std::stringstream s;

        s <<
          "inversion '" <<
          fHarmonyInversion <<
          "' is not compatible with bass quaternotes pitch '" <<
          msrQuarterTonesPitchKindAsStringInLanguage (
            fHarmonyBassQuarterTonesPitchKind,
            gGlobalMsrOahGroup->
              getMsrQuarterTonesPitchesLanguageKind ()) <<
          "'";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }

    // set the bass quartertones pitch according to the inversion
    fHarmonyBassQuarterTonesPitchKind =
      invertedHarmonyBassQuarterTonesPitchKind;
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmony " <<
      asString () <<
      std::endl;
  }
#endif
}

msrHarmony::~msrHarmony ()
{}

S_msrHarmony msrHarmony::createHarmonyNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating a newborn clone of harmony " <<
      asShortString () <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
#endif

  S_msrHarmony
    newbornClone =
      msrHarmony::create (
        fInputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        fHarmonyRootQuarterTonesPitchKind,
        fHarmonyKind,
        fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitchKind,
        fMeasureElementSoundingWholeNotes,
        fHarmonyDisplayWholeNotes,
        fHarmoniesStaffNumber,
        fHarmonyTupletFactor,
        fHarmonyWholeNotesOffset);

  newbornClone->setHarmoniesUpLinkToVoice (
    containingVoice);

  // frame JMI ???
  newbornClone->fHarmonyFrame =
    fHarmonyFrame;

  return newbornClone;
}

S_msrHarmony msrHarmony::createHarmonyDeepClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating a deep clone of harmony " <<
      asShortString () <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
#endif

  S_msrHarmony
    deepClone =
      msrHarmony::create (
        fInputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        fHarmonyRootQuarterTonesPitchKind,
        fHarmonyKind, fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitchKind,
// JMI        fHarmonySoundingWholeNotes,
        fMeasureElementSoundingWholeNotes,
        fHarmonyDisplayWholeNotes,
        fHarmoniesStaffNumber,
        fHarmonyTupletFactor,
        fHarmonyWholeNotesOffset);

  deepClone->setHarmoniesUpLinkToVoice (
    containingVoice);

  // JMI popoulate! v0.9.66

  return deepClone;
}

void msrHarmony::setHarmonyUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of harmony " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fHarmonyUpLinkToMeasure = measure;
}

void msrHarmony::setHarmonyTupletFactor (
  const msrTupletFactor& tupletFactor)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Setting the tuplet factor of harmony " <<
      asShortString () <<
      " to " <<
      tupletFactor.asString () <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fHarmonyTupletFactor = tupletFactor;
}

void msrHarmony::setHarmonyUpLinkToNote (
  const S_msrNote& note)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "==> Setting the uplink to note of harmony " <<
      asString () <<
      " to note " << note->asString () <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    note != nullptr,
    "note is null");
#endif

  fHarmonyUpLinkToNote = note;
}

// void msrHarmony::setMeasureElementMeasurePosition (
//   const S_msrMeasure measure,
//   const Rational&    measurePosition,
//   const std::string&      context)
// {
//   // set the harmony measure position, taking it's offset into account
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measure != nullptr,
//      "setMeasureElementMeasurePosition(): measure is null");
// #endif
//
//   // the offset can be negative, so we merely add it to measurePosition
//   // to obtain the harmony's actual measurePosition
//   // this overwrites it with the same value if fHarmonyWholeNotesOffset is null JMI ???
//   Rational
//     actualMeasurePosition =
//       measurePosition
//         +
//       fHarmonyWholeNotesOffset;
//
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//     gLogStream <<
//       "Setting harmony's measure position of " << asString () <<
//       " to " <<
//       measurePosition <<
//       " (was " <<
//       fMeasureElementMeasurePosition <<
//       ") in measure " <<
//       measure->asShortString () <<
//       " (measureElementMeasureNumber: " <<
//       fHarmonyUpLinkToMeasure->getMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"" <<
//       "', harmonyWholeNotesOffset = " <<
//       fHarmonyWholeNotesOffset <<
//       std::endl;
//   }
// #endif
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fHarmonyUpLinkToNote != nullptr,
//     "fHarmonyUpLinkToNote is null");
// #endif
//
//   // compute harmony's voice position
//   Rational
//     voicePosition =
//       measure->
//         getMeasureVoicePosition ()
//         +
//       actualMeasurePosition;
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measurePosition != msrMoment::K_MEASURE_POSITION_UNKNOWN,
//     "measurePosition == msrMoment::K_MEASURE_POSITION_UNKNOWN");
// #endif
//
//   // set harmony's measure position
//   fMeasureElementMeasurePosition = measurePosition;
//
//   // update current voice position
//   S_msrVoice
//     voice =
//       measure->
//         fetchMeasureUpLinkToVoice ();
//
//   voice->
//     incrementCurrentVoicePosition (
//       fHarmonyUpLinkToNote->
//         getMeasureElementSoundingWholeNotes ());
// }

void msrHarmony::setHarmonyFrame (const S_msrFrame& frame)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFrames ()) {
    gLogStream <<
      "Setting harmony " << asShortString ()  <<
      " frame to " << frame->asString () <<
      std::endl;
  }
#endif

  fHarmonyFrame = frame;
}

void msrHarmony::incrementHarmonySoundingWholeNotesDuration (
  int             inputLineNumber,
  const Rational& wholeNotesDelta)
{
  // compute currentHarmony's future sounding whole notes
  Rational
    augmentedSoundingWholeNotes =
      fMeasureElementSoundingWholeNotes
        +
      wholeNotesDelta;

  // extend currentHarmony's sounding whole notes
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Extending the sounding whole notes of harmony " <<
      asString () <<
      " from " <<
      fMeasureElementSoundingWholeNotes <<
      " to " <<
      augmentedSoundingWholeNotes <<
      " to fill measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fHarmoniesUpLinkToVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  setMeasureElementSoundingWholeNotes (
    augmentedSoundingWholeNotes,
    "incrementHarmonySoundingWholeNotesDuration()");
}

void msrHarmony::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmony::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmony::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmony::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmony::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmony::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmony::browseData (basevisitor* v)
{
  // browse harmony degrees if any
  if (fHarmonyDegreesList.size ()) {
    for (
      std::list<S_msrHarmonyDegree>::const_iterator i = fHarmonyDegreesList.begin ();
      i != fHarmonyDegreesList.end ();
      ++i
    ) {
      // browse the harmony degree
      msrBrowser<msrHarmonyDegree> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fHarmonyFrame) {
    // browse the frame
    msrBrowser<msrFrame> browser (v);
    browser.browse (*fHarmonyFrame);
  }
}

std::string msrHarmony::asString () const
{
  std::stringstream s;

  s <<
    "[Harmony" <<
    ", " << std::hex << std::showbase << this << std::dec <<
    ", fMeasureElementMeasurePosition: " <<
    fMeasureElementMeasurePosition <<
    ", fHarmonyRootQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyRootQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    ", fHarmonyKind: " <<
    fHarmonyKind;

  // print the harmony uplink to note
  s <<
    ", fHarmonyUpLinkToNote: ";
  if (fHarmonyUpLinkToNote) {
    s <<
      fHarmonyUpLinkToNote->asString ();
  }
  else {
    s << "[NONE]";
  }

  s <<
    ", fHarmonyUpLinkToMeasure: ";
    if (fHarmonyUpLinkToMeasure) {
      s <<
        fHarmonyUpLinkToMeasure->asString ();
    }
    else {
      s << "[NONE]";
    }

//   s <<
//     ", fHarmonyUpLinkToVoice: ";
//     if (fHarmonyUpLinkToVoice) {
//       s <<
//         fHarmonyUpLinkToVoice->asString ();
//     }
//     else {
//       s << "[NONE]";
//     }

  s <<
  // print the harmony bass voice position
//   os <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << ": " <<
//     fMeasureElementVoicePosition <<
//     std::endl;

    ", fMeasureElementSoundingWholeNotes: " <<
    fMeasureElementSoundingWholeNotes <<
    ", fHarmonyDisplayWholeNotes: " <<
    fHarmonyDisplayWholeNotes <<

    ", fHarmonyWholeNotesOffset: " <<
    fHarmonyWholeNotesOffset <<
    ", fMeasureElementMeasurePosition: " <<
    fMeasureElementMeasurePosition <<

    ", fHarmonyKindText: \"" <<
    fHarmonyKindText << "\"";

  s << ", fHarmonyInversion: ";
  if (fHarmonyInversion == K_HARMONY_INVERSION_NONE) {
    s << "[NONE]";
  }
  else {
    s << fHarmonyInversion;
  }

  if (fHarmonyBassQuarterTonesPitchKind != msrQuarterTonesPitchKind::kQTP_UNKNOWN) {
    s <<
      ", fHarmonyBassQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyBassQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ());
  }

  if (fHarmonyDegreesList.size ()) {
    s <<
      ", fHarmonyDegreesList: [";

    std::list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin (),
      iEnd   = fHarmonyDegreesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  // print the harmonies staff number
  s <<
    ", fHarmoniesStaffNumber: ";
  if (fHarmoniesStaffNumber == msrStaff::K_STAFF_NUMBER_UNKNOWN)
    s << "[NONE]";
  else
    s << fHarmoniesStaffNumber;

  // print the harmony tuplet factor
  s <<
    ", fHarmonyTupletFactor: " <<
    fHarmonyTupletFactor.asString ();

  // print the harmony frame
  s << ", fHarmonyFrame: ";
  if (fHarmonyFrame) {
    s << fHarmonyFrame;
  }
  else {
    s << "[NONE]";
  }

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrHarmony::print (std::ostream& os) const
{
  os <<
    "[Harmony" <<
     ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 44;

  // print the harmony measure position
  os <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << ": " <<
    fMeasureElementMeasurePosition <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureElementSoundingWholeNotes" << ": " <<
    fMeasureElementSoundingWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fHarmonyDisplayWholeNotes" << ": " <<
    fHarmonyDisplayWholeNotes <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fHarmonyRootQuarterTonesPitchKind" << ": " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyRootQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fHarmonyKind" << ": " <<
    fHarmonyKind <<
    std::endl;

  // print the harmony whole notes offset
  os <<
    std::setw (fieldWidth) <<
    "fHarmonyWholeNotesOffset" << ": " << fHarmonyWholeNotesOffset <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fHarmonyKindText" << ": \"" <<
    fHarmonyKindText <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fHarmonyBassQuarterTonesPitchKind" << ": " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyBassQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fHarmonyInversion" << ": ";
  if (fHarmonyInversion == K_HARMONY_INVERSION_NONE) {
    os << "[NONE]";
  }
  else {
    os << fHarmonyInversion;
  }
  os << std::endl;

  // print harmony degrees if any
  os <<
    std::setw (fieldWidth) <<
    "fHarmonyDegreesList";

  if (fHarmonyDegreesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin (),
      iEnd   = fHarmonyDegreesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        (*i)->asString ();
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    os << std::endl;

    --gIndenter;
  }
  else {
    os <<
      ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the harmonies staff number
  os <<
    std::setw (fieldWidth) <<
    "fHarmoniesStaffNumber" << ": ";
  if (fHarmoniesStaffNumber == msrStaff::K_STAFF_NUMBER_UNKNOWN) {
    os << "[NONE]";
  }
  else {
    os << fHarmoniesStaffNumber;
  }
  os << std::endl;

  // print the harmony tuplet factor
  os <<
    std::setw (fieldWidth) <<
    "fHarmonyTupletFactor" << ": " << fHarmonyTupletFactor.asString () <<
    std::endl;

  // print the harmony frame
  os <<
    std::setw (fieldWidth) <<
    "fHarmonyFrame" << ": ";
  if (fHarmonyFrame) {
    os << fHarmonyFrame;
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // print the harmony uplink to note
  os <<
    std::setw (fieldWidth) <<
    "fHarmonyUpLinkToNote" << ": ";
  if (fHarmonyUpLinkToNote) {
    os <<
      std::endl <<
      gTab << fHarmonyUpLinkToNote->asString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // print the harmony uplink to measure
  os <<
    std::setw (fieldWidth) <<
    "fHarmonyUpLinkToMeasure" << ": ";
  if (fHarmonyUpLinkToMeasure) {
    os <<
      fHarmonyUpLinkToMeasure->asString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // print the harmony uplink to voice
//   os <<
//     std::setw (fieldWidth) <<
//     "fHarmonyUpLinkToVoice" << ": ";
//   if (fHarmonyUpLinkToVoice) {
//     os <<
//       fHarmonyUpLinkToVoice->asString ();
//   }
//   else {
//     os << "[NONE]";
//   }
//   os << std::endl;

  // print the harmony measure number
  os <<
    std::setw (fieldWidth) <<
    "measureElementMeasureNumber" << ": ";
  if (fHarmonyUpLinkToMeasure) {
    os <<
      fHarmonyUpLinkToMeasure->getMeasureNumber ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  // print the harmony bass voice position
//   os <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << ": " <<
//     fMeasureElementVoicePosition <<
//     std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrHarmony& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
void printHarmonyDetails (
  std::ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind)
{
  // fetch the quartertones pitch kind
  msrQuarterTonesPitchKind
    rootQuarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        rootSemiTonesPitchKind);

  std::string
    rootQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        rootQuarterTonesPitchKind,
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  std::string
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
    std::endl << std::endl;

  ++gIndenter;

  // create the harmony intervals
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        harmonyKind);

  // fetch the intervals items for these intervals
  // with rootSemiTonesPitchKind as root
  const std::vector <S_msrHarmonyInterval>&
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

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        os <<
          "==> inversion: " << inversion <<
          ", initial invertedHarmonyStructure:" <<
          std::endl;

        ++gIndenter;
        os <<
          invertedHarmonyStructure <<
          std::endl;
        --gIndenter;
      }
#endif

      // get the inverted harmony structure intervals
      const std::vector <S_msrHarmonyInterval>&
        invertedHarmonyStructureIntervals =
          invertedHarmonyStructure->
            getHarmonyStructureIntervals ();

      // fetch the notes for these intervals
      /* JMI
      std::vector<S_msrHarmonyInterval>::const_iterator
        iBegin = invertedHarmonyStructureIntervals.begin (),
        iEnd   = invertedHarmonyStructureIntervals.end (),
        i      = iBegin;
        */
      std::vector<S_msrHarmonyInterval>::const_reverse_iterator
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
        std::endl;

      ++gIndenter;

      for ( ; ; ) {
        S_msrHarmonyInterval
          harmonyInterval = (*i);

        msrIntervalKind
          intervalKind =
            harmonyInterval->
              getHarmonyIntervalIntervalKind ();

        const int fieldWidth1 = 17;

        os << std::left <<
          std::setw (fieldWidth1) <<
          msrIntervalKindAsString (intervalKind) <<
          ": ";

        // fetch the semitones pitch kind
        msrSemiTonesPitchKind
          noteSemiTonesPitchKind =
            noteAtIntervalFromSemiTonesPitch (
              K_MF_INPUT_LINE_UNKNOWN,
              intervalKind,
              rootSemiTonesPitchKind);

        // fetch the quartertones pitch kind
        msrQuarterTonesPitchKind
          noteQuarterTonesPitchKind =
            quarterTonesPitchKindFromSemiTonesPitchKind (
              noteSemiTonesPitchKind);

        // print it
        const int fieldWidth2 = 8;

        os << std::left <<
          std::setw (fieldWidth2) <<
          msrQuarterTonesPitchKindAsStringInLanguage (
            noteQuarterTonesPitchKind,
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
            /* JMI
          ", octave " << relativeOctave <<
          " (" <<
          noteSemiTonesPitchKind <<
          ")" <<
          */
          std::endl;

        if (++i == iEnd) break;

        // no std::endl here
      } // for

      --gIndenter;

      os << std::endl;
    } // for
  }

  --gIndenter;
}

//______________________________________________________________________________
void printHarmonyAnalysis (
  std::ostream&              os,
  msrSemiTonesPitchKind rootSemiTonesPitchKind,
  msrHarmonyKind        harmonyKind,
  int                   inversion)
{
  // fetch the quartertones pitch kind
  msrQuarterTonesPitchKind
    rootQuarterTonesPitchKind =
      quarterTonesPitchKindFromSemiTonesPitchKind (
        rootSemiTonesPitchKind);

  std::string
    rootQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        rootQuarterTonesPitchKind,
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  std::string
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
    std::endl << std::endl;

  ++gIndenter;

  // create the harmony intervals
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        harmonyKind);

  // fetch the intervals items for these intervals
  // with rootSemiTonesPitchKind as root
  const std::vector <S_msrHarmonyInterval>&
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

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        os <<
          "==> inversion: " << inversion <<
          ", initial invertedHarmonyStructure:" <<
          std::endl;

        ++gIndenter;
        os <<
          invertedHarmonyStructure <<
          std::endl;
        --gIndenter;
      }
#endif

      // get the inverted harmony structure intervals
      const std::vector <S_msrHarmonyInterval>&
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
          std::endl;

        ++gIndenter;

        std::vector<S_msrHarmonyInterval>::const_reverse_iterator
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
                K_MF_INPUT_LINE_UNKNOWN,
                intervalKind,
                rootSemiTonesPitchKind);

          // fetch the quartertones pitch kind
          msrQuarterTonesPitchKind
            noteQuarterTonesPitchKind =
              quarterTonesPitchKindFromSemiTonesPitchKind (
                noteSemiTonesPitchKind);

          // print it
          const int fieldWidth2 = 5;

          os << std::left <<
            std::setw (fieldWidth2) <<
            msrQuarterTonesPitchKindAsStringInLanguage (
              noteQuarterTonesPitchKind,
              gGlobalLpsrOahGroup->
                getLpsrQuarterTonesPitchesLanguageKind ()) <<
            ": " <<
            msrIntervalKindAsString (intervalKind) <<
            std::endl;

          if (++i == iEnd) break;

          // no std::endl here
        } // for

        --gIndenter;

        os << std::endl;
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
          std::endl;

        ++gIndenter;

        int tritonsCounter = 0;

        std::vector<S_msrHarmonyInterval>::const_iterator
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
                K_MF_INPUT_LINE_UNKNOWN,
                intervalKind1,
                rootSemiTonesPitchKind);

          // fetch the quartertones pitch kind
          msrQuarterTonesPitchKind
            noteQuarterTonesPitchKind1 =
              quarterTonesPitchKindFromSemiTonesPitchKind (
                noteSemiTonesPitchKind1);

          // print the invervals
          std::vector<S_msrHarmonyInterval>::const_iterator
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
                  K_MF_INPUT_LINE_UNKNOWN,
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

            os << std::left <<
              std::setw (fieldWidth1) <<
              msrQuarterTonesPitchKindAsStringInLanguage (
                noteQuarterTonesPitchKind1,
                gGlobalLpsrOahGroup->
                  getLpsrQuarterTonesPitchesLanguageKind ()) <<

              " -> " <<

              std::setw (fieldWidth1) <<
              msrQuarterTonesPitchKindAsStringInLanguage (
                noteQuarterTonesPitchKind2,
                gGlobalLpsrOahGroup->
                  getLpsrQuarterTonesPitchesLanguageKind ()) <<

              ": " <<

              std::setw (fieldWidth2) << // JMI
              msrIntervalKindAsString (innerIntervalKind) <<

              '(' <<
              std::setw (fieldWidth2) <<
              msrIntervalKindAsString (intervalKind1) <<
              " -> " <<
              msrIntervalKindAsString (intervalKind2) <<
              ")" <<

              std::endl;

            --gIndenter;

            if (++i2 == iEnd2) break;
          } // for

          if (++i1 == iEnd1) break;

          os << std::endl;
        } // for

        --gIndenter;


        if (tritonsCounter > 0) {
          os <<
            "This harmony contains " <<
            mfSingularOrPlural (
              tritonsCounter, "triton", "tritons") <<
            std::endl;
        }

        os << std::endl;
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
        std::endl << std::endl;
    }
  }

  --gIndenter;
}

// harmonies structure
//______________________________________________________________________________
std::map<msrHarmonyKind, S_msrHarmonyStructure>
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> Creating harmony intervals '" <<
      harmonyStructureAsString () <<
      "'" <<
      std::endl;
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
    case msrHarmonyKind::kHarmony_UNKNOWN:
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
  const S_msrHarmonyInterval& harmonyInterval)
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
      std::endl;
  }

  if (visitor<S_msrHarmonyStructure>*
    p =
      dynamic_cast<visitor<S_msrHarmonyStructure>*> (v)) {
        S_msrHarmonyStructure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyStructure::visitStart ()" <<
             std::endl;
        p->visitStart (elem);
  }
}

void msrHarmonyStructure::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyStructure::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrHarmonyStructure>*
    p =
      dynamic_cast<visitor<S_msrHarmonyStructure>*> (v)) {
        S_msrHarmonyStructure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyStructure::visitEnd ()" <<
            std::endl;
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
    std::stringstream s;

    s <<
      "Sorry, inversion number '" <<
      inversionNumber <<
      "' does not exist for harmony intervals '" <<
      fHarmonyStructureHarmonyKind <<
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "==> invertHarmonyStructure (), inversion = " <<
      inversion <<
      ", original harmonyStructureIntervalsSize = " <<
      harmonyStructureIntervalsSize <<
      std::endl;
  }
#endif

  if (harmonyStructureIntervalsSize) {
    // add the first items
    for (size_t i = inversion; i < harmonyStructureIntervalsSize; ++i) {
      S_msrHarmonyInterval
        harmonyIntervalClone =
          fHarmonyStructureIntervals [i]->
            createHarmonyIntervalNewbornClone ();

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "--> adding first item to result:" <<
          std::endl;
        ++gIndenter;
        gLogStream <<
          harmonyIntervalClone <<
          std::endl;
        --gIndenter;
      }
#endif

      result->
        appendHarmonyIntervalToHarmonyStructure (
          harmonyIntervalClone);

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "==> result harmony structure after adding first item :" <<
          std::endl;

        ++gIndenter;
        gLogStream <<
          result <<
          std::endl;
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

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "--> adding last item to resultlast item :" <<
          std::endl;
        ++gIndenter;
        gLogStream <<
          harmonyIntervalClone <<
          std::endl;
        --gIndenter;
      }
#endif

      result->
        appendHarmonyIntervalToHarmonyStructure (
          harmonyIntervalClone);

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        gLogStream <<
          "==> result harmony structure after  after adding last item:" <<
          std::endl;

        ++gIndenter;
        gLogStream <<
          result <<
          std::endl;
        --gIndenter;
      }
#endif
    } // for
  }

  return result;
}

std::list<msrSemiTonesPitchKind> buildSemiTonesChord (
  msrHarmonyKind        harmonyKind,
  msrSemiTonesPitchKind rootNote)
{
  std::list<msrSemiTonesPitchKind> result;

  // create the harmony intervals
  S_msrHarmonyStructure
    harmonyStructure =
      msrHarmonyStructure::create (
        harmonyKind);

  // add the root to the harmony
  result.push_back (rootNote);

  // add the other notes to the harmony
  const std::vector<S_msrHarmonyInterval>&
    harmonyStructureIntervals =
      harmonyStructure->
        getHarmonyStructureIntervals ();

  for (size_t i = 1; i << harmonyStructureIntervals.size (); ++i) {
    result.push_back (rootNote);
  } // for

  return result;
}

std::string msrHarmonyStructure::harmonyStructureAsString () const
{
  std::stringstream s;

  s <<
    "HarmonyStructure" <<
    ", " <<
    fHarmonyStructureHarmonyKind <<
    ", " <<
    mfSingularOrPlural (
      fHarmonyStructureIntervals.size (), "item", "items");

  return s.str ();
}

void msrHarmonyStructure::print (std::ostream& os) const
{
  os <<
    "HarmonyStructure" <<
    ", fHarmonyContentsHarmonyKind: " <<
    fHarmonyStructureHarmonyKind <<
    ", " <<
    mfSingularOrPlural (
      fHarmonyStructureIntervals.size (), "interval", "intervals") <<
  /* JMI
    ", line: " << fInputLineNumber <<
    */
    std::endl;

  ++gIndenter;

  if (fHarmonyStructureIntervals.size ()) {
    std::vector<S_msrHarmonyInterval>::const_reverse_iterator
      iBegin = fHarmonyStructureIntervals.crbegin (),
      iEnd   = fHarmonyStructureIntervals.crend (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrHarmonyInterval
        harmonyInterval = (*i);

      gLogStream <<
        harmonyInterval->harmonyIntervalAsShortString () <<
        std::endl;

      if (++i == iEnd) break;
    } // for
  }
  else {
    gLogStream <<
      "no intervals" <<
      std::endl;
  }

  --gIndenter;
}

void msrHarmonyStructure::printAllHarmoniesStructures (std::ostream& os)
{
  os <<
    "All the known harmonies structures are:" <<
    std::endl << std::endl;

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
      std::endl;
  } // for

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrHarmonyStructure& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
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
    std::endl;

  ++gIndenter;

  for (auto e : mfEnumAll<msrHarmonyKind> ()) {
    gLogStream <<
      msrHarmonyKindAsString (e) << ":" <<
      std::endl;

    ++gIndenter;

    S_msrHarmonyStructure
      harmonyStructure =
        gGlobalHarmonyStructuresMap [e];

    if (harmonyStructure) {
      gLogStream <<
        harmonyStructure <<
        std::endl;
    }
    else {
      gLogStream <<
        "no intervals" <<
        std::endl;
    }

    --gIndenter;

    gLogStream << std::endl;
  } // for

  --gIndenter;

  gLogStream << std::endl;
}


}
