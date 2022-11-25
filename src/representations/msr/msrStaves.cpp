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

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrBarLines.h"
#include "msrBreaks.h"
#include "msrRehearsalMarks.h"
#include "msrRehearsalMarks.h"
#include "msrTempos.h"

#include "oahOah.h"

#include "oahOah.h"
#include "msrOah.h"
#include "mxsr2msrOah.h"
#include "msr2msrOah.h"

#include "msrWae.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// constant
const int msrStaff::K_NO_STAFF_NUMBER = -777;

// global variable
int msrStaff::gStaffMaxRegularVoices = 4;

S_msrStaff msrStaff::create (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber,
  const S_msrPart& staffUpLinkToPart)
{
  msrStaff* o =
    new msrStaff (
      inputLineNumber,
      staffKind,
      staffNumber,
      staffUpLinkToPart);
  assert (o != nullptr);
  return o;
}

msrStaff::msrStaff (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber,
  const S_msrPart& staffUpLinkToPart)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    staffUpLinkToPart != nullptr,
    "staffUpLinkToPart is null");

  // set staff part upLink
  fStaffUpLinkToPart =
    staffUpLinkToPart;

  // set staff kind and number
  fStaffKind   = staffKind;
  fStaffNumber = staffNumber;

  // do other initializations
  initializeStaff ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Creating staff \"" << asString () <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif
}

void msrStaff::initializeStaff ()
{
  fStaffRegularVoicesCounter = 0;

  // set staff name
  switch (fStaffKind) {
    case msrStaffKind::kStaffKindRegular:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_Staff_" +
        mfInt2EnglishWord (fStaffNumber);
      break;

    case msrStaffKind::kStaffKindTablature:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_Tablature_" +
        mfInt2EnglishWord (fStaffNumber);
      break;

    case msrStaffKind::kStaffKindHarmonies:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_HARMONIES_Staff";
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_FIGURED_BASS_Staff";
      break;

    case msrStaffKind::kStaffKindDrum:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_DRUM_Staff";
      break;

    case msrStaffKind::kStaffKindRythmic:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_RYTHMIC_Staff";
      break;
  } // switch

  ++gIndenter;

  // check the staff number
  switch (fStaffKind) {
    case msrStaffKind::kStaffKindRegular:
      // the staff number should not be negative
      if (fStaffNumber < 0) {
        std::stringstream s;

        s <<
          "regular staff number " << fStaffNumber <<
          " is not positive";

        mfAssert (
          __FILE__, __LINE__,
          false,
          s.str ());
      }
      break;

    case msrStaffKind::kStaffKindTablature:
      break;

    case msrStaffKind::kStaffKindHarmonies:
      break;

    case msrStaffKind::kStaffKindFiguredBass:
    /* JMI
      if (fStaffNumber != msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER) {
        std::stringstream s;

        s <<
          "figured bass staff number " << fStaffNumber <<
          " is not equal to " << msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      */
      break;

    case msrStaffKind::kStaffKindDrum:
      break;

    case msrStaffKind::kStaffKindRythmic:
      break;
  } // switch

  // staff shortest note
  fStaffShortestNoteDuration =
    Rational (INT_MAX, 1);
  fStaffShortestNoteTupletFactor =
    Rational (1, 1);

  // get the initial staff details from the part if any
  S_msrStaffDetails
    partStaffDetails =
      fStaffUpLinkToPart->
        getCurrentPartStaffDetails ();

  if (partStaffDetails) {
    // append it to the staff
    appendStaffDetailsToStaff (partStaffDetails);
  }

  // get the initial clef from the part if any
  {
    S_msrClef
      clef =
        fStaffUpLinkToPart->
          getPartCurrentClef ();

    if (clef) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceClefs ()) {
        gLogStream <<
          "Appending part clef '" << clef->asString () <<
          "' as initial clef to staff \"" <<
          fStaffName <<
          "\" in part " <<
          fStaffUpLinkToPart->getPartCombinedName () <<
          std::endl;
      }
#endif

      appendClefToStaff (clef); // JMI
    }
  }

  // get the initial key from the part if any
  {
    //* JMI
    S_msrKey
      key =
        fStaffUpLinkToPart->
          getPartCurrentKey ();

    if (key) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceKeys ()) {
        gLogStream <<
          "Appending part key '" << key->asString () <<
          "' as initial key to staff \"" <<
          fStaffName <<
          "\" in part " <<
          fStaffUpLinkToPart->getPartCombinedName () <<
          std::endl;
      }
#endif

      appendKeyToStaff (key);
    }
  }

  // get the initial transposition from the part if any
  {
    S_msrTransposition
      transposition =
        fStaffUpLinkToPart->
          getPartCurrentTransposition ();

    if (transposition) {
#ifdef TRACING_IS_ENABLED
      if ( gGlobalTracingOahGroup->getTraceTranspositions ()) {
        gLogStream <<
          "Appending part transposition '" << transposition->asString () <<
          "' as initial transposition to staff \"" <<
          fStaffName <<
          "\" in part " <<
          fStaffUpLinkToPart->getPartCombinedName () <<
          std::endl;
      }
#endif

      fStaffCurrentTransposition = transposition;

      appendTranspositionToAllStaffVoices (transposition);
    }
  }

  // set staff instrument names default values // JMI
  fStaffInstrumentName =
    fStaffUpLinkToPart->
      getPartInstrumentName ();
  fStaffInstrumentAbbreviation =
    fStaffUpLinkToPart->
      getPartInstrumentAbbreviation ();

  // multiple full-bar rests
  fStaffContainsMultipleFullBarRests = false;

  --gIndenter;
}

msrStaff::~msrStaff ()
{}

S_msrStaff msrStaff::createStaffNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Creating a newborn clone of staff \"" <<
      fStaffName <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrStaff
    newbornClone =
      msrStaff::create (
        fInputLineNumber,
        fStaffKind,
        fStaffNumber,
        containingPart);

  newbornClone->fStaffName =
    fStaffName;

  newbornClone->fStaffNumber =
    fStaffNumber;

  newbornClone->fStaffInstrumentName =
    fStaffInstrumentName;

  newbornClone->fStaffInstrumentAbbreviation =
    fStaffInstrumentAbbreviation;

  return newbornClone;
}

S_msrPartGroup msrStaff::fetchStaffUpLinkToPartGroup () const
{
  S_msrPartGroup result;

  if (fStaffUpLinkToPart) {
    result =
      fStaffUpLinkToPart->
        getPartUpLinkToPartGroup ();
  }

  return result;
}

S_msrScore msrStaff::fetchStaffUpLinkToScore () const
{
  S_msrScore result;

  if (fStaffUpLinkToPart) {
    result =
      fStaffUpLinkToPart->
        fetchPartUpLinkToScore ();
  }

  return result;
}

void msrStaff::registerShortestNoteInStaffIfRelevant (const S_msrNote& note)
{
  // is note the shortest one in this staff?
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

  if (noteSoundingWholeNotes < fStaffShortestNoteDuration) {
    // set the staff shortest note duration
    this->
      setStaffShortestNoteDuration (
        noteSoundingWholeNotes);

    // cascade this new value to the staff's part
    fStaffUpLinkToPart ->
      registerShortestNoteInPartIfRelevant (
        note);

/* JMI
    // set the staff shortest note tuplet factor
    fStaffShortestNoteTupletFactor =
      note->
        getNoteTupletFactor ();

    // cascade this new value to the staff's part
    staffPartUplink ->
      setPartShortestNoteTupletFactor (
        fStaffShortestNoteTupletFactor);
*/

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note in staff \"" << fStaffName << "\"" <<
        " becomes " << note->asString () <<
        std::endl;
    }
#endif
  }

