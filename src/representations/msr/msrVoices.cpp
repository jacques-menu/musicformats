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

std::string msrVoiceFinalizationStatusKindAsString (
  msrVoiceCreateInitialLastSegmentKind voiceCreateInitialLastSegmentKind)
{
  std::string result;

  switch (voiceCreateInitialLastSegmentKind) { // JMI ???
    case msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes:
      result = "kCreateInitialLastSegmentYes";
      break;
    case msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentNo:
      result = "kCreateInitialLastSegmentNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrVoiceCreateInitialLastSegmentKind& elt)
{
  os << msrVoiceFinalizationStatusKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrVoice msrVoice::create (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceKind      voiceKind,
  const mfVoiceNumber&     voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
                    voiceCreateInitialLastSegmentKind,
  const S_msrStaff& voiceUpLinkToStaff)
{
  msrVoice* obj =
    new msrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceCreateInitialLastSegmentKind,
      voiceUpLinkToStaff);
  assert (obj != nullptr);
  return obj;
}

S_msrVoice msrVoice::createRegularVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff& voiceUpLinkToStaff)
{
  return
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindRegular,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceUpLinkToStaff);
}

S_msrVoice msrVoice::createHarmoniesVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff& voiceUpLinkToStaff) // unused yet JMI 0.9.66
{
  return
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindHarmonies,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceUpLinkToStaff);
}

S_msrVoice msrVoice::createFiguredBassVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff& voiceUpLinkToStaff) // unused yet JMI0.9.66
{
  return
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindFiguredBass,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceUpLinkToStaff);
}

msrVoice::msrVoice (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceKind      voiceKind,
  const mfVoiceNumber&     voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
                    voiceCreateInitialLastSegmentKind,
  const S_msrStaff& voiceUpLinkToStaff)
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
    voiceCreateInitialLastSegmentKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Creating voice \"" << asString () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrVoice::msrVoice (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber)
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
    msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes); // JMI default value

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Creating voice \"" << asString () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrVoice::~msrVoice ()
{}

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
      "Setting the regular voice staff sequential number of voice \"" <<
      fVoiceName <<
      "\" to " << regularVoiceStaffSequentialNumber;

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
  const mfVoiceNumber& voiceNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Setting the names of '" <<
      fVoiceKind <<
      "' voice from voice number '" << voiceNumber <<
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

void msrVoice::initializeVoice (
  msrVoiceCreateInitialLastSegmentKind
    voiceCreateInitialLastSegmentKind)
{
  // the voice staff sequential number will be set
  // when regular voices are added to a staff
  // in setRegularVoiceStaffSequentialNumber ()
  fRegularVoiceStaffSequentialNumber = -1;

  ++gIndenter;

  // compute voice number
  mfVoiceNumber voiceNumber =
    gMsrOahGroup->getCreateVoicesStaffRelativeNumbers ()
      ? fRegularVoiceStaffSequentialNumber
      : fVoiceNumber;

  // set voice name
  setVoiceNamesFromNumber (
    fInputLineNumber,
    voiceNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Initializing voice \"" << fVoiceName <<
      "\"" <<
      ", voiceNumber: " << voiceNumber <<
      ", in staff \"" <<
      fVoiceUpLinkToStaff->getStaffPathLikeName () <<
      " line " << fInputLineNumber <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // check voice number
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // the voice number should be positive
      if (fVoiceNumber < 1 || fVoiceNumber > 4) {
        std::stringstream ss;

        ss <<
          "regular voice number " << fVoiceNumber <<
          " is not in the 1..4 range";

          /* JMI ???
        msrError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
          */
      }
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

  // multiple measure rests
  fVoiceContainsMultipleMeasureRests = false;
  fVoiceRemainingMultipleMeasureRests = 0;

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

  // create the initial last segment if needed
  switch (voiceCreateInitialLastSegmentKind) {
    case msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes:
      // sanity check // JMI LAST
      mfAssert (
        __FILE__, mfInputLineNumber (__LINE__),
        fVoiceLastSegment == nullptr,
        "fVoiceLastSegment is NULL");

      // create the last segment
      fVoiceLastSegment =
        msrSegment::create (
          fInputLineNumber,
          this);

      if (! fVoiceFirstSegment) {
        fVoiceFirstSegment = fVoiceLastSegment;
      }
      break;
    case msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentNo:
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    gLog <<
      "Initial contents of voice \"" << fVoiceName <<
      "\" in staff \"" <<
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

void msrVoice::changeVoiceIdentity ( // after a deep clone is created
  const mfVoiceNumber& voiceNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Changing the partGroupSequentialNumber of voice \"" <<
      fVoiceName <<
      "\"" <<
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

S_msrVoice msrVoice::createVoiceNewbornClone (
  const S_msrStaff& staffClone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of voice \"" <<
      fVoiceName <<
      "\"";

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

  S_msrVoice
    newbornClone =
      msrVoice::create (
        fInputLineNumber,
        fVoiceKind,
        fVoiceNumber,
        msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentNo,
          // initial segment will be created upon a later segment visit
        staffClone);

  // voice numbers
  newbornClone->fRegularVoiceStaffSequentialNumber =
    fRegularVoiceStaffSequentialNumber;

  newbornClone->fRegularVoiceOrdinalNumberInPart =
    fRegularVoiceOrdinalNumberInPart;

  // voice name
  newbornClone->fVoiceName =
    fVoiceName;

  return newbornClone;
}

S_msrVoice msrVoice::createVoiceDeepClone (
  const mfInputLineNumber& inputLineNumber,
  msrVoiceKind      voiceKind,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff& containingStaff)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Creating a deep clone of voice \"" <<
      fVoiceName <<
      "\"" <<
      ", to be placed in containing staff \"" <<
      containingStaff->getStaffPathLikeName () << "\"" <<
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
      msrVoice::create (
        fInputLineNumber,
        voiceKind,
        voiceNumber,
        msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentNo,
          // the voice initial last segment
          // will be created by deep cloning below
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

  // initial elements
  int numberOfInitialElements =
    fVoiceInitialElementsList.size ();

  if (numberOfInitialElements) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceVoices ()) {
      std::stringstream ss;

      ss <<
        "There are " <<
        numberOfInitialElements <<
        " initial elements in voice \"" <<
        fVoiceName <<
        "\"";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    for (S_msrVoiceElement voiceElement : fVoiceInitialElementsList) {
      // handle the deep clone
      S_msrVoiceElement
        elementDeepClone;

      if (
        S_msrRepeat repeat = dynamic_cast<msrRepeat*>(&(*(voiceElement)))
      ) {
          /* JMI ???
        // create the repeat deep clone
        elementDeepClone =
          repeat->createRepeatDeepClone (
            deepClone);
            */
      }

      else if (
        // create the segment deep clone
        S_msrSegment segment = dynamic_cast<msrSegment*>(&(*(voiceElement)))
      ) {
        elementDeepClone =
          segment->createSegmentDeepClone (
            deepClone);
      }

      else {
        std::stringstream ss;

        ss <<
          "voice  \"" <<
          fVoiceName <<
          "\" initial elements element should be a repeat or a segment";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      // append the element deep clone to the voice deep clone
      deepClone->
        fVoiceInitialElementsList.push_back (
          elementDeepClone);
    } // for
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceVoices ()) {
      std::stringstream ss;

      ss <<
        "There are no initial elements in voice \"" <<
        fVoiceName <<
        "\" to be deep copied";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // last segment
  if (fVoiceLastSegment) { // JMI
    deepClone->fVoiceLastSegment =
      fVoiceLastSegment->
        createSegmentDeepClone (
          deepClone);

    if (! deepClone->fVoiceFirstSegment) {
      deepClone->fVoiceFirstSegment =
        deepClone->fVoiceLastSegment;
    }
  }
  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceVoices ()) {
      std::stringstream ss;

      ss <<
        "There is no last segment in voice \"" <<
        fVoiceName <<
        "\" to be deep copied";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // multiple measure rests
  deepClone->fVoiceContainsMultipleMeasureRests =
    fVoiceContainsMultipleMeasureRests;

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

void msrVoice::setVoiceLastSegmentInVoiceClone (
  const S_msrSegment& segment)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsBasics ()) {
    std::stringstream ss;

    ss <<
      "Setting segment '" <<
      segment->asShortString () <<
      "' as last segment in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << segment->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    segment != nullptr,
    "segment is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fVoiceLastSegment = segment;

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
}

void msrVoice::appendSegmentToVoiceClone ( //JMI unused ???
  const S_msrSegment& segment)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending segment '" <<
      segment->asString () <<
      "' to voice clone \"" <<
      fVoiceName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // does segment belong to a repeat?
  if (! fVoicePendingRepeatsStack.empty ()) {
    // yes

 // JMI   if (fVoiceLastSegment) {

    // append segment to whichever part of the repeat is adequate
    S_msrRepeat
      currentRepeat =
        fVoicePendingRepeatsStack.front ();

    currentRepeat->
      appendSegmentToRepeat (
        segment->getInputLineNumber (),
        segment,
        "appendSegmentToVoiceClone()");
  }

  else {
    // no
  }

  {
/* JMI
    // move fVoiceLastSegment to the initial voice elements list
    moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
      segment->getInputLineNumber (),
      "appendSegmentToVoiceClone()");
      */

    // segment becomes the fVoiceLastSegment
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSegmentsBasics ()) {
      std::stringstream ss;

      ss <<
        "Segment '" <<
        segment->asShortString () <<
        "' becomes the new last segment in voice clone \"" <<
        fVoiceName <<
        "\"";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fVoiceLastSegment = segment;

    if (! fVoiceFirstSegment) {
      fVoiceFirstSegment = fVoiceLastSegment;
    }
  }
}

void msrVoice::setVoiceLastAppendedMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsBasics ()) {
    std::stringstream ss;

    ss <<
      "Setting voice last appended measure to ";

    if (measure) {
      gLog <<
        measure->asString ();
    }
    else {
      gLog << "[NULL]";
    }

    gLog <<
      " in voice " <<
      asString () <<
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastAppendedMeasure = measure;
}

void msrVoice::cascadeNetNextMeasureNumberInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in voice \"" << fVoiceName << "\"" <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fVoiceLastSegment) {
    fVoiceLastSegment->
      setNextMeasureNumberInSegment (
        inputLineNumber,
        nextMeasureNumber);
  }
  else {
    // JMI ??? 0.9.71
  }

  // is there a current multiple measure rests in this voice?
  if (fVoiceMultipleMeasureRestsWaitingForItsNextMeasureNumber) {
    // yes
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
      std::stringstream ss;

      ss <<
        "There is a multiple measure rests waiting for its next measure number" <<
        ", fVoiceRemainingMultipleMeasureRests: " <<
        fVoiceRemainingMultipleMeasureRests <<
        "' in voice \"" <<
        fVoiceName << "\"";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    --fVoiceRemainingMultipleMeasureRests;

    // is this the last measure in the row?
    if (fVoiceRemainingMultipleMeasureRests == 0) {
      // yes, set waiting multiple measure rests's next measure number
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
        std::stringstream ss;

        ss <<
          "Setting multiple measure rests next measure number to '" <<
          nextMeasureNumber <<
          "' in voice \"" <<
          fVoiceName << "\"";

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fVoiceMultipleMeasureRestsWaitingForItsNextMeasureNumber->
        setNextMeasureNumber (
          nextMeasureNumber);

      // forget about this waiting multiple measure rests
      fVoiceMultipleMeasureRestsWaitingForItsNextMeasureNumber = nullptr;
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
      " \"" << fVoiceName << "\"" <<
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
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Measure " <<
      measure->asShortString () <<
      " is the first measure in  voice \"" << fVoiceName << "\"" <<
      "', line " << measure->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceFirstMeasure = measure;
}

void msrVoice::appendMeasureCloneToVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& measureClone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Appending measure clone '" <<
      measureClone->asShortString () <<
      "' to voice '" << fVoiceName << "\"" <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append measureClone to the voice last segment
  fVoiceLastSegment->
    appendMeasureToSegment (measureClone);

  // measureClone is the new voice last appended measure
  setVoiceLastAppendedMeasure (
    measureClone);

  --gIndenter;
}

void msrVoice::setWholeNotesSinceLastRegularMeasureEnd (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting voice whole notes since last regular measure end to '" <<
      wholeNotes.asString () <<
  // JMI    "' (" << context << ")" <<
      " in voice \"" << fVoiceName << "\"" <<
      "', line " << inputLineNumber;

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
      "' should be positive or null");

  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    wholeNotes.getDenominator () > 0,
    "mfWholeNotes denominator '" +
      std::to_string (wholeNotes.getDenominator ()) +
      "' should be positive");
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
      "Setting voice current after repeat component phase kind to '" <<
      msrVoiceRepeatPhaseKindAsString (
        afterRepeatComponentPhaseKind) <<
 // JMI     "' (" << context << ")" <<
      " in voice \"" << fVoiceName << "\"" <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceRepeatPhaseKind =
    afterRepeatComponentPhaseKind;
}

void msrVoice::createNewLastSegmentForVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a new last segment for voice \"" <<
      fVoiceName <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    std::string
      combinedContext =
        "createNewLastSegmentForVoice() 1 called from " + context;

    displayVoice (
      inputLineNumber,
      combinedContext);
  }
#endif // MF_TRACE_IS_ENABLED

  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    std::string
      combinedContext =
        "createNewLastSegmentForVoice() 2 called from " + context;

    displayVoice (
      inputLineNumber,
      combinedContext);
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::createNewLastSegmentFromItsFirstMeasureForVoice (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& firstMeasure,
  const std::string&  context)
{
  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a new last segment '" <<
      fVoiceLastSegment->asShortString () <<
      "' from its first measure '" <<
      firstMeasure->getMeasureNumber () <<
      "' for voice \"" <<
      fVoiceName <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append firstMeasure to fVoiceLastSegment
  fVoiceLastSegment->
    appendMeasureToSegment (firstMeasure);

  // firstMeasure is the new voice last appended measure
  setVoiceLastAppendedMeasure (
    firstMeasure);

  // is firstMeasure the first one it the voice?
  if (! fVoiceFirstMeasure) {
    // yes, register it as such
    setVoiceFirstMeasure (
      firstMeasure);

    firstMeasure->
      setMeasureIsFirstInVoice ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createNewLastSegmentFromItsFirstMeasureForVoice()");
  }
#endif // MF_TRACE_IS_ENABLED
}

S_msrMeasure msrVoice::cascadeCreateAMeasureAndAppendItInVoice (
  const mfInputLineNumber& inputLineNumber,
  int                    previousMeasureEndInputLineNumber,
  const mfMeasureNumber& measureNumber,
  msrMeasureImplicitKind measureImplicitKind)
{
  S_msrMeasure result;

  fVoiceCurrentMeasureNumber = measureNumber;

  fCallsCounter++;

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating measure '" <<
//       measureNumber <<
//       "' and appending it to voice \"" << fVoiceName << "\"" <<
//       "', line " << inputLineNumber;
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
      "++++ cascadeCreateAMeasureAndAppendItInVoice() POUSSE, fCallsCounter: " << fCallsCounter << " ++++" <<
      std::endl;
    this->print (gLog);
    gLog <<
      std::endl;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "cascadeCreateAMeasureAndAppendItInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // is there an on-going multiple measure rests?
  if (fOnGoingMultipleMeasureRest) {
    // yes

    // create a measure
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "Creating a measure with number '" <<
        measureNumber <<
        "' in voice \"" << fVoiceName << "\"" <<
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
        fVoiceLastSegment); // JMI 0.9.67  0.9.63 ???

    // set result's ordinal number
    result->
      setMeasureOrdinalNumberInVoice (
        incrementVoiceCurrentMeasureOrdinalNumber ());

    // append it to the current multiple measure rests
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "Appending measure " <<
        result->asString () <<
        " to current multiple measure rests " <<
        fVoiceCurrentMultipleMeasureRest->asString () <<
        "in voice \"" <<
        fVoiceName << "\"" <<
        "', line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fVoiceCurrentMultipleMeasureRest->
      appendMeasureToMultipleMeasureRest (
        result);
  }

  else {
    // no

    // create the voice last segment if needed
//     if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    if (! fVoiceLastSegment) { // JMI 0.9.73 WHY CAN IT BE NULL ???
      createNewLastSegmentForVoice (
        inputLineNumber,
        "cascadeCreateAMeasureAndAppendItInVoice() 2");
    }

    // append a new measure with given number to voice last segment
    result =
      fVoiceLastSegment->
        cascadeCreateAMeasureAndAppendItInSegment (
          inputLineNumber,
          previousMeasureEndInputLineNumber,
          measureNumber,
          measureImplicitKind);
  }

  // result is the new voice last appended measure
  setVoiceLastAppendedMeasure (
    result);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "cascadeCreateAMeasureAndAppendItInVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  return result;
}

