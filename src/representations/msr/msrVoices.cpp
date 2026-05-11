/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "oahOah.h"

#include "waeOah.h"

#include "msrOah.h"

#include "msrVoices.h"

#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrDoubleTremolos.h"
#include "msrLineBreaks.h"
#include "msrMeasureRepeats.h"
#include "msrPageBreaks.h"
#include "msrRehearsalMarks.h"
#include "msrRepeats.h"
#include "msrRepeatConstants.h"
#include "msrStaves.h"
#include "msrTempos.h"
#include "msrTuplets.h"
#include "msrVoiceStaffChanges.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// voice kinds
//______________________________________________________________________________
std::string msrVoiceKindAsString (
  msrVoiceKind voiceKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrVoiceKind& elt)
{
  os << msrVoiceKindAsString (elt);
  return os;
}

std::string msrVoiceKindAsStringForPrint (
  msrVoiceKind voiceKind)
{
  std::string result;

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

//______________________________________________________________________________
std::string msrVoiceRepeatPhaseKindAsString (
  msrVoiceRepeatPhaseKind
    afterRepeatComponentPhaseKind)
{
  std::string result;

  switch (afterRepeatComponentPhaseKind) {
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseNone:
      result = "kVoiceRepeatPhaseNone";
      break;
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart:
      result = "kVoiceRepeatPhaseAfterCommonPart";
      break;
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHookedEnding:
      result = "kVoiceRepeatPhaseAfterHookedEnding";
      break;
    case msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHooklessEnding:
      result = "kVoiceRepeatPhaseAfterHooklessEnding";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrVoiceRepeatPhaseKind& elt)
{
  os << msrVoiceRepeatPhaseKindAsString (elt);
  return os;
}

std::string msrVoiceFinalizationStatusKindAsString (
  msrVoiceFinalizationStatusKind voiceFinalizationStatusKind)
{
  std::string result;

  switch (voiceFinalizationStatusKind) {
    case msrVoiceFinalizationStatusKind::kKeepVoice:
      result = "kKeepVoice";
      break;
    case msrVoiceFinalizationStatusKind::kEraseVoice:
      result = "kEraseVoice";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrVoiceFinalizationStatusKind& elt)
{
  os << msrVoiceFinalizationStatusKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrVoice msrVoice::create (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceKind             voiceKind,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff&        voiceUpLinkToStaff)
{
  msrVoice* obj =
    new msrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceUpLinkToStaff);
  assert (obj != nullptr);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating voice " <<
      obj->asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

S_msrVoice msrVoice::createAsWellAsItsSegment (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceKind             voiceKind,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff&        voiceUpLinkToStaff)
{
  msrVoice* obj =
    new msrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceUpLinkToStaff);
  assert (obj != nullptr);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating voice " <<
      obj->asString () <<
      " as well as its segment";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the voice segment on the fly
  obj->setVoiceSegment (
    msrSegment::create (
      inputLineNumber,
      msrSegmentKind::kSegmentKindAtVoiceLevel,
      obj)); // JMI ??? ZOULOU 0.9.76

  return obj;
}

msrVoice::msrVoice (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceKind             voiceKind,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff&        voiceUpLinkToStaff)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    voiceUpLinkToStaff != nullptr,
    "voiceUpLinkToStaff is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set voice staff upLink
  fVoiceUpLinkToStaff = voiceUpLinkToStaff;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice number
  fVoiceNumber = voiceNumber;

  // set regular voice ordinal number in part
  fRegularVoiceOrdinalNumberInPart = 0;

  // do other initializations
  initializeVoice (
    inputLineNumber);
}

msrVoice::msrVoice (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceKind             voiceKind,
  int                      voiceNumber)
    : msrElement (inputLineNumber)
{
  // set voice staff upLink
  fVoiceUpLinkToStaff = nullptr;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice number
  fVoiceNumber = voiceNumber;

  // do other initializations
  initializeVoice (
    inputLineNumber);
}

msrVoice::~msrVoice ()
{}

void msrVoice::initializeVoice (
  const mfInputLineNumber& inputLineNumber)
{
  // the voice staff sequential number will be set
  // when regular voices are added to a staff
  // in setRegularVoiceStaffSequentialNumber ()
  fRegularVoiceStaffSequentialNumber = -1;

  ++gIndenter;

  // compute voice number
  mfVoiceNumber
    voiceNumber =
      gMsrOahGroup->getCreateVoicesStaffRelativeNumbers ()
        ? fRegularVoiceStaffSequentialNumber
        : fVoiceNumber;

  // set voice name
  setVoiceNamesFromNumber (
    fInputLineNumber,
    voiceNumber);

  // check voice number
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // the voice number should be positive
//       if (fVoiceNumber < 1 || fVoiceNumber > 4) { // JMI 0.9.76
//         std::stringstream ss;
//
//         ss <<
//           "regular voice number " << fVoiceNumber <<
//           " is not in the 1..4 range";
//
//         msrError (
//           gServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
    /* JMI
      if (fVoiceNumber != K_MF_PART_FIGURED_BASS_VOICE_NUMBER) {
        std::stringstream ss;

        ss <<
          "figured bass voice number " << fVoiceNumber <<
          " is not equal to " << K_MF_PART_FIGURED_BASS_VOICE_NUMBER;

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      */
      break;
  } // switch

  // voice shortest note
  fVoiceShortestNoteWholeNotes =
    mfWholeNotes (INT_MAX, 1);
  fVoiceShortestNoteTupletFactor =
    mfRational (1, 1);

  // set voice current measure number
  fVoiceCurrentMeasureNumber = // JMI "??";
    fetchVoiceUpLinkToPart ()->
      getPartCurrentMeasureNumber ();

  // set voice  current ordinal measure number
  fVoiceCurrentMeasureOrdinalNumber = 0;

  // set voice current measure purist number
  fVoiceCurrentMeasurePuristNumber = 1; // default value,
                                        // supposing there's no anacrusis

  // music has not been inserted in voice yet
  fVoiceIsMusicallyEmpty = true;

  // counters
  fVoiceActualNotesCounter = 0;
  fVoiceRestsCounter = 0;
  fVoiceSkipsCounter = 0;
  fVoiceActualHarmoniesCounter = 0;
  fVoiceActualFiguredBassesCounter = 0;

  // set a default 4/4 time, since there can be none JMI 0.9.68
  this->setVoiceCurrentTimeSignature (
    msrTimeSignature::createFourQuartersTime (
      0)); // inputLineNumber

  // regular measure ends detection
  fWholeNotesSinceLastRegularMeasureEnd = K_WHOLE_NOTES_ZERO;

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    fInputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseNone);

  // multi-measure rests
  fVoiceContainsMultiMeasureRests = false;
  fVoiceRemainingMultiMeasureRests = 0;

  // measures repests
  fVoiceContainsMeasureRepeats = false;

  // voice position
  fCurrentVoicePosition =
    K_WHOLE_NOTES_ZERO;

  // voice moment
  fCurrentVoiceMoment =
    mfMoment (
      K_POSITION_IN_MEASURE_ZERO,
      K_WHOLE_NOTES_ZERO);

  // voice finalization
  fVoiceHasBeenFinalized = false;

  // calls counter (for debug)
  fCallsCounter = 0;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    gLog <<
      "*** Initial contents of voice " <<
      fVoiceName <<
      " in staff \"" <<
      fVoiceUpLinkToStaff->getStaffPathLikeName () <<
      "\":" <<
      std::endl;

    ++gIndenter;

    this->print (gLog);

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

S_msrVoice msrVoice::createVoiceNewbornClone (
  const S_msrStaff& staffClone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a voice newborn clone of " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    staffClone != nullptr,
    "staffClone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  msrVoice* newbornClone =
    new msrVoice (
      fInputLineNumber,
      fVoiceKind,
      fVoiceNumber,
      staffClone);
  assert (newbornClone != nullptr);

  // DON'T create the voice segment on the fly, it will be created upon browsing

  // voice numbers
  newbornClone->fRegularVoiceStaffSequentialNumber =
    fRegularVoiceStaffSequentialNumber;

  newbornClone->fRegularVoiceOrdinalNumberInPart =
    fRegularVoiceOrdinalNumberInPart;

  // voice name
  newbornClone->fVoiceName =
    fVoiceName;

  // DON'T create the voice segment,
  // that will be done upon browing

  return newbornClone;
}

S_msrVoice msrVoice::createVoiceDeepClone (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceKind             voiceKind,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff&        containingStaff)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a deep clone of voice " <<
      fVoiceName <<
      ", to be placed in containing staff \"" <<
      containingStaff->getStaffPathLikeName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingStaff != nullptr,
    "containingStaff is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    gLog <<
      "****" <<
      " BEFORE deepClone: " <<
      std::endl;

    print (gLog);

    gLog <<
      std::endl <<
      "****" <<
      std::endl << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrVoice
    deepClone =
      msrVoice::createAsWellAsItsSegment (
        fInputLineNumber,
        voiceKind,
        voiceNumber,
        containingStaff);

  // voice numbers
  deepClone->fRegularVoiceStaffSequentialNumber =
    fRegularVoiceStaffSequentialNumber;

  deepClone->fRegularVoiceOrdinalNumberInPart =
    fRegularVoiceOrdinalNumberInPart;

  // voice name
  if (false) { // JMI
    deepClone->fVoiceName =
      fVoiceName;
  }

/* JMI
  // set voice name // JMI
  deepClone->
    setVoiceNamesFromNumber (
      fInputLineNumber,
      voiceNumber);
*/

  // counters
  deepClone->fVoiceActualNotesCounter =
    fVoiceActualNotesCounter;

  deepClone->fVoiceRestsCounter =
    fVoiceRestsCounter;

  deepClone->fVoiceSkipsCounter =
    fVoiceSkipsCounter;

  deepClone->fVoiceActualHarmoniesCounter =
    fVoiceActualHarmoniesCounter;

  deepClone->fVoiceActualFiguredBassesCounter =
    fVoiceActualFiguredBassesCounter;

  // measures
  deepClone->fVoiceCurrentMeasureNumber =
    fVoiceCurrentMeasureNumber;
  deepClone->fVoiceCurrentMeasureOrdinalNumber =
    fVoiceCurrentMeasureOrdinalNumber;
  deepClone->fVoiceCurrentMeasurePuristNumber =
    fVoiceCurrentMeasurePuristNumber;

  // musically empty voices
  deepClone->fVoiceIsMusicallyEmpty =
    fVoiceIsMusicallyEmpty;

  // regular measure ends detection
  deepClone->fWholeNotesSinceLastRegularMeasureEnd =
    fWholeNotesSinceLastRegularMeasureEnd;

  // incomplete measures after repeats detection
  deepClone->fCurrentVoiceRepeatPhaseKind =
    fCurrentVoiceRepeatPhaseKind;

  // multi-measure rests
  deepClone->fVoiceContainsMultiMeasureRests =
    fVoiceContainsMultiMeasureRests;

  // measures reeats
  deepClone->fVoiceContainsMeasureRepeats =
    fVoiceContainsMeasureRepeats;

  // stanzas
  for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
    S_msrStanza stanza = thePair.second;

    deepClone->
      addStanzaToVoiceWithoutCatchUp ( // JMI
        stanza->
          createStanzaDeepClone (
            deepClone));
  } // for

  // upLinks
  deepClone->fVoiceUpLinkToStaff =
    containingStaff;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    std::stringstream ss;

    ss <<
      "****" <<
      " AFTER deepClone: " <<
      std::endl <<
      deepClone <<
      std::endl <<
      "****";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return deepClone;
}

S_msrVoice msrVoice::createRegularVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff&        voiceUpLinkToStaff)
{
  return
    msrVoice::createAsWellAsItsSegment (
      inputLineNumber,
      msrVoiceKind::kVoiceKindRegular,
      voiceNumber,
      voiceUpLinkToStaff);
}

S_msrVoice msrVoice::createHarmoniesVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff&        voiceUpLinkToStaff) // unused yet JMI 0.9.66
{
  return
    msrVoice::createAsWellAsItsSegment (
      inputLineNumber,
      msrVoiceKind::kVoiceKindHarmonies,
      voiceNumber,
      voiceUpLinkToStaff);
}

S_msrVoice msrVoice::createFiguredBassVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff&        voiceUpLinkToStaff) // unused yet JMI0.9.66
{
  return
    msrVoice::createAsWellAsItsSegment (
      inputLineNumber,
      msrVoiceKind::kVoiceKindFiguredBass,
      voiceNumber,
      voiceUpLinkToStaff);
}

void msrVoice::setVoiceSegment (const S_msrSegment& segment)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceVoicesBasics ()
      ||
    gTraceOahGroup->getTraceSegmentsBasics ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting voice segment in voice " <<
      asString () <<
      " to " <<
      segment->asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment = segment;

  // set it as the current voice recipient segment
  // this will change when repeats within the voice are being built
//   setVoiceCurrentRecipientSegment (segment);
}

// void msrVoice::setVoiceCurrentRecipientSegment (const S_msrSegment& segment)
// {
//  #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceSegmentsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Setting voice current recipient segment in voice " <<
//       asString () <<
//       " to " <<
//       segment->asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// //   fVoiceSegment = fVoiceSegment;
// }

S_msrSegment msrVoice::fetchVoiceCurrentRecipientSegment () const
{
  S_msrSegment result;

  if (fVoicePendingRepeatsStack.empty ()) {
    result = fVoiceSegment;
  }
  else {
    result =
      fVoicePendingRepeatsStack.front ()->
        getRepeatCommonPart ()-> // TEMP JMI 0.9.76 could also be an ending.. ZAZA
          getRepeatElementSegment ();
  }

  return result;
}

S_msrPart msrVoice::fetchVoiceUpLinkToPart () const
{
  S_msrPart result;

  if (fVoiceUpLinkToStaff) {
    result =
      fVoiceUpLinkToStaff->
        getStaffUpLinkToPart ();
  }

  return result;
}

S_msrPartGroup msrVoice::fetchVoiceUpLinkToPartGroup () const
{
  S_msrPartGroup result;

  if (fVoiceUpLinkToStaff) {
    result =
      fVoiceUpLinkToStaff->
        fetchStaffUpLinkToPartGroup ();
  }

  return result;
}

S_msrScore msrVoice::fetchVoiceUpLinkToScore () const
{
  S_msrScore result;

  if (fVoiceUpLinkToStaff) {
    result =
      fVoiceUpLinkToStaff->
        fetchStaffUpLinkToScore ();
  }

  return result;
}

void msrVoice::setRegularVoiceStaffSequentialNumber (
  int regularVoiceStaffSequentialNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Setting the regular voice staff sequential number of voice " <<
      fVoiceName <<
      " to " << regularVoiceStaffSequentialNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fRegularVoiceStaffSequentialNumber =
    regularVoiceStaffSequentialNumber;
}

void msrVoice::setVoiceNamesFromNumber (
  const mfInputLineNumber& inputLineNumber,
  const mfVoiceNumber&     voiceNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Setting the names of " <<
      fVoiceKind <<
      " voice from voice number " << voiceNumber <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      fVoiceName =
        fVoiceUpLinkToStaff->getStaffPathLikeName () +
        "_Voice_" +
        mfVoiceNumberAsString (voiceNumber);

      fVoicePathLikeName =
        fVoiceUpLinkToStaff->getStaffPathLikeName () +
        "_Voice_" +
        mfIntToEnglishWord (mfVoiceNumberAsInteger (voiceNumber));
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      fVoiceName =
        fVoiceUpLinkToStaff->getStaffPathLikeName () +
        "_HARMONIES_Voice";

      fVoicePathLikeName =
        fVoiceUpLinkToStaff->getStaffPathLikeName () +
        "_HARMONIES_Voice" +
        mfIntToEnglishWord (mfVoiceNumberAsInteger (voiceNumber));
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      fVoiceName =
        fVoiceUpLinkToStaff->getStaffPathLikeName () +
        "_FIGURED_BASS_Voice";

      fVoicePathLikeName =
        fVoiceUpLinkToStaff->getStaffPathLikeName () +
        "_FIGURED_BASS_Voice" +
        mfIntToEnglishWord (mfVoiceNumberAsInteger (voiceNumber));
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "The resulting voice names are \"" <<
      fVoiceName <<
      "\" and " <<
      fVoicePathLikeName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::changeVoiceIdentity ( // after a deep clone is created
  const mfVoiceNumber& voiceNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Changing the partGroupSequentialNumber of voice " <<
      fVoiceName <<
      ", number: " << voiceNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // make it a regular voice
  setVoiceKind (
    msrVoiceKind::kVoiceKindRegular);

  // set its voice number
  setVoiceNumber (
    voiceNumber);

  // set its name
  setVoiceNamesFromNumber (
    fInputLineNumber,
    voiceNumber);
}

bool msrVoice::compareVoicesByIncreasingNumber (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  return
    first->fVoiceNumber
      <
    second->fVoiceNumber;
}

bool msrVoice::compareVoicesToHaveHarmoniesAboveCorrespondingVoice (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  mfVoiceNumber
    firstVoiceNumber =
      first->fVoiceNumber,
    secondVoiceNumber =
      second->fVoiceNumber;

  if (firstVoiceNumber > K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER) {
//     firstVoiceNumber -= K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER + 1; // JMI 0.9.75
    firstVoiceNumber = firstVoiceNumber - (K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER + 1);
  }
  if (secondVoiceNumber > K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER) {
//     secondVoiceNumber -= K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER + 1; // JMI 0.9.75
    secondVoiceNumber = secondVoiceNumber - (K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER + 1);
  }

  bool result =
    firstVoiceNumber < secondVoiceNumber;

  return result;

  /* JMI
  switch (firstVoiceNumber) {
    case msrVoiceKind::kVoiceKindRegular:
      switch (secondVoiceNumber) {
        case msrVoiceKind::kVoiceKindRegular:
          break;

        case msrVoiceKind::kVoiceKindHarmonies:
          result =
            secondVoiceNumber - K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER
              >
            firstVoiceNumber;
          break;

        case msrVoiceKind::kVoiceKindFiguredBass:
          break;
      } // switch
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

      switch (secondVoiceNumber) {
        case msrVoiceKind::kVoiceKindRegular:
          result =
            firstVoiceNumber - K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER
              >
            secondVoiceNumber;
          break;

        case msrVoiceKind::kVoiceKindHarmonies:
          break;

        case msrVoiceKind::kVoiceKindFiguredBass:
          break;
      } // switch
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      switch (secondVoiceNumber) {
        case msrVoiceKind::kVoiceKindRegular:
          break;

        case msrVoiceKind::kVoiceKindDynamics:
          break;

        case msrVoiceKind::kVoiceKindHarmonies:
          break;

        case msrVoiceKind::kVoiceKindFiguredBass:
          break;
      } // switch
      break;
  } // switch

  return result;
  */
}

bool msrVoice::compareVoicesToHaveFiguredBassesBelowCorrespondingVoice (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  mfVoiceNumber
    firstVoiceNumber =
      first->fVoiceNumber,
    secondVoiceNumber =
      second->fVoiceNumber;

  if (firstVoiceNumber > K_MF_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) {
//     firstVoiceNumber -= K_MF_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + 1;
    firstVoiceNumber = firstVoiceNumber - (K_MF_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + 1);
  }
  if (secondVoiceNumber > K_MF_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) {
//     secondVoiceNumber -= K_MF_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + 1;
    secondVoiceNumber = secondVoiceNumber - (K_MF_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + 1);
  }

  bool result =
    firstVoiceNumber > secondVoiceNumber;

  return result;
}

// void msrVoice::appendSegmentToVoiceClone ( //JMI unused ???
//   const S_msrSegment& segment)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceSegmentsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending segment " <<
//       segment->asString () <<
//       " to voice clone \"" <<
//       fVoiceName <<
//       "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // does segment belong to a repeat?
//   if (! fVoicePendingRepeatsStack.empty ()) {
//     // yes
// }

void msrVoice::setVoiceLastAppendedMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsBasics ()) {
    std::stringstream ss;

    ss <<
      "Setting voice last appended measure to " <<
      fetchMeasureAsString (measure) <<
      " in voice " <<
      fVoiceName <<
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastAppendedMeasure = measure;
}

void msrVoice::edacsacSetNextMeasureNumberInVoice ( // cascade bottom
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Edacsaccing setting next measure number to " <<
      nextMeasureNumber <<
      ", in voice " <<
      fVoiceName <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // is there a current multi-measure rests in this voice?
  if (fVoiceMultiMeasureRestsWaitingForItsNextMeasureNumber) {
    // yes
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
      std::stringstream ss;

      ss <<
        "There is a multi-measure rests waiting for its next measure number" <<
        ", fVoiceRemainingMultiMeasureRests: " <<
        fVoiceRemainingMultiMeasureRests <<
        " in voice " <<
        fVoiceName;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    --fVoiceRemainingMultiMeasureRests;

    // is this the last measure in the row?
    if (fVoiceRemainingMultiMeasureRests == 0) {
      // yes, set waiting multi-measure rests's next measure number
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
        std::stringstream ss;

        ss <<
          "Setting multi-measure rests next measure number to " <<
          nextMeasureNumber <<
          " in voice " <<
          fVoiceName;

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fVoiceMultiMeasureRestsWaitingForItsNextMeasureNumber->
        setNextMeasureNumber (
          nextMeasureNumber);

      // forget about this waiting multi-measure rests
      fVoiceMultiMeasureRestsWaitingForItsNextMeasureNumber = nullptr;
    }
  }

  --gIndenter;
}