/* JMI
  if (noteDisplayWholeNotes < fStaffShortestNoteDuration) {
    fStaffShortestNoteDuration = noteDisplayWholeNotes;
  }
    */
}

void msrStaff::setStaffCurrentClef  (
  const S_msrClef& clef)
{
  fStaffCurrentClef = clef;
};

void msrStaff::setStaffCurrentKey (
  const S_msrKey& key)
{
  fStaffCurrentKey = key;
};

void msrStaff::setStaffCurrentTime (
  const S_msrTimeSignature& timeSignature){
  fStaffCurrentTimeSignature = time;
};

std::string msrStaff::staffNumberAsString () const
{
  std::string result = std::to_string (fStaffNumber);

  if (fStaffNumber == msrPart::K_PART_HARMONIES_STAFF_NUMBER) {
    result += " (msrPart::K_PART_HARMONIES_STAFF_NUMBER)";
  }
  else if (fStaffNumber == msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER) {
    result += " (msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER)";
  }
  else {
    // nothing more
  }

  return result;
}

/* KEEP JMI
const int msrStaff::getStaffNumberOfMusicVoices () const
{
  int result = 0;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      switch (voice->getVoiceKind ()) {
        case msrVoiceKind::kVoiceKindRegular:
          if (voice->getMusicHasBeenInsertedInVoice ()) {
            ++result;
          }
          break;

        case msrVoiceKind::kVoiceKindDynamics:
          break;

        case msrVoiceKind::kVoiceKindHarmonies: // JMI
          break;

        case msrVoiceKind::kVoiceKindFiguredBass: // JMI
          break;
      } // switch

  } // for

  return result;
}
*/

void msrStaff::createAMeasureAndAppendItToStaff (
  int           inputLineNumber,
  int           previousMeasureEndInputLineNumber,
  const std::string& measureNumber,
  msrMeasureImplicitKind
                measureImplicitKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to staff \"" << fStaffName << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  // cascade it to all voices
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    // sanity check
    mfAssert (
      __FILE__, __LINE__,
      voice != nullptr,
      "voice is null");

    voice->
      createAMeasureAndAppendItToVoice (
        inputLineNumber,
        previousMeasureEndInputLineNumber,
        measureNumber,
        measureImplicitKind);
  } // for
}

void msrStaff::setNextMeasureNumberInStaff (
  int           inputLineNumber,
  const std::string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in staff \"" << fStaffName << "\"" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // cascade it to all voices
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    // sanity check
    mfAssert (
      __FILE__, __LINE__,
      voice != nullptr,
      "voice is null");

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Propagating the setting of next measure number '" <<
        nextMeasureNumber <<
        "', line " << inputLineNumber <<
        ", in voice \"" << voice->getVoiceName () << "\"" <<
        std::endl;
    }
#endif

    voice->
      setNextMeasureNumberInVoice (
        inputLineNumber,
        nextMeasureNumber);
  } // for

  --gIndenter;
}

S_msrVoice msrStaff::createRegularVoiceInStaffByItsNumber (
  int           inputLineNumber,
  int           voiceNumber,
  const std::string& currentMeasureNumber)
{
  ++fStaffRegularVoicesCounter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating regular voice with number '" <<
      voiceNumber <<
      "' as regular voice number '" <<
      fStaffRegularVoicesCounter <<
      "' of staff \"" << fStaffName <<
      "\", line " << inputLineNumber <<
      "\", current measure number: " <<
      currentMeasureNumber <<
 // JMI     " in part " << fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  // are there too many regular voices in this staff?
  if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
    std::stringstream s;

    s <<
      "staff \"" << fStaffName <<
      "\" is already filled up with " <<
      msrStaff::gStaffMaxRegularVoices << " regular voices" <<
      std::endl <<
      ". voice number " <<
      voiceNumber <<
      " overflows it" <<
      std::endl <<
      ", fStaffRegularVoicesCounter = " <<
      fStaffRegularVoicesCounter <<
      ", msrStaff::gStaffMaxRegularVoices = " <<
      msrStaff::gStaffMaxRegularVoices <<
      std::endl;

      /* JMI ???
    musicxmlError (
// JMI    musicxmlWarning ( JMI
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
      */
  }

  // is this voice number already in the regular voices std::map?
  std::map<int, S_msrVoice>::const_iterator
    it =
      fStaffVoiceNumbersToRegularVoicesMap.find (voiceNumber);

  if (it != fStaffVoiceNumbersToRegularVoicesMap.end ()) {
    // yes
    S_msrVoice
      olderVoice = (*it).second;

    std::stringstream s;

    s <<
      "attempting to create a voice anew by number" <<
      ", voiceNumber: " << voiceNumber <<
      ", currentMeasureNumber: " << currentMeasureNumber <<
      " ' in staff \"" <<
      fStaffName <<
      "\", " <<
      olderVoice->asShortString () <<
      " already exists with this same voiceNumber" <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        inputLineNumber,
        msrVoiceKind::kVoiceKindRegular,
        voiceNumber,
        msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        this);

  // register the voice in staff by its number
  registerRegularVoiceByItsNumber (
    inputLineNumber,
    voice,
    voiceNumber);

  return voice;
}

/*
    case msrVoiceKind::kVoiceKindHarmonies:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Creating harmonies voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoiceKindAsString (voiceKind) <<
          "' in staff \"" << fStaffName <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffUpLinkToPart->getPartCombinedName () <<
          std::endl;
      }
#endif
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Creating figured bass voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoiceKindAsString (voiceKind) <<
          "' in staff \"" << fStaffName <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffUpLinkToPart->getPartCombinedName () <<
          std::endl;
      }
#endif
      break;
  } // switch

  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        inputLineNumber,
        voiceKind,
        voiceNumber,
        msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
        this);

  // take this new voice into account if relevant
  switch (voiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // register the voice in staff by its number
      registerRegularVoiceByItsNumber (
        inputLineNumber,
        voice,
        voiceNumber);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // register the voice in staff by its number
      registerPartLevelVoiceInStaff (
        inputLineNumber,
        voice);
      break;
  } // switch
*/

void msrStaff::setStaffShortestNoteDuration (
  const Rational& duration)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note duration in staff \"" <<
        fStaffName <<
        "\" becomes " <<
        duration <<
        std::endl;
    }
#endif

  fStaffShortestNoteDuration = duration;
}

void msrStaff::setStaffShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note tuplet factor in part \"" <<
        fStaffName <<
        "\" becomes " <<
        noteTupletFactor <<
        std::endl;
    }
#endif

  fStaffShortestNoteTupletFactor = noteTupletFactor;
}

void msrStaff::registerVoiceInStaffAllVoicesList (
  const S_msrVoice& voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" in all voices std::list of staff " << fStaffName <<
      std::endl;
  }
#endif

  ++gIndenter;

  // sanity check
  if (fStaffAllVoicesList.size ()) {
    for (S_msrVoice knownVoice : fStaffAllVoicesList) {
      if (voice == knownVoice) {
        std::stringstream s;

        s <<
          "Voice \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices std::list";

        gLogStream <<
          std::endl <<
          "---> knownVoice:" <<
          std::endl;

        ++gIndenter;
        gLogStream <<
          knownVoice <<
          std::endl << std::endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gGlobalServiceRunData->getInputSourceName (),
          voice->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }

      if (voice->getVoiceName () == knownVoice->getVoiceName ()) {
        std::stringstream s;

        s <<
          "A voice with the same name as \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices std::list";

        gLogStream <<
          std::endl <<
          "---> knownVoice:" <<
          std::endl;

        ++gIndenter;
        gLogStream <<
          knownVoice <<
          std::endl << std::endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gGlobalServiceRunData->getInputSourceName (),
          voice->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
    } // for
  }

  // register voice in this staff's 'all voices' std::list
  fStaffAllVoicesList.push_back (voice);

  // register it in the part uplink
  fStaffUpLinkToPart->
    registerVoiceInPartAllVoicesList (voice);

  --gIndenter;
}

