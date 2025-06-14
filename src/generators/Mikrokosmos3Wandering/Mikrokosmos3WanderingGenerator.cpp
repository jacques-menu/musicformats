/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <regex>

#include "Mikrokosmos3WanderingGenerator.h"

#include "mfPreprocessorSettings.h"

#include "msrParts.h"
#include "msrBarLines.h"

#include "oahOah.h"
#include "msrOah.h"
#include "oahDisplayOah.h"


namespace MusicFormats
{

//------------------------------------------------------------------------
S_Mikrokosmos3WanderingGenerator Mikrokosmos3WanderingGenerator::create ()
{
  // create the insider handler
  Mikrokosmos3WanderingGenerator* obj = new
    Mikrokosmos3WanderingGenerator ();
  assert (obj != nullptr);
  return obj;
}

Mikrokosmos3WanderingGenerator::Mikrokosmos3WanderingGenerator ()
{
  // initialize it
  initializeMikrokosmos3WanderingGenerator ();
}

Mikrokosmos3WanderingGenerator::~Mikrokosmos3WanderingGenerator ()
{}

void Mikrokosmos3WanderingGenerator::initializeMikrokosmos3WanderingGenerator ()
{
  // JMI
}

//------------------------------------------------------------------------
S_msrScore Mikrokosmos3WanderingGenerator::createAndPopulateTheScore (
  msrGenerationAPIKind generationAPIKind)
//------------------------------------------------------------------------
{
  S_msrScore
    theMsrScore =
      createTheScore (
        generationAPIKind);

  S_msrPart
    part =
      createPartInScore (theMsrScore);

  // create and populate the two staves in part
if (true) // for tests JMI
  S_msrStaff
    upperStaff =
      createAndPopulateUpperStaffInPart (
        part,
        generationAPIKind);

if (true) // for tests JMI
  S_msrStaff
    lowerStaff =
      createAndPopulateLowerStaffInPart (
        part,
        generationAPIKind);

  // finalize the part

  part->
    finalizePartAndAllItsMeasures (
      mfInputLineNumber (__LINE__));

  if (gMsrOahGroup->getDisplayMsr ()) { // JMI 0.9.67
/* JMI
    gLog <<
      "___PART___" <<
      part <<
      std::endl;
*/

    // print the score
    gOutput <<
      std::endl <<
      "%--------------------------------------------------------------" <<
      std::endl <<
      "The Mikrokosmos III Wandering MSR score contains:" <<
      std::endl <<
      "%--------------------------------------------------------------" <<
      std::endl << std::endl;

    ++gIndenter;
    gOutput <<
      theMsrScore <<
      std::endl;
    --gIndenter;
  }

  return theMsrScore;
}

//------------------------------------------------------------------------
S_msrScore Mikrokosmos3WanderingGenerator::createTheScore (
  msrGenerationAPIKind generationAPIKind)
//------------------------------------------------------------------------
{
  // create the score
  S_msrScore
    theMsrScore =
      msrScore::create (
        mfInputLineNumber (__LINE__),
        "Mikrokosmos3Wandering");

  // create its identification
  S_msrIdentification
    identification =
      msrIdentification::create (
        mfInputLineNumber (__LINE__));

  // register it in the score
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
      "MSR "
        +
      msrGenerationAPIKindAsString (generationAPIKind)
        +
      " API");

  return theMsrScore;
}

//------------------------------------------------------------------------
S_msrStaff Mikrokosmos3WanderingGenerator::createStaffInPart (
  const mfStaffNumber& staffNumber,
  const S_msrPart& part)
//------------------------------------------------------------------------
{
  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        mfInputLineNumber (__LINE__), msrStaffKind::kStaffKindRegular, staffNumber, part);

  // append it to the part
  part ->
    addStaffToPartCloneByItsNumber ( // JMI NOT clone???
      staff);

  return staff;
}