void msrVoice::incrementVoiceCurrentMeasurePuristNumber (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  if (fVoiceCurrentMeasurePuristNumber < 0) { // JMI 0.9.67
    fVoiceCurrentMeasurePuristNumber = 0;

    fVoiceFirstMeasurePuristNumber =
      fVoiceCurrentMeasurePuristNumber;
  }
  else {
    ++fVoiceCurrentMeasurePuristNumber;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
    std::stringstream ss;

    ss <<
      "Incrementing the voice current measure purist number of voice " <<
      fVoiceName <<
      " to " <<
      fVoiceCurrentMeasurePuristNumber <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::setVoiceFirstMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresBasics ()) {
    std::stringstream ss;

    ss <<
      "Measure " <<
      measure->asShortString () <<
      " is the first measure in  voice " <<
      fVoiceName <<
      ", line " << measure->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceFirstMeasure = measure;
}

void msrVoice::addMeasureCloneToVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure&      measureClone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresBasics ()) {
    std::stringstream ss;

    ss <<
      "Adding measure clone " <<
      measureClone->asString () <<
      " to voice clone " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  S_msrSegment
    voiceCurrentRecipientSegment =
      fetchVoiceCurrentRecipientSegment ();

  // append measureClone to the voice current recipient segment
  voiceCurrentRecipientSegment->
    appendMeasureToSegment (measureClone);

  // measureClone is the new voice last appended measure
  setVoiceLastAppendedMeasure (
    measureClone);

  --gIndenter;
}

void msrVoice::setWholeNotesSinceLastRegularMeasureEnd (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes&      wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting voice whole notes since last regular measure end to " <<
      wholeNotes.asString () <<
  // JMI    " (" << context << ")" <<
      " in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    wholeNotes.getNumerator () >= 0,
    "mfWholeNotes numerator '" +
      std::to_string (wholeNotes.getNumerator ()) +
      " should be positive or null");

  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    wholeNotes.getDenominator () > 0,
    "mfWholeNotes denominator '" +
      std::to_string (wholeNotes.getDenominator ()) +
      " should be positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fWholeNotesSinceLastRegularMeasureEnd = wholeNotes;
}

void msrVoice::setCurrentVoiceRepeatPhaseKind (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceRepeatPhaseKind
           afterRepeatComponentPhaseKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting voice current after repeat component phase kind to " <<
     afterRepeatComponentPhaseKind <<
 // JMI     " (" << context << ")" <<
      " in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceRepeatPhaseKind =
    afterRepeatComponentPhaseKind;
}

S_msrMeasure msrVoice::createAMeasureAndAppendItInVoice (
  const mfInputLineNumber& inputLineNumber,
  int                      previousMeasureEndInputLineNumber,
  const mfMeasureNumber&   measureNumber,
  msrMeasureImplicitNumberKind
                           measureImplicitNumberKind)
{
  S_msrMeasure result;

  fVoiceCurrentMeasureNumber = measureNumber;

  fCallsCounter++;

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasuresBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating measure " <<
//       measureNumber <<
//       " and appending it to voice " <<
      // fVoiceName <<
//  //       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED

  if (
//     true ||
    false
      &&
    (
      fCallsCounter == 2 && fVoiceName
        ==
      "Part_POne_HARMONIES_Staff_Voice_Eleven_HARMONIES"
    )
  ) { // POUSSE JMI
    gLog <<
      std::endl <<
      "++++ createAMeasureAndAppendItInVoice() POUSSE, fCallsCounter: " << fCallsCounter << " ++++" <<
      std::endl;
    this->print (gLog);
    gLog <<
      std::endl;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createAMeasureAndAppendItInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // is there an on-going multi-measure rests?
  if (fOnGoingMultiMeasureRest) {
    // yes

    // create a measure
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "Creating a measure with number " <<
        measureNumber <<
        " in voice " <<
        fVoiceName <<
        "', line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    result =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        fVoiceSegment); // JMI 0.9.76 ??? ZOULOU

    // set result's ordinal number
    result->
      setMeasureOrdinalNumberInVoice (
        incrementVoiceCurrentMeasureOrdinalNumber ());

    // append it to the current multi-measure rests
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresBasics ()) {
      std::stringstream ss;

      ss <<
        "Appending measure " <<
        result->asString () <<
        " to current multi-measure rests " <<
        fVoiceCurrentMultiMeasureRest->asString () <<
        "in voice " <<
        fVoiceName <<
        "', line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fVoiceCurrentMultiMeasureRest->
      appendMeasureToMultiMeasureRest (
        result);
  }

  else {
    // no, there is no on-going multi-measure rests

    // make sure the voice current recipient has been set
//     if (! fVoiceSegment) {
//       fVoiceSegment = fVoiceSegment;
//     }

    // append a new measure with given number to the voice segment
    result =
      fVoiceSegment->
        createAMeasureAndAppendItInSegment (
          inputLineNumber,
          previousMeasureEndInputLineNumber,
          measureNumber,
          measureImplicitNumberKind);
  }

  // result is the new voice last appended measure
  setVoiceLastAppendedMeasure (
    result);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createAMeasureAndAppendItInVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  return result;
}

// S_msrVoice msrVoice::createRegularVoiceHarmoniesVoice (
//   const mfInputLineNumber& inputLineNumber,
//   const mfMeasureNumber&   currentMeasureNumber)
// {
//   if (fRegularVoiceForwardLinkToHarmoniesVoice) {
//     std::stringstream ss;
//
//     ss <<
//       "voice " <<
//       fVoiceName <<
//       " already has a harmonies voice";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // create the voice harmonies voice
//   mfVoiceNumber
//     regularVoiceHarmoniesVoiceNumber =
//       fVoiceNumber + K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER; // JMI 0.9.75
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceHarmonies ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating harmonies voice for regular voice " <<
//       fVoiceName <<
//       " with voice number " <<
//       regularVoiceHarmoniesVoiceNumber <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fRegularVoiceForwardLinkToHarmoniesVoice =
//     msrVoice::createAsWellAsItsSegment (
//       inputLineNumber,
//       msrVoiceKind::kVoiceKindHarmonies,
//       regularVoiceHarmoniesVoiceNumber,
//       fVoiceUpLinkToStaff);
//
//   // register it in the staff
//   fVoiceUpLinkToStaff->
//     registerVoiceInStaff (
//       inputLineNumber,
//       fRegularVoiceForwardLinkToHarmoniesVoice);
//
//   // set backward link
//   fRegularVoiceForwardLinkToHarmoniesVoice->
//     fHarmoniesVoiceRegularVoiceBackwardLink = this;
//
//   return fRegularVoiceForwardLinkToHarmoniesVoice;
// }
//
// S_msrVoice msrVoice::createRegularVoiceFiguredBassVoice (
//   const mfInputLineNumber& inputLineNumber,
//   const mfMeasureNumber&   currentMeasureNumber)
// {
//   if (fRegularVoiceForwardLinkToFiguredBassVoice) {
//     std::stringstream ss;
//
//     ss <<
//       "voice " <<
//       fVoiceName <<
//       " already has a figured bass voice";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // create the voice figured bass voice
//   int regularVoiceFiguredBassVoiceNumber =
//     K_MF_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + fVoiceNumber;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceFiguredBasses ()) {
//     gLog <<
//       "Creating figured bass voice for regular voice " <<
//       fVoiceName <<
//       " with voice number " <<
//       regularVoiceFiguredBassVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fRegularVoiceForwardLinkToFiguredBassVoice =
//     msrVoice::createAsWellAsItsSegment (
//       inputLineNumber,
//       msrVoiceKind::kVoiceKindFiguredBass,
//       regularVoiceFiguredBassVoiceNumber,
//       fVoiceUpLinkToStaff);
//
//   // register it in the staff
//   fVoiceUpLinkToStaff->
//     registerVoiceInStaff (
//       inputLineNumber,
//       fRegularVoiceForwardLinkToFiguredBassVoice);
//
//   // set backward link
//   fRegularVoiceForwardLinkToFiguredBassVoice->
//     fFiguredBassVoiceRegularVoiceBackwardLink = this;
//
//   return fRegularVoiceForwardLinkToFiguredBassVoice;
// }

S_msrStanza msrVoice::addStanzaToVoiceByItsNumber (
  const mfInputLineNumber& inputLineNumber,
  const mfStanzaNumber& stanzaNumber)
{
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    std::stringstream ss;

    ss <<
      "stanza " << stanzaNumber <<
      " already exists in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      std::endl;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // create the stanza
  S_msrStanza
    stanza =
      msrStanza::create (
        inputLineNumber,
        stanzaNumber,
        this);

  // add the stanza to this voice
  addStanzaToVoice (stanza);

  // return it
  return stanza;
}

void msrVoice::addStanzaToVoice (const S_msrStanza& stanza)
{
  // get stanza number
  const mfStanzaNumber&
    stanzaNumber =
      stanza->getStanzaNumber ();

  // register stanza in this voice
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

void msrVoice::addStanzaToVoiceClone (const S_msrStanza& stanza)
{
  // get stanza number
  const mfStanzaNumber&
    stanzaNumber =
      stanza->getStanzaNumber ();

  // register stanza in this voice
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice clone " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

void msrVoice::addStanzaToVoiceWithoutCatchUp (const S_msrStanza& stanza)
{
  // get stanza number
  const mfStanzaNumber& stanzaNumber =
    stanza->getStanzaNumber ();

  // register stanza in this voice
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

S_msrStanza msrVoice::createStanzaInVoiceIfNotYetDone (
  const mfInputLineNumber& inputLineNumber,
  const mfStanzaNumber& stanzaNumber,
  const std::string& stanzaName) // JMI
{
  S_msrStanza stanza;

  // is stanzaNumber already known in voice?
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    // yes, use it
    stanza =
      fVoiceStanzasMap [stanzaNumber];
  }

  else {
    // no, create it and add it to the voice
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "Creating stanza" <<
        " number " << stanzaNumber <<
        ", name \"" << stanzaName <<
        ", in voice " <<
        fVoiceName <<
        ", line " << inputLineNumber <<
        ", fVoiceStanzasMap.size (): " << fVoiceStanzasMap.size ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    stanza =
      addStanzaToVoiceByItsNumber (
        inputLineNumber, stanzaNumber);
  }

  return stanza;
}

S_msrStanza msrVoice::fetchStanzaInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfStanzaNumber&    stanzaNumber,
  const std::string&       stanzaName)
{
  S_msrStanza stanza;

  // is stanzaNumber known in voice?
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    // yes, use it
    stanza =
      fVoiceStanzasMap [stanzaNumber];
  }

  else {
    std::stringstream ss;

    gLog <<
      std::endl << std::endl << std::endl <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE 1" <<
      std::endl;

    print (gLog);

    gLog <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE 2" <<
      std::endl << std::endl << std::endl <<
      std::endl;


    ss <<
      "Stanza" <<
      " number " << stanzaNumber <<
      ", name \"" << stanzaName <<
      ", not found in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      ", fVoiceStanzasMap.size (): " << fVoiceStanzasMap.size () <<
      std::endl;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  return stanza;
}

void msrVoice::setVoiceCurrentClef  (
  const S_msrClef& clef)
{
  fVoiceCurrentClef = clef;

  if (! fVoiceFirstClef) {
    fVoiceFirstClef = fVoiceCurrentClef;
  }
};

void msrVoice::setVoiceCurrentKey (
  const S_msrKey& key)
{
  fVoiceCurrentKey = key;
};

void msrVoice::setVoiceCurrentTimeSignature (
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Setting voice current time signature of " <<
      asString () <<
      " to " <<
      timeSignature->asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceCurrentTimeSignature = timeSignature;
};

void msrVoice::appendMxmlPrintLayoutToVoice (
  const S_msrMxmlPrintLayout& MxmlPrintLayout)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMxmlPrintLayouts ()) {
    std::stringstream ss;

    ss <<
      "Appending print layout " << MxmlPrintLayout->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  fVoiceSegment->
    appendMxmlPrintLayoutToSegment (MxmlPrintLayout);

  --gIndenter;
}

void msrVoice::appendClefKeyTimeSignatureGroupToVoice  (
  const S_msrClefKeyTimeSignatureGroup&
    clefKeyTimeSignatureGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending clefKeyTimeSignatureGroup " <<
      clefKeyTimeSignatureGroup->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set voice current clef if relevant
  S_msrClef
    clefToBeAdded =
      clefKeyTimeSignatureGroup->getClef ();
  if (clefToBeAdded) {
    this->setVoiceCurrentClef (clefToBeAdded);
  }

  // set voice current key if relevant
  S_msrKey
    keyToBeAdded =
      clefKeyTimeSignatureGroup->getKey ();
  if (keyToBeAdded) {
    this->setVoiceCurrentKey (keyToBeAdded);
  }

  // set voice current time signature if relevant
  S_msrTimeSignature
    timeSignatureToBeAdded =
      clefKeyTimeSignatureGroup->getTimeSignature ();

  if (timeSignatureToBeAdded) {
    this->setVoiceCurrentTimeSignature (timeSignatureToBeAdded);
  }

  // append clefKeyTimeSignatureGroup to the voice segment
  fVoiceSegment->
    appendClefKeyTimeSignatureGroupToSegment (
      clefKeyTimeSignatureGroup);

  --gIndenter;
}

// void msrVoice::appendClefToVoice  (
//   const S_msrClef& clef)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceClefs ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending clef " << clef
//       " to voice " <<
//         fVoiceName;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // set voice current clef
//   this->setVoiceCurrentClef (clef);
//
//   if (fVoiceIsMusicallyEmpty) {
//     // append clef to the voice segment
//     fVoiceSegment->
//       appendClefToSegment (clef);
//   }
//
//   else {
//     // moving clefs to the left, thus prepend to the voice segment
//     fVoiceSegment->
// //       prependClefToSegment (clef);
//       appendClefToSegment (clef); // JMI 0.9.67
//   }
//
//   --gIndenter;
// }
//
// void msrVoice::appendKeyToVoice (
//   const S_msrKey& key)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceKeys ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending key " << key
//       " to voice " <<
      // fVoiceName;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // set voice current clef
//   this->setVoiceCurrentKey (key);
//
//   // append key to the voice segment
//   fVoiceSegment->
//     appendKeyToSegment (key);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceKeysDetails ()) {
//     displayVoice ( // JMI TEMP
//       key->getInputLineNumber (),
//       "appendKeyToVoice()");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   --gIndenter;
// }

void msrVoice::appendTimeSignatureToVoice (
  const S_msrTimeSignature& timeSignature){
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature " <<
      timeSignature->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set voice current timeSignature
  this->setVoiceCurrentTimeSignature (timeSignature);

  // append timeSignature to the the voice segment
  fVoiceSegment->
    appendTimeSignatureToSegment (timeSignature);

  --gIndenter;
}

void msrVoice::appendTimeSignatureToVoiceClone (
  const S_msrTimeSignature& timeSignature) // superflous ??? JMI
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature " <<
      timeSignature->asString () <<
      " to voice clone \"" <<
      fVoiceName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set voice current time signature
  this->setVoiceCurrentTimeSignature (timeSignature);

  // append timeSignature to the the voice segment
  fVoiceSegment->
    appendTimeSignatureToSegmentClone (timeSignature);

  --gIndenter;
}

// void msrVoice::insertHiddenMeasureAndBarLineInVoiceClone (
//   const mfInputLineNumber& inputLineNumber,
//   const mfPositionInMeasure& positionInMeasure)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Inserting hidden measure and barLine at position " <<
//       positionInMeasure.asString () <<
//       " to voice clone \"" <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // insert hidden measure and barLine to the the voice segment
//   fVoiceSegment->
//     insertHiddenMeasureAndBarLineInSegmentClone (
//       inputLineNumber,
//       positionInMeasure);
//
//   --gIndenter;
// }

S_msrNote msrVoice::fetchVoiceFirstNonGraceNote () const
{
  S_msrNote result;

//     // get the voice's first measure
//     S_msrMeasure
//       voiceFirstMeasure =
//         fVoicSegment->
//           getSegmentFirstMeasure ();
//
//     if (voiceFirstMeasure) {
//       // get the first measure's elements list
//       const std::list <S_msrMeasureElement>&
//         firstMeasureElementsList =
//           voiceFirstMeasure->
//             getMeasureElementsList ();
//
//       // fetch the first note in the first measure to which
//       // a grace notes group can be attached
//       // i.e. one not in a grace notes group itself,
//       // possibly inside a chord or tuplet
//
//       if (! firstMeasureElementsList.empty ()) {
//         for (S_msrMeasureElement measureElement : firstMeasureElementsList) {
//           if (
//             S_msrNote note = dynamic_cast<msrNote*>(&(*measureElement))
//           ) {
//             result = note;
//             break;
//           }
//
//           else if (
//             S_msrChord chord = dynamic_cast<msrChord*>(&(*measureElement))
//           ) {
//             // get the chord's first note
//             result = chord->fetchChordFirstNonGraceNote ();
//             break;
//           }
//
//           else if (
//             S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*measureElement))
//           ) {
//             // get the tuplet's first note
//             result = tuplet->fetchTupletFirstNonGraceNote ();
//             break;
//           }
//
//           else if (
//             S_msrRepeat repeat = dynamic_cast<msrRepeat*>(&(*measureElement))
//           ) {
//             // get the repeat's first note
//             result = repeat->fetchRepeatFirstNonGraceNote ();
//             break;
//           }
//
//           else if (
//             S_msrClef clef = dynamic_cast<msrClef*>(&(*measureElement))
//           ) {
//             // ignore this clef
//           }
//
//           else if (
//             S_msrKey key = dynamic_cast<msrKey*>(&(*measureElement))
//           ) {
//             // ignore this key
//           }
//
//           else if (
//             S_msrTimeSignature
//               timeSignature =
//                 dynamic_cast<msrTimeSignature*>(&(*measureElement))
//           ) {
//             // ignore this timeSignature
//           }
//
//           else {
//             // ignore this measureElement and return nullptr // JMI ???
//             /*
//             std::stringstream ss;
//
//             ss <<
//               "tuplet first measureElement should be a note, a chord or another tuplet, found instead " <<
//               measureElement->asShortString () <<
//               "'";
//
//             msrInternalError (
//               gServiceRunData->getInputSourceName (),
//               fInputLineNumber,
//               __FILE__, mfInputLineNumber (__LINE__),
//               ss.str ());
//               */
//           }
//         } // for
//       }
//     }

//     else {
// #ifdef MF_TRACE_IS_ENABLED
//       if (true || gTraceOahGroup->getTraceMeasuresDetails ()) { // JMI 0.9.67  // JMI 0.9.66
//         gLog <<
//           "++++++++++ fetchVoiceFirstNonGraceNote(), this voice: ++++++++++" <<
//           std::endl;
//         ++gIndenter;
//         gLog << std::hex << std::showbase << this << std::dec; // JMI 0.9.69;
//         --gIndenter;
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       std::stringstream ss;
//
//       ss <<
//         "there is no first measure in the first segment in voice " << // JMI 0.9.63  // JMI 0.9.66
//         fVoiceName <<
//         ", since the latter is empty";
//
//       msrInternalError (
//         gServiceRunData->getInputSourceName (),
//         fInputLineNumber,
//         __FILE__, mfInputLineNumber (__LINE__),
//         ss.str ());
//     }

  return result;
}

void msrVoice::setVoiceShortestNoteWholeNotes (
  const mfWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "The new shortest note wholeNotes in voice " <<
        fVoiceName <<
        " becomes " <<
        wholeNotes.asString ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  fVoiceShortestNoteWholeNotes = wholeNotes;
}

void msrVoice::setVoiceShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "The new shortest note tuplet factor in part \"" <<
        fVoiceName <<
        "\" becomes " <<
        noteTupletFactor;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  fVoiceShortestNoteTupletFactor = noteTupletFactor;
}

void msrVoice::registerShortestNoteInVoiceIfRelevant (const S_msrNote& note)
{
  // is note the shortest one in this voice?
  mfWholeNotes
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

      /* JMI
  mfWholeNotes
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();
        */

  if (noteSoundingWholeNotes < fVoiceShortestNoteWholeNotes) {
    // set the voice shortest note wholeNotes
    this->
      setVoiceShortestNoteWholeNotes (
        noteSoundingWholeNotes);

    // cascade this new value to the voice's part
    S_msrPart
      voicePartUplink =
        fetchVoiceUpLinkToPart ();

    voicePartUplink ->
      registerShortestNoteInPartIfRelevant (
        note);

/* JMI
    // set the voice shortest note tuplet factor
    fVoiceShortestNoteTupletFactor =
      note->
        getNoteTupletFactor ();

    // cascade this new value to the voice's part
    voicePartUplink ->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
*/

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "The new shortest note in voice " <<
        fVoiceName <<
        " becomes " << note->asString ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

/* JMI
  if (noteDisplayWholeNotes < fVoiceShortestNoteWholeNotes) {
    fVoiceShortestNoteWholeNotes = noteDisplayWholeNotes;
  }
    */
}

// void msrVoice::registerNoteAsVoiceLastAppendedNote (const S_msrNote& note)
// {
//   fVoiceLastAppendedNote = note;
//
//   // is note the shortest one in this voice?
//   mfWholeNotes
//     noteSoundingWholeNotes =
//       note->
//         getMeasureElementSoundingWholeNotes (),
//     noteDisplayWholeNotes =
//       note->
//         getNoteDisplayWholeNotes (); // JMI
//
//   if (noteSoundingWholeNotes < fVoiceShortestNoteWholeNotes) {
//     fVoiceShortestNoteWholeNotes = noteSoundingWholeNotes;
//   }
//   if (noteDisplayWholeNotes < fVoiceShortestNoteWholeNotes) {
//     fVoiceShortestNoteWholeNotes = noteDisplayWholeNotes;
//   }
// }

void msrVoice::edacsacAppendHarmonyToVoice ( // cascade bottom
  const mfInputLineNumber& inputLineNumber,
  const S_msrHarmony&        harmony,
  const mfPositionInMeasure& positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Edacsaccing appending harmony " << harmony->asString () <<
      " to voice " <<
      fVoiceName <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindHarmonies:
      break;

    case msrVoiceKind::kVoiceKindDynamics: // JMI ??? 0.9.67
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        std::stringstream ss;

        ss <<
          "cannot append a harmony to " <<
          fVoiceKind <<
          " voice " <<
          fVoiceName;

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;
  } // switch
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append the harmony to the voice the voice segment
  fVoiceSegment->
    appendHarmonyToSegment (
      inputLineNumber,
      harmony,
      positionInMeasureToAppendAt);

  --gIndenter;

  // register harmony
  ++fVoiceActualHarmoniesCounter;


  fVoiceIsMusicallyEmpty = false;
}