void msrStaff::registerVoiceByItsNumber (
  int        inputLineNumber,
  const S_msrVoice& voice)
{
  int voiceNumber = voice->getVoiceNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice named \"" << voice->getVoiceName () <<
      "\", " <<
      voice->asShortString () <<
      ", by its number '" << voiceNumber <<
      "\" in staff " << fStaffName <<
      std::endl;
  }
#endif

  ++gIndenter;

  // register it in the 'all voices' std::list
  registerVoiceInStaffAllVoicesList (voice); // JMI v0.9.63 NASTY bug???

  // register voice in the 'numbers to all voices' std::map
  fStaffVoiceNumbersToAllVoicesMap [voiceNumber] = voice;

  // sort the all voices std::list if necessary
  switch (voice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // set regularVoice staff sequential number
      voice->
        setRegularVoiceStaffSequentialNumber (
          fStaffRegularVoicesCounter);

      // register it in 'regular voices' std::list
      fStaffRegularVoicesList.push_back (voice);

      // register voice in the 'numbers to regular voices' std::map
      fStaffVoiceNumbersToRegularVoicesMap [voiceNumber] = voice;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Sorting the voices in staff \"" <<
          fStaffName << "\"" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      // sort fStaffAllVoicesList, to have harmonies just before
      // the corresponding voice
      if (fStaffAllVoicesList.size ()) {
        fStaffAllVoicesList.sort (
          compareVoicesToHaveHarmoniesAboveCorrespondingVoice);
      }
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Sorting the voices in staff \"" <<
          fStaffName << "\"" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      // sort fStaffAllVoicesList, to have figured bass elements just after
      // the corresponding voice
      if (fStaffAllVoicesList.size ()) {
        fStaffAllVoicesList.sort (
          compareVoicesToHaveFiguredBassesBelowCorrespondingVoice);
      }
      break;
  } // switch

  --gIndenter;
}

void msrStaff::registerRegularVoiceByItsNumber (
  int        inputLineNumber,
  const S_msrVoice& regularVoice,
  int        voiceNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering regular voice " <<
      regularVoice->asShortString () <<
      " by it's number '" << voiceNumber <<
      "\" in staff " << fStaffName <<
      "'s regular voices std::list with sequential number '" <<
      fStaffRegularVoicesCounter <<
      "'" <<
      std::endl;
  }
#endif

  // sanity check
  if (fStaffAllVoicesList.size ()) {
    for (S_msrVoice knownVoice : fStaffAllVoicesList) {
      if (regularVoice == knownVoice) {
        std::stringstream s;

        s <<
          "Voice \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices std::list";

        gLogStream <<
          std::endl <<
          "---> knownVoice:" <<
          std::endl;

        ++gIndenter;
        gLogStream <<
          knownVoice <<
          std::endl << std::endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gGlobalServiceRunData->getInputSourceName (),
          regularVoice->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }

      if (regularVoice->getVoiceName () == knownVoice->getVoiceName ()) {
        std::stringstream s;

        s <<
          "A voice with the same name as \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices std::list";

        gLogStream <<
          std::endl <<
          "---> knownVoice:" <<
          std::endl;

        ++gIndenter;
        gLogStream <<
          knownVoice <<
          std::endl << std::endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gGlobalServiceRunData->getInputSourceName (),
          regularVoice->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
    } // for
  }

  ++gIndenter;

  registerVoiceByItsNumber (
    inputLineNumber,
    regularVoice);

  --gIndenter;
}

void msrStaff::registerHarmoniesVoiceByItsNumber (
  int        inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering harmonies voice " <<
      voice->asString () <<
     " by its number in staff \"" << fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // register the harmonies voice by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voice);
}

void msrStaff::registerFiguredBassVoiceByItsNumber (
  int        inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering figured bass voice " <<
      voice->asString () <<
     " by its number in staff \"" << fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // register the figured bass voice by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voice);
}

S_msrVoice msrStaff::fetchRegularVoiceFromStaffByItsNumber (
  int inputLineNumber,
  int voiceNumber)
{
  S_msrVoice result; // JMI avoid repetitive messages! v0.9.66

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Fetching voice number '" <<
      voiceNumber <<
     "' in staff \"" << fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // search std::list ??? JMI
  for (std::pair<int, S_msrVoice> thePair : fStaffVoiceNumbersToRegularVoicesMap) {
#ifdef TRACING_IS_ENABLED
    int        number = thePair.first;
#endif
    const S_msrVoice& voice  = thePair.second;

    if (
      voice->getVoiceNumber ()
        ==
      voiceNumber
    ) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Voice number '" << voiceNumber <<
          "' in staff \"" << fStaffName << "\"" <<
          " has staff relative number '" << number <<
          "'" <<
          std::endl;
      }
#endif

      result = voice;
      break;
    }
  } // for

  return result;
}

void msrStaff::assignSequentialNumbersToRegularVoicesInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Assigning sequential numbers to the regular voices in staff \"" <<
      fStaffName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // assign sequential numbers to the regular voices,
  // needed to know about voices 1, 2, 3 and 4
  fStaffRegularVoicesList.sort (
    compareVoicesByIncreasingNumber);

  if (fStaffRegularVoicesList.size ()) {
    int voiceSequentialCounter = 0;

    std::list<S_msrVoice>::const_iterator
      iBegin = fStaffRegularVoicesList.begin (),
      iEnd   = fStaffRegularVoicesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      if (i == iEnd) break;

      const S_msrVoice& voice = (*i);

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is null");

      ++voiceSequentialCounter;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Voice \"" <<
          voice->getVoiceName () <<
          "\" gets sequential number " <<
          voiceSequentialCounter <<
          " in staff \"" <<
          fStaffName <<
          "\"" <<
          std::endl;
      }
#endif

      voice->
        setRegularVoiceStaffSequentialNumber (
          voiceSequentialCounter);

      if (++i == iEnd) break;
    } // for
  }
}

S_msrVoice msrStaff::fetchFirstRegularVoiceFromStaff (
  int inputLineNumber)
{
  S_msrVoice result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Fetching first regular voice in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (! fStaffRegularVoicesList.size ()) {
    std::stringstream s;

    s <<
      "staff " << fStaffNumber <<
      " in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      " doesn't contain any regular voices, cannot fetch the first one" <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  else {
    result = fStaffRegularVoicesList.front ();

    // sanity check
    mfAssert (
      __FILE__, __LINE__,
      result->getRegularVoiceStaffSequentialNumber () == 1,
      "result->getRegularVoiceStaffSequentialNumber () is not equal to 1");

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceVoices ()) {
      gLogStream <<
        "The first regular voice in staff \"" <<
        fStaffName <<
        "\" is " <<
        result->asShortString () <<
        std::endl;
    }
#endif
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> result = \"" <<
      result->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  return result;
}

