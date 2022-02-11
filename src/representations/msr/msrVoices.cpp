/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"


#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrVoices.h"

#include "msrRepeats.h"
#include "msrStaves.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string voiceRepeatPhaseKindAsString (
  msrVoiceRepeatPhaseKind
    afterRepeatComponentPhaseKind)
{
  string result;

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

string voiceFinalizationStatusKindAsString (
  msrVoiceFinalizationStatusKind voiceFinalizationStatusKind)
{
  string result;

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

string voiceFinalizationStatusKindAsString (
  msrVoiceCreateInitialLastSegmentKind voiceCreateInitialLastSegmentKind)
{
  string result;

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

//______________________________________________________________________________
S_msrRepeatDescr msrRepeatDescr::create (
  int         repeatDescrStartInputLineNumber,
  S_msrRepeat repeatDescrRepeat)
{
  msrRepeatDescr* o = new
    msrRepeatDescr (
      repeatDescrStartInputLineNumber,
      repeatDescrRepeat);
  assert (o != nullptr);
  return o;
}

msrRepeatDescr::msrRepeatDescr (
  int         repeatDescrStartInputLineNumber,
  S_msrRepeat repeatDescrRepeat)
{
  fRepeatDescrStartInputLineNumber =
    repeatDescrStartInputLineNumber;
  fRepeatDescrRepeat = repeatDescrRepeat;
}

msrRepeatDescr::~msrRepeatDescr ()
{}

string msrRepeatDescr::repeatDescrAsString () const
{
  stringstream s;

  s <<
    "fRepeatDescrRepeat = '" <<
    fRepeatDescrRepeat->asShortString () <<
    "', fRepeatDescrStartInputLineNumber = " <<
    fRepeatDescrStartInputLineNumber;

  return s.str ();
}

void msrRepeatDescr::print (ostream& os) const
{
  const int fieldWidth = 29;

  os << left <<
    setw (fieldWidth) <<
    "fRepeatDescrRepeat" << " : " <<
    fRepeatDescrRepeat <<
    endl <<
    setw (fieldWidth) <<
    "fRepeatDescrStartInputLineNumber" << " : " <<
    fRepeatDescrStartInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrRepeatDescr& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
// constants
const int msrVoice::K_NO_VOICE_NUMBER                      = -99;
const int msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER    =  20;
const int msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER =  40;

// global variable
int msrVoice::gVoicesCounter = 0;

S_msrVoice msrVoice::create (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUpLink)
{
  msrVoice* o =
    new msrVoice (
      inputLineNumber,
      voiceKind,
      voiceNumber,
      voiceCreateInitialLastSegmentKind,
      voiceStaffUpLink);
  assert (o != nullptr);

  return o;
}

S_msrVoice msrVoice::createRegularVoice (
  int        inputLineNumber,
  int        voiceNumber,
  S_msrStaff voiceStaffUpLink)
{
  return
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindRegular,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceStaffUpLink);
}

S_msrVoice msrVoice::createHarmoniesVoice ( // unused yet JMI
  int        inputLineNumber,
  int        voiceNumber,
  S_msrStaff voiceStaffUpLink)
{
  return
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindHarmonies,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceStaffUpLink);
}

S_msrVoice msrVoice::createFiguredBassVoice ( // unused yet JMI
  int        inputLineNumber,
  int        voiceNumber,
  S_msrStaff voiceStaffUpLink)
{
  return
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindFiguredBass,
      voiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        // the initial last segment is ready to receive music
      voiceStaffUpLink);
}

msrVoice::msrVoice (
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  msrVoiceCreateInitialLastSegmentKind
               voiceCreateInitialLastSegmentKind,
  S_msrStaff   voiceStaffUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceStaffUpLink != nullptr,
    "voiceStaffUpLink is null");

  // set voice staff upLink
  fVoiceStaffUpLink = voiceStaffUpLink;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice number
  fVoiceNumber = voiceNumber;

  // do other initializations
  initializeVoice (
    voiceCreateInitialLastSegmentKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating voice \"" << asString () << "\"" <<
      endl;
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
  fVoiceStaffUpLink = nullptr;

  // set voice kind
  fVoiceKind = voiceKind;

  // set voice number
  fVoiceNumber = voiceNumber;

  // do other initializations
  initializeVoice (
    msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes); // JMI default value

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating voice \"" << asString () << "\"" <<
      endl;
  }
#endif
}

msrVoice::~msrVoice ()
{}

S_msrPart msrVoice::fetchVoicePartUpLink () const
{
  S_msrPart result;

  if (fVoiceStaffUpLink) {
    result =
      fVoiceStaffUpLink->
        getStaffPartUpLink ();
  }

  return result;
}

S_msrPartGroup msrVoice::fetchVoicePartGroupUpLink () const
{
  S_msrPartGroup result;

  if (fVoiceStaffUpLink) {
    result =
      fVoiceStaffUpLink->
        fetchStaffPartGroupUpLink ();
  }

  return result;
}

S_msrScore msrVoice::fetchVoiceScoreUpLink () const
{
  S_msrScore result;

  if (fVoiceStaffUpLink) {
    result =
      fVoiceStaffUpLink->
        fetchStaffScoreUpLink ();
  }

  return result;
}

void msrVoice::setRegularVoiceStaffSequentialNumber (
  int regularVoiceStaffSequentialNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Setting the regular voice staff sequential number of voice \"" <<
      getVoiceName () <<
      "\" to " << regularVoiceStaffSequentialNumber <<
      endl;
  }
#endif

  fRegularVoiceStaffSequentialNumber =
    regularVoiceStaffSequentialNumber;
}

void msrVoice::setVoiceNameFromNumber (
  int inputLineNumber,
  int voiceNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Setting the name of '" <<
      msrVoiceKindAsString (fVoiceKind) <<
      "' voice from voice number '" << voiceNumber <<
      "'" <<
      endl;
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_Voice_" +
        mfInt2EnglishWord (voiceNumber);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_Voice_" +
        mfInt2EnglishWord (
          voiceNumber) + // JMI - msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER) +
          "_HARMONIES";
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      fVoiceName =
        fVoiceStaffUpLink->getStaffName () +
        "_Voice_" +
        mfInt2EnglishWord (
          voiceNumber) + // JMI - msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) +
          "_FIGURED_BASS";
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "The resulting voice name is \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif
}

void msrVoice::setVoiceLastSegmentInVoiceClone (
  S_msrSegment segment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Setting segment '" <<
      segment->asShortString () <<
      "' as last segment in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      ", line " << segment->getInputLineNumber () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");

  fVoiceLastSegment = segment;

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }
}

void msrVoice::appendSegmentToVoiceClone ( //JMI unused ???
  S_msrSegment segment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending segment '" <<
      segment->asString () <<
      "' to voice clone \"" <<
      fVoiceName <<
      "\"" <<
      endl;
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Segment '" <<
        segment->asShortString () <<
        "' becomes the new last segment in voice clone \"" <<
        fVoiceName <<
        "\"" <<
        endl;
    }
#endif

    fVoiceLastSegment = segment;

    if (! fVoiceFirstSegment) {
      fVoiceFirstSegment = fVoiceLastSegment;
    }
  }
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Initializing voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUpLink->getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  // check voice number
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // the voice number should be positive
      if (fVoiceNumber < 1 || fVoiceNumber > 4) {
        stringstream s;

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
        stringstream s;

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
    rational (INT_MAX, 1);
  fVoiceShortestNoteTupletFactor =
    rational (1, 1);

  // set voice current measure number
  fVoiceCurrentMeasureNumber = // JMI "??";
    fetchVoicePartUpLink ()->
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
  fVoiceActualFiguredBassElementsCounter = 0;

  // regular measure ends detection
  fWholeNotesSinceLastRegularMeasureEnd = rational (0, 1);

  // set voice current after repeat component phase kind
  setCurrentVoiceRepeatPhaseKind (
    fInputLineNumber,
    msrVoiceRepeatPhaseKind::kVoiceRepeatPhaseNone);

  // full measure rests
  fVoiceContainsFullMeasureRests  = false;
  fVoiceRemainingFullMeasureRests = 0;

  // measures repests
  fVoiceContainsMeasureRepeats = false;

    // position in voice
  fCurrentPositionInVoice = rational (0, 1);
  fCurrentMomentInVoice = msrMoment (rational (0,1), rational (0,1));

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    gLogStream <<
      "Initial contents of voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUpLink->getStaffName () <<
      "\":" <<
      endl;

    ++gIndenter;

    this->print (gLogStream);

    --gIndenter;
  }
#endif

  --gIndenter;
}

void msrVoice::changeVoiceIdentity ( // after a deep clone
  int voiceNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Changing identity of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", number: " << voiceNumber <<
      endl;
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
  S_msrStaff staffClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating a newborn clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    staffClone != nullptr,
    "staffClone is null");

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
  int          inputLineNumber,
  msrVoiceKind voiceKind,
  int          voiceNumber,
  S_msrStaff   containingStaff)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating a deep clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", to be placed in containing staff \"" <<
      containingStaff->getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingStaff != nullptr,
    "containingStaff is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    gLogStream <<
      "****" <<
      " BEFORE voiceDeepClone = " <<
      endl;

    print (gLogStream);

    gLogStream <<
      endl <<
      "****" <<
      endl;
  }
#endif

  S_msrVoice
    voiceDeepClone =
      msrVoice::create (
        fInputLineNumber,
        voiceKind,
        voiceNumber,
        msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentNo,
          // the voice initial last segment
          // will be created by deep cloning below
        containingStaff);

  // voice numbers
  voiceDeepClone->fRegularVoiceStaffSequentialNumber =
    fRegularVoiceStaffSequentialNumber;

  // voice name
  if (false) { // JMI
    voiceDeepClone->fVoiceName =
      fVoiceName;
  }

/* JMI
  // set voice name // JMI
  voiceDeepClone->
    setVoiceNameFromNumber (
      fInputLineNumber,
      voiceNumber);
*/

  // counters
  voiceDeepClone->fVoiceActualNotesCounter =
    fVoiceActualNotesCounter;

  voiceDeepClone->fVoiceRestsCounter =
    fVoiceRestsCounter;

  voiceDeepClone->fVoiceSkipsCounter =
    fVoiceSkipsCounter;

  voiceDeepClone->fVoiceActualHarmoniesCounter =
    fVoiceActualHarmoniesCounter;

  voiceDeepClone->fVoiceActualFiguredBassElementsCounter =
    fVoiceActualFiguredBassElementsCounter;

  // measures
  voiceDeepClone->fVoiceCurrentMeasureNumber =
    fVoiceCurrentMeasureNumber;
  voiceDeepClone->fVoiceCurrentMeasureOrdinalNumber =
    fVoiceCurrentMeasureOrdinalNumber;
  voiceDeepClone->fVoiceCurrentMeasurePuristNumber =
    fVoiceCurrentMeasurePuristNumber;

  // musically empty voices
  voiceDeepClone->fMusicHasBeenInsertedInVoice =
    fMusicHasBeenInsertedInVoice;

  // regular measure ends detection
  voiceDeepClone->fWholeNotesSinceLastRegularMeasureEnd =
    fWholeNotesSinceLastRegularMeasureEnd;

  // incomplete measures after repeats detection
  voiceDeepClone->fCurrentVoiceRepeatPhaseKind =
    fCurrentVoiceRepeatPhaseKind;

  // initial elements
  int numberOfInitialElements =
    fVoiceInitialElementsList.size ();

  if (numberOfInitialElements) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "There are " <<
        numberOfInitialElements <<
        " initial elements in voice \"" <<
        getVoiceName () <<
        "\"" <<
        endl;
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
            voiceDeepClone);
            */
      }

      else if (
        // create the segment deep clone
        S_msrSegment segment = dynamic_cast<msrSegment*>(&(*(voiceElement)))
      ) {
        elementDeepClone =
          segment->createSegmentDeepClone (
            voiceDeepClone);
      }

      else {
        stringstream s;

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
      voiceDeepClone->
        fVoiceInitialElementsList.push_back (
          elementDeepClone);
    } // for
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "There are no initial elements in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
#endif
  }

  // last segment
  if (fVoiceLastSegment) { // JMI
    voiceDeepClone->fVoiceLastSegment =
      fVoiceLastSegment->
        createSegmentDeepClone (
          voiceDeepClone);

    if (! voiceDeepClone->fVoiceFirstSegment) {
      voiceDeepClone->fVoiceFirstSegment =
        voiceDeepClone->fVoiceLastSegment;
    }
  }
  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "There is no last segment in voice \"" <<
        getVoiceName () <<
        "\" to be deep copied" <<
        endl;
    }
#endif
  }

  // full measure rests
  voiceDeepClone->fVoiceContainsFullMeasureRests =
    fVoiceContainsFullMeasureRests;

  // measures reeats
  voiceDeepClone->fVoiceContainsMeasureRepeats =
    fVoiceContainsMeasureRepeats;

  // stanzas
  for (pair<string, S_msrStanza> thePair : fVoiceStanzasMap) {
    S_msrStanza stanza = thePair.second;

    voiceDeepClone->
      addStanzaToVoiceWithoutCatchUp ( // JMI
        stanza->
          createStanzaDeepClone (
            voiceDeepClone));
  } // for

  // upLinks
  voiceDeepClone->fVoiceStaffUpLink =
    containingStaff;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    gLogStream <<
      "****" <<
      " AFTER voiceDeepClone = " <<
      endl <<
      voiceDeepClone <<
      endl <<
      "****" <<
      endl;
  }
#endif

  return voiceDeepClone;
}

