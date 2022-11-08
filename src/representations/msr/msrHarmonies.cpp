/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrPitchesNames.h"

#include "msrMeasures.h"

#include "msrHarmonies.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

// harmonies
//______________________________________________________________________________
// constant
const int K_HARMONY_NO_INVERSION = -1;

string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind)
{
  string result;

  switch (harmonyKind) {
    case msrHarmonyKind::kHarmony_NO_:
      result = "***kHarmony_NO_***";
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
    case msrHarmonyKind::kHarmony_NO_:
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
    case msrHarmonyKind::kHarmony_NO_:
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
  msrHarmonyKind result = msrHarmonyKind::kHarmony_NO_;

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmony degree '" <<
      asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif
}

msrHarmonyDegree::~msrHarmonyDegree ()
{}

void msrHarmonyDegree::setHarmonyDegreeUpLinkToHarmony (
  S_msrHarmony UpLinkToHarmony)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    UpLinkToHarmony != nullptr,
     "UpLinkToHarmony is null");

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
      endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyDegree::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmonyDegree::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyDegree::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyDegree::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmonyDegree::browseData (basevisitor* v)
{}

string msrHarmonyDegreeTypeKindAsString (
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  string result;

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

string msrHarmonyDegree::harmonyDegreeKindAsShortString () const
{
  string result;

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

string msrHarmonyDegree::asString () const
{
  stringstream s;

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

void msrHarmonyDegree::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrHarmonyDegree& elt)
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
S_msrHarmony msrHarmony::create (
  int                      inputLineNumber,
  S_msrMeasure             upLinkToMeasure,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  const string&            harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  const Rational&          harmonySoundingWholeNotes,
  const Rational&          harmonyDisplayWholeNotes,
  int                      harmoniesStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
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
  S_msrMeasure             upLinkToMeasure,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  const string&            harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  const Rational&          harmonySoundingWholeNotes,
  const Rational&          harmonyDisplayWholeNotes,
  int                      harmoniesStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
  const Rational&          harmonyWholeNotesOffset)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure),
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
    if (fHarmonyBassQuarterTonesPitchKind != msrQuarterTonesPitchKind::kQTP_NO_) {
      if (
        invertedHarmonyBassQuarterTonesPitchKind
          !=
        fHarmonyBassQuarterTonesPitchKind
        ) {
        stringstream s;

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmony " <<
      asString () <<
      endl;
  }
#endif
}

msrHarmony::~msrHarmony ()
{}

S_msrHarmony msrHarmony::createHarmonyNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating a newborn clone of harmony " <<
      asShortString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrHarmony
    newbornClone =
      msrHarmony::create (
        fInputLineNumber,
        nullptr, // will be set when harmony is appended to a measure JMI v0.9.66 PIM
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
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating a deep clone of harmony " <<
      asShortString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrHarmony
    harmonyDeepClone =
      msrHarmony::create (
        fInputLineNumber,
        nullptr, // will be set when harmony is appended to a measure JMI v0.9.66 PIM
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

  harmonyDeepClone->setHarmoniesUpLinkToVoice (
    containingVoice);

  // JMI popoulate! v0.9.66

  return harmonyDeepClone;
}

void msrHarmony::setHarmonyTupletFactor (
  msrTupletFactor tupletFactor)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Setting the tuplet factor of harmony " <<
      asShortString () <<
      " to " <<
      tupletFactor.asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fHarmonyTupletFactor = tupletFactor;
}

void msrHarmony::setHarmonyUpLinkToNote (S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Setting harmony " << asShortString ()  <<
      " note uplink to " << note->asString () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    note != nullptr,
    "note is null");

  fHarmonyUpLinkToNote = note;
}

void msrHarmony::setHarmonyMeasurePosition (
  const S_msrMeasure measure,
  const Rational&    measurePosition,
  const string&      context)
{
  // set the harmony position in measure, taking it's offset into account

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
     "setHarmonyMeasurePosition(): measure is null");

  // the offset can be negative, so we merely add it to measurePosition
  // to obtain the harmony's actual measurePosition
  // this overwrites it with the same value if fHarmonyWholeNotesOffset is null JMI ???
  Rational
    actualMeasurePosition =
      measurePosition
        +
      fHarmonyWholeNotesOffset;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting harmony's position in measure of " << asString () <<
      " to " <<
      measurePosition <<
      " (was " <<
      fMeasureElementMeasurePosition <<
      ") in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      fetchMeasureElementMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"" <<
      "', harmonyWholeNotesOffset = " <<
      fHarmonyWholeNotesOffset <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fHarmonyUpLinkToNote != nullptr,
    "fHarmonyUpLinkToNote is null");

  // compute harmony's position in voice
  Rational
    voicePosition =
      measure->
        getMeasureVoicePosition ()
        +
      actualMeasurePosition;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set harmony's position in measure
  fMeasureElementMeasurePosition = measurePosition;

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureUpLinkToVoice ();

  voice->
    incrementCurrentVoicePosition (
      fHarmonyUpLinkToNote->
        getMeasureElementSoundingWholeNotes ());
}

