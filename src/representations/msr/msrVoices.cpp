/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "mfStaticSettings.h"

#include "visitor.h"


#include "msrWae.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "oahOah.h"
#include "oahEarlyOptions.h"

#include "msrOah.h"

#include "msrVoices.h"

#include "msrBreaks.h"
#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrDoubleTremolos.h"
#include "msrMeasureRepeats.h"
#include "msrRehearsalMarks.h"
#include "msrRepeats.h"
#include "msrRepeatConstants.h"
#include "msrStaves.h"
#include "msrTempos.h"
#include "msrVoiceStaffChanges.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

// voices
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
S_msrRepeatDescr msrRepeatDescr::create (
  int                repeatDescrStartInputLineNumber,
  const S_msrRepeat& repeatDescrRepeat)
{
  msrRepeatDescr* o = new
    msrRepeatDescr (
      repeatDescrStartInputLineNumber,
      repeatDescrRepeat);
  assert (o != nullptr);
  return o;
}

msrRepeatDescr::msrRepeatDescr (
  int                repeatDescrStartInputLineNumber,
  const S_msrRepeat& repeatDescrRepeat)
{
  fRepeatDescrStartInputLineNumber =
    repeatDescrStartInputLineNumber;
  fRepeatDescrRepeat = repeatDescrRepeat;
}

msrRepeatDescr::~msrRepeatDescr ()
{}

std::string msrRepeatDescr::asString () const
{
  std::stringstream s;

  s <<
    "[msrRepeatDescr" <<
    ", fRepeatDescrRepeat: " <<
    fRepeatDescrRepeat->asShortString () <<
    ", fRepeatDescrStartInputLineNumber: " <<
    fRepeatDescrStartInputLineNumber <<
    ']';

  return s.str ();
}

void msrRepeatDescr::print (std::ostream& os) const
{
  const int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fRepeatDescrRepeat" << ": " <<
    fRepeatDescrRepeat <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fRepeatDescrStartInputLineNumber" << ": " <<
    fRepeatDescrStartInputLineNumber <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrRepeatDescr& elt)
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
// constants
const int msrVoice::K_VOICE_NUMBER_UNKNOWN                      = -99;
const int msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER    =  20;
const int msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER =  40;

// global variable
int msrVoice::gVoicesCounter = 0;

S_msrVoice msrVoice::create (
  int               inputLineNumber,
  msrVoiceKind      voiceKind,
  int               voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
                    voiceCreateInitialLastSegmentKind,
  const S_msrStaff& voiceUpLinkToStaff)
{
  msrVoice* o =
    new msrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceCreateInitialLastSegmentKind,
      voiceUpLinkToStaff);
  assert (o != nullptr);
  return o;
}

S_msrVoice msrVoice::createRegularVoice (
  int               inputLineNumber,
  int               voiceNumber,
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
  int               inputLineNumber,
  int               voiceNumber,
  const S_msrStaff& voiceUpLinkToStaff) // unused yet JMI v0.9.66
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
  int               inputLineNumber,
  int               voiceNumber,
  const S_msrStaff& voiceUpLinkToStaff) // unused yet JMIv0.9.66
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
  int               inputLineNumber,
  msrVoiceKind      voiceKind,
  int               voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
                    voiceCreateInitialLastSegmentKind,
  const S_msrStaff& voiceUpLinkToStaff)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceUpLinkToStaff != nullptr,
    "voiceUpLinkToStaff is null");
#endif

  // set voice staff upLink
  fVoiceUpLinkToStaff = voiceUpLinkToStaff;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice number
  fVoiceNumber = voiceNumber;

  // do other initializations
  initializeVoice (
    voiceCreateInitialLastSegmentKind);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating voice \"" << asString () << "\"" <<
      std::endl;
  }
#endif
}

msrVoice::msrVoice (
  int          inputLineNumber,
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating voice \"" << asString () << "\"" <<
      std::endl;
  }
#endif
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Setting the regular voice staff sequential number of voice \"" <<
      getVoiceName () <<
      "\" to " << regularVoiceStaffSequentialNumber <<
      std::endl;
  }
#endif

  fRegularVoiceStaffSequentialNumber =
    regularVoiceStaffSequentialNumber;
}

void msrVoice::setVoiceNameFromNumber (
  int inputLineNumber,
  int voiceNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Setting the name of '" <<
      msrVoiceKindAsString (fVoiceKind) <<
      "' voice from voice number '" << voiceNumber <<
      "'" <<
      std::endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      fVoiceName =
        fVoiceUpLinkToStaff->getStaffName () +
        "_Voice_" +
        mfInt2EnglishWord (voiceNumber);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      fVoiceName =
        fVoiceUpLinkToStaff->getStaffName () +
        "_Voice_" +
        mfInt2EnglishWord (
          voiceNumber) + // JMI - msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER) +
          "_HARMONIES";
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      fVoiceName =
        fVoiceUpLinkToStaff->getStaffName () +
        "_Voice_" +
        mfInt2EnglishWord (
          voiceNumber) + // JMI - msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) +
          "_FIGURED_BASS";
      break;
  } // switch

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "The resulting voice name is \"" <<
      fVoiceName <<
      "\"" <<
      std::endl;
  }
#endif
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
  int voiceNumber =
    gGlobalMsrOahGroup->getCreateVoicesStaffRelativeNumbers () // JMI use it
      ? fRegularVoiceStaffSequentialNumber
      : fVoiceNumber;

  // set voice name
  setVoiceNameFromNumber (
    fInputLineNumber,
    voiceNumber);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Initializing voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceUpLinkToStaff->getStaffName () <<
      "\"" <<
      std::endl;
  }
#endif

  // check voice number
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // the voice number should be positive
      if (fVoiceNumber < 1 || fVoiceNumber > 4) {
        std::stringstream s;

        s <<
          "regular voice number " << fVoiceNumber <<
          " is not in the 1..4 range";

          /* JMI ???
        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
    /* JMI
      if (fVoiceNumber != msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER) {
        std::stringstream s;

        s <<
          "figured bass voice number " << fVoiceNumber <<
          " is not equal to " << msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      */
      break;
  } // switch

  // voice shortest note
  fVoiceShortestNoteDuration =
    Rational (INT_MAX, 1);
  fVoiceShortestNoteTupletFactor =
    Rational (1, 1);

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
  fMusicHasBeenInsertedInVoice = false;

  // counters
  fVoiceActualNotesCounter         = 0;
  fVoiceRestsCounter               = 0;
  fVoiceSkipsCounter               = 0;
  fVoiceActualHarmoniesCounter     = 0;
  fVoiceActualFiguredBassesCounter = 0;

  // regular measure ends detection
  fWholeNotesSinceLastRegularMeasureEnd = Rational (0, 1);

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    fInputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseNone);

  // multiple full-bar rests
  fVoiceContainsMultipleFullBarRests  = false;
  fVoiceRemainingMultipleFullBarRests = 0;

  // measures repests
  fVoiceContainsMeasureRepeats = false;

    // voice position
  fCurrentVoicePosition = Rational (0, 1);
  fCurrentVoiceMoment = msrMoment (Rational (0,1), Rational (0,1));

  // voice finalization
  fVoiceHasBeenFinalized = false;

  // calls counter (for debug)
  fCallsCounter = 0;

  // create the initial last segment if requested
  switch (voiceCreateInitialLastSegmentKind) {
    case msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes:
      // sanity check // JMI LAST
      mfAssert (
        __FILE__, __LINE__,
        fVoiceLastSegment == nullptr,
        "fVoiceLastSegment is null");

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    gLogStream <<
      "Initial contents of voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceUpLinkToStaff->getStaffName () <<
      "\":" <<
      std::endl;

    ++gIndenter;

    this->print (gLogStream);

    --gIndenter;
  }
#endif

  --gIndenter;
}

void msrVoice::changeVoiceIdentity ( // after a deep clone is created
  int voiceNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Changing identity of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", number: " << voiceNumber <<
      std::endl;
  }
#endif

  // make it a regular voice
  setVoiceKind (
    msrVoiceKind::kVoiceKindRegular);

  // set its voice number
  setVoiceNumber (
    voiceNumber);

  // set its name
  setVoiceNameFromNumber (
    fInputLineNumber,
    voiceNumber);
}

S_msrVoice msrVoice::createVoiceNewbornClone (
  const S_msrStaff& staffClone)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating a newborn clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    staffClone != nullptr,
    "staffClone is null");
#endif

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

  // voice name
  newbornClone->fVoiceName =
    fVoiceName;

  return newbornClone;
}

S_msrVoice msrVoice::createVoiceDeepClone (
  int               inputLineNumber,
  msrVoiceKind      voiceKind,
  int               voiceNumber,
  const S_msrStaff& containingStaff)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating a deep clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", to be placed in containing staff \"" <<
      containingStaff->getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingStaff != nullptr,
    "containingStaff is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    gLogStream <<
      "****" <<
      " BEFORE deepClone = " <<
      std::endl;

    print (gLogStream);

    gLogStream <<
      std::endl <<
      "****" <<
      std::endl;
  }
#endif

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

  // voice name
  if (false) { // JMI
    deepClone->fVoiceName =
      fVoiceName;
  }

/* JMI
  // set voice name // JMI
  deepClone->
    setVoiceNameFromNumber (
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
  deepClone->fMusicHasBeenInsertedInVoice =
    fMusicHasBeenInsertedInVoice;

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
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "There are " <<
        numberOfInitialElements <<
        " initial elements in voice \"" <<
        getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif

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
        std::stringstream s;

        s <<
          "voice  \"" <<
          getVoiceName () <<
          "\" initial elements element should be a repeat or a segment";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // append the element deep clone to the voice deep clone
      deepClone->
        fVoiceInitialElementsList.push_back (
          elementDeepClone);
    } // for
  }

  else {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "There are no initial elements in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        std::endl;
    }
#endif
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
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "There is no last segment in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        std::endl;
    }
#endif
  }

  // multiple full-bar rests
  deepClone->fVoiceContainsMultipleFullBarRests =
    fVoiceContainsMultipleFullBarRests;

  // measures reeats
  deepClone->fVoiceContainsMeasureRepeats =
    fVoiceContainsMeasureRepeats;

  // stanzas
  for (std::pair<std::string, S_msrStanza> thePair : fVoiceStanzasMap) {
    const S_msrStanza& stanza = thePair.second;

    deepClone->
      addStanzaToVoiceWithoutCatchUp ( // JMI
        stanza->
          createStanzaDeepClone (
            deepClone));
  } // for

  // upLinks
  deepClone->fVoiceUpLinkToStaff =
    containingStaff;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    gLogStream <<
      "****" <<
      " AFTER deepClone = " <<
      std::endl <<
      deepClone <<
      std::endl <<
      "****" <<
      std::endl;
  }
#endif

  return deepClone;
}

void msrVoice::setVoiceLastSegmentInVoiceClone (
  const S_msrSegment& segment)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Setting segment '" <<
      segment->asShortString () <<
      "' as last segment in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << segment->getInputLineNumber () <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");
#endif

  fVoiceLastSegment = segment;

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
}

void msrVoice::appendSegmentToVoiceClone ( //JMI unused ???
  const S_msrSegment& segment)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending segment '" <<
      segment->asString () <<
      "' to voice clone \"" <<
      fVoiceName <<
      "\"" <<
      std::endl;
  }
#endif

  int inputLineNumber =
    segment->getInputLineNumber ();

  // does segment belong to a repeat?
  if (fVoicePendingRepeatDescrsStack.size ()) {
    // yes

 // JMI   if (fVoiceLastSegment) {

    // append segment to whichever part of the repeat is adequate
    S_msrRepeat
      currentRepeat =
        fVoicePendingRepeatDescrsStack.front ()->
          getRepeatDescrRepeat ();

    currentRepeat->
      appendSegmentToRepeat (
        inputLineNumber,
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
      inputLineNumber,
      "appendSegmentToVoiceClone()");
      */

    // segment becomes the fVoiceLastSegment
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Segment '" <<
        segment->asShortString () <<
        "' becomes the new last segment in voice clone \"" <<
        fVoiceName <<
        "\"" <<
        std::endl;
    }
#endif

    fVoiceLastSegment = segment;

    if (! fVoiceFirstSegment) {
      fVoiceFirstSegment = fVoiceLastSegment;
    }
  }
}

void msrVoice::setVoiceLastAppendedMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Setting voice last appended measure to ";

    if (measure) {
      gLogStream <<
        measure->asString ();
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      " in voice " <<
      asString () <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fVoiceLastAppendedMeasure = measure;
}

void msrVoice::setNextMeasureNumberInVoice (
  int           inputLineNumber,
  const std::string& nextMeasureNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    setNextMeasureNumberInSegment (
      inputLineNumber,
      nextMeasureNumber);

  // is there a current multiple full-bar rests in this voice?
  if (fVoiceMultipleFullBarRestsWaitingForItsNextMeasureNumber) {
    // yes
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
      gLogStream <<
        "There is a multiple full-bar rests waiting for its next measure number" <<
        ", fVoiceRemainingMultipleFullBarRests = " <<
        fVoiceRemainingMultipleFullBarRests <<
        "' in voice \"" <<
        fVoiceName << "\"" <<
        std::endl;
    }
#endif

    --fVoiceRemainingMultipleFullBarRests;

    // is this the last measure in the row?
    if (fVoiceRemainingMultipleFullBarRests == 0) {
      // yes, set waiting multiple full-bar rests's next measure number
#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
        gLogStream <<
          "Setting multiple full-bar rests next measure number to '" <<
          nextMeasureNumber <<
          "' in voice \"" <<
          fVoiceName << "\"" <<
          std::endl;
      }
#endif

      fVoiceMultipleFullBarRestsWaitingForItsNextMeasureNumber->
        setMultipleFullBarRestsNextMeasureNumber (
          nextMeasureNumber);

      // forget about this waiting multiple full-bar rests
      fVoiceMultipleFullBarRestsWaitingForItsNextMeasureNumber = nullptr;
    }
  }

  --gIndenter;
}

void msrVoice::incrementVoiceCurrentMeasurePuristNumber (
  int           inputLineNumber,
  const std::string& context)
{
  if (fVoiceCurrentMeasurePuristNumber < 0) {
    fVoiceCurrentMeasurePuristNumber = 0;

    fVoiceFirstMeasurePuristNumber =
      fVoiceCurrentMeasurePuristNumber;
  }
  else {
    ++fVoiceCurrentMeasurePuristNumber;
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Incrementing voice current measure purist number to '" <<
      fVoiceCurrentMeasurePuristNumber <<
      "' (" << context << ")" <<
      " in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif
}

void msrVoice::setVoiceFirstMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Measure " <<
      measure->asShortString () <<
      " is the first measure in  voice \"" << getVoiceName () << "\"" <<
      "', line " << measure->getInputLineNumber () <<
      std::endl;
  }
#endif

  fVoiceFirstMeasure = measure;
}

void msrVoice::appendMeasureCloneToVoiceClone (
  int          inputLineNumber,
  const S_msrMeasure& measureClone)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending measure clone '" <<
      measureClone->asShortString () <<
      "' to voice '" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

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
  int             inputLineNumber,
  const Rational& value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting voice whole notes since last regular measure end to '" <<
      value <<
  // JMI    "' (" << context << ")" <<
      " in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  fWholeNotesSinceLastRegularMeasureEnd = value;
}

void msrVoice::setCurrentVoiceRepeatPhaseKind (
  int      inputLineNumber,
  msrVoiceRepeatPhaseKind
           afterRepeatComponentPhaseKind)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Setting voice current after repeat component phase kind to '" <<
      msrVoiceRepeatPhaseKindAsString (
        afterRepeatComponentPhaseKind) <<
 // JMI     "' (" << context << ")" <<
      " in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentVoiceRepeatPhaseKind =
    afterRepeatComponentPhaseKind;
}

void msrVoice::createNewLastSegmentForVoice (
  int           inputLineNumber,
  const std::string& context)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a new last segment for voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }

  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    std::string
      combinedContext =
        "createNewLastSegmentForVoice() 1 called from " + context;

    displayVoice (
      inputLineNumber,
      combinedContext);
  }
#endif

  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    std::string
      combinedContext =
        "createNewLastSegmentForVoice() 2 called from " + context;

    displayVoice (
      inputLineNumber,
      combinedContext);
  }
#endif
}

void msrVoice::createNewLastSegmentFromItsFirstMeasureForVoice (
  int           inputLineNumber,
  const S_msrMeasure&  firstMeasure,
  const std::string& context)
{
  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a new last segment '" <<
      fVoiceLastSegment->asShortString () <<
      "' from its first measure '" <<
      firstMeasure->getMeasureNumber () <<
      "' for voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createNewLastSegmentFromItsFirstMeasureForVoice()");
  }