void msrVoice::setNextMeasureNumberInVoice (
  int           inputLineNumber,
  const string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    setNextMeasureNumberInSegment (
      inputLineNumber,
      nextMeasureNumber);

  // is there a pending full measure rests in this voice?
  if (fVoiceFullMeasureRestsWaitingForItsNextMeasureNumber) {
    // yes
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
      gLogStream <<
        "There is a full measure rests waiting for its next measure number" <<
        ", fVoiceRemainingFullMeasureRests = " <<
        fVoiceRemainingFullMeasureRests <<
        "' in voice \"" <<
        fVoiceName << "\"" <<
        endl;
    }
#endif

    --fVoiceRemainingFullMeasureRests;

    // is this the last measure in the row?
    if (fVoiceRemainingFullMeasureRests == 0) {
      // yes, set waiting full measure rests's next measure number
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
        gLogStream <<
          "Setting full measure rests next measure number to '" <<
          nextMeasureNumber <<
          "' in voice \"" <<
          fVoiceName << "\"" <<
          endl;
      }
#endif

      fVoiceFullMeasureRestsWaitingForItsNextMeasureNumber->
        setFullMeasureRestsNextMeasureNumber (
          nextMeasureNumber);

      // forget about this waiting full measure rests
      fVoiceFullMeasureRestsWaitingForItsNextMeasureNumber = nullptr;
    }
  }

  --gIndenter;
}

void msrVoice::incrementVoiceCurrentMeasurePuristNumber (
  int           inputLineNumber,
  const string& context)
{
  if (fVoiceCurrentMeasurePuristNumber < 0) {
    fVoiceCurrentMeasurePuristNumber = 0;
    fVoiceFirstMeasurePuristNumber = fVoiceCurrentMeasurePuristNumber;
  }
  else {
    ++fVoiceCurrentMeasurePuristNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Incrementing voice current measure purist number to '" <<
      fVoiceCurrentMeasurePuristNumber <<
      "' (" << context << ")" <<
      " in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif
}

void msrVoice::appendMeasureCloneToVoiceClone (
  int          inputLineNumber,
  S_msrMeasure measureClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending measure clone '" <<
      measureClone->asShortString () <<
      "' to voice '" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append measureClone to the voice last segment
  fVoiceLastSegment->
    appendMeasureToSegment (measureClone);

  // measureClone is the new voice last appended measure
  fVoiceLastAppendedMeasure = measureClone;

  --gIndenter;
}

void msrVoice::setWholeNotesSinceLastRegularMeasureEnd (
  int             inputLineNumber,
  const rational& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting voice whole notes since last regular measure end to '" <<
      value <<
  // JMI    "' (" << context << ")" <<
      " in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fWholeNotesSinceLastRegularMeasureEnd = value;
}

void msrVoice::setCurrentVoiceRepeatPhaseKind (
  int      inputLineNumber,
  msrVoiceRepeatPhaseKind
           afterRepeatComponentPhaseKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Setting voice current after repeat component phase kind to '" <<
      voiceRepeatPhaseKindAsString (
        afterRepeatComponentPhaseKind) <<
 // JMI     "' (" << context << ")" <<
      " in voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fCurrentVoiceRepeatPhaseKind =
    afterRepeatComponentPhaseKind;
}

void msrVoice::createNewLastSegmentForVoice (
  int           inputLineNumber,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a new last segment for voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    string
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    string
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
  S_msrMeasure  firstMeasure,
  const string& context)
{
  // create the last segment
  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      this);

  if (! fVoiceFirstSegment) {
    fVoiceFirstSegment = fVoiceLastSegment;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Creating a new last segment '" <<
      fVoiceLastSegment->asShortString () <<
      "' from its first measure '" <<
      firstMeasure->getMeasureElementMeasureNumber () <<
      "' for voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // append firstMeasure to fVoiceLastSegment
  fVoiceLastSegment->
    appendMeasureToSegment (firstMeasure);

  // firstMeasure is the new voice last appended measure
  fVoiceLastAppendedMeasure = firstMeasure;

  // is firstMeasure the first one it the voice?
  if (! fVoiceFirstMeasure) {
    // yes, register it as such
    setVoiceFirstMeasure (
      firstMeasure);

    firstMeasure->
      setMeasureFirstInVoice ();
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createNewLastSegmentFromItsFirstMeasureForVoice()");
  }
#endif
}

S_msrMeasure msrVoice::createAMeasureAndAppendItToVoice (
  int           inputLineNumber,
  const string& measureNumber,
  msrMeasureImplicitKind
                measureImplicitKind)
{
  fVoiceCurrentMeasureNumber = measureNumber;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to voice \"" << getVoiceName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
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
      endl <<
      "++++ createAMeasureAndAppendItToVoice() POUSSE, fCallsCounter: " << fCallsCounter << " ++++" <<
      endl;
    this->print (gLogStream);
    gLogStream <<
      endl;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "createAMeasureAndAppendItToVoice() 1");
  }
#endif

  ++gIndenter;

  // create the voice last segment if needed
  if (! fVoiceLastSegment) {
    createNewLastSegmentForVoice (
      inputLineNumber,
      "createAMeasureAndAppendItToVoice() 2");
  }

  // append a new measure with given number to voice last segment
  S_msrMeasure
    result =
      fVoiceLastSegment->
        createAMeasureAndAppendItToSegment (
          inputLineNumber,
          measureNumber,
          measureImplicitKind);

  // result is the new voice last appended measure
  fVoiceLastAppendedMeasure = result;

#ifdef TRACING_IS_ENABLED
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
  const string& currentMeasureNumber)
{
  if (fRegularVoiceHarmoniesVoiceForwardLink) {
    stringstream s;

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmonies voice for regular voice \"" <<
      getVoiceName () <<
      "\" with voice number " <<
      regularVoiceHarmoniesVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fRegularVoiceHarmoniesVoiceForwardLink =
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindHarmonies,
      regularVoiceHarmoniesVoiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
      fVoiceStaffUpLink);

  // register it in the staff
  fVoiceStaffUpLink->
    registerVoiceInStaff (
      inputLineNumber,
      fRegularVoiceHarmoniesVoiceForwardLink);

  // set backward link
  fRegularVoiceHarmoniesVoiceForwardLink->
    fHarmoniesVoiceRegularVoiceBackwardLink = this;

  return fRegularVoiceHarmoniesVoiceForwardLink;
}

S_msrVoice msrVoice::createRegularVoiceFiguredBassVoice (
  int           inputLineNumber,
  const string& currentMeasureNumber)
{
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" already has a figured bass voice";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice figured bass voice
  int regularVoiceFiguredBassVoiceNumber =
    msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + fVoiceNumber;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating figured bass voice for regular voice \"" <<
      getVoiceName () <<
      "\" with voice number " <<
      regularVoiceFiguredBassVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fRegularVoiceFiguredBassVoiceForwardLink =
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindFiguredBass,
      regularVoiceFiguredBassVoiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
      fVoiceStaffUpLink);

  // register it in the staff
  fVoiceStaffUpLink->
    registerVoiceInStaff (
      inputLineNumber,
      fRegularVoiceFiguredBassVoiceForwardLink);

  // set backward link
  fRegularVoiceFiguredBassVoiceForwardLink->
    fFiguredBassVoiceRegularVoiceBackwardLink = this;

  return fRegularVoiceFiguredBassVoiceForwardLink;
}

S_msrStanza msrVoice::addStanzaToVoiceByItsNumber (
  int           inputLineNumber,
  const string& stanzaNumber)
{
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    stringstream s;

    s <<
      "stanza " << stanzaNumber <<
      " already exists in voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;

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

void msrVoice::addStanzaToVoice (S_msrStanza stanza)
{
  // get stanza number
  string stanzaNumber =
    stanza->getStanzaNumber ();

  // register stanza in this voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

void msrVoice::addStanzaToVoiceWithoutCatchUp (S_msrStanza stanza)
{
  // get stanza number
  string stanzaNumber =
    stanza->getStanzaNumber ();

  // register stanza in this voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

S_msrStanza msrVoice::createStanzaInVoiceIfNotYetDone (
  int           inputLineNumber,
  const string& stanzaNumber,
  const string& stanzaName) // JMI
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLyrics ()) {
      gLogStream <<
        "Creating stanza" <<
        " number " << stanzaNumber <<
        ", name \"" << stanzaName << "\"" <<
        ", in voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        ", fVoiceStanzasMap.size () = " << fVoiceStanzasMap.size () <<
        endl;
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
  const string& stanzaNumber,
  const string& stanzaName) // JMI
{
  S_msrStanza stanza;

  // is stanzaNumber known in voice?
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    // yes, use it
    stanza =
      fVoiceStanzasMap [stanzaNumber];
  }

  else {
    stringstream s;

    gLogStream <<
      endl << endl << endl <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      endl;

    print (gLogStream);

    gLogStream <<
      "VOICEVOICEVOICEVOICEVOICEVOICEVOICEVOICE" <<
      endl << endl << endl <<
      endl;


    s <<
      "Stanza" <<
      " number " << stanzaNumber <<
      ", name \"" << stanzaName << "\"" <<
      ", not found in voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      ", fVoiceStanzasMap.size () = " << fVoiceStanzasMap.size () <<
      endl;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return stanza;
}

void msrVoice::setVoiceCurrentClef (S_msrClef clef)
{
  fVoiceCurrentClef = clef;

  if (! fVoiceFirstClef) {
    fVoiceFirstClef = fVoiceCurrentClef;
  }
};

void msrVoice::setVoiceCurrentKey (S_msrKey key)
{
  fVoiceCurrentKey = key;
};

void msrVoice::setVoiceCurrentTimeSignature (S_msrTimeSignature timeSignature)
{
  fVoiceCurrentTimeSignature = timeSignature;
};

void msrVoice::appendPrintLayoutToVoice (S_msrPrintLayout printLayout)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePrintLayouts ()) {
    gLogStream <<
      "Appending print layout '" << printLayout->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    appendPrintLayoutToSegment (printLayout);

  --gIndenter;
}

void msrVoice::appendClefToVoice (S_msrClef clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" << clef->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
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

void msrVoice::appendKeyToVoice (S_msrKey key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " << key->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // set voice current clef
  this->setVoiceCurrentKey (key);

  // append key to last segment
  fVoiceLastSegment->
    appendKeyToSegment (key);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    displayVoice ( // JMI TEMP
      key->getInputLineNumber (),
      "appendKeyToVoice()");
  }
#endif

  --gIndenter;
}

void msrVoice::appendTimeSignatureToVoice (S_msrTimeSignature timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" << timeSignature->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
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

void msrVoice::appendTimeSignatureToVoiceClone (S_msrTimeSignature timeSignature) // superflous ??? JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" << timeSignature->asString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
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
  const rational& positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos () || gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      positionInMeasure <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

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
    // get the segment's measures list
    const list<S_msrMeasure>&
      firstSegmentMeasuresList =
        fVoiceFirstSegment->
          getSegmentMeasuresList ();

    if (firstSegmentMeasuresList.size ()) {
      // get the segment's first measure
      S_msrMeasure
        firstMeasure =
          firstSegmentMeasuresList.front ();

      // get the first measure's elements list
      const list<S_msrMeasureElement>&
        firstMeasureElementsList =
          firstMeasure->
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
            stringstream s;

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

  return result;
}

void msrVoice::setVoiceShortestNoteDuration (
  const rational& duration)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note duration in voice \"" <<
        getVoiceName () <<
        "\" becomes " <<
        duration <<
        endl;
    }
#endif

  fVoiceShortestNoteDuration = duration;
}

void msrVoice::setVoiceShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note tuplet factor in part \"" <<
        getVoiceName () <<
        "\" becomes " <<
        noteTupletFactor <<
        endl;
    }
#endif

  fVoiceShortestNoteTupletFactor = noteTupletFactor;
}

void msrVoice::registerShortestNoteInVoiceIfRelevant (S_msrNote note)
{
  // is note the shortest one in this voice?
  rational
    noteSoundingWholeNotes =
      note->
        getNoteSoundingWholeNotes ();

      /* JMI
  rational
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
        fetchVoicePartUpLink ();

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

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note in voice \"" << getVoiceName () << "\"" <<
        " becomes " << note->asString () <<
        endl;
    }
#endif
  }

/* JMI
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }
    */
}

void msrVoice::registerNoteAsVoiceLastAppendedNote (S_msrNote note)
{
  fVoiceLastAppendedNote = note;

  // is note the shortest one in this voice?
  rational
    noteSoundingWholeNotes =
      note->
        getNoteSoundingWholeNotes (),
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
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

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
        stringstream s;

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

  --gIndenter;
}

void msrVoice::appendHarmonyToHarmoniesVoice (
  S_msrHarmony harmony)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    harmony->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to harmonies voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
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

void msrVoice::appendFiguredBassElementToFiguredBassVoice (
  S_msrFiguredBassElement figuredBassElement)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    figuredBassElement->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending figured bass element " << figuredBassElement->asString () <<
      " to figured bass voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append the figuredBassElement to the voice last segment
  fVoiceLastSegment->
    appendFiguredBassElementToSegment (figuredBassElement);

  // register harmony
  ++fVoiceActualFiguredBassElementsCounter;
  fMusicHasBeenInsertedInVoice = true;

  --gIndenter;
}

void msrVoice::appendHarmonyToVoiceClone (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " << harmony->asString () <<
      " to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
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
      break;

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        stringstream s;

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

void msrVoice::appendFiguredBassElementToVoice (
  S_msrFiguredBassElement figuredBassElement)
{
  int inputLineNumber =
    figuredBassElement->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass element " << figuredBassElement->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      appendFiguredBassElementToFiguredBassVoice (
        figuredBassElement);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        stringstream s;

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

  --gIndenter;
}

