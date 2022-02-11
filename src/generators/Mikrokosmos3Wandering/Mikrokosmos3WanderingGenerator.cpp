/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <regex>

#include "Mikrokosmos3WanderingGenerator.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "displayOah.h"





using namespace std;

namespace MusicFormats
{

//------------------------------------------------------------------------
S_Mikrokosmos3WanderingGenerator Mikrokosmos3WanderingGenerator::create ()
{
  // create the insider handler
  Mikrokosmos3WanderingGenerator* o = new
    Mikrokosmos3WanderingGenerator ();
  assert (o != nullptr);

  return o;
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
      __LINE__);

  if (gGlobalMsrOahGroup->getDisplayMsr ()) { // JMI CAFE
/* JMI
    gLogStream <<
      "___PART___" <<
      part <<
      endl;
*/

    // print the score
    gOutputStream <<
      endl <<
      "%--------------------------------------------------------------" <<
      endl <<
      "The Mikrokosmos III Wandering MSR score contains:" <<
      endl <<
      "%--------------------------------------------------------------" <<
      endl << endl;

    ++gIndenter;
    gOutputStream <<
      theMsrScore <<
      endl;
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
        __LINE__,
        "Mikrokosmos3Wandering");

  // create its identification
  S_msrIdentification
    identification =
      msrIdentification::create (
        __LINE__);

  // register it in the score
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
      "MSR "
        +
      msrGenerationAPIKindAsString (generationAPIKind)
        +
      " API");

  return theMsrScore;
}

//------------------------------------------------------------------------
S_msrStaff Mikrokosmos3WanderingGenerator::createStaffInPart (
  int staffNumber, S_msrPart part)
//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
S_msrStaff Mikrokosmos3WanderingGenerator::createAndPopulateUpperStaffInPart (
  S_msrPart            part,
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
        __LINE__,
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
  S_msrPart            part,
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
        __LINE__,
        1,
       lowerStaff);

  // create voice 2 in lowerStaff
  S_msrVoice
    lowerVoice2 =
      createRegularVoiceInStaff (
        __LINE__,
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
  S_msrScore theMsrScore)
//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
S_msrVoice Mikrokosmos3WanderingGenerator::createRegularVoiceInStaff (
  int          inputLineNumber,
  int          voiceNumber,
  S_msrStaff   staff)
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
  int           inputLineNumber,
  const string& measureNumber,
  S_msrVoice    voice)
//------------------------------------------------------------------------
{
  return
    voice->
      createAMeasureAndAppendItToVoice (
        inputLineNumber,
        measureNumber,
        msrMeasureImplicitKind::kMeasureImplicitKindNo);
}

//------------------------------------------------------------------------
// upperVoice1 - Functions API
void Mikrokosmos3WanderingGenerator::populateUpperVoice1WithTheFunctionsAPI (
  S_msrVoice upperVoice1)