//------------------------------------------------------------------------
S_msrStaff Mikrokosmos3WanderingGenerator::createAndPopulateUpperStaffInPart (
  const S_msrPart&            part,
  msrGenerationAPIKind generationAPIKind)
//------------------------------------------------------------------------
{
  // create the upper staff
  S_msrStaff
    upperStaff =
      createStaffInPart (
        1, part);

  // create voice 1 in upperStaff
  S_msrVoice
    upperVoice1 =
      createRegularVoiceInStaff (
        mfInputLineNumber (__LINE__),
        1,
       upperStaff);

  // populate its voice(s)
  switch (generationAPIKind) {
    case msrGenerationAPIKind::kMsrFunctionsAPIKind:
      populateUpperVoice1WithTheFunctionsAPI (
        upperVoice1);
      break;

    case msrGenerationAPIKind::kMsrStringsAPIKind:
      populateUpperVoice1WithTheStringsAPI (
        upperVoice1);
      break;
  } // switch

  return upperStaff;
}

//------------------------------------------------------------------------
S_msrStaff Mikrokosmos3WanderingGenerator::createAndPopulateLowerStaffInPart (
  const S_msrPart&            part,
  msrGenerationAPIKind generationAPIKind)
//------------------------------------------------------------------------
{
  // create the lower staff
  S_msrStaff
    lowerStaff =
      createStaffInPart (
        2, part);

  // create voice 1 in lowerStaff
  S_msrVoice
    lowerVoice1 =
      createRegularVoiceInStaff (
        mfInputLineNumber (__LINE__),
        1,
       lowerStaff);

  // create voice 2 in lowerStaff
  S_msrVoice
    lowerVoice2 =
      createRegularVoiceInStaff (
        mfInputLineNumber (__LINE__),
        2,
       lowerStaff);

  // populate its voice(s)
  // populate its voice(s)
  switch (generationAPIKind) {
    case msrGenerationAPIKind::kMsrFunctionsAPIKind:
      populateLowerVoice1WithTheFunctionsAPI (
        lowerVoice1);
      populateLowerVoice2WithTheFunctionsAPI (
        lowerVoice2);
      break;

    case msrGenerationAPIKind::kMsrStringsAPIKind:
      populateLowerVoice1WithTheStringsAPI (
        lowerVoice1);
      populateLowerVoice2WithTheStringsAPI (
        lowerVoice2);
      break;
  } // switch

  return lowerStaff;
}

//------------------------------------------------------------------------
S_msrPart Mikrokosmos3WanderingGenerator::createPartInScore (
  const S_msrScore& theMsrScore)
//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
S_msrVoice Mikrokosmos3WanderingGenerator::createRegularVoiceInStaff (
  const mfInputLineNumber& inputLineNumber,
  int          voiceNumber,
  const S_msrStaff&   staff)
//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
S_msrMeasure Mikrokosmos3WanderingGenerator::createMeasureInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber& measureNumber,
  const S_msrVoice&    voice)
//------------------------------------------------------------------------
{
  return
    voice->
      cascadeCreateAMeasureAndAppendItInVoice (
        inputLineNumber,
        333, //         previousMeasureEndInputLineNumber, 0.9.62
        measureNumber,
        msrMeasureImplicitKind::kMeasureImplicitKindNo);
}

//------------------------------------------------------------------------
// upperVoice1 - Functions API
void Mikrokosmos3WanderingGenerator::populateUpperVoice1WithTheFunctionsAPI (
  const S_msrVoice& upperVoice1)