S_msrVoice msrVoice::createRegularVoiceHarmoniesVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   currentMeasureNumber)
{
  if (fRegularVoiceForwardLinkToHarmoniesVoice) {
    std::stringstream ss;

    ss <<
      "Voice \"" <<
      fVoiceName <<
      "\" already has a harmonies voice";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // create the voice harmonies voice
  mfVoiceNumber
    regularVoiceHarmoniesVoiceNumber =
      fVoiceNumber + K_MF_VOICE_HARMONIES_VOICE_BASE_NUMBER; // JMI 0.9.75

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Creating harmonies voice for regular voice \"" <<
      fVoiceName <<
      "\" with voice number " <<
      regularVoiceHarmoniesVoiceNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fRegularVoiceForwardLinkToHarmoniesVoice =
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindHarmonies,
      regularVoiceHarmoniesVoiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
      fVoiceUpLinkToStaff);

  // register it in the staff
  fVoiceUpLinkToStaff->
    registerVoiceInStaff (
      inputLineNumber,
      fRegularVoiceForwardLinkToHarmoniesVoice);

  // set backward link
  fRegularVoiceForwardLinkToHarmoniesVoice->
    fHarmoniesVoiceRegularVoiceBackwardLink = this;

  return fRegularVoiceForwardLinkToHarmoniesVoice;
}

// S_msrVoice msrVoice::createRegularVoiceFiguredBassVoice (
//   const mfInputLineNumber& inputLineNumber,
//   const mfMeasureNumber&   currentMeasureNumber)
// {
//   if (fRegularVoiceForwardLinkToFiguredBassVoice) {
//     std::stringstream ss;
//
//     ss <<
//       "Voice \"" <<
//       fVoiceName <<
//       "\" already has a figured bass voice";
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
//       "Creating figured bass voice for regular voice \"" <<
//       fVoiceName <<
//       "\" with voice number " <<
//       regularVoiceFiguredBassVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fRegularVoiceForwardLinkToFiguredBassVoice =
//     msrVoice::create (
//       inputLineNumber,
//       msrVoiceKind::kVoiceKindFiguredBass,
//       regularVoiceFiguredBassVoiceNumber,
//       msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
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
      " already exists in voice \"" <<
      fVoiceName << "\"" <<
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
      ") to voice \"" << fVoiceName << "\"";

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
      ") to voice \"" << fVoiceName << "\"";

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
        ", name \"" << stanzaName << "\"" <<
        ", in voice \"" << fVoiceName << "\"" <<
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
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      std::endl;

    print (gLog);

    gLog <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      std::endl << std::endl << std::endl <<
      std::endl;


    ss <<
      "Stanza" <<
      " number " << stanzaNumber <<
      ", name \"" << stanzaName << "\"" <<
      ", not found in voice \"" << fVoiceName << "\"" <<
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

void msrVoice::appendMusicXMLPrintLayoutToVoice (
  const S_msrMusicXMLPrintLayout& musicXMLPrintLayout)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
    std::stringstream ss;

    ss <<
      "Appending print layout '" << musicXMLPrintLayout->asString () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  fVoiceLastSegment->
    appendMusicXMLPrintLayoutToSegment (musicXMLPrintLayout);

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
      " to voice \"" << fVoiceName << "\"";

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

//   if (fVoiceIsMusicallyEmpty) {
//     // append clefKeyTimeSignatureGroup to last segment
//     fVoiceLastSegment->
//       appendClefKeyTimeSignatureGroupToSegment (
//         clefKeyTimeSignatureGroup);
//   }
//
//   else {
//     // moving clefKeyTimeSignatureGroups to the left, thus prepend to last segment
//     fVoiceLastSegment->
// //       prependClefKeyTimeSignatureGroupToSegment (clefKeyTimeSignatureGroup);
//       appendClefKeyTimeSignatureGroupToSegment (
//         clefKeyTimeSignatureGroup); // JMI 0.9.67
//   }

  // create the voice last segment if needed
  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    createNewLastSegmentForVoice (
      clefKeyTimeSignatureGroup->getInputLineNumber (),
      "appendClefKeyTimeSignatureGroupToVoice()");
  }
  // append clefKeyTimeSignatureGroup to last segment
  fVoiceLastSegment->
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
//       " to voice \"" << fVoiceName << "\"" <<
//       std::endl;
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
//     // append clef to last segment
//     fVoiceLastSegment->
//       appendClefToSegment (clef);
//   }
//
//   else {
//     // moving clefs to the left, thus prepend to last segment
//     fVoiceLastSegment->
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
//       " to voice \"" << fVoiceName << "\"" <<
//       std::endl;
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
//   // append key to last segment
//   fVoiceLastSegment->
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
      " to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set voice current timeSignature
  this->setVoiceCurrentTimeSignature (timeSignature);

  // append timeSignature to the last segment
  fVoiceLastSegment->
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
      " to voice clone \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set voice current time signature
  this->setVoiceCurrentTimeSignature (timeSignature);

  // append timeSignature to the last segment
  fVoiceLastSegment->
    appendTimeSignatureToSegmentClone (timeSignature);

  --gIndenter;
}

void msrVoice::insertHiddenMeasureAndBarLineInVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const mfPositionInMeasure& positionInMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Inserting hidden measure and barLine at position " <<
      positionInMeasure.asString () <<
      "' to voice clone \"" << fVoiceName << "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // insert hidden measure and barLine to the last segment
  fVoiceLastSegment->
    insertHiddenMeasureAndBarLineInSegmentClone (
      inputLineNumber,
      positionInMeasure);

  --gIndenter;
}

S_msrNote msrVoice::fetchVoiceFirstNonGraceNote () const
{
  S_msrNote result;

  if (fVoiceFirstSegment) {
    // get the voice's first measure
    S_msrMeasure
      voiceFirstMeasure =
        fVoiceFirstSegment->
          getSegmentFirstMeasure ();

    if (voiceFirstMeasure) {
      // get the first measure's elements list
      const std::list <S_msrMeasureElement>&
        firstMeasureElementsList =
          voiceFirstMeasure->
            getMeasureElementsList ();

      // fetch the first note in the first measure to which
      // a grace notes group can be attached
      // i.e. one not in a grace notes group itself,
      // possibly inside a chord or tuplet

      if (! firstMeasureElementsList.empty ()) {
        for (S_msrMeasureElement measureElement : firstMeasureElementsList) {
          if (
            S_msrNote note = dynamic_cast<msrNote*>(&(*measureElement))
          ) {
            result = note;
            break;
          }

          else if (
            S_msrChord chord = dynamic_cast<msrChord*>(&(*measureElement))
          ) {
            // get the chord's first note
            result = chord->fetchChordFirstNonGraceNote ();
            break;
          }

          else if (
            S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*measureElement))
          ) {
            // get the tuplet's first note
            result = tuplet->fetchTupletFirstNonGraceNote ();
            break;
          }

          else if (
            S_msrRepeat repeat = dynamic_cast<msrRepeat*>(&(*measureElement))
          ) {
            // get the repeat's first note
            result = repeat->fetchRepeatFirstNonGraceNote ();
            break;
          }

          else if (
            S_msrClef clef = dynamic_cast<msrClef*>(&(*measureElement))
          ) {
            // ignore this clef
          }

          else if (
            S_msrKey key = dynamic_cast<msrKey*>(&(*measureElement))
          ) {
            // ignore this key
          }

          else if (
            S_msrTimeSignature
              timeSignature =
                dynamic_cast<msrTimeSignature*>(&(*measureElement))
          ) {
            // ignore this timeSignature
          }

          else {
            // ignore this measureElement and return nullptr JMI ???
            /*
            std::stringstream ss;

            ss <<
              "tuplet first measureElement should be a note, a chord or another tuplet, found instead '" <<
              measureElement->asShortString () <<
              "'";

            msrInternalError (
              gServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
              */
          }
        } // for
      }
    }
  }

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
//         "there is no first measure in the first segment in voice \"" << // JMI 0.9.63  // JMI 0.9.66
//         fVoiceName <<
//         "\", since the latter is empty";
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
        "The new shortest note wholeNotes in voice \"" <<
        fVoiceName <<
        "\" becomes " <<
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
        "The new shortest note in voice \"" << fVoiceName << "\"" <<
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

void msrVoice::cascadeAppendHarmonyToVoice (
  const mfInputLineNumber& inputLineNumber,
  const S_msrHarmony&        harmony,
  const mfPositionInMeasure& positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " << harmony->asString () <<
      " to voice \"" << fVoiceName << "\"" <<
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
          " voice \"" <<
          fVoiceName <<
          "\"";

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

  // append the harmony to the voice last segment
  fVoiceLastSegment->
    appendHarmonyToSegment (
      inputLineNumber,
      harmony,
      positionInMeasureToAppendAt);

  --gIndenter;

  // register harmony
  ++fVoiceActualHarmoniesCounter;


  fVoiceIsMusicallyEmpty = false;
}

void msrVoice::appendHarmoniesListToVoice (
  const mfInputLineNumber&        inputLineNumber,
  const std::list <S_msrHarmony>& harmoniesList,
  const mfPositionInMeasure&      positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmonies list to voice \"" << // JMI 0.9.67 HARMFUL
      fVoiceName <<
      "\"" <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the harmonies to the voice last segment
  fVoiceLastSegment->
    appendHarmoniesListToSegment (
      inputLineNumber,
      harmoniesList,
      positionInMeasureToAppendAt);
}

void msrVoice::cascadeAppendHarmonyToVoiceClone (
  const S_msrHarmony& harmony)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " << harmony->asString () <<
      " to voice clone \"" << fVoiceName << "\"" <<
      ", line " << harmony->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindHarmonies:
      fVoiceLastSegment->
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
      " to voice \"" << fVoiceName << "\"" <<
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
          " voice \"" <<
          fVoiceName <<
          "\"";

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

  // append figuredBass to the voice last segment
  fVoiceLastSegment->
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
//       figuredBasssesList->asString () << // JMI 0.9.67 HARMFUL
      " to voice \"" << fVoiceName << "\"" <<
      "\"" <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the figured basses to the voice last segment
  fVoiceLastSegment->
    cascadeAppendFiguredBassesListToSegment (
      inputLineNumber,
      figuredBasssesList,
      positionInMeasureToAppendAt);
}

void msrVoice::appendFiguredBassToVoiceClone (
  const S_msrFiguredBass& figuredBass)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass " << figuredBass->asString () <<
      " to voice clone \"" << fVoiceName << "\"" <<
      ", line " << figuredBass->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      fVoiceLastSegment->
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
//   const mfWholeNotes& wholeNotesPositionInMeasure)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePositionInMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Padding up to measure position '" <<
//       wholeNotesPositionInMeasure.asString () <<
//       "' whole notes in voice \"" <<
//       fVoiceName <<
//       "\", line " << inputLineNumber <<
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
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     fVoiceLastSegment != nullptr,
//     "fVoiceLastSegment is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // pad up the voice's last segment
//   fVoiceLastSegment->
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
//   const mfInputLineNumber& inputLineNumber,
//   const mfWholeNotes& backupTargetMeasureElementPositionInMeasure)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceDurations ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Backup by a '" <<
//       backupTargetMeasureElementPositionInMeasure.asString () <<
//       "' whole notes step length in voice \"" <<
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
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     fVoiceLastSegment != nullptr,
//     "fVoiceLastSegment is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // pad up the voice's last segment
//   fVoiceLastSegment->
//     casadeBackupByWholeNotesStepLengthInSegment (
//       inputLineNumber,
//       backupTargetMeasureElementPositionInMeasure);
//
//   --gIndenter;
// }

void msrVoice::cascadeAppendPaddingNoteToVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& forwardStepLength)
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
      ", to voice \"" <<
      fVoiceName <<
      "\",line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // pad up the voice's last segment
  fVoiceLastSegment->
    cascadeAppendPaddingNoteToSegment (
      inputLineNumber,
      forwardStepLength);

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
      "Appending transposition '" <<
      transposition->asString () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
    appendTranspositionToSegment (transposition);
}

void msrVoice::appendStaffDetailsToVoice (
  const S_msrStaffDetails& staffDetails)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // staff details are attributes in MusicXML,
  // create the voice last segment if needed
  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    createNewLastSegmentForVoice (
      staffDetails->getInputLineNumber (),
      "appendStaffDetailsToVoice()");
  }

  fVoiceLastSegment->
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
      " to voice \"" <<
      fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
    appendTempoToSegment (tempo);
}

void msrVoice::appendOctaveShiftToVoice (
  const S_msrOctaveShift& octaveShift)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceOctaveShifts ()) {
    std::stringstream ss;

    ss <<
      "Appending octave shift '" <<
      octaveShift->getOctaveShiftKind () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
    appendOctaveShiftToSegment (octaveShift);
}

void msrVoice::appendScordaturaToVoice (
  const S_msrScordatura& scordatura)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceScordaturas ()) {
    std::stringstream ss;

    ss <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
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
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
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
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
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
      "Appending rehearsalMark '" << rehearsalMark->getRehearsalMarkText () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
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
      "\"" << fVoiceName << "\"" <<
      ", line " << voiceStaffChange->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append voice staff change to voice's last segment
  fVoiceLastSegment->
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
      " to voice \"" << fVoiceName << "\"" <<
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

  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    std::stringstream ss;

    ss <<
      "fVoiceLastSegment is null in appendNoteToVoice()";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      note->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotesDetails ()) {
      this->displayVoice (
        note->getInputLineNumber (),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // append the note to the last segment
  fVoiceLastSegment->
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
      " to voice clone \"" << fVoiceName << "\"" <<
      ", line " << note->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the note to the last segment
  fVoiceLastSegment->
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
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
    appendDoubleTremoloToSegment (doubleTremolo);

  fVoiceIsMusicallyEmpty = false;
}

void msrVoice::appendChordToVoice (const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    gLog <<
      "appendChordToVoice(): Appending chord:" <<
      std::endl;

    ++gIndenter;
    chord->print (gLog);
    --gIndenter;

    gLog <<
      "to voice \"" << fVoiceName << "\"" <<
      std::endl << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // append chord to voice last segment
  fVoiceLastSegment->
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
      " to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append tuplet to voice last segment
  fVoiceLastSegment->
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
      " ahead of voice if needed in \"" << fVoiceName << "\"";

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

  if (! fVoiceFirstSegment) {
    // these graceNotesGroup appears at the beginning of the voice:
    // create a first segment
    createNewLastSegmentForVoice (
      graceNotesGroup->getInputLineNumber (),
      "addGraceNotesGroupBeforeAheadOfVoiceIfNeeded()");

    fVoiceFirstSegment = fVoiceLastSegment;

    // then create the first measure
    cascadeCreateAMeasureAndAppendItInVoice (
      graceNotesGroup->getInputLineNumber (),
      333, //         previousMeasureEndInputLineNumber, 0.9.62
      graceNotesGroup->
        getGraceNotesGroupMeasureNumber (),
      msrMeasureImplicitKind::kMeasureImplicitKindNo);
  }

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
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first chord of voice \"" << fVoiceName <<
        "\", i.e. '" <<
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
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first note of voice \"" << fVoiceName <<
        "\", i.e. '" <<
        voiceFirstNote->asShortString () <<
        "'";

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
      " to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
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
      " to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceFirstSegment->
    prependAfterGraceNotesToSegment (afterGraceNotes);

  fVoiceIsMusicallyEmpty = false;
}
*/

void msrVoice::appendSyllableToVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfStanzaNumber& stanzaNumber,
  const std::string&   stanzaName,
  const S_msrSyllable& syllable)
{
  // append syllable to this voice
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending syllable " <<
      syllable->asString () <<
      " to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch stanzaNumber in this voice
  S_msrStanza
    stanza =
      createStanzaInVoiceIfNotYetDone (
        inputLineNumber,
        stanzaNumber,
        stanzaName);

  // add the syllable to the stanza clone
  stanza->
    appendSyllableToStanzaClone (
      syllable,
      fVoiceLastAppendedMeasure);
}