void msrVoice::cascadeAppendHarmoniesListToVoice (
  const mfInputLineNumber&        inputLineNumber,
  const std::list <S_msrHarmony>& harmoniesList,
  const mfPositionInMeasure&      positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmonies list to voice " << // JMI 0.9.67 HARMFUL
      fVoiceName <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the harmonies to the voice the voice segment
  fVoiceSegment->
    cascadeAppendHarmoniesListToSegment (
      inputLineNumber,
      harmoniesList,
      positionInMeasureToAppendAt);
}

void msrVoice::edacsacAppendHarmonyToVoiceClone ( // cascade bottom
  const S_msrHarmony& harmony)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Edacsaccing appending harmony " << harmony->asString () <<
      " to voice clone \"" <<
      fVoiceName <<
      ", line " << harmony->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindHarmonies:
      fVoiceSegment->
        appendHarmonyToSegmentClone (harmony);

      // register harmony
      ++fVoiceActualHarmoniesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        std::stringstream ss;

        ss <<
          "cannot append a harmony to " <<
          fVoiceKind <<
          " voice clone \"" <<
          fVoiceName <<
          "\"";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          harmony->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendFiguredBassToVoice (
  const mfInputLineNumber& inputLineNumber,
  const S_msrFiguredBass&   figuredBass,
  const mfPositionInMeasure& positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass " << figuredBass->asString () <<
      " to voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break; // JMI 0.9.67

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        std::stringstream ss;

        ss <<
          "cannot append a figured bass to " <<
          fVoiceKind <<
          " voice " <<
          fVoiceName;

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;
  } // switch
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // append figuredBass to the voice the voice segment
  fVoiceSegment->
    appendFiguredBassToSegment (
      inputLineNumber,
      figuredBass,
      positionInMeasureToAppendAt);

  --gIndenter;

  // register figuredBass
  ++fVoiceActualFiguredBassesCounter;
  fVoiceIsMusicallyEmpty = false;
}

void msrVoice::cascadeAppendFiguredBassesListToVoice (
  const mfInputLineNumber&            inputLineNumber,
  const std::list <S_msrFiguredBass>& figuredBasssesList,
  const mfPositionInMeasure&          positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured basses list \"" <<
//       figuredBasssesList->asString () << // JMI 0.9.76 HARMFUL
      " to voice " <<
      fVoiceName <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the figured basses to the voice the voice segment
  ++gIndenter;
  fVoiceSegment->
    cascadeAppendFiguredBassesListToSegment (
      inputLineNumber,
      figuredBasssesList,
      positionInMeasureToAppendAt);
  --gIndenter;
}

void msrVoice::appendFiguredBassToVoiceClone (
  const S_msrFiguredBass& figuredBass)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass " << figuredBass->asString () <<
      " to voice clone \"" <<
      fVoiceName <<
      ", line " << figuredBass->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      fVoiceSegment->
        appendFiguredBassToSegmentClone (figuredBass);

      // register figured bass
      ++fVoiceActualFiguredBassesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        std::stringstream ss;

        ss <<
          "cannot append a figured bass to " <<
          fVoiceKind <<
          " voice clone \"" <<
          fVoiceName <<
          "\"";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          figuredBass->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;
  } // switch

  --gIndenter;
}

// void msrVoice::padUpToPositionInMeasureInVoice (
//   const mfInputLineNumber& inputLineNumber,
//   const mfWholeNotes&      wholeNotesPositionInMeasure)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePositionInMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Padding up to measure position " <<
//       wholeNotesPositionInMeasure.asString () <<
//       " whole notes in voice " <<
//       fVoiceName <<
//       ", line " << inputLineNumber <<
//       std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, mfInputLineNumber (__LINE__),
//         ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // pad up the voice's the voice segment
//   fVoiceSegment->
//     padUpToPositionInMeasureInSegment (
//       inputLineNumber,
//       wholeNotesPositionInMeasure);
//
//   // pad up the voice's stanzas // JMI ???
//   if (! fVoiceStanzasMap.empty ()) {
//     for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
//       S_msrStanza stanza = thePair.second;
//
//       stanza->
//         padUpToMeasureCurrentPositionInMeasureInStanza (
//           inputLineNumber,
//           wholeNotesPositionInMeasure);
//     } // for
//   }
//
//   --gIndenter;
// }

// void msrVoice::casadeBackupByWholeNotesStepLengthInVoice (
//   const mfInputLineNumber&
//              inputLineNumber,
//   const mfWholeNotes&
//              backupTargetMeasureElementPositionInMeasure)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceDurations ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Backup by a " <<
//       backupTargetMeasureElementPositionInMeasure.asString () <<
//       " whole notes step length in voice " <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // pad up the voice's the voice segment
//   fVoiceSegment->
//     casadeBackupByWholeNotesStepLengthInSegment (
//       inputLineNumber,
//       backupTargetMeasureElementPositionInMeasure);
//
//   --gIndenter;
// }

void msrVoice::cascadeAppendPaddingNoteToVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes&      forwardStepLength)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotes ()
      ||
    gTraceOahGroup->getTraceVoices ()
  ) {
    std::stringstream ss;

    ss <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to voice " <<
      fVoiceName <<
      ",line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // pad up the voice's the voice segment
  ++gIndenter;
  fVoiceSegment->
    cascadeAppendPaddingNoteToSegment (
      inputLineNumber,
      forwardStepLength);
  --gIndenter;

  // account for padding note's wholeNotes in the part drawing measure position
  fVoiceUpLinkToStaff->
    getStaffUpLinkToPart ()->
      incrementPartCurrentDrawingPositionInMeasure (
        inputLineNumber,
        forwardStepLength);

  // pad up the voice's stanzas JMI USELESS??? 0.9.70
  if (! fVoiceStanzasMap.empty ()) {
    for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      stanza->
        appendPaddingNoteToStanza (
          inputLineNumber,
          forwardStepLength);
    } // for
  }

  --gIndenter;
}

void msrVoice::appendTranspositionToVoice (
  const S_msrTransposition& transposition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTranspositions ()) {
    std::stringstream ss;

    ss <<
      "Appending transposition " <<
      transposition->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendTranspositionToSegment (transposition);
}

void msrVoice::appendStaffDetailsToVoice (
  const S_msrStaffDetails& staffDetails)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Appending staff details " <<
      staffDetails->asShortString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // staff details are attributes in MusicXML JMI 0.9.76 ???

  fVoiceSegment->
    appendStaffDetailsToSegment (staffDetails);
}

void msrVoice::appendTempoToVoice (
  const S_msrTempo& tempo)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Appending tempo " <<
      tempo->asShortString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendTempoToSegment (tempo);
}

void msrVoice::appendOctaveShiftToVoice (
  const S_msrOctaveShift& octaveShift)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceOctaveShifts ()) {
    std::stringstream ss;

    ss <<
      "Appending octave shift " <<
      octaveShift->getOctaveShiftKind () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendOctaveShiftToSegment (octaveShift);
}

void msrVoice::appendScordaturaToVoice (
  const S_msrScordatura& scordatura)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceScordaturas ()) {
    std::stringstream ss;

    ss <<
      "Appending scordatura " <<
      scordatura->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendScordaturaToSegment (scordatura);
}

void msrVoice::appendAccordionRegistrationToVoice (
  const S_msrAccordionRegistration&
    accordionRegistration)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceAccordionRegistrations ()) {
    std::stringstream ss;

    ss <<
      "Appending accordion registration " <<
      accordionRegistration->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendAccordionRegistrationToSegment (
      accordionRegistration);
}

void msrVoice::appendHarpPedalsTuningToVoice (
  const S_msrHarpPedalsTuning&
    harpPedalsTuning)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarpPedals ()) {
    std::stringstream ss;

    ss <<
      "Appending harp pedals tuning " <<
      harpPedalsTuning->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendHarpPedalsTuningToSegment (
      harpPedalsTuning);
}

void msrVoice::appendRehearsalMarkToVoice (
  const S_msrRehearsalMark& rehearsalMark)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRehearsalMarks ()) {
    std::stringstream ss;

    ss <<
      "Appending rehearsalMark " << rehearsalMark->getRehearsalMarkText () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendRehearsalMarkToSegment (rehearsalMark);
}

void msrVoice::appendVoiceStaffChangeToVoice (
  const S_msrVoiceStaffChange& voiceStaffChange)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChanges ()) {
    std::stringstream ss;

    ss <<
      "Appending voice staff change " <<
      voiceStaffChange->asString () <<
      " to voice " <<
      fVoiceName <<
      ", line " << voiceStaffChange->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append voice staff change to voice's the voice segment
  fVoiceSegment->
    appendVoiceStaffChangeToSegment (
      voiceStaffChange);

  --gIndenter;
}

void msrVoice::appendNoteToVoice (const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending note " <<
      note->asString () <<
      " to voice " <<
      fVoiceName <<
      ", line " << note->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // fetch the part
  S_msrPart
    part =
      fetchVoiceUpLinkToPart ();

  // fetch the part current measure position
  mfPositionInMeasure
    partCurrentDrawingPositionInMeasure =
      part->
        getPartCurrentDrawingPositionInMeasure ();

  // append the note to the the voice segment
  fVoiceSegment->
    appendNoteToSegment (
      note,
      partCurrentDrawingPositionInMeasure);

  // is this note the shortest one in this voice?
  this->
    registerShortestNoteInVoiceIfRelevant (
      note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;

  // account for note's wholeNotes in the part drawing measure position
  part->
    incrementPartCurrentDrawingPositionInMeasure (
      note->getInputLineNumber (),
      note->getMeasureElementSoundingWholeNotes ());

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNoteKind::kNote_UNKNOWN_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      // register rest
      ++fVoiceRestsCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      // don't account skips as music
      ++fVoiceSkipsCounter;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendNoteToVoiceClone (const S_msrNote& note) {
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending note " <<
      note->asString () <<
      " to voice clone \"" <<
      fVoiceName <<
      ", line " << note->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the note to the the voice segment
  fVoiceSegment->
    appendNoteToSegmentClone (note);

  // is this note the shortest one in this voice?
  this->
    registerShortestNoteInVoiceIfRelevant (
      note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNoteKind::kNote_UNKNOWN_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      // register rest
      ++fVoiceRestsCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      // don't account skips as music
      ++fVoiceSkipsCounter;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      // register actual note
      ++fVoiceActualNotesCounter;
      fVoiceIsMusicallyEmpty = false;
      break;
  } // switch
}

void msrVoice::appendDoubleTremoloToVoice (
  const S_msrDoubleTremolo& doubleTremolo)

{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Appending double tremolo " <<
      doubleTremolo->asShortString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendDoubleTremoloToSegment (doubleTremolo);

  fVoiceIsMusicallyEmpty = false;
}

void msrVoice::appendChordToVoice (const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending chord " <<
      chord->asShortString () <<
      " to voice " <<
      fVoiceName <<
      ", line " << chord->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append chord to voice the voice segment
  fVoiceSegment->
    appendChordToSegment (chord);

  // a chord can be appended to the voice
  // before it has been populated with its member notes
  // account for chord's wholeNotes in the part drawing measure position if relevant
  mfWholeNotes
    chordSoundingWholeNotes =
      chord->getMeasureElementSoundingWholeNotes ();

  if (chordSoundingWholeNotes.getNumerator () != 0) { // JMI 0.9.74
    fVoiceUpLinkToStaff->
      getStaffUpLinkToPart ()->
        incrementPartCurrentDrawingPositionInMeasure (
          chord->getInputLineNumber (),
          chord->getMeasureElementSoundingWholeNotes ());
  }

  // get the chord's notes vector
  const std::vector <S_msrNote>&
    chordNotesVector =
      chord->
        getChordNotesVector ();

  size_t chordNotesVectorSize =
    chordNotesVector.size ();

  if (chordNotesVectorSize) {
    {
      // get the chord's first note
      S_msrNote
        chordFirstNote =chordNotesVector.at (0);

      // is chordFirstNote the shortest one in this voice?
      this->
        registerShortestNoteInVoiceIfRelevant (
          chordFirstNote);
    }

    {
      // get the chord's last note
      S_msrNote
        chordLastNote =
          chordNotesVector.at (chordNotesVectorSize - 1);

/* JMI
      // is chordLastNote the shortest one in this voice?
      this->
        registerShortestNoteInVoiceIfRelevant (
          chordLastNote);
*/

      // register chordLastNote as the last appended one into this voice
      fVoiceLastAppendedNote = chordLastNote;
    }
  }

  fVoiceIsMusicallyEmpty = false;
}

void msrVoice::appendTupletToVoice (const S_msrTuplet& tuplet)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending tuplet " << tuplet->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append tuplet to voice the voice segment
  fVoiceSegment->
    appendTupletToSegment (tuplet);

  // account for tuplet's wholeNotes in the part's drawing measure position
  fVoiceUpLinkToStaff->
    getStaffUpLinkToPart ()->
      incrementPartCurrentDrawingPositionInMeasure (
        tuplet->getInputLineNumber (),
        tuplet->getMeasureElementSoundingWholeNotes ());

  --gIndenter;

  fVoiceIsMusicallyEmpty = false;
}

void msrVoice::registerTupletNoteInVoice (
  const S_msrNote& note)
{
  // is this note the shortest one in this voice?
  registerShortestNoteInVoiceIfRelevant (note);

  // fetch voice last measure
  S_msrMeasure
    voiceLastMeasure =
      fetchVoiceLastMeasure (
        note->getInputLineNumber ());

  // account for the duration of note in voice last measure
  voiceLastMeasure->
    accountForTupletMemberNoteNotesDurationInMeasure (
      note);
}

void msrVoice::addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
  const S_msrGraceNotesGroup& graceNotesGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Adding grace notes " <<
      graceNotesGroup->asString () <<
      " ahead of voice if needed in \"" <<
       fVoiceName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  gLog <<
    std::endl <<
    "======================= prependGraceNotesToVoice" <<
    std::endl;
  this->print (gLog);
  gLog <<
    "=======================" <<
    std::endl << std::endl;
  */

    // then create the first measure
    createAMeasureAndAppendItInVoice (
      graceNotesGroup->getInputLineNumber (),
      333, //         previousMeasureEndInputLineNumber, 0.9.62
      graceNotesGroup->
        getGraceNotesGroupMeasureNumber (),
      msrMeasureImplicitNumberKind::kMeasureImplicitNumberKindNo);
//   }

  // such grace notes groups should be attached to the voice's first note,
  // or to the first chord if the latter belongs to such

  // fetch the voice's first note
  S_msrNote
    voiceFirstNote =
      fetchVoiceFirstNonGraceNote (); // JMI

  // get the voice first note's uplink to chord
  S_msrChord
    firstNoteShortcutUpLinkToChord =
      voiceFirstNote->
        getNoteShortcutUpLinkToChord ();

  if (firstNoteShortcutUpLinkToChord) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Attaching grace notes before " <<
        graceNotesGroup->asString () <<
        " to the first chord of voice " <<
        fVoiceName <<
        ", i.e. " <<
        firstNoteShortcutUpLinkToChord->asShortString () <<
        "'";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

/*
    firstNoteShortcutUpLinkToChord->
      setChordGraceNotesGroupBefore (
        graceNotesGroup);
        */

    // create a grace notes group link
    const S_msrChordGraceNotesGroupLink&
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          graceNotesGroup->getInputLineNumber (),
          graceNotesGroup,
          firstNoteShortcutUpLinkToChord);

    // register it in the chord
    firstNoteShortcutUpLinkToChord->
      setChordGraceNotesGroupLinkBefore (
        graceNotesGroup->getInputLineNumber (),
        chordChordGraceNotesGroupLink);
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Attaching grace notes before " <<
        graceNotesGroup->asString () <<
        " to the first note of voice " << fVoiceName <<
        ", i.e. " <<
        voiceFirstNote->asShortString ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    voiceFirstNote->
      setGraceNotesGroupBeforeNote (
        graceNotesGroup);
  }

  fVoiceIsMusicallyEmpty = false;
}

/* JMI
void msrVoice::appendAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Appending after grace notes " << // JMI AfterGraceNotes <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendAfterGraceNotesToSegment (afterGraceNotes);

  fVoiceIsMusicallyEmpty = false;
}

void msrVoice::prependAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Prepending after grace notes " << // JMI AfterGraceNotes <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoicSegment->
    prependAfterGraceNotesToSegment (afterGraceNotes);

  fVoiceIsMusicallyEmpty = false;
}
*/

// void msrVoice::appendSyllableToVoice (
//   const mfInputLineNumber& inputLineNumber,
//   const mfStanzaNumber& stanzaNumber,
//   const std::string&   stanzaName,
//   const S_msrSyllable& syllable)
// {
//   // append syllable to this voice
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceLyrics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending syllable " <<
//       syllable->asString () <<
//       " to voice " <<
//       asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // fetch stanzaNumber in this voice
//   S_msrStanza
//     stanza =
//       createStanzaInVoiceIfNotYetDone (
//         inputLineNumber,
//         stanzaNumber,
//         stanzaName);
//
//   // add the syllable to the stanza clone
//   stanza->
//     appendSyllableToStanzaClone (
//       syllable,
//       fVoiceLastAppendedMeasure);
// }

void msrVoice::appendBarCheckToVoice (
  const S_msrBarCheck& barCheck)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Appending bar check " <<
      barCheck->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendBarCheckToSegment (barCheck);
}

void msrVoice::appendBarNumberCheckToVoice (
  const S_msrBarNumberCheck& barNumberCheck)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Appending barnumber check " <<
      barNumberCheck->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendBarNumberCheckToSegment (barNumberCheck);
}

void msrVoice::appendLineBreakToVoice  (
  const S_msrLineBreak& lineBreak)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Appending line break " << lineBreak->asString () <<
      " to voice " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendLineBreakToSegment (lineBreak);

  // cascade this lineBreak to the voice stanzas if any
  if (! fVoiceStanzasMap.empty ()) {
    for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      stanza->
        appendLineBreakSyllableToStanza (
          lineBreak->getInputLineNumber (),
          fVoiceLastAppendedMeasure);
    } // for
  }
}

void msrVoice::appendPageBreakToVoice (
  const S_msrPageBreak& pageBreak)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Appending page break " << pageBreak->asString () <<
      " to voice " << fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendPageBreakToSegment (pageBreak);

  // cascade this pageBreak to the voice stanzas if any
  if (! fVoiceStanzasMap.empty ()) {
    for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      stanza->
        appendPageBreakSyllableToStanza (
          pageBreak->getInputLineNumber (),
          fVoiceLastAppendedMeasure);
    } // for
  }
}

// void msrVoice::prependOtherElementToVoice (const S_msrMeasureElement& elem) {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceVoices ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Prepending other element " << elem <<
//       " to voice " <<
      // fVoiceName;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fVoiceSegment->
//     prependOtherElementToSegment (elem);
// }

// void msrVoice::appendOtherElementToVoice (const S_msrMeasureElement& elem) {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceVoices ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending other element " << elem <<
//       " to voice " <<
      // fVoiceName;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fVoiceSegment->
//     appendOtherElementToSegment (elem);
// }

S_msrMeasure msrVoice::fetchVoiceLastMeasure (
  const mfInputLineNumber& inputLineNumber) const
{
  S_msrMeasure
    result =
      fVoiceSegment->
        getSegmentLastMeasure (); // JMI 0.9.63 ZAZA

//   else { JMI 0.9.63
//     std::stringstream ss;
//
//     ss <<
//       "Attempting to fetch voice last measure in an empty measure elements list";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }

  return result;
}

S_msrMeasureElement msrVoice::fetchVoiceLastMeasureElement (
  const mfInputLineNumber& inputLineNumber) const
{
  S_msrMeasureElement result;

  S_msrMeasure
    lastMeasure =
      fetchVoiceLastMeasure (inputLineNumber);

  const std::list <S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->getMeasureElementsList ();

  if (lastMeasureElementsList.size ()) {
    result = lastMeasureElementsList.back ();
  }

  else {
    std::stringstream ss;

    ss <<
      "Attempting to fetch voice last element in an empty elements list";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  return result;
}

void msrVoice::pushRepeatOntoVoiceRepeatsStack (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat&       repeat,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Pushing repeat onto the repeats stack in voice " <<
      repeat->asShortString () <<
      ", in voice: " <<
      fVoiceName <<
      ", context: " + context <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoicePendingRepeatsStack.push_front (
    repeat);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::string
      combinedContext =
        "pushRepeatOntoVoiceRepeatsStack() + " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif // MF_TRACE_IS_ENABLED
}

S_msrRepeat msrVoice::popRepeatFromVoiceRepeatsStack (
  const mfInputLineNumber& inputLineNumber,
  const                    std::string& context)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    ! fVoicePendingRepeatsStack.empty (),
    "fVoicePendingRepeatsStack is EMPTY");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // get the inner-most repeat
  if (fVoicePendingRepeatsStack.empty ()) {
    std::stringstream ss;

    ss <<
      "Voice repeats stack is empty when attempting to pop a repeat " <<
      " in voice " <<
      fVoiceName;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  S_msrRepeat
    innerMostRepeat =
      fVoicePendingRepeatsStack.front ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Popping repeat from the stack in voice ***** " <<
      innerMostRepeat->asShortString () <<
      " in voice " <<
      fVoiceName <<
      ", context: " + context <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // pop it from repeats stack
  fVoicePendingRepeatsStack.pop_front ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::string
      combinedContext =
        "popRepeatFromVoiceRepeatsStack() context: " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif // MF_TRACE_IS_ENABLED

  return innerMostRepeat;
}