void msrStaff::registerVoiceInStaff (
  int        inputLineNumber,
  const S_msrVoice& voice)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voice != nullptr,
    "voice is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " <<
      fStaffRegularVoicesCounter <<
      " of staff \"" << fStaffName <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  ++gIndenter;

  // get voice kind
  msrVoiceKind voiceKind =
    voice->getVoiceKind ();

  // take this new voice into account if relevant
  switch (voiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // take that regular voice into account
      ++fStaffRegularVoicesCounter;

      // are there too many voices in this staff?
      if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
        std::stringstream s;

        s <<
          "staff \"" << fStaffName <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          std::endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          std::endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          std::endl;

          /* JMI ???
        musicxmlError (
    // JMI    musicxmlWarning ( JMI
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
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
      break;
  } // switch

  // register the voice by it's number
  int voiceNumber = voice->getVoiceNumber ();

  switch (voiceKind) { // JMI merge with previous switch?
    case msrVoiceKind::kVoiceKindRegular:
      // register regular voice in staff by its number
      registerRegularVoiceByItsNumber (
        inputLineNumber,
        voice,
        voiceNumber);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      // register harmonies voice in staff by its number
      registerHarmoniesVoiceByItsNumber (
        inputLineNumber,
        voice);
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      // register figured bass voice in staff by its number
      registerFiguredBassVoiceByItsNumber (
        inputLineNumber,
        voice);
      break;
  } // switch

  --gIndenter;
}

void msrStaff::registerPartLevelVoiceInStaff (
  int        inputLineNumber,
  const S_msrVoice& voice)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voice != nullptr,
    "voice is null");

  ++gIndenter;

  // get voice kind
  msrVoiceKind voiceKind =
    voice->getVoiceKind ();

  // take this new voice into account if relevant
  switch (voiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      // take that regular voice into account // JMI
      ++fStaffRegularVoicesCounter;

      // are there too many voices in this staff?
      if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
        std::stringstream s;

        s <<
          "staff \"" << fStaffName <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          std::endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          std::endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          std::endl;

          /* JMI ???
        musicxmlError (
    // JMI    musicxmlWarning ( JMI
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;
  } // switch

  // register voice in this staff
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as part level voice " <<
      fStaffRegularVoicesCounter <<
      " in staff \"" << fStaffName <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  // register it in staff by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voice);

  // register the voice in staff by it's number
  int voiceNumber = voice->getVoiceNumber ();

  switch (voiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // register regular voice in staff by its number
      registerRegularVoiceByItsNumber (
        inputLineNumber,
        voice,
        voiceNumber);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      // register harmonies voice in staff by its number
      registerHarmoniesVoiceByItsNumber (
        inputLineNumber,
        voice);
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      // register figured bass voice in staff by its number
      registerFiguredBassVoiceByItsNumber (
        inputLineNumber,
        voice);
      break;
  } // switch

  --gIndenter;
}

void msrStaff::registerVoiceInStaffClone (
  int        inputLineNumber,
  const S_msrVoice& voice)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voice != nullptr,
    "voice is null");

  ++gIndenter;

  // get voice kind
  msrVoiceKind
    voiceKind =
      voice->getVoiceKind ();

  // register voice in this staff
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " <<
      fStaffRegularVoicesCounter <<
      " of staff clone \"" << fStaffName <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  // register the voice in staff clone by it's number
  int voiceNumber = voice->getVoiceNumber ();

  switch (voiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // take that regular voice into account
      ++fStaffRegularVoicesCounter;

      // are there too many voices in this staff?
      if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
        std::stringstream s;

        s <<
          "staff clone \"" << fStaffName <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          std::endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          std::endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          std::endl;

          /* JMI ???
        musicxmlError (
    // JMI    musicxmlWarning ( JMI
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }

      // register regular voice in staff clone by its number
      registerRegularVoiceByItsNumber (
        inputLineNumber,
        voice,
        voiceNumber);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      // register harmonies voice in staff clone by its number
      registerHarmoniesVoiceByItsNumber (
        inputLineNumber,
        voice);
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      // register figured bass voice in staff clone by its number
      registerFiguredBassVoiceByItsNumber (
        inputLineNumber,
        voice);
      break;
  } // switch

  --gIndenter;
}

void msrStaff::appendClefToStaff  (
  const S_msrClef& clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" << clef->asString () <<
      "' to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append clef to the staff,
  // unless we should ignore redundant clefs
  // and a clef equal to the current clef is found
  Bool doAppendClefToStaff (true);

  if (fStaffCurrentClef) {
    if (
      gGlobalMxsr2msrOahGroup->getIgnoreRedundantClefs ()
        &&
      clef->isEqualTo (fStaffCurrentClef)
    ) {
      doAppendClefToStaff = false;
    }
  }

  if (doAppendClefToStaff) {
    // register clef as current staff clef
    fStaffCurrentClef = clef;

    // cascade clef to all voices
    for (const S_msrVoice& voice : fStaffAllVoicesList) {
      voice->
        appendClefToVoice (clef);
    } // for
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceClefs ()) {
      gLogStream <<
        "Clef '" <<
        clef->asString () <<
        "' ignored because it is already present in staff " <<
        fStaffName <<
        "\" in part " <<
        fStaffUpLinkToPart->getPartCombinedName () <<
        std::endl;
    }
#endif
  }

  --gIndenter;
}

void msrStaff::appendKeyToStaff (S_msrKey  key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " << key->asString () <<
      " to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append key to staff?
  Bool doAppendKeyToStaff (true);

  if (fStaffCurrentKey) {
    if (
      gGlobalMxsr2msrOahGroup->getIgnoreRedundantKeys ()
        &&
      fStaffCurrentKey->isEqualTo (key)
    ) {
      doAppendKeyToStaff = false;
    }

    else {
      if (key->isEqualTo (fStaffCurrentKey)) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceKeys ()) {
          gLogStream <<
            "Key '" <<
            key->asString () <<
            "' ignored because it is already present in staff " <<
            fStaffName <<
            "\" in part " <<
            fStaffUpLinkToPart->getPartCombinedName () <<
            std::endl;
        }
#endif

        doAppendKeyToStaff = false;
      }
    }
  }

  if (doAppendKeyToStaff) {
    // register key as current staff key
    fStaffCurrentKey = key;

    // cascade it to all voices
    for (const S_msrVoice& voice : fStaffAllVoicesList) {
      voice->
        appendKeyToVoice (key);
    } // for
  }

  --gIndenter;
}

void msrStaff::appendTimeSignatureToStaff (
  const S_msrTimeSignature& timeSignature){
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time '" << timeSignature->asString () <<
      "' to staff \"" <<
      fStaffName <<
      "\"" <<
      std::endl;
  }
#endif

  ++gIndenter;

  // append time to staff?
  Bool doAppendTimeToStaff (true);

  if (fStaffCurrentTimeSignature) {
    if (
      gGlobalMxsr2msrOahGroup->getIgnoreRedundantTimes ()
        &&
      fStaffCurrentTimeSignature->isEqualTo (timeSignature)
    ) {
      doAppendTimeToStaff = false;
    }

    else {
      if (timeSignature->isEqualTo (fStaffCurrentTimeSignature)) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
          gLogStream <<
            "Time '" <<
            timeSignature->asString () <<
            "' ignored because it is already present in staff " <<
            fStaffName <<
            "\" in part " <<
            fStaffUpLinkToPart->getPartCombinedName () <<
            std::endl;
        }
#endif

        doAppendTimeToStaff = false;
      }
    }
  }

  if (doAppendTimeToStaff) {
    // register timeSignature as current staff time signature
    fStaffCurrentTimeSignature = timeSignature;

    // cascade it to all voices
    for (const S_msrVoice& voice : fStaffAllVoicesList) {
      voice->
        appendTimeSignatureToVoice (timeSignature);
    } // for
  }

  --gIndenter;
}

void msrStaff::appendTimeSignatureToStaffClone (
  const S_msrTimeSignature& timeSignature){
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time '" << timeSignature->asString () <<
      "' to staff clone \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  ++gIndenter;

  // set staff time signature
  fStaffCurrentTimeSignature = timeSignature;

  // cascade it to all voices
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendTimeSignatureToVoiceClone (timeSignature);
  } // for

  --gIndenter;
}

void msrStaff::appendTempoToStaff (
  const S_msrTempo& tempo)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Appending tempo " << tempo->asString () <<
      " to staff " <<
      fStaffName <<
      std::endl;
  }
#endif

 // append tempo to registered staves
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendTempoToVoice (tempo);
  } // for
}

void msrStaff::appendRehearsalMarkToStaff (
  const S_msrRehearsalMark& rehearsalMark)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRehearsalMarks ()) {
    gLogStream <<
      "Appending rehearsal mark " << rehearsalMark->asString () <<
      " to staff " <<
      fStaffName <<
      std::endl;
  }
#endif

 // append rehearsal mark to registered staves
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendRehearsalMarkToVoice (rehearsalMark);
  } // for
}

void msrStaff::appendLineBreakToStaff  (
  const S_msrLineBreak& lineBreak)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
    gLogStream <<
      "Appending line break " << lineBreak->asString () <<
      " to staff " <<
      fStaffName <<
      std::endl;
  }
#endif

 // append line break to registered staves
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendLineBreakToVoice (lineBreak);
  } // for
}

void msrStaff::appendPageBreakToStaff (
  const S_msrPageBreak& pageBreak)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
    gLogStream <<
      "Appending page break " << pageBreak->asString () <<
      " to staff " <<
      fStaffName <<
      std::endl;
  }
#endif

 // append page break to registered staves
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendPageBreakToVoice (pageBreak);
  } // for
}

void msrStaff::insertHiddenMeasureAndBarLineInStaffClone (
  int             inputLineNumber,
  const Rational& measurePosition)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos () || gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      measurePosition <<
      "' in staff clone \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // cascade it to all voices
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      insertHiddenMeasureAndBarLineInVoiceClone (
        inputLineNumber,
        measurePosition);
  } // for

  --gIndenter;
}

/* JMI
void msrStaff::nestContentsIntoNewRepeatInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Nesting contents into new repeat in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      nestContentsIntoNewRepeatInVoice (
        inputLineNumber);
  } // for
}
*/

void msrStaff::handleRepeatStartInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      handleRepeatStartInVoice (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrStaff::handleRepeatEndInStaff (
  int           inputLineNumber,
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      handleRepeatEndInVoice (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  --gIndenter;
}

void msrStaff::handleRepeatEndingStartInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      handleRepeatEndingStartInVoice (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrStaff::handleRepeatEndingEndInStaff (
  int              inputLineNumber,
  const std::string&    repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
                    repeatEndingKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending end in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      handleRepeatEndingEndInVoice (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for

  --gIndenter;
}

/* JMI
void msrStaff::finalizeRepeatEndInStaff (
  int           inputLineNumber,
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Finalizing a repeat upon its end in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      finalizeRepeatEndInVoice (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  --gIndenter;
}
*/

void msrStaff::createMeasureRepeatFromItsFirstMeasuresInStaff (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a measures repeat from it's first measure in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      createMeasureRepeatFromItsFirstMeasures (
        inputLineNumber,
        measureRepeatMeasuresNumber,
        measureRepeatSlashes);
  } // for
}

void msrStaff::appendPendingMeasureRepeatToStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending the pending measures repeat to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendPendingMeasureRepeatToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::appendMultipleFullBarRestsToStaff (
  int inputLineNumber,
  int multipleFullBarRestsMeasuresNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending a multiple full-bar rest for " <<
      mfSingularOrPlural (
        multipleFullBarRestsMeasuresNumber, "measure", "measures") <<
      " to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fStaffContainsMultipleFullBarRests = true;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendMultipleFullBarRestsToVoice (
        inputLineNumber,
        multipleFullBarRestsMeasuresNumber);
  } // for
}

void msrStaff::replicateLastAppendedMeasureInStaff (
  int inputLineNumber,
  int replicatasNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Replicating last appended measure in staff \"" <<
      fStaffName <<
      "\"" <<
      std::endl;
  }
#endif

  // add multiple rest to all staves
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      replicateLastAppendedMeasureInVoice (
        inputLineNumber,
        replicatasNumber);
  } // for
}

