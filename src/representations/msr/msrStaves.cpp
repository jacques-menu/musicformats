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
// #include "mfConstants.h"

#include "mfAssert.h"
// #include "mfConstants.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrBarLines.h"
#include "msrLineBreaks.h"
#include "msrPageBreaks.h"
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
int msrStaff::sStaffMaxRegularVoices = 4; // JMI TEMP MOD 0.9.70

S_msrStaff msrStaff::create (
  const mfInputLineNumber& inputLineNumber,
  msrStaffKind     staffKind,
  int              staffNumber)
{
  msrStaff* obj =
    new msrStaff (
      inputLineNumber,
      staffKind,
      staffNumber);
  assert (obj != nullptr);
  return obj;
}

S_msrStaff msrStaff::create (
  const mfInputLineNumber& inputLineNumber,
  msrStaffKind     staffKind,
  int              staffNumber,
  const S_msrPart& staffUpLinkToPart)
{
  msrStaff* obj =
    new msrStaff (
      inputLineNumber,
      staffKind,
      staffNumber);
  assert (obj != nullptr);

  // set staff uplink to part
  obj->setStaffUpLinkToPart (staffUpLinkToPart);

  return obj;
}

msrStaff::msrStaff (
  const mfInputLineNumber& inputLineNumber,
  msrStaffKind     staffKind,
  int              staffNumber)
    : msrElement (inputLineNumber)
{
  // set staff kind and number
  fStaffKind = staffKind;
  fStaffNumber = staffNumber;

  // do other initializations
  initializeStaff ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Creating staff \"" << asString () <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

S_msrStaff msrStaff::createStaffNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of staff \"" <<
      fStaffPathLikeName <<
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
    "containingPart is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrStaff
    newbornClone =
      msrStaff::create (
        fInputLineNumber,
        fStaffKind,
        fStaffNumber,
        containingPart);

  newbornClone->fStaffPathLikeName =
    fStaffPathLikeName;

  newbornClone->fStaffNumber =
    fStaffNumber;

  newbornClone->fStaffInstrumentName =
    fStaffInstrumentName;

  newbornClone->fStaffInstrumentAbbreviation =
    fStaffInstrumentAbbreviation;

  return newbornClone;
}

msrStaff::~msrStaff ()
{}

void msrStaff::initializeStaff ()
{
  fStaffRegularVoicesCounter = 0;

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
          fInputLineNumber,
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
    mfWholeNotes (INT_MAX, 1);
  fStaffShortestNoteTupletFactor =
    mfRational (1, 1);

  --gIndenter;
}

void msrStaff::setStaffUpLinkToPart (const S_msrPart& part)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    part != nullptr,
    "part is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fStaffUpLinkToPart = part;

  // copy stuff
  copyStuffFromUpLinkToPartToStaff ();
}