//------------------------------------------------------------------------
{
  // measure 1
  // ----------------------------------

  {
    // create the measure

    std::string measure1number = "1"; // the measure number is a std::string;

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
          measure1,
          msrClefKind::kClefTreble,
          1));

    // append the key

    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          mfInputLineNumber (__LINE__),
          measure1,
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

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure1number,
            msrQuarterTonesPitchKind::kQTP_G_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure1->
        appendNoteToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure1number,
            msrQuarterTonesPitchKind::kQTP_F_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      note3->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure1->
        appendNoteToMeasure (note3);
    }

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure1number,
            msrQuarterTonesPitchKind::kQTP_E_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam end to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure1->
        appendNoteToMeasure (note1);
    }
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure
    std::string measure2number = "2"; // the measure number is a std::string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure2number,
          upperVoice1);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createThreeQuartersTime (
          mfInputLineNumber (__LINE__)));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 4), // soundingWholeNotes
            mfWholeNotes (1, 4), // displayWholeNotes
            0);             // dotsNumber

      // append the slur stop to the note
      note1->
        appendSlurToNote (
          msrSlur::create (
            mfInputLineNumber (__LINE__),
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStop,
            msrLineTypeKind::kLineTypeSolid));

      // append the note to the measure
      measure2->
        appendNoteToMeasure (note1);
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_G_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the dynamic to the note
      note2->
        appendDynamicToNote (
          msrDynamic::create (
            mfInputLineNumber (__LINE__),
            msrDynamicKind::kDynamicMP,
            msrPlacementKind::kPlacementBelow));

      // append the slur start to the note
      note2->
        appendSlurToNote (
          msrSlur::create (
            mfInputLineNumber (__LINE__),
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStart,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow));

      // append the first beam begin to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure2->
        appendNoteToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_F_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      note3->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure2->
        appendNoteToMeasure (note3);
    }

    {
      S_msrNote
        note4 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_E_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure2->
        appendNoteToMeasure (note4);
    }

    {
      S_msrNote
        note5 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam end to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      measure2->
        appendNoteToMeasure (note5);
    }
  }


  // measure 3
  // ----------------------------------

  {
    // create the measure

    std::string measure3number = "3"; // the measure number is a std::string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure3number,
          upperVoice1);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTwoQuartersTime (
          mfInputLineNumber (__LINE__)));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_C_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);             // dotsNumber

      if (false) { // for tests JMI
        // append the dynamic to the note
        note1->
          appendDynamicToNote (
            msrDynamic::create (
              mfInputLineNumber (__LINE__),
              msrDynamicKind::kDynamicPPP,
              msrPlacementKind::kPlacementBelow));
      }

      // append the first beam begin to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the crescendo wedge to it
      note1->
        appendWedgeToNote (
          msrWedge::create (
            mfInputLineNumber (__LINE__),
            1, // wedgeNumber
            msrWedgeKind::kWedgeCrescendo,
            msrWedgeNienteKind::kWedgeNienteNo,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow,
            3));

      // append the spanner start to the note
      S_msrSpanner
        spannerStart =
          msrSpanner::create (
            mfInputLineNumber (__LINE__),
            1, // spannerNumber
            msrSpannerKind::kSpannerDashes,
            msrSpannerTypeKind::kSpannerTypeStart,
            msrPlacementKind::kPlacementAbove,
            note1);

      spannerStart->
        setSpannerBeginText ("ritard.");

      if (false) { // for tests JMI
        spannerStart->
          setSpannerMiddleText ("middle");
      }

      if (false) { // for tests JMI
        spannerStart->
          setSpannerEndText ("end");
      }

      note1->
        appendSpannerToNote (spannerStart);

      // append the note to the measure
      measure3->
        appendNoteToMeasure (note1);
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 16), // soundingWholeNotes
            mfWholeNotes (1, 16), // displayWholeNotes
            0);               // dotsNumber

      // append the first beam continue to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the second beam begin to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            2, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure3->
        appendNoteToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_E_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 16), // soundingWholeNotes
            mfWholeNotes (1, 16), // displayWholeNotes
            0);              // dotsNumber

      // append the second beam end to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            2, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the first beam end to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure3->
        appendNoteToMeasure (note3);
    }

    {
      S_msrNote
        note4 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_F_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam begin to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure3->
        appendNoteToMeasure (note4);
    }

    {
      S_msrNote
        note5 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_E_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the wedge stop to the note
      note5->
        appendWedgeToNote (
          msrWedge::create (
            mfInputLineNumber (__LINE__),
            1, // wedgeNumber
            msrWedgeKind::kWedgeStop,
            msrWedgeNienteKind::kWedgeNienteNo,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow,
            3));

      // append the first beam end to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure3->
        appendNoteToMeasure (note5);
    }
  }


  // measure 4
  // ----------------------------------

  {
    // create the measure

    std::string measure4number = "4"; // the measure number is a std::string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure4number,
          upperVoice1);

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure4number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 2), // soundingWholeNotes
            mfWholeNotes (1, 2), // displayWholeNotes
            0);              // dotsNumber

      // set the note tie start
      note1->
        appendTieToNote (
          msrTie::create (
            mfInputLineNumber (__LINE__),
            msrTieKind::kTieStart,
            msrPlacementKind::kPlacementBelow));

      // append the decrescendo wedge to the note
      note1->
        appendWedgeToNote (
          msrWedge::create (
            mfInputLineNumber (__LINE__),
            2, // wedgeNumber
            msrWedgeKind::kWedgeDecrescendo,
            msrWedgeNienteKind::kWedgeNienteNo,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow,
            4));

      // append the note to the measure
      measure4->
        appendNoteToMeasure (note1);
    }
  }


  // measure 5
  // ----------------------------------

  {
    // create the measure

    std::string measure5number = "5"; // the measure number is a std::string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure5number,
          upperVoice1);

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure5number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 2), // soundingWholeNotes
            mfWholeNotes (1, 2), // displayWholeNotes
            0);              // dotsNumber

      // set the note tie stop
      note1->
        appendTieToNote (
          msrTie::create (
            mfInputLineNumber (__LINE__),
            msrTieKind::kTieStop));

      // append the slur stop to the note
      note1->
        appendSlurToNote (
          msrSlur::create (
            mfInputLineNumber (__LINE__),
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStop,
            msrLineTypeKind::kLineTypeSolid));

      // append the wedge stop to the note
      note1->
        appendWedgeToNote (
          msrWedge::create (
            mfInputLineNumber (__LINE__),
            2, // wedgeNumber
            msrWedgeKind::kWedgeStop,
            msrWedgeNienteKind::kWedgeNienteNo,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow,
            5));

      // append the spanner start to the note
      note1->
        appendSpannerToNote (
          msrSpanner::create (
            mfInputLineNumber (__LINE__),
            1, // spannerNumber
            msrSpannerKind::kSpannerDashes,
            msrSpannerTypeKind::kSpannerTypeStop,
            msrPlacementKind::kPlacementAbove,
            note1));

      // append the note to the measure
      measure5->
        appendNoteToMeasure (note1);
    }

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          mfInputLineNumber (__LINE__),
          measure5));
  }
}