void msrVoice::appendBarCheckToVoice (
  const S_msrBarCheck& barCheck)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Appending bar check " <<
      barCheck->asString () <<
      " to voice \"" <<
      fVoiceName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
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
      " to voice \"" << fVoiceName <<  "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
    appendBarNumberCheckToSegment (barNumberCheck);
}

void msrVoice::appendLineBreakToVoice  (
  const S_msrLineBreak& lineBreak)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Appending line break '" << lineBreak->asString () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
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
      "Appending page break '" << pageBreak->asString () <<
      "' to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
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
//       "Prepending other element '" << elem <<
//       "' to voice \"" << fVoiceName << "\"" <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fVoiceLastSegment->
//     prependOtherElementToSegment (elem);
// }

// void msrVoice::appendOtherElementToVoice (const S_msrMeasureElement& elem) {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceVoices ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending other element '" << elem <<
//       "' to voice \"" << fVoiceName << "\"" <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fVoiceLastSegment->
//     appendOtherElementToSegment (elem);
// }

S_msrMeasure msrVoice::fetchVoiceLastMeasure (
  const mfInputLineNumber& inputLineNumber) const
{
  S_msrMeasure result;

  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is NULL");

  result =
    fVoiceLastSegment->
      getSegmentLastMeasure (); // JMI 0.9.63

//   else { JMI 0.9.63
//     std::stringstream ss;
//
//     ss <<
//       "attempt at fetching voice last measure in an empty measure elements list";
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
      "attempt at fetching voice last element in an empty elements list";

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
  const S_msrRepeat& repeat,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Pushing repeat ***** " <<
      repeat->asShortString () <<
      " onto the repeats stack in voice \"" <<
      fVoiceName <<
      "\"" <<
      " from context " + context <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoicePendingRepeatsStack.push_front (
    repeat);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::string
      combinedContext =
        "pushRepeatOntoVoiceRepeatsStack() called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::popRepeatFromVoiceRepeatsStack (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Popping repeat ***** 1 " <<
//       repeat->asShortString () <<
      " from the repeats stack in voice \"" <<
      fVoiceName <<
      "\"" <<
      " from context " + context <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::string
      combinedContext =
        "popRepeatFromVoiceRepeatsStack() 1 called from context " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif // MF_TRACE_IS_ENABLED

  if (
    repeat
      !=
    fVoicePendingRepeatsStack.front ()
  ) {
    std::stringstream ss;

    ss <<
      "cannot pop repeat '" <<
      repeat->asShortString () <<
      "' from the stack since it is not at the top" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Popping repeat ***** 2 " <<
      repeat->asString () <<
      " from the repeat stack in voice \"" <<
      fVoiceName <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // pop it from repeats stack
  fVoicePendingRepeatsStack.pop_front ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::string
      combinedContext =
        "popRepeatFromVoiceRepeatsStack() 2 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::displayVoiceRepeatsStack (
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
    asShortString () <<
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
        repeat->asString ();
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLog << std::endl;
    } // for

    --gIndenter;
  }

  gLog <<
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
    " - " << context <<
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
        repeat->asString ();
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
}

void msrVoice::displayVoiceRepeatsStackAndVoice ( // JMI never used ???
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  displayVoiceRepeatsStack (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

void msrVoice::displayVoiceMeasureRepeat (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  gLog <<
    std::endl <<
    ">>++++++++++++++++ Displaying voice measure repeats " << context <<
    std::endl <<
    "The current voice measures repeat contains " <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  ++gIndenter;

  if (fVoicePendingMeasureRepeat) {
    fVoicePendingMeasureRepeat->
      displayMeasureRepeat (
        inputLineNumber,
        context);
  }
  else {
    gLog <<
      "[NULL]" <<
      std::endl;
  }

  --gIndenter;

  gLog <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

void msrVoice::displayVoiceMeasureRepeatAndVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  displayVoiceMeasureRepeat (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

// 0.9.62
//   // print the voice measures flat list
//   size_t voiceMeasuresFlatListSize =
//     fVoiceMeasuresFlatList.size ();
//
//   os <<
//     std::setw (fieldWidth) <<
//     "fVoiceMeasuresFlatList";
//   if (voiceMeasuresFlatListSize) {
//     os <<
//       mfSingularOrPlural (
//         voiceMeasuresFlatListSize, "element", "elements");
//   }
//   else {
//     os <<
//       ": " << "[EMPTY]";
//   }
//   os << std::endl;
//
//   if (voiceMeasuresFlatListSize) {
//     ++gIndenter;
//
//     std::list <S_msrMeasure>::const_iterator
//       iBegin = fVoiceMeasuresFlatList.begin (),
//       iEnd   = fVoiceMeasuresFlatList.end (),
//       i      = iBegin;
//
//     for ( ; ; ) {
//       S_msrMeasure measure = (*i);
//
//       // print the measure
//       if (gTraceOahGroup->getTraceMeasures ()) {
//         os <<
//           measure->asStringForMeasuresSlices ();
//       }
//       else {
//         os <<
//           measure->getMeasureNumber ();
//       }
//       if (++i == iEnd) break;
//       os << std::endl;
//     } // for
//     os << std::endl;
//
//     --gIndenter;
//   }

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

void msrVoice::displayVoiceMultipleMeasureRests (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  gLog <<
    std::endl <<
    ">>++++++++++++++++ voice multiple measure rests " << context <<
    std::endl <<
    "The current voice multiple measure rests contains " <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  ++gIndenter;

  if (fVoiceCurrentMultipleMeasureRest) {
    fVoiceCurrentMultipleMeasureRest->
      displayMultipleMeasureRest (
        inputLineNumber,
        context);
  }
  else {
    gLog <<
      "[NULL]" <<
      std::endl;
  }

  --gIndenter;

  gLog <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

void msrVoice::displayVoiceMultipleMeasureRestsAndVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  displayVoiceMultipleMeasureRests (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

S_msrRepeat msrVoice::createARepeatAndStackIt (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat and stacking it in voice \"" <<
      fVoiceName <<
      "\"" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeat
    result =
      msrRepeat::create (
        inputLineNumber,
        2, // repeatTimes, default value JMI
        this);

  // push it onto the voice's repeat descrs stack
  pushRepeatOntoVoiceRepeatsStack (
    inputLineNumber,
    result,
    "createARepeatAndStackIt() 1");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "createARepeatAndStackIt() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

S_msrRepeat msrVoice::createARepeatCloneAndStackIt (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat clone and stacking it in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeat
    result =
      repeat->
        createRepeatNewbornClone (this);

  // push it onto the voice's repeat descrs stack
  pushRepeatOntoVoiceRepeatsStack (
    inputLineNumber,
    result,
    "createARepeatCloneAndStackIt()");

  return result;
}

void msrVoice::moveVoiceInitialElementsToRepeatCommonPart (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeatCommonPart& repeatCommonPart,
  const std::string&           context)
{
  // move the voice initial elements to the new repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Moving the " <<
      mfSingularOrPlural (
        fVoiceInitialElementsList.size (),
        "initial element",
        "initial elements") <<
      " in voice \"" <<
      fVoiceName <<
      "\" to repeat common part '" <<
      repeatCommonPart->asShortString () <<
      "' (" << context << ")" <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list <S_msrVoiceElement>::iterator i = fVoiceInitialElementsList.begin ();
    i != fVoiceInitialElementsList.end ();
    ++i
  ) {
    S_msrVoiceElement element = (*i);

    // append the element to the new segment
    repeatCommonPart->
      appendVoiceElementToRepeatCommonPart (
        inputLineNumber,
        element,
        "moveVoiceInitialElementsToRepeatCommonPart()");

    // remove it from the voice initial elements
    i = fVoiceInitialElementsList.erase (i);
  } // for
}

void msrVoice::moveVoiceLastSegmentToRepeatCommonPart (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeatCommonPart& repeatCommonPart,
  const std::string&           context)
{
  // move the voice last segment to repeatCommonPart
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Moving the voice last segment ";

    if (fVoiceLastSegment) { // JMI should not be necessary?
      ss <<
        fVoiceLastSegment->asShortString ();
    }
    else {
      ss <<
        "[NULL]";
    }

    ss <<
      " to repeat common part '" <<
      repeatCommonPart->asShortString () <<
      "' (" << context << ")" <<
      "' in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    std::string
      combinedContext =
        "moveVoiceLastSegmentToRepeatCommonPart() 1 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif // MF_TRACE_IS_ENABLED

  if (fVoiceLastSegment) { // JMI should not be necessary?
  /* JMI
    // fetch the last segment's last measure // JMI ???
    std::string
      combinedContext =
        "moveVoiceLastSegmentToRepeatCommonPart() 2 called from " + context;

    S_msrMeasure
      voiceLastSegmentLastMeasure =
        fVoiceLastSegment->
          fetchLastMeasureFromSegment (
            inputLineNumber,
            combinedContext);
*/

    // set voice current after repeat component phase kind
    setCurrentVoiceRepeatPhaseKind (
      inputLineNumber,
      msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);

    // append fVoiceLastSegment to the repeat common part
    repeatCommonPart->
      appendSegmentToRepeatCommonPart (
        inputLineNumber,
        fVoiceLastSegment,
        context);

    // forget about this voice last segment
    fVoiceLastSegment = nullptr;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    std::string
      combinedContext =
        "moveVoiceLastSegmentToRepeatCommonPart() 3 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::moveAllVoiceContentsToRepeatCommonPart (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeatCommonPart& repeatCommonPart,
  const std::string&           context)
{
  moveVoiceInitialElementsToRepeatCommonPart (
    inputLineNumber,
    repeatCommonPart,
    context);

  moveVoiceLastSegmentToRepeatCommonPart (
    inputLineNumber,
    repeatCommonPart,
    context);
}

void msrVoice::moveVoiceLastSegmentToRepeatEnding (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeatEnding& repeatEnding,
  const std::string&       context)
{
  // move the voice last segment to repeatEnding
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Moving the voice last segment to repeat ending '" <<
      repeatEnding->asShortString () <<
      "' in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fVoiceLastSegment) { // JMI should not be necessary?
    repeatEnding->
      appendSegmentToRepeatEnding (
        inputLineNumber,
        fVoiceLastSegment,
        context);

    // forget about this voice last segment
    fVoiceLastSegment = nullptr;
  }
}

void msrVoice::appendRepeatToInitialVoiceElements (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
  // append repeat to the list of initial elements
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat '" <<
      repeat->asString () <<
      "' to the initial elements in voice \"" <<
      fVoiceName <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is the previous element in the voice elements list a repeat?
  S_msrVoiceElement previousElement;

  if (! fVoiceInitialElementsList.empty ()) {
    previousElement =
      fVoiceInitialElementsList.back ();
  }

  if (previousElement) {
    if (
      S_msrRepeat previousRepeat = dynamic_cast<msrRepeat*>(&(*previousElement))
    ) {
      // set the respective immediately preceding and following repeats
      previousRepeat->
        setImmediatelyFollowingRepeat (
          repeat);
      repeat->
        setImmediatelyPrecedingRepeat (
          previousRepeat);
    }
  }

  // do append the repeat
  fVoiceInitialElementsList.push_back (
    repeat);
}

// void msrVoice::cascadeAppendMultipleMeasureRestToInitialVoiceElements (
//   const mfInputLineNumber&        inputLineNumber,
//   const S_msrMultipleMeasureRest& multipleMeasureRest,
//   const std::string&              context)
// {
//   // append multipleMeasureRest to the list of initial elements
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
//     gLog <<
//       "Appending multiple measure rest " <<
//       multipleMeasureRest->asString () <<
//       " to the initial elements in voice \"" <<
//       fVoiceName <<
//       "\" (" << context << ")" <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// //   fVoiceInitialElementsList.push_back ( JMI 0.9.67
// //     multipleMeasureRest);
// }

void msrVoice::appendMeasureRepeatToInitialVoiceElements (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat,
  const std::string&        context)
{
  // append measureRepeat to the list of initial elements
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to the initial elements in voice \"" <<
      fVoiceName <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   fVoiceInitialElementsList.push_back ( JMI 0.9.67
//     measureRepeat);
}

void msrVoice::appendVoiceLastSegmentToInitialVoiceElements (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  // append segment to the list of initial elements
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending voice last segment '" <<
      fVoiceLastSegment->asString () <<
      "' to the initial elements in voice \"" <<
      fVoiceName <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceInitialElementsList.push_back (
    fVoiceLastSegment);
}

void msrVoice::moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  if (fVoiceLastSegment) {
    const std::list <S_msrSegmentElement>&
      segmentElementsList =
        fVoiceLastSegment->
          getSegmentElementsList ();

    if (! segmentElementsList.empty ()) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceSegmentsBasics ()) {
        std::stringstream ss;

        ss <<
          "Moving voice last segment '" <<
          fVoiceLastSegment->asString () <<
          "' to the initial elements in voice \"" <<
          fVoiceName <<
          "\" (" << context << ")" <<
          ", line " << inputLineNumber;

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (gTraceOahGroup->getTraceSegmentsDetails ()) {
        displayVoice (
          inputLineNumber,
          "moveVoiceLastSegmentToInitialVoiceElementsIfRelevant() 1");
      }
#endif // MF_TRACE_IS_ENABLED

      // append segment to the list of initial elements
      fVoiceInitialElementsList.push_back (
        fVoiceLastSegment);

      // forget about this last segment
      fVoiceLastSegment = nullptr;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceSegmentsDetails ()) {
        displayVoice (
          inputLineNumber,
          "moveVoiceLastSegmentToInitialVoiceElementsIfRelevant() 2");
      }
#endif // MF_TRACE_IS_ENABLED
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceSegmentsBasics ()) {
        std::stringstream ss;

        ss <<
          "Voice last segment '" <<
          fVoiceLastSegment->asString () <<
          "' contains no measure, not moved to the initial elements in voice \"" <<
          fVoiceName <<
          "\" (" << context << ")" <<
          ", line " << inputLineNumber;

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSegmentsBasics ()) {
      std::stringstream ss;

      ss <<
        "Voice last segment '" <<
        fVoiceLastSegment->asString () <<
        "' is null, not moved to the initial elements in voice \"" <<
        fVoiceName <<
        "\" (" << context << ")" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

void msrVoice::appendRepeatCloneToInitialVoiceElements (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat& repeatCLone,
  const std::string& context)
{
  // append repeatCLone to the list of initial elements
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat cLone '" <<
      repeatCLone->asString () <<
      "' to the initial elements in voice \"" <<
      fVoiceName <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceInitialElementsList.push_back (
    repeatCLone);
}

void msrVoice::handleVoiceLevelRepeatStart (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat start in voice \"" <<
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

  // is there a voice last segment?
  if (fVoiceLastSegment) {

    // fetch last segment's measure elements list
    std::list <S_msrSegmentElement>
      voiceLastSegmentElementsList =
        fVoiceLastSegment->
          getSegmentElementsList ();

    // are there measures in the voice last segment?
    if (! voiceLastSegmentElementsList.empty ()) {
      // yes

      // fetch last measure in the last segment
      S_msrMeasure
        lastMeasureInLastSegment =
          fVoiceLastSegment->
            getSegmentLastMeasure (); // JMI 0.9.63

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceRepeatsDetails ()) {
        lastMeasureInLastSegment->
          displayMeasure (
            inputLineNumber,
            "lastMeasureInLastSegment - handleVoiceLevelRepeatStart() 2");
      }
#endif // MF_TRACE_IS_ENABLED

      // let's look at the last measure in detail

      // fetch its elements list
      const std::list <S_msrMeasureElement>&
        lastMeasureElementsList =
          lastMeasureInLastSegment->
            getMeasureElementsList ();

      if (! lastMeasureElementsList.empty ()) {
        // the last measure is not empty

        mfPositionInMeasure
          measureCurrentPositionInMeasure =
            lastMeasureInLastSegment->
              getMeasureCurrentPositionInMeasure ();

        mfWholeNotes
          fullMeasureWholeNotesDuration =
            lastMeasureInLastSegment->
              getFullMeasureWholeNotesDuration ();

        // is there a measure splitting?
        if ( // JMI better criterion???
          measureCurrentPositionInMeasure.getNumerator () > 0
            &&
          measureCurrentPositionInMeasure.asWholeNotes ()
            <
          fullMeasureWholeNotesDuration
        ) {
          // yes this measure is not yet complete and should be split
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceRepeats ()) {
            gLog <<
              "Splitting measure '" <<
              lastMeasureInLastSegment->asShortString () <<
              "' upon a repeat start in voice \"" <<
              fVoiceName <<
              "\"" <<
              ", measureCurrentPositionInMeasure: " <<
              measureCurrentPositionInMeasure.asString () <<
              ", fullMeasureWholeNotesDuration: " <<
              fullMeasureWholeNotesDuration.asString () <<
              ", line " << inputLineNumber <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

/* JMI
          // finalize lastMeasureInLastSegment
          lastMeasureInLastSegment->
            finalizeMeasure (
              inputLineNumber,
              msrMeasureRepeatContextKind::kMeasureRepeatContextNone,
              "handleVoiceLevelRepeatStart() 3");
*/
          // append last segment to initial voice elements list
          appendVoiceLastSegmentToInitialVoiceElements (
            inputLineNumber,
            "handleVoiceLevelRepeatStart() 4");

          // create a new last segment for the voice
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceSegments ()) {
            gLog <<
              "Creating a new last segment for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          createNewLastSegmentForVoice (
            inputLineNumber,
            "handleVoiceLevelRepeatStart() 5");

          // create a new measure with the same number as the voice last measure
          // and append it to the voice,
          cascadeCreateAMeasureAndAppendItInVoice (
            inputLineNumber,
            333, //         previousMeasureEndInputLineNumber, 0.9.62
            lastMeasureInLastSegment->getMeasureNumber (),
            msrMeasureImplicitKind::kMeasureImplicitKindNo);

        /* JMI
          // set it as created before a repeat
          voiceLastMeasure->
            setMeasureCreatedForARepeatKind ( // JMI 0.9.66
              msrMeasure::kMeasureCreatedForARepeatBefore);
              */
        }
        else {
          // no this measure is complete
          // JMI ???
        }
      }

      else {
        // the last measure elements list is empty,
        // keep it for a new voice last segment

        // remove last measure
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceRepeats ()) {
          std::stringstream ss;

          ss <<
            "Removing the last measure in voice \"" <<
            fVoiceName <<
            "\"" <<
            " (voice level start)" <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

//         S_msrMeasure
//           dummyMeasure = // JMI ??? 0.9.72
//             fVoiceLastSegment->
//               removeLastMeasureFromSegment (
//                 inputLineNumber,
//                 "handleVoiceLevelRepeatStart() 55");

        // append the voice last segment to the initial voice elements list
        appendVoiceLastSegmentToInitialVoiceElements (
          inputLineNumber,
          "handleVoiceLevelRepeatStart() 555");

        // create a new last segment containing a new measure for the voice
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceRepeats ()) {
          std::stringstream ss;

          ss <<
            "Creating a new last segment with the first common part measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

//         createNewLastSegmentFromItsFirstMeasureForVoice (
//           inputLineNumber,
//           dummyMeasure,
//           "handleVoiceLevelRepeatStart() 5555");
      }
    }
  }

  else {
    // no voice last segment JMI ???
  }

  // create the repeat and stack it
  S_msrRepeat
    newRepeat =
      createARepeatAndStackIt (
        inputLineNumber,
        "handleVoiceLevelRepeatStart() 10");

  // create the repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat common part upon its start in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        inputLineNumber,
        newRepeat);

  // register it in newRepeat
  newRepeat->
    setRepeatCommonPart (
      repeatCommonPart);

  // set newRepeat as having an explicit start
  newRepeat->
    setRepeatExplicitStartKind (
      msrRepeatExplicitStartKind::kRepeatExplicitStartYes);

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
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a nested repeat start in voice \"" <<
      fVoiceName <<
      "\"" <<
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

void msrVoice::handleRepeatStartInVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start in voice \"" <<
      fVoiceName <<
      "\"" <<
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
      "handleRepeatStartInVoice() 1");
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
      "handleRepeatStartInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleVoiceLevelRepeatEndWithoutStart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber& measureNumber,
  int                repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat end without start in voice \"" <<
      fVoiceName <<
      "\"" <<
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
      "handleVoiceLevelRepeatEndWithoutStart() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
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
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat upon its end in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

  S_msrRepeat
    newRepeat =
      msrRepeat::create (
        repeatInputLineNumber,
        repeatTimes,
        this);

  // create the repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat common part upon its end in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        repeatInputLineNumber,
        newRepeat);

  // register it in newRepeat
  newRepeat->
    setRepeatCommonPart (
      repeatCommonPart);

  // fetch the last segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelRepeatEndWithoutStart() 2");

  // set voice current after repeat component phase kind
  // before finalizeLastAppendedMeasureInVoice()
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
  // append the voice last segment to the new repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending the voice last segment in voice \"" <<
      fVoiceName <<
      "\" to the new voice-level repeat common part" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  repeatCommonPart->
    appendSegmentToRepeatCommonPart (
      inputLineNumber,
      fVoiceLastSegment,
      "handleVoiceLevelRepeatEndWithoutStart() 4");

  // create a new last segment for the voice
  createNewLastSegmentForVoice (
    inputLineNumber,
    "handleVoiceLevelRepeatEndWithoutStart() 5");

  // set newRepeat's build phase to completed
  newRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted);

  // append newRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelRepeatEndWithoutStart() 6");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStart() 7");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleVoiceLevelContainingRepeatEndWithoutStart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber& measureNumber,
  int                repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level rcontaining epeat end without start in voice \"" <<
      fVoiceName <<
      "\"" <<
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
      "handleVoiceLevelContainingRepeatEndWithoutStart() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
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
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat upon its end in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

  S_msrRepeat
    newRepeat =
      msrRepeat::create (
        repeatInputLineNumber,
        repeatTimes,
        this);

  // create the repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat common part upon its end in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeatCommonPart
    newRepeatCommonPart =
      msrRepeatCommonPart::create (
        repeatInputLineNumber,
        newRepeat);

  // register it in newRepeat
  newRepeat->
    setRepeatCommonPart (
      newRepeatCommonPart);

  // fetch the last segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelContainingRepeatEndWithoutStart() 1");

  // fetch the top of the repeats stack
  S_msrRepeat
    repeatsStackTopRepeat =
      fVoicePendingRepeatsStack.front ();

//   // pop it from the repeats stack
//   popRepeatFromVoiceRepeatsStack (
//     inputLineNumber,
//     repeatsStackTopRepeat,
//     "handleVoiceLevelContainingRepeatEndWithoutStart() 2");

  // append it to newRepeat's common part
  newRepeatCommonPart->
    appendRepeatToRepeatCommonPart (
      inputLineNumber,
      repeatsStackTopRepeat,
      "handleVoiceLevelContainingRepeatEndWithoutStart() 3");

  // append newRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelContainingRepeatEndWithoutStart() 4");

  // append the voice last segment to the new repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending the voice last segment in voice \"" <<
      fVoiceName <<
      "\" to the new voice-level repeat common part" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  newRepeatCommonPart->
    appendSegmentToRepeatCommonPart (
      inputLineNumber,
      fVoiceLastSegment,
      "handleVoiceLevelContainingRepeatEndWithoutStart() 5");

/* JMI SURE
  // create a new last segment for the voice
  createNewLastSegmentForVoice (
    inputLineNumber,
    "handleVoiceLevelContainingRepeatEndWithoutStart()");
    */

  // set newRepeat's build phase to completed
  newRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted);

  // append newRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelContainingRepeatEndWithoutStart() 6");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStart() 7");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatEndWithStart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber& measureNumber,
  int                repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat end with start in voice \"" <<
      fVoiceName <<
      "\"" <<
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
      "handleVoiceLevelRepeatEndWithStart() 1");
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "This repeat end with a start is at the voice-level" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

  // grab current repeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // pop it from the repeats stack
  popRepeatFromVoiceRepeatsStack (
    inputLineNumber,
    currentRepeat,
    "handleVoiceLevelRepeatEndWithStart() 2");

  // set its repeat times field
  currentRepeat->
    setRepeatTimes (repeatTimes);

  // create the currentRepeat's common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat common part upon repeat end in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        inputLineNumber,
        currentRepeat);

  // register it in newRepeat
  currentRepeat->
    setRepeatCommonPart (
      repeatCommonPart);

  // move the voice last segment to the new repeat common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Moving the voice last segment in voice \"" <<
      fVoiceName <<
      "\" to the current voice-level repeat common part" <<
      " and forget about it" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  moveVoiceLastSegmentToRepeatCommonPart (
    inputLineNumber,
    repeatCommonPart,
    "handleVoiceLevelRepeatEndWithStart() 3");

