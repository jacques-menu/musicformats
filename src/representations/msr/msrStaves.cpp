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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrStaves.h"

#include "oahOah.h"

#include "oahOah.h"
#include "msrOah.h"
#include "mxsr2msrOah.h"
#include "msr2msrOah.h"

#include "msrBrowsers.h"


using namespace std;

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
  S_msrPart    staffPartUpLink)
{
  msrStaff* o =
    new msrStaff (
      inputLineNumber,
      staffKind,
      staffNumber,
      staffPartUpLink);
  assert (o != nullptr);

  return o;
}

msrStaff::msrStaff (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber,
  S_msrPart    staffPartUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    staffPartUpLink != nullptr,
    "staffPartUpLink is null");

  // set staff part upLink
  fStaffPartUpLink =
    staffPartUpLink;

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
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
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
        fStaffPartUpLink->getPartMsrName () +
        "_Staff_" +
        mfInt2EnglishWord (fStaffNumber);
      break;

    case msrStaffKind::kStaffKindTablature:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_Tablature_" +
        mfInt2EnglishWord (fStaffNumber);
      break;

    case msrStaffKind::kStaffKindHarmonies:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_HARMONIES_Staff";
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_FIGURED_BASS_Staff";
      break;

    case msrStaffKind::kStaffKindDrum:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_DRUM_Staff";
      break;

    case msrStaffKind::kStaffKindRythmic:
      fStaffName =
        fStaffPartUpLink->getPartMsrName () +
        "_RYTHMIC_Staff";
      break;
  } // switch

  ++gIndenter;

  // check the staff number
  switch (fStaffKind) {
    case msrStaffKind::kStaffKindRegular:
      // the staff number should not be negative
      if (fStaffNumber < 0) {
        stringstream s;

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
        stringstream s;

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
    rational (INT_MAX, 1);
  fStaffShortestNoteTupletFactor =
    rational (1, 1);

  // get the initial staff details from the part if any
  S_msrStaffDetails
    partStaffDetails =
      fStaffPartUpLink->
        getCurrentPartStaffDetails ();

  if (partStaffDetails) {
    // append it to the staff
    appendStaffDetailsToStaff (partStaffDetails);
  }

  // get the initial clef from the part if any
  {
    S_msrClef
      clef =
        fStaffPartUpLink->
          getPartCurrentClef ();

    if (clef) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceClefs ()) {
        gLogStream <<
          "Appending part clef '" << clef->asString () <<
          "' as initial clef to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
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
        fStaffPartUpLink->
          getPartCurrentKey ();

    if (key) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceKeys ()) {
        gLogStream <<
          "Appending part key '" << key->asString () <<
          "' as initial key to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif

      appendKeyToStaff (key);
    }
  }

  // get the initial transpose from the part if any
  {
    S_msrTranspose
      transpose =
        fStaffPartUpLink->
          getPartCurrentTranspose ();

    if (transpose) {
#ifdef TRACING_IS_ENABLED
      if ( gGlobalTracingOahGroup->getTraceTranspositions ()) {
        gLogStream <<
          "Appending part transpose '" << transpose->asString () <<
          "' as initial transpose to staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif

      fStaffCurrentTranspose = transpose;

      appendTransposeToAllStaffVoices (transpose);
    }
  }

  // set staff instrument names default values // JMI
  fStaffInstrumentName =
    fStaffPartUpLink->
      getPartInstrumentName ();
  fStaffInstrumentAbbreviation =
    fStaffPartUpLink->
      getPartInstrumentAbbreviation ();

  // full measure rests
  fStaffContainsFullMeasureRests = false;

  --gIndenter;
}

msrStaff::~msrStaff ()
{}

S_msrStaff msrStaff::createStaffNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Creating a newborn clone of staff \"" <<
      fStaffName <<
      "\"" <<
      endl;
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

S_msrPartGroup msrStaff::fetchStaffPartGroupUpLink () const
{
  S_msrPartGroup result;

  if (fStaffPartUpLink) {
    result =
      fStaffPartUpLink->
        getPartPartGroupUpLink ();
  }

  return result;
}

S_msrScore msrStaff::fetchStaffScoreUpLink () const
{
  S_msrScore result;

  if (fStaffPartUpLink) {
    result =
      fStaffPartUpLink->
        fetchPartScoreUpLink ();
  }

  return result;
}

void msrStaff::registerShortestNoteInStaffIfRelevant (S_msrNote note)
{
  // is note the shortest one in this staff?
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

  if (noteSoundingWholeNotes < fStaffShortestNoteDuration) {
    // set the staff shortest note duration
    this->
      setStaffShortestNoteDuration (
        noteSoundingWholeNotes);

    // cascade this new value to the staff's part
    fStaffPartUpLink ->
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
        "The new shortest note in staff \"" << getStaffName () << "\"" <<
        " becomes " << note->asString () <<
        endl;
    }
#endif
  }

/* JMI
  if (noteDisplayWholeNotes < fStaffShortestNoteDuration) {
    fStaffShortestNoteDuration = noteDisplayWholeNotes;
  }
    */
}

