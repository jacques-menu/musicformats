/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <ostream>

#include "msrMeasuresEnumTypes.h"


namespace MusicFormats
{

// measures
//______________________________________________________________________________
std::string msrMeasureKindAsString (
  msrMeasureKind measureKind)
{
  std::string result;

  switch (measureKind) {
    case msrMeasureKind::kMeasureKindUnknown:
      result = "kMeasureKindUnknown***";
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

std::ostream& operator << (std::ostream& os, const msrMeasureKind& elt)
{
  os << msrMeasureKindAsString (elt);
  return os;
}

std::string msrMeasureKindasShortStringForMeasuresSlices (
  msrMeasureKind measureKind)
{
  std::string result;

  switch (measureKind) {
    case msrMeasureKind::kMeasureKindUnknown:
      result = "kMeasureKindUnknown***";
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

std::string msrMeasureImplicitKindAsString (
  msrMeasureImplicitKind measureImplicitKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrMeasureImplicitKind& elt)
{
  os << msrMeasureImplicitKindAsString (elt);
  return os;
}


}

/*
//   if (measurePositionsDelta.getNumerator () > 0) {
//     // there is at least one note between
//   }

//   // is a padding skip note needed?
//   if (measurePositionsDelta.getNumerator () > 0) {
//     // create a padding skip note
//     S_msrNote
//       skipNote =
//         createPaddingSkipNoteForVoice (
//           inputLineNumber,
//           measurePositionsDelta,
//           voice);
//
//     // set its position in measure
//     skipNote->
//       setMeasureElementMeasurePosition (
//         this,
//         fCurrentMeasureWholeNotesDuration,
//         "handleSubsequentHarmonyInHarmoniesMeasure() 2");
//
//     // insert skipNote before currentHarmony in the measure's elements list
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
//       gLogStream <<
//         "Inserting subsequent padding note " <<
//         skipNote->asString () <<
//         " before currentHarmony " <<
//         currentHarmony->asString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//     }
// #endif
//
//     // insert skipNote in the measure elements list before (*i)
//     insertElementInMeasureBeforeIterator (
//       inputLineNumber,
//       i,
//       skipNote);
//   }
//
//   else if (measurePositionsDelta.getNumerator () < 0) {
//     // the two harmonies overlap in time
//     std::stringstream s;
//
//     s <<
//       "Previous harmony " <<
//       previousHarmony->asString () <<
//       " overlaps current harmony " <<
//       currentHarmony->asString ();
//
//     msrInternalWarning (
//       gGlobalServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       s.str ());
//
//     // compute previousHarmony's future sounding whole notes
//     Rational
//       reducedSoundingWholeNotes =
//         previousHarmonySoundingWholeNotes
//           + // the delta is negative
//         measurePositionsDelta;
//
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
//       gLogStream <<
//         "Reducing the sounding whole notes of harmony FOO " <<
//         previousHarmony->asString () <<
//         " from " <<
//         previousHarmonySoundingWholeNotes <<
//         " to " <<
//         reducedSoundingWholeNotes <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//     }
// #endif
//
//     if (reducedSoundingWholeNotes.getNumerator () == 0) {
//       std::stringstream s;
//
//       s <<
//         "Cannot reduce the duration of harmony " <<
//         previousHarmony->asShortString () <<
//         " to 0 : leaving it as it is";
//
//       msrInternalWarning (
//         gGlobalServiceRunData->getInputSourceName (),
//         inputLineNumber,
// //  JMI             __FILE__, __LINE__,
//         s.str ());
//     }
//     else {
//       // set previousHarmony's duration to the reduced value
//       previousHarmony->
//         setMeasureElementSoundingWholeNotes (
//           reducedSoundingWholeNotes);
//     }
//   }
*/