void msrStaff::addEmptyMeasuresToStaff (
  int           inputLineNumber,
  const std::string& previousMeasureNumber,
  int           emptyMeasuresNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Adding " <<
      mfSingularOrPlural (
        emptyMeasuresNumber,
        "empty measure", "empty measures") <<
      " to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fStaffContainsMultipleFullBarRests = true;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      addEmptyMeasuresToVoice (
        inputLineNumber,
        previousMeasureNumber,
        emptyMeasuresNumber);
  } // for
}

void msrStaff::appendPendingMultipleFullBarRestsToStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending the pending multiple rest to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendPendingMultipleFullBarRestsToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::appendMultipleFullBarRestsCloneToStaff (
  int               inputLineNumber,
  const S_msrMultipleFullBarRests& multipleFullBarRests)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending multiple rest '" <<
      multipleFullBarRests->asString () <<
      "' to staff clone \"" <<
      fStaffName <<
      "\"" <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendMultipleFullBarRestsCloneToVoiceClone (
        inputLineNumber,
        multipleFullBarRests);
  } // for
}

void msrStaff::appendRepeatCloneToStaff (
  int         inputLineNumber,
  const S_msrRepeat& repeatCLone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat clone to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendRepeatCloneToVoiceClone (
        inputLineNumber, repeatCLone);
  } // for
}

void msrStaff::appendRepeatEndingCloneToStaff (
  const S_msrRepeatEnding& repeatEndingClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a repeat ending clone to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      ", line " << repeatEndingClone->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++gIndenter;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendRepeatEndingCloneToVoice (
        repeatEndingClone);
  } // for

  --gIndenter;
}

void msrStaff::appendBarLineToStaff (
  const S_msrBarLine& barLine)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " << barLine->asString () <<
      " to staff " <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  ++gIndenter;

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendBarLineToVoice (barLine);
  } // for

  --gIndenter;
}

void msrStaff::appendTranspositionToStaff (
  const S_msrTransposition& transposition)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Setting transposition '" <<
      transposition->asString () <<
      "' in staff " <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  // set staff transposition
  Bool doAppendTransposeToStaff (true);

  if (! fStaffCurrentTransposition) {
    doAppendTransposeToStaff = true; // JMI ???
  }

  else {
    if (transposition->isEqualTo (fStaffCurrentTransposition)) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
        gLogStream <<
          "Transpose '" <<
          transposition->asString () <<
          "' ignored because it is already present in staff " <<
          fStaffName <<
          "\" in part " <<
          fStaffUpLinkToPart->getPartCombinedName () <<
          std::endl;
      }
#endif

      doAppendTransposeToStaff = false;
    }
  }

  if (doAppendTransposeToStaff) {
    // register transposition as current staff transposition
    fStaffCurrentTransposition = transposition;

    // cascade it to all voices
    appendTranspositionToAllStaffVoices (transposition);
  }
}