void msrStaff::setStaffCurrentClef (S_msrClef clef)
{
  fStaffCurrentClef = clef;
};

void msrStaff::setStaffCurrentKey (S_msrKey key)
{
  fStaffCurrentKey = key;
};

void msrStaff::setStaffCurrentTime (S_msrTimeSignature timeSignature)
{
  fStaffCurrentTimeSignature = time;
};

string msrStaff::staffNumberAsString () const
{
  string result = to_string (fStaffNumber);

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

  for (S_msrVoice voice : fStaffAllVoicesList) {
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
  const string& measureNumber,
  msrMeasureImplicitKind
                measureImplicitKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to staff \"" << getStaffName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
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
  const string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
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
        endl;
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
  const string& currentMeasureNumber)
{
  ++fStaffRegularVoicesCounter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating regular voice with number '" <<
      voiceNumber <<
      "' as regular voice number '" <<
      fStaffRegularVoicesCounter <<
      "' of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
      "\", current measure number: " <<
      currentMeasureNumber <<
 // JMI     " in part " << fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  // are there too many regular voices in this staff?
  if (fStaffRegularVoicesCounter > msrStaff::gStaffMaxRegularVoices) {
    stringstream s;

    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " <<
      msrStaff::gStaffMaxRegularVoices << " regular voices" <<
      endl <<
      ". voice number " <<
      voiceNumber <<
      " overflows it" <<
      endl <<
      ", fStaffRegularVoicesCounter = " <<
      fStaffRegularVoicesCounter <<
      ", msrStaff::gStaffMaxRegularVoices = " <<
      msrStaff::gStaffMaxRegularVoices <<
      endl;

      /* JMI ???
    musicxmlError (
// JMI    musicxmlWarning ( JMI
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
      */
  }

  // is this voice number already in the regular voices map?
  map<int, S_msrVoice>::const_iterator
    it =
      fStaffVoiceNumbersToRegularVoicesMap.find (voiceNumber);

  if (it != fStaffVoiceNumbersToRegularVoicesMap.end ()) {
    // yes
    S_msrVoice
      olderVoice = (*it).second;

    stringstream s;

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
          "' in staff \"" << getStaffName () <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffPartUpLink->getPartCombinedName () <<
          endl;
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
          "' in staff \"" << getStaffName () <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffPartUpLink->getPartCombinedName () <<
          endl;
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
  const rational& duration)
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note duration in staff \"" <<
        getStaffName () <<
        "\" becomes " <<
        duration <<
        endl;
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
        getStaffName () <<
        "\" becomes " <<
        noteTupletFactor <<
        endl;
    }
#endif

  fStaffShortestNoteTupletFactor = noteTupletFactor;
}

void msrStaff::registerVoiceInStaffAllVoicesList (
  S_msrVoice voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" in all voices list of staff " << getStaffName () <<
      endl;
  }
#endif

  ++gIndenter;

  // sanity check
  if (fStaffAllVoicesList.size ()) {
    for (S_msrVoice knownVoice : fStaffAllVoicesList) {
      if (voice == knownVoice) {
        stringstream s;

        s <<
          "Voice \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices list";

        gLogStream <<
          endl <<
          "---> knownVoice:" <<
          endl;

        ++gIndenter;
        gLogStream <<
          knownVoice <<
          endl << endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gGlobalServiceRunData->getInputSourceName (),
          voice->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }

      if (voice->getVoiceName () == knownVoice->getVoiceName ()) {
        stringstream s;

        s <<
          "A voice with the same name as \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices list";

        gLogStream <<
          endl <<
          "---> knownVoice:" <<
          endl;

        ++gIndenter;
        gLogStream <<
          knownVoice <<
          endl << endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gGlobalServiceRunData->getInputSourceName (),
          voice->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
    } // for
  }

  // register voice in this staff's 'all voices' list
  fStaffAllVoicesList.push_back (voice);

  // register it in the part uplink
  fStaffPartUpLink->
    registerVoiceInPartAllVoicesList (voice);

  --gIndenter;
}