//------------------------------------------------------------------------
// upperVoice1 - Strings API
void Mikrokosmos3WanderingGenerator::populateUpperVoice1WithTheStringsAPI (
  const S_msrVoice& upperVoice1)
//------------------------------------------------------------------------
{
/* JMI
  // measure 1
  // ----------------------------------

  {
    // create the measure


    std::string measure1number = "1"; // the measure number is a std::string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure1number,
          upperVoice1);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::createClefFromString (
          mfInputLineNumber (__LINE__),
          "treble",
          1));

    // append the key

    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          mfInputLineNumber (__LINE__),
          measure1,
          msrQuarterTonesPitchKind::kQTP_C_Natural,
          msrModeKind::kModeMajor,
          0)); // keyCancel JMI

    // append the time

    measure1->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          mfInputLineNumber (__LINE__),
          "2/4"));

    // append the notes

    S_msrNote
      note1 =
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "a'8",
          measure1number);

    // dynamic
    note1->
      appendDynamicToNote (
       msrDynamic::createDynamicFromString (
          mfInputLineNumber (__LINE__),
          "pp",
          msrPlacementKind::kPlacementBelow));

    // append the note to the measure
    measure1->
      appendNoteToMeasure (note1);

    measure1->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "g'8",
          measure1number));

    measure1->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "f'8",
          measure1number));

    measure1->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "e'8",
          measure1number));
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure

    std::string measure2number = "2"; // the measure number is a std::string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure2number,
          upperVoice1);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          mfInputLineNumber (__LINE__),
          "3/4"));

    // append the notes

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "d'4",
          measure2number));

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "g'8",
          measure2number));

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "f'8",
          measure2number));

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "e'8",
          measure2number));

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "d'8",
          measure2number));
  }


  // measure 3
  // ----------------------------------

  {
    // create the measure

    std::string measure3number = "3"; // the measure number is a std::string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure3number,
          upperVoice1);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          mfInputLineNumber (__LINE__),
          "2/4"));

    // append the notes

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "c'8",
          measure3number));

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "d'16",
          measure3number));

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "e'16",
          measure3number));

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "f'8",
          measure3number));

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "e'8",
          measure3number));
  }


  // measure 4
  // ----------------------------------

  {
    // create the measure

    std::string measure4number = "4"; // the measure number is a std::string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure4number,
          upperVoice1);

    // append the notes

    {
      measure4->
        appendNoteToMeasure (
          msrNote::createNoteFromString (
            mfInputLineNumber (__LINE__),
            "d'2",
            measure4number));
    }
  }


  // measure 5
  // ----------------------------------

  {
    // create the measure

    std::string measure5number = "5"; // the measure number is a std::string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure5number,
          upperVoice1);

    // append the notes

    {
      measure5->
        appendNoteToMeasure (
          msrNote::createNoteFromString (
            mfInputLineNumber (__LINE__),
            "d'2",
            measure5number));
    }

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          mfInputLineNumber (__LINE__),
          measure5));
  }
*/
}