void msrVoice::displayPendingRepeatsStack (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  size_t repeatsStackSize =
    fVoicePendingRepeatsStack.size ();

  gLog <<
    std::endl <<
    ">>++++++++++++++++ Displaying voice repeats stack " << context <<
    std::endl <<
    "The repeats stack in voice " <<
    fVoiceName <<
    " contains " <<
    mfSingularOrPlural (repeatsStackSize, "element", "elements") <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  if (repeatsStackSize) {
    std::list <S_msrRepeat>::const_iterator
      iBegin = fVoicePendingRepeatsStack.begin (),
      iEnd   = fVoicePendingRepeatsStack.end (),
      i      = iBegin;

    ++gIndenter;

    int n = repeatsStackSize;
    for ( ; ; ) {
      S_msrRepeat repeat = (*i);

      gLog <<
        "v (" << n << ")" <<
        ", getInputLineNumber: " <<
        repeat->getInputLineNumber () <<
        std::endl;

      ++gIndenter;
      gLog <<
        repeat->asShortString ();
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLog << std::endl;
    } // for

    --gIndenter;
  }

  gLog <<
    std::endl <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

void msrVoice::displayVoiceRepeatsStackSummary (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  size_t repeatsStackSize =
    fVoicePendingRepeatsStack.size ();

  gLog <<
    std::endl <<
    "The voice repeats stack contains " <<
    mfSingularOrPlural (repeatsStackSize, "element", "elements") <<
    " (context: " << context << ')' <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  if (repeatsStackSize) {
    std::list <S_msrRepeat>::const_iterator
      iBegin = fVoicePendingRepeatsStack.begin (),
      iEnd   = fVoicePendingRepeatsStack.end (),
      i      = iBegin;

    ++gIndenter;

    int n = repeatsStackSize;
    for ( ; ; ) {
      S_msrRepeat repeat = (*i);

      gLog <<
        "v (" << n << ")" <<
        ", getInputLineNumber: " <<
        repeat->getInputLineNumber () <<
        std::endl;

      ++gIndenter;
      gLog <<
        repeat->asShortString () <<
        std::endl;
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLog << std::endl;
    } // for

    gLog <<
      "===============" <<
      std::endl << std::endl;

    --gIndenter;
  }
  else {
    gLog << std::endl;
  }
}

void msrVoice::displayVoiceMeasuresFlatList (
  int fieldWidth) const
{
  size_t voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();

  gLog <<
    std::setw (fieldWidth) <<
    "fVoiceMeasuresFlatList" << ": ";
  if (voiceMeasuresFlatListSize) {
    gLog <<
      mfSingularOrPlural (
        voiceMeasuresFlatListSize, "element", "elements") <<
      std::endl;

    ++gIndenter;

//     int counter = 0;
    for (S_msrMeasure measure :fVoiceMeasuresFlatList) {
//       ++counter;

      // print the measure
// #ifdef MF_TRACE_IS_ENABLED
//      if (gTraceOahGroup->getTraceMeasures ()) { // JMI 0.9.67
        gLog <<
          measure->asStringForMeasuresSlices (); // JMI ??? 0.9.70
//      }
//      else {
//        gLog << measure->getMeasureNumber ();
 //     }
// #else
//       gLog << measure->getMeasureNumber ();
// #endif // MF_TRACE_IS_ENABLED

      gLog << std::endl;
    } // for

    --gIndenter;
  }
  else {
    gLog << "[EMPTY]";
  }
  gLog << std::endl;
}

void msrVoice::moveVoiceSegmentLastAppendedMeasureToRepeatCommonPart (
  const mfInputLineNumber&     inputLineNumber,
  const S_msrRepeatCommonPart& repeatCommonPart,
  const std::string&           context)
{
  // move the voice the voice segment to repeatCommonPart
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Moving the voice last appended measure " <<
      fetchMeasureAsShortString (fVoiceLastAppendedMeasure) <<
      " in voice " <<
      fVoiceName <<
      " to repeat COMMON PART " <<
      repeatCommonPart->asShortString () <<
      " (" << context << ")" <<
      " in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fVoiceLastAppendedMeasure != nullptr,
    "fVoiceLastAppendedMeasure is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);

//   // append fVoiceLastAppendedMeasure to the repeat common part ZOULOU VIRER
  repeatCommonPart->
    appendMeasureToRepeatElement (
      inputLineNumber,
      fVoiceLastAppendedMeasure,
      context);

  // remove fVoiceLastAppendedMeasure from fVoiceSegment
  fVoiceSegment->
    removeLastMeasureFromSegment (  // ABANO
      inputLineNumber,
      "moveVoiceSegmentLastAppendedMeasureToRepeatCommonPart()");

  // DON't forget about this voice the voice segment!
//   fVoiceLastAppendedMeasure = nullptr;
}

void msrVoice::edacsacHandleRepeatStartInVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "edacsacHandleRepeatStartInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // analyze this repeat start's context
      switch (fVoicePendingRepeatsStack.size ()) {
        case 0:
          // this repeat start is at the voice-level
          // -------------------------------------
          handleVoiceLevelRepeatStart (
            inputLineNumber);
          break;

        default:
          // this repeat start belongs to a nested repeat
          // ---------------------------------------
          handleNestedRepeatStartInVoice (
            inputLineNumber);
      } // switch
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "edacsacHandleRepeatStartInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleVoiceLevelRepeatEndWithoutStart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   measureNumber,
  int                      repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat end WITHOUT start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStart() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the repeat
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat upon its end in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI 0.9.76
//
//   S_msrRepeat
//     newRepeat =
//       msrRepeat::create (
//         repeatInputLineNumber,
//         repeatTimes,
//         this);
//
//   // create the repeat common part
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating a repeat COMMON PART upon its end in voice " <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED

//   S_msrRepeatCommonPart
//     repeatCommonPart =
//       msrRepeatCommonPart::create (
//         repeatInputLineNumber,
//         newRepeat);
//
//   // register it in newRepeat
//   newRepeat->
//     setRepeatCommonPart (
//       repeatCommonPart);

  // fetch the the voice segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelRepeatEndWithoutStart() 2");

  // set voice current after repeat component phase kind
  // before edacsacFinalizeLastAppendedMeasureInVoice()
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);
/* JMI
  // finalize current measure in voice
  voiceLastSegmentLastMeasure->
    finalizeMeasure (
      inputLineNumber,
      msrMeasureRepeatContextKind::kMeasureRepeatContextCommonPartLastMeasure,
      "handleVoiceLevelRepeatEndWithoutStart() 3"
      );
*/
  // append the voice the voice segment to the new repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending the voice the voice segment in voice " <<
      fVoiceName <<
      " to the new voice-level repeat COMMON PART" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   // set newRepeat's build phase to completed
//   newRepeat->
//     setCurrentRepeatBuildPhaseKind (
//       msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStart() 7");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleVoiceLevelContainingRepeatEndWithoutStart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   measureNumber,
  int                      repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level containing repeat end without start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStart() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the repeat
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat upon its end in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

  S_msrRepeat
    newRepeat =
      msrRepeat::createWithExplicitStartKindAndUplinkAndCommonPart (
        repeatInputLineNumber,
        repeatTimes,
        msrRepeatExplicitStartKind::kRepeatExplicitStartNo,
        this);

  // append innerMostRepeat to the voice's segment
  fVoiceSegment->
    appendRepeatToSegment (newRepeat); // ZAZA

  // create the repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat COMMON PART upon its end in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   S_msrRepeatCommonPart
//     newRepeatCommonPart =
//       msrRepeatCommonPart::create (
//         repeatInputLineNumber,
//         newRepeat);
//
//   // register it in newRepeat
//   newRepeat->
//     setRepeatCommonPart (
//       newRepeatCommonPart);

//   // fetch the the voice segment's last measure
//   S_msrMeasure
//     voiceLastSegmentLastMeasure =
//       fVoiceSegment->
//         fetchLastMeasureFromSegment (
//           inputLineNumber,
//           "handleVoiceLevelContainingRepeatEndWithoutStart() 1");
//
//   // fetch the top of the repeats stack
//   if (fVoicePendingRepeatsStack.empty ()) {
//     std::stringstream ss;
//
//     ss <<
//       "repeats stack is empty when attempting to handle repeat COMMON PART start " <<
//       " in voice clone " <<
//       fVoiceName;
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   S_msrRepeat
//     repeatsStackTopRepeat =
//       fVoicePendingRepeatsStack.front ();
//
// //   // pop it from the repeats stack
// //   popRepeatFromVoiceRepeatsStack (
// //     inputLineNumber,
// //     repeatsStackTopRepeat,
// //     "handleVoiceLevelContainingRepeatEndWithoutStart() 2");
//
//   // append it to newRepeat's common part
//   newRepeatCommonPart->
//     appendRepeatToRepeatCommonPart (
//       inputLineNumber,
//       repeatsStackTopRepeat,
//       "handleVoiceLevelContainingRepeatEndWithoutStart() 3");

  // append the voice the voice segment to the new repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending the voice the voice segment in voice " <<
      fVoiceName <<
      " to the new voice-level repeat COMMON PART" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set newRepeat's build phase to completed
  newRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStart() 7");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatEndWithStart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   measureNumber,
  int                      repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat end WITH start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStart() 1");
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "This repeat end with a start is at the voice-level" <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // fetch the voice's last measure
  S_msrMeasure
    voiceLastMeasure =
      fetchVoiceLastMeasure (
        inputLineNumber);

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);

  // pop innerMostRepeat from the repeats stack
  S_msrRepeat
    innerMostRepeat =
      popRepeatFromVoiceRepeatsStack (
        inputLineNumber,
        "handleVoiceLevelRepeatEndWithStart() 2");

  // set its repeat times field
  innerMostRepeat->
    setRepeatTimes (repeatTimes);

  // create the innerMostRepeat's common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat COMMON PART upon repeat end in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   S_msrRepeatCommonPart
//     repeatCommonPart =
//       msrRepeatCommonPart::create (
//         inputLineNumber,
//         innerMostRepeat);
//
//   // register it in innerMostRepeat
//   innerMostRepeat->
//     setRepeatCommonPart (
//       repeatCommonPart);

  // move the voice last measure to the new repeat common part
  moveVoiceSegmentLastAppendedMeasureToRepeatCommonPart (
    inputLineNumber,
    innerMostRepeat->getRepeatCommonPart (),
    "handleVoiceLevelRepeatEndWithStart() 3");

  // set innerMostRepeat's build phase to completed
  innerMostRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted);

  // append innerMostRepeat to the voice's segment
  fVoiceSegment->
    appendRepeatToSegment (innerMostRepeat); // ZAZA

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStart() 5");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleNestedRepeatEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   measureNumber,
  int                      repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a nested repeat end in voice " <<
      fVoiceName <<
      ", measureNumber: " <<
      measureNumber <<
      ", repeatTimes: " <<
      repeatTimes <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() BEGIN");
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "This repeat is nested" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// fetch the voice's last measure
  S_msrMeasure
    voiceLastMeasure =
      fetchVoiceLastMeasure (
        inputLineNumber);

  // is there a measure splitting?
  if (
    voiceLastMeasure->getMeasureCurrentPositionInMeasure ().asWholeNotes ()
      ==
    voiceLastMeasure->getFullMeasureWholeNotesDuration ()
  ) {
    // this measure is incomplete and should be split
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeatsBasics ()) {
      std::stringstream ss;

      ss <<
        "Splitting measure " <<
        voiceLastMeasure->asShortString () <<
        " upon a repeat end in voice " <<
        fVoiceName <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create a new measure with the same number as the voice last measure
    // and append it to the voice,
    createAMeasureAndAppendItInVoice (
      inputLineNumber,
      333, //         previousMeasureEndInputLineNumber, 0.9.62
      measureNumber,
      msrMeasureImplicitNumberKind::kMeasureImplicitNumberKindNo);

  /* JMI
    // set it as created after a repeat
    voiceLastMeasure->
      setMeasureCreatedForARepeatKind ( // JMI 0.9.66
        msrMeasure::kMeasureCreatedForARepeatAfter);
        */
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() END");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::edacsacHandleRepeatEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&    measureNumber,
  int                       repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "edacsacHandleRepeatEndInVoice() BEGIN");
  }
#endif // MF_TRACE_IS_ENABLED

//   gLog << "*** msrVoice::edacsacHandleRepeatEndInVoice(), *this: ***" <<
//     std::endl <<
//     *this <<
//     std::endl <<
//     std::endl;


  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // analyze this repeat end's context
        switch (fVoicePendingRepeatsStack.size ()) {
          case 0:
            // this repeat is at the voice-level and has no start
            // -------------------------------------
            handleVoiceLevelRepeatEndWithoutStart (
              inputLineNumber,
              measureNumber,
              repeatTimes);
            break;

          case 1:
            {
              handleVoiceLevelRepeatEndWithStart (
                inputLineNumber,
                measureNumber,
                repeatTimes);

//               // fetch the top of the repeats stack
        //   if (fVoicePendingRepeatsStack.empty ()) {
        //     std::stringstream ss;
        //
        //     ss <<
        //       "repeats stack is empty when attempting to handle repeat COMMON PART start " <<
        //       " in voice clone " <<
        //       asShortString ();
        //
        //     msrInternalError (
        //       gServiceRunData->getInputSourceName (),
        //       fInputLineNumber,
        //       __FILE__, mfInputLineNumber (__LINE__),
        //       ss.str ());
        //   }
//
//
//               S_msrRepeat
//                 repeatsStackTopRepeat =
//                   fVoicePendingRepeatsStack.front ();
//
//               // analyze it
//               switch (repeatsStackTopRepeat->getCurrentRepeatBuildPhaseKind ()) {
//                 case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
//                   {
//                     std::stringstream ss;
//
//                     ss <<
//                       "Attempting to end repeat " <<
//                       repeatsStackTopRepeat->asShortString () <<
//                       " right after it has been created" <<
//                       " in voice " <<
//                       asShortString () <<
//                       " ";
//
//                     msrInternalError (
//                       gServiceRunData->getInputSourceName (),
//                       fInputLineNumber,
//                       __FILE__, mfInputLineNumber (__LINE__),
//                       ss.str ());
//                   }
//                   break;
//
//                 case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
//                 case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
//                   // this repeat is at the voice-level and has a start
//                   // -------------------------------------
//                   handleVoiceLevelRepeatEndWithStart (
//                     inputLineNumber,
//                     measureNumber,
//                     repeatTimes);
//                   break;
//
//                 case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
//                   // this repeat is at the voice-level, has no start
//                   // and contains repeatsStackTop
//                   // -------------------------------------
//                   handleVoiceLevelContainingRepeatEndWithoutStart (
//                     inputLineNumber,
//                     measureNumber,
//                     repeatTimes);
//                   break;
//               } // switch
            }
            break;

          default:
            // newRepeat is a nested repeat
            // ---------------------------------------
            handleNestedRepeatEndInVoice (
              inputLineNumber,
              measureNumber,
              repeatTimes);
        } // switch
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "edacsacHandleRepeatEndInVoice() END");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithImplicitStart (
  const mfInputLineNumber& inputLineNumber,
  S_msrRepeat&             currentRepeat)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    currentRepeat != nullptr,
    "currentRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat ENDING start WITHOUT explicit start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithImplicitStart() BEGIN");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  S_msrRepeatCommonPart repeatCommonPart;

  if (currentRepeat) { // JMI ???
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();
  }

  else {
    // create the repeat and stack it
    std::stringstream s1;

    s1 <<
      "Creating a voice-level repeat upon its first ENDING start WITHOUT explicit start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    S_msrRepeat
      newRepeat =
        msrRepeat::createWithCommonPartAndUplink (
          inputLineNumber,
          currentRepeat->getRepeatTimes (),
          this);

  // push repeat clone as the (new) current repeat
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeatsBasics ()) {
      std::stringstream ss;

      ss <<
        "Pushing repeat clone as the new current repeat in voice " <<
        fVoiceName;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  // push the repeat clone onto the voice's repeat descrs stack
  pushRepeatOntoVoiceRepeatsStack (
    inputLineNumber,
    newRepeat,
    "appendRepeatCloneToVoiceClone() 2");

    // create the repeat common part
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeatsBasics ()) {
      std::stringstream ss;

      ss <<
        "Creating a repeat COMMON PART upon its end in voice " <<
        fVoiceName <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // fetch last measure
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Fetching the last measure in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrMeasure
    lastMeasure =
      fVoiceSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelRepeatEndingStartWithImplicitStart() BEGIN");

  // fetch lastMeasure's elements list
  const std::list <S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (true || lastMeasureElementsList.empty ()) { // JMIJMIJMIJMIJMIJMI  // JMI 0.9.66
    // it is empty, keep it for a new voice the voice segment

    // remove last measure
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeatsBasics ()) {
      std::stringstream ss;

      ss <<
        "Removing the last measure in voice " <<
        fVoiceName <<
        " (voice level ending without explicit start)" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // move voice the voice segment into the repeat common part
    std::stringstream s2;

    s2 <<
      "Moving the voice the voice segment to the repeat COMMON PART in voice " <<
      fVoiceName <<
      "handleVoiceLevelRepeatEndingStartWithImplicitStart()";

    // move the voice last measure to the new repeat common part
    moveVoiceSegmentLastAppendedMeasureToRepeatCommonPart (
      inputLineNumber,
      currentRepeat->getRepeatCommonPart (),
      s2.str ());
  }

  else {
    // it is not empty
  }

  // set voiceLevelRepeat's build phase
//   newRepeat->
//     setCurrentRepeatBuildPhaseKind (
//       msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithImplicitStart() END");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithExplicitStart (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat ENDING start WITH explicit start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStart() BEGIN");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // fetch currentRepeat
  if (fVoicePendingRepeatsStack.empty ()) {
    std::stringstream ss;

    ss <<
      "Voice repeats stack is empty when attempting to handle voice level repeat ENDING start WITH explicit start in voice " <<
      fVoiceName;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // get currentRepeat's common part
  S_msrRepeatCommonPart
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();

  // fetch last measure of the voice segment
  S_msrMeasure
    lastMeasure =
      fVoiceSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelRepeatEndingStartWithExplicitStart() 2");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Fetching the last measure of the the voice segment in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      ", it is:" <<
      std::endl <<
      lastMeasure->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch lastMeasure's elements list
  const std::list <S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (true || lastMeasureElementsList.empty ()) { // JMIJMIJMIJMIJMIJMI  // JMI 0.9.66
    // it is empty, keep it for a new voice the voice segment

    // remove last measure
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeatsBasics ()) {
      std::stringstream ss;

      ss <<
        "Removing the last measure in voice " <<
        fVoiceName <<
          " (voice level ending with explicit start)" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

//     S_msrMeasure
//       dummyMeasure =
//         fVoiceSegment->
//           removeLastMeasureFromSegment (
//             inputLineNumber,
//             "handleVoiceLevelRepeatEndingStartWithImplicitStart() 3");

    // move voice the voice segment into the repeat common part
    std::stringstream s2;

    s2 <<
      "Moving the voice the voice segment to the repeat COMMON PART in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    // move the voice last measure to the new repeat common part
    moveVoiceSegmentLastAppendedMeasureToRepeatCommonPart (
      inputLineNumber,
      currentRepeat->getRepeatCommonPart (),
      s2.str ());
  }

  else {
    // it is not empty
  }

  // set currentRepeat's build phase
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStart() END");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatStart (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStart() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

// ZOULOU ZOULOU

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeat
    newRepeat =
      msrRepeat::createWithExplicitStartKindAndUplinkAndCommonPartAndSegment (
//       msrRepeat::create ( // ZOULOU ZOULOU
        inputLineNumber,
        2, // repeatTimes, default value JMI
        msrRepeatExplicitStartKind::kRepeatExplicitStartNo,
        this);

  // push it onto the voice's repeats stack
  pushRepeatOntoVoiceRepeatsStack (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelRepeatStart()");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStart()");
  }
#endif // MF_TRACE_IS_ENABLED


// ZOULOU ZOULOU

//   // create the repeat common part
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating a repeat COMMON PART upon its start in voice " <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   S_msrRepeatCommonPart
//     repeatCommonPart =
//       msrRepeatCommonPart::createAsWellAsItsSegment (
//         inputLineNumber,
//         newRepeat);
//
//   // register it in newRepeat
//   newRepeat->
//     setRepeatCommonPart (
//       repeatCommonPart);

//   // set newRepeat as having an explicit start // ZOULOU
//   newRepeat->
//     setRepeatExplicitStartKind (
//       msrRepeatExplicitStartKind::kRepeatExplicitStartYes);


  // append innerMostRepeat to the voice's segment
  fVoiceSegment->
    appendRepeatToSegment (newRepeat); // ZAZA

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStart() 11");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleNestedRepeatStartInVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a nested repeat start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatStartInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::nestContentsIntoNewRepeatInVoice (
  const mfInputLineNumber& inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
        // are there measures in the voice the voice segment?
        if (! fVoiceSegment->getSegmentElementsList ().empty ()) {
          // finalize current measure in voice
          edacsacFinalizeLastAppendedMeasureInVoice (
            inputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceRepeatsDetails ()) {
            displayPendingRepeatsStack (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 1");
          }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceRepeatsDetails ()) {
            displayPendingRepeatsStack (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 2");
          }
#endif // MF_TRACE_IS_ENABLED
        }
      break;
  } // switch
}