/* JMI SURE ???
  // create a new last segment to collect the remainder of the voice,
  if (false) {
  createNewLastSegmentForVoice ( // JMI
    inputLineNumber,
    "handleVoiceLevelRepeatEndWithStart() BOF");
  }
    */

  // set currentRepeat's build phase to completed
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted);

  // append currentRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    currentRepeat,
    "handleVoiceLevelRepeatEndWithStart() 4");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStart() 5");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleNestedRepeatEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber& measureNumber,
  int                repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a nested repeat end in voice \"" <<
      fVoiceName <<
      "\"" <<
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
      "handleNestedRepeatEndInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
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
    if (gTraceOahGroup->getTraceRepeats ()) {
      std::stringstream ss;

      ss <<
        "Splitting measure '" <<
        voiceLastMeasure->asShortString () <<
        "' upon a repeat end in voice \"" <<
        fVoiceName <<
        "\"" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create a new measure with the same number as the voice last measure
    // and append it to the voice,
    cascadeCreateAMeasureAndAppendItInVoice (
      inputLineNumber,
      333, //         previousMeasureEndInputLineNumber, 0.9.62
      measureNumber,
      msrMeasureImplicitKind::kMeasureImplicitKindNo);

  /* JMI
    // set it as created after a repeat
    voiceLastMeasure->
      setMeasureCreatedForARepeatKind ( // JMI 0.9.66
        msrMeasure::kMeasureCreatedForARepeatAfter);
        */
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleRepeatEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&    measureNumber,
  int                       repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

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
              // fetch the top of the repeats stack
              S_msrRepeat
                repeatsStackTopRepeat =
                  fVoicePendingRepeatsStack.front ();

              // analyze it
              switch (repeatsStackTopRepeat->getCurrentRepeatBuildPhaseKind ()) {
                case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
                  {
                    std::stringstream ss;

                    ss <<
                      "attempt at end repeat '" <<
                      repeatsStackTopRepeat->asShortString () <<
                      "' right after it has been created" <<
                      " in voice '" <<
                      asShortString () <<
                      "' ";

                    msrInternalError (
                      gServiceRunData->getInputSourceName (),
                      fInputLineNumber,
                      __FILE__, mfInputLineNumber (__LINE__),
                      ss.str ());
                  }
                  break;

                case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
                case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
                  // this repeat is at the voice-level and has a start
                  // -------------------------------------
                  handleVoiceLevelRepeatEndWithStart (
                    inputLineNumber,
                    measureNumber,
                    repeatTimes);
                  break;

                case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
                  // this repeat is at the voice-level, has no start
                  // and contains repeatsStackTop
                  // -------------------------------------
                  handleVoiceLevelContainingRepeatEndWithoutStart (
                    inputLineNumber,
                    measureNumber,
                    repeatTimes);
                  break;
              } // switch
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
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
  const mfInputLineNumber& inputLineNumber,
  S_msrRepeat& currentRepeat)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat ending start without explicit start in voice \"" <<
      fVoiceName <<
      "\"" <<
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
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  S_msrRepeat           repeat;
  S_msrRepeatCommonPart repeatCommonPart;

  if (currentRepeat) {
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();
  }

  else {
    // create the repeat and stack it
    std::stringstream s1;

    s1 <<
      "Creating a voice-level repeat upon its first ending in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

    currentRepeat =
      createARepeatAndStackIt (
        repeatInputLineNumber,
        s1.str ());

    // create the repeat common part
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeats ()) {
      std::stringstream ss;

      ss <<
        "Creating a repeat common part upon its end in voice \"" <<
        fVoiceName <<
        "\"" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    repeatCommonPart =
      msrRepeatCommonPart::create (
        repeatInputLineNumber,
        currentRepeat);

    // register it in newRepeat
    currentRepeat->
      setRepeatCommonPart (
        repeatCommonPart);
  }

  // fetch last measure
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Fetching the last measure in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrMeasure
    lastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 1");

  // fetch lastMeasure's elements list
  const std::list <S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (true || lastMeasureElementsList.empty ()) { // JMIJMIJMIJMIJMIJMI  // JMI 0.9.66
    // it is empty, keep it for a new voice last segment

    // remove last measure
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeats ()) {
      std::stringstream ss;

      ss <<
        "Removing the last measure in voice \"" <<
        fVoiceName <<
        "\"" <<
        " (voice level ending without explicit start)" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

//     S_msrMeasure
//       dummyMeasure =
//         fVoiceLastSegment->
//           removeLastMeasureFromSegment (
//             inputLineNumber,
//             "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 2");

    // move voice last segment into the repeat common part
    std::stringstream s2;

    s2 <<
      "Moving the voice last segment to the repeat common part in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    moveVoiceLastSegmentToRepeatCommonPart (
      inputLineNumber,
      repeatCommonPart,
      s2.str ());

    // create a new last segment containing a new measure for the voice
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeats ()) {
      std::stringstream ss;

      ss <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    createNewLastSegmentFromItsFirstMeasureForVoice (
      inputLineNumber,
      lastMeasure,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 3");
  }

  else {

    // it is not empty

    // append last segment to initial voice elements list
    appendVoiceLastSegmentToInitialVoiceElements (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 4");

    // create a new last segment for the voice
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSegments ()) {
      std::stringstream ss;

      ss <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart( 5)");
  }

  // set currentRepeat's build phase
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithExplicitStart (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a voice-level repeat ending start with explicit start in voice \"" <<
      fVoiceName <<
      "\"" <<
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
      "handleVoiceLevelRepeatEndingStartWithExplicitStart() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // fetch currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // get currentRepeat's common part
  S_msrRepeatCommonPart
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();

  // fetch last measure of last segment
  S_msrMeasure
    lastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelRepeatEndingStartWithExplicitStart() 2");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Fetching the last measure of the last segment in voice \"" <<
      fVoiceName <<
      "\"" <<
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
    // it is empty, keep it for a new voice last segment

    // remove last measure
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeats ()) {
      std::stringstream ss;

      ss <<
        "Removing the last measure in voice \"" <<
        fVoiceName <<
        "\"" <<
        " (voice level ending with explicit start)" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

//     S_msrMeasure
//       dummyMeasure =
//         fVoiceLastSegment->
//           removeLastMeasureFromSegment (
//             inputLineNumber,
//             "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 3");

    // move voice last segment into the repeat common part
    std::stringstream s2;

    s2 <<
      "Moving the voice last segment to the repeat common part in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    moveVoiceLastSegmentToRepeatCommonPart (
      inputLineNumber,
      repeatCommonPart,
      s2.str ());

    // create a new last segment containing a new measure for the voice
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRepeats ()) {
      std::stringstream ss;

      ss <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    createNewLastSegmentFromItsFirstMeasureForVoice (
      inputLineNumber,
      lastMeasure,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 4");
  }

  else {
    // it is not empty

    // append last segment to initial voice elements list
    appendVoiceLastSegmentToInitialVoiceElements (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 5");

    // create a new last segment for the voice
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSegments ()) {
      std::stringstream ss;

      ss <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 6");
  }

  // set currentRepeat's build phase
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStart() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::nestContentsIntoNewRepeatInVoice (
  const mfInputLineNumber& inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // are there measures in the voice last segment?
        if (! fVoiceLastSegment->getSegmentElementsList ().empty ()) {
          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceRepeatsDetails ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 1");
          }
#endif // MF_TRACE_IS_ENABLED

          // move voice last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "nestContentsIntoNewRepeatInVoice() 1");

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceRepeatsDetails ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 2");
          }
#endif // MF_TRACE_IS_ENABLED
        }
      }
      break;
  } // switch
}