void msrStaff::copyStuffFromUpLinkToPartToStaff ()
{
  // set staff names
  switch (fStaffKind) {
    case msrStaffKind::kStaffKindRegular:
      fStaffPathLikeName =
        fStaffUpLinkToPart->getPartPathLikeName () +
        "_Staff_" +
        mfIntToEnglishWord (fStaffNumber);
      break;

    case msrStaffKind::kStaffKindTablature:
      fStaffPathLikeName =
        fStaffUpLinkToPart->getPartPathLikeName () +
        "_TABLATURE_Staff";
      break;

    case msrStaffKind::kStaffKindHarmonies:
      fStaffPathLikeName =
        fStaffUpLinkToPart->getPartPathLikeName () +
        "_HARMONIES_Staff";
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      fStaffPathLikeName =
        fStaffUpLinkToPart->getPartPathLikeName () +
        "_FIGURED_BASS_Staff";
      break;

    case msrStaffKind::kStaffKindDrum:
      fStaffPathLikeName =
        fStaffUpLinkToPart->getPartPathLikeName () +
        "_DRUM_Staff";
      break;

    case msrStaffKind::kStaffKindRythmic:
      fStaffPathLikeName =
        fStaffUpLinkToPart->getPartPathLikeName () +
        "_RYTHMIC_Staff";
      break;
  } // switch

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
          fStaffPathLikeName <<
          "\" in part " <<
          fStaffUpLinkToPart->fetchPartNameForTrace ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      appendClefToStaff (
        0, // JMI 0.9.67, groupInputLineNumber
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
          fStaffPathLikeName <<
          "\" in part " <<
          fStaffUpLinkToPart->fetchPartNameForTrace ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      appendKeyToStaff (
        0, // JMI 0.9.67, groupInputLineNumber
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
          fStaffPathLikeName <<
          "\" in part " <<
          fStaffUpLinkToPart->fetchPartNameForTrace ();

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

  // multiple measure rests
  fStaffContainsMultipleMeasureRests = false;
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
        "The new shortest note in staff \"" << fStaffPathLikeName << "\"" <<
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

  // cascade it to all voices
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
  const mfInputLineNumber& inputLineNumber,
  int                    previousMeasureEndInputLineNumber,
  const std::string&     measureNumber,
  msrMeasureImplicitKind measureImplicitKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to staff \"" << fStaffPathLikeName << "\"" <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
  if (! fStaffAllVoicesList.empty ()) {
    // cascade it to all voices
    for (S_msrVoice voice : fStaffAllVoicesList) {
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
    // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      voice->
        cascadeCreateAMeasureAndAppendItInVoice (
          inputLineNumber,
          previousMeasureEndInputLineNumber,
          measureNumber,
          measureImplicitKind);
    } // for
  }
}

void msrStaff::setNextMeasureNumberInStaff (
  const mfInputLineNumber& inputLineNumber,
  const std::string& nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      ", in staff \"" << fStaffPathLikeName << "\"" <<
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
      "voice is NULL");
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
  const mfInputLineNumber& inputLineNumber,
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
      "' of staff \"" << fStaffPathLikeName <<
      "\", line " << inputLineNumber <<
      "\", current measure number: " <<
      currentMeasureNumber;
 // JMI     " in part " << fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // are there too many regular voices in this staff?
  if (fStaffRegularVoicesCounter > msrStaff::sStaffMaxRegularVoices) {
    std::stringstream ss;

    ss <<
      "staff \"" << fStaffPathLikeName <<
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
      fStaffRegularVoicesMap.find (voiceNumber);

  if (it != fStaffRegularVoicesMap.end ()) {
    // yes
    S_msrVoice
      olderVoice = (*it).second;

    std::stringstream ss;

    ss <<
      "attempt at creating a voice anew by number" <<
      ", voiceNumber: " << voiceNumber <<
      ", currentMeasureNumber: " << currentMeasureNumber <<
      " ' in staff \"" <<
      fStaffPathLikeName <<
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
          "' in staff \"" << fStaffPathLikeName <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffUpLinkToPart->fetchPartNameForTrace ();

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
          "' in staff \"" << fStaffPathLikeName <<
          "\", line " << inputLineNumber <<
          "\", current measure number: " <<
          currentMeasureNumber <<
     // JMI     " in part " << fStaffUpLinkToPart->fetchPartNameForTrace ();

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
  const mfWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "The new shortest note wholeNotes in staff \"" <<
        fStaffPathLikeName <<
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
        fStaffPathLikeName <<
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
      "\" in all voices list of staff " << fStaffPathLikeName <<
      " line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (! fStaffAllVoicesList.empty ()) {
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
          voice->getInputLineNumber (),
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
          voice->getInputLineNumber (),
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
  const mfInputLineNumber& inputLineNumber,
  int               staffNumber,
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
      "\" in staff " << fStaffPathLikeName <<
      " with number " << staffNumber <<
      " line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // register it in the 'all voices' list
  registerVoiceInStaffAllVoicesList (voice); // JMI 0.9.63 NASTY bug???

  // register voice in the 'numbers to all voices' map
  fStaffAllVoicesMap [voiceNumber] = voice;

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
      fStaffRegularVoicesMap [voiceNumber] = voice;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceVoices ()) {
        std::stringstream ss;

        ss <<
          "Sorting the voices in staff \"" <<
          fStaffPathLikeName << "\"" <<
          ", line " << inputLineNumber;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // sort fStaffAllVoicesList, to have harmonies
      // just before the corresponding voice // JMI, 0.9.73
      if (! fStaffAllVoicesList.empty ()) {
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
          fStaffPathLikeName << "\"" <<
          " with number " << staffNumber <<
          ", line " << inputLineNumber;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // sort fStaffAllVoicesList, to have figured bass elements
      // just after the corresponding voice
      if (! fStaffAllVoicesList.empty ()) {
        fStaffAllVoicesList.sort (
          msrVoice::compareVoicesToHaveFiguredBassesBelowCorrespondingVoice);
      }
      break;
  } // switch

  // register voice in its staff at the part level
  fStaffUpLinkToPart->
    registerVoiceInPartVoicesList (
//       inputLineNumber, // JMI 0.9.72
      voice);

  --gIndenter;
}

void msrStaff::registerRegularVoiceByItsNumber (
  const mfInputLineNumber& inputLineNumber,
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
      "\" in staff " << fStaffPathLikeName <<
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
  if (! fStaffAllVoicesList.empty ()) {
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
          regularVoice->getInputLineNumber (),
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
          regularVoice->getInputLineNumber (),
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
    fStaffNumber,
    regularVoice);

  --gIndenter;
}

void msrStaff::registerHarmoniesVoiceByItsNumber (
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering harmonies voice " <<
      voice->asString () <<
     " by its number in staff \"" << fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the harmonies voice by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    fStaffNumber,
    voice);
}

void msrStaff::registerFiguredBassVoiceByItsNumber (
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering figured bass voice " <<
      voice->asString () <<
     " by its number in staff \"" << fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the figured bass voice by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    fStaffNumber,
    voice);
}

S_msrVoice msrStaff::fetchRegularVoiceFromStaffByItsNumber (
  const mfInputLineNumber& inputLineNumber,
  int voiceNumber)
{
  S_msrVoice result; // JMI avoid repetitive messages! 0.9.66

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Fetching voice by its number " <<
      voiceNumber <<
     " in staff \"" << fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // search list ??? JMI
  for (std::pair <int, S_msrVoice> thePair : fStaffRegularVoicesMap) {
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
          "' in staff \"" << fStaffPathLikeName << "\"" <<
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
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Assigning sequential numbers to the regular voices in staff \"" <<
      fStaffPathLikeName <<
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

  if (! fStaffRegularVoicesList.empty ()) {
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
        "voice is NULL");
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
          fStaffPathLikeName <<
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
  const mfInputLineNumber& inputLineNumber)
{
  S_msrVoice result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Fetching first regular voice in staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fStaffRegularVoicesList.empty ()) {
    std::stringstream ss;

    ss <<
      "staff " << fStaffNumber <<
      " in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
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
        fStaffPathLikeName <<
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
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voice != nullptr,
    "voice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " <<
      fStaffRegularVoicesCounter <<
      " of staff \"" << fStaffPathLikeName <<
      "\", line " << inputLineNumber;
// JMI       " in part " << fStaffUpLinkToPart->fetchPartNameForTrace ();

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
          "staff \"" << fStaffPathLikeName <<
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
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voice != nullptr,
    "voice is NULL");
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
          "staff \"" << fStaffPathLikeName <<
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
      " in staff \"" << fStaffPathLikeName <<
      "\", line " << inputLineNumber;
// JMI       " in part " << fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register it in staff by its number
  registerVoiceByItsNumber (
    inputLineNumber,
    fStaffNumber,
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
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoice& voice)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voice != nullptr,
    "voice is NULL");
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
      " of staff clone \"" << fStaffPathLikeName <<
      "\", line " << inputLineNumber;
// JMI       " in part " << fStaffUpLinkToPart->fetchPartNameForTrace ();

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
          "staff clone \"" << fStaffPathLikeName <<
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
//       fStaffPathLikeName <<
//       "\" in part " <<
//       fStaffUpLinkToPart->fetchPartNameForTrace ();
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
//         fStaffPathLikeName <<
//         "\" in part " <<
//         fStaffUpLinkToPart->fetchPartNameForTrace ();
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
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

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
          clef->getInputLineNumber (),
          groupInputLineNumber);
    }

    // register clef in fCurrentClefKeyTimeSignatureGroup
    // before the latter is cascaded to the voices if just created JMI 0.9.70
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
    fStaffCurrentClef = clef; // JMI 0.9.67
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
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

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
            fStaffPathLikeName <<
            "\" in part " <<
            fStaffUpLinkToPart->fetchPartNameForTrace ();

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
          key->getInputLineNumber (),
          groupInputLineNumber);
    }

    // register key in fCurrentClefKeyTimeSignatureGroup
    // before the latter is cascaded to the voices if just created JMI 0.9.70
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
      fStaffPathLikeName <<
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
            fStaffPathLikeName <<
            "\" in part " <<
            fStaffUpLinkToPart->fetchPartNameForTrace ();

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
          timeSignature->getInputLineNumber (),
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
    // before the latter is cascaded to the voices if just created JMI 0.9.70
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
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

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
//       fStaffPathLikeName <<
//       "\" in part " <<
//       fStaffUpLinkToPart->fetchPartNameForTrace () <<
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
      fStaffPathLikeName;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append tempo to all voices
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
      fStaffPathLikeName;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append rehearsal mark to all voices
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
      fStaffPathLikeName;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append line break to all voices
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
      fStaffPathLikeName;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append page break to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendPageBreakToVoice (pageBreak);
  } // for
}

