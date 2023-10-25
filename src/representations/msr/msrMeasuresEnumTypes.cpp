/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

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
    case msrMeasureKind::kMeasureKindIncompleteLastMeasure:
      result = "kMeasureKindIncompleteLastMeasure";
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
    case msrMeasureKind::kMeasureKindOverFlowing:
      result = "kMeasureKindOverFlowing";
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

mfIndentedStringStream& operator << (mfIndentedStringStream& iss, const msrMeasureKind elt)
{
  iss.getStringstream () <<
    msrMeasureKindAsString (elt);

  return iss;
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
    case msrMeasureKind::kMeasureKindIncompleteLastMeasure:
      result = "kMeasureKindIncompleteLastMeasure";
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
    case msrMeasureKind::kMeasureKindOverFlowing:
      result = "kMeasureKindOverFlowing";
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

mfIndentedStringStream& operator << (mfIndentedStringStream& iss, const msrMeasureImplicitKind elt)
{
  iss.getStringstream () <<
    msrMeasureImplicitKindAsString (elt);

  return iss;
}

std::string msrMeasureFirstInSegmentKindAsString (
  msrMeasureFirstInSegmentKind measureFirstInSegmentKind)
{
  std::string result;

  switch (measureFirstInSegmentKind) {
    case msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindUnknown:
      result = "kMeasureFirstInSegmentKindUnknown";
      break;
    case msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindYes:
      result = "kMeasureFirstInSegmentKindYes";
      break;
    case msrMeasureFirstInSegmentKind::kMeasureFirstInSegmentKindNo:
      result = "kMeasureFirstInSegmentKindNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrMeasureFirstInSegmentKind& elt)
{
  os << msrMeasureFirstInSegmentKindAsString (elt);
  return os;
}

std::string msrMeasureRepeatContextKindAsString (
  msrMeasureRepeatContextKind measureRepeatContextKind)
{
  std::string result;

  switch (measureRepeatContextKind) {
    case msrMeasureRepeatContextKind::kMeasureRepeatContext_UNKNOWN_:
      result = "kMeasureRepeatContext_UNKNOWN_";
      break;
    case msrMeasureRepeatContextKind::kMeasureRepeatContextNone:
      result = "kMeasureRepeatContextNone";
      break;
    case msrMeasureRepeatContextKind::kMeasureRepeatContextCommonPartLastMeasure:
      result = "kMeasureRepeatContextCommonPartLastMeasure";
      break;
    case msrMeasureRepeatContextKind::kMeasureRepeatContextHookedEndingLastMeasure:
      result = "kMeasureRepeatContextHookedEndingLastMeasure";
      break;
    case msrMeasureRepeatContextKind::kMeasureRepeatContextHooklessEndingLastMeasure:
      result = "kMeasureRepeatContextHooklessEndingLastMeasure";
      break;
    case msrMeasureRepeatContextKind::kMeasureRepeatContextNextMeasureAfterCommonPart:
      result = "kMeasureRepeatContextNextMeasureAfterCommonPart";
      break;
    case msrMeasureRepeatContextKind::kMeasureRepeatContextNextMeasureAfterHookedEnding:
      result = "kMeasureRepeatContextNextMeasureAfterHookedEnding";
      break;
    case msrMeasureRepeatContextKind::kMeasureRepeatContextNextMeasureAfterHooklessEnding:
      result = "kMeasureRepeatContextNextMeasureAfterHooklessEnding";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrMeasureRepeatContextKind& elt)
{
  os << msrMeasureRepeatContextKindAsString (elt);
  return os;
}

std::string msrMeasureEndRegularKindAsString (
  msrMeasureEndRegularKind measureEndRegularKind)
{
  std::string result;

  switch (measureEndRegularKind) {
    case msrMeasureEndRegularKind::kMeasureEndRegularKindUnknown:
      result = "kMeasureEndRegularKindUnknown";
      break;
    case msrMeasureEndRegularKind::kMeasureEndRegularKindYes:
      result = "kMeasureEndRegularKindYes";
      break;
    case msrMeasureEndRegularKind::kMeasureEndRegularKindNo:
      result = "kMeasureEndRegularKindNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrMeasureEndRegularKind& elt)
{
  os << msrMeasureEndRegularKindAsString (elt);
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
//     // set its measure position
//     skipNote->
//       setMeasurePosition (
//         this,
//         fMeasureCurrentAccumulatedWholeNotesDuration,
//         "handleASubsequentHarmonyInAHarmoniesMeasure() 2");
//
//     // insert skipNote before currentHarmony in the measure's elements list
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Inserting subsequent padding note " <<
//         skipNote->asString () <<
//         " before currentHarmony " <<
//         currentHarmony->asString () <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//
//           gWaeHandler->waeTrace (
//             __FILE__, __LINE__,
//             ss.str (),
//     }
// #endif // MF_TRACE_IS_ENABLED
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
//     std::stringstream ss;
//
//     ss <<
//       "Previous harmony " <<
//       previousHarmony->asString () <<
//       " overlaps current harmony " <<
//       currentHarmony->asString ();
//
//     msrInternalWarning (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       ss.str ());
//
//     // compute previousHarmony's future sounding whole notes
//     mfRational
//       reducedSoundingWholeNotes =
//         previousHarmonySoundingWholeNotes
//           + // the delta is negative
//         measurePositionsDelta;
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Reducing the sounding whole notes of harmony " <<
//         previousHarmony->asString () <<
//         " from " <<
//         previousHarmonySoundingWholeNotes <<
//         " to " <<
//         reducedSoundingWholeNotes <<
//         " in voice \"" <<
//         voice->getVoiceName () <<
//         "\", line " << inputLineNumber <<
//         std::endl;
//
//           gWaeHandler->waeTrace (
//             __FILE__, __LINE__,
//             ss.str (),
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     if (reducedSoundingWholeNotes.getNumerator () == 0) {
//       std::stringstream ss;
//
//       ss <<
//         "Cannot reduce the duration of harmony " <<
//         previousHarmony->asShortString () <<
//         " to 0 : leaving it as it is";
//
//       msrInternalWarning (
//         gServiceRunData->getInputSourceName (),
//         inputLineNumber,
// //  JMI             __FILE__, __LINE__,
//         ss.str ());
//     }
//     else {
//       // set previousHarmony's duration to the reduced value
//       previousHarmony->
//         setSoundingWholeNotes (
//           reducedSoundingWholeNotes);
//     }
//   }
*/