void msrVoice::appendFiguredBassElementToVoiceClone (
  S_msrFiguredBassElement figuredBassElement)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    figuredBassElement->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending figured bass element " << figuredBassElement->asString () <<
      " to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      fVoiceLastSegment->
        appendFiguredBassElementToSegmentClone (figuredBassElement);

      // register figured bass
      ++fVoiceActualFiguredBassElementsCounter;
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        stringstream s;

        s <<
          "cannot append a figured bass to " <<
          msrVoiceKindAsString (fVoiceKind) <<
          " voice clone \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          figuredBassElement->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::padUpToPositionInMeasureInVoice (
  int             inputLineNumber,
  const rational& wholeNotesPositionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Padding up to position in measure '" <<
      wholeNotesPositionInMeasure <<
      "' whole notes in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");

  // pad up the voice's last segment
  fVoiceLastSegment->
    padUpToPositionInMeasureInSegment (
      inputLineNumber,
      wholeNotesPositionInMeasure);

  // pad up the voice's stanzas // JMI ???
  if (fVoiceStanzasMap.size ()) {
    for (pair<string, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      stanza->
        padUpToCurrentMeasureWholeNotesDurationInStanza (
          inputLineNumber,
          wholeNotesPositionInMeasure);
    } // for
  }

  --gIndenter;
}

void msrVoice::backupByWholeNotesStepLengthInVoice (
  int             inputLineNumber,
  const rational& backupTargetMeasureElementPositionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Backup by a '" <<
      backupTargetMeasureElementPositionInMeasure <<
      "' whole notes step length in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");

  // pad up the voice's last segment
  fVoiceLastSegment->
    backupByWholeNotesStepLengthInSegment (
      inputLineNumber,
      backupTargetMeasureElementPositionInMeasure);

  --gIndenter;
}

void msrVoice::appendPaddingNoteToVoice (
  int             inputLineNumber,
  const rational& forwardStepLength)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to voice \"" <<
      getVoiceName () <<
      "\",line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // pad up the voice's last segment
  fVoiceLastSegment->
    appendPaddingNoteToSegment (
      inputLineNumber,
      forwardStepLength);

  // account for padding note's duration in staff
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      incrementPartCurrentPositionInMeasure (
        inputLineNumber,
        forwardStepLength);

  // pad up the voice's stanzas
  if (fVoiceStanzasMap.size ()) {
    for (pair<string, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      stanza->
        appendPaddingNoteToStanza (
          inputLineNumber,
          forwardStepLength);
    } // for
  }

  --gIndenter;
}

void msrVoice::appendTransposeToVoice (
  S_msrTranspose transpose)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Appending transpose '" <<
      transpose->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendTransposeToSegment (transpose);
}

void msrVoice::appendStaffDetailsToVoice (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
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

void msrVoice::appendTempoToVoice (S_msrTempo tempo)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Appending tempo " <<
      tempo->asShortString () <<
      " to voice \"" <<
      getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendTempoToSegment (tempo);
}

void msrVoice::appendOctaveShiftToVoice (
  S_msrOctaveShift octaveShift)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceOctaveShifts ()) {
    gLogStream <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendOctaveShiftToSegment (octaveShift);
}

void msrVoice::appendScordaturaToVoice (
  S_msrScordatura scordatura)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceScordaturas ()) {
    gLogStream <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendScordaturaToSegment (scordatura);
}

void msrVoice::appendAccordionRegistrationToVoice (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceAccordionRegistrations ()) {
    gLogStream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendAccordionRegistrationToSegment (
      accordionRegistration);
}

void msrVoice::appendHarpPedalsTuningToVoice (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarpPedals ()) {
    gLogStream <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendHarpPedalsTuningToSegment (
      harpPedalsTuning);
}

void msrVoice::appendRehearsalToVoice (S_msrRehearsal rehearsal)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRehearsals ()) {
    gLogStream <<
      "Appending rehearsal '" << rehearsal->getRehearsalText () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendRehearsalToSegment (rehearsal);
}

void msrVoice::appendVoiceStaffChangeToVoice (
  S_msrVoiceStaffChange voiceStaffChange)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    voiceStaffChange->getInputLineNumber ();
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "Appending voice staff change '" <<
      voiceStaffChange->asString () <<
      "' to voice " <<
      " \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // append voice staff change to voice's last segment
  fVoiceLastSegment->
    appendVoiceStaffChangeToSegment (
      voiceStaffChange);

  --gIndenter;
}

void msrVoice::appendNoteToVoice (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note '" <<
      endl;

    ++gIndenter;

    gLogStream <<
      note <<
        "' to voice \"" << getVoiceName () << "\"" <<
        ", line " << inputLineNumber <<
        endl;

    --gIndenter;
  }
#endif

  // fetch the part
  S_msrPart
    part =
      fetchVoicePartUpLink ();

  // fetch the part current position in measure
  rational
    partCurrentPositionInMeasure =
      part->
        getPartCurrentPositionInMeasure ();

  if (! fVoiceLastSegment) {
    stringstream s;

    s <<
      "fVoiceLastSegment is null in appendNoteToVoice()";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
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
      partCurrentPositionInMeasure);

  // is this note the shortest one in this voice?
  this->
    registerShortestNoteInVoiceIfRelevant (
      note);

  // register note as the last appended one into this voice
  fVoiceLastAppendedNote = note;

  // account for note's duration in staff
  part->
    incrementPartCurrentPositionInMeasure (
      inputLineNumber,
      note->getNoteSoundingWholeNotes ());

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNoteKind::k_NoNote:
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
  const list<S_msrHarmony>&
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
      // append the harmony to the part harmonies voice
      partHarmoniesVoice->
        appendHarmonyToVoice (
          harmony);
    } // for
  }

  // are there figured bass elements attached to this note?
  const list<S_msrFiguredBassElement>&
    noteFiguredBassElementsList =
      note->
        getNoteFiguredBassElementsList ();

  if (noteFiguredBassElementsList.size ()) {
    // get the current part's figured bass voice
    S_msrVoice
      partFiguredBassVoice =
        part->
          getPartFiguredBassVoice ();

    list<S_msrFiguredBassElement>::const_iterator i;
    for (S_msrFiguredBassElement figuredBassElement : noteFiguredBassElementsList) {
      // append the figured bass element to the part figured bass voice
      partFiguredBassVoice->
        appendFiguredBassElementToVoice (
          figuredBassElement);
    } // for
  }
}

void msrVoice::appendNoteToVoiceClone (S_msrNote note) {
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    note->getInputLineNumber ();
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Appending note '" <<
      note->asShortString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
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
    case msrNoteKind::k_NoNote:
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
  S_msrDoubleTremolo doubleTremolo)

{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Appending double tremolo '" <<
      doubleTremolo->asShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendDoubleTremoloToSegment (doubleTremolo);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendChordToVoice (S_msrChord chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending chord '" << chord->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  // append chord to voice last segment
  fVoiceLastSegment->
    appendChordToSegment (chord);

  // account for chord duration in the part current position in measure
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      incrementPartCurrentPositionInMeasure (
        chord->getInputLineNumber (),
        chord->getChordSoundingWholeNotes ());

  // get the chord's notes vector
  const vector<S_msrNote>&
    chordNotesVector =
      chord->
        getChordNotesVector ();

  unsigned int chordNotesVectorSize =
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

void msrVoice::appendTupletToVoice (S_msrTuplet tuplet)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Appending tuplet " << tuplet->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // append tuplet to voice last segment
  fVoiceLastSegment->
    appendTupletToSegment (tuplet);

  // account for tuplet duration in the part's current position in measure
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      incrementPartCurrentPositionInMeasure (
        tuplet->getInputLineNumber (),
        tuplet->getTupletSoundingWholeNotes ());

  --gIndenter;

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
  S_msrGraceNotesGroup graceNotesGroup)
{
  int inputLineNumber =
    graceNotesGroup->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Adding grace notes '" <<
      graceNotesGroup->asString () <<
      "' ahead of voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

/* JMI
  gLogStream <<
    endl <<
    "======================= prependGraceNotesToVoice" <<
    endl;
  this->print (gLogStream);
  gLogStream <<
    "=======================" <<
    endl << endl;
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

  // get the voice first note's chord upLink
  S_msrChord
    firstNoteDirectChordUpLink =
      voiceFirstNote->
        getNoteDirectChordUpLink ();

  if (firstNoteDirectChordUpLink) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first chord of voice \"" << getVoiceName () <<
        "\", i.e. '" <<
        firstNoteDirectChordUpLink->asShortString () <<
        "'" <<
        endl;
    }
#endif

/*
    firstNoteDirectChordUpLink->
      setChordGraceNotesGroupBefore (
        graceNotesGroup);
        */

    // create a grace notes group link
    S_msrChordGraceNotesGroupLink
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          graceNotesGroup->getInputLineNumber (),
          graceNotesGroup,
          firstNoteDirectChordUpLink);

    // register it in the chord
    firstNoteDirectChordUpLink->
      setChordGraceNotesGroupLinkBefore (
        inputLineNumber,
        chordChordGraceNotesGroupLink);

  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Attaching grace notes before '" <<
        graceNotesGroup->asString () <<
        "' to the first note of voice \"" << getVoiceName () <<
        "\", i.e. '" <<
        voiceFirstNote->asShortString () <<
        "'" <<
        endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Appending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::prependAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Prepending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceFirstSegment->
    prependAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}
*/

void msrVoice::appendSyllableToVoice (
  int           inputLineNumber,
  const string& stanzaNumber,
  const string& stanzaName,
  S_msrSyllable syllable)
{
  // append syllable to this voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending syllable '" <<
      syllable->asString () <<
      "' to voice \"" << fVoiceName << "\"" <<
      endl;
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

void msrVoice::appendBarCheckToVoice (S_msrBarCheck barCheck)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending bar check '" << barCheck->asString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendBarCheckToSegment (barCheck);
}

void msrVoice::appendBarNumberCheckToVoice (
  S_msrBarNumberCheck barNumberCheck)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending barnumber check '" <<
      barNumberCheck->asString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendBarNumberCheckToSegment (barNumberCheck);
}

void msrVoice::appendLineBreakToVoice (S_msrLineBreak lineBreak)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending line break '" << lineBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendLineBreakToSegment (lineBreak);

  // cascade this lineBreak to the voice stanzas if any
  if (fVoiceStanzasMap.size ()) {
    for (pair<string, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      stanza->appendLineBreakSyllableToStanza (
        lineBreak->getInputLineNumber (),
        fVoiceCurrentMeasurePuristNumber);
    } // for
  }
}

void msrVoice::appendPageBreakToVoice (S_msrPageBreak pageBreak)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Appending page break '" << pageBreak->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    appendPageBreakToSegment (pageBreak);
}

void msrVoice::prependOtherElementToVoice (S_msrMeasureElement elem) {
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Prepending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  fVoiceLastSegment->
    prependOtherElementToSegment (elem);
}

void msrVoice::appendOtherElementToVoice (S_msrMeasureElement elem) {
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
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

  const list<S_msrMeasure>&
    lastSegmentMeasuresList =
      fVoiceLastSegment->getSegmentMeasuresList ();

  if (lastSegmentMeasuresList.size ()) {
    result = lastSegmentMeasuresList.back ();
  }
  else {
    stringstream s;

    s <<
      "attempting to fetch voice last measure in an empty measures list";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

S_msrMeasureElement msrVoice::fetchVoiceLastElement (
  int inputLineNumber) const
{
  S_msrMeasureElement result;

  S_msrMeasure
    lastMeasure =
      fetchVoiceLastMeasure (inputLineNumber);

  const list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->getMeasureElementsList ();

  if (lastMeasureElementsList.size ()) {
    result = lastMeasureElementsList.back ();
  }

  else {
    stringstream s;

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
  S_msrRepeat   repeat,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Pushing repeat '" <<
      repeat->asShortString () <<
      "' onto the repeats stack in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fVoicePendingRepeatDescrsStack.push_front (
    msrRepeatDescr::create (
      inputLineNumber,
      repeat));

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    string
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
  S_msrRepeat   repeat,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    string
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
    stringstream s;

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Popping repeat '" <<
      repeat->asString () <<
      "' from the repeat stack in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // pop it from repeats stack
  fVoicePendingRepeatDescrsStack.pop_front ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    string
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
  const string& context)
{
  unsigned int repeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  gLogStream <<
    endl <<
    ">>++++++++++++++++ Displaying voice repeats stack " << context <<
    endl <<
    "The voice repeats stack contains " <<
    mfSingularOrPlural (repeatDescrsStackSize, "element", "elements") <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  if (repeatDescrsStackSize) {
    list<S_msrRepeatDescr>::const_iterator
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
        endl;

      ++gIndenter;
      (*i)->
        getRepeatDescrRepeat ()->
          printShort (gLogStream);
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLogStream << endl;
    } // for

    --gIndenter;
  }

  gLogStream <<
    " <<++++++++++++++++ " <<
    endl << endl;
}

void msrVoice::displayVoiceRepeatsStackSummary (
  int           inputLineNumber,
  const string& context)
{
  unsigned int repeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  gLogStream <<
    endl <<
    "The voice repeats stack contains " <<
    mfSingularOrPlural (repeatDescrsStackSize, "element", "elements") <<
    " - " << context <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  if (repeatDescrsStackSize) {
    list<S_msrRepeatDescr>::const_iterator
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
        endl;

      ++gIndenter;
      (*i)->
        getRepeatDescrRepeat ()->
          printShort (gLogStream);
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLogStream << endl;
    } // for

    gLogStream <<
      "===============" <<
      endl << endl;

    --gIndenter;
  }
}