void msrVoice::handleNestedRepeatEndingStartInVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a nested repeat ENDING start voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleNestedRepeatEndingStartInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::edacsacHandleRepeatEndingStartInVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Edacsaccing handling a repeat ENDING start in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "edacsacHandleRepeatEndingStartInVoice() BEGIN");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        ++gIndenter;

        // analyze this repeat end's context
        switch (fVoicePendingRepeatsStack.size ()) {
          // not the right criterion JMI 0.9.76 ZAZA
          case 0:
            // this the first ending of a voice-level repeat without start
            // -------------------------------------
//             handleVoiceLevelRepeatEndingStartWithImplicitStart (
            handleVoiceLevelRepeatEndingStartWithExplicitStart (
              inputLineNumber); // set later in XXX() JMI 0.9.66
            break;

          case 1:
            {
              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatsStack.front ();

              switch (currentRepeat->getRepeatExplicitStartKind ()) {
                case msrRepeatExplicitStartKind::kRepeatExplicitStartUnknown:
                  {
                    std::stringstream ss;

                    ss <<
                      "The current repeat at the top of the repeats stack "
                      "is of unknown RepeatExplicitStartKind"; // ZAZA

                    msrInternalError (
                      gServiceRunData->getInputSourceName (),
                      inputLineNumber,
                      __FILE__, mfInputLineNumber (__LINE__),
                      ss.str ());
                  }
                  break;

                case msrRepeatExplicitStartKind::kRepeatExplicitStartNo:
                  // this the first ending of a voice-level repeat without a start // ZAZA
                  // -------------------------------------
                  handleVoiceLevelRepeatEndingStartWithImplicitStart (
                    inputLineNumber,
                    currentRepeat);
                  break;

                case msrRepeatExplicitStartKind::kRepeatExplicitStartYes:
                  // this the first ending of a voice-level repeat with a start // ZAZA
                  // -------------------------------------
                  handleVoiceLevelRepeatEndingStartWithExplicitStart (
                    inputLineNumber);
                  break;
              } // switch
            }
            break;

          default:
            // newRepeat is a nested repeat
            // ---------------------------------------
            handleNestedRepeatEndingStartInVoice (
              inputLineNumber);
        } // switch

        --gIndenter;
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "edacsacHandleRepeatEndingStartInVoice() END");
  }
#endif // MF_TRACE_IS_ENABLED
}

// S_msrSegment msrVoice::edacsacHandleRepeatEndingStartInVoiceClone (
//   const mfInputLineNumber& inputLineNumber,
//   msrRepeatEndingKind      repeatEndingKind,
//   const std::string&       repeatEndingNumber) // a string, because if may be "1, 2" for example
// {
//   S_msrSegment result;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "edacsacHandleRepeatEndingStartInVoiceClone() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   switch (fVoiceKind) {
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       {
//         // handle the repeat ending start
//         ++gIndenter;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//           std::stringstream ss;
//
//           ss <<
//             "Handling a repeat ENDING upon its start in voice clone \"" <<
//             fVoiceName <<
//                   ", line " << inputLineNumber;
//
//           gWaeHandler->waeTrace (
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str ());
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         // is there a current repeat?
//         switch (fVoicePendingRepeatsStack.size ()) {
//           case 0:
//             {
//               std::stringstream ss;
//
//               ss <<
//                 "repeats stack is empty when attempting to handle a repeat ENDING start in voice clone " <<
//                 asShortString ();
//
//               msrInternalError (
//                 gServiceRunData->getInputSourceName (),
//                 fInputLineNumber,
//                 __FILE__, mfInputLineNumber (__LINE__),
//                 ss.str ());
//             }
//           break;
//
//           case 1:
//             {
//               // this repeat ending is part of a voice-level repeat
//
//               // fetch currentRepeat
//               S_msrRepeat
//                 currentRepeat =
//                   fVoicePendingRepeatsStack.front ();
//
//               // create a repeat ending
// #ifdef MF_TRACE_IS_ENABLED
//               if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//                 std::stringstream ss;
//
//                 ss <<
//                   "Creating a " <<
//                   msrRepeatEndingKindAsStringForTrace (repeatEndingKind) <<
//                   " repeat ENDING in current repeat in voice clone " <<
//                   fVoiceName <<
//                   ", line " << inputLineNumber;
//
//                 gWaeHandler->waeTrace (
//                   __FILE__, mfInputLineNumber (__LINE__),
//                   ss.str ());
//               }
// #endif // MF_TRACE_IS_ENABLED
//
//               S_msrRepeatEnding
//                 repeatEnding =
//                   msrRepeatEnding::create (
//                     inputLineNumber,
//                     repeatEndingNumber,
//                     repeatEndingKind,
//                     currentRepeat);
//
//               // add the repeat ending to the voice current repeat
// #ifdef MF_TRACE_IS_ENABLED
//               if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//                 std::stringstream ss;
//
//                 ss <<
//                   "Appending a " <<
//                  msrRepeatEndingKindAsStringForTrace (repeatEndingKind) <<
//                   " repeat ENDING to current repeat in voice " <<
//                   fVoiceName;
//
//                 gWaeHandler->waeTrace (
//                   __FILE__, mfInputLineNumber (__LINE__),
//                   ss.str ());
//               }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//               if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//                 displayPendingRepeatsStack (
//                   inputLineNumber,
//                   "before adding a HOOKED repeat ENDING to current repeat");
//               }
// #endif // MF_TRACE_IS_ENABLED
//
//               currentRepeat->
//                 addRepeatEndingToRepeat (
//                   inputLineNumber,
//                   repeatEnding);
//
//               // set currentRepeat's build phase
//               currentRepeat->
//                 setCurrentRepeatBuildPhaseKind (
//                   msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings);
//
//               // the new current segment has just been created in repeatCommonPart
//               result =
//                 repeatEnding->
//                   getRepeatElementSegment ();
//             }
//           break;
//
//           default:
//             {
//               // the current repeat is nested
//
//               // JMI ???
//
//               // move voice the voice segment into the repeat common part
// #ifdef MF_TRACE_IS_ENABLED
//               if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//                 std::stringstream ss;
//
//                 ss <<
//                   "Moving the voice the voice segment to the repeat COMMON PART in voice clone " <<
//                   fVoiceName <<
//                   ", line " << inputLineNumber;
//
//                 gWaeHandler->waeTrace (
//                   __FILE__, mfInputLineNumber (__LINE__),
//                   ss.str ());
//               }
// #endif // MF_TRACE_IS_ENABLED
//             }
//         } // switch
//
//         --gIndenter;
//       }
//       break;
//   } // switch
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "edacsacHandleRepeatEndingStartInVoiceClone() 2");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return result;
// }

// void msrVoice::handleSegmentCloneEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrSegment&      segmentClone)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceSegments ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling segment " <<
//       segmentClone->asShortString () <<
//       " in voice clone \"" <<
//       fVoiceName <<
//       "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceSegmentsDetails ()) {
//     displayVoice (
//       inputLineNumber,
//       "handleSegmentCloneEndInVoiceClone() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (fVoicePendingMeasureRepeat) {
//     // segmentClone belongs to a measure repeat
//
//     switch (
//       fVoicePendingMeasureRepeat->getCurrentMeasureRepeatBuildPhaseKind ()
//     ) {
//       case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseJustCreated:
//         {
//           std::stringstream ss;
//
//           ss <<
//             "segment " <<
//             segmentClone->asShortString () <<
//             "'cannot be added to a just created measure repeat";
//
//           msrError (
//             gServiceRunData->getInputSourceName (),
//             inputLineNumber,
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str ());
//         }
//         break;
//
//       case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInPattern:
//         {
//           // get fVoicePendingMeasureRepeat's pattern
//           S_msrMeasureRepeatPattern
//             measureRepeatPattern =
//               fVoicePendingMeasureRepeat->
//                 getMeasureRepeatPattern ();
//
//           // set segmentClone as the pattern's segment
//           measureRepeatPattern->
//             setMeasureRepeatPatternSegment (
//         // JMI      inputLineNumber,
//               segmentClone);
//         }
//         break;
//
//       case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInReplicas:
//         {
//           // get fVoicePendingMeasureRepeat's replicas
//           S_msrMeasureRepeatReplicas
//             measureRepeatReplicas =
//               fVoicePendingMeasureRepeat->
//                 getMeasureRepeatReplicas ();
//
//           // set segmentClone as the replicas's segment
//           measureRepeatReplicas->
//             setMeasureRepeatReplicasSegment (
//         // JMI      inputLineNumber,
//               segmentClone);
//         }
//         break;
//
//       case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseCompleted:
//         {
//           std::stringstream ss;
//
//           ss <<
//             "segment " <<
//             segmentClone->asShortString () <<
//             "'cannot be added to a completed measure repeat";
//
//           msrError (
//             gServiceRunData->getInputSourceName (),
//             inputLineNumber,
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str ());
//         }
//         break;
//     } // switch
//   }
//
//   else if (! fVoicePendingRepeatsStack.empty ()) {
//     // segmentClone belongs to a repeat
//
//     // append segment to whichever part of the repeat is adequate
//     S_msrRepeat
//       currentRepeat =
//         fVoicePendingRepeatsStack.front ();
//
// //     currentRepeat->
// //       appendSegmentToRepeat (
// //         inputLineNumber,
// //         segmentClone,
// //         "handleSegmentCloneEndInVoiceClone() 2");
//   }
//
//   else {
//     // segmentClone is a voice-level segment
//   }
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceSegmentsDetails ()) {
//     displayVoice (
//       inputLineNumber,
//       "handleSegmentCloneEndInVoiceClone() 3");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   --gIndenter;
// }

/* JMI
void msrVoice::finalizeRepeatEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   measureNumber,
  int                      repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "finalizeRepeatEndInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // finalize current measure in voice
        edacsacFinalizeLastAppendedMeasureInVoice (
          inputLineNumber);

        // fetch the repeat
        if (fVoicePendingRepeatsStack.empty ()) {
          std::stringstream ss;

          ss <<
            "repeats stack is empty when attempting to finalize a repeat in voice " <<
            fVoiceName;

          msrInternalError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }

        S_msrRepeat
          repeat =
            fVoicePendingRepeatsStack.front ();

        // get repeat's common part
        S_msrRepeatCommonPart
          repeatCommonPart =
            repeat->
              getRepeatCommonPart ();

        // is there another repeat to nest into this repeat?
        if (fVoicePendingRepeatsStack.empty ()) {
          // yes, this repeat contains a nested repeat
        }

        else {
          // no, this repeat is at the voice-level
        }
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "finalizeRepeatEndInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}
*/

void msrVoice::edacsacCreateAMeasureRepeatAndAppendItToVoice ( // cascade bottom
  const mfInputLineNumber& inputLineNumber,
  int                      measureRepeatMeasuresNumber,
  int                      measureRepeatSlashesNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Edacsaccing creating a measure repeat in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // this occurs after an empty measure has just been created,
  // hence the repeated measure/measures is/are the
  // measureRepeatMeasuresNumber measures preceding the last one

//   int
//     lastSegmentMeasuresNumber =
//       fVoiceSegment->
//         getSegmentElementsList ().size ();
//     availableMeasuresNumber =
//       lastSegmentMeasuresNumber - 1;

//   if (
//     availableMeasuresNumber < measureRepeatMeasuresNumber) {
//     std::stringstream ss;
//
//     ss <<
//       "Attempting to create a measure repeat with " <<
//       measureRepeatMeasuresNumber <<
//       " measures while current the voice segment only has " <<
//       availableMeasuresNumber <<
//       " available";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }

  // grab the just created last measure from the voice,
  // (i.e. the one containing:
  //   <measure-repeat ... type="start">2</measure-repeat>)
  // which is the first replica measure
//   S_msrMeasure
//     firstReplicaMeasure =
//       removeLastMeasureFromVoice (
//         inputLineNumber);

  // create the measure repeat repeated segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating the measure repeat repeated segment in voice " <<
      fVoiceName <<
      ", fVoiceNumber: " << fVoiceNumber <<
      ", in staff \"" <<
      fVoiceUpLinkToStaff->getStaffPathLikeName () <<
      " line " << fInputLineNumber <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrSegment
    repeatedSegment =
      msrSegment::create (
        inputLineNumber,
        msrSegmentKind::kSegmentKindInMeasureRepeat,
        this);

//   // remove the repeated measure(s) for the the voice segment
//   // and prepend them to the repeated segment
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing the last " <<
//       mfSingularOrPlural (
//         measureRepeatMeasuresNumber, "measure", "measures") <<
//       " (to be repeated) from voice " <<
//       fVoiceName;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED

//   for (int i = 0; i < measureRepeatMeasuresNumber; ++i) {
//     S_msrMeasure
//       lastMeasure =
//         removeLastMeasureFromVoice (
//           inputLineNumber);
//
//     repeatedSegment->
//       prependMeasureToSegment (
//         lastMeasure);
//   } // for

  // create the measure repeat
  if (fVoicePendingMeasureRepeat) {
    std::stringstream ss;

    ss <<
      "Attempting to create a measure repeat while another one is pending";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  fVoicePendingMeasureRepeat =
    msrMeasureRepeat::create (
      inputLineNumber,
      measureRepeatMeasuresNumber,
      measureRepeatSlashesNumber);

  fVoicePendingMeasureRepeat->
    setUpLinkToMeasureRepeatToVoice (this);

  // create the measure repeat pattern
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a measure repeat pattern in voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrMeasureRepeatPattern
    measureRepeatPattern =
      msrMeasureRepeatPattern::create (
        inputLineNumber,
        fVoicePendingMeasureRepeat);

  // set the repeated segment as the measure repeat pattern segment
  measureRepeatPattern->
    setMeasureRepeatPatternSegment (
      repeatedSegment);

  // set the measure repeat pattern
  fVoicePendingMeasureRepeat->
    setMeasureRepeatPattern (
      measureRepeatPattern);

  // keep the measure repeat pending

//   // print resulting voice contents
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
//     displayVoiceMeasureRepeatAndVoice (
//       inputLineNumber,
//       "edacsacCreateAMeasureRepeatAndAppendItToVoice() 3");
//   }
// #endif // MF_TRACE_IS_ENABLED
}

void msrVoice::cascadeAppendMultiMeasureRestToVoice (
  const mfInputLineNumber&        inputLineNumber,
  const S_msrMultiMeasureRest& multiMeasureRest)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending multi-measure rest " <<
      multiMeasureRest->asShortString () <<
      " to voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // analyze this multi-measure rests's context
  switch (fVoicePendingRepeatsStack.size ()) {
    case 0:
      // this multi-measure rests is at the voice-level
      // -------------------------------------

      // append multiMeasureRest to it
      ++gIndenter;
      fVoiceSegment->
        edacsacAppendMultiMeasureRestToSegment (
          multiMeasureRest);
      --gIndenter;
      break;

    default:
      // this multi-measure rests is inside a repeat
      // -------------------------------------
      S_msrRepeat
        currentRepeat =
          fVoicePendingRepeatsStack.front ();

//       currentRepeat->
//         cascadeAppendMultiMeasureRestToRepeat (
//           inputLineNumber,
//           multiMeasureRest,
//           "cascadeAppendMultiMeasureRestToVoice() 2");
  } // switch
}

void msrVoice::appendMeasureRepeatToVoice (
  const mfInputLineNumber&  inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measure repeat " <<
      measureRepeat->asShortString () <<
      " to voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
//     displayVoiceMeasureRepeatAndVoice (
//       inputLineNumber,
//       "appendMeasureRepeatToVoice() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED

  // analyze this measure repeats's context
  switch (fVoicePendingRepeatsStack.size ()) {
    case 0:
      // this measure repeat is at the voice-level
      // -------------------------------------
      appendMeasureRepeatToVoiceElementsList (
        measureRepeat);
      break;

    default:
      // this measure repeat is inside a repeat
      // -------------------------------------
      S_msrRepeat
        currentRepeat =
          fVoicePendingRepeatsStack.front ();

      currentRepeat->
        appendMeasureRepeatToRepeat (
          inputLineNumber,
          measureRepeat,
          "appendMeasureRepeatToVoice() 2");
  } // switch

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
//     displayVoiceMeasureRepeatAndVoice (
//       inputLineNumber,
//       "appendMeasureRepeatToVoice() 3");
//   }
// #endif // MF_TRACE_IS_ENABLED
}

void msrVoice::appendPendingMeasureRepeatToVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    std::stringstream ss;

    ss <<
      "Appending pending measure repeat to voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // does the current measure repeat exist?
  if (! fVoicePendingMeasureRepeat) {
    std::stringstream ss;

    ss <<
      "Attempting to append a pending measure repeat which doesn't exist";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // fetch the the voice segment's measure elements list
  std::list <S_msrSegmentElement>
    voiceLastSegmentMeasureList =
      fVoiceSegment->
        getSegmentElementsList ();

  // grab the just created last measure
  // in the the voice segment's measure elements list,
  // (i.e. the one containing:
  //   <measure-repeat type="stop"/>)
  // which is the next measure after the measure repeat
  if (voiceLastSegmentMeasureList.empty ()) {
    std::stringstream ss;

    ss <<
      "Attempting to grab first measure of voice the voice segment, that contains none";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  S_msrMeasure
    nextMeasureAfterMeasureRepeat =
      fVoiceSegment->
        getSegmentLastMeasure (); // JMI 0.9.63

// BOFBOFBOF JMI
  // remove the next measure from the the voice segment's measure elements list
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Removing last measure in the voice segment in measures list " <<
      "in voice " <<
      fVoiceName <<
      ", nextMeasureAfterMeasureRepeat: " <<
      nextMeasureAfterMeasureRepeat->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
}
#endif // MF_TRACE_IS_ENABLED

  voiceLastSegmentMeasureList.pop_back ();

  // create the measure repeat replicas contents
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a measure repeat replicas contents for voice " <<
      fVoiceName <<
      "\" is:";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrMeasureRepeatReplicas
    measureRepeatReplicas =
      msrMeasureRepeatReplicas::create (
        inputLineNumber,
        fVoicePendingMeasureRepeat);

  // set the voice the voice segment as the measure repeat replicas segment
  measureRepeatReplicas->
    setMeasureRepeatReplicasSegment (
      fVoiceSegment);

  fVoicePendingMeasureRepeat->
    setMeasureRepeatReplicas (
      measureRepeatReplicas);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting pending measure repeat replicas segment in voice " <<
      fVoiceName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoicePendingMeasureRepeat->
    setMeasureRepeatReplicas (
      measureRepeatReplicas);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting measure repeat segment to voice the voice segment for voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoicePendingMeasureRepeat->
    getMeasureRepeatReplicas ()->
      setMeasureRepeatReplicasSegment (
        fVoiceSegment);

  // append pending measure repeat to the voice
  appendMeasureRepeatToVoice (
    inputLineNumber,
    fVoicePendingMeasureRepeat);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLog <<
      "Appending pending measure repeat to voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::createMeasureRepeatAndAppendItToVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  int                      measureRepeatMeasuresNumber,
  int                      measureRepeatSlashesNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLog <<
      "Creating measure repeat and appending it to voice clone \"" <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // does the pending measure repeat exist?
        if (fVoicePendingMeasureRepeat) {
          std::stringstream ss;

          ss <<
            "Attempting to create a measure repeat while another one is pending";

          msrInternalError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }

        // create the measure repeat
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Creating and appending a measure repeat in voice " <<
            fVoiceName <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoicePendingMeasureRepeat =
          msrMeasureRepeat::create (
            inputLineNumber,
            measureRepeatMeasuresNumber,
            measureRepeatSlashesNumber);

        fVoicePendingMeasureRepeat->
          setUpLinkToMeasureRepeatToVoice (this);

        // create a measure repeat pattern from current the voice segment
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Creating a measure repeat pattern from current the voice segment in voice " <<
            fVoiceName <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        S_msrMeasureRepeatPattern
          measureRepeatPattern =
            msrMeasureRepeatPattern::create (
              inputLineNumber,
              fVoicePendingMeasureRepeat);

        // set current the voice segment as the measure repeat pattern segment
        measureRepeatPattern->
          setMeasureRepeatPatternSegment (
            fVoiceSegment);

        // set the measure repeat pattern
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Setting repeat COMMON PART in voice " <<
            fVoiceName;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoicePendingMeasureRepeat->
          setMeasureRepeatPattern (
            measureRepeatPattern);

        // append the measure repeat to the list of initial elements
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Appending measure repeat to voice " <<
            fVoiceName;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // append pending measure repeat to the voice
        appendMeasureRepeatToVoice (
          inputLineNumber,
          fVoicePendingMeasureRepeat);
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLog <<
      "Creating measure repeat and appending it to voice clone \"" <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::setVoiceContainsMultiMeasureRests (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      ", contains multi-measure rests";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceContainsMultiMeasureRests = true;
}

void msrVoice::setVoiceContainsMeasureRepeats (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      ", contains measure repeats";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceContainsMeasureRepeats = true;
}

void msrVoice::createAMultiMeasureRestAndAppendItToVoice (
  const mfInputLineNumber& inputLineNumber,
  int                      multiMeasureRestMeasuresNumber,
  int                      multiMeasureRestSlashesNumber,
  msrUseSymbolsKind        multiMeasureRestUseSymbolsKind)
{
  // create a multi-measure rests
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending a multi-measure rest for " <<
      mfSingularOrPlural (
        multiMeasureRestMeasuresNumber, "measure", "measures") <<
      " to voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        ++gIndenter;

        // this method is called after the measure containing:
        //   <multiple-rest ... type="start">NNN</multiple-rest>)
        // which is the first measure rest,
        // has been appended to the voice

//         // grab the just created last measure from the voice
//         S_msrMeasure
//           firstRestMeasure =
//             removeLastMeasureFromVoice (
//               inputLineNumber);

        // create the multi-measure rests
        if (fVoiceCurrentMultiMeasureRest) {
          std::stringstream ss;

          ss <<
            "Attempting to create a multi-measure rests while another one is pending";

          msrInternalWarning (
//           msrInternalError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
//             __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Creating a multi-measure rest for " <<
            mfSingularOrPlural (
              multiMeasureRestMeasuresNumber, "measure", "measures") <<
            " in voice " <<
            fVoiceName <<
                  ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoiceCurrentMultiMeasureRest =
          msrMultiMeasureRest::create (
            inputLineNumber,
            multiMeasureRestMeasuresNumber,
            multiMeasureRestSlashesNumber,
            multiMeasureRestUseSymbolsKind);

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Appending multi-measure rest " <<
            fVoiceCurrentMultiMeasureRest->asString () <<
            " to the the voice segment of voice " <<
            fVoiceName <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoiceSegment->
          edacsacAppendMultiMeasureRestToSegment (
            fVoiceCurrentMultiMeasureRest);

//         // append firstRestMeasure to fVoiceCurrentMultiMeasureRest
//         fVoiceCurrentMultiMeasureRest->
//           appendMeasureToMultiMeasureRest (
//             firstRestMeasure);

         // remember fVoiceCurrentMultiMeasureRest for later next measure number setting
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Registering multi-measure rests " <<
            fVoiceCurrentMultiMeasureRest->asString () <<
            " as waiting for its next measure number" <<
            " in voice " <<
            fVoiceName <<
            "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoiceMultiMeasureRestsWaitingForItsNextMeasureNumber =
          fVoiceCurrentMultiMeasureRest;

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Setting fVoiceRemainingMultiMeasureRests to " <<
            multiMeasureRestMeasuresNumber <<
            " in voice " <<
            fVoiceName <<
            "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        fVoiceRemainingMultiMeasureRests =
          multiMeasureRestMeasuresNumber;

//         // this voice contails multi-measure rests
//         this->setVoiceContainsMultiMeasureRests (
//           inputLineNumber); // JMI 0.9.67
//
//         // keep the multi-measure rests pending

        --gIndenter;
      }
      break;
  } // switch

  fOnGoingMultiMeasureRest = true;
}

void msrVoice::replicateLastAppendedMeasureInVoice (
  const mfInputLineNumber& inputLineNumber,
  int                      replicatasNumber)
{
  mfMeasureNumber
    voiceLastAppendedMeasureMeasureNumber =
      fVoiceLastAppendedMeasure->
        getMeasureNumber ();

  for (int i = 1; i <= replicatasNumber; ++i) {
    // create a clone of the last appended measure
    S_msrMeasure
      lastAppendedMeasureClone =
        fVoiceLastAppendedMeasure->
          createMeasureDeepClone (
            fVoiceSegment);

    // change its contents
    lastAppendedMeasureClone->
      setMeasureNumber (
        mfMeasureNumberAsString (voiceLastAppendedMeasureMeasureNumber) +
          '.' + // JMI 0.9.75
          std::to_string (i) +
          " (replicated)");

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
      std::stringstream ss;

      ss <<
        "Replicating last appended measure " <<
        fVoiceLastAppendedMeasure->getMeasureNumber () <<
        " as measure " <<
        lastAppendedMeasureClone->getMeasureNumber () <<
        " in voice " <<
        fVoiceName <<
          std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // register its whole notes wholeNotes
    fetchVoiceUpLinkToPart ()->
      registerOrdinalMeasureNumberWholeNotes (
        inputLineNumber,
        lastAppendedMeasureClone->
          getMeasureOrdinalNumberInVoice (),
        lastAppendedMeasureClone->
          getFullMeasureWholeNotesDuration ()); // JMI

    // append it to the voice the voice segment
    fVoiceSegment->
      appendMeasureToSegment (lastAppendedMeasureClone);

    //   // update fVoiceLastAppendedMeasure // JMI
    //   fVoiceLastAppendedMeasure->
    //     setNextMeasureNumber (
    //       );
  } // for
}

void msrVoice::appendEmptyMeasuresToVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   previousMeasureNumber,
  int                      emptyMeasuresNumber)
{
  // create a multi-measure rests
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Adding " <<
      mfSingularOrPlural (
        emptyMeasuresNumber, "empty measure", "empty measures") <<
      " to voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get the empty measure whole notes wholeNotes
  // JMI maybe not OK if first measure such as after a repeat segment???
  mfWholeNotes
    fullMeasureWholeNotesDuration; // JMI ??? = ??? 0.9.72
//      =
//       fullMeasureWholeNotesDuration->
//         getFullMeasureWholeNotesDuration ();

  for (int i = 1; i <= emptyMeasuresNumber; ++i) {
    // create a measure
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating an empty measure and appending it to segment " <<
      asString () <<
      ", in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string measureNumber = std::to_string (i) + " (added)"; // JMI

  S_msrMeasure
    emptyMeasure =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        fVoiceSegment);

  // set emptyMeasure's ordinal number
  emptyMeasure->
    setMeasureOrdinalNumberInVoice (
      this->
        incrementVoiceCurrentMeasureOrdinalNumber ());

  // set its whole notes wholeNotes JMI 0.9.70
//   emptyMeasure->
//     setFullMeasureWholeNotesDuration (
//       fullMeasureWholeNotesDuration);

  // create a rest the whole empty measure long
  S_msrNote
    wholeMeasureRestNote =
      msrNote::createRestNote (
        inputLineNumber,
        measureNumber,
        fullMeasureWholeNotesDuration, // soundingWholeNotes
        fullMeasureWholeNotesDuration, // displayWholeNotes
        0); // dotsNumber

   wholeMeasureRestNote->
    setNoteOccupiesAFullMeasure ();

  // append it to emptyMeasure
  mfPositionInMeasure
    partCurrentDrawingPositionInMeasure; // needs to be supplied ??? JMI 0.9.72

  emptyMeasure->
    appendNoteToMeasureAtPosition (
      wholeMeasureRestNote,
      partCurrentDrawingPositionInMeasure);

  // append emptyMeasure to the voice the voice segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating an empty measure " << measureNumber <<
      " and appending it to segment " << asString () <<
      ", in voice " <<
      fVoiceName  <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendMeasureToSegment (emptyMeasure);


//   // change the measure number
//   fVoiceLastAppendedMeasure->
//     setMeasureNumber (
//       fVoiceLastAppendedMeasure->
//         getMeasureNumber () + " added"); // JMI BLARK 0.9.61

  } //for
}