//------------------------------------------------------------------------
{
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

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            __LINE__,
            measure1number,
            msrQuarterTonesPitchKind::kQTP_G_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure1->
        appendNoteOrPaddingToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            __LINE__,
            measure1number,
            msrQuarterTonesPitchKind::kQTP_F_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      note3->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure1->
        appendNoteOrPaddingToMeasure (note3);
    }

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure1number,
            msrQuarterTonesPitchKind::kQTP_E_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam end to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure1->
        appendNoteOrPaddingToMeasure (note1);
    }
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure
    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          upperVoice1);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createThreeQuartersTime (
          __LINE__));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 4), // soundingWholeNotes
            rational (1, 4), // displayWholeNotes
            0);             // dotsNumber

      // append the slur stop to the note
      note1->
        appendSlurToNote (
          msrSlur::create (
            __LINE__,
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStop,
            msrLineTypeKind::kLineTypeSolid));

      // append the note to the measure
      measure2->
        appendNoteOrPaddingToMeasure (note1);
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_G_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the dynamic to the note
      note2->
        appendDynamicToNote (
          msrDynamic::create (
            __LINE__,
            msrDynamicKind::kDynamicMP,
            msrPlacementKind::kPlacementBelow));

      // append the slur start to the note
      note2->
        appendSlurToNote (
          msrSlur::create (
            __LINE__,
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStart,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow));

      // append the first beam begin to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure2->
        appendNoteOrPaddingToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_F_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      note3->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure2->
        appendNoteOrPaddingToMeasure (note3);
    }

    {
      S_msrNote
        note4 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_E_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure2->
        appendNoteOrPaddingToMeasure (note4);
    }

    {
      S_msrNote
        note5 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam end to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      measure2->
        appendNoteOrPaddingToMeasure (note5);
    }
  }


  // measure 3
  // ----------------------------------

  {
    // create the measure

    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          upperVoice1);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTwoQuartersTime (
          __LINE__));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_C_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);             // dotsNumber

      if (false) { // for tests JMI
        // append the dynamic to the note
        note1->
          appendDynamicToNote (
            msrDynamic::create (
              __LINE__,
              msrDynamicKind::kDynamicPPP,
              msrPlacementKind::kPlacementBelow));
      }

      // append the first beam begin to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the crescendo wedge to it
      note1->
        appendWedgeToNote (
          msrWedge::create (
            __LINE__,
            msrWedge::kWedgeCrescendo,
            msrWedge::kWedgeNienteNo,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow));

      // append the spanner start to the note
      S_msrSpanner
        spannerStart =
          msrSpanner::create (
            __LINE__,
            1, // spannerNumber
            msrSpanner::kSpannerDashes,
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
        appendNoteOrPaddingToMeasure (note1);
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 16), // soundingWholeNotes
            rational (1, 16), // displayWholeNotes
            0);               // dotsNumber

      // append the first beam continue to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the second beam begin to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            2, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure3->
        appendNoteOrPaddingToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_E_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 16), // soundingWholeNotes
            rational (1, 16), // displayWholeNotes
            0);              // dotsNumber

      // append the second beam end to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            2, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the first beam end to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure3->
        appendNoteOrPaddingToMeasure (note3);
    }

    {
      S_msrNote
        note4 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_F_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam begin to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure3->
        appendNoteOrPaddingToMeasure (note4);
    }

    {
      S_msrNote
        note5 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_E_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the wedge stop to the note
      note5->
        appendWedgeToNote (
          msrWedge::create (
            __LINE__,
            msrWedge::kWedgeStop,
            msrWedge::kWedgeNienteNo,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow));

      // append the first beam end to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure3->
        appendNoteOrPaddingToMeasure (note5);
    }
  }


  // measure 4
  // ----------------------------------

  {
    // create the measure

    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          upperVoice1);

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure4number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 2), // soundingWholeNotes
            rational (1, 2), // displayWholeNotes
            0);              // dotsNumber

      // set the note tie start
      note1->
        setNoteTie (
          msrTie::create (
            __LINE__,
            msrTieKind::kTieStart,
            msrPlacementKind::kPlacementBelow));

      // append the decrescendo wedge to the note
      note1->
        appendWedgeToNote (
          msrWedge::create (
            __LINE__,
            msrWedge::kWedgeDecrescendo,
            msrWedge::kWedgeNienteNo,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow));

      // append the note to the measure
      measure4->
        appendNoteOrPaddingToMeasure (note1);
    }
  }


  // measure 5
  // ----------------------------------

  {
    // create the measure

    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          upperVoice1);

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure5number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 2), // soundingWholeNotes
            rational (1, 2), // displayWholeNotes
            0);              // dotsNumber

      // set the note tie stop
      note1->
        setNoteTie (
          msrTie::create (
            __LINE__,
            msrTieKind::kTieStop));

      // append the slur stop to the note
      note1->
        appendSlurToNote (
          msrSlur::create (
            __LINE__,
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStop,
            msrLineTypeKind::kLineTypeSolid));

      // append the wedge stop to the note
      note1->
        appendWedgeToNote (
          msrWedge::create (
            __LINE__,
            msrWedge::kWedgeStop,
            msrWedge::kWedgeNienteNo,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementBelow));

      // append the spanner start to the note
      note1->
        appendSpannerToNote (
          msrSpanner::create (
            __LINE__,
            1, // spannerNumber
            msrSpanner::kSpannerDashes,
            msrSpannerTypeKind::kSpannerTypeStop,
            msrPlacementKind::kPlacementAbove,
            note1));

      // append the note to the measure
      measure5->
        appendNoteOrPaddingToMeasure (note1);
    }

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          __LINE__));
  }
}

//------------------------------------------------------------------------
// upperVoice1 - Strings API
void Mikrokosmos3WanderingGenerator::populateUpperVoice1WithTheStringsAPI (
  S_msrVoice upperVoice1)