void msrVoice::displayVoiceRepeatsStackAndVoice ( // JMI never used ???
  int           inputLineNumber,
  const string& context)
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
  const string& context)
{
  gLogStream <<
    endl <<
    ">>++++++++++++++++ Displaying voice measure repeats " << context <<
    endl <<
    "The current voice measures repeat contains " <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  ++gIndenter;

  if (fVoicePendingMeasureRepeat) {
    fVoicePendingMeasureRepeat->
      displayMeasureRepeat (
        inputLineNumber,
        context);
  }
  else {
    gLogStream <<
      "none" <<
      endl;
  }

  --gIndenter;

  gLogStream <<
    " <<++++++++++++++++ " <<
    endl << endl;
}

void msrVoice::displayVoiceMeasureRepeatAndVoice (
  int           inputLineNumber,
  const string& context)
{
  displayVoiceMeasureRepeat (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

void msrVoice::displayVoiceFullMeasureRests (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    ">>++++++++++++++++ voice full measure rests " << context <<
    endl <<
    "The current voice full measure rests contains " <<
    ", line " << inputLineNumber <<
    ":" <<
    endl;

  ++gIndenter;

  if (fVoicePendingFullMeasureRests) {
    fVoicePendingFullMeasureRests->
      displayFullMeasureRests (
        inputLineNumber,
        context);
  }
  else {
    gLogStream <<
      "none" <<
      endl;
  }

  --gIndenter;

  gLogStream <<
    " <<++++++++++++++++ " <<
    endl << endl;
}

void msrVoice::displayVoiceFullMeasureRestsAndVoice (
  int           inputLineNumber,
  const string& context)
{
  displayVoiceFullMeasureRests (
    inputLineNumber,
    context);

  displayVoice (
    inputLineNumber,
    context);
}

S_msrRepeat msrVoice::createARepeatAndStackIt (
  int           inputLineNumber,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat and stacking it in voice \"" <<
      getVoiceName () <<
      "\"" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "createARepeatAndStackIt() 2");
  }
#endif

  return result;
}

S_msrRepeat msrVoice::createARepeatCloneAndStackIt (
  int           inputLineNumber,
  S_msrRepeat   repeat,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat clone and stacking it in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      " (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
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
  S_msrRepeatCommonPart repeatCommonPart,
  const string&         context)
{
  // move the voice initial elements to the new repeat common part
#ifdef TRACING_IS_ENABLED
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
      endl;
  }
#endif

  for (
    list<S_msrVoiceElement>::iterator i = fVoiceInitialElementsList.begin ();
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
  S_msrRepeatCommonPart repeatCommonPart,
  const string&         context)
{
  // move the voice last segment to repeatCommonPart
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Moving the voice last segment ";

    if (fVoiceLastSegment) { // JMI should not be necessary?
      gLogStream <<
        fVoiceLastSegment->asShortString ();
    }
    else {
      gLogStream <<
        "null";
    }

    gLogStream <<
      " to repeat common part '" <<
      repeatCommonPart->asShortString () <<
      "' (" << context << ")" <<
      "' in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    string
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
    string
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    string
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
  S_msrRepeatCommonPart repeatCommonPart,
  const string&         context)
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
  S_msrRepeatEnding repeatEnding,
  const string&     context)
{
  // move the voice last segment to repeatEnding
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Moving the voice last segment to repeat ending '" <<
      repeatEnding->asShortString () <<
      "' in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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
  S_msrRepeat   repeat,
  const string& context)
{
  // append repeat to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat '" <<
      repeat->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
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

void msrVoice::appendFullMeasureRestsToInitialVoiceElements (
  int                   inputLineNumber,
  S_msrFullMeasureRests fullMeasureRests,
  const string&         context)
{
  // append fullMeasureRests to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Appending full measure rests '" <<
      fullMeasureRests->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fVoiceInitialElementsList.push_back (
    fullMeasureRests);
}

void msrVoice::appendMeasureRepeatToInitialVoiceElements (
  int                inputLineNumber,
  S_msrMeasureRepeat measureRepeat,
  const string&      context)
{
  // append measureRepeat to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fVoiceInitialElementsList.push_back (
    measureRepeat);
}

void msrVoice::appendVoiceLastSegmentToInitialVoiceElements (
  int           inputLineNumber,
  const string& context)
{
  // append segment to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending voice last segment '" <<
      fVoiceLastSegment->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fVoiceInitialElementsList.push_back (
    fVoiceLastSegment);
}

void msrVoice::moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
  int           inputLineNumber,
  const string& context)
{
  if (fVoiceLastSegment) {
    const list<S_msrMeasure>&
      segmentMeasuresList =
        fVoiceLastSegment->
          getSegmentMeasuresList ();

    if (segmentMeasuresList.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceSegments ()) {
        gLogStream <<
          "Moving voice last segment '" <<
          fVoiceLastSegment->asString () <<
          "' to the initial elements in voice \"" <<
          getVoiceName () <<
          "\" (" << context << ")" <<
          ", line " << inputLineNumber <<
          endl;
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

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceSegmentsDetails ()) {
        displayVoice (
          inputLineNumber,
          "moveVoiceLastSegmentToInitialVoiceElementsIfRelevant() 2");
      }
#endif
    }

    else {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceSegments ()) {
        gLogStream <<
          "Voice last segment '" <<
          fVoiceLastSegment->asString () <<
          "' contains no measure, not moved to the initial elements in voice \"" <<
          getVoiceName () <<
          "\" (" << context << ")" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif
    }
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Voice last segment '" <<
        fVoiceLastSegment->asString () <<
        "' is null, not moved to the initial elements in voice \"" <<
        getVoiceName () <<
        "\" (" << context << ")" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif
  }
}

void msrVoice::appendRepeatCloneToInitialVoiceElements (
  int           inputLineNumber,
  S_msrRepeat   repeatCLone,
  const string& context)
{
  // append repeatCLone to the list of initial elements
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat cLone '" <<
      repeatCLone->asString () <<
      "' to the initial elements in voice \"" <<
      getVoiceName () <<
      "\" (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fVoiceInitialElementsList.push_back (
    repeatCLone);
}

void msrVoice::handleVoiceLevelRepeatStart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat start in voice \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatStart() 1");
  }
#endif

  ++gIndenter;

  // is there a voice last segment?
  if (fVoiceLastSegment) {

    // fetch last segment's measures list
    list<S_msrMeasure>
      voiceLastSegmentMeasuresList =
        fVoiceLastSegment->
          getSegmentMeasuresList ();

    // are there measures in the voice last segment?
    if (voiceLastSegmentMeasuresList.size ()) {
      // yes

      // fetch last measure in the last segment
      S_msrMeasure
        lastMeasureInLastSegment =
          voiceLastSegmentMeasuresList.back ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
        lastMeasureInLastSegment->
          displayMeasure (
            inputLineNumber,
            "lastMeasureInLastSegment - handleVoiceLevelRepeatStart() 2");
      }
#endif

      // let's look at the last measure in detail

      // fetch its elements list
      const list<S_msrMeasureElement>&
        lastMeasureElementsList =
          lastMeasureInLastSegment->
            getMeasureElementsList ();

      if (lastMeasureElementsList.size ()) {
        // the last measure is not empty

        rational
          currentMeasureWholeNotesDuration =
            lastMeasureInLastSegment->getCurrentMeasureWholeNotesDuration ();
        rational
          fullMeasureWholeNotesDuration =
            lastMeasureInLastSegment->getFullMeasureWholeNotesDuration ();

        // is there a measure splitting?
        if ( // JMI better criterion???
          currentMeasureWholeNotesDuration.getNumerator () > 0
            &&
          currentMeasureWholeNotesDuration
            <
          fullMeasureWholeNotesDuration
        ) {
          // yes this measure is not yet complete and should be split
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceRepeats ()) {
            gLogStream <<
              "Splitting measure '" <<
              lastMeasureInLastSegment->asShortString () <<
              "' upon a repeat start in voice \"" <<
              getVoiceName () <<
              "\"" <<
              ", currentMeasureWholeNotesDuration: " <<
              currentMeasureWholeNotesDuration <<
              ", fullMeasureWholeNotesDuration: " <<
              fullMeasureWholeNotesDuration <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

/* JMI
          // finalize lastMeasureInLastSegment
          lastMeasureInLastSegment->
            finalizeMeasure (
              inputLineNumber,
              msrMeasure::kMeasureRepeatContextKindNone,
              "handleVoiceLevelRepeatStart() 3");
*/
          // append last segment to initial voice elements list
          appendVoiceLastSegmentToInitialVoiceElements (
            inputLineNumber,
            "handleVoiceLevelRepeatStart() 4");

          // create a new last segment for the voice
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceSegments ()) {
            gLogStream <<
              "Creating a new last segment for voice \"" <<
              fVoiceName << "\"" <<
              ", line " << inputLineNumber <<
              endl;
          }
#endif

          createNewLastSegmentForVoice (
            inputLineNumber,
            "handleVoiceLevelRepeatStart() 5");

          // create a new measure with the same number as the voice last measure
          // and append it to the voice,
          createAMeasureAndAppendItToVoice (
            inputLineNumber,
            lastMeasureInLastSegment->getMeasureElementMeasureNumber (),
            msrMeasureImplicitKind::kMeasureImplicitKindNo);

        /* JMI
          // set it as created before a repeat
          voiceLastMeasure->
            setMeasureCreatedForARepeatKind (
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
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Removing the last measure in voice \"" <<
            getVoiceName () <<
            "\"" <<
            " (voice level start)" <<
            ", line " << inputLineNumber <<
            endl;
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
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Creating a new last segment with the first common part measure for voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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
      msrRepeat::kRepeatExplicitStartYes);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a nested repeat start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatStartInVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatStartInVoice() 2");
  }
#endif
}

void msrVoice::handleVoiceLevelRepeatEndWithoutStart (
  int           inputLineNumber,
  const string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat end without start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStart() 1");
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the repeat
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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
      msrMeasure::kMeasureRepeatContextKindCommonPartLastMeasure,
      "handleVoiceLevelRepeatEndWithoutStart() 3"
      );
*/
  // append the voice last segment to the new repeat common part
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending the voice last segment in voice \"" <<
      getVoiceName () <<
      "\" to the new voice-level repeat common part" <<
      ", line " << inputLineNumber <<
      endl;
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
      msrRepeat::kRepeatBuildPhaseCompleted);

  // append newRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelRepeatEndWithoutStart() 6");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithoutStart() 7");
  }
#endif

  --gIndenter;
}

void msrVoice::handleVoiceLevelContainingRepeatEndWithoutStart (
  int           inputLineNumber,
  const string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level rcontaining epeat end without start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStart() 1");
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat end without a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the repeat
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending the voice last segment in voice \"" <<
      getVoiceName () <<
      "\" to the new voice-level repeat common part" <<
      ", line " << inputLineNumber <<
      endl;
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
      msrRepeat::kRepeatBuildPhaseCompleted);

  // append newRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    newRepeat,
    "handleVoiceLevelContainingRepeatEndWithoutStart() 6");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelContainingRepeatEndWithoutStart() 7");
  }
#endif

  --gIndenter;
}

void msrVoice::handleVoiceLevelRepeatEndWithStart (
  int           inputLineNumber,
  const string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat end with start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStart() 1");
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat end with a start is at the voice-level" <<
      ", line " << inputLineNumber <<
      endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Moving the voice last segment in voice \"" <<
      getVoiceName () <<
      "\" to the current voice-level repeat common part" <<
      " and forget about it" <<
      ", line " << inputLineNumber <<
      endl;
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
      msrRepeat::kRepeatBuildPhaseCompleted);

  // append currentRepeat to the list of initial elements
  appendRepeatToInitialVoiceElements (
    inputLineNumber,
    currentRepeat,
    "handleVoiceLevelRepeatEndWithStart() 4");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleVoiceLevelRepeatEndWithStart() 5");
  }
#endif

  --gIndenter;
}

void msrVoice::handleNestedRepeatEndInVoice (
  int           inputLineNumber,
  const string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a nested repeat end in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() 1");
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "This repeat is nested" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

// fetch the voice's last measure
  S_msrMeasure
    voiceLastMeasure =
      fetchVoiceLastMeasure (
        inputLineNumber);

  // is there a measure splitting?
  if (
    voiceLastMeasure->getCurrentMeasureWholeNotesDuration ()
      ==
    voiceLastMeasure->getFullMeasureWholeNotesDuration ()
  ) {
    // this measure is incomplete and should be split
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Splitting measure '" <<
        voiceLastMeasure->asShortString () <<
        "' upon a repeat end in voice \"" <<
        getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    // create a new measure with the same number as the voice last measure
    // and append it to the voice,
    createAMeasureAndAppendItToVoice (
      inputLineNumber,
      measureNumber,
      msrMeasureImplicitKind::kMeasureImplicitKindNo);

  /* JMI
    // set it as created after a repeat
    voiceLastMeasure->
      setMeasureCreatedForARepeatKind (
        msrMeasure::kMeasureCreatedForARepeatAfter);
        */
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndInVoice (
  int           inputLineNumber,
  const string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
                case msrRepeat::kRepeatBuildPhaseJustCreated:
                  {
                    stringstream s;

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

                case msrRepeat::kRepeatBuildPhaseInCommonPart:
                case msrRepeat::kRepeatBuildPhaseInEndings:
                  // this repeat is at the voice-level and has a start
                  // -------------------------------------
                  handleVoiceLevelRepeatEndWithStart (
                    inputLineNumber,
                    measureNumber,
                    repeatTimes);
                  break;

                case msrRepeat::kRepeatBuildPhaseCompleted:
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndInVoice() 2");
  }
#endif
}

void msrVoice::handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
  int         inputLineNumber,
  S_msrRepeat currentRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat ending start without explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
    stringstream s1;

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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Creating a repeat common part upon its end in voice \"" <<
        getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Fetching the last measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrMeasure
    lastMeasure =
      fVoiceLastSegment->
        fetchLastMeasureFromSegment (
          inputLineNumber,
          "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 1");

  // fetch lastMeasure's elements list
  const list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (true || ! lastMeasureElementsList.size ()) { // JMIJMIJMIJMIJMIJMI
    // it is empty, keep it for a new voice last segment

    // remove last measure
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Removing the last measure in voice \"" <<
        getVoiceName () <<
        "\"" <<
        " (voice level ending without explicit start)" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    S_msrMeasure
      dummyMeasure =
        fVoiceLastSegment->
          removeLastMeasureFromSegment (
            inputLineNumber,
            "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 2");

    // move voice last segment into the repeat common part
    stringstream s2;

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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart( 5)");
  }

  // set currentRepeat's build phase
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeat::kRepeatBuildPhaseInEndings);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a voice-level repeat ending start with explicit start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Fetching the last measure of the last segment in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", it is:" <<
      endl <<
      lastMeasure->asShortString () <<
      endl;
  }