void msrVoice::handleNestedRepeatEndingStartInVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a nested repeat ending start voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
 }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndingStartInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleRepeatEndingStartInVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending start in voice \"" <<
      fVoiceName <<
      "\"" <<
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
      "handleRepeatEndingStartInVoice() 1");
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
          case 0:
            // this the first ending of a voice-level repeat without start
            // -------------------------------------
            handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
              inputLineNumber,
              gNullRepeat); // set later in XXX() JMI 0.9.66
            break;

          case 1:
            {
              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatsStack.front ();

              switch (currentRepeat->getRepeatExplicitStartKind ()) {
                case msrRepeatExplicitStartKind::kRepeatExplicitStartUnknown:
                  // JMI ??? 0.9.71 this the first ending of a voice-level repeat without a start
                  // -------------------------------------
                  handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
                    inputLineNumber,
                    currentRepeat);
                  break;

                case msrRepeatExplicitStartKind::kRepeatExplicitStartNo:
                  // this the first ending of a voice-level repeat without a start
                  // -------------------------------------
                  handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
                    inputLineNumber,
                    currentRepeat);
                  break;

                case msrRepeatExplicitStartKind::kRepeatExplicitStartYes:
                  // this the first ending of a voice-level repeat with a start
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
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleRepeatEndingStartInVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  msrRepeatEndingKind
                     repeatEndingKind,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // handle the repeat ending start
        ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceRepeats ()) {
          std::stringstream ss;

          ss <<
            "Handling a repeat ending upon its start in voice clone \"" <<
            fVoiceName <<
            "\"" <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // is there a current repeat?
        switch (fVoicePendingRepeatsStack.size ()) {
          case 0:
            {
              std::stringstream ss;

              ss <<
                "repeats stack is empty when attempting to handle a repeat ending start in voice clone " <<
                asShortString ();

              msrInternalError (
                gServiceRunData->getInputSourceName (),
                fInputLineNumber,
                __FILE__, mfInputLineNumber (__LINE__),
                ss.str ());
            }
          break;

          case 1:
            {
              // this repeat ending is part of a voice-level repeat

              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatsStack.front ();

              // create a repeat ending
#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceRepeats ()) {
                gLog <<
                  "Creating a " <<
                  msrRepeatEndingKindAsString (
                    repeatEndingKind) <<
                  " repeat ending in current repeat in voice clone \"" <<
                  fVoiceName <<
                  "\"" <<
                  std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

              S_msrRepeatEnding
                repeatEnding =
                  msrRepeatEnding::create (
                    inputLineNumber,
                    repeatEndingNumber,
                    repeatEndingKind,
                    currentRepeat);

              // add the repeat ending to the voice current repeat
#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceRepeats ()) {
                gLog <<
                  "Appending a " <<
                  msrRepeatEndingKindAsString (
                    repeatEndingKind) <<
                  " repeat ending to current repeat in voice \"" <<
                  fVoiceName <<
                  "\"" <<
                  std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceRepeatsDetails ()) {
                displayVoiceRepeatsStackSummary (
                  inputLineNumber,
                  "before adding a hooked repeat ending to current repeat");
              }
#endif // MF_TRACE_IS_ENABLED

              currentRepeat->
                addRepeatEndingToRepeat (
                  inputLineNumber,
                  repeatEnding);

              // set currentRepeat's build phase
              currentRepeat->
                setCurrentRepeatBuildPhaseKind (
                  msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings);
            }
          break;

          default:
            {
              // the current repeat is nested

              // JMI ???

              // move voice last segment into the repeat common part
#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceRepeats ()) {
                gLog <<
                  "Moving the voice last segment to the repeat common part in voice clone \"" <<
                  fVoiceName <<
                  "\"" <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif // MF_TRACE_IS_ENABLED
            }
        } // switch

        --gIndenter;
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoiceClone() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleSegmentCloneEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrSegment& segmentClone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegments ()) {
    std::stringstream ss;

    ss <<
      "Handling segment '" <<
      segmentClone->asShortString () <<
      "' in voice clone \"" <<
      fVoiceName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsDetails ()) {
    displayVoice (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  if (fVoicePendingMeasureRepeat) {
    // segmentClone belongs to a measures repeat

    switch (
      fVoicePendingMeasureRepeat->getCurrentMeasureRepeatBuildPhaseKind ()
    ) {
      case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseJustCreated:
        {
          std::stringstream ss;

          ss <<
            "segment '" <<
            segmentClone->asShortString () <<
            "'cannot be added to a just created measures repeat";

          msrError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
        break;

      case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInPattern:
        {
          // get fVoicePendingMeasureRepeat's pattern
          S_msrMeasureRepeatPattern
            measureRepeatPattern =
              fVoicePendingMeasureRepeat->
                getMeasureRepeatPattern ();

          // set segmentClone as the pattern's segment
          measureRepeatPattern->
            setMeasureRepeatPatternSegment (
        // JMI      inputLineNumber,
              segmentClone);
        }
        break;

      case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInReplicas:
        {
          // get fVoicePendingMeasureRepeat's replicas
          S_msrMeasureRepeatReplicas
            measureRepeatReplicas =
              fVoicePendingMeasureRepeat->
                getMeasureRepeatReplicas ();

          // set segmentClone as the replicas's segment
          measureRepeatReplicas->
            setMeasureRepeatReplicasSegment (
        // JMI      inputLineNumber,
              segmentClone);
        }
        break;

      case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseCompleted:
        {
          std::stringstream ss;

          ss <<
            "segment '" <<
            segmentClone->asShortString () <<
            "'cannot be added to a completed measures repeat";

          msrError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
        break;
    } // switch
  }

  else if (! fVoicePendingRepeatsStack.empty ()) {
    // segmentClone belongs to a repeat

    // append segment to whichever part of the repeat is adequate
    S_msrRepeat
      currentRepeat =
        fVoicePendingRepeatsStack.front ();

    currentRepeat->
      appendSegmentToRepeat (
        inputLineNumber,
        segmentClone,
        "handleSegmentCloneEndInVoiceClone() 2");
  }

  else {
    // segmentClone is a voice-level segment

    // move fVoiceLastSegment to the initial voice elements list
    moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone()");
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegmentsDetails ()) {
    displayVoice (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

/* JMI
void msrVoice::finalizeRepeatEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber& measureNumber,
  int                repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
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
        finalizeLastAppendedMeasureInVoice (
          inputLineNumber);

        // fetch the repeat
        if (fVoicePendingRepeatsStack.empty ()) {
          std::stringstream ss;

          ss <<
            "repeats stack is empty when attempt at finalizing a repeat in voice '" <<
            asShortString () <<
            "' ";

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

/ * JMI
          // move all the voice contents to the new repeat common part
          moveAllVoiceContentsToRepeatCommonPart (
            inputLineNumber,
            repeatCommonPart,
            "finalize repeat");
          // move the voice initial elements to the new repeat common part
        * /
        }

        else {
          // no, this repeat is at the voice-level

/ * JMI
          // move all the voice contents to the new repeat common part
          moveAllVoiceContentsToRepeatCommonPart ( // JMI
            inputLineNumber,
            repeatCommonPart,
            "finalizing voice-level repeat");
            * /
        }
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "finalizeRepeatEndInVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}
*/

void msrVoice::cascadeCreateAMeasureRepeatAndAppendItToVoice (
  const mfInputLineNumber& inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashesNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLog <<
      "Creating a measure repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "cascadeCreateAMeasureRepeatAndAppendItToVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // this occurs after an empty measure has just been created,
  // hence the repeated measure/measures is/are the
  // measureRepeatMeasuresNumber measures preceding the last one

//   int
//     lastSegmentMeasuresNumber =
//       fVoiceLastSegment->
//         getSegmentElementsList ().size ();
//     availableMeasuresNumber =
//       lastSegmentMeasuresNumber - 1;

//   if (
//     availableMeasuresNumber < measureRepeatMeasuresNumber) {
//     std::stringstream ss;
//
//     ss <<
//       "attempt at creating a measures repeat with " <<
//       measureRepeatMeasuresNumber <<
//       " measures while current last segment only has " <<
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

  // create the measures repeat repeated segment
  S_msrSegment
    repeatedSegment =
      msrSegment::create (
        inputLineNumber,
        this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

//   // remove the repeated measure(s) for the last segment
//   // and prepend them to the repeated segment
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasureRepeats ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing the last " <<
//       mfSingularOrPlural (
//         measureRepeatMeasuresNumber, "measure", "measures") <<
//       " (to be repeated) from voice \"" <<
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

  // create the measures repeat
  if (fVoicePendingMeasureRepeat) {
    std::stringstream ss;

    ss <<
      "attempt at creating a measures repeat while another one is pending";

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

  // create the measures repeat pattern
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a measures repeat pattern in voice \"" <<
      fVoiceName <<
      "\"";

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

  // set the repeated segment as the measures repeat pattern segment
  measureRepeatPattern->
    setMeasureRepeatPatternSegment (
      repeatedSegment);

  // set the measures repeat pattern
  fVoicePendingMeasureRepeat->
    setMeasureRepeatPattern (
      measureRepeatPattern);

  // create a new last segment to collect the measures repeat replicas,
  // containing the first, yet incomplete, replica
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a new last segment with the first replica measure for voice \"" <<
      fVoiceName << "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   createNewLastSegmentFromItsFirstMeasureForVoice (
//     inputLineNumber,
//     firstReplicaMeasure,
//     "cascadeCreateAMeasureRepeatAndAppendItToVoice() 2");

  // keep the measures repeat pending

  // print resulting voice contents
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "cascadeCreateAMeasureRepeatAndAppendItToVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::cascadeAppendMultipleMeasureRestToVoice (
  const mfInputLineNumber&        inputLineNumber,
  const S_msrMultipleMeasureRest& multipleMeasureRest)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending multiple measure rest " <<
      multipleMeasureRest->asShortString () <<
      " to voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "cascadeAppendMultipleMeasureRestToVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // analyze this multiple measure rests's context
  switch (fVoicePendingRepeatsStack.size ()) {
    case 0:
      // this multiple measure rests is at the voice-level
      // -------------------------------------

      // create the voice last segment if needed
      if (false && ! fVoiceLastSegment) { // JMI 0.9.73
        createNewLastSegmentForVoice (
          inputLineNumber,
          "cascadeAppendMultipleMeasureRestToVoice()");
      }

      // append multipleMeasureRest to it
      fVoiceLastSegment->
        cascadeAppendMultipleMeasureRestToSegment (
          multipleMeasureRest);
      break;

    default:
      // this multiple measure rests is inside a repeat
      // -------------------------------------
      S_msrRepeat
        currentRepeat =
          fVoicePendingRepeatsStack.front ();

      currentRepeat->
        cascadeAppendMultipleMeasureRestToRepeat (
          inputLineNumber,
          multipleMeasureRest,
          "cascadeAppendMultipleMeasureRestToVoice() 2");
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "cascadeAppendMultipleMeasureRestToVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::appendMeasureRepeatToVoice (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat '" <<
      measureRepeat->asShortString () <<
      "' to voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendMeasureRepeatToVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // analyze this measure repeats's context
  switch (fVoicePendingRepeatsStack.size ()) {
    case 0:
      // this measures repeat is at the voice-level
      // -------------------------------------
      appendMeasureRepeatToVoiceElementsList (
        measureRepeat);
      break;

    default:
      // this measures repeat is inside a repeat
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendMeasureRepeatToVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::appendPendingMeasureRepeatToVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLog <<
      "Appending pending measures repeat to voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasureRepeatToVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // does the current measures repeat exist?
  if (! fVoicePendingMeasureRepeat) {
    std::stringstream ss;

    ss <<
      "attempt at appending a pending measures repeat which doesn't exist";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // fetch the last segment's measure elements list
  std::list <S_msrSegmentElement>
    voiceLastSegmentMeasureList =
      fVoiceLastSegment->
        getSegmentElementsList ();

  // grab the just created last measure
  // in the last segment's measure elements list,
  // (i.e. the one containing:
  //   <measure-repeat type="stop"/>)
  // which is the next measure after the measures repeat
  if (voiceLastSegmentMeasureList.empty ()) {
    std::stringstream ss;

    ss <<
      "attempt at grabbing first measure of voice last segment, that contains none";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  S_msrMeasure
    nextMeasureAfterMeasureRepeat =
      fVoiceLastSegment->
        getSegmentLastMeasure (); // JMI 0.9.63

// BOFBOFBOF JMI
  // remove the next measure from the last segment's measure elements list
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Removing last measure in last segment" <<
      "in voice \"" <<
      fVoiceName <<
      "\":" <<
      std::endl;
    ++gIndenter;
    gLog <<
      nextMeasureAfterMeasureRepeat;
    gLog << std::endl;
    --gIndenter;
}
#endif // MF_TRACE_IS_ENABLED

  voiceLastSegmentMeasureList.pop_back ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasureRepeatToVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  // create the measures repeat replicas contents
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a measures repeat replicas contents for voice \"" <<
      fVoiceName << "\" is:";

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

  // set the voice last segment as the measures repeat replicas segment
  measureRepeatReplicas->
    setMeasureRepeatReplicasSegment (
      fVoiceLastSegment);

  fVoicePendingMeasureRepeat->
    setMeasureRepeatReplicas (
      measureRepeatReplicas);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting pending measures repeat replicas segment in voice \"" <<
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
      "Setting measures repeat segment to voice last segment for voice \"" <<
      fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoicePendingMeasureRepeat->
    getMeasureRepeatReplicas ()->
      setMeasureRepeatReplicasSegment (
        fVoiceLastSegment);

  // forget about this voice last segment
  fVoiceLastSegment = nullptr; // JMI

  // append pending measures repeat to the voice
  appendMeasureRepeatToVoice (
    inputLineNumber,
    fVoicePendingMeasureRepeat);

  // create a new last segment to collect the remainder of the voice,
  // containing the next, yet incomplete, measure
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a new last segment with the AAA measures repeat next measure for voice \"" <<
      fVoiceName << "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  createNewLastSegmentFromItsFirstMeasureForVoice (
    inputLineNumber,
    nextMeasureAfterMeasureRepeat,
    "appendPendingMeasureRepeatToVoice() 3");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLog <<
      "Appending pending measures repeat to voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasureRepeatToVoice() 4");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::createMeasureRepeatAndAppendItToVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashesNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLog <<
      "Creating measures repeat and appending it to voice clone \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "createMeasureRepeatAndAppendItToVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // does the pending measures repeat exist?
        if (fVoicePendingMeasureRepeat) {
          std::stringstream ss;

          ss <<
            "attempt at creating a measures repeat while another one is pending";

          msrInternalError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }

        // create the measures repeat
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Creating and appending a measures repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
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

        // create a measures repeat pattern from current last segment
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Creating a measures repeat pattern from current last segment in voice \"" <<
            fVoiceName <<
            "\"" <<
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

        // set current last segment as the measures repeat pattern segment
        measureRepeatPattern->
          setMeasureRepeatPatternSegment (
            fVoiceLastSegment);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr; // JMI

        // set the measures repeat pattern
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Setting repeat common part in voice \"" <<
            fVoiceName <<
            "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoicePendingMeasureRepeat->
          setMeasureRepeatPattern (
            measureRepeatPattern);

        // append the measures repeat to the list of initial elements
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Appending measures repeat to voice \"" <<
            fVoiceName <<
            "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // append pending measures repeat to the voice
        appendMeasureRepeatToVoice (
          inputLineNumber,
          fVoicePendingMeasureRepeat);
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLog <<
      "Creating measures repeat and appending it to voice clone \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "createMeasureRepeatAndAppendItToVoiceClone() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::setVoiceContainsMultipleMeasureRests (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains multiple measure rests";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceContainsMultipleMeasureRests = true;
}

void msrVoice::setVoiceContainsMeasureRepeats (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains measure repeats";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceContainsMeasureRepeats = true;
}

void msrVoice::createAMultipleMeasureRestAndAppendItToVoice (
  const mfInputLineNumber& inputLineNumber,
  int               multipleMeasureRestMeasuresNumber,
  int               multipleMeasureRestSlashesNumber,
  msrUseSymbolsKind multipleMeasureRestUseSymbolsKind)
{
  // create a multiple measure rests
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending a multiple measure rest for " <<
      mfSingularOrPlural (
        multipleMeasureRestMeasuresNumber, "measure", "measures") <<
      " to voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "cascadeAppendMultipleMeasureRestToVoice() 1");
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

        // create the multiple measure rests
        if (fVoiceCurrentMultipleMeasureRest) {
          std::stringstream ss;

          ss <<
            "attempt at creating a multiple measure rests while another one is pending";

          msrInternalWarning (
//           msrInternalError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
//             __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }

//         if (false && ! fVoiceLastSegment) { // JMI 0.9.73 // JMI 0.9.63 useless???
//           // create a new last segment for the voice
//           createNewLastSegmentForVoice (
//             inputLineNumber,
//             "cascadeAppendMultipleMeasureRestToVoice()");
//         }

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Creating a multiple measure rest for " <<
            mfSingularOrPlural (
              multipleMeasureRestMeasuresNumber, "measure", "measures") <<
            " in voice \"" <<
            fVoiceName <<
            "\"" <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoiceCurrentMultipleMeasureRest =
          msrMultipleMeasureRest::create (
            inputLineNumber,
            multipleMeasureRestMeasuresNumber,
            multipleMeasureRestSlashesNumber,
            multipleMeasureRestUseSymbolsKind);

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Appending multiple measure rest " <<
            fVoiceCurrentMultipleMeasureRest->asString () <<
            " to the last segment of voice \"" <<
            fVoiceName <<
            "\"" <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoiceLastSegment->
          cascadeAppendMultipleMeasureRestToSegment (
            fVoiceCurrentMultipleMeasureRest);

//         // append firstRestMeasure to fVoiceCurrentMultipleMeasureRest
//         fVoiceCurrentMultipleMeasureRest->
//           appendMeasureToMultipleMeasureRest (
//             firstRestMeasure);

         // remember fVoiceCurrentMultipleMeasureRest for later next measure number setting
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Registering multiple measure rests " <<
            fVoiceCurrentMultipleMeasureRest->asString () <<
            " as waiting for its next measure number" <<
            " in voice \"" <<
            fVoiceName << "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fVoiceMultipleMeasureRestsWaitingForItsNextMeasureNumber =
          fVoiceCurrentMultipleMeasureRest;

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Setting fVoiceRemainingMultipleMeasureRests to '" <<
            multipleMeasureRestMeasuresNumber <<
            "' in voice \"" <<
            fVoiceName << "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        fVoiceRemainingMultipleMeasureRests =
          multipleMeasureRestMeasuresNumber;

//         // create a new segment to collect the multiple measure rests,
//         // containing the first, rest measure
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
//           gLog <<
//             "Creating a new last segment to create the first, rest measure in voice \"" <<
//             fVoiceName << "\"" <<
//             ", line " << inputLineNumber <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         createNewLastSegmentFromItsFirstMeasureForVoice (
//           inputLineNumber,
//           firstRestMeasure,
//           "cascadeAppendMultipleMeasureRestToVoice() 3");
//
//         // this voice contails multiple measure rests
//         this->setVoiceContainsMultipleMeasureRests (
//           inputLineNumber); // JMI 0.9.67
//
//         // keep the multiple measure rests pending

        --gIndenter;
      }
      break;
  } // switch

  // print resulting voice contents
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "cascadeAppendMultipleMeasureRestToVoice() 4");
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingMultipleMeasureRest = true;
}

void msrVoice::replicateLastAppendedMeasureInVoice (
  const mfInputLineNumber& inputLineNumber,
  int replicatasNumber)
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
            fVoiceLastSegment);

    // change its contents
    lastAppendedMeasureClone->
      setMeasureNumber (
        mfMeasureNumberAsString (voiceLastAppendedMeasureMeasureNumber) +
          '.' + // JMI 0.9.75
          std::to_string (i) +
          " (replicated)");

#ifdef MF_TRACE_IS_ENABLED
      std::stringstream ss;

      ss <<
        "Replicating last appended measure " <<
        fVoiceLastAppendedMeasure->getMeasureNumber () <<
        " as measure " <<
        lastAppendedMeasureClone->getMeasureNumber () <<
        " in voice \"" <<
        fVoiceName <<
        "\"" <<
        std::endl;
#endif // MF_TRACE_IS_ENABLED

    // register its whole notes wholeNotes
    fetchVoiceUpLinkToPart ()->
      registerOrdinalMeasureNumberWholeNotes (
        inputLineNumber,
        lastAppendedMeasureClone->
          getMeasureOrdinalNumberInVoice (),
        lastAppendedMeasureClone->
          getFullMeasureWholeNotesDuration ()); // JMI

    // append it to the voice last segment
    fVoiceLastSegment->
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
  // create a multiple measure rests
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Adding " <<
      mfSingularOrPlural (
        emptyMeasuresNumber, "empty measure", "empty measures") <<
      " to voice \"" <<
      fVoiceName <<
      "\"" <<
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
      ", in voice \"" <<
      fVoiceName <<
      "\"" <<
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
        fVoiceLastSegment);

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

  // append emptyMeasure to the voice last segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating an empty measure '" << measureNumber <<
      "' and appending it to segment " << asString () <<
      ", in voice \"" <<
      fVoiceName  <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fVoiceLastSegment->
    appendMeasureToSegment (emptyMeasure);


//   // change the measure number
//   fVoiceLastAppendedMeasure->
//     setMeasureNumber (
//       fVoiceLastAppendedMeasure->
//         getMeasureNumber () + " added"); // JMI BLARK 0.9.61

  } //for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "appendEmptyMeasuresToVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // print resulting voice contents
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "appendEmptyMeasuresToVoice() 4");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::appendPendingMultipleMeasureRestsToVoice (
    const mfInputLineNumber& inputLineNumber)
{
  // a multiple measure rests is a voice element,
  // and can be voice-level as well as part of a repeat

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
      if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
        std::stringstream ss;

        ss <<
          "Appending current multiple measure rests " <<
          fVoiceCurrentMultipleMeasureRest->asShortString () <<
          " to voice \"" <<
          fVoiceName <<
          "\"" <<
          ", line " << inputLineNumber <<
          std::endl;
      }

    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "appendPendingMultipleMeasureRestsToVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
//         // does the current multiple measure rests exist?
//         if (! fVoiceCurrentMultipleMeasureRest) {
//           std::stringstream ss;
//
//           ss <<
//             "attempt at appending a current multiple measure rests which doesn't exist";
//
//           msrInternalError (
//             gServiceRunData->getInputSourceName (),
//             inputLineNumber,
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str ());
//         }

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Appending current multiple measure rests " <<
            fVoiceCurrentMultipleMeasureRest->asShortString () <<
            " to the last segment of voice \"" <<
            fVoiceName <<
            "\"" <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
//        fVoiceLastSegment->
          // JMI 0.9.67

        // set current multiple measure rests last measure purist number
        fVoiceCurrentMultipleMeasureRest->
          setLastMeasurePuristMeasureNumber (
            inputLineNumber,
            fVoiceCurrentMeasurePuristNumber);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr;

        // append current multiple measure rests to the voice
        cascadeAppendMultipleMeasureRestToVoice (
          inputLineNumber,
          fVoiceCurrentMultipleMeasureRest);

        // forget about this current multiple measure rests
        fVoiceCurrentMultipleMeasureRest = nullptr;
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "appendPendingMultipleMeasureRestsToVoice() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleMultipleMeasureRestsStartInVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMultipleMeasureRest& multipleMeasureRest)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Handling multiple measure rests start in voice clone \"" <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "handleMultipleMeasureRestsStartInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:

      // is there a voice last segment?
//       if (fVoiceLastSegment) {
//
//         // are there measures in the voice last segment?
//         if (! fVoiceLastSegment->getSegmentElementsList ().empty ()) {
//
//           // finalize current measure in voice
//           finalizeLastAppendedMeasureInVoice (
//             inputLineNumber);
//
//           // move voice last segment to the list of initial elements
//           moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
//             inputLineNumber,
//             "handleMultipleMeasureRestsStartInVoiceClone() 2");
//
//           // forget about fVoiceCurrentMultipleMeasureRest
//           fVoiceCurrentMultipleMeasureRest = nullptr;
//
//           // create a new last segment containing a new measure for the voice
//           createNewLastSegmentForVoice (
//             inputLineNumber,
//             "handleMultipleMeasureRestsStartInVoiceClone()");
//
// #ifdef MF_TRACE_IS_ENABLED
//           if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
//             displayVoiceMultipleMeasureRestsAndVoice (
//               inputLineNumber,
//               "handleMultipleMeasureRestsStartInVoiceClone() 3");
//           }
// #endif // MF_TRACE_IS_ENABLED
//         }
//       }

      // is there already a current multiple measure rests in this voice?
      if (fVoiceCurrentMultipleMeasureRest) {
        std::stringstream ss;

        ss <<
          "current voice multiple measure rests is not null when attempting to handle multiple measure rests start in voice clone " <<
          asShortString ();

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      // create the multiple measure rests clone and register it
      fVoiceCurrentMultipleMeasureRest =
        multipleMeasureRest->
          createMultipleMeasureRestNewbornClone (
            fVoiceLastSegment); // JMI ??? JMI 0.9.67

      // this voice contails multiple measure rests
      this->setVoiceContainsMultipleMeasureRests (
        inputLineNumber);

      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "handleMultipleMeasureRestsStartInVoiceClone() 4");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleMultipleMeasureRestsEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Handling multiple measure rests end in voice clone \"" <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
    displayVoiceMultipleMeasureRestsAndVoice (
      inputLineNumber,
      "handleMultipleMeasureRestsEndInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:

      // is there a current multiple measure rests in this voice?
      if (! fVoiceCurrentMultipleMeasureRest) {
        std::stringstream ss;

        ss <<
          "current voice multiple measure rests is null when attempting to handle multiple measure rests end in voice clone " <<
          asShortString ();

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      // set current multiple measure rests last measure purist number
      fVoiceCurrentMultipleMeasureRest->
        setLastMeasurePuristMeasureNumber (
          inputLineNumber,
          fVoiceCurrentMeasurePuristNumber);

      // forget about fVoiceCurrentMultipleMeasureRest
      fVoiceCurrentMultipleMeasureRest = nullptr;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
        displayVoice (
          inputLineNumber,
          "handleMultipleMeasureRestsEndInVoiceClone() 3");
      }
#endif // MF_TRACE_IS_ENABLED
    break;
  } // switch

  --gIndenter;
}

void msrVoice::appendMultipleMeasureRestCloneToVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMultipleMeasureRest& multipleMeasureRestClone)
{
  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    multipleMeasureRestClone != nullptr,
    "multipleMeasureRestClone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
          std::stringstream ss;

          ss <<
            "Appending multiple measure rest clone " <<
            multipleMeasureRestClone->asString () <<
            " to voice clone \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }

        if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
          displayVoiceMultipleMeasureRestsAndVoice (
            inputLineNumber,
            "appendMultipleMeasureRestCloneToVoiceClone() 1");
        }
#endif // MF_TRACE_IS_ENABLED

        // is multiple measure rests nested in a repeat?
        if (! fVoicePendingRepeatsStack.empty ()) {
          // yes

          S_msrRepeat
            currentRepeat =
              fVoicePendingRepeatsStack.front ();

          // grab the multiple measure rests segment, i.e. the voice's last segment JMI ???
          S_msrSegment
            multipleMeasureRestSegment =
              fVoiceLastSegment;

          // append it to the current repeat's common part
          currentRepeat->
            getRepeatCommonPart ()->
              appendSegmentToRepeatCommonPart ( // NO !!!
                inputLineNumber,
                multipleMeasureRestSegment,
                "appendMultipleMeasureRestCloneToVoiceClone() 2");

          // forget about this voice last segment
          fVoiceLastSegment = nullptr;
        }

        else {
          // no
          // JMI ???
        }

        // append the multiple measure rests clone to the voice
        cascadeAppendMultipleMeasureRestToVoice (
          inputLineNumber,
          multipleMeasureRestClone);

        // print resulting voice contents
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultipleMeasureRestsDetails ()) {
          displayVoice (
            inputLineNumber,
            "appendMultipleMeasureRestCloneToVoiceClone() 3");
        }
