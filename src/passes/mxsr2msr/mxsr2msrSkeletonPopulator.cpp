/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "xml_tree_browser.h"
#include "mfAssert.h"
#include "mfConstants.h"
#include "mfStringsHandling.h"

#include "msrBreaks.h"
#include "msrMeasureConstants.h"
#include "msrRehearsalMarks.h"
#include "msrVoiceStaffChanges.h"

#include "oahOah.h"

#include "waeOah.h"

#include "mxsr2msrOah.h"
#include "msrOah.h"

#include "mxsr2msrSkeletonPopulator.h"

#include "waeHandlers.h"

#include "mxsr2msrWae.h"


namespace MusicFormats
{

//________________________________________________________________________
mxsr2msrSkeletonPopulator::mxsr2msrSkeletonPopulator (
  const S_msrScore&     scoreSkeleton,
  mxsrEventsCollection& theKnownEventsCollection)
    : fKnownEventsCollection (theKnownEventsCollection)
{
  // initialize note data to a neutral state
  initializeNoteData ();

  // the MSR score we're populating
  fMsrScore = scoreSkeleton;

  // divisions
  fCurrentDivisionsPerQuarterNote = 1;

  // scaling handling
  fCurrentMillimeters = -1;
  fCurrentTenths = -1;

  // page layout
  fCurrentPageMarginsTypeKind =
    msrMarginTypeKind::kMarginBoth; // default value

  // print
  fCurrentDisplayText = "";

  // measure style handling
  fCurrentSlashTypeKind =
    msrSlashTypeKind::kSlashType_UNKNOWN_;
  fCurrentUseDotsKind =
    msrUseDotsKind::kUseDots_UNKNOWN_;
  fCurrentSlashUseStemsKind =
    msrSlashUseStemsKind::kSlashUseStems_UNKNOWN_;

  fCurrentBeatRepeatSlashes = -1;

  fCurrentMeasureRepeatKind =
    msrMeasureRepeatKind::kMeasureRepeat_UNKNOWN_;

  fCurrentMeasureRepeatMeasuresNumber = -1;
  fCurrentMeasureRepeatSlashesNumber = -1;

  fCurrentMeasureRestsNumber = 0;
  fRemainingMeasureRestsCounter = 0;

  fCurrentSlashDotsNumber = -1;
  fCurrentSlashGraphicNotesDurationKind =
    msrNotesDurationKind::kNotesDuration_UNKNOWN_;

  // staff handling
  fPreviousNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
  fCurrentNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // staff changes handling
  fCurrentRecipientStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

//   fCurrentTakeOffStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
//   fCurrentLandingStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // staff details handling
  fStaffDetailsStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  fCurrentStaffTypeKind =
    msrStaffTypeKind::kStaffTypeRegular;

  fCurrentShowFretsKind =
    msrShowFretsKind::kShowFretsNumbers; // default value

  fCurrentPrintObjectKind =
    msrPrintObjectKind::kPrintObjectYes; // default value

  fCurrentPrintSpacingKind =
    msrPrintSpacingKind::kPrintSpacingNo; // default value ??? JMI

  // staff tuning handling
  fCurrentStaffTuningAlterationKind =
     msrAlterationKind::kAlteration_UNKNOWN_;
  fCurrentStaffTuningOctaveKind =
    msrOctaveKind::kOctave_UNKNOWN_;

  fCurrentStaffDetailsCapo = 0;
  fCurrentStaffDetailsStaffSize = 0;

  // scordatura handling
  fCurrentAccordNumber = -1;
  fCurrentAccordDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentAccordAlterationKind    =
    msrAlterationKind::kAlteration_UNKNOWN_;
  fCurrentAccordOctaveKind        =
    msrOctaveKind::kOctave_UNKNOWN_;

  // voice handling
  fCurrentNoteVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;

  // measures handling
  fPartMeasuresCounter = 0;
  fScoreFirstMeasureNumber = "";
  fPartFirstMeasureNumber = "";
  fCurrentMeasureNumber = "MeasureNumber_???";

  fPreviousMeasureEndInputLineNumber = -1;

  // attributes
  fCurrentAttributesInputLineNumber = K_MF_INPUT_LINE_UNKNOWN_; // JMI v0.9.68

  // clef handling
  fCurrentClefStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
  fCurrentClefSign = "";
  fCurrentClefLine = -1;
  fCurrentClefOctaveChange = -97;

  // key handling
  fCurrentKeyKind = msrKeyKind::kKeyTraditional;

  fCurrentKeyStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
  fCurrentKeyFifths = -1;
  fCurrentKeyCancelFifths = -37;
  fCurrentModeKind = msrModeKind::kMode_UNKNOWN_;

//   // time handling
//   // time signature is crucially needed for measures management,
//   // we cannot stay without any,
//   // and there may be none in the MusicXML data
//
//   // create the default 4/4 time JMI v0.9.70
//   fCurrentTimeSignature =
//     msrTimeSignature::createFourQuartersTime (
//       0); // inputLineNumber
//
// //   // register time in staff
// //   fCurrentPart->
// //     appendTimeSignatureToPart ( // JMI v0.9.67
// //       0, // groupInputLineNumber
// //       fCurrentTimeSignature);

  fCurrentTimeSignatureSymbolKind =
    msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone; // default value

  // codas handling
  fCodasCounter = 0;

  // transpose handling
  fCurrentTransposeStaffNumber = -213;
  fCurrentTransposeDiatonic = -214;
  fCurrentTransposeChromatic = -215;

  // direction handling
  fCurrentDirectionStaffNumber = 1; // it may be absent
  fCurrentDirectionVoiceNumber = 1; // it may be absent

  // accordion-registration handling
  fCurrentAccordionHigh = 0;
  fCurrentAccordionMiddle = 0;
  fCurrentAccordionLow = 0;

  fCurrentAccordionNumbersCounter = 0;

  // metronome handling
  fCurrentMetrenomePerMinute = "";
  fCurrentMetronomeParenthesedKind =
    msrTempoParenthesizedKind::kTempoParenthesizedNo;

  fCurrentMetrenomeDotsNumber = 0;
  fCurrentMetrenomeRelationKind =
    msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone;
  fCurrentMetronomeNotesDurationKind =
    msrNotesDurationKind::kNotesDuration_UNKNOWN_;
  fCurrentMetronomeBeamValue = "";

  fCurrentMetrenomeNormalDotsNumber = 0;

  // time handling
  fCurrentTimeStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
  fCurrentTimeSignatureBeats = "";

  // lyrics handling
  fCurrentStanzaNumber = K_STANZA_NUMBER_UNKNOWN_;
  fCurrentStanzaName = K_STANZA_NAME_UNKNOWN_;

  fCurrentSyllabic = "";

  fCurrentSyllableKind =
    msrSyllableKind::kSyllableNone;
  fCurrentSyllableExtendKind =
    msrSyllableExtendKind::kSyllableExtend_NONE;

  fFirstSyllableInSlurKind =
    msrSyllableKind::kSyllableNone;
  fFirstSyllableInLigatureKind =
    msrSyllableKind::kSyllableNone;

  // harmonies handling
  fHarmoniesVoicesCounter = 0;

  fCurrentHarmonyRootDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentHarmonyRootAlterationKind =
    msrAlterationKind::kAlteration_UNKNOWN_;

  fCurrentHarmonyFunctionText = "";

  fCurrentHarmonyKind = msrHarmonyKind::kHarmony_UNKNOWN_;
  fCurrentHarmonyKindText = "";

  fCurrentHarmonyBassDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentHarmonyBassAlterationKind =
    msrAlterationKind::kAlteration_UNKNOWN_;

  fCurrentHarmonyDegreeValue = -1;
  fCurrentHarmonyDegreeAlterationKind =
    msrAlterationKind::kAlteration_UNKNOWN_;

  fCurrentHarmoniesStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  fCurrentHarmonyWholeNotesOffset = msrWholeNotes (0, 1);

  // figured bass handling
  fFiguredBassVoicesCounter = 0;

  fCurrentFiguredBassSoundingWholeNotes =
    msrWholeNotes (0, 1);
  fCurrentFiguredBassDisplayWholeNotes =
    msrWholeNotes (0, 1);
  fCurrentFiguredBassParenthesesKind =
    msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo; // default value
  fCurrentFigureNumber = -1;

  // frames handling
  fCurrentFrameStrings = -1;
  fCurrentFrameFrets = -1;
  fCurrentFrameFirstFret = -1;
  fCurrentFrameNoteStringNumber = -1;
  fCurrentFrameNoteFretNumber = -1;
  fCurrentFrameNoteFingering = -1;
  fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeNone;

  // barLine handling
  fCurrentBarLineEndingNumber = ""; // may be "1, 2"
  fCurrentBarLineHasSegnoKind =
    msrBarLineHasSegnoKind::kBarLineHasSegnoNo;
  fCurrentBarLineHasCodaKind =
    msrBarLineHasCodaKind::kBarLineHasCodaNo;

  fCurrentBarLineLocationKind =
    msrBarLineLocationKind::kBarLineLocationNone;
  fCurrentBarLineStyleKind =
    msrBarLineStyleKind::kBarLineStyleNone;
  fCurrentBarLineEndingTypeKind =
    msrBarLineEndingTypeKind::kBarLineEndingTypeNone;
  fCurrentBarLineRepeatDirectionKind =
     msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone;
  fCurrentBarLineRepeatWingedKind =
    msrBarLineRepeatWingedKind::kBarLineRepeatWingedNone;

  // repeats handling
  fRepeatEndCounter = 0;
  fCurrentRepeatStartMeasureNumber = "";

  // MusicXML notes handling
  fCurrentNoteDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentNoteAlterationKind =
    msrAlterationKind::kAlteration_UNKNOWN_;

  // note events handling
	fCurrentNoteSequentialNumber = 0;

  fCurrentNotePrintObjectKind =
    msrPrintObjectKind::kPrintObjectYes; // default value

  // note heads
  fCurrentNoteHeadKind =
    msrNoteHeadKind::kNoteHeadNormal;
  fCurrentNoteHeadFilledKind =
    msrNoteHeadFilledKind::kNoteHeadFilledYes;
  fCurrentNoteHeadParenthesesKind =
    msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo;

  // technicals handling
  fBendAlterValue = -39.3f;

  // tremolos handling
  fCurrentDoubleTremoloTypeKind =
    msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_;

  // tuplets handling
  fCurrentTupletTypeKind =
    msrTupletTypeKind::kTupletTypeNone;

  fCurrentTupletNumber = -1;
  fPreviousTupletNumber = -1;

  fCurrentTupletActualNumber = -1;
  fCurrentTupletActualType = "?";
  fCurrentTupletActualDotsNumber = -1;

  fCurrentTupletNormalNumber = -1;
  fCurrentTupletNormalType = "?";
  fCurrentTupletNormalDotsNumber = -1;

  fCurrentTupletActualNumber = -1;
  fCurrentTupletActualType = "";
  fCurrentTupletActualDotsNumber = 0;

  fCurrentTupletNormalNumber = -1;
  fCurrentTupletNormalType = "";
  fCurrentTupletNormalDotsNumber = 0;

  // backup handling
  fCurrentBackupDivisions = -1;

  // forward handling
  fCurrentForwardDivisions = 1;

  fCurrentForwardStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
  fCurrentForwardVoiceNumber = K_VOICE_NUMBER_UNKNOWN_;
}

mxsr2msrSkeletonPopulator::~mxsr2msrSkeletonPopulator ()
{}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::browseMxsr (
  const Sxmlelement& theMxsr)
{
  if (theMxsr) {
    // create a tree browser on this visitor
    tree_browser<xmlelement> browser (this);

    // browse the MXSR
    browser.browse (*theMxsr);
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::initializeNoteData ()
{
  // basic note description

// JMI  fCurrentNoteKind = msrNoteKind::kNote_UNKNOWN_;

  // transpose handling
//    fCurrentTransposeDouble = false; NO ??? JMI

  // sounding notes
  fCurrentNoteIsSounding = true; // until this get known to be false

  // rests
  fCurrentNoteIsARest = false;
  fCurrentRestIsAMeasureRest = false;

  // grace notes
  fCurrentNoteIsAGraceNote = false;

  // cue notes
  fCurrentNoteIsACueNote = false;
  fCurrentNoteIsACueNoteKind =
    msrNoteIsACueNoteKind::kNoteIsACueNoteNo;

  // unpitched notes
  fCurrentNoteIsUnpitched = false;

  // cross staff chords
//   fCurrentNoteIsCrossStaves = false;

  // tremolos handling
  fCurrentNoteBelongsToADoubleTremolo = false;

  // chords handling
  fCurrentNoteBelongsToAChord = false;

  // tuplets handling
  fCurrentNoteHasATimeModification = false; // superflous JMI ???
  fCurrentNoteBelongsToATuplet = false;

  fCurrentNoteDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentNoteAlterationKind =
    msrAlterationKind::kAlterationNatural;

  fCurrentNoteOctave = msrOctaveKind::kOctave_UNKNOWN_;

  fCurrentNoteSoundingWholeNotes =
    msrWholeNotes (0, 1);
  fCurrentNoteSoundingWholeNotesFromNotesDuration =
    msrWholeNotes (0, 1);

  fCurrentDisplayDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentDisplayOctave =
    msrOctaveKind::kOctave_UNKNOWN_;
  fCurrentNoteDisplayWholeNotes =
    msrWholeNotes (0, 1);
  fCurrentNoteDisplayWholeNotesFromType =
    msrWholeNotes (0, 1);

  // note head

  fCurrentNoteHeadKind =
    msrNoteHeadKind::kNoteHeadNormal;

  fCurrentNoteHeadFilledKind =
    msrNoteHeadFilledKind::kNoteHeadFilledYes;

  fCurrentNoteHeadParenthesesKind =
    msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo;

  // pitch

  fCurrentNoteQuarterTonesPitchKind =
    msrQuarterTonesPitchKind::kQTP_UNKNOWN_;

  fCurrentNoteOctave = msrOctaveKind::kOctave_UNKNOWN_;

  // duration

  fCurrentNoteDotsNumber = 0;

  // sounding whole notes

  fCurrentNoteSoundingWholeNotes =
    msrWholeNotes (K_WHOLE_NOTES_NUMERATOR_UNKNOWN_, 1);
  fCurrentNoteSoundingWholeNotesFromNotesDuration =
    msrWholeNotes (K_WHOLE_NOTES_NUMERATOR_UNKNOWN_, 1);

  // display whole notes

  fCurrentNoteDisplayWholeNotes =
    msrWholeNotes (K_WHOLE_NOTES_NUMERATOR_UNKNOWN_, 1);
  fCurrentNoteDisplayWholeNotesFromType =
    msrWholeNotes (K_WHOLE_NOTES_NUMERATOR_UNKNOWN_, 1);

  // display

  fCurrentNoteGraphicNotesDurationKind =
    msrNotesDurationKind::kNotesDuration_UNKNOWN_;

  fCurrentDisplayOctave =
    msrOctaveKind::kOctave_UNKNOWN_;

  fCurrentNoteQuarterTonesDisplayPitchKind =
    msrQuarterTonesPitchKind::kQTP_UNKNOWN_;

  fCurrentDisplayDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;

  // accidentals

  fCurrentAccidentalKind =
    msrAccidentalKind::kAccidentalNone; // default value

  fCurrentEditorialAccidentalKind =
    msrEditorialAccidentalKind::kEditorialAccidentalNo; // default value

  fCurrentCautionaryAccidentalKind =
    msrCautionaryAccidentalKind::kCautionaryAccidentalNo; // default value

  // current note MusicXML staff number

  fCurrentNoteStaffNumber = 1; // default value, it may be absent

  // current note MusicXML voice number

  fCurrentNoteVoiceNumber = 1; // default value, it may be absent

//   fCurrentChordStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // tuplets

  fCurrentNoteActualNotes = -1;
  fCurrentNoteNormalNotes = -1;

  fCurrentTupletActualNumber = -1;
  fCurrentTupletActualType = "";
  fCurrentTupletActualDotsNumber = 0;

  fCurrentTupletNormalNumber = -1;
  fCurrentTupletNormalType = "";
  fCurrentTupletNormalDotsNumber = 0;

  // note lyrics
  fCurrentSyllableExtendKind =
    msrSyllableExtendKind::kSyllableExtend_NONE;

// JMI  fCurrentNoteSyllableExtendKind = kSyllableExtend_NONE; v0.9.67
}

void mxsr2msrSkeletonPopulator::displayStaffAndVoiceInformation (
  int                inputLineNumber,
	const std::string& context)
{
	gLog <<
		context <<
		", fCurrentNote: ";

	if (fCurrentNote) {
	  gLog << fCurrentNote->asString ();
	}
	else {
	  gLog << "[NULL]";
	}

	gLog <<
		", fPreviousNoteStaffNumber: " <<
		mfStaffNumberAsString (fPreviousNoteStaffNumber) <<
		", fCurrentNoteStaffNumber: " <<
		mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
		", fCurrentNoteVoiceNumber: " <<
		mfVoiceNumberAsString (fCurrentNoteVoiceNumber) <<
		", fCurrentRecipientStaffNumber: " <<
		mfStaffNumberAsString (fCurrentRecipientStaffNumber) <<

		", fCurrentPartStaffMsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]: ";

	if (
	  fCurrentPartStaffMsrVoicesMap
      [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]
  ) {
		gLog <<
			fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName ();
	}
	else {
		gLog << "[NULL]";
	}

	gLog <<
		", fCurrentPartStaffMsrVoicesMap [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]: ";

	if (
	  fCurrentPartStaffMsrVoicesMap
      [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]
  ) {
		gLog <<
			fCurrentPartStaffMsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName ();
	}
	else {
		gLog << "[NULL]";
	}

	gLog <<
		", line " << inputLineNumber <<
		std::endl << std::endl;
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::printVoicesLastMetNoteMap (
  int                inputLineNumber,
  const std::string& context)
{
  size_t
    staffVoicesLastMetNoteMapSize =
      fStaffVoicesLastMetNoteMap.size ();

  gLog <<
    std::endl <<
    "fStaffVoicesLastMetNoteMap contains " <<
    mfSingularOrPlural (
      staffVoicesLastMetNoteMapSize, "stave", "staves") <<
    ", context: " << context <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  if (staffVoicesLastMetNoteMapSize) {
    ++gIndenter;

    for (
      std::pair <int, std::map <int, S_msrNote>> primaryPair :
        fStaffVoicesLastMetNoteMap
    ) {
      int
        staffNumber = primaryPair.first;

      for (
        std::pair <int, S_msrNote> secondaryPair :
          primaryPair.second
      ) {
        int
          voiceNumber = secondaryPair.first;
        S_msrNote
          note = secondaryPair.second;

        gLog <<
          "staffNumber " << staffNumber <<
          ", voiceNumber " <<  voiceNumber <<
          ":" <<
          std::endl;

        ++gIndenter;

        gLog <<
          note <<
          std::endl;

        --gIndenter;

        gLog << std::endl;
      } // for
    } // for

    --gIndenter;
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::checkStep (
  int                inputLineNumber,
  const std::string& stepValue,
  const std::string& markup)
{
  if (stepValue.size () == 1) {
    char firstChar = stepValue [0];

    if (firstChar < 'A' || firstChar > 'G') {
      std::stringstream ss;

      ss <<
        markup <<
        " value " << firstChar <<
        " is not a letter from A to G";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  else {
    std::stringstream ss;

    ss <<
      markup <<
      " value " << stepValue <<
      " should be a single letter from A to G";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::populateCurrentPartStavesVectorFromPart (
  const S_msrPart& part)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      ">>> Part \"" <<
      part->getPartName () <<
      "\" has stave numbers ranging from 1 to " <<
      fCurrentPart->getPartStavesMap ().size ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget any previous contents if any
  fCurrentPartStavesVector.clear ();

  // reserve room for enough elements // index 0 is not used
  fCurrentPartStavesVector.reserve (fCurrentPart->getPartStavesMap ().size () + 1);

  // populate the vector
  for (
    std::pair <int, S_msrStaff> thePair : fCurrentPart->getPartStavesMap ()
  ) {
    int staffNumber  = thePair.first;
    S_msrStaff staff = thePair.second;

    // register the staff
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceStaves ()) {
      std::stringstream ss;

      ss <<
        ">>> Part \"" <<
        part->getPartName () <<
        "\" registering staff " << staff <<
        "in fCurrentPartStavesVector under staffNumber " << staffNumber <<
        ", line " << staff->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentPartStavesVector [staffNumber] = staff;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    displayCurrentPartStavesVector (
      0,
      "after mxsr2msrSkeletonPopulator::populateCurrentPartStavesVectorFromPart()");
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::displayCurrentPartStavesVector (
  int                inputLineNumber,
  const std::string& context)
{
  gLog <<
    ">>> fCurrentPartStavesVector contents" <<
    ", context: " << context <<
    std::endl;

  for (S_msrStaff staff : fCurrentPartStavesVector) {
    int staffNumber = staff->getStaffNumber ();

    gLog <<
      staffNumber <<
      ": " <<
      staff->asShortString () <<
      std::endl;
  } // for

  gLog << std::endl;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::populateCurrentPartStaffVoicesMapsFromPart (
  const S_msrPart& part)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceStavesBasics ()
      ||
    gTraceOahGroup->getTraceVoicesBasics ()
  ) {
    std::stringstream ss;

    ss <<
      ">>> Populating the staff voices maps for part \"" <<
      part->getPartName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget any previous contents if any
  fCurrentPartStaffMsrVoicesMap.clear ();
  fCurrentPartStaffMxsrVoicesMap.clear ();

  // populate the voices vector
  size_t
    currentPartStaffVoicesMapSize =
      part->getPartStaffVoicesMap ().size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesBasics ()) {
    std::stringstream ss;

    ss <<
      ">>> populateCurrentPartStaffVoicesMapsFromPart(): "
      "part->getPartStaffVoicesMap () " <<
      part->getPartName () <<
      " contains " <<
      mfSingularOrPlural (
        currentPartStaffVoicesMapSize, "stave", "staves");

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (currentPartStaffVoicesMapSize) {
    ++gIndenter;

    for (
      std::pair <int, std::map <int, S_msrVoice>> primaryPair :
        part->getPartStaffVoicesMap ()
    ) {
      int
        staffNumber = primaryPair.first;

      for (
        std::pair <int, S_msrVoice> secondaryPair :
          primaryPair.second
      ) {
        int
          voiceNumber = secondaryPair.first;
        S_msrVoice
          theMsrVoice = secondaryPair.second;

        // register the regular voice
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceVoicesBasics ()) {
          std::stringstream ss;

          ss <<
            ">>> Creating the voice handler for voice " <<
            theMsrVoice <<
            " in staff " <<
            staffNumber <<
            " in part \"" <<
            part->getPartName () <<
            "\"";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentPartStaffMsrVoicesMap
          [staffNumber][voiceNumber] =
            theMsrVoice;

        // create the voice voice handler for it
        fCurrentPartStaffMxsrVoicesMap
          [staffNumber][voiceNumber] =
            mxsrVoice::create (theMsrVoice);
      } // for
    } // for

    --gIndenter;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceStavesBasics ()
      ||
    gTraceOahGroup->getTraceVoicesBasics ()
  ) {
    // display the resulting maps
    displayCurrentPartStaffMxsrVoicesMap ();

    displayCurrentPartStaffMsrVoicesMap (
      0,
      "after mxsr2msrSkeletonPopulator::populateCurrentPartStaffVoicesMapsFromPart()");
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::displayCurrentPartStaffMsrVoicesMap (
  int                inputLineNumber,
  const std::string& context)
{
  size_t
    currentPartStaffMsrVoicesMapSize =
      fCurrentPartStaffMsrVoicesMap.size ();

  gLog <<
    std::endl <<
    "fCurrentPartStaffMsrVoicesMap contains " <<
    mfSingularOrPlural (
      currentPartStaffMsrVoicesMapSize, "stave", "staves") <<
    ", context: " << context <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  if (currentPartStaffMsrVoicesMapSize) {
    ++gIndenter;

    for (
      std::pair <int, std::map <int, S_msrVoice>> primaryPair :
        fCurrentPartStaffMsrVoicesMap
    ) {
      int
        staffNumber = primaryPair.first;

      for (
        std::pair <int, S_msrVoice> secondaryPair :
          primaryPair.second
      ) {
        int
          voiceNumber = secondaryPair.first;
        S_msrVoice
          theMsrVoice = secondaryPair.second;

        gLog <<
          "staffNumber " << staffNumber <<
          ", voiceNumber " <<  voiceNumber <<
          ":" <<
          std::endl;

        ++gIndenter;

        gLog <<
          theMsrVoice <<
          std::endl;

        --gIndenter;

        gLog << std::endl;
      } // for
    } // for

    --gIndenter;
  }

  gLog << std::endl;
}

void mxsr2msrSkeletonPopulator::displayCurrentPartStaffMxsrVoicesMap ()
{
  size_t
    currentPartStaffMxsrVoicesMapSize =
      fCurrentPartStaffMxsrVoicesMap.size ();

  gLog <<
    std::endl <<
    "fCurrentPartStaffMxsrVoicesMap contains " <<
    mfSingularOrPlural (
      currentPartStaffMxsrVoicesMapSize, "stave", "staves") <<
    ":" <<
    std::endl;

  if (currentPartStaffMxsrVoicesMapSize) {
    ++gIndenter;

    for (
      std::pair <int, std::map <int, S_mxsrVoice>> primaryPair :
        fCurrentPartStaffMxsrVoicesMap
    ) {
      int
        staffNumber = primaryPair.first;

      for (
        std::pair <int, S_mxsrVoice> secondaryPair :
          primaryPair.second
      ) {
        int
          voiceNumber = secondaryPair.first;
        S_mxsrVoice
          theMxsrVoice = secondaryPair.second;

        gLog <<
          "staffNumber " << staffNumber <<
          ", voiceNumber " <<  voiceNumber <<
          ":" <<
          std::endl;

        ++gIndenter;

        gLog <<
          theMxsrVoice <<
          std::endl;

        --gIndenter;

        gLog << std::endl;
      } // for
    } // for

    --gIndenter;
  }

  gLog << std::endl;
}

//______________________________________________________________________________
S_msrVoice mxsr2msrSkeletonPopulator::fetchFirstVoiceFromCurrentPart (
  int inputLineNumber)
{
  int staffNumber = 1;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Fetching first voice in staff " <<
      staffNumber <<
      " from current part" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch the staff from current part
  S_msrStaff
    staff =
      fCurrentPartStavesVector [staffNumber];

  // fetch the first voice from the staff
  S_msrVoice
    theMsrVoice =
      staff->
        fetchFirstRegularVoiceFromStaff (
          inputLineNumber);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (! theMsrVoice) {
    staff->print (gLog); // JMI

    std::stringstream ss;

    ss <<
      "first voice not found in score skeleton's staff \"" <<
      staff->getStaffName () <<
      "\"" <<
      ", line " << inputLineNumber;

    mxsr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "--> fetchFirstVoiceFromCurrentPart() returns " <<
      theMsrVoice->getVoiceName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return theMsrVoice;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_millimeters& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_millimeters" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMillimeters = (float)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_tenths& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tenths" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTenths = (float)(*elt);
}

void mxsr2msrSkeletonPopulator::visitEnd (S_scaling& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_scaling" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a scaling
  S_msrScaling
    scaling =
      msrScaling::create (
        elt->getInputStartLineNumber (),
        fCurrentMillimeters,
        fCurrentTenths);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGeometry ()) {
    std::stringstream ss;

    ss <<
      "There are " << fCurrentTenths <<
      " tenths for " <<  fCurrentMillimeters;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set the MSR score's scaling
  fMsrScore->
    setScaling (scaling);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_system_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_system_layout" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the system layout
  fCurrentSystemLayout =
    msrSystemLayout::create (
      elt->getInputStartLineNumber ());

  fOnGoingSystemLayout = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_system_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_system_layout" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingPrint) {
    // set the current print layout's system layout
    fCurrentMusicXMLPrintLayout->
      setSystemLayout (
        fCurrentSystemLayout);
  }
  else {
    // set the MSR score system layout
    fMsrScore->
      setSystemLayout (
        fCurrentSystemLayout);
  }

  // forget about the current system layout
  fCurrentSystemLayout = nullptr;

  fOnGoingSystemLayout = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_system_margins& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_system_margins" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fOnGoingSystemLayout) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<system-margins /> is out of context");
  }

  fOnGoingSystemMargins = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_system_distance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_system_distance" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingSystemLayout) {
    float systemDistanceTenths = (float)(*elt);

    fCurrentSystemLayout->
      setSystemDistance (
        msrLength::create (
          msrLengthUnitKind::kUnitMillimeter,
          systemDistanceTenths * fCurrentMillimeters / fCurrentTenths));
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<system-distance /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_top_system_distance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_top_system_distance" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingSystemLayout) {
    float topSystemDistanceTenths = (float)(*elt);

    fCurrentSystemLayout->
      setTopSystemDistance (
        msrLength::create (
          msrLengthUnitKind::kUnitMillimeter,
          topSystemDistanceTenths * fCurrentMillimeters / fCurrentTenths));
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<top-system-distance /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_system_margins& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_system_margins" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fOnGoingSystemLayout) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<system-margins /> is out of context");
  }

  fOnGoingSystemMargins = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_system_dividers& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_system_dividers" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    std::stringstream ss;

    ss <<
      "<system-dividers /> is not supported yet by " <<
      gOahOahGroup->getOahOahGroupServiceName ();

/* JMI
    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());
     */
}

void mxsr2msrSkeletonPopulator::visitStart (S_left_divider& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_left_divider" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);
}

void mxsr2msrSkeletonPopulator::visitStart (S_right_divider& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_right_divider" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_notations& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_notations" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI
}

void mxsr2msrSkeletonPopulator::visitStart (S_other_notation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_other_notation" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_page_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_page_layout" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a page layout
  fCurrentPageLayout =
    msrPageLayout::create (
      elt->getInputStartLineNumber ());

  fOnGoingPageLayout = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_page_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_page_layout" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingPrint) {
    // JMI
  }
  else {
    // set the score page layout
    fMsrScore->
      setPageLayout (fCurrentPageLayout);
  }

  fOnGoingPageLayout = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_page_height& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_page_height" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingPageLayout) {
    float pageHeight = (float)(*elt);

    fCurrentPageLayout->
      setPageHeight (
        msrLength::create (
          msrLengthUnitKind::kUnitMillimeter,
          pageHeight * fCurrentMillimeters / fCurrentTenths));
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<page-height /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_page_width& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_page_width" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingPageLayout) {
    float pageWidth = (float)(*elt);

    fCurrentPageLayout->
      setPageWidth (
        msrLength::create (
          msrLengthUnitKind::kUnitMillimeter,
          pageWidth * fCurrentMillimeters / fCurrentTenths));
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<page-width /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_page_margins& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_page_margins" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingPageLayout) {
    std::string pageMarginsType =
      elt->getAttributeValue ("type");

    fCurrentPageMarginsTypeKind = msrMarginTypeKind::kMarginBoth; // default value

    if      (pageMarginsType == "odd")
      fCurrentPageMarginsTypeKind = msrMarginTypeKind::kMarginOdd;
    else if (pageMarginsType == "even")
      fCurrentPageMarginsTypeKind = msrMarginTypeKind::kMarginEven;
    else if (pageMarginsType == "both")
      fCurrentPageMarginsTypeKind = msrMarginTypeKind::kMarginBoth;
    else if (pageMarginsType. size ()) {
      std::stringstream ss;

      ss <<
        "unknown page margins type \"" <<
        pageMarginsType <<
        "\"";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<page-margins /> is out of context");
  }

  // create a margins group
  fCurrentPageLayoutMarginsGroup =
    msrMarginsGroup::create (
      fCurrentPageMarginsTypeKind);

  // create a margins group if not yet done
  switch (fCurrentPageMarginsTypeKind) {
    case msrMarginTypeKind::kMarginOdd:
      fCurrentPageLayout->
        setOddMarginsGroup (
          elt->getInputStartLineNumber (),
          fCurrentPageLayoutMarginsGroup);
      break;
    case msrMarginTypeKind::kMarginEven:
      fCurrentPageLayout->
        setEvenMarginsGroup (
          elt->getInputStartLineNumber (),
          fCurrentPageLayoutMarginsGroup);
      break;
    case msrMarginTypeKind::kMarginBoth: // default value
      fCurrentPageLayout->
        setBothMarginsGroup (
          elt->getInputStartLineNumber (),
          fCurrentPageLayoutMarginsGroup);
      break;
  } // switch

  fOnGoingPageMargins = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_page_margins& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_page_margins" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current page layout margins group
  fCurrentPageLayoutMarginsGroup = nullptr;

  fOnGoingPageMargins = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_left_margin& elt)
{
  float leftMargin = (float)(*elt);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_left_margin" <<
      ", " << leftMargin << " tenths" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  msrLength
    leftMarginLength (
      msrLengthUnitKind::kUnitMillimeter,
      leftMargin * fCurrentMillimeters / fCurrentTenths);

  if (fOnGoingPageMargins) {
    fCurrentPageLayoutMarginsGroup->
      setLeftMargin (
        elt->getInputStartLineNumber (),
        msrMargin::create (
          fCurrentPageMarginsTypeKind,
          leftMarginLength));
  }

  else if (fOnGoingSystemMargins) {
    fCurrentSystemLayout->
      setLeftMargin (
        msrMargin::create (
          fCurrentPageMarginsTypeKind,
          msrLength (
            msrLengthUnitKind::kUnitMillimeter,
            leftMargin * fCurrentMillimeters / fCurrentTenths)));
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<left-margin /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_right_margin& elt)
{
  float rightMargin = (float)(*elt);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_right_margin" <<
      ", " << rightMargin << " tenths" <<
       ", line " << elt->getInputStartLineNumber () <<
     std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  msrLength
    rightMarginLength (
      msrLengthUnitKind::kUnitMillimeter,
      rightMargin * fCurrentMillimeters / fCurrentTenths);

  if (fOnGoingPageMargins) {
    fCurrentPageLayoutMarginsGroup->
      setRightMargin (
        elt->getInputStartLineNumber (),
        msrMargin::create (
          fCurrentPageMarginsTypeKind,
          rightMarginLength));
  }

  else if (fOnGoingSystemMargins) {
    fCurrentSystemLayout->
      setRightMargin (
        msrMargin::create (
          fCurrentPageMarginsTypeKind,
          msrLength (
            msrLengthUnitKind::kUnitMillimeter,
            rightMargin * fCurrentMillimeters / fCurrentTenths)));
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<right-margin /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_top_margin& elt)
{
  float topMargin = (float)(*elt);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_top_margin" <<
      ", " << topMargin << " tenths" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingPageMargins) {
    fCurrentPageLayoutMarginsGroup->
      setTopMargin (
        elt->getInputStartLineNumber (),
        msrMargin::create (
          fCurrentPageMarginsTypeKind,
          msrLength (
            msrLengthUnitKind::kUnitMillimeter,
            topMargin * fCurrentMillimeters / fCurrentTenths)));
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<top-margin /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_bottom_margin& elt)
{
  float bottomMargin = (float)(*elt);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bottom_margin" <<
      ", " << bottomMargin << " tenths" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingPageMargins) {
    fCurrentPageLayoutMarginsGroup->
      setBottomMargin (
        elt->getInputStartLineNumber (),
        msrMargin::create (
          fCurrentPageMarginsTypeKind,
          msrLength (
            msrLengthUnitKind::kUnitMillimeter,
            bottomMargin * fCurrentMillimeters / fCurrentTenths)));
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<bottom-margin /> is out of context");
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_staff_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_layout" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
From DalSegno.xml: JMI there is no <staff-distance /> ...
        <staff-layout>
          <?DoletSibelius JustifyAllStaves=false?>
          <?DoletSibelius ExtraSpacesAbove=3?>
        </staff-layout>
*/

  // number
  int staffNumber = elt->getAttributeIntValue ("number", 0);

  // create a staff layout
  fCurrentStaffLayout =
    msrStaffLayout::create (
      elt->getInputStartLineNumber (),
      staffNumber);

  if (fOnGoingPrint) {
    // append it to the current print layout
    fCurrentMusicXMLPrintLayout->
      appendStaffLayout (
        fCurrentStaffLayout);
  }
  else {
    // set the MSR score staff layout
    fMsrScore->
      setStaffLayout (
        fCurrentStaffLayout);
  }

  fOnGoingStaffLayout = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_staff_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_staff_layout" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current staff layout
  fCurrentStaffLayout = nullptr;

  fOnGoingStaffLayout = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_staff_distance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_distance" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingStaffLayout) {
    float staffDistanceTenths = (float)(*elt);

    fCurrentStaffLayout->
      setStaffDistance (
        msrLength::create (
          msrLengthUnitKind::kUnitMillimeter,
          staffDistanceTenths * fCurrentMillimeters / fCurrentTenths));
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<staff-distance /> is out of context");
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_measure_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()
  ) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_layout" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a measure layout
  fCurrentMeasureLayout =
    msrMeasureLayout::create (
      elt->getInputStartLineNumber ());

  fOnGoingMeasureLayout = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_measure_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_measure_layout" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current measure layout
  fCurrentMeasureLayout = nullptr;

  fOnGoingMeasureLayout = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_measure_distance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_distance" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingMeasureLayout) {
    float measureDistanceTenths = (float)(*elt);

    fCurrentMeasureLayout->
      setMeasureDistance (
        msrLength::create (
          msrLengthUnitKind::kUnitMillimeter,
          measureDistanceTenths * fCurrentMillimeters / fCurrentTenths));
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<measure-distance /> is out of context");
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_appearance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_appearance" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
<!--
  The appearance element controls general graphical
  settings for the music's final form appearance on a
  printed page of display. This includes support
  for line widths, definitions for note sizes, and standard
  distances between notation elements, plus an extension
  element for other aspects of appearance.

  The line-width element indicates the width of a line type
  in tenths. The type attribute defines what type of line is
  being defined. Values include beam, bracket, dashes,
  enclosure, ending, extend, heavy barLine, leger,
  light barLine, octave shift, pedal, slur middle, slur tip,
  staff, stem, tie middle, tie tip, tuplet bracket, and
  wedge. The text content is expressed in tenths.

  The note-size element indicates the percentage of the
  regular note size to use for notes with a cue and large
  size as defined in the type element. The grace type is
  used for notes of cue size that that include a grace
  element. The cue type is used for all other notes with
  cue size, whether defined explicitly or implicitly via a
  cue element. The large type is used for notes of large
  size. The text content represent the numeric percentage.
  A value of 100 would be identical to the size of a regular
  note as defined by the music font.

  The distance element represents standard distances between
  notation elements in tenths. The type attribute defines what
  type of distance is being defined. Values include hyphen
  (for hyphens in lyrics) and beam.

  The glyph element represents what SMuFL glyph should be used
  for different variations of symbols that are semantically
  identical. The type attribute specifies what type of glyph
  is being defined. The element value specifies what
  SMuFL glyph to use, including recommended stylistic
  regulars.

  Glyph type attribute values include quarter-rest,
  g-clef-ottava-bassa, c-clef, f-clef, percussion-clef,
  octave-shift-up-8, octave-shift-down-8,
  octave-shift-continue-8, octave-shift-down-15,
  octave-shift-up-15, octave-shift-continue-15,
  octave-shift-down-22, octave-shift-up-22, and
  octave-shift-continue-22. A quarter-rest type specifies the
  glyph to use when a note has a rest element and a type value
  of quarter. The c-clef, f-clef, and percussion-clef types
  specify the glyph to use when a clef sign element value is C,
  F, or percussion respectively. The g-clef-ottava-bassa type
  specifies the glyph to use when a clef sign element value is
  G and the clef-octave-change element value is -1. The
  octave-shift types specify the glyph to use when an
  octave-shift type attribute value is up, down, or continue
  and the octave-shift size attribute value is 8, 15, or 22.

  The SMuFL glyph name should match the type. For instance,
  a type of quarter-rest would use values restQuarter,
  restQuarterOld, or restQuarterZ. A type of g-clef-ottava-bassa
  would use values gClef8vb, gClef8vbOld, or gClef8vbCClef. A
  type of octave-shift-up-8 would use values ottava, ottavaBassa,
  ottavaBassaBa, ottavaBassaVb, or octaveBassa.

  The other-appearance element is used to define any
  graphical settings not yet in the current version of the
  MusicXML format. This allows extended representation,
  though without application interoperability.
-->
<!ELEMENT appearance
  (line-width*, note-size*, distance*,
   other-appearance*)>
<!ELEMENT line-width %layout-tenths;>
<!ATTLIST line-width
    type CDATA #REQUIRED
>
<!ELEMENT note-size (#PCDATA)>
<!ATTLIST note-size
    type (cue | grace | large) #REQUIRED
>
<!ELEMENT distance %layout-tenths;>
<!ATTLIST distance
    type CDATA #REQUIRED
>
<!ELEMENT other-appearance (#PCDATA)>
<!ATTLIST other-appearance
    type CDATA #REQUIRED
>
*/

  // create an appearance
  fCurrentAppearance =
    msrAppearance::create (
      elt->getInputStartLineNumber ());

  // set the MSR score appearance
  fMsrScore->
    setAppearance (
      fCurrentAppearance);

  fOnGoingAppearance = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_appearance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_appearance" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current appearance
  fCurrentAppearance = nullptr;

  fOnGoingAppearance = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_line_width& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_line_width" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingAppearance) {
    // value
    float lineWidthTenths = (float)(*elt);

    // type
    std::string lineWidthTypeString = elt->getAttributeValue ("type");

    if (lineWidthTypeString.size ()) {
      msrLineWidthTypeKind
        lineWidthTypeKind =
          msrLineWidthTypeKind::kLineWidthType_UNKNOWN_;

      if      (lineWidthTypeString == "beam")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeBeam;
      else if (lineWidthTypeString == "bracket")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeBracket;
      else if (lineWidthTypeString == "dashes")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeDashes;
      else if (lineWidthTypeString == "enclosure")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeEnclosure;
      else if (lineWidthTypeString == "ending")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeEnding;
      else if (lineWidthTypeString == "extend")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeExtend;
      else if (lineWidthTypeString == "heavy barline")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeHeavyBarLine;
      else if (lineWidthTypeString == "leger")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeHeavyLeger;
      else if (lineWidthTypeString == "light barline")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeLightBarLine;
      else if (lineWidthTypeString == "octave shift")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeOctaveShift;
      else if (lineWidthTypeString == "pedal")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypePedal;
      else if (lineWidthTypeString == "slur middle")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeSlurMiddle;
      else if (lineWidthTypeString == "slur tip")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeSlurTip;
      else if (lineWidthTypeString == "staff")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeStaff;
      else if (lineWidthTypeString == "stem")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeStem;
      else if (lineWidthTypeString == "tie middle")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeTieMiddle;
      else if (lineWidthTypeString == "tie tip")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeTieTip;
      else if (lineWidthTypeString == "tuplet bracket")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeTupletBracket;
      else if (lineWidthTypeString == "wedge")
        lineWidthTypeKind = msrLineWidthTypeKind::kLineWidthTypeWedge;

      else {
        std::stringstream ss;

        ss <<
          "<line-width /> type \"" <<
          lineWidthTypeString <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }

      // create a line width
      S_msrLineWidth
        lineWidth =
          msrLineWidth::create (
            elt->getInputStartLineNumber (),
            lineWidthTypeKind,
            msrLength::create (
              msrLengthUnitKind::kUnitMillimeter,
              lineWidthTenths * fCurrentMillimeters / fCurrentTenths));

      // append it to the current appearance
      fCurrentAppearance->
        appendLineWidth (lineWidth);
    }
    else {
      std::stringstream ss;

      ss <<
        "<line-width /> type \"" <<
        lineWidthTypeString <<
        "\" is missing";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<line-width /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_note_size& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note_size" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingAppearance) {
    // value
    float noteSizePercentage = (float)(*elt);

    // type
    std::string noteSizeTypeString = elt->getAttributeValue ("type");

    if (noteSizeTypeString.size ()) {
      msrNoteSizeTypeKind
        noteSizeTypeKind =
          msrNoteSizeTypeKind::kNoteSizeType_UNKNOWN_;

      if      (noteSizeTypeString == "full")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeFull;
      else if (noteSizeTypeString == "grace")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeGrace;
      else if (noteSizeTypeString == "cue")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeCue;
      else if (noteSizeTypeString == "grace-cue")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeGraceCue;
      else if (noteSizeTypeString == "large")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeLarge;

      else {
        std::stringstream ss;

        ss <<
          "<note-size /> type \"" <<
          noteSizeTypeString <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }

      // create a note size
      S_msrNoteSize
        noteSize =
          msrNoteSize::create (
            elt->getInputStartLineNumber (),
            noteSizeTypeKind,
            noteSizePercentage);

      // append it to the current appearance
      fCurrentAppearance->
        appendNoteSize (noteSize);
    }
    else {
      std::stringstream ss;

      ss <<
        "<note-size /> type \"" <<
        noteSizePercentage <<
        "\" is missing";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<note-size /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_distance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_distance" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingAppearance) {
    // value
    float distanceTenths = (float)(*elt);

    // type
    std::string distanceTypeString = elt->getAttributeValue ("type");

    if (distanceTypeString.size ()) {
      msrDistanceTypeKind
        distanceTypeKind =
          msrDistanceTypeKind::kDistanceType_UNKNOWN_;

      if      (distanceTypeString == "hyphen")
        distanceTypeKind = msrDistanceTypeKind::kDistanceTypeHyphen;
      else if (distanceTypeString == "beam")
        distanceTypeKind = msrDistanceTypeKind::kDistanceTypeBeam;

      else {
        std::stringstream ss;

        ss <<
          "<distance /> type \"" <<
          distanceTypeString <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }

      // create a distance
      S_msrDistance
        distance =
          msrDistance::create (
            elt->getInputStartLineNumber (),
            distanceTypeKind,
            msrLength::create (
              msrLengthUnitKind::kUnitMillimeter,
              distanceTenths * fCurrentMillimeters / fCurrentTenths));

      // append it to the current appearance
      fCurrentAppearance->
        appendDistance (distance);
    }
    else {
      std::stringstream ss;

      ss <<
        "<distance /> type \"" <<
        distanceTypeString <<
        "\" is missing";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<distance /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_glyph& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_glyph" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingAppearance) {
    // value
    std::string glyphValue = elt->getValue ();

    // type
    std::string glyphTypeString = elt->getAttributeValue ("type");

    if (glyphTypeString.size ()) {
      msrGlyphTypeKind
        glyphTypeKind =
          msrGlyphTypeKind::kGlyphType_UNKNOWN_;

      if      (glyphTypeString == "quarter-rest")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeQuarterRest;
      else if (glyphTypeString == "g-clef-ottava-bassa")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeGClefOttavaBassa;
      else if (glyphTypeString == "c-clef")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeCClef;
      else if (glyphTypeString == "f-clef")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeFClef;
      else if (glyphTypeString == "percussion-clef")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypePercussionClef;
      else if (glyphTypeString == "octave-shift-up-8")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeOctaveShiftUp8;
      else if (glyphTypeString == "octave-shift-down-8")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeOctaveShiftDown8;
      else if (glyphTypeString == "octave-shift-continue-8")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeOctaveShiftContinue8;
      else if (glyphTypeString == "octave-shift-down-15")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeOctaveShiftDown15;
      else if (glyphTypeString == "octave-shift-up-15")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeOctaveShiftUp15;
      else if (glyphTypeString == "octave-shift-continue-15")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeOctaveShiftContinue15;
      else if (glyphTypeString == "octave-shift-down-22")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeOctaveShiftDown22;
      else if (glyphTypeString == "octave-shift-up-22")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeOctaveShiftUp22;
      else if (glyphTypeString == "octave-shift-continue-22")
        glyphTypeKind = msrGlyphTypeKind::kGlyphTypeOctaveShiftContinue22;

      else {
        std::stringstream ss;

        ss <<
          "<glyph /> type \"" <<
          glyphTypeString <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }

      // create a glyph
      S_msrGlyph
        glyph =
          msrGlyph::create (
            elt->getInputStartLineNumber (),
            glyphTypeKind,
            glyphValue);

      // append it to the current appearance
      fCurrentAppearance->
        appendGlyph (glyph);
    }
    else {
      std::stringstream ss;

      ss <<
        "<glyph /> type \"" <<
        glyphTypeString <<
        "\" is missing";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<glyph /> is out of context");
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_other_appearance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_other_appearance" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingAppearance) {
    // what can we find in such a markup??? JMI
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<other-appearance /> is out of context");
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_part& elt)
{
//   gLog << elt; //JMI v0.9.66 create MusicFormats's own smart pointer type ???

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string partID = elt->getAttributeValue ("id");

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  if (
    gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
  ) {
#ifdef MF_TRACE_IS_ENABLED
    if (
      gTraceOahGroup->getTraceParts ()
    ) {
      std::stringstream ss;

      ss <<
        "<!--=== partID \"" << partID << "\"" <<
        ", line " << elt->getInputStartLineNumber () << " ===-->";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

  // fetch current part from its partID
  fCurrentPart =
    fMsrScore->
      fetchPartFromScoreByItsPartID (
        elt->getInputStartLineNumber (),
        partID);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (! fCurrentPart) {
    // fetch fMsrScore's part list
    std::list <S_msrPart> partsList;

    fMsrScore->
      collectScorePartsList (
        elt->getInputStartLineNumber (),
        partsList);

    if (partsList.size () == 0) {
      std::stringstream ss;

      fMsrScore->
        printSummary (gLog); // JMI v0.9.69

      gLog <<
        fMsrScore; // JMI v0.9.69

      fMsrScore->
        displayPartGroupsList ("visitStart (S_part& elt)");

      ss <<
        "part \"" << partID <<
        "\" not found in score skeleton" <<
        ", line " << elt->getInputStartLineNumber ();

      mxsr2msrInternalError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }

    else {
      // there's only one part in the part list,
      // assume this is the one JMI v0.9.70
      fCurrentPart =
        partsList.front ();

      partID =
        fCurrentPart->
          getPartID ();

      std::stringstream ss;

      ss <<
        "part 'id' is empty, using " <<
        partID <<
        " since it is the only part in the <part-list />";

      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        ss.str ());
    }
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "%--------------------------------------------------------------" <<
      std::endl <<
      "   ===>> " <<
      std::endl <<
      "      " <<
      "part " <<
      fCurrentPart-> fetchPartCombinedName () <<
      ", line " <<
      elt->getInputStartLineNumber () <<
      std::endl <<
      "   <<===" <<
      std::endl <<
      "%--------------------------------------------------------------" <<
      std::endl;

    gWaeHandler->waeTraceWithoutInputLocation ( // JMI v0.9.71 without CODE location ???
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the current part in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  if (fCurrentPart) {
    serviceRunData->
      setCurrentPartIDAndName (
        fCurrentPart->getPartIDAndName ());
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoicesBasics ()) {
    // display the current part's voices map
    fCurrentPart->
      displayPartStaffVoicesMap (
        elt->getInputStartLineNumber (),
        "mxsr2msrSkeletonPopulator::visitStart (S_part& elt)");
  }
#endif

  // populate the staves vector from the current part
  populateCurrentPartStavesVectorFromPart (fCurrentPart);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    // display the stave vectors
    displayCurrentPartStavesVector (
      elt->getInputStartLineNumber (),
      "mxsr2msrSkeletonPopulator::visitStart (S_part& elt)");
  }
#endif

  // populate the voices vector from the current part
  populateCurrentPartStaffVoicesMapsFromPart (fCurrentPart);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    // display the voices vectors
    displayCurrentPartStaffMsrVoicesMap (
      elt->getInputStartLineNumber (),
      "mxsr2msrSkeletonPopulator::visitStart (S_part& elt)");
  }
#endif

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "--------------------------------------------" <<
      std::endl <<
      "Analyzing part " <<
      fCurrentPart->
        fetchPartCombinedName () <<
        " -- start";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // no time has been defined yet
  fCurrentTimeSignature = nullptr;

  // staff numbers
  fCurrentNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
//   fCurrentNoteStaffNumber = 1; // JMI v0.9.71 default voice number

  fPreviousNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // voice numbers
//   fCurrentNoteVoiceNumber = K_STAFF_NUMBER_UNKNOWN_;
  fCurrentNoteVoiceNumber = 1; // JMI v0.9.71 default voice number

  // staff change detection

  // cross staff chords
//   fCurrentNoteIsCrossStaves = false; // needed ??? JMI v0.9.68

  // get this part's staves map
  std::map <int, S_msrStaff>
    partStavesMap =
      fCurrentPart->
        getPartStavesMap ();

  // repeats
  fCurrentRepeatStartMeasureNumber = "";
  fCurrentRepeatEndingStartBarLine = nullptr;

  // measures
  fPartMeasuresCounter = 0;
  fScoreFirstMeasureNumber = "";
  fPartFirstMeasureNumber = "";
  fCurrentMeasureNumber = "MeasureNumber_???";

  fPreviousMeasureEndInputLineNumber = -1;

  ++gIndenter; // will be decremented in visitEnd (S_part& elt)
}

void mxsr2msrSkeletonPopulator::visitEnd (S_part& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // has been incremented in visitEnd (S_part& elt)

/*
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Analyzing part " <<
      fCurrentPart->
        fetchPartCombinedName () <<
        " -- end" <<
      std::endl <<
      "--------------------------------------------";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
*/

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCurrentNoteVoiceNumber != K_VOICE_NUMBER_UNKNOWN_, // JMI v0.9.72 ???
    "fCurrentNoteVoiceNumber is unknown");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // the elements pending since before the note if any
  // can now be appended to the latter's voice
  // prior to the note itself
//   attachPendingVoiceLevelElementsToVoice (
//     fCurrentPartStaffMsrVoicesMap
//         [fCurrentNoteVoiceNumber]);

  attachPendingPartLevelElementsIfAnyToPart (
    fCurrentPart);

  // finalize the current part
  fCurrentPart->
    finalizePart (
      elt->getInputStartLineNumber ());

  // is this part name in the parts ignore IDs set?
  if (gGlobalMxsr2msrOahGroup->getPartsIgnoreIDSet ().size ()) {
    std::set <std::string>::iterator
      it =
        gGlobalMxsr2msrOahGroup->getPartsIgnoreIDSet ().find (
          fCurrentPart->
            getPartID ());

    if (it != gGlobalMxsr2msrOahGroup->getPartsIgnoreIDSet ().end ()) {
      // the simplest way to ignore this part
      // is to remove it from its part-group
      // now that is has been completely built and populated
      fCurrentPart->
        getPartUpLinkToPartGroup ()->
          removePartFromPartGroup (
            elt->getInputStartLineNumber (),
            fCurrentPart);
    }
  }

  // is this part name in the parts keep IDs set?
  if (gGlobalMxsr2msrOahGroup->getMusicXMLPartsKeepIDSet ().size ()) {
    std::set <std::string>::iterator
      it =
        gGlobalMxsr2msrOahGroup->getMusicXMLPartsKeepIDSet ().find (
          fCurrentPart->
            getPartID ());

    if (it == gGlobalMxsr2msrOahGroup->getMusicXMLPartsKeepIDSet ().end ()) {
      // the simplest way not to keep this part
      // is to remove it from its part-group
      // now that is has been completely built and populated
      fCurrentPart->
        getPartUpLinkToPartGroup ()->
          removePartFromPartGroup (
            elt->getInputStartLineNumber (),
            fCurrentPart);
    }
  }

  // is this part name in the parts ignore names set?
  if (gGlobalMxsr2msrOahGroup->getMusicXMLPartsIgnoreNameSet ().size ()) {
    std::set <std::string>::iterator
      it =
        gGlobalMxsr2msrOahGroup->getMusicXMLPartsIgnoreNameSet ().find (
          fCurrentPart->
            getPartName ());

    if (it != gGlobalMxsr2msrOahGroup->getMusicXMLPartsIgnoreNameSet ().end ()) {
      // the simplest way to ignore this part
      // is to remove it from its part-group
      // now that is has been completely built and populated
      fCurrentPart->
        getPartUpLinkToPartGroup ()->
          removePartFromPartGroup (
            elt->getInputStartLineNumber (),
            fCurrentPart);
    }
  }

  // is this part name in the parts keep names set?
  if (gGlobalMxsr2msrOahGroup->getMusicXMLPartsKeepNameSet ().size ()) {
    std::set <std::string>::iterator
      it =
        gGlobalMxsr2msrOahGroup->getMusicXMLPartsKeepNameSet ().find (
          fCurrentPart->
            getPartName ());

    if (it == gGlobalMxsr2msrOahGroup->getMusicXMLPartsKeepNameSet ().end ()) {
      // the simplest way not to keep this part
      // is to remove it from its part-group
      // now that is has been completely built and populated
      fCurrentPart->
        getPartUpLinkToPartGroup ()->
          removePartFromPartGroup (
            elt->getInputStartLineNumber (),
            fCurrentPart);
    }
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_attributes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_attributes" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the clef, key and time signagure inside one and the same <attribute /> markup
  // are recognized as such by the latter's input line number, which they share
  fCurrentAttributesInputLineNumber = elt->getInputStartLineNumber ();

//   fOnGoingClefKeyTimeSignatureGroup = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_attributes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_attributes" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // have the divisions been defined alright?
  if (! fCurrentDivisions) {
    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      "no <divisions/> markup found in MusicXML data, using 1 by default");

    fCurrentDivisionsPerQuarterNote = 1;

    // create default current divisions
    fCurrentDivisions =
      msrDivisions::create (
        elt->getInputStartLineNumber (),
        fCurrentDivisionsPerQuarterNote);
  }

//   fOnGoingClefKeyTimeSignatureGroup = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_divisions& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_divisions" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentDivisionsPerQuarterNote = (int)(*elt);

  if (
    fCurrentDivisionsPerQuarterNote < 1
      ||
    fCurrentDivisionsPerQuarterNote > 16383
  ) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "divisions per quarter note should be between 1 and 16383");
  }

  // set current part's divisions per quarter note
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
    std::stringstream ss;

    if (fCurrentDivisionsPerQuarterNote == 1) {
      ss <<
        "There is 1 division";
    }
    else {
      ss <<
        "There are " <<
        fCurrentDivisionsPerQuarterNote <<
        " divisions";
    }

    ss <<
      " per quarter note in part " <<
      fCurrentPart->fetchPartCombinedName() <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create current divisions
  fCurrentDivisions =
    msrDivisions::create (
      elt->getInputStartLineNumber (),
      fCurrentDivisionsPerQuarterNote);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_clef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_clef" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // https://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-clef.htm

  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.

  fCurrentClefStaffNumber =
    elt->getAttributeIntValue ("number", 0);

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

  fCurrentClefLine = 0;
  fCurrentClefOctaveChange = 0;
  fCurrentClefSign = "";
}

void mxsr2msrSkeletonPopulator::visitStart (S_sign& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sign" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentClefSign = elt->getValue();
}

void mxsr2msrSkeletonPopulator::visitStart (S_line& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_line" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentClefLine = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_clef_octave_change& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_clef_octave_change" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentClefOctaveChange = (int)(*elt);

  if (fCurrentClefOctaveChange < -2 || fCurrentClefOctaveChange > 2) {
    std::stringstream ss;

    ss <<
      "clef-octave-change \"" << fCurrentClefOctaveChange <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_clef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_clef" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // convert clef to upper case for analysis
  std::string currentClefSignToUpperCase =
    mfStringToUpperCase (fCurrentClefSign);

  msrClefKind clefKind = msrClefKind::kClef_UNKNOWN_;

  if (currentClefSignToUpperCase == "G") {

    if      (fCurrentClefLine == 2) {
      switch (fCurrentClefOctaveChange) {
        case -2:
          clefKind = msrClefKind::kClefTrebleMinus15;
          break;
        case -1:
          clefKind = msrClefKind::kClefTrebleMinus8;
          break;
        case 0:
          clefKind = msrClefKind::kClefTreble;
          break;
        case +1:
          clefKind = msrClefKind::kClefTreblePlus8;
          break;
        case +2:
          clefKind = msrClefKind::kClefTreblePlus15;
          break;
        default:
          {
            // should not occur
          }
      } // switch
    }

    else if (fCurrentClefLine == 1)
      clefKind = msrClefKind::kClefTrebleLine1;

    else {
      std::stringstream ss;

      ss <<
        "'G' clef line \"" << fCurrentClefLine <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  else if (currentClefSignToUpperCase == "F") {

    if ( fCurrentClefLine == 4 ) {
      switch (fCurrentClefOctaveChange) {
        case -2:
          clefKind = msrClefKind::kClefBassMinus15;
          break;
        case -1:
          clefKind = msrClefKind::kClefBassMinus8;
          break;
        case 0:
          clefKind = msrClefKind::kClefBass;
          break;
        case +1:
          clefKind = msrClefKind::kClefBassPlus8;
          break;
        case +2:
          clefKind = msrClefKind::kClefBassPlus15;
          break;
        default:
          {
            // should not occur
          }
      } // switch
    }

    else if ( fCurrentClefLine == 3 ) {
      clefKind = msrClefKind::kClefVarbaritone;
      /* JMI
      switch (fCurrentClefOctaveChange) {
        case -2:
          clefKind = msrClefKind::kClefBassMinus15;
          break;
        case -1:
          clefKind = msrClefKind::kClefBassMinus8;
          break;
        case 0:
          clefKind = msrClefKind::kClefBass;
          break;
        case +1:
          clefKind = msrClefKind::kClefBassPlus8;
          break;
        case +2:
          clefKind = msrClefKind::kClefBassPlus15;
          break;
        default:
          {
            // should not occur
          }
      } // switch
      */
    }

    else {
      std::stringstream ss;

      ss <<
        "'F' clef line \"" << fCurrentClefLine <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  else if (currentClefSignToUpperCase == "C") {

    switch (fCurrentClefLine) {
      case 1:
        clefKind = msrClefKind::kClefSoprano;
        break;
      case 2:
        clefKind = msrClefKind::kClefMezzoSoprano;
        break;
      case 3:
        clefKind = msrClefKind::kClefAlto;
        break;
      case 4:
        clefKind = msrClefKind::kClefTenor;
        break;
      case 5:
        clefKind = msrClefKind::kClefBaritone;
        break;
      default:
        {
          std::stringstream ss;

          ss <<
            "'C' clef line \"" << fCurrentClefLine <<
            "\" is unknown";

          mxsr2msrError (
            gServiceRunData->getInputSourceName (),
            elt->getInputStartLineNumber (),
            __FILE__, __LINE__,
            ss.str ());
        }
    } // switch
  }

  else if (currentClefSignToUpperCase == "TAB") {

    switch (fCurrentClefLine) {
      case 4:
        clefKind = msrClefKind::kClefTablature4;
        break;
      case 5:
        clefKind = msrClefKind::kClefTablature5;
        break;
      case 6:
        clefKind = msrClefKind::kClefTablature6;
        break;
      case 7:
        clefKind = msrClefKind::kClefTablature7;
        break;
      default:
        {
          std::stringstream ss;

          ss <<
            "tablature line \"" << fCurrentClefLine <<
            "\" is unknown";

          mxsr2msrError (
            gServiceRunData->getInputSourceName (),
            elt->getInputStartLineNumber (),
            __FILE__, __LINE__,
            ss.str ());
        }
    } // switch
  }

  else if (currentClefSignToUpperCase == "PERCUSSION") {
    clefKind = msrClefKind::kClefPercussion;
  }

  else if (currentClefSignToUpperCase == "JIANPU") {
    clefKind = msrClefKind::kClefJianpu;
  }

  else if (currentClefSignToUpperCase == "NONE") {
    clefKind = msrClefKind::kClef_UNKNOWN_;
  }

  else {
    // unknown clef sign
    std::stringstream ss;

    ss <<
      "clef sign \"" << fCurrentClefSign <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // is this clef sign in the replace clef map?
  const std::map <msrClefKind, msrClefKind>&
    replaceClefKindToClefKindMapVariable =
      gGlobalMxsr2msrOahGroup->
        getReplaceClefKindToClefKindMapVariable ();

  std::map <msrClefKind, msrClefKind>::const_iterator
    it =
      replaceClefKindToClefKindMapVariable.find (clefKind);

  if (it != replaceClefKindToClefKindMapVariable.end ()) {
    // yes, replace the clef accordinglingly
    msrClefKind
      newClefKind = (*it).second;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceClefs ()) {
      std::stringstream ss;

      ss <<
        "Replace clef " <<
        msrClefKindAsString (clefKind) <<
        " by " <<
        msrClefKindAsString (newClefKind) <<
        ", line " << elt->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    clefKind = newClefKind;
  }

  // create clef
  S_msrClef
    clef =
      msrClef::create (
        elt->getInputStartLineNumber (),
        clefKind,
        fCurrentClefStaffNumber);

  // register clef in part or staff
/*
  The optional number attribute refers to staff numbers within the part,
	from top to bottom on the system. A value of 1 is assumed if not present.
*/
  if (fCurrentClefStaffNumber == 0) {
    fCurrentPart->
      appendClefToPart (
        fCurrentAttributesInputLineNumber,
        clef);
  }
  else {
    S_msrStaff
      staff =
        fCurrentPartStavesVector [fCurrentClefStaffNumber];

    staff->
      appendClefToStaff (
        fCurrentAttributesInputLineNumber,
        clef);
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_key& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_key" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // The optional number attribute refers to staff numbers.
  // If absent (0), apply to all part staves.
  fCurrentKeyStaffNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentKeyKind = msrKeyKind::kKeyTraditional;

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

  // traditional

  fCurrentKeyFifths       = 0;
  fCurrentKeyCancelFifths = 0;

  fCurrentModeKind = msrModeKind::kMode_UNKNOWN_;

  // Humdrum-Scot

  fCurrentHumdrumScotKeyItem = nullptr;
}

void mxsr2msrSkeletonPopulator::visitStart (S_cancel& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_cancel" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentKeyCancelFifths = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_fifths& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fifths" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentKeyKind = msrKeyKind::kKeyTraditional;

  fCurrentKeyFifths = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_mode& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_mode" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string mode = elt->getValue();

  if       (mode == "none") {
    fCurrentModeKind = msrModeKind::kMode_UNKNOWN_;
  }
  else if  (mode == "major") {
    fCurrentModeKind = msrModeKind::kModeMajor;
  }
  else  if (mode == "minor") {
    fCurrentModeKind = msrModeKind::kModeMinor;
  }
  else  if (mode == "ionian") {
    fCurrentModeKind = msrModeKind::kModeIonian;
  }
  else  if (mode == "dorian") {
    fCurrentModeKind = msrModeKind::kModeDorian;
  }
  else  if (mode == "phrygian") {
    fCurrentModeKind = msrModeKind::kModePhrygian;
  }
  else  if (mode == "lydian") {
    fCurrentModeKind = msrModeKind::kModeLydian;
  }
  else  if (mode == "mixolydian") {
    fCurrentModeKind = msrModeKind::kModeMixolydian;
  }
  else  if (mode == "aeolian") {
    fCurrentModeKind = msrModeKind::kModeAeolian;
  }
  else  if (mode == "locrian") {
    fCurrentModeKind = msrModeKind::kModeLocrian;
  }
  else {
    std::stringstream ss;

    ss <<
      "mode " << mode << " is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_key_step& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_key_step" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentHumdrumScotKeyItem) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "Humdrum/Scot key step found while another one is being handled");
  }

  fCurrentKeyKind = msrKeyKind::kKeyHumdrumScot;

  std::string step = elt->getValue();

  // check the step value
  checkStep (
    elt->getInputStartLineNumber (),
    step,
    "<key-step/>");

  // determine diatonic pitch
  msrDiatonicPitchKind keyDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (step [0]);

  // create the Humdrum/Scot item
  fCurrentHumdrumScotKeyItem =
    msrHumdrumScotKeyItem::create (
      elt->getInputStartLineNumber ());

  // populate it with the diatonic pitch
  fCurrentHumdrumScotKeyItem->
    setKeyItemDiatonicPitchKind (
      keyDiatonicPitchKind);

  // insert it into the items vector
  fCurrentHumdrumScotKeyItemsVector.
    insert (
      fCurrentHumdrumScotKeyItemsVector.end (),
      fCurrentHumdrumScotKeyItem);
}

void mxsr2msrSkeletonPopulator::visitStart (S_key_alter& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_key_alter" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fCurrentHumdrumScotKeyItem) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "Humdrum/Scot key alter found while no key step is being handled");
  }

  float keyAlter = (float)(*elt);

  // determine the alteration
  msrAlterationKind
    keyAlterationKind =
      msrAlterationKindFromMusicXMLAlter (
        keyAlter);

  if (keyAlterationKind == msrAlterationKind::kAlteration_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "key alter " << keyAlter <<
      " should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // complement the current Humdrum/Scot item with the alteration
  fCurrentHumdrumScotKeyItem->
    setKeyItemAlterationKind (
      keyAlterationKind);

  // forget about this item
  fCurrentHumdrumScotKeyItem = nullptr;
}

void mxsr2msrSkeletonPopulator::visitStart (S_key_octave& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_key_octave" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int keyOctaveNumber = (int)(*elt);

  msrOctaveKind
    keyOctaveKind =
      msrOctaveKindFromNumber (
        elt->getInputStartLineNumber (),
        keyOctaveNumber);

  int number = elt->getAttributeIntValue ("number", 0);

/* JMI
If the cancel attribute is
  set to yes, then this number refers to an element specified
  by the cancel element. It is no by default.
*/

  // fetch Humdrum/Scot item with 'number' in the vector
  S_msrHumdrumScotKeyItem item;

  try {
    // indices start at 0
    item = fCurrentHumdrumScotKeyItemsVector [number - 1];
  } // try

  catch ( int e) {
    std::stringstream ss;

    ss <<
      "item " << number <<
      " not found in Humdrum/Scot key items" <<
      std::endl <<
      "An std::exception number " << e << " occurred";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  if (! item) { // JMI
    gLog << "----------------" << std::endl;

    int counter = 0;
    for (
      std::vector <S_msrHumdrumScotKeyItem>::const_iterator i =
        fCurrentHumdrumScotKeyItemsVector.begin ();
      i != fCurrentHumdrumScotKeyItemsVector.end ();
      ++i
  ) {
      gLog <<
        ++counter << ": " << (*i) <<
        std::endl;
    } // for

    gLog << "----------------" << std::endl;

    std::stringstream ss;

    ss <<
      "item " << number <<
      " not found in Humdrum/Scot key items";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // complement the item with the octave
  item->
    setKeyItemOctaveKind (
      keyOctaveKind);
}

void mxsr2msrSkeletonPopulator::visitEnd (S_key& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_key" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle key
  S_msrKey key;

  switch (fCurrentKeyKind) {
    case msrKeyKind::kKeyTraditional:
      key =
        handleTraditionalKey (elt->getInputStartLineNumber ());
      break;

    case msrKeyKind::kKeyHumdrumScot:
      key =
        handleHumdrumScotKey (elt->getInputStartLineNumber ());
      break;
  } // switch

  // register key in part or staff
/*
  The optional number attribute refers to staff numbers,
	from top to bottom on the system. If absent, the key
	signature applies to all staves in the part.
*/
  if (fCurrentKeyStaffNumber == 0)
    fCurrentPart->
      appendKeyToPart (
        fCurrentAttributesInputLineNumber,
        key);

  else {
    S_msrStaff
      staff =
        fCurrentPartStavesVector [fCurrentKeyStaffNumber];

    staff->
      appendKeyToStaff (
        fCurrentAttributesInputLineNumber,
        key);
  }
}

S_msrKey mxsr2msrSkeletonPopulator::handleTraditionalKey (
  int inputLineNumber)
{
  // key fifths number
  msrQuarterTonesPitchKind
    keyTonicPitchKind =
      msrQuarterTonesPitchKind::kQTP_UNKNOWN_;

  switch (fCurrentKeyFifths) {
    case 0:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_C_Natural;
      break;
    case 1:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_G_Natural;
      break;
    case 2:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_D_Natural;
      break;
    case 3:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_A_Natural;
      break;
    case 4:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_E_Natural;
      break;
    case 5:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_B_Natural;
      break;
    case 6:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_F_Sharp;
      break;
    case 7:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_C_Sharp;
      break;

    case 8: // JMI
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_G_Sharp;
      break;
    case 9:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_D_Sharp;
      break;
    case 10:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_A_Sharp;
      break;
    case 11:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_E_Sharp;
      break;

    case -1:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_F_Natural;
      break;
    case -2:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_B_Flat;
      break;
    case -3:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_E_Flat;
      break;
    case -4:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_A_Flat;
      break;
    case -5:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_D_Flat;
      break;
    case -6:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_G_Flat;
      break;
    case -7:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_C_Flat;
      break;

    case -8: // JMI
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_F_Flat;
      break;
    case -9:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
      break;
    case -10:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
      break;
    case -11:
      keyTonicPitchKind = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
      break;

    default: // unknown key sign!!
      {
      std::stringstream ss;

      ss <<
        "unknown key fifths number \"" << fCurrentKeyFifths << "\"";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
      }
  } // switch

  // create the key
  S_msrKey
    key =
      msrKey::createTraditional (
        inputLineNumber,
        keyTonicPitchKind,
        fCurrentModeKind,
        fCurrentKeyCancelFifths);

  // return it
  return key;
}

S_msrKey mxsr2msrSkeletonPopulator::handleHumdrumScotKey (
  int inputLineNumber)
{
 //  msrQuarterTonesPitch fCurrentNoteQuarterTonesPitch; // JMI BOF

  fCurrentNoteQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentNoteDiatonicPitchKind,
      fCurrentNoteAlterationKind);

  // create the key
  S_msrKey
    key =
      msrKey::createHumdrumScot (
        inputLineNumber);

  // populate the key with the Humdrum/Scot items
  if (fCurrentHumdrumScotKeyItemsVector.size ()) {
    for (
      std::vector <S_msrHumdrumScotKeyItem>::const_iterator i =
        fCurrentHumdrumScotKeyItemsVector.begin ();
      i != fCurrentHumdrumScotKeyItemsVector.end ();
      ++i
  ) {
      key->
        appendHumdrumScotKeyItem ((*i));
    } // for

    fCurrentHumdrumScotKeyItemsVector.clear ();
  }

  else
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "Humdrum/Scot key is empty");

  // return it
  return key;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_time& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_time" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTimeStaffNumber =
    elt->getAttributeIntValue ("number", 0);

  std::string timeSignatureSymbol =
    elt->getAttributeValue ("symbol");

  fCurrentTimeSignatureSymbolKind =
    msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone; // default value

  if       (timeSignatureSymbol == "common") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon;
  }
  else  if (timeSignatureSymbol == "cut") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut;
  }
  else  if (timeSignatureSymbol == "note") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote;
  }
  else  if (timeSignatureSymbol == "dotted-note") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote;
  }
  else  if (timeSignatureSymbol == "single-number") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber;
  }

  else {
    if (timeSignatureSymbol.size ()) {
      std::stringstream ss;

      ss <<
        "time symbol " << timeSignatureSymbol << " is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  fCurrentTimeSignatureBeats = "";

  fOnGoingInterchangeable = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_beats& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beats" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTimeSignatureBeats = elt->getValue (); // can be a string such as 3+2
}

void mxsr2msrSkeletonPopulator::visitStart (S_beat_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beat_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int beatType = (int)(*elt);

  // extract the numbers list from the beat type
  std::list <int>
    beatNumbers =
      mfExtractNumbersFromString (
        fCurrentTimeSignatureBeats, // may contain "1+5+3"
        false); // 'true' to debug it

  // create the time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        elt->getInputStartLineNumber ());

  // populate it
  if (beatNumbers.size ()) {
    // append the beats numbers to the time signature item
    for (
      std::list <int>::const_iterator i = beatNumbers.begin ();
      i != beatNumbers.end ();
      ++i
  ) {
      timeSignatureItem->
        appendBeatsNumber ((*i));
    } // for

    // set the time signature item beat type
    timeSignatureItem->
      setTimeSignatureBeatValue (beatType);
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "beat type doesn't contain any beats numbers");
  }

  // append the time signature item to the current time signature items vector
  fCurrentTimeSignatureItemsVector.insert (
    fCurrentTimeSignatureItemsVector.end (),
    timeSignatureItem);
}

void mxsr2msrSkeletonPopulator::visitStart (S_senza_misura& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_senza_misura" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura;
}

void mxsr2msrSkeletonPopulator::visitStart (S_interchangeable& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_interchangeable" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string interchangeableSymbol =
    elt->getAttributeValue ("symbol");

  fCurrentInterchangeableSymbolKind =
    msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone; // default value

  if       (interchangeableSymbol == "common") {
    fCurrentInterchangeableSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon;
  }
  else  if (interchangeableSymbol == "cut") {
    fCurrentInterchangeableSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut;
  }
  else  if (interchangeableSymbol == "note") {
    fCurrentInterchangeableSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote;
  }
  else  if (interchangeableSymbol == "dotted-note") {
    fCurrentInterchangeableSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote;
  }
  else  if (interchangeableSymbol == "single-number") {
    fCurrentInterchangeableSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber;
  }

  else {
    if (interchangeableSymbol.size ()) {
      std::stringstream ss;

      ss <<
        "interchangeable symbol " << interchangeableSymbol << " is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  std::string interchangeableSeparator =
    elt->getAttributeValue ("separator");

  fCurrentInterchangeableSeparatorKind =
    msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorNone; // default value

  if       (interchangeableSymbol == "none") {
    fCurrentInterchangeableSeparatorKind = msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorNone;
  }
  else  if (interchangeableSymbol == "horizontal") {
    fCurrentInterchangeableSeparatorKind = msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorHorizontal;
  }
  else  if (interchangeableSymbol == "diagonal") {
    fCurrentInterchangeableSeparatorKind = msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorDiagonal;
  }
  else  if (interchangeableSymbol == "vertical") {
    fCurrentInterchangeableSeparatorKind = msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorVertical;
  }
  else  if (interchangeableSymbol == "adjacent") {
    fCurrentInterchangeableSeparatorKind = msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorAdjacent;
  }

  else {
    if (interchangeableSymbol.size ()) {
      std::stringstream ss;

      ss <<
        "interchangeable symbol " << interchangeableSymbol << " is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  fOnGoingInterchangeable = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_time_relation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_time_relation" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string timeSignatureRelation = elt->getValue ();

  fCurrentInterchangeableRelationKind =
    msrTimeSignatureRelationKind::kTimeSignatureRelationNone; // default value

  if       (timeSignatureRelation == "parentheses") {
    fCurrentInterchangeableRelationKind =
      msrTimeSignatureRelationKind::kTimeSignatureRelationParentheses;
  }
  else  if (timeSignatureRelation == "bracket") {
    fCurrentInterchangeableRelationKind =
      msrTimeSignatureRelationKind::kTimeSignatureRelationBracket;
  }
  else  if (timeSignatureRelation == "equals") {
    fCurrentInterchangeableRelationKind =
      msrTimeSignatureRelationKind::kTimeSignatureRelationEquals;
  }
  else  if (timeSignatureRelation == "slash") {
    fCurrentInterchangeableRelationKind =
      msrTimeSignatureRelationKind::kTimeSignatureRelationSlash;
  }
  else  if (timeSignatureRelation == "space") {
    fCurrentInterchangeableRelationKind =
      msrTimeSignatureRelationKind::kTimeSignatureRelationSpace;
  }
  else  if (timeSignatureRelation == "hyphen") {
    fCurrentInterchangeableRelationKind =
      msrTimeSignatureRelationKind::kTimeSignatureRelationHyphen;
  }

  else {
    if (timeSignatureRelation.size ()) {
      std::stringstream ss;

      ss <<
        "time-relation " << timeSignatureRelation << " is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_time& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_time" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the time signature
  fCurrentTimeSignature =
    msrTimeSignature::create (
      elt->getInputStartLineNumber (),
      fCurrentTimeSignatureSymbolKind);

  // populate the time signature with the time signature items
  if (fCurrentTimeSignatureItemsVector.size ()) {
    for (S_msrTimeSignatureItem timeSignatureItem : fCurrentTimeSignatureItemsVector) {
      fCurrentTimeSignature->
        appendTimeSignatureItem (timeSignatureItem);
    } // for

    // forget about the current time signature items
    fCurrentTimeSignatureItemsVector.clear ();
  }

  else {
    // only a 'semza misura' time may be empty
    if (  fCurrentTimeSignatureSymbolKind != msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        "time is empty");
    }
  }

  // register time signature in part or staff
/*
  The optional number attribute refers to staff
	numbers within the part, from top to bottom on the system.
	If absent, the time signature applies to all staves in the
	part.
*/
  if (fCurrentTimeStaffNumber == 0)
    fCurrentPart->
      appendTimeSignatureToPart (
        fCurrentAttributesInputLineNumber,
        fCurrentTimeSignature);

  else {
    S_msrStaff
      staff =
        fCurrentPartStavesVector [fCurrentTimeStaffNumber];

    staff->
      appendTimeSignatureToStaff (
        fCurrentAttributesInputLineNumber,
        fCurrentTimeSignature);
  }
}

//______________________________________________________________________________

/*

open xmlsamples3.1/Telemann.xml

<score-instrument id="P1-I1">
    <instrument-name>Voice 1</instrument-name>
   </score-instrument>
*/

void mxsr2msrSkeletonPopulator::visitStart (S_score_instrument& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_score_instrument" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
<!--
  The score-instrument element allows for multiple instruments
  per score-part. As with the score-part element, each
  score-instrument has a required ID attribute, a name,
  and an optional abbreviation. The instrument-name and
  instrument-abbreviation are typically used within a software
  application, rather than appearing on the printed page of a
  score.

  A score-instrument element is also required if the score
  specifies MIDI 1.0 channels, banks, or programs. An initial
  midi-instrument assignment can also be made here. MusicXML
  software should be able to automatically assign reasonable
  channels and instruments without these elements in simple
  cases, such as where part names match General MIDI
  instrument names.

  The score-instrument element can also distinguish multiple
  instruments of the same type that are on the same part,
  such as Clarinet 1 and Clarinet 2 instruments within a
  Clarinets 1 and 2 part.

  The virtual-instrument-data entity is defined in the
  common.mod file, as it can be used within both the
  score-part and instrument-change elements.
-->
<!ELEMENT score-instrument
  (instrument-name, instrument-abbreviation?,
  %virtual-instrument-data;)>
<!ATTLIST score-instrument
    id ID #REQUIRED
>
<!ELEMENT instrument-name (#PCDATA)>
<!ELEMENT instrument-abbreviation (#PCDATA)>

    <score-part id="P1">
      <part-name print-object="no">Voice</part-name>
      <score-instrument id="P1-I6">
        <instrument-name>Voice</instrument-name>
        <instrument-sound>voice.vocals</instrument-sound>
        <solo/>
      </score-instrument>
*/

//  int instruments = (int)(*elt); // JMI
}


void mxsr2msrSkeletonPopulator::visitStart (S_instrument_name& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument_name" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
        <instruments>2</instruments>
*/

//  int instruments = (int)(*elt); // JMI
}

void mxsr2msrSkeletonPopulator::visitStart (S_solo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_solo" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
        <instruments>2</instruments>
*/

//  int instruments = (int)(*elt); // JMI
}

void mxsr2msrSkeletonPopulator::visitStart (S_instruments& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instruments" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
<!--
  Instruments are only used if more than one instrument is
  represented in the part (e.g., oboe I and II where they
  play together most of the time). If absent, a value of 1
  is assumed.
-->
<!ELEMENT instruments (#PCDATA)>

    <measure number="1" width="324">
      <attributes>
        <divisions>768</divisions>
        <key>
          <fifths>0</fifths>
          <mode>major</mode>
        </key>
        <time>
          <beats>4</beats>
          <beat-type>4</beat-type>
        </time>
        <instruments>2</instruments>
        <clef>
          <sign>G</sign>
          <line>2</line>
        </clef>
      </attributes>

      <attributes>
        <divisions>8</divisions>
        <key>
          <fifths>2</fifths>
          <mode>major</mode>
        </key>
        <time>
          <beats>3</beats>
          <beat-type>8</beat-type>
        </time>
        <staves>2</staves>
        <instruments>5</instruments>
*/

//  int instruments = (int)(*elt); // JMI
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_transpose& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_transpose" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTransposeStaffNumber = elt->getAttributeIntValue ("number", 0);

  fCurrentTransposeDiatonic     = 0;
  fCurrentTransposeChromatic    = 0;
  fCurrentTransposeOctaveChange = 0;
  fCurrentTransposeDouble       = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_diatonic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_diatonic" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTransposeDiatonic = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_chromatic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_chromatic" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTransposeChromatic = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_octave_change& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting octave_change" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTransposeOctaveChange = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_double& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting double" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  /*
    If the double element is present,
    it indicates that the music is doubled one octave down
    from what is currently written
    (as is the case for mixed cello / bass parts in orchestral literature).
  */

  fCurrentTransposeDouble = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_transpose& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_transpose" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // bring the transpose chromatic value in the -11..+11 interval
  if (fCurrentTransposeChromatic < -11) {
    int
      auxTransposeChromatic =
        fCurrentTransposeChromatic,
      octaveOffset = 0;

    while (auxTransposeChromatic < -11) {
      auxTransposeChromatic += 12;
      ++octaveOffset;
    } // while

    std::stringstream ss;

    ss <<
      "Transpose: augmenting chromatic " <<
      fCurrentTransposeChromatic <<
      " to " <<
      auxTransposeChromatic <<
      " and decrementing octave change by " <<
      octaveOffset;

    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());

    fCurrentTransposeChromatic    =  auxTransposeChromatic;
    fCurrentTransposeOctaveChange -= octaveOffset;
  }

  else if (fCurrentTransposeChromatic > 11) {
    int
      auxTransposeChromatic =
        fCurrentTransposeChromatic,
      octaveOffset = 0;

    while (auxTransposeChromatic > 11) {
      auxTransposeChromatic -= 12;
      ++octaveOffset;
    } // while

    std::stringstream ss;

    ss <<
      "fTransposition: diminishing  chromatic to " <<
      fCurrentTransposeChromatic <<
      " to " << auxTransposeChromatic <<
      " and incrementing octave change by " << octaveOffset;

    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());

    fCurrentTransposeChromatic    =  auxTransposeChromatic;
    fCurrentTransposeOctaveChange += octaveOffset;
  }

  // create the msrTransposition
  S_msrTransposition
    transposition =
      msrTransposition::create (
        elt->getInputStartLineNumber (),
        fCurrentTransposeDiatonic,
        fCurrentTransposeChromatic,
        fCurrentTransposeOctaveChange,
        fCurrentTransposeDouble);

  if (fCurrentTransposeStaffNumber == 0)
    fCurrentPart->
      appendTranspositionToPart (transposition);

  else {
    S_msrStaff
      staff =
        fCurrentPartStavesVector [fCurrentTransposeStaffNumber];

    staff->
      appendTranspositionToStaff (transposition);
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_direction& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_direction" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  fCurrentDirectionPlacementKind =
    msrPlacementKindFromString (
      elt->getInputStartLineNumber (),
      placementString);

  fCurrentDirectionStaffNumber = 1; // it may be absent
  // no <voice /> possible in <direction /> ??? JMI
  // fCurrentDirectionVoiceNumber = 1; // it may be absent

  fCurrentMetronomeTempo = nullptr;

  fOnGoingDirection = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_direction& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_direction" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentMetronomeTempo) {
    size_t pendingWordsSize = fPendingWordsList.size ();

    if (pendingWordsSize) {
      while (fPendingWordsList.size ()) {
        S_msrWords
          words =
            fPendingWordsList.front();

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceWords ()) {
          std::stringstream ss;

          ss <<
            "Attaching words " <<
            words->asString () <<
            " to metronome tempo " <<
            fCurrentMetronomeTempo->asString ();

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentMetronomeTempo->
          appendWordsToTempo (words);

        // forget about this words
        fPendingWordsList.pop_front();
      } // while

      // append the tempo to the pending tempos list
      fPendingTemposList.push_back (fCurrentMetronomeTempo);

      fCurrentMetronomeTempo = nullptr;
    }
  }

  fOnGoingDirection = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_direction_type& elt)
{
/*
<!ELEMENT direction-type (rehearsalMark+ | segno+ | coda+ |
  (words | symbol)+ | wedge | dynamics+ | dashes |
  bracket | pedal | metronome | octave-shift | harp-pedals |
  damp | damp-all | eyeglasses | string-mute |
  scordatura | image | principal-voice | percussion+ |
  accordion-registration | staff-divide | other-direction)>
<!ATTLIST direction-type
    %optional-unique-id;
>
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_direction_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fOnGoingDirection) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<direction-type /> is out of context");
  }

  fOnGoingDirectionType = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_direction_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_direction_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingDirectionType = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_offset& elt)
{
/*
<!--
	The position attributes are based on MuseData print
	suggestions. For most elements, any program will compute
	a default x and y position. The position attributes let
	this be changed two ways.

	The default-x and default-y attributes change the
	computation of the default position. For most elements,
	the origin is changed relative to the left-hand side of
	the note or the musical position within the bar (x) and
	the top line of the staff (y).

	For the following elements, the default-x value changes
	the origin relative to the start of the current measure:

		- note
		- figured-bass
		- harmony
		- link
		- directive
		- measure-numbering
		- all descendants of the part-list element
		- all children of the direction-type element

	This origin is from the start of the entire measure,
	at either the left barline or the start of the system.

	When the default-x attribute is used within a child element
	of the part-name-display, part-abbreviation-display,
	group-name-display, or group-abbreviation-display elements,
	it changes the origin relative to the start of the first
	measure on the system. These values are used when the current
	measure or a succeeding measure starts a new system. The same
	change of origin is used for the group-symbol element.

	For the note, figured-bass, and harmony elements, the
	default-x value is considered to have adjusted the musical
	position within the bar for its descendant elements.

	Since the credit-words and credit-image elements are not
	related to a measure, in these cases the default-x and
	default-y attributes adjust the origin relative to the
	bottom left-hand corner of the specified page.

	The relative-x and relative-y attributes change the position
	relative to the default position, either as computed by the
	individual program, or as overridden by the default-x and
	default-y attributes.

	Positive x is right, negative x is left; positive y is up,
	negative y is down. All units are in tenths of interline
	space. For stems, positive relative-y lengthens a stem
	while negative relative-y shortens it.

	The default-x and default-y position attributes provide
	higher-resolution positioning data than related features
	such as the placement attribute and the offset element.
	Applications reading a MusicXML file that can understand
	both features should generally rely on the default-x and
	default-y attributes for their greater accuracy. For the
	relative-x and relative-y attributes, the offset element,
	placement attribute, and directive attribute provide
	context for the relative position information, so the two
	features should be interpreted together.

	As elsewhere in the MusicXML format, tenths are the global
	tenths defined by the scaling element, not the local tenths
	of a staff resized by the staff-size element.
-->
<!ENTITY % position
	"default-x     %tenths;    #IMPLIED
	 default-y     %tenths;    #IMPLIED
	 relative-x    %tenths;    #IMPLIED
	 relative-y    %tenths;    #IMPLIED">
>


<!ELEMENT direction (direction-type+, offset?,
	%editorial-voice;, staff?, sound?)>
<!ATTLIST direction
    %placement;
    %directive;
    %optional-unique-id;
>



<!--
	An offset is represented in terms of divisions, and
	indicates where the direction will appear relative to
	the current musical location. This affects the visual
	appearance of the direction. If the sound attribute is
	"yes", then the offset affects playback too. If the sound
	attribute is "no", then any sound associated with the
	direction takes effect at the current location. The sound
	attribute is "no" by default for compatibility with earlier
	versions of the MusicXML format. If an element within a
	direction includes a default-x attribute, the offset value
	will be ignored when determining the appearance of that
	element.
-->
<!ELEMENT offset (#PCDATA)>
<!ATTLIST offset
    sound %yes-no; #IMPLIED
>




<!ELEMENT sound ((midi-device?, midi-instrument?, play?)*,
	offset?)>
<!ATTLIST sound
    tempo CDATA #IMPLIED
    dynamics CDATA #IMPLIED
    dacapo %yes-no; #IMPLIED
    segno CDATA #IMPLIED
    dalsegno CDATA #IMPLIED
    coda CDATA #IMPLIED
    tocoda CDATA #IMPLIED
    divisions CDATA #IMPLIED
    forward-repeat %yes-no; #IMPLIED
    fine CDATA #IMPLIED
    %time-only;
    pizzicato %yes-no; #IMPLIED
    pan CDATA #IMPLIED
    elevation CDATA #IMPLIED
    damper-pedal %yes-no-number; #IMPLIED
    soft-pedal %yes-no-number; #IMPLIED
    sostenuto-pedal %yes-no-number; #IMPLIED
    %optional-unique-id;
>



<!ELEMENT harmony ((%harmony-chord;)+, frame?,
	offset?, %editorial;, staff?)>
<!ATTLIST harmony
    type (explicit | implied | alternate) #IMPLIED
    %print-object;
    print-frame  %yes-no; #IMPLIED
    %print-style;
    %placement;
    %optional-unique-id;
>



      <direction placement="below">
        <direction-type>
          <wedge default-y="-59" spread="0" type="crescendo"/>
        </direction-type>
        <offset>-21</offset>
      </direction>
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_offset" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int offsetValue = (int)(*elt);

  // set offset whole notes
  msrWholeNotes
    offsetWholeNotesFromNotesDuration =
      msrWholeNotes (
        offsetValue,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

  // sound

  std::string offsetSound =elt->getAttributeValue ("sound");

  Bool offsetSoundValue; // JMI

  if (offsetSound.size ()) {
    if (offsetSound == "yes")
      offsetSoundValue = true;

    else if (offsetSound == "no")
      offsetSoundValue = false;

    else {
      std::stringstream ss;

      ss <<
        "offset sound value " << offsetSound <<
        " should be 'yes' or 'no'";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  if (false && offsetSoundValue) {
    // JMI
  }

  if (fOnGoingDirection) { // JMI
  }

  else if (fOnGoingHarmony) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Harmony offset \"" << offsetValue << "\"" <<
        ", represents = \"" <<
       offsetWholeNotesFromNotesDuration <<
       "\" whole notes";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentHarmonyWholeNotesOffset =
      offsetWholeNotesFromNotesDuration;
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_other_direction& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_direction" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_sound& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sound" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingDirection) {
    // tempo
    std::string tempoString = elt->getAttributeValue ("tempo"); // to be seen JMI

    if (tempoString.size ()) {
      fCurrentMetronomeTempo =
        msrTempo::createTempoPerMinute (
          elt->getInputStartLineNumber (),
          msrDottedNotesDuration (
            msrNotesDurationKind::kNotesDurationQuarter,
            0),       // JMI could be different??? v0.9.66
          tempoString,
          msrTempoParenthesizedKind::kTempoParenthesizedNo,
          msrPlacementKind::kPlacementBelow);
    }
  }

  else {
    // measure-level <sound /> markup

    // dynamics
    std::string dynamicsString = elt->getAttributeValue ("dynamics");

    if (dynamicsString.size ()) {
      // JMI v0.9.72 ???
    }
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_sound& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_sound" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_octave_shift& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_octave_shift" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // size

  std::string octaveShiftSizeString = elt->getAttributeValue ("size");

  int octaveShiftSize;

  std::istringstream inputStream (octaveShiftSizeString);

  inputStream >> octaveShiftSize;

//   if (! octaveShiftSizeString.size ()) { // JMI v0.9.67
//     std::stringstream ss;
//
//     ss <<
//       "octave shift size absent, assuming 8";
//
//     mxsr2msrWarning (
//       gServiceRunData->getInputSourceName (),
//       elt->getInputStartLineNumber (),
//       ss.str ());
//   }
//
//   else {
//     std::istringstream inputStream (octaveShiftSizeString);
//
//     inputStream >> octaveShiftSize;
//
//     if (
//       octaveShiftSize != 8
//         &&
//       octaveShiftSize != 15
//       /* JMI allow larger values???
//         &&
//       octaveShiftSize != 22
//         &&
//       octaveShiftSize != 27
//       */
//     ) {
//       std::stringstream ss;
//
//       ss <<
//         "octave-shift size \"" << octaveShiftSize <<
//     // JMI    "\" should be 8, 15, 22 or 27";
//         "\" is wrong, should be 8 or 15, replaced by 8";
//
//   // JMI    mxsr2msrError (
//       mxsr2msrWarning (
//         gServiceRunData->getInputSourceName (),
//         elt->getInputStartLineNumber (),
//    //     __FILE__, __LINE__,
//         ss.str ());
//     }
//
//     octaveShiftSize = 8;
//   }

  // type

  std::string type = elt->getAttributeValue ("type");

  msrOctaveShiftKind
    octaveShiftKind = msrOctaveShiftKind::kOctaveShiftNone;

  if      (type == "up")
    octaveShiftKind = msrOctaveShiftKind::kOctaveShiftUp;
  else if (type == "down")
    octaveShiftKind = msrOctaveShiftKind::kOctaveShiftDown;
  else if (type == "stop")
    octaveShiftKind = msrOctaveShiftKind::kOctaveShiftStop;
  else if (type == "continue")
    octaveShiftKind = msrOctaveShiftKind::kOctaveShiftContinue;

  else {
    std::stringstream ss;

    ss <<
      "octave-shift type \"" << type <<
      "\"" << "is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // create an octave shift
  S_msrOctaveShift
    octaveShift =
      msrOctaveShift::create (
        elt->getInputStartLineNumber (),
        octaveShiftKind,
        octaveShiftSize);

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOctaveShifts ()) {
      std::stringstream ss;

      ss <<
        "Creating octave shift " <<
        octaveShift->asString () <<
        ", line " << elt->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  // append the octave shift to the pending octave shifts list
  fPendingOctaveShiftsList.push_back (octaveShift);
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_words& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_words" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string wordsValue = elt->getValue ();

  if (! wordsValue.size ()) {
 #ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceWords ()) {
      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        "words contents is empty");
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // justify
  std::string wordsJustifyString = elt->getAttributeValue ("justify");

  msrJustifyKind
    justifyKind =
      msrJustifyKindFromString (
        elt->getInputStartLineNumber (),
        wordsJustifyString);

  // halign
  std::string wordsHorizontalAlignmentString = elt->getAttributeValue ("halign");

  msrHorizontalAlignmentKind
    horizontalAlignmentKind =
      msrHorizontalAlignmentKindFromString (
        elt->getInputStartLineNumber (),
        wordsHorizontalAlignmentString);

  // valign
  std::string wordsVerticalAlignmentString = elt->getAttributeValue ("valign");

  msrVerticalAlignmentKind
    verticalAlignmentKind =
      msrVerticalAlignmentKindFromString (
        elt->getInputStartLineNumber (),
        wordsVerticalAlignmentString);

  // font style
  std::string wordsFontStyleString = elt->getAttributeValue ("font-style");

  msrFontStyleKind fontStyleKind = msrFontStyleKind::kFontStyleNone; // default value

  if      (wordsFontStyleString == "normal")
    fontStyleKind = msrFontStyleKind::kFontStyleNormal;
  else if (wordsFontStyleString == "italic")
    fontStyleKind = msrFontStyleKind::KFontStyleItalic;
  else {
    if (wordsFontStyleString.size ()) {
      std::stringstream ss;

      ss <<
        "font-style value " << wordsFontStyleString <<
        " should be 'normal' or 'italic'";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }


/* JMI
  // justify
  std::string wordsFontJustifyString = elt->getAttributeValue ("justify");

  msrJustifyKind fontJustifyKind = msrJustifyKind::kJustifyNone; // default value

  if      (wordsFontJustifyString == "left")
    fontJustifyKind = msrFontStyleKind::kFontStyleNormal;
  else if (wordsFontJustifyString == "center")
    fontJustifyKind = msrFontStyleKind::KFontStyleItalic;
  else if (wordsFontJustifyString == "right")
    fontJustifyKind = msrFontStyleKind::KFontStyleItalic;
  else {
    if (wordsFontJustifyString.size ()) {
      std::stringstream ss;

      ss <<
        "justify value " << wordsFontJustifyString <<
        " should be 'normal' or 'italic'";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }
  */

  // font size
  std::string wordsFontSizeString = elt->getAttributeValue ("font-size");

  msrFontSizeKind
    fontSizeKind =
      msrFontSizeKind::kFontSizeNone; // default value

  float fontSizeFloatValue = 0.0;

  if      (wordsFontSizeString == "xx-smal")
    fontSizeKind = msrFontSizeKind::kFontSizeXXSmall;
  else if (wordsFontSizeString == "x-small")
    fontSizeKind = msrFontSizeKind::kFontSizeXSmall;
  else if (wordsFontSizeString == "small")
    fontSizeKind = msrFontSizeKind::kFontSizeSmall;
  else if (wordsFontSizeString == "medium")
    fontSizeKind = msrFontSizeKind::kFontSizeMedium;
  else if (wordsFontSizeString == "large")
    fontSizeKind = msrFontSizeKind::kFontSizeLarge;
  else if (wordsFontSizeString == "x-large")
    fontSizeKind = msrFontSizeKind::kFontSizeXLarge;
  else if (wordsFontSizeString == "xx-large")
    fontSizeKind = msrFontSizeKind::kFontSizeXXLarge;
  else {
    fontSizeFloatValue =
      elt->getAttributeFloatValue ("font-size", 0.0);
    fontSizeKind = msrFontSizeKind::kFontSizeNumeric;
  }

  S_msrFontSize fontSize;

  switch (fontSizeKind) {
    case msrFontSizeKind::kFontSizeNone:
    case msrFontSizeKind::kFontSizeXXSmall:
    case msrFontSizeKind::kFontSizeXSmall:
    case msrFontSizeKind::kFontSizeSmall:
    case msrFontSizeKind::kFontSizeMedium:
    case msrFontSizeKind::kFontSizeLarge:
    case msrFontSizeKind::kFontSizeXLarge:
    case msrFontSizeKind::kFontSizeXXLarge:
      fontSize =
        msrFontSize::create (fontSizeKind);
      break;

    case msrFontSizeKind::kFontSizeNumeric:
      fontSize =
        msrFontSize::create (fontSizeFloatValue);
      break;
    } // switch

  // font weight
  std::string wordsFontWeightString = elt->getAttributeValue ("font-weight");

  msrFontWeightKind fontWeightKind = msrFontWeightKind::kFontWeightNone; // default value

  if      (wordsFontWeightString == "normal")
    fontWeightKind = msrFontWeightKind::kFontWeightNormal;
  else if (wordsFontWeightString == "bold")
    fontWeightKind = msrFontWeightKind::kFontWeightBold;
  else {
    if (wordsFontWeightString.size ()) {
      std::stringstream ss;

      ss <<
        "font-weight value " << wordsFontWeightString <<
        " should be 'normal' or 'bold'";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // XMLLang
  std::string wordsXMLLangString = elt->getAttributeValue ("xml:lang");

  msrXMLLangKind
    wordsXMLLangKind =
      msrXMLLangKindFromString (
        elt->getInputStartLineNumber (),
        wordsXMLLangString);

  // should the MusicXML words be ignored?
  if (! gGlobalMxsr2msrOahGroup->getIgnoreMusicXMLWords ()) {
    // no

    Bool wordsHasBeenHandled (false);

    // is wordsValue to be converted to an MSR tempo?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToTempo (wordsValue)) {
//       convertWordsToTempo (
//         elt->getInputStartLineNumber (),
//         wordsValue);

      // create an msrWords containing wordsValue
      S_msrWords
        words =
          msrWords::create (
            elt->getInputStartLineNumber (),
            fCurrentDirectionPlacementKind,
            wordsValue,
            justifyKind,
            horizontalAlignmentKind,
            verticalAlignmentKind,
            fontStyleKind,
            fontSize,
            fontWeightKind,
            wordsXMLLangKind,
            fCurrentDirectionStaffNumber);

      // create an msrTempo containing words
      S_msrTempo
        tempo =
          msrTempo::createTempoWordsOnly (
            elt->getInputStartLineNumber (),
            words,
            msrTempoParenthesizedKind::kTempoParenthesizedNo,    // JMI
            msrPlacementKind::kPlacementAbove); // JMI

#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceWords ()
          ||
        gTraceOahGroup->getTraceTempos ()
      ) {
        std::stringstream ss;

        ss <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR tempo" <<
          ", fCurrentDirectionStaffNumber: " <<
          fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fPendingTemposList.push_back (tempo);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a rehearsal mark?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToRehearsalMark (wordsValue)) {
//       convertWordsToRehearsalMark (
//         elt->getInputStartLineNumber (),
//         wordsValue);

      // create an msrWords containing wordsValue
      S_msrRehearsalMark
        rehearsalMark =
          msrRehearsalMark::create (
            elt->getInputStartLineNumber (),
            msrRehearsalMarkKind::kRehearsalMarkNone, // JMI allow for other values???
            wordsValue,
            fCurrentDirectionPlacementKind);

#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceWords ()
          ||
        gTraceOahGroup->getTraceRehearsalMarks ()
      ) {
        std::stringstream ss;

        ss <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR rehearsal mark" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append the rehearsalMark to the pending tempos list
      fPendingRehearsalMarksList.push_back (rehearsalMark);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a segno?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToSegno (wordsValue)) {
//       convertWordsToSegno (
//         elt->getInputStartLineNumber (),
//         wordsValue);
      // create the segno
      S_msrSegno
        segno =
          msrSegno::create (
            elt->getInputStartLineNumber (),
            fCurrentDirectionStaffNumber);

#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceWords ()
          ||
        gTraceOahGroup->getTraceSegnos ()
      ) {
        std::stringstream ss;

        ss <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR segno" <<
          ", fCurrentDirectionStaffNumber: " <<
          fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append dal segno to the pending tempos list
      fPendingSegnosList.push_back (segno);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a dal segno?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToDalSegno (wordsValue)) {
//       convertWordsToDalSegno (
//         elt->getInputStartLineNumber (),
//         wordsValue);
      // create a dal segno element containing wordsValue
      S_msrDalSegno
        dalSegno =
          msrDalSegno::create (
            elt->getInputStartLineNumber (),
            msrDalSegnoKind::kDalSegno,
            wordsValue,
            fCurrentDirectionStaffNumber);

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWords ()) {
        std::stringstream ss;

        ss <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR dal segno " <<
          dalSegno->asString () <<
          ", fCurrentDirectionStaffNumber: " <<
          fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fPendingDalSegnosList.push_back (dalSegno);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a dal segno al fine?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToDalSegnoAlFine (wordsValue)) {
//       convertWordsToDalSegnoAlFine (
//         elt->getInputStartLineNumber (),
//         wordsValue);
      // create a dal segno element containing wordsValue
      S_msrDalSegno
        dalSegno =
          msrDalSegno::create (
            elt->getInputStartLineNumber (),
            msrDalSegnoKind::kDalSegno,
            wordsValue,
            fCurrentDirectionStaffNumber);

#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceWords ()
          ||
        gTraceOahGroup->getTraceDalSegnos ()
      ) {
        std::stringstream ss;

        ss <<
          "Converting words " <<
          wordsValue <<
          " to an MSR dal segno " <<
          dalSegno->asString () <<
          ", fCurrentDirectionStaffNumber: " <<
          fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fPendingDalSegnosList.push_back (dalSegno);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a dal segno al coda?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToDalSegnoAlCoda (wordsValue)) {
//       convertWordsToDalSegnoAlCoda (
//         elt->getInputStartLineNumber (),
//         wordsValue);
      // create a dal segno element containing wordsValue
      S_msrDalSegno
        dalSegno =
          msrDalSegno::create (
            elt->getInputStartLineNumber (),
            msrDalSegnoKind::kDalSegno,
            wordsValue,
            fCurrentDirectionStaffNumber);

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWords ()) {
        std::stringstream ss;

        ss <<
          "Converting words " <<
          wordsValue <<
          " to an MSR dal segno " <<
          dalSegno->asString () <<
          ", fCurrentDirectionStaffNumber: " <<
          fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fPendingDalSegnosList.push_back (dalSegno);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a coda first?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToCodaFirst (wordsValue)) {
//       convertWordsToCoda (
//         elt->getInputStartLineNumber (),
//         wordsValue);
      // create the coda
      S_msrCoda
        coda =
          msrCoda::create (
            elt->getInputStartLineNumber (),
            fCurrentDirectionStaffNumber,
            msrCodaKind::kCodaFirst);

#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceWords ()
          ||
        gTraceOahGroup->getTraceCodas ()
      ) {
        std::stringstream ss;

        ss <<
          "Converting words " <<
          wordsValue <<
          " to an MSR coda first " <<
          coda->asString () <<
          ", fCurrentDirectionStaffNumber: " <<
          fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append it to the pending codas list
      fPendingCodasList.push_back (coda);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a coda second?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToCodaSecond (wordsValue)) {
//       convertWordsToCoda (
//         elt->getInputStartLineNumber (),
//         wordsValue);
      // create the coda
      S_msrCoda
        coda =
          msrCoda::create (
            elt->getInputStartLineNumber (),
            fCurrentDirectionStaffNumber,
            msrCodaKind::kCodaSecond);

#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceWords ()
          ||
        gTraceOahGroup->getTraceCodas ()
      ) {
        std::stringstream ss;

        ss <<
          "Converting words " <<
          wordsValue <<
          " to an MSR coda second " <<
          coda->asString () <<
          ", fCurrentDirectionStaffNumber: " <<
          fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append it to the pending codas list
      fPendingCodasList.push_back (coda);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a cresc?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToCresc (wordsValue)) {
#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceWords ()
          ||
        gTraceOahGroup->getTraceDynamics ()
          ||
        gTraceOahGroup->getTraceCrescDecrescs ()
      ) {
        std::stringstream ss;

        ss <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR cresc" <<
          ", fCurrentDirectionStaffNumber: " <<
          fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

//       convertWordsToCresc (
//         elt->getInputStartLineNumber (),
//         wordsValue);
      // create an msrCrescDecresc
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceCrescDecrescs ()) {
        std::stringstream ss;

        ss <<
          "Creating an msrCrescDecresc crescendo" <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      S_msrCrescDecresc
        crescDecresc =
          msrCrescDecresc::create (
            elt->getInputStartLineNumber (),
            msrCrescDecrescKind::kCrescDecrescCrescendo);

      // append the rehearsalMark to the pending tempos list
      fPendinCrescDecrescsList.push_back (crescDecresc);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a decresc?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToDecresc (wordsValue)) {
#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceWords ()
          ||
        gTraceOahGroup->getTraceDynamics ()
          ||
        gTraceOahGroup->getTraceCrescDecrescs ()
      ) {
        std::stringstream ss;

        ss <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR decresc" <<
          ", fCurrentDirectionStaffNumber: " <<
          fCurrentDirectionStaffNumber <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

//       convertWordsToDecresc (
//         elt->getInputStartLineNumber (),
//         wordsValue);
      // create an msrCrescDecresc
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceCrescDecrescs ()) {
        std::stringstream ss;

        ss <<
          "Creating an msrCrescDecresc decrescendo" <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      S_msrCrescDecresc
        crescDecresc =
          msrCrescDecresc::create (
            elt->getInputStartLineNumber (),
            msrCrescDecrescKind::kCrescDecrescDecrescendo);

      // append the rehearsalMark to the pending tempos list
      fPendinCrescDecrescsList.push_back (crescDecresc);

      wordsHasBeenHandled = true;
    }

    // has wordsValue ben handled?
    if (! wordsHasBeenHandled) {
      // create the words
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWords ()) {
        std::stringstream ss;

        ss <<
          "Creating words \"" << wordsValue << "\"" <<
          ", placement: \"" <<
          msrPlacementKindAsString (
            fCurrentDirectionPlacementKind) <<
          "\"" <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // should we convert wordsValue to bold?
      if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToBold (wordsValue)) {
        fontWeightKind = msrFontWeightKind::kFontWeightBold;
      }

      // should we convert wordsValue to italic?
      if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToItalic (wordsValue)) {
        fontStyleKind = msrFontStyleKind::KFontStyleItalic;
      }

      // should we place wordsValue above the staff?
      if (gGlobalMxsr2msrOahGroup->wordsIsToBePlacedAbove (wordsValue)) {
        fCurrentDirectionPlacementKind = msrPlacementKind::kPlacementAbove;
      }

      // should we place wordsValue below the staff?
      if (gGlobalMxsr2msrOahGroup->wordsIsToBePlacedBelow (wordsValue)) {
        fCurrentDirectionPlacementKind = msrPlacementKind::kPlacementBelow;
      }

      S_msrWords
        words =
          msrWords::create (
            elt->getInputStartLineNumber (),
            fCurrentDirectionPlacementKind,
            wordsValue,
            justifyKind,
            horizontalAlignmentKind,
            verticalAlignmentKind,
            fontStyleKind,
            fontSize,
            fontWeightKind,
            wordsXMLLangKind,
            fCurrentDirectionStaffNumber);

      fPendingWordsList.push_back (words);
    }
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_accordion_registration& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accordion_registration" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
      <direction>
        <direction-type>
          <accordion-registration>
              <accordion-high/>
              <accordion-middle>5</accordion-middle>
              <accordion-low/>
          </accordion-registration>
        </direction-type>
      </direction>
*/

  fCurrentAccordionHigh   = 0;
  fCurrentAccordionMiddle = 0;
  fCurrentAccordionLow    = 0;

  fCurrentAccordionNumbersCounter = 0;
}

void mxsr2msrSkeletonPopulator::visitStart (S_accordion_high& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accordion_high" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentAccordionHigh = 1;

  ++fCurrentAccordionNumbersCounter;
}

void mxsr2msrSkeletonPopulator::visitStart (S_accordion_middle& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accordion_middle" <<
       ", line " << elt->getInputStartLineNumber () <<
     std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentAccordionMiddle = (int)(*elt);

  if (fCurrentAccordionMiddle < 1 || fCurrentAccordionMiddle > 3) {
    std::stringstream ss;

    ss <<
      "accordion middle " <<
      fCurrentAccordionMiddle << " should be 1, 2 or 3" <<
      ", replaced by 1";

    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());

    fCurrentAccordionMiddle = 1;
  }

  ++fCurrentAccordionNumbersCounter;
}

void mxsr2msrSkeletonPopulator::visitStart (S_accordion_low& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accordion_low" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentAccordionLow = 1;

  ++fCurrentAccordionNumbersCounter;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_accordion_registration& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_accordion_registration" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // An accordion-registration element needs to have
  // at least one of the child elements present

  if (fCurrentAccordionNumbersCounter == 0) {
    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      "accordion-registration has 0 child element, ignoring it");
  }

  else {
    // create the accordion registration
    S_msrAccordionRegistration
      accordionRegistration =
        msrAccordionRegistration::create (
          elt->getInputStartLineNumber (),
          fCurrentAccordionHigh,
          fCurrentAccordionMiddle,
          fCurrentAccordionLow);

    // append it to the current part
    fCurrentPart->
      appendAccordionRegistrationToPart (
        accordionRegistration);
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_metronome& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string parentheses = elt->getAttributeValue ("parentheses");

  // JMI fCurrentMetronomeParenthesedKind default value?

  if (parentheses.size ()) {
    if (parentheses == "yes")
      fCurrentMetronomeParenthesedKind =
        msrTempoParenthesizedKind::kTempoParenthesizedYes;
    else if (parentheses == "no")
      fCurrentMetronomeParenthesedKind =
        msrTempoParenthesizedKind::kTempoParenthesizedNo;
    else {
      std::stringstream ss;

      ss <<
        "parentheses value " << parentheses <<
        " should be 'yes' or 'no'";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);


  fCurrentMetronomeBeatUnitsVector.clear();

  fCurrentMetrenomePerMinute = "";

  fCurrentMetrenomeDotsNumber = 0;
  fCurrentMetrenomeRelationKind = msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone;
  fCurrentMetronomeNotesDurationKind = msrNotesDurationKind::kNotesDuration_UNKNOWN_;
  fCurrentMetronomeBeamValue = "";

  fOnGoingMetronomeTuplet = false;
  fCurrentMetrenomeNormalDotsNumber = 0;

  fCurrentMetronomeRelationLeftElements = nullptr;
  fCurrentMetronomeRelationRightElements = nullptr;

  fCurrentMetronomeTuplet = nullptr;

  // don't create fCurrentMetronomeTempo now,
  // this will be done in visitEnd (S_metronome& elt)
  // when the details are known
}

void mxsr2msrSkeletonPopulator::visitStart (S_beat_unit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beat_unit" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string beatUnitString = elt->getValue();

  // the type contains a display duration
  msrNotesDurationKind
    beatUnitNotesDurationKind =
      msrNotesDurationKindFromMusicXMLString (
        elt->getInputStartLineNumber (),
        beatUnitString);

  // there can be several <beat-unit/> in a <metronome/> markup,
  if (fCurrentMetronomeBeatUnitsVector.size () < 2) {
    // register beat unit in in dotted durations list
    fCurrentMetronomeBeatUnitsVector.push_back (
      msrDottedNotesDuration (
        beatUnitNotesDurationKind,
        0)); // dots number
  }
  else {
    std::stringstream ss;

    ss <<
      "there are more than 2 <beat-unit/> elements in a <metronome/> markup";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_beat_unit_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beat_unit_dot" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentMetronomeBeatUnitsVector.size ()) {
    fCurrentMetronomeBeatUnitsVector.back ().incrDotsNumber ();
  }
  else {
    std::stringstream ss;

    ss <<
      "beat unit dot occurs without any prior beat unit";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_per_minute& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_per_minute" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMetrenomePerMinute = elt->getValue ();
}

void mxsr2msrSkeletonPopulator::visitStart (S_metronome_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingMetronomeNote = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_metronome_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string noteType = elt->getValue();

  // the type contains a display duration,
  fCurrentMetronomeNotesDurationKind =
    msrNotesDurationKindFromMusicXMLString (
     elt->getInputStartLineNumber (),
     noteType);
}

void mxsr2msrSkeletonPopulator::visitStart (S_metronome_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_dot" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentMetrenomeDotsNumber;
}

void mxsr2msrSkeletonPopulator::visitStart (S_metronome_beam& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_beam" <<
      ", line " << elt->getInputStartLineNumber () <<
     std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // value

  fCurrentMetronomeBeamValue = elt->getValue();

  msrBeamKind beamKind = msrBeamKind::kBeam_UNKNOWN_;

  if      (fCurrentMetronomeBeamValue == "begin") {
    beamKind = msrBeamKind::kBeamBegin;
  }
  else if (fCurrentMetronomeBeamValue == "continue") {
    beamKind = msrBeamKind::kBeamContinue;
  }
  else if (fCurrentMetronomeBeamValue == "end") {
    beamKind = msrBeamKind::kBeamEnd;
  }
  else if (fCurrentMetronomeBeamValue == "forward hook") {
    beamKind = msrBeamKind::kBeamForwardHook;
  }
  else if (fCurrentMetronomeBeamValue == "backward hook") {
    beamKind = msrBeamKind::kBeamBackwardHook;
  }
  else {
    std::stringstream ss;

    ss <<
      "metronome beam \"" << fCurrentMetronomeBeamValue <<
      "\"" << "is not known";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // number

  fCurrentBeamNumber =
    elt->getAttributeIntValue ("number", 0);

  // create metronome note beam
  S_msrBeam
    beam =
      msrBeam::create (
        elt->getInputStartLineNumber (),
        fCurrentBeamNumber,
        beamKind);

  // register it
  fPendingMetronomeBeamsList.push_back (beam);
}

void mxsr2msrSkeletonPopulator::attachCurrentMetronomeBeamsToMetronomeNote (
  const S_msrTempoNote& tempoNote)
{
  // attach the current articulations to the note
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceTempos ()
      ||
    gTraceOahGroup->getTraceBeams ()
  ) {
    std::stringstream ss;

    ss <<
      "Attaching current beams to tempoNote " <<
      tempoNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingMetronomeBeamsList.size ()) {
    S_msrBeam
      beam =
        fPendingMetronomeBeamsList.front();

#ifdef MF_TRACE_IS_ENABLED
    if (
      gTraceOahGroup->getTraceTempos ()
        ||
      gTraceOahGroup->getTraceBeams ()
    ) {
      std::stringstream ss;

      ss <<
        "Attaching beam " <<
        beam->asString () <<
        " to tempoNote " << tempoNote->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    tempoNote->
      appendBeamToTempoNote (beam);

    // forget about this articulation
    fPendingMetronomeBeamsList.pop_front();
  } // while
}

void mxsr2msrSkeletonPopulator::visitEnd (S_metronome_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_metronome_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // convert metronome note duration into whole notes
  fCurrentMetronomeNoteWholeNotesFromMetronomeType =
    msrNotesDurationKindAsWholeNotes (
      fCurrentMetronomeNotesDurationKind);

  // take metronome dots into account if any
  if (fCurrentNoteDotsNumber > 0) {
    int dots = fCurrentMetrenomeDotsNumber;

    while (dots > 0) {
      fCurrentMetronomeNoteWholeNotesFromMetronomeType *=
        mfRational (3, 2);

      --dots;
    } // while
  }

  // create the metronome note
  S_msrTempoNote
    tempoNote =
      msrTempoNote::create (
        elt->getInputStartLineNumber (),
        fCurrentMetronomeNoteWholeNotesFromMetronomeType,
        false /* tempoNoteBelongsToATuplet JMI */);

  // attach beams if any to metronome note
  if (fPendingMetronomeBeamsList.size ()) {
    attachCurrentMetronomeBeamsToMetronomeNote (
      tempoNote);
  }

  if (fCurrentMetronomeTuplet) {
    // register metronome note as metronome tuplet member

    fCurrentMetronomeTuplet->
      addTempoNoteToTempoTuplet (
        tempoNote);
  }

  else {
    // register stand-alone metronome note

    if (fCurrentMetrenomeRelationKind == msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone) {
      // this metronome note belongs to the left elements list

      if (! fCurrentMetronomeRelationLeftElements) {
        // create the relation left elements
        fCurrentMetronomeRelationLeftElements =
          msrTempoNotesRelationshipElements::create (
            elt->getInputStartLineNumber (),
            msrTempoNotesRelationshipElementsKind::kTempoNotesRelationshipElementsLeft);
      }

      fCurrentMetronomeRelationLeftElements->
        addElementToTempoNotesRelationshipElements (
          tempoNote);
    }
    else {
      // this metronome note belongs to the right elements list

      if (! fCurrentMetronomeRelationRightElements) {
        // create the relation right elements
        fCurrentMetronomeRelationRightElements =
          msrTempoNotesRelationshipElements::create (
            elt->getInputStartLineNumber (),
            msrTempoNotesRelationshipElementsKind::kTempoNotesRelationshipElementsRight);
      }

      fCurrentMetronomeRelationRightElements->
        addElementToTempoNotesRelationshipElements (
          tempoNote);
    }
  }

  fOnGoingMetronomeNote = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_metronome_relation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_relation" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string metrenomeRelation = elt->getValue ();

  fCurrentMetrenomeRelationKind = msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone;

  if (metrenomeRelation == "equals") {
    fCurrentMetrenomeRelationKind = msrTempoNotesRelationshipKind::kTempoNotesRelationshipEquals;
  }
  else {
    std::stringstream ss;

    ss <<
      "metronome relation \"" << metrenomeRelation <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_metronome_tuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_tuplet" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  fCurrentTupletNumber = elt->getAttributeIntValue ("number", 0);

  // bracket

  {
    std::string tupletBracket = elt->getAttributeValue ("bracket");

    fCurrentTempoTupletBracketKind = msrTempoTupletBracketKind::kTempoTupletBracketYes; // option ??? JMI

    if      (tupletBracket == "yes")
      fCurrentTempoTupletBracketKind = msrTempoTupletBracketKind::kTempoTupletBracketYes;
    else if (tupletBracket == "no")
      fCurrentTempoTupletBracketKind = msrTempoTupletBracketKind::kTempoTupletBracketNo;
    else {
      if (tupletBracket.size ()) {
        std::stringstream ss;

        ss <<
          "tuplet bracket \"" << tupletBracket <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
      else {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTuplets ()) {
          std::stringstream ss;

          ss <<
            "tuplet bracket is empty: this is implementation dependent," <<
            " \"yes\" is assumed" <<
            ", line " << elt->getInputStartLineNumber () <<
            std::endl; // option ??? JMI v0.9.68

          mxsr2msrWarning (
            gServiceRunData->getInputSourceName (),
            elt->getInputStartLineNumber (),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }
    }
  }

  // type

  {
    std::string tupletType = elt->getAttributeValue ("type");

    fCurrentTempoTupletTypeKind = msrTempoTupletTypeKind::kTempoTupletTypeNone;

    if      (tupletType == "start")
      fCurrentTempoTupletTypeKind = msrTempoTupletTypeKind::kTempoTupletTypeStart;
    else if (tupletType == "stop")
      fCurrentTempoTupletTypeKind = msrTempoTupletTypeKind::kTempoTupletTypeStop;
    else {
      std::stringstream ss;

      ss <<
        "tuplet type \"" << tupletType <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // show-number

  {
    std::string tupletShowNumber = elt->getAttributeValue ("show-number");

    fCurrentTempoTupletShowNumberKind =
      msrTempoTupletShowNumberKind::kTempoTupletShowNumberActual; // default value

    if      (tupletShowNumber == "actual") {
      fCurrentTempoTupletShowNumberKind = msrTempoTupletShowNumberKind::kTempoTupletShowNumberActual;
    }
    else if (tupletShowNumber == "both") {
      fCurrentTempoTupletShowNumberKind = msrTempoTupletShowNumberKind::kTempoTupletShowNumberBoth;
    }
    else if (tupletShowNumber == "none") {
      fCurrentTempoTupletShowNumberKind = msrTempoTupletShowNumberKind::kTempoTupletShowNumberNone;
    }
    else {
      if (tupletShowNumber.size ()) {
        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          "metronome tuplet show-number \"" + tupletShowNumber + "\" is unknown");
      }
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "fCurrentTempoTupletTypeKind: " <<
      fCurrentTempoTupletTypeKind <<
      "fCurrentTempoTupletBracketKind: " <<
      fCurrentTempoTupletBracketKind <<
      "fCurrentTempoTupletShowNumberKind: " <<
      fCurrentTempoTupletShowNumberKind;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingMetronomeTuplet = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_normal_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_normal_dot" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentMetrenomeNormalDotsNumber;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_metronome_tuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_metronome_tuplet" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fCurrentTempoTupletTypeKind) {
    case msrTempoTupletTypeKind::kTempoTupletTypeNone:
      break;

    case msrTempoTupletTypeKind::kTempoTupletTypeStart:
      {
        // create metronome tuplet
        fCurrentMetronomeTuplet =
          msrTempoTuplet::create (
            elt->getInputStartLineNumber (),
            fCurrentTempoTupletNumber,
            fCurrentTempoTupletBracketKind,
            fCurrentTempoTupletShowNumberKind,
            msrTupletFactor (
              fCurrentMetronomeNoteActualNotes,
              fCurrentMetronomeNoteNormalNotes),
            fCurrentMetronomeNoteWholeNotesFromMetronomeType);

        // register the metronome tuplet
        if (fCurrentMetrenomeRelationKind == msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone) {
          // this metronome tuplet belongs to the left elements list

          if (! fCurrentMetronomeRelationLeftElements) {
            // create the relation left elements
            fCurrentMetronomeRelationLeftElements =
              msrTempoNotesRelationshipElements::create (
                elt->getInputStartLineNumber (),
                msrTempoNotesRelationshipElementsKind::kTempoNotesRelationshipElementsLeft);
          }

          fCurrentMetronomeRelationLeftElements->
            addElementToTempoNotesRelationshipElements (
              fCurrentMetronomeTuplet);
        }
        else {
          // this metronome tuplet belongs to the right elements list

          if (! fCurrentMetronomeRelationRightElements) {
            // create the relation right elements
            fCurrentMetronomeRelationRightElements =
              msrTempoNotesRelationshipElements::create (
                elt->getInputStartLineNumber (),
                msrTempoNotesRelationshipElementsKind::kTempoNotesRelationshipElementsRight);
          }

          fCurrentMetronomeRelationRightElements->
            addElementToTempoNotesRelationshipElements (
              fCurrentMetronomeTuplet);
        }
      }
      break;

    case msrTempoTupletTypeKind::kTempoTupletTypeStop:
      // don't set fCurrentMetronomeTuplet to nullptr here,
      // it will be needed for the current metronome note a bit later
      break;
  } // switch

  fOnGoingMetronomeTuplet = false;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_metronome& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_metronome" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fCurrentMetronomeParenthesedKind ??? JMI

  // determine tempo kind
  msrTempoKBeatUnitsKind tempoKind = msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_;

  int  beatUnitsSize    = fCurrentMetronomeBeatUnitsVector.size ();
  Bool perMinutePresent = fCurrentMetrenomePerMinute.size () > 0;

  if (beatUnitsSize == 1 && perMinutePresent) {
    tempoKind = msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute;
  }
  else if (beatUnitsSize == 2 && ! perMinutePresent) {
    tempoKind = msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence;
  }
  else if (
    fCurrentMetrenomeRelationKind != msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone
  ) {
    tempoKind = msrTempoKBeatUnitsKind::kTempoNotesRelationship;
  }

  if (tempoKind == msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_) {
      std::stringstream ss;

      ss <<
        "ill-formed <metronome/> markup: there are " <<
        fCurrentMetronomeBeatUnitsVector.size () <<
        " beat units";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // create the tempo
  switch (tempoKind) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      {
        msrDottedNotesDuration
          beatUnits =
            fCurrentMetronomeBeatUnitsVector [0];

        fCurrentMetronomeTempo =
          msrTempo::createTempoPerMinute (
            elt->getInputStartLineNumber (),
            beatUnits,
            fCurrentMetrenomePerMinute,
            fCurrentMetronomeParenthesedKind,
            fCurrentDirectionPlacementKind);
        }
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      {
        msrDottedNotesDuration
          beatUnits =
            fCurrentMetronomeBeatUnitsVector [0];

        fCurrentMetronomeTempo =
          msrTempo::createTempoBeatUnitEquivalent (
            elt->getInputStartLineNumber (),
            beatUnits,
            fCurrentMetronomeBeatUnitsVector [1],
            fCurrentMetronomeParenthesedKind,
            fCurrentDirectionPlacementKind);
      }
      break;

    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      fCurrentMetronomeTempo =
        msrTempo::createTempoNotesRelationship (
          elt->getInputStartLineNumber (),
          fCurrentMetronomeRelationLeftElements,
          fCurrentMetrenomeRelationKind, // msrTempoNotesRelationshipKind::kTempoNotesRelationshipEquals here
          fCurrentMetronomeRelationRightElements,
          fCurrentMetronomeParenthesedKind,
          fCurrentDirectionPlacementKind);
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Creating tempo " <<
      fCurrentMetronomeTempo->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append metrenome words to tempo if any
  S_msrWords tempoWords;

  size_t pendingWordsSize = fPendingWordsList.size ();

  if (pendingWordsSize) {
    if (pendingWordsSize > 1) {
      while (fPendingWordsList.size ()) {
        S_msrWords
          words =
            fPendingWordsList.front();

#ifdef MF_TRACE_IS_ENABLED
        if (
          gTraceOahGroup->getTraceTempos ()
            ||
          gTraceOahGroup->getTraceWords ()
        ) {
          std::stringstream ss;

          ss <<
            "Attaching words " <<
            words->asString () <<
            " to tempo " <<
            fCurrentMetronomeTempo->asString ();

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentMetronomeTempo->
          appendWordsToTempo (words);

        // forget about this words
        fPendingWordsList.pop_front();
      } // while


#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWords ()) {
        std::stringstream ss;

        ss <<
          "<direction/> contains " <<
          pendingWordsSize <<
          " <words/> markups";

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }

    while (fPendingWordsList.size ()) {
      S_msrWords
        words =
          fPendingWordsList.front ();

      // append the words to the temp
      fCurrentMetronomeTempo->
        appendWordsToTempo (words);

      // remove it from the list
      fPendingWordsList.pop_front ();
    } // while
  }

  // append the tempo to the pending tempos list
  fPendingTemposList.push_back (fCurrentMetronomeTempo);
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_staves& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_direction" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_staff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // staff number
  fCurrentNoteStaffNumber = int(*elt);

  // the staff number should be positive
  if (fCurrentNoteStaffNumber <= 0) {
    std::stringstream ss;

    ss <<
      "staff number " << fCurrentNoteStaffNumber <<
      " is not positive" <<
      ", line " << elt->getInputStartLineNumber ();

    mfAssert(
      __FILE__, __LINE__,
      false,
      ss.str ());
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    S_msrStaff
      staff = // fCurrentStaff ??? JMI v0.9.67
        fCurrentPartStavesVector [fCurrentNoteStaffNumber];

    std::stringstream ss;

    ss <<
      "--> S_staff, fCurrentNoteStaffNumber: " <<
      fCurrentNoteStaffNumber <<
      std::endl <<
      "--> S_staff, current staff name: " <<
      staff->getStaffName();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the current staff number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentStaveNumber ( // JMI v0.9.72 ???
      fCurrentNoteStaffNumber);

  // set concepts-specific staff numbers
  if (fOnGoingForward) {
    fCurrentForwardStaffNumber = fCurrentNoteStaffNumber;
  }

  else if (fOnGoingNote) {
    // regular staff indication in note/rest, fine
  }

  else if (fOnGoingDirection) {
    fCurrentDirectionStaffNumber = fCurrentNoteStaffNumber;
  }

  else if (fOnGoingHarmony) {
    fCurrentHarmoniesStaffNumber = fCurrentNoteStaffNumber;
  }

  else {
    std::stringstream ss;

    ss <<
      "<staff number /> " <<
      fCurrentNoteStaffNumber <<
      " is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_staff_details& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_details" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  fStaffDetailsStaffNumber =
    elt->getAttributeIntValue ("number", 1); // default value is 1, as for clef

  // show-frets

  {
    std::string showFrets = elt->getAttributeValue ("show-frets");

    fCurrentShowFretsKind =
      msrShowFretsKind::kShowFretsNumbers; // default value

    if      (showFrets == "numbers") {

      fCurrentShowFretsKind =
        msrShowFretsKind::kShowFretsNumbers;

    }
    else if (showFrets == "letters") {

      fCurrentShowFretsKind =
        msrShowFretsKind::kShowFretsLetters;

    }
    else {
      if (showFrets.size ()) {
        std::stringstream ss;

        ss << "show-frets " << showFrets << " unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

  // print-spacing

  std::string
    printSpacing =
      elt->getAttributeValue ("print-spacing");

  fCurrentPrintSpacingKind =
    msrPrintSpacingKind::kPrintSpacingNo; // default value ??? JMI

  if      (printSpacing == "yes") {
    fCurrentPrintSpacingKind =
      msrPrintSpacingKind::kPrintSpacingYes;
  }
  else if (printSpacing == "no") {
    fCurrentPrintSpacingKind =
      msrPrintSpacingKind::kPrintSpacingNo;
  }
  else {
    if (printSpacing.size ()) {
      std::stringstream ss;

      ss << "print-spacing " << printSpacing << " is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Handling staff details:" <<
      std::endl <<
      gTab << "StaffDetailsStaffNumber" << ": " <<
      fStaffDetailsStaffNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffTypeKind =
    msrStaffTypeKind::kStaffTypeRegular;

  fCurrentStaffTuningAlterationKind = msrAlterationKind::kAlteration_UNKNOWN_;
  fCurrentStaffTuningOctaveKind     = msrOctaveKind::kOctave_UNKNOWN_;

  fCurrentStaffDetailsStaffSize = 0;

  fCurrentStaffLinesNumber = 5; // default value

  fCurrentStaffDetailsCapo = 0;

  // create the staff details
  fCurrentStaffDetails =
    msrStaffDetails::create (
      elt->getInputStartLineNumber (),
      fCurrentStaffTypeKind,
      fCurrentShowFretsKind,
      fCurrentPrintObjectKind,
      fCurrentPrintSpacingKind);

  ++gIndenter; // will be decremented in visitEnd (S_staff_details& elt)
}

void mxsr2msrSkeletonPopulator::visitStart (S_staff_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string staffType = elt->getValue ();

  if      (staffType == "ossia") {

    fCurrentStaffTypeKind =
      msrStaffTypeKind::kStaffTypeOssia;

  }
  else if (staffType == "cue") {

    fCurrentStaffTypeKind =
      msrStaffTypeKind::kStaffTypeCue;

  }
  else if (staffType == "editorial") {

    fCurrentStaffTypeKind =
      msrStaffTypeKind::kStaffTypeEditorial;

  }
  else if (staffType == "regular") {

    fCurrentStaffTypeKind =
      msrStaffTypeKind::kStaffTypeRegular;

  }
  else if (staffType == "alternate") {

    fCurrentStaffTypeKind =
      msrStaffTypeKind::kStaffTypeAlternate;

  }
  else {
    std::stringstream ss;

    ss << "staff-type" << staffType << "unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_staff_lines& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_lines" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffLinesNumber = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_staff_tuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_tuning" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffTuningLine =
    elt->getAttributeIntValue ("line", 0);

  fCurrentStaffTuningAlterationKind = msrAlterationKind::kAlterationNatural; // may be absent
  fCurrentStaffTuningOctaveKind     = msrOctaveKind::kOctave_UNKNOWN_;

  fOnGoingStaffTuning = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_tuning_step& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuning_step" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string tuningStep = elt->getValue();

  checkStep (
    elt->getInputStartLineNumber (),
    tuningStep,
    "<tuning-step/>");

  msrDiatonicPitchKind
    tuningDiatonicKind =
      msrDiatonicPitchKindFromChar (
        tuningStep [0]);

  if (fOnGoingStaffTuning) {
    fCurrentStaffTuningDiatonicPitchKind = tuningDiatonicKind;
  }
  else if (fOnGoingAccord) {
    fCurrentAccordDiatonicPitchKind = tuningDiatonicKind;
  }
  else {
    std::stringstream ss;

    ss <<
      "<tuning-step /> " <<
      tuningStep <<
      " is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_tuning_octave& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuning_octave" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int tuningOctaveNumber = (int)(*elt);

  msrOctaveKind
    tuningOctaveKind =
      msrOctaveKindFromNumber (
        elt->getInputStartLineNumber (),
        tuningOctaveNumber);

  if (fOnGoingStaffTuning) {
    fCurrentStaffTuningOctaveKind = tuningOctaveKind;
  }
  else if (fOnGoingAccord) {
    fCurrentAccordOctaveKind = tuningOctaveKind;
  }
  else {
    std::stringstream ss;

    ss <<
      "<tuning-octave /> " <<
      tuningOctaveNumber <<
      " is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_tuning_alter& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuning_alter" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  float tuningAlter = (float)(*elt);

  msrAlterationKind
    tuningAlterationKind =
      msrAlterationKindFromMusicXMLAlter (
        tuningAlter);

  if (tuningAlterationKind == msrAlterationKind::kAlteration_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "tuning alter " << tuningAlter <<
      " should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  if (fOnGoingStaffTuning) {
    fCurrentStaffTuningAlterationKind = tuningAlterationKind;
  }
  else if (fOnGoingAccord) {
    fCurrentAccordAlterationKind = tuningAlterationKind;
  }
  else {
    std::stringstream ss;

    ss <<
      "<tuning-alter /> " <<
      tuningAlter <<
      " is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_staff_tuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_staff_tuning" <<
       ", line " << elt->getInputStartLineNumber () <<
     std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch relevant staff
  S_msrStaff
    staff =
      fCurrentPartStavesVector [fStaffDetailsStaffNumber];

  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromDiatonicPitchAndAlteration (
        elt->getInputStartLineNumber (),
        fCurrentStaffTuningDiatonicPitchKind,
        fCurrentStaffTuningAlterationKind);

  // create the staff tuning
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesDetails ()) {
    std::stringstream ss;

    ss <<
      "Creating staff tuning:" <<
      std::endl;

    ++gIndenter;

    constexpr int fieldWidth = 32;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentStaffTuningLine" << ": " <<
      fCurrentStaffTuningLine <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStaffTuningDiatonicPitch" << ": " <<
      msrDiatonicPitchKindAsStringInLanguage (
        gMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind (),
        fCurrentStaffTuningDiatonicPitchKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStaffTuningAlteration" << ": " <<
      msrAlterationKindAsString (
        fCurrentStaffTuningAlterationKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "quarterTonesPitch" << ": " <<
      msrQuarterTonesPitchKindAsStringInLanguage (
        quarterTonesPitchKind,
        gMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind ()) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "currentStaffTuningOctaveKind" << ": " <<
      msrOctaveKindAsString (fCurrentStaffTuningOctaveKind) <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // create the staff tuning
  S_msrStaffTuning
    staffTuning =
      msrStaffTuning::create (
        elt->getInputStartLineNumber (),
        fCurrentStaffTuningLine,
        quarterTonesPitchKind,
        fCurrentStaffTuningOctaveKind);

  // add it to the current staff details
  fCurrentStaffDetails->
    addStaffTuningToStaffDetails (
      staffTuning);

  fOnGoingStaffTuning = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_voice& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_details" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "<!--=== voiceName \"" << "elt->getVoiceName ()" << "\"" <<
      ", line " << elt->getInputStartLineNumber () << " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the voice number can be out of 1..4 range
  fCurrentNoteVoiceNumber = int(*elt);

  // register the current voice number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentVoiceNumber (
      fCurrentNoteVoiceNumber);

  // set concepts-specific voice numbers

  if (fOnGoingForward) {
    fCurrentForwardVoiceNumber = fCurrentNoteVoiceNumber;
  }

  else if (fOnGoingNote) {
    // regular voice indication in note/rest, fine
  }

  else if (fOnGoingDirection) {
    fCurrentDirectionVoiceNumber = fCurrentNoteVoiceNumber;
  }

  else {
    std::stringstream ss;

    ss <<
      "<voice /> " <<
      mfVoiceNumberAsString (fCurrentNoteVoiceNumber) <<
      " is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_backup& elt)
{
/*
<!--
  The backup and forward elements are required to coordinate
  multiple voices in one part, including music on multiple
  staves. The forward element is generally used within voices
  and staves, while the backup element is generally used to
  move between voices and staves. Thus the backup element
  does not include voice or staff elements. NotesDuration values
  should always be positive, and should not cross measure
  boundaries or mid-measure changes in the divisions value.
-->
<!ELEMENT backup (duration, %editorial;)>
<!ELEMENT forward
  (duration, %editorial-voice;, staff?)>

      <backup>
        <duration>8</duration>
      </backup>
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_backup" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingBackup = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_backup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_backup" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceBackup ()) {
    std::stringstream ss;

    ss <<
      "Backup by " <<
      mfSingularOrPlural (
        fCurrentBackupDivisions, "division", "divisions") <<
        ", fCurrentDivisionsPerQuarterNote: " <<
        fCurrentDivisionsPerQuarterNote <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // compute the backup step length
  msrWholeNotes
    backupStepLength =
      msrWholeNotes (
        fCurrentBackupDivisions,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

  // update current measure position
  fCurrentPart->
    decrementPartCurrentDrawingMeasurePosition (
      elt->getInputStartLineNumber (),
      backupStepLength);

//   // reset notes staff numbers // JMI v0.9.68
//   fPreviousNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
//   fCurrentNoteStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // handle the backup right now:
//   handleBackup (
//     elt->getInputStartLineNumber ());

  // staff changes handling
  fCurrentRecipientStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // chords handling
  if (fOnGoingChord) {
//     finalizeCurrentChord (
//       elt->getInputStartLineNumber ());

    fOnGoingChord = false;
  }

  // tuplets handling
  S_mxsrVoice
    theMxsrVoice =
      fCurrentPartStaffMxsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber];

  if (theMxsrVoice && theMxsrVoice->getTupletsStackSize ()) {
    handleTupletEndEventsIfAnyAfterNoteHandling ();
  }

  fOnGoingBackup = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_forward& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_forward" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the <staff /> and <voice /> are optional,
  // in which case the same staff/voice number remains in effect
  fCurrentForwardStaffNumber = 1; // default value fCurrentNoteStaffNumber; v0.9.68
  fCurrentForwardVoiceNumber = 1; // default value fCurrentNoteVoiceNumber; v0.9.68

  fOnGoingForward = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_forward& elt)
{
/*
<!--
  The backup and forward elements are required to coordinate
  multiple voices in one part, including music on multiple
  staves. The forward element is generally used within voices
  and staves, while the backup element is generally used to
  move between voices and staves. Thus the backup element
  does not include voice or staff elements. NotesDuration values
  should always be positive, and should not cross measure
  boundaries or mid-measure changes in the divisions value.
-->
<!ELEMENT backup (duration, %editorial;)>
<!ELEMENT forward
  (duration, %editorial-voice;, staff?)>

      <forward>
        <duration>4</duration>
        <voice>2</voice>
        <staff>1</staff>
      </forward>
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_forward" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the <staff /> element is present only in case of a staff change ??? JNI v0.9.67

  // compute the backup step length
  msrWholeNotes
    forwardStepLength =
      msrWholeNotes (
        fCurrentForwardDivisions,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceForward ()) {
    std::stringstream ss;

    ss <<
      "Handling forward" <<
      ", fCurrentForwardDivisions: " <<
      fCurrentForwardDivisions <<
      ", fCurrentDivisionsPerQuarterNote: " <<
      fCurrentDivisionsPerQuarterNote <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", fCurrentForwardStaffNumber: " <<
      fCurrentForwardStaffNumber <<
      ", fCurrentForwardVoiceNumber: " <<
      mfVoiceNumberAsString (fCurrentForwardVoiceNumber) <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // account for forwardStepLength wholeNotes in the part current measure position
  fCurrentPart->
    incrementPartCurrentDrawingMeasurePosition (
      elt->getInputStartLineNumber (),
      forwardStepLength);

  // the staff number if any should be positive
  if (fCurrentForwardStaffNumber == K_STAFF_NUMBER_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "staff number " << fCurrentForwardStaffNumber <<
      " is not positive" <<
      ", line " << elt->getInputStartLineNumber ();

      mfAssert (
        __FILE__, __LINE__,
        false,
        ss.str ());
  }

  // the voice number if any should be positive
  if (fCurrentForwardVoiceNumber == K_VOICE_NUMBER_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "voice number " <<
      fCurrentForwardVoiceNumber <<
      " is not positive" <<
      ", line " << elt->getInputStartLineNumber ();

      mfAssert (
        __FILE__, __LINE__,
        false,
        ss.str ());
  }

  // fetch the voice to be forwarded to
  S_msrVoice
    voiceToBeForwardedTo =
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentForwardVoiceNumber];

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceToBeForwardedTo != nullptr,
    "voiceToBeForwardedTo is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // compute the forward step length
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceForward ()) {
    std::stringstream ss;

    ss <<
      "--> current forward divisions: " <<
      fCurrentForwardDivisions;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append a padding note to the voice to be forwarded
  voiceToBeForwardedTo ->
    appendPaddingNoteToVoice (
      elt->getInputStartLineNumber (),
      forwardStepLength);

  // staff changes handling
  fCurrentRecipientStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // chords handling
  if (fOnGoingChord) {
//     finalizeCurrentChord (
//       elt->getInputStartLineNumber ());

    fOnGoingChord = false;
  }

  // tuplets handling
  S_mxsrVoice
    theMxsrVoice =
      fCurrentPartStaffMxsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber];

  if (theMxsrVoice && theMxsrVoice->getTupletsStackSize ()) {
    handleTupletEndEventsIfAnyAfterNoteHandling ();
  }

  fOnGoingForward = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_tied& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tied" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// <tied orientation="over" type="start"/>

  // type

  std::string tiedType =
    elt->getAttributeValue ("type");

  fCurrentTiedOrientation =
    elt->getAttributeValue ("orientation");

  fCurrentTieKind = msrTieKind::kTieNone;

  if      (tiedType == "start") {
    fCurrentTieKind = msrTieKind::kTieStart;
  }
  else if (tiedType == "continue") {
    fCurrentTieKind = msrTieKind::kTieContinue;
  }
  else if (tiedType == "stop") {
    fCurrentTieKind = msrTieKind::kTieStop;
  }
  else {
    // inner tied notes may miss the "continue" type:
    // let' complain on slur notes outside of slurs
    if (! fOnGoingSlur) {
      if (tiedType.size ()) {
        std::stringstream ss;

        ss << "tied type \"" << fCurrentSlurType << "\" inside a slur is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }

    // inner tied notes may miss the "continue" type:
    // let' complain on ligature notes outside of ligatures
    if (! fOnGoingLigature) {
      if (tiedType.size ()) {
        std::stringstream ss;

        ss << "tied type \"" << fCurrentSlurType << "\" inside a ligature is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }

  }

  // color JMI ??? v0.9.70

  // should the tie kind be ignored?
  switch (fCurrentTieKind) {
    case msrTieKind::kTieNone:
      break;

    case msrTieKind::kTieStart:
    case msrTieKind::kTieContinue:
    case msrTieKind::kTieStop:
      if (! gGlobalMxsr2msrOahGroup->getIgnoreTies ()) {
        S_msrTie
          tie =
            msrTie::create (
              elt->getInputStartLineNumber (),
              fCurrentTieKind);

        fPendingTiesList.push_back (tie);
      }
      break;
  } // switch

  // is the current note a grace note?
  if (fCurrentNoteIsAGraceNote) {
    fCurrentGraceNotesGroupIsTied = true;
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::displaySlurStartsStack (
  const std::string& context)
{
  gLog <<
    std::endl <<
    ">>++++++++++++++++ " <<
    "The slurs starts stack contains:" <<
    std::endl;

  if (fSlurStartsStack.size ()) {
    std::list <S_msrSlur>::const_iterator
      iBegin = fSlurStartsStack.begin (),
      iEnd   = fSlurStartsStack.end (),
      i      = iBegin;

    ++gIndenter;

    for ( ; ; ) {
      gLog << "v " << (*i);
      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }

  gLog <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

// //________________________________________________________________________
// Bool mxsr2msrSkeletonPopulator::tupletsStopNumberIsExpected (
//   int tupletNumber)
// {
//   Bool result;
//
//   size_t tupletsStackSize = fTupletsStack.size ();
//
//   if (tupletsStackSize) {
//     for (S_msrTuplet tuplet : fTupletsStack) {
//       if (tuplet->getTupletNumber () == tupletNumber) {
//         result = true;
//         break;
//       }
//     } // for
//   }
//
//   return result;
// }

// //________________________________________________________________________
// void mxsr2msrSkeletonPopulator::displayTupletsStack (
//   const std::string& context)
// {
//   size_t tupletsStackSize = fTupletsStack.size ();
//
//   gLog <<
//     std::endl <<
//     ">>++++++++++++++ The tuplets stack contains " <<
//     mfSingularOrPlural (
//       tupletsStackSize, "element", "elements") <<
//     ':' <<
//     std::endl;
//
//   if (tupletsStackSize) {
//     std::list <S_msrTuplet>::const_iterator
//       iBegin = fTupletsStack.begin (),
//       iEnd   = fTupletsStack.end (),
//       i      = iBegin;
//
//     S_msrTuplet tuplet = (*i);
//
//     ++gIndenter;
//
//     int n = tupletsStackSize;
//     for ( ; ; ) {
//       gLog <<
//         "v (" << n << ")" <<
//         std::endl;
//
//       ++gIndenter;
//       gLog << tuplet;
//       --gIndenter;
//
//       --n;
//
//       if (++i == iEnd) break;
//
//       gLog << std::endl;
//     } // for
//
//     --gIndenter;
//   }
//
//   gLog <<
//     " <<++++++++++++++++ " <<
//     std::endl << std::endl;
// }

// //________________________________________________________________________
// void mxsr2msrSkeletonPopulator::displayMxsrTupletsVector (
//   const std::string& context)
// {
//   size_t mxsrVoicesVectorSize = fMxsrTupletsVector.size ();
//
//   gLog <<
//     std::endl <<
//     ">>++++++++++++++ The tuplets vector contains " <<
//     mfSingularOrPlural (
//       mxsrVoicesVectorSize, "element", "elements") <<
//     ':' <<
//     std::endl;
//
//   if (mxsrVoicesVectorSize) {
//     std::vector <S_msrTuplet>::const_iterator
//       iBegin = fMxsrTupletsVector.begin (),
//       iEnd   = fMxsrTupletsVectorfMxsrTupletsVector.end (),
//       i      = iBegin;
//
//     S_msrTuplet tuplet = (*i);
//
//     ++gIndenter;
//
//     int n = mxsrVoicesVectorSize;
//     for ( ; ; ) {
//       gLog <<
//         "v (" << n << ")" <<
//         std::endl;
//
//       ++gIndenter;
//       gLog << tuplet;
//       --gIndenter;
//
//       --n;
//
//       if (++i == iEnd) break;
//
//       gLog << std::endl;
//     } // for
//
//     --gIndenter;
//   }
//
//   gLog <<
//     " <<++++++++++++++++ " <<
//     std::endl << std::endl;
// }

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_slur& elt)
{
/*
The values of start, stop, and continue refer to how an
	element appears in musical score order, not in MusicXML
	document order. An element with a stop attribute may
	precede the corresponding element with a start attribute
	within a MusicXML document. This is particularly common
	in multi-staff music. For example, the stopping point for
	a slur may appear in staff 1 before the starting point for
	the slur appears in staff 2 later in the document.

	Slurs, tuplets, and many other features can be
	concurrent and overlapping within a single musical
	part. The number-level attribute distinguishes up to
	six concurrent objects of the same type. A reading
	program should be prepared to handle cases where
	the number-levels stop in an arbitrary order.
	Different numbers are needed when the features
	overlap in MusicXML document order. When a number-level
	value is implied, the value is 1 by default.

	Normal slurs, S-shaped slurs, and ties need only two
	bezier points: one associated with the start of the slur
	or tie, the other with the stop. Complex slurs and slurs
	divided over system breaks can specify additional
	bezier data at slur elements with a continue type.

<!--
	Slur elements are empty. Most slurs are represented with
	two elements: one with a start type, and one with a stop
	type. Slurs can add more elements using a continue type.
	This is typically used to specify the formatting of cross-
	system slurs, or to specify the shape of very complex slurs.
-->
<!ELEMENT slur EMPTY>
<!ATTLIST slur
    type %start-stop-continue; #REQUIRED
    number %number-level; "1"
    %line-type;
    %dashed-formatting;
    %position;
    %placement;
    %orientation;
    %bezier;
    %color;
    %optional-unique-id;
>
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slur" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalMxsr2msrOahGroup->getIgnoreSlurs ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlursDetails ()) {
      displaySlurStartsStack ("BEFORE handling slur");
    }
#endif // MF_TRACE_IS_ENABLED

    /*
      Only the  first note of the chord should get the slur notation. JMI ??? v0.9.70
      Some applications print out the slur for all notes,
      i.e. a stop and a start in sequqnce:
      these should be ignored
    */
    if (fCurrentNoteBelongsToAChord) {
      std::stringstream ss;

      ss <<
        "ignoring a slur in a chord member note other than the first one";

      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        ss.str ());
    }

    else {

      // number

      int slurNumber = elt->getAttributeIntValue ("number", 0);

      // type

      fCurrentSlurType = elt->getAttributeValue ("type");

      // placement

      std::string placementString = elt->getAttributeValue ("placement");

      fCurrentDirectionPlacementKind = // use it JMI ???
        msrPlacementKindFromString (
          elt->getInputStartLineNumber (),
          placementString);

      // a phrasing slur is recognized as such
      // when the nested regular slur start is found

      size_t slurStartsStackSize = fSlurStartsStack.size ();

      if (fCurrentSlurType == "start") {
        switch (slurStartsStackSize) {
          case 0:
            fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularStart;
            break;

//           case 1:
          default: // JMI v0.9.67
            {
              S_msrSlur
                containingSlur =
                  fSlurStartsStack.front ();

              fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularStart;

  /* JMI BUGGED?
              // the stack top is in fact a phrasing slur start
  #ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceSlurs ()) {
                gLog <<
" <<
                  containingSlur->asString () <<
                  " contains a nested slur, it is thus a phrasing slur start" <<
                  ", line " << elt->getInputStartLineNumber () <<
                  std::endl;
              }
  #endif // MF_TRACE_IS_ENABLED

              containingSlur->
                setSlurTypeKind (
                  msrSlurTypeKind::kSlurTypePhrasingStart);
                  */
            }
            break;

//           default:
//             {
//               std::stringstream ss;
//
//               ss <<
//                 "only one slur nesting level is meaningfull";
//
//         //      mxsr2msrError ( // JMI
//               mxsr2msrWarning (
//                 gServiceRunData->getInputSourceName (),
//                 elt->getInputStartLineNumber (),
//         //        __FILE__, __LINE__,
//                 ss.str (),
//             }
        } // switch

        fOnGoingSlur = true;
      }

      else if (fCurrentSlurType == "continue") {
        // the current slur continue kind depends on that of the stack's top
        switch (fSlurStartsStack.front ()->getSlurTypeKind ()) {
          case msrSlurTypeKind::kSlurTypeRegularStart:
            fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularContinue;
            break;

          case msrSlurTypeKind::kSlurTypePhrasingStart:
            // the stack top is in fact a phrasing slur start
            fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypePhrasingContinue;
            break;

          default:
            ; // should not occur
        } // switch
      }

      else if (fCurrentSlurType == "stop") {
        fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularStop;
        switch (slurStartsStackSize) {
          case 0:
            {
              std::stringstream ss;

              ss <<
                "a standalone slur 'stop' is meaningless, ignoring it";

              mxsr2msrWarning (
                gServiceRunData->getInputSourceName (),
                elt->getInputStartLineNumber (),
                ss.str ());
            }
            break;

          case 1:
          /* JMI
            // the current slur stop is regular
            fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularStop;

            // pop the top element off the stack
            fSlurStartsStack.pop_front ();
            break;
    */
          case 2:
            // the current slur stop kind depends on that of the stack's top
            switch (fSlurStartsStack.front ()->getSlurTypeKind ()) {
              case msrSlurTypeKind::kSlurTypeRegularStart:
                fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularStop;
                break;

              case msrSlurTypeKind::kSlurTypePhrasingStart:
                // the stack top is in fact a phrasing slur start
  #ifdef MF_TRACE_IS_ENABLED
                if (gTraceOahGroup->getTraceSlurs ()) {
                  gLog <<
                    "A slur stop matches a phrasing slur start, it is thus a phrasing slur stop" <<
                    ", line " << elt->getInputStartLineNumber () <<
                    std::endl;
                }
  #endif // MF_TRACE_IS_ENABLED

                fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypePhrasingStop;
                break;

              default:
                ; // should not occur
            } // switch

            // pop the top element off the stack
            fSlurStartsStack.pop_front ();
            break;

          default:
            ; // should not occur
        } // switch

        fOnGoingSlur = false;
      }

      else {
        // inner slur notes may miss the "continue" type:
        // let' complain only on slur notes outside of slurs
        if (! fOnGoingSlur) {
          if (fCurrentSlurType.size ()) {
            std::stringstream ss;

            ss <<
              "slur type \"" << fCurrentSlurType <<
              "\" is unknown";

            mxsr2msrError (
              gServiceRunData->getInputSourceName (),
              elt->getInputStartLineNumber (),
              __FILE__, __LINE__,
              ss.str ());
          }
        }
      }

      // line-type

      std::string slurLineType = elt->getAttributeValue ("line-type");

      msrLineTypeKind
        slurLineTypeKind =
          msrLineTypeKind::kLineTypeSolid; // default value

      if      (slurLineType == "solid") {
        slurLineTypeKind = msrLineTypeKind::kLineTypeSolid;
      }
      else if (slurLineType == "dashed") {
        slurLineTypeKind = msrLineTypeKind::kLineTypeDashed;
      }
      else if (slurLineType == "dotted") {
        slurLineTypeKind = msrLineTypeKind::kLineTypeDotted;
      }
      else if (slurLineType == "wavy") {
        slurLineTypeKind = msrLineTypeKind::kLineTypeWavy;
      }
      else {
        if (slurLineType.size ()) {
          mxsr2msrError (
            gServiceRunData->getInputSourceName (),
            elt->getInputStartLineNumber (),
            __FILE__, __LINE__,
            "slur line-type \"" + slurLineType + "\" is unknown");
        }
      }

    // color JMI

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceSlurs ()) {
        std::stringstream ss;

        ss <<
          "visitStart (S_slur&)"
          ", slurNumber: " <<
          slurNumber <<
          ", slurTypeKind: " <<
          msrSlurTypeKindAsString (
            fCurrentSlurTypeKind) <<
          ", slurLineType: " <<
          msrLineTypeKindAsString (
            slurLineTypeKind);

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      S_msrSlur
        slur =
          msrSlur::create (
            elt->getInputStartLineNumber (),
            slurNumber,
            fCurrentSlurTypeKind,
            slurLineTypeKind,
            fCurrentDirectionPlacementKind);

      fPendingSlursList.push_back (slur);

      // push slurs starts onto the stack
      switch (fCurrentSlurTypeKind) {
        case msrSlurTypeKind::kSlurTypeRegularStart:
        case msrSlurTypeKind::kSlurTypePhrasingStart:
          fSlurStartsStack.push_front (slur);
          break;
        default:
          ;
      } // switch
    }
  }

  // is the current note a grace note?
  if (fCurrentNoteIsAGraceNote) {
    fCurrentGraceNotesGroupIsSlurred = true;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlursDetails ()) {
    displaySlurStartsStack ("AFTER handling slur");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_bracket& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bracket" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  int ligatureNumber =
    elt->getAttributeIntValue ("number", 0);

  // type

  std::string ligatureType =
    elt->getAttributeValue ("type");

  fCurrentLigatureKind = msrLigatureKind::kLigatureNone;

  if      (ligatureType == "start") {
    fCurrentLigatureKind = msrLigatureKind::kLigatureStart;
    fOnGoingLigature = true;
  }
  else if (ligatureType == "continue") {
    fCurrentLigatureKind = msrLigatureKind::kLigatureContinue;
  }
  else if (ligatureType == "stop") {
    fCurrentLigatureKind = msrLigatureKind::kLigatureStop;
    fOnGoingLigature = false;
  }
  else {

    // inner ligature notes may miss the "continue" type:
    // let's complain on ligature notes outside of ligatures
    if (! fOnGoingLigature) {
      if (ligatureType.size ()) {
        std::stringstream ss;

        ss <<
          "ligature type \"" << ligatureType <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // line-end

  std::string ligatureLineEndValue = elt->getAttributeValue ("line-end");

  msrLigatureLineEndKind
    ligatureLineEndKind = msrLigatureLineEndKind::kLigatureLineEndNone;

  if      (ligatureLineEndValue == "up") {
    ligatureLineEndKind = msrLigatureLineEndKind::kLigatureLineEndUp;
  }
  else if (ligatureLineEndValue == "down") {
    ligatureLineEndKind = msrLigatureLineEndKind::kLigatureLineEndDown;
  }
  else if (ligatureLineEndValue == "both") {
    ligatureLineEndKind = msrLigatureLineEndKind::kLigatureLineEndBoth;
  }
  else if (ligatureLineEndValue == "arrow") {
    ligatureLineEndKind = msrLigatureLineEndKind::kLigatureLineEndArrow;
  }
  else if (ligatureLineEndValue == "none") {
    ligatureLineEndKind = msrLigatureLineEndKind::kLigatureLineEndNone;
  }
  else {
    if (ligatureLineEndValue.size ()) {
      std::stringstream ss;

      ss <<
        "ligature line-end \"" << ligatureLineEndValue <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // line-type

  std::string ligatureLineTypeValue = elt->getAttributeValue ("line-type");

  msrLineTypeKind
    ligatureLineTypeKind =
      msrLineTypeKind::kLineTypeSolid; // default value

  if      (ligatureLineTypeValue == "solid") {
    ligatureLineTypeKind = msrLineTypeKind::kLineTypeSolid;
  }
  else if (ligatureLineTypeValue == "dashed") {
    ligatureLineTypeKind = msrLineTypeKind::kLineTypeDashed;
  }
  else if (ligatureLineTypeValue == "dotted") {
    ligatureLineTypeKind = msrLineTypeKind::kLineTypeDotted;
  }
  else if (ligatureLineTypeValue == "wavy") {
    ligatureLineTypeKind = msrLineTypeKind::kLineTypeWavy;
  }
  else {
    if (ligatureLineTypeValue.size ()) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        "ligature line-type \"" + ligatureLineTypeValue + "\" is unknown");
    }
  }

  // color JMI

  // create the ligature
  S_msrLigature
    ligature =
      msrLigature::create (
        elt->getInputStartLineNumber (),
        ligatureNumber,
        fCurrentLigatureKind,
        ligatureLineEndKind,
        ligatureLineTypeKind,
        fCurrentDirectionPlacementKind);

  // append it to the pending ligatures list
  fPendingLigaturesList.push_back (ligature);

  switch (fCurrentLigatureKind) {
    case msrLigatureKind::kLigatureStart:
      // remember this ligature spanner start
      switch (fCurrentDirectionPlacementKind) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          {
#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTraceLigatures ()) {
              std::stringstream ss;

              ss <<
                "Bracket start found with no placement, placing it above by default";

              mxsr2msrWarning (
                gServiceRunData->getInputSourceName (),
                elt->getInputStartLineNumber (),
                ss.str ());
            }
#endif // MF_TRACE_IS_ENABLED

            ligature->setLigaturePlacementKind (msrPlacementKind::kPlacementAbove);
            fCurrentLigatureStartAbove = ligature;
          }
          break;

        case msrPlacementKind::kPlacementAbove:
          fCurrentLigatureStartAbove = ligature;
          break;

        case msrPlacementKind::kPlacementBelow:
          fCurrentLigatureStartBelow = ligature;
          break;
      } // switch
      break;

    case msrLigatureKind::kLigatureStop:
      // set spanner two-way sideLinks
      // between both ends of the ligature spanner

      switch (fCurrentDirectionPlacementKind) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          {
#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTraceLigatures ()) {
              std::stringstream ss;

              ss <<
                "Bracket stop found with no placement, placing it above by default";

              mxsr2msrWarning (
                gServiceRunData->getInputSourceName (),
                elt->getInputStartLineNumber (),
                ss.str ());
            }
#endif // MF_TRACE_IS_ENABLED

            ligature->setLigaturePlacementKind (msrPlacementKind::kPlacementAbove);

            if (! fCurrentLigatureStartAbove) {
              std::stringstream ss;

              ss <<
                "Bracket 'stop above' found with no corresponding bracket start, ignoring it"; // JMI cannot occur

           // JMI   mxsr2msrError (
              mxsr2msrWarning (
                gServiceRunData->getInputSourceName (),
                elt->getInputStartLineNumber (),
           //     __FILE__, __LINE__,
                ss.str ());
            }

            else {
              ligature->
                setLigatureSideLinkToOtherEnd (
                  fCurrentLigatureStartAbove);

              // forget this ligature spanner start
              fCurrentLigatureStartAbove = nullptr;
            }
          }
          break;

        case msrPlacementKind::kPlacementAbove:
          if (! fCurrentLigatureStartAbove) {
            std::stringstream ss;

            ss <<
              "Bracket 'stop above' found with no corresponding bracket start, ignoring it";

         // JMI   mxsr2msrError (
            mxsr2msrWarning (
              gServiceRunData->getInputSourceName (),
              elt->getInputStartLineNumber (),
         //     __FILE__, __LINE__,
              ss.str ());
          }

          else {
            ligature->
              setLigatureSideLinkToOtherEnd (
                fCurrentLigatureStartAbove);

            // forget this ligature spanner start
            fCurrentLigatureStartAbove = nullptr;
          }
          break;

        case msrPlacementKind::kPlacementBelow:
          if (! fCurrentLigatureStartBelow) {
            std::stringstream ss;

            ss <<
              "Bracket 'stop below' found with no corresponding bracket start, ignoring it";

         // JMI   mxsr2msrError (
            mxsr2msrWarning (
              gServiceRunData->getInputSourceName (),
              elt->getInputStartLineNumber (),
         //     __FILE__, __LINE__,
              ss.str ());
          }

          else {
            ligature->
              setLigatureSideLinkToOtherEnd (
                fCurrentLigatureStartBelow);

            // forget this ligature spanner start
            fCurrentLigatureStartBelow = nullptr;
          }
          break;
      } // switch
      break;

    case msrLigatureKind::kLigatureContinue:
      // JMI ???
      break;

    case msrLigatureKind::kLigatureNone:
      // JMI ???
      break;
  } // switch
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_wedge& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_wedge" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  int wedgeNumber =
    elt->getAttributeIntValue ("number", -1); // number is mandatory

  // type

  std::string type = elt->getAttributeValue("type");

  msrWedgeKind wedgeKind = msrWedgeKind::kWedgeKindNone;

  if      (type == "crescendo") {
    wedgeKind = msrWedgeKind::kWedgeCrescendo;
  }
  else if (type == "diminuendo") {
    wedgeKind = msrWedgeKind::kWedgeDecrescendo;
  }
  else if (type == "stop") {
    wedgeKind = msrWedgeKind::kWedgeStop;
  }
  else {
    if (type.size ()) {
      std::stringstream ss;

      ss <<
        "unknown wedge type \"" +
        type +
        "\", should be 'crescendo', 'diminuendo' or 'stop'";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // niente

  std::string nienteString = elt->getAttributeValue ("niente");

  msrWedgeNienteKind
    wedgeNienteKind = msrWedgeNienteKind::kWedgeNienteNo;

  if       (nienteString == "yes") {
    wedgeNienteKind = msrWedgeNienteKind::kWedgeNienteYes;
  }
  else  if (nienteString == "no") {
    wedgeNienteKind = msrWedgeNienteKind::kWedgeNienteNo;
  }
  else {
    if (nienteString.size ()) {
      std::stringstream ss;

      ss <<
        "wedge niente \"" << nienteString <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // line-type

  std::string wedgeLineType = elt->getAttributeValue ("line-type");

  msrLineTypeKind
    wedgeLineTypeKind =
      msrLineTypeKind::kLineTypeSolid; // default value

  if      (wedgeLineType == "solid") {
    wedgeLineTypeKind = msrLineTypeKind::kLineTypeSolid;
  }
  else if (wedgeLineType == "dashed") {
    wedgeLineTypeKind = msrLineTypeKind::kLineTypeDashed;
  }
  else if (wedgeLineType == "dotted") {
    wedgeLineTypeKind = msrLineTypeKind::kLineTypeDotted;
  }
  else if (wedgeLineType == "wavy") {
    wedgeLineTypeKind = msrLineTypeKind::kLineTypeWavy;
  }
  else {
    if (wedgeLineType.size ()) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        "wedge line-type \"" + wedgeLineType + "\" is unknown");
    }
  }

  // color JMI ??? v0.9.70

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllWedgesBelow ()) {
    fCurrentDirectionPlacementKind = msrPlacementKind::kPlacementBelow;
  }

  if (! gGlobalMxsr2msrOahGroup->getIgnoreWedges ()) {
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceWedges ()) {
    std::stringstream ss;

    ss <<
      "Creating wedge" <<
      ", wedgeKind: " << wedgeKind <<
      ", fCurrentDirectionPlacementKind: " <<
      fCurrentDirectionPlacementKind <<
      ", fCurrentNoteVoiceNumber: " <<
      fCurrentNoteVoiceNumber <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    S_msrWedge
      wedge =
        msrWedge::create (
          elt->getInputStartLineNumber (),
          wedgeNumber,
          wedgeKind,
          wedgeNienteKind,
          wedgeLineTypeKind,
          fCurrentDirectionPlacementKind,
          fCurrentNoteVoiceNumber);

    fPendingWedgesList.push_back (wedge);

    fPendingVoiceWedgesList.push_back (
      std::make_pair (
        fCurrentNoteVoiceNumber,
        wedge));
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_lyric& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_lyric" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  {
    fCurrentStanzaNumber =
      elt->getAttributeValue ("number");

    if (fCurrentStanzaNumber.size () == 0) {
      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        "lyric number is empty, using \"1\" by default");

      fCurrentStanzaNumber = "1";
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "Setting fCurrentStanzaNumber to " <<
        fCurrentStanzaNumber <<
        ", line " << elt->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // register it as current stanza number,
    // that remains set another positive value is found,
    // thus allowing a skip syllable to be generated
    // for notes without lyrics
  }

  // name

  {
    fCurrentStanzaName =
      elt->getAttributeValue ("name");

    if (fCurrentStanzaName.size () == 0) {
      // lyrics names are not so frequent after all...
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLyrics ()) {
        std::stringstream ss;

        ss <<
          "Lyric name is empty, using \"" <<
          K_STANZA_NAME_UNKNOWN_ <<
          "\" by default";

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentStanzaName = K_STANZA_NAME_UNKNOWN_;
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "Setting fCurrentStanzaName to \"" <<
        fCurrentStanzaName <<
        "\", line " << elt->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // register it as current stanza name,
    // that remains set another positive value is found,
    // thus allowing a skip syllable to be generated
    // for notes without lyrics
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

  // color JMI

  // forget about any previous texts and elisions found if any
  fCurrentSyllableElementsList.clear ();

  // a <text/> markup puts an end to the effect of <extend/> JMI v0.9.67
  fCurrentSyllableExtendKind = msrSyllableExtendKind::kSyllableExtend_NONE;

  fOnGoingLyric = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_syllabic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_syllabic" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentSyllabic = elt->getValue();

  fCurrentSyllableKind = msrSyllableKind::kSyllableNone;

  if      (fCurrentSyllabic == "single") {
    fCurrentSyllableKind = msrSyllableKind::kSyllableSingle;
  }
  else if (fCurrentSyllabic == "begin") {
    fCurrentSyllableKind = msrSyllableKind::kSyllableBegin;
  }
  else if (fCurrentSyllabic == "middle") {
    fCurrentSyllableKind = msrSyllableKind::kSyllableMiddle;
  }
  else if (fCurrentSyllabic == "end") {
    fCurrentSyllableKind = msrSyllableKind::kSyllableEnd;
  }
  else {
    std::stringstream ss;

    ss <<
      "syllabic \"" << fCurrentSyllabic <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

//   // forget about any previous texts found
//   fCurrentSyllableElementsList.clear ();
}

void mxsr2msrSkeletonPopulator::visitStart (S_text& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputStartLineNumber =
    elt->getInputStartLineNumber ();

  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_text" <<
      ", line " << inputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string textValue = elt->getValue();

  mfConvertHTMLEntitiesToPlainCharacters (textValue); // JMI v0.9.70 ???

  // color JMI

  // there can be several <text/>'s and <elision/> in a row, hence the list
  fCurrentSyllableElementsList.push_back (
    msrSyllableElement (
      elt->getInputStartLineNumber (),
      msrSyllableElementKind::kSyllableElementText,
      textValue));

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "visitStart (S_text& elt), textValue = \"" << textValue << "\"" <<
      ", line " << inputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    ++gIndenter;

    constexpr int fieldWidth = 23;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "line " << ": " << inputStartLineNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStanzaNumber" << ": " << fCurrentStanzaNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentSyllabic" << ": " << fCurrentSyllabic <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentSyllableElementsList" << ": " <<
      syllableElementsListAsString (fCurrentSyllableElementsList) <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_elision& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_elision" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string elisionValue = elt->getValue ();

/*
  Multiple syllables on a single note are separated by elision elements.
  A hyphen in the
    text element should only be used for an actual hyphenated
    word.
  Two text elements that are not separated by an
    elision element are part of the same syllable, but may have
    different text formatting.
*/

  fCurrentSyllableElementsList.push_back (
    msrSyllableElement (
      elt->getInputStartLineNumber (),
      msrSyllableElementKind::kSyllableElementElision,
      elisionValue));

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "visitStart (S_elision& elt), elisionValue = \"" << elisionValue << "\"" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_extend& elt)
{
/*
<!--
	The extend element represents lyric word extension /
	melisma lines as well as figured bass extensions. The
	optional type and position attributes are added in
	Version 3.0 to provide better formatting control.
-->
<!ELEMENT extend EMPTY>
<!ATTLIST extend
    type %start-stop-continue; #IMPLIED
    %position;
    %color;
>
*/

  int inputStartLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_extend" <<
      ", line " << inputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // type

  std::string extendType =
    elt->getAttributeValue ("type");

  fCurrentSyllableExtendKind =
    msrSyllableExtendKind::kSyllableExtend_NONE; // default value

  if (fOnGoingLyric) {
    if      (extendType == "start") {
      fCurrentSyllableExtendKind =
        msrSyllableExtendKind::kSyllableExtendTypeStart;
    }
    else if (extendType == "continue") {
      fCurrentSyllableExtendKind =
        msrSyllableExtendKind::kSyllableExtendTypeContinue;
    }
    else if (extendType == "stop") {
      fCurrentSyllableExtendKind =
        msrSyllableExtendKind::kSyllableExtendTypeStop;
    }
    else if (! extendType.size ()) {
      fCurrentSyllableExtendKind =
        msrSyllableExtendKind::kSyllableExtendTypeLess;
    }
    else {
      std::stringstream ss;

      ss <<
        "extend type \"" << extendType <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        inputStartLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  else if (fOnGoingFiguredBass) { // JMI v0.9.67
  }

  // color JMI
}

void mxsr2msrSkeletonPopulator::visitEnd (S_lyric& elt)
{
  int inputStartLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_lyric" <<
      ", line " << inputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentSyllableKind == msrSyllableKind::kSyllableNone) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      // syllabic is not mandatory, thus:
      std::stringstream ss;

      ss <<
        "<lyric /> has no <syllabic/> component, using 'single' by default";

      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        inputStartLineNumber,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentSyllableKind = msrSyllableKind::kSyllableSingle;
  }

  if (fCurrentNoteIsARest) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "syllable " <<
        syllableElementsListAsString (fCurrentSyllableElementsList) <<
        " is attached to a rest";

      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        inputStartLineNumber,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentSyllableKind =
      msrSyllableKind::kSyllableSkipOnRestNote;

//     if (fCurrentSyllableElementsList.size ()) {
//       // register a skip in lyrics for rests with syllables
//       fCurrentSyllableKind =
//         msrSyllableKind::kSyllableOnRestNote;
//     }
//     else {
//       // don't register a skip in lyrics for rests without syllables
//       fCurrentSyllableKind =
//         msrSyllableKind::kSyllableSkipOnRestNote;
//     }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyricsDetails ()) {
    gLog <<
      "==> visitEnd (S_lyric&), fCurrentSyllableKind: " <<
      fCurrentSyllableKind <<
      ", line " << inputStartLineNumber <<
      ", with:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      "Lyric data:" <<
      std::endl;

    {
      ++gIndenter;

      constexpr int fieldWidth = 31;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentNoteStaffNumber" << ": " <<
        mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
        std::endl <<

        std::setw (fieldWidth) <<
        "fCurrentStanzaNumber" << ": " << fCurrentStanzaNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
        std::endl <<

        std::setw (fieldWidth) <<
        "fCurrentSyllableElementsList" << "  " <<
        syllableElementsListAsString (fCurrentSyllableElementsList) <<
        std::endl <<

        std::setw (fieldWidth) <<
        "fCurrentSyllableExtendKind" << ": " <<
        fCurrentSyllableExtendKind <<
        std::endl <<

        std::setw (fieldWidth) <<
        "fCurrentNoteIsARest" << ": " <<
        fCurrentNoteIsARest <<
        std::endl <<

        std::setw (fieldWidth) <<
        "fCurrentRestIsAMeasureRest" << ": " <<
        fCurrentRestIsAMeasureRest <<
        std::endl;

      gLog << std::left <<
        "fNoteTiesList:";
      if (fPendingTiesList.size ()) {
        ++gIndenter;
        for (S_msrTie tie : fPendingTiesList) {
          gLog << tie;
        } // for
        --gIndenter;
      }
      else {
        gLog << "[NONE]" << std::endl;
      }

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentTieKind" << ": \"" <<
        msrTieKindAsString (fCurrentTieKind) <<
        "\"" <<
        std::endl;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentSlurTypeKind" << ": \"" <<
        msrSlurTypeKindAsString (fCurrentSlurTypeKind) <<
        "\"" <<
        std::endl;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentLigatureKind" << ": \"" <<
        fCurrentLigatureKind <<
        "\"" <<
        std::endl;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fOnGoingSlur" << ": " <<
        fOnGoingSlur <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fOnGoingSlurHasStanza" << ": " <<
        fOnGoingSlurHasStanza <<
        std::endl;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fOnGoingLigature" << ": " <<
        fOnGoingLigature <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fOnGoingLigatureHasStanza" << ": " <<
        fOnGoingLigatureHasStanza <<
        std::endl;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fFirstSyllableInSlurKind" << ": \"" <<
        fFirstSyllableInSlurKind <<
        "\"" <<
        std::endl;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fFirstSyllableInLigatureKind" << ": \"" <<
        fFirstSyllableInLigatureKind <<
        "\"" <<
      std::endl;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentSyllableKind" << ": \"" <<
        fCurrentSyllableKind <<
        "\"" <<
      std::endl;

      --gIndenter;
    }

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "==> visitEnd (S_lyric&), fCurrentSyllableKind: " <<
      fCurrentSyllableKind <<
      ", line: " << inputStartLineNumber <<
      ", with:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      "Lyric data:" <<
      std::endl;

    {
      ++gIndenter;

      constexpr int fieldWidth = 31;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentNoteStaffNumber" << ": " <<
        mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaNumber" << ": " << fCurrentStanzaNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentSyllableElementsList" << ": " <<
        syllableElementsListAsString (fCurrentSyllableElementsList) <<
        std::endl;

      --gIndenter;
    }

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalMxsr2msrOahGroup->getIgnoreLyrics ()) {
    // fetch the current voice, fCurrentNote has not been set/updated yet
    S_msrVoice
      currentNoteVoice =
        fCurrentPartStaffMsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

    // fetch stanzaNumber in the current note's voice
    S_msrStanza
      stanza =
        currentNoteVoice->
          fetchStanzaInVoice (
            inputStartLineNumber,
            fCurrentStanzaNumber,
            fCurrentStanzaName);

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "Creating a syllable \"" <<
        fCurrentSyllableKind <<
        "\", fCurrentSyllableElementsList = \"" <<
        syllableElementsListAsString (fCurrentSyllableElementsList) <<
        "\"" <<
        ", whole notes: " <<
        fCurrentNoteSoundingWholeNotesFromNotesDuration <<
        " sounding from duration, " <<
         fCurrentNoteDisplayWholeNotesFromType <<
        ", display from type" <<
        ", syllabic: \"" <<
        fCurrentSyllableKind << "\"" <<
        ", in stanza " << stanza->getStanzaName () <<
        ", line " << inputStartLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create a syllable
    S_msrSyllable
      syllable =
        msrSyllable::create (
          inputStartLineNumber,
          fCurrentSyllableKind,
          fCurrentSyllableExtendKind,
          fCurrentStanzaNumber,
          fCurrentNoteSoundingWholeNotesFromNotesDuration,
          msrTupletFactor (
            fCurrentNoteActualNotes,
            fCurrentNoteNormalNotes),
          stanza);

    // append the lyric texts to the syllable
    for (msrSyllableElement syllableElement : fCurrentSyllableElementsList) {
      syllable->
        appendSyllableElementToSyllable (syllableElement);
    } // for

    // don't forget about fCurrentSyllableElementsList here,
    // this will be done in visitStart (S_syllabic& )

    // appendSyllableToNoteAndSetItsUpLinkToNote()
    // will be called in handleLyrics(),
    // after the note has been created

    // append syllable to current note's syllables list
    fCurrentNoteSyllablesList.push_back (
      syllable);

    // fetch the voice
    S_msrVoice
      theMsrVoice =
        stanza->getStanzaUpLinkToVoice ();

    // set the syllable's measure uplink
    syllable->
      setSyllableUpLinkToMeasure (
        theMsrVoice->
          fetchVoiceLastMeasure (inputStartLineNumber));

    // fetch the part
    S_msrPart
      part =
        theMsrVoice->
          fetchVoiceUpLinkToPart ();

    // fetch the part current measure position
    msrWholeNotes
      partCurrentDrawingMeasurePosition =
        part->
          getPartCurrentDrawingMeasurePosition ();

    // append syllable to stanza
    stanza->
      appendSyllableToStanza (
        syllable,
        theMsrVoice->getVoiceLastAppendedMeasure (),
        partCurrentDrawingMeasurePosition);
  }

  // DON'T register current note as having lyrics,
  // it's only the case when there are <text/> inside the <lyric/>:
  // the latter may contain only an <extend/> markup

  fOnGoingLyric = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_measure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take this measure into account
  ++fPartMeasuresCounter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresDetails ()) {
    std::stringstream ss;

    ss <<
      "==> visitStart (S_measure" <<
      ", fPartMeasuresCounter: " <<
      fPartMeasuresCounter <<
      ", fCurrentMeasureNumber: " <<
        fCurrentMeasureNumber <<
      ", line " << elt->getInputStartLineNumber () <<
      ", in part \"" <<
      fCurrentPart->fetchPartCombinedName () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number
  fCurrentMeasureNumber =
    elt->getAttributeValue ("number");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "%--------------------------------------------------------------" <<
      std::endl <<
      "   ===>> " <<
      std::endl <<
      "      " <<
      "part " <<
      fCurrentPart-> fetchPartCombinedName () <<
      std::endl <<
      "      " <<
      "measure " << fCurrentMeasureNumber <<
      ", line " <<
      elt->getInputStartLineNumber () <<
      std::endl <<
      "   <<===" <<
      std::endl <<
      "%--------------------------------------------------------------" <<
      std::endl;

    gWaeHandler->waeTraceWithoutInputLocation ( // JMI v0.9.71 without CODE location ???
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // implicit
/*
  Measures with an implicit attribute set to "yes"
  never display a measure number,
  regardless of the measure-numbering setting.
  *
  The implicit attribute is set to "yes" for measures where
  the measure number should never appear, such as pickup
  measures and the last half of mid-measure repeats. The
  value is "no" if not specified.
*/

  std::string
    implicitString =
      elt->getAttributeValue ("implicit");

  msrMeasureImplicitKind
    measureImplicitKind =
      msrMeasureImplicitKind::kMeasureImplicitKindNo; // default value

  if       (implicitString == "yes") {
    measureImplicitKind =
      msrMeasureImplicitKind::kMeasureImplicitKindYes;
  }
  else  if (implicitString == "no") {
    measureImplicitKind =
      msrMeasureImplicitKind::kMeasureImplicitKindNo;
  }
  else {
    if (implicitString.size ()) {
      std::stringstream ss;

      ss <<
        "implicit \"" << implicitString <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // non-controlling
  std::string
    nonControllingString =
      elt->getAttributeValue ("non-controlling");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    // width
    int
      widthValue =
        elt->getAttributeIntValue ("width", 0);

    std::stringstream ss;

    ss <<
      "<!--=== " <<
      "partName \"" << fCurrentPart->getPartName () << "\"" <<
      ", partID: \"" << fCurrentPart->getPartID () << "\"" <<
      ", fCurrentMeasureNumber: \"" << fCurrentMeasureNumber << "\"" <<
      ", measureImplicitKind: " << measureImplicitKind <<
      ", nonControllingString: \"" << nonControllingString << "\"" <<
      ", widthValue: " << widthValue <<
      ", line " << elt->getInputStartLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the current measure number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentMeasureNumber (
      fCurrentMeasureNumber);

  // set next measure number in current part' previous measure
  // if this measure is not the first one
  if (fPartMeasuresCounter > 1) { // JMI v0.9.71
//     if (fCurrentPart) {
      fCurrentPart->
        setNextMeasureNumberInPart (
          elt->getInputStartLineNumber (),
          fCurrentMeasureNumber);
//     }
//     else {
//       // JMI ???
//     }
  }

  // consistency check
  if (! fPartFirstMeasureNumber.size ()) {
    // this is the first measure in the part
    fPartFirstMeasureNumber = fCurrentMeasureNumber;

    if (fScoreFirstMeasureNumber.size () == 0) {
      // this is the first measure of the first part in the score
      fScoreFirstMeasureNumber = fPartFirstMeasureNumber;
    }
    else {
      if (fPartFirstMeasureNumber != fScoreFirstMeasureNumber) {
        std::stringstream ss;

        ss <<
          "measure numbering inconsistency: first measure numbers '" <<
          fScoreFirstMeasureNumber <<
           "' and '" <<
          fPartFirstMeasureNumber <<
          "' found";

        mxsr2msrInternalError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // reset the part measure position
  fCurrentPart->
    setPartCurrentDrawingMeasurePosition (
      elt->getInputStartLineNumber (),
      msrWholeNotes (0, 1));

  // forget about the current non-grace note JMI v0.9.67???
// JMI  fCurrentNonGraceNote = nullptr;

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // create a new measure and append it to the current part
  fCurrentPart->
    createAMeasureAndAppendItToPart (
      elt->getInputStartLineNumber (),
      fPreviousMeasureEndInputLineNumber,
      fCurrentMeasureNumber,
      measureImplicitKind);

  fPreviousMeasureEndInputLineNumber = elt->getInputStartLineNumber ();

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

/* JMI
  // is this measure number in the debug set?
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDetailed ()) {
    std::set <int>::const_iterator
      it =
        gServiceRunData->fTraceDetailedMeasureNumbersSet.find (fMeasuresCounter);

    if (it != gTraceOahGroup->getTraceDetailedMeasureNumbersSet ().end ()) {
      // yes, activate detailed trace for it
      gGlobalMxsr2msrOahGroup = gGlobalMxsr2msrOahWithDetailedTrace;
      gServiceRunData  = gGlobalCurrentServiceRunDataWithDetailedTrace;
      gMsrOahGroup      = gGlobalMsrOahWithDetailedTrace;
      gLpsrOahGroup     = gGlobalLpsrOahWithDetailedTrace;
      gGlobalLpsr2lilypondOahGroup = gGlobalLilypondOahWithDetailedTrace;
    }
  }
#endif // MF_TRACE_IS_ENABLED
  */

  // reset the drawing measure position in the current part
  fCurrentPart->
    resetPartCurrentDrawingMeasurePosition (
      elt->getInputStartLineNumber ());
}

void mxsr2msrSkeletonPopulator::visitEnd (S_measure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_measure" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // take finalization action if relevant v0.9.70
  if (
    fCurrentNoteStaffNumber != K_STAFF_NUMBER_UNKNOWN_
      &&
    fCurrentNoteStaffNumber != K_STAFF_NUMBER_UNKNOWN_
  ) {
    // fetch the voice
    S_msrVoice
      theMsrVoice =
        fCurrentPartStaffMsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

    // fetch note to attach to
    S_msrNote
      noteToAttachTo =
      /*
      // JMI might prove not precise enough??? v0.9.72
  //      fStaffVoicesLastMetNoteMap [currentNoteVoice];
        fStaffVoicesLastMetNoteMap [
          std::make_pair (
            fCurrentNoteStaffNumber,
            fCurrentNoteVoiceNumber)
          ];
      */
        theMsrVoice->getVoiceLastAppendedNote (); // ??? JMI v0.9.70

    // is there a pending grace notes group?
    if (fPendingGraceNotesGroup) {
  #ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceGraceNotes ()) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "fCurrentGraceNotesGroupNotes IS NOT NULL at the end of measure " << // JMI
          elt->getAttributeValue ("number") <<
          std::endl;

        ss <<
          "+++++++++++++++++" <<
          fCurrentPart;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
  #endif // MF_TRACE_IS_ENABLED

      // attach these grace notes group as an after grace notes group
      // to the last note found in its voice
      if (! noteToAttachTo) {
        std::stringstream ss;

        ss <<
          "cannot find voice to insert after grace notes group into: " <<
          "chordFirstNote is null" <<
          ", fCurrentNoteStaffNumber: " <<
          mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
          std::endl <<
          ", fCurrentNoteVoiceNumber: " <<
          fCurrentNoteVoiceNumber <<
          ", line " << elt->getInputStartLineNumber ();

        mxsr2msrInternalError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }

      // set the pending grace notes group's kind to 'after'
      fPendingGraceNotesGroup->
        setGraceNotesGroupKind (
          msrGraceNotesGroupKind::kGraceNotesGroupAfter);

      // attach it to the note
      noteToAttachTo->
        setGraceNotesGroupAfterNote (
          fPendingGraceNotesGroup);

      // forget about this grace notes group
      fPendingGraceNotesGroup = nullptr;
    }

    // attach the spanners if any to the note
    if (fCurrentSpannersList.size ()) {
      attachCurrentSpannersToNote (
        noteToAttachTo,
        "mxsr2msrSkeletonPopulator::visitEnd (S_measure& elt)");
    }

    // are there pending voices wedges?
#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  if (
    gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
  ) {
#ifdef MF_TRACE_IS_ENABLED
    if (
      gTraceOahGroup->getTraceParts ()
    ) {
      // maintainance check
      if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN // JMI v0.9.70
        std::stringstream ss;

        int numberOfPendingVoicesWedges = fPendingVoiceWedgesList.size ();

        if (numberOfPendingVoicesWedges > 0) {
          ss <<
            "fPendingVoiceWedgesList contains PENDING voice wedges ZOU: " << // JMI v0.9.70
            mfSingularOrPlural (
              numberOfPendingVoicesWedges, "element", "elements");

          mxsr2msrWarning (
            gServiceRunData->getInputSourceName (),
            fCurrentNote->getInputStartLineNumber (),
            ss.str ());
        }
      }
    }
#endif // MF_TRACE_IS_ENABLED
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

    // finalize current measure in the part,
    // to add skips if necessary and set measure kind
    fCurrentPart->
      finalizeLastAppendedMeasureInPart (
        elt->getInputStartLineNumber ());

    // should this measure be replicated?
    const std::map <std::string,int>&
      measuresToBeReplicatedStringToIntMap =
        gGlobalMxsr2msrOahGroup->getMeasuresToBeReplicatedStringToIntMap ();

    if (measuresToBeReplicatedStringToIntMap.size ()) { // JMI v0.9.70
      // should we add empty measures after current measures?
      std::map <std::string,int>::const_iterator
        it =
          measuresToBeReplicatedStringToIntMap.find (
            fCurrentMeasureNumber);

      if (it != measuresToBeReplicatedStringToIntMap.end ()) {
        // fCurrentMeasureNumber is to be replicated,
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
          std::stringstream ss;

          ss <<
            std::endl <<
            "Replicating meaure " <<
            fCurrentMeasureNumber <<
            " in part " <<
            fCurrentPart->fetchPartCombinedName ();

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
  #endif // MF_TRACE_IS_ENABLED

        int measureReplicatesNumber = (*it).second;

        fCurrentPart->
          replicateLastAppendedMeasureInPart (
            elt->getInputStartLineNumber (),
            measureReplicatesNumber);
      }
      else {
        // fRemainingMeasureRestsCounter JMI ???
      }
    }
  }

  // should empty measures be added after this one?
  const std::map <std::string,int>&
    addEmptyMeasuresStringToIntMap =
      gGlobalMxsr2msrOahGroup->getAddEmptyMeasuresStringToIntMap ();

  // should we add empty measures after current measures?
  if (addEmptyMeasuresStringToIntMap.size ()) {
    std::map <std::string,int>::const_iterator
      it =
        addEmptyMeasuresStringToIntMap.find (
          fCurrentMeasureNumber);

    if (it != addEmptyMeasuresStringToIntMap.end ()) {
      // fCurrentMeasureNumber is present in the map,
      // fetch the number of empty measures to add
      std::stringstream ss;

      ss << (*it).second;

      int measuresToBeAdded;

      ss >> measuresToBeAdded;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "Adding " <<
          mfSingularOrPlural (
            measuresToBeAdded, "empty measure", "empty measures") <<
          " to part " <<
          fCurrentPart->fetchPartCombinedName ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentPart->
        appendEmptyMeasuresToPart (
          elt->getInputStartLineNumber (),
          fCurrentMeasureNumber,
          measuresToBeAdded);
    }
    else {
      // fRemainingMeasureRestsCounter JMI ???
    }
  }

  // staff changes handling
  // a new measure knows nothing about staff changes in the previous one if any
  // a new sequence of staff changes will start on its first note's staff
  fCurrentRecipientStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
}

// void mxsr2msrSkeletonPopulator::handleOnGoingMultiMeasureRestsAtTheEndOfMeasure (
//   int inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
//     constexpr int fieldWidth = 37;
//
//     gLog <<
//       "--> mxsr2msrSkeletonPopulator::handleOnGoingMultiMeasureRestsAtTheEndOfMeasure()" <<
//       std::endl;
//
//     ++gIndenter;
//
//     gLog <<
//       std::setw (fieldWidth) <<
//       "fRemainingMeasureRestsCounter" << ": " <<
//       fRemainingMeasureRestsCounter <<
//       std::endl << std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (fRemainingMeasureRestsCounter <= 0) {
//     mxsr2msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, __LINE__,
//       "fRemainingMeasureRestsCounter problem");
//   }
//
//   // account for one more measure rest in the multi-measure rests
//   --fRemainingMeasureRestsCounter;
//
//   if (fRemainingMeasureRestsCounter == 0) {
//     // all multi-measure rests have been handled,
//     // the current one is the first after the  multi-measure rests
//     fCurrentPart->
//       appendPendingMultiMeasureRestsToPart (
//         inputLineNumber);
//
//     if (fRemainingMeasureRestsCounter == 1) {
//       fCurrentPart-> // JMI ??? BOF
//         setNextMeasureNumberInPart (
//           inputLineNumber,
//           fCurrentMeasureNumber);
//     }
//   }
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
//     constexpr int fieldWidth = 37;
//
//     gLog <<
//       "--> mxsr2msrSkeletonPopulator::handleOnGoingMultiMeasureRestsAtTheEndOfMeasure()" <<
//       ", onGoingMultiMeasureRests:" <<
//       std::endl;
//
//     ++gIndenter;
//
//     gLog <<
//       std::setw (fieldWidth) <<
//       "fRemainingMeasureRestsCounter" << ": " <<
//       fRemainingMeasureRestsCounter <<
//       std::endl <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
// }

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_print& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_print" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  The print element contains general printing parameters,
  including the layout elements defined in the layout.mod
  file. The part-name-display and part-abbreviation-display
  elements used in the score.mod file may also be used here
  to change how a part name or abbreviation is displayed over
  the course of a piece. They take effect when the current
  measure or a succeeding measure starts a new system.

  The new-system and new-page attributes indicate whether
  to force a system or page break, or to force the current
  music onto the same system or page as the preceding music.
  Normally this is the first music data within a measure.
  If used in multi-part music, they should be placed in the
  same positions within each part, or the results are
  undefined. The page-number attribute sets the number of a
  new page; it is ignored if new-page is not "yes". Version
  2.0 adds a blank-page attribute. This is a positive integer
  value that specifies the number of blank pages to insert
  before the current measure. It is ignored if new-page is
  not "yes". These blank pages have no music, but may have
  text or images specified by the credit element. This is
  used to allow a combination of pages that are all text,
  or all text and images, together with pages of music.

Staff spacing between multiple staves is measured in
  tenths of staff lines (e.g. 100 = 10 staff lines). This is
  deprecated as of Version 1.1; the staff-layout element
  should be used instead. If both are present, the
  staff-layout values take priority.

  Layout elements in a print statement only apply to the
  current page, system, staff, or measure. Music that
  follows continues to take the default values from the
  layout included in the defaults element.

<!ELEMENT print (page-layout?, system-layout?, staff-layout*,
    measure-layout?, measure-numbering?, part-name-display?,
    part-abbreviation-display?)>
<!ATTLIST print
    staff-spacing %tenths; #IMPLIED
    new-system %yes-no; #IMPLIED
    new-page %yes-no; #IMPLIED
    blank-page NMTOKEN #IMPLIED
    page-number CDATA #IMPLIED
    %optional-unique-id;
>

  */

  // create a print layout
  fCurrentMusicXMLPrintLayout =
     msrMusicXMLPrintLayout::create (
      elt->getInputStartLineNumber ());

  // handle 'staff-spacing' if present

  const std::string& staffSpacing =
    elt->getAttributeValue ("staff-spacing");

  if (staffSpacing.size ()) {
    std::stringstream ss;

    ss << staffSpacing;
    float value;
    ss >> value;

    fCurrentMusicXMLPrintLayout->setStaffSpacing (value);
  }

  // handle 'new-system' if present and relevant

  if (! gGlobalMxsr2msrOahGroup->getIgnoreMusicXMLLineBreaks ()) {
    const std::string& newSystem = elt->getAttributeValue ("new-system");

    if (newSystem.size ()) {
      fCurrentMusicXMLPrintLayout->setNewSystem ();

      if (newSystem == "yes") {
        // create a line break
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceLineBreaks ()) {
          std::stringstream ss;

          ss <<
            "Creating a line break, " <<
            "line: " << elt->getInputStartLineNumber ();

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
  #endif // MF_TRACE_IS_ENABLED

        S_msrLineBreak
          lineBreak =
            msrLineBreak::create (
              elt->getInputStartLineNumber (),
              1333333, // JMI v0.9.70 next purist number???
              msrUserSelectedLineBreakKind::kUserSelectedLineBreakNo);

        // append lineBreak to the pending line breaks
        fPendingLineBreaksList.push_back (lineBreak);
      }

      else if (newSystem == "no") {
        // ignore it
      }

      else {
        std::stringstream ss;

        ss << "new-system \"" << newSystem <<
        "\" is unknown in '<print />', should be 'yes', 'no' or empty";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // handle 'new-page' if present and relevant

  if (! gGlobalMxsr2msrOahGroup->getIgnoreMusicXMLPageBreaks ()) {
    const std::string& newPage = elt->getAttributeValue ("new-page");

    if (newPage.size ()) {
      fCurrentMusicXMLPrintLayout->setNewPage ();

      if (newPage == "yes") { // JMI
        // create a page break
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTracePageBreaks ()) {
          gLog <<
            "Creating a page break, " <<
            "line: " << elt->getInputStartLineNumber () <<
            std::endl;
        }
  #endif // MF_TRACE_IS_ENABLED

        S_msrPageBreak
          pageBreak =
            msrPageBreak::create (
              elt->getInputStartLineNumber (),
              gNullMeasure, // JMI ??? v0.9.70
              14444444, // JMI v0.9.70 next purist number???
              msrUserSelectedPageBreakKind::kUserSelectedPageBreakNo);

        // append it to the pending page breaks
        fPendingPageBreaksList.push_back (pageBreak);
       }

      else if (newPage == "no") {
        // ignore it
      }

      else {
        std::stringstream ss;

        ss << "new-page \"" << newPage <<
        "\" is unknown in '<print />', should be 'yes', 'no' or empty";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // handle 'blank-page' if present

  const int blankPage = elt->getAttributeIntValue ("blank-page", 0);

  if (blankPage > 0) {
    fCurrentMusicXMLPrintLayout->setBlankPage (blankPage);
  }

  // handle 'page-number' if present

  const int pageNumber = elt->getAttributeIntValue ("page-number", 0);

  if (pageNumber > 0) {
    fCurrentMusicXMLPrintLayout->setPageNumber (pageNumber);
  }

  fCurrentDisplayText = "";

  fOnGoingPrint = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_print& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_print" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // attach pending line breaks if any to the current part
  if (fPendingLineBreaksList.size ()) {
    attachPendingLineBreaksToPart (fCurrentPart);
  }

  // attach pending page breaks if any to the current part
  if (fPendingPageBreaksList.size ()) {
    attachPendingPageBreaksToPart (fCurrentPart);
  }

  // append the current print layout to voice 1 in staff 1 of the current part
  // it's not worth using specific 'layout voices' for such part-level stuff
  S_msrVoice
    voiceOneInStaffOne =
      fetchFirstVoiceFromCurrentPart (
        elt->getInputStartLineNumber ());

  voiceOneInStaffOne->
    appendMusicXMLPrintLayoutToVoice (
      fCurrentMusicXMLPrintLayout);

  // forget about the current print layout
  fCurrentMusicXMLPrintLayout = nullptr;

  fOnGoingPrint = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_measure_numbering& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_numbering" <<
       ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_barline& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_barline" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentBarLineEndingNumber    = ""; // may be "1, 2"

  fCurrentBarLineHasSegnoKind = msrBarLineHasSegnoKind::kBarLineHasSegnoNo;
  fCurrentBarLineHasCodaKind  = msrBarLineHasCodaKind::kBarLineHasCodaNo;

  fCurrentBarLineLocationKind        = msrBarLineLocationKind::kBarLineLocationNone;
  fCurrentBarLineStyleKind           = msrBarLineStyleKind::kBarLineStyleNone;
  fCurrentBarLineEndingTypeKind      = msrBarLineEndingTypeKind::kBarLineEndingTypeNone;
  fCurrentBarLineRepeatDirectionKind = msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone;
  fCurrentBarLineRepeatWingedKind    = msrBarLineRepeatWingedKind::kBarLineRepeatWingedNone;

  fCurrentBarLineTimes = 2; // default value JMI ??? v0.9.64

  // location

  {
    std::string
      location =
        elt->getAttributeValue ("location");

    fCurrentBarLineLocationKind =
      msrBarLineLocationKind::kBarLineLocationRight; // by default

    if       (location == "left") {
      fCurrentBarLineLocationKind = msrBarLineLocationKind::kBarLineLocationLeft;
    }
    else  if (location == "middle") {
      fCurrentBarLineLocationKind = msrBarLineLocationKind::kBarLineLocationMiddle;
    }
    else if  (location == "right") {
      fCurrentBarLineLocationKind = msrBarLineLocationKind::kBarLineLocationRight;
    }
    else {
      if (location.size () > 0) {
        std::stringstream ss;

        ss <<
          "barLine location \"" << location <<
          "\" is unknown, using 'right' by default";

     // JMI   mxsr2msrError (
        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
     //     __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  fOnGoingBarLine = true;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_bar_style& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bar_style" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // style

  std::string barStyle = elt->getValue();

  fCurrentBarLineStyleKind =
    msrBarLineStyleKind::kBarLineStyleNone; // default value

  if      (barStyle == "regular") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleRegular;
  }
  else if (barStyle == "dotted") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleDotted;
  }
  else if (barStyle == "dashed") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleDashed;
  }
  else if (barStyle == "heavy") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleHeavy;
  }
  else if (barStyle == "light-light") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleLightLight;
  }
  else if (barStyle == "light-heavy") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleLightHeavy;
  }
  else if (barStyle == "heavy-light") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleHeavyLight;
  }
  else if (barStyle == "heavy-heavy") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleHeavyHeavy;
  }
  else if (barStyle == "tick") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleTick;
  }
  else if (barStyle == "short") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleShort;
  }
  else if (barStyle == "none") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleNone;
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "bar-style \"" + barStyle + "\" is unknown");
  }

  // color JMI
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_segno& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_segno" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingDirectionType) {
    // create the segno
    S_msrSegno
      segno =
        msrSegno::create (
          elt->getInputStartLineNumber (),
          fCurrentDirectionStaffNumber);

    // append it to the pending segnos list
    fPendingSegnosList.push_back (segno);
  }

  else if (fOnGoingBarLine) {
    fCurrentBarLineHasSegnoKind =
      msrBarLineHasSegnoKind::kBarLineHasSegnoYes;
  }

  else {
    std::stringstream ss;

    ss << "<segno /> is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_coda& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_coda" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingDirectionType) {
    // account for this coda
    ++fCodasCounter;

    msrCodaKind codaKind = msrCodaKind::kCodaFirst;

    switch (fCodasCounter) {
      case 1:
        codaKind = msrCodaKind::kCodaFirst;
        break;
      case 2:
        codaKind = msrCodaKind::kCodaSecond;
        break;
      default:
        {
          std::stringstream ss;

          ss <<
            "this is <coda /> number " << fCodasCounter <<
            ", only two are meaningful";

          mxsr2msrError (
            gServiceRunData->getInputSourceName (),
            elt->getInputStartLineNumber (),
            __FILE__, __LINE__,
            ss.str ());
        }
    } // switch

    // create the coda
    S_msrCoda
      coda =
        msrCoda::create (
          elt->getInputStartLineNumber (),
          fCurrentDirectionStaffNumber,
          codaKind);

    // append it to the pending codas list
    fPendingCodasList.push_back (coda);
  }

  else if (fOnGoingBarLine) {
    fCurrentBarLineHasCodaKind =
      msrBarLineHasCodaKind::kBarLineHasCodaYes;
  }

  else {
    std::stringstream ss;

    ss << "<coda /> is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_eyeglasses& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_eyeglasses" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingDirectionType) {
    // create the eyeglasses
    S_msrEyeGlasses
      eyeGlasses =
        msrEyeGlasses::create (
          elt->getInputStartLineNumber ());

    // append it to the pending eyeglasses list
    fPendingEyeGlassesList.push_back (eyeGlasses);
  }

  else {
    std::stringstream ss;

    ss << "<eyeGlasses /> is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_pedal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pedal" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // type

  std::string type = elt->getAttributeValue ("type");

  msrPedalTypeKind pedalTypeKind = msrPedalTypeKind::kPedalType_UNKNOWN_;

  if       (type == "start") {
    pedalTypeKind = msrPedalTypeKind::kPedalTypeStart;
  }
  else  if (type == "continue") {
    pedalTypeKind = msrPedalTypeKind::kPedalTypeContinue;
  }
  else  if (type == "change") {
    pedalTypeKind = msrPedalTypeKind::kPedalTypeChange;
  }
  else  if (type == "stop") {
    pedalTypeKind = msrPedalTypeKind::kPedalTypeStop;
  }
  else {
    std::stringstream ss;

    ss << "pedal type " << type << " is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // line

  std::string line = elt->getAttributeValue ("line");

  msrPedalLineKind pedalLineKind = msrPedalLineKind::kPedalLineNo;

  if       (line == "yes") {
    pedalLineKind = msrPedalLineKind::kPedalLineYes;
  }
  else  if (line == "no") {
    pedalLineKind = msrPedalLineKind::kPedalLineNo;
  }
  else {
    if (line.size ()) {
      std::stringstream ss;

      ss <<
        "pedal line \"" << line <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // sign

  std::string sign = elt->getAttributeValue ("sign");

  msrPedalSignKind
    pedalSignKind =
      msrPedalSignKind::kPedalSignNo;

  if       (sign == "yes") {
    pedalSignKind = msrPedalSignKind::kPedalSignYes;
  }
  else  if (sign == "no") {
    pedalSignKind = msrPedalSignKind::kPedalSignNo;
  }
  else {
    if (sign.size ()) {
      std::stringstream ss;

      ss <<
        "pedal sign \"" << sign <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // create the pedal
  S_msrPedal
    pedal =
      msrPedal::create (
        elt->getInputStartLineNumber (),
        pedalTypeKind,
        pedalLineKind,
        pedalSignKind);

  if (fOnGoingDirectionType) {
    // append it to the pending pedals list
    fPendingPedalsList.push_back (pedal);
  }
  else {
    std::stringstream ss;

    ss << "<pedal /> " << pedal->asShortString () << " is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_ending& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ending" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  {
    fCurrentBarLineEndingNumber =
      elt->getAttributeValue ("number"); // may be "1, 2"

    if (! fCurrentBarLineEndingNumber.size ()) {
      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        "mandatory ending number is missing, assuming \"1\"");

      fCurrentBarLineEndingNumber = "1";
    }
  }

  // type

  {
    std::string type =
      elt->getAttributeValue ("type");

    fCurrentBarLineEndingTypeKind =
      msrBarLineEndingTypeKind::kBarLineEndingTypeNone;

    if       (type == "start") {
      fCurrentBarLineEndingTypeKind =
        msrBarLineEndingTypeKind::kBarLineEndingTypeStart;
    }
    else  if (type == "stop") {
      fCurrentBarLineEndingTypeKind =
        msrBarLineEndingTypeKind::kBarLineEndingTypeStop;
    }
    else  if (type == "discontinue") {
      fCurrentBarLineEndingTypeKind =
        msrBarLineEndingTypeKind::kBarLineEndingTypeDiscontinue;
    }
    else {
      std::stringstream ss;

      ss <<
        "ending type \"" << type <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_repeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_repeat" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // direction

  {
    std::string direction = elt->getAttributeValue ("direction");

    fCurrentBarLineRepeatDirectionKind =
      msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone;

    if       (direction == "forward") {
      fCurrentBarLineRepeatDirectionKind =
        msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionForward;
    }
    else  if (direction == "backward") {
      fCurrentBarLineRepeatDirectionKind =
        msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionBackward;
    }
    else {
      std::stringstream ss;

      ss <<
        "repeat direction \"" << direction <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // winged

  {
    std::string winged = elt->getAttributeValue ("winged");

    fCurrentBarLineRepeatWingedKind =
      msrBarLineRepeatWingedKind::kBarLineRepeatWingedNone; // default value

    if (winged.size ()) {
      if       (winged == "none") {
        fCurrentBarLineRepeatWingedKind =
          msrBarLineRepeatWingedKind::kBarLineRepeatWingedNone;
      }
      else if (winged == "straight") {
        fCurrentBarLineRepeatWingedKind =
          msrBarLineRepeatWingedKind::kBarLineRepeatWingedStraight;
      }
      else  if (winged == "curved") {
        fCurrentBarLineRepeatWingedKind =
          msrBarLineRepeatWingedKind::kBarLineRepeatWingedCurved;
      }
      else  if (winged == "double-straight") {
        fCurrentBarLineRepeatWingedKind =
          msrBarLineRepeatWingedKind::kBarLineRepeatWingedDoubleStraight;
      }
      else  if (winged == "double-curved") {
        fCurrentBarLineRepeatWingedKind =
          msrBarLineRepeatWingedKind::kBarLineRepeatWingedDoubleCurved;
      }
      else {
        std::stringstream ss;

        ss <<
          "repeat winged \"" << winged <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // time signatures

  {
    fCurrentBarLineTimes =
      elt->getAttributeIntValue ("times", 2); // default value JMI ???
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitEnd (S_barline& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_barline" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the barLine
  S_msrBarLine
    barLine =
      msrBarLine::create (
        elt->getInputStartLineNumber (),
        fCurrentBarLineLocationKind,
        fCurrentBarLineStyleKind,
        fCurrentBarLineRepeatDirectionKind,
        fCurrentBarLineEndingTypeKind,
        fCurrentBarLineEndingNumber,
        fCurrentBarLineTimes,
        msrBarLineCategoryKind::kBarLineCategory_UNKNOWN_, // will be set afterwards
        fCurrentBarLineHasSegnoKind,
        fCurrentBarLineHasCodaKind,
        fCurrentBarLineRepeatWingedKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Creating barLine " <<
      barLine->asString () <<
      " in part " <<
      fCurrentPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // wait until its category is defined
  // to append the barLine to the current segment

  // handle the barLine according to: JMI
  // http://www.musicxml.com/tutorial/the-midi-compatible-part/repeats/

  Bool barLineHasBeenHandled (false);

  switch (fCurrentBarLineLocationKind) {
    case msrBarLineLocationKind::kBarLineLocationNone:
      // should not occur
      break;

    case msrBarLineLocationKind::kBarLineLocationLeft:
    // ---------------------------------------------------------
      if (
        fCurrentBarLineEndingTypeKind
          ==
        msrBarLineEndingTypeKind::kBarLineEndingTypeStart
      ) {
        // ending start, don't know yet whether it's hooked or hookless
        // ------------------------------------------------------
        if (! fCurrentBarLineEndingNumber.size ()) {
          mxsr2msrWarning (
            gServiceRunData->getInputSourceName (),
            elt->getInputStartLineNumber (),
            "mandatory ending number is missing, assuming \"1\"");

          fCurrentBarLineEndingNumber = "1";
        }

        // don't know yet whether repeat ending start barLine is hooked or hookless
        // remember it in fCurrentRepeatEndingStartBarLine,
        fCurrentRepeatEndingStartBarLine = barLine;

        // handle the repeat ending start
        handleRepeatEndingStart (barLine);

        barLineHasBeenHandled = true;
      }

      else if (
        fCurrentBarLineRepeatDirectionKind
          ==
        msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionForward
      ) {
        // repeat start
        // ------------------------------------------------------
        // set the barLine category
        barLine->
          setBarLineCategory (
            msrBarLineCategoryKind::kBarLineCategoryRepeatStart);

        // handle the repeat start
        handleRepeatStart (barLine);

        barLineHasBeenHandled = true;
      }

      else {
        // ---------------------------------------------------------
        std::stringstream ss;

        ss <<
          "cannot handle barLine of type kBarLineLocationLeft " <<
          barLine->asString () <<
          ", fCurrentBarLineLocationKind: " << fCurrentBarLineLocationKind <<
          ", fCurrentBarLineStyleKind: " << fCurrentBarLineStyleKind <<
          ", fCurrentBarLineEndingTypeKind: " << fCurrentBarLineEndingTypeKind <<
          ", fCurrentBarLineRepeatDirectionKind: " << fCurrentBarLineRepeatDirectionKind <<
          ", fCurrentBarLineRepeatWingedKind: " << fCurrentBarLineRepeatWingedKind;

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
      break;

    case msrBarLineLocationKind::kBarLineLocationMiddle:
    // ---------------------------------------------------------
      {
        // JMI ??? v0.9.71
        std::stringstream ss;

        ss <<
          "cannot handle barLine of type kBarLineLocationMiddle " <<
          barLine->asString () <<
          ", fCurrentBarLineLocationKind: " << fCurrentBarLineLocationKind <<
          ", fCurrentBarLineStyleKind: " << fCurrentBarLineStyleKind <<
          ", fCurrentBarLineEndingTypeKind: " << fCurrentBarLineEndingTypeKind <<
          ", fCurrentBarLineRepeatDirectionKind: " << fCurrentBarLineRepeatDirectionKind <<
          ", fCurrentBarLineRepeatWingedKind: " << fCurrentBarLineRepeatWingedKind;

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
      break;

    case msrBarLineLocationKind::kBarLineLocationRight:
    // ---------------------------------------------------------
      if (
        fCurrentBarLineEndingTypeKind == msrBarLineEndingTypeKind::kBarLineEndingTypeStop
          &&
        fCurrentBarLineEndingNumber.size () != 0
      ) {
        // hooked ending end
        // ------------------------------------------------------
        // set current barLine ending start category
        fCurrentRepeatEndingStartBarLine->
          setBarLineCategory (
            msrBarLineCategoryKind::kBarLineCategoryHookedEndingStart);

        // set this barLine's category
        barLine->
          setBarLineCategory (
            msrBarLineCategoryKind::kBarLineCategoryHookedEndingEnd);

        // handle the repeat hooked ending end
        handleRepeatHookedEndingEnd (barLine);

        barLineHasBeenHandled = true;
      }

      else if (
        fCurrentBarLineRepeatDirectionKind
          ==
        msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionBackward
      ) {
        // repeat end
        // ------------------------------------------------------

        // set this barLine's category
        barLine->
          setBarLineCategory (
            msrBarLineCategoryKind::kBarLineCategoryRepeatEnd);

        // handle the repeat end
        handleRepeatEnd (barLine);

        barLineHasBeenHandled = true;
      }

      else if (
        fCurrentBarLineEndingTypeKind == msrBarLineEndingTypeKind::kBarLineEndingTypeDiscontinue
          &&
        fCurrentBarLineEndingNumber.size () != 0
      ) {
        // hookless ending end
        // ------------------------------------------------------
        // set current barLine ending start category
        fCurrentRepeatEndingStartBarLine->
          setBarLineCategory (
            msrBarLineCategoryKind::kBarLineCategoryHooklessEndingStart);

        // set this barLine's category
        barLine->
          setBarLineCategory (
            msrBarLineCategoryKind::kBarLineCategoryHooklessEndingEnd);

        // handle the repeat hookless ending end
        handleRepeatHooklessEndingEnd (barLine);

        barLineHasBeenHandled = true;
      }

      else if (fCurrentBarLineStyleKind == msrBarLineStyleKind::kBarLineStyleLightLight) {
        // double barline JMI v0.9.71
        // set this barLine's category
        barLine->
          setBarLineCategory (
            msrBarLineCategoryKind::kBarLineCategoryStandalone);

        // handle the final bar line
        fPendingBarLinesList.push_back (barLine);

        barLineHasBeenHandled = true;
      }

      else if (fCurrentBarLineStyleKind == msrBarLineStyleKind::kBarLineStyleLightHeavy) {
        // final barline JMI v0.9.71
        // set this barLine's category
        barLine->
          setBarLineCategory (
            msrBarLineCategoryKind::kBarLineCategoryStandalone);

        // handle the final bar line
        fPendingBarLinesList.push_back (barLine);

        barLineHasBeenHandled = true;
      }

      else {
        // ---------------------------------------------------------
        std::stringstream ss;

        ss <<
          "cannot handle barLine of type kBarLineLocationRight " <<
          barLine->asString () <<
          ", fCurrentBarLineLocationKind: " << fCurrentBarLineLocationKind <<
          ", fCurrentBarLineStyleKind: " << fCurrentBarLineStyleKind <<
          ", fCurrentBarLineEndingTypeKind: " << fCurrentBarLineEndingTypeKind <<
          ", fCurrentBarLineRepeatDirectionKind: " << fCurrentBarLineRepeatDirectionKind <<
          ", fCurrentBarLineRepeatWingedKind: " << fCurrentBarLineRepeatWingedKind;

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }

      // forget about current repeat ending start barLine
      fCurrentRepeatEndingStartBarLine = nullptr;

      break;
  } // switch

  // set the barLine category to stand alone if not yet handled
  if (! barLineHasBeenHandled) {
    switch (fCurrentBarLineStyleKind) {
      case msrBarLineStyleKind::kBarLineStyleRegular:
      case msrBarLineStyleKind::kBarLineStyleDotted:
      case msrBarLineStyleKind::kBarLineStyleDashed:
      case msrBarLineStyleKind::kBarLineStyleHeavy:
      case msrBarLineStyleKind::kBarLineStyleLightLight:
      case msrBarLineStyleKind::kBarLineStyleLightHeavy:
      case msrBarLineStyleKind::kBarLineStyleHeavyLight:
      case msrBarLineStyleKind::kBarLineStyleHeavyHeavy:
      case msrBarLineStyleKind::kBarLineStyleTick:
      case msrBarLineStyleKind::kBarLineStyleShort:
        barLine->
          setBarLineCategory (
            msrBarLineCategoryKind::kBarLineCategoryStandalone);

        // append the bar line to the current part
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceBarLines ()) {
          std::stringstream ss;

          ss <<
            "Appending a standalone barLine to part " <<
            fCurrentPart->fetchPartCombinedName () << ":" <<
            std::endl;

          ++gIndenter;
          ss << barLine;
          --gIndenter;
        }
  #endif // MF_TRACE_IS_ENABLED

//         fCurrentPart->
//           appendBarLineToPart (barLine);
        fPendingBarLinesList.push_back (barLine); // JMIJMIJMI

        barLineHasBeenHandled = true;
        break;

      case msrBarLineStyleKind::kBarLineStyleNone:
        std::stringstream ss;

        ss <<
          "barLine " <<
          barLine->asString () <<
          " has no barLine style";

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
     //     __FILE__, __LINE__,
          ss.str ());
        break;
    } // switch
  }

  // has this barLine been handled?
  if (! barLineHasBeenHandled) {
    std::stringstream ss;

    ss << std::left <<
      "cannot handle a barLine containing: " <<
      barLine->asString ();

//     mxsr2msrInternalWarning (
    mxsr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

/* JMI
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  switch (barLine->getBarLineCategory ()) {
    case msrBarLineCategoryKind::kBarLineCategory_UNKNOWN_:
      {
        std::stringstream ss;

        ss <<
          "barLine " <<
          barLine->asString () <<
          " has no barLine category" <<
          ", line " << elt->getInputStartLineNumber ();

      mxsr2msrInternalError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
      }
      break;
    case msrBarLineCategoryKind::kBarLineCategoryStandalone:
    case msrBarLineCategoryKind::kBarLineCategoryRepeatStart:
    case msrBarLineCategoryKind::kBarLineCategoryRepeatEnd:
    case msrBarLineCategoryKind::kBarLineCategoryHookedEndingStart:
    case msrBarLineCategoryKind::kBarLineCategoryHookedEndingEnd:
    case msrBarLineCategoryKind::kBarLineCategoryHooklessEndingStart:
    case msrBarLineCategoryKind::kBarLineCategoryHooklessEndingEnd:
      break;
  } // switch
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  fOnGoingBarLine = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

	++fCurrentNoteSequentialNumber;

  fCurrentNoteInputStartLineNumber =
    elt->getInputStartLineNumber ();

  // initialize note data to a neutral state
  initializeNoteData ();

  // harmonies

  fCurrentHarmonyWholeNotesOffset = msrWholeNotes (0, 1);

  // lyrics

  fCurrentStanzaNumber = K_STANZA_NUMBER_UNKNOWN_;
  fCurrentStanzaName = K_STANZA_NAME_UNKNOWN_;

  fCurrentSyllabic = "";
  // don't forget about fCurrentSyllableElementsList here,
  // this will be done in visitStart (S_syllabic& )
  fCurrentSyllableKind = msrSyllableKind::kSyllableNone;

  // stems

  fCurrentStem = nullptr;

  // tremolos

  fCurrentDoubleTremoloTypeKind = msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_;

  // ties

  fCurrentTiedOrientation = "";

  // slurs

  fCurrentSlurType = "";
  fCurrentSlurTypeKind = msrSlurTypeKind::kSlurType_UNKNOWN_;

  // ligatures

  fCurrentLigatureKind = msrLigatureKind::kLigatureNone;

  // print-object

  std::string printObjectString =
    elt->getAttributeValue ("print-object");

  fCurrentNotePrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

  // note color, unofficial ??? JMI v0.9.70

  std::string noteColorAlphaRGB = elt->getAttributeValue ("color");

  fCurrentNoteRGB   = "";
  fCurrentNoteAlpha = "";

  Bool wellFormedColor (true);

  size_t noteColorAlphaRGBSize = noteColorAlphaRGB.size ();

  if (noteColorAlphaRGBSize) {
    if (noteColorAlphaRGB [0] != '#') {
      wellFormedColor = false;
    }
    else {
      size_t
        found =
          noteColorAlphaRGB.find_first_not_of ("#0123456789ABCDEF");

      if (found != std::string::npos) {
        wellFormedColor = false;
      }
      else {
        switch (noteColorAlphaRGBSize) {
          case 7: // RGB
            fCurrentNoteAlpha = "FF";
            fCurrentNoteRGB   = noteColorAlphaRGB.substr (1, 6);
            break;
          case 9: // ARGB
            fCurrentNoteAlpha = noteColorAlphaRGB.substr (1, 2);
            fCurrentNoteRGB   = noteColorAlphaRGB.substr (3, 8);
            break;
          default:
            wellFormedColor = false;
        } // switch
      }
    }
  }

  if (! wellFormedColor) {
    std::stringstream ss;

    ss <<
      "note color \"" << noteColorAlphaRGB <<
      "\" should contain 6 or 8 upper case hexadecimal digits prededed by a '#'";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  fOnGoingNote = true;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_step& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_step" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string step = elt->getValue();

  checkStep (
    elt->getInputStartLineNumber (),
    step,
    "<step/>");

  fCurrentNoteDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (step [0]);
}

void mxsr2msrSkeletonPopulator::visitStart (S_alter& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_alter" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  float alter = (float)(*elt);

  fCurrentNoteAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      alter);

  if (fCurrentNoteAlterationKind == msrAlterationKind::kAlteration_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "alter " << alter <<
      " should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_octave& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_octave" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int octaveNumber = (int)(*elt);

  if (octaveNumber < 0 || octaveNumber > 9) {
    std::stringstream ss;

    ss <<
      "octave number " << octaveNumber <<
      " is not in the 0..9 range, '0' is assumed";

    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());

    octaveNumber = 0;
  }

  fCurrentNoteOctave =
    msrOctaveKindFromNumber (
      elt->getInputStartLineNumber (),
      octaveNumber);
}

void mxsr2msrSkeletonPopulator::visitStart (S_duration& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_duration" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int duration = (int)(*elt); // divisions

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDurations ()) {
    std::stringstream ss;

    ss <<
      "duration: " << duration <<
      ", fOnGoingBackup: " << fOnGoingBackup <<
      ", fOnGoingForward: " << fOnGoingForward <<
      ", fOnGoingNote: " << fOnGoingNote <<
      ", fOnGoingFiguredBass: " << fOnGoingFiguredBass <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingBackup) {
    fCurrentBackupDivisions = duration;
  }

  else if (fOnGoingForward) {
    fCurrentForwardDivisions = duration;
  }

  else if (fOnGoingNote) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotesDetails ()) {
      std::stringstream ss;

      ss <<
        "fCurrentDivisionsPerQuarterNote: " <<
        fCurrentDivisionsPerQuarterNote;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // set current grace note whole notes
    fCurrentNoteSoundingWholeNotesFromNotesDuration =
      msrWholeNotes (
        duration,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotesDetails ()) {
      std::stringstream ss;

      ss <<
        "fCurrentNoteSoundingWholeNotesFromNotesDuration: " <<
        fCurrentNoteSoundingWholeNotesFromNotesDuration;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

/* JMI
    // set current grace note display whole notes
    // to note sounding whole notes
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteSoundingWholeNotesFromNotesDuration; // by default
     */
  }

  else if (fOnGoingFiguredBass) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceFiguredBasses ()) {
      std::stringstream ss;

      ss <<
        "fCurrentDivisionsPerQuarterNote: " <<
        fCurrentDivisionsPerQuarterNote;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // set current figured bass whole notes duration
    fCurrentFiguredBassSoundingWholeNotes =
      msrWholeNotes (
        duration,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceFiguredBasses ()) {
      std::stringstream ss;

      ss <<
        "fCurrentFiguredBassSoundingWholeNotes: " <<
        fCurrentFiguredBassSoundingWholeNotes;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss << "<duration /> " << duration << " is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

//  gLog << "=== mxsr2msrSkeletonPopulator::visitStart (S_duration& elt), fCurrentNotesDuration: " << fCurrentNotesDuration << std::endl; JMI
}

void mxsr2msrSkeletonPopulator::visitStart (S_instrument& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
<!--
  If multiple score-instruments are specified in a
  score-part, there should be an instrument element for
  each note in the part. The id attribute is an IDREF back
  to the score-instrument ID. Notes that are shared between
  multiple score-instruments can have more than one instrument
  element.
-->
<!ELEMENT instrument EMPTY>
<!ATTLIST instrument
    id IDREF #REQUIRED
>

      <note default-x="99.80" default-y="-10.00">
        <unpitched>
          <display-step>E</display-step>
          <display-octave>4</display-octave>
          </unpitched>
        <duration>6</duration>
        <instrument id="P1-I38"/>
        <voice>1</voice>
        <type>quarter</type>
        <stem>down</stem>
        <notehead>x</notehead>
        </note>
*/

//  int instruments = (int)(*elt); // JMI
}

void mxsr2msrSkeletonPopulator::visitStart (S_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_dot" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentNoteDotsNumber;
}

void mxsr2msrSkeletonPopulator::visitStart (S_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
 Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.
*/

  {
    std::string noteType = elt->getValue();

    // the type contains a display duration,
    fCurrentNoteGraphicNotesDurationKind =
      msrNotesDurationKindFromMusicXMLString (
        elt->getInputStartLineNumber (),
        noteType);
  }

  // size

  {
    std::string noteTypeSize = elt->getAttributeValue ("size");

    if (noteTypeSize == "full") {
      // a regular note
    }
    else if (noteTypeSize == "cue") { // USE IT! JMI ???
    }

    else {
      if (noteTypeSize.size ()) {
        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
            "note type size \"" + noteTypeSize + "\" is unknown");
      }
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    /* JMI
    gLog <<
      "noteType: \"" <<
      noteType <<
      "\"" <<
      std::endl <<
      "noteTypeSize: \"" <<
      noteTypeSize <<
      "\"" <<
      std::endl;
        */
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_notehead& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_notehead" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  {
    std::string noteHead = elt->getValue();

    if      (noteHead == "slash") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadSlash; }
    else if (noteHead == "triangle") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadTriangle; }
    else if (noteHead == "diamond")   {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadDiamond; }
    else if (noteHead == "square") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadSquare; }
    else if (noteHead == "cross") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadCross; }
    else if (noteHead == "x") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadX; }
    else if (noteHead == "circle-x") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadCircleX; }
    else if (noteHead == "inverted triangle") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadInvertedTriangle; }
    else if (noteHead == "arrow down") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadArrowDown; }
    else if (noteHead == "arrow up") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadArrowUp; }
    else if (noteHead == "slashed") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadSlashed; }
    else if (noteHead == "back slashed") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadBackSlashed; }
    else if (noteHead == "normal") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadNormal; }
    else if (noteHead == "cluster") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadCluster; }
    else if (noteHead == "circle dot") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadCircleDot; }
    else if (noteHead == "left triangle") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadLeftTriangle; }
    else if (noteHead == "rectangle") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadRectangle; }
    else if (noteHead == "none") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadNone; }
    else if (noteHead == "do") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadDo; }
    else if (noteHead == "re") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadRe; }
    else if (noteHead == "mi") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadMi; }
    else if (noteHead == "fa") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadFa; }
    else if (noteHead == "fa up") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadFaUp; }
    else if (noteHead == "so") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadSo; }
    else if (noteHead == "la") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadLa; }
    else if (noteHead == "ti") {
      fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadTi; }
    else {
      std::stringstream ss;

      ss <<
        "note head \"" << noteHead <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // filled

  {
    std::string noteHeadFilled = elt->getAttributeValue ("filled");

    if      (noteHeadFilled == "yes")
      fCurrentNoteHeadFilledKind = msrNoteHeadFilledKind::kNoteHeadFilledYes;
    else if (noteHeadFilled == "no")
      fCurrentNoteHeadFilledKind = msrNoteHeadFilledKind::kNoteHeadFilledNo;
    else {
      if (noteHeadFilled.size ()) {
        std::stringstream ss;

        ss <<
          "note head filled \"" << noteHeadFilled <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // parentheses

  {
    std::string noteHeadParentheses = elt->getAttributeValue ("parentheses");

    if      (noteHeadParentheses == "yes")
      fCurrentNoteHeadParenthesesKind = msrNoteHeadParenthesesKind::kNoteHeadParenthesesYes;
    else if (noteHeadParentheses == "no")
      fCurrentNoteHeadParenthesesKind = msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo;
    else {
      if (noteHeadParentheses.size ()) {
        std::stringstream ss;

        ss <<
          "note head parentheses \"" << noteHeadParentheses <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // color JMI v0.9.71
}

void mxsr2msrSkeletonPopulator::visitStart (S_accidental& elt) // JMI
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accidental" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // value

  {
    std::string accidentalValue = elt->getValue ();

    fCurrentAccidentalKind = msrAccidentalKind::kAccidentalNone;

    if      (accidentalValue == "sharp")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSharp;
    else if (accidentalValue == "natural")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalNatural;
    else if (accidentalValue == "flat")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalFlat;
    else if (accidentalValue == "double-sharp")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalDoubleSharp;
    else if (accidentalValue == "sharp-sharp")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSharpSharp;
    else if (accidentalValue == "flat-flat")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalFlatFlat;
    else if (accidentalValue == "natural-sharp")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalNaturalSharp;
    else if (accidentalValue == "natural-flat")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalNaturalFlat;
    else if (accidentalValue == "quarter-flat")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalQuarterFlat;
    else if (accidentalValue == "quarter-sharp")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalQuarterSharp;
    else if (accidentalValue == "three-quarters-flat")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalThreeQuartersFlat;
    else if (accidentalValue == "three-quarters-sharp")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalThreeQuartersSharp;

    else if (accidentalValue == "sharp-down")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSharpDown;
    else if (accidentalValue == "sharp-up")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSharpUp;
    else if (accidentalValue == "natural-down")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalNaturalDown;
    else if (accidentalValue == "natural-up")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalNaturalUp;
    else if (accidentalValue == "flat-down")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalFlatDown;
    else if (accidentalValue == "flat-up")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalFlatUp;
    else if (accidentalValue == "triple-sharp")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalTripleSharp;
    else if (accidentalValue == "triple-flat")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalTripleFlat;
    else if (accidentalValue == "slash-quarter-sharp")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSlashQuarterSharp;
    else if (accidentalValue == "slash-sharp")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSlashSharp;
    else if (accidentalValue == "slash-flat")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSlashFlat;
    else if (accidentalValue == "double-slash-flat")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalDoubleSlashFlat;
    else if (accidentalValue == "sharp-1")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSharp_1;
    else if (accidentalValue == "sharp-2")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSharp_2;
    else if (accidentalValue == "sharp-3")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSharp_3;
    else if (accidentalValue == "sharp-5")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSharp_5;
    else if (accidentalValue == "flat-1")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalFlat_1;
    else if (accidentalValue == "flat-2")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalFlat_2;
    else if (accidentalValue == "flat-3")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalFlat_3;
    else if (accidentalValue == "flat-4")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalFlat_4;
    else if (accidentalValue == "sori")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalSori;
    else if (accidentalValue == "koron")
      fCurrentAccidentalKind = msrAccidentalKind::kAccidentalKoron;
    else {
      if (accidentalValue.size ()) {
        std::stringstream ss;

        ss <<
          "accidental \"" << accidentalValue <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // editorial

  {
    std::string editorialAccidental = elt->getAttributeValue ("editorial");

    fCurrentEditorialAccidentalKind =
      msrEditorialAccidentalKind::kEditorialAccidentalNo; // default value

    if      (editorialAccidental == "yes")
      fCurrentEditorialAccidentalKind =
        msrEditorialAccidentalKind::kEditorialAccidentalYes;
    else if (editorialAccidental == "no")
      fCurrentEditorialAccidentalKind =
        msrEditorialAccidentalKind::kEditorialAccidentalNo;
    else {
      if (editorialAccidental.size ()) {
        std::stringstream ss;

        ss <<
          "editorial accidental \"" << editorialAccidental <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // cautionary

  {
    std::string cautionaryAccidental = elt->getAttributeValue ("cautionary");

    fCurrentCautionaryAccidentalKind =
      msrCautionaryAccidentalKind::kCautionaryAccidentalNo; // default value

    if      (cautionaryAccidental == "yes")
      fCurrentCautionaryAccidentalKind =
        msrCautionaryAccidentalKind::kCautionaryAccidentalYes;
    else if (cautionaryAccidental == "no")
      fCurrentCautionaryAccidentalKind =
        msrCautionaryAccidentalKind::kCautionaryAccidentalNo;
    else {
      if (cautionaryAccidental.size ()) {
        std::stringstream ss;

        ss <<
          "cautionary accidental \"" << cautionaryAccidental <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_stem& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_stem" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
<!--
  Stems can be down, up, none, or double. For down and up
  stems, the position attributes can be used to specify
  stem length. The relative values specify the end of the
  stem relative to the program default. Default values
  specify an absolute end stem position. Negative values of
  relative-y that would flip a stem instead of shortening
  it are ignored. A stem element associated with a rest
  refers to a stemlet.
-->
<!ELEMENT stem (#PCDATA)>
<!ATTLIST stem
    %position;
    %color;
>
*/

  std::string stem = elt->getValue();

  // kind
  msrStemKind stemKind = msrStemKind::kStemKind_NONE; // default value JMI v0.9.70

  if      (stem == "up")
    stemKind = msrStemKind::kStemKindUp;

  else if (stem == "down")
    stemKind = msrStemKind::kStemKindDown;

  else if (stem == "none")
    stemKind = msrStemKind::kStemKindNeutral;

  else if (stem == "double")
    stemKind = msrStemKind::kStemKindDouble;

  else {
    std::stringstream ss;

    ss <<
      "stem \"" << stem <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // color JMI

  fCurrentStem =
    msrStem::create (
      elt->getInputStartLineNumber (),
      stemKind);
}

void mxsr2msrSkeletonPopulator::visitStart (S_beam& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beam" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // value

  fCurrentBeamValue = elt->getValue();

  msrBeamKind beamKind = msrBeamKind::kBeam_UNKNOWN_;

  if      (fCurrentBeamValue == "begin") {
    beamKind = msrBeamKind::kBeamBegin;
  }
  else if (fCurrentBeamValue == "continue") {
    beamKind = msrBeamKind::kBeamContinue;
  }
  else if (fCurrentBeamValue == "end") {
    beamKind = msrBeamKind::kBeamEnd;
  }
  else if (fCurrentBeamValue == "forward hook") {
    beamKind = msrBeamKind::kBeamForwardHook;
  }
  else if (fCurrentBeamValue == "backward hook") {
    beamKind = msrBeamKind::kBeamBackwardHook;
  }
  else {
    std::stringstream ss;

    ss <<
      "beam \"" << fCurrentBeamValue <<
      "\"" << "is not known";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // number

  fCurrentBeamNumber =
    elt->getAttributeIntValue ("number", 1); // default value

  S_msrBeam
    beam =
      msrBeam::create (
        elt->getInputStartLineNumber (),
        fCurrentBeamNumber,
        beamKind);

  // color JMI ???

  fPendingBeamsList.push_back (beam);

  // is the current note a grace note?
  if (fCurrentNoteIsAGraceNote) {
    fCurrentGraceNotesGroupIsBeamed = true;
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_measure_style& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_style" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // color JMI
}

void mxsr2msrSkeletonPopulator::visitStart (S_beat_repeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beat_repeat" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
<!--
  The beat-repeat element is used to indicate that a single
  beat (but possibly many notes) is repeated. Both the start
  and stop of the beat being repeated should be specified.
  The slashes attribute specifies the number of slashes to
  use in the symbol. The use-dots attribute indicates whether
  or not to use dots as well (for instance, with mixed rhythm
  patterns). By default, the value for slashes is 1 and the
  value for use-dots is no.
-->
<!ELEMENT beat-repeat ((slash-type, slash-dot*)?, except-voice*)>
<!ATTLIST beat-repeat
    type %start-stop; #REQUIRED
    slashes NMTOKEN #IMPLIED
    use-dots %yes-no; #IMPLIED
>
*/

  // slashes

  fCurrentBeatRepeatSlashes = elt->getAttributeIntValue ("slashes", 0);

  // use-dots

  std::string useDotsString = elt->getAttributeValue ("use-dots");

  fCurrentUseDotsKind =
    msrUseDotsFromString (
      elt->getInputStartLineNumber (),
      useDotsString);
}

void mxsr2msrSkeletonPopulator::visitStart (S_measure_repeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_repeat" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
  The multiple-rest and measure-repeat symbols indicate the
  number of measures covered in the element content. The
  beat-repeat and slash elements can cover partial measures.
  All but the multiple-rest element use a type attribute to
  indicate starting and stopping the use of the style. The
  optional number attribute specifies the staff number from
  top to bottom on the system, as with clef.
*/

  fCurrentMeasureRepeatMeasuresNumber = (int)(*elt);

  // slashes

  fCurrentMeasureRepeatSlashesNumber =
    elt->getAttributeIntValue ("slashes", 1); // default value

  // type

  std::string measureRepeatType =
    elt->getAttributeValue ("type");

  fCurrentMeasureRepeatKind =
    msrMeasureRepeatKind::kMeasureRepeat_UNKNOWN_;

  if      (measureRepeatType == "start") {
    fCurrentMeasureRepeatKind =
      msrMeasureRepeatKind::kMeasureRepeatStart; // JMI

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating measures repeat from its first measures" <<
      "in part " <<
      fCurrentPart->fetchPartCombinedName () <<
      ", fCurrentMeasureRepeatMeasuresNumber: " <<
      fCurrentMeasureRepeatMeasuresNumber <<
      ", fCurrentMeasureRepeatSlashesNumber: " <<
      fCurrentMeasureRepeatSlashesNumber <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    fCurrentPart->
      createMeasureRepeatFromItsFirstMeasuresInPart (
        elt->getInputStartLineNumber (),
        fCurrentMeasureRepeatMeasuresNumber,
        fCurrentMeasureRepeatSlashesNumber);
  }

  else if (measureRepeatType == "stop") {
    fCurrentMeasureRepeatKind =
      msrMeasureRepeatKind::kMeasureRepeatStop; // JMI

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat " <<
      "to part " <<
      fCurrentPart->fetchPartCombinedName () <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    fCurrentPart->
      appendPendingMeasureRepeatToPart (
        elt->getInputStartLineNumber ());
  }

  else {
    std::stringstream ss;

    ss <<
      "measure-repeat type \"" << measureRepeatType <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_multiple_rest& elt)
{
/*
<!--
  A measure-style indicates a special way to print partial
  to multi-measures within a part. This includes multiple
  rests over several measures, repeats of beats, single, or
  multi-measures, and use of slash notation.

  The multiple-rest and measure-repeat elements indicate the
  number of measures covered in the element content. The
  beat-repeat and slash elements can cover partial measures.
  All but the multiple-rest element use a type attribute to
  indicate starting and stopping the use of the style. The
  optional number attribute specifies the staff number from
  top to bottom on the system, as with clef.
-->
<!ELEMENT measure-style (multiple-rest |
  measure-repeat | beat-repeat | slash)>
<!ATTLIST measure-style
    number CDATA #IMPLIED
    %font;
    %color;
    %optional-unique-id;
>

<!--
  The text of the multiple-rest element indicates the number
  of measures in the multiple rest. Multiple rests may use
  the 1-bar / 2-bar / 4-bar rest symbols, or a single shape.
  The use-symbols attribute indicates which to use; it is no
  if not specified.
-->
<!ELEMENT multiple-rest (#PCDATA)>
<!ATTLIST multiple-rest
    use-symbols %yes-no; #IMPLIED
>

    <measure number="1" width="298">
      <direction placement="above">
        <direction-type>
          <words default-y="28" font-size="10.8" font-weight="bold" relative-x="-31">Adagio non troppo</words>
        </direction-type>
      </direction>
      <attributes>
        <measure-style>
          <multiple-rest>2</multiple-rest>
        </measure-style>
      </attributes>
      <note>
        <rest measure="yes"/>
        <duration>96</duration>
        <voice>1</voice>
      </note>
    </measure>
    <!--=======================================================-->
    <measure number="2" width="0">
      <note>
        <rest measure="yes"/>
        <duration>96</duration>
        <voice>1</voice>
      </note>
      <!--barline location="right">
        <bar-style>light-light</bar-style>
      </barline-->
    </measure>
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_multiple_rest" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMeasureRestsNumber = (int)(*elt);

  std::string useSymbols = elt->getAttributeValue ("use-symbols");

  // what do we do with fMultiMeasureRestsUseSymbols ??? JMI v0.9.63
  if      (useSymbols == "yes") {
    fMultiMeasureRestsUseSymbols = true;
  }
  else if (useSymbols == "no") {
    fMultiMeasureRestsUseSymbols = false;
  }
  else {
    if (useSymbols.size ()) {
      std::stringstream ss;

      ss <<
        "multiple rest use symbols " <<
        useSymbols <<
        " is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
      }
  }

  // create a multi-measure rests
  fCurrentPart->
    appendMultiMeasureRestToPart (
      elt->getInputStartLineNumber (),
      fCurrentMeasureRestsNumber);

  // set remaining multi-measure rests counter
  fRemainingMeasureRestsCounter =
    fCurrentMeasureRestsNumber;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_multiple_rest& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_multiple_rest" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_slash& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slash" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // type

  std::string slashType = elt->getAttributeValue ("type");

  if      (slashType == "start")
    fCurrentSlashTypeKind = msrSlashTypeKind::kSlashTypeStart;
  else if (slashType == "stop")
    fCurrentSlashTypeKind = msrSlashTypeKind::kSlashTypeStop;
  else {
    std::stringstream ss;

    ss <<
      "slash type \"" << slashType <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // use-dots

  std::string useDotsString = elt->getAttributeValue ("use-dots");

  fCurrentUseDotsKind =
    msrUseDotsFromString (
      elt->getInputStartLineNumber (),
      useDotsString);

  // use-stems

  std::string slashUseStems = elt->getAttributeValue ("use-stems");

  if      (slashUseStems == "yes")
    fCurrentSlashUseStemsKind = msrSlashUseStemsKind::kSlashUseStemsYes;
  else if (slashUseStems == "no")
    fCurrentSlashUseStemsKind = msrSlashUseStemsKind::kSlashUseStemsNo;
  else {
    if (slashUseStems.size ()) {
      std::stringstream ss;

      ss <<
        "slash use-stems \"" << slashUseStems <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  fCurrentSlashDotsNumber = 0;
}

void mxsr2msrSkeletonPopulator::visitStart (S_slash_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slash_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string slashType = elt->getValue();

  // the type contains a display duration,
  fCurrentSlashGraphicNotesDurationKind =
    msrNotesDurationKindFromMusicXMLString (
      elt->getInputStartLineNumber (),
      slashType);

  // size

  std::string slashTypeSize = elt->getAttributeValue ("size");

  if (slashTypeSize == "cue") { // USE IT! JMI ??? v0.9.72
  }

  else {
    if (slashTypeSize.size ()) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
          "slash type size \"" + slashTypeSize + "\" is unknown");
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlashes ()) {
    std::stringstream ss;

    ss <<
      "slashType: \"" <<
      slashType <<
      "\"" <<
      std::endl <<
      "slashTypeSize: \"" <<
      slashTypeSize <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_slash_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slash_dot" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentSlashDotsNumber;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_slash& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_slash" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrSlash
    slash =
      msrSlash::create (
        elt->getInputStartLineNumber (),
        fCurrentSlashTypeKind,
        fCurrentUseDotsKind,
        fCurrentSlashUseStemsKind);

  fPendingSlashesList.push_back (slash);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_articulations& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_articulations" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_accent& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accent" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationAccent,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_breath_mark& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_breath_mark" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationBreathMark,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_caesura& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_caesura" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationCaesura,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_spiccato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_spiccato" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationSpiccato,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_staccato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staccato" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationStaccato,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_staccatissimo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staccatissimo" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationStaccatissimo,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_stress& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_stress" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationStress,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_unstress& elt)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_unstress" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationUnstress,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_detached_legato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_detached_legato" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationDetachedLegato,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_strong_accent& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_strong_accent" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  // type : upright inverted  (Binchois20.xml) // JMI
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationStrongAccent,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_tenuto& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tenuto" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // type : upright inverted  (Binchois20.xml) // JMI ???

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationTenuto,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_doit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_doit" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationDoit,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_falloff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_falloff" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationFalloff,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_plop& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_plop" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationPlop,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_scoop& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_scoop" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        elt->getInputStartLineNumber (),
        msrArticulationKind::kArticulationScoop,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_articulations& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_articulations" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_arpeggiate& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_arpeggiate" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // number

  int number = elt->getAttributeIntValue ("number", 0);

  // direction

  std::string directionString = elt->getAttributeValue ("direction");

  msrDirectionKind directionKind = msrDirectionKind::kDirectionNone; // default value

  if      (directionString == "up")
    directionKind = msrDirectionKind::kDirectionUp;
  else if (directionString == "down")
    directionKind = msrDirectionKind::kDirectionDown;
  else {
    if (directionString.size ()) {
      std::stringstream ss;

      ss <<
        "arpeggiate direction \"" << directionString << "\"" << "is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // color JMI

  // create the arpeggiato
  S_msrArpeggiato
    arpeggiato =
      msrArpeggiato::create (
        elt->getInputStartLineNumber (),
        placementKind,
        directionKind,
        number);

  fCurrentArticulations.push_back (arpeggiato);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_non_arpeggiate& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_non_arpeggiate" << // JMI
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // type

  std::string typeString = elt->getAttributeValue ("type");

  msrNonArpeggiatoTypeKind
    nonArpeggiatoTypeKind =
      msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeNone; // default value

  if      (typeString == "top")
    nonArpeggiatoTypeKind =
      msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeTop;
  else if (typeString == "bottom")
    nonArpeggiatoTypeKind =
      msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeBottom;
  else {
    if (typeString.size ()) {

      std::stringstream ss;

      ss <<
        "non-arpeggiate type \"" << typeString <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // number

  int number = elt->getAttributeIntValue ("number", 0);

  // color JMI

  // create the non arpeggiato
  S_msrNonArpeggiato
    nonArpeggiato =
      msrNonArpeggiato::create (
        elt->getInputStartLineNumber (),
        placementKind,
        nonArpeggiatoTypeKind,
        number);

  fCurrentArticulations.push_back (nonArpeggiato);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_technical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_technical" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTechnical = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_technical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_technical" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTechnical = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_arrow& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_arrow" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalArrow,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_bend_alter& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bend_alter" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBendAlterValue = (float)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_bend& elt) // JMI
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bend" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitEnd (S_bend& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bend" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical with float
  S_msrTechnicalWithFloat
    technicalWithFloat =
      msrTechnicalWithFloat::create (
        elt->getInputStartLineNumber (),
        msrTechnicalWithFloatKind::kTechnicalWithFloatBend,
        fBendAlterValue,
        placementKind);

  fCurrentTechnicalWithFloatsList.push_back (
    technicalWithFloat);
}

void mxsr2msrSkeletonPopulator::visitStart (S_double_tongue& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_double_tongue" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalDoubleTongue,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_down_bow& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_down_bow" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalDownBow,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_fingering& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fingering" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int fingeringValue = (int)(*elt);

  if (fOnGoingTechnical) {
    // placement

    std::string placementString = elt->getAttributeValue ("placement");

    msrPlacementKind
      placementKind =
        msrPlacementKindFromString (
          elt->getInputStartLineNumber (),
          placementString);

  // create the technical with integer
    S_msrTechnicalWithInteger
      technicalWithInteger =
        msrTechnicalWithInteger::create (
          elt->getInputStartLineNumber (),
          msrTechnicalWithIntegerKind::kFingering,
          fingeringValue,
          placementKind);

    fCurrentTechnicalWithIntegersList.push_back (
      technicalWithInteger);
  }

  else if (fOnGoingFrameNote) {
    fCurrentFrameNoteFingering = fingeringValue;
  }

  else {
    std::stringstream ss;

    ss <<
      "<fingering /> \"" << fingeringValue <<
      "\" is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_fingernails& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fingernails" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalFingernails,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_fret& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fret" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int fretValue = (int)(*elt);

  if (fOnGoingTechnical) {
    // placement

    std::string placementString = elt->getAttributeValue ("placement");

    msrPlacementKind
      fretPlacementKind =  msrPlacementKind::kPlacement_UNKNOWN_;

    // color JMI ???

    // create the technical with integer
    S_msrTechnicalWithInteger
      technicalWithInteger =
        msrTechnicalWithInteger::create (
          elt->getInputStartLineNumber (),
          msrTechnicalWithIntegerKind::kFret,
          fretValue,
          fretPlacementKind);

    fCurrentTechnicalWithIntegersList.push_back (
      technicalWithInteger);
  }

  else if (fOnGoingFrameNote) {
    fCurrentFrameNoteFretNumber = fretValue;
  }

  else {
    std::stringstream ss;

    ss <<
      "<fret /> \"" << fretValue <<
      "\" is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_hammer_on& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_hammer_on" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string hammerOnValue = elt->getValue ();

  // type

  std::string hammerOnType = elt->getAttributeValue ("type");

  msrTechnicalTypeKind hammerOnTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalType_UNKNOWN_;

  if      (hammerOnType == "start")
    hammerOnTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalTypeStart;
  else if (hammerOnType == "stop")
    hammerOnTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalTypeStop;
  else {
    if (hammerOnType.size ()) {
      std::stringstream ss;

      ss <<
        "hammer-on type \"" << hammerOnType <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        elt->getInputStartLineNumber (),
        msrTechnicalWithStringKind::kHammerOn,
        hammerOnTechnicalTypeKind,
        hammerOnValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrSkeletonPopulator::visitStart (S_handbell& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_handbell" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string handBellValue = elt->getValue ();

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        elt->getInputStartLineNumber (),
        msrTechnicalWithStringKind::kHandbell,
        msrTechnicalTypeKind::kTechnicalType_UNKNOWN_,
        handBellValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrSkeletonPopulator::visitStart (S_harmonic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_harmonic" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalHarmonic,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_heel& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_heel" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalHeel,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_hole& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_hole" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalHole,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_open_string& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_open_string" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalOpenString,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_other_technical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_other_technical" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string otherTechnicalValue = elt->getValue ();

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        elt->getInputStartLineNumber (),
        msrTechnicalWithStringKind::kOtherTechnical,
        msrTechnicalTypeKind::kTechnicalType_UNKNOWN_,
        otherTechnicalValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrSkeletonPopulator::visitStart (S_pluck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pluck" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string pluckValue = elt->getValue ();

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        elt->getInputStartLineNumber (),
        msrTechnicalWithStringKind::kPluck,
        msrTechnicalTypeKind::kTechnicalType_UNKNOWN_,
        pluckValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrSkeletonPopulator::visitStart (S_pull_off& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pull_off" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string pullOffValue = elt->getValue ();

  // type

  std::string pullOffType = elt->getAttributeValue ("type");

  msrTechnicalTypeKind pullOffTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalType_UNKNOWN_;

  if      (pullOffType == "start")
    pullOffTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalTypeStart;
  else if (pullOffType == "stop")
    pullOffTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalTypeStop;
  else {
    if (pullOffType.size ()) {
      std::stringstream ss;

      ss <<
        "pull-off type \"" << pullOffType <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        elt->getInputStartLineNumber (),
        msrTechnicalWithStringKind::kPullOff,
        pullOffTechnicalTypeKind,
        pullOffValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrSkeletonPopulator::visitStart (S_snap_pizzicato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_snap_pizzicato" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalSnapPizzicato,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_stopped& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_stopped" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalStopped,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_string& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_string" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
        <notations>
          <technical>
            <std::string>5</std::string>
            <fret>0</fret>
          </technical>
        </notations>
*/

  std::string stringValue = elt->getValue();

  int stringIntegerValue;

  std::istringstream inputStream (stringValue);

  inputStream >> stringIntegerValue;

  if (! stringValue.size ()) {
    std::stringstream ss;

    stringIntegerValue = 0;

    ss <<
      "string value \"" << stringValue <<
      "\" is empty, " << stringIntegerValue << " is assumed";

    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());
  }

  if (fOnGoingTechnical) {
    // placement

    std::string placementString = elt->getAttributeValue ("placement");

    msrPlacementKind
      placementKind =
        msrPlacementKindFromString (
          elt->getInputStartLineNumber (),
          placementString);

  // create the technical with integer
    S_msrTechnicalWithInteger
      technicalWithInteger =
        msrTechnicalWithInteger::create (
          elt->getInputStartLineNumber (),
          msrTechnicalWithIntegerKind::kString,
          stringIntegerValue,
          placementKind);

    fCurrentTechnicalWithIntegersList.push_back (
      technicalWithInteger);
  }

  else if (fOnGoingFrame) {
    fCurrentFrameNoteStringNumber = stringIntegerValue;
  }

  else {
    std::stringstream ss;

    ss <<
      "<std::string /> \"" << stringValue <<
      "\" is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_tap& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tap" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalTap,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_thumb_position& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_thumb_position" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalThumbPosition,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_toe& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_toe" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalToe,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_triple_tongue& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_triple_tongue" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalTripleTongue,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrSkeletonPopulator::visitStart (S_up_bow& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_up_bow" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        elt->getInputStartLineNumber (),
        msrTechnicalKind::kTechnicalUpBow,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_fermata& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fermata" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string fermataTextValue = elt->getValue ();

  // kind

  msrFermataShapeKind
    fermataShapeKind =
      msrFermataShapeKind::kArticulationFermataNormal; // default value

  if      (fermataTextValue == "normal")
    fermataShapeKind = msrFermataShapeKind::kArticulationFermataNormal;

  else if (fermataTextValue == "angled")
    fermataShapeKind = msrFermataShapeKind::kArticulationFermataAngled;

  else if (fermataTextValue == "square")
    fermataShapeKind = msrFermataShapeKind::kArticulationFermataSquare;

  else {
    if (fermataTextValue.size ()) {
      std::stringstream ss;

      ss <<
        "fermata kind \"" << fermataTextValue <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // type

  std::string fermataTypeValue = elt->getAttributeValue ("type");

  msrArticulationFermataType
    articulationFermataType =
      msrArticulationFermataType::kArticulationFermataTypeNone; // default value

  if      (fermataTypeValue == "upright")
    articulationFermataType = msrArticulationFermataType::kArticulationFermataTypeUpright;

  else if (fermataTypeValue == "inverted")
    articulationFermataType = msrArticulationFermataType::kArticulationFermataTypeInverted;

  else {
    if (fermataTypeValue.size ()) {
      std::stringstream ss;

      ss <<
        "fermata type \"" << fermataTypeValue <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // create the fermata
  S_msrFermata
    fermata =
      msrFermata::create (
        elt->getInputStartLineNumber (),
        fermataShapeKind,
        articulationFermataType);

  fCurrentArticulations.push_back (fermata);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_ornaments& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ornaments" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_tremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting tremolo" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // value (tremolo marks number)

  std::string tremoloMarksNumberString =
    elt->getValue ();

  int tremoloMarksNumber = (int)(*elt);

  if (! tremoloMarksNumberString.size ()) {
    std::stringstream ss;

    tremoloMarksNumber = 1;

    ss <<
      "--> tremolo value is missing, " << tremoloMarksNumber << " assumed";

    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());
  }

  if (tremoloMarksNumber < 0 || tremoloMarksNumber > 8) { // JMI what does 0 mean?
    std::stringstream ss;

    ss <<
      "tremolo value \"" << tremoloMarksNumber <<
      "\" should be between 0 and 8";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // type

  std::string tremoloType = elt->getAttributeValue ("type");

  fCurrentDoubleTremoloTypeKind = msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle; // default value

  if      (tremoloType == "single")
    fCurrentDoubleTremoloTypeKind = msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle;

  else if (tremoloType == "start")
    fCurrentDoubleTremoloTypeKind = msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart;

  else if (tremoloType == "stop")
    fCurrentDoubleTremoloTypeKind = msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop;

  else if (tremoloType.size ()) {
    std::stringstream ss;

    ss <<
      "tremolo type \"" << tremoloType <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    singleTremoloPlacementKind = msrPlacementKind::kPlacement_UNKNOWN_;

  msrPlacementKind
    doubleTremoloPlacementKind = msrPlacementKind::kPlacement_UNKNOWN_;

/* JMI ???
  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);
*/

  if      (placementString == "above") {
    switch (fCurrentDoubleTremoloTypeKind) {
      case msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_:
        // just to avoid a compiler message
        break;

      case msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle:
        singleTremoloPlacementKind = msrPlacementKind::kPlacementAbove;
        break;

      case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart:
      case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop:
        doubleTremoloPlacementKind = msrPlacementKind::kPlacementAbove;
        break;
    } // switch
  }

  else if (placementString == "below") {
    switch (fCurrentDoubleTremoloTypeKind) {
      case msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_:
        // just to avoid a compiler message
        break;

      case msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle:
        singleTremoloPlacementKind = msrPlacementKind::kPlacementBelow;
        break;

      case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart:
      case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop:
        doubleTremoloPlacementKind = msrPlacementKind::kPlacementBelow;
        break;
    } // switch
  }

  else if (placementString.size ()) {

    std::stringstream ss;

    ss <<
      "tremolo placement \"" << placementString <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // handle double tremolos
  switch (fCurrentDoubleTremoloTypeKind) {
    case msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_:
      // just to avoid a compiler message
      break;

    case msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle:
      // create a single tremolo, it will be attached to current note
      // in attachCurrentSingleTremoloToNote()
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTremolos ()) {
        std::stringstream ss;

        ss <<
          "Creating a single tremolo" <<
          ", line " << elt->getInputStartLineNumber () <<
          ", " <<
          mfSingularOrPlural (
            tremoloMarksNumber, "mark", "marks") <<
          ", placement : " <<
          msrPlacementKindAsString (
            singleTremoloPlacementKind);

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentSingleTremolo =
        msrSingleTremolo::create (
          elt->getInputStartLineNumber (),
          tremoloMarksNumber,
          singleTremoloPlacementKind);
      break;

    case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart:
  //    if (! fCurrentDoubleTremolo) { JMI
      {
        // fetch the current note's voice
//         S_msrVoice UNUSED
//           currentNoteVoice =
//             fCurrentPartStaffMsrVoicesMap
//         [fCurrentNoteVoiceNumber];

        // create a double tremolo start
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTremolos ()) {
          std::stringstream ss;

          ss <<
            "Creating a double tremolo" <<
            ", line " << elt->getInputStartLineNumber () <<
            ", " <<
            mfSingularOrPlural (
              tremoloMarksNumber, "mark", "marks") <<
            ", placement : " <<
            msrPlacementKindAsString (
              doubleTremoloPlacementKind);

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentDoubleTremolo =
          msrDoubleTremolo::create (
            elt->getInputStartLineNumber (),
            msrDoubleTremoloKind::kDoubleTremoloNotes,
            msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart,
            tremoloMarksNumber,
            doubleTremoloPlacementKind);
      }

/*
      else {
        std::stringstream ss;

        ss <<
          "<tremolo/> start when a current double tremolo is already open";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
*/
      break;

    case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop:
      if (fCurrentDoubleTremolo) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTremolos ()) {
          std::stringstream ss;

          ss <<
            "Meeting a double tremolo stop" <<
            ", line " << elt->getInputStartLineNumber ();

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // it will be handled in handleSoundingNote()
      }

      else {
        std::stringstream ss;

        ss <<
          "<tremolo/> stop whit no preceeding <tremolo/> start";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
      break;
  } // switch
}

void mxsr2msrSkeletonPopulator::visitStart (S_trill_mark& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_trill_mark" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // type : upright inverted  (Binchois20.xml) JMI

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentTrill,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_dashes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_dashes" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // type : upright inverted  (Binchois20.xml) JMI v0.9.67

  // number

  int dashesNumber = elt->getAttributeIntValue ("number", 0); // JMI

  // type

  std::string dashesType = elt->getAttributeValue ("type");

  msrSpannerTypeKind fDashesSpannerTypeKind = msrSpannerTypeKind::kSpannerType_UNKNOWN_;

  if      (dashesType == "start")
    fDashesSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeStart;
  else if (dashesType == "continue")
    fDashesSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeContinue;
  else if (dashesType == "stop")
    fDashesSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeStop;
  else {
    if (dashesType.size ()) {
      std::stringstream ss;

      ss <<
        "dashes type \"" << dashesType <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // color ??? JMI

  // create the spanner
  S_msrSpanner
    spanner =
      msrSpanner::create (
        elt->getInputStartLineNumber (),
        dashesNumber,
        msrSpannerKind::kSpannerDashes,
        fDashesSpannerTypeKind,
        placementKind,
        nullptr); // will be set later REMOVE??? JMI

  fCurrentSpannersList.push_back (spanner);
}

void mxsr2msrSkeletonPopulator::visitStart (S_wavy_line& elt)
{
/*
<!--
	Fermata and wavy-line elements can be applied both to
	notes and to barlines, so they are defined here. Wavy
	lines are one way to indicate trills; when used with a
	barline element, they should always have type="continue"
	set. The fermata text content represents the shape of the
	fermata sign and may be normal, angled, square,
	double-angled, double-square, double-dot, half-curve,
	curlew, or an empty string. An empty fermata element
	represents a normal fermata. The fermata type is upright
	if not specified.
-->
<!ELEMENT fermata  (#PCDATA)>
<!ATTLIST fermata
    type (upright | inverted) #IMPLIED
    %print-style;
    %optional-unique-id;
>
<!ELEMENT wavy-line EMPTY>
<!ATTLIST wavy-line
    type %start-stop-continue; #REQUIRED
    number %number-level; #IMPLIED
    %position;
    %placement;
    %color;
    %trill-sound;
>
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_wavy_line" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  int wavyLineNumber = elt->getAttributeIntValue ("number", 0); // JMI

  // type

  std::string wavyLineType = elt->getAttributeValue ("type");

  msrSpannerTypeKind fWavyLineSpannerTypeKind = msrSpannerTypeKind::kSpannerType_UNKNOWN_;

  if      (wavyLineType == "start")
    fWavyLineSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeStart;
  else if (wavyLineType == "continue")
    fWavyLineSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeContinue;
  else if (wavyLineType == "stop")
    fWavyLineSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeStop;
  else {
    if (wavyLineType.size ()) {
      std::stringstream ss;

      ss <<
        "wavy-line type \"" << wavyLineType <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // color JMI ???

  // create the spanner
  S_msrSpanner
    spanner =
      msrSpanner::create (
        elt->getInputStartLineNumber (),
        wavyLineNumber,
        msrSpannerKind::kSpannerWavyLine,
        fWavyLineSpannerTypeKind,
        placementKind,
        nullptr); // will be set later REMOVE??? JMI v0.9.67

  switch (fWavyLineSpannerTypeKind) {
    case msrSpannerTypeKind::kSpannerTypeStart:
      fCurrentSpannersList.push_back (spanner);

      // remember this wavy line spanner start
      fCurrentWavyLineSpannerStart = spanner;
      break;

    case msrSpannerTypeKind::kSpannerTypeStop:
      // set spanner two-way sideLinks
      // between both ends of the wavy line spanner
      if (! fCurrentWavyLineSpannerStart) {
        std::stringstream ss;

        ss <<
          "wavy-line stop found without corresponding start, ignoring it";

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          ss.str ());
      }
      else {
        fCurrentSpannersList.push_back (spanner);

        spanner->
          setSpannerSideLinkToOtherEnd (
            fCurrentWavyLineSpannerStart);

        // forget about this wavy line spanner start
        fCurrentWavyLineSpannerStart = nullptr;
      }
      break;

    case msrSpannerTypeKind::kSpannerTypeContinue:
      break;

    case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
      // JMI ???
      break;
  } // switch
}

void mxsr2msrSkeletonPopulator::visitStart (S_turn& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_turn" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_inverted_turn& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_inverted_turn" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentInvertedTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_delayed_turn& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_delayed_turn" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentDelayedTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_delayed_inverted_turn& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_delayed_inverted_turn" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentDelayedInvertedTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_vertical_turn& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_vertical_turn" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentVerticalTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_mordent& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_mordent" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentMordent,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_inverted_mordent& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_inverted_mordent" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentInvertedMordent,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_schleifer& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_schleifer" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentSchleifer,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_shake& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_shake" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentShake,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_accidental_mark& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accidental_mark" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string accidentalMark = elt->getValue ();

/*
<!--
  Actual notated accidentals. Valid values include: sharp,
  natural, flat, double-sharp, sharp-sharp, flat-flat,
  natural-sharp, natural-flat, quarter-flat, quarter-sharp,
  three-quarters-flat, three-quarters-sharp, sharp-down,
  sharp-up, natural-down, natural-up, flat-down, flat-up,
  double-sharp-down, double-sharp-up, flat-flat-down,
  flat-flat-up, arrow-down, arrow-up, triple-sharp,
  triple-flat, slash-quarter-sharp, slash-sharp, slash-flat,
  double-slash-flat, sharp-1, sharp-2, sharp-3, sharp-5,
  flat-1, flat-2, flat-3, flat-4, sori, koron, and other.

  The quarter- and three-quarters- accidentals are
  Tartini-style quarter-tone accidentals. The -down and -up
  accidentals are quarter-tone accidentals that include
  arrows pointing down or up. The slash- accidentals
  are used in Turkish classical music. The numbered
  sharp and flat accidentals are superscripted versions
  of the accidental signs, used in Turkish folk music.
  The sori and koron accidentals are microtonal sharp and
  flat accidentals used in Iranian and Persian music. The
  other accidental covers accidentals other than those listed
  here. It is usually used in combination with the smufl
  attribute to specify a particular SMuFL accidental. The
  smufl attribute may be used with any accidental value to
  help specify the appearance of symbols that share the same
  MusicXML semantics. The attribute value is a SMuFL canonical
  glyph name that starts with acc.

  Editorial and cautionary indications are indicated
  by attributes. Values for these attributes are "no" if not
  present. Specific graphic display such as parentheses,
  brackets, and size are controlled by the level-display
  entity defined in the common.mod file.
-->
<!ELEMENT accidental (#PCDATA)>
<!ATTLIST accidental
    cautionary %yes-no; #IMPLIED
    editorial %yes-no; #IMPLIED
    %level-display;
    %print-style;
    %smufl;
>

*/

  msrAccidentalKind
    currentOrnamentAccidentalKind =
      msrAccidentalKind::kAccidentalNone;

  if (accidentalMark      == "sharp")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSharp;

  else if (accidentalMark == "natural")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalNatural;

  else if (accidentalMark == "flat")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalFlat;

  else if (accidentalMark == "double-sharp")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalDoubleSharp;

  else if (accidentalMark == "sharp-sharp")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSharpSharp;

  else if (accidentalMark == "flat-flat")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalFlat;

  else if (accidentalMark == "natural-sharp")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalNaturalSharp;

  else if (accidentalMark == "natural-flat")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalQuarterFlat;

  else if (accidentalMark == "quarter-flat")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalNaturalFlat;

  else if (accidentalMark == "quarter-sharp")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalQuarterSharp;

  else if (accidentalMark == "three-quarters-flat")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalThreeQuartersFlat;

  else if (accidentalMark == "three-quarters-sharp")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalThreeQuartersSharp;

  else if (accidentalMark == "sharp-down")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSharpDown;

  else if (accidentalMark == "sharp-up")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSharpUp;

  else if (accidentalMark == "natural-down")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalNaturalDown;

  else if (accidentalMark == "natural-up")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalNaturalUp;

  else if (accidentalMark == "flat-down")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalFlatDown;

  else if (accidentalMark == "flat-up")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalFlatUp;

  else if (accidentalMark == "triple-sharp")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalTripleSharp;

  else if (accidentalMark == "triple-flat")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalTripleFlat;

  else if (accidentalMark == "slash-quarter-sharp")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSlashQuarterSharp;

  else if (accidentalMark == "slash-sharp")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSlashSharp;

  else if (accidentalMark == "slash-flat")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSlashFlat;

  else if (accidentalMark == "double-slash-flat")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalDoubleSlashFlat;

  else if (accidentalMark == "sharp-1")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSharp_1;

  else if (accidentalMark == "sharp-2")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSharp_2;

  else if (accidentalMark == "sharp-3")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSharp_3;

  else if (accidentalMark == "sharp-5")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSharp_3;

  else if (accidentalMark == "flat-1")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalFlat_1;

  else if (accidentalMark == "flat-2")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalFlat_2;

  else if (accidentalMark == "flat-3")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalFlat_3;

  else if (accidentalMark == "flat-4")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalFlat_4;

  else if (accidentalMark == "sori")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalSori;

  else if (accidentalMark == "koron")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalKoron;

  else if (accidentalMark == "other")
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalOther;

  else if (accidentalMark.size ()) {
    std::stringstream ss;

    ss <<
      "accidental-mark \"" << accidentalMark <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        elt->getInputStartLineNumber (),
        msrOrnamentKind::kOrnamentAccidentalKind,
        placementKind);

  ornament->
    setOrnamentAccidentalKind (
      currentOrnamentAccidentalKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_ornaments& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_ornaments" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_f& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_f" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_ff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ff" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_fff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fff" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicFFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_ffff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ffff" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicFFFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_fffff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fffff" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicFFFFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_ffffff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ffffff" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicFFFFFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_p& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_p" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_pp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_ppp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ppp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicPPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_pppp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pppp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicPPPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_ppppp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ppppp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicPPPPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_pppppp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pppppp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicPPPPPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_mf& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_mf" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicMF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrSkeletonPopulator::visitStart (S_mp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_mp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicMP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_fp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicFP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_fz& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fz" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicFZ,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_pf& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pf" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicPF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_rf& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rf" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicRF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_sf& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sf" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicSF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_rfz& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rfz" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicRFZ,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_sfz& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sfz" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicSFZ,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_sfp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sfp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicSFP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_sfpp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sfpp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicSFPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_sffz& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sffz" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicSFFZ,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_sfzp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sfzp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicSFZP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_n& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_n" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrDynamic
      dynamics =
        msrDynamic::create (
          elt->getInputStartLineNumber (),
          msrDynamicKind::kDynamicN,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_other_dynamics& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_other_dynamics" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string otherDynamicsValue = elt->getValue ();

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the other dynamics
  if (! gGlobalMxsr2msrOahGroup->getIgnoreDynamics ()) {
    S_msrOtherDynamic
      otherDynamic =
        msrOtherDynamic::create (
          elt->getInputStartLineNumber (),
          otherDynamicsValue,
          placementKind);

    fPendingOtherDynamicxList.push_back(otherDynamic);
  }
}

//______________________________________________________________________________
/*
  The damper-pedal, soft-pedal, and sostenuto-pedal
  attributes effect playback of the three common piano
  pedals and their MIDI controller equivalents. The yes
  value indicates the pedal is depressed; no indicates
  the pedal is released. A numeric value from 0 to 100
  may also be used for half pedaling. This value is the
  percentage that the pedal is depressed. A value of 0 is
  equivalent to no, and a value of 100 is equivalent to yes.

    damper-pedal %yes-no-number; #IMPLIED
    soft-pedal %yes-no-number; #IMPLIED
    sostenuto-pedal %yes-no-number; #IMPLIED
*/

/*
void mxsr2msrSkeletonPopulator::visitStart (S_damper_pedal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_damper_pedal" <<
      ", line " << elt->getInputStartLineNumber () <<
      std::endl;
#endif // MF_TRACE_IS_ENABLED

  std::string damperPedalValue = elt->getValue ();

  // check damper pedal value
  if      (damperPedalValue == "yes")
    fCurrentDamperPedalKind
      msrDamperPedal::kDamperPedalValueYes;

  else if (damperPedalValue == "no")
    fCurrentDamperPedalKind =
      msrDamperPedal::kDamperPedalValueNo;

  else {
    std::istringstream inputStream (damperPedalValue);

    inputStream >> fCurrentDamperPedalIntegerValue;

    if (
      fCurrentDamperPedalIntegerValue < 0
        &&
      fCurrentDamperPedalIntegerValue > 100) {
      std::stringstream ss;

      ss <<
        "damper pedal integer value \"" <<
        fCurrentDamperPedalIntegerValue <<
        "\" should be between 0 and 100";

      mxsr2msrError (
        elt->getInputStartLineNumber (),
        ss.str ());
    }

    fCurrentDamperPedalKind =
      msrDamperPedal::kDamperPedalValueZeroToAHundred;

/ * JMI
    if (damperPedalValue.size ()) {
      mxsr2msrError (
        elt->getInputStartLineNumber (),
        "unknown damper pedal \"" +
          damperPedalValue +
          "\", should be 'yes', 'no' or a number from 0 to 100");
    }
    * /
  }


  S_msrDamperPedal
    damperPedal =
      msrDamperPedal::create (
        elt->getInputStartLineNumber (),
        otherDynamicsValue);

}

void mxsr2msrSkeletonPopulator::visitStart (S_soft_pedal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_soft_pedal" <<
       ", line " << elt->getInputStartLineNumber () <<
     std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  std::string softPedalValue = elt->getValue ();

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the other dynamics
  S_msrOtherDynamic
    otherDynamic =
      msrOtherDynamic::create (
        elt->getInputStartLineNumber (),
        otherDynamicsValue);

  fPendingOtherDynamicxList.push_back(otherDynamic);
}

void mxsr2msrSkeletonPopulator::visitStart (S_sostenuto_pedal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sostenuto_pedal" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string sostenutoPedalValue = elt->getValue ();

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        elt->getInputStartLineNumber (),
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the other dynamics
  S_msrOtherDynamic
    otherDynamic =
      msrOtherDynamic::create (
        elt->getInputStartLineNumber (),
        otherDynamicsValue);

  fPendingOtherDynamicxList.push_back(otherDynamic);
}
*/

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_grace& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_grace" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // slash

  std::string slashString = elt->getAttributeValue ("slash");

  fCurrentGraceNotesGroupIsSlashed = false; // default value
  fCurrentGraceNotesGroupIsBeamed = false; // default value

  fCurrentGraceNotesGroupIsTied = false; // default value
  fCurrentGraceNotesGroupIsSlurred = false; // default value

  if      (slashString == "yes")
    fCurrentGraceNotesGroupIsSlashed = true;

  else if (slashString == "no")
    fCurrentGraceNotesGroupIsSlashed = false;

  else {
    if (slashString.size ()) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        "grace slash \"" + slashString + "\" unknown, should be 'yes' or 'no'");
    }
  }

  // should all grace notes be slashed?
  if (gGlobalMxsr2msrOahGroup->getSlashAllGraceNotes ()) {
    fCurrentGraceNotesGroupIsSlashed = true;
  }

  // should all grace notes be beamed?
  if (gGlobalMxsr2msrOahGroup->getBeamAllGraceNotes ()) {
    fCurrentGraceNotesGroupIsBeamed = true;
  }

  fCurrentStealTimeFollowing =
    elt->getAttributeValue ("steal-time-following");

  fCurrentStealTimePrevious =
    elt->getAttributeValue ("steal-time-previous");

  fCurrentMakeTimeSignature =
    elt->getAttributeValue ("make-time");

  fCurrentNoteIsAGraceNote = true;

  fCurrentNoteIsSounding = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_cue& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_cue" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteIsACueNote = true;
  fCurrentNoteIsACueNoteKind = msrNoteIsACueNoteKind::kNoteIsACueNoteYes;

  fCurrentNoteIsSounding = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_chord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_chord" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_time_modification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_time_modification" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // there may be no '<tuplet number="n" type="start" />'
  // in the tuplet notes after the first one,
  // so we detect tuplet notes on '<time-modification>' ??? JMI
  fCurrentNoteHasATimeModification = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_actual_notes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_actual_notes" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int actualNotes = (int)(*elt);

  if (fOnGoingNote) {
    fCurrentNoteActualNotes = actualNotes;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      std::stringstream ss;

      ss <<
        "fCurrentNoteActualNotes: " <<
        fCurrentNoteActualNotes;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // notes inside a tuplet have no <tuplet/> markup
    // and 2 actual notes indicate a double tremolo
    switch (fCurrentNoteActualNotes) {
      case 2:
        fCurrentNoteBelongsToADoubleTremolo = true;
        break;
      default:
        fCurrentNoteBelongsToATuplet = true; // JMI v0.9.70 what when both in tuplet and a chord?
    } // switch
  }

  else if (fOnGoingMetronomeNote) {
    fCurrentMetronomeNoteActualNotes = actualNotes;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTempos ()) {
      std::stringstream ss;

      ss <<
        "fCurrentMetronomeNoteActualNotes: " <<
        fCurrentMetronomeNoteActualNotes;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "<actual-notes /> \"" << actualNotes <<
      "\" is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_normal_notes& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_normal_notes" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int normalNotes = (int)(*elt);

  if (fOnGoingNote) {
    fCurrentNoteNormalNotes = normalNotes;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      std::stringstream ss;

      ss <<
        "fCurrentNoteNormalNotes: " <<
        fCurrentNoteNormalNotes;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // notes inside a tuplet have no <tuplet/> markup
    // and 1 actual note indicates a double tremolo

//     this is superflous since actual_notes has been seen already: // JMI v0.9.70
//     switch (fCurrentNoteNormalNotes) {
//       case 1:
//         fCurrentNoteBelongsToADoubleTremolo = true;
//         break;
//       default:
//         fCurrentNoteBelongsToATuplet = true;
//     } // switch
  }

  else if (fOnGoingMetronomeNote) {
    fCurrentMetronomeNoteNormalNotes = normalNotes;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTempos ()) {
      std::stringstream ss;

      ss <<
        "fCurrentMetronomeNoteNormalNotes: " <<
        fCurrentMetronomeNoteNormalNotes;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "<normal-notes /> \"" << normalNotes <<
      "\" is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_normal_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_normal_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string normalTypeString = elt->getValue();

  if (fOnGoingNote) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      std::stringstream ss;

      ss <<
        "normalTypeString: " <<
        normalTypeString;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // the type contains a display duration
    fCurrentNoteNormalTypeNotesDuration =
      msrNotesDurationKindFromMusicXMLString (
        elt->getInputStartLineNumber (),
        normalTypeString);

  /*
    // there can be several <beat-unit/> in a <metronome/> markup,
    // register beat unit in in dotted durations list
    fCurrentMetronomeBeatUnitsVector.push_back (
      msrDottedNotesDuration (
        fCurrentNoteNormalTypeNotesDuration,
        0));
        */
  }

  else if (fOnGoingMetronomeNote) { // JMI ???
    fCurrentMetronomeNoteNormalType = normalTypeString;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTempos ()) {
      std::stringstream ss;

      ss <<
        "fCurrentMetronomeNoteNormalType: " <<
        fCurrentMetronomeNoteNormalType;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  }

  else {
    std::stringstream ss;

    ss <<
      "<normal-type /> \"" << normalTypeString <<
      "\" is out of context";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_tuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // type

  {
    std::string tupletType = elt->getAttributeValue ("type");

//     msrTupletTypeKind // JMI ??? v0.9.72
//       previousTupletTypeKind = fCurrentTupletTypeKind;

    fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeNone;

    if      (tupletType == "start") {
      fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStart;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTupletsBasics ()) {
        std::stringstream ss;

        ss <<
          "--> There is a tuplet start" <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }

    else if (tupletType == "stop") {
      fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStop;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTupletsBasics ()) {
        std::stringstream ss;

        ss <<
          "--> There is a tuplet stop" <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }

    else if (tupletType == "continue") { // less frequent
      fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeContinue;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTupletsBasics ()) {
        std::stringstream ss;

        ss <<
          "--> There is a tuplet continue" <<
          ", line " << elt->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }

    else {
      std::stringstream ss;

      ss <<
        "tuplet type \"" << tupletType <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  // number

  fPreviousTupletNumber = fCurrentTupletNumber;

  // the tuplet number may be absent, so use 0 in that case
  fCurrentTupletNumber = elt->getAttributeIntValue ("number", 0);

  // bracket

  {
    std::string tupletBracket =
      elt->getAttributeValue ("bracket");

    fCurrentTupletBracketKind =
      msrTupletBracketKind::kTupletBracketYes; // option ??? JMI v0.9.70

    if      (tupletBracket == "yes")
      fCurrentTupletBracketKind = msrTupletBracketKind::kTupletBracketYes;
    else if (tupletBracket == "no")
      fCurrentTupletBracketKind = msrTupletBracketKind::kTupletBracketNo;
    else {
      if (tupletBracket.size ()) {
        std::stringstream ss;

        ss <<
          "tuplet bracket \"" << tupletBracket <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
      else {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTuplets ()) {
          std::stringstream ss;

          ss <<
            "tuplet bracket is empty: this is implementation dependent," <<
            " \"yes\" is assumed" <<
            ", line " << elt->getInputStartLineNumber () <<
            std::endl; // option ??? JMI v0.9.68

          mxsr2msrWarning (
            gServiceRunData->getInputSourceName (),
            elt->getInputStartLineNumber (),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }
    }
  }

  // show-number

  {
    std::string tupletShowNumber = elt->getAttributeValue ("show-number");

    fCurrentTupletShowNumberKind =
      msrTupletShowNumberKind::kTupletShowNumberActual; // default value

    if      (tupletShowNumber == "actual") {
      fCurrentTupletShowNumberKind = msrTupletShowNumberKind::kTupletShowNumberActual;
    }
    else if (tupletShowNumber == "both") {
      fCurrentTupletShowNumberKind = msrTupletShowNumberKind::kTupletShowNumberBoth;
    }
    else if (tupletShowNumber == "none") {
      fCurrentTupletShowNumberKind = msrTupletShowNumberKind::kTupletShowNumberNone;
    }
    else {
      if (tupletShowNumber.size ()) {
        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          "tuplet show-number \"" + tupletShowNumber + "\" is unknown");
      }
    }
  }

  // show-type

  {
    std::string tupletShowType = elt->getAttributeValue ("show-type");

    fCurrentTupletShowTypeKind = msrTupletShowTypeKind::kTupletShowTypeNone; // default value

    if      (tupletShowType == "actual") {
      fCurrentTupletShowTypeKind = msrTupletShowTypeKind::kTupletShowTypeActual;
    }
    else if (tupletShowType == "both") {
      fCurrentTupletShowTypeKind = msrTupletShowTypeKind::kTupletShowTypeBoth;
    }
    else if (tupletShowType == "none") {
      fCurrentTupletShowTypeKind = msrTupletShowTypeKind::kTupletShowTypeNone;
    }
    else {
      if (tupletShowType.size ()) {
        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          "tuplet show-type \"" + tupletShowType + "\" is unknown");
      }
    }
  }

  // line-shape

  {
    std::string tupletLineShape = elt->getAttributeValue ("line-shape");

    fCurrentTupletLineShapeKind =
      msrTupletLineShapeKind::kTupletLineShapeStraight; // default value

    if      (tupletLineShape == "straight")
      fCurrentTupletLineShapeKind = msrTupletLineShapeKind::kTupletLineShapeStraight;
    else if (tupletLineShape == "curved")
      fCurrentTupletLineShapeKind = msrTupletLineShapeKind::kTupletLineShapeCurved;
    else {
      if (tupletLineShape.size ()) {
        std::stringstream ss;

        ss <<
          "tuplet line-shape \"" << tupletLineShape <<
          "\" is unknown";

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  fCurrentTupletPlacementKind =
    msrPlacementKindFromString (
      elt->getInputStartLineNumber (),
      placementString);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Tuplet information gathered: " <<
      "fCurrentTupletNumber: " <<
      fCurrentTupletNumber <<
      ", fCurrentTupletTypeKind: " <<
      fCurrentTupletTypeKind <<
      ", fCurrentTupletBracketKind: " <<
      fCurrentTupletBracketKind <<
      ", fCurrentTupletShowNumberKind: " <<
      fCurrentTupletShowNumberKind <<
      ", fCurrentTupletShowTypeKind: " <<
      fCurrentTupletShowTypeKind <<
      ", fCurrentTupletPlacementKind: " <<
      fCurrentTupletPlacementKind;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteBelongsToATuplet = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_tuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_tuplet_actual& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_actual" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletActual = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_tuplet_actual& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet_actual" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletActual = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_tuplet_normal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_normal" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletNormal = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_tuplet_normal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet_normal" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletNormal = false;
}

void mxsr2msrSkeletonPopulator::visitStart (S_tuplet_number& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_number" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // not handled JMI
  int tupletNumberValue = (int)(*elt);

  if (fOnGoingTupletActual) {
    fCurrentTupletActualNumber = tupletNumberValue;
  }
  else if (fOnGoingTupletNormal) {
    fCurrentTupletNormalNumber = tupletNumberValue;
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<tuplet-number /> out of context");
  }

  // color JMI

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "tuplet number (not handled): " <<
      tupletNumberValue;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_tuplet_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // not handled JMI
  std::string tupletTypeValue = elt->getValue();

  if (fOnGoingTupletActual) {
    fCurrentTupletActualType = tupletTypeValue;
  }
  else if (fOnGoingTupletNormal) {
    fCurrentTupletNormalType = tupletTypeValue;
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<tuplet-type /> out of context");
  }

  // color JMI

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "tuplet type (not handled): " <<
      tupletTypeValue;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_tuplet_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_dot" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingTupletActual) {
    ++fCurrentTupletActualDotsNumber;
  }
  else if (fOnGoingTupletNormal) {
    ++fCurrentTupletNormalDotsNumber;
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      "<tuplet-dot /> out of context");
  }

  // color JMI
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_glissando& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_glissando" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // glissando text, i.e. the text along it
  std::string glissandoTextValue = elt->getValue ();

  // number

  int glissandoNumber = elt->getAttributeIntValue ("number", 0);

  // type

  std::string glissandoType = elt->getAttributeValue ("type");

  msrGlissandoTypeKind
    glissandoTypeKind = msrGlissandoTypeKind::kGlissandoTypeNone;

  if      (glissandoType == "start")
    glissandoTypeKind = msrGlissandoTypeKind::kGlissandoTypeStart;
  else if (glissandoType == "stop")
    glissandoTypeKind = msrGlissandoTypeKind::kGlissandoTypeStop;
  else {
    std::stringstream ss;

    ss <<
      "glissando type \"" << glissandoType <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // line-type

  std::string glissandoLineType = elt->getAttributeValue ("line-type");

  msrLineTypeKind
    glissandoLineTypeKind =
      msrLineTypeKind::kLineTypeSolid; // default value

  if      (glissandoLineType == "solid") {
    glissandoLineTypeKind = msrLineTypeKind::kLineTypeSolid;
  }
  else if (glissandoLineType == "dashed") {
    glissandoLineTypeKind = msrLineTypeKind::kLineTypeDashed;
  }
  else if (glissandoLineType == "dotted") {
    glissandoLineTypeKind = msrLineTypeKind::kLineTypeDotted;
  }
  else if (glissandoLineType == "wavy") {
    glissandoLineTypeKind = msrLineTypeKind::kLineTypeWavy;
  }
  else {
    if (glissandoLineType.size ()) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        "glissando line-type \"" + glissandoLineType + "\" is unknown");
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGlissandos ()) {
    std::stringstream ss;

    ss <<
      "glissandoNumber: " <<  glissandoNumber <<
      "glissandoType: " << glissandoTypeKind <<
      "glissandoLineType: " << glissandoLineTypeKind <<
      "glissandoTextValue: " << glissandoTextValue;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create glissando
  S_msrGlissando
    glissando =
      msrGlissando::create (
        elt->getInputStartLineNumber (),
        glissandoNumber,
        glissandoTypeKind,
        glissandoLineTypeKind,
        glissandoTextValue);

  // register glissando in this visitor
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGlissandos ()) {
    std::stringstream ss;

    ss <<
      "Appending glissando " <<
      glissando->asString () <<
      " to the glissandos pending list";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPendingGlissandosList.push_back (glissando);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_slide& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slide" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // slide text, i.e. the text along it
  std::string slideTextValue = elt->getValue ();

  // number

  int slideNumber = elt->getAttributeIntValue ("number", 0);

  // type

  std::string slideType = elt->getAttributeValue ("type");

  msrSlideTypeKind
    slideTypeKind = msrSlideTypeKind::kSlideTypeNone;

  if      (slideType == "start")
    slideTypeKind = msrSlideTypeKind::kSlideTypeStart;
  else if (slideType == "stop")
    slideTypeKind = msrSlideTypeKind::kSlideTypeStop;
  else {
    std::stringstream ss;

    ss <<
      "slide type \"" << slideType <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // line-type

  std::string slideLineType = elt->getAttributeValue ("line-type");

  msrLineTypeKind
    slideLineTypeKind =
      msrLineTypeKind::kLineTypeSolid; // default value

  if      (slideLineType == "solid") {
    slideLineTypeKind = msrLineTypeKind::kLineTypeSolid;
  }
  else if (slideLineType == "dashed") {
    slideLineTypeKind = msrLineTypeKind::kLineTypeDashed;
  }
  else if (slideLineType == "dotted") {
    slideLineTypeKind = msrLineTypeKind::kLineTypeDotted;
  }
  else if (slideLineType == "wavy") {
    slideLineTypeKind = msrLineTypeKind::kLineTypeWavy;
  }
  else {
    if (slideLineType.size ()) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        "slide line-type \"" + slideLineType + "\" is unknown");
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlides ()) {
    std::stringstream ss;

    ss <<
      "slideNumber: " <<
      slideNumber <<
      "slideType: " <<
      slideTypeKind <<
      "slideLineType: " <<
      slideLineTypeKind;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create slide
  S_msrSlide
    slide =
      msrSlide::create (
        elt->getInputStartLineNumber (),
        slideNumber,
        slideTypeKind,
        slideLineTypeKind,
        slideTextValue);

  // register glissando in this visitor
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlides ()) {
    std::stringstream ss;

    ss <<
      "Appending slide " <<
      slide->asString () <<
      " to the slides pending list";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPendingSlidesList.push_back (slide);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_rest& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rest" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  /*
    <measure number='65'>
      <print new-system='yes'/>
      <note>
        <rest measure='yes'/>
        <duration>3072</duration>
        <voice>1</voice>
        <staff>1</staff>
      </note>
      <backup>
        <duration>3072</duration>
      </backup>
      <note>
        <rest measure='yes'/>
        <duration>3072</duration>
        <voice>5</voice>
        <staff>2</staff>
      </note>
    </measure>

      <note>
        <rest/>
        <duration>24</duration>
        <voice>1</voice>
      </note>

      <rest>
        <display-step>E</display-step>
        <display-octave>4</display-octave>
      </rest>

*/

  fCurrentNoteQuarterTonesPitchKind = msrQuarterTonesPitchKind::kQTP_Rest;

  fCurrentNoteIsARest = true;
  fCurrentNoteIsSounding = false;

  // measure rest?
  std::string restMeasure = elt->getAttributeValue ("measure");

  fCurrentRestIsAMeasureRest = false;

  if (restMeasure == "yes") {
    fCurrentRestIsAMeasureRest = true;
  }
  else if (restMeasure == "no") {
    fCurrentRestIsAMeasureRest = false;
  }

  else {
    if (restMeasure.size ()) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        "rest measure \"" + restMeasure + "\" is unknown");
    }
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_display_step& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_display_step" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string displayStep = elt->getValue();

  checkStep (
    elt->getInputStartLineNumber (),
    displayStep,
    "<display-step/>");

  fCurrentDisplayDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (
      displayStep [0]);

  // pitched rests don't allow for alterations since
  // the display-step merely indicates where to place them on the staff
  fCurrentNoteAlterationKind = msrAlterationKind::kAlterationNatural;
}

void mxsr2msrSkeletonPopulator::visitStart (S_display_octave& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_display_octave" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  int displayOctaveNumber = (int)(*elt);

  if (displayOctaveNumber < 0 || displayOctaveNumber > 9) {
    std::stringstream ss;

    ss <<
      "display octave number " << displayOctaveNumber <<
      " is not in the 0..9 range, '0' is assumed";

    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());

    displayOctaveNumber = 0;
  }

  fCurrentDisplayOctave =
    msrOctaveKindFromNumber (
      elt->getInputStartLineNumber (),
      displayOctaveNumber);
}

void mxsr2msrSkeletonPopulator::visitEnd (S_unpitched& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_unpitched" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteIsUnpitched = true;

 // fCurrentNoteDiatonicPitch = // JMI
   // fCurrentHarmonyRootDiatonicPitch;
}

// //______________________________________________________________________________
// void mxsr2msrSkeletonPopulator::finalizeCurrentChord (
//   int inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChords ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Finalizing current chord START:" <<
//       std::endl <<
//       fCurrentChord <<
//       std::endl <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fCurrentChord->
//     finalizeChord (
//       inputLineNumber);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChords ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Finalizing current chord END: " <<
//       std::endl <<
//       fCurrentChord <<
//       std::endl <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // forget about the current chord
//   fCurrentChord = nullptr;
// }

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::printCurrentChord ()
{
  gLog <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl;

  if (fCurrentChord) {
    gLog <<
      "fCurrentChord contains:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      fCurrentChord;

    --gIndenter;
  }

  else {
    gLog <<
     "fCurrentChord is null" <<
     std::endl;
  }

  gLog <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteArticulationsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrArticulation>
    noteArticulationsList =
      note->
        getNoteArticulationsList ();

  for (S_msrArticulation articulation : noteArticulationsList) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceArticulations ()) {
      std::stringstream ss;

      ss <<
        "Copying articulation " <<
        articulation->getArticulationKind () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendArticulationToChord (articulation);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteTechnicalsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrTechnical>
    noteTechnicalsList =
      note->
        getNoteTechnicalsList ();

  for (S_msrTechnical technical : noteTechnicalsList) {

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTechnicals ()) {
    std::stringstream ss;

    ss <<
      "Copying technical " <<
      technical->getTechnicalKind () <<
      " from note " << note->asString () <<
      " to chord" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendTechnicalToChord (technical);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteTechnicalWithIntegersListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrTechnicalWithInteger>
    noteTechnicalWithIntegersList =
      note->
        getNoteTechnicalWithIntegersList ();

  for (S_msrTechnicalWithInteger technicalWithInteger : noteTechnicalWithIntegersList) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      gLog <<
        "Copying technical " <<
        technicalWithInteger->getTechnicalWithIntegerKind () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendTechnicalWithIntegerToChord (technicalWithInteger);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteTechnicalWithFloatsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrTechnicalWithFloat>
    noteTechnicalWithFloatsList =
      note->
        getNoteTechnicalWithFloatsList ();

  for (S_msrTechnicalWithFloat technicalWithFloat : noteTechnicalWithFloatsList) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Copying technical " <<
        technicalWithFloat->getTechnicalWithFloatKind () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendTechnicalWithFloatToChord (technicalWithFloat);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteTechnicalWithStringsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrTechnicalWithString>
    noteTechnicalWithStringsList =
      note->
        getNoteTechnicalWithStringsList ();

  for (S_msrTechnicalWithString technicalWithString : noteTechnicalWithStringsList) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Copying technical " <<
        technicalWithString->getTechnicalWithStringKind () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendTechnicalWithStringToChord (technicalWithString);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteOrnamentsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrOrnament>
    noteOrnamentsList =
      note->
        getNoteOrnamentsList ();

  for (S_msrOrnament ornament : noteOrnamentsList) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOrnaments ()) {
      std::stringstream ss;

      ss <<
        "Copying ornament " <<
        msrOrnamentKindAsString (ornament->getOrnamentKind ()) <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendOrnamentToChord (ornament);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteSpannersListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrSpanner>
    noteSpannersList =
      note->
        getNoteSpannersList ();

  for (S_msrSpanner spanner : noteSpannersList) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSpanners ()) {
      std::stringstream ss;

      ss <<
        "Copying spanner " <<
        spanner->getSpannerKind () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendSpannerToChord (spanner);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteSingleTremoloToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  S_msrSingleTremolo
    noteSingleTremolo =
      note->
        getNoteSingleTremolo ();

  if (noteSingleTremolo) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTremolos ()) {
      std::stringstream ss;

      ss <<
        "Copying singleTremolo " <<
        noteSingleTremolo->asString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      setChordSingleTremolo (noteSingleTremolo);
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteDynamicsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrDynamic>
    noteDynamicsList =
      note->
        getNoteDynamicsList ();

  for (S_msrDynamic dynamic : noteDynamicsList) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDynamics ()) {
      std::stringstream ss;

      ss <<
        "Copying dynamics " <<
        msrDynamicKindAsString (dynamic->getDynamicKind ()) <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendDynamicToChord (dynamic);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteOtherDynamicsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrOtherDynamic>
    noteOtherDynamicsList =
      note->
        getNoteOtherDynamicsList ();

  for (S_msrOtherDynamic otherDynamic : noteOtherDynamicsList ) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDynamics ()) {
      std::stringstream ss;

      ss <<
        "Copying other dynamics " <<
        otherDynamic->asString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendOtherDynamicToChord (otherDynamic);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteWordsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrWords>
    noteWordsList =
      note->
        getNoteWordsList ();

  std::list <S_msrWords>::const_iterator i;

  for (S_msrWords word : noteWordsList ) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceWords ()) {
      std::stringstream ss;

      ss <<
        "Copying words " <<
        word->asString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendWordsToChord (word);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteStemToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  S_msrStem
    noteStem =
      note->
        getNoteStem ();

  if (noteStem) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceStems ()) {
      std::stringstream ss;

      ss <<
        "Copying stem " <<
        noteStem->asString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendStemToChord (noteStem);
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteBeamsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrBeam>
    noteBeamsList =
      note->
        getNoteBeamsList ();

  for (S_msrBeam beam : noteBeamsList ) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceBeams ()) {
      std::stringstream ss;

      ss <<
        "Copying beam " <<
        beam->asString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendBeamToChord (beam);
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "==> AFTER copying beams to chord:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      chord <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::appendNoteBeamsListLinksToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrBeam>
    noteBeamsList =
      note->
        getNoteBeamsList ();

  if (noteBeamsList.size ()) {
    S_msrBeam beam = noteBeamsList.front ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceBeams ()) {
      std::stringstream ss;

      ss <<
        "Adding beam link of " <<
        beam->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        ", line " << note->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the beam link
    S_msrChordBeamLink
      chordBeamLink =
        msrChordBeamLink::create (
          chord->getInputStartLineNumber (),
          beam,
          chord);

    // append it in the chord
    chord->
      appendChordBeamLinkToChord (chordBeamLink);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "==> AFTER appending note's beams links to chord:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      chord <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteTiesToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

  if (noteTiesList.size ()) {
    for (S_msrTie noteTie : noteTiesList) {

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTies ()) {
        std::stringstream ss;

        ss <<
          "Appending tie " <<
          noteTie->asString () <<
          " from note " << note->asString () <<
          " to chord" << chord->asString ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      chord->
        appendTieToChord (noteTie);
    } // for
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTies ()) {
    std::stringstream ss;

    ss <<
      "==> AFTER appending tie to chord:" <<
      std::endl;

    ++gIndenter;

    ss <<
      chord <<
      std::endl;

    --gIndenter;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

/*
//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteSlursListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's slurs if any from the first note to chord

  std::list <S_msrSlur>
    noteSlursList =
      note->
        getNoteSlursList ();

  std::list <S_msrSlur>::const_iterator i;
  for (S_msrOtherDynamic : S_msrOtherDynamic ) {
     noteSlursList.begin ();
    i != noteSlursList.end ();
    ++i
  ) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlurs ()) {
      gLog <<
        "Copying slur " <<
        (*i)->asString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendSlurToChord ((*i));
  } // for
}
*/

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::appendNoteSlursListLinksToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrSlur>
    noteSlursList =
      note->
        getNoteSlursList ();

  for (S_msrSlur slur : noteSlursList ) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlurs ()) {
      std::stringstream ss;

      ss <<
        "Adding slur link of " <<
        slur->asString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the slur link
    S_msrChordSlurLink
      chordSlurLink =
        msrChordSlurLink::create (
          chord->getInputStartLineNumber (),
          slur,
          chord);

    // append it in the chord
    chord->
      appendChordSlurLinkToChord (chordSlurLink);
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlurs ()) {
    std::stringstream ss;

    ss <<
      "==> AFTER appending note's slurs links to chord:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      chord <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteLigaturesListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrLigature>
    noteLigaturesList =
      note->
        getNoteLigaturesList ();

  for (S_msrLigature ligature : noteLigaturesList) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLigatures ()) {
      std::stringstream ss;

      ss <<
        "Copying ligature " <<
        ligature->getLigatureKind () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendLigatureToChord (ligature);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNotePedalsListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrPedal>
    notePedalsList =
      note->
        getNotePedalsList ();

  for (S_msrPedal pedal : notePedalsList) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePedals ()) {
      std::stringstream ss;

      ss <<
        "Copying pedal " <<
        pedal->asString () <<
        " from note " << note->asString () <<
        " to chord" << chord->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendPedalToChord (pedal);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteSlashesListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrSlash>
    noteSlashesList =
      note->
        getNoteSlashesList ();

  for (S_msrSlash slash : noteSlashesList ) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlashes ()) {
      std::stringstream ss;

      ss <<
        "Copying slash " <<
        slash->asString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendSlashToChord (slash);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteWedgesListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrWedge>
    noteWedgesList =
      note->
        getNoteWedgesList ();

  std::list <S_msrWedge>::const_iterator i;
  for (S_msrWedge wedge : noteWedgesList ) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceWedges ()) {
      std::stringstream ss;

      ss <<
        "Copying wedges " <<
        msrWedgeKindAsString (wedge->getWedgeKind ()) <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendWedgeToChord (wedge);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteSegnosListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrSegno>
    noteSegnosList =
      note->
        getNoteSegnosList ();

  std::list <S_msrSegno>::const_iterator i;
  for (S_msrSegno segno : noteSegnosList ) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSegnos ()) {
      std::stringstream ss;

      ss <<
        "Copying segno " <<
        segno->asShortString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendSegnoToChord (segno);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteDalSegnosListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrDalSegno>
    noteDalSegnosList =
      note->
        getNoteDalSegnosList ();

  std::list <S_msrDalSegno>::const_iterator i;
  for (S_msrDalSegno dalSegno : noteDalSegnosList ) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDalSegnos ()) {
      std::stringstream ss;

      ss <<
        "Copying dal degno " <<
        dalSegno->asShortString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendDalSegnoToChord (dalSegno);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteCodasListToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  std::list <S_msrCoda>
    noteCodasList =
      note->
        getNoteCodasList ();

  std::list <S_msrCoda>::const_iterator i;
  for (S_msrCoda coda : noteCodasList ) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceCodas ()) {
      std::stringstream ss;

      ss <<
        "Copying coda " <<
        coda->asShortString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendCodaToChord (coda);
  } // for
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteOctaveShiftToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  S_msrOctaveShift
    noteOctaveShift =
      note->
        getNoteOctaveShift ();

  if (noteOctaveShift) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOctaveShifts ()) {
      std::stringstream ss;

      ss <<
        "Copying octave shift " <<
        noteOctaveShift->asString () <<
        " from note " << note->asString () <<
        " to chord";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      setChordOctaveShift (noteOctaveShift);
  }
}

/*
//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteGraceNotesGroupsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  S_msrGraceNotesGroup
    graceNotesGroupBefore =
      note->
        getGraceNotesGroupBeforeNote ();

  if (graceNotesGroupBefore) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Copying grace notes group before note " <<
        graceNotesGroupBefore->asShortString () <<
        " from note " << note->asString () <<
        " to chord " << chord->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      setChordGraceNotesGroupBefore (graceNotesGroupBefore);
  }

  S_msrGraceNotesGroup
    graceNotesGroupAfter =
      note->
        getGraceNotesGroupAfterNote ();

  if (graceNotesGroupAfter) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Copying grace notes group after note " <<
        graceNotesGroupAfter->asShortString () <<
        " from note " << note->asString () <<
        " to chord " << chord->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      setChordGraceNotesGroupAfter (graceNotesGroupAfter);
  }
}
*/

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::addNoteGraceNotesGroupsLinksToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // add link of note's grace notes groups if any to chord

  S_msrGraceNotesGroup
    graceNotesGroupBefore =
      note->
        getGraceNotesGroupBeforeNote ();

  if (graceNotesGroupBefore) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Adding grace notes group link before " <<
        graceNotesGroupBefore->asShortString () <<
        " from note " << note->asString () <<
        " to chord " << chord <<
        "";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the grace notes group link
    const S_msrChordGraceNotesGroupLink&
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          chord->getInputStartLineNumber (),
          graceNotesGroupBefore,
          chord);

    // register it in the chord
    chord->
      setChordGraceNotesGroupLinkBefore (
        note->getInputStartLineNumber (), // JMI ???
        chordChordGraceNotesGroupLink);
  }

  S_msrGraceNotesGroup
    graceNotesGroupAfter =
      note->
        getGraceNotesGroupAfterNote ();

  if (graceNotesGroupAfter) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Adding grace notes group link after " <<
        graceNotesGroupAfter->asShortString () <<
        " from note " << note->asString () <<
        " to chord " << chord->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the grace notes group link
    const S_msrChordGraceNotesGroupLink&
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          chord->getInputStartLineNumber (),
          graceNotesGroupAfter,
          chord);

    // register it in the chord
    chord->
      setChordGraceNotesGroupLinkAfter (
        note->getInputStartLineNumber (), // JMI ???
        chordChordGraceNotesGroupLink);
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteHarmoniesToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  const std::list <S_msrHarmony>&
    noteHarmoniesList =
      note->getNoteHarmoniesList ();

  if (noteHarmoniesList.size ()) {
    for (S_msrHarmony harmony : noteHarmoniesList) {

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceHarmonies ()) {
        mfIndentedStringStream iss;

        iss <<
          "Copying harmony:" <<
          '\n';

        ++gIndenter;
        iss <<
          harmony->asString () <<
          '\n';
        --gIndenter;

        iss <<
          "from note:" <<
          '\n';

        ++gIndenter;
        iss <<
          note->asString () <<
          '\n';
        --gIndenter;

        iss <<
          "to chord:" <<
          '\n';

        ++gIndenter;
        iss <<
          chord->asString () <<
          '\n';
        --gIndenter;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          iss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      chord->
        appendHarmonyToChord (harmony);

    } // for
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteElementsIfAnyToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    note != nullptr, // JMI v0.9.70
    "chord is null");

  mfAssert (
    __FILE__, __LINE__,
    chord != nullptr, // JMI v0.9.70
    "chord is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // copy note's articulations if any to the chord
  if (note->getNoteArticulationsList ().size ()) {
    copyNoteArticulationsListToChord (note, chord);
  }

  // copy note's technicals if any to the chord
  if (note->getNoteTechnicalsList ().size ()) {
      copyNoteTechnicalsListToChord (note, chord);
  }
  if (note->getNoteTechnicalWithIntegersList ().size ()) {
      copyNoteTechnicalWithIntegersListToChord (note, chord);
  }
  if (note->getNoteTechnicalWithFloatsList ().size ()) {
      copyNoteTechnicalWithFloatsListToChord (note, chord);
  }
  if (note->getNoteTechnicalWithStringsList ().size ()) {
      copyNoteTechnicalWithStringsListToChord (note, chord);
  }

  // copy note's ornaments if any to the chord
  if (note->getNoteOrnamentsList ().size ()) {
    copyNoteOrnamentsListToChord (note, chord);
  }

  // copy note's spanners if any to the chord
  if (note->getNoteSpannersList ().size ()) {
    copyNoteSpannersListToChord (note, chord);
  }

  // copy note's single tremolo if any to the chord
  if (note->getNoteSingleTremolo ()) {
    copyNoteSingleTremoloToChord (note, chord);
  }

  // copy note's dynamics if any to the chord
  if (note->getNoteDynamicsList ().size ()) {
    copyNoteDynamicsListToChord (note, chord);
  }

  // copy note's other dynamics if any to the chord
  if (note->getNoteOtherDynamicsList ().size ()) {
    copyNoteOtherDynamicsListToChord (note, chord);
  }

  // copy note's words if any to the chord
  if (note->getNoteWordsList ().size ()) {
    copyNoteWordsListToChord (note, chord);
  }

  // copy note's stem if any to the chord
  if (note->getNoteStem ()) {
    copyNoteStemToChord (note, chord);
  }

  // copy note's beams if any to the chord
  if (note->getNoteBeamsList ().size ()) {
    copyNoteBeamsListToChord (note, chord);
//     appendNoteBeamsListLinksToChord (note, chord);
  }

  // copy note's ties if any to the chord // JMI v0.9.70
  if (note->getNoteTiesList ().size ()) {
    copyNoteTiesToChord (note, chord);
  }

  // copy note's slurs if any to the chord
// JMI  copyNoteSlursListToChord (note, chord);
  if (note->getNoteSlursList ().size ()) {
    appendNoteSlursListLinksToChord (note, chord);
  }

  // copy note's ligatures if any to the chord
  if (note->getNoteLigaturesList ().size ()) {
    copyNoteLigaturesListToChord (note, chord);
  }

  // copy note's pedals if any to the chord
  if (note->getNotePedalsList ().size ()) {
    copyNotePedalsListToChord (note, chord);
  }

  // copy note's slashes if any to the chord
  if (note->getNoteSlashesList ().size ()) {
    copyNoteSlashesListToChord (note, chord);
  }

  // copy note's wedges if any to the chord
  if (note->getNoteWedgesList ().size ()) {
    copyNoteWedgesListToChord (note, chord);
  }

  // copy note's segnos if any to the chord
  if (note->getNoteSegnosList ().size ()) {
    copyNoteSegnosListToChord (note, chord);
  }

  // copy note's del segnos if any to the chord
  if (note->getNoteDalSegnosList ().size ()) {
    copyNoteDalSegnosListToChord (note, chord);
  }

  // copy note's codas if any to the chord
  if (note->getNoteCodasList ().size ()) {
    copyNoteCodasListToChord (note, chord);
  }

  // copy note's octave shift if any to the chord
  if (note->getNoteOctaveShift ()) {
    copyNoteOctaveShiftToChord (note, chord);
  }

  // copy note's grace notes groups if any to the chord
//  copyNoteGraceNotesGroupsToChord (note, chord);
  if (
    note->getGraceNotesGroupBeforeNote ()
      ||
    note->getGraceNotesGroupAfterNote ()
  ) {
    addNoteGraceNotesGroupsLinksToChord (note, chord);
  }

  // copy note's harmony if any to the chord
  if (note->getNoteHarmoniesList ().size ()) {
    copyNoteHarmoniesToChord (note, chord);
  }
}

S_msrTuplet mxsr2msrSkeletonPopulator::createTuplet (
  int inputLineNumber)
{
  // create the tuplet
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a tuplet of " <<
      fCurrentNoteActualNotes <<
      '/' <<
      fCurrentNoteNormalNotes;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "fCurrentTupletNumber: " <<
      fCurrentTupletNumber <<
      ", fCurrentTupletTypeKind: " <<
      fCurrentTupletTypeKind <<
      ", fCurrentTupletBracketKind: " <<
      fCurrentTupletBracketKind <<
      ", fCurrentTupletShowNumberKind: " <<
      fCurrentTupletShowNumberKind <<
      ", fCurrentTupletShowTypeKind: " <<
      fCurrentTupletShowTypeKind <<
      ", fCurrentTupletPlacementKind: " <<
      fCurrentTupletPlacementKind;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  const S_msrTuplet&
    tuplet =
      msrTuplet::create (
        inputLineNumber,
        fCurrentTupletNumber,
        fCurrentTupletBracketKind,
        fCurrentTupletLineShapeKind,
        fCurrentTupletTypeKind,
        fCurrentTupletShowNumberKind,
        fCurrentTupletShowTypeKind,
        msrTupletFactor (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes),
        fCurrentTupletPlacementKind);

  return tuplet;
}

void mxsr2msrSkeletonPopulator::copyNoteElementsIfAnyToTuplet (
  const S_msrNote&  firstNote,
  const S_msrTuplet tuplet)
{
  // firstNote is the first tuplet note,
  // and is currently at the end of the voice
  // it defines the tuplets sounding and display whole notes

//   int firstNoteInputLineNumber =
//     firstNote->getInputStartLineNumber ();

  // create the tuplet
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Populating tuplet from its first note " <<
      firstNote->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   tuplet->
//     setMeasureElementSoundingWholeNotes (
//       note->getMeasureElementSoundingWholeNotes ());
//   tuplet->
//     setChordDisplayWholeNotes (
//       note->getChordDisplayWholeNotes ());
//
//   tuplet->
//     setChordMeasureFullLength (
//       note-> gsetChordMeasureFullLength ());
}

void mxsr2msrSkeletonPopulator::handleTupletStart (
  const S_msrTuplet& tuplet,
  const S_msrVoice&  currentNoteVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling tuplet start of tuplet:" <<
      std::endl;
    ++gIndenter;
    ss <<
      tuplet->asString ();
    --gIndenter;
    ss <<
      " in voice" <<
      currentNoteVoice->getVoiceNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle tuplet
  S_mxsrVoice
    theMxsrVoice =
      fCurrentPartStaffMxsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber];

  switch (theMxsrVoice->getTupletsStackSize ()) {
    case 0:
      // tuplet is an outermost tuplet

      // register tuplet by pushing it onto the tuplet stack
      theMxsrVoice->pushTupletOntoTupletsStack (tuplet);

      // append it to currentNoteVoice
      // so that we know its measure position at once
      currentNoteVoice->
        appendTupletToVoice (tuplet);

      break;

    default:
      // tuplet is a nested tuplet

      // register tuplet by pushing it onto the tuplet stack
      theMxsrVoice->pushTupletOntoTupletsStack (tuplet);
      break;
  } //switch

  // register the 'started' tuplet number in the set
//   fExpectedTupletsStopNumbersSet.insert (fCurrentTupletNumber);

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTupletsDetails ()) {
//       displayTupletsStack (
//         "############## mxsr2msrSkeletonPopulator:handleTupletStart() 1");
//   }
//   if (gTraceOahGroup->getTraceTupletsDetails ()) {
//       displayVoicesTupletsStacksMap (
//         "############## mxsr2msrSkeletonPopulator:handleTupletStart() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED

//   // register tuplet in the last handled tuplet in voice map
//   fLastHandledTupletInVoiceMap [
//     std::make_pair (
//       fCurrentNoteStaffNumber,
//       fCurrentNoteVoiceNumber)
//     ] =
//     tuplet;

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTupletsDetails ()) {
//     displayLastHandledTupletInVoiceMap (
//       "############## mxsr2msrSkeletonPopulator:handleTupletStart() 2");
//   }
// #endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::handleTupletContinue (
  const S_msrNote&  note,
  const S_msrVoice& currentNoteVoice)
{
    S_mxsrVoice
      theMxsrVoice =
        fCurrentPartStaffMxsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

  if (theMxsrVoice->getTupletsStackSize ()) {
    S_msrTuplet
      tupletStackTop =
        theMxsrVoice->getTupletsStackTop ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "Handling tuplet continue of tuplet stack top:" <<
        std::endl;
      ++gIndenter;
      ss <<
        tupletStackTop->asString ();
      --gIndenter;
      ss <<
        " in voice" <<
        currentNoteVoice->getVoiceNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // populate tupletStackTop
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      gLog <<
        "--> mxsr2msrSkeletonPopulator::handleTupletContinue() 1: adding tuplet member note " <<
        note->asShortString () <<
        " to stack top tuplet " <<
        tupletStackTop->asString () <<
        " in voice" <<
        currentNoteVoice->getVoiceNumber () <<
        ", line " << note->getInputStartLineNumber () <<
        std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    // add note to tupletStackTop
    tupletStackTop->
      appendNoteToTuplet (
        note);

    fCurrentPartStaffMsrVoicesMap
      [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
        registerTupletNoteInVoice (note);

// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceTupletsDetails ()) {
//       theMxsrVoice->
//         displayTupletsStack (
//           "############## mxsr2msrSkeletonPopulator:kTupletTypeContinue");
//     }
//     if (gTraceOahGroup->getTraceTupletsDetails ()) {
//       theMxsrVoice->
//         displayVoicesTupletsStacksMap (
//           "############## mxsr2msrSkeletonPopulator:kTupletTypeContinue");
//     }
// #endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "handleTupletContinue() 2:" <<
      std::endl <<
      "tuplet member note " <<
      note->
        asShortString () <<
      " cannot be added, tuplets stack is empty" <<
      ", line " << note->getInputStartLineNumber ();

    mxsr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      note->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTupletsDetails ()) {
//     displayLastHandledTupletInVoiceMap (
//       "############## mxsr2msrSkeletonPopulator:handleTupletContinue() 3");
//   }
// #endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::handleTupletStop (
  const S_msrNote&  note,
  const S_msrVoice& currentNoteVoice)
{
    S_mxsrVoice
      theMxsrVoice =
        fCurrentPartStaffMxsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

  if (theMxsrVoice->getTupletsStackSize ()) {
    S_msrTuplet
      tupletStackTop =
        theMxsrVoice->getTupletsStackTop ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      mfIndentedStringStream iss;

      iss <<
        "Handling tuplet stop" << '\n';
      ++gIndenter;

        iss <<
          "note: " << '\n';
        ++gIndenter;
          iss <<
            note->asShortString () << '\n';
        --gIndenter;

        iss <<
          "tuplet stack top:" << '\n';
        ++gIndenter;
          iss <<
            tupletStackTop->asString () << '\n';
        --gIndenter;

        iss <<
          "in voice:" << '\n';
        ++gIndenter;
          iss <<
            currentNoteVoice->asShortString () << '\n';
        --gIndenter;

      --gIndenter;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        iss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // populate tupletStackTop
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      gLog <<
        "--> mxsr2msrSkeletonPopulator::handleTupletStop(): adding tuplet member note " <<
        note->asShortString () <<
        " to stack top tuplet " <<
        tupletStackTop->asString () <<
        " in voice" <<
        currentNoteVoice->getVoiceNumber () <<
        ", line " << note->getInputStartLineNumber () <<
        std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    // add note to tupletStackTop
    tupletStackTop->
      appendNoteToTuplet (
        note);

    fCurrentPartStaffMsrVoicesMap
      [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
        registerTupletNoteInVoice (note);

// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceTupletsDetails ()) {
//       theMxsrVoice->
//         displayTupletsStack (
//           "############## mxsr2msrSkeletonPopulator:kTupletTypeContinue");
//     }
//     if (gTraceOahGroup->getTraceTupletsDetails ()) {
//       theMxsrVoice->
//         displayVoicesTupletsStacksMap (
//           "############## mxsr2msrSkeletonPopulator:kTupletTypeContinue");
//     }
// #endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "handleTupletStop() 2:" <<
      " tuplet member note " <<
      note->
        asShortString () <<
      " cannot be added, tuplets stack is empty" <<
      ", line " << note->getInputStartLineNumber ();

    mxsr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      note->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTupletsDetails ()) {
//     displayLastHandledTupletInVoiceMap (
//       "############## mxsr2msrSkeletonPopulator:handleTupletStop() 2");
//   }
// #endif // MF_TRACE_IS_ENABLED
}

// void mxsr2msrSkeletonPopulator::reduceTupletStackTop (
//   const S_msrNote&  note,
//   const S_msrVoice& currentNoteVoice)
// {
// // #ifdef MF_TRACE_IS_ENABLED
// //   if (gTraceOahGroup->getTraceTupletsDetails ()) {
// //       displayTupletsStack (
// //         "############## mxsr2msrSkeletonPopulator:reduceTupletStackTop() 1");
// //   }
// //   if (gTraceOahGroup->getTraceTupletsDetails ()) {
// //       displayVoicesTupletsStacksMap (
// //         "############## mxsr2msrSkeletonPopulator:reduceTupletStackTop() 1");
// //   }
// // #endif // MF_TRACE_IS_ENABLED
//
//   S_mxsrVoice
//     theMxsrVoice =
//       fCurrentPartStaffMxsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];
//
//   switch (theMxsrVoice->getTupletsStackSize ()) {
//     case 0:
//       {
//         std::stringstream ss;
//
//         ss <<
//           "reduceTupletStackTop() 3:" <<
//           std::endl <<
//           "tuplet member note " <<
//           note->
//             asShortString () <<
//           " cannot be added to any tuplet, the tuplets stack is empty";
//
//         mxsr2msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           note->getInputStartLineNumber (),
//           __FILE__, __LINE__,
//           ss.str ());
//       }
//       break;
//
//     case 1:
//       {
//         // the only started tuplet is the outer-most one:
//         // add the note to it before finalizing it
//         S_msrTuplet
//           tupletStackTop =
//             theMxsrVoice->getTupletsStackTop ();
//
//         // populate the tuplet at the top of the stack
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceTupletsBasics ()) {
//           gLog <<
//             "--> mxsr2msrSkeletonPopulator::reduceTupletStackTop() 4, note " <<
//             note->asShortString () <<
//             " to stack top tuplet " <<
//             tupletStackTop->asString () <<
//             " in voice" <<
//             currentNoteVoice->getVoiceNumber () <<
//             ", line " << note->getInputStartLineNumber () <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         if (! fThereIsAPendingTupletStop) { // CHORD_TUP
//           // JMI v0.9.70 tuplet stop not on last chord member
//           // see ChordsInTupletWithTupletStopNotOnTheChordLastNote.xml
//
//           // add note to tupletStackTop
//           tupletStackTop->
//             appendNoteToTuplet (
//               note);
//
//           fCurrentPartStaffMsrVoicesMap
//         [fCurrentNoteVoiceNumber]->
//             registerTupletNoteInVoice (note);
//         }
//
// // #ifdef MF_TRACE_IS_ENABLED
// //         if (gTraceOahGroup->getTraceTupletsDetails ()) {
// //           theMxsrVoice->
// //             displayTupletsStack (
// //               "############## mxsr2msrSkeletonPopulator:kTupletTypeStop, outer-most");
// //         }
// //         if (gTraceOahGroup->getTraceTupletsDetails ()) {
// //           theMxsrVoice->
// //             displayVoicesTupletsStacksMap (
// //               "############## mxsr2msrSkeletonPopulator:kTupletTypeStop, outer-most");
// //         }
// // #endif // MF_TRACE_IS_ENABLED
//
//         // end of a tuplet forces handling of the pending one
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceTupletsBasics ()) {
//           gLog <<
//             "--> mxsr2msrSkeletonPopulator::reduceTupletStackTop(): finalizing pending tuplet stack top" <<
//             ", line " << note->getInputStartLineNumber () <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//     theMxsrVoice->
//       finalizeTupletStackTopAndPopItFromTupletsStack ( // JMI v0.9.71 // CHORD_TUP
//           note->getInputStartLineNumber (),
//           "mxsr2msrSkeletonPopulator:reduceTupletStackTop() 4");
//
//         // don't pop the inner-most tuplet from the stack yet
// //         theMxsrVoice->getTupletsStack ().pop_front (); // JMI DEDIEUDIEU
//       }
//       break;
//
//     default:
//       {
//         // nested tuplet:
//         // add note to tuplet stack top
//         theMxsrVoice->getTupletsStackTop ()->
//           appendNoteToTuplet (
//             note);
//
//         fCurrentPartStaffMsrVoicesMap
//          [fCurrentNoteVoiceNumber]->
//           registerTupletNoteInVoice (note);
//
//         theMxsrVoice->
//           finalizeTupletStackTopAndPopItFromTupletsStack ( // JMI v0.9.71
//             note->getInputStartLineNumber (),
//             "mxsr2msrSkeletonPopulator:reduceTupletStackTop() 6");
//
//         S_msrTuplet
//           tupletStackTop =
//             theMxsrVoice->getTupletsStackTop ();
//
//         // populate the tuplet at the top of the stack
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceTuplets ()) {
//           gLog <<
//             "--> mxsr2msrSkeletonPopulator:reduceTupletStackTop(): adding nested tuplet member note " <<
//             note->asShortString () <<
//             " to stack top tuplet " <<
//             tupletStackTop->asString () <<
//             ", line " << note->getInputStartLineNumber () <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
// // #ifdef MF_TRACE_IS_ENABLED
// //         if (gTraceOahGroup->getTraceTupletsDetails ()) {
// //           theMxsrVoice->
// //             displayTupletsStack (
// //               "############## kTupletTypeStop, nested");
// //         }
// //         if (gTraceOahGroup->getTraceTupletsDetails ()) {
// //           theMxsrVoice->
// //             displayVoicesTupletsStacksMap (
// //               "############## kTupletTypeStop, nested");
// //         }
// // #endif // MF_TRACE_IS_ENABLED
//
//       }
//   } // switch
//
// // #ifdef MF_TRACE_IS_ENABLED
// //   if (gTraceOahGroup->getTraceTupletsDetails ()) {
// //     displayLastHandledTupletInVoiceMap (
// //       "############## mxsr2msrSkeletonPopulator:reduceTupletStackTop() 2");
// //   }
// // #endif // MF_TRACE_IS_ENABLED
//
//   // forget about the pending tuplet stop
//   fThereIsAPendingTupletStop = false; // CHORD_TUP
//
//   fNoteWithThePendingTupletStop = nullptr;
//   fVoiceOfTheNoteWithThePendingTupletStop = nullptr;
// }

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachCurrentArticulationsToCurrentNote ()
{
  // attach the current articulations if any to the fCurrentNote
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceArticulations ()) {
    std::stringstream ss;

    ss <<
      "Attaching current articulations to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fCurrentArticulations.size ()) {
    S_msrArticulation
      art =
        fCurrentArticulations.front();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Attaching articulation " <<
        art->getArticulationKind () <<
        " to note " << fCurrentNote->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentNote->
      appendArticulationToNote (art);

    // forget about this articulation
    fCurrentArticulations.pop_front();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachCurrentTechnicalsToCurrentNote ()
{
  // attach the current technicals to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTechnicals ()) {
    std::stringstream ss;

    ss <<
      "Attaching current technicals to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fCurrentTechnicalsList.size ()) {
    S_msrTechnical
      tech =
        fCurrentTechnicalsList.front();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Attaching technical " <<
        tech->asString () <<
        " to note " << fCurrentNote->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentNote->appendTechnicalToNote (tech);

    // forget about this technical
    fCurrentTechnicalsList.pop_front();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachCurrentTechnicalWithIntegersToCurrentNote ()
{
  // attach the current technicals to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTechnicals ()) {
    std::stringstream ss;

    ss <<
      "Attaching current technical with integers to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fCurrentTechnicalWithIntegersList.size ()) {
    S_msrTechnicalWithInteger
      tech =
        fCurrentTechnicalWithIntegersList.front();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Attaching technical with integer " <<
        tech->asString () <<
        " to note " << fCurrentNote->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentNote->appendTechnicalWithIntegerToNote (tech);

    // forget about this technical
    fCurrentTechnicalWithIntegersList.pop_front();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachCurrentTechnicalWithFloatsToCurrentNote ()
{
  // attach the current technicals to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTechnicals ()) {
    std::stringstream ss;

    ss <<
      "Attaching current technical with floats to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fCurrentTechnicalWithFloatsList.size ()) {
    S_msrTechnicalWithFloat
      tech =
        fCurrentTechnicalWithFloatsList.front();

#ifdef MF_TRACE_IS_ENABLED
if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Attaching technical with integer " <<
        tech->asString () <<
        " to note " << fCurrentNote->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentNote->appendTechnicalWithFloatToNote (tech);

    // forget about this technical
    fCurrentTechnicalWithFloatsList.pop_front();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachCurrentTechnicalWithStringsToCurrentNote ()
{
  // attach the current technicals to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTechnicals ()) {
    std::stringstream ss;

    ss <<
      "Attaching current technical with strings to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fCurrentTechnicalWithStringsList.size ()) {
    S_msrTechnicalWithString
      tech =
        fCurrentTechnicalWithStringsList.front();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Attaching technical with std::string " <<
        tech->asString () <<
        " to note " << fCurrentNote->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentNote->appendTechnicalWithStringToNote (tech);

    // forget about this technical
    fCurrentTechnicalWithStringsList.pop_front();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachCurrentOrnamentsToCurrentNote ()
{
  // attach the current ornaments to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceOrnaments ()) {
    std::stringstream ss;

    ss <<
      "Attaching current ornaments to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fCurrentOrnamentsList.size ()) {
    S_msrOrnament
      ornament =
        fCurrentOrnamentsList.front();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Attaching ornament " <<
        msrOrnamentKindAsString (ornament->getOrnamentKind ()) <<
        " to note " << fCurrentNote->asString () <<
        std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentNote->appendOrnamentToNote (ornament);

    // forget about this ornament
    fCurrentOrnamentsList.pop_front();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachCurrentSpannersToNote (
  const S_msrNote&   note,
  const std::string& context)
{
  // attach the current spanners to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSpanners ()) {
    std::stringstream ss;

    ss <<
        "Attaching current spanners to note " <<
        fCurrentNote->asString () <<
        ", context: " << context <<
        ", line " << fCurrentNote->getInputStartLineNumber () <<
        std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//     Bool doHandleSpanner (true);
//     Bool spannerStopMetForThisNote (false);
//
//     S_msrSpanner delayedStopSpanner;

  while (fCurrentSpannersList.size ()) {
    S_msrSpanner
      spanner =
        fCurrentSpannersList.front();

//       switch (spanner->getSpannerKind ()) {
//         case msrSpannerKind::kSpannerDashes: // JMI
//           switch (spanner->getSpannerTypeKind ()) {
//             case msrSpannerTypeKind::kSpannerTypeStart:
//               spannerStopMetForThisNote = true;
//               break;
//             case msrSpannerTypeKind::kSpannerTypeStop:
//               doHandleSpanner =
//                 ! spannerStopMetForThisNote;
//               break;
//             case msrSpannerTypeKind::kSpannerTypeContinue:
//               break;
//             case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
//               // JMI ???
//               break;
//           } // switch
//           break;
//
//         case msrSpannerKind::kSpannerWavyLine:
//           switch (spanner->getSpannerTypeKind ()) {
//             case msrSpannerTypeKind::kSpannerTypeStart:
//               spannerStopMetForThisNote = true;
//               break;
//             case msrSpannerTypeKind::kSpannerTypeStop:
//               doHandleSpanner =
//                 ! spannerStopMetForThisNote;
//               break;
//             case msrSpannerTypeKind::kSpannerTypeContinue:
//               break;
//             case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
//               // JMI ???
//               break;
//           } // switch
//           break;
//       } // switch

//       if (doHandleSpanner) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceSpanners ()) {
        std::stringstream ss;

        ss <<
          "Attaching spanner " <<
          spanner->asShortString () <<
          " to note " << fCurrentNote->asString ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentNote->appendSpannerToNote (spanner);

      // set spanner note upLink
      spanner->
        setSpannerUpLinkToNote (note);

      // forget about this spanner
      fCurrentSpannersList.pop_front ();
//       }

//       else { // check it is the same spanner kind JMI
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceSpanners ()) {
//           std::stringstream ss;
//
//           ss <<
//             "Spanner start amd stop on one and the same note' to note " <<
//             fCurrentNote->asString () <<
//             ", delaying 'stop' until next note" <<
//             std::endl;
//
//           gWaeHandler->waeTrace (
//             __FILE__, __LINE__,
//             ss.str (),
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         // keep track of this stop spanner
//         delayedStopSpanner = spanner;
//
//         // forget about this spanner to avoid infinite loop
//         fCurrentSpannersList.pop_front ();
//       }
  } // while

//     // append delayed stop spanner if any again to the list
//     if (delayedStopSpanner) {
//       fCurrentSpannersList.push_back (
//         delayedStopSpanner);
//     }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachCurrentSingleTremoloToCurrentNote ()
{
  // attach the current singleTremolo to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Attaching current singleTremolo to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNote->setNoteSingleTremolo (fCurrentSingleTremolo);

  // set single tremolo graphic duration kind
  fCurrentSingleTremolo->
    setSingleTremoloGraphicNotesDurationKind (
      fCurrentNote->
        getNoteGraphicNotesDurationKind ());

  // forget about this single tremolo
  fCurrentSingleTremolo = nullptr;
}

//______________________________________________________________________________
/* JMI
void mxsr2msrSkeletonPopulator::attachCurrentArticulationsToChord ( // JMI
  const S_msrChord& chord)
{
  if (fCurrentArticulations.size ()) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceArticulations ()) {
      gLog <<
        "Attaching current articulations to chord " <<
        chord->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    std::list <S_msrArticulation>::const_iterator i;
    for (S_msrArticulation articulation : fCurrentArticulations) {

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceArticulations ()) {
        gLog <<
          "Attaching articulation " <<  articulation <<
          " to chord " << chord;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      chord->
        appendArticulationToChord (articulation);
      } // for
  }
}
*/

/*
//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachCurrentOrnamentsToChord ( // JMI
  const S_msrChord& chord)
{
  if (fCurrentOrnamentsList.size ()) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOrnaments ()) {
      gLog <<
        "Attaching current ornaments to chord " <<
        chord->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    std::list <S_msrOrnament>::const_iterator i;
    for (S_msrOrnament ornament : fCurrentOrnamentsList) {

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceOrnaments ()) {
        gLog <<
          "Attaching ornament " <<  ornament << " to chord " <<
          chord;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      chord->
        appendOrnamentToChord (ornament);
      } // for
  }
}
*/

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingTemposToVoice (
  const S_msrVoice& voice)
{
  // attach the pending tempos if any to the voice
  if (fPendingTemposList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTempos ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending tempos to voice \""  <<
        voice->getVoiceName () <<
        "\"";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    while (fPendingTemposList.size ()) {
      S_msrTempo
        tempo =
          fPendingTemposList.front ();

      voice->
        appendTempoToVoice (tempo);

      fPendingTemposList.pop_front ();
    } // while
  }
}

void mxsr2msrSkeletonPopulator::attachPendingTemposToPart (
  const S_msrPart& part)
{
  // attach the pending tempos if any to the voice
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending tempos to part \""  <<
      part->getPartName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingTemposList.size ()) {
    S_msrTempo
      tempo =
        fPendingTemposList.front ();

    part->
      appendTempoToPart (tempo);

    fPendingTemposList.pop_front ();
  } // while
}

// void mxsr2msrSkeletonPopulator::attachPendingBarLinesToVoice (
//   const S_msrVoice& voice)
// {
//   // attach the pending barlines if any to the voice
//   if (fPendingBarLinesList.size ()) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceBarLines ()) {
//       std::stringstream ss;
//
//       ss <<
//         "Attaching pending barlines to voice \""  <<
//         voice->getVoiceName () <<
//         "\"";
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     while (fPendingBarLinesList.size ()) {
//       S_msrBarLine
//         barLine =
//           fPendingBarLinesList.front ();
//
// //       fCurrentPart->
// //         appendBarLineToPart (barLine); // JMI ??? v0.9.63
//
//       voice->
//         appendBarLineToVoice (barLine); // JMIJMIJMI
//
//       fPendingBarLinesList.pop_front ();
//     } // while
//   }
// }

void mxsr2msrSkeletonPopulator::attachPendingBarLinesToPart (
  const S_msrPart& part)
{
  // attach the pending barlines if any to the voice
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending barlines to part \""  <<
      part->getPartName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingBarLinesList.size ()) {
    S_msrBarLine
      barLine =
        fPendingBarLinesList.front ();

//       fCurrentPart->
//         appendBarLineToPart (barLine); // JMI ??? v0.9.63

    part->
      appendBarLineToPart (barLine); // JMIJMIJMI

    fPendingBarLinesList.pop_front ();
  } // while
}

// void mxsr2msrSkeletonPopulator::attachPendingRehearsalMarksToVoice (
//   const S_msrVoice& voice)
// {
//  // attach the pending rehearsals to the note
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRehearsalMarks ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Attaching pending rehearsals to voice \""  <<
//       voice->getVoiceName () <<
//       "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   while (fPendingRehearsalMarksList.size ()) {
//     S_msrRehearsalMark
//       rehearsalMark =
//         fPendingRehearsalMarksList.front ();
//
//     voice->
//       appendRehearsalMarkToVoice (rehearsalMark);
//
//     fPendingRehearsalMarksList.pop_front ();
//   } // while
// }

void mxsr2msrSkeletonPopulator::attachPendingRehearsalMarksToPart (
  const S_msrPart& part)
{
 // attach the pending rehearsals to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRehearsalMarks ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending rehearsals to part \""  <<
      part->getPartName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingRehearsalMarksList.size ()) {
    S_msrRehearsalMark
      rehearsalMark =
        fPendingRehearsalMarksList.front ();

    part->
      appendRehearsalMarkToPart (rehearsalMark);

    fPendingRehearsalMarksList.pop_front ();
  } // while
}

void mxsr2msrSkeletonPopulator::attachPendingLineBreaksToPart (
  const S_msrPart& part)
{
 // attach the pending line breaks to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLineBreaks ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending line breaks to part \""  <<
      part->getPartName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingLineBreaksList.size ()) {
    S_msrLineBreak
      lineBreak =
        fPendingLineBreaksList.front ();

    // append it to the voice
    part->
      appendLineBreakToPart (lineBreak);

    // remove it from the list
    fPendingLineBreaksList.pop_front ();
  } // while
}

void mxsr2msrSkeletonPopulator::attachPendingPageBreaksToPart (
  const S_msrPart& part)
{
 // attach the pending page breaks to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePageBreaks ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending page breaks to part \""  <<
      part->getPartName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingPageBreaksList.size ()) {
    S_msrPageBreak
      pageBreak =
        fPendingPageBreaksList.front ();

    // append it to the voice
    part->
      appendPageBreakToPart (pageBreak);

    // remove it from the list
    fPendingPageBreaksList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingTiesToCurrentNote ()
{
 // attach the pending ties to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTies ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending ties to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingTiesList.size ()) {
    S_msrTie
      tie =
        fPendingTiesList.front ();

    fCurrentNote->appendTieToNote (tie);

    fPendingTiesList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingSegnosToCurrentNote ()
{
 // attach the pending segno to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegnos ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending segno to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingSegnosList.size ()) {
    S_msrSegno
      segno =
        fPendingSegnosList.front ();

    fCurrentNote->appendSegnoToNote (segno);

    fPendingSegnosList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingDalSegnosToCurrentNote ()
{
 // attach the pending dal segno to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDalSegnos ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending dal segno to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingDalSegnosList.size ()) {
    S_msrDalSegno
      dalSegno =
        fPendingDalSegnosList.front ();

    fCurrentNote->appendDalSegnoToNote (dalSegno);

    fPendingDalSegnosList.pop_front ();
  } // while
}

void mxsr2msrSkeletonPopulator::attachPendingDalSegnosToChord (
  const S_msrChord& chord)
{
 // attach the pending dal segno if any to the chord
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDalSegnos ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending dal segno to chord " <<
      chord->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingDalSegnosList.size ()) {
    S_msrDalSegno
      dalSegno =
        fPendingDalSegnosList.front ();

    chord->
      appendDalSegnoToChord (dalSegno);

    fPendingDalSegnosList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingCodasToCurrentNote ()
{
 // attach the pending coda to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCodas ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending codas to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingCodasList.size ()) {
    S_msrCoda
      coda =
        fPendingCodasList.front ();

    fCurrentNote->appendCodaToNote (coda);

    fPendingCodasList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingCrescDecrescsToCurrentNote ()
{
 // attach the pending crescDecresc to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCrescDecrescs ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending crescDecresc to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendinCrescDecrescsList.size ()) {
    S_msrCrescDecresc
      crescDecresc =
        fPendinCrescDecrescsList.front ();

    fCurrentNote->appendCrescDecrescToNote (crescDecresc);

    fPendinCrescDecrescsList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingEyeGlassesToCurrentNote ()
{
 // attach the pending eyeglasses to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceEyeGlasses ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending eyeglasses to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingEyeGlassesList.size ()) {
    S_msrEyeGlasses
      eyeGlasses =
        fPendingEyeGlassesList.front ();

    fCurrentNote->appendEyeGlassesToNote (eyeGlasses);

    fPendingEyeGlassesList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingDampsToCurrentNote ()
{
 // attach the pending damps to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDamps ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending damps to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingDampsList.size ()) {
    S_msrDamp
      damp =
        fPendingDampsList.front ();

    fCurrentNote->appendDampToNote (damp);

    fPendingDampsList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingDampAllsToCurrentNote ()
{
 // attach the pending damp alls to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDampAlls ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending damp alls to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingDampAllsList.size ()) {
    S_msrDampAll
      dampAll =
        fPendingDampAllsList.front ();

    fCurrentNote->appendDampAllToNote (dampAll);

    fPendingDampAllsList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingOctaveShiftsToCurrentNote ()
{
 // attach the pending octave shifts to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceOctaveShifts ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending octave shifts to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingOctaveShiftsList.size ()) {
    S_msrOctaveShift
      octaveShift =
        fPendingOctaveShiftsList.front ();

    fCurrentNote->setNoteOctaveShift (octaveShift);

    fPendingOctaveShiftsList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingScordaturasToCurrentNote ()
{
 // attach the pending scordatura to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceScordaturas ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending scordaturas to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingScordaturasList.size ()) {
    S_msrScordatura
      scordatura =
        fPendingScordaturasList.front ();

    fCurrentNote->appendScordaturaToNote (scordatura);

    fPendingScordaturasList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingDynamicsToCurrentNote ()
{
 // attach the pending dynamics to the note
  Bool delayAttachment (false);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDynamics ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending dynamics to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    if (gGlobalMxsr2msrOahGroup->getDelayRestsDynamics ()) {
      gLog <<
        "Delaying dynamics attached to a rest until next note" <<
        std::endl;

      delayAttachment = true;
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
     if (gTraceOahGroup->getTraceLyrics ()) {
        std::stringstream ss;

        int numberOfDynamics = fPendingDynamicxList.size ();

        if (numberOfDynamics > 1) {
          ss <<
            "there are " << numberOfDynamics << " dynamics";
        }
        else {
          ss <<
            "there is 1 dynamics";
        }
        ss <<
          " attached to a rest";

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          fCurrentNote->getInputStartLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  if (! delayAttachment) {
    while (fPendingDynamicxList.size ()) {
      S_msrDynamic
        dynamics =
          fPendingDynamicxList.front ();

      fCurrentNote->appendDynamicToNote (dynamics);
      fPendingDynamicxList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingOtherDynamicsToCurrentNote ()
{
 // attach the pending other dynamics to the note
  Bool delayAttachment (false);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDynamics ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending other dynamics to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    if (gGlobalMxsr2msrOahGroup->getDelayRestsDynamics ()) {
      gLog <<
        "Delaying dynamics attached to a rest until next note" <<
        std::endl;

      delayAttachment = true;
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceDynamics ()) {
        std::stringstream ss;

        int numberOfOtherDynamics = fPendingOtherDynamicxList.size ();

        if (numberOfOtherDynamics > 1) {
          ss <<
            "there are " << numberOfOtherDynamics << " other dynamics";
        }
        else {
          ss <<
            "there is 1 other dynamics";
        }
        ss <<
          " attached to a rest";

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          fCurrentNote->getInputStartLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  if (! delayAttachment) {
    while (fPendingOtherDynamicxList.size ()) {
      S_msrOtherDynamic
        otherDynamic =
          fPendingOtherDynamicxList.front ();

      fCurrentNote->appendOtherDynamicToNote (otherDynamic);
      fPendingOtherDynamicxList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingWordsToCurrentNote ()
{
  // attach the pending words to the note
  Bool delayAttachment;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceWords ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending words to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    if (gGlobalMxsr2msrOahGroup->getDelayRestsWords ()) {
      gLog <<
        "Delaying word(s) attached to a rest until next note" <<
        std::endl;

      delayAttachment = true;
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWords ()) {
        std::stringstream ss;

        int numberOfWords = fPendingWordsList.size ();

        ss <<
          mfSingularOrPluralWithoutNumber (
            numberOfWords, "There is", "There are") <<
          ' ' <<
          numberOfWords <<
          ' ' <<
          mfSingularOrPluralWithoutNumber (
            numberOfWords, "word", "words") <<
          " attached to a rest";

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          fCurrentNote->getInputStartLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  if (! delayAttachment) {
    while (fPendingWordsList.size ()) {
      S_msrWords
        words =
          fPendingWordsList.front ();

      fCurrentNote->appendWordsToNote (words);

      fPendingWordsList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingBeamsToCurrentNote ()
{
  // attach the pending beams to the note
  Bool delayAttachment (false);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending beams to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    if (gGlobalMxsr2msrOahGroup->getDelayRestsBeams ()) {
      gLog <<
        "Delaying beam attached to a rest until next note" <<
        std::endl;

      delayAttachment = true;
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceBeams ()) {
        std::stringstream ss;

        int numberOfBeams = fPendingBeamsList.size ();

        if (numberOfBeams > 1) {
          ss <<
            "there are " << numberOfBeams << " beams";
        }
        else {
          ss <<
            "there is 1 beam";
        }
        ss <<
          " attached to a rest";

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          fCurrentNote->getInputStartLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  if (! delayAttachment) {
    while (fPendingBeamsList.size ()) {
      S_msrBeam
        beam =
          fPendingBeamsList.front ();

      fCurrentNote->appendBeamToNote (beam);
      fPendingBeamsList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingSlursToCurrentNote ()
{
  // attach the pending slurs to the note
  Bool delayAttachment (false);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlurs ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending slurs to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    if (gGlobalMxsr2msrOahGroup->getDelayRestsSlurs ()) {
      gLog <<
        "Delaying slur attached to a rest until next note" <<
        std::endl;

      delayAttachment = true;
    }

    else {
      std::stringstream ss;

      int numberOfSlurs = fPendingSlursList.size ();

      if (numberOfSlurs > 1) {
        ss <<
          "there are " << numberOfSlurs << " slurs";
      }
      else {
        ss <<
          "there is 1 slur";
      }
      ss <<
        " attached to a rest";

      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        fCurrentNote->getInputStartLineNumber (),
        ss.str ());
    }
  }

  if (! delayAttachment) {
    while (fPendingSlursList.size ()) {
      S_msrSlur
        slur =
          fPendingSlursList.front ();

      fCurrentNote->appendSlurToNote (slur);
      fPendingSlursList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingLigaturesToCurrentNote ()
{
  // attach the pending ligatures to the note
#ifdef MF_TRACE_IS_ENABLED
  int numberOfLigatures =
    fPendingLigaturesList.size ();

  if (gTraceOahGroup->getTraceLigatures ()) {
    std::stringstream ss;

    ss <<
      "Attaching " <<
      mfSingularOrPlural (
       numberOfLigatures,
        "ligature", "ligatures") <<
      " to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  Bool delayAttachment (false);

  if (fCurrentNoteIsARest) {
    if (gGlobalMxsr2msrOahGroup->getDelayRestsLigatures ()) {
      gLog <<
        "Delaying ligature attached to a rest until next note" <<
        std::endl;

      delayAttachment = true;
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLigatures ()) {
        std::stringstream ss;

        ss <<
          mfSingularOrPluralWithoutNumber (
            numberOfLigatures,
            "there is", "there are") <<
            ' ' <<
          mfSingularOrPluralWithoutNumber (
            numberOfLigatures,
            "ligature", "ligatures") <<
          " attached to rest " <<
          fCurrentNote->asShortString ();

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          fCurrentNote->getInputStartLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  if (! delayAttachment) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLigatures ()) {
        std::stringstream ss;

        ss <<
          mfSingularOrPluralWithoutNumber (
            numberOfLigatures,
            "there is", "there are") <<
            ' ' <<
          numberOfLigatures <<
            ' ' <<
            " pending " <<
          mfSingularOrPluralWithoutNumber (
            numberOfLigatures,
            "ligature", "ligatures") <<
          " attached to note " <<
          fCurrentNote->asShortString ();

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          fCurrentNote->getInputStartLineNumber (),
          ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // append ligatures to note only if they belong to a suitable voice,
    // i.e. above goes to voice 1 or 3, and below to voice 2 or 4

    std::list <S_msrLigature>::iterator
      iBegin = fPendingLigaturesList.begin (),
      iEnd   = fPendingLigaturesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrLigature
        ligature = (*i);

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLigatures ()) {
        std::stringstream ss;

        ss <<
          "--> ligature: " <<
          ligature->asString ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // fetch ligatures placement kind
      msrPlacementKind
        ligaturePlacementKind =
          ligature->
            getLigaturePlacementKind ();

//         // fetch note's measure JMI v0.9.63
//         S_msrMeasure
//           noteMeasure =
//             fCurrentNote->
//               getMeasureElementUpLinkToMeasure ();
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
// sanity check
//         mfAssert (
//           __FILE__, __LINE__,
//           noteMeasure != nullptr,
//           "attachPendingLigaturesToNote (): noteMeasure is null",
// #endif // MF_TRACE_IS_ENABLED
//
//         // fetch note's segment
//         S_msrSegment
//           noteSegment =
//             noteMeasure->
//               getMeasureUpLinkToSegment ();
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//         mfAssert (
//           __FILE__, __LINE__,
//           noteSegment != nullptr,
//           "noteSegment is null",
// #endif // MF_TRACE_IS_ENABLED
//
//         // fetch note's voice
//         S_msrVoice
//           noteVoice =
//             noteSegment->
//               getSegmentUpLinkToVoice ();
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//         sanity check
//         mfAssert (
//           __FILE__, __LINE__,
//           noteVoice != nullptr,
//           "noteVoice is null",
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

      // handle ligature placement kind
      switch (ligaturePlacementKind) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          // should not occur
          break;

        case msrPlacementKind::kPlacementAbove:
        case msrPlacementKind::kPlacementBelow:
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceLigatures ()) {
            gLog <<
              "Attaching pending ligature " <<
              ligature->asString () <<
              " to note " <<
              fCurrentNote->asString () <<
//                 " in voice \"" <<
//                 noteVoice->getVoiceName () <<
//                 "\"" <<
              ", line " << ligature->getInputStartLineNumber () <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          fCurrentNote->appendLigatureToNote (ligature);
          break;
      } // switch

      i = fPendingLigaturesList.erase (i);

      if (++i == iEnd) break;
    } // for
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingPedalsToCurrentNote ()
{
  // attach the pending pedals to the note
  Bool delayAttachment (false);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePedals ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending pedals to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    if (gGlobalMxsr2msrOahGroup->getDelayRestsPedals ()) {
      gLog <<
        "Delaying pedal attached to a rest until next note" <<
        std::endl;

      delayAttachment = true;
    }

    else {
      std::stringstream ss;

      int numberOfPedals = fPendingPedalsList.size ();

      if (numberOfPedals > 1) {
        ss <<
          "there are " << numberOfPedals << " pedals";
      }
      else {
        ss <<
          "there is 1 pedal";
      }
      ss <<
        " attached to a rest";

      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        fCurrentNote->getInputStartLineNumber (),
        ss.str ());
    }
  }

  if (! delayAttachment) {
    while (fPendingPedalsList.size ()) {
      S_msrPedal
        pedal =
          fPendingPedalsList.front ();

      fCurrentNote->appendPedalToNote (pedal);
      fPendingPedalsList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingSlashesToCurrentNote ()
{
  // attach the pending slashes to the note
  Bool delayAttachment (false);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlashes ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending slashes to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    if (gGlobalMxsr2msrOahGroup->getDelayRestsSlashes ()) {
      gLog <<
        "Delaying slash attached to a rest until next note" <<
    std::endl;

      delayAttachment = true;
    }

    else {
      std::stringstream ss;

      int numberOfSlashes = fPendingSlashesList.size ();

      if (numberOfSlashes > 1) {
        ss <<
          "there are " << numberOfSlashes << " slashes";
      }
      else {
        ss <<
          "there is 1 slash";
      }
      ss <<
        " attached to a rest";

      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        fCurrentNote->getInputStartLineNumber (),
        ss.str ());
    }
  }

  if (! delayAttachment) {
    while (fPendingSlashesList.size ()) {
      S_msrSlash
        slash =
          fPendingSlashesList.front ();

      fCurrentNote->appendSlashToNote (slash);
      fPendingSlashesList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingWedgesToCurrentNote ()
{
  // attach the pending wedges to the note
  Bool delayAttachment (false);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceWedges ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending wedges to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    if (gGlobalMxsr2msrOahGroup->getDelayRestsWedges ()) {
      gLog <<
        "Delaying wedge attached to a rest until next note" <<
    std::endl;

      delayAttachment = true;
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWedges ()) {
        std::stringstream ss;

        int numberOfWedges = fPendingWedgesList.size ();

        ss <<
          "fPendingWedgesList contains " <<
          mfSingularOrPlural (
            numberOfWedges, "element", "elements") <<
          " attached to a rest";

        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          fCurrentNote->getInputStartLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  if (! delayAttachment) {
    while (fPendingWedgesList.size ()) {
      S_msrWedge
        wedge =
          fPendingWedgesList.front ();

      fCurrentNote->appendWedgeToNote (wedge);

      fPendingWedgesList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingGlissandosToCurrentNote ()
{
 // attach the pending glissandos to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGlissandos ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending glissandos to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingGlissandosList.size ()) {
    S_msrGlissando
      glissando =
        fPendingGlissandosList.front ();

    fCurrentNote->appendGlissandoToNote (glissando);

    // take care of no lyrics on kGlissandoTypeStop
    switch (glissando->getGlissandoTypeKind ()) {
      case msrGlissandoTypeKind::kGlissandoTypeNone:
      case msrGlissandoTypeKind::kGlissandoTypeStart:
        break;

      case msrGlissandoTypeKind::kGlissandoTypeStop:
        // fetch the voice
        S_msrVoice
          theMsrVoice =
            fCurrentPartStaffMsrVoicesMap
              [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

#ifdef MF_TRACE_IS_ENABLED
        // get the voice's stanzas map
        const std::map <std::string, S_msrStanza>&
          voiceStanzasMap =
            theMsrVoice->
              getVoiceStanzasMap ();

        if (gTraceOahGroup->getTraceGlissandos ()) {
          gLog <<
            "--> attachPendingGlissandosToNote ()"
            ", voiceStanzasMap.size (): " <<
            voiceStanzasMap.size () <<
            ", line " << glissando->getInputStartLineNumber () <<
            std::endl;
        }
#endif // MF_TRACE_IS_ENABLED
        break;
    } // switch

    fPendingGlissandosList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingSlidesToCurrentNote ()
{
 // attach the pending slides to the note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlides ()) {
    std::stringstream ss;

    ss <<
      "Attaching pending slides to note " <<
      fCurrentNote->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  while (fPendingSlidesList.size ()) {
    S_msrSlide
      slide =
        fPendingSlidesList.front ();

    fCurrentNote->appendSlideToNote (slide);

    // take care of no lyrics on kSlideTypeStop
    switch (slide->getSlideTypeKind ()) {
      case msrSlideTypeKind::kSlideTypeNone:
      case msrSlideTypeKind::kSlideTypeStart:
        break;

      case msrSlideTypeKind::kSlideTypeStop:
        // fetch the voice
        S_msrVoice
          theMsrVoice =
            fCurrentPartStaffMsrVoicesMap
              [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

        // get the voice's stanzas map
        const std::map <std::string, S_msrStanza>&
          voiceStanzasMap =
            theMsrVoice->
              getVoiceStanzasMap ();

#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTraceSlides ()) {
              gLog <<
                "--> attachPendingSlidesToNote ()"
                ", voiceStanzasMap.size (): " <<
                voiceStanzasMap.size () <<
                ", line " << slide->getInputStartLineNumber () <<
                std::endl;
            }
#endif // MF_TRACE_IS_ENABLED

        if (voiceStanzasMap.size ()) {
          // there are lyrics in this voice
          /* JMI
          if (! fCurrentSyllableElementsList.size ()) {
            // append a skip to lyrics the same duration as the note
#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTraceSlides ()) {
              gLog <<
                "Attaching a skip syllable to note " <<
                fCurrentNote->asString () <<
                " that has a slide stop and no lyrics " <<
                ", line " << slide->getInputStartLineNumber () <<
                std::endl;
            }
#endif // MF_TRACE_IS_ENABLED

            for (
              std::map <std::string, S_msrStanza>::const_iterator i = voiceStanzasMap.begin ();
              i != voiceStanzasMap.end ();
              ++i
            ) {
              S_msrStanza stanza = (*i).second;
              // create a skip syllable
              S_msrSyllable
                syllable =
                  msrSyllable::create (
                    slide->getInputStartLineNumber (),
                    msrSyllableKind::kSyllableSkipRest,
                    msrSyllableExtendKind::kSyllableExtend_NONE, // fCurrentSyllableExtendKind, // JMI v0.9.67
                    fCurrentStanzaNumber,
                    fCurrentNoteSoundingWholeNotesFromNotesDuration,
                    stanza);

              // append syllable to current note's syllables list
              fCurrentNoteSyllablesList.push_back (
                syllable);

              // append syllable to stanza
              stanza->
                appendSyllableToStanza (syllable);
            } // for

            fASkipSyllableHasBeenGeneratedForcurrentNote = true;
          }
          */
        }
        break;
    } // switch

    fPendingSlidesList.pop_front ();
  } // while
}

// void mxsr2msrSkeletonPopulator::attachPendingVoiceLevelElementsToVoice (
//   const S_msrVoice& voice)
// {
// //   JMI
// //   gLog <<
// //     "attachPendingVoiceLevelElementsToVoice()" <<
// //     ", fPendingTemposList.size (): " << fPendingTemposList.size () <<
// //     ", fPendingBarLinesList.size (): " << fPendingBarLinesList.size () <<
// //     ", fPendingLineBreaksList.size (): " << fPendingLineBreaksList.size () <<
// //     ", fPendingPageBreaksList.size (): " << fPendingPageBreaksList.size () <<
// //     std::endl;
// //     */
// //
// //   // the elements pending since before the note
// //   // can now be appended to the latter's voice
// //   // prior to the note itself
// //
// //   // attach pending rehearsals if any to voice
// //   if (fPendingRehearsalMarksList.size ()) {
// //     attachPendingRehearsalMarksToVoice (voice);
// //   }
// //
// //   // attach pending barlines if any to voice
// //   if (fPendingBarLinesList.size ()) {
// //    attachPendingBarLinesToVoice (voice);
// //   }
// //
// //   // attach pending tempos if any to voice
// //   attachPendingTemposToVoice (voice);
// //
// //   // attach pending line breaks if any to voice
// //   if (fPendingLineBreaksList.size ()) {
// //     attachLineBreaksToVoice (voice);
// //   }
// //
// //   // attach pending page breaks if any to voice
// //   if (fPendingPageBreaksList.size ()) {
// //     attachPageBreaksToVoice (voice);
// //   }
// }

void mxsr2msrSkeletonPopulator::attachPendingPartLevelElementsIfAnyToPart ( // JMI v0.9.63
  const S_msrPart& part)
{
  /* JMI
  gLog <<
    "attachPendingPartLevelElementsIfAnyToPart()" <<
    ", fPendingTemposList.size (): " << fPendingTemposList.size () <<
    ", fPendingBarLinesList.size (): " << fPendingBarLinesList.size () <<
    ", fPendingLineBreaksList.size (): " << fPendingLineBreaksList.size () <<
    ", fPendingPageBreaksList.size (): " << fPendingPageBreaksList.size () <<
    std::endl;
    */

  // the elements pending since before the note // JMI
  // can now be appended to the latter's voice
  // prior to the note itself

  // attach pending rehearsals if any to part
  if (fPendingRehearsalMarksList.size ()) {
    attachPendingRehearsalMarksToPart (part);
  }

  // attach pending barlines if any to part
  if (fPendingBarLinesList.size ()) {
    attachPendingBarLinesToPart (part);
  }

  // attach pending tempos if any to part
  if (fPendingTemposList.size ()) {
    attachPendingTemposToPart (part);
  }
}

void mxsr2msrSkeletonPopulator::attachPendingNoteLevelElementsIfAnyToCurrentNote ()
{
  // attach the pending ties, if any, to the note
  if (fPendingTiesList.size ()) {
    attachPendingTiesToCurrentNote ();
  }

  // attach the pending segnos, if any, to the note
  if (fPendingSegnosList.size ()) {
    attachPendingSegnosToCurrentNote ();
  }

  // attach the pending codas, if any, to the note
  if (fPendingCodasList.size ()) {
    attachPendingCodasToCurrentNote ();
  }

  // attach the pending crescDecresc, if any, to the note
  if (fPendinCrescDecrescsList.size ()) {
    attachPendingCrescDecrescsToCurrentNote ();
  }

  // attach the pending eyeglasses, if any, to the note
  if (fPendingEyeGlassesList.size ()) {
    attachPendingEyeGlassesToCurrentNote ();
  }

  // attach the pending damps, if any, to the note
  if (fPendingDampsList.size ()) {
    attachPendingDampsToCurrentNote ();
  }

  // attach the pending damp alls, if any, to the note
  if (fPendingDampAllsList.size ()) {
    attachPendingDampAllsToCurrentNote ();
  }

  // attach the pending scordaturas, if any, to the note
  if (fPendingScordaturasList.size ()) {
    attachPendingScordaturasToCurrentNote ();
  }

  // attach the pending octave shifts, if any, to the note
  if (fPendingOctaveShiftsList.size ()) {
    attachPendingOctaveShiftsToCurrentNote ();
  }

  // attach the pending dynamics, if any, to the note
  if (fPendingDynamicxList.size ()) {
    attachPendingDynamicsToCurrentNote ();
  }

  // attach the pending other dynamics, if any, to the note
  if (fPendingOtherDynamicxList.size ()) {
    attachPendingOtherDynamicsToCurrentNote ();
  }

  // attach the pending words, if any, to the note
  if (fPendingWordsList.size ()) {
    attachPendingWordsToCurrentNote ();
  }

  // attach the pending beams, if any, to the note
  if (fPendingBeamsList.size ()) {
    attachPendingBeamsToCurrentNote ();
  }

  // attach the pending slurs, if any, to the note
  if (fPendingSlursList.size ()) {
    attachPendingSlursToCurrentNote ();
  }

  // attach the pending ligatures, if any, to the note
  if (fPendingLigaturesList.size ()) {
    attachPendingLigaturesToCurrentNote ();
  }

  // attach the pending pedals, if any, to the note
  if (fPendingPedalsList.size ()) {
    attachPendingPedalsToCurrentNote ();
  }

  // attach the pending slashes, if any, to the note
  if (fPendingSlashesList.size ()) {
    attachPendingSlashesToCurrentNote ();
  }

  // attach the pending wedges, if any, to the note
  if (fPendingWedgesList.size ()) {
    attachPendingWedgesToCurrentNote ();
  }

//   // attach the pending wedges, if any and relevant, to the note // JMI v0.9.71
//   if (fPendingVoiceWedgesList.size ()) {
//     attachPendingVoicesWedgesToCurrentNoteIfRelevant (
//       fCurrentNoteVoiceNumber);
//   }

  // attach the pending glissandos, if any, to the note
  if (fPendingGlissandosList.size ()) {
    attachPendingGlissandosToCurrentNote ();
  }

  // attach the pending slides, if any, to the note
  if (fPendingSlidesList.size ()) {
    attachPendingSlidesToCurrentNote ();
  }
}

//______________________________________________________________________________
S_msrNote mxsr2msrSkeletonPopulator::createNote (
  int inputLineNumber)
{
  // determine quarter tones note pitch
  if (fCurrentNoteIsARest) { // JMI v0.9.70
    fCurrentNoteQuarterTonesPitchKind =
      msrQuarterTonesPitchKind::kQTP_Skip;
  }
  else {
    fCurrentNoteQuarterTonesPitchKind =
      quarterTonesPitchKindFromDiatonicPitchAndAlteration (
        inputLineNumber,
        fCurrentNoteDiatonicPitchKind,
        fCurrentNoteAlterationKind);
  }

  // determine quarter tones note display pitch
  fCurrentNoteQuarterTonesDisplayPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentDisplayDiatonicPitchKind,
      fCurrentNoteAlterationKind);

  // has the current note graphic duration been specified
  // in a '<type>' markup?
  switch (fCurrentNoteGraphicNotesDurationKind) {
    case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
      // use the same duration as the one from the duration
      // internally ??? JMI v0.9.72
      fCurrentNoteDisplayWholeNotesFromType =
        fCurrentNoteSoundingWholeNotesFromNotesDuration;
      break;

    default:
      // convert note graphic duration into whole notes
      fCurrentNoteDisplayWholeNotesFromType =
        msrNotesDurationKindAsWholeNotes (
          fCurrentNoteGraphicNotesDurationKind);

// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTraceNotesDetails ()) {
//         gLog <<
//           std::endl <<
//           "(1):" <<
//           std::endl <<
//           "fCurrentNoteGraphicNotesDurationKind: " <<
//           msrNotesDurationKindAsString (
//             fCurrentNoteGraphicNotesDurationKind) <<
//           std::endl <<
//           "fCurrentNoteDisplayWholeNotesFromType: " <<
//           fCurrentNoteDisplayWholeNotesFromType <<
//           std::endl <<
//           "fCurrentNoteDotsNumber: " <<
//           fCurrentNoteDotsNumber <<
//           std::endl << std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED

      // take dots into account if any
      if (fCurrentNoteDotsNumber > 0) {
        msrWholeNotes
          wholeNotesIncrement =
            fCurrentNoteDisplayWholeNotesFromType * mfRational (1, 2);
        int
          dots =
            fCurrentNoteDotsNumber;

        while (dots > 0) {
          fCurrentNoteDisplayWholeNotesFromType +=
            wholeNotesIncrement;

          wholeNotesIncrement *= mfRational (1, 2);

          --dots;
        } // while
      }
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    gLog <<
      "===> createNote(), gathered note informations:" <<
      std::endl;

    ++gIndenter;

    constexpr int fieldWidth = 48;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentNoteSoundingWholeNotesFromNotesDuration" << " : " <<
      fCurrentNoteSoundingWholeNotesFromNotesDuration <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteGraphicNotesDuration" << " : " <<
        msrNotesDurationKindAsString (
          fCurrentNoteGraphicNotesDurationKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteDotsNumber" << " : " <<
        fCurrentNoteDotsNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteDisplayWholeNotesFromType" << " : " <<
      fCurrentNoteDisplayWholeNotesFromType <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteIsARest" << " : " <<
      fCurrentNoteIsARest <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentDivisionsPerQuarterNote" << " : " <<
      fCurrentDivisionsPerQuarterNote <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNotePrintObjectKind" << " : " <<
      fCurrentNotePrintObjectKind <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteRGB" << " : " <<
      fCurrentNoteRGB <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteAlpha" << " : " <<
      fCurrentNoteAlpha <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteStaffNumber =" << " : " <<
      mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fPreviousNoteStaffNumber" << " : " <<
      mfStaffNumberAsString (fPreviousNoteStaffNumber) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteVoiceNumber" << " : " <<
      mfVoiceNumberAsString (fCurrentNoteVoiceNumber) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentRecipientStaffNumber" << " : " <<
      mfStaffNumberAsString (fCurrentRecipientStaffNumber) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteIsSounding" << " : " <<
      fCurrentNoteIsSounding <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteIsARest" << " : " <<
      fCurrentNoteIsARest <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentRestIsAMeasureRest" << " : " <<
      fCurrentRestIsAMeasureRest <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteIsAGraceNote" << " : " <<
      fCurrentNoteIsAGraceNote <<
      std::endl <<


      std::setw (fieldWidth) <<
      "fCurrentNoteIsACueNote" << " : " <<
      fCurrentNoteIsACueNote <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteBelongsToAChord" << " : " <<
      fCurrentNoteBelongsToAChord <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteBelongsToATuplet" << " : " <<
      fCurrentNoteBelongsToATuplet <<
      std::endl <<

      std::setw (fieldWidth) <<
      "inputLineNumber" << " : " <<
      inputLineNumber <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsAGraceNote) {
    // set current grace note display whole notes
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteDisplayWholeNotesFromType;
  }

  else {
    if (
      fCurrentDoubleTremoloTypeKind == msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart
        ||
      fCurrentDoubleTremoloTypeKind == msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop
    ) {
      // double tremolo note
      if (
        fCurrentNoteGraphicNotesDurationKind == msrNotesDurationKind::kNotesDuration_UNKNOWN_
      ) {
        std::stringstream ss;

        ss <<
          "double tremolo note lacks a <type/>"; // JMI a completer

        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }

      // set current double tremolo note display whole notes
      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteDisplayWholeNotesFromType;
    }

    else if (fCurrentNoteIsARest) {
      // rest

      // set current rest sounding and display whole notes
      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteSoundingWholeNotesFromNotesDuration;

      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteDisplayWholeNotesFromType;

  /* JMI
      // set current note sounding and display whole notes
      if (fCurrentNoteSoundingWholeNotesFromNotesDuration.getNumerator () == 0) {
        // only <type /> was found, no <duration /> was specified
        fCurrentNoteDisplayWholeNotes =
          fCurrentNoteDisplayWholeNotesFromType;

        fCurrentNoteSoundingWholeNotes =
          fCurrentNoteDisplayWholeNotes; // same value by default
      }
      else {
        // <duration /> was found
        fCurrentNoteSoundingWholeNotes =
          fCurrentNoteSoundingWholeNotesFromNotesDuration;

        fCurrentNoteDisplayWholeNotes =
          fCurrentNoteSoundingWholeNotes; // same value by default
      }
      */
    }

    else {
      // other non-grace note

      // set current note sounding and display whole notes
      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteSoundingWholeNotesFromNotesDuration;

      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteDisplayWholeNotesFromType;

      /* JMI
      if (fCurrentNoteSoundingWholeNotesFromNotesDuration.getNumerator () == 0) {
        // only <type /> was found, no <duration /> was specified
        fCurrentNoteDisplayWholeNotes =
          fCurrentNoteDisplayWholeNotesFromType;

        fCurrentNoteSoundingWholeNotes =
          fCurrentNoteDisplayWholeNotes; // same value by default
      }
      else {
        // <duration /> was found
        fCurrentNoteSoundingWholeNotes =
          fCurrentNoteSoundingWholeNotesFromNotesDuration;

        fCurrentNoteDisplayWholeNotes =
          fCurrentNoteSoundingWholeNotes; // same value by default
      }
      */
    }
  }

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  if (
    gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
  ) {
#ifdef MF_TRACE_IS_ENABLED
    if (
      gTraceOahGroup->getTraceParts ()
    ) {
      std::stringstream ss;

      ss <<
        "---> note sanity test: " <<
        ", fCurrentDivisionsPerQuarterNote: " <<
        fCurrentDivisionsPerQuarterNote <<
        ", fCurrentNoteNormalNotes: " <<
        fCurrentNoteNormalNotes <<
        ", fCurrentNoteActualNotes: " <<
        fCurrentNoteActualNotes <<
        ", fCurrentNoteSoundingWholeNotes: " <<
        fCurrentNoteSoundingWholeNotes;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());

      if (
        fCurrentDivisionsPerQuarterNote * fCurrentNoteNormalNotes
          !=
        fCurrentNoteSoundingWholeNotes.getNumerator () * fCurrentNoteActualNotes
      ) {
        std::stringstream ss;

        ss <<
          ", fCurrentDivisionsPerQuarterNote * fCurrentNoteNormalNotes = " <<
          fCurrentDivisionsPerQuarterNote * fCurrentNoteNormalNotes <<
          ", fCurrentNoteSoundingWholeNotes.getNumerator () * fCurrentNoteActualNotes = " <<
          fCurrentNoteSoundingWholeNotes.getNumerator () * fCurrentNoteActualNotes;

  //       mfAssert (
  //         __FILE__, __LINE__,
  //         false, // JMI v0.9.70
  //         ss.str ());
        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
    }
#endif // MF_TRACE_IS_ENABLED
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

  // create the note
  S_msrNote
    note =
      msrNote::create (
        inputLineNumber,
        gNullMeasure,
        	// will be set later in setMeasureElementUpLinkToMeasure()

        msrNoteKind::kNote_UNKNOWN_,
          // will be set later by 'setNoteKind()' when it becomes known later

        fCurrentNoteQuarterTonesPitchKind,
        fCurrentNoteOctave,

        fCurrentNoteSoundingWholeNotes,
        fCurrentNoteDisplayWholeNotes,

        fCurrentNoteDotsNumber,

        fCurrentNoteGraphicNotesDurationKind,

        fCurrentNoteQuarterTonesDisplayPitchKind,
        fCurrentDisplayOctave,

        fCurrentNoteIsACueNoteKind,

        fCurrentNotePrintObjectKind,

        fCurrentNoteHeadKind,
        fCurrentNoteHeadFilledKind,
        fCurrentNoteHeadParenthesesKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "++++++ Creating note:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      note <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // register note as the last one met in its voice
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotesBasics ()
      ||
    gTraceOahGroup->getTraceStaffChangesBasics ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting last met note in voice " <<
      mfVoiceNumberAsString (fCurrentNoteVoiceNumber) <<
      " to note " <<
      note->asShortString () <<
      ", fCurrentRecipientStaffNumber: " <<
  		mfStaffNumberAsString (fCurrentRecipientStaffNumber) <<
      ", fCurrentNoteVoiceNumber: " <<
  		mfVoiceNumberAsString (fCurrentNoteVoiceNumber);

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register note as the last one in its voice
  fCurrentPartStaffMxsrVoicesMap
    [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]->
      setLastMetNoteInVoice (note);

  return note;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::populateCurrentNoteWithCurrentInformations (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotesBasics ()
//       ||
//     gTraceOahGroup->getTraceStaffChangesBasics ()
  ) {
    std::stringstream ss;

    ss <<
      "==> populateCurrentNoteWithCurrentInformations() " <<
      fCurrentNote->asString () <<
       " into";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    displayStaffAndVoiceInformation (
      fCurrentNote->getInputStartLineNumber (),
      "populateCurrentNoteWithCurrentInformations())");
  }
#endif // MF_TRACE_IS_ENABLED

  // set current note's color if relevant
  if (fCurrentNoteRGB.size () || fCurrentNoteAlpha.size ()) {
    msrColorAlphaRGB
      colorAlphaRGB (
        fCurrentNoteRGB,
        fCurrentNoteAlpha);

    fCurrentNote->
      setNoteColorAlphaRGB (
        colorAlphaRGB);
  }

//   // attach the regular pending elements (not dal segnos), if any, to fCurrentNote
//   attachPendingNoteLevelElementsIfAnyToCurrentNote (); // would be too early ??? JMI
//
  fCurrentNote->
    setNoteAccidentalKind (
      fCurrentAccidentalKind);

  fCurrentNote->
    setNoteEditorialAccidentalKind (
      fCurrentEditorialAccidentalKind);

  fCurrentNote->
    setNoteCautionaryAccidentalKind (
      fCurrentCautionaryAccidentalKind);

  // check <duration/> and <type/> consistency if relevant
  if (
    fCurrentNoteSoundingWholeNotesFromNotesDuration
      !=
    fCurrentNoteDisplayWholeNotesFromType
      &&
    ! fCurrentNoteHasATimeModification
  ) {
    switch (fCurrentNote->getNoteKind ()) {
      case msrNoteKind::kNote_UNKNOWN_:
        break;

      case msrNoteKind::kNoteRegularInTuplet:
      case msrNoteKind::kNoteRestInTuplet:
      case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      case msrNoteKind::kNoteUnpitchedInTuplet:
  // JMI      break;

      case msrNoteKind::kNoteRestInMeasure:
      case msrNoteKind::kNoteSkipInMeasure:
      case msrNoteKind::kNoteUnpitchedInMeasure:
      case msrNoteKind::kNoteRegularInMeasure:
      case msrNoteKind::kNoteRegularInChord:
        if (! fCurrentNoteIsAGraceNote) {
          std::stringstream ss;

          ss <<
            "note duration inconsistency: divisions indicates " <<
            fCurrentNoteSoundingWholeNotesFromNotesDuration <<
            " while type indicates " <<
            fCurrentNoteDisplayWholeNotesFromType <<
            ", using the latter";

          mxsr2msrWarning (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            ss.str ());
        }

        fCurrentNote->
          setMeasureElementSoundingWholeNotes (
            fCurrentNoteDisplayWholeNotesFromType,
            "mxsr2msrSkeletonPopulator::populateCurrentNoteWithCurrentInformations()");
        break;

      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
      case msrNoteKind::kNoteInChordInGraceNotesGroup:
      case msrNoteKind::kNoteInDoubleTremolo:
        break;
    } // switch
  }

//   // set fCurrentNote tie if any
//   if (fCurrentTie) {
//     fPendingTiesList.push_back (fCurrentTie);
// //     fCurrentNote->
// //       appendTieToNote (fCurrentTie);
//
//     // forget about this tie
//     fCurrentTie = nullptr;
//   }
//
  // set fCurrentNote stem if any
  if (fCurrentStem) {
    fCurrentNote->
      setNoteStem (fCurrentStem);

    // forget about this stem
    fCurrentStem = nullptr;
  }

/* JMI
  // attach the beams if any to the note
  if (fPendingBeamsList.size ()) {
    for (S_msrBeam beam : fPendingBeamsList) {
      fCurrentNote->
        appendBeamToCurrentNote (beam);
    } // for

    fPendingBeamsList.clear ();
  }
*/

  // attach the articulations if any to the note
  if (fCurrentArticulations.size ()) {
    attachCurrentArticulationsToCurrentNote ();
  }

  // attach the technicals if any to the note
  if (fCurrentTechnicalsList.size ()) {
    attachCurrentTechnicalsToCurrentNote ();
  }
  if (fCurrentTechnicalWithIntegersList.size ()) {
    attachCurrentTechnicalWithIntegersToCurrentNote ();
  }
  if (fCurrentTechnicalWithFloatsList.size ()) {
    attachCurrentTechnicalWithFloatsToCurrentNote ();
  }
  if (fCurrentTechnicalWithStringsList.size ()) {
    attachCurrentTechnicalWithStringsToCurrentNote ();
  }

  // attach the ornaments if any to the note
  if (fCurrentOrnamentsList.size ()) {
    attachCurrentOrnamentsToCurrentNote ();
  }

  // attach the spanners if any to the note
  if (fCurrentSpannersList.size ()) {
    attachCurrentSpannersToNote (
      fCurrentNote,
      "populateCurrentNoteWithCurrentInformations()");
  }

  // attach the singleTremolo to the note
  if (fCurrentSingleTremolo) {
    attachCurrentSingleTremoloToCurrentNote ();
  }

  // handling the pending grace notes group if any
  if (fPendingGraceNotesGroup && ! fCurrentNoteIsAGraceNote) {
    // this is the first note after the grace notes group

    // attach the current grace notes to this note
    switch (fPendingGraceNotesGroup->getGraceNotesGroupKind ()) {
      case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
        fCurrentNote->
          setGraceNotesGroupBeforeNote (
            fPendingGraceNotesGroup);
        break;
      case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
        fCurrentNote->
          setGraceNotesGroupAfterNote (
            fPendingGraceNotesGroup);
        break;
    } // switch

    // forget about the pending grace notes
    fPendingGraceNotesGroup = nullptr;
  }
}

void mxsr2msrSkeletonPopulator::populateCurrentNoteAfterItHasBeenHandled (
  int inputLineNumber) // JMI v0.9.72
{
  // attach the regular pending elements (not dal segnos), if any, to fCurrentNote
  // only now because <lyric> follows <glissando> and <slide> in MusicXML JMI ???
  attachPendingNoteLevelElementsIfAnyToCurrentNote ();
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleCurrentNote (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "handleCurrentNote()" <<
      ", fCurrentTupletNumber: " <<
      fCurrentTupletNumber <<
      ", fCurrentTupletTypeKind: " <<
      fCurrentTupletTypeKind <<
      ", fCurrentTupletBracketKind: " <<
      fCurrentTupletBracketKind <<
      ", fCurrentTupletShowNumberKind: " <<
      fCurrentTupletShowNumberKind <<
      ", fCurrentTupletShowTypeKind: " <<
      fCurrentTupletShowTypeKind <<
      ", fCurrentTupletPlacementKind: " <<
      fCurrentTupletShowTypeKind <<

      ", fCurrentNoteIsSounding: " <<
      fCurrentNoteIsSounding <<

      ", fCurrentNoteIsARest: " <<
      fCurrentNoteIsARest <<

      ", fCurrentRestIsAMeasureRest: " <<
      fCurrentRestIsAMeasureRest <<

      ", fCurrentNoteIsAGraceNote: " <<
      fCurrentNoteIsAGraceNote <<

      ", fCurrentNoteIsACueNote: " <<
      fCurrentNoteIsACueNote <<

      ", fCurrentNoteBelongsToAChord: " <<
      fCurrentNoteBelongsToAChord <<

      ", fCurrentNoteBelongsToATuplet: " <<
      fCurrentNoteBelongsToATuplet <<

      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   S_mxsrVoice
//     theMxsrVoice =
//       fCurrentPartStaffMxsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

  // handle the note itself
  // ======================

  if (fCurrentNoteIsSounding) {
    // sounding notes are the most frequent ones

    if (fCurrentNoteBelongsToAChord) {
      // current note is a sounding chord member

      if (fCurrentNoteBelongsToATuplet) {
        // chord member in a tuplet
        handleChordMemberNoteInATuplet (
          fCurrentNote);
      }
      else {
        // chord member
        handleChordMemberNote (
          fCurrentNote);
      }
    }

    else if (fCurrentNoteBelongsToATuplet) {
      // tuplet member
        handleTupletMemberNote (
          fCurrentNote);
    }

    else {
      // standalone sounding note
      handleSoundingNote (
        fCurrentNote);
    }
  }

  else if (fCurrentNoteIsARest) {
    // rests are less frequent
    handleRest (
      fCurrentNote);
  }

//   else if (fCurrentRestIsAMeasureRest) {
//     // measure rests are less frequent
//     handleRest (
//       fCurrentNote);
//   }

  else if (fCurrentNoteIsAGraceNote) {
    // grace notes are less frequent

    if (fCurrentNoteBelongsToAChord) {
      handleChordMemberNoteInAGraceNotesGroup (
        fCurrentNote);
    }
    else {
      handleGraceNote (
        fCurrentNote); // JMI v0.9.72
    }
  }

//   else if (fCurrentNoteIsACueNote) {
//     // cue notes are event far less frequent
//
// //     handleCueNote (
// //       fCurrentNote);
//   }

  else {
    std::stringstream ss;

    ss <<
      "--> handleCurrentNote(): don't know how to characterize this note" <<
      ", fCurrentNoteStaffNumber: " <<
      mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
      std::endl <<
      ", fCurrentNoteVoiceNumber: " <<
      mfVoiceNumberAsString (fCurrentNoteVoiceNumber) <<
      ", line " << inputLineNumber;

    mxsr2msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }



//   if (fCurrentNoteIsStandalone) {
//     // regular note or rest
//
//     // this note terminates a tuplet if any
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//         // sanity check
//         mfAssert (
//           __FILE__, __LINE__,
//           fCurrentNoteVoiceNumber != K_VOICE_NUMBER_UNKNOWN_,
//           "fCurrentNoteVoiceNumber is unknown");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// //     const S_mxsrVoice&
// //       theMxsrVoice =
// //         fCurrentPartStaffMxsrVoicesMap
// //           [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];
// //
// //     if (theMxsrVoice->getTupletsStackSize ()) {
// //       theMxsrVoice->
// //         finalizeTupletStackTopAndPopItFromTupletsStack ( // JMI v0.9.71
// //           inputLineNumber,
// //           "handleCurrentNote()");
// //     }
//
//     // handle the note/rest
//     handleSoundingNote ();
//   }

  // finalizing current chord if relevant
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "--> fCurrentNoteBelongsToAChord: " <<
      fCurrentNoteBelongsToAChord <<
      "--> fOnGoingChord: " <<
      fOnGoingChord <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fCurrentNoteBelongsToAChord) {
    if (fOnGoingChord) {
       // is there a pending tuplet stop?
//       if (fThereIsAPendingTupletStop) { // CHORD_TUP
//         // handle the tuplet stop
//         handleTupletStop (
//           fNoteWithThePendingTupletStop,
//           fVoiceOfTheNoteWithThePendingTupletStop);
//       }
    }

    if (fCurrentDoubleTremolo) {
      // forget about a double tremolo containing a chord
    // JMI XXL BOFS  fCurrentDoubleTremolo = nullptr;
    }

//     // is there a pending tuplet stop?
//     if (false && fThereIsAPendingTupletStop) { // CHORD_TUP
//       // handle the tuplet stop
//       handleTupletStop (
//         fNoteWithThePendingTupletStop,
//         fVoiceOfTheNoteWithThePendingTupletStop);
//     }
  }

  // register fCurrentNote as the last found note for the current voice
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
  /* JMI
    gLog <<
      "--> STORING " <<
      fCurrentNote->asShortString () <<
      " as last note found in voice " <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->getVoiceName () <<
      std::endl <<
      "-->  fCurrentNoteStaffNumber: " <<
      mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
      std::endl <<
      "--> fCurrentNoteVoiceNumber : " <<
      mfVoiceNumberAsString (fCurrentNoteVoiceNumber) <<
      std::endl <<
      / * JMI
      "--> staff name : " <<
      staff->getStaffName () <<
      std::endl <<
      * /
      "--> voice name : " <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->getVoiceName () <<
      std::endl;
      */
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffVoicesLastMetNoteMap
    [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber] =
      fCurrentNote;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    printVoicesLastMetNoteMap (
      inputLineNumber,
      "handleNote()");
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachPendingGraceNotesGroupToNoteIfRelevant (
  int inputLineNumber)
{
  // is there a pending grace notes group?
  if (fPendingGraceNotesGroup) {
#ifdef MF_TRACE_IS_ENABLED
    if (gGlobalMxsr2msrOahGroup->getTraceBackup ()) {
      gLog <<
        "Attaching pending grace notes group to current non-grace note upon backup" <<
        std::endl;
    }

    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "fCurrentGraceNotesGroupNotes IS NOT NULL upon <backup/>" << // JMI
        ", line " << inputLineNumber <<
        std::endl;

      gLog <<
        "+++++++++++++++++ fPendingGraceNotesGroup:" <<
        std::endl <<
        fPendingGraceNotesGroup <<
        "+++++++++++++++++" <<
        std::endl;

//     // fetch note to attach to  // JMI v0.9.67
//     S_msrNote
//       noteToAttachTo =
//       /*
//       // JMI might prove not precise enough???
//   //      fStaffVoicesLastMetNoteMap [fCurrentPartStaffMsrVoicesMap
//         [fCurrentNoteVoiceNumber]];
//         fStaffVoicesLastMetNoteMap [
//           std::make_pair (
//             fCurrentNoteStaffNumber,
//             fCurrentNoteVoiceNumber)
//           ];
//       */
//         theMsrVoice->getVoiceLastAppendedNote (); // JMI v0.9.72
    }

#endif // MF_TRACE_IS_ENABLED

//     if (false && fCurrentNonGraceNote) { // JMI v0.9.72 ???
// #ifdef MF_TRACE_IS_ENABLED
//       gLog <<
//         "+++++++++++++++++ fCurrentNonGraceNote:" <<
//         std::endl <<
//         fCurrentNonGraceNote <<
//         "+++++++++++++++++" <<
//         std::endl;
// #endif // MF_TRACE_IS_ENABLED
//
//       // set the pending grace notes group's kind to 'after'
//       fPendingGraceNotesGroup->
//         setGraceNotesGroupKind (
//           msrGraceNotesGroupKind::kGraceNotesGroupAfter);
//
//       // attach it to fCurrentNonGraceNote
//       fCurrentNonGraceNote->
//         setGraceNotesGroupAfterNote (
//           fPendingGraceNotesGroup);
//     }
//
//     else {
      std::stringstream ss;

      ss <<
        "cannot find voice to insert after grace notes group into: " <<
        "chordFirstNote is null" <<
        ", fCurrentNoteStaffNumber: " <<
        mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
        std::endl <<
        ", fCurrentNoteVoiceNumber: " <<
        mfVoiceNumberAsString (fCurrentNoteVoiceNumber) <<
        ", line " << inputLineNumber;

      mxsr2msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
//     }

    // forget about this grace notes group
    fPendingGraceNotesGroup = nullptr;
  }
}


//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::attachThePendingDalSegnosIfAny ()
{
  if (fPreviousMeasureElement) {
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*fPreviousMeasureElement))
    ) {
      if (fPendingDalSegnosList.size ()) {
        attachPendingDalSegnosToCurrentNote ();
      }
    }
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*fPreviousMeasureElement))
    ) {
      if (fPendingDalSegnosList.size ()) {
        attachPendingDalSegnosToChord (chord);
      }
    }
    else {
      // FOO JMI
    }
  }
  else {
    // FOO JMI
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleStaffChangeTakeOffEventIfAnyBeforeNoteCreation ()
{
/*
      Event 1:
        [StaffChangeEvent
          fEventInputStartLineNumber    : L120
          fEventSequentialNumber        : E1
          fNoteSequentialNumber         : N1
          fNoteEventStaffNumber         : S2
          fNoteEventVoiceNumber         : V1
          fStaffChangeEventKind         : kEventStaffChangeTakeOff
          fTakeOffInputStartLineNumber  : L120
          fLandingInputStartLineNumber  : L132
          fTakeOffStaffNumber           : S2
          fLandingStaffNumber           : S1
          staff change                  : S2/V1 ->> S1/V1
        ]
*/

//   fCurrentTakeOffStaffNumber = K_STAFF_NUMBER_UNKNOWN_;
//   fCurrentLandingStaffNumber = K_STAFF_NUMBER_UNKNOWN_;

  // fetch the staff change landing event upon this note if any
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
    std::stringstream ss;

    ss <<
      "--------> handleStaffChangeTakeOffEventIfAnyBeforeNoteCreation(): " <<
      "fCurrentNoteSequentialNumber: " << fCurrentNoteSequentialNumber <<
      "fCurrentRecipientStaffNumber: " << fCurrentRecipientStaffNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fCurrentNoteStaffChangeTakeOff =
    fKnownEventsCollection.
      fetchStaffChangeTakeOffAtNoteSequentialNumber (
        fCurrentNoteSequentialNumber);

  if (fCurrentNoteStaffChangeTakeOff) {
    // there is a staff change landing
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
      std::stringstream ss;

      ss <<
        "There is a staff change handleStaffChangeTakeOffEventIfAnyBeforeNoteCreation():" <<
        '\n' <<
        fCurrentNoteStaffChangeTakeOff <<
        "fCurrentRecipientStaffNumber: " << fCurrentRecipientStaffNumber <<
        "\n\n";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
    int
      noteSequentialNumber =
        fCurrentNoteStaffChangeTakeOff->
          getNoteSequentialNumber ();

    int
      eventInputStartLineNumber =
        fCurrentNoteStaffChangeTakeOff->
          getEventInputStartLineNumber ();

    // sanity checks
    if (noteSequentialNumber != fCurrentNoteSequentialNumber) {
      std::stringstream ss;

      ss <<
        "noteSequentialNumber: " <<
        noteSequentialNumber <<
        " is not equal to fCurrentNoteSequentialNumber: " <<
        fCurrentNoteSequentialNumber;

      mfAssert (
        __FILE__, __LINE__,
        false,
        ss.str ());
    }

    if (eventInputStartLineNumber != fCurrentNoteInputStartLineNumber) {
      std::stringstream ss;

      ss <<
        "eventInputStartLineNumber: " <<
        eventInputStartLineNumber <<
        " is not equal to fCurrentNoteInputStartLineNumber: " <<
        fCurrentNoteInputStartLineNumber;

      mfAssert (
        __FILE__, __LINE__,
        false,
        ss.str ());
    }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//     // the recipient staff number is now that of the take off note
//     fCurrentRecipientStaffNumber =
//       takeOffStaffNumber; // JMI v0.9.72

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
    std::stringstream ss;

    ss <<
      "--------> handleStaffChangeTakeOffEventIfAnyBeforeNoteCreation(): " <<
      "fCurrentRecipientStaffNumber: " << fCurrentRecipientStaffNumber <<
      ", eventInputStartLineNumber: " << eventInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
  }

  fOnGoingStaffChange = true;
}

void mxsr2msrSkeletonPopulator::createStaffChange (
  int                    inputLineNumber,
  S_mxsrStaffChangeEvent staffChangeTakeOffEvent)
{
  if (staffChangeTakeOffEvent) {
    int
      eventInputStartLineNumber =
        staffChangeTakeOffEvent->
          getEventInputStartLineNumber ();
//       eventInputEndLineNumber =
//         staffChangeTakeOffEvent->
//           getEventInputEndLineNumber ();

    int
      takeOffStaffNumber =
        staffChangeTakeOffEvent->
          getTakeOffStaffNumber (),
     landingStaffNumber =
        staffChangeTakeOffEvent->
          getLandingStaffNumber ();

    int
      noteEventVoiceNumber =
        staffChangeTakeOffEvent->
          getNoteEventVoiceNumber ();


#ifdef MF_TRACE_IS_ENABLED
    int
      noteSequentialNumber =
        staffChangeTakeOffEvent->
          getNoteSequentialNumber ();

    gLog <<
      "There is a staff change createStaffChange():" <<
      std::endl <<
      staffChangeTakeOffEvent <<
      "fCurrentRecipientStaffNumber: " << fCurrentRecipientStaffNumber <<
      std::endl <<
      std::endl;

// 		if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
// 			std::stringstream ss;
//
// 			ss <<
// 				"===> There is a staff change createStaffChange() " <<
// 				staffChangeTakeOffEvent->getStaffChangeEventKind () <<
// 				" event:" <<
// 				'\n' <<
// 				"eventInputStartLineNumber: " <<
// 				eventInputStartLineNumber <<
// 				"eventInputEndLineNumber: " <<
// 				eventInputEndLineNumber <<
//
// 				", noteSequentialNumber: " <<
// 				noteSequentialNumber <<
//
// 				", takeOffStaffNumber: " <<
// 				takeOffStaffNumber <<
// 				", landingStaffNumber: " <<
// 				landingStaffNumber <<
//
// 				", noteEventVoiceNumber: " <<
// 				noteEventVoiceNumber <<
//
// 				", line " << eventInputStartLineNumber;
//
// 			gWaeHandler->waeTrace (
// 				__FILE__, __LINE__,
// 				ss.str ());
// 		}
#endif // MF_TRACE_IS_ENABLE

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
    // sanity checks
    if (noteSequentialNumber != fCurrentNoteSequentialNumber) {
      std::stringstream ss;

      ss <<
        "noteSequentialNumber: " <<
        noteSequentialNumber <<
        " is not equal to fCurrentNoteSequentialNumber: " <<
        fCurrentNoteSequentialNumber;

      mfAssert (
        __FILE__, __LINE__,
        false,
        ss.str ());
    }

    if (eventInputStartLineNumber != fCurrentNoteInputStartLineNumber) {
      std::stringstream ss;

      ss <<
        "eventInputStartLineNumber: " <<
        eventInputStartLineNumber <<
        " is not equal to fCurrentNoteInputStartLineNumber: " <<
        fCurrentNoteInputStartLineNumber;

      mfAssert (
        __FILE__, __LINE__,
        false,
        ss.str ());
    }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

    // fetch the current note's staff
    S_msrStaff
      takeOffStaff =
        fCurrentPartStavesVector [takeOffStaffNumber],
      landingStaff =
        fCurrentPartStavesVector [landingStaffNumber]; // JMI v0.9.72

    // create the voice staff change
    S_msrVoiceStaffChange
      voiceStaffChange =
        msrVoiceStaffChange::create (
          eventInputStartLineNumber,
          gNullMeasure,  // JMI v0.9.72 ???  // set later in setMeasureElementUpLinkToMeasure()
          takeOffStaff,
          landingStaff);

    // append the voice staff change to the event's voice number
    // before the note itself is appended
    fCurrentPartStaffMsrVoicesMap
      [fCurrentRecipientStaffNumber][noteEventVoiceNumber]->
        appendVoiceStaffChangeToVoice (
          voiceStaffChange);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "--> handleStaffChangeTakeOffEventIfAnyBeforeNoteCreation()" <<
  		", fCurrentRecipientStaffNumber: " <<
  		mfStaffNumberAsString (fCurrentRecipientStaffNumber) <<
  		", fCurrentNoteVoiceNumber: " <<
  		mfVoiceNumberAsString (fCurrentNoteVoiceNumber) <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::copyNoteValuesToCurrentChord (
  S_msrNote note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    gLog <<
      "===> copyNoteValuesToCurrentChord(), note: " <<
      note <<
      std::endl << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Copying values from note " <<
      note->asString () <<
      " to current chord " <<
      fCurrentChord->asString () <<
      ", line " << note->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // mark note as being the first one in the chord
  note->
    setNoteIsAChordsFirstMemberNote ();

  // whole notes
  fCurrentChord->
    setMeasureElementSoundingWholeNotes (
      note->
        getMeasureElementSoundingWholeNotes (),
      "copyNoteValuesToCurrentChord()");

  fCurrentChord->
    setChordDisplayWholeNotes (
      note->
        getNoteDisplayWholeNotes ());

  // graphic duration
  fCurrentChord->
    setChordGraphicNotesDurationKind (
      note->
        getNoteGraphicNotesDurationKind ());

  // copy newChordNote's elements if any to the current chord JMI v0.9.72
//   copyNoteElementsIfAnyToChord (
//     note,
//     fCurrentChord);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleChordBeginIfAnyBeforeNoteCreation ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    gLog <<
      "===> handleChordBeginIfAnyBeforeNoteCreation(), gathered note informations:" <<
      std::endl;

    ++gIndenter;

    constexpr int fieldWidth = 48;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentNoteStaffNumber =" << " : " <<
      mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fPreviousNoteStaffNumber" << " : " <<
      mfStaffNumberAsString (fPreviousNoteStaffNumber) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteVoiceNumber" << " : " <<
      mfVoiceNumberAsString (fCurrentNoteVoiceNumber) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentRecipientStaffNumber" << " : " <<
      mfStaffNumberAsString (fCurrentRecipientStaffNumber) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteIsAGraceNote" << " : " <<
      fCurrentNoteIsAGraceNote <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteIsACueNote" << " : " <<
      fCurrentNoteIsACueNote <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteBelongsToAChord" << " : " <<
      fCurrentNoteBelongsToAChord <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteBelongsToATuplet" << " : " <<
      fCurrentNoteBelongsToATuplet <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteInputStartLineNumber" << " : " <<
      fCurrentNoteInputStartLineNumber <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteChordBegin =
    fKnownEventsCollection.
      fetchChordBeginAtNoteSequentialNumber (
        fCurrentNoteSequentialNumber);

  if (fCurrentNoteChordBegin) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChordsBasics ()) {
      std::stringstream ss;

      ss <<
        "--> handleChordBeginIfAnyBeforeNoteCreation(): there is a " <<
        fCurrentNoteChordBegin->getEventInputStartLineNumber () <<
        " chord event" <<
        ", fCurrentNoteSequentialNumber: " <<
        fCurrentNoteSequentialNumber <<
        ", fCurrentNoteStaffNumber: " <<
        fCurrentNoteStaffNumber <<
        ", fCurrentNoteVoiceNumber: " <<
        fCurrentNoteVoiceNumber <<
        ", fCurrentNoteChordEnd: " <<
        fCurrentNoteChordBegin->asString () <<
        ", line " << fCurrentNoteInputStartLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    switch (fCurrentNoteChordBegin->getChordEventKind ()) {
      case mxsrChordEventKind::kEventChord_NONE:
        // should not occur
        break;

      case mxsrChordEventKind::kEventChordBegin:
        // create a chord
        createChord (
          fCurrentNoteChordBegin->
            getEventInputStartLineNumber ());

        fCurrentNoteBelongsToAChord = true;

        fOnGoingChord = true;
        break;

      case mxsrChordEventKind::kEventChordEnd:
        // should not occur
        break;
    } // switch
  }

  else {
    // is this a chord member note without any chord event?
    if (fOnGoingChord) {
      fCurrentNoteBelongsToAChord = true;
    }
  }
}

void mxsr2msrSkeletonPopulator::handleChordEndIfAnyAfterNoteHandling ()
{
  fCurrentNoteChordEnd =
    fKnownEventsCollection.
      fetchChordEndAtNoteSequentialNumber (
        fCurrentNoteSequentialNumber);

  if (fCurrentNoteChordEnd) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChordsBasics ()) {
      std::stringstream ss;

      ss <<
        "--> handleChordEndIfAnyAfterNoteHandling(): there is a " <<
        fCurrentNoteChordEnd->getChordEventKind () <<
        " chord event" <<
        ", fCurrentNoteSequentialNumber: " <<
        fCurrentNoteSequentialNumber <<
        ", fCurrentNoteStaffNumber: " <<
        fCurrentNoteStaffNumber <<
        ", fCurrentNoteVoiceNumber: " <<
        fCurrentNoteVoiceNumber <<
        ", fCurrentNoteChordEnd: " <<
        fCurrentNoteChordEnd->asString () <<
        ", line " << fCurrentNoteInputStartLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    switch (fCurrentNoteChordEnd->getChordEventKind ()) {
      case mxsrChordEventKind::kEventChord_NONE:
        // should not occur
        break;

      case mxsrChordEventKind::kEventChordBegin:
        // nothing here, already handled earlier in this method
        break;

      case mxsrChordEventKind::kEventChordEnd:
        if (! fCurrentChordHasBeenPopulatedFromItsFirstNote) {
          copyNoteValuesToCurrentChord (
            fCurrentChordFirstNote);

          // forget about the current chord begin
          fCurrentChordFirstNote = nullptr;

          fCurrentChordHasBeenPopulatedFromItsFirstNote = true;
        }

        // append current chord to the current voice in the recipient staff
        // only now, so that the chord sounding duration is known
        // and accounted for in the measure
        fCurrentPartStaffMsrVoicesMap
          [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]->
            appendChordToVoice (
              fCurrentChord);

        // forget about the current chord
        fCurrentChord = nullptr;

        fOnGoingChord = false;
        break;
    } // switch

    // forget about fCurrentNoteChordEnd
    fCurrentNoteChordEnd = nullptr;
  }
}

void mxsr2msrSkeletonPopulator::createChord (int inputLineNumber)
{
  // create the current chord
  fCurrentChord =
    msrChord::create (
      inputLineNumber);

  // register it as not yet populated fron its first note
  fCurrentChordHasBeenPopulatedFromItsFirstNote = false;

  // remember the current note as the first one in the current chord
  fCurrentChordFirstNote = fCurrentNote;

//   // append current chord to the current voice in the recipient staff
//   fCurrentPartStaffMsrVoicesMap
//     [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]->
//       appendChordToVoice (
//         fCurrentChord);
}

//________________________________________________________________________
/*
    fTupletsBeginsList: 1 element, in note sequential number order
      Note 1:
        [TupletEvent
          fEventInputStartLineNumber : 33
          fEventSequentialNumber     : E1
          fNoteSequentialNumber      : N1
          fNoteEventStaffNumber      : S1
          fNoteEventVoiceNumber      : V1
          fTupletEventKind           : kEventTupletBegin
          fTupletNumber              : T1
        ]


    --------

    fTupletsEndsList: 1 element, in note sequential number order
      Note 2:
        [TupletEvent
          fEventInputStartLineNumber : 59
          fEventSequentialNumber     : E2
          fNoteSequentialNumber      : N3
          fNoteEventStaffNumber      : S1
          fNoteEventVoiceNumber      : V1
          fTupletEventKind           : kEventTupletEnd
          fTupletNumber              : T1
        ]

  ]
*/

void mxsr2msrSkeletonPopulator::handleTupletBeginEventsIfAnyBeforeNoteCreation ()
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "--------> handleTupletBeginEventsIfAnyBeforeNoteCreation()";
//         ", tupletEventKind: " << tupletEventKind <<
//         ", tupletNumber: " << tupletNumber <<
//         ", line " << tupletEvent->getEventInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif
}

void mxsr2msrSkeletonPopulator::handleTupletBeginEventsIfAnyAfterNoteCreation ()
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "--------> handleTupletBeginEventsIfAnyBeforeNoteCreation()";
//         ", tupletEventKind: " << tupletEventKind <<
//         ", tupletNumber: " << tupletNumber <<
//         ", line " << tupletEvent->getEventInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

  std::list <S_mxsrTupletEvent> tupletBeginsList;

  fKnownEventsCollection.fetchTupletBeginsList (
    fCurrentNoteSequentialNumber,
    tupletBeginsList);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    gLog <<
      "--> tupletBeginsList contains " <<
      mfSingularOrPlural (
        tupletBeginsList.size (),
        "element",
        "elements") <<
      ", in note sequential number order" <<
      std::endl;

    ++gIndenter;

    for (S_mxsrTupletEvent tupletEvent : tupletBeginsList) {
      int
        eventSequentialNumber = tupletEvent->getEventSequentialNumber ();

      gLog <<
        "Note " << eventSequentialNumber <<
        ':' <<
        std::endl;

      ++gIndenter;
      gLog <<
        tupletEvent <<
        std::endl;
      --gIndenter;
    } // for

    --gIndenter;
  }
#endif

  // handling the tuplet begin events
  for (S_mxsrTupletEvent tupletEvent : tupletBeginsList) {
#ifdef MF_TRACE_IS_ENABLED
    mxsrTupletEventKind
      tupletEventKind =
        tupletEvent->getTupletEventKind ();

    int
      tupletNumber =
        tupletEvent->getTupletNumber ();

    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "--------> handleTupletBeginEventsIfAnyBeforeNoteCreation()" <<
        ", tupletEventKind: " << tupletEventKind <<
        ", tupletNumber: " << tupletNumber <<
        ", line " << tupletEvent->getEventInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

//     switch (tupletEventKind) {
//       case mxsrTupletEventKind::kEventTuplet_NONE:
//         // should not occur
//         break;
//
//       case mxsrTupletEventKind::kEventTupletBegin:
//         break;
//
//       case mxsrTupletEventKind::kEventTupletEnd:
//         fATupletIsEnding = true;
//         fEndingTupletNumber = tupletNumber;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceTupletsBasics ()) {
//           std::stringstream ss;
//
//           ss <<
//             "--------> handleTupletBeginEventsIfAnyBeforeNoteCreation(): tuplet number " <<
//             tupletNumber <<
//             " is ending" <<
//             ", line " << tupletEvent->getEventInputStartLineNumber ();
//
//           gWaeHandler->waeTrace (
//             __FILE__, __LINE__,
//             ss.str ());
//         }
// #endif
//         break;
//     } // switch

    // create the tuplet upon its first note
    S_msrTuplet
      tuplet =
        createTuplet (78978979);

    // populate it from its first note
    copyNoteElementsIfAnyToTuplet (
      fCurrentNote,
      tuplet);

    // append note as first note of tuplet
    tuplet->
      appendNoteToTuplet (
        fCurrentNote);

    // fetch the current voice
    S_msrVoice
      currentNoteVoice =
        fCurrentPartStaffMxsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
            getMsrVoice (); // JMI v0.9.72 ???

    // handle the tuplet start
    handleTupletStart (
      tuplet,
      currentNoteVoice);
  } // for
}

void mxsr2msrSkeletonPopulator::handleTupletBeginEventsAfterNoteIfAny ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "--------> handleTupletBeginEventsAfterNoteIfAny()";
//         ", tupletEventKind: " << tupletEventKind <<
//         ", tupletNumber: " << tupletNumber <<
//         ", line " << tupletEvent->getEventInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif
}

void mxsr2msrSkeletonPopulator::handleTupletEndEventsBeforeNoteIfAny ()
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "--------> handleTupletEndEventsBeforeNoteIfAny()";
//         ", tupletEventKind: " << tupletEventKind <<
//         ", tupletNumber: " << tupletNumber <<
//         ", line " << tupletEvent->getEventInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

  std::list <S_mxsrTupletEvent> tupletEndsList;

  fKnownEventsCollection.fetchTupletEndsList (
    fCurrentNoteSequentialNumber,
    tupletEndsList);

//     printTupletEventsList (
//       gLog,
//       noteSequentialNumber,
//       collectedEndsList,
//       "fetchTupletEndsList(), resultingEndsList:");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    gLog <<
      "--> tupletEndsList contains " <<
      mfSingularOrPlural (
        tupletEndsList.size (),
        "element",
        "elements") <<
      ", in note sequential number order" <<
      std::endl;

    ++gIndenter;

    for (S_mxsrTupletEvent tupletEvent : tupletEndsList) {
      int
        eventSequentialNumber = tupletEvent->getEventSequentialNumber ();

      gLog <<
        "Note " << eventSequentialNumber <<
        ':' <<
        std::endl;

      ++gIndenter;
      gLog <<
        tupletEvent <<
        std::endl;
      --gIndenter;
    } // for

    --gIndenter;
  }
#endif

  // is this note the last one in a tuplet?
  fATupletIsEnding = false;

  // handling the tuplet begin events
  for (S_mxsrTupletEvent tupletEvent : tupletEndsList) {
    mxsrTupletEventKind
      tupletEventKind =
        tupletEvent->getTupletEventKind ();

    int
      tupletNumber =
        tupletEvent->getTupletNumber ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "--------> handleTupletEndEventsBeforeNoteIfAny()" <<
        ", tupletEventKind: " << tupletEventKind <<
        ", tupletNumber: " << tupletNumber <<
        ", line " << tupletEvent->getEventInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

    switch (tupletEventKind) {
      case mxsrTupletEventKind::kEventTuplet_NONE:
        // should not occur
        break;

      case mxsrTupletEventKind::kEventTupletBegin:
        break;

      case mxsrTupletEventKind::kEventTupletEnd:
        fATupletIsEnding = true;
        fEndingTupletNumber = tupletNumber;

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTupletsBasics ()) {
          std::stringstream ss;

          ss <<
            "--------> handleTupletEndEventsBeforeNoteIfAny(): tuplet number " <<
            tupletNumber <<
            " is ending" <<
            ", line " << tupletEvent->getEventInputStartLineNumber ();

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        break;
    } // switch
  } // for
}

void mxsr2msrSkeletonPopulator::handleTupletEndEventsIfAnyAfterNoteHandling ()
{
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsBasics ()) {
      std::stringstream ss;

      ss <<
        "--------> handleTupletEndEventsIfAnyAfterNoteHandling()";
//         ", tupletEventKind: " << tupletEventKind <<
//         ", tupletNumber: " << tupletNumber <<
//         ", line " << tupletEvent->getEventInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

//   std::list <S_mxsrTupletEvent> tupletEndsList;
//
//   fKnownEventsCollection.fetchTupletEndsList (
//     fCurrentNoteSequentialNumber,
//     tupletEndsList);
//
// //     printTupletEventsList (
// //       gLog,
// //       noteSequentialNumber,
// //       collectedEndsList,
// //       "fetchTupletEndsList(), resultingEndsList:");
//
//   gLog <<
//     "--> tupletEndsList contains " <<
//     mfSingularOrPlural (
//       tupletEndsList.size (),
//       "element",
//       "elements") <<
//     ", in note sequential number order" <<
//     std::endl;
//
//   ++gIndenter;
//
//   for (S_mxsrTupletEvent tupletEvent : tupletEndsList) {
//     int
//       eventSequentialNumber = tupletEvent->getEventSequentialNumber ();
//
//     gLog <<
//       "Note " << eventSequentialNumber <<
//       ':' <<
//       std::endl;
//
//     ++gIndenter;
//     gLog <<
//       tupletEvent <<
//       std::endl;
//     --gIndenter;
//   } // for
//
//   --gIndenter;
//
//   // is this note the last one in a tuplet ?
//   fATupletIsEnding = false;
//
//   // handling the tuplet begin events
//   for (S_mxsrTupletEvent tupletEvent : tupletEndsList) {
//     mxsrTupletEventKind
//       tupletEventKind =
//         tupletEvent->getTupletEventKind ();
//
//     int
//       tupletNumber =
//         tupletEvent->getTupletNumber ();
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceTupletsBasics ()) {
//       std::stringstream ss;
//
//       ss <<
//         "--------> handleTupletEndEventsBeforeNoteIfAny()" <<
//         ", tupletEventKind: " << tupletEventKind <<
//         ", tupletNumber: " << tupletNumber <<
//         ", line " << tupletEvent->getEventInputStartLineNumber ();
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif
//
//     switch (tupletEventKind) {
//       case mxsrTupletEventKind::kEventTuplet_NONE:
//         // should not occur
//         break;
//
//       case mxsrTupletEventKind::kEventTupletBegin:
//         break;
//
//       case mxsrTupletEventKind::kEventTupletEnd:
//         fATupletIsEnding = true;
//         fEndingTupletNumber = tupletNumber;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceTupletsBasics ()) {
//           std::stringstream ss;
//
//           ss <<
//             "--------> handleTupletEndEventsBeforeNoteIfAny(): tuplet number " <<
//             tupletNumber <<
//             " is ending" <<
//             ", line " << tupletEvent->getEventInputStartLineNumber ();
//
//           gWaeHandler->waeTrace (
//             __FILE__, __LINE__,
//             ss.str ());
//         }
// #endif
//         break;
//     } // switch
//   } // for
}

void mxsr2msrSkeletonPopulator::finalizeTupletIfAny (
  int inputLineNumber)
{
  if (fCurrentNoteBelongsToATuplet) {
//     Bool aTupletIsStopping (false);
//
//     switch (fCurrentTupletTypeKind) {
//       case msrTupletTypeKind::kTupletTypeNone:
//         break;
//       case msrTupletTypeKind::kTupletTypeStart:
//         break;
//       case msrTupletTypeKind::kTupletTypeContinue:
//         break;
//       case msrTupletTypeKind::kTupletTypeStop:
//         aTupletIsStopping = true;
//         break;
//     } // switch

    if (fATupletIsEnding) {
      S_mxsrVoice
        theMxsrVoice =
          fCurrentPartStaffMxsrVoicesMap
            [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

      if (theMxsrVoice->getTupletsStackSize ()) { // JMI v0.9.71
        theMxsrVoice->
          finalizeTupletStackTopAndPopItFromTupletsStack (
            inputLineNumber,
            "finalizeTupletIfAny()");
      }
      else {
        std::stringstream ss;

        ss <<
          "==> tuplet stop with no corresponding tuplet start" <<
          ", on note: " << fCurrentNote->asString ();

        mxsr2msrError (
    //     mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
    }
  }
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitEnd (S_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  /*
    This is a complex method, due to the fact that
    dynamics, wedges, chords and tuplets
    are not ordered in the same way in MusicXML and LilyPond.

    Staff number is analyzed before voice number
    but occurs after it in the MusicXML tree.
    That's why the treatment below has been postponed until this method
  */

  /*
    Staff assignment is only needed for music notated on multiple staves.
    Used by both notes and directions.
    Staff values are numbers, with 1 referring to the top-most staff in a part.
  */

  // attach the pending dal segnos, if any,
  // to the ** previous ** note or chord
  // fetch current note's voice
  attachThePendingDalSegnosIfAny ();

  // staff changes handling
  if (fCurrentRecipientStaffNumber == K_STAFF_NUMBER_UNKNOWN_) {
    // we're at the beginning of the part of right after a <backup /> or <forward />,
    // so the new current recipient staff number is that of the current note
    fCurrentRecipientStaffNumber =
      fCurrentNoteStaffNumber;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChangesBasics ()) {
    std::stringstream ss;

    ss <<
      "--------> fCurrentRecipientStaffNumber visitEnd (S_note& elt): " <<
      "fCurrentNoteSequentialNumber: " << fCurrentNoteSequentialNumber <<
      "fCurrentRecipientStaffNumber: " << fCurrentRecipientStaffNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // remember current note as the previous measure element // JMI v0.9.67 LATER???
  fPreviousMeasureElement = fCurrentNote;

  // handle staff change take off if any,
  // in which case fCurrentRecipientStaffNumber will be updated
  handleStaffChangeTakeOffEventIfAnyBeforeNoteCreation ();

  // chords may be nested in tuplets, hence:
  // first, handle the tuplet begin events upon this note if any
  handleTupletBeginEventsIfAnyBeforeNoteCreation ();

  // then handle the chord begin event upon this note if any
  handleChordBeginIfAnyBeforeNoteCreation ();

  // create the note
  fCurrentNote =
    createNote (
      elt->getInputStartLineNumber ());

// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fCurrentNoteVoiceNumber != K_VOICE_NUMBER_UNKNOWN_,
//     "fCurrentNoteVoiceNumber is unknown");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

  handleTupletBeginEventsIfAnyAfterNoteCreation ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "==> fCurrentPartStaffMsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber] is now " <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->asShortString () <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    displayStaffAndVoiceInformation (
      elt->getInputStartLineNumber (),
      "mxsr2msrSkeletonPopulator::visitEnd (S_note& elt)");
  }
#endif // MF_TRACE_IS_ENABLED

  // the elements pending since before the note if any
  // can now be appended to the latter's voice
  // prior to the note itself
//   attachPendingVoiceLevelElementsToVoice (
//     fCurrentPartStaffMsrVoicesMap
//         [fCurrentNoteVoiceNumber]);

  attachPendingPartLevelElementsIfAnyToPart (
    fCurrentPart);

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // handle the current note and append it to the current recipient voice
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // are all fCurrentNote uplinks set alright ??? JMI v0.9.67
  populateCurrentNoteWithCurrentInformations (
    elt->getInputStartLineNumber ());

  handleCurrentNote (
    elt->getInputStartLineNumber ());

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // populate fCurrentNote before the note itself is handled
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // CAUTION JMI v0.9.66
  // permuted the order of populateNote() and handleCurrentNote()
  // to have fCurrentNote's harmonies list already populated if relevant
  // when fCurrentNote is appended to the voice,
  // so as to compute the harmonies positions in the measure.

//   // are all fCurrentNote uplinks set alright ??? JMI v0.9.66
//   populateCurrentNoteWithCurrentInformations ();

//   ////////////////////////////////////////////////////////////////////
//   ////////////////////////////////////////////////////////////////////
//   // handle fCurrentNote itself
//   ////////////////////////////////////////////////////////////////////
//   ////////////////////////////////////////////////////////////////////
//
//   handleCurrentNote (elt->getInputStartLineNumber ());

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // populate fCurrentNote after the note itself is handled
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // are all fCurrentNote uplinks set alright ??? JMI v0.9.66
  populateCurrentNoteAfterItHasBeenHandled (
    elt->getInputStartLineNumber ());

//   // attach the pending wedges, if any and relevant, to the note
//   if (fPendingVoiceWedgesList.size ()) {
//     attachPendingVoicesWedgesToCurrentNoteIfRelevant (
//       fCurrentNoteVoiceNumber);
//   }

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // handle the pending harmonies and figured basses if any JMI HARMFULL
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceTuplets ()
      ||
    gTraceOahGroup->getTraceHarmonies ()
  ) {
    std::stringstream ss;

    ss <<
      "==> fCurrentNote: ";
    if (fCurrentNote) {
      ss << fCurrentNote->asString ();
    }
    else {
      ss << "[NULL]";
    }
    ss << std::endl;

    ss <<
      "==> fCurrentOuterMostTupletFirstNote: ";
    if (fCurrentOuterMostTupletFirstNote) {
      ss << fCurrentOuterMostTupletFirstNote->asString ();
    }
    else {
      ss << "[NULL]";
    }
    ss << std::endl;

    ss <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the pending harmonies elements if any
  if (fPendingHarmoniesList.size ()) {
      handlePendingHarmonies ();
//     }
//     else {
//       // wait until the end of the outermost tuplet
//     }
  }

  // handle the pending figured basses if any
  if (fPendingFiguredBassesList.size ()) {
      handlePendingFiguredBasses ();
//     }
//     else {
//       // wait until the end of the outermost tuplet
//     }
  }

  // update current outermost tuplet relative offset if relevant
  if (! fCurrentNoteIsAGraceNote) {
    if (fCurrentOuterMostTuplet) {
//     if (fCurrentOuterMostTupletFirstNote)
      fCurrentOuterMostTupletRelativeOffset +=
        fCurrentNote->getMeasureElementSoundingWholeNotes ();
//         fCurrentNoteSoundingWholeNotes;

#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceTuplets ()
          ||
        gTraceOahGroup->getTraceHarmonies ()
      ) {
        std::stringstream ss;

        ss <<
          "==> fCurrentOuterMostTupletRelativeOffset becomes: " <<
          fCurrentOuterMostTupletRelativeOffset.asString () <<
          ", fCurrentNote" << fCurrentNote->asString ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());

        displayPendingHarmoniesList ("mxsr2msrSkeletonPopulator::visitEnd (S_note& elt)");
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // handle the tuplet stops for the note if any
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // is there a tuplet to be finalized? BANJO
  finalizeTupletIfAny (
    elt->getInputStartLineNumber ());

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // handle the lyrics for the note after the latter itself is handled if relevant
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // lyrics if any have to be handled in all cases
  // if they are not to be ignored

  // done only now because attachPendingNoteLevelElementsToNote() // JMI v0.9.67 HARMFUL
  // may append skip syllables to the notes // JMI v0.9.70
  if (! gGlobalMxsr2msrOahGroup->getIgnoreLyrics ()) {
    handleLyricsForCurrentNoteAfterItHasBeenHandled ();
  }

  // create a staff change if relevant
  if (fCurrentNoteStaffChangeTakeOff) {
    createStaffChange (
      elt->getInputStartLineNumber (),
      fCurrentNoteStaffChangeTakeOff);
  }

  // chords may be nested in tuplets, hence:
  // first, handle the chord end event upon this note if any
  // forget about the on-going chord if relevant
  handleChordEndIfAnyAfterNoteHandling ();

  // then, handle the tuplet end events upon this note if any
  handleTupletEndEventsIfAnyAfterNoteHandling ();

	// set current note MusicXML staff number as previous for the next note
  fPreviousNoteStaffNumber = fCurrentNoteStaffNumber;

  fOnGoingNote = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handlePendingHarmonies ()
{
  switch (fPendingHarmoniesList.size ()) {
    case 0:
      break;

    case 1:
      handlePendingSingleHarmony (
        fPendingHarmoniesList.front ());
      break;

    default:
      handlePendingMultipleHarmonies ();
      break;
  } //switch

  // forget about fCurrentNote's harmonies if any
  fPendingHarmoniesList.clear ();

  // reset voice harmonies counter
  fHarmoniesVoicesCounter = 0;
}

void mxsr2msrSkeletonPopulator::handlePendingSingleHarmony (
  const S_msrHarmony& harmony)
{
  msrWholeNotes
    currentNoteSoundingWholeNotes =
      fCurrentNote->
        getMeasureElementSoundingWholeNotes (),
    currentNoteDisplayWholeNotes =
      fCurrentNote->
        getNoteDisplayWholeNotes ();

  msrWholeNotes
    currentNoteMeasurePosition =
      fCurrentNote->
        getMeasureElementMeasurePosition ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Handling the single pending harmony " <<
      harmony->asString () <<
      " for note " <<
      fCurrentNote->asShortString () <<
      ", currentNoteSoundingWholeNotes: " << currentNoteSoundingWholeNotes.asFractionString () <<
      ", currentNoteDisplayWholeNotes: " << currentNoteDisplayWholeNotes.asFractionString () <<
      ", currentNoteMeasurePosition: " << currentNoteMeasurePosition.asString () <<
      ", line " << fCurrentNote->getInputStartLineNumber () <<
      std::endl;

    ss <<
      "fCurrentOuterMostTupletFirstNote: ";
    if (fCurrentOuterMostTupletFirstNote) {
      ss <<
        fCurrentOuterMostTupletFirstNote->asString ();
    }
    else {
      ss << "[NULL]";
    }
    ss << std::endl;

    ss <<
      "fCurrentOuterMostTuplet: ";
    if (fCurrentOuterMostTuplet) {
      ss <<
        fCurrentOuterMostTuplet->asString ();
    }
    else {
      ss << "[NULL]";
    }
    ss << std::endl;

    ss <<
      ", fCurrentOuterMostTupletRelativeOffset: " <<
      fCurrentOuterMostTupletRelativeOffset.asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    displayPendingHarmoniesList ("handlePendingSingleHarmony() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // set harmony's sounding whole notes
  harmony->
    setHarmonySoundingWholeNotes (
      fCurrentNote->getMeasureElementSoundingWholeNotes (),
      "handlePendingSingleHarmony() 2");

  // set harmony's display whole notes
  harmony->
    setHarmonyDisplayWholeNotes (
      fCurrentNote->getNoteDisplayWholeNotes (),
      "handlePendingSingleHarmony() 3");

  // set harmony's tuplet factor
  harmony->
    setHarmonyTupletFactor (
      msrTupletFactor (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes));

  // get harmony's whole notes offset
  msrWholeNotes
    harmonyWholeNotesOffset =
      harmony->
        getHarmonyWholeNotesOffset ();

  // append harmony to fCurrentPart
  if (fCurrentOuterMostTuplet) {
    msrWholeNotes
      currentTopLevelTupletMeasurePosition =
        fCurrentOuterMostTuplet->getMeasureElementMeasurePosition ();

    // compute harmony's measure position
    msrWholeNotes
      harmonyMeasurePosition =
        currentTopLevelTupletMeasurePosition
          +
        fCurrentOuterMostTupletRelativeOffset;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Appending the single pending harmony " <<
        harmony->asString () <<
        ", fCurrentOuterMostTupletRelativeOffset: " <<
        fCurrentOuterMostTupletRelativeOffset.asString () <<
        ", harmonyMeasurePosition: " <<
        harmonyMeasurePosition <<
        ", for note " <<
        fCurrentNote->asShortString () <<
        " to part " <<
        fCurrentPart->fetchPartCombinedName () <<
        ", line " << harmony->getInputStartLineNumber ()  <<
        std::endl;

      ss <<
        "fCurrentOuterMostTupletFirstNote: ";
      if (fCurrentOuterMostTupletFirstNote) {
        ss <<
          fCurrentOuterMostTupletFirstNote->asString ();
      }
      else {
        ss << "[NULL]";
      }
      ss << std::endl;

      ss <<
        "fCurrentOuterMostTuplet: ";
      if (fCurrentOuterMostTuplet) {
        ss <<
          fCurrentOuterMostTuplet->asString ();
      }
      else {
        ss << "[NULL]";
      }
      ss << std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());

  //     displayPendingHarmoniesList ("handlePendingSingleHarmony() 4");
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentPart->
      appendHarmonyToPart (
        fCurrentOuterMostTuplet->getInputStartLineNumber (),
        harmony,
        harmonyMeasurePosition);
  }

  else {
    // harmony is at the voice level JMI v0.9.67
    fCurrentPart->
      appendHarmonyToPart (
        fCurrentNote->getInputStartLineNumber (),
        harmony,
        fCurrentNote->getMeasureElementMeasurePosition ());
  }
}

void mxsr2msrSkeletonPopulator::handlePendingMultipleHarmonies ()
{
  msrWholeNotes
    currentNoteSoundingWholeNotes =
      fCurrentNote->
        getMeasureElementSoundingWholeNotes (),
    currentNoteDisplayWholeNotes =
      fCurrentNote->
        getNoteDisplayWholeNotes ();

  msrWholeNotes
    currentNoteMeasurePosition =
      fCurrentNote->
        getMeasureElementMeasurePosition ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Handling the " <<
      fPendingHarmoniesList.size () <<
      " pending harmonies for note " <<
      fCurrentNote->asShortString () <<
      ", currentNoteSoundingWholeNotes: " << currentNoteSoundingWholeNotes.asFractionString () <<
      ", currentNoteDisplayWholeNotes: " << currentNoteDisplayWholeNotes.asFractionString () <<
      ", currentNoteMeasurePosition: " << currentNoteMeasurePosition.asString () <<
      ", line " << fCurrentNote->getInputStartLineNumber () <<
      std::endl;

    ss <<
      "fCurrentOuterMostTuplet: ";
    if (fCurrentOuterMostTuplet) {
      ss <<
        fCurrentOuterMostTuplet->asString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      ", fCurrentOuterMostTupletRelativeOffset: " <<
      fCurrentOuterMostTupletRelativeOffset.asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    displayPendingHarmoniesList ("handlePendingMultipleHarmonies() 1");
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Sorting the " <<
      fPendingHarmoniesList.size () <<
      " harmonies for note " <<
      fCurrentNote->asShortString () <<
      " in offset relative order " <<
      ", line " << fCurrentNote->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // sort fPendingHarmoniesList by increasing offset,
  // just in case the harmonies are out of order
  fPendingHarmoniesList.sort (
    msrHarmony::compareHarmoniesByIncreasingOffset);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    displayPendingHarmoniesList ("handlePendingMultipleHarmonies() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  // compute the sounding whole notes of each harmony except the last one in the list
  // the offset delta between them gives their duration,
  // hence their position relative to the note
  // the first harmony in the list has position 0 relative to the note

  S_msrHarmony
    previousHarmony;

  msrWholeNotes
    previousWholeNotesOffsetInTheLoop =
      msrWholeNotes (0, 1);

  msrWholeNotes
    currentHarmonySoundingWholeNotes =
      msrWholeNotes (0, 1);

  msrWholeNotes
    currentNoteRelativePosition =
      msrWholeNotes (0, 1);

  // let's go for the first to the next to last harmonies in the list
  // ----------------------------------------------

  for (S_msrHarmony currentHarmony : fPendingHarmoniesList) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "--> handlePendingHarmony, " <<
        ", currentHarmony: " << currentHarmony->asString () <<
        ", line " << currentHarmony->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // get currentHarmony's whole notes offset
    msrWholeNotes
      currentHarmonyWholeNotesOffset =
        currentHarmony->
          getHarmonyWholeNotesOffset ();

    if (previousHarmony) {
      // compute the offset delta
      msrWholeNotes
        offsetDelta =
          currentHarmonyWholeNotesOffset - previousWholeNotesOffsetInTheLoop;

      // compute the currentHarmony's sounding whole notes
      // as a fraction of currentNoteSoundingWholeNotes
      mfRational
        fraction (
          offsetDelta / currentNoteSoundingWholeNotes);

      // set the previous harmony's sounding whole notes
      msrWholeNotes
        currentHarmonySoundingWholeNotes =
          currentNoteSoundingWholeNotes * fraction;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceHarmonies ()) {
        std::stringstream ss;

        ss <<
          "--> handlePendingHarmony, " <<
          ", currentHarmonySoundingWholeNotes: " << currentHarmonySoundingWholeNotes.asFractionString () <<
          ", currentHarmonyWholeNotesOffset: " << currentHarmonyWholeNotesOffset <<
          ", previousWholeNotesOffsetInTheLoop: " << previousWholeNotesOffsetInTheLoop <<
          ", offsetDelta: " << offsetDelta <<
          ", currentNoteSoundingWholeNotes: " << currentNoteSoundingWholeNotes.asFractionString () <<
          ", fraction: " << fraction <<
          ", line " << currentHarmony->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      previousHarmony->
        setHarmonySoundingWholeNotes (
          currentHarmonySoundingWholeNotes,
          "mxsr2msrSkeletonPopulator::handlePendingMultipleHarmonies() 3, first to next to last harmony");

      // remember the currentHarmony's whole notes offset as previous
      previousWholeNotesOffsetInTheLoop = currentHarmonyWholeNotesOffset;
    }

    // set the currentHarmony's display whole notes JMI useless??? v0.9.66
    currentHarmony->
      setHarmonyDisplayWholeNotes (
        currentNoteDisplayWholeNotes,
        "mxsr2msrSkeletonPopulator::handlePendingMultipleHarmonies() 4, first to next to last harmony");

    // set the currentHarmony's tuplet factor // JMI v0.9.67
    currentHarmony->
      setHarmonyTupletFactor (
        msrTupletFactor (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes));

    // take the harmony into account
    currentNoteRelativePosition +=
      currentHarmonySoundingWholeNotes;

    // go one list element ahead
    previousHarmony = currentHarmony;
  } // for

  // let's go for the last harmony in the list
  // ----------------------------------------------

  // here, previousHarmony contains the last one in the list
  S_msrHarmony
    lastHarmony = previousHarmony;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    gLog << "=====> now handling the last harmony in the list" << std::endl;

    std::stringstream ss;

    ss <<
      "--> handlePendingHarmony, " <<
      ", lastHarmony: " << lastHarmony->asString () <<
      ", line " << lastHarmony->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get lastHarmony's whole notes offset
  msrWholeNotes
    lastHarmonyWholeNotesOffset =
      lastHarmony->
        getHarmonyWholeNotesOffset ();

  // compute the offset delta
  msrWholeNotes
    offsetDelta =
      currentNoteSoundingWholeNotes - previousWholeNotesOffsetInTheLoop;

  // compute the lastHarmony's sounding whole notes
  // as a fraction of currentNoteSoundingWholeNotes
  mfRational
    fraction (
      offsetDelta / currentNoteSoundingWholeNotes);

  // set the sounding whole notes of the last harmony in the list
  msrWholeNotes
    lastHarmonySoundingWholeNotes =
      currentNoteSoundingWholeNotes * fraction;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "--> handlePendingHarmony, LAST HARMONY OF THE LIST" <<
      ", lastHarmonySoundingWholeNotes: " << lastHarmonySoundingWholeNotes.asFractionString () <<
      ", lastHarmonyWholeNotesOffset: " << lastHarmonyWholeNotesOffset <<
      ", previousWholeNotesOffsetInTheLoop: " << previousWholeNotesOffsetInTheLoop <<
      ", currentNoteSoundingWholeNotes: " << currentNoteSoundingWholeNotes.asFractionString () <<
      ", offsetDelta: " << offsetDelta <<
      ", fraction: " << fraction <<
      ", line " << lastHarmony->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  lastHarmony->
    setHarmonySoundingWholeNotes (
      lastHarmonySoundingWholeNotes,
      "mxsr2msrSkeletonPopulator::handlePendingMultipleHarmonies() 5, last harmony in the list");

  // set the display whole notes of the last harmony in the list
  lastHarmony->
    setHarmonyDisplayWholeNotes (
      currentNoteDisplayWholeNotes,
      "mxsr2msrSkeletonPopulator::handlePendingMultipleHarmonies() 6, last harmony in the list");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    displayPendingHarmoniesList ("mxsr2msrSkeletonPopulator::handlePendingMultipleHarmonies() 7");
  }
#endif // MF_TRACE_IS_ENABLED

  // append the figured basses list to current part
  if (fCurrentOuterMostTuplet) {
    fCurrentPart->
      appendHarmoniesListToPart (
        fCurrentOuterMostTuplet->getInputStartLineNumber (),
        fPendingHarmoniesList,
        fCurrentOuterMostTuplet->getMeasureElementMeasurePosition ());
  }
  else {
    fCurrentPart->
      appendHarmoniesListToPart (
        fCurrentNote->getInputStartLineNumber (),
        fPendingHarmoniesList,
        fCurrentNote->getMeasureElementMeasurePosition ());
  }
}

void mxsr2msrSkeletonPopulator::displayPendingHarmoniesList (
  std::string context)
{
  gLog <<
    std::endl <<
    ">>> +++++++++++++++++++++++++++++++++++++++++++++++++++++ >>>" <<
    std::endl <<
    "Pending harmonies list -- " <<
    "context: " << context <<
    ", fCurrentOuterMostTupletRelativeOffset: " <<
    fCurrentOuterMostTupletRelativeOffset.asString () <<
    ':' <<
    std::endl;

  ++gIndenter;

  for (S_msrHarmony harmony : fPendingHarmoniesList) {
    gLog << harmony->asString () << std::endl;
  } // for

  --gIndenter;

  gLog <<
    "<<< +++++++++++++++++++++++++++++++++++++++++++++++++++++ <<<" <<
    std::endl <<
    std::endl;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handlePendingFiguredBasses ()
{
  switch (fPendingFiguredBassesList.size ()) {
    case 0:
      break;

    case 1:
      handlePendingSingleFiguredBass (
        fPendingFiguredBassesList.front ());
      break;

    default:
      handlePendingMultipleFiguredBasses ();
      break;
  } //switch

  // forget about fCurrentNote's figured basses if any
  fPendingFiguredBassesList.clear ();

  // reset figured basses counter
  fFiguredBassVoicesCounter = 0;
}

void mxsr2msrSkeletonPopulator::handlePendingSingleFiguredBass (
  const S_msrFiguredBass& figuredBass)
{
  // set figuredBass's sounding whole notes
  figuredBass->
    setFiguredBassSoundingWholeNotes (
      fCurrentNote->getMeasureElementSoundingWholeNotes (),
      "handlePendingSingleFiguredBass ()");

  // set figuredBass's display whole notes
  figuredBass->
    setFiguredBassDisplayWholeNotes ( // JMI useless??? v0.9.66
      fCurrentNote->getNoteDisplayWholeNotes ());

  // set figuredBass's tuplet factor
  figuredBass->
    setFiguredBassTupletFactor (
      msrTupletFactor (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes));

//   // handle the pending figured bass elements if any
//   if (fCurrentNote == fCurrentOuterMostTupletFirstNote) {
//     if (fPendingFiguredBassesList.size ()) {
//       handlePendingFiguredBasses ();
//     }
//   }

  // append figuredBass to fCurrentPart
  fCurrentPart->
    appendFiguredBassToPart (
      fCurrentNote->getInputStartLineNumber (),
      figuredBass,
      fCurrentNote->getMeasureElementMeasurePosition ());
}

void mxsr2msrSkeletonPopulator::handlePendingMultipleFiguredBasses ()
{
  msrWholeNotes
    currentNoteSoundingWholeNotes =
      fCurrentNote->
        getMeasureElementSoundingWholeNotes (),
    currentNoteDisplayWholeNotes =
      fCurrentNote->
        getNoteDisplayWholeNotes ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Handling the " <<
      fPendingHarmoniesList.size () <<
      " pending figured basses for note " <<
      fCurrentNote->asShortString () <<
      ", currentNoteSoundingWholeNotes: " << currentNoteSoundingWholeNotes.asFractionString () <<
      ", currentNoteDisplayWholeNotes: " << currentNoteDisplayWholeNotes.asFractionString () <<
      ", line " << fCurrentNote->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    displayPendingFiguredBassesList ("handlePendingMultipleFiguredBasses() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrFiguredBass currentFiguredBass : fPendingFiguredBassesList) {
    // get currentFiguredBass's whole notes duration
    msrWholeNotes
      figuredBassWholeNotesDuration =
        currentFiguredBass->
//           getFiguredBassWholeNotesDuration ();
          getMeasureElementSoundingWholeNotes ();

    // compute the currentFiguredBass's sounding whole notes
    // as a fraction of currentNoteSoundingWholeNotes
    msrWholeNotes
      figuredBassSoundingWholeNotes;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceFiguredBasses ()) {
      std::stringstream ss;

      ss <<
        "handlePendingFiguredBass, " <<
        ", figuredBassSoundingWholeNotes: " << figuredBassSoundingWholeNotes.asFractionString () <<
        ", figuredBassWholeNotesDuration: " << figuredBassWholeNotesDuration <<
        ", line " << currentFiguredBass->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // set currentFiguredBass's sounding whole notes
    currentFiguredBass->
      setFiguredBassSoundingWholeNotes (
        figuredBassSoundingWholeNotes,
        "mxsr2msrSkeletonPopulator::handlePendingMultipleFiguredBasses() 3");

    // set the currentFiguredBass's display whole notes
    currentFiguredBass->
      setFiguredBassDisplayWholeNotes (
        currentNoteDisplayWholeNotes);

    // set the currentFiguredBass's tuplet factor // JMI v0.9.67
    currentFiguredBass->
      setFiguredBassTupletFactor (
        msrTupletFactor (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes));
  } // for

  // append the figured basses list to current part
  fCurrentPart->
    appendFiguredBassesListToPart (
      fCurrentNote->getInputStartLineNumber (),
      fPendingFiguredBassesList,
      fCurrentNote->getMeasureElementMeasurePosition ());

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    displayPendingHarmoniesList ("handlePendingMultipleFiguredBasses() 3");
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::displayPendingFiguredBassesList (
  std::string context)
{
  gLog <<
    "Pending figured basses list -- " <<
    "context: " << context <<
    ':' <<
    std::endl;

  ++gIndenter;

  for (S_msrFiguredBass figuredBass : fPendingFiguredBassesList) {
    gLog << figuredBass->asString () << std::endl;
  } // for

  --gIndenter;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleSoundingNote (
  const S_msrNote& note)
{
  ++gIndenter;

  // register note/rest kind right now, to have a nice trace below
  if (fCurrentNoteIsAGraceNote) { // JMI ??? v0.9.72 should not occur here???
    // gracenote
    fCurrentNote->
      setNoteKind (
        msrNoteKind::kNoteRegularInGraceNotesGroup);
  }

  else {
    // non-grace note
//     fCurrentNonGraceNote = fCurrentNote;
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceNotes ()) { // JMI
//       std::stringstream ss;
//
//       ss <<
//         "handleSoundingNote(), fCurrentNonGraceNote: " <<
//         fCurrentNonGraceNote->asShortString ();
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED

    if (
      fCurrentDoubleTremoloTypeKind == msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart
        ||
      fCurrentDoubleTremoloTypeKind == msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop
    ) {
      // double tremolo note
      fCurrentNote->
        setNoteKind (
          msrNoteKind::kNoteInDoubleTremolo);
    }

    else {
      // regular or unpitched note or rest
      if (fCurrentNoteIsARest) { // JMI ??? v0.9.72 should not occur here???
        fCurrentNote->
          setNoteKind (
            msrNoteKind::kNoteRestInMeasure);
      }
      else if (fCurrentNoteIsUnpitched) {
        fCurrentNote->
          setNoteKind (
            msrNoteKind::kNoteUnpitchedInMeasure);
      }
      else {
        fCurrentNote->
          setNoteKind (
            msrNoteKind::kNoteRegularInMeasure);
      }
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) { // JMI
    gLog <<
      "handleSoundingNote(), fCurrentNote: " <<
      std::endl;
    ++gIndenter;
    gLog <<
      fCurrentNote <<
      std::endl;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling non-chord, non-tuplet note or rest " <<
       fCurrentNote->asShortString () << // NO, would lead to infinite recursion ??? JMI
      ", fCurrentPartStaffMsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]: \"" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName () <<
      "\", line " << fCurrentNote->getInputStartLineNumber () <<
      ":" <<
      std::endl;

    ++gIndenter;

    gLog <<
      fCurrentNote->asString () <<
      std::endl;

    constexpr int fieldWidth = 25;

    gLog << std::left <<
      std::setw (fieldWidth) << "voice" << ": \"" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName () <<
      "\"" <<
      std::endl <<
      std::setw (fieldWidth) << "line:" << ": " <<
      fCurrentNote->getInputStartLineNumber () <<
      std::endl <<
      std::setw (fieldWidth) << "fCurrentNoteIsAGraceNote" << ": " <<
      fCurrentNoteIsAGraceNote <<
      std::endl;

    if (gTraceOahGroup->getTraceNotesDetails ()) {
      std::stringstream ss;

      ss <<
        std::setw (fieldWidth) << "fPendingGraceNotesGroup" << ": " <<
        std::endl <<
        "======================= handleSoundingNote()" <<
        ", line " << fCurrentNote->getInputStartLineNumber () <<
        std::endl;
      fCurrentPart->print (gLog);
      gLog <<
        "=======================" <<
        std::endl << std::endl;

      if (fPendingGraceNotesGroup) {
        gLog <<
          fPendingGraceNotesGroup;
      }
      else {
 //       gLog <<
 //         "fCurrentGraceNotesGroupNotes is NULL"; // JMI
      }

      gLog << std::endl;
    }

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsAGraceNote) {
    if (! fPendingGraceNotesGroup) {
      // this is the first grace note in a grace notes group

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceGraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Creating grace notes for note " <<
          fCurrentNote->asString () <<
          " in voice \"" <<
          fCurrentPartStaffMsrVoicesMap
            [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
              getVoiceName () <<
          "\"";

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // create grace notes group
      fPendingGraceNotesGroup =
        msrGraceNotesGroup::create (
          fCurrentNote->getInputStartLineNumber (),
          msrGraceNotesGroupKind::kGraceNotesGroupBefore, // default value
          fCurrentGraceNotesGroupIsSlashed,
          fCurrentGraceNotesGroupIsBeamed,
          fCurrentGraceNotesGroupIsTied,
          fCurrentGraceNotesGroupIsSlurred,
          fCurrentMeasureNumber);

      // should all grace notes be slurred?
      if (gGlobalMxsr2msrOahGroup->getSlurAllGraceNotes ()) {
        fPendingGraceNotesGroup->
          setGraceNotesGroupIsTied ();
      }

/* JMI
      // register that last handled note if any is followed by grace notes
      const S_msrNote&
        lastHandledNoteInVoice =
          fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteVoiceNumber]->
            getVoiceLastAppendedNote ();

      if (lastHandledNoteInVoice) {
        lastHandledNoteInVoice->
          setNoteIsFollowedByGraceNotes ();
      }
*/

      // append the grace notes to the current voice // NO JMI
      /*
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteVoiceNumber]->
        appendGraceNotesToVoice (
          fCurrentGraceNotesGroupNotes);
        //  */
    }

    // register that last handled note if any is followed by grace notes JMI ???
    S_msrNote
      lastHandledNoteInVoice =
        fCurrentPartStaffMsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
            getVoiceLastAppendedNote ();

    if (lastHandledNoteInVoice) {
      lastHandledNoteInVoice->
        setNoteIsFollowedByGraceNotesGroup ();
    }

    // append fCurrentNote to the current grace notes
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Appending note " <<
        fCurrentNote->asString () <<
        " to grace notes group in voice \"" <<
        fCurrentPartStaffMsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
            getVoiceName () <<
        "\", line " << fCurrentNote->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // append fCurrentNote to the pending grace notes group
    fPendingGraceNotesGroup->
      appendNoteToGraceNotesGroup (
      	fCurrentNote);
  }

  else {
    // current note is no grace note

    if (
      fCurrentDoubleTremoloTypeKind != msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_
    ) {
      // fCurrentNote belongs to a tremolo

      switch (fCurrentDoubleTremoloTypeKind) {
        case msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_:
          // just to avoid a compiler message
          break;

        case msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle:
          // append fCurrentNote to the current voice
  #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Appending single tremolo " <<
              fCurrentNote->asString () <<
              ", line " << fCurrentNote->getInputStartLineNumber () <<
              ", to voice \"" <<
              fCurrentPartStaffMsrVoicesMap
                [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
                  getVoiceName () <<
              "\"" <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
  #endif // MF_TRACE_IS_ENABLED

          fCurrentPartStaffMsrVoicesMap
            [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
              appendNoteToVoice (fCurrentNote);

          // fCurrentSingleTremolo is handled in
          // attachCurrentSingleTremoloToNote()
          break;

        case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart:
          // register fCurrentNote as first element of the current double tremolo
  #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Setting regular note " <<
              fCurrentNote->asString () <<
              ", line " << fCurrentNote->getInputStartLineNumber () <<
              ", as double tremolo first element" <<
              " in voice \"" <<
              fCurrentPartStaffMsrVoicesMap
                [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
                  getVoiceName () <<
              "\"" <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
  #endif // MF_TRACE_IS_ENABLED

          fCurrentDoubleTremolo->
            setDoubleTremoloNoteFirstElement (
              fCurrentNote);
          break;

        case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop:
          // register fCurrentNote as second element of the current double tremolo
  #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Setting regular note " <<
              fCurrentNote->asString () <<
              ", line " << fCurrentNote->getInputStartLineNumber () <<
              ", as double tremolo second element" <<
              " in voice \"" <<
              fCurrentPartStaffMsrVoicesMap
                [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
                  getVoiceName () <<
              "\"" <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, __LINE__,
              ss.str ());
          }
  #endif // MF_TRACE_IS_ENABLED

          fCurrentDoubleTremolo->
            setDoubleTremoloNoteSecondElement (
              fCurrentNote);

          // append current double tremolo to current voice
          fCurrentPartStaffMsrVoicesMap
            [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
              appendDoubleTremoloToVoice (
                fCurrentDoubleTremolo);

          // forget about the current double tremolo
         // fCurrentDoubleTremolo = 0; // JMI not if there's a chord in the double tremolo XXL BOF
          break;
      } // switch
    }

    else {
      // current note is a regular note or a rest

      // append fCurrentNote to the current voice to append to
  #ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Appending regular note or rest " <<
          fCurrentNote->asString () <<
          ", line " << fCurrentNote->getInputStartLineNumber () <<
          ", to voice \"" <<
          fCurrentPartStaffMsrVoicesMap
            [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
              getVoiceName () <<
          "\"";

        ss <<
          ", fCurrentRecipientStaffNumber: " << fCurrentRecipientStaffNumber <<
          ", fCurrentNoteVoiceNumber: " << fCurrentNoteVoiceNumber;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
  #endif // MF_TRACE_IS_ENABLED

      ++gIndenter;

//   gLog << "--------}> fCurrentRecipientStaffNumber: " << fCurrentRecipientStaffNumber << std::endl;

      fCurrentPartStaffMsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]->
          appendNoteToVoice (fCurrentNote);

      if (false) { // XXL, syllable sans fSyllableNote assigne JMI v0.9.70
        gLog <<
          "&&&&&&&&&&&&&&&&&& fCurrentPartStaffMsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber] (" <<
          fCurrentPartStaffMsrVoicesMap
            [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
              getVoiceName () <<
          ") contents &&&&&&&&&&&&&&&&&&" <<
          std::endl <<
          fCurrentPartStaffMsrVoicesMap
            [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber] <<
          std::endl;
      }

      --gIndenter;
    }
  }

  // take care of slurs JMI ???
  switch (fCurrentSlurTypeKind) {
    case msrSlurTypeKind::kSlurTypeRegularStart:
    case msrSlurTypeKind::kSlurTypePhrasingStart:
      fFirstSyllableInSlurKind = fCurrentSyllableKind;
      break;

    case msrSlurTypeKind::kSlurTypeRegularContinue:
      break;
    case msrSlurTypeKind::kSlurTypePhrasingContinue:
      break;

    case msrSlurTypeKind::kSlurTypeRegularStop:
    case msrSlurTypeKind::kSlurTypePhrasingStop:
      fFirstSyllableInSlurKind = msrSyllableKind::kSyllableNone;
      break;
    case msrSlurTypeKind::kSlurType_UNKNOWN_:
      ;
  } // switch

  // take care of ligatures JMI ???
  switch (fCurrentLigatureKind) {
    case msrLigatureKind::kLigatureStart:
      fFirstSyllableInLigatureKind = fCurrentSyllableKind;
      break;
    case msrLigatureKind::kLigatureContinue:
      break;
    case msrLigatureKind::kLigatureStop:
      fFirstSyllableInLigatureKind = msrSyllableKind::kSyllableNone;
      break;
    default:
      ;
  } // switch

  --gIndenter;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleRest (
  const S_msrNote& note)
{
  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) { // JMI
    std::stringstream ss;

    ss <<
      "handleRest(), note: " <<
      note->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   fCurrentNonGraceNote = note;

  note->
    setNoteKind (
      msrNoteKind::kNoteRestInMeasure);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) { // JMI
    gLog <<
      "handleSoundingNote(), note: " <<
      std::endl;
    ++gIndenter;
    gLog <<
      note <<
      std::endl;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling rest rest " <<
       note->asShortString () << // NO, would lead to infinite recursion ??? JMI
      ", fCurrentPartStaffMsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]: \"" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName () <<
      "\", line " << note->getInputStartLineNumber () <<
      ":" <<
      std::endl;

    ++gIndenter;

    gLog <<
      note->asString () <<
      std::endl;

    constexpr int fieldWidth = 25;

    gLog << std::left <<
      std::setw (fieldWidth) << "voice" << ": \"" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName () <<
      "\"" <<
      std::endl <<
      std::setw (fieldWidth) << "line:" << ": " <<
      note->getInputStartLineNumber () <<
      std::endl;

    if (gTraceOahGroup->getTraceNotesDetails ()) {
      std::stringstream ss;

      ss <<
        std::setw (fieldWidth) << "fPendingGraceNotesGroup" << ": " <<
        std::endl <<
        "======================= handleSoundingNote()" <<
        ", line " << note->getInputStartLineNumber () <<
        std::endl;
      fCurrentPart->print (gLog);
      gLog <<
        "=======================" <<
        std::endl << std::endl;

      gLog << std::endl;
    }

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // append note to the current voice to append to
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Appending regular note or rest " <<
        note->asString () <<
        ", line " << note->getInputStartLineNumber () <<
        ", to voice \"" <<
        fCurrentPartStaffMsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
            getVoiceName () <<
        "\"";

      ss <<
        ", fCurrentRecipientStaffNumber: " << fCurrentRecipientStaffNumber <<
        ", fCurrentNoteVoiceNumber: " << fCurrentNoteVoiceNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

//   gLog << "--------}> fCurrentRecipientStaffNumber: " << fCurrentRecipientStaffNumber << std::endl;

  fCurrentPartStaffMsrVoicesMap
    [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]->
      appendNoteToVoice (note);

  if (false) { // XXL, syllable sans fSyllableNote assigne JMI v0.9.70
    gLog <<
      "&&&&&&&&&&&&&&&&&& fCurrentPartStaffMsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber] (" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName () <<
      ") contents &&&&&&&&&&&&&&&&&&" <<
      std::endl <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber] <<
      std::endl;
  }

  --gIndenter;

  // take care of slurs JMI ???
  switch (fCurrentSlurTypeKind) {
    case msrSlurTypeKind::kSlurTypeRegularStart:
    case msrSlurTypeKind::kSlurTypePhrasingStart:
      fFirstSyllableInSlurKind = fCurrentSyllableKind;
      break;

    case msrSlurTypeKind::kSlurTypeRegularContinue:
      break;
    case msrSlurTypeKind::kSlurTypePhrasingContinue:
      break;

    case msrSlurTypeKind::kSlurTypeRegularStop:
    case msrSlurTypeKind::kSlurTypePhrasingStop:
      fFirstSyllableInSlurKind = msrSyllableKind::kSyllableNone;
      break;
    case msrSlurTypeKind::kSlurType_UNKNOWN_:
      ;
  } // switch

  // take care of ligatures JMI ???
  switch (fCurrentLigatureKind) {
    case msrLigatureKind::kLigatureStart:
      fFirstSyllableInLigatureKind = fCurrentSyllableKind;
      break;
    case msrLigatureKind::kLigatureContinue:
      break;
    case msrLigatureKind::kLigatureStop:
      fFirstSyllableInLigatureKind = msrSyllableKind::kSyllableNone;
      break;
    default:
      ;
  } // switch

  --gIndenter;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleGraceNote (
  const S_msrNote& note)
{
  ++gIndenter;

  // register note/rest kind right now, to have a nice trace below
  note->
    setNoteKind (
      msrNoteKind::kNoteRegularInGraceNotesGroup);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) { // JMI
    gLog <<
      "handleGraceNote(), note: " <<
      std::endl;
    ++gIndenter;
    gLog <<
      note <<
      std::endl;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling grace note " <<
       note->asShortString () << // NO, would lead to infinite recursion ??? JMI
      ", fCurrentPartStaffMsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]: \"" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName () <<
      "\", line " << note->getInputStartLineNumber () <<
      ":" <<
      std::endl;

    ++gIndenter;

    gLog <<
      note->asString () <<
      std::endl;

    constexpr int fieldWidth = 25;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "voice" << ": \"" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->getVoiceName () <<
      "\"" <<
      std::endl <<
      std::setw (fieldWidth) << "line:" << ": " <<
      note->getInputStartLineNumber () <<
      std::endl <<
      std::setw (fieldWidth) << "fCurrentNoteIsAGraceNote" << ": " <<
      fCurrentNoteIsAGraceNote <<
      std::endl;

    if (gTraceOahGroup->getTraceNotesDetails ()) {
      std::stringstream ss;

      ss <<
        std::setw (fieldWidth) << "fPendingGraceNotesGroup" << ": " <<
        std::endl <<
        "======================= handleGraceNote()" <<
        ", line " << note->getInputStartLineNumber () <<
        std::endl;
      fCurrentPart->print (gLog);
      gLog <<
        "=======================" <<
        std::endl << std::endl;

      if (fPendingGraceNotesGroup) {
        gLog <<
          fPendingGraceNotesGroup;
      }
      else {
 //       gLog <<
 //         "fCurrentGraceNotesGroupNotes is NULL"; // JMI
      }

      gLog << std::endl;
    }

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fPendingGraceNotesGroup) {
    // this is the first grace note in a grace notes group

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Creating grace notes group upon its first note " <<
        note->asString () <<
        " in voice \"" <<
        fCurrentPartStaffMsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
            getVoiceName () << "\"";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create grace notes group
    fPendingGraceNotesGroup =
      msrGraceNotesGroup::create (
        note->getInputStartLineNumber (),
        msrGraceNotesGroupKind::kGraceNotesGroupBefore, // default value
        fCurrentGraceNotesGroupIsSlashed,
        fCurrentGraceNotesGroupIsBeamed,
        fCurrentGraceNotesGroupIsTied,
        fCurrentGraceNotesGroupIsSlurred,
        fCurrentMeasureNumber);

    // should all grace notes be slurred?
    if (gGlobalMxsr2msrOahGroup->getSlurAllGraceNotes ()) {
      fPendingGraceNotesGroup->
        setGraceNotesGroupIsTied ();
    }

/* JMI
    // register that last handled note if any is followed by grace notes
    const S_msrNote&
      lastHandledNoteInVoice =
        fCurrentPartStaffMsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
            getVoiceLastAppendedNote ();

    if (lastHandledNoteInVoice) {
      lastHandledNoteInVoice->
        setNoteIsFollowedByGraceNotes ();
    }
*/

    // append the grace notes to the current voice // NO JMI
    /*
    fCurrentPartStaffMsrVoicesMap
      [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
        appendGraceNotesToVoice (
          fCurrentGraceNotesGroupNotes);
      //  */
  }

  // register that last handled note if any is followed by grace notes JMI ???
  S_msrNote
    lastHandledNoteInVoice =
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceLastAppendedNote ();

  if (lastHandledNoteInVoice) {
    lastHandledNoteInVoice->
      setNoteIsFollowedByGraceNotesGroup ();
  }

  // append note to the current grace notes
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Appending grace note " <<
      note->asString () <<
      " to grace notes group in voice \"" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName () <<
      "\", line " << note->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append note to the pending grace notes group
  fPendingGraceNotesGroup->
    appendNoteToGraceNotesGroup (
      note);

  // take care of slurs JMI ??? v0.9.72
  switch (fCurrentSlurTypeKind) {
    case msrSlurTypeKind::kSlurTypeRegularStart:
    case msrSlurTypeKind::kSlurTypePhrasingStart:
      fFirstSyllableInSlurKind = fCurrentSyllableKind;
      break;

    case msrSlurTypeKind::kSlurTypeRegularContinue:
      break;
    case msrSlurTypeKind::kSlurTypePhrasingContinue:
      break;

    case msrSlurTypeKind::kSlurTypeRegularStop:
    case msrSlurTypeKind::kSlurTypePhrasingStop:
      fFirstSyllableInSlurKind = msrSyllableKind::kSyllableNone;
      break;
    case msrSlurTypeKind::kSlurType_UNKNOWN_:
      ;
  } // switch

  // take care of ligatures JMI ???
  switch (fCurrentLigatureKind) {
    case msrLigatureKind::kLigatureStart:
      fFirstSyllableInLigatureKind = fCurrentSyllableKind;
      break;
    case msrLigatureKind::kLigatureContinue:
      break;
    case msrLigatureKind::kLigatureStop:
      fFirstSyllableInLigatureKind = msrSyllableKind::kSyllableNone;
      break;
    default:
      ;
  } // switch

  --gIndenter;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleLyricsForCurrentNoteAfterItHasBeenHandled ()
{
  int currentNoteInputLineNumber =
    fCurrentNote->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Handling lyrics for note " <<
      fCurrentNote->asShortString () <<
      ", line " << currentNoteInputLineNumber <<
      std::endl;

    ++gIndenter;

    constexpr int fieldWidth = 37;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentPartStaffMsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]" <<
      " = \"" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName () <<
      "\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNote" << " = \"" << fCurrentNote->asShortString () << "\"" <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fLastHandledNoteInVoiceHasLyrics" << ": " <<
      fLastHandledNoteInVoiceHasLyrics <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentSyllableExtendKind" << "" << ": " <<
      fCurrentSyllableExtendKind <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentSyllableElementsList.size ()" << ": " <<
      fCurrentSyllableElementsList.size () <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentStanzaNumber" << ": " << fCurrentStanzaNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStanzaName" << ": " << fCurrentStanzaName << "\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentSyllableElementsList" << ": " <<
      syllableElementsListAsString (fCurrentSyllableElementsList) <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentSyllableElementsList.size ()) {
    // fCurrentNote has lyrics attached to it

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyricsDetails ()) {
      std::stringstream ss;

      ss <<
        "Note " <<
        fCurrentNote->asShortString () <<
        " has lyrics attached to it" << " ***";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    for (S_msrSyllable syllable : fCurrentNoteSyllablesList) {
      // append syllable to currentNote and set upLink to it
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "*** ---> for (S_msrSyllable syllable : fCurrentNoteSyllablesList) {" <<
        ", syllable: " << syllable;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

      syllable->
        appendSyllableToNoteAndSetItsUpLinkToNote (
        	fCurrentNote);
    } // for

    // forget about all of fCurrentNote's syllables
    fCurrentNoteSyllablesList.clear ();
  }

  else {
    // fCurrentNote has no lyrics attached to it

    // don't create a skip for chord note members except the first one
    // nor for grace notes JMI ??? v0.9.70

    if (! (fCurrentNoteBelongsToAChord || fCurrentNoteIsAGraceNote)) {
      // get the current note voice's stanzas map
      const std::map <std::string, S_msrStanza>&
        voiceStanzasMap =
          fCurrentPartStaffMsrVoicesMap
            [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
              getVoiceStanzasMap ();

      for (
        std::map <std::string, S_msrStanza>::const_iterator i = voiceStanzasMap.begin ();
        i != voiceStanzasMap.end ();
        ++i
      ) {
        S_msrStanza stanza = (*i).second;

        // choose the syllable kind
        msrSyllableKind
          syllableKind =
            fCurrentNoteIsARest // JMI ??? v0.9.70
              ? msrSyllableKind::kSyllableSkipOnRestNote
              : msrSyllableKind::kSyllableSkipOnNonRestNote;

        // create a skip syllable
        S_msrSyllable
          skipSyllable =
            msrSyllable::create (
              currentNoteInputLineNumber,
              syllableKind,
              fCurrentSyllableExtendKind,
              fCurrentStanzaNumber,
              fCurrentNoteSoundingWholeNotesFromNotesDuration,
              msrTupletFactor (
                fCurrentNoteActualNotes,
                fCurrentNoteNormalNotes),
              stanza);

        // set syllable note upLink to fCurrentNote
        skipSyllable->
          appendSyllableToNoteAndSetItsUpLinkToNote (
            fCurrentNote);

        // fetch the voice
        S_msrVoice
          theMsrVoice =
            stanza->getStanzaUpLinkToVoice ();

        // fetch the part
        S_msrPart
          part =
            theMsrVoice->
              fetchVoiceUpLinkToPart ();

        // fetch the part current measure position
        msrWholeNotes
          partCurrentDrawingMeasurePosition =
            part->
              getPartCurrentDrawingMeasurePosition ();

        // append syllable to stanza
        stanza->
          appendSyllableToStanza (
            skipSyllable,
            theMsrVoice->getVoiceLastAppendedMeasure (),
            partCurrentDrawingMeasurePosition);
      } // for
    }
  }

  // register whether the new last handled note has lyrics
  fLastHandledNoteInVoiceHasLyrics =
    fCurrentSyllableElementsList.size ();
}

void mxsr2msrSkeletonPopulator::handleChordMemberNote (
  const S_msrNote& note)
{
  int newChodeNoteInputLineNumber =
    note->getInputStartLineNumber ();

  // set note kind as a chord member
  note->
    setNoteKind (
      msrNoteKind::kNoteRegularInChord);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Handling a note belonging to a chord" <<
      ", note:" <<
      std::endl;

    ++gIndenter;

    gLog << note <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

//   if (fCurrentNoteIsARest) {
//     mxsr2msrError (
//       gServiceRunData->getInputSourceName (),
//       newChodeNoteInputLineNumber,
//       __FILE__, __LINE__,
//       "a rest cannot belong to a chord");
//   }

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChords ()) {
//     std::stringstream ss;
//
//     ss <<
//       std::endl <<
// //       ", fCurrentChordStaffNumber: " <<
// //       mfStaffNumberAsString (fCurrentChordStaffNumber) <<
//       ", fPreviousNoteStaffNumber: " <<
//       mfStaffNumberAsString (fPreviousNoteStaffNumber) <<
//       ", fCurrentNoteStaffNumber: " <<
//       mfStaffNumberAsString (fCurrentNoteStaffNumber) <<
// //       ", staffNumberToUse: " <<
// //        mfStaffNumberAsString (staffNumberToUse) <<
//       ", line " << newChodeNoteInputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCurrentNoteVoiceNumber != K_VOICE_NUMBER_UNKNOWN_,
    "fCurrentNoteVoiceNumber is unknown");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Handling a chord member note" <<
      ", fCurrentPartStaffMsrVoicesMap [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]: \"" <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName () <<
      "\", fOnGoingChord: " <<
      fOnGoingChord <<
      ", line " << newChodeNoteInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChordsDetails ()) {
//     gLog <<
//       std::endl <<
//       "======================= handleChordMemberNote" <<
//       ", line " << newChodeNoteInputLineNumber <<
//       std::endl;
//     fCurrentPart->print (gLog);
//     gLog <<
//       "=======================" <<
//       std::endl << std::endl;
//
// /* JMI
//     if (fCurrentGraceNotesGroupNotes) {
//       gLog <<
//         fCurrentGraceNotesGroupNotes;
//     }
//     else {
// //       gLog <<
// //         "fCurrentGraceNotesGroupNotes is NULL"; // JMI
//     }
// */
//
//     gLog << std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED

  // fetch current note's kind
  msrNoteKind
    noteKind =
      note->getNoteKind ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    printVoicesLastMetNoteMap (
      newChodeNoteInputLineNumber,
      "handleChordMemberNote()");
  }
#endif // MF_TRACE_IS_ENABLED

  fPreviousMeasureElement = fCurrentChord;

  // handle chord's new note
  switch (noteKind) {
    case msrNoteKind::kNoteRestInMeasure:
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
      break;

    case msrNoteKind::kNoteInDoubleTremolo: // JMI v0.9.72
      {
        /* JMI
        // fetch note's sounding divisions
        int chordFirstNoteSoundingWholeNotes = // JMI
          note->
            getMeasureElementSoundingWholeNotes ();
            */

        /* JMI
        // updating chord's divisions // JMI
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceChords ()) {
          gLog <<
            "Updating sounding divisions for double tremolo chord " <<
            " " << chord <<
            " to " << chordFirstNoteSoundingWholeNotes <<
            " in voice \"" <<
            fCurrentPartStaffMsrVoicesMap
      [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->getVoiceName () <<
            "\"" <<
            std::endl;
        }
#endif // MF_TRACE_IS_ENABLED

        chord->
          setMeasureElementSoundingWholeNotes ( // ??? JMI
            chordFirstNoteSoundingWholeNotes,
            "mxsr2msrSkeletonPopulator::handleChordMemberNote()");
            */

        if (note->getNoteIsFirstNoteInADoubleTremolo ()) {
          // replace double tremolo's first element by chord
          fCurrentDoubleTremolo->
            setDoubleTremoloChordFirstElement (
              fCurrentChord);
        }

        else if (note->getNoteIsSecondNoteInADoubleTremolo ()) {
          // replace double tremolo's second element by chord
          fCurrentDoubleTremolo->
            setDoubleTremoloChordSecondElement (
              fCurrentChord);
        }

        else {
          std::stringstream ss;

          ss <<
            "note " <<
            note->asShortString () <<
            " belongs to a double tremolo, but is not marked as such" <<
            ", line " << newChodeNoteInputLineNumber;

          mxsr2msrInternalError (
            gServiceRunData->getInputSourceName (),
            newChodeNoteInputLineNumber,
            __FILE__, __LINE__,
            ss.str ());
        }
      }
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteRegularInChord:
      // error? JMI
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;

    case msrNoteKind::kNote_UNKNOWN_:
      break;
  } // switch

  // append note to current chord
  fCurrentChord->
    appendNoteToChord (
      note,
      fCurrentPartStaffMsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]);

//     if (fCurrentChordHasBeenPopulatedFromItsFirstNote) {
//       // populate current note from its first notes
//       copyNoteValuesToCurrentChord (
//         note);
//
//       // forget about current chord first note
//       fCurrentChordFirstNote = nullptr;
//
//       fCurrentChordHasBeenPopulatedFromItsFirstNote = true;
//     }

  // copy note's elements if any to the current chord
  copyNoteElementsIfAnyToChord (
    note,
    fCurrentChord);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::handleTupletMemberNote (
  const S_msrNote& note)
{
  int noteInputLineNumber =
    note->getInputStartLineNumber ();

 // register note as a tuplet member
 if (fCurrentNoteIsUnpitched) {
    note->
      setNoteKind (msrNoteKind::kNoteUnpitchedInTuplet);
  }
  else if (fCurrentNoteIsARest) {
    note->
      setNoteKind (msrNoteKind::kNoteRestInTuplet);
  }
  else {
    note->
      setNoteKind (msrNoteKind::kNoteRegularInTuplet);
  }

  if (fCurrentNoteSoundingWholeNotesFromNotesDuration.getNumerator () == 0) {
    // no duration has been found,
    // determine sounding from display whole notes
    note->
      determineTupletMemberSoundingFromDisplayWholeNotes (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a note belonging to a tuplet" <<
      ", note: " <<
      note->
        asShortString () <<
      ", line " << noteInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is there an ongoing chord?
  if (! fOnGoingChord) {
    // note is the first one after a chord in a tuplet,
    // JMI v0.9.72 ???
  }

  // fetch the current note's voice
  S_msrVoice
    firstNoteVoice =
      fCurrentPartStaffMsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber];

  // get the voice handler
  S_mxsrVoice
    theMxsrVoice =
      fCurrentPartStaffMxsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

  switch (fCurrentTupletTypeKind) {
    case msrTupletTypeKind::kTupletTypeNone:
      // nothing to do JMI v0.9.72
      break;

    case msrTupletTypeKind::kTupletTypeStart:
      {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTuplets ()) {
          std::stringstream ss;

          ss <<
            "--> handleTupletMemberNote(), kTupletTypeStart: note: " <<
            note->asShortString () <<
            ", line " << noteInputLineNumber;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        firstNoteVoice->
          registerTupletNoteInVoice (note);

// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceTuplets ()) {
//           // only after appendNoteToTuplet() has set the note's uplink to tuplet
//           std::stringstream ss;
//
//           ss <<
//             "Handling first note " <<
//             note->asShortString () <<
//             " of tuplet " <<
//             tuplet->asString ();
//
//           gWaeHandler->waeTrace (
//             __FILE__, __LINE__,
//             ss.str ());
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         // handle the tuplet start
//         handleTupletStart (
//           tuplet,
//           firstNoteVoice);

        // swith to continuation mode // JMI v0.9.72 ??? ???
        // this is handy in case the forthcoming tuplet members
        // are not explictly of the "continue" type
//         fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeContinue;
      }
      break;

    case msrTupletTypeKind::kTupletTypeContinue:
      // handle the tuplet continue
      handleTupletContinue (
        note,
        firstNoteVoice);
      break;

    case msrTupletTypeKind::kTupletTypeStop:
//       fThereIsAPendingTupletStop = true; // CHORD_TUP

      fNoteWithThePendingTupletStop = note;
      fVoiceOfTheNoteWithThePendingTupletStop = firstNoteVoice;

      // handle the tuplet stop FIRST_AFTER
      handleTupletStop (
        note,
        firstNoteVoice);
      break;
  } // switch

/* JMI ???
  // forget about this tuplet type, needed for nested tuplets
  fCurrentTupletTypeKind =
    msrTupletTypeKind::kTupletTypeNone;
    */
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleChordMemberNoteInATuplet (
  const S_msrNote& newChordNote)
{
  int newChordNoteInputLineNumber =
    newChordNote->getInputStartLineNumber ();

  S_mxsrVoice
    theMxsrVoice =
      fCurrentPartStaffMxsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

  // set new note kind as a chord or grace chord member JMI ???
  newChordNote->
    setNoteKind (
      msrNoteKind::kNoteRegularInChord);

  // apply tuplet sounding factor to note
  if (fCurrentNoteSoundingWholeNotesFromNotesDuration.getNumerator () == 0) {
    // no duration has been found,
    // determine sounding from display whole notes
    newChordNote->
      determineTupletMemberSoundingFromDisplayWholeNotes (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Handling a note belonging to a chord in a tuplet" <<
      ", newChordNote: " <<
      newChordNote->
        asShortString () <<
      ", theMxsrVoice->getTupletsStackSize (): " <<
      theMxsrVoice->getTupletsStackSize ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   if (fCurrentNoteIsARest) {
//     mxsr2msrError (
//       gServiceRunData->getInputSourceName (),
//       newChordNoteInputLineNumber,
//       __FILE__, __LINE__,
//       "a rest cannot belong to a chord in a tuplet");
//   }

//   // is there a pending tuplet stop?
//   if (fThereIsAPendingTupletStop) { // CHORD_TUP
//     // handle the tuplet stop
//     handleTupletStop (
//       fNoteWithThePendingTupletStop,
//       fVoiceOfTheNoteWithThePendingTupletStop);
//   }

  // should a chord be created?
  if (! fOnGoingChord) {
    // fetch the current tuplet, i.e. the top of the stack
    S_msrTuplet currentTuplet;

    //* JMI
    if (theMxsrVoice->getTupletsStackSize ()) {
      currentTuplet =
        theMxsrVoice->getTupletsStackTop ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check JMI v0.9.70
      mfAssert (
        __FILE__, __LINE__,
        currentTuplet != nullptr,
        "currentTuplet is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
    }

    else {
      std::stringstream ss;

      ss <<
        "handleChordMemberNoteInATuplet():" <<
        std::endl <<
        " a tuplet member chord " <<
        "cannot be added, tuplets stack is empty" <<
        ", line " << newChordNoteInputLineNumber;

      mxsr2msrInternalError (
        gServiceRunData->getInputSourceName (),
        newChordNoteInputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
    // */

//     currentTuplet =
//       fLastHandledTupletInVoiceMap [
//         std::make_pair (
//           fCurrentNoteStaffNumber,
//           fCurrentNoteVoiceNumber)
//       ];
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsDetails ()) { // JMI v0.9.71
      gLog <<
        std::endl <<
        std::endl <<
        "&&&&&&&&&&&&&&&&&& theMxsrVoice before removeLastNoteFromTuplet:";

      theMxsrVoice->print (gLog);

      gLog <<
        std::endl <<
        "&&&&&&&&&&&&&&&&&&" <<
        std::endl <<
        std::endl;
    }
#endif // MF_TRACE_IS_ENABLED


    // remove and fetch tupletLastNote from the current tuplet,
    // it will be the first chord member note
    S_msrNote
      tupletLastNote =
        currentTuplet->
          removeLastNoteFromTuplet (
            newChordNoteInputLineNumber);

/* JMI
    S_msrNote
      tupletLastNote =
  //      fStaffVoicesLastMetNoteMap [fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]];
        fStaffVoicesLastMetNoteMap [
          std::make_pair (fCurrentNoteStaffNumber, fCurrentNoteVoiceNumber)
          ];

    fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
      removeNoteFromVoice (
        newChordNoteInputLineNumber,
        tupletLastNote);
*/

// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceTupletsDetails ()) {
//       theMxsrVoice->
//         displayTupletsStack (
//           "############## After removeLastNoteFromTuplet()");
//     }
//     if (gTraceOahGroup->getTraceTupletsDetails ()) {
//       theMxsrVoice->
//         displayVoicesTupletsStacksMap (
//           "############## After removeLastNoteFromTuplet()");
//     }
// #endif // MF_TRACE_IS_ENABLED


    S_mxsrVoice
      theMxsrVoice =
        fCurrentPartStaffMxsrVoicesMap
          [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber];

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsDetails ()) { // JMI v0.9.71
      gLog <<
        std::endl <<
        std::endl <<
        "&&&&&&&&&&&&&&&&&& theMxsrVoice before appendChordToTuplet:" <<
        std::endl;

      theMxsrVoice->print (gLog);

      gLog <<
        std::endl <<
        "&&&&&&&&&&&&&&&&&&" <<
        std::endl <<
        std::endl;

      gLog <<
        std::endl <<
        std::endl <<
        "&&&&&&&&&&&&&&&&&& fCurrentChord before appendChordToTuplet:" <<
        std::endl;

      fCurrentChord->print (gLog);

      gLog <<
        std::endl <<
        "&&&&&&&&&&&&&&&&&&" <<
        std::endl <<
        std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    // add chord to the current tuplet instead of tupletLastNote
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChordsBasics ()) {
      std::stringstream ss;

      ss <<
        "Adding chord " <<
        fCurrentChord <<
        " to stack top tuplet " <<
        currentTuplet <<
        ", line " << newChordNoteInputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    currentTuplet->
      appendChordToTuplet (fCurrentChord);

//     // is this chord the last one in the tuplet?
//     if (currentTuplet->getTupletHasBeenFilled ()) { JMI v0.9.71
//       // fThereIsAPendingTupletStop // CHORD_TUP
//
//       // handle the tuplet stop
//       reduceTupletStackTop (
//         fNoteWithThePendingTupletStop,
//         fVoiceOfTheNoteWithThePendingTupletStop);
//     }

    if (fCurrentNoteSoundingWholeNotesFromNotesDuration.getNumerator () == 0) {
      // no duration has been found,
      // determine sounding from display whole notes
      newChordNote->
        determineTupletMemberSoundingFromDisplayWholeNotes (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes);
    }
  }

  // register note as another member of chord
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Adding another note " <<
      newChordNote->
        asShortString () <<
      ", line " << newChordNoteInputLineNumber <<
      " to current chord in voice " <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentChord->
    appendNoteToChord (
      newChordNote,
      fCurrentPartStaffMsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsIfAnyToChord (
    newChordNote,
    fCurrentChord);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleChordMemberNoteInAGraceNotesGroup (
  const S_msrNote& newChordNote)
{
  int newChordNoteInputLineNumber =
    newChordNote->getInputStartLineNumber ();

  // set new note kind as a grace chord member
  newChordNote->
    setNoteKind (
      msrNoteKind::kNoteInChordInGraceNotesGroup);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords () || gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Handling a note belonging to a chord in grace notes" <<
      ", newChordNote is " <<
      newChordNote->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   if (fCurrentNoteIsARest) {
//     mxsr2msrError (
//       gServiceRunData->getInputSourceName (),
//       newChordNoteInputLineNumber,
//       __FILE__, __LINE__,
//       "a rest cannot belong to a chord in a grace notes group");
//   }

  // add note to current chord
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Adding another note " <<
      newChordNote->
        asShortString () <<
      ", line " << newChordNoteInputLineNumber <<
      " to current chord in grace notes group in voice " <<
      fCurrentPartStaffMsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]->
          getVoiceName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentChord->
    appendNoteToChord (
      newChordNote,
      fCurrentPartStaffMsrVoicesMap
        [fCurrentRecipientStaffNumber][fCurrentNoteVoiceNumber]);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsIfAnyToChord (
    newChordNote,
    fCurrentChord);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif // MF_TRACE_IS_ENABLED
}

// void mxsr2msrSkeletonPopulator::displayLastHandledTupletInVoiceMap (const std::string& header)
// {
//   gLog <<
//     std::endl <<
//     header <<
//     ", fLastHandledTupletInVoiceMap contains:";
//
//   if (! fLastHandledTupletInVoiceMap.size ()) {
//     gLog <<
//       " none" <<
//       std::endl;
//   }
//
//   else {
//     std::map <std::pair <int, int>, S_msrTuplet>::const_iterator
//       iBegin = fLastHandledTupletInVoiceMap.begin (),
//       iEnd   = fLastHandledTupletInVoiceMap.end (),
//       i      = iBegin;
//
//     gLog << std::endl;
//
//     ++gIndenter;
//
//     for ( ; ; ) {
//       gLog <<
//         "staff " << (*i).first.first <<
//         ", voice " <<  (*i).first.second <<
//         std::endl;
// //        "\"" << (*i).first->getVoiceName () <<
// //        "\" ----> " << (*i).second->asString ();
//       if (++i == iEnd) break;
//       gLog << std::endl;
//     } // for
//
//     gLog << std::endl;
//
//     --gIndenter;
//   }
//
//   gLog << std::endl;
// }

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleRepeatStart (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start" <<
    /* JMI
      ", measure " <<
        barLine->getBarLineMeasureNumber () <<
      ", position " <<
      barLine->getBarLineMeasurePosition () <<
      */
      ", line " << barLine->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // remember repeat start measure number
  fCurrentRepeatStartMeasureNumber = barLine->getInputStartLineNumber ();

  // prepare for repeat in current part
  fCurrentPart->
    handleRepeatStartInPart (
      barLine->getInputStartLineNumber ());

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleRepeatEnd (
  const S_msrBarLine& barLine)
{
  std::string repeatStartMeasureNumber =
    fCurrentRepeatStartMeasureNumber.size ()
      ? // there was a repeat start before hand
        fCurrentRepeatStartMeasureNumber
      : // there is an implicit repeat start at the beginning of the part
        fPartFirstMeasureNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat end in part " <<
      fCurrentPart->fetchPartCombinedName () <<
      ", fCurrentMeasureNumber: \"" << fCurrentMeasureNumber <<
      "\", fCurrentRepeatStartMeasureNumber: \"" << fCurrentRepeatStartMeasureNumber <<
      "\", repeatStartMeasureNumber: \"" << repeatStartMeasureNumber <<
      "\", line " << barLine->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI

  fCurrentPart->
    handleRepeatEndInPart (
      barLine->getInputStartLineNumber (),
      repeatStartMeasureNumber,
      barLine->getBarLineTimes ());

  // forget about the current repeat start barLine
  fCurrentRepeatStartMeasureNumber = "";

  ++fRepeatEndCounter;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleRepeatEndingStart (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending start in part " <<
      fCurrentPart->fetchPartCombinedName () <<
      ", line " << barLine->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    std::stringstream ss;

    ss <<
      std::endl << std::endl <<
      "****************** handleRepeatEndingStart()" <<
      ", line " << barLine->getInputStartLineNumber () <<
      std::endl <<
      fCurrentPart;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the repeat ending
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending upon its start in part " <<
      fCurrentPart->fetchPartCombinedName () <<
      ", line " << barLine->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPart->
    handleRepeatEndingStartInPart (
      barLine->getInputStartLineNumber ());

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleRepeatHookedEndingEnd (
  const S_msrBarLine& barLine)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat hooked ending end in part " <<
      fCurrentPart->fetchPartCombinedName () <<
      ", line " << barLine->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    std::stringstream ss;

    ss <<
      std::endl << std::endl <<
      "****************** handleRepeatHookedEndingEnd()" <<
      ", line " << barLine->getInputStartLineNumber () <<
      std::endl <<
      fCurrentPart;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI

  // create a hooked repeat ending from the current segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a hooked repeat ending in part " <<
      fCurrentPart->fetchPartCombinedName () <<
      ", line " << barLine->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPart->
    handleRepeatEndingEndInPart (
      barLine->getInputStartLineNumber (),
      fCurrentBarLineEndingNumber,
      msrRepeatEndingKind::kRepeatEndingHooked);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::handleRepeatHooklessEndingEnd (
  const S_msrBarLine& barLine)
{
  /*
  The discontinue value is typically used for the last ending in a set,
  where there is no downward hook to mark the end of an ending:

    <barLine location="right">
      <ending type="discontinue" number="2"/>
    </barLine>
  */

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat hookless ending end in part " <<
      fCurrentPart->fetchPartCombinedName () <<
      ", line " << barLine->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    std::stringstream ss;

    ss <<
      std::endl << std::endl <<
      "****************** handleRepeatHooklessEndingEnd()" <<
      ", line " << barLine->getInputStartLineNumber () <<
      std::endl <<
      fCurrentPart;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine);

  // create a hookless repeat ending from the current segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Handling a hookless repeat ending in part " <<
      fCurrentPart->fetchPartCombinedName () <<
      ", line " << barLine->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPart->
    handleRepeatEndingEndInPart (
      barLine->getInputStartLineNumber (),
      fCurrentBarLineEndingNumber,
      msrRepeatEndingKind::kRepeatEndingHookless);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_rehearsal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rehearsal" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string rehearsalValue = elt->getValue();

  std::string rehearsalEnclosure =
    elt->getAttributeValue ("enclosure");

  msrRehearsalMarkKind
    rehearsalKind =
      msrRehearsalMarkKind::kRehearsalMarkNone; // default value

  if      (rehearsalEnclosure == "none") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkNone;
  }
  else if (rehearsalEnclosure == "rectangle") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkRectangle;
  }
  else if (rehearsalEnclosure == "square") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkSquare;
  }
  else if (rehearsalEnclosure == "oval") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkOval;
  }
  else if (rehearsalEnclosure == "circle") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkCircle;
  }
  else if (rehearsalEnclosure == "bracket") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkBracket;
  }
  else if (rehearsalEnclosure == "triangle") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkTriangle;
  }
  else if (rehearsalEnclosure == "diamond") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkDiamond;
  }
  else if (rehearsalEnclosure == "pentagon") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkPentagon;
  }
  else if (rehearsalEnclosure == "hexagon") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkHexagon;
  }
  else if (rehearsalEnclosure == "heptagon") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkHeptagon;
  }
  else if (rehearsalEnclosure == "octagon") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkOctagon;
  }
  else if (rehearsalEnclosure == "nonagon") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkNonagon;
  }
  else if (rehearsalEnclosure == "decagon") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkDecagon;
  }
  else {
    if (rehearsalEnclosure.size ()) {
      std::stringstream ss;

      ss <<
        "rehearsalMark enclosure \"" << rehearsalEnclosure <<
        "\"" << " is unknown, ignored";

      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        ss.str ());
    }
  }

  // create a rehearsalMark
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRehearsalMarks ()) {
    std::stringstream ss;

    ss <<
      "Creating rehearsalMark \"" << rehearsalValue << "\"" <<
      " in part " <<
      fCurrentPart->fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRehearsalMark
    rehearsalMark =
      msrRehearsalMark::create (
        elt->getInputStartLineNumber (),
        rehearsalKind,
        rehearsalValue,
        fCurrentDirectionPlacementKind);

  // append the rehearsalMark to the pending tempos list
  fPendingRehearsalMarksList.push_back (rehearsalMark);
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_harmony& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_harmony" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);

  ++fHarmoniesVoicesCounter;

  fCurrentHarmonyInputLineNumber       = elt->getInputStartLineNumber ();

  fCurrentHarmonyRootDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentHarmonyRootAlterationKind    = msrAlterationKind::kAlterationNatural;

  fCurrentHarmonyFunctionText          = "";

  fCurrentHarmonyKind                  = msrHarmonyKind::kHarmony_UNKNOWN_;
  fCurrentHarmonyKindText              = "";

  fCurrentHarmonyInversion             = K_HARMONY_INVERSION_NONE;

  fCurrentHarmonyBassDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentHarmonyBassAlterationKind    = msrAlterationKind::kAlterationNatural;

  fCurrentHarmonyDegreeValue           = -1;
  fCurrentHarmonyDegreeAlterationKind  = msrAlterationKind::kAlterationNatural;

  fCurrentHarmonyWholeNotesOffset = msrWholeNotes (0, 1);

  fOnGoingHarmony = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_root& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_root" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);
}

void mxsr2msrSkeletonPopulator::visitStart (S_root_step& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_root_step" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string step = elt->getValue ();

  checkStep (
    elt->getInputStartLineNumber (),
    step,
    "<root-step/>");

  fCurrentHarmonyRootDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (
      step [0]);
}

void mxsr2msrSkeletonPopulator::visitStart (S_root_alter& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_root_alter" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  float rootAlter = (float)(*elt);

  fCurrentHarmonyRootAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      rootAlter);

  if (fCurrentHarmonyRootAlterationKind == msrAlterationKind::kAlteration_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "root alter " << rootAlter <<
      " should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_function& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_function" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string fCurrentHarmonyFunctionText = elt->getValue ();
}

void mxsr2msrSkeletonPopulator::visitStart (S_kind& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_kind" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // harmony kind
  // ----------------------------------

  std::string kindValue = elt->getValue ();

  fCurrentHarmonyKindText =
    elt->getAttributeValue ("text");

  // check harmony kindValue
  if      (kindValue == "major")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajor;
  else if (kindValue == "minor")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinor;
  else if (kindValue == "augmented")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyAugmented;
  else if (kindValue == "diminished")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDiminished;

  else if (kindValue == "dominant")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDominant;
  else if (kindValue == "major-seventh")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorSeventh;
  else if (kindValue == "minor-seventh")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorSeventh;
  else if (kindValue == "diminished-seventh")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDiminishedSeventh;

  else if (kindValue == "augmented-seventh")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyAugmentedSeventh;
  else if (kindValue == "half-diminished")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyHalfDiminished;
  else if (kindValue == "major-minor")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorMajorSeventh;

  else if (kindValue == "major-sixth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorSixth;
  else if (kindValue == "minor-sixth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorSixth;

  else if (kindValue == "dominant-ninth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDominantNinth;
  else if (kindValue == "major-ninth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorNinth;
  else if (kindValue == "minor-ninth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorNinth;

  else if (kindValue == "dominant-11th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDominantEleventh;
  else if (kindValue == "major-11th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorEleventh;
  else if (kindValue == "minor-11th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorEleventh;

  else if (kindValue == "dominant-13th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDominantThirteenth;
  else if (kindValue == "major-13th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorThirteenth;
  else if (kindValue == "minor-13th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorThirteenth;

  else if (kindValue == "suspended-second")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonySuspendedSecond;
  else if (kindValue == "suspended-fourth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonySuspendedFourth;

  else if (kindValue == "Neapolitan")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyNeapolitan;
  else if (kindValue == "Italian")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyItalian;
  else if (kindValue == "French")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyFrench;
  else if (kindValue == "German")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyGerman;

  else if (kindValue == "pedal")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyPedal;
  else if (kindValue == "power")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyPower;
  else if (kindValue == "Tristan")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyTristan;
  else if (kindValue == "other")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyOther;

  else if (kindValue == "none") {
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyNone;
  }

  else {
    if (kindValue.size ()) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        "harmony kind \"" + kindValue + "\" os unknown");
    }
    else {
      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        "empty harmony kind, replaced by 'major'");

      fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajor;
    }
  }

  // harmony use symbols
  // ----------------------------------

  std::string kindUseSymbols = elt->getAttributeValue ("use-symbols");

/* JMI v0.9.67
  if      (kindUseSymbols == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindUseSymbols == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindUseSymbols.size ()) {
      std::stringstream ss;

      ss <<
        "kind use-symbols \"" << kindUseSymbols <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
      }
  }
*/

  // harmony use stack degrees
  // ----------------------------------

  std::string kindStackDegrees = elt->getAttributeValue ("stack-degrees");

/* JMI v0.9.67
  if      (kindStackDegrees == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindStackDegrees == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindStackDegrees.size ()) {
      std::stringstream ss;

      ss <<
        "kind stack-degrees " << kindStackDegrees <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
      }
  }
  */

  // harmony use parentheses degrees
  // ----------------------------------

  std::string kindParenthesesDegrees = elt->getAttributeValue ("parentheses-degrees");

/* JMI v0.9.67
  if      (kindParenthesesDegrees == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindParenthesesDegrees == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindParenthesesDegrees.size ()) {
      std::stringstream ss;

      ss <<
        "kind parentheses-degrees \"" << kindParenthesesDegrees <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
      }
  }
  */

  // harmony use bracket degrees
  // ------------------

  std::string kindBracketDegrees = elt->getAttributeValue ("bracket-degrees");

/* JMI v0.9.67
  if      (kindBracketDegrees == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindBracketDegrees == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindBracketDegrees.size ()) {
      std::stringstream ss;

      ss <<
        "kind bracket-degrees \"" << kindBracketDegrees <<
        "\" is unknown";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
      }
  }
  */
}

void mxsr2msrSkeletonPopulator::visitStart (S_inversion& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_inversion" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
  Inversion is a number indicating which inversion is used:
  0 for root position, 1 for first inversion, etc.
*/

  fCurrentHarmonyInversion = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_bass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bass" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_bass_step& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bass_step" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string step = elt->getValue();

  checkStep (
    elt->getInputStartLineNumber (),
    step,
    "<bass-step/>");

  fCurrentHarmonyBassDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (
      step [0]);
}

void mxsr2msrSkeletonPopulator::visitStart (S_bass_alter& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bass_alter" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  float bassAlter = (float)(*elt);

  fCurrentHarmonyBassAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      bassAlter);

  if (fCurrentHarmonyBassAlterationKind == msrAlterationKind::kAlteration_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "bass alter " << bassAlter <<
      " should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_degree& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_degree" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      elt->getInputStartLineNumber (),
      printObjectString);
}

void mxsr2msrSkeletonPopulator::visitStart (S_degree_value& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_degree_value" <<
       ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentHarmonyDegreeValue = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_degree_alter& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_degree_alter" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  float degreeAlter = (float)(*elt);

  fCurrentHarmonyDegreeAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      degreeAlter);

  if (fCurrentHarmonyDegreeAlterationKind == msrAlterationKind::kAlteration_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "degree alter " << degreeAlter <<
      " should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_degree_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_degree_type" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string degreeType = elt->getValue ();

  // check harmony degree type
  if      (degreeType == "add")
    fCurrentHarmonyDegreeTypeKind =
      msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAdd;

  else if (degreeType == "alter")
    fCurrentHarmonyDegreeTypeKind =
      msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAlter;

  else if (degreeType == "subtract")
    fCurrentHarmonyDegreeTypeKind =
      msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeSubstract;

  else {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        "harmony degree-type \"" + degreeType + "\" is unknown");
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_degree& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_degree" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create harmony degree
  S_msrHarmonyDegree
    harmonyDegree =
      msrHarmonyDegree::create (
        elt->getInputStartLineNumber (),
        fCurrentHarmonyDegreeValue,
        fCurrentHarmonyDegreeAlterationKind,
        fCurrentHarmonyDegreeTypeKind);

  // register it in current harmony degrees list
  fCurrentHarmonyDegreesList.push_back (
    harmonyDegree);
}

void mxsr2msrSkeletonPopulator::visitEnd (S_harmony& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_harmony" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // convert root diatonic pitch to a quarter tone pitch
  fCurrentHarmonyRootQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      elt->getInputStartLineNumber (),
      fCurrentHarmonyRootDiatonicPitchKind,
      fCurrentHarmonyRootAlterationKind);

  // convert bass diatonic pitch to a quarter tone pitch
  fCurrentHarmonyBassQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      elt->getInputStartLineNumber (),
      fCurrentHarmonyBassDiatonicPitchKind,
      fCurrentHarmonyBassAlterationKind);

  // check data consistency
  if (
    fCurrentHarmonyKind == msrHarmonyKind::kHarmonyOther
      &&
    fCurrentHarmonyDegreesList.size () == 0
  ) {
    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      "harmony kind is 'other' but there are no harmony degrees, this is strange...");
  }

  if (
    fCurrentHarmonyRootQuarterTonesPitchKind
      ==
    fCurrentHarmonyBassQuarterTonesPitchKind) {
    std::stringstream ss;

    ss <<
      "harmony root and bass notes are both equal to " <<
      msrDiatonicPitchKindAsStringInLanguage (
        gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
        diatonicPitchKindFromQuarterTonesPitchKind (
          elt->getInputStartLineNumber (),
          fCurrentHarmonyRootQuarterTonesPitchKind)) <<
      ", ignoring the latter";

    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());

    fCurrentHarmonyBassQuarterTonesPitchKind =
      msrQuarterTonesPitchKind::kQTP_UNKNOWN_;
  }

  if (gGlobalMxsr2msrOahGroup->getIgnoreHarmonies ()) {
 #ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Ignoring harmony" <<
        ", line " <<
        elt->getInputStartLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
  else {
   // create the harmony
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
      std::stringstream ss;

      ss <<
        "Creating a harmony" <<
        ", line " << elt->getInputStartLineNumber () << ":" <<
        std::endl;

      ++gIndenter;

      constexpr int fieldWidth = 32;

      gLog << std::left <<
        std::setw (fieldWidth) << "fCurrentPart" << ": " <<
        fCurrentPart->fetchPartCombinedName () <<
        std::endl <<
        /* JMI
        std::setw (fieldWidth) << "harmoniesVoice" << ": " <<
        harmoniesVoice->getVoiceName () <<
        std::endl <<
        */

        std::setw (fieldWidth) << "fCurrentHarmonyRootDiatonicPitch" << ": " <<
        msrDiatonicPitchKindAsStringInLanguage (
          gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          fCurrentHarmonyRootDiatonicPitchKind) <<
        std::endl <<
        std::setw (fieldWidth) << "fCurrentHarmonyRootAlteration" << ": " <<
        msrAlterationKindAsString(
          fCurrentHarmonyRootAlterationKind) <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyFunctionText" << ": " <<
        fCurrentHarmonyFunctionText <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyKind" << ": " <<
        fCurrentHarmonyKind <<
        std::endl <<
        std::setw (fieldWidth) << "fCurrentHarmonyKindText" << ": " <<
        fCurrentHarmonyKindText <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyInversion" << ": " <<
        fCurrentHarmonyInversion <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyBassDiatonicPitch" << ": " <<
        msrDiatonicPitchKindAsStringInLanguage (
          gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          fCurrentHarmonyBassDiatonicPitchKind) <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyBassAlteration" << ": " <<
        msrAlterationKindAsString(
          fCurrentHarmonyBassAlterationKind) <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentNoteSoundingWholeNotes" << ": " <<
        fCurrentNoteSoundingWholeNotes <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmoniesStaffNumber" << ": " <<
        fCurrentHarmoniesStaffNumber <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyWholeNotesOffset" << ": " <<
        fCurrentHarmonyWholeNotesOffset <<
        std::endl;

      --gIndenter;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    S_msrHarmony
      harmony =
        msrHarmony::create (
          fCurrentHarmonyInputLineNumber,
          gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

          fCurrentHarmonyRootQuarterTonesPitchKind,

          fCurrentHarmonyKind,
          fCurrentHarmonyKindText,

          fCurrentHarmonyFunctionText,

          fCurrentHarmonyInversion,

          fCurrentHarmonyBassQuarterTonesPitchKind,

          K_WHOLE_NOTES_UNKNOWN_,      // harmonySoundingWholeNotes,
                                      // will be set in handlePendingHarmonies()
          K_WHOLE_NOTES_UNKNOWN_,      // harmonyDisplayWholeNotes,
                                      // will be set in handlePendingHarmonies()
          fCurrentHarmoniesStaffNumber,
          msrTupletFactor (1, 1),     // will be set upon next note handling
          fCurrentHarmonyWholeNotesOffset);

    // append pending harmony degrees if any to the harmony
    if (! fCurrentHarmonyDegreesList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceHarmoniesDetails ()) {
        mxsr2msrWarning (
          gServiceRunData->getInputSourceName (),
          elt->getInputStartLineNumber (),
          "harmony has no degrees contents");
      }
#endif // MF_TRACE_IS_ENABLED
    }

    else {
      // handle harmony degrees if any
      while (fCurrentHarmonyDegreesList.size ()) {
        S_msrHarmonyDegree
          harmonyDegree =
            fCurrentHarmonyDegreesList.front ();

        // set harmony degree harmony upLink
        harmonyDegree->
          setHarmonyDegreeUpLinkToHarmony (
            harmony);

        // append it to harmony's degrees list
        harmony->
          appendHarmonyDegreeToHarmony (
            harmonyDegree);

        // remove it from the list
        fCurrentHarmonyDegreesList.pop_front ();
      } // while
    }

    // attach the current frame if any to the harmony
    if (fCurrentFrame) {
      harmony->setHarmonyFrame (fCurrentFrame);
    }

    // append harmony to the pending harmonies list
    fPendingHarmoniesList.push_back (harmony);
  }

  fOnGoingHarmony = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_frame& elt)
{
/*
<!--
	The frame element represents a frame or fretboard diagram
	used together with a chord symbol. The representation is
	based on the NIFF guitar grid with additional information.
	The frame-strings and frame-frets elements give the
	overall size of the frame in vertical lines (strings) and
	horizontal spaces (frets).

	The frame element's unplayed attribute indicates what to
	display above a string that has no associated frame-note
	element. Typical values are x and the empty string. If the
	attribute is not present, the display of the unplayed
	string is application-defined.
-->
<!ELEMENT frame
	(frame-strings, frame-frets, first-fret?, frame-note+)>
<!ATTLIST frame
    %position;
    %color;
    %halign;
    %valign-image;
    height  %tenths;  #IMPLIED
    width   %tenths;  #IMPLIED
    unplayed CDATA    #IMPLIED
    %optional-unique-id;
>
<!ELEMENT frame-strings (#PCDATA)>
<!ELEMENT frame-frets (#PCDATA)>

<!--
	The first-fret indicates which fret is shown in the top
	space of the frame; it is fret 1 if the element is not
	present. The optional text attribute indicates how this
	is represented in the fret diagram, while the location
	attribute indicates whether the text appears to the left
	or right of the frame.
-->
<!ELEMENT first-fret (#PCDATA)>
<!ATTLIST first-fret
    text CDATA #IMPLIED
    location %left-right; #IMPLIED
>

<!--
	The frame-note element represents each note included in
	the frame. The definitions for string, fret, and fingering
	are found in the common.mod file. An open string will
	have a fret value of 0, while a muted string will not be
	associated with a frame-note element.
-->
<!ELEMENT frame-note (string, fret, fingering?, barre?)>

<!--
	The barre element indicates placing a finger over
	multiple strings on a single fret. The type is "start"
	for the lowest pitched string (e.g., the string with
	the highest MusicXML number) and is "stop" for the
	highest pitched string.
-->
<!ELEMENT barre EMPTY>
<!ATTLIST barre
    type %start-stop; #REQUIRED
    %color;
>
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fOnGoingHarmony) {
    std::stringstream ss;

    ss << "<frame /> out of context, not handled";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

/*
      <harmony>
        <root>
          <root-step>C</root-step>
        </root>
        <kind>major</kind>
        <frame>
          <frame-strings>6</frame-strings>
          <frame-frets>4</frame-frets>
          <frame-note>
            <std::string>6</std::string>
            <fret>0</fret>
          </frame-note>
          <frame-note>
            <std::string>5</std::string>
            <fret>3</fret>
          </frame-note>
          <frame-note>
            <std::string>4</std::string>
            <fret>2</fret>
          </frame-note>
          <frame-note>
            <std::string>3</std::string>
            <fret>0</fret>
          </frame-note>
          <frame-note>
            <std::string>2</std::string>
            <fret>1</fret>
          </frame-note>
          <frame-note>
            <std::string>1</std::string>
            <fret>0</fret>
          </frame-note>
        </frame>
      </harmony>
*/

  float degreeAlter = (float)(*elt);

  // alteration

  fCurrentHarmonyDegreeAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      degreeAlter);

  if (fCurrentHarmonyDegreeAlterationKind == msrAlterationKind::kAlteration_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "degree alter " << degreeAlter <<
      " should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // color JMI

  fOnGoingFrame = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_frame_strings& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame_strings" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentFrameStrings = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_frame_frets& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame_frets" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentFrameFrets = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_first_fret& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_first_fret" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
          <first-fret location="right" text="12fr.">12</first-fret>
*/

  fCurrentFrameFirstFret = (int)(*elt);

  // the following two are not used yet JMI
  std::string firstFretLocation = elt->getAttributeValue ("location");
  std::string firstFretText = elt->getAttributeValue ("text");
}

void mxsr2msrSkeletonPopulator::visitStart (S_frame_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentFrameNoteStringNumber = -1;
  fCurrentFrameNoteFretNumber = -1;
  fCurrentFrameNoteFingering = -1;
  fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeNone;

  fOnGoingFrameNote = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_barre& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_barre" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // type

  std::string barreType = elt->getAttributeValue ("type");

  fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeNone;

  if      (barreType == "start")
    fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeStart;
  else if (barreType == "stop")
    fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeStop;
  else {
    std::stringstream ss;

    ss <<
      "barre type \"" << barreType <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // color JMI
}

void mxsr2msrSkeletonPopulator::visitEnd (S_frame_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame_note" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the frame note
  S_msrFrameNote
    frameNote =
      msrFrameNote::create (
        elt->getInputStartLineNumber (),
        fCurrentFrameNoteStringNumber,
        fCurrentFrameNoteFretNumber,
        fCurrentFrameNoteFingering,
        fCurrentFrameNoteBarreTypeKind);

  // append the frame note to the pending frame notes list
  fPendingFramesNotesList.push_back (frameNote);

  fOnGoingFrameNote = false;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_frame& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_frame" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the frame
  fCurrentFrame =
    msrFrame::create (
      elt->getInputStartLineNumber (),
      fCurrentFrameStrings,
      fCurrentFrameFrets,
      fCurrentFrameFirstFret);

  // handling the current pending frame notes if any,
  // so that they get attached to the note right now
  if (fPendingFramesNotesList.size ()) {
    while (fPendingFramesNotesList.size ()) {
      S_msrFrameNote
        frameNote =
          fPendingFramesNotesList.front ();

      // append the frame note to the frame
      fCurrentFrame->
        appendFrameNoteToFrame (frameNote);

      // remove it from the list
      fPendingFramesNotesList.pop_front ();
    } // while
  }

  fOnGoingFrame = false;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_figured_bass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_figured_bass" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fFiguredBassVoicesCounter;

  std::string parentheses = elt->getAttributeValue ("parentheses");

  fCurrentFiguredBassParenthesesKind =
    msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo; // default value

  if (parentheses.size ()) {
    if (parentheses == "yes")
      fCurrentFiguredBassParenthesesKind =
        msrFiguredBassParenthesesKind::kFiguredBassParenthesesYes;

    else if (parentheses == "no")
     fCurrentFiguredBassParenthesesKind =
        msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo;

    else {
      std::stringstream ss;

      ss <<
        "parentheses value " << parentheses <<
        " should be 'yes' or 'no'";

      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        elt->getInputStartLineNumber (),
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  fCurrentFiguredBassInputLineNumber   = -1;

  fCurrentFigureNumber = -1;

  fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefix_UNKNOWN_;
  fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffix_UNKNOWN_;

  fCurrentFiguredBassSoundingWholeNotes = msrWholeNotes (0, 1);
  fCurrentFiguredBassDisplayWholeNotes  = msrWholeNotes (0, 1);

  fOnGoingFiguredBass = true;
}

void mxsr2msrSkeletonPopulator::visitStart (S_figure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_figure" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_prefix& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_prefix" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string prefix =
    elt->getValue ();

  fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefix_UNKNOWN_;

  if      (prefix == "double-flat")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixDoubleFlat;

  else if (prefix == "flat")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixFlat;

  else if (prefix == "flat-flat")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixFlat;

  else if (prefix == "natural")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixNatural;

  else if (prefix == "sharp-sharp")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixSharpSharp;

  else if (prefix == "sharp")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixSharp;

  else if (prefix == "double-sharp")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixDoubleSharp;

  else if (prefix.size ()) {
    std::stringstream ss;

    ss <<
      "prefix \"" << prefix <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

}

void mxsr2msrSkeletonPopulator::visitStart (S_figure_number& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_figure_number" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentFigureNumber = (int)(*elt);

  if (fCurrentFigureNumber > 13) {
    std::stringstream ss;

    ss <<
      "figure-number " << fCurrentFigureNumber <<
      " is greater that 13, that's strange...";

    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_suffix& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_suffix" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string suffix =
    elt->getValue ();

  fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffix_UNKNOWN_;

  if      (suffix == "double-flat")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixDoubleFlat;

  else if (suffix == "flat")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixFlat;

  else if (suffix == "flat-flat")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixFlat;

  else if (suffix == "natural")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixNatural;

  else if (suffix == "sharp-sharp")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixSharpSharp;

  else if (suffix == "sharp")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixSharp;

  else if (suffix == "double-sharp")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixDoubleSharp;

  else if (suffix == "slash")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixSlash;

  else if (suffix.size ()) {
    std::stringstream ss;

    ss <<
      "suffix \"" << suffix <<
      "\" is unknown";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_figure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_figure" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the bass figure
  S_msrBassFigure
    bassFigure =
      msrBassFigure::create (
        elt->getInputStartLineNumber (),
        fCurrentPart,
        fCurrentFigurePrefixKind,
        fCurrentFigureNumber,
        fCurrentFigureSuffixKind);

  // append it to the pending figures list
  fPendingFiguredBassFiguresList.push_back (
    bassFigure);
}

void mxsr2msrSkeletonPopulator::visitEnd (S_figured_bass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_figured_bass" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the figured bass element
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating a figured bass" <<
      ", line " << elt->getInputStartLineNumber () << ":";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the figured bass element
  // if the sounding whole notes is 0/1 (no <duration /> was found), JMI ???
  // it will be set to the next note's sounding whole notes later

  S_msrFiguredBass
    figuredBass =
      msrFiguredBass::create (
        elt->getInputStartLineNumber (),
        fCurrentFiguredBassSoundingWholeNotes,
        fCurrentFiguredBassDisplayWholeNotes,
        fCurrentFiguredBassParenthesesKind,
        msrTupletFactor (1, 1)); // will be set upon next note handling

  // attach pending figures to the figured bass
  if (! fPendingFiguredBassFiguresList.size ()) {
    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      "figured-bass has no figures contents, ignoring it");
  }
  else {
    // append the pending figures to the figured bass element
    for (S_msrBassFigure bassFigure : fPendingFiguredBassFiguresList) {
      figuredBass->
        appendFigureToFiguredBass (bassFigure);
    } // for

    // forget about those pending figures
    fPendingFiguredBassFiguresList.clear ();

    // append figuredBass to the pending figured basses list
    fPendingFiguredBassesList.push_back (figuredBass);
  }

  fOnGoingFiguredBass = false;
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_harp_pedals& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_harp_pedals" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
<!--
  The harp-pedals element is used to create harp pedal
  diagrams. The pedal-step and pedal-alter elements use
  the same values as the step and alter elements. For
  easiest reading, the pedal-tuning elements should follow
  standard harp pedal order, with pedal-step values of
  D, C, B, E, F, G, and A.
-->
<!ELEMENT harp-pedals (pedal-tuning)+>
<!ATTLIST harp-pedals
    %print-style-align;
>
<!ELEMENT pedal-tuning (pedal-step, pedal-alter)>
<!ELEMENT pedal-step (#PCDATA)>
<!ELEMENT pedal-alter (#PCDATA)>

<!-- Harp damping marks -->
<!ELEMENT damp EMPTY>
<!ATTLIST damp
    %print-style-align;
>
<!ELEMENT damp-all EMPTY>
<!ATTLIST damp-all
    %print-style-align;
>

        <direction-type>
          <harp-pedals>
            <pedal-tuning>
              <pedal-step>D</pedal-step>
              <pedal-alter>0</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>C</pedal-step>
              <pedal-alter>-1</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>B</pedal-step>
              <pedal-alter>-1</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>E</pedal-step>
              <pedal-alter>0</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>F</pedal-step>
              <pedal-alter>0</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>G</pedal-step>
              <pedal-alter>1</pedal-alter>
            </pedal-tuning>
            <pedal-tuning>
              <pedal-step>A</pedal-step>
              <pedal-alter>-1</pedal-alter>
            </pedal-tuning>
          </harp-pedals>
        </direction-type>

*/

  // create the harp pedals tuning
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarpPedalsTunings ()) {
    std::stringstream ss;

    ss <<
      "Creating harp pedals tuning:";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentHarpPedalsTuning =
    msrHarpPedalsTuning::create (
      elt->getInputStartLineNumber ());

  // add it to the current part
  fCurrentPart->
    appendHarpPedalsTuningToPart (
      fCurrentHarpPedalsTuning);
}

void mxsr2msrSkeletonPopulator::visitStart (S_pedal_tuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pedal_tuning" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrSkeletonPopulator::visitStart (S_pedal_step& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pedal_step" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string tuningStep = elt->getValue();

  checkStep (
    elt->getInputStartLineNumber (),
    tuningStep,
    "<pedal-step/>");

  fCurrentHarpPedalDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (
      tuningStep [0]);
}

void mxsr2msrSkeletonPopulator::visitStart (S_pedal_alter& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pedal_alter" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  float pedalAlter = (float)(*elt);

  fCurrentHarpPedalAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      pedalAlter);

  if (fCurrentHarpPedalAlterationKind == msrAlterationKind::kAlteration_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "pedal alter " << pedalAlter <<
      " should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      elt->getInputStartLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }
}

void mxsr2msrSkeletonPopulator::visitEnd (S_pedal_tuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_pedal_tuning" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a harp pedals tuning
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarpPedalsTunings ()) {
    gLog <<
      "Creating harp pedal tuning:" <<
      std::endl;

    ++gIndenter;

    constexpr int fieldWidth = 31;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentHarpPedalDiatonicPitch" << ": " <<
      msrDiatonicPitchKindAsStringInLanguage (
        gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
        fCurrentHarpPedalDiatonicPitchKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentHarpPedalAlteration" << ": " <<
      msrAlterationKindAsString (
        fCurrentHarpPedalAlterationKind) <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentHarpPedalsTuning->
    addPedalTuning (
      elt->getInputStartLineNumber (),
      fCurrentHarpPedalDiatonicPitchKind,
      fCurrentHarpPedalAlterationKind);
}

void mxsr2msrSkeletonPopulator::visitStart (S_damp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_damp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingDirectionType) {
    // create the damp
    S_msrDamp
      damp =
        msrDamp::create (
          elt->getInputStartLineNumber ());

    // append it to the pending damps list
    fPendingDampsList.push_back (damp);
  }
}

void mxsr2msrSkeletonPopulator::visitStart (S_damp_all& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_damp_all" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingDirectionType) {
    // create the damp all
    S_msrDampAll
      dampAll =
        msrDampAll::create (
          elt->getInputStartLineNumber ());

    // append it to the pending damp alls list
    fPendingDampAllsList.push_back (dampAll);
  }
}

//________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_capo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_capo" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffDetailsCapo = (int)(*elt);
}

void mxsr2msrSkeletonPopulator::visitStart (S_staff_size& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_size" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffDetailsStaffSize = (int)(*elt);
  // JMI not used
}

void mxsr2msrSkeletonPopulator::visitEnd (S_staff_details& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_staff_details" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    constexpr int fieldWidth = 29;

    ss << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentStaffLinesNumber" << ": " <<
      fCurrentStaffLinesNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fStaffDetailsStaffNumber" << ": " <<
      fStaffDetailsStaffNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStaffDetailsCapo" << ": " <<
      fCurrentStaffDetailsCapo <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStaffDetailsStaffSize" << ": " <<
      fCurrentStaffDetailsStaffSize;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // has been incremented in visitEnd (S_staff_details& elt)

  // set staff details line number
  fCurrentStaffDetails->
    setStaffLinesNumber (
      fCurrentStaffLinesNumber);

  // append staff details in part or staff
  if (fStaffDetailsStaffNumber == 0)
    fCurrentPart->
      appendStaffDetailsToPart (
        fCurrentStaffDetails);

  else {
    S_msrStaff
      staff =
        fCurrentPartStavesVector [fStaffDetailsStaffNumber];

    staff->
      appendStaffDetailsToStaff (
        fCurrentStaffDetails);
  }

  // forget about this staff details
  fCurrentStaffDetails = nullptr;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_scordatura& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_scordatura" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
  <direction>
    <direction-type>
      <scordatura>
        <accord std::string="3">
          <tuning-step>C</tuning-step>
          <tuning-octave>3</tuning-octave>
          </accord>
        <accord std::string="2">
          <tuning-step>G</tuning-step>
          <tuning-octave>5</tuning-octave>
          </accord>
        <accord std::string="1">
          <tuning-step>E</tuning-step><
          tuning-octave>5</tuning-octave>
          </accord>
      </scordatura>
    </direction-type>
  </direction>
*/

  fCurrentScordatura =
    msrScordatura::create (
      elt->getInputStartLineNumber ());
}

void mxsr2msrSkeletonPopulator::visitStart (S_accord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accord" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
        <accord std::string="3">
          <tuning-step>C</tuning-step>
          <tuning-octave>3</tuning-octave>
          </accord>
*/

  fCurrentAccordNumber = elt->getAttributeIntValue ("std::string", 0);

  fCurrentAccordDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;

  fCurrentAccordAlterationKind =
    msrAlterationKind::kAlterationNatural; // default value

  fCurrentAccordOctaveKind =
    msrOctaveKind::kOctave_UNKNOWN_;

  fOnGoingAccord = true;
}

void mxsr2msrSkeletonPopulator::visitEnd (S_accord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_accord" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingAccord = false;

//   gLog <<
//     "S_tuning_octave: tuningOctaveNumber: " << tuningOctaveNumber << std::endl;

  S_msrStringTuning
    stringTuning =
      msrStringTuning::create (
        elt->getInputStartLineNumber (),
        fCurrentAccordNumber,
        fCurrentAccordDiatonicPitchKind,
        fCurrentAccordAlterationKind,
        fCurrentAccordOctaveKind);

  fCurrentScordatura->
    addStringTuningToScordatura (
      stringTuning);
}

void mxsr2msrSkeletonPopulator::visitEnd (S_scordatura& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_scordatura" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the current scordatura to the pending scordatura list
  fPendingScordaturasList.push_back (fCurrentScordatura);

  // forget about this scordatura
  fCurrentScordatura = nullptr;
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_instrument_sound& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument_sound" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_virtual_instrument& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_virtual_instrument" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_midi_device& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_midi_device" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
      <midi-device>SmartMusic SoftSynth</midi-device>
*/
}

//______________________________________________________________________________
void mxsr2msrSkeletonPopulator::visitStart (S_midi_instrument& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_midi_instrument" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
      <midi-instrument id="P2-I2">
        <midi-channel>2</midi-channel>
        <midi-program>70</midi-program>
        <volume>80</volume>
        <pan>4</pan>
      </midi-instrument>
*/
}


} // namespace



// void mxsr2msrSkeletonPopulator::convertWordsToTempo (
//   int                inputLineNumber,
//   const std::string& wordsValue)
// {
//   // create an msrWords containing wordsValue
//   S_msrWords
//     words =
//       msrWords::create (
//         inputLineNumber,
//         fCurrentDirectionPlacementKind,
//         wordsValue,
//         justifyKind,
//         horizontalAlignmentKind,
//         verticalAlignmentKind,
//         fontStyleKind,
//         fontSize,
//         fontWeightKind,
//         wordsXMLLangKind,
//         fCurrentDirectionStaffNumber);
//
//   // create an msrTempo containing words
//   S_msrTempo
//     tempo =
//       msrTempo::createTempoWordsOnly (
//         inputLineNumber,
//         words,
//         msrTempoParenthesizedKind::kTempoParenthesizedNo,    // JMI
//         msrPlacementKind::kPlacementAbove); // JMI
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (
//     gTraceOahGroup->getTraceWords ()
//       ||
//     gTraceOahGroup->getTraceDalSegnos ()
//   ) {
//     gLog <<
//       "Converting words \"" <<
//       wordsValue <<
//       "\" to an MSR tempo" <<
//       ", fCurrentDirectionStaffNumber: " <<
//       mfStaffNumberAsString (fCurrentDirectionStaffNumber) <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fPendingTemposList.push_back (tempo);
// }

// void mxsr2msrSkeletonPopulator::convertWordsToRehearsalMark (
//   int                inputLineNumber,
//   const std::string& wordsValue)
//     {
//       // create an msrWords containing wordsValue
//       S_msrRehearsalMark
//         rehearsalMark =
//           msrRehearsalMark::create (
//             inputLineNumber,
//             msrRehearsalMarkKind::kRehearsalMarkNone, // JMI allow for other values???
//             wordsValue,
//             fCurrentDirectionPlacementKind);
//
//     #ifdef MF_TRACE_IS_ENABLED
//       if (
//         gTraceOahGroup->getTraceWords ()
//           ||
//         gTraceOahGroup->getTraceDalSegnos ()
//       ) {
//         gLog <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR rehearsal mark" <<
//           ", fCurrentDirectionStaffNumber: " <<
//           mfStaffNumberAsString (fCurrentDirectionStaffNumber) <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif // MF_TRACE_IS_ENABLED
//
//       // append the rehearsalMark to the pending tempos list
//       fPendingRehearsalMarksList.push_back (rehearsalMark);
// }
//
// void mxsr2msrSkeletonPopulator::convertWordsToSegno (
//   int                inputLineNumber,
//   const std::string& wordsValue)
//     {
//       // create an msrWords containing wordsValue
//       S_msrRehearsalMark
//         rehearsalMark =
//           msrRehearsalMark::create (
//             inputLineNumber,
//             msrRehearsalMarkKind::kRehearsalMarkNone, // JMI allow for other values???
//             wordsValue,
//             fCurrentDirectionPlacementKind);
//
//     #ifdef MF_TRACE_IS_ENABLED
//       if (
//         gTraceOahGroup->getTraceWords ()
//           ||
//         gTraceOahGroup->getTraceDalSegnos ()
//       ) {
//         gLog <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR dal segno" <<
//           ", fCurrentDirectionStaffNumber: " <<
//           mfStaffNumberAsString (fCurrentDirectionStaffNumber) <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif // MF_TRACE_IS_ENABLED
//
//       // append dal segno to the pending tempos list
//       fPendingRehearsalMarksList.push_back (rehearsalMark);
// }
//
// {
//   // create an msrWords containing wordsValue
//   S_msrRehearsalMark
//     rehearsalMark =
//       msrRehearsalMark::create (
//         inputLineNumber,
//         msrRehearsalMarkKind::kRehearsalMarkNone, // JMI allow for other values???
//         wordsValue,
//         fCurrentDirectionPlacementKind);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (
//     gTraceOahGroup->getTraceWords ()
//       ||
//     gTraceOahGroup->getTraceDalSegnos ()
//   ) {
//     gLog <<
//       "Converting words \"" <<
//       wordsValue <<
//       "\" to an MSR dal segno al fine" <<
//       ", fCurrentDirectionStaffNumber: " <<
//       mfStaffNumberAsString (fCurrentDirectionStaffNumber) <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // append dal segno al fine to the pending tempos list
//   fPendingRehearsalMarksList.push_back (rehearsalMark);
// }
//
// void mxsr2msrSkeletonPopulator::convertWordsToDalSegnoAlCoda (
//   int                inputLineNumber,
//   const std::string& wordsValue)
// {
//   // create an msrWords containing wordsValue
//   S_msrRehearsalMark
//     rehearsalMark =
//       msrRehearsalMark::create (
//         inputLineNumber,
//         msrRehearsalMarkKind::kRehearsalMarkNone, // JMI allow for other values???
//         wordsValue,
//         fCurrentDirectionPlacementKind);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (
//     gTraceOahGroup->getTraceWords ()
//       ||
//     gTraceOahGroup->getTraceDalSegnos ()
//   ) {
//     gLog <<
//       "Converting words \"" <<
//       wordsValue <<
//       "\" to an MSR dal segno al coda" <<
//       ", fCurrentDirectionStaffNumber: " <<
//       mfStaffNumberAsString (fCurrentDirectionStaffNumber) <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // append dal segno al coda to the pending tempos list
//   fPendingRehearsalMarksList.push_back (rehearsalMark);
// }
// void mxsr2msrSkeletonPopulator::convertWordsToCoda (
//   int                inputLineNumber,
//   const std::string& wordsValue)
//     {
//       // create an msrWords containing wordsValue
//       S_msrRehearsalMark
//         rehearsalMark =
//           msrRehearsalMark::create (
//             inputLineNumber,
//             msrRehearsalMarkKind::kRehearsalMarkNone, // JMI allow for other values???
//             wordsValue,
//             fCurrentDirectionPlacementKind);
//
//     #ifdef MF_TRACE_IS_ENABLED
//       if (
//         gTraceOahGroup->getTraceWords ()
//           ||
//         gTraceOahGroup->getTraceDalSegnos ()
//       ) {
//         gLog <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR coda" <<
//           ", fCurrentDirectionStaffNumber: " <<
//           mfStaffNumberAsString (fCurrentDirectionStaffNumber) <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif // MF_TRACE_IS_ENABLED
//
//       // append coda to the pending tempos list
//       fPendingRehearsalMarksList.push_back (rehearsalMark);
// }
//
// void mxsr2msrSkeletonPopulator::convertWordsToCresc (
//   int                inputLineNumber,
//   const std::string& wordsValue)
//     {
//       // create an msrWords containing wordsValue
//       S_msrCrescDecresc
//         crescDecresc =
//           msrCrescDecresc::create (
//             inputLineNumber,
//             msrCrescDecrescKind::kCrescDecrescCrescendo);
//
//       fPendinCrescDecrescsList.push_back (crescDecresc);
//
//     #ifdef MF_TRACE_IS_ENABLED
//       if (
//         gTraceOahGroup->getTraceWords ()
//           ||
//         gTraceOahGroup->getTraceDalSegnos ()
//       ) {
//         gLog <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR cresc" <<
//           ", fCurrentDirectionStaffNumber: " <<
//           mfStaffNumberAsString (fCurrentDirectionStaffNumber) <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif // MF_TRACE_IS_ENABLED
//
//       // append the rehearsalMark to the pending tempos list
//       fPendinCrescDecrescsList.push_back (crescDecresc);
// }



// void mxsr2msrSkeletonPopulator::convertWordsToDecresc (
//   int                inputLineNumber,
//   const std::string& wordsValue)
//     {
//       // create an msrWords containing wordsValue
//       S_msrCrescDecresc
//         crescDecresc =
//           msrCrescDecresc::create (
//             inputLineNumber,
//             msrCrescDecrescKind::kCrescDecrescCrescendo);
//
//       fPendinCrescDecrescsList.push_back (crescDecresc);
//
//     #ifdef MF_TRACE_IS_ENABLED
//       if (
//         gTraceOahGroup->getTraceWords ()
//           ||
//         gTraceOahGroup->getTraceDalSegnos ()
//       ) {
//         gLog <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR decresc" <<
//           ", fCurrentDirectionStaffNumber: " <<
//           mfStaffNumberAsString (fCurrentDirectionStaffNumber _<<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif // MF_TRACE_IS_ENABLED
//
//       // append the rehearsalMark to the pending tempos list
//       fPendinCrescDecrescsList.push_back (crescDecresc);
// }

//             switch (noteVoice->getRegularVoiceStaffSequentialNumber ()) {
//               case 1:
//               case 3:
// #ifdef MF_TRACE_IS_ENABLED
//                 if (gTraceOahGroup->getTraceLigatures ()) {
//                   gLog <<
//                     "Attaching pending ligature above to note " <<
//                     note->asString () <<
//                     " in voice \"" <<
//                     noteVoice->getVoiceName () <<
//                     "\"" <<
//                     ", line " << ligature->getInputStartLineNumber () <<
//                     std::endl;
//                 }
// #endif // MF_TRACE_IS_ENABLED
//
//                 note->appendLigatureToCurrentNote (ligature);
//                 break;
//               default:
//                 ;
//             } // switch

//             switch (noteVoice->getRegularVoiceStaffSequentialNumber ()) {
//               case 2:
//               case 4:
// #ifdef MF_TRACE_IS_ENABLED
//                 if (gTraceOahGroup->getTraceLigatures ()) {
//                   gLog <<
//                     "Attaching pending ligature below to note " <<
//                     note->asString () <<
//                     " in voice \"" <<
//                     noteVoice->getVoiceName () <<
//                     "\"" <<
//                     ", line " << ligature->getInputStartLineNumber () <<
//                     std::endl;
//                 }
// #endif // MF_TRACE_IS_ENABLED
//
//                 note->appendLigatureToCurrentNote (ligature);
//                 break;
//               default:
//                 ;
//             } // switch





//     Bool doCreateASkipSyllable =
//      // ! fASkipSyllableHasBeenGeneratedForcurrentNote; JMI
//       ! fCurrentSyllableElementsList.size ();
//
//     switch (fCurrentSyllableExtendKind) { // JMI v0.9.68
//       case msrSyllableExtendKind::kSyllableExtend_NONE:
//         break;
//       case msrSyllableExtendKind::kSyllableExtendTypeLess:
// //         doCreateASkipSyllable = true; // JMI
//         break;
//       case msrSyllableExtendKind::kSyllableExtendTypeStart:
//         break;
//       case msrSyllableExtendKind::kSyllableExtendTypeContinue:
//  //        doCreateASkipSyllable = true; // JMI
//         break;
//       case msrSyllableExtendKind::kSyllableExtendTypeStop:
//         break;
//     } // switch
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceLyrics ()) {
//       std::stringstream ss;
//
//       ss <<
//         "*** ---> handleLyricsForCurrentNoteAfterItHasBeenHandled()" <<
//         ", doCreateASkipSyllable: " << doCreateASkipSyllable <<
//         std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     if (doCreateASkipSyllable) {
//       if (
//         ! (fCurrentNoteBelongsToAChord || fCurrentNoteIsAGraceNote)
//       ) {
//         // get the current note voice's stanzas map
//         const std::map <std::string, S_msrStanza>&
//           voiceStanzasMap =
//             fCurrentPartStaffMsrVoicesMap
//         [fCurrentNoteStaffNumber][fCurrentNoteVoiceNumber]->
//               getVoiceStanzasMap ();
//
//         for (
//           std::map <std::string, S_msrStanza>::const_iterator i = voiceStanzasMap.begin ();
//           i != voiceStanzasMap.end ();
//           ++i
//         ) {
//           S_msrStanza stanza = (*i).second;
//
//           //choose the syllable kind
//           msrSyllableKind
//             syllableKind =
//             fCurrentNoteIsARest
//               ? msrSyllableKind::kSyllableSkipOnRestNote
//               : msrSyllableKind::kSyllableSkipOnNonRestNote;
//
//           // create a skip syllable
//           S_msrSyllable
//             syllable =
//               msrSyllable::create (
//                 currentInputLineNumber,
//                 syllableKind,
//                 fCurrentSyllableExtendKind,
//                 fCurrentStanzaNumber,
//                 fCurrentNoteSoundingWholeNotesFromNotesDuration,
//                 msrTupletFactor (
//                   fCurrentNoteActualNotes,
//                   fCurrentNoteNormalNotes),
//                 stanza);
//
//           // set syllable note upLink to fCurrentNote
//           syllable->
//             appendSyllableToNoteAndSetItsUpLinkToNote (
//             	fCurrentNote);
//
//           // append syllable to stanza
//           stanza->
//             appendSyllableToStanza (syllable);
//         } // for
//       }
//     }


// //______________________________________________________________________________
// void mxsr2msrSkeletonPopulator::attachPendingVoicesWedgesToCurrentNoteIfRelevant ( // superflous??? JMI v0.9.71
//   int theVoiceNumber)
// {
//   // attach the pending wedges to the note
//   Bool delayAttachment (false);
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceWedges ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Attaching pending voice wedges to note " <<
//       fCurrentNote->asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (fCurrentNoteIsARest) {
//     if (gGlobalMxsr2msrOahGroup->getDelayRestsWedges ()) {
//       gLog <<
//         "Delaying voice wedge attached to a rest until next note" <<
//     std::endl;
//
//       delayAttachment = true;
//     }
//
//     else {
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTraceWedges ()) {
//         std::stringstream ss;
//
//         int numberOfWedges = fPendingVoiceWedgesList.size ();
//
//         ss <<
//           "fPendingVoiceWedgesList contains " <<
//           mfSingularOrPlural (
//             numberOfWedges, "element", "elements");
//
//         mxsr2msrWarning (
//           gServiceRunData->getInputSourceName (),
//           fCurrentNote->getInputStartLineNumber (),
//           ss.str ());
//       }
// #endif // MF_TRACE_IS_ENABLED
//     }
//   }
//
//   if (! delayAttachment) {
//     while (fPendingVoiceWedgesList.size ()) {
//       // grab voiceWedgePair
//       std::pair <int, S_msrWedge>
//         voiceWedgePair =
//           fPendingVoiceWedgesList.front ();
//
//       int
//         voiceNumber =
//           voiceWedgePair.first;
//
//       S_msrWedge
//         wedge =
//           voiceWedgePair.second;
//
//       if (theVoiceNumber == voiceNumber) {
//         fCurrentNote->appendWedgeToNote (wedge);
//       }
//
//       fPendingVoiceWedgesList.pop_front ();
//     } // while
//   }
// }