void msrHarmony::setHarmonyFrame (S_msrFrame frame)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFrames ()) {
    gLogStream <<
      "Setting harmony " << asShortString ()  <<
      " frame to " << frame->asString () <<
      endl;
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
#ifdef TRACING_IS_ENABLED
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
      endl;
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
      endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmony::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmony::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmony::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmony::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmony::browseData (basevisitor* v)
{
  // browse harmony degrees if any
  if (fHarmonyDegreesList.size ()) {
    for (
      list<S_msrHarmonyDegree>::const_iterator i = fHarmonyDegreesList.begin ();
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

string msrHarmony::asString () const
{
  stringstream s;

  s <<
    "[Harmony" <<
    ", fHarmonyRootQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyRootQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    ", fHarmonyKind: " <<
    msrHarmonyKindAsShortString (fHarmonyKind) <<

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
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION) {
    s << "[NONE]";
  }
  else {
    s << fHarmonyInversion;
  }

  if (fHarmonyBassQuarterTonesPitchKind != msrQuarterTonesPitchKind::kQTP_NO_) {
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

    list<S_msrHarmonyDegree>::const_iterator
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
  if (fHarmoniesStaffNumber == msrStaff::K_NO_STAFF_NUMBER)
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

  // print the harmony note uplink
  s << ", fHarmonyUpLinkToNote: ";
  if (fHarmonyUpLinkToNote) {
    s << fHarmonyUpLinkToNote;
  }
  else {
    s << "[NONE]";
  }

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrHarmony::print (ostream& os) const
{
  os <<
    "[Harmony" <<
     ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 44;

  os << left <<
    setw (fieldWidth) <<
    "harmonyRoot" << " : " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyRootQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    endl <<
    setw (fieldWidth) <<
    "fHarmonyKind" << " : " <<
    msrHarmonyKindAsString (fHarmonyKind) <<
    endl <<

    setw (fieldWidth) <<
    "fMeasureElementSoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "fHarmonyDisplayWholeNotes" << " : " <<
    fHarmonyDisplayWholeNotes <<
    endl;

  // print the harmony whole notes offset
  os <<
    setw (fieldWidth) <<
    "fHarmonyWholeNotesOffset" << " : " << fHarmonyWholeNotesOffset <<
    endl;

  os <<
    setw (fieldWidth) <<
    "fHarmonyKindText" << " : \"" <<
    fHarmonyKindText <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fHarmonyBassQuarterTonesPitchKind" << " : " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyBassQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    endl;

  os <<
    setw (fieldWidth) <<
    "fHarmonyInversion" << " : ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION) {
    os << "[NONE]";
  }
  else {
    os << fHarmonyInversion;
  }
  os << endl;

  // print harmony degrees if any
  os <<
    setw (fieldWidth) <<
    "fHarmonyDegreesList";

  if (fHarmonyDegreesList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin (),
      iEnd   = fHarmonyDegreesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        (*i)->asString ();
      if (++i == iEnd) break;
      os << endl;
    } // for

    os << endl;

    --gIndenter;
  }
  else {
    os <<
      " : " <<
      "[NONE]" <<
      endl;
  }

  // print the harmonies staff number
  os <<
    setw (fieldWidth) <<
    "fHarmoniesStaffNumber" << " : ";
  if (fHarmoniesStaffNumber == msrStaff::K_NO_STAFF_NUMBER) {
    os << "[NONE]";
  }
  else {
    os << fHarmoniesStaffNumber;
  }
  os << endl;

  // print the harmony tuplet factor
  os <<
    setw (fieldWidth) <<
    "fHarmonyTupletFactor" << " : " << fHarmonyTupletFactor.asString () <<
    endl;

  // print the harmony frame
  os <<
    setw (fieldWidth) <<
    "fHarmonyFrame" << " : ";
  if (fHarmonyFrame) {
    os << fHarmonyFrame;
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  // print the harmony measure number
  os <<
    setw (fieldWidth) <<
    "measureElementMeasureNumber" << " : " << fetchMeasureElementMeasureNumber () <<
    endl;

  // print the harmony position in measure
  os <<
    setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << " : " << fMeasureElementMeasurePosition <<
    endl;

  // print the harmony bass position in voice
//   os <<
//     setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << " : " << fMeasureElementVoicePosition <<
//     endl;

  // print the harmony note uplink
  os <<
    setw (fieldWidth) <<
    "fHarmonyUpLinkToNote" << " : ";
  if (fHarmonyUpLinkToNote) {
    os <<
      endl <<
      gTab << fHarmonyUpLinkToNote->asString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrHarmony& elt)
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
                K_MF_NO_INPUT_LINE_NUMBER,
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
                K_MF_NO_INPUT_LINE_NUMBER,
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
                  K_MF_NO_INPUT_LINE_NUMBER,
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


}