//------------------------------------------------------------------------
{
/* JMI
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
        msrClef::createClefFromString (
          __LINE__,
          "treble",
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
        msrTimeSignature::createTimeFromString (
          __LINE__,
          "2/4"));

    // append the notes

    S_msrNote
      note1 =
        msrNote::createNoteFromString (
          __LINE__,
          "a'8",
          measure1number);

    // dynamic
    note1->
      appendDynamicToNote (
       msrDynamic::createDynamicFromString (
          __LINE__,
          "pp",
          msrPlacementKind::kPlacementBelow));

    // append the note to the measure
    measure1->
      appendNoteOrPaddingToMeasure (note1);

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g'8",
          measure1number));

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "f'8",
          measure1number));

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "e'8",
          measure1number));
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure

    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          upperVoice1);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          __LINE__,
          "3/4"));

    // append the notes

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'4",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g'8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "f'8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "e'8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'8",
          measure2number));
  }


  // measure 3
  // ----------------------------------

  {
    // create the measure

    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          upperVoice1);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          __LINE__,
          "2/4"));

    // append the notes

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "c'8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'16",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "e'16",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "f'8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "e'8",
          measure3number));
  }


  // measure 4
  // ----------------------------------

  {
    // create the measure

    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          upperVoice1);

    // append the notes

    {
      measure4->
        appendNoteOrPaddingToMeasure (
          msrNote::createNoteFromString (
            __LINE__,
            "d'2",
            measure4number));
    }
  }


  // measure 5
  // ----------------------------------

  {
    // create the measure

    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          upperVoice1);

    // append the notes

    {
      measure5->
        appendNoteOrPaddingToMeasure (
          msrNote::createNoteFromString (
            __LINE__,
            "d'2",
            measure5number));
    }

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          __LINE__));
  }
*/
}

//------------------------------------------------------------------------
// lowerVoice1 - Functions API
void Mikrokosmos3WanderingGenerator::populateLowerVoice2WithTheFunctionsAPI (
  S_msrVoice lowerVoice1)
//------------------------------------------------------------------------
{
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
          lowerVoice1);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::create (
          __LINE__,
          msrClefKind::kClefBass,
          2));

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
      measure1->
        appendNoteOrPaddingToMeasure (
          msrNote::createRegularNote (
            __LINE__,
            measure1number,
            msrQuarterTonesPitchKind::kQTP_B_Natural,
            msrOctaveKind::kOctave3,
            rational (1, 4), // soundingWholeNotes
            rational (1, 4), // displayWholeNotes
            0));             // dotsNumber
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            __LINE__,
            measure1number,
            msrQuarterTonesPitchKind::kQTP_D_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the dynamic to the note
      note2->
        appendDynamicToNote (
          msrDynamic::create (
            __LINE__,
            msrDynamicKind::kDynamicPP,
            msrPlacementKind::kPlacementBelow));

      // append the first beam begin to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the slur start to the note
      note2->
        appendSlurToNote (
          msrSlur::create (
            __LINE__,
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStart,
            msrLineTypeKind::kLineTypeSolid,
            msrPlacementKind::kPlacementAbove));

      // append the note to the measure
      measure1->
        appendNoteOrPaddingToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            __LINE__,
            measure1number,
            msrQuarterTonesPitchKind::kQTP_C_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure1->
        appendNoteOrPaddingToMeasure (note3);
    }
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure

    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          lowerVoice1);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createThreeQuartersTime (
          __LINE__));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_B_Natural,
            msrOctaveKind::kOctave3,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure2->
        appendNoteOrPaddingToMeasure (note1);
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_A_Natural,
            msrOctaveKind::kOctave3,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam end to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure2->
        appendNoteOrPaddingToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_A_Flat,
            msrOctaveKind::kOctave3,
            rational (1, 4), // soundingWholeNotes
            rational (1, 4), // displayWholeNotes
            0);             // dotsNumber

      // append the slur stop to the note
      note3->
        appendSlurToNote (
          msrSlur::create (
            __LINE__,
            1, // slurNumber
            msrSlurTypeKind::kSlurTypeRegularStop,
            msrLineTypeKind::kLineTypeSolid));

      // append the note to the measure
      measure2->
        appendNoteOrPaddingToMeasure (note3);
    }

    {
      S_msrNote
        note4 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_C_Natural,
            msrOctaveKind::kOctave4,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the dynamic to the note
      note4->
        appendDynamicToNote (
          msrDynamic::create (
            __LINE__,
            msrDynamicKind::kDynamicMP,
            msrPlacementKind::kPlacementBelow));

      // append the first beam begin to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure2->
        appendNoteOrPaddingToMeasure (note4);
    }

    {
      S_msrNote
        note5 =
          msrNote::createRegularNote (
            __LINE__,
            measure2number,
            msrQuarterTonesPitchKind::kQTP_B_Flat,
            msrOctaveKind::kOctave3,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure2->
        appendNoteOrPaddingToMeasure (note5);
    }
  }


  // measure 3
  // ----------------------------------

  {
    // create the measure

    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          lowerVoice1);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTwoQuartersTime (
          __LINE__));

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_A_Natural,
            msrOctaveKind::kOctave3,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam continue to the note
      note1->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the note to the measure
      measure3->
        appendNoteOrPaddingToMeasure (note1);
    }

    {
      S_msrNote
        note2 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_G_Natural,
            msrOctaveKind::kOctave3,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);              // dotsNumber

      // append the first beam end to the note
      note2->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure3->
        appendNoteOrPaddingToMeasure (note2);
    }

    {
      S_msrNote
        note3 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_F_Sharp,
            msrOctaveKind::kOctave3,
            rational (1, 8), // soundingWholeNotes
            rational (1, 8), // displayWholeNotes
            0);             // dotsNumber

      // append the first beam begin to the note
      note3->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamBegin));

      measure3->
        appendNoteOrPaddingToMeasure (note3);
    }

    {
      S_msrNote
        note4 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_G_Natural,
            msrOctaveKind::kOctave3,
            rational (1, 16), // soundingWholeNotes
            rational (1, 16), // displayWholeNotes
            0);               // dotsNumber

      // append the first beam continue to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamContinue));

      // append the second beam begin to the note
      note4->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            2, // beamNumber
            msrBeamKind::kBeamBegin));

      // append the note to the measure
      measure3->
        appendNoteOrPaddingToMeasure (note4);
    }

    {
      S_msrNote
        note5 =
          msrNote::createRegularNote (
            __LINE__,
            measure3number,
            msrQuarterTonesPitchKind::kQTP_A_Natural,
            msrOctaveKind::kOctave3,
            rational (1, 16), // soundingWholeNotes
            rational (1, 16), // displayWholeNotes
            0);               // dotsNumber

      // append the second beam end to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            2, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the first beam end to the note
      note5->
        appendBeamToNote (
          msrBeam::create (
            __LINE__,
            1, // beamNumber
            msrBeamKind::kBeamEnd));

      // append the note to the measure
      measure3->
        appendNoteOrPaddingToMeasure (note5);
    }
  }


  // measure 4
  // ----------------------------------

  {
    // create the measure

    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          lowerVoice1);

    // append the notes

    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b4",
          measure4number));

    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a4",
          measure4number));
  }

  // measure 5
  // ----------------------------------

  {
    // create the measure

    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          lowerVoice1);

    // append the notes

    measure5->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g2",
          measure5number));

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          __LINE__));
  }
}