//------------------------------------------------------------------------
// lowerVoice1 - Functions API
void Mikrokosmos3WanderingGenerator::populateLowerVoice2WithTheFunctionsAPI (
  const S_msrVoice& lowerVoice1)
//------------------------------------------------------------------------
{
  // measure 1
  // ----------------------------------

  {
    // create the measure

    std::string measure1number = "1"; // the measure number is a std::string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure1number,
          lowerVoice1);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::create (
          mfInputLineNumber (__LINE__),
          measure1,
          msrClefKind::kClefBass,
          2));

    // append the key

    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          mfInputLineNumber (__LINE__),
          measure1,
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
      measure1->
        appendNoteToMeasure (
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure1number,
            msrQuarterTonesPitchKind::kQTP_B_Natural,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 4), // soundingWholeNotes
            mfWholeNotes (1, 4), // displayWholeNotes
            0));             // dotsNumber
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure1number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the dynamic to the note
      note2->
        appendDynamicToNote (
          msrDynamic::create (
            mfInputLineNumber (__LINE__),
            msrDynamicKind::kDynamicPP,
            msrPlacementKind::kPlacementBelow));

      // append the first beam begin to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the slur start to the note
      note2->
        appendSlurToNote (
          msrSlur::create (
            mfInputLineNumber (__LINE__),
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStart,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementAbove));

      // append the note to the measure
      measure1->
        appendNoteToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure1number,
            msrQuarterTonesPitchKind::kQTP_C_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure1->
        appendNoteToMeasure (note3);
    }
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure

    std::string measure2number = "2"; // the measure number is a std::string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure2number,
          lowerVoice1);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createThreeQuartersTime (
          mfInputLineNumber (__LINE__)));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_B_Natural,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure2->
        appendNoteToMeasure (note1);
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_A_Natural,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam end to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure2->
        appendNoteToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_A_Flat,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 4), // soundingWholeNotes
            mfWholeNotes (1, 4), // displayWholeNotes
            0);             // dotsNumber

      // append the slur stop to the note
      note3->
        appendSlurToNote (
          msrSlur::create (
            mfInputLineNumber (__LINE__),
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStop,
            msrLineTypeKind::kLineTypeSolid));

      // append the note to the measure
      measure2->
        appendNoteToMeasure (note3);
    }

    {
      S_msrNote
        note4 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_C_Natural,
            msrOctaveKind::kOctave4,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the dynamic to the note
      note4->
        appendDynamicToNote (
          msrDynamic::create (
            mfInputLineNumber (__LINE__),
            msrDynamicKind::kDynamicMP,
            msrPlacementKind::kPlacementBelow));

      // append the first beam begin to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure2->
        appendNoteToMeasure (note4);
    }

    {
      S_msrNote
        note5 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure2number,
            msrQuarterTonesPitchKind::kQTP_B_Flat,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure2->
        appendNoteToMeasure (note5);
    }
  }


  // measure 3
  // ----------------------------------

  {
    // create the measure

    std::string measure3number = "3"; // the measure number is a std::string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure3number,
          lowerVoice1);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTwoQuartersTime (
          mfInputLineNumber (__LINE__)));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_A_Natural,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure3->
        appendNoteToMeasure (note1);
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_G_Natural,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam end to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure3->
        appendNoteToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_F_Sharp,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 8), // soundingWholeNotes
            mfWholeNotes (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam begin to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      measure3->
        appendNoteToMeasure (note3);
    }

    {
      S_msrNote
        note4 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_G_Natural,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 16), // soundingWholeNotes
            mfWholeNotes (1, 16), // displayWholeNotes
            0);               // dotsNumber

      // append the first beam continue to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the second beam begin to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            2, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure3->
        appendNoteToMeasure (note4);
    }

    {
      S_msrNote
        note5 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure3number,
            msrQuarterTonesPitchKind::kQTP_A_Natural,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 16), // soundingWholeNotes
            mfWholeNotes (1, 16), // displayWholeNotes
            0);               // dotsNumber

      // append the second beam end to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            2, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the first beam end to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            mfInputLineNumber (__LINE__),
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure3->
        appendNoteToMeasure (note5);
    }
  }


  // measure 4
  // ----------------------------------

  {
    // create the measure

    std::string measure4number = "4"; // the measure number is a std::string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure4number,
          lowerVoice1);

    // append the notes

    measure4->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "b4",
          measure4number));

    measure4->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "a4",
          measure4number));
  }

  // measure 5
  // ----------------------------------

  {
    // create the measure

    std::string measure5number = "5"; // the measure number is a std::string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure5number,
          lowerVoice1);

    // append the notes

    measure5->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "g2",
          measure5number));

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          mfInputLineNumber (__LINE__),
          measure5));
  }
}