#endif
}

S_msrMeasure msrVoice::createAMeasureAndAppendItToVoice (
  int           inputLineNumber,
  int           previousMeasureEndInputLineNumber,
  const std::string& measureNumber,
  msrMeasureImplicitKind
                measureImplicitKind)
{
  S_msrMeasure result;

  fVoiceCurrentMeasureNumber = measureNumber;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCallsCounter++;

  if (
//     true
//       ||
    false
      &&
    (
      fCallsCounter == 2 && getVoiceName ()
        ==
      "Part_POne_HARMONIES_Staff_Voice_Eleven_HARMONIES"
    )
  ) { // POUSSE JMI
    gLogStream <<
      std::endl <<
      "++++ createAMeasureAndAppendItToVoice() POUSSE, fCallsCounter: " << fCallsCounter << " ++++" <<
      std::endl;
    this->print (gLogStream);
    gLogStream <<
      std::endl;
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createAMeasureAndAppendItToVoice() 1");
  }
#endif

  ++gIndenter;

  // is there a current multiple full-bar rests?
  if (fVoiceCurrentMultipleFullBarRests) {
    // yes

    // create a measure
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating a measure with number '" <<
      measureNumber <<
      "' in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

    result =
      msrMeasure::create (
        inputLineNumber,
        measureNumber,
        fVoiceLastSegment); // JMI KAKA  v0.9.63 ???

    // set result's ordinal number
    result->
      setMeasureOrdinalNumberInVoice (
        incrementVoiceCurrentMeasureOrdinalNumber ());

    // append it to the current multiple full-bar rests
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending measure " <<
      result->asString () <<
      " to current multiple full-bar rests " <<
      fVoiceCurrentMultipleFullBarRests->asString () <<
      "in voice \"" <<
      getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

    fVoiceCurrentMultipleFullBarRests->
      appendMeasureToMultipleFullBarRests (
        result);
  }

  else {
    // no

    // create the voice last segment if needed
    if (! fVoiceLastSegment) {
      createNewLastSegmentForVoice (
        inputLineNumber,
        "createAMeasureAndAppendItToVoice() 2");
    }

    // append a new measure with given number to voice last segment
    result =
      fVoiceLastSegment->
        createAMeasureAndAppendItToSegment (
          inputLineNumber,
          previousMeasureEndInputLineNumber,
          measureNumber,
          measureImplicitKind);
  }

  // result is the new voice last appended measure
  setVoiceLastAppendedMeasure (
    result);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createAMeasureAndAppendItToVoice() 3");
  }
#endif

  --gIndenter;

  return result;
}

S_msrVoice msrVoice::createRegularVoiceHarmoniesVoice (
  int           inputLineNumber,
  const std::string& currentMeasureNumber)
{
  if (fRegularVoiceForwardLinkToHarmoniesVoice) {
    std::stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" already has a harmonies voice";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice harmonies voice
  int regularVoiceHarmoniesVoiceNumber =
    msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER + fVoiceNumber;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    std::stringstream s;

    s <<
      "Creating harmonies voice for regular voice \"" <<
      getVoiceName () <<
      "\" with voice number " <<
      regularVoiceHarmoniesVoiceNumber <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      s.str ());
  }
#endif

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
//   int           inputLineNumber,
//   const std::string& currentMeasureNumber)
// {
//   if (fRegularVoiceForwardLinkToFiguredBassVoice) {
//     std::stringstream s;
//
//     s <<
//       "Voice \"" <<
//       getVoiceName () <<
//       "\" already has a figured bass voice";
//
//     msrInternalError (
//       gGlobalServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, __LINE__,
//       s.str ());
//   }
//
//   // create the voice figured bass voice
//   int regularVoiceFiguredBassVoiceNumber =
//     msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + fVoiceNumber;
//
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceFiguredBasses ()) {
//     gLogStream <<
//       "Creating figured bass voice for regular voice \"" <<
//       getVoiceName () <<
//       "\" with voice number " <<
//       regularVoiceFiguredBassVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif
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
  int           inputLineNumber,
  const std::string& stanzaNumber)
{
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    std::stringstream s;

    s <<
      "stanza " << stanzaNumber <<
      " already exists in voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
  std::string stanzaNumber =
    stanza->getStanzaNumber ();

  // register stanza in this voice
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

void msrVoice::addStanzaToVoiceWithoutCatchUp (const S_msrStanza& stanza)
{
  // get stanza number
  std::string stanzaNumber =
    stanza->getStanzaNumber ();

  // register stanza in this voice
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

S_msrStanza msrVoice::createStanzaInVoiceIfNotYetDone (
  int           inputLineNumber,
  const std::string& stanzaNumber,
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
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLyrics ()) {
      gLogStream <<
        "Creating stanza" <<
        " number " << stanzaNumber <<
        ", name \"" << stanzaName << "\"" <<
        ", in voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        ", fVoiceStanzasMap.size (): " << fVoiceStanzasMap.size () <<
        std::endl;
    }
#endif

    stanza =
      addStanzaToVoiceByItsNumber (
        inputLineNumber, stanzaNumber);
  }

  return stanza;
}

S_msrStanza msrVoice::fetchStanzaInVoice (
  int           inputLineNumber,
  const std::string& stanzaNumber,
  const std::string& stanzaName) // JMI
{
  S_msrStanza stanza;

  // is stanzaNumber known in voice?
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    // yes, use it
    stanza =
      fVoiceStanzasMap [stanzaNumber];
  }

  else {
    std::stringstream s;

    gLogStream <<
      std::endl << std::endl << std::endl <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      std::endl;

    print (gLogStream);

    gLogStream <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      std::endl << std::endl << std::endl <<
      std::endl;


    s <<
      "Stanza" <<
      " number " << stanzaNumber <<
      ", name \"" << stanzaName << "\"" <<
      ", not found in voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      ", fVoiceStanzasMap.size (): " << fVoiceStanzasMap.size () <<
      std::endl;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
  const S_msrTimeSignature& timeSignature){
  fVoiceCurrentTimeSignature = timeSignature;
};

void msrVoice::appendMusicXMLPrintLayoutToVoice (
  const S_msrMusicXMLPrintLayout& musicXMLPrintLayout)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMusicXMLPrintLayouts ()) {
    gLogStream <<
      "Appending print layout '" << musicXMLPrintLayout->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    appendMusicXMLPrintLayoutToSegment (musicXMLPrintLayout);

  --gIndenter;
}

void msrVoice::appendClefToVoice  (
  const S_msrClef& clef)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" << clef->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // set voice current clef
  this->setVoiceCurrentClef (clef);

  if (fMusicHasBeenInsertedInVoice) {
    // append clef to last segment
    fVoiceLastSegment->
      appendClefToSegment (clef);
  }

  else {
    // moving clefs to the left, thus prepend to last segment
    fVoiceLastSegment->
      prependClefToSegment (clef);
  }

  --gIndenter;
}

void msrVoice::appendKeyToVoice (
  const S_msrKey& key)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " << key->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // set voice current clef
  this->setVoiceCurrentKey (key);

  // append key to last segment
  fVoiceLastSegment->
    appendKeyToSegment (key);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeysDetails ()) {
    displayVoice ( // JMI TEMP
      key->getInputLineNumber (),
      "appendKeyToVoice()");
  }
#endif

  --gIndenter;
}

void msrVoice::appendTimeSignatureToVoice (
  const S_msrTimeSignature& timeSignature){
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" << timeSignature->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" << timeSignature->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // set voice current time signature
  this->setVoiceCurrentTimeSignature (timeSignature);

  // append timeSignature to the last segment
  fVoiceLastSegment->
    appendTimeSignatureToSegmentClone (timeSignature);

  --gIndenter;
}

void msrVoice::insertHiddenMeasureAndBarLineInVoiceClone (
  int             inputLineNumber,
  const Rational& measurePosition)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      measurePosition <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // insert hidden measure and barLine to the last segment
  fVoiceLastSegment->
    insertHiddenMeasureAndBarLineInSegmentClone (
      inputLineNumber,
      measurePosition);

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
      const std::list<S_msrMeasureElement>&
        firstMeasureElementsList =
          voiceFirstMeasure->
            getMeasureElementsList ();

      // fetch the first note in the first measure to which
      // a grace notes group can be attached
      // i.e. one not in a grace notes group itself,
      // possibly inside a chord or tuplet

      if (firstMeasureElementsList.size ()) {
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
            std::stringstream s;

            s <<
              "tuplet first measureElement should be a note, a chord or another tuplet, found instead '" <<
              measureElement->asShortString () <<
              "'";

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
              */
          }
        } // for
      }
    }
  }

//     else {
// #ifdef MF_TRACING_IS_ENABLED
//       if (true || gGlobalTracingOahGroup->getTraceMeasuresDetails ()) { // KAKA  // JMI v0.9.66
//         gLogStream <<
//           "++++++++++ fetchVoiceFirstNonGraceNote(), this voice: ++++++++++" <<
//           std::endl;
//         ++gIndenter;
//         gLogStream << this;;
//         --gIndenter;
//       }
// #endif
//
//       std::stringstream s;
//
//       s <<
//         "there is no first measure in the first segment in voice \"" << // JMI v0.9.63  // JMI v0.9.66
//         getVoiceName () <<
//         "\", since the latter is empty";
//
//       msrInternalError (
//         gGlobalServiceRunData->getInputSourceName (),
//         fInputLineNumber,
//         __FILE__, __LINE__,
//         s.str ());
//     }

  return result;
}

void msrVoice::setVoiceShortestNoteDuration (
  const Rational& duration)
{
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note duration in voice \"" <<
        getVoiceName () <<
        "\" becomes " <<
        duration <<
        std::endl;
    }
#endif

  fVoiceShortestNoteDuration = duration;
}

void msrVoice::setVoiceShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note tuplet factor in part \"" <<
        getVoiceName () <<
        "\" becomes " <<
        noteTupletFactor <<
        std::endl;
    }
#endif

  fVoiceShortestNoteTupletFactor = noteTupletFactor;
}

void msrVoice::registerShortestNoteInVoiceIfRelevant (const S_msrNote& note)
{
  // is note the shortest one in this voice?
  Rational
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

      /* JMI
  Rational
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();
        */

  if (noteSoundingWholeNotes < fVoiceShortestNoteDuration) {
    // set the voice shortest note duration
    this->
      setVoiceShortestNoteDuration (
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

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note in voice \"" << getVoiceName () << "\"" <<
        " becomes " << note->asString () <<
        std::endl;
    }
#endif
  }

/* JMI
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }
    */
}

void msrVoice::registerNoteAsVoiceLastAppendedNote (const S_msrNote& note)
{
  fVoiceLastAppendedNote = note;

  // is note the shortest one in this voice?
  Rational
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes (),
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes (); // JMI

  if (noteSoundingWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteSoundingWholeNotes;
  }
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }
}

void msrVoice::appendHarmonyToVoice (
  const S_msrHarmony& harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    std::stringstream s;

    s <<
      "Appending harmony " << harmony->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      s.str ());
  }
#endif

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindHarmonies:
      appendHarmonyToHarmoniesVoice (
        harmony);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        std::stringstream s;

        s <<
          "cannot append a harmony to " <<
          msrVoiceKindAsString (fVoiceKind) <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
#endif

  --gIndenter;
}

void msrVoice::appendHarmonyToHarmoniesVoice (
  const S_msrHarmony& harmony)
{
#ifdef MF_TRACING_IS_ENABLED
  int inputLineNumber =
    harmony->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    std::stringstream s;

    s <<
      "Appending harmony " << harmony->asString () <<
      " to harmonies voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      s.str ());
  }
#endif

  ++gIndenter;

  // append the harmony to the voice last segment
  fVoiceLastSegment->
    appendHarmonyToSegment (harmony);

  // register harmony
  ++fVoiceActualHarmoniesCounter;
  fMusicHasBeenInsertedInVoice = true;

  --gIndenter;
}

void msrVoice::appendFiguredBassToFiguredBassVoice (
  const S_msrFiguredBass& figuredBass)
{
#ifdef MF_TRACING_IS_ENABLED
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    std::stringstream s;

    s <<
      "Appending figured bass " << figuredBass->asString () <<
      " to figured bass voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      s.str ());
  }
#endif

  ++gIndenter;

  // append the figuredBass to the voice last segment
  fVoiceLastSegment->
    appendFiguredBassToSegment (figuredBass);

  // register harmony
  ++fVoiceActualFiguredBassesCounter;
  fMusicHasBeenInsertedInVoice = true;

  --gIndenter;
}

void msrVoice::appendHarmonyToVoiceClone (const S_msrHarmony& harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    std::stringstream s;

    s <<
      "Appending harmony " << harmony->asString () <<
      " to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      s.str ());
  }
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindHarmonies:
      fVoiceLastSegment->
        appendHarmonyToSegmentClone (harmony);

      // register harmony
      ++fVoiceActualHarmoniesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        std::stringstream s;

        s <<
          "cannot append a harmony to " <<
          msrVoiceKindAsString (fVoiceKind) <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendFiguredBassToVoice (
  const S_msrFiguredBass& figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      appendFiguredBassToFiguredBassVoice (
        figuredBass);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        std::stringstream s;

        s <<
          "cannot append a figured bass to " <<
          msrVoiceKindAsString (fVoiceKind) <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
#endif

  --gIndenter;
}

void msrVoice::appendFiguredBassToVoiceClone (
  const S_msrFiguredBass& figuredBass)
{
#ifdef MF_TRACING_IS_ENABLED
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Appending figured bass " << figuredBass->asString () <<
      " to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      fVoiceLastSegment->
        appendFiguredBassToSegmentClone (figuredBass);

      // register figured bass
      ++fVoiceActualFiguredBassesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        std::stringstream s;

        s <<
          "cannot append a figured bass to " <<
          msrVoiceKindAsString (fVoiceKind) <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          figuredBass->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::padUpToMeasurePositionInVoice (
  int             inputLineNumber,
  const Rational& wholeNotesMeasurePosition)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Padding up to measure position '" <<
      wholeNotesMeasurePosition <<
      "' whole notes in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");
#endif

  // pad up the voice's last segment
  fVoiceLastSegment->
    padUpToMeasurePositionInSegment (
      inputLineNumber,
      wholeNotesMeasurePosition);

  // pad up the voice's stanzas // JMI ???
  if (fVoiceStanzasMap.size ()) {
    for (std::pair<std::string, S_msrStanza> thePair : fVoiceStanzasMap) {
      const S_msrStanza& stanza = thePair.second;

      stanza->
        padUpToMeasureWholeNotesDurationInStanza (
          inputLineNumber,
          wholeNotesMeasurePosition);
    } // for
  }

  --gIndenter;
}

void msrVoice::backupByWholeNotesStepLengthInVoice (
  int             inputLineNumber,
  const Rational& backupTargetMeasureElementMeasurePosition)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    gLogStream <<
      "Backup by a '" <<
      backupTargetMeasureElementMeasurePosition <<
      "' whole notes step length in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");
#endif

  // pad up the voice's last segment
  fVoiceLastSegment->
    backupByWholeNotesStepLengthInSegment (
      inputLineNumber,
      backupTargetMeasureElementMeasurePosition);

  --gIndenter;
}

void msrVoice::appendPaddingNoteToVoice (
  int             inputLineNumber,
  const Rational& forwardStepLength)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to voice \"" <<
      getVoiceName () <<
      "\",line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // pad up the voice's last segment
  fVoiceLastSegment->
    appendPaddingNoteToSegment (
      inputLineNumber,
      forwardStepLength);

  // account for padding note's duration in staff
  fVoiceUpLinkToStaff->
    getStaffUpLinkToPart ()->
      incrementPartMeasurePosition (
        inputLineNumber,
        forwardStepLength);

  // pad up the voice's stanzas
  if (fVoiceStanzasMap.size ()) {
    for (std::pair<std::string, S_msrStanza> thePair : fVoiceStanzasMap) {
      const S_msrStanza& stanza = thePair.second;

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Appending transposition '" <<
      transposition->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendTranspositionToSegment (transposition);
}

void msrVoice::appendStaffDetailsToVoice (
  const S_msrStaffDetails& staffDetails)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  // staff details are attributes in MusicXML,
  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Appending tempo " <<
      tempo->asShortString () <<
      " to voice \"" <<
      getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendTempoToSegment (tempo);
}

void msrVoice::appendOctaveShiftToVoice (
  const S_msrOctaveShift& octaveShift)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceOctaveShifts ()) {
    gLogStream <<
      "Appending octave shift '" <<
      octaveShift->getOctaveShiftKind () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendOctaveShiftToSegment (octaveShift);
}

void msrVoice::appendScordaturaToVoice (
  const S_msrScordatura& scordatura)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceScordaturas ()) {
    gLogStream <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendScordaturaToSegment (scordatura);
}