//------------------------------------------------------------------------
// lowerVoice1 - Strings API
void Mikrokosmos3WanderingGenerator::populateLowerVoice2WithTheStringsAPI (
  S_msrVoice lowerVoice1)
//------------------------------------------------------------------------
{
/* JMI
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
          lowerVoice1);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::createClefFromString (
          __LINE__,
          "bass",
          2));

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
        msrTimeSignature::createTimeFromString (
          __LINE__,
          "2/4"));

    // append the notes

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b4",
          measure1number));

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "d'8",
          measure1number));

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "c'8",
          measure1number));
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure

    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          lowerVoice1);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          __LINE__,
          "3/4"));

    // append the notes

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "aes4",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "c'8",
          measure2number));

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "bes8",
          measure2number));
  }

  // measure 3
  // ----------------------------------

  {
    // create the measure

    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          lowerVoice1);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          __LINE__,
          "2/4"));

    // append the notes

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "fis8",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g16",
          measure3number));

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a16",
          measure3number));
  }

  // measure 4
  // ----------------------------------

  {
    // create the measure

    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          lowerVoice1);

    // append the notes

    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b2",
          measure4number));
  }

  // measure 5
  // ----------------------------------

  {
    // create the measure

    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          lowerVoice1);

    // append the notes

    measure5->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b2",
          measure5number));

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          __LINE__));
  }
*/
}

//------------------------------------------------------------------------
// lowerVoice2 - Functions API
void Mikrokosmos3WanderingGenerator::populateLowerVoice1WithTheFunctionsAPI (
  S_msrVoice lowerVoice2)