void msrVoice::appendPendingMultiMeasureRestsToVoice (
    const mfInputLineNumber& inputLineNumber)
{
  // a multi-measure rests is a voice element,
  // and can be voice-level as well as part of a repeat

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRestsDetails ()) {
      if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
        std::stringstream ss;

        ss <<
          "Appending current multi-measure rests " <<
          fVoiceCurrentMultiMeasureRest->asShortString () <<
          " to voice " <<
          fVoiceName <<
          ", line " << inputLineNumber;

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
//         // does the current multi-measure rests exist?
//         if (! fVoiceCurrentMultiMeasureRest) {
//           std::stringstream ss;
//
//           ss <<
//             "Attempting to append a current multi-measure rests which doesn't exist";
//
//           msrInternalError (
//             gServiceRunData->getInputSourceName (),
//             inputLineNumber,
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str ());
//         }

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Appending current multi-measure rests " <<
            fVoiceCurrentMultiMeasureRest->asShortString () <<
            " to the the voice segment of voice " <<
            fVoiceName <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
//        fVoiceSegment->
          // JMI 0.9.67

        // set current multi-measure rests last measure purist number
        fVoiceCurrentMultiMeasureRest->
          setLastMeasurePuristMeasureNumber (
            inputLineNumber,
            fVoiceCurrentMeasurePuristNumber);

        // append current multi-measure rests to the voice
        cascadeAppendMultiMeasureRestToVoice (
          inputLineNumber,
          fVoiceCurrentMultiMeasureRest);

        // forget about this current multi-measure rests
        fVoiceCurrentMultiMeasureRest = nullptr;
      }
      break;
  } // switch
}

// void msrVoice::handleMultiMeasureRestsStartInVoiceClone (
//   const mfInputLineNumber&        inputLineNumber,
//   const S_msrMultiMeasureRest& multiMeasureRest)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling multi-measure rests start in voice clone \"" <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   switch (fVoiceKind) {
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//
//       // is there already a current multi-measure rests in this voice?
//       if (fVoiceCurrentMultiMeasureRest) {
//         std::stringstream ss;
//
//         ss <<
//           "current voice multi-measure rests is not null when attempting to handle multi-measure rests start in voice clone " <<
//           asShortString ();
//
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
//
//       // create the multi-measure rests clone and register it
//       fVoiceCurrentMultiMeasureRest =
//         multiMeasureRest->
//           createMultiMeasureRestNewbornClone (
//             fVoiceSegment); // JMI ??? JMI 0.9.67
//
//       // this voice contails multi-measure rests
//       this->setVoiceContainsMultiMeasureRests (
//         inputLineNumber);
//
//       break;
//   } // switch
//
//   --gIndenter;
// }

// void msrVoice::handleMultiMeasureRestsEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling multi-measure rests end in voice clone \"" <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   switch (fVoiceKind) {
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//
//       // is there a current multi-measure rests in this voice?
//       if (! fVoiceCurrentMultiMeasureRest) {
//         std::stringstream ss;
//
//         ss <<
//           "current voice multi-measure rests is NULL when attempting to handle multi-measure rests end in voice clone " <<
//           asShortString ();
//
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
//
//       // set current multi-measure rests last measure purist number
//       fVoiceCurrentMultiMeasureRest->
//         setLastMeasurePuristMeasureNumber (
//           inputLineNumber,
//           fVoiceCurrentMeasurePuristNumber);
//
//       // forget about fVoiceCurrentMultiMeasureRest
//       fVoiceCurrentMultiMeasureRest = nullptr;
//
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTraceMultiMeasureRestsDetails ()) {
//         displayVoice (
//           inputLineNumber,
//           "handleMultiMeasureRestsEndInVoiceClone() 3");
//       }
// #endif // MF_TRACE_IS_ENABLED
//     break;
//   } // switch
//
//   --gIndenter;
// }

void msrVoice::appendMultiMeasureRestCloneToVoiceClone (
  const mfInputLineNumber&        inputLineNumber,
  const S_msrMultiMeasureRest& multiMeasureRestClone)
{
  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    multiMeasureRestClone != nullptr,
    "multiMeasureRestClone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Appending multi-measure rest clone " <<
            multiMeasureRestClone->asString () <<
            " to voice clone \"" <<
            fVoiceName <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // is multi-measure rests nested in a repeat?
        if (! fVoicePendingRepeatsStack.empty ()) {
          // yes

          S_msrRepeat
            currentRepeat =
              fVoicePendingRepeatsStack.front ();

//           // grab the multi-measure rests segment, i.e. the voice's the voice segment // JMI ??? ZAZA
        }

        else {
          // no
          // JMI ???
        }

        // append the multi-measure rests clone to the voice
        cascadeAppendMultiMeasureRestToVoice (
          inputLineNumber,
          multiMeasureRestClone);

        // print resulting voice contents
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultiMeasureRestsDetails ()) {
          displayVoice (
            inputLineNumber,
            "appendMultiMeasureRestCloneToVoiceClone() 3");
        }
#endif // MF_TRACE_IS_ENABLED
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendRepeatCloneToVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat&       repeatCLone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat clone " <<
      repeatCLone->asString () <<
      " to voice clone \"" <<
      fVoiceName <<  "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    repeatCLone != nullptr,
    "repeatCLone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // push repeat clone as the (new) current repeat
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceRepeatsBasics ()) {
          std::stringstream ss;

          ss <<
            "Pushing repeat clone as the new current repeat in voice " <<
            fVoiceName;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // push the repeat clone onto the voice's repeat descrs stack
        pushRepeatOntoVoiceRepeatsStack (
          inputLineNumber,
          repeatCLone,
          "appendRepeatCloneToVoiceClone() 2");
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 4");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

// void msrVoice::handleMeasureRepeatStartInVoiceClone (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrMeasureRepeat& measureRepeat)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling measure repeat start in voice clone \"" <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   switch (fVoiceKind) {
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       // is there already a current measure repeat in this voice?
//       if (fVoicePendingMeasureRepeat) {
//         std::stringstream ss;
//
//         ss <<
//           "current voice measure repeat is not null when attempting to handle measure repeat start in voice clone " <<
//           asShortString ();
//
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
//
//       // create the measure repeat clone and register it
//       fVoicePendingMeasureRepeat =
//         measureRepeat->
//           createMeasureRepeatNewbornClone ();
//
//       // this voice contails measure repeats
//       this->setVoiceContainsMeasureRepeats (
//         inputLineNumber);
//
//       // set fVoicePendingMeasureRepeat's build phase to completed
//       fVoicePendingMeasureRepeat->
//         setCurrentMeasureRepeatBuildPhaseKind (
//           msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseJustCreated);
//       break;
//   } // switch
//
//   --gIndenter;
// }

// void msrVoice::handleMeasureRepeatEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling measure repeat end in voice clone \"" <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   switch (fVoiceKind) {
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       // is there a current measure repeat in this voice?
//       if (! fVoicePendingMeasureRepeat) {
//         std::stringstream ss;
//
//         ss <<
//           "current voice measure repeat is NULL when attempting to handle measure repeat end in voice clone " <<
//           asShortString ();
//
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
//
//       // forget about fVoicePendingMeasureRepeat
//       fVoicePendingMeasureRepeat = nullptr;
//
//     break;
//   } // switch
//
//   --gIndenter;
// }

// void msrVoice::handleMeasureRepeatPatternStartInVoiceClone (
//   const mfInputLineNumber& inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling measure repeat start in voice clone \"" <<
//       fVoiceName <<
//       "\", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   if (! fVoicePendingMeasureRepeat) {
//     std::stringstream ss;
//
//     ss <<
//       "current voice measure repeat is NULL when attempting to handle measure repeat pattern start " <<
//       " in voice clone " <<
//       asShortString ();
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // create fVoicePendingMeasureRepeat' rest pattern
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating a measure repeat pattern upon its start in voice " <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   S_msrMeasureRepeatPattern
//     measureRepeatPattern =
//       msrMeasureRepeatPattern::create (
//         inputLineNumber,
//         fVoicePendingMeasureRepeat);
//
//   // register it in fVoicePendingMeasureRepeat
//   fVoicePendingMeasureRepeat->
//     setMeasureRepeatPattern (
//       measureRepeatPattern);
//
//   --gIndenter;
// }

// void msrVoice::handleMeasureRepeatPatternEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling measure repeat end in voice clone \"" <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   if (! fVoicePendingMeasureRepeat) {
//     std::stringstream ss;
//
//     ss <<
//       "current voice measure repeat is NULL when attempting to handle measure repeat pattern end " <<
//       " in voice clone " <<
//       asShortString ();
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // get fVoicePendingMeasureRepeat's pattern
//   S_msrMeasureRepeatPattern
//     measureRepeatPattern =
//       fVoicePendingMeasureRepeat->
//         getMeasureRepeatPattern ();
//
//   // set fVoiceLastSegment as measureRepeatPattern' segment
//   measureRepeatPattern->
//     setMeasureRepeatPatternSegment (
//       fVoiceSegment);
//
//   --gIndenter;
// }

// void msrVoice::handleMeasureRepeatReplicasStartInVoiceClone (
//   const mfInputLineNumber& inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling measure repeat start in voice clone \"" <<
//       fVoiceName <<
//       "\", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   if (! fVoicePendingMeasureRepeat) {
//     std::stringstream ss;
//
//     ss <<
//       "current voice measure repeat is NULL when attempting to handle measure repeat replicas start " <<
//       " in voice clone " <<
//       asShortString ();
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // create fVoicePendingMeasureRepeat' replicas
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating a measure repeat replicas upon its start in voice " <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   S_msrMeasureRepeatReplicas
//     measureRepeatReplicas =
//       msrMeasureRepeatReplicas::create (
//         inputLineNumber,
//         fVoicePendingMeasureRepeat);
//
//   // register it in fVoicePendingMeasureRepeat
//   fVoicePendingMeasureRepeat->
//     setMeasureRepeatReplicas (
//       measureRepeatReplicas);
//
//   --gIndenter;
// }

// void msrVoice::handleMeasureRepeatReplicasEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling measure repeat end in voice clone \"" <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   if (! fVoicePendingMeasureRepeat) {
//     std::stringstream ss;
//
//     ss <<
//       "current voice measure repeat is NULL when attempting to handle measure repeat replicas end " <<
//       " in voice clone " <<
//       asShortString ();
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // get fVoicePendingMeasureRepeat's replicas
//   S_msrMeasureRepeatReplicas
//     measureRepeatReplicas =
//       fVoicePendingMeasureRepeat->
//         getMeasureRepeatReplicas ();
//
//   // set fVoiceLastSegment as measureRepeatReplicas' segment
//   measureRepeatReplicas->
//     setMeasureRepeatReplicasSegment (
//       fVoiceSegment);
//
//   --gIndenter;
// }

// void msrVoice::appendMeasureRepeatCloneToVoiceClone (
//   const mfInputLineNumber&  inputLineNumber,
//   const S_msrMeasureRepeat& measureRepeatClone)
// {
//   ++gIndenter;
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     measureRepeatClone != nullptr,
//     "measureRepeatClone is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   switch (fVoiceKind) { // superflous // JMI ???
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       {
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//           std::stringstream ss;
//
//           ss <<
//             "Appending measure repeat clone " <<
//             measureRepeatClone->asString () <<
//             " to voice clone \"" <<
//             fVoiceName <<
//             ", line " << inputLineNumber <<
//             std::endl;
//
//           gWaeHandler->waeTrace (
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str ());
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         // is measure repeat nested in a repeat?
//         if (! fVoicePendingRepeatsStack.empty ()) {
//           // yes
//
//           S_msrRepeat
//             currentRepeat =
//               fVoicePendingRepeatsStack.front ();
//
// //           // grab the measure repeat segment, i.e. the voice's the voice segment // JMI ???
// //           S_msrSegment
// //             measureRepeatSegment =
// //               fVoiceLastSegment;
//
// //           // append it to the current repeat's common part
// //           currentRepeat->
// //             getRepeatCommonPart ()->
// //               appendSegmentToRepeatCommonPart ( // NO !!!
// //                 inputLineNumber,
// //                 measureRepeatSegment,
// //                 "appendMeasureRepeatCloneToVoiceClone() 2");
//         }
//
//         else {
//           // no
//           // JMI ???
//         }
//
//         // append the measure repeat clone to the voice
//         appendMeasureRepeatToVoice (
//           inputLineNumber,
//           measureRepeatClone);
//
// //         // print resulting voice contents
// // #ifdef MF_TRACE_IS_ENABLED
// //         if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
// //           displayVoiceMeasureRepeatAndVoice (
// //             inputLineNumber,
// //             "appendMeasureRepeatCloneToVoiceClone() 3");
// //         }
// // #endif // MF_TRACE_IS_ENABLED
//       }
//       break;
//   } // switch
//
//   --gIndenter;
// }

void msrVoice::handleHookedRepeatEndingEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string& repeatEndingNumber) // a string, because if may be "1, 2" for example
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a HOOKED repeat ENDING in voice " <<
      fVoiceName <<  "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

/* JMI
        size_t voicerepeatsStackSize =
          fVoicePendingRepeatsStack.size ();
*/

  if (fVoicePendingRepeatsStack.empty ()) {
    std::stringstream ss;

    ss <<
      "Voice repeats stack is empty when attempting to handle a HOOKED repeat ENDING end in voice " <<
      fVoiceName;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // finalize current measure in voice
//   edacsacFinalizeLastAppendedMeasureInVoice ( // ZAZA
//     inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // create a HOOKED repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kRepeatEndingHooked;

  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::createAsWellAsItsSegment (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

  // add the repeat ending to the voice current repeat
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending a " <<
      msrRepeatEndingKindAsStringForTrace (repeatEndingKind) <<
      " repeat ENDING to current repeat in voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "before adding a HOOKED repeat ENDING to current repeat");
  }
#endif // MF_TRACE_IS_ENABLED

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleHooklessRepeatEndingEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string& repeatEndingNumber) // a string, because if may be "1, 2" for example
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a HOOKLESS repeat ENDING in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fVoicePendingRepeatsStack.empty ()) {
    std::stringstream ss;

    ss <<
      "Voice repeats stack is empty when attempting to handle a HOOKLESS repeat ENDING end in voice " <<
      fVoiceName;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // finalize current measure in voice
//   edacsacFinalizeLastAppendedMeasureInVoice ( // ZAZA
//     inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // create a HOOKLESS repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kRepeatEndingHookless;

  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::createAsWellAsItsSegment (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

  // add the repeat ending it to the voice current repeat
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending a " <<
      msrRepeatEndingKindAsStringForTrace (repeatEndingKind) <<
      " repeat ENDING to current repeat in voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "before adding a HOOKLESS repeat ENDING to current repeat");
  }
#endif // MF_TRACE_IS_ENABLED

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "after adding a HOOKLESS repeat ENDING to current repeat");
  }
#endif // MF_TRACE_IS_ENABLED

  // pop current repeat from the voice's repeat descrs stack
  S_msrRepeat
    innerMostRepeat =
      popRepeatFromVoiceRepeatsStack (
        inputLineNumber,
        "handleHooklessRepeatEndingEndInVoice");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 5");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::edacsacHandleRepeatEndingEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string&       repeatEndingNumber, // a string, because if may be "1, 2" for example
  msrRepeatEndingKind repeatEndingKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ENDING end in voice " <<
      fVoiceName <<
      ", repeatEndingNumber: " << repeatEndingNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        switch (repeatEndingKind) {
          case msrRepeatEndingKind::kRepeatEndingHooked:
            handleHookedRepeatEndingEndInVoice (
              inputLineNumber,
              repeatEndingNumber);
            break;

          case msrRepeatEndingKind::kRepeatEndingHookless:
            handleHooklessRepeatEndingEndInVoice (
              inputLineNumber,
              repeatEndingNumber);
            break;
        } // switch
      break;
    }
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayPendingRepeatsStack (
      inputLineNumber,
      "appendRepeatEndingToVoice() 0");
  }
#endif // MF_TRACE_IS_ENABLED
}