void msrVoice::appendAccordionRegistrationToVoice (
  const S_msrAccordionRegistration&
    accordionRegistration)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceAccordionRegistrations ()) {
    gLogStream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendAccordionRegistrationToSegment (
      accordionRegistration);
}

void msrVoice::appendHarpPedalsTuningToVoice (
  const S_msrHarpPedalsTuning&
    harpPedalsTuning)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarpPedals ()) {
    gLogStream <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendHarpPedalsTuningToSegment (
      harpPedalsTuning);
}

void msrVoice::appendRehearsalMarkToVoice (
  const S_msrRehearsalMark& rehearsalMark)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRehearsalMarks ()) {
    gLogStream <<
      "Appending rehearsalMark '" << rehearsalMark->getRehearsalMarkText () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendRehearsalMarkToSegment (rehearsalMark);
}

void msrVoice::appendVoiceStaffChangeToVoice (
  const S_msrVoiceStaffChange& voiceStaffChange)
{
#ifdef MF_TRACING_IS_ENABLED
  int inputLineNumber =
    voiceStaffChange->getInputLineNumber ();
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "Appending voice staff change '" <<
      voiceStaffChange->asString () <<
      "' to voice " <<
      " \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append voice staff change to voice's last segment
  fVoiceLastSegment->
    appendVoiceStaffChangeToSegment (
      voiceStaffChange);

  --gIndenter;
}

void msrVoice::appendNoteToVoice (const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note " <<
      note->asShortString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // fetch the part
  S_msrPart
    part =
      fetchVoiceUpLinkToPart ();

  // fetch the part current measure position
  Rational
    partMeasurePosition =
      part->
        getPartMeasurePosition ();

  if (! fVoiceLastSegment) {
    std::stringstream s;

    s <<
      "fVoiceLastSegment is null in appendNoteToVoice()";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
      this->displayVoice (
        inputLineNumber,
        s.str ());
    }
#endif
  }

  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegment (
      note,
      partMeasurePosition);

  // is this note the shortest one in this voice?
  this->
    registerShortestNoteInVoiceIfRelevant (
      note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;

  // account for note's duration in staff
  part->
    incrementPartMeasurePosition (
      inputLineNumber,
      note->getMeasureElementSoundingWholeNotes ());

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNoteKind::kNote_UNKNOWN:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      // register rest
      ++fVoiceRestsCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      // don't account skips as music
      ++fVoiceSkipsCounter;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch

  // are there harmonies attached to this note? // BLARK
  const std::list<S_msrHarmony>&
    noteHarmoniesList =
      note->
        getNoteHarmoniesList ();

  if (noteHarmoniesList.size ()) {
    // get the current part's harmonies voice
    S_msrVoice
      partHarmoniesVoice =
        part->
          getPartHarmoniesVoice ();

    for (S_msrHarmony harmony : noteHarmoniesList) {
      // append the harmony to the part harmonies voice,
      // only now that the note has been handled
      partHarmoniesVoice->
        appendHarmonyToVoice (
          harmony);
    } // for
  }

  // are there figured bass elements attached to this note?
  const std::list<S_msrFiguredBass>&
    noteFiguredBassesList =
      note->
        getNoteFiguredBassesList ();

  if (noteFiguredBassesList.size ()) {
    // get the current part's figured bass voice
    S_msrVoice
      partFiguredBassVoice =
        part->
          getPartFiguredBassVoice ();

    std::list<S_msrFiguredBass>::const_iterator i;
    for (S_msrFiguredBass figuredBass : noteFiguredBassesList) {
      // append the figured bass element to the part figured bass voice
      partFiguredBassVoice->
        appendFiguredBassToVoice (
          figuredBass);
    } // for
  }

  --gIndenter;
}

void msrVoice::appendNoteToVoiceClone (const S_msrNote& note) {
#ifdef MF_TRACING_IS_ENABLED
  int inputLineNumber =
    note->getInputLineNumber ();
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note '" <<
      note->asShortString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
    case msrNoteKind::kNote_UNKNOWN:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      // register rest
      ++fVoiceRestsCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      // don't account skips as music
      ++fVoiceSkipsCounter;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      // register actual note
      ++fVoiceActualNotesCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;
  } // switch
}

void msrVoice::appendDoubleTremoloToVoice (
  const S_msrDoubleTremolo& doubleTremolo)

{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendDoubleTremoloToSegment (doubleTremolo);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendChordToVoice (const S_msrChord& chord)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending chord '" << chord->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  // append chord to voice last segment
  fVoiceLastSegment->
    appendChordToSegment (chord);

  // account for chord duration in the part current measure position
  fVoiceUpLinkToStaff->
    getStaffUpLinkToPart ()->
      incrementPartMeasurePosition (
        chord->getInputLineNumber (),
        chord->getMeasureElementSoundingWholeNotes ());

  // get the chord's notes vector
  const std::vector<S_msrNote>&
    chordNotesVector =
      chord->
        getChordNotesVector ();

  size_t chordNotesVectorSize =
    chordNotesVector.size ();

  if (chordNotesVectorSize) {
    {
      // get the chord's first note
      S_msrNote
        chordFirstNote = chordNotesVector [0];

      // is chordFirstNote the shortest one in this voice?
      this->
        registerShortestNoteInVoiceIfRelevant (
          chordFirstNote);
    }

    {
      // get the chord's last note
      S_msrNote
        chordLastNote =
          chordNotesVector [chordNotesVectorSize - 1];

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

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendTupletToVoice (const S_msrTuplet& tuplet)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending tuplet " << tuplet->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append tuplet to voice last segment
  fVoiceLastSegment->
    appendTupletToSegment (tuplet);

  // account for tuplet duration in the part's current measure position
  fVoiceUpLinkToStaff->
    getStaffUpLinkToPart ()->
      incrementPartMeasurePosition (
        tuplet->getInputLineNumber (),
        tuplet->getMeasureElementSoundingWholeNotes ());

  --gIndenter;

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
  const S_msrGraceNotesGroup& graceNotesGroup)
{
  int inputLineNumber =
    graceNotesGroup->getInputLineNumber ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Adding grace notes '" <<
      graceNotesGroup->asString () <<
      "' ahead of voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

/* JMI
  gLogStream <<
    std::endl <<
    "======================= prependGraceNotesToVoice" <<
    std::endl;
  this->print (gLogStream);
  gLogStream <<
    "=======================" <<
    std::endl << std::endl;
  */

  if (! fVoiceFirstSegment) {
    // these graceNotesGroup appears at the beginning of the voice:
    // create a first segment
    createNewLastSegmentForVoice (
      inputLineNumber,
      "addGraceNotesGroupBeforeAheadOfVoiceIfNeeded()");

    fVoiceFirstSegment = fVoiceLastSegment;

    // then create the first measure
    createAMeasureAndAppendItToVoice (
      inputLineNumber,
      333, //         previousMeasureEndInputLineNumber, v0.9.62
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
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first chord of voice \"" << getVoiceName () <<
        "\", i.e. '" <<
        firstNoteShortcutUpLinkToChord->asShortString () <<
        "'" <<
        std::endl;
    }
#endif

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
        inputLineNumber,
        chordChordGraceNotesGroupLink);

  }

  else {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first note of voice \"" << getVoiceName () <<
        "\", i.e. '" <<
        voiceFirstNote->asShortString () <<
        "'" <<
        std::endl;
    }
#endif

    voiceFirstNote->
      setNoteGraceNotesGroupBefore (
        graceNotesGroup);
  }

  fMusicHasBeenInsertedInVoice = true;
}

/* JMI
void msrVoice::appendAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Appending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::prependAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Prepending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceFirstSegment->
    prependAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}
*/

void msrVoice::appendSyllableToVoice (
  int           inputLineNumber,
  const std::string& stanzaNumber,
  const std::string& stanzaName,
  const S_msrSyllable& syllable)
{
  // append syllable to this voice
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending syllable '" <<
      syllable->asString () <<
      "' to voice \"" << fVoiceName << "\"" <<
      std::endl;
  }
#endif

  // fetch stanzaNumber in this voice
  S_msrStanza
    stanza =
      createStanzaInVoiceIfNotYetDone (
        inputLineNumber,
        stanzaNumber,
        stanzaName);

  // add the syllable to the stanza
  stanza->
    appendSyllableToStanza (
      syllable);
}

void msrVoice::appendBarCheckToVoice (
  const S_msrBarCheck& barCheck)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending bar check " <<
      barCheck->asString () <<
      " to voice \"" <<
      getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendBarCheckToSegment (barCheck);
}

void msrVoice::appendBarNumberCheckToVoice (
  const S_msrBarNumberCheck& barNumberCheck)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending barnumber check " <<
      barNumberCheck->asString () <<
      " to voice \"" << getVoiceName () <<  "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendBarNumberCheckToSegment (barNumberCheck);
}

void msrVoice::appendLineBreakToVoice  (
  const S_msrLineBreak& lineBreak)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending line break '" << lineBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendLineBreakToSegment (lineBreak);

  // cascade this lineBreak to the voice stanzas if any
  if (fVoiceStanzasMap.size ()) {
    for (std::pair<std::string, S_msrStanza> thePair : fVoiceStanzasMap) {
      const S_msrStanza& stanza = thePair.second;

      stanza->appendLineBreakSyllableToStanza (
        lineBreak->getInputLineNumber (),
        fVoiceCurrentMeasurePuristNumber);
    } // for
  }
}

void msrVoice::appendPageBreakToVoice (
  const S_msrPageBreak& pageBreak)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending page break '" << pageBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendPageBreakToSegment (pageBreak);
}

void msrVoice::prependOtherElementToVoice (const S_msrMeasureElement& elem) {
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Prepending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    prependOtherElementToSegment (elem);
}

void msrVoice::appendOtherElementToVoice (const S_msrMeasureElement& elem) {
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendOtherElementToSegment (elem);
}

S_msrMeasure msrVoice::fetchVoiceLastMeasure (
  int inputLineNumber) const
{
  S_msrMeasure result;

  mfAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");

  result =
    fVoiceLastSegment->
      getSegmentLastMeasure (); // JMI v0.9.63

//   else { JMI v0.9.63
//     std::stringstream s;
//
//     s <<
//       "attempting to fetch voice last measure in an empty measure elements list";
//
//     msrInternalError (
//       gGlobalServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, __LINE__,
//       s.str ());
//   }

  return result;
}