//------------------------------------------------------------------------
// lowerVoice1 - Strings API
void Mikrokosmos3WanderingGenerator::populateLowerVoice2WithTheStringsAPI (
  const S_msrVoice& lowerVoice1)
//------------------------------------------------------------------------
{
/* JMI
  // measure 1
  // ----------------------------------

  {
    // create the measure

    std::string measure1number = "1"; // the measure number is a std::string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure1number,
          lowerVoice1);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::createClefFromString (
          mfInputLineNumber (__LINE__),
          "bass",
          2));

    // append the key

    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          mfInputLineNumber (__LINE__),
          measure1,
          msrQuarterTonesPitchKind::kQTP_C_Natural,
          msrModeKind::kModeMajor,
          0)); // keyCancel JMI

    // append the time

    measure1->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          mfInputLineNumber (__LINE__),
          "2/4"));

    // append the notes

    measure1->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "b4",
          measure1number));

    measure1->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "d'8",
          measure1number));

    measure1->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "c'8",
          measure1number));
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure

    std::string measure2number = "2"; // the measure number is a std::string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure2number,
          lowerVoice1);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          mfInputLineNumber (__LINE__),
          "3/4"));

    // append the notes

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "b8",
          measure2number));

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "a8",
          measure2number));

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "aes4",
          measure2number));

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "c'8",
          measure2number));

    measure2->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "bes8",
          measure2number));
  }

  // measure 3
  // ----------------------------------

  {
    // create the measure

    std::string measure3number = "3"; // the measure number is a std::string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure3number,
          lowerVoice1);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          mfInputLineNumber (__LINE__),
          "2/4"));

    // append the notes

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "a8",
          measure3number));

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "g8",
          measure3number));

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "fis8",
          measure3number));

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "g16",
          measure3number));

    measure3->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "a16",
          measure3number));
  }

  // measure 4
  // ----------------------------------

  {
    // create the measure

    std::string measure4number = "4"; // the measure number is a std::string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure4number,
          lowerVoice1);

    // append the notes

    measure4->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "b2",
          measure4number));
  }

  // measure 5
  // ----------------------------------

  {
    // create the measure

    std::string measure5number = "5"; // the measure number is a std::string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure5number,
          lowerVoice1);

    // append the notes

    measure5->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "b2",
          measure5number));

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          mfInputLineNumber (__LINE__),
          measure5));
  }
*/
}