#endif // MF_TRACE_IS_ENABLED
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendRepeatCloneToVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat& repeatCLone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat clone '" <<
      repeatCLone->asString () <<
      "' to voice clone \"" <<
      fVoiceName <<  "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
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
        // pushing repeat clone as the (new) current repeat
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceRepeats ()) {
          std::stringstream ss;

          ss <<
            "Pushing repeat clone as the new current repeat in voice \"" <<
            fVoiceName <<
            "\"";

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

        // append it to the list of initial elements
        appendRepeatCloneToInitialVoiceElements (
          inputLineNumber,
          repeatCLone,
          "appendRepeatCloneToVoiceClone() 3");
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 4");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleMeasureRepeatStartInVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling measures repeat start in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatStartInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // is there already a current measures repeat in this voice?
      if (fVoicePendingMeasureRepeat) {
        std::stringstream ss;

        ss <<
          "current voice measures repeat is not null when attempting to handle measures repeat start in voice clone " <<
          asShortString ();

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      // create the measures repeat clone and register it
      fVoicePendingMeasureRepeat =
        measureRepeat->
          createMeasureRepeatNewbornClone ();

      // this voice contails measure repeats
      this->setVoiceContainsMeasureRepeats (
        inputLineNumber);

      // set fVoicePendingMeasureRepeat's build phase to completed
      fVoicePendingMeasureRepeat->
        setCurrentMeasureRepeatBuildPhaseKind (
          msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseJustCreated);

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
        displayVoiceMeasureRepeatAndVoice (
          inputLineNumber,
          "handleMeasureRepeatStartInVoiceClone() 2");
      }
#endif // MF_TRACE_IS_ENABLED
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatStartInVoiceClone() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleMeasureRepeatEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling measures repeat end in voice clone \"" <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatEndInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // is there a current measures repeat in this voice?
      if (! fVoicePendingMeasureRepeat) {
        std::stringstream ss;

        ss <<
          "current voice measures repeat is null when attempting to handle measures repeat end in voice clone " <<
          asShortString ();

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      // append current voice rest measure to the initial voice elements list
      appendMeasureRepeatToInitialVoiceElements (
        inputLineNumber,
        fVoicePendingMeasureRepeat,
        "handleMeasureRepeatEndInVoiceClone() 2");

      // forget about fVoicePendingMeasureRepeat
      fVoicePendingMeasureRepeat = nullptr;

    break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatEndInVoiceClone() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleMeasureRepeatPatternStartInVoiceClone (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling measures repeat start in voice clone \"" <<
      fVoiceName <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternStartInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fVoicePendingMeasureRepeat) {
    std::stringstream ss;

    ss <<
      "current voice measures repeat is null when attempting to handle measures repeat pattern start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // create fVoicePendingMeasureRepeat' rest pattern
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a measures repeat pattern upon its start in voice \"" <<
      fVoiceName <<
      "\"" <<
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

  // register it in fVoicePendingMeasureRepeat
  fVoicePendingMeasureRepeat->
    setMeasureRepeatPattern (
      measureRepeatPattern);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternStartInVoiceClone() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleMeasureRepeatPatternEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling measures repeat end in voice clone \"" <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternEndInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fVoicePendingMeasureRepeat) {
    std::stringstream ss;

    ss <<
      "current voice measures repeat is null when attempting to handle measures repeat pattern end '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // get fVoicePendingMeasureRepeat's pattern
  S_msrMeasureRepeatPattern
    measureRepeatPattern =
      fVoicePendingMeasureRepeat->
        getMeasureRepeatPattern ();

  // set fVoiceLastSegment as measureRepeatPattern' segment
  measureRepeatPattern->
    setMeasureRepeatPatternSegment (
      fVoiceLastSegment);

  // forget about fVoiceLastSegment
 // fVoiceLastSegment = nullptr;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternEndInVoiceClone() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleMeasureRepeatReplicasStartInVoiceClone (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling measures repeat start in voice clone \"" <<
      fVoiceName <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasStartInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fVoicePendingMeasureRepeat) {
    std::stringstream ss;

    ss <<
      "current voice measures repeat is null when attempting to handle measures repeat replicas start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // create fVoicePendingMeasureRepeat' replicas
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a measures repeat replicas upon its start in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

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

  // register it in fVoicePendingMeasureRepeat
  fVoicePendingMeasureRepeat->
    setMeasureRepeatReplicas (
      measureRepeatReplicas);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasStartInVoiceClone() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleMeasureRepeatReplicasEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling measures repeat end in voice clone \"" <<
      fVoiceName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasEndInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fVoicePendingMeasureRepeat) {
    std::stringstream ss;

    ss <<
      "current voice measures repeat is null when attempting to handle measures repeat replicas end '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // get fVoicePendingMeasureRepeat's replicas
  S_msrMeasureRepeatReplicas
    measureRepeatReplicas =
      fVoicePendingMeasureRepeat->
        getMeasureRepeatReplicas ();

  // set fVoiceLastSegment as measureRepeatReplicas' segment
  measureRepeatReplicas->
    setMeasureRepeatReplicasSegment (
      fVoiceLastSegment);

  // forget about fVoiceLastSegment
 // fVoiceLastSegment = nullptr;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasEndInVoiceClone() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::appendMeasureRepeatCloneToVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasureRepeat& measureRepeatClone)
{
  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    measureRepeatClone != nullptr,
    "measureRepeatClone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  switch (fVoiceKind) { // superflous JMI ???
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) {
          std::stringstream ss;

          ss <<
            "Appending measures repeat clone '" <<
            measureRepeatClone->asString () <<
            "' to voice clone \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }

        if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
          displayVoiceMeasureRepeatAndVoice (
            inputLineNumber,
            "appendMeasureRepeatCloneToVoiceClone() 1");
        }
#endif // MF_TRACE_IS_ENABLED

        // is measures repeat nested in a repeat?
        if (! fVoicePendingRepeatsStack.empty ()) {
          // yes

          S_msrRepeat
            currentRepeat =
              fVoicePendingRepeatsStack.front ();

          // grab the measures repeat segment, i.e. the voice's last segment JMI ???
          S_msrSegment
            measureRepeatSegment =
              fVoiceLastSegment;

          // append it to the current repeat's common part
          currentRepeat->
            getRepeatCommonPart ()->
              appendSegmentToRepeatCommonPart ( // NO !!!
                inputLineNumber,
                measureRepeatSegment,
                "appendMeasureRepeatCloneToVoiceClone() 2");

          // forget about this voice last segment
          fVoiceLastSegment = nullptr;
        }

        else {
          // no
          // JMI ???
        }

        // append the measures repeat clone to the voice
        appendMeasureRepeatToVoice (
          inputLineNumber,
          measureRepeatClone);

        // print resulting voice contents
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeatsDetails ()) {
          displayVoiceMeasureRepeatAndVoice (
            inputLineNumber,
            "appendMeasureRepeatCloneToVoiceClone() 3");
        }
#endif // MF_TRACE_IS_ENABLED
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::handleHookedRepeatEndingEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a hooked repeat ending in voice \"" <<
      fVoiceName <<  "\"" <<
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
      "repeats stack is empty when attempting to handle a hooked repeat ending end in voice " <<
      asShortString ();

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // finalize current measure in voice
  finalizeLastAppendedMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // create a hooked repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kRepeatEndingHooked;

  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHookedRepeatEndingEndInVoice() 2");

  // add the repeat ending to the voice current repeat
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "before adding a hooked repeat ending to current repeat");
  }