void msrStaff::appendStaffDetailsToStaff (
  const S_msrStaffDetails& staffDetails)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    staffDetails != nullptr,
    "staffDetails is null");

  // register staff details in staff
  fCurrentStaffStaffDetails = staffDetails;

  // set staff kind accordingly if relevant
  switch (staffDetails->getStaffLinesNumber ()) {
    case 1:
      if (gGlobalMsrOahGroup->getCreateSingleLineStavesAsRythmic ()) {
        fStaffKind = msrStaffKind::kStaffKindRythmic;
      }
      else {
        fStaffKind = msrStaffKind::kStaffKindDrum;
      }
      break;
    default:
      ;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Setting staff kind to '" <<
      msrStaffKindAsString (fStaffKind) <<
      "' in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendStaffDetailsToVoice (staffDetails);
  } // for
}

void msrStaff::appendTranspositionToAllStaffVoices (
  const S_msrTransposition& transposition)
{
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendTranspositionToVoice (transposition);
  } // for
}

void msrStaff::appendScordaturaToStaff (
  const S_msrScordatura& scordatura)
{
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendScordaturaToVoice (
        scordatura);
  } // for
}

void msrStaff::appendAccordionRegistrationToStaff (
  const S_msrAccordionRegistration&
    accordionRegistration)
{
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendAccordionRegistrationToVoice (
        accordionRegistration);
  } // for
}

void msrStaff::appendHarpPedalsTuningToStaff (
  const S_msrHarpPedalsTuning& harpPedalsTuning)
{
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      appendHarpPedalsTuningToVoice (
        harpPedalsTuning);
  } // for
}

void msrStaff::finalizeLastAppendedMeasureInStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing last appended measure in staff \"" <<
      fStaffName <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // finalize all the voices in the staff
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    switch (voice->getVoiceKind ()) {
      case msrVoiceKind::kVoiceKindRegular:
      case msrVoiceKind::kVoiceKindDynamics:
      case msrVoiceKind::kVoiceKindHarmonies:
      case msrVoiceKind::kVoiceKindFiguredBass:
        {
          // handle the regular voice
          voice->
            finalizeLastAppendedMeasureInVoice (
              inputLineNumber);

/* JMI this will be done later
          // handle the harmonies voice if any
          S_msrVoice
            regularVoiceHarmoniesVoice =
              voice->
                getRegularVoiceForwardLinkToHarmoniesVoice ();

          if (regularVoiceHarmoniesVoice) {
            regularVoiceHarmoniesVoice->
              finalizeLastAppendedMeasureInVoice (
                inputLineNumber);
          }

          // handle the figured bass voice if any
          S_msrVoice
            regularVoiceFiguredBassVoice =
              voice->
                getFiguredBassVoiceRegularVoiceBackwardLink ();

          if (regularVoiceFiguredBassVoice) {
            regularVoiceFiguredBassVoice->
              finalizeLastAppendedMeasureInVoice (
                inputLineNumber);
          }
*/
        }
        break;

/* JMI
      case msrVoiceKind::kVoiceKindHarmonies:
      case msrVoiceKind::kVoiceKindFiguredBass:
        break;
        */
    } // switch
  } // for

  --gIndenter;
}

bool msrStaff::compareVoicesByIncreasingNumber (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  return
    first->getVoiceNumber ()
      <
    second->getVoiceNumber ();
}

bool msrStaff::compareVoicesToHaveHarmoniesAboveCorrespondingVoice (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  int
    firstVoiceNumber =
      first->getVoiceNumber (),
    secondVoiceNumber =
      second->getVoiceNumber ();

  if (firstVoiceNumber > msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER) {
    firstVoiceNumber -= msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER + 1;
  }
  if (secondVoiceNumber > msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER) {
    secondVoiceNumber -= msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER + 1;
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
            secondVoiceNumber - msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER
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
            firstVoiceNumber - msrVoice::K_VOICE_HARMONIES_VOICE_BASE_NUMBER
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

bool msrStaff::compareVoicesToHaveFiguredBassesBelowCorrespondingVoice (
  const S_msrVoice& first,
  const S_msrVoice& second)
{
  int
    firstVoiceNumber =
      first->getVoiceNumber (),
    secondVoiceNumber =
      second->getVoiceNumber ();

  if (firstVoiceNumber > msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) {
    firstVoiceNumber -= msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + 1;
  }
  if (secondVoiceNumber > msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER) {
    secondVoiceNumber -= msrVoice::K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER + 1;
  }

  bool result =
    firstVoiceNumber > secondVoiceNumber;

  return result;
}

void msrStaff::finalizeStaff (int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Finalizing staff \"" <<
      fStaffName << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // finalize the voices
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing the voices in staff \"" <<
      fStaffName << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    voice->
      finalizeVoice (
        inputLineNumber);
  } // for

  // collect the staff measures slices from the voices
  collectStaffMeasuresSlices (
    inputLineNumber);

  --gIndenter;
}

void msrStaff::collectStaffMeasuresIntoFlatListsVector (
  int inputLineNumber)
{
  // collect measures from the staff voices
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Collecting measures from the staff voices into staff \"" <<
      fStaffName <<
      "s measures flat std::list std::vector" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fStaffAllVoicesList) {
    S_msrVoice foo = voice; // ??? JMI v0.9.66
  } // for
}

void msrStaff::collectStaffMeasuresSlices (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    size_t
      staffVoiceNumbersToAllVoicesMapSize =
        fStaffVoiceNumbersToAllVoicesMap.size ();

    gLogStream <<
      "Collecting the measures slices of staff \"" <<
      fStaffName <<
      "\", " <<
      mfSingularOrPluralWithoutNumber (
        staffVoiceNumbersToAllVoicesMapSize, "there is", "there are") <<
      ' ' <<
      mfSingularOrPlural (
        staffVoiceNumbersToAllVoicesMapSize, "voice", "voices") <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fStaffMeasuresSlicesSequence == nullptr,
    "fStaffMeasuresSlicesSequence is not null");

  ++gIndenter;

  // create the staff measures slices sequence
  fStaffMeasuresSlicesSequence =
    msrMeasuresSlicesSequence::create (
      fStaffName); // origin

  // populate it
  for (const S_msrVoice& voice : fStaffAllVoicesList) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
      gLogStream <<
        "---> voice \"" <<
        voice->getVoiceName () <<
        "\":" <<
        std::endl;
    }
#endif

    ++gIndenter;

    // get the voice measures slices sequence
    S_msrMeasuresSlicesSequence
      voiceMeasuresSlicesSequence =
        voice->
          getVoiceMeasuresSlicesSequence ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
      gLogStream <<
        "---> voiceMeasuresSlicesSequence";

      if (voiceMeasuresSlicesSequence) {
        gLogStream <<
          std::endl;
        ++gIndenter;
        gLogStream <<
          voiceMeasuresSlicesSequence;
        --gIndenter;
      }
      else {
        gLogStream << " : [NONE]" << std::endl;
      }
    }
#endif

    // merge it with the voice measures slices sequence
    if (voiceMeasuresSlicesSequence) { // JMI
      fStaffMeasuresSlicesSequence =
        fStaffMeasuresSlicesSequence->
          mergeWithMeasuresSlicesSequence (
            inputLineNumber,
            fStaffName,
            voiceMeasuresSlicesSequence);
    }

    // identify the solo notes and rests in the staff
    fStaffMeasuresSlicesSequence->
      identifySoloNotesAndRests ();

    --gIndenter;
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "---> staffMeasuresSlicesSequence:" <<
      std::endl;

    ++gIndenter;
    gLogStream <<
      fStaffMeasuresSlicesSequence;
    --gIndenter;
  }