S_msrMeasureElement msrVoice::fetchVoiceLastMeasureElement (
  int inputLineNumber) const
{
  S_msrMeasureElement result;

  S_msrMeasure
    lastMeasure =
      fetchVoiceLastMeasure (inputLineNumber);

  const std::list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->getMeasureElementsList ();

  if (lastMeasureElementsList.size ()) {
    result = lastMeasureElementsList.back ();
  }

  else {
    std::stringstream s;

    s <<
      "attempting to fetch voice last element in an empty elements list";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void msrVoice::pushRepeatOntoRepeatDescrsStack (
  int           inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Pushing repeat '" <<
      repeat->asShortString () <<
      "' onto the repeats stack in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fVoicePendingRepeatDescrsStack.push_front (
    msrRepeatDescr::create (
      inputLineNumber,
      repeat));

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    std::string
      combinedContext =
        "pushRepeatOntoRepeatDescrsStack() called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif
}

void msrVoice::popRepeatFromRepeatDescrsStack (
  int           inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    std::string
      combinedContext =
        "popRepeatFromRepeatDescrsStack() 1 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif

  if (
    repeat
      !=
    fVoicePendingRepeatDescrsStack.front ()->getRepeatDescrRepeat ()
  ) {
    std::stringstream s;

    s <<
      "cannot pop repeat '" <<
      repeat->asShortString () <<
      "' from the stack since it is not at the top" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Popping repeat '" <<
      repeat->asString () <<
      "' from the repeat stack in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // pop it from repeats stack
  fVoicePendingRepeatDescrsStack.pop_front ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    std::string
      combinedContext =
        "popRepeatFromRepeatDescrsStack() 2 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif
}

void msrVoice::displayVoiceRepeatsStack (
  int           inputLineNumber,
  const std::string& context)
{
  size_t repeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  gLogStream <<
    std::endl <<
    ">>++++++++++++++++ Displaying voice repeats stack " << context <<
    std::endl <<
    "The voice repeats stack contains " <<
    mfSingularOrPlural (repeatDescrsStackSize, "element", "elements") <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  if (repeatDescrsStackSize) {
    std::list<S_msrRepeatDescr>::const_iterator
      iBegin = fVoicePendingRepeatDescrsStack.begin (),
      iEnd   = fVoicePendingRepeatDescrsStack.end (),
      i      = iBegin;

    ++gIndenter;

    int n = repeatDescrsStackSize;
    for ( ; ; ) {
      gLogStream <<
        "v (" << n << ")" <<
        ", fRepeatDescrStartInputLineNumber: " <<
        (*i)->getRepeatDescrStartInputLineNumber () <<
        std::endl;

      ++gIndenter;
      gLogStream <<
        (*i)->
          getRepeatDescrRepeat ();
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLogStream << std::endl;
    } // for

    --gIndenter;
  }

  gLogStream <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

void msrVoice::displayVoiceRepeatsStackSummary (
  int           inputLineNumber,
  const std::string& context)
{
  size_t repeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  gLogStream <<
    std::endl <<
    "The voice repeats stack contains " <<
    mfSingularOrPlural (repeatDescrsStackSize, "element", "elements") <<
    " - " << context <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  if (repeatDescrsStackSize) {
    std::list<S_msrRepeatDescr>::const_iterator
      iBegin = fVoicePendingRepeatDescrsStack.begin (),
      iEnd   = fVoicePendingRepeatDescrsStack.end (),
      i      = iBegin;

    ++gIndenter;

    int n = repeatDescrsStackSize;
    for ( ; ; ) {
      gLogStream <<
        "v (" << n << ")" <<
        ", fRepeatDescrStartInputLineNumber: " <<
        (*i)->getRepeatDescrStartInputLineNumber () <<
        std::endl;

      ++gIndenter;
      gLogStream <<
        (*i)->
          getRepeatDescrRepeat ();
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLogStream << std::endl;
    } // for

    gLogStream <<
      "===============" <<
      std::endl << std::endl;

    --gIndenter;
  }
}

void msrVoice::displayVoiceRepeatsStackAndVoice ( // JMI never used ???
  int           inputLineNumber,
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
  int           inputLineNumber,
  const std::string& context)
{
  gLogStream <<
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
    gLogStream <<
      "[NONE]" <<
      std::endl;
  }

  --gIndenter;

  gLogStream <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

void msrVoice::displayVoiceMeasureRepeatAndVoice (
  int           inputLineNumber,
  const std::string& context)
{
  displayVoiceMeasureRepeat (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

// v0.9.62
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
//     std::list<S_msrMeasure>::const_iterator
//       iBegin = fVoiceMeasuresFlatList.begin (),
//       iEnd   = fVoiceMeasuresFlatList.end (),
//       i      = iBegin;
//
//     for ( ; ; ) {
//       S_msrMeasure measure = (*i);
//
//       // print the measure
//       if (gGlobalTracingOahGroup->getTraceMeasures ()) {
//         os <<
//           measure->asShortStringForMeasuresSlices ();
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

  gLogStream <<
    std::setw (fieldWidth) <<
    "fVoiceMeasuresFlatList" << ": ";
  if (voiceMeasuresFlatListSize) {
    gLogStream <<
      mfSingularOrPlural (
        voiceMeasuresFlatListSize, "element", "elements");

    ++gIndenter;

    std::list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    int counter = 0;
    for (S_msrMeasure measure :fVoiceMeasuresFlatList) {
      ++counter;

      // print the measure
#ifdef MF_TRACING_IS_ENABLED
//      if (gGlobalTracingOahGroup->getTraceMeasures ()) { // CAFE JMI v0.9.66
        gLogStream <<
          measure->asShortStringForMeasuresSlices ();
//      }
//      else {
//        gLogStream << measure->getMeasureNumber ();
 //     }
#else
      gLogStream << measure->getMeasureNumber ();
#endif

      if (++i == iEnd) break;

//       if (counter % 8 == 1) {
//         gLogStream << ' ';
//       }
//       else {
        gLogStream << std::endl;
//       }
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
  else {
    gLogStream << "[EMPTY]";
  }
  gLogStream << std::endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    std::list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    int counter = 0;
    for (S_msrMeasure measure :fVoiceMeasuresFlatList) {
      ++counter;

      // print the measure
#ifdef MF_TRACING_IS_ENABLED
//      if (gGlobalTracingOahGroup->getTraceMeasures ()) { // CAFE JMI v0.9.66
        gLogStream <<
          measure->asShortStringForMeasuresSlices ();
//      }
//      else {
//        gLogStream << measure->getMeasureNumber ();
 //     }
#else
      gLogStream << measure->getMeasureNumber ();
#endif

      if (++i == iEnd) break;

//       if (counter % 8 == 1) {
//         gLogStream << ' ';
//       }
//       else {
        gLogStream << std::endl;
//       }
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
}

void msrVoice::displayVoiceMultipleFullBarRests (
  int           inputLineNumber,
  const std::string& context)
{
  gLogStream <<
    std::endl <<
    ">>++++++++++++++++ voice multiple full-bar rests " << context <<
    std::endl <<
    "The current voice multiple full-bar rests contains " <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  ++gIndenter;

  if (fVoiceCurrentMultipleFullBarRests) {
    fVoiceCurrentMultipleFullBarRests->
      displayMultipleFullBarRests (
        inputLineNumber,
        context);
  }
  else {
    gLogStream <<
      "[NONE]" <<
      std::endl;
  }

  --gIndenter;

  gLogStream <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

void msrVoice::displayVoiceMultipleFullBarRestsAndVoice (
  int           inputLineNumber,
  const std::string& context)
{
  displayVoiceMultipleFullBarRests (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

S_msrRepeat msrVoice::createARepeatAndStackIt (
  int           inputLineNumber,
  const std::string& context)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat and stacking it in voice \"" <<
      getVoiceName () <<
      "\"" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrRepeat
    result =
      msrRepeat::create (
        inputLineNumber,
        2, // repeatTimes, default value JMI
        this);

  // push it onto the voice's repeat descrs stack
  pushRepeatOntoRepeatDescrsStack (
    inputLineNumber,
    result,
    "createARepeatAndStackIt() 1");

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "createARepeatAndStackIt() 2");
  }
#endif

  return result;
}

S_msrRepeat msrVoice::createARepeatCloneAndStackIt (
  int           inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat clone and stacking it in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrRepeat
    result =
      repeat->
        createRepeatNewbornClone (this);

  // push it onto the voice's repeat descrs stack
  pushRepeatOntoRepeatDescrsStack (
    inputLineNumber,
    result,
    "createARepeatCloneAndStackIt()");

  return result;
}

void msrVoice::moveVoiceInitialElementsToRepeatCommonPart (
  int                   inputLineNumber,
  const S_msrRepeatCommonPart& repeatCommonPart,
  const std::string&    context)
{
  // move the voice initial elements to the new repeat common part
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Moving the " <<
      mfSingularOrPlural (
        fVoiceInitialElementsList.size (),
        "initial element",
        "initial elements") <<
      " in voice \"" <<
      getVoiceName () <<
      "\" to repeat common part '" <<
      repeatCommonPart->asShortString () <<
      "' (" << context << ")" <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  for (
    std::list<S_msrVoiceElement>::iterator i = fVoiceInitialElementsList.begin ();
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
  int                   inputLineNumber,
  const S_msrRepeatCommonPart& repeatCommonPart,
  const std::string&    context)
{
  // move the voice last segment to repeatCommonPart
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Moving the voice last segment ";

    if (fVoiceLastSegment) { // JMI should not be necessary?
      gLogStream <<
        fVoiceLastSegment->asShortString ();
    }
    else {
      gLogStream <<
        "[NONE]";
    }

    gLogStream <<
      " to repeat common part '" <<
      repeatCommonPart->asShortString () <<
      "' (" << context << ")" <<
      "' in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    std::string
      combinedContext =
        "moveVoiceLastSegmentToRepeatCommonPart() 1 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    std::string
      combinedContext =
        "moveVoiceLastSegmentToRepeatCommonPart() 3 called from " + context;

    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      combinedContext);
  }
#endif
}

void msrVoice::moveAllVoiceContentsToRepeatCommonPart (
  int                   inputLineNumber,
  const S_msrRepeatCommonPart& repeatCommonPart,
  const std::string&    context)
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
  int               inputLineNumber,
  const S_msrRepeatEnding& repeatEnding,
  const std::string&     context)
{
  // move the voice last segment to repeatEnding
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Moving the voice last segment to repeat ending '" <<
      repeatEnding->asShortString () <<
      "' in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
  int           inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
  // append repeat to the list of initial elements
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat '" <<
      repeat->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // is the previous element in the voice elements list a repeat?
  S_msrVoiceElement previousElement;

  if (fVoiceInitialElementsList.size ()) {
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

// void msrVoice::appendMultipleFullBarRestsToInitialVoiceElements (
//   int                       inputLineNumber,
//   const S_msrMultipleFullBarRests& multipleFullBarRests,
//   const std::string&        context)
// {
//   // append multipleFullBarRests to the list of initial elements
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
//     gLogStream <<
//       "Appending multiple full-bar rests " <<
//       multipleFullBarRests->asString () <<
//       " to the initial elements in voice \"" <<
//       getVoiceName () <<
//       "\" (" << context << ")" <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif
//
// //   fVoiceInitialElementsList.push_back ( KAKA
// //     multipleFullBarRests);
// }

void msrVoice::appendMeasureRepeatToInitialVoiceElements (
  int                inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat,
  const std::string&      context)
{
  // append measureRepeat to the list of initial elements
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

//   fVoiceInitialElementsList.push_back ( KAKA
//     measureRepeat);
}

void msrVoice::appendVoiceLastSegmentToInitialVoiceElements (
  int           inputLineNumber,
  const std::string& context)
{
  // append segment to the list of initial elements
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending voice last segment '" <<
      fVoiceLastSegment->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fVoiceInitialElementsList.push_back (
    fVoiceLastSegment);
}

void msrVoice::moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
  int           inputLineNumber,
  const std::string& context)
{
  if (fVoiceLastSegment) {
    const std::list<S_msrSegmentElement>&
      segmentElementsList =
        fVoiceLastSegment->
          getSegmentElementsList ();

    if (segmentElementsList.size ()) {
#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceSegments ()) {
        gLogStream <<
          "Moving voice last segment '" <<
          fVoiceLastSegment->asString () <<
          "' to the initial elements in voice \"" <<
          getVoiceName () <<
          "\" (" << context << ")" <<
          ", line " << inputLineNumber <<
          std::endl;
      }

      if (gGlobalTracingOahGroup->getTraceSegmentsDetails ()) {
        displayVoice (
          inputLineNumber,
          "moveVoiceLastSegmentToInitialVoiceElementsIfRelevant() 1");
      }
#endif

      // append segment to the list of initial elements
      fVoiceInitialElementsList.push_back (
        fVoiceLastSegment);

      // forget about this last segment
      fVoiceLastSegment = nullptr;

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceSegmentsDetails ()) {
        displayVoice (
          inputLineNumber,
          "moveVoiceLastSegmentToInitialVoiceElementsIfRelevant() 2");
      }
#endif
    }

    else {
#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceSegments ()) {
        gLogStream <<
          "Voice last segment '" <<
          fVoiceLastSegment->asString () <<
          "' contains no measure, not moved to the initial elements in voice \"" <<
          getVoiceName () <<
          "\" (" << context << ")" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif
    }
  }

  else {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Voice last segment '" <<
        fVoiceLastSegment->asString () <<
        "' is null, not moved to the initial elements in voice \"" <<
        getVoiceName () <<
        "\" (" << context << ")" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif
  }
}

void msrVoice::appendRepeatCloneToInitialVoiceElements (
  int           inputLineNumber,
  const S_msrRepeat&   repeatCLone,
  const std::string& context)
{
  // append repeatCLone to the list of initial elements
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat cLone '" <<
      repeatCLone->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fVoiceInitialElementsList.push_back (
    repeatCLone);
}

void msrVoice::handleVoiceLevelRepeatStart (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat start in voice \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStart() 1");
  }
#endif

  ++gIndenter;

  // is there a voice last segment?
  if (fVoiceLastSegment) {

    // fetch last segment's measure elements list
    std::list<S_msrSegmentElement>
      voiceLastSegmentElementsList =
        fVoiceLastSegment->
          getSegmentElementsList ();

    // are there measures in the voice last segment?
    if (voiceLastSegmentElementsList.size ()) {
      // yes

      // fetch last measure in the last segment
      S_msrMeasure
        lastMeasureInLastSegment =
          fVoiceLastSegment->
            getSegmentLastMeasure (); // JMI v0.9.63

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
        lastMeasureInLastSegment->
          displayMeasure (
            inputLineNumber,
            "lastMeasureInLastSegment - handleVoiceLevelRepeatStart() 2");
      }
#endif

      // let's look at the last measure in detail

      // fetch its elements list
      const std::list<S_msrMeasureElement>&
        lastMeasureElementsList =
          lastMeasureInLastSegment->
            getMeasureElementsList ();

      if (lastMeasureElementsList.size ()) {
        // the last measure is not empty

        Rational
          measureWholeNotesDuration =
            lastMeasureInLastSegment->getMeasureWholeNotesDuration ();
        Rational
          fullMeasureWholeNotesDuration =
            lastMeasureInLastSegment->getFullMeasureWholeNotesDuration ();

        // is there a measure splitting?
        if ( // JMI better criterion???
          measureWholeNotesDuration.getNumerator () > 0
            &&
          measureWholeNotesDuration
            <
          fullMeasureWholeNotesDuration
        ) {
          // yes this measure is not yet complete and should be split
#ifdef MF_TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceRepeats ()) {
            gLogStream <<
              "Splitting measure '" <<
              lastMeasureInLastSegment->asShortString () <<
              "' upon a repeat start in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", measureWholeNotesDuration: " <<
              measureWholeNotesDuration <<
              ", fullMeasureWholeNotesDuration: " <<
              fullMeasureWholeNotesDuration <<
              ", line " << inputLineNumber <<
              std::endl;
          }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceSegments ()) {
            gLogStream <<
              "Creating a new last segment for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              std::endl;
          }
#endif

          createNewLastSegmentForVoice (
            inputLineNumber,
            "handleVoiceLevelRepeatStart() 5");

          // create a new measure with the same number as the voice last measure
          // and append it to the voice,
          createAMeasureAndAppendItToVoice (
            inputLineNumber,
            333, //         previousMeasureEndInputLineNumber, v0.9.62
            lastMeasureInLastSegment->getMeasureNumber (),
            msrMeasureImplicitKind::kMeasureImplicitKindNo);

        /* JMI
          // set it as created before a repeat
          voiceLastMeasure->
            setMeasureCreatedForARepeatKind ( // JMI v0.9.66
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
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Removing the last measure in voice \"" <<
            getVoiceName () <<
            "\"" <<
            " (voice level start)" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        S_msrMeasure
          dummyMeasure =
            fVoiceLastSegment->
              removeLastMeasureFromSegment (
                inputLineNumber,
                "handleVoiceLevelRepeatStart() 55");

        // append the voice last segment to the initial voice elements list
        appendVoiceLastSegmentToInitialVoiceElements (
          inputLineNumber,
          "handleVoiceLevelRepeatStart() 555");

        // create a new last segment containing a new measure for the voice
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Creating a new last segment with the first common part measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        createNewLastSegmentFromItsFirstMeasureForVoice (
          inputLineNumber,
          dummyMeasure,
          "handleVoiceLevelRepeatStart() 5555");
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStart() 11");
  }
#endif

  --gIndenter;
}

void msrVoice::handleNestedRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a nested repeat start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatStartInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // analyze this repeat start's context
      switch (fVoicePendingRepeatDescrsStack.size ()) {
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatStartInVoice() 2");
  }
#endif
}

void msrVoice::handleVoiceLevelRepeatEndWithoutStart (
  int           inputLineNumber,
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat end without start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStart() 1");
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the repeat
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

  S_msrRepeat
    newRepeat =
      msrRepeat::create (
        repeatInputLineNumber,
        repeatTimes,
        this);

  // create the repeat common part
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending the voice last segment in voice \"" <<
      getVoiceName () <<
      "\" to the new voice-level repeat common part" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStart() 7");
  }
#endif

  --gIndenter;
}

void msrVoice::handleVoiceLevelContainingRepeatEndWithoutStart (
  int           inputLineNumber,
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level rcontaining epeat end without start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStart() 1");
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the repeat
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

  S_msrRepeat
    newRepeat =
      msrRepeat::create (
        repeatInputLineNumber,
        repeatTimes,
        this);

  // create the repeat common part
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // pop it from the repeats stack
  popRepeatFromRepeatDescrsStack (
    inputLineNumber,
    repeatsStackTopRepeat,
    "handleVoiceLevelContainingRepeatEndWithoutStart() 2");

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending the voice last segment in voice \"" <<
      getVoiceName () <<
      "\" to the new voice-level repeat common part" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStart() 7");
  }
#endif

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatEndWithStart (
  int           inputLineNumber,
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat end with start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStart() 1");
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat end with a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // pop it from the repeats stack
  popRepeatFromRepeatDescrsStack (
    inputLineNumber,
    currentRepeat,
    "handleVoiceLevelRepeatEndWithStart() 2");

  // set its repeat times field
  currentRepeat->
    setRepeatTimes (repeatTimes);

  // create the currentRepeat's common part
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Moving the voice last segment in voice \"" <<
      getVoiceName () <<
      "\" to the current voice-level repeat common part" <<
      " and forget about it" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStart() 5");
  }
#endif

  --gIndenter;
}

void msrVoice::handleNestedRepeatEndInVoice (
  int           inputLineNumber,
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a nested repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() 1");
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat is nested" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

// fetch the voice's last measure
  S_msrMeasure
    voiceLastMeasure =
      fetchVoiceLastMeasure (
        inputLineNumber);

  // is there a measure splitting?
  if (
    voiceLastMeasure->getMeasureWholeNotesDuration ()
      ==
    voiceLastMeasure->getFullMeasureWholeNotesDuration ()
  ) {
    // this measure is incomplete and should be split
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Splitting measure '" <<
        voiceLastMeasure->asShortString () <<
        "' upon a repeat end in voice \"" <<
        getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    // create a new measure with the same number as the voice last measure
    // and append it to the voice,
    createAMeasureAndAppendItToVoice (
      inputLineNumber,
      333, //         previousMeasureEndInputLineNumber, v0.9.62
      measureNumber,
      msrMeasureImplicitKind::kMeasureImplicitKindNo);

  /* JMI
    // set it as created after a repeat
    voiceLastMeasure->
      setMeasureCreatedForARepeatKind ( // JMI v0.9.66
        msrMeasure::kMeasureCreatedForARepeatAfter);
        */
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndInVoice (
  int           inputLineNumber,
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // analyze this repeat end's context
        switch (fVoicePendingRepeatDescrsStack.size ()) {
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
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

              // analyze it
              switch (repeatsStackTopRepeat->getCurrentRepeatBuildPhaseKind ()) {
                case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
                  {
                    std::stringstream s;

                    s <<
                      "attempting to end repeat '" <<
                      repeatsStackTopRepeat->asShortString () <<
                      "' right after it has been created" <<
                      " in voice '" <<
                      asShortString () <<
                      "' ";

                    msrInternalError (
                      gGlobalServiceRunData->getInputSourceName (),
                      fInputLineNumber,
                      __FILE__, __LINE__,
                      s.str ());
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndInVoice() 2");
  }
#endif
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
  int          inputLineNumber,
  S_msrRepeat& currentRepeat)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat ending start without explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 1");
  }
#endif

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
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber;

    int repeatInputLineNumber = 1; // could find first measure's input line number??? JMI

    currentRepeat =
      createARepeatAndStackIt (
        repeatInputLineNumber,
        s1.str ());

    // create the repeat common part
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Creating a repeat common part upon its end in voice \"" <<
        getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Fetching the last measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrMeasure
    lastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 1");

  // fetch lastMeasure's elements list
  const std::list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (true || ! lastMeasureElementsList.size ()) { // JMIJMIJMIJMIJMIJMI  // JMI v0.9.66
    // it is empty, keep it for a new voice last segment

    // remove last measure
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Removing the last measure in voice \"" <<
        getVoiceName () <<
        "\"" <<
        " (voice level ending without explicit start)" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    S_msrMeasure
      dummyMeasure =
        fVoiceLastSegment->
          removeLastMeasureFromSegment (
            inputLineNumber,
            "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 2");

    // move voice last segment into the repeat common part
    std::stringstream s2;

    s2 <<
      "Moving the voice last segment to the repeat common part in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber;

    moveVoiceLastSegmentToRepeatCommonPart (
      inputLineNumber,
      repeatCommonPart,
      s2.str ());

    // create a new last segment containing a new measure for the voice
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart( 5)");
  }

  // set currentRepeat's build phase
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithExplicitStart (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat ending start with explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStart() 1");
  }
#endif

  ++gIndenter;

  // fetch currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Fetching the last measure of the last segment in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", it is:" <<
      std::endl <<
      lastMeasure->asShortString () <<
      std::endl;
  }
#endif

  // fetch lastMeasure's elements list
  const std::list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (true || ! lastMeasureElementsList.size ()) { // JMIJMIJMIJMIJMIJMI  // JMI v0.9.66
    // it is empty, keep it for a new voice last segment

    // remove last measure
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Removing the last measure in voice \"" <<
        getVoiceName () <<
        "\"" <<
        " (voice level ending with explicit start)" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    S_msrMeasure
      dummyMeasure =
        fVoiceLastSegment->
          removeLastMeasureFromSegment (
            inputLineNumber,
            "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 3");

    // move voice last segment into the repeat common part
    std::stringstream s2;

    s2 <<
      "Moving the voice last segment to the repeat common part in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber;

    moveVoiceLastSegmentToRepeatCommonPart (
      inputLineNumber,
      repeatCommonPart,
      s2.str ());

    // create a new last segment containing a new measure for the voice
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 6");
  }

  // set currentRepeat's build phase
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithExplicitStart() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::nestContentsIntoNewRepeatInVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentElementsList ().size ()) {
          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

#ifdef MF_TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 1");
          }
#endif

          // move voice last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "nestContentsIntoNewRepeatInVoice() 1");

#ifdef MF_TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 2");
          }
#endif
        }
      }
      break;
  } // switch
}