void msrStaff::registerVoiceByItsNumber (
  int        inputLineNumber,
  S_msrVoice voice)
{
  int voiceNumber = voice->getVoiceNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering voice by its number '" << voiceNumber <<
      "', named \"" << voice->getVoiceName () <<
      "\" in staff " << getStaffName () <<
      endl;
  }
#endif

  ++gIndenter;

  // register it in the 'all voices' list
  registerVoiceInStaffAllVoicesList (voice);

  // register voice in the 'numbers to all voices' map
  fStaffVoiceNumbersToAllVoicesMap [voiceNumber] = voice;

  // sort the all voices list if necessary
  switch (voice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // set regularVoice staff sequential number
      voice->
        setRegularVoiceStaffSequentialNumber (
          fStaffRegularVoicesCounter);

      // register it in 'regular voices' list
      fStaffRegularVoicesList.push_back (voice);

      // register voice in the 'numbers to regular voices' map
      fStaffVoiceNumbersToRegularVoicesMap [voiceNumber] = voice;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Sorting the voices in staff \"" <<
          getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          endl;
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
          getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      // sort fStaffAllVoicesList, to have figured bass elements just after
      // the corresponding voice
      if (fStaffAllVoicesList.size ()) {
        fStaffAllVoicesList.sort (
          compareVoicesToHaveFiguredBassElementsBelowCorrespondingVoice);
      }
      break;
  } // switch

  --gIndenter;
}