#endif

  // fetch lastMeasure's elements list
  const list<S_msrMeasureElement>&
    lastMeasureElementsList =
      lastMeasure->
        getMeasureElementsList ();

  // let's look at lastMeasure in detail
  if (true || ! lastMeasureElementsList.size ()) { // JMIJMIJMIJMIJMIJMI
    // it is empty, keep it for a new voice last segment

    // remove last measure
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Removing the last measure in voice \"" <<
        getVoiceName () <<
        "\"" <<
        " (voice level ending with explicit start)" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    S_msrMeasure
      dummyMeasure =
        fVoiceLastSegment->
          removeLastMeasureFromSegment (
            inputLineNumber,
            "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 3");

    // move voice last segment into the repeat common part
    stringstream s2;

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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRepeats ()) {
      gLogStream <<
        "Creating a new last segment with the first ending measure for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegments ()) {
      gLogStream <<
        "Creating a new last segment for voice \"" <<
        fVoiceName << "\"" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    createNewLastSegmentForVoice (
      inputLineNumber,
      "handleVoiceLevelRepeatEndingStartWithoutExplicitStart() 6");
  }

  // set currentRepeat's build phase
  currentRepeat->
    setCurrentRepeatBuildPhaseKind (
      msrRepeat::kRepeatBuildPhaseInEndings);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {
          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceRepeats ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "nestContentsIntoNewRepeatInVoice() 1");
          }
#endif

          // move voice last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "nestContentsIntoNewRepeatInVoice() 1");

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a nested repeat ending start voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
 }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleNestedRepeatEndingStartInVoice() 2");
  }
#endif
}

void msrVoice::handleRepeatEndingStartInVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
              nullptr);
            break;

          case 1:
            {
              // fetch currentRepeat
              S_msrRepeat
                currentRepeat =
                  fVoicePendingRepeatDescrsStack.front ()->
                    getRepeatDescrRepeat ();

              switch (currentRepeat->getRepeatExplicitStartKind ()) {
                case msrRepeat::kRepeatExplicitStartNo:
                  // this the first ending of a voice-level repeat without a start
                  // -------------------------------------
                  handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
                    inputLineNumber,
                    currentRepeat);
                  break;

                case msrRepeat::kRepeatExplicitStartYes:
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
  const string& repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Handling a repeat ending upon its start in voice clone \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        // is there a current repeat?
        switch (fVoicePendingRepeatDescrsStack.size ()) {
          case 0:
            {
              stringstream s;

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
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceRepeats ()) {
                gLogStream <<
                  "Creating a " <<
                  msrRepeatEndingKindAsString (
                    repeatEndingKind) <<
                  " repeat ending in current repeat in voice clone \"" <<
                  fVoiceName <<
                  "\"" <<
                  endl;
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
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceRepeats ()) {
                gLogStream <<
                  "Appending a " <<
                  msrRepeatEndingKindAsString (
                    repeatEndingKind) <<
                  " repeat ending to current repeat in voice \"" <<
                  fVoiceName <<
                  "\"" <<
                  endl;
              }
#endif

#ifdef TRACING_IS_ENABLED
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

              // set currentRepeat's build phase
              currentRepeat->
                setCurrentRepeatBuildPhaseKind (
                  msrRepeat::kRepeatBuildPhaseInEndings);
            }
          break;

          default:
            {
              // the current repeat is nested

              // JMI ???

              // move voice last segment into the repeat common part
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceRepeats ()) {
                gLogStream <<
                  "Moving the voice last segment to the repeat common part in voice clone \"" <<
                  getVoiceName () <<
                  "\"" <<
                  ", line " << inputLineNumber <<
                  endl;
              }
#endif
            }
        } // switch

        --gIndenter;
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatEndingStartInVoiceClone() 2");
  }
#endif
}

void msrVoice::handleSegmentCloneEndInVoiceClone (
  int          inputLineNumber,
  S_msrSegment segmentClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Handling segment '" <<
      segmentClone->asShortString () <<
      "' in voice clone \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegmentsDetails ()) {
    displayVoice (
      inputLineNumber,
      "handleSegmentCloneEndInVoiceClone() 1");
  }
#endif

  // analyze segmentClone's context,
  // full measure rests and measure repeats first,
  // since they can be nested in repeats
  if (fVoicePendingFullMeasureRests) {
    // segmentClone belongs to a full measure rests

    // get fVoicePendingFullMeasureRests's contents
    S_msrFullMeasureRestsContents
      fullMeasureRestsContents =
        fVoicePendingFullMeasureRests->
          getFullMeasureRestsContents ();

    // set segmentClone as the contents's segment
    fullMeasureRestsContents->
      setFullMeasureRestsContentsSegment (
        inputLineNumber,
        segmentClone);
  }

  else if (fVoicePendingMeasureRepeat) {
    // segmentClone belongs to a measures repeat

    switch (
      fVoicePendingMeasureRepeat->getCurrentMeasureRepeatBuildPhaseKind ()
    ) {
      case msrMeasureRepeat::kMeasureRepeatBuildPhaseJustCreated:
        {
          stringstream s;

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

      case msrMeasureRepeat::kMeasureRepeatBuildPhaseInPattern:
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

      case msrMeasureRepeat::kMeasureRepeatBuildPhaseInReplicas:
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

      case msrMeasureRepeat::kMeasureRepeatBuildPhaseCompleted:
        {
          stringstream s;

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

#ifdef TRACING_IS_ENABLED
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
  const string& measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
          stringstream s;

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a measures repeat from its first measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

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
        getSegmentMeasuresList ().size (),
    availableMeasuresNumber =
      lastSegmentMeasuresNumber - 1;

  if (
    availableMeasuresNumber < measureRepeatMeasuresNumber) {
    stringstream s;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Removing the last " <<
      mfSingularOrPlural (
        measureRepeatMeasuresNumber, "measure", "measures") <<
      " (to be repeated) from voice \"" <<
      fVoiceName <<
      endl;
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
    stringstream s;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a measures repeat pattern in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a new last segment with the first replica measure for voice \"" <<
      fVoiceName << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  createNewLastSegmentFromItsFirstMeasureForVoice (
    inputLineNumber,
    firstReplicaMeasure,
    "createMeasureRepeatFromItsFirstMeasures() 2");

  // keep the measures repeat pending

  // print resulting voice contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "createMeasureRepeatFromItsFirstMeasures() 3");
  }
#endif
}

void msrVoice::appendFullMeasureRestsToVoice (
  int                   inputLineNumber,
  S_msrFullMeasureRests fullMeasureRests)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Appending full measure rests '" <<
      fullMeasureRests->asShortString () <<
      "' to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "appendFullMeasureRestsToVoice() 1");
  }
#endif

  // analyze this full measure rests's context
  switch (fVoicePendingRepeatDescrsStack.size ()) {
    case 0:
      // this full measure rests is at the voice-level
      // -------------------------------------
      appendFullMeasureRestsToVoiceElementsList (
        fullMeasureRests);
      break;

    default:
      // this full measure rests is inside a repeat
      // -------------------------------------
      S_msrRepeat
        currentRepeat =
          fVoicePendingRepeatDescrsStack.front ()->
            getRepeatDescrRepeat ();

      currentRepeat->
        appendFullMeasureRestsToRepeat (
          inputLineNumber,
          fullMeasureRests,
          "appendFullMeasureRestsToVoice() 2");
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "appendFullMeasureRestsToVoice() 3");
  }
#endif
}

void msrVoice::appendMeasureRepeatToVoice (
  int                 inputLineNumber,
  S_msrMeasureRepeat measureRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measureRepeat->asShortString () <<
      "' to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendMeasureRepeatToVoice() 3");
  }
#endif
}

void msrVoice::appendPendingMeasureRepeatToVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Appending pending measures repeat to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasureRepeatToVoice() 1");
  }
#endif

  // does the current measures repeat exist?
  if (! fVoicePendingMeasureRepeat) {
    stringstream s;

    s <<
      "attempting to append a pending measures repeat which doesn't exist";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // fetch the last segment's measure list
  list<S_msrMeasure>
    voiceLastSegmentMeasureList =
      fVoiceLastSegment->
        getSegmentMeasuresList ();

  // grab the just created last measure
  // in the last segment's measure list,
  // (i.e. the one containing:
  //   <measure-repeat type="stop"/>)
  // which is the next measure after the measures repeat
  if (! voiceLastSegmentMeasureList.size ()) {
    stringstream s;

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
      voiceLastSegmentMeasureList.back ();

// BOFBOFBOF JMI
  // remove the next measure from the last segment's measure list
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Removing last measure in last segment" <<
      "in voice \"" <<
      fVoiceName <<
      "\":" <<
      endl;
    ++gIndenter;
    gLogStream <<
      nextMeasureAfterMeasureRepeat;
    gLogStream << endl;
    --gIndenter;
}
#endif

  voiceLastSegmentMeasureList.pop_back ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "appendPendingMeasureRepeatToVoice() 2");
  }
#endif

  // create the measures repeat replicas contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a measures repeat replicas contents for voice \"" <<
      fVoiceName << "\" is:" <<
      endl;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Setting pending measures repeat replicas segment in voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fVoicePendingMeasureRepeat->
    setMeasureRepeatReplicas (
      measureRepeatReplicas);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Setting measures repeat segment to voice last segment for voice \"" <<
      fVoiceName << "\"" <<
      endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a new last segment with the AAA measures repeat next measure for voice \"" <<
      fVoiceName << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  createNewLastSegmentFromItsFirstMeasureForVoice (
    inputLineNumber,
    nextMeasureAfterMeasureRepeat,
    "appendPendingMeasureRepeatToVoice() 3");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Appending pending measures repeat to voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating measures repeat and appending it to voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

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
          stringstream s;

          s <<
            "attempting to create a measures repeat while another one is pending";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        // create the measures repeat
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Creating and appending a measures repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        fVoicePendingMeasureRepeat =
          msrMeasureRepeat::create (
            inputLineNumber,
            measureRepeatMeasuresNumber,
            measureRepeatSlashesNumber,
            this);

        // create a measures repeat pattern from current last segment
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Creating a measures repeat pattern from current last segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
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
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Setting repeat common part in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        fVoicePendingMeasureRepeat->
          setMeasureRepeatPattern (
            measureRepeatPattern);

        // append the measures repeat to the list of initial elements
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Appending measures repeat to voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
#endif

        // append pending measures repeat to the voice
        appendMeasureRepeatToVoice (
          inputLineNumber,
          fVoicePendingMeasureRepeat);
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating measures repeat and appending it to voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;

    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "createMeasureRepeatAndAppendItToVoiceClone() 2");
  }
#endif
}

void msrVoice::setVoiceContainsFullMeasureRests (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains full measure rests" <<
      endl;
  }
#endif

  fVoiceContainsFullMeasureRests = true;
}

void msrVoice::setVoiceContainsMeasureRepeats (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains measure repeats" <<
      endl;
  }
#endif

  fVoiceContainsMeasureRepeats = true;
}

void msrVoice::createFullMeasureRestsInVoice (
  int inputLineNumber,
  int fullMeasureRestsNumber)
{
  // create a full measure rests
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Creating multiple full measure rests in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", " <<
      mfSingularOrPlural (
        fullMeasureRestsNumber, "measure", "measures") <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "createFullMeasureRestsInVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        ++gIndenter;

        // grab the just created last measure from the voice,
        // (i.e. the one containing:
        //   <multiple-rest ... type="start">2</multiple-rest>)
        // which is the first rest measure
        S_msrMeasure
          firstRestMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // move the current voice last segment to the initial elements list
        moveVoiceLastSegmentToInitialVoiceElementsIfRelevant ( //JMI
          inputLineNumber,
          "createFullMeasureRestsInVoice() 2");

        // create the full measure rests
        if (fVoicePendingFullMeasureRests) {
          stringstream s;

          s <<
            "attempting to create a full measure rests while another one is pending";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

        fVoicePendingFullMeasureRests =
          msrFullMeasureRests::create (
            inputLineNumber,
            firstRestMeasure->getFullMeasureWholeNotesDuration (),
            fullMeasureRestsNumber,
            this);

         // remember fVoicePendingFullMeasureRests for later next measure number setting
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Registering full measure rests as waiting for its next measure number" <<
            ", fullMeasureRestsNumber = " <<
            fullMeasureRestsNumber <<
            " in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        fVoiceFullMeasureRestsWaitingForItsNextMeasureNumber =
          fVoicePendingFullMeasureRests;

        fVoiceRemainingFullMeasureRests =
          fullMeasureRestsNumber;

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Setting fVoiceRemainingFullMeasureRests to '" <<
            fVoiceRemainingFullMeasureRests <<
            "' in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        // create a new segment to collect the full measure rests,
        // containing the first, rest measure
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Creating a new last segment containing the first rest measure in voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        createNewLastSegmentFromItsFirstMeasureForVoice (
          inputLineNumber,
          firstRestMeasure,
          "createFullMeasureRestsInVoice() 3");

        // this voice contails full measure rests
        this->setVoiceContainsFullMeasureRests (
          inputLineNumber);

        // keep the full measure rests pending

        --gIndenter;
      }
      break;
  } // switch

  // print resulting voice contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "createFullMeasureRestsInVoice() 4");
  }