#endif // MF_TRACE_IS_ENABLED

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleHooklessRepeatEndingEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a hookless repeat ending in voice \"" <<
      fVoiceName <<  "\"" <<
      ", line " << inputLineNumber <<
      std::endl;  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fVoicePendingRepeatsStack.empty ()) {
    std::stringstream ss;

    ss <<
      "repeats stack is empty when attempting to handle a hookless repeat ending end in voice '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // finalize current measure in voice
  finalizeLastAppendedMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // create a hookless repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kRepeatEndingHookless;

  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHookedRepeatEndingEndInVoice() 2");

  // add the repeat ending it to the voice current repeat
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "before adding a hookless repeat ending to current repeat");
  }
#endif // MF_TRACE_IS_ENABLED

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "after adding a hookless repeat ending to current repeat");
  }
#endif // MF_TRACE_IS_ENABLED

  // append currentRepeat to the list of initial elements
  appendRepeatCloneToInitialVoiceElements (
    inputLineNumber,
    currentRepeat,
    "handleHooklessRepeatEndingEndInVoice() 3");

  // pop it from the voice's repeat descrs stack
  popRepeatFromVoiceRepeatsStack (
    inputLineNumber,
    currentRepeat,
    "handleHooklessRepeatEndingEndInVoice");

  // create a new last segment containing a new measure for the voice
  createNewLastSegmentForVoice (
    inputLineNumber,
    "handleHooklessRepeatEndingEndInVoice() 4");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 5");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleRepeatEndingEndInVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string&  repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind)
{
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
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoice() 0");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleRepeatCommonPartStartInVoiceClone (
  const mfInputLineNumber& inputLineNumber) // may be "1, 2"
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat common part start in voice clone \"" <<
      fVoiceName <<  "\"" <<
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
      "handleRepeatCommonPartStartInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fVoicePendingRepeatsStack.empty ()) {
    std::stringstream ss;

    ss <<
      "repeats stack is empty when attempting to handle repeat common part start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // create currentRepeat's common part
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat common part upon its start in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        inputLineNumber,
        currentRepeat);

  // register it in currentRepeat
  currentRepeat->
    setRepeatCommonPart (
      repeatCommonPart);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartStartInVoiceClone() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleRepeatCommonPartEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber) // may be "1, 2"
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat common part end in voice clone \"" <<
      fVoiceName <<  "\"" <<
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
      "handleRepeatCommonPartEndInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fVoicePendingRepeatsStack.empty ()) {
    std::stringstream ss;

    ss <<
      "repeats stack is empty when attempting to handle repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // get currentRepeat's common part
  S_msrRepeatCommonPart
    repeatCommonPart =
      currentRepeat->
        getRepeatCommonPart ();

/* JMI
  // fetch the last segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleRepeatCommonPartEndInVoiceClone() 2");
*/

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterCommonPart);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleHookedRepeatEndingEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a hooked repeat ending in voice clone \"" <<
      fVoiceName <<  "\"" <<
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
      "handleHookedRepeatEndingEndInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fVoicePendingRepeatsStack.empty ()) {
    std::stringstream ss;

    ss <<
      "repeats stack is empty when attempting to handle hooked repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // create a hooked repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kRepeatEndingHooked;

  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

  // fetch the last segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleHookedRepeatEndingEndInVoiceClone() 2");

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHookedEnding);

  // move the voice last segment to repeatEnding
  moveVoiceLastSegmentToRepeatEnding (
    inputLineNumber,
    repeatEnding,
    "handleHookedRepeatEndingEndInVoiceClone()");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleHooklessRepeatEndingEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a hookless repeat ending in voice clone \"" <<
      fVoiceName <<  "\"" <<
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
      "handleHooklessRepeatEndingEndInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fVoicePendingRepeatsStack.empty ()) {
    std::stringstream ss;

    ss <<
      "repeats stack is empty when attempting to handle hookless repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatsStack.front ();

  // create a hookless repeat ending
  msrRepeatEndingKind
    repeatEndingKind =
      msrRepeatEndingKind::kRepeatEndingHookless;

  S_msrRepeatEnding
    repeatEnding =
      msrRepeatEnding::create (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind,
        currentRepeat);

  // fetch the last segment's last measure
  S_msrMeasure
    voiceLastSegmentLastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleHookedRepeatEndingEndInVoiceClone() 2");

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    inputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseAfterHooklessEnding);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::handleRepeatEndingEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const std::string&  repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoiceClone() 1");
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
            handleHookedRepeatEndingEndInVoiceClone (
              inputLineNumber,
              repeatEndingNumber);
            break;

          case msrRepeatEndingKind::kRepeatEndingHookless:
            handleHooklessRepeatEndingEndInVoiceClone (
              inputLineNumber,
              repeatEndingNumber);
            break;
        } // switch
      break;
    }
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoiceClone() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrVoice::handleRepeatStartInVoiceClone (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat& repeat)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start in voice clone \"" <<
      fVoiceName <<
      "\"" <<
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
      "handleRepeatStartInVoiceClone() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // fetch last segment's measure elements list
        const std::list <S_msrSegmentElement>
          voiceLastSegmentElementsList =
            fVoiceLastSegment->
              getSegmentElementsList ();

        // are there measures in the voice last segment?
        if (! voiceLastSegmentElementsList.empty ()) {
          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

          // move current last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "handleRepeatStartInVoiceClone() 2");

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceRepeatsDetails ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "handleRepeatStartInVoiceClone() 3");
          }
#endif // MF_TRACE_IS_ENABLED
        }
      }

      // create the repeat clone and stack it
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceRepeats ()) {
        std::stringstream ss;

        ss <<
          "Creating a repeat upon its start in voice clone \"" <<
          fVoiceName <<
          "\"" <<
          ", line " << inputLineNumber;

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      S_msrRepeat
        repeatClone =
          createARepeatCloneAndStackIt (
            inputLineNumber,
            repeat,
            "handleRepeatStartInVoiceClone() 4");

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceRepeatsDetails ()) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "handleRepeatStartInVoiceClone() 5");
      }
#endif // MF_TRACE_IS_ENABLED
      break;
  } // switch

  --gIndenter;
}

void msrVoice::handleRepeatEndInVoiceClone (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat end in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
      /* JMI
        // finalize current measure in voice
        finalizeLastAppendedMeasureInVoice (
          inputLineNumber);
          */

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceRepeatsDetails ()) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "handleRepeatEndInVoiceClone() 1");
        }