void msrStaff::registerRegularVoiceByItsNumber (
  int        inputLineNumber,
  S_msrVoice regularVoice,
  int        voiceNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering regular voice " <<
      regularVoice->asShortString () <<
      " by it's number '" << voiceNumber <<
      "\" in staff " << getStaffName () <<
      "'s regular voices list with sequential number '" <<
      fStaffRegularVoicesCounter <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  if (fStaffAllVoicesList.size ()) {
    for (S_msrVoice knownVoice : fStaffAllVoicesList) {
      if (regularVoice == knownVoice) {
        stringstream s;

        s <<
          "Voice \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices list";

        gLogStream <<
          endl <<
          "---> knownVoice:" <<
          endl;

        ++gIndenter;
        gLogStream <<
          knownVoice <<
          endl << endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gGlobalServiceRunData->getInputSourceName (),
          regularVoice->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }

      if (regularVoice->getVoiceName () == knownVoice->getVoiceName ()) {
        stringstream s;

        s <<
          "A voice with the same name as \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices list";

        gLogStream <<
          endl <<
          "---> knownVoice:" <<
          endl;

        ++gIndenter;
        gLogStream <<
          knownVoice <<
          endl << endl;
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
  S_msrVoice voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering harmonies voice " <<
      voice->asString () <<
     " by its number in staff \"" << getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // register the harmonies voice by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voice);
}

void msrStaff::registerFiguredBassVoiceByItsNumber (
  int        inputLineNumber,
  S_msrVoice voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Registering figured bass voice " <<
      voice->asString () <<
     " by its number in staff \"" << getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
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
  S_msrVoice result; // JMI avoid repetitive messages!

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Fetching voice number '" <<
      voiceNumber <<
     "' in staff \"" << getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // search list ??? JMI
  for (pair<int, S_msrVoice> thePair : fStaffVoiceNumbersToRegularVoicesMap) {
#ifdef TRACING_IS_ENABLED
    int        number = thePair.first;
#endif
    S_msrVoice voice  = thePair.second;

    if (
      voice->getVoiceNumber ()
        ==
      voiceNumber
    ) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceVoices ()) {
        gLogStream <<
          "Voice number '" << voiceNumber <<
          "' in staff \"" << getStaffName () << "\"" <<
          " has staff relative number '" << number <<
          "'" <<
          endl;
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
      endl;
  }
#endif

  // assign sequential numbers to the regular voices,
  // needed to know about voices 1, 2, 3 and 4
  fStaffRegularVoicesList.sort (
    compareVoicesByIncreasingNumber);

  if (fStaffRegularVoicesList.size ()) {
    int voiceSequentialCounter = 0;

    list<S_msrVoice>::const_iterator
      iBegin = fStaffRegularVoicesList.begin (),
      iEnd   = fStaffRegularVoicesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      if (i == iEnd) break;

      S_msrVoice voice = (*i);

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
          endl;
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
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (! fStaffRegularVoicesList.size ()) {
    stringstream s;

    s <<
      "staff " << fStaffNumber <<
      " in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
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
        getStaffName () <<
        "\" is " <<
        result->asShortString () <<
        endl;
    }
#endif
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> result = \"" <<
      result->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  return result;
}

void msrStaff::registerVoiceInStaff (
  int        inputLineNumber,
  S_msrVoice voice)
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
      " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUpLink->getPartCombinedName () <<
      endl;
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
        stringstream s;

        s <<
          "staff \"" << getStaffName () <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          endl;

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
  S_msrVoice voice)
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
        stringstream s;

        s <<
          "staff \"" << getStaffName () <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          endl;

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
      " in staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUpLink->getPartCombinedName () <<
      endl;
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
  S_msrVoice voice)
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
      " of staff clone \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUpLink->getPartCombinedName () <<
      endl;
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
        stringstream s;

        s <<
          "staff clone \"" << getStaffName () <<
          "\" is already filled up with " <<
          msrStaff::gStaffMaxRegularVoices << " regular voices," <<
          endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          endl <<
          ", fStaffRegularVoicesCounter = " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::gStaffMaxRegularVoices = " <<
          msrStaff::gStaffMaxRegularVoices <<
          endl;

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

void msrStaff::appendClefToStaff (S_msrClef clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" << clef->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
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
    for (S_msrVoice voice : fStaffAllVoicesList) {
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
        getStaffName () <<
        "\" in part " <<
        fStaffPartUpLink->getPartCombinedName () <<
        endl;
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
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
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
            getStaffName () <<
            "\" in part " <<
            fStaffPartUpLink->getPartCombinedName () <<
            endl;
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
    for (S_msrVoice voice : fStaffAllVoicesList) {
      voice->
        appendKeyToVoice (key);
    } // for
  }

  --gIndenter;
}

void msrStaff::appendTimeSignatureToStaff (S_msrTimeSignature timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time '" << timeSignature->asString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\"" <<
      endl;
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
            getStaffName () <<
            "\" in part " <<
            fStaffPartUpLink->getPartCombinedName () <<
            endl;
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
    for (S_msrVoice voice : fStaffAllVoicesList) {
      voice->
        appendTimeSignatureToVoice (timeSignature);
    } // for
  }

  --gIndenter;
}

void msrStaff::appendTimeSignatureToStaffClone (S_msrTimeSignature timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time '" << timeSignature->asString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  ++gIndenter;

  // set staff time signature
  fStaffCurrentTimeSignature = timeSignature;

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendTimeSignatureToVoiceClone (timeSignature);
  } // for

  --gIndenter;
}

