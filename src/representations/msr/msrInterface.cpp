/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "displayMsrVoicesFlatViewVisitor.h"
#include "displayMsrNamesVisitor.h"
#include "displayMsrMeasuresSlicesVisitor.h"
#include "displayMsrSummaryVisitor.h"

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
        mfInputLineNumber (__LINE__),
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
        mfInputLineNumber (__LINE__),
        "msrScore::create()");

  // create its identification
  S_msrIdentification
    identification =
      msrIdentification::create (
        mfInputLineNumber (__LINE__));

  theMsrScore->
    setIdentification (
      identification);

  // set the identification's work title
  identification->
    setIdentificationWorkCreditTypeTitle (
      mfInputLineNumber (__LINE__),
      "Mikrokosmos III Wandering");

  // append the identification's composer
  identification->
    appendComposer (
      mfInputLineNumber (__LINE__),
      "Zoltán Kodály");

  // set the identification's software
  identification->
    appendSoftware (
      mfInputLineNumber (__LINE__),
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
    __FILE__, mfInputLineNumber (__LINE__),
    theMsrScore != nullptr,
    "theMsrScore is NULL");
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
    __FILE__, mfInputLineNumber (__LINE__),
    theMsrScore != nullptr,
    "theMsrScore is NULL");
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
    __FILE__, mfInputLineNumber (__LINE__),
    theMsrScore != nullptr,
    "theMsrScore is NULL");
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

  // create an displayMsrSummaryVisitor visitor
  displayMsrSummaryVisitor
    summaryVisitor (
      msrOpts);

  summaryVisitor.printMsrScoreSummary (
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
    __FILE__, mfInputLineNumber (__LINE__),
    theMsrScore != nullptr,
    "theMsrScore is NULL");
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

  // create an displayMsrNamesVisitor visitor
  displayMsrNamesVisitor
    namesVisitor (
      msrOpts);

  namesVisitor.printMsrScoreNames (
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
void displayMsrScoreVoicesFlatView (
  S_msrScore                    theMsrScore,
  const S_msrOahGroup&          msrOpts,
  const std::string&            passDescription,
  msrVoicesFlatViewDetailedKind displayVoicesFlatViewDetailsKind)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    theMsrScore != nullptr,
    "theMsrScore is NULL");
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

  // create an displayMsrVoicesFlatViewVisitor visitor
  displayMsrVoicesFlatViewVisitor
    voicesFlatViewVisitor (
      msrOpts,
      displayVoicesFlatViewDetailsKind);

  voicesFlatViewVisitor.printMsrScoreVoicesFlatView (
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
void displayMsrScoreMeasuresSlices (
  const S_msrScore&    theMsrScore,
  const S_msrOahGroup& msrOpts,
  const std::string&   passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    theMsrScore != nullptr,
    "theMsrScore is NULL");
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

  // create an displayMsrMeasuresSlicesVisitor visitor
  displayMsrMeasuresSlicesVisitor
    measuresSlicesVisitor (
      msrOpts);

  measuresSlicesVisitor.printMsrScoreMeasuresSlices (
    theMsrScore);

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
        mfInputLineNumber (__LINE__), 1, 1, "OnlyPartGroup", nullptr, theMsrScore);

  // add it to the score
  theMsrScore->
    addPartGroupToScore (
      partGroup);

  // create the part
  S_msrPart
    part =
      msrPart::create (
        mfInputLineNumber (__LINE__),
        "OnlyPart",
        partGroup);

  // append it to the part group
  partGroup->
    appendPartToPartGroup (
      part);

if (false) { // JMI TEMP
  part->
    setPartShortestNoteWholeNotes (
      mfWholeNotes (1, 16));
}

  return part;
}

//_______________________________________________________________________________
S_msrStaff createStaffInPart (
  const mfStaffNumber& staffNumber,
  const S_msrPart& part)
{
  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        mfInputLineNumber (__LINE__),
        msrStaffKind::kStaffKindRegular,
        staffNumber,
        part);

  // append it to the part
  part ->
    addStaffToPartCloneByItsNumber ( // JMI NOT clone???
      staff);

  return staff;
}

//_______________________________________________________________________________
S_msrVoice createRegularVoiceInStaff (
  const mfInputLineNumber& inputLineNumber,
  const mfVoiceNumber&     voiceNumber,
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
  const mfInputLineNumber& inputLineNumber,
  std::string       measureNumber,
  const S_msrVoice& voice)
{
  return
    voice->
      cascadeCreateAMeasureAndAppendItInVoice (
        inputLineNumber,
        333, //         previousMeasureEndInputLineNumber, 0.9.62
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
          mfInputLineNumber (__LINE__),
          measure1number,
          upperVoice1);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::create (
          mfInputLineNumber (__LINE__),
          ???measure???,
          msrClefKind::kClefTreble,
          1));

    // append the key

    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          mfInputLineNumber (__LINE__),
          msrQuarterTonesPitchKind::kQTP_C_Natural,
          msrModeKind::kModeMajor,
          0)); // keyCancel JMI

    // append the time

    measure1->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTwoQuartersTime (
          mfInputLineNumber (__LINE__)));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure1number,
            msrQuarterTonesPitchKind::kQTP_A_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the dynamic to the note
      note1->
        appendDynamicToNote (
          msrDynamic::create (
            mfInputLineNumber (__LINE__),
            msrDynamicKind::kDynamicPP,
            msrPlacementKind::kPlacementBelow));

      // append the slur start to the note
      note1->
        appendSlurToNote (
          msrSlur::create (
            mfInputLineNumber (__LINE__),
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStart,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow));

      // append the first beam begin to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure1->
        appendNoteToMeasure (note1);
    }
*/




}