// S_msrSegment msrVoice::handleRepeatCommonPartStartInVoiceClone (
//   const mfInputLineNumber& inputLineNumber) // a string, because if may be "1, 2" for example
// {
//   S_msrSegment result;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling a repeat COMMON PART start in voice clone \"" <<
//       fVoiceName <<  "\"" <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "handleRepeatCommonPartStartInVoiceClone() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   if (fVoicePendingRepeatsStack.empty ()) {
//     std::stringstream ss;
//
//     ss <<
//       "repeats stack is empty when attempting to handle repeat COMMON PART start " <<
//       " in voice clone " <<
//       fVoiceName;
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // grab currentRepeat
//   S_msrRepeat
//     currentRepeat =
//       fVoicePendingRepeatsStack.front ();
//
//   // create currentRepeat's common part
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating a repeat COMMON PART upon its start in voice clone " <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   S_msrRepeatCommonPart
//     repeatCommonPart =
//       msrRepeatCommonPart::create (
//         inputLineNumber,
//         currentRepeat);
//
//   // register it in currentRepeat
//   currentRepeat->
//     setRepeatCommonPart (
//       repeatCommonPart);
//
//   // the result is the new repeat common part's segment
//   result =
//     repeatCommonPart->
//       getRepeatElementSegment ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "handleRepeatCommonPartStartInVoiceClone() 2");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   --gIndenter;
//
//   return result;
// }

// void msrVoice::handleRepeatCommonPartEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber) // a string, because if may be "1, 2" for example
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling a repeat COMMON PART end in voice clone \"" <<
//       fVoiceName <<  "\"" <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "handleRepeatCommonPartEndInVoiceClone() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   if (fVoicePendingRepeatsStack.empty ()) {
//     std::stringstream ss;
//
//     ss <<
//       "repeats stack is empty when attempting to handle repeat ENDING " <<
// //       repeatEnding->asShortString () <<
//       " in voice clone " <<
//       fVoiceName;
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // grab currentRepeat
//   S_msrRepeat
//     currentRepeat =
//       fVoicePendingRepeatsStack.front ();
//
//   // get currentRepeat's common part
//   S_msrRepeatCommonPart
//     repeatCommonPart =
//       currentRepeat->
//         getRepeatCommonPart ();
//
// /* JMI
//   // fetch the the voice segment's last measure
//   S_msrMeasure
//     voiceLastSegmentLastMeasure =
//       fVoiceSegment->
//         fetchLastMeasureFromSegment (
//           inputLineNumber,
//           "handleRepeatCommonPartEndInVoiceClone() 2");
// */
//
//   // set voice current after repeat component phase kind
//   setCurrentVoiceRepeatPhaseKind (
//     inputLineNumber,
//     msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "handleRepeatCommonPartEndInVoiceClone() 3");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   --gIndenter;
// }

// void msrVoice::handleHookedRepeatEndingEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber,
//   const std::string& repeatEndingNumber) // a string, because if may be "1, 2" for example
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling a HOOKED repeat ENDING in voice clone \"" <<
//       fVoiceName <<  "\"" <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "handleHookedRepeatEndingEndInVoiceClone() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   if (fVoicePendingRepeatsStack.empty ()) {
//     std::stringstream ss;
//
//     ss <<
//       "repeats stack is empty when attempting to handle a HOOKED repeat ENDING in voice clone " <<
//       fVoiceName;
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // grab currentRepeat
//   S_msrRepeat
//     currentRepeat =
//       fVoicePendingRepeatsStack.front ();
//
//   // create a HOOKED repeat ending
//   msrRepeatEndingKind
//     repeatEndingKind =
//       msrRepeatEndingKind::kRepeatEndingHooked;
//
//   S_msrRepeatEnding
//     repeatEnding =
//       msrRepeatEnding::create (
//         inputLineNumber,
//         repeatEndingNumber,
//         repeatEndingKind,
//         currentRepeat);
//
//   // fetch the the voice segment's last measure
//   S_msrMeasure
//     voiceLastSegmentLastMeasure =
//       fVoiceSegment->
//         fetchLastMeasureFromSegment (
//           inputLineNumber,
//           "handleHookedRepeatEndingEndInVoiceClone() 2");
//
//   // set voice current after repeat component phase kind
//   setCurrentVoiceRepeatPhaseKind (
//     inputLineNumber,
//     msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHookedEnding);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "handleHookedRepeatEndingEndInVoiceClone() 3");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   --gIndenter;
// }

// void msrVoice::handleHooklessRepeatEndingEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber,
//   const std::string& repeatEndingNumber) // a string, because if may be "1, 2" for example
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling a HOOKLESS repeat ENDING in voice clone \"" <<
//       fVoiceName <<  "\"" <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "handleHooklessRepeatEndingEndInVoiceClone() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   if (fVoicePendingRepeatsStack.empty ()) {
//     std::stringstream ss;
//
//     ss <<
//       "repeats stack is empty when attempting to handle a HOOKLESS repeat ENDING in voice clone " <<
//       fVoiceName;
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // grab currentRepeat
//   S_msrRepeat
//     currentRepeat =
//       fVoicePendingRepeatsStack.front ();
//
//   // create a HOOKLESS repeat ENDING
//   msrRepeatEndingKind
//     repeatEndingKind =
//       msrRepeatEndingKind::kRepeatEndingHookless;
//
//   S_msrRepeatEnding
//     repeatEnding =
//       msrRepeatEnding::create (
//         inputLineNumber,
//         repeatEndingNumber,
//         repeatEndingKind,
//         currentRepeat);
//
//   // fetch the the voice segment's last measure
//   S_msrMeasure
//     voiceLastSegmentLastMeasure =
//       fVoiceSegment->
//         fetchLastMeasureFromSegment (
//           inputLineNumber,
//           "handleHookedRepeatEndingEndInVoiceClone() 2");
//
//   // set voice current after repeat component phase kind
//   setCurrentVoiceRepeatPhaseKind (
//     inputLineNumber,
//     msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHooklessEnding);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "handleHooklessRepeatEndingEndInVoiceClone() 3");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   --gIndenter;
// }

// void msrVoice::handleRepeatEndingEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber,
//   const std::string&       repeatEndingNumber, // a string, because if may be "1, 2" for example
//   msrRepeatEndingKind      repeatEndingKind)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "appendRepeatEndingToVoiceClone() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   switch (fVoiceKind) {
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       {
//         switch (repeatEndingKind) {
//           case msrRepeatEndingKind::kRepeatEndingHooked:
//             handleHookedRepeatEndingEndInVoiceClone (
//               inputLineNumber,
//               repeatEndingNumber);
//             break;
//
//           case msrRepeatEndingKind::kRepeatEndingHookless:
//             handleHooklessRepeatEndingEndInVoiceClone (
//               inputLineNumber,
//               repeatEndingNumber);
//             break;
//         } // switch
//       break;
//     }
//   } // switch
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "appendRepeatEndingToVoiceClone() 2");
//   }
// #endif // MF_TRACE_IS_ENABLED
// }

// void msrVoice::edacsacHandleRepeatStartInVoiceClone (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrRepeat&       repeat)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling a repeat start in voice clone \"" <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayPendingRepeatsStack (
//       inputLineNumber,
//       "edacsacHandleRepeatStartInVoiceClone() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   switch (fVoiceKind) {
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       // create the repeat clone and stack it
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//         std::stringstream ss;
//
//         ss <<
//           "Creating a repeat upon its start in voice clone \"" <<
//           fVoiceName <<
//               ", line " << inputLineNumber;
//
//         gWaeHandler->waeTrace (
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       S_msrRepeat
//         repeatClone =
//           createARepeatCloneAndStackIt (
//             inputLineNumber,
//             repeat,
//             "edacsacHandleRepeatStartInVoiceClone() 4");
//
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//         displayPendingRepeatsStack (
//           inputLineNumber,
//           "edacsacHandleRepeatStartInVoiceClone() 5");
//       }
// #endif // MF_TRACE_IS_ENABLED
//       break;
//   } // switch
//
//   --gIndenter;
// }

// void msrVoice::handleRepeatEndInVoiceClone (
//   const mfInputLineNumber& inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling a repeat end in voice clone \"" <<
//       fVoiceName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   switch (fVoiceKind) {
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       {
//       /* JMI
//         // finalize current measure in voice
//         edacsacFinalizeLastAppendedMeasureInVoice (
//           inputLineNumber);
//           */
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//           displayPendingRepeatsStack (
//             inputLineNumber,
//             "handleRepeatEndInVoiceClone() 1");
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         // is there a current repeat?
//         switch (fVoicePendingRepeatsStack.size ()) {
//           case 0:
//             {
//               std::stringstream ss;
//
//               ss <<
//                 "repeats stack is empty when attempting to handle a repeat end in voice clone " <<
//                 asShortString () <<
//                 " ";
//
//               msrInternalError (
//                 gServiceRunData->getInputSourceName (),
//                 fInputLineNumber,
//                 __FILE__, mfInputLineNumber (__LINE__),
//                 ss.str ());
//             }
//             break;
//
//           case 1:
//             {
//               // this is a voice-level repeat
//
//               // fetch currentRepeat
//               S_msrRepeat
//                 currentRepeat =
//                   fVoicePendingRepeatsStack.front ();
//
//               // get currentRepeat's common part
//               S_msrRepeatCommonPart
//                 repeatCommonPart =
//                   currentRepeat->
//                     getRepeatCommonPart ();
//
// //               // pop currentRepeat from the voice's repeat descrs stack
// //               popRepeatFromVoiceRepeatsStack (
// //                 inputLineNumber,
// //                 currentRepeat,
// //                 "handleRepeatEndInVoiceClone() 3");
//             }
//             break;
//
//           default:
//             {
//               // this is a nested repeat
//
//               // fetch currentRepeat
//               S_msrRepeat
//                 currentRepeat =
//                   fVoicePendingRepeatsStack.front ();
//
//               // get currentRepeat's common part
//               S_msrRepeatCommonPart
//                 repeatCommonPart =
//                   currentRepeat->
//                     getRepeatCommonPart ();
//
// //               // pop currentRepeat from the voice's repeat descrs stack
// //               popRepeatFromVoiceRepeatsStack (
// //                 inputLineNumber,
// //                 currentRepeat,
// //                 "handleRepeatEndInVoiceClone() 5");
//             }
//         } // switch
//
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//           displayPendingRepeatsStack (
//             inputLineNumber,
//             "handleRepeatEndInVoiceClone() 6");
//         }
// #endif // MF_TRACE_IS_ENABLED
//       }
//       break;
//   } // switch
//
//   --gIndenter;
// }

void msrVoice::appendMeasureRepeatReplicaToVoice (
  const mfInputLineNumber& inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Appending a measure repeat replica to voice " <<
            fVoiceName <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // fetch last measure's full measure whole notes
        /* JMI
        int fullMeasureWholeNotesDuration =
          fVoiceSegment->
            getSegmentElementsList ().back ()->
              getFullMeasureWholeNotesDuration ();
              */

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) { // JMI 0.9.67
          gLog <<
            std::endl <<
            "***********" <<
            std::endl << std::endl;
          print (gLog);
          gLog <<
            "***********" <<
            std::endl << std::endl;
        }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
        // sanity check
        mfAssert (
          __FILE__, mfInputLineNumber (__LINE__),
          fVoicePendingMeasureRepeat != nullptr,
          "fVoicePendingMeasureRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

        // create a measure repeat replicas
        S_msrMeasureRepeatReplicas
          measureRepeatReplicas =
            msrMeasureRepeatReplicas::create (
              inputLineNumber,
              fVoicePendingMeasureRepeat);

        // set the voice the voice segment as the measure repeat replicas segment
        measureRepeatReplicas->
          setMeasureRepeatReplicasSegment (
            fVoiceSegment);

        // set the measure repeat replicas in the voice current measure repeat
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) { // // JMI 0.9.67
          std::stringstream ss;

          ss <<
            "Setting the measure repeat replica to current measure repeat BBB in voice " <<
            fVoiceName;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoicePendingMeasureRepeat->
          setMeasureRepeatReplicas (
            measureRepeatReplicas);
      }
      break;
  } // switch
}

void msrVoice::appendMeasureRepeatToVoiceElementsList (
  const S_msrMeasureRepeat& measureRepeat)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    measureRepeat != nullptr,
    "measureRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measure repeat " <<
      measureRepeat->asString () <<
      " to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendMeasureRepeatToSegment (measureRepeat);
}

void msrVoice::appendRepeatEndingCloneToVoice ( // JMI
  const S_msrRepeatEnding& repeatEndingClone)
{
  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // add the repeat ending it to the voice current repeat
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceRepeatsDetails ()) {
          std::stringstream ss;

          ss <<
            "Appending a " <<
            repeatEndingClone->getRepeatEndingKind () <<
            " repeat ENDING clone to current repeat in voice " <<
            fVoiceName <<
                  std::endl;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());

          displayPendingRepeatsStack (
            repeatEndingClone->getInputLineNumber (),
            "appendRepeatEndingCloneToVoice() 1");
        }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
        // sanity check
        if (fVoicePendingRepeatsStack.empty ()) {
          std::stringstream ss;

          ss <<
            "Voice repeats stack is empty when attempting to append a " <<
            repeatEndingClone->getRepeatEndingKind () <<
            " repeat ENDING to voice " <<
            fVoiceName;

          msrInternalError (
            gServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

        S_msrRepeat
          currentRepeat =
            fVoicePendingRepeatsStack.front ();

        currentRepeat->
          addRepeatEndingToRepeat (
            repeatEndingClone->getInputLineNumber (),
            repeatEndingClone);

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceRepeatsDetails ()) {
          displayPendingRepeatsStack (
            repeatEndingClone->getInputLineNumber (),
            "appendRepeatEndingCloneToVoice() 2");
        }
#endif // MF_TRACE_IS_ENABLED
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::prependBarLineToVoice (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Prepending barLine " <<
      barLine->asString () <<
      " to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  fVoiceSegment->
    prependBarLineToSegment (barLine);

  --gIndenter;
}

void msrVoice::appendBarLineToVoice (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Appending barLine " <<
      barLine->asString () <<
      " to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  fVoiceSegment->
    appendBarLineToSegment (barLine);

  --gIndenter;
}

void msrVoice::appendSegnoToVoice (const S_msrSegno& segno)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegnos ()) {
    std::stringstream ss;

    ss <<
      "Appending a segno to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendSegnoToSegment (segno);
}

void msrVoice::appendCodaToVoice (const S_msrCoda& coda)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCodas ()) {
    std::stringstream ss;

    ss <<
      "Appending a coda to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendCodaToSegment (coda);
}

void msrVoice::appendEyeGlassesToVoice (
  const S_msrEyeGlasses& eyeGlasses)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceEyeGlasses ()) {
    std::stringstream ss;

    ss <<
      "Appending a eyeGlasses to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendEyeGlassesToSegment (eyeGlasses);
}

void msrVoice::appendPedalToVoice (const S_msrPedal& pedal)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePedals ()) {
    std::stringstream ss;

    ss <<
      "Appending a pedal to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendPedalToSegment (pedal);
}

void msrVoice::appendDampToVoice (
  const S_msrDamp& damp)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDamps ()) {
    std::stringstream ss;

    ss <<
      "Appending a damp to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendDampToSegment (damp);
}

void msrVoice::appendDampAllToVoice (
  const S_msrDampAll& dampAll)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDampAlls ()) {
    std::stringstream ss;

    ss <<
      "Appending a damp all to voice " <<
      fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceSegment->
    appendDampAllToSegment (dampAll);
}

/*
S_msrElement msrVoice::removeLastElementFromVoice (  // JMI
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Removing last note" <<
      " from voice " << fVoiceName;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return
    fVoiceSegment->
      removeLastElementFromSegment (inputLineNumber);
}
*/

// void msrVoice::removeNoteFromVoice (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrNote& note)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChords ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing note " <<
//       note->asShortString () <<
//       " from voice " <<
//       fVoiceName;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // remove note from voice the voice segment
//   fVoiceSegment->
//     removeNoteFromSegment (
//       inputLineNumber,
//       note);
//
//   // update the part current measure position
//   fVoiceUpLinkToStaff->
//     getStaffUpLinkToPart ()->
//       decrementPartCurrentDrawingPositionInMeasure (
//         inputLineNumber,
//         note->
//           getMeasureElementSoundingWholeNotes ());
//
//   --gIndenter;
// }

// void msrVoice::removeElementFromVoice (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrElement& element)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceVoices ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing element " <<
//       element->asShortString () <<
//       " from voice " <<
//       fVoiceName;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   fVoiceSegment->
//     removeElementFromSegment (
//       inputLineNumber,
//       element);
//
//   --gIndenter;
// }

// S_msrMeasure msrVoice::removeLastMeasureFromVoice (
//   const mfInputLineNumber& inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing last measure from voice " <<
//       fVoiceName;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // remove last measure
//   S_msrMeasure
//     result =
//       fVoiceSegment->
//         removeLastMeasureFromSegment (
//           inputLineNumber,
//           "removeLastMeasureFromVoice()");
//
//   --gIndenter;
//
//   // return it
//   return result;
// }

void msrVoice::edacsacFinalizeLastAppendedMeasureInVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresBasics ()) {
    std::stringstream ss;

    ss <<
      "Edacsaccing finalizing last appended measure in voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "edacsacFinalizeLastAppendedMeasureInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

//  return; // ZOULOU

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fVoiceLastAppendedMeasure != nullptr,
    "fVoiceLastAppendedMeasure is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // finalize fVoiceLastAppendedMeasure
  msrMeasureRepeatContextKind
    measureRepeatContextKind =
      msrMeasureRepeatContextKind::kMeasureRepeatContextNone;

  fVoiceLastAppendedMeasure->
    finalizeMeasure (
      fVoiceLastAppendedMeasure->getInputLineNumber (),
      measureRepeatContextKind,
      "edacsacFinalizeLastAppendedMeasureInVoice() 2");

/* JMI NO v0.0.70
  // forget about fVoiceLastAppendedMeasure
  setVoiceLastAppendedMeasure (
    nullptr);
*/

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // append a measure end syllable to the voice stanzas if any
      if (! fVoiceStanzasMap.empty ()) {
        for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
          S_msrStanza stanza = thePair.second;

          // fetch the part
          S_msrPart
            part =
              fetchVoiceUpLinkToPart ();

           // fetch the part current measure position
          mfPositionInMeasure
            partCurrentDrawingPositionInMeasure =
              part->
                getPartCurrentDrawingPositionInMeasure ();

         stanza->
            appendMeasureEndSyllableToStanza (
              inputLineNumber,
              fVoiceLastAppendedMeasure,
              partCurrentDrawingPositionInMeasure);
        } // for
      }

// JMI
      // handle the harmonies voice if any
      if (fRegularVoiceForwardLinkToHarmoniesVoice) {
        fRegularVoiceForwardLinkToHarmoniesVoice->
          edacsacFinalizeLastAppendedMeasureInVoice (
            inputLineNumber);
      }

      // handle the figured bass voice if any
      if (fRegularVoiceForwardLinkToFiguredBassVoice) {
        fRegularVoiceForwardLinkToFiguredBassVoice->
          edacsacFinalizeLastAppendedMeasureInVoice (
            inputLineNumber);
      }
//
      break;

    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "edacsacFinalizeLastAppendedMeasureInVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::collectVoiceMeasuresIntoFlatList (
  const mfInputLineNumber& inputLineNumber)
{
  // collect measures from the the voice segment if any
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceVoices ()) {
      std::stringstream ss;

      ss <<
        "Collecting measures from the the voice segment into voice " <<
        fVoiceName <<
        " into the measures flat list" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    std::list <S_msrMeasure>
      lastSegmentMeasuresFlatList =
        fVoiceSegment->
          getSegmentMeasureList ();

    if (! lastSegmentMeasuresFlatList.empty ()) {
      for (S_msrMeasure measure : lastSegmentMeasuresFlatList) {
        fVoiceMeasuresFlatList.push_back (measure);
      } // for
    }
}

void msrVoice::finalizeVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Finalizing voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  if (fVoiceHasBeenFinalized) {
    std::stringstream ss;

    ss <<
      "Attempting to finalize voice " <<
      asShortString () <<
      " more than once";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // set part shortest note wholeNotes if relevant
  S_msrPart
    voicePart =
      fetchVoiceUpLinkToPart ();

  mfWholeNotes
    partShortestNoteWholeNotes =
      voicePart->
        getPartShortestNoteWholeNotes ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "--> in voice " <<
      fVoiceName <<
      ", fVoiceShortestNoteWholeNotes: " <<
      fVoiceShortestNoteWholeNotes.asFractionString () <<
      ", partShortestNoteWholeNotes: " << partShortestNoteWholeNotes;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fVoiceShortestNoteWholeNotes < partShortestNoteWholeNotes) {
    // set the voice part shortest note wholeNotes
    voicePart->
      setPartShortestNoteWholeNotes (
        fVoiceShortestNoteWholeNotes);

    // set the voice part shortest note tuplet factor // JMI
    voicePart->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
  }

  // are there pending repeats in the voice repeats stack???
  size_t voicePendingRepeatsStackSize =
    fVoicePendingRepeatsStack.size ();

  if (voicePendingRepeatsStackSize) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresDetails ()) {
        displayPendingRepeatsStack (
          inputLineNumber,
          "finalizeVoice() 2");
      }
#endif // MF_TRACE_IS_ENABLED

    std::stringstream ss;

    ss <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatsStackSize, "There is", "There are") <<
      ' ' <<
      voicePendingRepeatsStackSize <<
      ' ' <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatsStackSize, "repeat", "repeats") <<
      " pending in the voice repeats stack in voice " <<
      asShortString ();

    msrWarning (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      ss.str ());
  }

  // collect the voice measures into the measures flat list
  collectVoiceMeasuresIntoFlatList (
    inputLineNumber);

  fVoiceHasBeenFinalized = true;

  // finalize the harmonies voice if any
  if (fRegularVoiceForwardLinkToHarmoniesVoice) {
    fRegularVoiceForwardLinkToHarmoniesVoice->finalizeVoice (
      inputLineNumber);
  }

  // finalize the figured bass voice if any
  if (fRegularVoiceForwardLinkToFiguredBassVoice) {
    fRegularVoiceForwardLinkToFiguredBassVoice->finalizeVoice (
      inputLineNumber);
  }