void msrStaff::insertHiddenMeasureAndBarLineInStaffClone (
  const mfInputLineNumber& inputLineNumber,
  const mfPositionInMeasure& positionInMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Inserting hidden measure and barLine at position " <<
      positionInMeasure.asString () <<
      "' in staff clone \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
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
        positionInMeasure);
  } // for

  --gIndenter;
}

/* JMI
void msrStaff::nestContentsIntoNewRepeatInStaff (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Nesting contents into new repeat in staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      nestContentsIntoNewRepeatInVoice (
        inputLineNumber);
  } // for
}
*/

void msrStaff::cascadeHandleRepeatStartInStaff (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start in staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
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
      handleRepeatStartInVoice (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrStaff::cascadeHandleRepeatEndInStaff (
  const mfInputLineNumber& inputLineNumber,
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat end in staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
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
      handleRepeatEndInVoice (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  --gIndenter;
}

void msrStaff::cascadeHandleRepeatEndingStartInStaff (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending start in staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
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
      handleRepeatEndingStartInVoice (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrStaff::cascadeHandleRepeatEndingEndInStaff (
  const mfInputLineNumber& inputLineNumber,
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
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
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
      handleRepeatEndingEndInVoice (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for

  --gIndenter;
}

/* JMI
void msrStaff::finalizeRepeatEndInStaff (
  const mfInputLineNumber& inputLineNumber,
  const std::string& measureNumber,
  int           repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Finalizing a repeat upon its end in staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
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
      finalizeRepeatEndInVoice (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  --gIndenter;
}
*/

void msrStaff::cascadeCreateAMeasureRepeatAndAppendItToStaff (
  const mfInputLineNumber& inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashesNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a measures repeat from it's first measure in staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      cascadeCreateAMeasureRepeatAndAppendItToVoice (
        inputLineNumber,
        measureRepeatMeasuresNumber,
        measureRepeatSlashesNumber);
  } // for
}

void msrStaff::appendPendingMeasureRepeatToStaff (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending the pending measures repeat to staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendPendingMeasureRepeatToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::cascadeAppendMultipleMeasureRestToStaff (
  const mfInputLineNumber& inputLineNumber,
  int               multipleMeasureRestMeasuresNumber,
  int               multipleMeasureRestSlashesNumber,
  msrUseSymbolsKind multipleMeasureRestUseSymbolsKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending a multiple measure rest for " <<
      mfSingularOrPlural (
        multipleMeasureRestMeasuresNumber, "measure", "measures") <<
      " to staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffContainsMultipleMeasureRests = true;

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      createAMultipleMeasureRestAndAppendItToVoice (
        inputLineNumber,
        multipleMeasureRestMeasuresNumber,
        multipleMeasureRestSlashesNumber,
        multipleMeasureRestUseSymbolsKind);
  } // for
}

void msrStaff::replicateLastAppendedMeasureInStaff (
  const mfInputLineNumber& inputLineNumber,
  int replicatasNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Replicating last appended measure in staff \"" <<
      fStaffPathLikeName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // add multiple rest to all staves
  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      replicateLastAppendedMeasureInVoice (
        inputLineNumber,
        replicatasNumber);
  } // for
}

void msrStaff::appendEmptyMeasuresToStaff (
  const mfInputLineNumber& inputLineNumber,
  const std::string& previousMeasureNumber,
  int           emptyMeasuresNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Adding " <<
      mfSingularOrPlural (
        emptyMeasuresNumber,
        "empty measure", "empty measures") <<
      " to staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffContainsMultipleMeasureRests = true;

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendEmptyMeasuresToVoice (
        inputLineNumber,
        previousMeasureNumber,
        emptyMeasuresNumber);
  } // for
}

void msrStaff::appendPendingMultipleMeasureRestsToStaff (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending the pending multiple rest to staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendPendingMultipleMeasureRestsToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::appendMultipleMeasureRestCloneToStaff (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMultipleMeasureRest& multipleMeasureRests)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending multiple rest '" <<
      multipleMeasureRests->asString () <<
      "' to staff clone \"" <<
      fStaffPathLikeName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendMultipleMeasureRestCloneToVoiceClone (
        inputLineNumber,
        multipleMeasureRests);
  } // for
}

void msrStaff::appendRepeatCloneToStaff (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat& repeatCLone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat clone to staff \"" <<
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
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
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
      ", line " << repeatEndingClone->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade it to all voices
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
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade it to all voices
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
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

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
          fStaffPathLikeName <<
          "\" in part " <<
          fStaffUpLinkToPart->fetchPartNameForTrace ();

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
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

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
    "staffDetails is NULL");
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
      fStaffPathLikeName <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendStaffDetailsToVoice (staffDetails);
  } // for
}