//------------------------------------------------------------------------
// lowerVoice2 - Functions API
void Mikrokosmos3WanderingGenerator::populateLowerVoice1WithTheFunctionsAPI (
  const S_msrVoice& lowerVoice2)
//------------------------------------------------------------------------
{
  // measure 1
  // ----------------------------------

  {
    // create the measure

    std::string measure1number = "1"; // the measure number is a std::string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure1number,
          lowerVoice2);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::create (
          mfInputLineNumber (__LINE__),
          measure1,
          msrClefKind::kClefBass,
          2));

    // append the key

    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          mfInputLineNumber (__LINE__),
          measure1,
          msrQuarterTonesPitchKind::kQTP_C_Natural,
          msrModeKind::kModeMajor,
          0)); // keyCancel JMI

    // append the time

    measure1->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTwoQuartersTime (
          mfInputLineNumber (__LINE__)));

    // append the notes

    measure1->
      appendNoteToMeasure (
        msrNote::createSkipNote (
          mfInputLineNumber (__LINE__),
          measure1number,
          mfWholeNotes (1, 2), // soundingWholeNotes
          mfWholeNotes (1, 2), // displayWholeNotes
          0));             // dotsNumber
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure

    std::string measure2number = "2"; // the measure number is a std::string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure2number,
          lowerVoice2);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createThreeQuartersTime (
          mfInputLineNumber (__LINE__)));

    // append the notes

    measure2->
      appendNoteToMeasure (
        msrNote::createSkipNote (
          mfInputLineNumber (__LINE__),
          measure2number,
          mfWholeNotes (3, 4), // soundingWholeNotes
          mfWholeNotes (2, 4), // displayWholeNotes
          0));             // dotsNumber
  }

  // measure 3
  // ----------------------------------

  {
    // create the measure

    std::string measure3number = "3"; // the measure number is a std::string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure3number,
          lowerVoice2);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTwoQuartersTime (
          mfInputLineNumber (__LINE__)));

    // append the notes

    measure3->
      appendNoteToMeasure (
        msrNote::createSkipNote (
          mfInputLineNumber (__LINE__),
          measure3number,
          mfWholeNotes (1, 2), // soundingWholeNotes
          mfWholeNotes (1, 2), // displayWholeNotes
          0));             // dotsNumber
  }

  // measure 4
  // ----------------------------------

  {
    // create the measure

    std::string measure4number = "4"; // the measure number is a std::string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure4number,
          lowerVoice2);

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure4number,
            msrQuarterTonesPitchKind::kQTP_B_Natural,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 2), // soundingWholeNotes
            mfWholeNotes (1, 2), // displayWholeNotes
            0);             // dotsNumber

      // set the note tie start
      note1->
        appendTieToNote (
          msrTie::create (
            mfInputLineNumber (__LINE__),
            msrTieKind::kTieStart,
            msrPlacementKind::kPlacementAbove));

      measure4->
        appendNoteToMeasure (note1);
    }
  }


  // measure 5
  // ----------------------------------

  {
    // create the measure

    std::string measure5number = "5"; // the measure number is a std::string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure5number,
          lowerVoice2);

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            mfInputLineNumber (__LINE__),
            measure5number,
            msrQuarterTonesPitchKind::kQTP_B_Natural,
            msrOctaveKind::kOctave3,
            mfWholeNotes (1, 2), // soundingWholeNotes
            mfWholeNotes (1, 2), // displayWholeNotes
            0);             // dotsNumber

      // set the note tie stop
      note1->
        appendTieToNote (
          msrTie::create (
            mfInputLineNumber (__LINE__),
            msrTieKind::kTieStop));

      // append the note to the measure
      measure5->
        appendNoteToMeasure (note1);
    }

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          mfInputLineNumber (__LINE__),
          measure5));
  }
}