void msrVoice::handleNestedRepeatEndingStartInVoice (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a nested repeat ending start voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
 }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndingStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndingStartInVoice (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        ++gIndenter;

        // analyze this repeat end's context
        switch (fVoicePendingRepeatDescrsStack.size ()) {
          case 0:
            // this the first ending of a voice-level repeat without start
            // -------------------------------------
            handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
              inputLineNumber,
              gGlobalNullRepeatSmartPointer); // set later in XXX() JMI v0.9.66
            break;

          case 1:
            {
              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

              switch (currentRepeat->getRepeatExplicitStartKind ()) {
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndingStartInVoiceClone (
  int           inputLineNumber,
  msrRepeatEndingKind
                repeatEndingKind,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // handle the repeat ending start
        ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Handling a repeat ending upon its start in voice clone \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        // is there a current repeat?
        switch (fVoicePendingRepeatDescrsStack.size ()) {
          case 0:
            {
              std::stringstream s;

              s <<
                "repeats stack is empty when attempting to handle a repeat ending start in voice clone '" <<
                asShortString () <<
                "' ";

              msrInternalError (
                gGlobalServiceRunData->getInputSourceName (),
                fInputLineNumber,
                __FILE__, __LINE__,
                s.str ());
            }
          break;

          case 1:
            {
              // this repeat ending is part of a voice-level repeat

              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

              // create a repeat ending
#ifdef MF_TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceRepeats ()) {
                gLogStream <<
                  "Creating a " <<
                  msrRepeatEndingKindAsString (
                    repeatEndingKind) <<
                  " repeat ending in current repeat in voice clone \"" <<
                  fVoiceName <<
                  "\"" <<
                  std::endl;
              }
#endif

              S_msrRepeatEnding
                repeatEnding =
                  msrRepeatEnding::create (
                    inputLineNumber,
                    repeatEndingNumber,
                    repeatEndingKind,
                    currentRepeat);

              // add the repeat ending to the voice current repeat
#ifdef MF_TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceRepeats ()) {
                gLogStream <<
                  "Appending a " <<
                  msrRepeatEndingKindAsString (
                    repeatEndingKind) <<
                  " repeat ending to current repeat in voice \"" <<
                  fVoiceName <<
                  "\"" <<
                  std::endl;
              }
#endif

#ifdef MF_TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
                displayVoiceRepeatsStackSummary (
                  inputLineNumber,
                  "before adding a hooked repeat ending to current repeat");
              }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceRepeats ()) {
                gLogStream <<
                  "Moving the voice last segment to the repeat common part in voice clone \"" <<
                  getVoiceName () <<
                  "\"" <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif
            }
        } // switch

        --gIndenter;
      }
      break;
  } // switch

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoiceClone() 2");
  }
#endif
}

void msrVoice::handleSegmentCloneEndInVoiceClone (
  int          inputLineNumber,
  const S_msrSegment& segmentClone)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Handling segment '" <<
      segmentClone->asShortString () <<
      "' in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegmentsDetails ()) {
    displayVoice (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone() 1");
  }
#endif

  if (fVoicePendingMeasureRepeat) {
    // segmentClone belongs to a measures repeat

    switch (
      fVoicePendingMeasureRepeat->getCurrentMeasureRepeatBuildPhaseKind ()
    ) {
      case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseJustCreated:
        {
          std::stringstream s;

          s <<
            "segment '" <<
            segmentClone->asShortString () <<
            "'cannot be added to a just created measures repeat";

          msrError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
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
          std::stringstream s;

          s <<
            "segment '" <<
            segmentClone->asShortString () <<
            "'cannot be added to a completed measures repeat";

          msrError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
        break;
    } // switch
  }

  else if (fVoicePendingRepeatDescrsStack.size ()) {
    // segmentClone belongs to a repeat

    // append segment to whichever part of the repeat is adequate
    S_msrRepeat
      currentRepeat =
        fVoicePendingRepeatDescrsStack.front ()->
          getRepeatDescrRepeat ();

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegmentsDetails ()) {
    displayVoice (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

/* JMI
void msrVoice::finalizeRepeatEndInVoice (
  int    inputLineNumber,
  const std::string& measureNumber,
  int    repeatTimes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "finalizeRepeatEndInVoice() 1");
  }
#endif

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
        if (! fVoicePendingRepeatDescrsStack.size ()) {
          std::stringstream s;

          s <<
            "repeats stack is empty when attempting to finalize a repeat in voice '" <<
            asShortString () <<
            "' ";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        S_msrRepeat
          repeat =
            fVoicePendingRepeatDescrsStack.front ()->
              getRepeatDescrRepeat ();

        // get repeat's common part
        S_msrRepeatCommonPart
          repeatCommonPart =
            repeat->
              getRepeatCommonPart ();

        // is there another repeat to nest into this repeat?
        if (! fVoicePendingRepeatDescrsStack.size ()) {
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "finalizeRepeatEndInVoice() 2");
  }
#endif
}
*/

void msrVoice::createMeasureRepeatFromItsFirstMeasures (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashesNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLogStream <<
      "Creating a measures repeat from its first measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "createMeasureRepeatFromItsFirstMeasures() 1");
  }
#endif

  // this occurs after an empty measure has just been created,
  // hence the repeated measure/measures is/are the
  // measureRepeatMeasuresNumber measures preceding the last one

  int
    lastSegmentMeasuresNumber =
      fVoiceLastSegment->
        getSegmentElementsList ().size (),
    availableMeasuresNumber =
      lastSegmentMeasuresNumber - 1;

  if (
    availableMeasuresNumber < measureRepeatMeasuresNumber) {
    std::stringstream s;

    s <<
      "attempting to create a measures repeat with " <<
      measureRepeatMeasuresNumber <<
      " measures while current last segment only has " <<
      availableMeasuresNumber <<
      " available";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab the just created last measure from the voice,
  // (i.e. the one containing:
  //   <measure-repeat ... type="start">2</measure-repeat>)
  // which is the first replica measure
  S_msrMeasure
    firstReplicaMeasure =
      removeLastMeasureFromVoice (
        inputLineNumber);

  // create the measures repeat repeated segment
  S_msrSegment
    repeatedSegment =
      msrSegment::create (
        inputLineNumber,
        this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

  // remove the repeated measure(s) for the last segment
  // and prepend them to the repeated segment
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Removing the last " <<
      mfSingularOrPlural (
        measureRepeatMeasuresNumber, "measure", "measures") <<
      " (to be repeated) from voice \"" <<
      fVoiceName <<
      std::endl;
  }
#endif

  for (int i = 0; i < measureRepeatMeasuresNumber; ++i) {
    S_msrMeasure
      lastMeasure =
        removeLastMeasureFromVoice (
          inputLineNumber);

    repeatedSegment->
      prependMeasureToSegment (
        lastMeasure);
  } // for

  // create the measures repeat
  if (fVoicePendingMeasureRepeat) {
    std::stringstream s;

    s <<
      "attempting to create a measures repeat while another one is pending";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fVoicePendingMeasureRepeat =
    msrMeasureRepeat::create (
      inputLineNumber,
      measureRepeatMeasuresNumber,
      measureRepeatSlashesNumber,
      this);

  // create the measures repeat pattern
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a measures repeat pattern in voice \"" <<
      fVoiceName <<
      "\"" <<
      std::endl;
  }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a new last segment with the first replica measure for voice \"" <<
      fVoiceName << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  createNewLastSegmentFromItsFirstMeasureForVoice (
    inputLineNumber,
    firstReplicaMeasure,
    "createMeasureRepeatFromItsFirstMeasures() 2");

  // keep the measures repeat pending

  // print resulting voice contents
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "createMeasureRepeatFromItsFirstMeasures() 3");
  }
#endif
}

void msrVoice::appendMultipleFullBarRestsToVoice (
  int                       inputLineNumber,
  const S_msrMultipleFullBarRests& multipleFullBarRests)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending multiple full-bar rests " <<
      multipleFullBarRests->asShortString () <<
      " to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "appendMultipleFullBarRestsToVoice() 1");
  }
#endif

  // analyze this multiple full-bar rests's context
  switch (fVoicePendingRepeatDescrsStack.size ()) {
    case 0:
      // this multiple full-bar rests is at the voice-level
      // -------------------------------------

      // create the voice last segment if needed
      if (! fVoiceLastSegment) {
        createNewLastSegmentForVoice (
          inputLineNumber,
          "appendMultipleFullBarRestsToVoice()");
      }

      // append multipleFullBarRests to it
      fVoiceLastSegment->
        appendMultipleFullBarRestsToSegment (
          multipleFullBarRests);
      break;

    default:
      // this multiple full-bar rests is inside a repeat
      // -------------------------------------
      S_msrRepeat
        currentRepeat =
          fVoicePendingRepeatDescrsStack.front ()->
            getRepeatDescrRepeat ();

      currentRepeat->
        appendMultipleFullBarRestsToRepeat (
          inputLineNumber,
          multipleFullBarRests,
          "appendMultipleFullBarRestsToVoice() 2");
  } // switch

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "appendMultipleFullBarRestsToVoice() 3");
  }
#endif
}

void msrVoice::appendMeasureRepeatToVoice (
  int                 inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measureRepeat->asShortString () <<
      "' to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendMeasureRepeatToVoice() 1");
  }
#endif

  // analyze this measure repeats's context
  switch (fVoicePendingRepeatDescrsStack.size ()) {
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
          fVoicePendingRepeatDescrsStack.front ()->
            getRepeatDescrRepeat ();

      currentRepeat->
        appendMeasureRepeatToRepeat (
          inputLineNumber,
          measureRepeat,
          "appendMeasureRepeatToVoice() 2");
  } // switch

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendMeasureRepeatToVoice() 3");
  }
#endif
}

void msrVoice::appendPendingMeasureRepeatToVoice (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLogStream <<
      "Appending pending measures repeat to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasureRepeatToVoice() 1");
  }
#endif

  // does the current measures repeat exist?
  if (! fVoicePendingMeasureRepeat) {
    std::stringstream s;

    s <<
      "attempting to append a pending measures repeat which doesn't exist";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // fetch the last segment's measure elements list
  std::list<S_msrSegmentElement>
    voiceLastSegmentMeasureList =
      fVoiceLastSegment->
        getSegmentElementsList ();

  // grab the just created last measure
  // in the last segment's measure elements list,
  // (i.e. the one containing:
  //   <measure-repeat type="stop"/>)
  // which is the next measure after the measures repeat
  if (! voiceLastSegmentMeasureList.size ()) {
    std::stringstream s;

    s <<
      "attempting to grab first measure of voice last segment, that contains none";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  S_msrMeasure
    nextMeasureAfterMeasureRepeat =
      fVoiceLastSegment->
        getSegmentLastMeasure (); // JMI v0.9.63

// BOFBOFBOF JMI
  // remove the next measure from the last segment's measure elements list
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Removing last measure in last segment" <<
      "in voice \"" <<
      fVoiceName <<
      "\":" <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      nextMeasureAfterMeasureRepeat;
    gLogStream << std::endl;
    --gIndenter;
}
#endif

  voiceLastSegmentMeasureList.pop_back ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasureRepeatToVoice() 2");
  }
#endif

  // create the measures repeat replicas contents
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a measures repeat replicas contents for voice \"" <<
      fVoiceName << "\" is:" <<
      std::endl;
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Setting pending measures repeat replicas segment in voice \"" <<
      getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fVoicePendingMeasureRepeat->
    setMeasureRepeatReplicas (
      measureRepeatReplicas);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Setting measures repeat segment to voice last segment for voice \"" <<
      fVoiceName << "\"" <<
      std::endl;
  }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a new last segment with the AAA measures repeat next measure for voice \"" <<
      fVoiceName << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  createNewLastSegmentFromItsFirstMeasureForVoice (
    inputLineNumber,
    nextMeasureAfterMeasureRepeat,
    "appendPendingMeasureRepeatToVoice() 3");

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLogStream <<
      "Appending pending measures repeat to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasureRepeatToVoice() 4");
  }
#endif
}

void msrVoice::createMeasureRepeatAndAppendItToVoiceClone (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashesNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLogStream <<
      "Creating measures repeat and appending it to voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "createMeasureRepeatAndAppendItToVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // does the pending measures repeat exist?
        if (fVoicePendingMeasureRepeat) {
          std::stringstream s;

          s <<
            "attempting to create a measures repeat while another one is pending";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // create the measures repeat
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Creating and appending a measures repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        fVoicePendingMeasureRepeat =
          msrMeasureRepeat::create (
            inputLineNumber,
            measureRepeatMeasuresNumber,
            measureRepeatSlashesNumber,
            this);

        // create a measures repeat pattern from current last segment
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Creating a measures repeat pattern from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            std::endl;
        }
#endif

        fVoicePendingMeasureRepeat->
          setMeasureRepeatPattern (
            measureRepeatPattern);

        // append the measures repeat to the list of initial elements
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Appending measures repeat to voice \"" <<
            fVoiceName <<
            "\"" <<
            std::endl;
        }
#endif

        // append pending measures repeat to the voice
        appendMeasureRepeatToVoice (
          inputLineNumber,
          fVoicePendingMeasureRepeat);
      }
      break;
  } // switch

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    gLogStream <<
      "Creating measures repeat and appending it to voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "createMeasureRepeatAndAppendItToVoiceClone() 2");
  }
#endif
}

void msrVoice::setVoiceContainsMultipleFullBarRests (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains multiple full-bar rests" <<
      std::endl;
  }
#endif

  fVoiceContainsMultipleFullBarRests = true;
}

void msrVoice::setVoiceContainsMeasureRepeats (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains measure repeats" <<
      std::endl;
  }
#endif

  fVoiceContainsMeasureRepeats = true;
}

void msrVoice::appendMultipleFullBarRestsToVoice (
  int inputLineNumber,
  int multipleFullBarRestsMeasuresNumber)
{
  // create a multiple full-bar rests
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending a multiple full-bar rest for " <<
      mfSingularOrPlural (
        multipleFullBarRestsMeasuresNumber, "measure", "measures") <<
      " to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "appendMultipleFullBarRestsToVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        ++gIndenter;

        // this method is called after the measure containing:
        //   <multiple-rest ... type="start">NNN</multiple-rest>)
        // which is the first full-bar rest measure,
        // has been appended to the voice

        // grab the just created last measure from the voice
        S_msrMeasure
          firstRestMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // create the multiple full-bar rests
        if (fVoiceCurrentMultipleFullBarRests) {
          std::stringstream s;

          s <<
            "attempting to create a multiple full-bar rests while another one is pending";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

//         if (! fVoiceLastSegment) { // JMI v0.9.63 useless???
//           // create a new last segment for the voice
//           createNewLastSegmentForVoice (
//             inputLineNumber,
//             "appendMultipleFullBarRestsToVoice()");
//         }

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
          gLogStream <<
            "Creating a multiple full-bar rest for " <<
            mfSingularOrPlural (
              multipleFullBarRestsMeasuresNumber, "measure", "measures") <<
            " in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif
        fVoiceCurrentMultipleFullBarRests =
          msrMultipleFullBarRests::create (
            inputLineNumber,
            multipleFullBarRestsMeasuresNumber,
            fVoiceLastSegment); // JMI ??? KAKA

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
          gLogStream <<
            "Appending multiple full-bar rest " <<
            fVoiceCurrentMultipleFullBarRests->asString () <<
            " to the last segment of voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif
        fVoiceLastSegment->
          appendMultipleFullBarRestsToSegment (
            fVoiceCurrentMultipleFullBarRests);

        // append firstRestMeasure to fVoiceCurrentMultipleFullBarRests
        fVoiceCurrentMultipleFullBarRests->
          appendMeasureToMultipleFullBarRests (
            firstRestMeasure);

         // remember fVoiceCurrentMultipleFullBarRests for later next measure number setting
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
          gLogStream <<
            "Registering multiple full-bar rests " <<
            fVoiceCurrentMultipleFullBarRests->asString () <<
            " as waiting for its next measure number" <<
            " in voice \"" <<
            fVoiceName << "\"" <<
            std::endl;
        }
#endif
        fVoiceMultipleFullBarRestsWaitingForItsNextMeasureNumber =
          fVoiceCurrentMultipleFullBarRests;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
          gLogStream <<
            "Setting fVoiceRemainingMultipleFullBarRests to '" <<
            multipleFullBarRestsMeasuresNumber <<
            "' in voice \"" <<
            fVoiceName << "\"" <<
            std::endl;
        }
