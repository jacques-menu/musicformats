/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "msr2namesVisitor.h"
#include "msr2summaryVisitor.h"

#include "msrWae.h"


#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "msdlParser.h"

#include "msdl2msrOah.h"

#include "msrInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msrBook createBook ()
{
  // create the book
  S_msrBook
    theMsrBook =
      msrBook::create (
        __LINE__,
        "msrBook::create()");

  return theMsrBook;
}

//_______________________________________________________________________________
S_msrScore createScore ()
{
  // create the score
  S_msrScore
    theMsrScore =
      msrScore::create (
        __LINE__,
        "msrScore::create()");

  // create its identification
  S_msrIdentification
    identification =
      msrIdentification::create (
        __LINE__);

  theMsrScore->
    setIdentification (
      identification);

  // set the identification's work title
  identification->
    setIdentificationWorkCreditTypeTitle (
      __LINE__,
      "Mikrokosmos III Wandering");

  // append the identification's composer
  identification->
    appendComposer (
      __LINE__,
      "Zoltán Kodály");

  // set the identification's software
  identification->
    appendSoftware (
      __LINE__,
      "MSDL converter"); // JMI

  return theMsrScore;
}

//_______________________________________________________________________________
void displayMsrScore (
  const S_msrScore&  theMsrScore,
  const std::string& passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock =  clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    std::endl <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    std::endl <<
    separator <<
    std::endl << std::endl <<

    theMsrScore <<

    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
    passDescription,
    mfTimingItemKind::kOptional,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream ss;

      ss <<
        "gIndenter value after " << passDescription << ": " <<
        gIndenter.getIndentation ();

      msrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayMsrScoreFull (
  const S_msrScore&  theMsrScore,
  const std::string& passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    std::endl <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    ", " << gLanguage->fullVersion () <<
    std::endl <<
    separator <<
    std::endl << std::endl;

    theMsrScore->printFull (gLog);

  gLog <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
    passDescription,
    mfTimingItemKind::kOptional,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream ss;

      ss <<
        "gIndenter value after " << passDescription << ": " <<
        gIndenter.getIndentation ();

      msrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayMsrScoreSummary (
  S_msrScore           theMsrScore,
  const S_msrOahGroup& msrOpts,
  const std::string&   passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    std::endl <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    ", " <<
    gLanguage->summary () <<
    std::endl <<
    separator <<
    std::endl << std::endl;

  // create an msr2summaryVisitor visitor
  msr2summaryVisitor
    summaryVisitor (
      msrOpts);

  summaryVisitor.printSummaryFromMsrScore (
    theMsrScore);

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
    passDescription,
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMsrScoreNames (
  S_msrScore           theMsrScore,
  const S_msrOahGroup& msrOpts,
  const std::string&   passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    std::endl <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    std::endl <<
    separator <<
    std::endl << std::endl;

  // create an msr2namesVisitor visitor
  msr2namesVisitor
    namesVisitor (
      msrOpts);

  namesVisitor.printNamesFromMsrScore (
    theMsrScore);

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
    passDescription,
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMsrScoreSlices (
  const S_msrScore&  theMsrScore,
  const std::string& passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    std::endl <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    ", " <<
    gLanguage->slices () <<
    std::endl <<
    separator <<
    std::endl << std::endl;

  theMsrScore->printSlices (gLog);

  gLog <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
    passDescription,
    mfTimingItemKind::kOptional,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream ss;

      ss <<
        "gIndenter value after " << passDescription << ": " <<
        gIndenter.getIndentation ();

      msrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
S_msrPart createPartInScore (
  const S_msrScore& theMsrScore)
{
  // create the part group
  S_msrPartGroup
    partGroup =
      msrPartGroup::create (
        __LINE__, 1, 1, "OnlyPartGroup", nullptr, theMsrScore);

  // add it to the score
  theMsrScore->
    addPartGroupToScore (
      partGroup);

  // create the part
  S_msrPart
    part =
      msrPart::create (
        __LINE__, "OnlyPart", partGroup);

  // append it to the part group
  partGroup->
    appendPartToPartGroup (
      part);

if (false) { // JMI TEMP
  part->
    setPartShortestNoteWholeNotes (
      msrWholeNotes (1, 16));
}

  return part;
}

//_______________________________________________________________________________
S_msrStaff createStaffInPart (
  int              staffNumber,
  const S_msrPart& part)
{
  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        __LINE__, msrStaffKind::kStaffKindRegular, staffNumber, part);

  // append it to the part
  part ->
    addStaffToPartCloneByItsNumber ( // JMI NOT clone???
      staff);

  return staff;
}

//_______________________________________________________________________________
S_msrVoice createRegularVoiceInStaff (
  int               inputLineNumber,
  int               voiceNumber,
  const S_msrStaff& staff)
{
  // create the functions voice
  S_msrVoice
    voice =
      msrVoice::createRegularVoice (
        inputLineNumber,
        voiceNumber,
        staff);

  // append it to the staff
  staff->
    registerVoiceInStaff (
      inputLineNumber,
      voice);

  return voice;
}

//_______________________________________________________________________________
S_msrMeasure createMeasureInVoice (
  int               inputLineNumber,
  std::string       measureNumber,
  const S_msrVoice& voice)
{
  return
    voice->
      createAMeasureAndAppendItToVoice (
        inputLineNumber,
        333, //         previousMeasureEndInputLineNumber, v0.9.62
        measureNumber,
        msrMeasureImplicitKind::kMeasureImplicitKindNo);
}

/*
  // measure 1
  // ----------------------------------

  {
    // create the measure

    std::string measure1number = "1"; // the measure number is a string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          __LINE__,
          measure1number,
          upperVoice1);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::create (
          __LINE__,
          ???measure???,
          msrClefKind::kClefTreble,
          1));

    // append the key

    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          __LINE__,
          msrQuarterTonesPitchKind::kQTP_C_Natural,
          msrModeKind::kModeMajor,
          0)); // keyCancel JMI

    // append the time

    measure1->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTwoQuartersTime (
          __LINE__));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure1number,
            msrQuarterTonesPitchKind::kQTP_A_Natural,
            msrOctaveKind::kOctave4,
            msrWholeNotes (1, 8), // soundingWholeNotes
            msrWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the dynamic to the note
      note1->
        appendDynamicToNote (
          msrDynamic::create (
            __LINE__,
            msrDynamicKind::kDynamicPP,
            msrPlacementKind::kPlacementBelow));

      // append the slur start to the note
      note1->
        appendSlurToNote (
          msrSlur::create (
            __LINE__,
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStart,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow));

      // append the first beam begin to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure1->
        appendNoteOrPaddingToMeasure (note1);
    }
*/




}