#endif

  --gIndenter;
}

void msrStaff::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaff::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaff::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaff::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaff::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaff::browseData ()" <<
      std::endl;
  }

/*
  if (fStaffTuningsList.size ()) {
    for (
      std::list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin ();
      i != fStaffTuningsList.end ();
      ++i
  ) {
      // browse the voice
      msrBrowser<msrStaffTuning> browser (v);
      browser.browse (*(*i));
    } // for
 //   --g Indenter;
  }
*/

/* JMI may be useful???
  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    for (const S_msrVoice& voice : fStaffAllVoicesList) {
        msrBrowser<msrVoice> browser (v);
        browser.browse (*((*i).second));
    } // for
  }
  */

#ifdef TRACING_IS_ENABLED // JMI
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) { // JMI TEMP
    gLogStream <<
      "++++++++ fStaffAllVoicesList.size(): " <<
      fStaffAllVoicesList.size () <<
      std::endl;

    if (fStaffAllVoicesList.size ()) {
      for (const S_msrVoice& voice : fStaffAllVoicesList) {
        gLogStream <<
          std::endl <<
          "+++++++++ voice: ++++++++" <<
          " \"" << voice->getVoiceName () << "\"" <<
          std::endl;
      } // for
    }
  }
#endif

  if (fStaffAllVoicesList.size ()) {
    for (const S_msrVoice& voice : fStaffAllVoicesList) {
      // get the  set of voices to ignore
      S_oahStringSetElementAtom
        ignoreMsrVoicesSetAtom =
          gGlobalMsr2msrOahGroup->
            getIgnoreMsrVoicesSetAtom ();;

      Bool
        ignoreMsrVoicesSetIsSetByAnOption =
          ignoreMsrVoicesSetAtom->
            getSetByAnOption ();;

      // get the  set of voices to keep
      S_oahStringSetElementAtom
        keepMsrVoicesSetAtom =
          gGlobalMsr2msrOahGroup->
            getKeepMsrVoicesSetAtom ();;

      Bool
        keepMsrVoicesSetIsSetByAnOption =
          keepMsrVoicesSetAtom->
            getSetByAnOption ();;

      // JMI this should be done in mxsr2msrOahGroup::checkGroupOptionsConsistency () v0.9.62
      if (ignoreMsrVoicesSetIsSetByAnOption && keepMsrVoicesSetIsSetByAnOption) {
        std::stringstream s;

        s <<
          "Options " <<
          ignoreMsrVoicesSetAtom->fetchNamesBetweenQuotes () <<
          " and " <<
          keepMsrVoicesSetAtom->fetchNamesBetweenQuotes () <<
          " are incompatible" <<
          std::endl;

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          voice->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }

      const std::set<std::string>&
        ignoreMsrVoicesSet =
          gGlobalMsr2msrOahGroup->
            getIgnoreMsrVoicesSet ();

#ifdef TRACING_IS_ENABLED // JMI
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        mfDisplayStringSet (
          "ignoreMsrVoicesSet",
          ignoreMsrVoicesSet,
          gLogStream);
      }
#endif

      const std::set<std::string>&
        keepMsrVoicesSet =
          gGlobalMsr2msrOahGroup->
            getKeepMsrVoicesSet ();

#ifdef TRACING_IS_ENABLED // JMI
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        mfDisplayStringSet (
          "keepMsrVoicesSet",
          keepMsrVoicesSet,
          gLogStream);
      }
#endif

      // a voice is to be browsed by default
      Bool voiceIsToBeBrowsed (true);

      std::string
        voiceName =
          voice->
            getVoiceName ();

      // is voiceName in the ignore voices set?
      if (ignoreMsrVoicesSetIsSetByAnOption) {
        // a voice is to be browsed by default
        if (
          mfStringIsInStringSet (
            voiceName,
            ignoreMsrVoicesSet)
        ) {
          voiceIsToBeBrowsed = false;
        }
      }

      if (keepMsrVoicesSetIsSetByAnOption) {
        // a voice is NOT to be browsed by default
        if (
          !
            mfStringIsInStringSet (
              voiceName,
              keepMsrVoicesSet)
        ) {
          voiceIsToBeBrowsed = false;
        }
      }

      // now take that into account
      if (voiceIsToBeBrowsed) {
        msrBrowser<msrVoice> browser (v);
        browser.browse (*voice);
      }
      else {
#ifdef TRACING_IS_ENABLED // JMI
        if (gGlobalTracingOahGroup->getTraceVoices ()) {
          gLogStream <<
            "Ignoring voice \"" <<
            voiceName <<
            "\"" <<
            std::endl;
        }
#endif
      }
    } // for
  }

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrStaff::browseData ()" <<
      std::endl;
  }
}

std::string msrStaff::asString () const
{
  std::stringstream s;

  s <<
    "[Staff \"" << fStaffName <<
    "\", fStaffKind: " <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    "\", fStaffNumber: " <<
    fStaffNumber <<
    ", fStaffAllVoicesList.size(): " << fStaffAllVoicesList.size () <<
    ", fStaffRegularVoicesCounter: " << fStaffRegularVoicesCounter <<
    ']' <<
    std::endl;

  return s.str ();
}

void msrStaff::print (std::ostream& os) const
{
  os <<
    "[Staff \"" << fStaffName <<
    "\", fStaffKind: " <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    "\", fStaffNumber: " <<
    fStaffNumber <<
    ", fStaffAllVoicesList.size(): " << fStaffAllVoicesList.size () <<
    ", fStaffRegularVoicesCounter: " << fStaffRegularVoicesCounter <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 38;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffNumber" << " : " <<
    staffNumberAsString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStaffUpLinkToPart" << " : " <<
    fStaffUpLinkToPart->getPartCombinedName () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStaffInstrumentName" << " : \"" <<
    fStaffInstrumentName <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStaffInstrumentAbbreviation" << " : \"" <<
    fStaffInstrumentAbbreviation <<
    std::endl;

  // staff shortest note
  os << std::left <<
    std::setw (fieldWidth) << "fStaffShortestNoteDuration" << " : " <<
    fStaffShortestNoteDuration <<
    std::endl <<
    std::setw (fieldWidth) << "fStaffShortestNoteTupletFactor" << " : " <<
    fStaffShortestNoteTupletFactor <<
    std::endl;

  // print current the staff clef if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fStaffCurrentClef" << " : ";

    if (fStaffCurrentClef) {
      os <<
        "'" <<
        fStaffCurrentClef->asShortString () <<
        "'";
    }
    else {
      os << "[NONE]";
    }

    os << std::endl;
  }
#endif

  // print the current staff key if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fStaffCurrentKey" << " : ";

    if (fStaffCurrentKey) {
      os <<
        "'" <<
        fStaffCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "[NONE]";
    }

    os << std::endl;
  }
#endif

  // print the current staff time if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fStaffCurrentTimeSignature" << " : ";

    if (fStaffCurrentTimeSignature) {
      os <<
        "'" <<
        fStaffCurrentTimeSignature->asShortString () <<
        "'";
    }
    else {
      os <<
        "[NONE]";
    }

    os << std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffDetails ()) {
    // print the staff details if any
    if (fCurrentStaffStaffDetails) {
      os <<
        fCurrentStaffStaffDetails;
    }
    else {
      os << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentStaffStaffDetails" << " : " << "[NONE]";
    }
    os << std::endl;
  }
