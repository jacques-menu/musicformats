/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "mfPreprocessorSettings.h"
#include "mfConstants.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrBarLines.h"
#include "msrBreaks.h"
#include "msrRehearsalMarks.h"
#include "msrRehearsalMarks.h"
#include "msrTempos.h"

#include "oahOah.h"

#include "oahOah.h"

#include "waeOah.h"

#include "msrOah.h"
#include "mxsr2msrOah.h"
#include "msr2msrOah.h"

#include "msrWae.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// global variable
int msrStaff::sStaffMaxRegularVoices = 4; // JMI TEMP MOD v0.9.70

S_msrStaff msrStaff::create (
  int              inputLineNumber,
  msrStaffKind     staffKind,
  int              staffNumber,
  const S_msrPart& staffUpLinkToPart)
{
  msrStaff* obj =
    new msrStaff (
      inputLineNumber,
      staffKind,
      staffNumber,
      staffUpLinkToPart);
  assert (obj != nullptr);
  return obj;
}

msrStaff::msrStaff (
  int              inputLineNumber,
  msrStaffKind     staffKind,
  int              staffNumber,
  const S_msrPart& staffUpLinkToPart)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    staffUpLinkToPart != nullptr,
    "staffUpLinkToPart is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set staff part upLink
  fStaffUpLinkToPart =
    staffUpLinkToPart;

  // set staff kind and number
  fStaffKind   = staffKind;
  fStaffNumber = staffNumber;

  // do other initializations
  initializeStaff ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Creating staff \"" << asString () <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrStaff::initializeStaff ()
{
  fStaffRegularVoicesCounter = 0;

  // set staff names
  switch (fStaffKind) {
    case msrStaffKind::kStaffKindRegular:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_Staff_" +
        std::to_string (fStaffNumber);

      fStaffAlphabeticName =
        fStaffUpLinkToPart->getPartAlphabeticName () +
        "_Staff_" +
        mfIntToEnglishWord (fStaffNumber);
      break;

    case msrStaffKind::kStaffKindTablature:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_TABLATURE_Staff";

      fStaffAlphabeticName =
        fStaffUpLinkToPart->getPartAlphabeticName () +
        "_TABLATURE_Staff";
      break;

    case msrStaffKind::kStaffKindHarmonies:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_HARMONIES_Staff";

      fStaffAlphabeticName =
        fStaffUpLinkToPart->getPartAlphabeticName () +
        "_HARMONIES_Staff";
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_FIGURED_BASS_Staff";

      fStaffAlphabeticName =
        fStaffUpLinkToPart->getPartAlphabeticName () +
        "_FIGURED_BASS_Staff";
      break;

    case msrStaffKind::kStaffKindDrum:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_DRUM_Staff";

      fStaffAlphabeticName =
        fStaffUpLinkToPart->getPartAlphabeticName () +
        "_DRUM_Staff";
      break;

    case msrStaffKind::kStaffKindRythmic:
      fStaffName =
        fStaffUpLinkToPart->getPartMsrName () +
        "_RYTHMIC_Staff";

      fStaffAlphabeticName =
        fStaffUpLinkToPart->getPartAlphabeticName () +
        "_RYTHMIC_Staff";
      break;
  } // switch

  ++gIndenter;

  // check the staff number
  switch (fStaffKind) {
    case msrStaffKind::kStaffKindRegular:
      // the staff number should not be negative
      if (fStaffNumber < 0) {
        std::stringstream ss;

        ss <<
          "regular staff number " << fStaffNumber <<
          " is not positive";

        mfAssert (
          __FILE__, __LINE__,
          false,
          ss.str ());
      }
      break;

    case msrStaffKind::kStaffKindTablature:
      break;

    case msrStaffKind::kStaffKindHarmonies:
      break;

    case msrStaffKind::kStaffKindFiguredBass:
    /* JMI
      if (fStaffNumber != K_PART_FIGURED_BASS_STAFF_NUMBER) {
        std::stringstream ss;

        ss <<
          "figured bass staff number " << fStaffNumber <<
          " is not equal to " << K_PART_FIGURED_BASS_STAFF_NUMBER;

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputStartLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      */
      break;

    case msrStaffKind::kStaffKindDrum:
      break;

    case msrStaffKind::kStaffKindRythmic:
      break;
  } // switch

  // staff shortest note
  fStaffShortestNoteWholeNotes =
    msrWholeNotes (INT_MAX, 1);
  fStaffShortestNoteTupletFactor =
    mfRational (1, 1);

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
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceClefs ()) {
        std::stringstream ss;

        ss <<
          "Appending part clef " <<
          clef->asString () <<
          " as initial clef to staff \"" <<
          fStaffName <<
          "\" in part " <<
          fStaffUpLinkToPart->fetchPartCombinedName ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      appendClefToStaff (
        0, // JMI v0.9.67, groupInputLineNumber
        clef);
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
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceKeys ()) {
        std::stringstream ss;

        ss <<
          "Appending part key '" <<
          key->asString () <<
          "' as initial key to staff \"" <<
          fStaffName <<
          "\" in part " <<
          fStaffUpLinkToPart->fetchPartCombinedName ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      appendKeyToStaff (
        0, // JMI v0.9.67, groupInputLineNumber
        key);
    }
  }

  // get the initial transposition from the part if any
  {
    S_msrTransposition
      transposition =
        fStaffUpLinkToPart->
          getPartCurrentTransposition ();

    if (transposition) {
#ifdef MF_TRACE_IS_ENABLED
      if ( gTraceOahGroup->getTraceTranspositions ()) {
        std::stringstream ss;

        ss <<
          "Appending part transposition '" << transposition->asString () <<
          "' as initial transposition to staff \"" <<
          fStaffName <<
          "\" in part " <<
          fStaffUpLinkToPart->fetchPartCombinedName ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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

  // multi-measure rests
  fStaffContainsMultiMeasureRests = false;

  --gIndenter;
}

msrStaff::~msrStaff ()
{}

S_msrStaff msrStaff::createStaffNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of staff \"" <<
      fStaffName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrStaff
    newbornClone =
      msrStaff::create (
        fInputStartLineNumber,
        fStaffKind,
        fStaffNumber,
        containingPart);

  newbornClone->fStaffName =
    fStaffName;
  newbornClone->fStaffAlphabeticName =
    fStaffAlphabeticName;

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
  msrWholeNotes
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

      /* JMI
  msrWholeNotes
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();
        */

  if (noteSoundingWholeNotes < fStaffShortestNoteWholeNotes) {
    // set the staff shortest note duration
    this->
      setStaffShortestNoteWholeNotes (
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "The new shortest note in staff \"" << fStaffName << "\"" <<
        " becomes " << note->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

/* JMI
  if (noteDisplayWholeNotes < fStaffShortestNoteWholeNotes) {
    fStaffShortestNoteWholeNotes = noteDisplayWholeNotes;
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

void msrStaff::setStaffCurrentTimeSignature (
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
    std::stringstream ss;

    ss <<
      "Setting staff current time signature of " <<
      asString () <<
      " to " <<
      timeSignature->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffCurrentTimeSignature = timeSignature;
};

std::string msrStaff::fetchStaffNumberAsString () const
{
  return mfStaffNumberAsString (fStaffNumber);
}

/* KEEP JMI
const int msrStaff::getStaffNumberOfMusicVoices () const
{
  int result = 0;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      switch (voice->getVoiceKind ()) {
        case msrVoiceKind::kVoiceKindRegular:
          if (voice->getMeasureIsMusicallyEmpty ()) {
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

void msrStaff::cascadeCreateAMeasureAndAppendItInStaff (
  int                inputLineNumber,
  int                previousMeasureEndInputLineNumber,
  const std::string& measureNumber,
  msrMeasureImplicitKind
                     measureImplicitKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to staff \"" << fStaffName << "\"" <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
    mfAssert (
      __FILE__, __LINE__,
      voice != nullptr,
      "voice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

    voice->
      cascadeCreateAMeasureAndAppendItInVoice (
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in staff \"" << fStaffName << "\"" <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
    mfAssert (
      __FILE__, __LINE__,
      voice != nullptr,
      "voice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "Propagating the setting of next measure number '" <<
        nextMeasureNumber <<
        "', line " << inputLineNumber <<
        ", in voice \"" << voice->getVoiceName () << "\"";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    voice->
      setNextMeasureNumberInVoice (
        inputLineNumber,
        nextMeasureNumber);
  } // for

  --gIndenter;
}

S_msrVoice msrStaff::createRegularVoiceInStaffByItsNumber (
  int                   inputLineNumber,
  int                   voiceNumber,
  const std::string&    currentMeasureNumber,
  const S_msrPartGroup& partUpLinkToPartGroup)
{
  ++fStaffRegularVoicesCounter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Creating regular voice with number '" <<
      voiceNumber <<
      "' as regular voice number '" <<
      fStaffRegularVoicesCounter <<
      "' of staff \"" << fStaffName <<
      "\", line " << inputLineNumber <<
      "\", current measure number: " <<
      currentMeasureNumber;
 // JMI     " in part " << fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // are there too many regular voices in this staff?
  if (fStaffRegularVoicesCounter > msrStaff::sStaffMaxRegularVoices) {
    std::stringstream ss;

    ss <<
      "staff \"" << fStaffName <<
      "\" is already filled up with " <<
      msrStaff::sStaffMaxRegularVoices << " regular voices" <<
      std::endl <<
      ". voice number " <<
      voiceNumber <<
      " overflows it" <<
      std::endl <<
      ", fStaffRegularVoicesCounter: " <<
      fStaffRegularVoicesCounter <<
      ", msrStaff::sStaffMaxRegularVoices: " <<
      msrStaff::sStaffMaxRegularVoices <<
      std::endl;

      /* JMI ???
    musicxmlError (
// JMI    musicxmlWarning ( JMI
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
      */
  }

  // is this voice number already in the regular voices map?
  std::map <int, S_msrVoice>::const_iterator
    it =
      fStaffVoiceNumbersToRegularVoicesMap.find (voiceNumber);

  if (it != fStaffVoiceNumbersToRegularVoicesMap.end ()) {
    // yes
    S_msrVoice
      olderVoice = (*it).second;

    std::stringstream ss;

    ss <<
      "attempt at creating a voice anew by number" <<
      ", voiceNumber: " << voiceNumber <<
      ", currentMeasureNumber: " << currentMeasureNumber <<
      " ' in staff \"" <<
      fStaffName <<
      "\", " <<
      olderVoice->asShortString () <<
      " already exists with this same voiceNumber" <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
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
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceVoices ()) {
        std::stringstream ss;

        ss <<
          "Creating harmonies voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoiceKindAsString (voiceKind) <<
          "' in staff \"" << fStaffName <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffUpLinkToPart->fetchPartCombinedName ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceVoices ()) {
        std::stringstream ss;

        ss <<
          "Creating figured bass voice number '" <<
          voiceNumber <<
          "', voiceKind '" <<
          msrVoiceKindAsString (voiceKind) <<
          "' in staff \"" << fStaffName <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffUpLinkToPart->fetchPartCombinedName ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
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

void msrStaff::setStaffShortestNoteWholeNotes (
  const msrWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "The new shortest note wholeNotes in staff \"" <<
        fStaffName <<
        "\" becomes " <<
        wholeNotes.asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  fStaffShortestNoteWholeNotes = wholeNotes;
}

void msrStaff::setStaffShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "The new shortest note tuplet factor in part \"" <<
        fStaffName <<
        "\" becomes " <<
        noteTupletFactor;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  fStaffShortestNoteTupletFactor = noteTupletFactor;
}

void msrStaff::registerVoiceInStaffAllVoicesList (
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" in all voices list of staff " << fStaffName <<
      " line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (fStaffAllVoicesList.size ()) {
    for (S_msrVoice knownVoice : fStaffAllVoicesList) {
      if (voice == knownVoice) {
        std::stringstream ss;

        ss <<
          "Voice \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices list";

        gLog <<
          std::endl <<
          "---> knownVoice:" <<
          std::endl;

        ++gIndenter;
        gLog <<
          knownVoice <<
          std::endl << std::endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gServiceRunData->getInputSourceName (),
          voice->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }

      if (voice->getVoiceName () == knownVoice->getVoiceName ()) {
        std::stringstream ss;

        ss <<
          "A voice with the same name as \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices list";

        gLog <<
          std::endl <<
          "---> knownVoice:" <<
          std::endl;

        ++gIndenter;
        gLog <<
          knownVoice <<
          std::endl << std::endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gServiceRunData->getInputSourceName (),
          voice->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    } // for
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // register voice in this staff's 'all voices' list
  fStaffAllVoicesList.push_back (voice);

  // register it in the part uplink
  fStaffUpLinkToPart->
    registerVoiceInPartVoicesList (voice);

  --gIndenter;
}

void msrStaff::registerVoiceByItsNumber (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
  int voiceNumber = voice->getVoiceNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering voice named \"" << voice->getVoiceName () <<
      "\", " <<
      voice->asShortString () <<
      ", by its number '" << voiceNumber <<
      "\" in staff " << fStaffName <<
      " line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // register it in the 'all voices' list
  registerVoiceInStaffAllVoicesList (voice); // JMI v0.9.63 NASTY bug???

  // register voice in the 'numbers to all voices' map
  fStaffVoiceNumbersToAllVoicesMap [voiceNumber] = voice;

  // sort the all voices list if necessary
  switch (voice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // set voice's staff sequential number
      voice->
        setRegularVoiceStaffSequentialNumber (
          fStaffRegularVoicesCounter);

      // register voice in 'regular voices' list
      fStaffRegularVoicesList.push_back (voice);

      // register voice in the 'numbers to regular voices' map
      fStaffVoiceNumbersToRegularVoicesMap [voiceNumber] = voice;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceVoices ()) {
        std::stringstream ss;

        ss <<
          "Sorting the voices in staff \"" <<
          fStaffName << "\"" <<
          ", line " << inputLineNumber;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // sort fStaffAllVoicesList, to have harmonies just before
      // the corresponding voice
      if (fStaffAllVoicesList.size ()) {
        fStaffAllVoicesList.sort (
          msrVoice::compareVoicesToHaveHarmoniesAboveCorrespondingVoice);
      }
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceVoices ()) {
        std::stringstream ss;

        ss <<
          "Sorting the voices in staff \"" <<
          fStaffName << "\"" <<
          ", line " << inputLineNumber;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // sort fStaffAllVoicesList, to have figured bass elements just after
      // the corresponding voice
      if (fStaffAllVoicesList.size ()) {
        fStaffAllVoicesList.sort (
          msrVoice::compareVoicesToHaveFiguredBassesBelowCorrespondingVoice);
      }
      break;
  } // switch

  // register voice in its staff at the part level
  fStaffUpLinkToPart->
    registerVoiceInPartStaffVoicesMap (voice);

  --gIndenter;
}

void msrStaff::registerRegularVoiceByItsNumber (
  int               inputLineNumber,
  const S_msrVoice& regularVoice,
  int               voiceNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering regular voice " <<
      regularVoice->asShortString () <<
      " by it's number '" << voiceNumber <<
      "\" in staff " << fStaffName <<
      "'s regular voices list with sequential number '" <<
      fStaffRegularVoicesCounter <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (fStaffAllVoicesList.size ()) {
    for (S_msrVoice knownVoice : fStaffAllVoicesList) {
      if (regularVoice == knownVoice) {
        std::stringstream ss;

        ss <<
          "Voice \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices list";

        gLog <<
          std::endl <<
          "---> knownVoice:" <<
          std::endl;

        ++gIndenter;
        gLog <<
          knownVoice <<
          std::endl << std::endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gServiceRunData->getInputSourceName (),
          regularVoice->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }

      if (regularVoice->getVoiceName () == knownVoice->getVoiceName ()) {
        std::stringstream ss;

        ss <<
          "A voice with the same name as \"" <<
          knownVoice->getVoiceName () <<
          "\" is already present in the staff's all voices list";

        gLog <<
          std::endl <<
          "---> knownVoice:" <<
          std::endl;

        ++gIndenter;
        gLog <<
          knownVoice <<
          std::endl << std::endl;
        --gIndenter;

        msrInternalError ( // JMI ???
          gServiceRunData->getInputSourceName (),
          regularVoice->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    } // for
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // register voice by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    regularVoice);

  --gIndenter;
}

void msrStaff::registerHarmoniesVoiceByItsNumber (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering harmonies voice " <<
      voice->asString () <<
     " by its number in staff \"" << fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the harmonies voice by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    voice);
}

void msrStaff::registerFiguredBassVoiceByItsNumber (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering figured bass voice " <<
      voice->asString () <<
     " by its number in staff \"" << fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Fetching voice by its number " <<
      voiceNumber <<
     " in staff \"" << fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // search list ??? JMI
  for (std::pair <int, S_msrVoice> thePair : fStaffVoiceNumbersToRegularVoicesMap) {
#ifdef MF_TRACE_IS_ENABLED
    int        number = thePair.first;
#endif // MF_TRACE_IS_ENABLED
    S_msrVoice voice = thePair.second;

    if (
      voice->getVoiceNumber ()
        ==
      voiceNumber
    ) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceVoices ()) {
        std::stringstream ss;

        ss <<
          "Fetching voice by its number " <<
          voiceNumber <<
          "' in staff \"" << fStaffName << "\"" <<
          " has staff relative number '" << number <<
          '\'';

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      result = voice;
      break;
    }
  } // for

  return result;
}

void msrStaff::assignSequentialNumbersToRegularVoicesInStaff (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Assigning sequential numbers to the regular voices in staff \"" <<
      fStaffName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // assign sequential numbers to the regular voices,
  // needed to know about voices 1, 2, 3 and 4
  fStaffRegularVoicesList.sort (
    msrVoice::compareVoicesByIncreasingNumber);

  if (fStaffRegularVoicesList.size ()) {
    int voiceSequentialCounter = 0;

    std::list <S_msrVoice>::const_iterator
      iBegin = fStaffRegularVoicesList.begin (),
      iEnd   = fStaffRegularVoicesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      if (i == iEnd) break;

      S_msrVoice voice = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      ++voiceSequentialCounter;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceVoices ()) {
        std::stringstream ss;

        ss <<
          "Voice \"" <<
          voice->getVoiceName () <<
          "\" gets sequential number " <<
          voiceSequentialCounter <<
          " in staff \"" <<
          fStaffName <<
          "\"";

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Fetching first regular voice in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fStaffRegularVoicesList.size ()) {
    std::stringstream ss;

    ss <<
      "staff " << fStaffNumber <<
      " in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      " doesn't contain any regular voices, cannot fetch the first one" <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  else {
    result = fStaffRegularVoicesList.front ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
    mfAssert (
      __FILE__, __LINE__,
      result->getRegularVoiceStaffSequentialNumber () == 1,
      "result->getRegularVoiceStaffSequentialNumber () is not equal to 1");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceVoices ()) {
      std::stringstream ss;

      ss <<
        "The first regular voice in staff \"" <<
        fStaffName <<
        "\" is " <<
        result->asShortString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "--> result: \"" <<
      result->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

void msrStaff::registerVoiceInStaff (
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voice != nullptr,
    "voice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " <<
      fStaffRegularVoicesCounter <<
      " of staff \"" << fStaffName <<
      "\", line " << inputLineNumber;
// JMI       " in part " << fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
      if (fStaffRegularVoicesCounter > msrStaff::sStaffMaxRegularVoices) {
        std::stringstream ss;

        ss <<
          "staff \"" << fStaffName <<
          "\" is already filled up with " <<
          msrStaff::sStaffMaxRegularVoices << " regular voices," <<
          std::endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          std::endl <<
          ", fStaffRegularVoicesCounter: " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::sStaffMaxRegularVoices: " <<
          msrStaff::sStaffMaxRegularVoices <<
          std::endl;

          /* JMI ???
        musicxmlError (
    // JMI    musicxmlWarning ( JMI
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
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
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voice != nullptr,
    "voice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
      if (fStaffRegularVoicesCounter > msrStaff::sStaffMaxRegularVoices) {
        std::stringstream ss;

        ss <<
          "staff \"" << fStaffName <<
          "\" is already filled up with " <<
          msrStaff::sStaffMaxRegularVoices << " regular voices," <<
          std::endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          std::endl <<
          ", fStaffRegularVoicesCounter: " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::sStaffMaxRegularVoices: " <<
          msrStaff::sStaffMaxRegularVoices <<
          std::endl;

          /* JMI ???
        musicxmlError (
    // JMI    musicxmlWarning ( JMI
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
          */
      }
      break;
  } // switch

  // register voice in this staff
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as part level voice " <<
      fStaffRegularVoicesCounter <<
      " in staff \"" << fStaffName <<
      "\", line " << inputLineNumber;
// JMI       " in part " << fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  int               inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voice != nullptr,
    "voice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // get voice kind
  msrVoiceKind
    voiceKind =
      voice->getVoiceKind ();

  // register voice in this staff
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " <<
      fStaffRegularVoicesCounter <<
      " of staff clone \"" << fStaffName <<
      "\", line " << inputLineNumber;
// JMI       " in part " << fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the voice in staff clone by it's number
  int voiceNumber = voice->getVoiceNumber ();

  switch (voiceKind) {
    case msrVoiceKind::kVoiceKindRegular:
      // take that regular voice into account
      ++fStaffRegularVoicesCounter;

      // are there too many voices in this staff?
      if (fStaffRegularVoicesCounter > msrStaff::sStaffMaxRegularVoices) {
        std::stringstream ss;

        ss <<
          "staff clone \"" << fStaffName <<
          "\" is already filled up with " <<
          msrStaff::sStaffMaxRegularVoices << " regular voices," <<
          std::endl <<
          "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
          std::endl <<
          ", fStaffRegularVoicesCounter: " <<
          fStaffRegularVoicesCounter <<
          ", msrStaff::sStaffMaxRegularVoices: " <<
          msrStaff::sStaffMaxRegularVoices <<
          std::endl;

          /* JMI ???
        musicxmlError (
    // JMI    musicxmlWarning ( JMI
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
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

// void msrStaff::appendClefKeyTimeSignatureGroupToStaff  (
//   const S_msrClefKeyTimeSignatureGroup& ClefKeyTimeSignatureGroup)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->fetchTraceClefKeyTimeSignatureGroups ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending ClefKeyTimeSignatureGroup " << ClefKeyTimeSignatureGroup->asString () <<
//       " to staff \"" <<
//       fStaffName <<
//       "\" in part " <<
//       fStaffUpLinkToPart->fetchPartCombinedName ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // append ClefKeyTimeSignatureGroup to the staff,
//   // unless we should ignore redundant clefs
//   // and a clef equal to the current clef is found
//   Bool doAppendClefKeyTimeSignatureGroupToStaff (true);
//
//   if (fStaffCurrentClef) {
//     if (
//       gGlobalMxsr2msrOahGroup->getIgnoreRedundantClefs ()
//         &&
//       clef->isEqualTo (fStaffCurrentClef)
//     ) {
//       doAppendClefToStaff = false;
//     }
//   }
//
//   if (doAppendClefToStaff) {
//     // register clef as current staff clef
//     fStaffCurrentClef = clef;
//
//     // cascade ClefKeyTimeSignatureGroup to all voices
//     for (S_msrVoice voice : fStaffAllVoicesList) {
//       voice->
//         appendClefKeyTimeSignatureGroupToVoice (clef);
//     } // for
//   }
//
//   else {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceClefs ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Clef " <<
//         clef
//         " ignored because it is already present in staff " <<
//         fStaffName <<
//         "\" in part " <<
//         fStaffUpLinkToPart->fetchPartCombinedName ();
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
//
//   --gIndenter;
// }
//

void msrStaff::appendClefToStaff  (
  int              groupInputLineNumber,
  const S_msrClef& clef)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefs ()) {
    std::stringstream ss;

    ss <<
      "Appending clef " <<
      clef->asString () <<
      " to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    Bool doCreateAClefKeyTimeSignatureGroup (false);

    if (
      fCurrentClefKeyTimeSignatureGroup
        &&
      (
        groupInputLineNumber
          ==
        fCurrentClefKeyTimeSignatureGroup->getGroupInputLineNumber ()
      )
    ) {
      // re-use fCurrentClefKeyTimeSignatureGroup
    }
    else {
      doCreateAClefKeyTimeSignatureGroup = true;
    }

    if (doCreateAClefKeyTimeSignatureGroup) {
      // create a new msrClefKeyTimeSignatureGroup
      fCurrentClefKeyTimeSignatureGroup =
        msrClefKeyTimeSignatureGroup::create (
          clef->getInputStartLineNumber (),
          groupInputLineNumber);
    }

    // register clef in fCurrentClefKeyTimeSignatureGroup
    // before the latter is cascaded to the voices if just created JMI v0.9.70
    fCurrentClefKeyTimeSignatureGroup->
      setClef (clef);

    if (doCreateAClefKeyTimeSignatureGroup) {
      // cascade fCurrentClefKeyTimeSignatureGroup to all voices
      for (S_msrVoice voice : fStaffAllVoicesList) {
        voice->
          appendClefKeyTimeSignatureGroupToVoice (
            fCurrentClefKeyTimeSignatureGroup);
      } // for
    }

    // register clef as current staff clef
    fStaffCurrentClef = clef; // JMI v0.9.67
  }

  --gIndenter;
}

void msrStaff::appendKeyToStaff (
  int              groupInputLineNumber,
  const S_msrKey&  key)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Appending key " <<
      key->asString () <<
      " to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceKeys ()) {
          std::stringstream ss;

          ss <<
            "Key '" <<
            key->asString () <<
            "' ignored because it is already present in staff " <<
            fStaffName <<
            "\" in part " <<
            fStaffUpLinkToPart->fetchPartCombinedName ();

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        doAppendKeyToStaff = false;
      }
    }
  }

  if (doAppendKeyToStaff) {
    Bool doCreateAClefKeyTimeSignatureGroup (false);

    if (
      fCurrentClefKeyTimeSignatureGroup
        &&
      (
        groupInputLineNumber
          ==
        fCurrentClefKeyTimeSignatureGroup->getGroupInputLineNumber ()
      )
    ) {
      // re-use fCurrentClefKeyTimeSignatureGroup
    }
    else {
      doCreateAClefKeyTimeSignatureGroup = true;
    }

    if (doCreateAClefKeyTimeSignatureGroup) {
      // create a new msrClefKeyTimeSignatureGroup
      fCurrentClefKeyTimeSignatureGroup =
        msrClefKeyTimeSignatureGroup::create (
          key->getInputStartLineNumber (),
          groupInputLineNumber);
    }

    // register key in fCurrentClefKeyTimeSignatureGroup
    // before the latter is cascaded to the voices if just created JMI v0.9.70
    fCurrentClefKeyTimeSignatureGroup->
      setKey (key);

    if (doCreateAClefKeyTimeSignatureGroup) {
      // cascade fCurrentClefKeyTimeSignatureGroup to all voices
      for (S_msrVoice voice : fStaffAllVoicesList) {
        voice->
          appendClefKeyTimeSignatureGroupToVoice (
            fCurrentClefKeyTimeSignatureGroup);
      } // for
    }

    // register key as current staff key
    fStaffCurrentKey = key;
  }

  --gIndenter;
}

void msrStaff::appendTimeSignatureToStaff (
  int                       groupInputLineNumber,
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature " <<
      timeSignature->asString () <<
      " to staff \"" <<
      fStaffName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append time signature to staff?
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
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTimeSignatures ()) {
          std::stringstream ss;

          ss <<
            "Time '" <<
            timeSignature->asString () <<
            "' ignored because it is already present in staff " <<
            fStaffName <<
            "\" in part " <<
            fStaffUpLinkToPart->fetchPartCombinedName ();

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        doAppendTimeToStaff = false;
      }
    }
  }

  if (doAppendTimeToStaff) {
    Bool doCreateAClefKeyTimeSignatureGroup (false);

    if (
      fCurrentClefKeyTimeSignatureGroup
        &&
      (
        groupInputLineNumber
          ==
        fCurrentClefKeyTimeSignatureGroup->getGroupInputLineNumber ()
      )
    ) {
      // re-use fCurrentClefKeyTimeSignatureGroup
    }
    else {
      doCreateAClefKeyTimeSignatureGroup = true;
    }

    if (doCreateAClefKeyTimeSignatureGroup) {
      // create a new msrClefKeyTimeSignatureGroup
      fCurrentClefKeyTimeSignatureGroup =
        msrClefKeyTimeSignatureGroup::create (
          timeSignature->getInputStartLineNumber (),
          groupInputLineNumber);
    }

    if (doCreateAClefKeyTimeSignatureGroup) {
      // cascade fCurrentClefKeyTimeSignatureGroup to all voices
      for (S_msrVoice voice : fStaffAllVoicesList) {
        voice->
          appendClefKeyTimeSignatureGroupToVoice (
            fCurrentClefKeyTimeSignatureGroup);
      } // for
    }

    // register timeSignature in fCurrentClefKeyTimeSignatureGroup
    // before the latter is cascaded to the voices if just created JMI v0.9.70
    fCurrentClefKeyTimeSignatureGroup->
      setTimeSignature (timeSignature);

    // register timeSignature as current staff time signature
    fStaffCurrentTimeSignature = timeSignature;

//     // cascade timeSignature to all voices
//     for (S_msrVoice voice : fStaffAllVoicesList) {
//       voice->
//         appendTimeSignatureToVoice (
//           timeSignature);
//     } // for
  }

  --gIndenter;
}

void msrStaff::appendClefKeyTimeSignatureGroupToStaffClone (
  const S_msrClefKeyTimeSignatureGroup& clefKeyTimeSignatureGroup){
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending clefKeyTimeSignatureGroup '" <<
      clefKeyTimeSignatureGroup->asString () <<
      "' to staff clone \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set staff time signature
  fCurrentClefKeyTimeSignatureGroup = clefKeyTimeSignatureGroup;

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendClefKeyTimeSignatureGroupToVoice (
        clefKeyTimeSignatureGroup);
  } // for

  --gIndenter;
}

// void msrStaff::appendTimeSignatureToStaffClone (
//   const S_msrTimeSignature& timeSignature){
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTimeSignatures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending time signature " << timeSignature
//       " to staff clone \"" <<
//       fStaffName <<
//       "\" in part " <<
//       fStaffUpLinkToPart->fetchPartCombinedName () <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
// //   fCurrentClefKeyTimeSignatureGroup->
// //     setTimeSignature (fCurrentTimeSignature);
//
//   // set staff time signature
//   fStaffCurrentTimeSignature = timeSignature;
//
//   // cascade it to all voices
//   for (S_msrVoice voice : fStaffAllVoicesList) {
//     voice->
//       appendTimeSignatureToVoice (timeSignature);
//   } // for
//
//   --gIndenter;
// }

void msrStaff::appendTempoToStaff (
  const S_msrTempo& tempo)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Appending tempo " << tempo->asString () <<
      " to staff " <<
      fStaffName;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append tempo to registered staves
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendTempoToVoice (tempo);
  } // for
}

void msrStaff::appendRehearsalMarkToStaff (
  const S_msrRehearsalMark& rehearsalMark)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRehearsalMarks ()) {
    std::stringstream ss;

    ss <<
      "Appending rehearsal mark " << rehearsalMark->asString () <<
      " to staff " <<
      fStaffName;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append rehearsal mark to registered staves
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendRehearsalMarkToVoice (rehearsalMark);
  } // for
}

void msrStaff::appendLineBreakToStaff  (
  const S_msrLineBreak& lineBreak)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLineBreaks ()) {
    std::stringstream ss;

    ss <<
      "Appending line break " << lineBreak->asString () <<
      " to staff " <<
      fStaffName;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append line break to registered staves
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendLineBreakToVoice (lineBreak);
  } // for
}

void msrStaff::appendPageBreakToStaff (
  const S_msrPageBreak& pageBreak)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePageBreaks ()) {
    std::stringstream ss;

    ss <<
      "Appending page break " << pageBreak->asString () <<
      " to staff " <<
      fStaffName;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append page break to registered staves
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendPageBreakToVoice (pageBreak);
  } // for
}

void msrStaff::insertHiddenMeasureAndBarLineInStaffClone (
  int                  inputLineNumber,
  const msrWholeNotes& measurePosition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Inserting hidden measure and barLine at position " <<
      measurePosition.asString () <<
      "' in staff clone \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Nesting contents into new repeat in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat end in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending start in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  const std::string&    repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
                    repeatEndingKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending end in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Finalizing a repeat upon its end in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a measures repeat from it's first measure in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending the pending measures repeat to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendPendingMeasureRepeatToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::appendMultiMeasureRestToStaff (
  int inputLineNumber,
  int multiMeasureRestsMeasuresNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending a multi-measure rest for " <<
      mfSingularOrPlural (
        multiMeasureRestsMeasuresNumber, "measure", "measures") <<
      " to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffContainsMultiMeasureRests = true;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendMultiMeasureRestToVoice (
        inputLineNumber,
        multiMeasureRestsMeasuresNumber);
  } // for
}

void msrStaff::replicateLastAppendedMeasureInStaff (
  int inputLineNumber,
  int replicatasNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Replicating last appended measure in staff \"" <<
      fStaffName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // add multiple rest to all staves
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      replicateLastAppendedMeasureInVoice (
        inputLineNumber,
        replicatasNumber);
  } // for
}

void msrStaff::appendEmptyMeasuresToStaff (
  int           inputLineNumber,
  const std::string& previousMeasureNumber,
  int           emptyMeasuresNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Adding " <<
      mfSingularOrPlural (
        emptyMeasuresNumber,
        "empty measure", "empty measures") <<
      " to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffContainsMultiMeasureRests = true;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendEmptyMeasuresToVoice (
        inputLineNumber,
        previousMeasureNumber,
        emptyMeasuresNumber);
  } // for
}

void msrStaff::appendPendingMultiMeasureRestsToStaff (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending the pending multiple rest to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendPendingMultiMeasureRestsToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::appendMultiMeasureRestCloneToStaff (
  int                          inputLineNumber,
  const S_msrMultiMeasureRest& multiMeasureRests)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending multiple rest '" <<
      multiMeasureRests->asString () <<
      "' to staff clone \"" <<
      fStaffName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendMultiMeasureRestCloneToVoiceClone (
        inputLineNumber,
        multiMeasureRests);
  } // for
}

void msrStaff::appendRepeatCloneToStaff (
  int                inputLineNumber,
  const S_msrRepeat& repeatCLone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat clone to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendRepeatCloneToVoiceClone (
        inputLineNumber, repeatCLone);
  } // for
}

void msrStaff::appendRepeatEndingCloneToStaff (
  const S_msrRepeatEnding& repeatEndingClone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending a repeat ending clone to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName () <<
      ", line " << repeatEndingClone->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendRepeatEndingCloneToVoice (
        repeatEndingClone);
  } // for

  --gIndenter;
}

void msrStaff::appendBarLineToStaff (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Appending barLine " << barLine->asString () <<
      " to staff " <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendBarLineToVoice (barLine);
  } // for

  --gIndenter;
}

void msrStaff::appendTranspositionToStaff (
  const S_msrTransposition& transposition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTranspositions ()) {
    std::stringstream ss;

    ss <<
      "Setting transposition '" <<
      transposition->asString () <<
      "' in staff " <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set staff transposition
  Bool doAppendTransposeToStaff (true);

  if (! fStaffCurrentTransposition) {
    doAppendTransposeToStaff = true; // JMI ???
  }

  else {
    if (transposition->isEqualTo (fStaffCurrentTransposition)) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTranspositions ()) {
        std::stringstream ss;

        ss <<
          "Transpose '" <<
          transposition->asString () <<
          "' ignored because it is already present in staff " <<
          fStaffName <<
          "\" in part " <<
          fStaffUpLinkToPart->fetchPartCombinedName ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesDetails ()) {
    std::stringstream ss;

    ss <<
      "Appending staff details '" <<
      staffDetails->asShortString () <<
      "' to staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    staffDetails != nullptr,
    "staffDetails is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // register staff details in staff
  fCurrentStaffStaffDetails = staffDetails;

  // set staff kind accordingly if relevant
  switch (staffDetails->getStaffLinesNumber ()) {
    case 1:
      if (gMsrOahGroup->getCreateSingleLineStavesAsRythmic ()) {
        fStaffKind = msrStaffKind::kStaffKindRythmic;
      }
      else {
        fStaffKind = msrStaffKind::kStaffKindDrum;
      }
      break;
    default:
      ;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Setting staff kind to '" <<
      msrStaffKindAsString (fStaffKind) <<
      "' in staff \"" <<
      fStaffName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendStaffDetailsToVoice (staffDetails);
  } // for
}

void msrStaff::appendTranspositionToAllStaffVoices (
  const S_msrTransposition& transposition)
{
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendTranspositionToVoice (transposition);
  } // for
}

void msrStaff::appendScordaturaToStaff (
  const S_msrScordatura& scordatura)
{
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendScordaturaToVoice (
        scordatura);
  } // for
}

void msrStaff::appendAccordionRegistrationToStaff (
  const S_msrAccordionRegistration&
    accordionRegistration)
{
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendAccordionRegistrationToVoice (
        accordionRegistration);
  } // for
}

void msrStaff::appendHarpPedalsTuningToStaff (
  const S_msrHarpPedalsTuning& harpPedalsTuning)
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Finalizing last appended measure in staff \"" <<
      fStaffName <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

bool msrStaff::compareStavesByIncreasingNumber (
  const S_msrStaff& first,
  const S_msrStaff& second)
{
  return
    first->fStaffNumber
      <
    second->fStaffNumber;
}

bool msrStaff::compareStavesToHaveFiguredBassesBelowCorrespondingPart (
  const S_msrStaff& first,
  const S_msrStaff& second)
{
  int
    firstStaffNumber =
      first->fStaffNumber,
    secondStaffNumber =
      second->fStaffNumber;

  if (firstStaffNumber > K_PART_FIGURED_BASS_STAFF_NUMBER) {
    firstStaffNumber -= K_PART_FIGURED_BASS_STAFF_NUMBER + 1;
  }
  if (secondStaffNumber > K_PART_FIGURED_BASS_STAFF_NUMBER) {
    secondStaffNumber -= K_PART_FIGURED_BASS_STAFF_NUMBER + 1;
  }

  bool result =
    firstStaffNumber > secondStaffNumber;

  return result;
}

void msrStaff::finalizeStaff (int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Finalizing staff \"" <<
      fStaffName << "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // finalize the voices
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Finalizing the voices in staff \"" <<
      fStaffName << "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Collecting measures from the staff voices into staff \"" <<
      fStaffName <<
      "'s measures flat list vector" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrVoice voice : fStaffAllVoicesList) {
    S_msrVoice foo = voice; // ??? JMI v0.9.66
  } // for
}

void msrStaff::collectStaffMeasuresSlices (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    size_t
      staffVoiceNumbersToAllVoicesMapSize =
        fStaffVoiceNumbersToAllVoicesMap.size ();

    std::stringstream ss;

    ss <<
      "Collecting the measures slices of staff \"" <<
      fStaffName <<
      "\", " <<
      mfSingularOrPluralWithoutNumber (
        staffVoiceNumbersToAllVoicesMapSize, "there is", "there are") <<
      ' ' <<
      mfSingularOrPlural (
        staffVoiceNumbersToAllVoicesMapSize, "voice", "voices") <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
    mfAssert (
      __FILE__, __LINE__,
      fStaffMeasuresSlicesSequence == nullptr,
      "fStaffMeasuresSlicesSequence is not null");
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // create the staff measures slices sequence
  fStaffMeasuresSlicesSequence =
    msrMeasuresSlicesSequence::create (
      fStaffName); // origin

  // populate it
  for (S_msrVoice voice : fStaffAllVoicesList) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresSlices ()) {
      std::stringstream ss;

      ss <<
        "---> voice \"" <<
        voice->getVoiceName () <<
        "\":";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    ++gIndenter;

    // get the voice measures slices sequence
    S_msrMeasuresSlicesSequence
      voiceMeasuresSlicesSequence =
        voice->
          getVoiceMeasuresSlicesSequence ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresSlices ()) {
      std::stringstream ss;

      ss <<
        "---> voiceMeasuresSlicesSequence";

      if (voiceMeasuresSlicesSequence) {
        gLog <<
          std::endl;
        ++gIndenter;
        gLog <<
          voiceMeasuresSlicesSequence;
        --gIndenter;
      }
      else {
        gLog << " : [NULL]" << std::endl;
      }
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "---> staffMeasuresSlicesSequence:" <<
      std::endl;

    ++gIndenter;
    gLog <<
      fStaffMeasuresSlicesSequence;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrStaff::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStaff::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStaff::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStaff::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStaff::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStaff::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
  if (fStaffTuningsList.size ()) {
    for (
      std::list <S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin ();
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

#ifdef MF_TRACE_IS_ENABLED // JMI
  if (gMsrOahGroup->getTraceMsrVisitors ()) { // JMI TEMP
    gLog <<
      "++++++++ fStaffAllVoicesList.size(): " <<
      fStaffAllVoicesList.size () <<
      std::endl;

    if (fStaffAllVoicesList.size ()) {
      for (S_msrVoice voice : fStaffAllVoicesList) {
        gLog <<
          std::endl <<
          "+++++++++ voice: ++++++++" <<
          " \"" << voice->getVoiceName () << "\"" <<
          std::endl;
      } // for
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (fStaffAllVoicesList.size ()) {
    for (S_msrVoice voice : fStaffAllVoicesList) {
      // get the  set of voices to ignore
      S_oahStringSetElementAtom
        ignoreMsrVoicesSetAtom =
          gGlobalMsr2msrOahGroup->
            getIgnoreMsrVoicesSetAtom ();;

      Bool
        ignoreMsrVoicesSetIsASelected =
          ignoreMsrVoicesSetAtom->
            getSelected ();;

      // get the  set of voices to keep
      S_oahStringSetElementAtom
        keepMsrVoicesSetAtom =
          gGlobalMsr2msrOahGroup->
            getKeepMsrVoicesSetAtom ();;

      Bool
        keepMsrVoicesSetIsASelected =
          keepMsrVoicesSetAtom->
            getSelected ();;

      // JMI this should be done in mxsr2msrOahGroup::checkGroupOptionsConsistency () v0.9.62
      if (ignoreMsrVoicesSetIsASelected && keepMsrVoicesSetIsASelected) {
        std::stringstream ss;

        ss <<
          "Options " <<
          ignoreMsrVoicesSetAtom->fetchNamesBetweenQuotes () <<
          " and " <<
          keepMsrVoicesSetAtom->fetchNamesBetweenQuotes () <<
          " are incompatible" <<
          std::endl;

        msrError (
          gServiceRunData->getInputSourceName (),
          voice->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }

      const std::set <std::string>&
        ignoreMsrVoicesSet =
          gGlobalMsr2msrOahGroup->
            getIgnoreMsrVoicesSet ();

#ifdef MF_TRACE_IS_ENABLED // JMI
      if (gTraceOahGroup->getTraceVoices ()) {
        mfDisplayStringSet (
          "ignoreMsrVoicesSet",
          ignoreMsrVoicesSet,
          gLog);
      }
#endif // MF_TRACE_IS_ENABLED

      const std::set <std::string>&
        keepMsrVoicesSet =
          gGlobalMsr2msrOahGroup->
            getKeepMsrVoicesSet ();

#ifdef MF_TRACE_IS_ENABLED // JMI
      if (gTraceOahGroup->getTraceVoices ()) {
        mfDisplayStringSet (
          "keepMsrVoicesSet",
          keepMsrVoicesSet,
          gLog);
      }
#endif // MF_TRACE_IS_ENABLED

      // a voice is to be browsed by default
      Bool voiceIsToBeBrowsed (true);

      std::string
        voiceName =
          voice->
            getVoiceName ();

      // is voiceName in the ignore voices set?
      if (ignoreMsrVoicesSetIsASelected) {
        // a voice is to be browsed by default
        if (
          mfStringIsInStringSet (
            voiceName,
            ignoreMsrVoicesSet)
        ) {
          voiceIsToBeBrowsed = false;
        }
      }

      if (keepMsrVoicesSetIsASelected) {
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
#ifdef MF_TRACE_IS_ENABLED // JMI
        if (gTraceOahGroup->getTraceVoices ()) {
          std::stringstream ss;

          ss <<
            "Ignoring voice \"" <<
            voiceName <<
            "\"";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }
    } // for
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrStaff::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

std::string msrStaff::asString () const
{
  std::stringstream ss;

  ss <<
    "[Staff \"" << fStaffName <<
    "\", fStaffKind: " <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    "\", fStaffNumber: " <<
    fStaffNumber <<
    ", fStaffAllVoicesList.size(): " << fStaffAllVoicesList.size () <<
    ", fStaffRegularVoicesCounter: " << fStaffRegularVoicesCounter <<
    ']' <<
    std::endl;

  return ss.str ();
}

void msrStaff::printFull (std::ostream& os) const
{
  os <<
    "[Staff printfull()\"" << fStaffName <<
    "\", fStaffAlphabeticName: " << fStaffAlphabeticName <<
    "\", fStaffKind: " <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    "\", fStaffNumber: " <<
    fStaffNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 38;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffAllVoicesList.size()" << ": " << fStaffAllVoicesList.size () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStaffRegularVoicesCounter" << ": " << fStaffRegularVoicesCounter <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffUpLinkToPart" << ": " <<
    fStaffUpLinkToPart->fetchPartCombinedName () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffInstrumentName" << ": \"" <<
    fStaffInstrumentName <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fStaffInstrumentAbbreviation" << ": \"" <<
    fStaffInstrumentAbbreviation <<
    std::endl;

  // staff shortest note
  os << std::left <<
    std::setw (fieldWidth) << "fStaffShortestNoteWholeNotes" << ": " <<
    fStaffShortestNoteWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) << "fStaffShortestNoteTupletFactor" << ": " <<
    fStaffShortestNoteTupletFactor <<
    std::endl;

  // print current the staff clef if any
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefs ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fStaffCurrentClef" << ": ";

    if (fStaffCurrentClef) {
      os <<
        '\'' <<
        fStaffCurrentClef->asShortString () <<
        "'";
    }
    else {
      os << "[NULL]";
    }

    os << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // print the current staff key if any
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fStaffCurrentKey" << ": ";

    if (fStaffCurrentKey) {
      os <<
        '\'' <<
        fStaffCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "[NULL]";
    }

    os << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // print the current staff time signature if any
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fStaffCurrentTimeSignature" << ": ";

    if (fStaffCurrentTimeSignature) {
      os <<
        '\'' <<
        fStaffCurrentTimeSignature->asShortString () <<
        "'";
    }
    else {
      os <<
        "[NULL]";
    }

    os << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesDetails ()) {
    // print the staff details if any
    if (fCurrentStaffStaffDetails) {
      os <<
        fCurrentStaffStaffDetails;
    }
    else {
      os << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentStaffStaffDetails" <<
        ": " << "[NULL]";
    }
    os << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // print the staff 'all voices' list
  os << std::left <<
    std::setw (fieldWidth) <<
    "Voice names in fStaffAllVoicesList" << ": ";

  if (fStaffAllVoicesList.size ()) {
    os << std::endl;
    ++gIndenter;

    for (S_msrVoice voice : fStaffAllVoicesList) {
      os << "\"" << voice->getVoiceName () << "\"" << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  // print the staff 'regular voices' list
  os << std::left <<
    std::setw (fieldWidth) <<
    "Voice names in fStaffRegularVoicesList" << ": ";

  if (fStaffRegularVoicesList.size ()) {
    os << std::endl;

    ++gIndenter;

    int voiceNumber = 0;

    for (S_msrVoice voice : fStaffRegularVoicesList) {
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      ++voiceNumber;

      os <<
        voiceNumber <<
        ": " <<
        voice->asShortString () <<
        std::endl;

      gIndenter.increment (3);

      os <<
//         "voiceKind: " << JMI
//         msrVoiceKindAsString (voice->getVoiceKind ()) <<
        "getRegularVoiceStaffSequentialNumber: " <<
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

  // print the staff 'voice numbers to all voices' map
  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffVoiceNumbersToAllVoicesMap" << ": ";

  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    os << std::endl;

    ++gIndenter;

    for (std::pair <int, S_msrVoice> thePair : fStaffVoiceNumbersToAllVoicesMap) {
      int        voiceNumber = thePair.first;
      S_msrVoice voice       = thePair.second;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      os <<
        voiceNumber <<
        ": " <<
        voice->asShortString () <<
        std::endl;

      gIndenter.increment (3);

      os <<
//         "voiceKind: " << JMI
//         msrVoiceKindAsString (voice->getVoiceKind ()) <<
        "getRegularVoiceStaffSequentialNumber: " <<
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

  // print the staff 'voice numbers to regular voices' map
  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffVoiceNumbersToRegularVoicesMap" << ": ";

  if (fStaffVoiceNumbersToRegularVoicesMap.size ()) {
    os << std::endl;

    ++gIndenter;

    for (std::pair <int, S_msrVoice> thePair : fStaffVoiceNumbersToRegularVoicesMap) {
      int        voiceNumber = thePair.first;
      S_msrVoice voice       = thePair.second;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      os <<
        voiceNumber <<
        ": " <<
        "Voice \"" << voice->getVoiceName () <<
        "\", line " << voice->getInputStartLineNumber () <<
        std::endl;

      gIndenter.increment (3);

      os <<
//        "voiceKind: " << JMI
//        msrVoiceKindAsString (voice->getVoiceKind ()) <<
        "getRegularVoiceStaffSequentialNumber: " <<
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

  // print the staff measures flat list vector
  size_t staffMeasuresFlatListsVectorSize =
    fStaffMeasuresFlatListsVector.size ();

  os <<
    std::setw (fieldWidth) <<
    "fStaffMeasuresFlatListsVector" << ": ";
  if (staffMeasuresFlatListsVectorSize) {
    os <<
      staffMeasuresFlatListsVectorSize << " elements";

    ++gIndenter;

    std::vector <std::list <S_msrMeasure>>::const_iterator
      iBegin = fStaffMeasuresFlatListsVector.begin (),
      iEnd   = fStaffMeasuresFlatListsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      const std::list <S_msrMeasure>&
        measuresList = (*i);

      // print the measurs list
      if (staffMeasuresFlatListsVectorSize) {
        ++gIndenter;

        std::list <S_msrMeasure>::const_iterator
          iBegin = measuresList.begin (),
          iEnd   = measuresList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrMeasure
            measure = (*i);

          // print the measure
          os << (*i)->asStringForMeasuresSlices ();

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
    "fStaffMeasuresSlicesSequence" << ": ";

  if (fStaffMeasuresSlicesSequence) {
    os << std::endl;
    ++gIndenter;
    os << fStaffMeasuresSlicesSequence;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }
  os << std::endl;

  // print the  voices
  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    std::map <int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumbersToAllVoicesMap.begin (),
      iEnd   = fStaffVoiceNumbersToAllVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

      voice->printFull (os);

      if (++i == iEnd) break;

      os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrStaff::print (std::ostream& os) const
{
  os <<
    "[Staff \"" << fStaffName <<
    "\", fStaffKind: " <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffAlphabeticName \"" << ": \"" << fStaffAlphabeticName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffNumber" << ": " << fStaffNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffAllVoicesList.size()" << ": " << fStaffAllVoicesList.size () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffRegularVoicesCounter" << ": " << fStaffRegularVoicesCounter <<
    std::endl;

  // print the  voices
  if (fStaffVoiceNumbersToAllVoicesMap.size ()) {
    os << std::endl;

    std::map <int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceNumbersToAllVoicesMap.begin (),
      iEnd   = fStaffVoiceNumbersToAllVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

      os << voice;

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
      "fStaff tunings list:" <<
      std::endl;

    std::list <S_msrStaffTuning>::const_iterator
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

    std::list <S_msrVoice>::const_iterator
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
//     std::map <int, S_msrVoice>::const_iterator
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