#endif
        fVoiceRemainingMultipleFullBarRests =
          multipleFullBarRestsMeasuresNumber;

//         // create a new segment to collect the multiple full-bar rests,
//         // containing the first, rest measure
// #ifdef MF_TRACING_IS_ENABLED
//         if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
//           gLogStream <<
//             "Creating a new last segment to create the first, rest measure in voice \"" <<
//             fVoiceName << "\"" <<
//             ", line " << inputLineNumber <<
//             std::endl;
//         }
// #endif
//
//         createNewLastSegmentFromItsFirstMeasureForVoice (
//           inputLineNumber,
//           firstRestMeasure,
//           "appendMultipleFullBarRestsToVoice() 3");
//
//         // this voice contails multiple full-bar rests
//         this->setVoiceContainsMultipleFullBarRests (
//           inputLineNumber); // KAKA
//
//         // keep the multiple full-bar rests pending

        --gIndenter;
      }
      break;
  } // switch

  // print resulting voice contents
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "appendMultipleFullBarRestsToVoice() 4");
  }
#endif
}

void msrVoice::replicateLastAppendedMeasureInVoice (
  int inputLineNumber,
  int replicatasNumber)
{
  std::string
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
        voiceLastAppendedMeasureMeasureNumber +
          '.' +
          std::to_string (i) +
          " (replicated)");

  #ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
      gLogStream <<
        "Replicating last appended measure " <<
        fVoiceLastAppendedMeasure->getMeasureNumber () <<
        " as measure " <<
        lastAppendedMeasureClone->getMeasureNumber () <<
        " in voice \"" <<
        getVoiceName () <<
        "\"" <<
        std::endl;
    }
  #endif

    // register its whole notes duration
    fetchVoiceUpLinkToPart ()->
      registerOrdinalMeasureNumberWholeNotesDuration (
        inputLineNumber,
        lastAppendedMeasureClone->
          getMeasureOrdinalNumberInVoice (),
        lastAppendedMeasureClone->
          getFullMeasureWholeNotesDuration ()); // JMI

    // append it to the voice last segment
    fVoiceLastSegment->
      appendMeasureToSegment (
        lastAppendedMeasureClone);

    //   // update fVoiceLastAppendedMeasure // JMI
    //   fVoiceLastAppendedMeasure->
    //     setNextMeasureNumber (
    //       );
  } // for
}

void msrVoice::addEmptyMeasuresToVoice (
  int           inputLineNumber,
  const std::string& previousMeasureNumber, // JMI ???
  int           emptyMeasuresNumber)
{
  // create a multiple full-bar rests
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Adding " <<
      mfSingularOrPlural (
        emptyMeasuresNumber, "empty measure", "empty measures") <<
      " to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // get the empty measure whole notes duration
  // JMI maybe not OK if first measure such as after a repeat segment???
  Rational
    emptyMeasureWholeNotesDuration =
      fVoiceLastAppendedMeasure->
        getFullMeasureWholeNotesDuration ();

  for (int i = 1; i <= emptyMeasuresNumber; ++i) {
    // create a measure
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating an empty measure and appending it to segment " <<
      asString () <<
      ", in voice \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

  // set its whole notes duration
//   emptyMeasure->
//     setFullMeasureWholeNotesDuration (
//       emptyMeasureWholeNotesDuration);

  // create a rest the whole empty measure long
  S_msrNote
    wholeMeasureRestNote =
      msrNote::createRestNote (
        inputLineNumber,
        measureNumber,
        emptyMeasureWholeNotesDuration, // soundingWholeNotes
        emptyMeasureWholeNotesDuration, // displayWholeNotes
        0); // dotsNumber

   wholeMeasureRestNote->
    setNoteOccupiesAFullMeasure ();

  // append it to emptyMeasure
  Rational partMeasurePosition; // needs to be supplied

  emptyMeasure->
    appendNoteToMeasure (
      wholeMeasureRestNote,
      partMeasurePosition);

  // append emptyMeasure to the voice last segment
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating an empty measure '" << measureNumber <<
      "' and appending it to segment " << asString () <<
      ", in voice \"" <<
      fVoiceName  <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fVoiceLastSegment->
    appendMeasureToSegment (emptyMeasure);


//   // change the measure number
//   fVoiceLastAppendedMeasure->
//     setMeasureNumber (
//       fVoiceLastAppendedMeasure->
//         getMeasureNumber () + " added"); // JMI BLARK v0.9.61

  } //for

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "addEmptyMeasuresToVoice() 1");
  }
#endif

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
//           "addEmptyMeasuresToVoice() 2");
//
//         // create the multiple full-bar rests
//         if (fVoiceCurrentMultipleFullBarRests) {
//           std::stringstream s;
//
//           s <<
//             "attempting to create a multiple full-bar rests while another one is pending";
//
//           msrInternalError (
//             gGlobalServiceRunData->getInputSourceName (),
//             inputLineNumber,
//             __FILE__, __LINE__,
//             s.str ());
//         }
//
// /* JMI
//         fVoiceCurrentMultipleFullBarRests =
//           msrMultipleFullBarRests::create (
//             inputLineNumber,
//             firstRestMeasure->getFullMeasureWholeNotesDuration (),
//             multipleFullBarRestsMeasuresNumber,
//             this);
// */
//
//          // remember fVoiceCurrentMultipleFullBarRests for later next measure number setting JMI ???
// #ifdef MF_TRACING_IS_ENABLED
//         if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
//           gLogStream <<
//             "Registering multiple full-bar rests " <<
//             fVoiceCurrentMultipleFullBarRests->asString () <<
//             " as waiting for its next measure number" <<
//             ", multipleFullBarRestsMeasuresNumber: " <<
//             multipleFullBarRestsMeasuresNumber <<
//             " in voice \"" <<
//             fVoiceName << "\"" <<
//             std::endl;
//         }
// #endif
//
//         fVoiceMultipleFullBarRestsWaitingForItsNextMeasureNumber =
//           fVoiceCurrentMultipleFullBarRests;
//
//         fVoiceRemainingMultipleFullBarRests =
//           multipleFullBarRestsMeasuresNumber;
//
// #ifdef MF_TRACING_IS_ENABLED
//         if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
//           gLogStream <<
//             "Setting fVoiceRemainingMultipleFullBarRests to '" <<
//             fVoiceRemainingMultipleFullBarRests <<
//             "' in voice \"" <<
//             fVoiceName << "\"" <<
//             std::endl;
//         }
// #endif
//
//         // create a new segment to collect the multiple full-bar rests,
//         // containing the first, rest measure
// #ifdef MF_TRACING_IS_ENABLED
//         if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
//           gLogStream <<
//             "Creating a new last segment to add the first, rest measure in voice \"" <<
//             fVoiceName << "\"" <<
//             ", line " << inputLineNumber <<
//             std::endl;
//         }
// #endif
//
// /* JMI
//         createNewLastSegmentFromItsFirstMeasureForVoice (
//           inputLineNumber,
//           firstRestMeasure,
//           "addEmptyMeasuresToVoice() 3");
// */
//
//         // this voice contails multiple full-bar rests
//         this->setVoiceContainsMultipleFullBarRests (
//           inputLineNumber);
//
//         // keep the multiple full-bar rests pending
//
//         --gIndenter;
//       }
//       break;
//   } // switch

  // print resulting voice contents
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "addEmptyMeasuresToVoice() 4");
  }
#endif
}

void msrVoice::appendPendingMultipleFullBarRestsToVoice (
    int inputLineNumber)
{
  // a multiple full-bar rests is a voice element,
  // and can be voice-level as well as part of a repeat

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
      if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
        gLogStream <<
          "Appending current multiple full-bar rests " <<
          fVoiceCurrentMultipleFullBarRests->asShortString () <<
          " to voice \"" <<
          getVoiceName () <<
          "\"" <<
          ", line " << inputLineNumber <<
          std::endl;
      }

    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "appendPendingMultipleFullBarRestsToVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
//         // does the current multiple full-bar rests exist?
//         if (! fVoiceCurrentMultipleFullBarRests) {
//           std::stringstream s;
//
//           s <<
//             "attempting to append a current multiple full-bar rests which doesn't exist";
//
//           msrInternalError (
//             gGlobalServiceRunData->getInputSourceName (),
//             inputLineNumber,
//             __FILE__, __LINE__,
//             s.str ());
//         }

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
          gLogStream <<
            "Appending current multiple full-bar rests " <<
            fVoiceCurrentMultipleFullBarRests->asShortString () <<
            " to the last segment of voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif
//        fVoiceLastSegment->
          // KAKA

        // set current multiple full-bar rests last measure purist number
        fVoiceCurrentMultipleFullBarRests->
          setMultipleFullBarRestsLastMeasurePuristMeasureNumber (
            inputLineNumber);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr;

        // append current multiple full-bar rests to the voice
        appendMultipleFullBarRestsToVoice (
          inputLineNumber,
          fVoiceCurrentMultipleFullBarRests);

        // forget about this current multiple full-bar rests
        fVoiceCurrentMultipleFullBarRests = nullptr;
      }
      break;
  } // switch

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "appendPendingMultipleFullBarRestsToVoice() 2");
  }
#endif
}

void msrVoice::handleMultipleFullBarRestsStartInVoiceClone (
  int                       inputLineNumber,
  const S_msrMultipleFullBarRests& multipleFullBarRests)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Handling multiple full-bar rests start in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "handleMultipleFullBarRestsStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:

      // is there a voice last segment?
//       if (fVoiceLastSegment) {
//
//         // are there measures in the voice last segment?
//         if (fVoiceLastSegment->getSegmentElementsList ().size ()) {
//
//           // finalize current measure in voice
//           finalizeLastAppendedMeasureInVoice (
//             inputLineNumber);
//
//           // move voice last segment to the list of initial elements
//           moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
//             inputLineNumber,
//             "handleMultipleFullBarRestsStartInVoiceClone() 2");
//
//           // forget about fVoiceCurrentMultipleFullBarRests
//           fVoiceCurrentMultipleFullBarRests = nullptr;
//
//           // create a new last segment containing a new measure for the voice
//           createNewLastSegmentForVoice (
//             inputLineNumber,
//             "handleMultipleFullBarRestsStartInVoiceClone()");
//
// #ifdef MF_TRACING_IS_ENABLED
//           if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
//             displayVoiceMultipleFullBarRestsAndVoice (
//               inputLineNumber,
//               "handleMultipleFullBarRestsStartInVoiceClone() 3");
//           }
// #endif
//         }
//       }

      // is there already a current multiple full-bar rests in this voice?
      if (fVoiceCurrentMultipleFullBarRests) {
        std::stringstream s;

        s <<
          "current voice multiple full-bar rests is not null when attempting to handle multiple full-bar rests start '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // create the multiple full-bar rests clone and register it
      fVoiceCurrentMultipleFullBarRests =
        multipleFullBarRests->
          createMultipleFullBarRestsNewbornClone (
            fVoiceLastSegment); // JMI ??? KAKA

      // this voice contails multiple full-bar rests
      this->setVoiceContainsMultipleFullBarRests (
        inputLineNumber);

      break;
  } // switch

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "handleMultipleFullBarRestsStartInVoiceClone() 4");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMultipleFullBarRestsEndInVoiceClone (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Handling multiple full-bar rests end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
    displayVoiceMultipleFullBarRestsAndVoice (
      inputLineNumber,
      "handleMultipleFullBarRestsEndInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:

      // is there a current multiple full-bar rests in this voice?
      if (! fVoiceCurrentMultipleFullBarRests) {
        std::stringstream s;

        s <<
          "current voice multiple full-bar rests is null when attempting to handle multiple full-bar rests end '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // set current multiple full-bar rests last measure purist number
      fVoiceCurrentMultipleFullBarRests->
        setMultipleFullBarRestsLastMeasurePuristMeasureNumber (
          inputLineNumber);

      // forget about fVoiceCurrentMultipleFullBarRests
      fVoiceCurrentMultipleFullBarRests = nullptr;

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
        displayVoice (
          inputLineNumber,
          "handleMultipleFullBarRestsEndInVoiceClone() 3");
      }
#endif
    break;
  } // switch

  --gIndenter;
}

void msrVoice::appendMultipleFullBarRestsCloneToVoiceClone (
  int                       inputLineNumber,
  const S_msrMultipleFullBarRests& multipleFullBarRestsClone)
{
  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    multipleFullBarRestsClone != nullptr,
    "multipleFullBarRestsClone is null");
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
          gLogStream <<
            "Appending multiple full-bar rests clone " <<
            multipleFullBarRestsClone->asString () <<
            " to voice clone \"" <<
            getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }

        if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
          displayVoiceMultipleFullBarRestsAndVoice (
            inputLineNumber,
            "appendMultipleFullBarRestsCloneToVoiceClone() 1");
        }
#endif

        // is multiple full-bar rests nested in a repeat?
        if (fVoicePendingRepeatDescrsStack.size ()) {
          // yes

          S_msrRepeat
            currentRepeat =
              fVoicePendingRepeatDescrsStack.front ()->
                getRepeatDescrRepeat ();

          // grab the multiple full-bar rests segment, i.e. the voice's last segment JMI ???
          S_msrSegment
            multipleFullBarRestsSegment =
              fVoiceLastSegment;

          // append it to the current repeat's common part
          currentRepeat->
            getRepeatCommonPart ()->
              appendSegmentToRepeatCommonPart ( // NO !!!
                inputLineNumber,
                multipleFullBarRestsSegment,
                "appendMultipleFullBarRestsCloneToVoiceClone() 2");

          // forget about this voice last segment
          fVoiceLastSegment = nullptr;
        }

        else {
          // no
          // JMI ???
        }

        // append the multiple full-bar rests clone to the voice
        appendMultipleFullBarRestsToVoice (
          inputLineNumber,
          multipleFullBarRestsClone);

        // print resulting voice contents
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMultipleFullBarRestsDetails ()) {
          displayVoice (
            inputLineNumber,
            "appendMultipleFullBarRestsCloneToVoiceClone() 3");
        }
#endif
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendRepeatCloneToVoiceClone (
  int         inputLineNumber,
  const S_msrRepeat& repeatCLone)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat clone '" <<
      repeatCLone->asString () <<
      "' to voice clone \"" <<
      getVoiceName () <<  "\"" <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 1");
  }
#endif

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeatCLone != nullptr,
    "repeatCLone is null");
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // pushing repeat clone as the (new) current repeat
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Pushing repeat clone as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            std::endl;
        }