void msrStaff::insertHiddenMeasureAndBarLineInStaffClone (
  int             inputLineNumber,
  const rational& positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos () || gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      positionInMeasure <<
      "' in staff clone \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      insertHiddenMeasureAndBarLineInVoiceClone (
        inputLineNumber,
        positionInMeasure);
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
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrVoice voice : fStaffAllVoicesList) {
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
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      handleRepeatStartInVoice (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrStaff::handleRepeatEndInStaff (
  int           inputLineNumber,
  const string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  for (S_msrVoice voice : fStaffAllVoicesList) {
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
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      handleRepeatEndingStartInVoice (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrStaff::handleRepeatEndingEndInStaff (
  int              inputLineNumber,
  const string&    repeatEndingNumber, // may be "1, 2"
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
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  for (S_msrVoice voice : fStaffAllVoicesList) {
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
  const string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Finalizing a repeat upon its end in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  for (S_msrVoice voice : fStaffAllVoicesList) {
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
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrVoice voice : fStaffAllVoicesList) {
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
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendPendingMeasureRepeatToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::createFullMeasureRestsInStaff (
  int inputLineNumber,
  int fullMeasureRestsMeasuresNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Creating " <<
      mfSingularOrPlural (
        fullMeasureRestsMeasuresNumber, "full measure rest", "full measures rests") <<
      " to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fStaffContainsFullMeasureRests = true;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      createFullMeasureRestsInVoice (
        inputLineNumber,
        fullMeasureRestsMeasuresNumber);
  } // for
}

void msrStaff::replicateLastAppendedMeasureInStaff (
  int inputLineNumber,
  int replicatasNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Replicating last appended measure in staff \"" <<
      getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  // add multiple rest to all staves
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      replicateLastAppendedMeasureInVoice (
        inputLineNumber,
        replicatasNumber);
  } // for
}

void msrStaff::addEmptyMeasuresToStaff (
  int           inputLineNumber,
  const string& previousMeasureNumber,
  int           fullMeasureRestsNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Adding " <<
      mfSingularOrPlural (
        fullMeasureRestsNumber, "full measure rest", "full measure rests") <<
      " to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fStaffContainsFullMeasureRests = true;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      addEmptyMeasuresToVoice (
        inputLineNumber,
        previousMeasureNumber,
        fullMeasureRestsNumber);
  } // for
}

void msrStaff::appendPendingFullMeasureRestsToStaff (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Appending the pending multiple rest to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendPendingFullMeasureRestsToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::appendFullMeasureRestsCloneToStaff (
  int               inputLineNumber,
  S_msrFullMeasureRests fullMeasureRests)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Appending multiple rest '" <<
      fullMeasureRests->asString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\"" <<
      endl;
  }
#endif

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendFullMeasureRestsCloneToVoiceClone (
        inputLineNumber,
        fullMeasureRests);
  } // for
}

void msrStaff::appendRepeatCloneToStaff (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendRepeatCloneToVoiceClone (
        inputLineNumber, repeatCLone);
  } // for
}

void msrStaff::appendRepeatEndingCloneToStaff (
  S_msrRepeatEnding repeatEndingClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a repeat ending clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      ", line " << repeatEndingClone->getInputLineNumber () <<
      endl;
  }
#endif

  ++gIndenter;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendRepeatEndingCloneToVoice (
        repeatEndingClone);
  } // for

  --gIndenter;
}

void msrStaff::appendBarLineToStaff (S_msrBarLine barLine)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " << barLine->asString () <<
      " to staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  ++gIndenter;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendBarLineToVoice (barLine);
  } // for

  --gIndenter;
}

void msrStaff::appendTransposeToStaff (
  S_msrTranspose transpose)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Setting transpose '" <<
      transpose->asString () <<
      "' in staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  // set staff transpose
  Bool doAppendTransposeToStaff (true);

  if (! fStaffCurrentTranspose) {
    doAppendTransposeToStaff = true; // JMI ???
  }

  else {
    if (transpose->isEqualTo (fStaffCurrentTranspose)) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
        gLogStream <<
          "Transpose '" <<
          transpose->asString () <<
          "' ignored because it is already present in staff " <<
          getStaffName () <<
          "\" in part " <<
          fStaffPartUpLink->getPartCombinedName () <<
          endl;
      }
#endif

      doAppendTransposeToStaff = false;
    }
  }

  if (doAppendTransposeToStaff) {
    // register transpose as current staff transpose
    fStaffCurrentTranspose = transpose;

    // cascade it to all voices
    appendTransposeToAllStaffVoices (transpose);
  }
}

void msrStaff::appendStaffDetailsToStaff (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
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
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendStaffDetailsToVoice (staffDetails);
  } // for
}

void msrStaff::appendTransposeToAllStaffVoices (
  S_msrTranspose transpose)
{
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendTransposeToVoice (transpose);
  } // for
}

void msrStaff::appendScordaturaToStaff (
  S_msrScordatura scordatura)
{
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendScordaturaToVoice (
        scordatura);
  } // for
}

void msrStaff::appendAccordionRegistrationToStaff (
  S_msrAccordionRegistration
    accordionRegistration)
{
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendAccordionRegistrationToVoice (
        accordionRegistration);
  } // for
}