void msrStaff::appendTranspositionToAllStaffVoices (
  const S_msrTransposition& transposition)
{
  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendTranspositionToVoice (transposition);
  } // for
}

void msrStaff::appendScordaturaToStaff (
  const S_msrScordatura& scordatura)
{
  // cascade it to all voices
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
  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendAccordionRegistrationToVoice (
        accordionRegistration);
  } // for
}

void msrStaff::appendHarpPedalsTuningToStaff (
  const S_msrHarpPedalsTuning& harpPedalsTuning)
{
  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    voice->
      appendHarpPedalsTuningToVoice (
        harpPedalsTuning);
  } // for
}

void msrStaff::finalizeLastAppendedMeasureInStaff (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Finalizing last appended measure in staff \"" <<
      fStaffPathLikeName <<
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

void msrStaff::finalizeStaff (const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Finalizing staff \"" <<
      fStaffPathLikeName << "\"" <<
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
      fStaffPathLikeName << "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
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
  const mfInputLineNumber& inputLineNumber)
{
  // collect measures from the staff voices
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Collecting measures from the staff voices into staff \"" <<
      fStaffPathLikeName <<
      "'s measures flat list vector" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // cascade it to all voices
  for (S_msrVoice voice : fStaffAllVoicesList) {
    S_msrVoice foo = voice; // ??? JMI 0.9.66
  } // for
}

void msrStaff::collectStaffMeasuresSlices (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    size_t
      StaffAllVoicesMapSize =
        fStaffAllVoicesMap.size ();

    std::stringstream ss;

    ss <<
      "Collecting the measures slices of staff \"" <<
      fStaffPathLikeName <<
      "\", " <<
      mfSingularOrPluralWithoutNumber (
        StaffAllVoicesMapSize, "there is", "there are") <<
      ' ' <<
      mfSingularOrPlural (
        StaffAllVoicesMapSize, "voice", "voices") <<
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
      fStaffPathLikeName); // origin

  // populate it
  // cascade it to all voices
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
            fStaffPathLikeName,
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
  if ! (fStaffTuningsList.empty ()) {
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
  if (f! StaffAllVoicesMap.empty ()) {
    // cascade it to all voices
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

    if (! fStaffAllVoicesList.empty ()) {
      // cascade it to all voices
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

  if (! fStaffAllVoicesList.empty ()) {
    // cascade it to all voices
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

      // JMI this should be done in mxsr2msrOahGroup::checkGroupOptionsConsistency () 0.9.62
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
          voice->getInputLineNumber (),
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
    "[Staff \"" << fStaffPathLikeName <<
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

void msrStaff::print (std::ostream& os) const
{
  os <<
    "[Staff \"" << fStaffPathLikeName <<
    "\", fStaffKind: " <<
    msrStaffKindAsStringForPrint (fStaffKind) <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffNumber" << ": " << fStaffNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fStaffRegularVoicesCounter" << ": " << fStaffRegularVoicesCounter <<
    std::endl;

  os << std::endl;

  // print the  voices
  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffAllVoicesMap.size()" << ": " << fStaffAllVoicesMap.size () <<
    std::endl <<

    "fStaffAllVoicesMap" << ": ";
  if (! fStaffAllVoicesMap.empty ()) {
    os << std::endl;

    ++gIndenter;

    std::map <int, S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesMap.begin (),
      iEnd   = fStaffAllVoicesMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

      os << voice;

      if (++i == iEnd) break;

      os << std::endl;
    } // for

    --gIndenter;
  }

  else {
    os << "[EMPTY]";
  }

  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffAllVoicesList.size()" << ": " << fStaffAllVoicesList.size () <<
    std::endl <<

    "fStaffAllVoicesList" << ": ";
  if (! fStaffAllVoicesList.empty ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesList.begin (),
      iEnd   = fStaffAllVoicesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << voice;

      if (++i == iEnd) break;

      os << std::endl;
    } // for

    --gIndenter;
  }

  else {
    os << "[EMPTY]";
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrStaff::printFull (std::ostream& os) const
{
  os <<
    "[Staff " << fStaffPathLikeName <<
    ", fStaffKind: " <<
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
    fStaffUpLinkToPart->fetchPartNameForTrace () <<
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

  if (! fStaffAllVoicesList.empty ()) {
    os << std::endl;
    ++gIndenter;

    // cascade it to all voices
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

  if (! fStaffRegularVoicesList.empty ()) {
    os << std::endl;

    ++gIndenter;

    int voiceNumber = 0;

    for (S_msrVoice voice : fStaffRegularVoicesList) {
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is NULL");
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
    "fStaffAllVoicesMap" << ": ";

  if (! fStaffAllVoicesMap.empty ()) {
    os << std::endl;

    ++gIndenter;

    for (std::pair <int, S_msrVoice> thePair : fStaffAllVoicesMap) {
      int        voiceNumber = thePair.first;
      S_msrVoice voice = thePair.second;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is NULL");
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
    "fStaffRegularVoicesMap" << ": ";

  if (! fStaffRegularVoicesMap.empty ()) {
    os << std::endl;

    ++gIndenter;

    for (std::pair <int, S_msrVoice> thePair : fStaffRegularVoicesMap) {
      int        voiceNumber = thePair.first;
      S_msrVoice voice = thePair.second;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        voice != nullptr,
        "voice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      os <<
        voiceNumber <<
        ": " <<
        "Voice \"" << voice->getVoiceName () <<
        "\", line " << voice->getInputLineNumber () <<
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
  if (! fStaffAllVoicesMap.empty ()) {
    std::map <int, S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesMap.begin (),
      iEnd   = fStaffAllVoicesMap.end (),
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

void msrStaff::printSummary (std::ostream& os) const
{
  os <<
    "[Staff \"" << fStaffPathLikeName <<
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
  if (! fStaffTuningsList.empty ()) {
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
  if (! fStaffAllVoicesList.empty ()) {
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

//   if (! fStaffAllVoicesMap.empty ()) {
//     std::map <int, S_msrVoice>::const_iterator
//       iBegin = fStaffAllVoicesMap.begin (),
//       iEnd   = fStaffAllVoicesMap.end (),
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