//------------------------------------------------------------------------
// lowerVoice2 - Strings API
void Mikrokosmos3WanderingGenerator::populateLowerVoice1WithTheStringsAPI (
  const S_msrVoice& lowerVoice2)
//------------------------------------------------------------------------
{
  // measure 1
  // ----------------------------------

  {
    // create the measure

    std::string measure1number = "1"; // the measure number is a std::string;

    S_msrMeasure
      measure1 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure1number,
          lowerVoice2);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::createClefFromString (
          mfInputLineNumber (__LINE__),
          measure1,
          "bass",
          2));

    // append the key

    measure1->
      appendKeyToMeasure (
        msrKey::createTraditional (
          mfInputLineNumber (__LINE__),
          measure1,
          msrQuarterTonesPitchKind::kQTP_C_Natural,
          msrModeKind::kModeMajor,
          0)); // keyCancel JMI

    // append the time

    measure1->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          mfInputLineNumber (__LINE__),
          "2/4"));

    // append the notes

    measure1->
      appendNoteToMeasure (
        msrNote::createSkipFromString (
          mfInputLineNumber (__LINE__),
          "s2",
          measure1number));
  }

  // measure 2
  // ----------------------------------

  {
    // create the measure

    std::string measure2number = "2"; // the measure number is a std::string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure2number,
          lowerVoice2);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          mfInputLineNumber (__LINE__),
          "3/4"));

    // append the notes

    measure2->
      appendNoteToMeasure (
        msrNote::createSkipFromString (
          mfInputLineNumber (__LINE__),
          "s2.",
          measure2number));
  }

  // measure 3
  // ----------------------------------

  {
    // create the measure

    std::string measure3number = "3"; // the measure number is a std::string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure3number,
          lowerVoice2);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          mfInputLineNumber (__LINE__),
          "2/4"));

    // append the notes

    measure3->
      appendNoteToMeasure (
        msrNote::createSkipFromString (
          mfInputLineNumber (__LINE__),
          "s2",
          measure3number));
  }

  // measure 4
  // ----------------------------------

  {
    // create the measure

    std::string measure4number = "4"; // the measure number is a std::string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure4number,
          lowerVoice2);

    // append the notes

    measure4->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "b4",
          measure4number));

    measure4->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "a4",
          measure4number));
  }

  // measure 5
  // ----------------------------------

  {
    // create the measure

    std::string measure5number = "5"; // the measure number is a std::string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          mfInputLineNumber (__LINE__),
          measure5number,
          lowerVoice2);

    // append the notes

    measure5->
      appendNoteToMeasure (
        msrNote::createNoteFromString (
          mfInputLineNumber (__LINE__),
          "g2",
          measure5number));

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          mfInputLineNumber (__LINE__),
          measure5));
  }
}


} // namespace