#endif

        // push the repeat clone onto the voice's repeat descrs stack
        pushRepeatOntoRepeatDescrsStack (
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 4");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasureRepeatStartInVoiceClone (
  int                 inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // is there already a current measures repeat in this voice?
      if (fVoicePendingMeasureRepeat) {
        std::stringstream s;

        s <<
          "current voice measures repeat is not null when attempting to handle measures repeat start '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // create the measures repeat clone and register it
      fVoicePendingMeasureRepeat =
        measureRepeat->
          createMeasureRepeatNewbornClone (
            this);

      // this voice contails measure repeats
      this->setVoiceContainsMeasureRepeats (
        inputLineNumber);

      // set fVoicePendingMeasureRepeat's build phase to completed
      fVoicePendingMeasureRepeat->
        setCurrentMeasureRepeatBuildPhaseKind (
          msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseJustCreated);

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
        displayVoiceMeasureRepeatAndVoice (
          inputLineNumber,
          "handleMeasureRepeatStartInVoiceClone() 2");
      }
#endif
      break;
  } // switch

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatStartInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasureRepeatEndInVoiceClone (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatEndInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // is there a current measures repeat in this voice?
      if (! fVoicePendingMeasureRepeat) {
        std::stringstream s;

        s <<
          "current voice measures repeat is null when attempting to handle measures repeat end '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasureRepeatPatternStartInVoiceClone (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternStartInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasureRepeat) {
    std::stringstream s;

    s <<
      "current voice measures repeat is null when attempting to handle measures repeat pattern start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create fVoicePendingMeasureRepeat' rest pattern
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a measures repeat pattern upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrMeasureRepeatPattern
    measureRepeatPattern =
      msrMeasureRepeatPattern::create (
        inputLineNumber,
        fVoicePendingMeasureRepeat);

  // register it in fVoicePendingMeasureRepeat
  fVoicePendingMeasureRepeat->
    setMeasureRepeatPattern (
      measureRepeatPattern);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternStartInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasureRepeatPatternEndInVoiceClone (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternEndInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasureRepeat) {
    std::stringstream s;

    s <<
      "current voice measures repeat is null when attempting to handle measures repeat pattern end '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternEndInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasureRepeatReplicasStartInVoiceClone (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasStartInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasureRepeat) {
    std::stringstream s;

    s <<
      "current voice measures repeat is null when attempting to handle measures repeat replicas start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create fVoicePendingMeasureRepeat' replicas
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a measures repeat replicas upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrMeasureRepeatReplicas
    measureRepeatReplicas =
      msrMeasureRepeatReplicas::create (
        inputLineNumber,
        fVoicePendingMeasureRepeat);

  // register it in fVoicePendingMeasureRepeat
  fVoicePendingMeasureRepeat->
    setMeasureRepeatReplicas (
      measureRepeatReplicas);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasStartInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasureRepeatReplicasEndInVoiceClone (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasEndInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasureRepeat) {
    std::stringstream s;

    s <<
      "current voice measures repeat is null when attempting to handle measures repeat replicas end '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasEndInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::appendMeasureRepeatCloneToVoiceClone (
  int                 inputLineNumber,
  const S_msrMeasureRepeat& measureRepeatClone)
{
  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatClone != nullptr,
    "measureRepeatClone is null");
#endif

  switch (fVoiceKind) { // superflous JMI ???
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Appending measures repeat clone '" <<
            measureRepeatClone->asString () <<
            "' to voice clone \"" <<
            getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }

        if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
          displayVoiceMeasureRepeatAndVoice (
            inputLineNumber,
            "appendMeasureRepeatCloneToVoiceClone() 1");
        }
#endif

        // is measures repeat nested in a repeat?
        if (fVoicePendingRepeatDescrsStack.size ()) {
          // yes

          S_msrRepeat
            currentRepeat =
              fVoicePendingRepeatDescrsStack.front ()->
                getRepeatDescrRepeat ();

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
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeatsDetails ()) {
          displayVoiceMeasureRepeatAndVoice (
            inputLineNumber,
            "appendMeasureRepeatCloneToVoiceClone() 3");
        }
#endif
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::handleHookedRepeatEndingEndInVoice (
  int           inputLineNumber,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hooked repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 1");
  }
#endif

  ++gIndenter;

/* JMI
        size_t voicerepeatDescrsStackSize =
          fVoicePendingRepeatDescrsStack.size ();
*/

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    std::stringstream s;

    s <<
      "repeats stack is empty when attempting to handle a hooked repeat ending end in voice '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // finalize current measure in voice
  finalizeLastAppendedMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "before adding a hooked repeat ending to current repeat");
  }
#endif

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleHooklessRepeatEndingEndInVoice (
  int           inputLineNumber,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hookless repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      std::endl;  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    std::stringstream s;

    s <<
      "repeats stack is empty when attempting to handle a hookless repeat ending end in voice '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // finalize current measure in voice
  finalizeLastAppendedMeasureInVoice (
    inputLineNumber);

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "before adding a hookless repeat ending to current repeat");
  }
#endif

  currentRepeat->
    addRepeatEndingToRepeat (
      inputLineNumber,
      repeatEnding);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "after adding a hookless repeat ending to current repeat");
  }
#endif

  // append currentRepeat to the list of initial elements
  appendRepeatCloneToInitialVoiceElements (
    inputLineNumber,
    currentRepeat,
    "handleHooklessRepeatEndingEndInVoice() 3");

  // pop it from the voice's repeat descrs stack
  popRepeatFromRepeatDescrsStack (
    inputLineNumber,
    currentRepeat,
    "handleHooklessRepeatEndingEndInVoice");

  // create a new last segment containing a new measure for the voice
  createNewLastSegmentForVoice (
    inputLineNumber,
    "handleHooklessRepeatEndingEndInVoice() 4");

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 5");
  }
#endif

  --gIndenter;
}

void msrVoice::handleRepeatEndingEndInVoice (
  int           inputLineNumber,
  const std::string& repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
                repeatEndingKind)
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoice() 0");
  }
#endif
}

void msrVoice::handleRepeatCommonPartStartInVoiceClone (
  int inputLineNumber) // may be "1, 2"
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat common part start in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartStartInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    std::stringstream s;

    s <<
      "repeats stack is empty when attempting to handle repeat common part start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

  // create currentRepeat's common part
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        inputLineNumber,
        currentRepeat);

  // register it in currentRepeat
  currentRepeat->
    setRepeatCommonPart (
      repeatCommonPart);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartStartInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleRepeatCommonPartEndInVoiceClone (
  int inputLineNumber) // may be "1, 2"
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat common part end in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    std::stringstream s;

    s <<
      "repeats stack is empty when attempting to handle repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleHookedRepeatEndingEndInVoiceClone (
  int           inputLineNumber,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hooked repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    std::stringstream s;

    s <<
      "repeats stack is empty when attempting to handle hooked repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleHooklessRepeatEndingEndInVoiceClone (
  int           inputLineNumber,
  const std::string& repeatEndingNumber) // may be "1, 2"
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hookless repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    std::stringstream s;

    s <<
      "repeats stack is empty when attempting to handle hookless repeat ending '" <<
 //     repeatEnding->asShortString () <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // grab currentRepeat
  S_msrRepeat
    currentRepeat =
      fVoicePendingRepeatDescrsStack.front ()->
        getRepeatDescrRepeat ();

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 3");
  }
#endif

  --gIndenter;
}

void msrVoice::handleRepeatEndingEndInVoiceClone (
  int           inputLineNumber,
  const std::string& repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoiceClone() 1");
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoiceClone() 2");
  }
#endif
}

void msrVoice::handleRepeatStartInVoiceClone (
  int         inputLineNumber,
  const S_msrRepeat& repeat)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatStartInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // fetch last segment's measure elements list
        const std::list<S_msrSegmentElement>
          voiceLastSegmentElementsList =
            fVoiceLastSegment->
              getSegmentElementsList ();

        // are there measures in the voice last segment?
        if (voiceLastSegmentElementsList.size ()) {
          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

          // move current last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "handleRepeatStartInVoiceClone() 2");

#ifdef MF_TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceRepeats ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "handleRepeatStartInVoiceClone() 3");
          }
#endif
        }
      }

      // create the repeat clone and stack it
#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceRepeats ()) {
        gLogStream <<
          "Creating a repeat upon its start in voice clone \"" <<
          getVoiceName () <<
          "\"" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      S_msrRepeat
        repeatClone =
          createARepeatCloneAndStackIt (
            inputLineNumber,
            repeat,
            "handleRepeatStartInVoiceClone() 4");

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceRepeats ()) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "handleRepeatStartInVoiceClone() 5");
      }
#endif
      break;
  } // switch

  --gIndenter;
}

void msrVoice::handleRepeatEndInVoiceClone (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "handleRepeatEndInVoiceClone() 1");
        }
#endif

        // is there a current repeat?
        switch (fVoicePendingRepeatDescrsStack.size ()) {
          case 0:
            {
              std::stringstream s;

              s <<
                "repeats stack is empty when attempting to handle a repeat end in voice clone '" <<
                asShortString () <<
                "' ";

              msrInternalError (
                gGlobalServiceRunData->getInputSourceName (),
                fInputLineNumber,
                __FILE__, __LINE__,
                s.str ());
            }
            break;

          case 1:
            {
              // this is a voice-level repeat

              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

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

              // pop currentRepeat from the voice's repeat descrs stack
              popRepeatFromRepeatDescrsStack (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone() 3");

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
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

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

              // pop currentRepeat from the voice's repeat descrs stack
              popRepeatFromRepeatDescrsStack (
                inputLineNumber,
                currentRepeat,
                "handleRepeatEndInVoiceClone() 5");
            }
        } // switch


#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "handleRepeatEndInVoiceClone() 6");
        }
#endif
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendMeasureRepeatReplicaToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Appending a measures repeat replica to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        // fetch last measure's full measure whole notes
        /* JMI
        int fullMeasureWholeNotesDuration =
          fVoiceLastSegment->
            getSegmentElementsList ().back ()->
              getFullMeasureWholeNotesDuration ();
              */

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) { // JMI
          gLogStream <<
            std::endl <<
            "***********" <<
            std::endl << std::endl;
          print (gLogStream);
          gLogStream <<
            "***********" <<
            std::endl << std::endl;
        }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
        // sanity check
        mfAssert (
          __FILE__, __LINE__,
          fVoicePendingMeasureRepeat != nullptr,
          "fVoicePendingMeasureRepeat is null");
#endif

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
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) { // JMI
          gLogStream <<
            "Setting the measures repeat replica to current measures repeat BBB in voice \"" <<
            fVoiceName <<
            "\"" <<
            std::endl;
        }
#endif

        fVoicePendingMeasureRepeat->
          setMeasureRepeatReplicas (
            measureRepeatReplicas);
      }
      break;
  } // switch
}

// void msrVoice::appendMultipleFullBarRestsToVoiceElementsList (
//   const S_msrMultipleFullBarRests& multipleFullBarRests)
// {
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
//     gLogStream <<
//       "Appending multiple full-bar rests " <<
//       multipleFullBarRests->asString () <<
//       " to voice \"" <<
//       fVoiceName <<
//       "\"" <<
//       std::endl;
//   }
// #endif
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     multipleFullBarRests != nullptr,
//     "multipleFullBarRests is null");
// #endif
//
// // KAKA  fVoiceInitialElementsList.push_back (multipleFullBarRests);
// }

void msrVoice::appendMeasureRepeatToVoiceElementsList (
  const S_msrMeasureRepeat& measureRepeat)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to voice \"" <<
      fVoiceName <<
      "\"," <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeat != nullptr,
    "measureRepeat is null");
#endif

// KAKA  fVoiceInitialElementsList.push_back (measureRepeat);
}

void msrVoice:: appendRepeatEndingCloneToVoice ( // JMI
  const S_msrRepeatEnding& repeatEndingClone)
{
#ifdef MF_TRACING_IS_ENABLED
  int inputLineNumber =
    repeatEndingClone->getInputLineNumber (); // JMI
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // add the repeat ending it to the voice current repeat
#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Appending a " <<
            msrRepeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending clone to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            std::endl;

          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "appendRepeatEndingCloneToVoice() 1");
        }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
        // sanity check
        if (! fVoicePendingRepeatDescrsStack.size ()) {
          std::stringstream s;

          s <<
            "repeats stack is empty when attempting to append a " <<
            msrRepeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending to voice '" <<
            asShortString () <<
            "' ";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
#endif

        S_msrRepeat
          currentRepeat =
            fVoicePendingRepeatDescrsStack.front ()->
              getRepeatDescrRepeat ();

        currentRepeat->
          addRepeatEndingToRepeat (
            repeatEndingClone->getInputLineNumber (),
            repeatEndingClone);

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "appendRepeatEndingCloneToVoice() 2");
        }
#endif
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::prependBarLineToVoice (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Prepending barLine '" <<
      barLine->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      std::endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    prependBarLineToSegment (barLine);

  --gIndenter;
}

void msrVoice::appendBarLineToVoice (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " <<
      barLine->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegnos ()) {
    gLogStream <<
      "Appending a segno to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      segno->getInputLineNumber (),
      "appendSegnoToVoice()");
  }

  fVoiceLastSegment->
    appendSegnoToSegment (segno);
}

void msrVoice::appendCodaToVoice (const S_msrCoda& coda)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceCodas ()) {
    gLogStream <<
      "Appending a coda to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      std::endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceEyeGlasses ()) {
    gLogStream <<
      "Appending a eyeGlasses to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      eyeGlasses->getInputLineNumber (),
      "appendEyeGlassesToVoice()");
  }

  fVoiceLastSegment->
    appendEyeGlassesToSegment (eyeGlasses);
}

void msrVoice::appendPedalToVoice (const S_msrPedal& pedal)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePedals ()) {
    gLogStream <<
      "Appending a pedal to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDamps ()) {
    gLogStream <<
      "Appending a damp to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDampAlls ()) {
    gLogStream <<
      "Appending a damp all to voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      dampAll->getInputLineNumber (),
      "appendDampAllToVoice()");
  }

  fVoiceLastSegment->
    appendDampAllToSegment (dampAll);
}

/*
S_msrElement msrVoice::removeLastElementFromVoice (  // JMI
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Removing last note" <<
      " from voice " << getVoiceName () <<
      std::endl;
  }
#endif

  return
    fVoiceLastSegment->
      removeLastElementFromSegment (inputLineNumber);
}
*/

void msrVoice::removeNoteFromVoice (
  int       inputLineNumber,
  const S_msrNote& note)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // remove note from voice last segment
  fVoiceLastSegment->
    removeNoteFromSegment (
      inputLineNumber,
      note);

  // update the part current measure position
  fVoiceUpLinkToStaff->
    getStaffUpLinkToPart ()->
      decrementPartMeasurePosition (
        inputLineNumber,
        note->
          getMeasureElementSoundingWholeNotes ());

  --gIndenter;
}

void msrVoice::removeElementFromVoice (
  int          inputLineNumber,
  const S_msrElement& element)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Removing element '" <<
      element->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    removeElementFromSegment (
      inputLineNumber,
      element);

  --gIndenter;
}

S_msrMeasure msrVoice::removeLastMeasureFromVoice (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Removing last measure from voice \"" <<
      getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // remove last measure
  S_msrMeasure
    result =
      fVoiceLastSegment->
        removeLastMeasureFromSegment (
          inputLineNumber,
          "removeLastMeasureFromVoice()");

  --gIndenter;

  // return it
  return result;
}

void msrVoice::finalizeLastAppendedMeasureInVoice (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing last appended measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }

  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeLastAppendedMeasureInVoice() 1");
  }
#endif

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVoiceLastAppendedMeasure != nullptr,
    "fVoiceLastAppendedMeasure is null");
#endif

  // finalize fVoiceLastAppendedMeasure
  msrMeasureRepeatContextKind
    measureRepeatContextKind =
      msrMeasureRepeatContextKind::kMeasureRepeatContextNone;

  fVoiceLastAppendedMeasure->
    finalizeMeasure (
      inputLineNumber,
      measureRepeatContextKind,
      "finalizeLastAppendedMeasureInVoice() 2");

/* JMI NO
  // forget about fVoiceLastAppendedMeasure
  setVoiceLastAppendedMeasure (
    nullptr);
*/

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // append a measure end syllable to the voice stanzas if any
      if (fVoiceStanzasMap.size ()) {
        for (std::pair<std::string, S_msrStanza> thePair : fVoiceStanzasMap) {
          const S_msrStanza& stanza = thePair.second;

          stanza->
            appendMeasureEndSyllableToStanza (
              inputLineNumber);
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeLastAppendedMeasureInVoice() 3");
  }
#endif

  --gIndenter;
}

void msrVoice:: collectVoiceMeasuresIntoFlatList (
  int inputLineNumber)
{
  // collect measures from the initial elements if any
  if (fVoiceInitialElementsList.size ()) {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "Collecting measures from the initial elements into voice \"" <<
        getVoiceName () <<
        "s measures flat list" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif
  }

  // collect measures from the last segment if any
  if (fVoiceLastSegment) {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "Collecting measures from the last segment into voice \"" <<
        getVoiceName () <<
        "s measures flat list" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    std::list<S_msrMeasure>
      lastSegmentMeasuresFlatList =
        fVoiceLastSegment->
          getSegmentMeasuresFlatList ();

    if (lastSegmentMeasuresFlatList.size ()) {
      for (S_msrMeasure measure : lastSegmentMeasuresFlatList) {
        fVoiceMeasuresFlatList.push_back (measure);
      } // for
    }
  }
}

void msrVoice::finalizeVoice (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }

  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeVoice() 1");
  }
#endif

  if (fVoiceHasBeenFinalized) {
    std::stringstream s;

    s <<
      "Attempting to finalize  voice \"" <<
      asShortString () <<
      "\" more than once";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // set part shortest note duration if relevant
  S_msrPart
    voicePart =
      fetchVoiceUpLinkToPart ();

  Rational
    partShortestNoteDuration =
      voicePart->
        getPartShortestNoteDuration ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", fVoiceShortestNoteDuration: " << fVoiceShortestNoteDuration <<
      ", partShortestNoteDuration: " << partShortestNoteDuration <<
      std::endl;
  }