void msrStaff::appendHarpPedalsTuningToStaff (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
  for (S_msrVoice voice : fStaffAllVoicesList) {
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
      getStaffName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // finalize all the voices in the staff
  for (S_msrVoice voice : fStaffAllVoicesList) {
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
                getRegularVoiceHarmoniesVoiceForwardLink ();

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

bool msrStaff::compareVoicesToHaveFiguredBassElementsBelowCorrespondingVoice (
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
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // finalize the voices
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Finalizing the voices in staff \"" <<
      getStaffName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (S_msrVoice voice : fStaffAllVoicesList) {
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
      getStaffName () <<
      "s measures flat list vector" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (S_msrVoice voice : fStaffAllVoicesList) {
    // ??? JMI
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
      getStaffName () <<
      "\", " <<
      mfSingularOrPluralWithoutNumber (
        staffVoiceNumbersToAllVoicesMapSize, "there is", "there are") <<
      ' ' <<
      mfSingularOrPlural (
        staffVoiceNumbersToAllVoicesMapSize, "voice", "voices") <<
      ", line " << inputLineNumber <<
      endl;
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
  for (S_msrVoice voice : fStaffAllVoicesList) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
      gLogStream <<
        "---> voice \"" <<
        voice->getVoiceName () <<
        "\":" <<
        endl;
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
          endl;
        ++gIndenter;
        gLogStream <<
          voiceMeasuresSlicesSequence;
        --gIndenter;
      }
      else {
        gLogStream << " : none" << endl;
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
      endl;

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
      endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaff::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaff::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaff::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaff::browseData ()" <<
      endl;
  }

/*
  if (fStaffTuningsList.size ()) {
    for (
      list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin ();
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
    for (S_msrVoice voice : fStaffAllVoicesList) {
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
      endl;

    if (fStaffAllVoicesList.size ()) {
      for (S_msrVoice voice : fStaffAllVoicesList) {
        gLogStream <<
          endl <<
          "+++++++++ voice: ++++++++" <<
          " \"" << voice->getVoiceName () << "\"" <<
          endl;
      } // for
    }
  }
#endif

  if (fStaffAllVoicesList.size ()) {
    for (S_msrVoice voice : fStaffAllVoicesList) {
      // get the  set of voices to ignore
      S_oahStringSetElementAtom
        ignoreMsrVoicesSetAtom =
          gGlobalMsr2msrOahGroup->
            getIgnoreMsrVoicesSetAtom ();;

      Bool
        ignoreMsrVoicesSetIsSetByUser =
          ignoreMsrVoicesSetAtom->
            getSetByUser ();;

      // get the  set of voices to keep
      S_oahStringSetElementAtom
        keepMsrVoicesSetAtom =
          gGlobalMsr2msrOahGroup->
            getKeepMsrVoicesSetAtom ();;

      Bool
        keepMsrVoicesSetIsSetByUser =
          keepMsrVoicesSetAtom->
            getSetByUser ();;

      // JMI this should be done in mxsr2msrOahGroup::checkGroupOptionsConsistency () v0.9.62
      if (ignoreMsrVoicesSetIsSetByUser && keepMsrVoicesSetIsSetByUser) {
        stringstream s;

        s <<
          "Options " <<
          ignoreMsrVoicesSetAtom->fetchNamesBetweenQuotes () <<
          " and " <<
          keepMsrVoicesSetAtom->fetchNamesBetweenQuotes () <<
          " are incompatible" <<
          endl;

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          voice->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }

      const set<string>&
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

      const set<string>&
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

      string
        voiceName =
          voice->
            getVoiceName ();

      // is voiceName in the ignore voices set?
      if (ignoreMsrVoicesSetIsSetByUser) {
        // a voice is to be browsed by default
        if (
          mfStringIsInStringSet (
            voiceName,
            ignoreMsrVoicesSet)
        ) {
          voiceIsToBeBrowsed = false;
        }
      }

      if (keepMsrVoicesSetIsSetByUser) {
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
            endl;
        }
#endif
      }
    } // for
  }

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrStaff::browseData ()" <<
      endl;
  }
}

void msrStaff::print (ostream& os) const
{
  os <<
    '[' <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    " staff \"" << getStaffName () <<
    "\", staff number '" <<
    fStaffNumber <<
    "', " <<
    mfSingularOrPlural (
      fStaffAllVoicesList.size (), "voice", "voices") <<
    ", " <<
    mfSingularOrPlural (
      fStaffRegularVoicesCounter,
      "regular voice",
      "regular voices") << // JMI
    ")" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 38;

  os << left <<
    setw (fieldWidth) <<
    "fStaffNumber" << " : " <<
    staffNumberAsString () <<
    endl <<
    setw (fieldWidth) <<
    "fStaffPartUpLink" << " : " <<
    fStaffPartUpLink->getPartCombinedName () <<
    endl <<
    setw (fieldWidth) <<
    "fStaffInstrumentName" << " : \"" <<
    fStaffInstrumentName <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fStaffInstrumentAbbreviation" << " : \"" <<
    fStaffInstrumentAbbreviation <<
    endl;

  // staff shortest note
  os << left <<
    setw (fieldWidth) << "fStaffShortestNoteDuration" << " : " <<
    fStaffShortestNoteDuration <<
    endl <<
    setw (fieldWidth) << "fStaffShortestNoteTupletFactor" << " : " <<
    fStaffShortestNoteTupletFactor <<
    endl;

  // print current the staff clef if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    os << left <<
      setw (fieldWidth) <<
      "fStaffCurrentClef" << " : ";

    if (fStaffCurrentClef) {
      os <<
        "'" <<
        fStaffCurrentClef->asShortString () <<
        "'";
    }
    else {
      os << "none";
    }

    os << endl;
  }
#endif

  // print the current staff key if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    os << left <<
      setw (fieldWidth) <<
      "fStaffCurrentKey" << " : ";

    if (fStaffCurrentKey) {
      os <<
        "'" <<
        fStaffCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current staff time if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    os << left <<
      setw (fieldWidth) <<
      "fStaffCurrentTimeSignature" << " : ";

    if (fStaffCurrentTimeSignature) {
      os <<
        "'" <<
        fStaffCurrentTimeSignature->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
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
      os << left <<
        setw (fieldWidth) <<
        "fCurrentStaffStaffDetails" << " : " << "none";
    }
    os << endl;
  }
#endif

  // print the staff 'all voices' list
  os << left <<
    setw (fieldWidth) <<
    "Voice names in fStaffAllVoicesList" << " : ";

  if (fStaffAllVoicesList.size ()) {
    os << endl;
    ++gIndenter;

    for (S_msrVoice voice : fStaffAllVoicesList) {
      os << "\"" << voice->getVoiceName () << "\"" << endl;
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  // print the staff 'regular voices' list
  os << left <<
    setw (fieldWidth) <<
    "Voice names in fStaffRegularVoicesList" << " : ";

  if (fStaffRegularVoicesList.size ()) {
    os << endl;

    ++gIndenter;

    int voiceNumber = 0;

    for (S_msrVoice voice : fStaffRegularVoicesList) {
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
        endl;

      gIndenter.increment (3);

      os <<
//         "voiceKind: " << JMI
//         msrVoiceKindAsString (voice->getVoiceKind ()) <<
        "regularVoiceStaffSequentialNumber: " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        endl;

      gIndenter.decrement (3);
    } // for

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  os << endl;

  // print the staff 'voice numbers to all voices' map
  os << left <<
    setw (fieldWidth) <<
    "fStaffVoiceNumbersToAllVoicesMap" << " : ";

  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    os << endl;

    ++gIndenter;

    for (pair<int, S_msrVoice> thePair : fStaffVoiceNumbersToAllVoicesMap) {
      int        voiceNumber = thePair.first;
      S_msrVoice voice       = thePair.second;

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is null");

      os <<
        voiceNumber <<
        " : " <<
        voice->asShortString () <<
        endl;

      gIndenter.increment (3);

      os <<
//         "voiceKind: " << JMI
//         msrVoiceKindAsString (voice->getVoiceKind ()) <<
        "regularVoiceStaffSequentialNumber: " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        endl;

      gIndenter.decrement (3);
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  // print the staff 'voice numbers to regular voices' map
  os << left <<
    setw (fieldWidth) <<
    "fStaffVoiceNumbersToRegularVoicesMap" << " : ";

  if (fStaffVoiceNumbersToRegularVoicesMap.size ()) {
    os << endl;

    ++gIndenter;

    for (pair<int, S_msrVoice> thePair : fStaffVoiceNumbersToRegularVoicesMap) {
      int        voiceNumber = thePair.first;
      S_msrVoice voice       = thePair.second;

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
        endl;

      gIndenter.increment (3);

      os <<
//        "voiceKind: " << JMI
//        msrVoiceKindAsString (voice->getVoiceKind ()) <<
        "regularVoiceStaffSequentialNumber: " <<
        voice->getRegularVoiceStaffSequentialNumber () <<
        endl;

      gIndenter.decrement (3);
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  os << endl;

  // print the staff measures flat list vector
  size_t staffMeasuresFlatListsVectorSize =
    fStaffMeasuresFlatListsVector.size ();

  os <<
    setw (fieldWidth) <<
    "fStaffMeasuresFlatListsVector" << " : ";
  if (staffMeasuresFlatListsVectorSize) {
    os <<
      staffMeasuresFlatListsVectorSize << " elements";

    ++gIndenter;

    vector<list<S_msrMeasure> >::const_iterator
      iBegin = fStaffMeasuresFlatListsVector.begin (),
      iEnd   = fStaffMeasuresFlatListsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      const list<S_msrMeasure>&
        measuresList = (*i);

      // print the measurs list
      if (staffMeasuresFlatListsVectorSize) {
        ++gIndenter;

        list<S_msrMeasure>::const_iterator
          iBegin = measuresList.begin (),
          iEnd   = measuresList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrMeasure
            measure = (*i);

          // print the measure
          os << (*i)->asShortStringForMeasuresSlices ();

          if (++i == iEnd) break;
          os << endl;
        } // for
        os << endl;

        --gIndenter;
      }

      if (++i == iEnd) break;

      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  os << endl;

  // print the staff measures slices sequence
  os <<
    setw (fieldWidth) <<
    "fStaffMeasuresSlicesSequence" << " : ";

  if (fStaffMeasuresSlicesSequence) {
    os << endl;
    ++gIndenter;
    os << fStaffMeasuresSlicesSequence;
    --gIndenter;
  }
  else {
    os << "none" << endl;
  }
  os << endl;

  // print the  voices
  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumbersToAllVoicesMap.begin (),
      iEnd   = fStaffVoiceNumbersToAllVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

      os << voice;

      if (++i == iEnd) break;

      os << endl;
    } // for
  }

  --gIndenter;

  os << ']' << endl;
}

void msrStaff::printShort (ostream& os) const
{
  os <<
    '[' <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    " staff \"" << getStaffName () <<
    "\", staff number '" <<
    fStaffNumber <<
    "', " <<
    mfSingularOrPlural (
      fStaffVoiceNumbersToAllVoicesMap.size (), "voice", "voices") <<
    ", " <<
    mfSingularOrPlural (
      fStaffRegularVoicesCounter,
      "regular voice",
      "regular voices") << // JMI
    ")" <<
    endl;

  ++gIndenter;

  // print the  voices
  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    os << endl;

    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumbersToAllVoicesMap.begin (),
      iEnd   = fStaffVoiceNumbersToAllVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

      voice->printShort (os);

      if (++i == iEnd) break;

      os << endl;
    } // for
  }

  --gIndenter;

  os << ']' << endl;
}

void msrStaff::printSummary (ostream& os) const
{
  os <<
    '[' <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    " staff \"" << getStaffName () <<
    "\", staff number '" <<
    fStaffNumber <<
    "', (" <<
    mfSingularOrPlural (
      fStaffAllVoicesList.size (), "voice", "voices") <<
    ")" <<
    endl;

  ++gIndenter;

  os <<
    "fStaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" <<
    endl;

/* JMI
  if (fStaffTuningsList.size ()) {
    os <<
      "fStaff tunings list:" <<
      endl;

    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin (),
      iEnd   = fStaffTuningsList.end (),
      i      = iBegin;

    ++gIndenter;
    for ( ; ; ) {
      os << (*i)->asString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    --gIndenter;
  }

  // print the voices names
  if (fStaffAllVoicesList.size ()) {
    os <<
      "Voices:" <<
      endl;

    ++gIndenter;

    list<S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesList.begin (),
      iEnd   = fStaffAllVoicesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice
        voice = (*i);

      os <<
        left <<
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
      os << endl;
    } // for

    --gIndenter;
  }

  os << endl;
*/

  --gIndenter;

  os << ']' << endl;
}

void msrStaff::printSlices (ostream& os) const
{
  os << "MSR part slices";

  os << endl << endl;

  ++gIndenter;

//   if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
//     map<int, S_msrVoice>::const_iterator
//       iBegin = fStaffVoiceNumbersToAllVoicesMap.begin (),
//       iEnd   = fStaffVoiceNumbersToAllVoicesMap.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       (*i)->
//         printSlices (os);
//       if (++i == iEnd) break;
//       os << endl;
//     } // for
//   }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrStaff& elt)
{
  elt->print (os);
  return os;
}


}