#endif
}

void msrVoice::addFullMeasureRestsToVoice (
  int           inputLineNumber,
  const string& previousMeasureNumber,
  int           fullMeasureRestsNumber)
{
  // create a full measure rests
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Adding multiple full measure rests in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", " <<
      mfSingularOrPlural (
        fullMeasureRestsNumber, "measure", "measures") <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "addFullMeasureRestsToVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        ++gIndenter;

        // move the current voice last segment to the initial elements list
        moveVoiceLastSegmentToInitialVoiceElementsIfRelevant ( //JMI
          inputLineNumber,
          "addFullMeasureRestsToVoice() 2");

        // create the full measure rests
        if (fVoicePendingFullMeasureRests) {
          stringstream s;

          s <<
            "attempting to create a full measure rests while another one is pending";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

/* JMI
        fVoicePendingFullMeasureRests =
          msrFullMeasureRests::create (
            inputLineNumber,
            firstRestMeasure->getFullMeasureWholeNotesDuration (),
            fullMeasureRestsNumber,
            this);
*/

         // remember fVoicePendingFullMeasureRests for later next measure number setting JMI ???
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Registering full measure rests as waiting for its next measure number" <<
            ", fullMeasureRestsNumber = " <<
            fullMeasureRestsNumber <<
            " in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        fVoiceFullMeasureRestsWaitingForItsNextMeasureNumber =
          fVoicePendingFullMeasureRests;

        fVoiceRemainingFullMeasureRests =
          fullMeasureRestsNumber;

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Setting fVoiceRemainingFullMeasureRests to '" <<
            fVoiceRemainingFullMeasureRests <<
            "' in voice \"" <<
            fVoiceName << "\"" <<
            endl;
        }
#endif

        // create a new segment to collect the full measure rests,
        // containing the first, rest measure
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Creating a new last segment containing the first rest measure in voice \"" <<
            fVoiceName << "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

/* JMI
        createNewLastSegmentFromItsFirstMeasureForVoice (
          inputLineNumber,
          firstRestMeasure,
          "addFullMeasureRestsToVoice() 3");
*/

        // this voice contails full measure rests
        this->setVoiceContainsFullMeasureRests (
          inputLineNumber);

        // keep the full measure rests pending

        --gIndenter;
      }
      break;
  } // switch

  // print resulting voice contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "addFullMeasureRestsToVoice() 4");
  }
#endif
}

void msrVoice::appendPendingFullMeasureRestsToVoice (
    int inputLineNumber)
{
  // a full measure rests is a voice element,
  // and can be voice-level as well as part of a repeat

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "appendPendingFullMeasureRestsToVoice() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // does the pending full measure rests exist?
        if (! fVoicePendingFullMeasureRests) {
          stringstream s;

          s <<
            "attempting to append a pending full measure rests which doesn't exist";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Appending pending full measure rests '" <<
            fVoicePendingFullMeasureRests->asShortString () <<
            "' to voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        // create the full measure rests contents
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Creating a full measure rests contents for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;
        }
#endif

        S_msrFullMeasureRestsContents
          fullMeasureRestsContents =
            msrFullMeasureRestsContents::create (
              inputLineNumber,
              fVoicePendingFullMeasureRests);

        // set voice last segment as the full measure rests contents segment
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Setting current last segment as full measure rests contents segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        fullMeasureRestsContents->
          setFullMeasureRestsContentsSegment (
            inputLineNumber,
            fVoiceLastSegment);

        // set fullMeasureRestsContents as the full measure rests contents
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Setting full measure rests contents in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
        }
#endif

        fVoicePendingFullMeasureRests->
          setFullMeasureRestsContents (
            fullMeasureRestsContents);

        // set pending full measure rests last measure purist number
        fVoicePendingFullMeasureRests->
          setFullMeasureRestsLastMeasurePuristMeasureNumber (
            inputLineNumber);

        // forget about this voice last segment
        fVoiceLastSegment = nullptr;

        // append pending full measure rests to the voice
        appendFullMeasureRestsToVoice (
          inputLineNumber,
          fVoicePendingFullMeasureRests);

        // forget about this pending full measure rests
        fVoicePendingFullMeasureRests = nullptr;
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "appendPendingFullMeasureRestsToVoice() 2");
  }
#endif
}

void msrVoice::handleFullMeasureRestsStartInVoiceClone (
  int               inputLineNumber,
  S_msrFullMeasureRests fullMeasureRests)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Handling full measure rests start in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "handleFullMeasureRestsStartInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:

      // is there a voice last segment?
      if (fVoiceLastSegment) {

        // are there measures in the voice last segment?
        if (fVoiceLastSegment->getSegmentMeasuresList ().size ()) {

          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

          // move voice last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "handleFullMeasureRestsStartInVoiceClone() 2");

          // forget about fVoicePendingFullMeasureRests
          fVoicePendingFullMeasureRests = nullptr;

          // create a new last segment containing a new measure for the voice
          createNewLastSegmentForVoice (
            inputLineNumber,
            "handleFullMeasureRestsStartInVoiceClone()");

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
            displayVoiceFullMeasureRestsAndVoice (
              inputLineNumber,
              "handleFullMeasureRestsStartInVoiceClone() 3");
          }
#endif
        }
      }

      // is there already a current full measure rests in this voice?
      if (fVoicePendingFullMeasureRests) {
        stringstream s;

        s <<
          "current voice full measure rests is not null when attempting to handle full measure rests start '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // create the full measure rests clone and register it
      fVoicePendingFullMeasureRests =
        fullMeasureRests->
          createFullMeasureRestsNewbornClone (
            this);

      // this voice contails full measure rests
      this->setVoiceContainsFullMeasureRests (
        inputLineNumber);

      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "handleFullMeasureRestsStartInVoiceClone() 4");
  }
#endif

  --gIndenter;
}

void msrVoice::handleFullMeasureRestsEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Handling full measure rests end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "handleFullMeasureRestsEndInVoiceClone() 1");
  }
#endif

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:

      // is there a current full measure rests in this voice?
      if (! fVoicePendingFullMeasureRests) {
        stringstream s;

        s <<
          "current voice full measure rests is null when attempting to handle full measure rests end '" <<
          "' in voice clone '" <<
          asShortString () <<
          "' ";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // set pending full measure rests last measure purist number
      fVoicePendingFullMeasureRests->
        setFullMeasureRestsLastMeasurePuristMeasureNumber (
          inputLineNumber);

      // append current voice rest measure to the initial voice elements list
      appendFullMeasureRestsToInitialVoiceElements (
        inputLineNumber,
        fVoicePendingFullMeasureRests,
        "handleFullMeasureRestsEndInVoiceClone() 2");

      // forget about fVoicePendingFullMeasureRests
      fVoicePendingFullMeasureRests = nullptr;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
        displayVoice (
          inputLineNumber,
          "handleFullMeasureRestsEndInVoiceClone() 3");
      }
#endif
    break;
  } // switch

  --gIndenter;
}

void msrVoice::handleFullMeasureRestsContentsStartInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Handling full measure rests contents start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "handleFullMeasureRestsContentsStartInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingFullMeasureRests) {
    stringstream s;

    s <<
      "current voice full measure rests is null when attempting to handle full measure rests contents start '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create fVoicePendingFullMeasureRests' rest contents
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Creating a full measure rests contents upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  S_msrFullMeasureRestsContents
    fullMeasureRestsContents =
      msrFullMeasureRestsContents::create (
        inputLineNumber,
        fVoicePendingFullMeasureRests);

  // register it in fVoicePendingFullMeasureRests
  fVoicePendingFullMeasureRests->
    setFullMeasureRestsContents (
      fullMeasureRestsContents);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "handleFullMeasureRestsContentsStartInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::handleFullMeasureRestsContentsEndInVoiceClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Handling full measure rests contents end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "handleFullMeasureRestsContentsEndInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingFullMeasureRests) {
    stringstream s;

    s <<
      "current voice full measure rests is null when attempting to handle full measure rests contents end '" <<
      "' in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // get fVoicePendingFullMeasureRests's contents
  S_msrFullMeasureRestsContents
    fullMeasureRestsContents =
      fVoicePendingFullMeasureRests->
        getFullMeasureRestsContents ();

  // set fVoiceLastSegment as fullMeasureRestsContents' segment
  fullMeasureRestsContents->
    setFullMeasureRestsContentsSegment (
      inputLineNumber,
      fVoiceLastSegment);

  // forget about fVoiceLastSegment
 // fVoiceLastSegment = nullptr;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    displayVoiceFullMeasureRestsAndVoice (
      inputLineNumber,
      "handleFullMeasureRestsContentsEndInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::appendFullMeasureRestsCloneToVoiceClone (
  int               inputLineNumber,
  S_msrFullMeasureRests fullMeasureRestsClone)
{
  ++gIndenter;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullMeasureRestsClone != nullptr,
    "fullMeasureRestsClone is null");

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          gLogStream <<
            "Appending full measure rests clone '" <<
            fullMeasureRestsClone->asString () <<
            "' to voice clone \"" <<
            getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            endl;

          displayVoiceFullMeasureRestsAndVoice (
            inputLineNumber,
            "appendFullMeasureRestsCloneToVoiceClone() 1");
        }
#endif

        // is full measure rests nested in a repeat?
        if (fVoicePendingRepeatDescrsStack.size ()) {
          // yes

          S_msrRepeat
            currentRepeat =
              fVoicePendingRepeatDescrsStack.front ()->
                getRepeatDescrRepeat ();

          // grab the full measure rests segment, i.e. the voice's last segment JMI ???
          S_msrSegment
            fullMeasureRestsSegment =
              fVoiceLastSegment;

          // append it to the current repeat's common part
          currentRepeat->
            getRepeatCommonPart ()->
              appendSegmentToRepeatCommonPart ( // NO !!!
                inputLineNumber,
                fullMeasureRestsSegment,
                "appendFullMeasureRestsCloneToVoiceClone() 2");

          // forget about this voice last segment
          fVoiceLastSegment = nullptr;
        }

        else {
          // no
          // JMI ???
        }

        // append the full measure rests clone to the voice
        appendFullMeasureRestsToVoice (
          inputLineNumber,
          fullMeasureRestsClone);

        // print resulting voice contents
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
          displayVoice (
            inputLineNumber,
            "appendFullMeasureRestsCloneToVoiceClone() 3");
        }
#endif
      }
      break;
  } // switch

  --gIndenter;
}

void msrVoice::appendRepeatCloneToVoiceClone (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat clone '" <<
      repeatCLone->asString () <<
      "' to voice clone \"" <<
      getVoiceName () <<  "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 1");
  }
#endif

  ++gIndenter;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeatCLone != nullptr,
    "repeatCLone is null");

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // pushing repeat clone as the (new) current repeat
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Pushing repeat clone as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatCloneToVoiceClone() 4");
  }
#endif

  --gIndenter;
}

void msrVoice::handleMeasureRepeatStartInVoiceClone (
  int                 inputLineNumber,
  S_msrMeasureRepeat measureRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
        stringstream s;

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
          msrMeasureRepeat::kMeasureRepeatBuildPhaseJustCreated);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
        displayVoiceMeasureRepeatAndVoice (
          inputLineNumber,
          "handleMeasureRepeatStartInVoiceClone() 2");
      }
#endif
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
        stringstream s;

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternStartInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasureRepeat) {
    stringstream s;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a measures repeat pattern upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatPatternEndInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasureRepeat) {
    stringstream s;

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasStartInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasureRepeat) {
    stringstream s;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating a measures repeat replicas upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasEndInVoiceClone() 1");
  }
#endif

  if (! fVoicePendingMeasureRepeat) {
    stringstream s;

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    displayVoiceMeasureRepeatAndVoice (
      inputLineNumber,
      "handleMeasureRepeatReplicasEndInVoiceClone() 2");
  }
#endif

  --gIndenter;
}

void msrVoice::appendMeasureRepeatCloneToVoiceClone (
  int                 inputLineNumber,
  S_msrMeasureRepeat measureRepeatClone)
{
  ++gIndenter;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatClone != nullptr,
    "measureRepeatClone is null");

  switch (fVoiceKind) { // superflous JMI ???
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Appending measures repeat clone '" <<
            measureRepeatClone->asString () <<
            "' to voice clone \"" <<
            getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            endl;

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
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
  const string& repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hooked repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoice() 1");
  }
#endif

  ++gIndenter;

/* JMI
        unsigned int voicerepeatDescrsStackSize =
          fVoicePendingRepeatDescrsStack.size ();
*/

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
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

#ifdef TRACING_IS_ENABLED
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
  const string& repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hookless repeat ending in voice \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoice() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending to current repeat in voice \"" <<
      fVoiceName <<
      "\"" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
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

#ifdef TRACING_IS_ENABLED
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

#ifdef TRACING_IS_ENABLED
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
  const string& repeatEndingNumber, // may be "1, 2"
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

#ifdef TRACING_IS_ENABLED
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat common part start in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartStartInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat common part upon its start in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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

#ifdef TRACING_IS_ENABLED
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat common part end in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleRepeatCommonPartEndInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

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

#ifdef TRACING_IS_ENABLED
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
  const string& repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hooked repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHookedRepeatEndingEndInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

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