#endif

  // print the staff 'all voices' std::list
  os << std::left <<
    std::setw (fieldWidth) <<
    "Voice names in fStaffAllVoicesList" << " : ";

  if (fStaffAllVoicesList.size ()) {
    os << std::endl;
    ++gIndenter;

    for (const S_msrVoice& voice : fStaffAllVoicesList) {
      os << "\"" << voice->getVoiceName () << "\"" << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  // print the staff 'regular voices' std::list
  os << std::left <<
    std::setw (fieldWidth) <<
    "Voice names in fStaffRegularVoicesList" << " : ";

  if (fStaffRegularVoicesList.size ()) {
    os << std::endl;

    ++gIndenter;

    int voiceNumber = 0;

    for (const S_msrVoice& voice : fStaffRegularVoicesList) {
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is null");

      ++voiceNumber;

      os <<
        voiceNumber <<
        " : " <<
        voice->asShortString () <<
        std::endl;

      gIndenter.increment (3);

      os <<
//         "voiceKind: " << JMI
//         msrVoiceKindAsString (voice->getVoiceKind ()) <<
        "regularVoiceStaffSequentialNumber: " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        std::endl;

      gIndenter.decrement (3);
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << std::endl;

  // print the staff 'voice numbers to all voices' std::map
  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffVoiceNumbersToAllVoicesMap" << " : ";

  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    os << std::endl;

    ++gIndenter;

    for (std::pair<int, S_msrVoice> thePair : fStaffVoiceNumbersToAllVoicesMap) {
      int        voiceNumber = thePair.first;
      const S_msrVoice& voice       = thePair.second;

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is null");

      os <<
        voiceNumber <<
        " : " <<
        voice->asShortString () <<
        std::endl;

      gIndenter.increment (3);

      os <<
//         "voiceKind: " << JMI
//         msrVoiceKindAsString (voice->getVoiceKind ()) <<
        "regularVoiceStaffSequentialNumber: " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        std::endl;

      gIndenter.decrement (3);
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  // print the staff 'voice numbers to regular voices' std::map
  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffVoiceNumbersToRegularVoicesMap" << " : ";

  if (fStaffVoiceNumbersToRegularVoicesMap.size ()) {
    os << std::endl;

    ++gIndenter;

    for (std::pair<int, S_msrVoice> thePair : fStaffVoiceNumbersToRegularVoicesMap) {
      int        voiceNumber = thePair.first;
      const S_msrVoice& voice       = thePair.second;

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is null");

      os <<
        voiceNumber <<
        " : " <<
        "Voice \"" << voice->getVoiceName () <<
        "\", line " << voice->getInputLineNumber () <<
        std::endl;

      gIndenter.increment (3);

      os <<
//        "voiceKind: " << JMI
//        msrVoiceKindAsString (voice->getVoiceKind ()) <<
        "regularVoiceStaffSequentialNumber: " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        std::endl;

      gIndenter.decrement (3);
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << std::endl;

  // print the staff measures flat std::list std::vector
  size_t staffMeasuresFlatListsVectorSize =
    fStaffMeasuresFlatListsVector.size ();

  os <<
    std::setw (fieldWidth) <<
    "fStaffMeasuresFlatListsVector" << " : ";
  if (staffMeasuresFlatListsVectorSize) {
    os <<
      staffMeasuresFlatListsVectorSize << " elements";

    ++gIndenter;

    std::vector<std::list<S_msrMeasure> >::const_iterator
      iBegin = fStaffMeasuresFlatListsVector.begin (),
      iEnd   = fStaffMeasuresFlatListsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      const std::list<S_msrMeasure>&
        measuresList = (*i);

      // print the measurs std::list
      if (staffMeasuresFlatListsVectorSize) {
        ++gIndenter;

        std::list<S_msrMeasure>::const_iterator
          iBegin = measuresList.begin (),
          iEnd   = measuresList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrMeasure
            measure = (*i);

          // print the measure
          os << (*i)->asShortStringForMeasuresSlices ();

          if (++i == iEnd) break;
          os << std::endl;
        } // for
        os << std::endl;

        --gIndenter;
      }

      if (++i == iEnd) break;

      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << std::endl;

  // print the staff measures slices sequence
  os <<
    std::setw (fieldWidth) <<
    "fStaffMeasuresSlicesSequence" << " : ";

  if (fStaffMeasuresSlicesSequence) {
    os << std::endl;
    ++gIndenter;
    os << fStaffMeasuresSlicesSequence;
    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
  }
  os << std::endl;

  // print the  voices
  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    std::map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumbersToAllVoicesMap.begin (),
      iEnd   = fStaffVoiceNumbersToAllVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      const S_msrVoice& voice = (*i).second;

      os << voice;

      if (++i == iEnd) break;

      os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrStaff::printShort (std::ostream& os) const
{
  os <<
    "[Staff \"" << fStaffName <<
    "\", fStaffKind: " <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    "\", fStaffNumber: " <<
    fStaffNumber <<
    ", fStaffAllVoicesList.size(): " << fStaffAllVoicesList.size () <<
    ", fStaffRegularVoicesCounter: " << fStaffRegularVoicesCounter <<
    std::endl;

  ++gIndenter;

  // print the  voices
  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    os << std::endl;

    std::map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumbersToAllVoicesMap.begin (),
      iEnd   = fStaffVoiceNumbersToAllVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      const S_msrVoice& voice = (*i).second;

      voice->printShort (os);

      if (++i == iEnd) break;

      os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrStaff::printSummary (std::ostream& os) const
{
  os <<
    "[Staff \"" << fStaffName <<
    "\", fStaffKind: " <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    "\", fStaffNumber: " <<
    fStaffNumber <<
    ", fStaffAllVoicesList.size(): " << fStaffAllVoicesList.size () <<
    ", fStaffRegularVoicesCounter: " << fStaffRegularVoicesCounter <<
    std::endl;

  ++gIndenter;

  os <<
    "fStaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" <<
    std::endl;

/* JMI
  if (fStaffTuningsList.size ()) {
    os <<
      "fStaff tunings std::list:" <<
      std::endl;

    std::list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin (),
      iEnd   = fStaffTuningsList.end (),
      i      = iBegin;

    ++gIndenter;
    for ( ; ; ) {
      os << (*i)->asString ();
      if (++i == iEnd) break;
      os << std::endl;
    } // for
    os << std::endl;
    --gIndenter;
  }

  // print the voices names
  if (fStaffAllVoicesList.size ()) {
    os <<
      "Voices:" <<
      std::endl;

    ++gIndenter;

    std::list<S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesList.begin (),
      iEnd   = fStaffAllVoicesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice
        voice = (*i);

      os <<
        std::left <<
          voice->getVoiceName () <<
          " (" <<
          mfSingularOrPlural (
            voice->getVoiceActualNotesCounter (),
            "actual note",
            "actual notes") <<
          ", " <<
          mfSingularOrPlural (
            voice->getVoiceStanzasMap ().size (),
            "stanza",
            "stanzas") <<
          ")";
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  os << std::endl;
*/

  --gIndenter;

  os << ']' << std::endl;
}

void msrStaff::printSlices (std::ostream& os) const
{
  os << "MSR staff slices";

  os << std::endl << std::endl;

  ++gIndenter;

//   if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
//     std::map<int, S_msrVoice>::const_iterator
//       iBegin = fStaffVoiceNumbersToAllVoicesMap.begin (),
//       iEnd   = fStaffVoiceNumbersToAllVoicesMap.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       (*i)->
//         printSlices (os);
//       if (++i == iEnd) break;
//       os << std::endl;
//     } // for
//   }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrStaff& elt)
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