#endif

  if (fVoiceShortestNoteDuration < partShortestNoteDuration) {
    // set the voice part shortest note duration
    voicePart->
      setPartShortestNoteDuration (
        fVoiceShortestNoteDuration);

    // set the voice part shortest note tuplet factor // JMI
    voicePart->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
  }

  // is this voice totally empty? this should be rare...
  if (
    fVoiceInitialElementsList.size () == 0
      &&
    fVoiceLastSegment != nullptr // JMI should not occur??? v0.9.63
      &&
    fVoiceLastSegment->getSegmentElementsList ().size () == 0
  ) {
    std::stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" is totally empty, no contents ever specified for it" <<
      std::endl;

    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  // are there pending repeats in the voice repeats stack???
  size_t voicePendingRepeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  if (voicePendingRepeatDescrsStackSize) {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "finalizeVoice() 2");
      }
#endif

    std::stringstream s;

    s <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "There is", "There are") <<
      ' ' <<
      voicePendingRepeatDescrsStackSize <<
      ' ' <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "repeat", "repeats") <<
      " pending in the voice repeats stack in voice \"" <<
      asShortString () <<
      "\" ";

    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      s.str ());
  }

  // collect the voice measures into the flat list
  collectVoiceMeasuresIntoFlatList (
    inputLineNumber);

  fVoiceHasBeenFinalized = true;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
      inputLineNumber,
      "finalizeVoice() 3");
  }
#endif

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
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }

  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeVoice() 1");
  }
#endif

  if (fVoiceHasBeenFinalized) {
    std::stringstream s;

    s <<
      "Attempting to finalize  voice \"" <<
      asShortString () <<
      "\" more than once";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // set part shortest note duration if relevant
  S_msrPart
    voicePart =
      fetchVoiceUpLinkToPart ();

  Rational
    partShortestNoteDuration =
      voicePart->
        getPartShortestNoteDuration ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", fVoiceShortestNoteDuration: " << fVoiceShortestNoteDuration <<
      ", partShortestNoteDuration: " << partShortestNoteDuration <<
      std::endl;
  }
#endif

  if (fVoiceShortestNoteDuration < partShortestNoteDuration) {
    // set the voice part shortest note duration
    voicePart->
      setPartShortestNoteDuration (
        fVoiceShortestNoteDuration);

    // set the voice part shortest note tuplet factor // JMI
    voicePart->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
  }

  // is this voice totally empty? this should be rare...
  if (
    fVoiceInitialElementsList.size () == 0
      &&
    fVoiceLastSegment->getSegmentElementsList ().size () == 0
  ) {
    std::stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" is totally empty, no contents ever specified for it" <<
      std::endl;

    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  // are there pending repeats in the voice repeats stack???
  size_t voicePendingRepeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  if (voicePendingRepeatDescrsStackSize) {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "finalizeVoice() 2");
      }
#endif

    std::stringstream s;

    s <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "There is", "There are") <<
      ' ' <<
      voicePendingRepeatDescrsStackSize <<
      ' ' <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "repeat", "repeats") <<
      " pending in the voice repeats stack in voice \"" <<
      asShortString () <<
      "\" ";

    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      s.str ());
  }

  // collect the voice measures into the flat list
  collectVoiceMeasuresIntoFlatList (
    inputLineNumber);

  fVoiceHasBeenFinalized = true;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
      inputLineNumber,
      "finalizeVoice() 3");
  }
#endif

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing all the measures of voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  for (S_msrMeasure measure : fVoiceMeasuresFlatList) {
    measure->
      finalizeMeasure (
        inputLineNumber,
        msrMeasureRepeatContextKind::kMeasureRepeatContextNone,
        "finalizeVoiceAndAllItsMeasures()");
  } // for
}

void msrVoice::checkBeamNumber (S_msrBeam beam, S_msrNote note)
{
  int inputLineNumber =
    beam->getInputLineNumber ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Checking the order of the beams begins, continues and ends" <<
      " upon beam '" <<
      beam->asShortString () <<
      "' and note '" <<
      note->asString () <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  msrBeamKind beamKind   = beam->getBeamKind ();
  int         beamNumber = beam->getBeamNumber ();

  size_t
    noteBeamNumbersStackSize =
      fVoiceBeamNumbersStack.size ();

  switch (beamKind) {
    case msrBeamKind::kBeam_UNKNOWN:
      {
        std::stringstream s;

        s <<
          "appending unknown beam " <<
          beam->asShortString () <<
          " to note " <<
          note->asShortString ();

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrBeamKind::kBeamBegin:
      if (noteBeamNumbersStackSize) {
        // the stack is not empty
        int voiceBeamNumbersStackTop =
          fVoiceBeamNumbersStack.top ();

        if (beamNumber != voiceBeamNumbersStackTop + 1) {
          std::stringstream s;

          s <<
            "appending beam begin " <<
            beam->asShortString () <<
            " to note " <<
            note->asShortString () <<
            "whilst the current beam began with number '" <<
            voiceBeamNumbersStackTop <<
            "'";

          msrError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }

      fVoiceBeamNumbersStack.push (beamNumber);
      break;

    case msrBeamKind::kBeamContinue:
      if (! noteBeamNumbersStackSize) {
        // the stack is empty
        std::stringstream s;

        s <<
          "appending beam continue " <<
          beam->asShortString () <<
          " to note " <<
          note->asShortString () <<
          "whilst there is no preceding beam begin with number '" <<
          beamNumber <<
          "'";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          beam->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      else {
        // the stack is not empty
        int voiceBeamNumbersStackTop =
          fVoiceBeamNumbersStack.top ();

        if (beamNumber != voiceBeamNumbersStackTop) {
          std::stringstream s;

          s <<
            "appending beam continue " <<
            beam->asShortString () <<
            " to note " <<
            note->asShortString () <<
            "whilst the current beam began with number '" <<
            voiceBeamNumbersStackTop <<
            "'";

          msrError (
            gGlobalServiceRunData->getInputSourceName (),
            beam->getInputLineNumber (),
            __FILE__, __LINE__,
            s.str ());
        }
      }
      break;

    case msrBeamKind::kBeamEnd:
      if (noteBeamNumbersStackSize) {
        // the stack is not empty
        int voiceBeamNumbersStackTop =
          fVoiceBeamNumbersStack.top ();

        if (beamNumber != voiceBeamNumbersStackTop) {
          std::stringstream s;

          s <<
            "appending beam end " <<
            beam->asShortString () <<
            " to note " <<
            note->asShortString () <<
            "whilst the current beam began with number '" <<
            voiceBeamNumbersStackTop <<
            "'";

          msrError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }

      fVoiceBeamNumbersStack.pop ();
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoice::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoice::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoice::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoice::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoice::browseData ()" <<
      std::endl;
  }

  // browse the voice initial elements
  if (fVoiceInitialElementsList.size ()) {
    for (
      std::list<S_msrVoiceElement>::const_iterator i = fVoiceInitialElementsList.begin ();
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
  if (fVoiceStanzasMap.size ()) {
    for (std::pair<std::string, S_msrStanza> thePair : fVoiceStanzasMap) {
      const S_msrStanza& stanza = thePair.second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrVoice::browseData ()" <<
      std::endl;
  }
}

std::string msrVoice::voiceNumberAsString () const
{
  std::string result = std::to_string (fVoiceNumber);

  if (fVoiceNumber == msrPart::K_PART_HARMONIES_VOICE_NUMBER) {
    result += " (msrPart::K_PART_HARMONIES_VOICE_NUMBER)";
  }
  else if (fVoiceNumber == msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER) {
    result += " (msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER)";
  }
  else {
    // nothing more
  }

  return result;
}

std::string msrVoice::regularVoiceStaffSequentialNumberAsString () const
{
  std::string result;

  if (fRegularVoiceStaffSequentialNumber == msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER) {
    result += " (msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER)";
  }
  else {
    // nothing more
  }

  return result;
}

std::string msrVoice::asShortString () const
{
  std::stringstream s;

  s <<
    "[Voice \"" << getVoiceName () << "\", " <<
    msrVoiceKindAsString (fVoiceKind) <<
//     msrVoiceKindAsStringForPrint (fVoiceKind) << // JMI
    " voice \"" << getVoiceName () <<
    "\", fVoiceNumber '" <<
    fVoiceNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrVoice::asString () const
{
  std::stringstream s;

  s <<
    "[Voice \"" << getVoiceName () << "\", " <<
    msrVoiceKindAsString (fVoiceKind) <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrVoice::displayVoice (
  int           inputLineNumber,
  const std::string& context) const
{
  gLogStream <<
    std::endl <<
    "*********>> Displaying voice \"" <<
    getVoiceName () <<
    "\" (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    std::endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    std::endl << std::endl;
}

void msrVoice::displayVoiceRepeatsStackMultipleFullBarRestsMeasureRepeatAndVoice (
  int           inputLineNumber,
  const std::string& context)
{
  displayVoice (
    inputLineNumber,
    context);

  displayVoiceRepeatsStack (
    inputLineNumber,
    context);

  displayVoiceMultipleFullBarRests (
    inputLineNumber,
    context);

  displayVoiceMeasureRepeat (
    inputLineNumber,
    context);
}

void msrVoice::printFull (std::ostream& os) const
{
  os <<
    '[' <<
    msrVoiceKindAsStringForPrint (fVoiceKind) <<
    " voice \"" << getVoiceName () <<
    "\", fVoiceNumber '" <<
    fVoiceNumber <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  os << std::left <<
    '(' <<
    mfSingularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured bass elements") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceRestsCounter, "rest", "rests") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceSkipsCounter, "skip", "skips") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ")" <<
    std::endl;

  const int fieldWidth = 41;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fVoiceUpLinkToStaff" << ": " <<
    fVoiceUpLinkToStaff->getStaffName () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceCurrentMeasureNumber" << ": \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
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
    std::endl;

#ifdef MF_TRACING_IS_ENABLED
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
    std::setw (fieldWidth) << "fVoiceFirstClef" << ": ";
  if (fVoiceFirstClef) {
    os <<
      fVoiceFirstClef;
  }
  else {
    os << "[NONE]" << std::endl;
  }
  os << std::left <<
    std::setw (fieldWidth) << "fVoiceCurrentClef" << ": ";
  if (fVoiceCurrentClef) {
    os <<
      fVoiceCurrentClef;
  }
  else {
    os << "[NONE]" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) << "fVoiceCurrentKey" << ": ";
  if (fVoiceCurrentKey) {
    os <<
      fVoiceCurrentKey;
  }
  else {
    os << "[NONE]" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) << "fVoiceCurrentTimeSignature" << ": ";
  if (fVoiceCurrentTimeSignature) {
    os << std::endl;
    ++gIndenter;

    os <<
      fVoiceCurrentTimeSignature;

    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
  }
#endif

// JMI
  // print the regular voice harmonies voice name if any
  os << std::left <<
    std::setw (fieldWidth) << "fRegularVoiceForwardLinkToHarmoniesVoice" << ": ";
  if (fRegularVoiceForwardLinkToHarmoniesVoice) {
    os <<
      fRegularVoiceForwardLinkToHarmoniesVoice->getVoiceName ();
  }
  else {
    os <<
      "[NONE]";
  }
  os << std::endl;

  // print the figured bass voice name if any
  os << std::left <<
    std::setw (fieldWidth) << "fRegularVoiceForwardLinkToFiguredBassVoice" << ": ";
  if (fRegularVoiceForwardLinkToFiguredBassVoice) {
    os <<
      fRegularVoiceForwardLinkToFiguredBassVoice->getVoiceName ();
  }
  else {
    os <<
      "[NONE]";
  }
  os << std::endl;
//

  os << std::left <<
    std::setw (fieldWidth) << "fVoiceShortestNoteDuration" << ": " <<
    fVoiceShortestNoteDuration <<
    std::endl <<
    std::setw (fieldWidth) << "fVoiceShortestNoteTupletFactor" << ": " <<
    std::endl;

  ++gIndenter;
  os <<
    fVoiceShortestNoteTupletFactor;
  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) << "fVoiceHasBeenFinalized" << ": " <<
    fVoiceHasBeenFinalized <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) << "fCurrentVoicePosition" << ": " <<
    fCurrentVoicePosition <<
    std::endl <<
    std::setw (fieldWidth) << "fCurrentVoiceMoment" << ": " <<
    fCurrentVoiceMoment;

  os << std::left <<
    std::setw (fieldWidth) << "fMusicHasBeenInsertedInVoice" << ": " <<
    fMusicHasBeenInsertedInVoice <<
    std::endl <<
    std::setw (fieldWidth) << "fVoiceContainsMultipleFullBarRests" << ": " <<
    fVoiceContainsMultipleFullBarRests <<
    std::endl <<
    std::setw (fieldWidth) << "fVoiceContainsMeasureRepeats" << ": " <<
    fVoiceContainsMeasureRepeats <<
    std::endl;

  // print the voice first segment if any
  os <<
    std::setw (fieldWidth) << "fVoiceFirstSegment" << ": ";
  if (fVoiceFirstSegment) {
    os <<
      "'" <<
      fVoiceFirstSegment->getSegmentAbsoluteNumber () <<
      "'";
    }
  else {
    os <<
      "[NONE]";
  }
  os << std::endl;

  // print the voice last appended measure if any
  os <<
    std::setw (fieldWidth) << "fVoiceLastAppendedMeasure" << ": ";
  if (fVoiceLastAppendedMeasure) {
    os <<
      "'" <<
      fVoiceLastAppendedMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "[NONE]";
  }
  os << std::endl;

  // print the voice first measure if any
  os <<
    std::setw (fieldWidth) << "fVoiceFirstMeasure" << ": ";
  if (fVoiceFirstMeasure) {
    os <<
      "'" <<
      fVoiceFirstMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "[NONE]";
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
        "[NONE]";
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
      "[NONE]" <<
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
      voiceInitialElementsListSize << " elements" <<
      std::endl;

    ++gIndenter;

    std::list<S_msrVoiceElement>::const_iterator
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
      std::endl <<
      fVoiceLastSegment;

    --gIndenter;
  }
  else {
    os <<
      "[NONE]" <<
      std::endl;
  }

  os << std::endl;

  // print the stanzas if any
  os <<
    std::setw (fieldWidth) <<
    "fVoiceStanzasMap" << ": ";

  if (fVoiceStanzasMap.size ()) {
    os << std::endl;

    ++gIndenter;

    for (std::pair<std::string, S_msrStanza> thePair : fVoiceStanzasMap) {
      const S_msrStanza& stanza = thePair.second;

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

void msrVoice::print (std::ostream& os) const
{
  os <<
    '[' <<
    msrVoiceKindAsStringForPrint (fVoiceKind) <<
    " voice \"" << getVoiceName () <<
    "\", fVoiceNumber '" <<
    fVoiceNumber <<
    "'" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 41;

  // print the regular voice harmonies voice name if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fRegularVoiceForwardLinkToHarmoniesVoice" << ": ";
  if (fRegularVoiceForwardLinkToHarmoniesVoice) {
    os <<
      fRegularVoiceForwardLinkToHarmoniesVoice->getVoiceName ();
  }
  else {
    os <<
      "[NONE]";
  }
  os << std::endl;

  // print the figured bass voice name if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fRegularVoiceForwardLinkToFiguredBassVoice" << ": ";
  if (fRegularVoiceForwardLinkToFiguredBassVoice) {
    os <<
      fRegularVoiceForwardLinkToFiguredBassVoice->getVoiceName ();
  }
  else {
    os <<
      "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    '(' <<
    mfSingularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured bass elements") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceRestsCounter, "rest", "rests") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceSkipsCounter, "skip", "skips") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ")" <<
    std::endl;

#ifdef MF_TRACING_IS_ENABLED
  displayVoiceMeasuresFlatList (fieldWidth);
#endif

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

    std::list<S_msrVoiceElement>::const_iterator
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

  /* JMI
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");
#endif
*/

  // print the last segment
  if (fVoiceLastSegment) {
    os <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fVoiceLastSegment" <<
      std::endl;

    ++gIndenter;
    os << fVoiceLastSegment;
    os << std::endl;
    --gIndenter;
  }
  else {
    os <<
      "*** voiceLastSegment is null ***" << // JMI
      std::endl;
  }
  os << std::endl;

  // print the stanzas if any
  if (fVoiceStanzasMap.size ()) {
    os <<
      "Stanzas:" <<
      std::endl;

    ++gIndenter;

    for (std::pair<std::string, S_msrStanza> thePair : fVoiceStanzasMap) {
      const S_msrStanza& stanza = thePair.second;

      os << stanza;
    } // for

    --gIndenter;
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
//   if (fPartAllStavesList.size ()) {
//     std::list<S_msrStaff>::const_iterator
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