//
}

void msrVoice::finalizeVoiceAndAllItsMeasures (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Finalizing voice " <<
      fVoiceName <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  if (fVoiceHasBeenFinalized) {
#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
    // maintainance check
    if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN // JMI 0.9.70
      std::stringstream ss;

      ss <<
        "Attempting to finalize  voice " <<
        asShortString () <<
        " more than once";

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
  }

  // set part shortest note wholeNotes if relevant
  S_msrPart
    voicePart =
      fetchVoiceUpLinkToPart ();

  mfWholeNotes
    partShortestNoteWholeNotes =
      voicePart->
        getPartShortestNoteWholeNotes ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "--> in voice " <<
      fVoiceName <<
      ", fVoiceShortestNoteWholeNotes: " <<
      fVoiceShortestNoteWholeNotes.asFractionString () <<
      ", partShortestNoteWholeNotes: " <<
      partShortestNoteWholeNotes.asFractionString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fVoiceShortestNoteWholeNotes < partShortestNoteWholeNotes) {
    // set the voice part shortest note wholeNotes
    voicePart->
      setPartShortestNoteWholeNotes (
        fVoiceShortestNoteWholeNotes);

    // set the voice part shortest note tuplet factor // JMI
    voicePart->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
  }

  // are there pending repeats in the voice repeats stack???
  size_t voicePendingRepeatsStackSize =
    fVoicePendingRepeatsStack.size ();

  if (voicePendingRepeatsStackSize) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresDetails ()) {
        displayPendingRepeatsStack (
          inputLineNumber,
          "finalizeVoice() 2");
      }
#endif // MF_TRACE_IS_ENABLED

    std::stringstream ss;

    ss <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatsStackSize, "There is", "There are") <<
      ' ' <<
      voicePendingRepeatsStackSize <<
      ' ' <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatsStackSize, "repeat", "repeats") <<
      " pending in the voice repeats stack in voice " <<
      asShortString () <<
      "\" ";

    msrWarning (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      ss.str ());
  }

  // collect the voice measures into the flat list
  collectVoiceMeasuresIntoFlatList (
    inputLineNumber);

  fVoiceHasBeenFinalized = true;

  // finalize the harmonies voice if any // JMI
  if (fRegularVoiceForwardLinkToHarmoniesVoice) {
    fRegularVoiceForwardLinkToHarmoniesVoice->
      finalizeVoice (
        inputLineNumber);
  }

  // finalize the figured bass voice if any
  if (fRegularVoiceForwardLinkToFiguredBassVoice) {
    fRegularVoiceForwardLinkToFiguredBassVoice->
      finalizeVoice (
        inputLineNumber);
  }
//

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Finalizing all the measures of voice " <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrMeasure measure : fVoiceMeasuresFlatList) {
    measure->
      finalizeMeasure (
        measure->getInputLineNumber (),
        msrMeasureRepeatContextKind::kMeasureRepeatContextNone,
        "finalizeVoiceAndAllItsMeasures()");
  } // for
}

void msrVoice::checkBeamNumber (
  const S_msrBeam& beam,
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "Checking the order of the beams begins, continues and ends" <<
      " upon beam " <<
      beam->asShortString () <<
      " and note " <<
      note->asString () <<
      "', line " << beam->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  msrBeamKind beamKind = beam->getBeamKind ();
  int         beamNumber = beam->getBeamNumber ();

  size_t
    noteBeamNumbersStackSize =
      fVoiceBeamNumbersStack.size ();

  switch (beamKind) {
    case msrBeamKind::kBeam_UNKNOWN_:
      {
        std::stringstream ss;

        ss <<
          "appending unknown beam " <<
          beam->asShortString () <<
          " to note " <<
          note->asShortString ();

        msrError (
          gServiceRunData->getInputSourceName (),
          beam->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrBeamKind::kBeamBegin:
      if (noteBeamNumbersStackSize) {
        // the stack is not empty
        int voiceBeamNumbersStackTop =
          fVoiceBeamNumbersStack.front ();

        if (beamNumber != voiceBeamNumbersStackTop + 1) {
          std::stringstream ss;

          ss <<
            "appending beam begin " <<
            beam->asShortString () <<
            " to note " <<
            note->asShortString () <<
            "whilst the current beam began with number " <<
            voiceBeamNumbersStackTop <<
            "'";

          msrError (
            gServiceRunData->getInputSourceName (),
            beam->getInputLineNumber (),
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
      }

      fVoiceBeamNumbersStack.push_front (beamNumber);
      break;

    case msrBeamKind::kBeamContinue:
      if (! noteBeamNumbersStackSize) {
        // the stack is empty
        std::stringstream ss;

        ss <<
          "appending beam continue " <<
          beam->asShortString () <<
          " to note " <<
          note->asShortString () <<
          "whilst there is no preceding beam begin with number " <<
          beamNumber <<
          "'";

        msrError (
          gServiceRunData->getInputSourceName (),
          beam->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      else {
        // the stack is not empty
        int voiceBeamNumbersStackTop =
          fVoiceBeamNumbersStack.front ();

        if (beamNumber != voiceBeamNumbersStackTop) {
          std::stringstream ss;

          ss <<
            "appending beam continue " <<
            beam->asShortString () <<
            " to note " <<
            note->asShortString () <<
            "whilst the current beam began with number " <<
            voiceBeamNumbersStackTop <<
            "'";

          msrError (
            gServiceRunData->getInputSourceName (),
            beam->getInputLineNumber (),
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
      }
      break;

    case msrBeamKind::kBeamEnd:
      if (noteBeamNumbersStackSize) {
        // the stack is not empty
        int voiceBeamNumbersStackTop =
          fVoiceBeamNumbersStack.front ();

        if (beamNumber != voiceBeamNumbersStackTop) {
          std::stringstream ss;

          ss <<
            "appending beam end " <<
            beam->asShortString () <<
            " to note " <<
            note->asShortString () <<
            "whilst the current beam began with number " <<
            voiceBeamNumbersStackTop <<
            "'";

          msrError (
            gServiceRunData->getInputSourceName (),
            beam->getInputLineNumber (),
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
      }

      fVoiceBeamNumbersStack.pop_front ();
      break;

    case msrBeamKind::kBeamForwardHook:
      // JMI ???
      break;
    case msrBeamKind::kBeamBackwardHook:
      // JMI ???
      break;
  } // switch
}

void msrVoice::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrVoice::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrVoice::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrVoice::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrVoice::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrVoice::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // browse the voice segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fVoiceSegment);

  // browse the voice stanzas
  if (! fVoiceStanzasMap.empty ()) {
    for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      if (! stanza->getSyllables ().empty ()) { // 2026.2
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrVoice::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrVoice::fetchVoiceNumberAsString () const
{
  return mfVoiceNumberAsString (fVoiceNumber);
}

std::string msrVoice::regularVoiceStaffSequentialNumberAsString () const
{
  std::string result;

  if (fRegularVoiceStaffSequentialNumber == K_MF_PART_FIGURED_BASS_VOICE_NUMBER) {
    result += " (K_MF_PART_FIGURED_BASS_VOICE_NUMBER)";
  }
  else {
    // nothing more
  }

  return result;
}

std::string msrVoice::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[Voice" << fVoicePathLikeName <<
    ", fVoiceKind: " <<
    fVoiceKind <<
    ", fVoiceSegment: " <<
    fVoiceSegment->asString () <<
    ", fVoiceNumber: " <<
    fVoiceNumber <<
    ", fRegularVoiceOrdinalNumberInPart: " <<
    fRegularVoiceOrdinalNumberInPart <<
    ", fVoiceSegment: " <<
    fetchSegmentAsShortString (fVoiceSegment) <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrVoice::asString () const
{
  std::stringstream ss;

  ss <<
    "[Voice" <<
    ", fVoicePathLikeName: " <<
    fVoicePathLikeName <<
    ", fVoiceKind: " <<
    fVoiceKind <<
    ", fVoiceSegment: " <<
    fetchSegmentAsShortString (fVoiceSegment) <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrVoice::displayVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context) const
{
  gLog <<
    std::endl <<
    "*********>> Displaying voice " <<
    fVoiceName <<
    " (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    std::endl;

  ++gIndenter;
  print (gLog);
  --gIndenter;

  gLog <<
    " <<*********" <<
    std::endl << std::endl;
}

void msrVoice::print (std::ostream& os) const
{
  os <<
    "[Voice" << fVoicePathLikeName <<
//     msrVoiceKindAsStringForPrint (fVoiceKind) <<
    ", " <<
    fVoicePathLikeName <<
    ", fVoiceNumber " <<
    fVoiceNumber <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 43;

  // print the regular voice harmonies voice name if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fRegularVoiceOrdinalNumberInPart " << ": " <<
    fRegularVoiceOrdinalNumberInPart <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fRegularVoiceForwardLinkToHarmoniesVoice" << ": " <<
    fetchVoiceAsShortString (fRegularVoiceForwardLinkToHarmoniesVoice) <<
    std::endl;

  // print the figured bass voice name if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fRegularVoiceForwardLinkToFiguredBassVoice" << ": " <<
    fetchVoiceAsShortString (fRegularVoiceForwardLinkToFiguredBassVoice) <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceActualNotesCounter" << ": " <<
    fVoiceActualNotesCounter <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceRestsCounter" << ": " <<
    fVoiceRestsCounter <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceSkipsCounter" << ": " <<
    fVoiceSkipsCounter <<
    std::endl << std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceActualHarmoniesCounter" << ": " <<
    fVoiceActualHarmoniesCounter <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceActualFiguredBassesCounter" << ": " <<
    fVoiceActualFiguredBassesCounter <<
    std::endl << std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceStanzasMap.size ():" << ": " <<
    fVoiceStanzasMap.size () <<
    std::endl;

  // print the voice uplink to staff if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceUpLinkToStaff" << ": " <<
    fetchStaffAsShortString (fVoiceUpLinkToStaff) <<
    std::endl;

#ifdef MF_TRACE_IS_ENABLED
// JMI 0.9.67 HARMFUL  displayVoiceMeasuresFlatList (fieldWidth);
#endif // MF_TRACE_IS_ENABLED

  os << std::endl;

  // print the voice segment
  os <<
    std::setw (fieldWidth) <<
    "***** fVoiceSegment *****" << ": ";
    os << std::endl;

    ++gIndenter;
    os << fVoiceSegment;
    --gIndenter;

  os << std::endl;

//   // print the current voice recipient segment
//   os <<
//     std::setw (fieldWidth) <<
//     "***** fVoiceSegment *****" << ": ";
//     os << std::endl;
//
//   if (fVoiceSegment) {
//     ++gIndenter;
//     // getSegmentAbsoluteNumber() could be used too // JMI ??? 0.9.76
//     if (
//       fVoiceSegment->getSegmentNumber ()
//         !=
//       fVoiceSegment->getSegmentNumber ()
//     ) {
//       os << fVoiceSegment;
//     }
//     else {
//       os << "--- same as fVoiceSegment ---" << std::endl;
//     }
//     --gIndenter;
//   }
//   else {
//     os << "[NULL]";
//   }
//   os << std::endl;

  // print the stanzas if any
  if (! fVoiceStanzasMap.empty ()) {
    os <<
      std::endl <<
      "***** fVoiceStanzasMap *****:" <<
      std::endl;

    ++gIndenter;

    for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      os << stanza;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrVoice::printFull (std::ostream& os) const
{
  os <<
    "[Voice" << fVoiceName <<
//     msrVoiceKindAsStringForPrint (fVoiceKind) <<
    ", " <<
    fVoicePathLikeName <<
    ", fVoiceNumber " <<
    fVoiceNumber <<
    ", fRegularVoiceOrdinalNumberInPart " <<
    fRegularVoiceOrdinalNumberInPart <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 43;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceActualNotesCounter" << ": " <<
    fVoiceActualNotesCounter <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceRestsCounter" << ": " <<
    fVoiceRestsCounter <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceSkipsCounter" << ": " <<
    fVoiceSkipsCounter <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceActualHarmoniesCounter" << ": " <<
    fVoiceActualHarmoniesCounter <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceActualFiguredBassesCounter" << ": " <<
    fVoiceActualFiguredBassesCounter <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceStanzasMap.size ()" << ": " <<
    fVoiceStanzasMap.size () <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceShortestNoteWholeNotes" << ": " <<
    fVoiceShortestNoteWholeNotes.asFractionString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceShortestNoteTupletFactor" << ": " <<
    std::endl;

  ++gIndenter;
  os <<
    fVoiceShortestNoteTupletFactor;
  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceCurrentMeasureNumber" << ": \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceIsMusicallyEmpty" << ": " <<
    fVoiceIsMusicallyEmpty <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceCurrentMeasureOrdinalNumber" << ": " <<
    fVoiceCurrentMeasureOrdinalNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceCurrentMeasurePuristNumber" << ": " <<
    fVoiceCurrentMeasurePuristNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fRegularVoiceStaffSequentialNumber" << ": " <<
    regularVoiceStaffSequentialNumberAsString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceHasBeenFinalized" << ": " <<
    fVoiceHasBeenFinalized <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCurrentVoicePosition" << ": " <<
    fCurrentVoicePosition <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fCurrentVoiceMoment" << ": " <<
    fCurrentVoiceMoment <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceContainsMultiMeasureRests" << ": " <<
    fVoiceContainsMultiMeasureRests <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceContainsMeasureRepeats" << ": " <<
    fVoiceContainsMeasureRepeats <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceUpLinkToStaff" << ": ";
  if (fVoiceUpLinkToStaff) {
    os <<
      fVoiceUpLinkToStaff->getStaffPathLikeName ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

#ifdef MF_TRACE_IS_ENABLED
  // regular measure ends detection
  os << std::left <<
    std::setw (fieldWidth) <<
    "fWholeNotesSinceLastRegularMeasureEnd" << ": " <<
    fWholeNotesSinceLastRegularMeasureEnd <<
    std::endl;

  // incomplete measures after repeats detection
  os << std::left <<
    std::setw (fieldWidth) <<
    "fCurrentVoiceRepeatPhaseKind" << ": " <<
    fCurrentVoiceRepeatPhaseKind <<
    std::endl;

  // print the voice first clef, and the current clef, key and time signature
  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceFirstClef" << ": ";
  if (fVoiceFirstClef) {
    os <<
      fVoiceFirstClef;
  }
  else {
    os << "[NULL]" << std::endl;
  }
  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceCurrentClef" << ": ";
  if (fVoiceCurrentClef) {
    os <<
      fVoiceCurrentClef;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceCurrentKey" << ": ";
  if (fVoiceCurrentKey) {
    os <<
      fVoiceCurrentKey;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceCurrentTimeSignature" << ": ";
  if (fVoiceCurrentTimeSignature) {
    os << std::endl;
    ++gIndenter;

    os <<
      fVoiceCurrentTimeSignature;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

// JMI
  // print the regular voice harmonies voice name if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fRegularVoiceForwardLinkToHarmoniesVoice" << ": ";
  if (fRegularVoiceForwardLinkToHarmoniesVoice) {
    os <<
      fRegularVoiceForwardLinkToHarmoniesVoice->fVoiceName;
  }
  else {
    os <<
      "[NULL]";
  }
  os << std::endl;

  // print the figured bass voice name if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fRegularVoiceForwardLinkToFiguredBassVoice" << ": ";
  if (fRegularVoiceForwardLinkToFiguredBassVoice) {
    os <<
      fRegularVoiceForwardLinkToFiguredBassVoice->fVoiceName;
  }
  else {
    os <<
      "[NULL]";
  }
  os << std::endl;

  // print the voice last appended measure if any
  os <<
    std::setw (fieldWidth) <<
    "fVoiceLastAppendedMeasure" << ": ";
  if (fVoiceLastAppendedMeasure) {
    os <<
      '\'' <<
      fVoiceLastAppendedMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "[NULL]";
  }
  os << std::endl;

  // print the voice first measure if any
  os <<
    std::setw (fieldWidth) <<
    "fVoiceFirstMeasure" << ": ";
  if (fVoiceFirstMeasure) {
    os <<
      '\'' <<
      fVoiceFirstMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "[NULL]";
  }
  os << std::endl;

  // print this voice's first non-grace note
  {
    S_msrNote
      voiceFirstNonGraceNote =
        this->
          fetchVoiceFirstNonGraceNote ();

    os <<
      std::setw (fieldWidth) <<
      "fVoiceFirstNonGraceNote" << ": ";
    if (voiceFirstNonGraceNote) {
      os <<
        voiceFirstNonGraceNote->asShortString ();
    }
    else {
      os <<
        "[NULL]";
    }
    os << std::endl;
  }

  // print the voice last appended note
  os <<
    std::setw (fieldWidth) <<
    "fVoiceLastAppendedNote" << ": ";
  if (fVoiceLastAppendedNote) {
    os << gTab <<
      fVoiceLastAppendedNote->asShortString ();
  }
  else {
    os <<
      "[NULL]" <<
      std::endl;
  }

  os << std::endl;

  // print the voice measures flat list
  displayVoiceMeasuresFlatList (fieldWidth);

  os << std::endl;

  // print the voice segment
  os <<
    std::setw (fieldWidth) <<
    "***** fVoiceSegment *****" << ": ";
    os << std::endl;

    ++gIndenter;
    os << fVoiceSegment;
    --gIndenter;

  os << std::endl;

  // print the current voice recipient segment
  os <<
    std::setw (fieldWidth) <<
    "***** fVoiceSegment *****" << ": ";
    os << std::endl;

  if (fVoiceSegment) {
      ++gIndenter;
      os << fVoiceSegment;
      --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // print the stanzas if any
  os <<
    std::setw (fieldWidth) <<
    "fVoiceStanzasMap" << ": ";

  if (! fVoiceStanzasMap.empty ()) {
    os << std::endl;

    ++gIndenter;

    for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      os << stanza;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "[EMPTY]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

// void msrVoice::printSlices (std::ostream& os) const JMI
// {
//   os << "MSR voice slices";
//
//   os << std::endl << std::endl;
//
//   ++gIndenter;
//
//   if (! fPartAllStavesList.empty ()) {
//     std::list <S_msrStaff>::const_iterator
//       iBegin = fPartAllStavesList.begin (),
//       iEnd   = fPartAllStavesList.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       (*i)->
//         printSlices (os);
//       if (++i == iEnd) break;
//       os << std::endl;
//     } // for
//   }
//
//   --gIndenter;
// }

std::ostream& operator << (std::ostream& os, const S_msrVoice& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]";
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const msrVoice& elt)
{
  elt.print (os);

  return os;
}

std::string fetchVoiceName (const S_msrVoice& voice)
{
  std::string result;

  if (voice) {
    result = voice->getVoiceName ();
  }
  else {
    result = "\"** VOICE IS NULL **\"";
  }

  return result;
}

std::string fetchVoiceAsShortString (const S_msrVoice& voice)
{
  std::string result;

  if (voice) {
    result =
      voice->asShortString ();
  }
  else {
    result = "\"** VOICE IS NULL **\"";
  }

  return result;
}

std::string fetchVoiceAsString (const S_msrVoice& voice)
{
  std::string result;

  if (voice) {
    result =
      voice->asString ();
  }
  else {
    result = "\"** VOICE IS NULL **\"";
  }

  return result;
}


}

//   switch (fVoiceKind) {//
//     case msrVoiceKind::kVoiceKindRegular:
//     case msrVoiceKind::kVoiceKindDynamics:
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       {
//         ++gIndenter;
//
//         // create the multi-measure rests
//         if (fVoiceCurrentMultiMeasureRest) {
//           std::stringstream ss;
//
//           ss <<
//             "Attempting to create a multi-measure rests while another one is pending";
//
//           msrInternalError (
//             gServiceRunData->getInputSourceName (),
//             inputLineNumber,
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str ());
//         }
//
// /* JMI
//         fVoiceCurrentMultiMeasureRest =
//           msrMultiMeasureRest::create (
//             inputLineNumber,
//             firstRestMeasure->getFullMeasureWholeNotesDuration (),
//             multiMeasureRestMeasuresNumber,
//             this);
// */
//
//          // remember fVoiceCurrentMultiMeasureRest for later next measure number setting // JMI ???
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
//           gLog <<
//             "Registering multi-measure rests " <<
//             fVoiceCurrentMultiMeasureRest->asString () <<
//             " as waiting for its next measure number" <<
//             ", multiMeasureRestMeasuresNumber: " <<
//             multiMeasureRestMeasuresNumber <<
//             " in voice " <<
//             fVoiceName <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         fVoiceMultiMeasureRestsWaitingForItsNextMeasureNumber =
//           fVoiceCurrentMultiMeasureRest;
//
//         fVoiceRemainingMultiMeasureRests =
//           multiMeasureRestMeasuresNumber;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
//           gLog <<
//             "Setting fVoiceRemainingMultiMeasureRests to " <<
//             fVoiceRemainingMultiMeasureRests <<
//             " in voice " <<
//             fVoiceName <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         // create a new segment to collect the multi-measure rests,
//         // containing the first, rest measure
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
//           gLog <<
//             "Creating a new the voice segment to add the first, rest measure in voice " <<
//             fVoiceName <<
//             ", line " << inputLineNumber <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         // this voice contails multi-measure rests
//         this->setVoiceContainsMultiMeasureRests (
//           inputLineNumber);
//
//         // keep the multi-measure rests pending
//
//         --gIndenter;
//       }
//       break;
//   } // switch