#endif // MF_TRACE_IS_ENABLED

        // is there a current repeat?
        switch (fVoicePendingRepeatsStack.size ()) {
          case 0:
            {
              std::stringstream ss;

              ss <<
                "repeats stack is empty when attempting to handle a repeat end in voice clone '" <<
                asShortString () <<
                "' ";

              msrInternalError (
                gServiceRunData->getInputSourceName (),
                fInputLineNumber,
                __FILE__, mfInputLineNumber (__LINE__),
                ss.str ());
            }
            break;

          case 1:
            {
              // this is a voice-level repeat

              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatsStack.front ();

              // get currentRepeat's common part
              S_msrRepeatCommonPart
                repeatCommonPart =
                  currentRepeat->
                    getRepeatCommonPart ();

              // append currentRepeat to the list of initial elements
              appendRepeatToInitialVoiceElements (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone() 2");

//               // pop currentRepeat from the voice's repeat descrs stack
//               popRepeatFromVoiceRepeatsStack (
//                 inputLineNumber,
//                 currentRepeat,
//                 "handleRepeatEndInVoiceClone() 3");

              // forget about the voice last segment JMI ??? BOF
              fVoiceLastSegment = nullptr;
            }
            break;

          default:
            {
              // this is a nested repeat

              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatsStack.front ();

              // get currentRepeat's common part
              S_msrRepeatCommonPart
                repeatCommonPart =
                  currentRepeat->
                    getRepeatCommonPart ();

              // append currentRepeat to the list of initial elements
              appendRepeatCloneToInitialVoiceElements (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone() 4");

//               // pop currentRepeat from the voice's repeat descrs stack
//               popRepeatFromVoiceRepeatsStack (
//                 inputLineNumber,
//                 currentRepeat,
//                 "handleRepeatEndInVoiceClone() 5");
            }
        } // switch


#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceRepeatsDetails ()) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "handleRepeatEndInVoiceClone() 6");
        }
#endif // MF_TRACE_IS_ENABLED
      }
      break;
  } // switch

  --gIndenter;
}

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
            "Appending a measures repeat replica to voice \"" <<
            fVoiceName <<  "\"" <<
            ", line " << inputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // fetch last measure's full measure whole notes
        /* JMI
        int fullMeasureWholeNotesDuration =
          fVoiceLastSegment->
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

        // create a measures repeat replicas
        S_msrMeasureRepeatReplicas
          measureRepeatReplicas =
            msrMeasureRepeatReplicas::create (
              inputLineNumber,
              fVoicePendingMeasureRepeat);

        // set the voice last segment as the measures repeat replicas segment
        measureRepeatReplicas->
          setMeasureRepeatReplicasSegment (
            fVoiceLastSegment);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr;

        // set the measures repeat replicas in the voice current measures repeat
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasureRepeats ()) { // // JMI 0.9.67
          std::stringstream ss;

          ss <<
            "Setting the measures repeat replica to current measures repeat BBB in voice \"" <<
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
      }
      break;
  } // switch
}

// void msrVoice::cascadeAppendMultipleMeasureRestToVoiceElementsList (
//   const S_msrMultipleMeasureRest& multipleMeasureRest)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
//     gLog <<
//       "Appending multiple measure rest " <<
//       multipleMeasureRest->asString () <<
//       " to voice \"" <<
//       fVoiceName <<
//       "\"" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     multipleMeasureRest != nullptr,
//     "multipleMeasureRest is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// // JMI 0.9.67  fVoiceInitialElementsList.push_back (multipleMeasureRest);
// }

void msrVoice::appendMeasureRepeatToVoiceElementsList (
  const S_msrMeasureRepeat& measureRepeat)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to voice \"" <<
      fVoiceName <<
      "\",";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    measureRepeat != nullptr,
    "measureRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

// JMI 0.9.67  fVoiceInitialElementsList.push_back (measureRepeat);
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
            msrRepeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending clone to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            std::endl;

          displayVoiceRepeatsStackSummary (
            repeatEndingClone->getInputLineNumber (),
            "appendRepeatEndingCloneToVoice() 1");
        }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
        // sanity check
        if (fVoicePendingRepeatsStack.empty ()) {
          std::stringstream ss;

          ss <<
            "repeats stack is empty when attempt at appending a " <<
            msrRepeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending to voice '" <<
            asShortString () <<
            "' ";

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
          displayVoiceRepeatsStackSummary (
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
      "Prepending barLine '" <<
      barLine->asString () <<
      "' to voice \"" << fVoiceName << "\"" <<
      ":";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  fVoiceLastSegment->
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
      " to voice \"" << fVoiceName << "\"" <<
      ":";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create the voice last segment if needed
  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    createNewLastSegmentForVoice (
      barLine->getInputLineNumber (),
      "appendBarLineToVoice()");
  }

  fVoiceLastSegment->
    appendBarLineToSegment (barLine);

  --gIndenter;
}

void msrVoice::appendSegnoToVoice (const S_msrSegno& segno)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegnos ()) {
    std::stringstream ss;

    ss <<
      "Appending a segno to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the voice last segment if needed
  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    createNewLastSegmentForVoice (
      segno->getInputLineNumber (),
      "appendSegnoToVoice()");
  }

  fVoiceLastSegment->
    appendSegnoToSegment (segno);
}

void msrVoice::appendCodaToVoice (const S_msrCoda& coda)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCodas ()) {
    std::stringstream ss;

    ss <<
      "Appending a coda to voice \"" << fVoiceName << "\"" <<
      ":";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the voice last segment if needed
  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    createNewLastSegmentForVoice (
      coda->getInputLineNumber (),
      "appendCodaToVoice()");
  }

  fVoiceLastSegment->
    appendCodaToSegment (coda);
}

void msrVoice::appendEyeGlassesToVoice (
  const S_msrEyeGlasses& eyeGlasses)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceEyeGlasses ()) {
    std::stringstream ss;

    ss <<
      "Appending a eyeGlasses to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the voice last segment if needed
  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    createNewLastSegmentForVoice (
      eyeGlasses->getInputLineNumber (),
      "appendEyeGlassesToVoice()");
  }

  fVoiceLastSegment->
    appendEyeGlassesToSegment (eyeGlasses);
}

void msrVoice::appendPedalToVoice (const S_msrPedal& pedal)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePedals ()) {
    std::stringstream ss;

    ss <<
      "Appending a pedal to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the voice last segment if needed
  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    createNewLastSegmentForVoice (
      pedal->getInputLineNumber (),
      "appendPedalToVoice()");
  }

  fVoiceLastSegment->
    appendPedalToSegment (pedal);
}

void msrVoice::appendDampToVoice (
  const S_msrDamp& damp)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDamps ()) {
    std::stringstream ss;

    ss <<
      "Appending a damp to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the voice last segment if needed
  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    createNewLastSegmentForVoice (
      damp->getInputLineNumber (),
      "appendDampToVoice()");
  }

  fVoiceLastSegment->
    appendDampToSegment (damp);
}

void msrVoice::appendDampAllToVoice (
  const S_msrDampAll& dampAll)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDampAlls ()) {
    std::stringstream ss;

    ss <<
      "Appending a damp all to voice \"" << fVoiceName << "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the voice last segment if needed
  if (false && ! fVoiceLastSegment) { // JMI 0.9.73
    createNewLastSegmentForVoice (
      dampAll->getInputLineNumber (),
      "appendDampAllToVoice()");
  }

  fVoiceLastSegment->
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
    fVoiceLastSegment->
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
//       "Removing note '" <<
//       note->asShortString () <<
//       "' from voice \"" << fVoiceName << "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // remove note from voice last segment
//   fVoiceLastSegment->
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
//       "Removing element '" <<
//       element->asShortString () <<
//       "' from voice \"" << fVoiceName << "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   fVoiceLastSegment->
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
//       "Removing last measure from voice \"" <<
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
//   // remove last measure
//   S_msrMeasure
//     result =
//       fVoiceLastSegment->
//         removeLastMeasureFromSegment (
//           inputLineNumber,
//           "removeLastMeasureFromVoice()");
//
//   --gIndenter;
//
//   // return it
//   return result;
// }

void msrVoice::finalizeLastAppendedMeasureInVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Finalizing last appended measure in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }

  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeLastAppendedMeasureInVoice() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

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
      "finalizeLastAppendedMeasureInVoice() 2");

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
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);
      }

      // handle the figured bass voice if any
      if (fRegularVoiceForwardLinkToFiguredBassVoice) {
        fRegularVoiceForwardLinkToFiguredBassVoice->
          finalizeLastAppendedMeasureInVoice (
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
      "finalizeLastAppendedMeasureInVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrVoice::collectVoiceMeasuresIntoFlatList (
  const mfInputLineNumber& inputLineNumber)
{
  // collect measures from the initial elements if any
  if (! fVoiceInitialElementsList.empty ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceVoices ()) {
      std::stringstream ss;

      ss <<
        "Collecting measures from the initial elements into voice \"" <<
        fVoiceName <<
        "s measures flat list" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // collect measures from the last segment if any
  if (fVoiceLastSegment) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceVoices ()) {
      std::stringstream ss;

      ss <<
        "Collecting measures from the last segment into voice \"" <<
        fVoiceName <<
        "s measures flat list" <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    std::list <S_msrMeasure>
      lastSegmentMeasuresFlatList =
        fVoiceLastSegment->
          getSegmentMeasureList ();

    if (! lastSegmentMeasuresFlatList.empty ()) {
      for (S_msrMeasure measure : lastSegmentMeasuresFlatList) {
        fVoiceMeasuresFlatList.push_back (measure);
      } // for
    }
  }
}

void msrVoice::finalizeVoice (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Finalizing voice \"" <<
      fVoiceName <<
      "\", line " << inputLineNumber <<
      std::endl;
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
        "### MAINTAINANCE ### -- " <<
        "Attempting to finalize  voice \"" <<
        asShortString () <<
        "\" more than once";

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
      "--> in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", fVoiceShortestNoteWholeNotes: " << fVoiceShortestNoteWholeNotes.asFractionString () <<
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

  // is this voice totally empty? this should be rare...
  if (
    fVoiceInitialElementsList.empty ()
      &&
    fVoiceLastSegment != nullptr // JMI should not occur??? 0.9.63
      &&
    fVoiceLastSegment->getSegmentElementsList ().empty ()
  ) {
    std::stringstream ss;

    ss <<
      "Voice \"" <<
      fVoiceName <<
      "\" is totally empty, no contents ever specified for it" <<
      std::endl;

    msrWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str ());
  }

  // are there pending repeats in the voice repeats stack???
  size_t voicePendingRepeatsStackSize =
    fVoicePendingRepeatsStack.size ();

  if (voicePendingRepeatsStackSize) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresDetails ()) {
        displayVoiceRepeatsStackSummary (
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
      " pending in the voice repeats stack in voice \"" <<
      asShortString () <<
      "\" ";

    msrWarning (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      ss.str ());
  }

  // collect the voice measures into the measures flat list
  collectVoiceMeasuresIntoFlatList (
    inputLineNumber);

  fVoiceHasBeenFinalized = true;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    displayVoiceRepeatsStackMultipleMeasureRestsMeasureRepeatAndVoice (
      inputLineNumber,
      "finalizeVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED

//
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
      "Finalizing voice \"" <<
      fVoiceName <<
      "\", line " << inputLineNumber <<
      std::endl;
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
        "### MAINTAINANCE ### -- " <<
        "Attempting to finalize  voice \"" <<
        asShortString () <<
        "\" more than once";

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
      "--> in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", fVoiceShortestNoteWholeNotes: " << fVoiceShortestNoteWholeNotes.asFractionString () <<
      ", partShortestNoteWholeNotes: " << partShortestNoteWholeNotes.asFractionString ();

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

  // is this voice totally empty? this should be rare...
  if (
    fVoiceInitialElementsList.empty ()
      &&
    fVoiceLastSegment->getSegmentElementsList ().empty ()
  ) {
    std::stringstream ss;

    ss <<
      "Voice \"" <<
      fVoiceName <<
      "\" is totally empty, no contents ever specified for it" <<
      std::endl;

    msrWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str ());
  }

  // are there pending repeats in the voice repeats stack???
  size_t voicePendingRepeatsStackSize =
    fVoicePendingRepeatsStack.size ();

  if (voicePendingRepeatsStackSize) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresDetails ()) {
        displayVoiceRepeatsStackSummary (
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
      " pending in the voice repeats stack in voice \"" <<
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesDetails ()) {
    displayVoiceRepeatsStackMultipleMeasureRestsMeasureRepeatAndVoice (
      inputLineNumber,
      "finalizeVoice() 3");
  }
#endif // MF_TRACE_IS_ENABLED

// JMI
  // finalize the harmonies voice if any
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
      "Finalizing all the measures of voice \"" <<
      fVoiceName <<
      "\", line " << inputLineNumber;

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
      " upon beam '" <<
      beam->asShortString () <<
      "' and note '" <<
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
            "whilst the current beam began with number '" <<
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
          "whilst there is no preceding beam begin with number '" <<
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
            "whilst the current beam began with number '" <<
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
            "whilst the current beam began with number '" <<
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

  // browse the voice initial elements
  if (! fVoiceInitialElementsList.empty ()) {
    for (
      std::list <S_msrVoiceElement>::const_iterator i = fVoiceInitialElementsList.begin ();
      i != fVoiceInitialElementsList.end ();
      ++i
    ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fVoiceLastSegment) {
    // browse the voice last segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fVoiceLastSegment);
  }

  // browse the voice stanzas
  if (! fVoiceStanzasMap.empty ()) {
    for (std::pair <mfStanzaNumber, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      if (stanza->getStanzaTextPresent ()) {
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
    "[Voice " << fVoicePathLikeName <<
    ", " <<
    fVoiceKind <<
//     msrVoiceKindAsStringForPrint (fVoiceKind) << // JMI
    " voice " << fVoiceName <<
    ", fVoiceNumber '" <<
    fVoiceNumber <<
    ", fRegularVoiceOrdinalNumberInPart " <<
    fRegularVoiceOrdinalNumberInPart <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrVoice::asString () const
{
  std::stringstream ss;

  ss <<
    "[Voice " << fVoicePathLikeName <<
//     msrVoiceKindAsStringForPrint (fVoiceKind) <<
    ", " <<
    fVoicePathLikeName <<
    ", " <<
    fVoiceKind <<
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
    "*********>> Displaying voice \"" <<
    fVoiceName <<
    "\" (" << context << ")" <<
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

void msrVoice::displayVoiceRepeatsStackMultipleMeasureRestsMeasureRepeatAndVoice (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context)
{
  displayVoice (
    inputLineNumber,
    context);

  displayVoiceRepeatsStack (
    inputLineNumber,
    context);

  displayVoiceMultipleMeasureRests (
    inputLineNumber,
    context);

  displayVoiceMeasureRepeat (
    inputLineNumber,
    context);
}

void msrVoice::print (std::ostream& os) const
{
  os <<
    "[Voice " << fVoicePathLikeName <<
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

#ifdef MF_TRACE_IS_ENABLED
// JMI 0.9.67 HARMFUL  displayVoiceMeasuresFlatList (fieldWidth);
#endif // MF_TRACE_IS_ENABLED

  // print the voice initial elements
  size_t voiceInitialElementsListSize =
    fVoiceInitialElementsList.size ();

  os <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fVoiceInitialElementsList";
  if (voiceInitialElementsListSize) {
    os << ": " <<  voiceInitialElementsListSize << " elements";
  }
  else {
    os << ": " << "[EMPTY]";
  }

  if (voiceInitialElementsListSize) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrVoiceElement>::const_iterator
      iBegin = fVoiceInitialElementsList.begin (),
      iEnd   = fVoiceInitialElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  os << std::endl;

  os << std::endl;

  // print the last segment
  os <<
    std::setw (fieldWidth) <<
    "fVoiceLastSegment" << ": ";
  if (fVoiceLastSegment) {
    os << std::endl;

    ++gIndenter;
    os << fVoiceLastSegment;
    --gIndenter;
  }
  else {
    os <<
      "[NULL]" <<
      std::endl;
  }

  os << std::endl;

  // print the voice uplink to staff if any
  os << std::left <<
    std::setw (fieldWidth) << "fVoiceUpLinkToStaff" << ": ";
  if (fVoiceUpLinkToStaff) {
    os <<
      fVoiceUpLinkToStaff->getStaffPathLikeName ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  // print the stanzas if any
  if (! fVoiceStanzasMap.empty ()) {
    os <<
      std::endl <<
      "Stanzas:" <<
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
    "[Voice " << fVoiceName <<
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
    "fVoiceContainsMultipleMeasureRests" << ": " <<
    fVoiceContainsMultipleMeasureRests <<
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
    msrVoiceRepeatPhaseKindAsString (
      fCurrentVoiceRepeatPhaseKind) <<
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
//

  // print the voice first segment if any
  os <<
    std::setw (fieldWidth) <<
    "fVoiceFirstSegment" << ": ";
  if (fVoiceFirstSegment) {
    os <<
      '\'' <<
      fVoiceFirstSegment->getSegmentAbsoluteNumber () <<
      "'";
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

  // print the voice initial elements
  size_t voiceInitialElementsListSize =
    fVoiceInitialElementsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fVoiceInitialElementsList" << ": ";

  if (voiceInitialElementsListSize) {
    os <<
      voiceInitialElementsListSize <<
      " elements" <<
      std::endl;

    ++gIndenter;

    for (
      std::list <S_msrVoiceElement>::const_iterator i =
        fVoiceInitialElementsList.begin ();
      i != fVoiceInitialElementsList.end ();
      ++i
    ) {
      S_msrVoiceElement voiceElement = (*i);

      // print the element
      os << voiceElement;
      if (++i == fVoiceInitialElementsList.end ()) break; //JMI 0.9.67ts
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << std::endl;

  // print the last segment
  os <<
    std::setw (fieldWidth) <<
    "fVoiceLastSegment" << ": ";

  if (fVoiceLastSegment) {
    ++gIndenter;

    os <<
      std::endl;

    fVoiceLastSegment->printFull (os);

    --gIndenter;
  }
  else {
    os <<
      "[NULL]" <<
      std::endl;
  }

  os << std::endl;

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
    os << "[NULL]" << std::endl;
  }

  return os;
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
//         // move the current voice last segment to the initial elements list
//         moveVoiceLastSegmentToInitialVoiceElementsIfRelevant ( //JMI
//           inputLineNumber,
//           "appendEmptyMeasuresToVoice() 2");
//
//         // create the multiple measure rests
//         if (fVoiceCurrentMultipleMeasureRest) {
//           std::stringstream ss;
//
//           ss <<
//             "attempt at creating a multiple measure rests while another one is pending";
//
//           msrInternalError (
//             gServiceRunData->getInputSourceName (),
//             inputLineNumber,
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str ());
//         }
//
// /* JMI
//         fVoiceCurrentMultipleMeasureRest =
//           msrMultipleMeasureRest::create (
//             inputLineNumber,
//             firstRestMeasure->getFullMeasureWholeNotesDuration (),
//             multipleMeasureRestMeasuresNumber,
//             this);
// */
//
//          // remember fVoiceCurrentMultipleMeasureRest for later next measure number setting JMI ???
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
//           gLog <<
//             "Registering multiple measure rests " <<
//             fVoiceCurrentMultipleMeasureRest->asString () <<
//             " as waiting for its next measure number" <<
//             ", multipleMeasureRestMeasuresNumber: " <<
//             multipleMeasureRestMeasuresNumber <<
//             " in voice \"" <<
//             fVoiceName << "\"" <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         fVoiceMultipleMeasureRestsWaitingForItsNextMeasureNumber =
//           fVoiceCurrentMultipleMeasureRest;
//
//         fVoiceRemainingMultipleMeasureRests =
//           multipleMeasureRestMeasuresNumber;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
//           gLog <<
//             "Setting fVoiceRemainingMultipleMeasureRests to '" <<
//             fVoiceRemainingMultipleMeasureRests <<
//             "' in voice \"" <<
//             fVoiceName << "\"" <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         // create a new segment to collect the multiple measure rests,
//         // containing the first, rest measure
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
//           gLog <<
//             "Creating a new last segment to add the first, rest measure in voice \"" <<
//             fVoiceName << "\"" <<
//             ", line " << inputLineNumber <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
// /* JMI
//         createNewLastSegmentFromItsFirstMeasureForVoice (
//           inputLineNumber,
//           firstRestMeasure,
//           "appendEmptyMeasuresToVoice() 3");
// */
//
//         // this voice contails multiple measure rests
//         this->setVoiceContainsMultipleMeasureRests (
//           inputLineNumber);
//
//         // keep the multiple measure rests pending
//
//         --gIndenter;
//       }
//       break;
//   } // switch

