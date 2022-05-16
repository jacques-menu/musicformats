/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfTiming.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "msdlParser.h"

#include "msdl2msrOah.h"

#include "msrInterface.h"


using namespace std;

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
    setIdentificationWorkTitle (
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
  S_msrScore    theMsrScore,
  const string& passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // start the clock
  clock_t startClock =  clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): " << passDescription <<
    endl <<
    separator <<
    endl << endl <<
    theMsrScore;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    passDescription,
    mfTimingItem::kOptional,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after " << passDescription << ": " <<
        gIndenter.getIndentation ();

      msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayMsrScoreShort (
  S_msrScore    theMsrScore,
  const string& passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): " << passDescription << ", short version" <<
    endl <<
    separator <<
    endl << endl;

  theMsrScore->printShort (gLogStream);

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    passDescription,
    mfTimingItem::kOptional,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after " << passDescription << ": " <<
        gIndenter.getIndentation ();

      msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayMsrScoreSummary (
  S_msrScore     theMsrScore,
  S_msrOahGroup& msrOpts,
  const string&  passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass (optional): " << passDescription  <<
      endl <<
      separator <<
      endl << endl;
  }
#endif

  // create an msr2summaryVisitor visitor
  msr2summaryVisitor
    summaryVisitor (
      msrOpts);

  summaryVisitor.printSummaryFromMsrScore (
    theMsrScore);

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    passDescription,
    mfTimingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMsrScoreNames (
  S_msrScore     theMsrScore,
  S_msrOahGroup& msrOpts,
  const string&  passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass (optional): " << passDescription <<
      endl <<
      separator <<
      endl << endl;
  }
#endif

  // create an msr2namesVisitor visitor
  msr2namesVisitor
    namesVisitor (
      msrOpts);

  namesVisitor.printNamesFromMsrScore (
    theMsrScore);

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    passDescription,
    mfTimingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayMsrScoreSlices (
  S_msrScore    theMsrScore,
  const string& passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    endl <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): " << passDescription <<
    endl <<
    separator <<
    endl << endl;

  theMsrScore->printSlices (gLogStream);

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    passDescription,
    mfTimingItem::kOptional,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after " << passDescription << ": " <<
        gIndenter.getIndentation ();

      msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
S_msrPart createPartInScore (
  S_msrScore theMsrScore)
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
    setPartShortestNoteDuration (
      rational (1, 16));
}

  return part;
}

//_______________________________________________________________________________
S_msrStaff createStaffInPart (
  int staffNumber, S_msrPart part)
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
  int          inputLineNumber,
  int          voiceNumber,
  S_msrStaff   staff)
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
  int        inputLineNumber,
  string     measureNumber,
  S_msrVoice voice)
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

    string measure1number = "1"; // the measure number is a string;

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
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
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