//------------------------------------------------------------------------
{
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
          lowerVoice2);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::create (
          __LINE__,
          msrClefKind::kClefBass,
          2));

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

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipNote (
          __LINE__,
          measure1number,
          rational (1, 2), // soundingWholeNotes
          rational (1, 2), // displayWholeNotes
          0));             // dotsNumber
  }


  // measure 2
  // ----------------------------------

  {
    // create the measure

    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          lowerVoice2);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createThreeQuartersTime (
          __LINE__));

    // append the notes

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipNote (
          __LINE__,
          measure2number,
          rational (3, 4), // soundingWholeNotes
          rational (2, 4), // displayWholeNotes
          0));             // dotsNumber
  }

  // measure 3
  // ----------------------------------

  {
    // create the measure

    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          lowerVoice2);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTwoQuartersTime (
          __LINE__));

    // append the notes

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipNote (
          __LINE__,
          measure3number,
          rational (1, 2), // soundingWholeNotes
          rational (1, 2), // displayWholeNotes
          0));             // dotsNumber
  }

  // measure 4
  // ----------------------------------

  {
    // create the measure

    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          lowerVoice2);

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure4number,
            msrQuarterTonesPitchKind::kQTP_B_Natural,
            msrOctaveKind::kOctave3,
            rational (1, 2), // soundingWholeNotes
            rational (1, 2), // displayWholeNotes
            0);             // dotsNumber

      // set the note tie start
      note1->
        setNoteTie (
          msrTie::create (
            __LINE__,
            msrTieKind::kTieStart,
            msrPlacementKind::kPlacementAbove));

      measure4->
        appendNoteOrPaddingToMeasure (note1);
    }
  }


  // measure 5
  // ----------------------------------

  {
    // create the measure

    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          lowerVoice2);

    // append the notes

    {
      S_msrNote
        note1 =
          msrNote::createRegularNote (
            __LINE__,
            measure5number,
            msrQuarterTonesPitchKind::kQTP_B_Natural,
            msrOctaveKind::kOctave3,
            rational (1, 2), // soundingWholeNotes
            rational (1, 2), // displayWholeNotes
            0);             // dotsNumber

      // set the note tie stop
      note1->
        setNoteTie (
          msrTie::create (
            __LINE__,
            msrTieKind::kTieStop));

      // append the note to the measure
      measure5->
        appendNoteOrPaddingToMeasure (note1);
    }

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          __LINE__));
  }
}

//------------------------------------------------------------------------
// lowerVoice2 - Strings API
void Mikrokosmos3WanderingGenerator::populateLowerVoice1WithTheStringsAPI (
  S_msrVoice lowerVoice2)
//------------------------------------------------------------------------
{
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
          lowerVoice2);

    // append the clef

    measure1->
      appendClefToMeasure (
        msrClef::createClefFromString (
          __LINE__,
          "bass",
          2));

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
        msrTimeSignature::createTimeFromString (
          __LINE__,
          "2/4"));

    // append the notes

    measure1->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipFromString (
          __LINE__,
          "s2",
          measure1number));
  }

  // measure 2
  // ----------------------------------

  {
    // create the measure

    string measure2number = "2"; // the measure number is a string;

    S_msrMeasure
      measure2 =
        createMeasureInVoice (
          __LINE__,
          measure2number,
          lowerVoice2);

    // append the time

    measure2->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          __LINE__,
          "3/4"));

    // append the notes

    measure2->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipFromString (
          __LINE__,
          "s2.",
          measure2number));
  }

  // measure 3
  // ----------------------------------

  {
    // create the measure

    string measure3number = "3"; // the measure number is a string;

    S_msrMeasure
      measure3 =
        createMeasureInVoice (
          __LINE__,
          measure3number,
          lowerVoice2);

    // append the time

    measure3->
      appendTimeSignatureToMeasure (
        msrTimeSignature::createTimeFromString (
          __LINE__,
          "2/4"));

    // append the notes

    measure3->
      appendNoteOrPaddingToMeasure (
        msrNote::createSkipFromString (
          __LINE__,
          "s2",
          measure3number));
  }

  // measure 4
  // ----------------------------------

  {
    // create the measure

    string measure4number = "4"; // the measure number is a string;

    S_msrMeasure
      measure4 =
        createMeasureInVoice (
          __LINE__,
          measure4number,
          lowerVoice2);

    // append the notes

    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "b4",
          measure4number));

    measure4->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "a4",
          measure4number));
  }

  // measure 5
  // ----------------------------------

  {
    // create the measure

    string measure5number = "5"; // the measure number is a string;

    S_msrMeasure
      measure5 =
        createMeasureInVoice (
          __LINE__,
          measure5number,
          lowerVoice2);

    // append the notes

    measure5->
      appendNoteOrPaddingToMeasure (
        msrNote::createNoteFromString (
          __LINE__,
          "g2",
          measure5number));

    // append the final barLine

    measure5->
      appendBarLineToMeasure (
        msrBarLine::createFinalBarLine (
          __LINE__));
  }
}


} // namespace