#ifdef TRACING_IS_ENABLED
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
  const string& repeatEndingNumber) // may be "1, 2"
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hookless repeat ending in voice clone \"" <<
      getVoiceName () <<  "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "handleHooklessRepeatEndingEndInVoiceClone() 1");
  }
#endif

  ++gIndenter;

  if (! fVoicePendingRepeatDescrsStack.size ()) {
    stringstream s;

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

#ifdef TRACING_IS_ENABLED
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
  const string& repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
            repeatEndingKind)
{
#ifdef TRACING_IS_ENABLED
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayVoiceRepeatsStackSummary (
      inputLineNumber,
      "appendRepeatEndingToVoiceClone() 2");
  }
#endif
}

void msrVoice::handleRepeatStartInVoiceClone (
  int         inputLineNumber,
  S_msrRepeat repeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
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

        // fetch last segment's measures list
        list<S_msrMeasure>
          voiceLastSegmentMeasuresList =
            fVoiceLastSegment->
              getSegmentMeasuresList ();

        // are there measures in the voice last segment?
        if (voiceLastSegmentMeasuresList.size ()) {
          // finalize current measure in voice
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);

          // move current last segment to the list of initial elements
          moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
            inputLineNumber,
            "handleRepeatStartInVoiceClone() 2");

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceRepeats ()) {
            displayVoiceRepeatsStackSummary (
              inputLineNumber,
              "handleRepeatStartInVoiceClone() 3");
          }
#endif
        }
      }

      // create the repeat clone and stack it
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceRepeats ()) {
        gLogStream <<
          "Creating a repeat upon its start in voice clone \"" <<
          getVoiceName () <<
          "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      S_msrRepeat
        repeatClone =
          createARepeatCloneAndStackIt (
            inputLineNumber,
            repeat,
            "handleRepeatStartInVoiceClone() 4");

#ifdef TRACING_IS_ENABLED
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in voice clone \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
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

#ifdef TRACING_IS_ENABLED
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
              stringstream s;

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


#ifdef TRACING_IS_ENABLED
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
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
          gLogStream <<
            "Appending a measures repeat replica to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
#endif

        // fetch last measure's full measure whole notes
        /* JMI
        int fullMeasureWholeNotesDuration =
          fVoiceLastSegment->
            getSegmentMeasuresList ().back ()->
              getFullMeasureWholeNotesDuration ();
              */

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) { // JMI
          gLogStream <<
            endl <<
            "***********" <<
            endl << endl;
          print (gLogStream);
          gLogStream <<
            "***********" <<
            endl << endl;
        }
#endif

        // sanity check
        mfAssert (
          __FILE__, __LINE__,
          fVoicePendingMeasureRepeat != nullptr,
          "fVoicePendingMeasureRepeat is null");

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
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) { // JMI
          gLogStream <<
            "Setting the measures repeat replica to current measures repeat BBB in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
        }
#endif

        fVoicePendingMeasureRepeat->
          setMeasureRepeatReplicas (
            measureRepeatReplicas);
      }
      break;
  } // switch
}

void msrVoice::appendFullMeasureRestsToVoiceElementsList (
  S_msrFullMeasureRests fullMeasureRests)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Appending full measure rests '" <<
      fullMeasureRests->asString () <<
      "' to voice \"" <<
      fVoiceName <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullMeasureRests != nullptr,
    "fullMeasureRests is null");

  fVoiceInitialElementsList.push_back (fullMeasureRests);
}

void msrVoice::appendMeasureRepeatToVoiceElementsList (
  S_msrMeasureRepeat measureRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to voice \"" <<
      fVoiceName <<
      "\"," <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeat != nullptr,
    "measureRepeat is null");

  fVoiceInitialElementsList.push_back (measureRepeat);
}

void msrVoice:: appendRepeatEndingCloneToVoice ( // JMI
  S_msrRepeatEnding repeatEndingClone)
{
#ifdef TRACING_IS_ENABLED
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
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceRepeats ()) {
          gLogStream <<
            "Appending a " <<
            msrRepeatEndingKindAsString (
              repeatEndingClone->getRepeatEndingKind ()) <<
            " repeat ending clone to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;

          displayVoiceRepeatsStackSummary (
            inputLineNumber,
            "appendRepeatEndingCloneToVoice() 1");
        }
#endif

        // sanity check
        if (! fVoicePendingRepeatDescrsStack.size ()) {
          stringstream s;

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

        S_msrRepeat
          currentRepeat =
            fVoicePendingRepeatDescrsStack.front ()->
              getRepeatDescrRepeat ();

        currentRepeat->
          addRepeatEndingToRepeat (
            repeatEndingClone->getInputLineNumber (),
            repeatEndingClone);

#ifdef TRACING_IS_ENABLED
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

void msrVoice::prependBarLineToVoice (S_msrBarLine barLine)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Prepending barLine '" <<
      barLine->asString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
  }
#endif

  ++gIndenter;

  fVoiceLastSegment->
    prependBarLineToSegment (barLine);

  --gIndenter;
}

void msrVoice::appendBarLineToVoice (S_msrBarLine barLine)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " <<
      barLine->asString () <<
      " to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
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

void msrVoice::appendSegnoToVoice (S_msrSegno segno)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegnos ()) {
    gLogStream <<
      "Appending a segno to voice \"" << getVoiceName () << "\"" <<
      endl;
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

void msrVoice::appendCodaToVoice (S_msrCoda coda)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceCodas ()) {
    gLogStream <<
      "Appending a coda to voice \"" << getVoiceName () << "\"" <<
      ":" <<
      endl;
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

void msrVoice::appendEyeGlassesToVoice (S_msrEyeGlasses eyeGlasses)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceEyeGlasses ()) {
    gLogStream <<
      "Appending a eyeGlasses to voice \"" << getVoiceName () << "\"" <<
      endl;
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

void msrVoice::appendPedalToVoice (S_msrPedal pedal)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePedals ()) {
    gLogStream <<
      "Appending a pedal to voice \"" << getVoiceName () << "\"" <<
      endl;
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

void msrVoice::appendDampToVoice (S_msrDamp damp)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDamps ()) {
    gLogStream <<
      "Appending a damp to voice \"" << getVoiceName () << "\"" <<
      endl;
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

void msrVoice::appendDampAllToVoice (S_msrDampAll dampAll)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDampAlls ()) {
    gLogStream <<
      "Appending a damp all to voice \"" << getVoiceName () << "\"" <<
      endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Removing last note" <<
      " from voice " << getVoiceName () <<
      endl;
  }
#endif

  return
    fVoiceLastSegment->
      removeLastElementFromSegment (inputLineNumber);
}
*/

void msrVoice::removeNoteFromVoice (
  int       inputLineNumber,
  S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Removing note '" <<
      note->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // remove note from voice last segment
  fVoiceLastSegment->
    removeNoteFromSegment (
      inputLineNumber,
      note);

  // update the part current position in measure
  fVoiceStaffUpLink->
    getStaffPartUpLink ()->
      decrementPartCurrentPositionInMeasure (
        inputLineNumber,
        note->
          getNoteSoundingWholeNotes ());

  --gIndenter;
}

void msrVoice::removeElementFromVoice (
  int          inputLineNumber,
  S_msrElement element)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Removing element '" <<
      element->asShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Removing last measure from voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  ++gIndenter;

  // remove (new) last measure
  S_msrMeasure result =
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing last appended measure in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeLastAppendedMeasureInVoice() 1");
  }
#endif

  ++gIndenter;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVoiceLastAppendedMeasure != nullptr,
    "fVoiceLastAppendedMeasure is null");

  // finalize fVoiceLastAppendedMeasure
  msrMeasure::msrMeasureRepeatContextKind
    measureRepeatContextKind =
      msrMeasure::kMeasureRepeatContextKindNone;

  fVoiceLastAppendedMeasure->
    finalizeMeasure (
      inputLineNumber,
      measureRepeatContextKind,
      "finalizeLastAppendedMeasureInVoice() 2");

/* JMI NO
  // forget about fVoiceLastAppendedMeasure
  fVoiceLastAppendedMeasure = nullptr;
*/

  switch (fVoiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // append a measure end syllable to the voice stanzas if any
      if (fVoiceStanzasMap.size ()) {
        for (pair<string, S_msrStanza> thePair : fVoiceStanzasMap) {
          S_msrStanza stanza = thePair.second;

          stanza->
            appendMeasureEndSyllableToStanza (
              inputLineNumber);
        } // for
      }

// JMI
      // handle the harmonies voice if any
      if (fRegularVoiceHarmoniesVoiceForwardLink) {
        fRegularVoiceHarmoniesVoiceForwardLink->
          finalizeLastAppendedMeasureInVoice (
            inputLineNumber);
      }

      // handle the figured bass voice if any
      if (fRegularVoiceFiguredBassVoiceForwardLink) {
        fRegularVoiceFiguredBassVoiceForwardLink->
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

#ifdef TRACING_IS_ENABLED
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "Collecting measures from the initial elements into voice \"" <<
        getVoiceName () <<
        "s measures flat list" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif
  }

  // collect measures from the last segment if any
  if (fVoiceLastSegment) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "Collecting measures from the last segment into voice \"" <<
        getVoiceName () <<
        "s measures flat list" <<
        ", line " << inputLineNumber <<
        endl;
    }
#endif

    list<S_msrMeasure> lastSegmentMeasuresFlatList;

    const list<S_msrMeasure>&
      voiceLastSegmentMeasuresList =
        fVoiceLastSegment->
          getSegmentMeasuresList ();

    if (voiceLastSegmentMeasuresList.size ()) {
      list<S_msrMeasure>::const_iterator
        iBegin = voiceLastSegmentMeasuresList.begin (),
        iEnd   = voiceLastSegmentMeasuresList.end (),
        i      = iBegin;

      for ( ; ; ) {
        fVoiceMeasuresFlatList.push_back ((*i));
        if (++i == iEnd) break;
      } // for
    }
  }
}

void msrVoice::finalizeVoice (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeVoice() 1");
  }
#endif

  if (fVoiceHasBeenFinalized) {
    stringstream s;

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
      fetchVoicePartUpLink ();

  rational
    partShortestNoteDuration =
      voicePart->
        getPartShortestNoteDuration ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", fVoiceShortestNoteDuration: " << fVoiceShortestNoteDuration <<
      ", partShortestNoteDuration: " << partShortestNoteDuration <<
      endl;
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
    fVoiceLastSegment->getSegmentMeasuresList ().size () == 0
  ) {
    stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" is totally empty, no contents ever specified for it" <<
      endl;

    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  // are there pending repeats in the voice repeats stack???
  unsigned int voicePendingRepeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  if (voicePendingRepeatDescrsStackSize) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "finalizeVoice() 2");
      }
#endif

    stringstream s;

    s <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "There is", "There are") <<
      " " <<
      voicePendingRepeatDescrsStackSize <<
      " " <<
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    displayVoiceRepeatsStackFullMeasureRestsMeasureRepeatAndVoice (
      inputLineNumber,
      "finalizeVoice() 3");
  }
#endif

//
  // finalize the harmonies voice if any
  if (fRegularVoiceHarmoniesVoiceForwardLink) {
    fRegularVoiceHarmoniesVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }

  // finalize the figured bass voice if any
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    fRegularVoiceFiguredBassVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }
//
}

void msrVoice::finalizeVoiceAndAllItsMeasures (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    displayVoice (
      inputLineNumber,
      "finalizeVoice() 1");
  }
#endif

  if (fVoiceHasBeenFinalized) {
    stringstream s;

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
      fetchVoicePartUpLink ();

  rational
    partShortestNoteDuration =
      voicePart->
        getPartShortestNoteDuration ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> in voice \"" <<
      getVoiceName () <<
      "\"" <<
      ", fVoiceShortestNoteDuration: " << fVoiceShortestNoteDuration <<
      ", partShortestNoteDuration: " << partShortestNoteDuration <<
      endl;
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
    fVoiceLastSegment->getSegmentMeasuresList ().size () == 0
  ) {
    stringstream s;

    s <<
      "Voice \"" <<
      getVoiceName () <<
      "\" is totally empty, no contents ever specified for it" <<
      endl;

    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  // are there pending repeats in the voice repeats stack???
  unsigned int voicePendingRepeatDescrsStackSize =
    fVoicePendingRepeatDescrsStack.size ();

  if (voicePendingRepeatDescrsStackSize) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
        displayVoiceRepeatsStackSummary (
          inputLineNumber,
          "finalizeVoice() 2");
      }
#endif

    stringstream s;

    s <<
      mfSingularOrPluralWithoutNumber (
        voicePendingRepeatDescrsStackSize, "There is", "There are") <<
      " " <<
      voicePendingRepeatDescrsStackSize <<
      " " <<
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoicesDetails ()) {
    displayVoiceRepeatsStackFullMeasureRestsMeasureRepeatAndVoice (
      inputLineNumber,
      "finalizeVoice() 3");
  }
#endif

// JMI
  // finalize the harmonies voice if any
  if (fRegularVoiceHarmoniesVoiceForwardLink) {
    fRegularVoiceHarmoniesVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }

  // finalize the figured bass voice if any
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    fRegularVoiceFiguredBassVoiceForwardLink->finalizeVoice (
      inputLineNumber);
  }
//

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing all the measures of voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  for (
    list<S_msrMeasure>::const_iterator i = fVoiceMeasuresFlatList.begin ();
    i != fVoiceMeasuresFlatList.end ();
    ++i
  ) {
    S_msrMeasure measure = (*i);

    measure->
      finalizeMeasure (
        inputLineNumber,
        msrMeasure::kMeasureRepeatContextKindNone,
        "finalizeVoiceAndAllItsMeasures()");
  } // for
}

void msrVoice::checkBeamNumber (S_msrBeam beam, S_msrNote note)
{
  int inputLineNumber =
    beam->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Checking the order of the beams begins, continues and ends" <<
      " upon beam '" <<
      beam->asShortString () <<
      "' and note '" <<
      note->asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  msrBeamKind beamKind   = beam->getBeamKind ();
  int         beamNumber = beam->getBeamNumber ();

  unsigned int
    noteBeamNumbersStackSize =
      fVoiceBeamNumbersStack.size ();

  switch (beamKind) {
    case msrBeamKind::k_NoBeam:
      {
        stringstream s;

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
          stringstream s;

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
        stringstream s;

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
          stringstream s;

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
          stringstream s;

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
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoice::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoice::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrVoice::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrVoice::browseData ()" <<
      endl;
  }

  // browse the voice initial elements
  if (fVoiceInitialElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fVoiceInitialElementsList.begin ();
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
    for (pair<string, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

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
      endl;
  }
}

string msrVoice::voiceNumberAsString () const
{
  string result = to_string (fVoiceNumber);

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

string msrVoice::regularVoiceStaffSequentialNumberAsString () const
{
  string result;

  if (fRegularVoiceStaffSequentialNumber == msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER) {
    result += " (msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER)";
  }
  else {
    // nothing more
  }

  return result;
}

string msrVoice::asShortString () const
{
  stringstream s;

  s <<
    msrVoiceKindAsStringForPrint (fVoiceKind) <<
    " voice \"" << getVoiceName () <<
    "\", fVoiceNumber '" <<
    fVoiceNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msrVoice::asString () const
{
  stringstream s;

  s <<
    "Voice \"" << getVoiceName () << "\", " <<
    msrVoiceKindAsString (fVoiceKind) <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrVoice::displayVoice (
  int           inputLineNumber,
  const string& context) const
{
  gLogStream <<
    endl <<
    "*********>> Displaying voice \"" <<
    getVoiceName () <<
    "\" (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    endl << endl;
}

void msrVoice::displayVoiceRepeatsStackFullMeasureRestsMeasureRepeatAndVoice (
  int           inputLineNumber,
  const string& context)
{
  displayVoice (
    inputLineNumber,
    context);

  displayVoiceRepeatsStack (
    inputLineNumber,
    context);

  displayVoiceFullMeasureRests (
    inputLineNumber,
    context);

  displayVoiceMeasureRepeat (
    inputLineNumber,
    context);
}

void msrVoice::print (ostream& os) const
{
  os <<
    "[" <<
    msrVoiceKindAsStringForPrint (fVoiceKind) <<
    " voice \"" << getVoiceName () <<
    "\", fVoiceNumber '" <<
    fVoiceNumber <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os << left <<
    "(" <<
    mfSingularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualFiguredBassElementsCounter, "figured bass", "figured bass elements") <<
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
    endl;

  const int fieldWidth = 41;

  os << left <<
    setw (fieldWidth) <<
    "fVoiceStaffUpLink" << " : " <<
    fVoiceStaffUpLink->getStaffName () <<
    endl <<

    setw (fieldWidth) <<
    "fVoiceCurrentMeasureNumber" << " : \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fVoiceCurrentMeasureOrdinalNumber" << " : " <<
    fVoiceCurrentMeasureOrdinalNumber <<
    endl <<

    setw (fieldWidth) <<
    "fVoiceCurrentMeasurePuristNumber" << " : " <<
    fVoiceCurrentMeasurePuristNumber <<
    endl <<

    setw (fieldWidth) <<
    "fRegularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    endl;

#ifdef TRACING_IS_ENABLED
  // regular measure ends detection
  os << left <<
    setw (fieldWidth) <<
    "fWholeNotesSinceLastRegularMeasureEnd" << " : " <<
    fWholeNotesSinceLastRegularMeasureEnd <<
    endl;

  // incomplete measures after repeats detection
  os << left <<
    setw (fieldWidth) <<
    "fCurrentVoiceRepeatPhaseKind" << " : " <<
    voiceRepeatPhaseKindAsString (
      fCurrentVoiceRepeatPhaseKind) <<
    endl;

  // print the voice first clef, and the current clef, key and time signature
  os << left <<
    setw (fieldWidth) << "fVoiceFirstClef" << " : ";
  if (fVoiceFirstClef) {
    os <<
      fVoiceFirstClef;
  }
  else {
    os << "null" << endl;
  }
  os << left <<
    setw (fieldWidth) << "fVoiceCurrentClef" << " : ";
  if (fVoiceCurrentClef) {
    os <<
      fVoiceCurrentClef;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "fVoiceCurrentKey" << " : ";
  if (fVoiceCurrentKey) {
    os <<
      fVoiceCurrentKey;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) << "fVoiceCurrentTimeSignature" << " : ";
  if (fVoiceCurrentTimeSignature) {
    ++gIndenter;
    os <<
      endl <<
      fVoiceCurrentTimeSignature;
    --gIndenter;
  }
  else {
    os << "null" << endl;
  }
#endif

// JMI
  // print the regular voice harmonies voice name if any
  os << left <<
    setw (fieldWidth) << "fRegularVoiceHarmoniesVoiceForwardLink" << " : ";
  if (fRegularVoiceHarmoniesVoiceForwardLink) {
    os <<
      fRegularVoiceHarmoniesVoiceForwardLink->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os << endl;

  // print the figured bass voice name if any
  os << left <<
    setw (fieldWidth) << "fRegularVoiceFiguredBassVoiceForwardLink" << " : ";
  if (fRegularVoiceFiguredBassVoiceForwardLink) {
    os <<
      fRegularVoiceFiguredBassVoiceForwardLink->getVoiceName ();
  }
  else {
    os <<
      "none";
  }
  os << endl;
//

  os << left <<
    setw (fieldWidth) << "fVoiceShortestNoteDuration" << " : " <<
    fVoiceShortestNoteDuration <<
    endl <<
    setw (fieldWidth) << "fVoiceShortestNoteTupletFactor" << " : " <<
    endl;

  ++gIndenter;
  os <<
    fVoiceShortestNoteTupletFactor;
  --gIndenter;

  os << left <<
    setw (fieldWidth) << "fVoiceHasBeenFinalized" << " : " <<
    fVoiceHasBeenFinalized <<
    endl;

  os << left <<
    setw (fieldWidth) << "fCurrentPositionInVoice" << " : " <<
    fCurrentPositionInVoice <<
    endl <<
    setw (fieldWidth) << "fCurrentMomentInVoice" << " : " <<
    fCurrentMomentInVoice;

  os << left <<
    setw (fieldWidth) << "fMusicHasBeenInsertedInVoice" << " : " <<
    fMusicHasBeenInsertedInVoice <<
    endl <<
    setw (fieldWidth) << "fVoiceContainsFullMeasureRests" << " : " <<
    fVoiceContainsFullMeasureRests <<
    endl <<
    setw (fieldWidth) << "fVoiceContainsMeasureRepeats" << " : " <<
    fVoiceContainsMeasureRepeats <<
    endl;

  // print the voice first segment if any
  os <<
    setw (fieldWidth) << "fVoiceFirstSegment" << " : ";
  if (fVoiceFirstSegment) {
    os <<
      "'" <<
      fVoiceFirstSegment->getSegmentAbsoluteNumber () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os << endl;

  // print the voice last appended measure if any
  os <<
    setw (fieldWidth) << "fVoiceLastAppendedMeasure" << " : ";
  if (fVoiceLastAppendedMeasure) {
    os <<
      "'" <<
      fVoiceLastAppendedMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os << endl;

  // print the voice first measure if any
  os <<
    setw (fieldWidth) << "fVoiceFirstMeasure" << " : ";
  if (fVoiceFirstMeasure) {
    os <<
      "'" <<
      fVoiceFirstMeasure->asShortString () <<
      "'";
    }
  else {
    os <<
      "none";
  }
  os << endl;

  // print this voice's first non-grace note
  {
    S_msrNote
      voiceFirstNonGraceNote =
        this->
          fetchVoiceFirstNonGraceNote ();

    os <<
      setw (fieldWidth) <<
      "fVoiceFirstNonGraceNote" << " : ";
    if (voiceFirstNonGraceNote) {
      os <<
        voiceFirstNonGraceNote->asShortString ();
    }
    else {
      os <<
        "none";
    }
    os << endl;
  }

  // print the voice last appended note
  os <<
    setw (fieldWidth) <<
    "fVoiceLastAppendedNote" << " : ";
  if (fVoiceLastAppendedNote) {
    os << gTab <<
      fVoiceLastAppendedNote->asShortString ();
  }
  else {
    os <<
      "none" <<
      endl;
  }

  os << endl;

  // print the voice measures flat list
  unsigned int voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();

  os <<
    setw (fieldWidth) <<
    "fVoiceMeasuresFlatList" << " : ";
  if (voiceMeasuresFlatListSize) {
    os <<
      " : " <<
      mfSingularOrPlural (
        voiceMeasuresFlatListSize, "element", "elements");
  }
  else {
    os << "empty";
  }
  os << endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasure measure = (*i);

      // print the measure
#ifdef TRACING_IS_ENABLED
//      if (gGlobalTracingOahGroup->getTraceMeasures ()) { // CAFE JMI
        os << measure->asShortStringForMeasuresSlices ();
//      }
//      else {
//        os << measure->getMeasureElementMeasureNumber ();
 //     }
#else
      os << measure->getMeasureElementMeasureNumber ();
#endif

      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << endl;

    --gIndenter;
  }

  os << endl;

  // print the voice initial elements
  unsigned int voiceInitialElementsListSize =
    fVoiceInitialElementsList.size ();

  os <<
    setw (fieldWidth) <<
    "fVoiceInitialElementsList" << " : ";

  if (voiceInitialElementsListSize) {
    os <<
      voiceInitialElementsListSize << " elements" <<
      endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fVoiceInitialElementsList.begin (),
      iEnd   = fVoiceInitialElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  os << endl;

  // print the last segment
  os <<
    setw (fieldWidth) <<
    "fVoiceLastSegment" << " : ";

  if (fVoiceLastSegment) {
    ++gIndenter;
    os <<
      endl <<
      fVoiceLastSegment;
    --gIndenter;
  }
  else {
    os <<
      "null" <<
      endl;
  }

  os << endl;

  // print the stanzas if any
  os <<
    setw (fieldWidth) <<
    "fVoiceStanzasMap" << " : ";

  if (fVoiceStanzasMap.size ()) {
    os << endl;

    ++gIndenter;

    for (pair<string, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      os << stanza;
    } // for

    --gIndenter;
  }
  else {
    os <<
      "empty" <<
      endl;
  }

  --gIndenter;

  os << "]" << endl;
}

void msrVoice::printShort (ostream& os) const
{
  os <<
    "[" <<
    msrVoiceKindAsStringForPrint (fVoiceKind) <<
    " voice \"" << getVoiceName () <<
    "\", fVoiceNumber '" <<
    fVoiceNumber <<
    "'" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os << left <<
    "(" <<
    mfSingularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualFiguredBassElementsCounter, "figured bass", "figured bass elements") <<
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
    endl;

  const int fieldWidth = 41;

#ifdef TRACING_IS_ENABLED
  // print the voice measures flat list
  unsigned int voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();

  os <<
    setw (fieldWidth) <<
    "fVoiceMeasuresFlatList";
  if (voiceMeasuresFlatListSize) {
    os <<
      mfSingularOrPlural (
        voiceMeasuresFlatListSize, "element", "elements");
  }
  else {
    os <<
      " : " << "empty";
  }
  os << endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasure measure = (*i);

      // print the measure
      if (gGlobalTracingOahGroup->getTraceMeasures ()) {
        os <<
          measure->asShortStringForMeasuresSlices ();
      }
      else {
        os <<
          measure->getMeasureElementMeasureNumber ();
      }
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;

    --gIndenter;
  }
#endif

  // print the voice initial elements
  unsigned int voiceInitialElementsListSize =
    fVoiceInitialElementsList.size ();

  os <<
    endl <<
    setw (fieldWidth) <<
    "fVoiceInitialElementsList";
  if (voiceInitialElementsListSize) {
    os << " : " <<  voiceInitialElementsListSize << " elements";
  }
  else {
    os << " : " << "empty";
  }

  if (voiceInitialElementsListSize) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fVoiceInitialElementsList.begin (),
      iEnd   = fVoiceInitialElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  os << endl;

  /* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVoiceLastSegment != nullptr,
    "fVoiceLastSegment is null");
    */

  // print the last segment
  if (fVoiceLastSegment) {
    os <<
      endl <<
      setw (fieldWidth) <<
      "fVoiceLastSegment" <<
      endl;

    ++gIndenter;
    fVoiceLastSegment->printShort (os);
    os <<
      endl;
    --gIndenter;
  }
  else {
    os <<
      "*** voiceLastSegment is null ***" << // JMI
      endl;
  }
  os << endl;

  // print the stanzas if any
  if (fVoiceStanzasMap.size ()) {
    os <<
      "Stanzas:" <<
      endl;

    ++gIndenter;

    for (pair<string, S_msrStanza> thePair : fVoiceStanzasMap) {
      S_msrStanza stanza = thePair.second;

      stanza->printShort (os);
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << "]" << endl;
}

// void msrVoice::printSlices (ostream& os) const JMI
// {
//   os << "MSR voice slices";
//
//   os << endl << endl;
//
//   ++gIndenter;
//
//   if (fPartAllStavesList.size ()) {
//     list<S_msrStaff>::const_iterator
//       iBegin = fPartAllStavesList.begin (),
//       iEnd   = fPartAllStavesList.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       (*i)->
//         printSlices (os);
//       if (++i == iEnd) break;
//       os << endl;
//     } // for
//   }
//
//   --gIndenter;
// }

ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print (os);
  return os;
}


}
