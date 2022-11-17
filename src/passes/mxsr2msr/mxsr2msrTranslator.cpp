/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // std::setw, std::setprecision, ...

#include "xml_tree_browser.h"

#include "wae.h"
#include "mxsr2msrWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mxsr2msrComponent.h"

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "mxsrOah.h"
#include "mxsr2msrOah.h"
#include "msrOah.h"

#include "mxsr2msrTranslator.h"


namespace MusicFormats
{

//________________________________________________________________________
mxsr2msrTranslator::mxsr2msrTranslator (
  S_msrScore scoreSkeleton)
{
  // initialize note data to a neutral state
  initializeNoteData ();

  // the MSR score we're populating
  fMsrScore = scoreSkeleton;

  // divisions
  fCurrentDivisionsPerQuarterNote = 1;

  // scaling handling
  fCurrentMillimeters = -1;
  fCurrentTenths      = -1;

  // page layout
  fCurrentPageMarginsTypeKind = msrMarginTypeKind::kMarginBoth; // default value

  // print
  fCurrentDisplayText = "";

  // measure style handling
  fCurrentSlashTypeKind = msrSlashTypeKind::kSlashType_NO_;
  fCurrentUseDotsKind   = msrUseDotsKind::kUseDots_NO_;
  fCurrentSlashUseStemsKind = msrSlashUseStemsKind::kSlashUseStems_NO_;

  fCurrentBeatRepeatSlashes = -1;

  fCurrentMeasureRepeatKind =
    msrMeasureRepeatKind::kMeasureRepeat_NO_;

  fCurrentMeasureRepeatMeasuresNumber = -1;
  fCurrentMeasureRepeatSlashesNumber  = -1;

  fCurrentMultipleFullBarRestsNumber  = 0;
  fRemainingExpectedMultipleFullBarRests = 0;

  fCurrentSlashDotsNumber = -1;
  fCurrentSlashGraphicDurationKind = msrDurationKind::kDuration_NO_;

  // staff details handling
  fStaffDetailsStaffNumber = msrStaff::K_NO_STAFF_NUMBER;

  fCurrentStaffTypeKind =
    msrStaffTypeKind::kStaffTypeRegular;

  fCurrentShowFretsKind =
    msrShowFretsKind::kShowFretsNumbers; // default value

  fCurrentPrintObjectKind =
    msrPrintObjectKind::kPrintObjectYes; // default value

  fCurrentPrintSpacingKind =
    msrPrintSpacingKind::kPrintSpacingNo; // default value ??? JMI

  // staff tuning handling
  fCurrentStaffTuningAlterationKind = msrAlterationKind::kAlteration_NO_;
  fCurrentStaffTuningOctaveKind     = msrOctaveKind::kOctave_NO_;

  fCurrentStaffDetailsCapo = 0;
  fCurrentStaffDetailsStaffSize = 0;

  // scordatura handling

  fCurrentAccordNumber = -1;
  fCurrentAccordDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fCurrentAccordAlterationKind    = msrAlterationKind::kAlteration_NO_;
  fCurrentAccordOctaveKind        = msrOctaveKind::kOctave_NO_;

  // staff handling
  fPreviousNoteMusicXMLStaffNumber = msrStaff::K_NO_STAFF_NUMBER;
  fCurrentMusicXMLStaffNumber      = msrStaff::K_NO_STAFF_NUMBER;

  // staff change detection
  fCurrentStaffNumberToInsertInto = 1; // default value JMI

  // cross staff chords
  fCurrentChordStaffNumber = msrStaff::K_NO_STAFF_NUMBER;

  // voice handling
  fPreviousMusicXMLVoiceNumber = msrVoice::K_NO_VOICE_NUMBER;
  fCurrentMusicXMLVoiceNumber  = msrVoice::K_NO_VOICE_NUMBER;

  // clef handling
  fCurrentClefStaffNumber = msrStaff::K_NO_STAFF_NUMBER;
  fCurrentClefSign = "";
  fCurrentClefLine = -1;
  fCurrentClefOctaveChange = -77;

  // key handling
  fCurrentKeyKind = msrKeyKind::kKeyTraditional;

  fCurrentKeyStaffNumber = msrStaff::K_NO_STAFF_NUMBER;
  fCurrentKeyFifths = -1;
  fCurrentKeyCancelFifths = -37;
  fCurrentModeKind = msrModeKind::kMode_NO_;

  // time handling
  fCurrentTimeSignatureSymbolKind =
    msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone; // default value

  // codas handling
  fCodasCounter = 0;

  // measures
  fPartMeasuresCounter = 0;
  fScoreFirstMeasureNumber = "";
  fPartFirstMeasureNumber = "";
  fCurrentMeasureNumber = "???";

  fPreviousMeasureEndInputLineNumber = -1;

  // transpose handling
  fCurrentTransposeNumber = -213;
  fCurrentTransposeDiatonic = -214;
  fCurrentTransposeChromatic = -215;

  // direction handling
  fCurrentDirectionStaffNumber = 1; // it may be absent
  fCurrentDirectionVoiceNumber = 1; // it may be absent

  // accordion-registration handling
  fCurrentAccordionHigh   = 0;
  fCurrentAccordionMiddle = 0;
  fCurrentAccordionLow    = 0;

  fCurrentAccordionNumbersCounter = 0;

  // metronome handling
  fCurrentMetrenomePerMinute = "";
  fCurrentMetronomeParenthesedKind = msrTempoParenthesizedKind::kTempoParenthesizedNo;

  fCurrentMetrenomeDotsNumber = 0;
  fCurrentMetrenomeRelationKind = msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone;
  fCurrentMetronomeDurationKind = msrDurationKind::kDuration_NO_;
  fCurrentMetronomeBeamValue = "";

  fCurrentMetrenomeNormalDotsNumber = 0;

  // time handling
  fCurrentTimeStaffNumber = msrStaff::K_NO_STAFF_NUMBER;
  fCurrentTimeSignatureBeats = "";

  // lyrics handling
  fCurrentStanzaNumber = msrStanza::K_NO_STANZA_NUMBER;
  fCurrentStanzaName = msrStanza::K_NO_STANZA_NAME;

  fCurrentSyllabic = "";

  fCurrentSyllableKind       = msrSyllableKind::kSyllableNone;
  fCurrentSyllableExtendKind = msrSyllableExtendKind::kSyllableExtendNone;

  fFirstSyllableInSlurKind     = msrSyllableKind::kSyllableNone;
  fFirstSyllableInLigatureKind = msrSyllableKind::kSyllableNone;

  // harmonies handling
  fHarmoniesVoicesCounter = 0;

  fCurrentHarmonyRootDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fCurrentHarmonyRootAlterationKind    = msrAlterationKind::kAlteration_NO_;
  fCurrentHarmonyKind                  = msrHarmonyKind::kHarmony_NO_;
  fCurrentHarmonyKindText              = "";
  fCurrentHarmonyBassDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fCurrentHarmonyBassAlterationKind    = msrAlterationKind::kAlteration_NO_;
  fCurrentHarmonyDegreeValue           = -1;
  fCurrentHarmonyDegreeAlterationKind  = msrAlterationKind::kAlteration_NO_;

  fCurrentHarmoniesStaffNumber = msrStaff::K_NO_STAFF_NUMBER;

  fCurrentHarmonyWholeNotesOffset = Rational (0, 1);

  // figured bass handling
  fFiguredBassVoicesCounter = 0;

  fCurrentFiguredBassSoundingWholeNotes = Rational (0, 1);
  fCurrentFiguredBassDisplayWholeNotes  = Rational (0, 1);
  fCurrentFiguredBassParenthesesKind =
    msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo; // default value
  fCurrentFigureNumber                  = -1;

  // frames handling
  fCurrentFrameStrings  = -1;
  fCurrentFrameFrets    = -1;
  fCurrentFrameFirstFret = -1;
  fCurrentFrameNoteStringNumber = -1;
  fCurrentFrameNoteFretNumber = -1;
  fCurrentFrameNoteFingering = -1;
  fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeNone;

  // barLine handling
  fCurrentBarLineEndingNumber    = ""; // may be "1, 2"
  fCurrentBarLineHasSegnoKind = msrBarLineHasSegnoKind::kBarLineHasSegnoNo;
  fCurrentBarLineHasCodaKind  = msrBarLineHasCodaKind::kBarLineHasCodaNo;

  fCurrentBarLineLocationKind        = msrBarLineLocationKind::kBarLineLocationNone;
  fCurrentBarLineStyleKind           = msrBarLineStyleKind::kBarLineStyleNone;
  fCurrentBarLineEndingTypeKind      = msrBarLineEndingTypeKind::kBarLineEndingTypeNone;
  fCurrentBarLineRepeatDirectionKind = msrBarLineRepeatDirectionKind::kBarLineRepeatDirectionNone;
  fCurrentBarLineRepeatWingedKind    = msrBarLineRepeatWingedKind::kBarLineRepeatWingedNone;

  // repeats handling
  fRepeatEndCounter = 0;
  fCurrentRepeatStartMeasureNumber = "";
  fCurrentRepeatEndingStartBarLine = nullptr;

  // MusicXML notes handling
  fCurrentNoteDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fCurrentNoteAlterationKind    = msrAlterationKind::kAlteration_NO_;

  // note print object kind
  fCurrentNotePrintObjectKind = msrPrintObjectKind::kPrintObjectNone;

  // note head
  fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadNormal;
  fCurrentNoteHeadFilledKind = msrNoteHeadFilledKind::kNoteHeadFilledYes;
  fCurrentNoteHeadParenthesesKind = msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo;

  // technicals handling
  fBendAlterValue = -39.3f;

  // tremolos handling
  fCurrentDoubleTremoloTypeKind = msrDoubleTremoloTypeKind::kDoubleTremoloType_NO_;

  // tuplets handling
  fCurrentTupletNumber  = -1;
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

  fCurrentForwardStaffNumber = msrStaff::K_NO_STAFF_NUMBER;
  fCurrentForwardVoiceNumber = msrVoice::K_NO_VOICE_NUMBER;
}

mxsr2msrTranslator::~mxsr2msrTranslator ()
{}

//________________________________________________________________________
void mxsr2msrTranslator::browseMxsr (
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
void mxsr2msrTranslator::initializeNoteData ()
{
  // basic note description

// JMI  fCurrentNoteKind = msrNoteKind::kNote_NO_;

  fCurrentNoteQuarterTonesPitchKind =
    msrQuarterTonesPitchKind::kQTP_NO_;

  fCurrentNoteSoundingWholeNotes             = Rational (-13, 1);
  fCurrentNoteSoundingWholeNotesFromDuration = Rational (-17, 1);

  fCurrentNoteDisplayWholeNotes         = Rational (-25, 1);
  fCurrentNoteDisplayWholeNotesFromType = Rational (-29, 1);

  fCurrentNoteDotsNumber = 0;

  fCurrentNoteGraphicDurationKind = msrDurationKind::kDuration_NO_;

  fCurrentNoteOctave = msrOctaveKind::kOctave_NO_;

  fCurrentNoteQuarterTonesDisplayPitchKind =
    msrQuarterTonesPitchKind::kQTP_NO_;
  fCurrentDisplayDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fCurrentDisplayOctave =
    msrOctaveKind::kOctave_NO_;

  // cue notes

  fCurrentNoteIsACueNoteKind = msrNoteIsACueNoteKind::kNoteIsACueNoteNo;

  // accidentals

  fCurrentAccidentalKind =
    msrAccidentalKind::kAccidentalNone; // default value

  fCurrentEditorialAccidentalKind =
    msrEditorialAccidentalKind::kEditorialAccidentalNo; // default value

  fCurrentCautionaryAccidentalKind =
    msrCautionaryAccidentalKind::kCautionaryAccidentalNo; // default value

  // current note staff number
  fCurrentMusicXMLStaffNumber = 1; // default value, it may be absent

  // current note voice number
  fPreviousMusicXMLVoiceNumber = fCurrentMusicXMLVoiceNumber;
  fCurrentMusicXMLVoiceNumber  = 1; // default value, it may be absent

  // staff change detection
  // fCurrentStaffNumberToInsertInto = 1; // JMI ???

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

// JMI  fCurrentNoteSyllableExtendKind = kSyllableExtendNone;
}

//________________________________________________________________________
void mxsr2msrTranslator::printVoicesLastMetNoteMap (
  int           inputLineNumber,
  const std::string& context)
{
  size_t
    voicesLastMetNoteMapSize =
      fVoicesLastMetNoteMap.size ();

  gLogStream <<
    std::endl <<
    "fVoicesLastMetNoteMap contains " <<
    mfSingularOrPlural (
      voicesLastMetNoteMapSize, "element", "elements") <<
    ", context: " << context <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  if (voicesLastMetNoteMapSize) {
    ++gIndenter;

//    std::map<S_msrVoice, S_msrNote>::const_iterator
    std::map<std::pair<int, int>, S_msrNote>::const_iterator
      iBegin = fVoicesLastMetNoteMap.begin (),
      iEnd   = fVoicesLastMetNoteMap.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream <<
  //      (*i).first->getVoiceName () <<
        "staff " << (*i).first.first <<
        ", voice " <<  (*i).first.second <<
        ":" <<
        std::endl;

      ++gIndenter;

      gLogStream <<
        (*i).second <<
        std::endl;

      --gIndenter;

      if (++i == iEnd) break;

      gLogStream << std::endl;
    } // for

    --gIndenter;
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::checkStep (
  int           inputLineNumber,
  const std::string& stepValue,
	const std::string& markup)
{
  if (stepValue.size () == 1) {
    char firstChar = stepValue [0];

    if (firstChar < 'A' || firstChar > 'G') {
      std::stringstream s;

      s <<
      	markup <<
        " value " << firstChar <<
        " is not a letter from A to G";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  else {
    std::stringstream s;

    s <<
			markup <<
      " value '" << stepValue <<
      "' should be a single letter from A to G";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
      s.str ());
  }
}

//______________________________________________________________________________
S_msrStaff mxsr2msrTranslator::fetchStaffFromCurrentPart (
  int inputLineNumber,
  int staffNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Fetching staff " <<
      staffNumber <<
      " from current part" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    staffNumber != msrStaff::K_NO_STAFF_NUMBER,
    "staffNumber == msrStaff::K_NO_STAFF_NUMBER");

  // fetch the staff from current part
  S_msrStaff
    staff =
      fCurrentPart->
        fetchStaffFromPart (staffNumber);

  // sanity check
  if (! staff) {
    std::stringstream s;

    s <<
      "staff '" << staffNumber <<
      "' not found in score skeleton's part " <<
      fCurrentPart->getPartCombinedName ();

    mxsr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "--> fetchStaffFromCurrentPart returns " <<
      staff->getStaffName () <<
      std::endl;
  }
#endif

  return staff;
}

//______________________________________________________________________________
S_msrVoice mxsr2msrTranslator::fetchVoiceFromCurrentPart (
  int inputLineNumber,
  int staffNumber,
  int voiceNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Fetching voice " <<
      voiceNumber <<
      " in staff " <<
      staffNumber <<
      " from current part" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // the voice number is relative to a part,

  // fetch the staff from current part
  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber,
        staffNumber);

  // fetch the voice from the staff
  S_msrVoice
    voice =
      staff->
        fetchRegularVoiceFromStaffByItsNumber (
          inputLineNumber,
          voiceNumber);

  // sanity check
  if (! voice) {
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      staff <<
      std::endl;
  }
#endif

    std::stringstream s;

    s <<
      "voice '" << voiceNumber <<
      "' not found in score skeleton's staff \"" <<
      staff->getStaffName () <<
      "\"";

    mxsr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

    /* JMI
  // fetch registered voice displaying staff number
  int voiceDisplayingStaffNumber = 1; //msrVoice::K_NO_VOICE_NUMBER; JMI
    // default, there may be no <staff /> markups
    */

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> fetchVoiceFromCurrentPart() returns " <<
      voice->getVoiceName () <<
      std::endl;
  }
#endif

  return voice;
}

//______________________________________________________________________________
S_msrVoice mxsr2msrTranslator::fetchFirstVoiceFromCurrentPart (
  int inputLineNumber)
{
  int staffNumber = 1;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Fetching first voice in staff " <<
      staffNumber <<
      " from current part" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // fetch the staff from current part
  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber,
        staffNumber);

  // fetch the first voice from the staff
  S_msrVoice
    voice =
      staff->
        fetchFirstRegularVoiceFromStaff (
          inputLineNumber);

  // sanity check
  if (! voice) {
    staff->print (gLogStream); // JMI

    std::stringstream s;

    s <<
      "first voice not found in score skeleton's staff \"" <<
      staff->getStaffName () <<
      "\"";

    mxsr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

    /* JMI
  // fetch registered voice displaying staff number
  int voiceDisplayingStaffNumber = 1; //msrVoice::K_NO_VOICE_NUMBER; JMI
    // default, there may be no <staff /> markups
    */

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "--> fetchVoiceFromCurrentPart() returns " <<
      voice->getVoiceName () <<
      std::endl;
  }
#endif

  return voice;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_millimeters& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_millimeters" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentMillimeters = (float)(*elt);
}

void mxsr2msrTranslator::visitStart (S_tenths& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tenths" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentTenths = (float)(*elt);
}

void mxsr2msrTranslator::visitEnd (S_scaling& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_scaling" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a scaling
  S_msrScaling
    scaling =
      msrScaling::create (
        inputLineNumber,
        fCurrentMillimeters,
        fCurrentTenths);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGeometry ()) {
    gLogStream <<
      "There are " << fCurrentTenths <<
      " tenths for " <<  fCurrentMillimeters <<
      std::endl;
  }
#endif

  // set the MSR score's scaling
  fMsrScore->
    setScaling (scaling);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_system_layout& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_system_layout" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the system layout
  fCurrentSystemLayout =
    msrSystemLayout::create (
      inputLineNumber);

  fOnGoingSystemLayout = true;
}

void mxsr2msrTranslator::visitEnd (S_system_layout& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_system_layout" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingPrint) {
    // set the current print layout's system layout
    fCurrentPrintLayout->
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
void mxsr2msrTranslator::visitStart (S_system_margins& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_system_margins" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (! fOnGoingSystemLayout) {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<system-margins /> is out of context");
  }

  fOnGoingSystemMargins = true;
}

void mxsr2msrTranslator::visitStart (S_system_distance& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_system_distance" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<system-distance /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_top_system_distance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_top_system_distance" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<top-system-distance /> is out of context");
  }
}

void mxsr2msrTranslator::visitEnd (S_system_margins& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_system_margins" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (! fOnGoingSystemLayout) {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<system-margins /> is out of context");
  }

  fOnGoingSystemMargins = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_system_dividers& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_system_dividers" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

    std::stringstream s;

    s <<
      "<system-dividers /> is not supported yet by " <<
      gGlobalOahOahGroup->getOahOahGroupServiceName ();

/* JMI
    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
     s.str ());
     */
}

void mxsr2msrTranslator::visitStart (S_left_divider& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_left_divider" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);
}

void mxsr2msrTranslator::visitStart (S_right_divider& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_right_divider" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_notations& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_notations" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // JMI
}

void mxsr2msrTranslator::visitStart (S_other_notation& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_other_notation" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_page_layout& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_page_layout" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a page layout
  fCurrentPageLayout =
    msrPageLayout::create (
      inputLineNumber);

  fOnGoingPageLayout = true;
}

void mxsr2msrTranslator::visitEnd (S_page_layout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_page_layout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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

void mxsr2msrTranslator::visitStart (S_page_height& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_page_height" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<page-height /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_page_width& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_page_width" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<page-width /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_page_margins& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_page_margins" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "unknown page margins type \"" <<
        pageMarginsType <<
        "\"";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
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
          inputLineNumber,
          fCurrentPageLayoutMarginsGroup);
      break;
    case msrMarginTypeKind::kMarginEven:
      fCurrentPageLayout->
        setEvenMarginsGroup (
          inputLineNumber,
          fCurrentPageLayoutMarginsGroup);
      break;
    case msrMarginTypeKind::kMarginBoth: // default value
      fCurrentPageLayout->
        setBothMarginsGroup (
          inputLineNumber,
          fCurrentPageLayoutMarginsGroup);
      break;
  } // switch

  fOnGoingPageMargins = true;
}

void mxsr2msrTranslator::visitEnd (S_page_margins& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_page_margins" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // forget about the current page layout margins group
  fCurrentPageLayoutMarginsGroup = nullptr;

  fOnGoingPageMargins = false;
}

void mxsr2msrTranslator::visitStart (S_left_margin& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  float leftMargin = (float)(*elt);

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_left_margin" <<
      ", " << leftMargin << " tenths" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  msrLength
    leftMarginLength (
      msrLengthUnitKind::kUnitMillimeter,
      leftMargin * fCurrentMillimeters / fCurrentTenths);

  if (fOnGoingPageMargins) {
    fCurrentPageLayoutMarginsGroup->
      setLeftMargin (
        inputLineNumber,
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
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<left-margin /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_right_margin& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  float rightMargin = (float)(*elt);

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_right_margin" <<
      ", " << rightMargin << " tenths" <<
       ", line " << inputLineNumber <<
     std::endl;
  }
#endif

  msrLength
    rightMarginLength (
      msrLengthUnitKind::kUnitMillimeter,
      rightMargin * fCurrentMillimeters / fCurrentTenths);

  if (fOnGoingPageMargins) {
    fCurrentPageLayoutMarginsGroup->
      setRightMargin (
        inputLineNumber,
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
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<right-margin /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_top_margin& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  float topMargin = (float)(*elt);

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_top_margin" <<
      ", " << topMargin << " tenths" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingPageMargins) {
    fCurrentPageLayoutMarginsGroup->
      setTopMargin (
        inputLineNumber,
        msrMargin::create (
          fCurrentPageMarginsTypeKind,
          msrLength (
            msrLengthUnitKind::kUnitMillimeter,
            topMargin * fCurrentMillimeters / fCurrentTenths)));
  }

  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<top-margin /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_bottom_margin& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  float bottomMargin = (float)(*elt);

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_bottom_margin" <<
      ", " << bottomMargin << " tenths" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingPageMargins) {
    fCurrentPageLayoutMarginsGroup->
      setBottomMargin (
        inputLineNumber,
        msrMargin::create (
          fCurrentPageMarginsTypeKind,
          msrLength (
            msrLengthUnitKind::kUnitMillimeter,
            bottomMargin * fCurrentMillimeters / fCurrentTenths)));
  }

  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<bottom-margin /> is out of context");
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_staff_layout& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff_layout" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      inputLineNumber,
      staffNumber);

  if (fOnGoingPrint) {
    // append it to the current print layout
    fCurrentPrintLayout->
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

void mxsr2msrTranslator::visitEnd (S_staff_layout& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_staff_layout" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // forget about the current staff layout
  fCurrentStaffLayout = nullptr;

  fOnGoingStaffLayout = false;
}

void mxsr2msrTranslator::visitStart (S_staff_distance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff_distance" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<staff-distance /> is out of context");
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_measure_layout& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_measure_layout" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a measure layout
  fCurrentMeasureLayout =
    msrMeasureLayout::create (
      inputLineNumber);

  fOnGoingMeasureLayout = true;
}

void mxsr2msrTranslator::visitEnd (S_measure_layout& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_measure_layout" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // forget about the current measure layout
  fCurrentMeasureLayout = nullptr;

  fOnGoingMeasureLayout = false;
}

void mxsr2msrTranslator::visitStart (S_measure_distance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_measure_distance" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<measure-distance /> is out of context");
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_appearance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_appearance" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      inputLineNumber);

  // set the MSR score appearance
  fMsrScore->
    setAppearance (
      fCurrentAppearance);

  fOnGoingAppearance = true;
}

void mxsr2msrTranslator::visitEnd (S_appearance& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_appearance" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // forget about the current appearance
  fCurrentAppearance = nullptr;

  fOnGoingAppearance = false;
}

void mxsr2msrTranslator::visitStart (S_line_width& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_line_width" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingAppearance) {
    // value
    float lineWidthTenths = (float)(*elt);

    // type
    std::string lineWidthTypeString = elt->getAttributeValue ("type");

    if (lineWidthTypeString.size ()) {
      msrLineWidthTypeKind
        lineWidthTypeKind =
          msrLineWidthTypeKind::kLineWidthType_NO_;

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
        std::stringstream s;

        s <<
          "<line-width /> type \"" <<
          lineWidthTypeString <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // create a line width
      S_msrLineWidth
        lineWidth =
          msrLineWidth::create (
            inputLineNumber,
            lineWidthTypeKind,
            msrLength::create (
              msrLengthUnitKind::kUnitMillimeter,
              lineWidthTenths * fCurrentMillimeters / fCurrentTenths));

      // append it to the current appearance
      fCurrentAppearance->
        appendLineWidth (lineWidth);
    }
    else {
      std::stringstream s;

      s <<
        "<line-width /> type \"" <<
        lineWidthTypeString <<
        "\" is missing";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<line-width /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_note_size& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_note_size" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingAppearance) {
    // value
    float noteSizePercentage = (float)(*elt);

    // type
    std::string noteSizeTypeString = elt->getAttributeValue ("type");

    if (noteSizeTypeString.size ()) {
      msrNoteSizeTypeKind
        noteSizeTypeKind =
          msrNoteSizeTypeKind::kNote_NO_SizeType;

      if      (noteSizeTypeString == "cue")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeCue;
      else if (noteSizeTypeString == "grace")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeGrace;
      else if (noteSizeTypeString == "large")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeLarge;

      else {
        std::stringstream s;

        s <<
          "<note-size /> type \"" <<
          noteSizeTypeString <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // create a note size
      S_msrNoteSize
        noteSize =
          msrNoteSize::create (
            inputLineNumber,
            noteSizeTypeKind,
            noteSizePercentage);

      // append it to the current appearance
      fCurrentAppearance->
        appendNoteSize (noteSize);
    }
    else {
      std::stringstream s;

      s <<
        "<note-size /> type \"" <<
        noteSizePercentage <<
        "\" is missing";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<note-size /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_distance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_distance" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingAppearance) {
    // value
    float distanceTenths = (float)(*elt);

    // type
    std::string distanceTypeString = elt->getAttributeValue ("type");

    if (distanceTypeString.size ()) {
      msrDistanceTypeKind
        distanceTypeKind =
          msrDistanceTypeKind::kDistanceType_NO_;

      if      (distanceTypeString == "hyphen")
        distanceTypeKind = msrDistanceTypeKind::kDistanceTypeHyphen;
      else if (distanceTypeString == "beam")
        distanceTypeKind = msrDistanceTypeKind::kDistanceTypeBeam;

      else {
        std::stringstream s;

        s <<
          "<distance /> type \"" <<
          distanceTypeString <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // create a distance
      S_msrDistance
        distance =
          msrDistance::create (
            inputLineNumber,
            distanceTypeKind,
            msrLength::create (
              msrLengthUnitKind::kUnitMillimeter,
              distanceTenths * fCurrentMillimeters / fCurrentTenths));

      // append it to the current appearance
      fCurrentAppearance->
        appendDistance (distance);
    }
    else {
      std::stringstream s;

      s <<
        "<distance /> type \"" <<
        distanceTypeString <<
        "\" is missing";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<distance /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_glyph& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_glyph" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingAppearance) {
    // value
    std::string glyphValue = elt->getValue ();

    // type
    std::string glyphTypeString = elt->getAttributeValue ("type");

    if (glyphTypeString.size ()) {
      msrGlyphTypeKind
        glyphTypeKind =
          msrGlyphTypeKind::kGlyphType_NO_;

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
        std::stringstream s;

        s <<
          "<glyph /> type \"" <<
          glyphTypeString <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // create a glyph
      S_msrGlyph
        glyph =
          msrGlyph::create (
            inputLineNumber,
            glyphTypeKind,
            glyphValue);

      // append it to the current appearance
      fCurrentAppearance->
        appendGlyph (glyph);
    }
    else {
      std::stringstream s;

      s <<
        "<glyph /> type \"" <<
        glyphTypeString <<
        "\" is missing";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<glyph /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_other_appearance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_other_appearance" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingAppearance) {
    // what can we find in such a markup??? JMI
  }
  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<other-appearance /> is out of context");
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_part& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_part" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string partID = elt->getAttributeValue ("id");

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceParts ()
      ||
    gGlobalOahEarlyOptions.getEarlyTracePasses ()
  ) {
    gLogStream <<
      std::endl <<
      "<!--=== part \"" << partID << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      std::endl;
  }
#endif

  // fetch current part from its partID
  fCurrentPart =
    fMsrScore->
      fetchPartFromScoreByItsPartID (
        inputLineNumber,
        partID);

  // sanity check
  if (! fCurrentPart) {
    // fetch fMsrScore's part std::list
    std::list<S_msrPart> partsList;

    fMsrScore->
      collectScorePartsList (
        inputLineNumber,
        partsList);

    if (partsList.size () == 1) {
      // there's only one part in the part std::list,
      // assume this is the one
      fCurrentPart =
        partsList.front ();

      partID =
        fCurrentPart->
          getPartID ();

      std::stringstream s;

      s <<
        "part 'id' is empty, using '" <<
        partID <<
        "' since it is the only part in the <part-std::list />";

      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }

    else {
      std::stringstream s;

      s <<
        "part \"" << partID <<
        "\" not found in score skeleton";

      mxsr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "--------------------------------------------" <<
      std::endl <<
      "Analyzing part " <<
      fCurrentPart->
        getPartCombinedName () <<
        " -- start" <<
      std::endl;
  }
#endif

  // no time has been defined yet
  fCurrentTimeSignature = nullptr;

  // staff numbers
  fPreviousNoteMusicXMLStaffNumber = msrStaff::K_NO_STAFF_NUMBER;
  fCurrentMusicXMLStaffNumber      = msrStaff::K_NO_STAFF_NUMBER;

  // staff change detection
  fCurrentStaffNumberToInsertInto = 1; // default value JMI msrStaff::K_NO_STAFF_NUMBER;

  // cross staff chords
  fCurrentNoteIsCrossStaves = false; // needed ??? JMI

  // get this part's staves std::map
  std::map<int, S_msrStaff>
    partStavesMap =
      fCurrentPart->
        getPartStaveNumbersToStavesMap ();

  // repeats
  fCurrentRepeatStartMeasureNumber = "";
  fCurrentRepeatEndingStartBarLine = nullptr;

  // measures
  fPartMeasuresCounter = 0;
  fScoreFirstMeasureNumber = "";
  fPartFirstMeasureNumber = "";
  fCurrentMeasureNumber = "???";

  fPreviousMeasureEndInputLineNumber = -1;

  fCurrentMusicXMLStaffNumber = msrStaff::K_NO_STAFF_NUMBER;
  fCurrentMusicXMLVoiceNumber = msrVoice::K_NO_VOICE_NUMBER;

  ++gIndenter;
}

void mxsr2msrTranslator::visitEnd (S_part& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_part" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  --gIndenter;

/*
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Analyzing part " <<
      fCurrentPart->
        getPartCombinedName () <<
        " -- end" <<
      std::endl <<
      "--------------------------------------------" <<
      std::endl;
  }
#endif
*/

  // fetch current note's voice
  S_msrVoice
    currentNoteVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    currentNoteVoice != nullptr,
    "currentNoteVoice is null");

  // the elements pending since before the note if any
  // can now be appended to the latter's voice
  // prior to the note itself
  attachPendingVoiceLevelElementsToVoice (
    currentNoteVoice);

  attachPendingPartLevelElementsToPart (
    fCurrentPart);

  // finalize the current part
  fCurrentPart->
    finalizePart (
      inputLineNumber);

  // is this part name in the parts ignore IDs set?
  if (gGlobalMxsr2msrOahGroup->getPartsIgnoreIDSet ().size ()) {
    std::set<std::string>::iterator
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
            inputLineNumber,
            fCurrentPart);
    }
  }

  // is this part name in the parts keep IDs set?
  if (gGlobalMxsr2msrOahGroup->getMusicXMLPartsKeepIDSet ().size ()) {
    std::set<std::string>::iterator
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
            inputLineNumber,
            fCurrentPart);
    }
  }

  // is this part name in the parts ignore names set?
  if (gGlobalMxsr2msrOahGroup->getMusicXMLMusicXMLPartsIgnoreNameSet ().size ()) {
    std::set<std::string>::iterator
      it =
        gGlobalMxsr2msrOahGroup->getMusicXMLMusicXMLPartsIgnoreNameSet ().find (
          fCurrentPart->
            getPartName ());

    if (it != gGlobalMxsr2msrOahGroup->getMusicXMLMusicXMLPartsIgnoreNameSet ().end ()) {
      // the simplest way to ignore this part
      // is to remove it from its part-group
      // now that is has been completely built and populated
      fCurrentPart->
        getPartUpLinkToPartGroup ()->
          removePartFromPartGroup (
            inputLineNumber,
            fCurrentPart);
    }
  }

  // is this part name in the parts keep names set?
  if (gGlobalMxsr2msrOahGroup->getMusicXMLPartsKeepNameSet ().size ()) {
    std::set<std::string>::iterator
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
            inputLineNumber,
            fCurrentPart);
    }
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_attributes& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_attributes" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitEnd (S_attributes& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_attributes" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // have the divisions been defined alright?
  if (! fCurrentDivisions) {
    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      "no <divisions/> markup found in MusicXML data, using 1 by default");

    fCurrentDivisionsPerQuarterNote = 1;

    // create default current divisions
    fCurrentDivisions =
      msrDivisions::create (
        inputLineNumber,
        fCurrentDivisionsPerQuarterNote);
  }

  // JMI and if there's no <attributes/> ???
  // time is crucially needed for measures management,
  // we cannot stay without any
  if (! fCurrentTimeSignature) {
    /* JMI
    // create the default 4/4 time
    fCurrentTimeSignature =
      msrTimeSignature::createFourQuartersTime (
        inputLineNumber);

    // register time in staff
    fCurrentPart->
      appendTimeSignatureToPart (fCurrentTimeSignature);
      */
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_divisions& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_divisions" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentDivisionsPerQuarterNote = (int)(*elt);

  if (
    fCurrentDivisionsPerQuarterNote < 1
      ||
    fCurrentDivisionsPerQuarterNote > 16383
  ) {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "divisions per quarter note should be between 1 and 16383");
  }

  // set current part's divisions per quarter note
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
    if (fCurrentDivisionsPerQuarterNote == 1) {
      gLogStream <<
        "There is 1 division";
    }
    else {
      gLogStream <<
        "There are " <<
        fCurrentDivisionsPerQuarterNote <<
        " divisions";
    }

    gLogStream <<
      " per quarter note in part " <<
      fCurrentPart->getPartCombinedName() <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create current divisions
  fCurrentDivisions =
    msrDivisions::create (
      inputLineNumber,
      fCurrentDivisionsPerQuarterNote);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_clef& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_clef" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      inputLineNumber,
      printObjectString);

  fCurrentClefLine = 0;
  fCurrentClefOctaveChange = 0;
  fCurrentClefSign = "";
}

void mxsr2msrTranslator::visitStart (S_sign& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_sign" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentClefSign = elt->getValue();
}

void mxsr2msrTranslator::visitStart (S_line& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_line" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentClefLine = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_clef_octave_change& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_clef_octave_change" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentClefOctaveChange = (int)(*elt);

  if (fCurrentClefOctaveChange < -2 || fCurrentClefOctaveChange > 2) {
    std::stringstream s;

    s <<
      "clef-octave-change \"" << fCurrentClefOctaveChange <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitEnd (S_clef& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_clef" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // convert clef to upper case for analysis
  std::string currentClefSignToUpperCase =
    mfStringToUpperCase (fCurrentClefSign);

  msrClefKind clefKind = msrClefKind::kClef_NO_;

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
      std::stringstream s;

      s <<
        "'G' clef line \"" << fCurrentClefLine <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
      std::stringstream s;

      s <<
        "'F' clef line \"" << fCurrentClefLine <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
          std::stringstream s;

          s <<
            "'C' clef line \"" << fCurrentClefLine <<
            "\" is unknown";

          mxsr2msrError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
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
          std::stringstream s;

          s <<
            "tablature line \"" << fCurrentClefLine <<
            "\" is unknown";

          mxsr2msrError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
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
    clefKind = msrClefKind::kClef_NO_;
  }

  else {
    // unknown clef sign
    std::stringstream s;

    s <<
      "clef sign \"" << fCurrentClefSign <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // is this clef sign in the replace clef std::map?
  const std::map<msrClefKind, msrClefKind>&
    replaceClefKindToClefKindMapVariable =
      gGlobalMxsr2msrOahGroup->
        getReplaceClefKindToClefKindMapVariable ();

  std::map<msrClefKind, msrClefKind>::const_iterator
    it =
      replaceClefKindToClefKindMapVariable.find (clefKind);

  if (it != replaceClefKindToClefKindMapVariable.end ()) {
    // yes, replace the clef accordinglingly
    msrClefKind
      newClefKind = (*it).second;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceClefs ()) {
      gLogStream <<
        "Replace clef " <<
        msrClefKindAsString (clefKind) <<
        " by " <<
        msrClefKindAsString (newClefKind) <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    clefKind = newClefKind;
  }

  // create clef
  S_msrClef
    clef =
      msrClef::create (
        inputLineNumber,
        clefKind,
        fCurrentClefStaffNumber);

  // register clef in part or staff
  if (fCurrentClefStaffNumber == 0) {
    fCurrentPart->
      appendClefToPart (clef);
  }
  else {
    S_msrStaff
      staff =
        fetchStaffFromCurrentPart (
          inputLineNumber,
          fCurrentClefStaffNumber);

    staff->
      appendClefToStaff (clef);
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_key& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_key" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      inputLineNumber,
      printObjectString);

  // traditional

  fCurrentKeyFifths       = 0;
  fCurrentKeyCancelFifths = 0;

  fCurrentModeKind = msrModeKind::kMode_NO_;

  // Humdrum-Scot

  fCurrentHumdrumScotKeyItem = nullptr;
}

void mxsr2msrTranslator::visitStart (S_cancel& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_cancel" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentKeyCancelFifths = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_fifths& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_fifths" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentKeyKind = msrKeyKind::kKeyTraditional;

  fCurrentKeyFifths = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_mode& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_mode" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string mode = elt->getValue();

  if       (mode == "[NONE]") {
    fCurrentModeKind = msrModeKind::kMode_NO_;
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
    std::stringstream s;

    s <<
      "mode '" << mode << "' is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_key_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_key_step" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fCurrentHumdrumScotKeyItem) {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "Humdrum/Scot key step found while another one is being handled");
  }

  fCurrentKeyKind = msrKeyKind::kKeyHumdrumScot;

  std::string step = elt->getValue();

  // check the step value
  checkStep (
    inputLineNumber,
    step,
    "<key-step/>");

  // determine diatonic pitch
  msrDiatonicPitchKind keyDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (step [0]);

  // create the Humdrum/Scot item
  fCurrentHumdrumScotKeyItem =
    msrHumdrumScotKeyItem::create (
      inputLineNumber);

  // populate it with the diatonic pitch
  fCurrentHumdrumScotKeyItem->
    setKeyItemDiatonicPitchKind (
      keyDiatonicPitchKind);

  // insert it into the items std::vector
  fCurrentHumdrumScotKeyItemsVector.
    insert (
      fCurrentHumdrumScotKeyItemsVector.end (),
      fCurrentHumdrumScotKeyItem);
}

void mxsr2msrTranslator::visitStart (S_key_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_key_alter" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (! fCurrentHumdrumScotKeyItem) {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "Humdrum/Scot key alter found while no key step is being handled");
  }

  float keyAlter = (float)(*elt);

  // determine the alteration
  msrAlterationKind
    keyAlterationKind =
      msrAlterationKindFromMusicXMLAlter (
        keyAlter);

  if (keyAlterationKind == msrAlterationKind::kAlteration_NO_) {
    std::stringstream s;

    s <<
      "key alter '" << keyAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // complement the current Humdrum/Scot item with the alteration
  fCurrentHumdrumScotKeyItem->
    setKeyItemAlterationKind (
      keyAlterationKind);

  // forget about this item
  fCurrentHumdrumScotKeyItem = nullptr;
}

void mxsr2msrTranslator::visitStart (S_key_octave& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_key_octave" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int keyOctaveNumber = (int)(*elt);

  msrOctaveKind
    keyOctaveKind =
      msrOctaveKindFromNumber (
        inputLineNumber,
        keyOctaveNumber);

  int number = elt->getAttributeIntValue ("number", 0);

/* JMI
If the cancel attribute is
  set to yes, then this number refers to an element specified
  by the cancel element. It is no by default.
*/

  // fetch Humdrum/Scot item with 'number' in the std::vector
  S_msrHumdrumScotKeyItem item;

  try {
    // indices start at 0
    item = fCurrentHumdrumScotKeyItemsVector [number - 1];
  }
  catch (int e) {
    std::stringstream s;

    s <<
      "item " << number <<
      " not found in Humdrum/Scot key items" <<
      std::endl <<
      "An std::exception number " << e << " occurred";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (! item) { // JMI
    gLogStream << "----------------" << std::endl;

    int counter = 0;
    for (
      std::vector<S_msrHumdrumScotKeyItem>::const_iterator i=
        fCurrentHumdrumScotKeyItemsVector.begin ();
      i!=fCurrentHumdrumScotKeyItemsVector.end ();
      ++i
  ) {
      gLogStream <<
        ++counter << ": " << (*i) <<
        std::endl;
    } // for

    gLogStream << "----------------" << std::endl;

    std::stringstream s;

    s <<
      "item " << number <<
      " not found in Humdrum/Scot key items";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // complement the item with the octave
  item->
    setKeyItemOctaveKind (
      keyOctaveKind);
}

void mxsr2msrTranslator::visitEnd (S_key& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_key" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // handle key
  S_msrKey key;

  switch (fCurrentKeyKind) {
    case msrKeyKind::kKeyTraditional:
      key =
        handleTraditionalKey (inputLineNumber);
      break;

    case msrKeyKind::kKeyHumdrumScot:
      key =
        handleHumdrumScotKey (inputLineNumber);
      break;
  } // switch

  // register key in part or staff
  if (fCurrentKeyStaffNumber == 0)
    fCurrentPart->
      appendKeyToPart (key);

  else {
    S_msrStaff
      staff =
        fetchStaffFromCurrentPart (
          inputLineNumber,
          fCurrentKeyStaffNumber);

    staff->
      appendKeyToStaff (key);
  }
}

S_msrKey mxsr2msrTranslator::handleTraditionalKey (
  int inputLineNumber)
{
  // key fifths number
  msrQuarterTonesPitchKind
    keyTonicPitchKind =
      msrQuarterTonesPitchKind::kQTP_NO_;

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
      std::stringstream s;

      s <<
        "unknown key fifths number \"" << fCurrentKeyFifths << "\"";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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

S_msrKey mxsr2msrTranslator::handleHumdrumScotKey (
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
      std::vector<S_msrHumdrumScotKeyItem>::const_iterator i=
        fCurrentHumdrumScotKeyItemsVector.begin ();
      i!=fCurrentHumdrumScotKeyItemsVector.end ();
      ++i
  ) {
      key->
        appendHumdrumScotKeyItem ((*i));
    } // for

    fCurrentHumdrumScotKeyItemsVector.clear ();
  }

  else
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "Humdrum/Scot key is empty");

  // return it
  return key;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_time& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_time" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "time symbol " << timeSignatureSymbol << " is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fCurrentTimeSignatureBeats = "";

  fOnGoingInterchangeable = false;
}

void mxsr2msrTranslator::visitStart (S_beats& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_beats" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentTimeSignatureBeats = elt->getValue (); // can be a std::string such as 3+2
}

void mxsr2msrTranslator::visitStart (S_beat_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_beat_type" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int beatType = (int)(*elt);

  // extract the numbers std::list from the beat type
  std::list<int>
    beatNumbers =
      mfExtractNumbersFromString (
        fCurrentTimeSignatureBeats, // may contain "1+5+3"
        false); // 'true' to debug it

  // create the time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  // populate it
  if (beatNumbers.size ()) {
    // append the beats numbers to the time signature item
    for (
      std::list<int>::const_iterator i = beatNumbers.begin ();
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
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "beat type doesn't contain any beats numbers");
  }

  // append the time signature item to the current time signature items std::vector
  fCurrentTimeSignatureItemsVector.insert (
    fCurrentTimeSignatureItemsVector.end (),
    timeSignatureItem);
}

void mxsr2msrTranslator::visitStart (S_senza_misura& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_senza_misura" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura;
}

void mxsr2msrTranslator::visitStart (S_interchangeable& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_interchangeable" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "interchangeable symbol " << interchangeableSymbol << " is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  std::string interchangeableSeparator =
    elt->getAttributeValue ("separator");

  fCurrentInterchangeableSeparatorKind =
    msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorNone; // default value

  if       (interchangeableSymbol == "[NONE]") {
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
      std::stringstream s;

      s <<
        "interchangeable symbol " << interchangeableSymbol << " is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fOnGoingInterchangeable = true;
}

void mxsr2msrTranslator::visitStart (S_time_relation& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_time_relation" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "time-relation " << timeSignatureRelation << " is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
}

void mxsr2msrTranslator::visitEnd (S_time& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_time" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the time
  fCurrentTimeSignature =
    msrTimeSignature::create (
      inputLineNumber,
      fCurrentTimeSignatureSymbolKind);

  // populate the time with the time signature items
  if (fCurrentTimeSignatureItemsVector.size ()) {
    for (
      std::vector<S_msrTimeSignatureItem>::const_iterator i =
        fCurrentTimeSignatureItemsVector.begin ();
      i!=fCurrentTimeSignatureItemsVector.end ();
      ++i
  ) {
      fCurrentTimeSignature->
        appendTimeSignatureItem ((*i));
    } // for

    fCurrentTimeSignatureItemsVector.clear ();
  }

  else {
    // only a 'semza misura' time may be empty
    if (  fCurrentTimeSignatureSymbolKind != msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        "time is empty");
    }
  }

  // register time in part or staff
  if (fCurrentTimeStaffNumber == 0)
    fCurrentPart->
      appendTimeSignatureToPart (fCurrentTimeSignature);

  else {
    S_msrStaff
      staff =
        fetchStaffFromCurrentPart (
          inputLineNumber,
          fCurrentTimeStaffNumber);

    staff->
      appendTimeSignatureToStaff (fCurrentTimeSignature);
  }
}

//______________________________________________________________________________

/*

open xmlsamples3.1/Telemann.xml

<score-instrument id="P1-I1">
    <instrument-name>Voice 1</instrument-name>
   </score-instrument>
*/

void mxsr2msrTranslator::visitStart (S_score_instrument& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_score_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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


void mxsr2msrTranslator::visitStart (S_instrument_name& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_instrument_name" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/*
        <instruments>2</instruments>
*/

//  int instruments = (int)(*elt); // JMI
}

void mxsr2msrTranslator::visitStart (S_solo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_solo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/*
        <instruments>2</instruments>
*/

//  int instruments = (int)(*elt); // JMI
}

void mxsr2msrTranslator::visitStart (S_instruments& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_instruments" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
void mxsr2msrTranslator::visitStart (S_transpose& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_transpose" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentTransposeNumber = elt->getAttributeIntValue ("number", 0);

  fCurrentTransposeDiatonic     = 0;
  fCurrentTransposeChromatic    = 0;
  fCurrentTransposeOctaveChange = 0;
  fCurrentTransposeDouble       = false;
}

void mxsr2msrTranslator::visitStart (S_diatonic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_diatonic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentTransposeDiatonic = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_chromatic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_chromatic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentTransposeChromatic = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_octave_change& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting octave_change" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentTransposeOctaveChange = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_double& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting double" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  /*
    If the double element is present,
    it indicates that the music is doubled one octave down
    from what is currently written
    (as is the case for mixed cello / bass parts in orchestral literature).
  */

  fCurrentTransposeDouble = true;
}

void mxsr2msrTranslator::visitEnd (S_transpose& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_transpose" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

    std::stringstream s;

    s <<
      "fTransposition: augmenting chromatic " <<
      fCurrentTransposeChromatic <<
      " to " << auxTransposeChromatic <<
      " and decrementing octave change by " << octaveOffset;

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());

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

    std::stringstream s;

    s <<
      "fTransposition: diminishing  chromatic to " <<
      fCurrentTransposeChromatic <<
      " to " << auxTransposeChromatic <<
      " and incrementing octave change by " << octaveOffset;

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());

    fCurrentTransposeChromatic    =  auxTransposeChromatic;
    fCurrentTransposeOctaveChange += octaveOffset;
  }

  // create the msrTransposition
  S_msrTransposition
    transposition =
      msrTransposition::create (
        inputLineNumber,
        fCurrentTransposeDiatonic,
        fCurrentTransposeChromatic,
        fCurrentTransposeOctaveChange,
        fCurrentTransposeDouble);

  if (fCurrentTransposeNumber == 0)
    fCurrentPart->
      appendTranspositionToPart (transposition);

  else {
    S_msrStaff
      staff =
        fetchStaffFromCurrentPart (
          inputLineNumber,
          fCurrentTransposeNumber);

    staff->
      appendTranspositionToStaff (transposition);
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_direction& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_direction" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  fCurrentDirectionPlacementKind =
    msrPlacementKindFromString (
      inputLineNumber,
      placementString);

  fCurrentDirectionStaffNumber = 1; // it may be absent
  // no <voice /> possible in <direction /> ??? JMI
  // fCurrentDirectionVoiceNumber = 1; // it may be absent

  fCurrentMetronomeTempo = nullptr;

  fOnGoingDirection = true;
}

void mxsr2msrTranslator::visitEnd (S_direction& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_direction" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fCurrentMetronomeTempo) {
    size_t pendingWordsSize = fPendingWordsList.size ();

    if (pendingWordsSize) {
      while (fPendingWordsList.size ()) {
        S_msrWords
          words =
            fPendingWordsList.front();

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceWords ()) {
          gLogStream <<
            "Attaching words '" <<
            words->asString () <<
            "' to metronome tempo '" <<
            fCurrentMetronomeTempo->asString () << "'" <<
            std::endl;
        }
#endif

        fCurrentMetronomeTempo->
          appendWordsToTempo (words);

        // forget about this words
        fPendingWordsList.pop_front();
      } // while

      // append the tempo to the pending tempos std::list
      fPendingTemposList.push_back (fCurrentMetronomeTempo);

      fCurrentMetronomeTempo = nullptr;
    }
  }

  fOnGoingDirection = false;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_direction_type& elt)
{
/*
<!ELEMENT direction-type (rehearsalMark+ | segno+ | coda+ |
  (words | symbol)+ | wedge | dynamics+ | dashes |
  bracket | pedal | metronome | octave-shift | harp-pedals |
  damp | damp-all | eyeglasses | std::string-mute |
  scordatura | image | principal-voice | percussion+ |
  accordion-registration | staff-divide | other-direction)>
<!ATTLIST direction-type
    %optional-unique-id;
>
*/

  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_direction_type" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (! fOnGoingDirection) {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<direction-type /> is out of context");
  }

  fOnGoingDirectionType = true;
}

void mxsr2msrTranslator::visitEnd (S_direction_type& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_direction_type" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fOnGoingDirectionType = false;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_offset& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_offset" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int offsetValue = (int)(*elt);

  // set offset whole notes
  Rational
    offsetWholeNotesFromDuration =
      Rational (
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
      std::stringstream s;

      s <<
        "offset sound value " << offsetSound <<
        " should be 'yes' or 'no'";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  if (false && offsetSoundValue) {
    // JMI
  }

  if (fOnGoingDirection) { // JMI
  }

  else if (fOnGoingHarmony) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Harmony offset \"" << offsetValue << "\"" <<
        ", represents = \'" <<
       offsetWholeNotesFromDuration <<
       "\' whole notes" <<
        std::endl;
    }
#endif

    fCurrentHarmonyWholeNotesOffset =
      offsetWholeNotesFromDuration;
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_other_direction& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_direction" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_sound& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_sound" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingDirection) {
    // tempo
    std::string tempoString = elt->getAttributeValue ("tempo"); // to be seen JMI

    if (tempoString.size ()) {
      fCurrentMetronomeTempo =
        msrTempo::createTempoPerMinute (
          inputLineNumber,
          msrDottedDuration (
            msrDurationKind::kDurationQuarter,
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
      // JMI v0.9.66
    }
  }
}

void mxsr2msrTranslator::visitEnd (S_sound& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_sound" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_octave_shift& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_octave_shift" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // size

  std::string octaveShiftSizeString = elt->getAttributeValue ("size");
  int    octaveShiftSize = 8;

  if (! octaveShiftSizeString.size ()) {
    std::stringstream s;

    s <<
      "octave shift size absent, assuming 8";

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  else {
    std::istringstream inputStream (octaveShiftSizeString);

    inputStream >> octaveShiftSize;

    if (
      octaveShiftSize != 8
        &&
      octaveShiftSize != 15
      /* JMI allow larger values???
        &&
      octaveShiftSize != 22
        &&
      octaveShiftSize != 27
      */
      ) {
      std::stringstream s;

      s <<
        "octave-shift size \"" << octaveShiftSize <<
    // JMI    "\" should be 8, 15, 22 or 27";
        "\" is wrong, should be 8 or 15, replaced by 8";

  // JMI    mxsr2msrError (
      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
   //     __FILE__, __LINE__,
        s.str ());
    }

    octaveShiftSize = 8;
  }

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
    std::stringstream s;

    s <<
      "octave-shift type \"" << type <<
      "\"" << "is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create an octave shift
  S_msrOctaveShift
    octaveShift =
      msrOctaveShift::create (
        inputLineNumber,
        octaveShiftKind,
        octaveShiftSize);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceOctaveShifts ()) {
      gLogStream <<
        "Creating octave shift " <<
        octaveShift->asString () <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

  // append the octave shift to the pending octave shifts std::list
  fPendingOctaveShiftsList.push_back (octaveShift);
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_words& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_words" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string wordsValue = elt->getValue ();

  if (! wordsValue.size ()) {
    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      "words contents is empty");
  }

  // justify
  std::string wordsJustifyString = elt->getAttributeValue ("justify");

  msrJustifyKind
    justifyKind =
      msrJustifyKindFromString (
          inputLineNumber,
        wordsJustifyString);

  // halign
  std::string wordsHorizontalAlignmentString = elt->getAttributeValue ("halign");

  msrHorizontalAlignmentKind
    horizontalAlignmentKind =
      msrHorizontalAlignmentKindFromString (
        inputLineNumber,
        wordsHorizontalAlignmentString);

  // valign
  std::string wordsVerticalAlignmentString = elt->getAttributeValue ("valign");

  msrVerticalAlignmentKind
    verticalAlignmentKind =
      msrVerticalAlignmentKindFromString (
        inputLineNumber,
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
      std::stringstream s;

      s <<
        "font-style value " << wordsFontStyleString <<
        " should be 'normal' or 'italic'";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
      std::stringstream s;

      s <<
        "justify value " << wordsFontJustifyString <<
        " should be 'normal' or 'italic'";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
      std::stringstream s;

      s <<
        "font-weight value " << wordsFontWeightString <<
        " should be 'normal' or 'bold'";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // XMLLang
  std::string wordsXMLLangString = elt->getAttributeValue ("xml:lang");

  msrXMLLangKind
    wordsXMLLangKind =
      msrXMLLangKindFromString (
        inputLineNumber,
        wordsXMLLangString);

  // should the MusicXML words be ignored?
  if (! gGlobalMxsr2msrOahGroup->getIgnoreMusicXMLWords ()) {
    // no

    Bool wordsHasBeenHandled (false);

    // is wordsValue to be converted to an MSR tempo?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToTempo (wordsValue)) {
//       convertWordsToTempo (
//         inputLineNumber,
//         wordsValue);

      // create an msrWords containing wordsValue
      S_msrWords
        words =
          msrWords::create (
            inputLineNumber,
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
            inputLineNumber,
            words,
            msrTempoParenthesizedKind::kTempoParenthesizedNo,    // JMI
            msrPlacementKind::kPlacementAbove); // JMI

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceTempos ()
      ) {
        gLogStream <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR tempo" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fPendingTemposList.push_back (tempo);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a rehearsal mark?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToRehearsalMark (wordsValue)) {
//       convertWordsToRehearsalMark (
//         inputLineNumber,
//         wordsValue);

      // create an msrWords containing wordsValue
      S_msrRehearsalMark
        rehearsalMark =
          msrRehearsalMark::create (
            inputLineNumber,
            msrRehearsalMarkKind::kRehearsalMarkNone, // JMI allow for other values???
            wordsValue,
            fCurrentDirectionPlacementKind);

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceRehearsalMarks ()
      ) {
        gLogStream <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR rehearsal mark" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      // append the rehearsalMark to the pending tempos std::list
      fPendingRehearsalMarksList.push_back (rehearsalMark);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a segno?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToSegno (wordsValue)) {
//       convertWordsToSegno (
//         inputLineNumber,
//         wordsValue);
      // create the segno
      S_msrSegno
        segno =
          msrSegno::create (
            inputLineNumber,
            fCurrentDirectionStaffNumber);

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceSegnos ()
      ) {
        gLogStream <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR segno" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      // append dal segno to the pending tempos std::list
      fPendingSegnosList.push_back (segno);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a dal segno?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToDalSegno (wordsValue)) {
//       convertWordsToDalSegno (
//         inputLineNumber,
//         wordsValue);
      // create a dal segno element containing wordsValue
      S_msrDalSegno
        dalSegno =
          msrDalSegno::create (
            inputLineNumber,
            msrDalSegnoKind::kDalSegno,
            wordsValue,
            fCurrentDirectionStaffNumber);

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceDalSegnos ()
      ) {
        gLogStream <<
          "Converting words '" <<
          wordsValue <<
          "' to an MSR dal segno '" <<
          dalSegno->asString () <<
          "'" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fPendingDalSegnosList.push_back (dalSegno);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a dal segno al fine?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToDalSegnoAlFine (wordsValue)) {
//       convertWordsToDalSegnoAlFine (
//         inputLineNumber,
//         wordsValue);
      // create a dal segno element containing wordsValue
      S_msrDalSegno
        dalSegno =
          msrDalSegno::create (
            inputLineNumber,
            msrDalSegnoKind::kDalSegno,
            wordsValue,
            fCurrentDirectionStaffNumber);

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceDalSegnos ()
      ) {
        gLogStream <<
          "Converting words '" <<
          wordsValue <<
          "' to an MSR dal segno '" <<
          dalSegno->asString () <<
          "'" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fPendingDalSegnosList.push_back (dalSegno);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a dal segno al coda?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToDalSegnoAlCoda (wordsValue)) {
//       convertWordsToDalSegnoAlCoda (
//         inputLineNumber,
//         wordsValue);
      // create a dal segno element containing wordsValue
      S_msrDalSegno
        dalSegno =
          msrDalSegno::create (
            inputLineNumber,
            msrDalSegnoKind::kDalSegno,
            wordsValue,
            fCurrentDirectionStaffNumber);

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceDalSegnos ()
      ) {
        gLogStream <<
          "Converting words '" <<
          wordsValue <<
          "' to an MSR dal segno '" <<
          dalSegno->asString () <<
          "'" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fPendingDalSegnosList.push_back (dalSegno);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a coda first?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToCodaFirst (wordsValue)) {
//       convertWordsToCoda (
//         inputLineNumber,
//         wordsValue);
      // create the coda
      S_msrCoda
        coda =
          msrCoda::create (
            inputLineNumber,
            fCurrentDirectionStaffNumber,
            msrCodaKind::kCodaFirst);

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceCodas ()
      ) {
        gLogStream <<
          "Converting words '" <<
          wordsValue <<
          "' to an MSR coda first '" <<
          coda->asString () <<
          "'" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      // append it to the pending codas std::list
      fPendingCodasList.push_back (coda);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a coda second?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToCodaSecond (wordsValue)) {
//       convertWordsToCoda (
//         inputLineNumber,
//         wordsValue);
      // create the coda
      S_msrCoda
        coda =
          msrCoda::create (
            inputLineNumber,
						fCurrentDirectionStaffNumber,
            msrCodaKind::kCodaSecond);

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceCodas ()
      ) {
        gLogStream <<
          "Converting words '" <<
          wordsValue <<
          "' to an MSR coda second '" <<
          coda->asString () <<
          "'" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      // append it to the pending codas std::list
      fPendingCodasList.push_back (coda);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a cresc?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToCresc (wordsValue)) {
#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceDynamics ()
          ||
        gGlobalTracingOahGroup->getTraceCrescDecrescs ()
      ) {
        gLogStream <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR cresc" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

//       convertWordsToCresc (
//         inputLineNumber,
//         wordsValue);
      // create an msrCrescDecresc
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceCrescDecrescs ()) {
        gLogStream <<
          "Creating an msrCrescDecresc crescendo" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      S_msrCrescDecresc
        crescDecresc =
          msrCrescDecresc::create (
            inputLineNumber,
            msrCrescDecrescKind::kCrescDecrescCrescendo);

      // append the rehearsalMark to the pending tempos std::list
      fPendinCrescDecrescsList.push_back (crescDecresc);

      wordsHasBeenHandled = true;
    }

    // is wordsValue to be converted to a decresc?
    if (gGlobalMxsr2msrOahGroup->wordsIsToBeConvertedToDecresc (wordsValue)) {
#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceWords ()
          ||
        gGlobalTracingOahGroup->getTraceDynamics ()
          ||
        gGlobalTracingOahGroup->getTraceCrescDecrescs ()
      ) {
        gLogStream <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR decresc" <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

//       convertWordsToDecresc (
//         inputLineNumber,
//         wordsValue);
      // create an msrCrescDecresc
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceCrescDecrescs ()) {
        gLogStream <<
          "Creating an msrCrescDecresc decrescendo" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      S_msrCrescDecresc
        crescDecresc =
          msrCrescDecresc::create (
            inputLineNumber,
            msrCrescDecrescKind::kCrescDecrescDecrescendo);

      // append the rehearsalMark to the pending tempos std::list
      fPendinCrescDecrescsList.push_back (crescDecresc);

      wordsHasBeenHandled = true;
    }

    // has wordsValue ben handled?
    if (! wordsHasBeenHandled) {
      // create the words
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceWords ()) {
        gLogStream <<
          "Creating words \"" << wordsValue << "\"" <<
          ", placement = \"" <<
          msrPlacementKindAsString (
            fCurrentDirectionPlacementKind) <<
          "\"" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

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
            inputLineNumber,
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

void mxsr2msrTranslator::visitStart (S_accordion_registration& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_accordion_registration" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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

void mxsr2msrTranslator::visitStart (S_accordion_high& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_accordion_high" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentAccordionHigh = 1;

  ++fCurrentAccordionNumbersCounter;
}

void mxsr2msrTranslator::visitStart (S_accordion_middle& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_accordion_middle" <<
       ", line " << inputLineNumber <<
     std::endl;
  }
#endif

  fCurrentAccordionMiddle = (int)(*elt);

  if (fCurrentAccordionMiddle < 1 || fCurrentAccordionMiddle > 3) {
    std::stringstream s;

    s <<
      "accordion middle " <<
      fCurrentAccordionMiddle << " should be 1, 2 or 3" <<
      ", replaced by 1";

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());

    fCurrentAccordionMiddle = 1;
  }

  ++fCurrentAccordionNumbersCounter;
}

void mxsr2msrTranslator::visitStart (S_accordion_low& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_accordion_low" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentAccordionLow = 1;

  ++fCurrentAccordionNumbersCounter;
}

void mxsr2msrTranslator::visitEnd (S_accordion_registration& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_accordion_registration" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // An accordion-registration element needs to have
  // at least one of the child elements present

  if (fCurrentAccordionNumbersCounter == 0) {
    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      "accordion-registration has 0 child element, ignoring it");
  }

  else {
    // create the accordion registration
    S_msrAccordionRegistration
      accordionRegistration =
        msrAccordionRegistration::create (
          inputLineNumber,
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
void mxsr2msrTranslator::visitStart (S_metronome& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_metronome" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "parentheses value " << parentheses <<
        " should be 'yes' or 'no'";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);


  fCurrentMetronomeBeatUnitsVector.clear();

  fCurrentMetrenomePerMinute = "";

  fCurrentMetrenomeDotsNumber = 0;
  fCurrentMetrenomeRelationKind = msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone;
  fCurrentMetronomeDurationKind = msrDurationKind::kDuration_NO_;
  fCurrentMetronomeBeamValue = "";

  fOnGoingMetronomeTuplet = false;
  fCurrentMetrenomeNormalDotsNumber = 0;

  fCurrentMetronomeRelationLeftElements  = nullptr;
  fCurrentMetronomeRelationRightElements = nullptr;

  fCurrentMetronomeTuplet = nullptr;

  // don't create fCurrentMetronomeTempo now,
  // this will be done in visitEnd (S_metronome& elt)
  // when the details are known
}

void mxsr2msrTranslator::visitStart (S_beat_unit& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_beat_unit" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string beatUnitString = elt->getValue();

  // the type contains a display duration
  msrDurationKind
    beatUnitDurationKind =
      msrDurationKindFromMusicXMLString (
        inputLineNumber,
        beatUnitString);

  // there can be several <beat-unit/> in a <metronome/> markup,
  if (fCurrentMetronomeBeatUnitsVector.size () < 2) {
    // register beat unit in in dotted durations std::list
    fCurrentMetronomeBeatUnitsVector.push_back (
      msrDottedDuration (
        beatUnitDurationKind,
        0)); // dots number
  }
  else {
    std::stringstream s;

    s <<
      "there are more than 2 <beat-unit/> elements in a <metronome/> markup";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_beat_unit_dot& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_beat_unit_dot" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fCurrentMetronomeBeatUnitsVector.size ()) {
    fCurrentMetronomeBeatUnitsVector.back ().incrDotsNumber ();
  }
  else {
    std::stringstream s;

    s <<
      "beat unit dot occurs without any prior beat unit";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_per_minute& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_per_minute" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentMetrenomePerMinute = elt->getValue ();
}

void mxsr2msrTranslator::visitStart (S_metronome_note& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_metronome_note" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fOnGoingMetronomeNote = true;
}

void mxsr2msrTranslator::visitStart (S_metronome_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_metronome_type" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string noteType = elt->getValue();

  // the type contains a display duration,
  fCurrentMetronomeDurationKind =
    msrDurationKindFromMusicXMLString (
     inputLineNumber,
     noteType);
}

void mxsr2msrTranslator::visitStart (S_metronome_dot& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_metronome_dot" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++fCurrentMetrenomeDotsNumber;
}

void mxsr2msrTranslator::visitStart (S_metronome_beam& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_metronome_beam" <<
      ", line " << inputLineNumber <<
     std::endl;
  }
#endif

  // value

  fCurrentMetronomeBeamValue = elt->getValue();

  msrBeamKind beamKind = msrBeamKind::kBeam_NO_;

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
    std::stringstream s;

    s <<
      "metronome beam \"" << fCurrentMetronomeBeamValue <<
      "\"" << "is not known";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // number

  fCurrentBeamNumber =
    elt->getAttributeIntValue ("number", 0);

  // create metronome note beam
  S_msrBeam
    beam =
      msrBeam::create (
        inputLineNumber,
        fCurrentBeamNumber,
        beamKind);

  // register it
  fPendingMetronomeBeamsList.push_back (beam);
}

void mxsr2msrTranslator::attachCurrentMetronomeBeamsToMetronomeNote (
  S_msrTempoNote tempoNote)
{
  // attach the current articulations if any to the note
  if (fPendingMetronomeBeamsList.size ()) {

#ifdef TRACING_IS_ENABLED
    if (
      gGlobalTracingOahGroup->getTraceTempos ()
        ||
      gGlobalTracingOahGroup->getTraceBeams ()
    ) {
      gLogStream <<
        "Attaching current beams to tempoNote " <<
        tempoNote->asString () <<
        std::endl;
    }
#endif

    while (fPendingMetronomeBeamsList.size ()) {
      S_msrBeam
        beam =
          fPendingMetronomeBeamsList.front();

#ifdef TRACING_IS_ENABLED
      if (
        gGlobalTracingOahGroup->getTraceTempos ()
          ||
        gGlobalTracingOahGroup->getTraceBeams ()
      ) {
        gLogStream <<
          "Attaching beam '" <<
          beam->asString () <<
          "' to tempoNote '" << tempoNote->asString () << "'" <<
          std::endl;
      }
#endif

      tempoNote->
        appendBeamToTempoNote (beam);

      // forget about this articulation
      fPendingMetronomeBeamsList.pop_front();
    } // while
  }
}

void mxsr2msrTranslator::visitEnd (S_metronome_note& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_metronome_note" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // convert metronome note duration into whole notes
  fCurrentMetronomeNoteWholeNotesFromMetronomeType =
    msrDurationKindAsWholeNotes (
      fCurrentMetronomeDurationKind);

  // take metronome dots into account if any
  if (fCurrentNoteDotsNumber > 0) {
    int dots = fCurrentMetrenomeDotsNumber;

    while (dots > 0) {
      fCurrentMetronomeNoteWholeNotesFromMetronomeType *=
        Rational (3, 2);

      --dots;
    } // while
  }

  // create the metronome note
  S_msrTempoNote
    tempoNote =
      msrTempoNote::create (
        inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
            msrTempoNotesRelationshipElementsKind::kTempoNotesRelationshipElementsRight);
      }

      fCurrentMetronomeRelationRightElements->
        addElementToTempoNotesRelationshipElements (
          tempoNote);
    }
  }

  fOnGoingMetronomeNote = false;
}

void mxsr2msrTranslator::visitStart (S_metronome_relation& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_metronome_relation" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string metrenomeRelation = elt->getValue ();

  fCurrentMetrenomeRelationKind = msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone;

  if (metrenomeRelation == "equals") {
    fCurrentMetrenomeRelationKind = msrTempoNotesRelationshipKind::kTempoNotesRelationshipEquals;
  }
  else {
    std::stringstream s;

    s <<
      "metronome relation \"" << metrenomeRelation <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_metronome_tuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_metronome_tuplet" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
        std::stringstream s;

        s <<
          "tuplet bracket \"" << tupletBracket <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTuplets ()) {
          std::stringstream s;

          s <<
            "tuplet bracket is empty: this is implementation dependent," <<
            " \"yes\" is assumed"; // option ??? JMI

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            s.str ());
        }
#endif
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
      std::stringstream s;

      s <<
        "tuplet type \"" << tupletType <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
    else if (tupletShowNumber == "[NONE]") {
      fCurrentTempoTupletShowNumberKind = msrTempoTupletShowNumberKind::kTempoTupletShowNumberNone;
    }
    else {
      if (tupletShowNumber.size ()) {
        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          "tuplet show-number \"" + tupletShowNumber + "\" is unknown");
      }
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "fCurrentTempoTupletTypeKind: " <<
			fCurrentTempoTupletTypeKind <<
      "fCurrentTempoTupletBracketKind: " <<
			fCurrentTempoTupletBracketKind <<
      "fCurrentTempoTupletShowNumberKind: " <<
			fCurrentTempoTupletShowNumberKind <<
      std::endl;
  }
#endif

  fOnGoingMetronomeTuplet = true;
}

void mxsr2msrTranslator::visitStart (S_normal_dot& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_normal_dot" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++fCurrentMetrenomeNormalDotsNumber;
}

void mxsr2msrTranslator::visitEnd (S_metronome_tuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_metronome_tuplet" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  switch (fCurrentTempoTupletTypeKind) {
    case msrTempoTupletTypeKind::kTempoTupletTypeNone:
      break;

    case msrTempoTupletTypeKind::kTempoTupletTypeStart:
    	{
				// create metronome tuplet
				fCurrentMetronomeTuplet =
					msrTempoTuplet::create (
						inputLineNumber,
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
								inputLineNumber,
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
								inputLineNumber,
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

void mxsr2msrTranslator::visitEnd (S_metronome& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_metronome" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // fCurrentMetronomeParenthesedKind ??? JMI

  // determine tempo kind
  msrTempoKBeatUnitsKind tempoKind = msrTempoKBeatUnitsKind::kTempoBeatUnits_NO_;

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

  if (tempoKind == msrTempoKBeatUnitsKind::kTempoBeatUnits_NO_) {
      std::stringstream s;

      s <<
        "ill-formed <metronome/> markup: there are " <<
        fCurrentMetronomeBeatUnitsVector.size () <<
        " beat units";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the tempo
  switch (tempoKind) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_NO_:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      {
        msrDottedDuration
          beatUnits =
            fCurrentMetronomeBeatUnitsVector [0];

        fCurrentMetronomeTempo =
          msrTempo::createTempoPerMinute (
            inputLineNumber,
            beatUnits,
            fCurrentMetrenomePerMinute,
            fCurrentMetronomeParenthesedKind,
            fCurrentDirectionPlacementKind);
        }
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      {
        msrDottedDuration
          beatUnits =
            fCurrentMetronomeBeatUnitsVector [0];

        fCurrentMetronomeTempo =
          msrTempo::createTempoBeatUnitEquivalent (
            inputLineNumber,
            beatUnits,
            fCurrentMetronomeBeatUnitsVector [1],
            fCurrentMetronomeParenthesedKind,
            fCurrentDirectionPlacementKind);
      }
      break;

    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      fCurrentMetronomeTempo =
        msrTempo::createTempoNotesRelationship (
          inputLineNumber,
          fCurrentMetronomeRelationLeftElements,
          fCurrentMetrenomeRelationKind, // msrTempoNotesRelationshipKind::kTempoNotesRelationshipEquals here
          fCurrentMetronomeRelationRightElements,
          fCurrentMetronomeParenthesedKind,
          fCurrentDirectionPlacementKind);
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Creating tempo '" <<
      fCurrentMetronomeTempo->asString () <<
      "'" <<
      std::endl;
  }
#endif

  // append metrenome words to tempo if any
  S_msrWords tempoWords;

  size_t pendingWordsSize = fPendingWordsList.size ();

  if (pendingWordsSize) {
    if (pendingWordsSize > 1) {
      while (fPendingWordsList.size ()) {
        S_msrWords
          words =
            fPendingWordsList.front();

#ifdef TRACING_IS_ENABLED
        if (
          gGlobalTracingOahGroup->getTraceTempos ()
            ||
          gGlobalTracingOahGroup->getTraceWords ()
        ) {
          gLogStream <<
            "Attaching words '" <<
            words->asString () <<
            "' to tempo '" <<
            fCurrentMetronomeTempo->asString () << "'" <<
            std::endl;
        }
#endif

        fCurrentMetronomeTempo->
          appendWordsToTempo (words);

        // forget about this words
        fPendingWordsList.pop_front();
      } // while


      std::stringstream s;

      s <<
        "<direction/> contains " <<
        pendingWordsSize <<
        " <words/> markups";

      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }

    while (fPendingWordsList.size ()) {
      S_msrWords
        words =
          fPendingWordsList.front ();

      // append the words to the temp
      fCurrentMetronomeTempo->
        appendWordsToTempo (words);

       // remove it from the std::list
      fPendingWordsList.pop_front ();
    } // while
  }

/* if (true) { // JMI
*/
  // append the tempo to the pending tempos std::list
  fPendingTemposList.push_back (fCurrentMetronomeTempo);
  /*
}
else {
  // fetch current direction's voice
  S_msrVoice
    currentDirectionsVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentDirectionStaffNumber,
        fCurrentDirectionVoiceNumber);

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    currentDirectionsVoice != nullptr,
    "currentDirectionsVoice is null");

  // append the tempo to the voice
  currentDirectionsVoice->
    appendTempoToVoice (fCurrentMetronomeTempo);
  }
  */
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_staves& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_direction" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_staff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentMusicXMLStaffNumber = int(*elt);

  // the staff number should be positive
  if (fCurrentMusicXMLStaffNumber <= 0) {
    std::stringstream s;

    s <<
      "staff number " << fCurrentMusicXMLStaffNumber <<
      " is not positive" <<
      ", line " << inputLineNumber;

      mfAssert (
        __FILE__, __LINE__,
      false, s.str ());
  }

  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "--> S_staff, fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      "--> S_staff, current staff name: " <<
      staff->getStaffName() <<
      std::endl;
  }
#endif

  if (fOnGoingForward) {
    fCurrentForwardStaffNumber = fCurrentMusicXMLStaffNumber;
  }

  else if (fOnGoingNote) {
    // regular staff indication in note/rest, fine
  }

  else if (fOnGoingDirection) {
    fCurrentDirectionStaffNumber = fCurrentMusicXMLStaffNumber;
  }

  else if (fOnGoingHarmony) {
    fCurrentHarmoniesStaffNumber = fCurrentMusicXMLStaffNumber;
  }

  else {
    std::stringstream s;

    s <<
      "<staff /> " <<
      fCurrentMusicXMLStaffNumber <<
      " is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_staff_details& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff_details" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
        std::stringstream s;

        s << "show-frets " << showFrets << " unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
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
      std::stringstream s;

      s << "print-spacing " << printSpacing << " is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Handling staff details:" <<
      std::endl <<
      gTab << "StaffDetailsStaffNumber" << " = " <<
      fStaffDetailsStaffNumber <<
      std::endl;
  }
#endif

  fCurrentStaffTypeKind =
    msrStaffTypeKind::kStaffTypeRegular;

  fCurrentStaffTuningAlterationKind = msrAlterationKind::kAlteration_NO_;
  fCurrentStaffTuningOctaveKind     = msrOctaveKind::kOctave_NO_;

  fCurrentStaffDetailsStaffSize = 0;

  fCurrentStaffLinesNumber = 5; // default value

  fCurrentStaffDetailsCapo = 0;

  // create the staff details
  fCurrentStaffDetails =
    msrStaffDetails::create (
      inputLineNumber,
      fCurrentStaffTypeKind,
      fCurrentShowFretsKind,
      fCurrentPrintObjectKind,
      fCurrentPrintSpacingKind);

  ++gIndenter;
}

void mxsr2msrTranslator::visitStart (S_staff_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff_type" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
    std::stringstream s;

    s << "staff-type" << staffType << "unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_staff_lines& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff_lines" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentStaffLinesNumber = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_staff_tuning& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff_tuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentStaffTuningLine =
    elt->getAttributeIntValue ("line", 0);

  fCurrentStaffTuningAlterationKind = msrAlterationKind::kAlterationNatural; // may be absent
  fCurrentStaffTuningOctaveKind     = msrOctaveKind::kOctave_NO_;

  fOnGoingStaffTuning = true;
}

void mxsr2msrTranslator::visitStart (S_tuning_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tuning_step" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string tuningStep = elt->getValue();

  checkStep (
    inputLineNumber,
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
    std::stringstream s;

    s <<
      "<tuning-step /> " <<
      tuningStep <<
      " is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_tuning_octave& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tuning_octave" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int tuningOctaveNumber = (int)(*elt);

  msrOctaveKind
    tuningOctaveKind =
      msrOctaveKindFromNumber (
        inputLineNumber,
        tuningOctaveNumber);

  if (fOnGoingStaffTuning) {
    fCurrentStaffTuningOctaveKind = tuningOctaveKind;
  }
  else if (fOnGoingAccord) {
    fCurrentAccordOctaveKind = tuningOctaveKind;
  }
  else {
    std::stringstream s;

    s <<
      "<tuning-octave /> " <<
      tuningOctaveNumber <<
      " is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_tuning_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tuning_alter" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  float tuningAlter = (float)(*elt);

  msrAlterationKind
    tuningAlterationKind =
      msrAlterationKindFromMusicXMLAlter (
        tuningAlter);

  if (tuningAlterationKind == msrAlterationKind::kAlteration_NO_) {
    std::stringstream s;

    s <<
      "tuning alter '" << tuningAlter <<
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (fOnGoingStaffTuning) {
    fCurrentStaffTuningAlterationKind = tuningAlterationKind;
  }
  else if (fOnGoingAccord) {
    fCurrentAccordAlterationKind = tuningAlterationKind;
  }
  else {
    std::stringstream s;

    s <<
      "<tuning-alter /> " <<
      tuningAlter <<
      " is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitEnd (S_staff_tuning& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_staff_tuning" <<
       ", line " << inputLineNumber <<
     std::endl;
  }
#endif

  // fetch relevant staff
  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber,
        fStaffDetailsStaffNumber); // test its value??? JMI

  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromDiatonicPitchAndAlteration (
        inputLineNumber,
        fCurrentStaffTuningDiatonicPitchKind,
        fCurrentStaffTuningAlterationKind);

  // create the staff tuning
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
      "Creating staff tuning:" <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 32;

    gLogStream << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentStaffTuningLine" << " = " <<
      fCurrentStaffTuningLine <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStaffTuningDiatonicPitch" << " = " <<
      msrDiatonicPitchKindAsStringInLanguage (
        gGlobalMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind (),
        fCurrentStaffTuningDiatonicPitchKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStaffTuningAlteration" << " = " <<
      msrAlterationKindAsString (
        fCurrentStaffTuningAlterationKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "quarterTonesPitch" << " = " <<
      msrQuarterTonesPitchKindAsStringInLanguage (
        quarterTonesPitchKind,
        gGlobalMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind ()) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "currentStaffTuningOctaveKind" << " = " <<
      msrOctaveKindAsString (fCurrentStaffTuningOctaveKind) <<
      std::endl;

    --gIndenter;
  }
#endif

  // create the staff tuning
  S_msrStaffTuning
    staffTuning =
      msrStaffTuning::create (
        inputLineNumber,
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
void mxsr2msrTranslator::visitStart (S_voice& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff_details" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentMusicXMLVoiceNumber = int(*elt);

  // the voice number can be out of 1..4 range

  if (fOnGoingForward) {
    fCurrentForwardVoiceNumber = fCurrentMusicXMLVoiceNumber;
  }

  else if (fOnGoingNote) {
    // regular voice indication in note/rest, fine
  }

  else if (fOnGoingDirection) {
    fCurrentDirectionVoiceNumber = fCurrentMusicXMLVoiceNumber;
  }

  else {
    std::stringstream s;

    s <<
      "<voice /> " <<
      fCurrentMusicXMLVoiceNumber <<
      " is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_backup& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_backup" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fOnGoingBackup = true;
}

void mxsr2msrTranslator::visitEnd (S_backup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_backup" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceBackup ()) {
    gLogStream <<
      "Backup by " <<
      mfSingularOrPlural (
        fCurrentBackupDivisions, "division", "divisions") <<
      " becomes pending" <<
      ", fCurrentStaffNumberToInsertInto: " <<
      fCurrentStaffNumberToInsertInto <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // reset notes staff numbers
  fPreviousNoteMusicXMLStaffNumber = msrStaff::K_NO_STAFF_NUMBER;
  fCurrentMusicXMLStaffNumber      = msrStaff::K_NO_STAFF_NUMBER;

  // handle the backup right now:
  handleBackup (
    inputLineNumber);

  // reset staff change detection
  // only now, it is used by handleBackup() v0.9.63
  fCurrentStaffNumberToInsertInto = msrStaff::K_NO_STAFF_NUMBER;

  fOnGoingBackup = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_forward& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_forward" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

//* JMI ???
  // the <staff /> element is present only
  // in case of a staff change
  fCurrentForwardStaffNumber = 1; // JMI default value??? fCurrentMusicXMLStaffNumber;

  // the <voice /> element is present only
  // in case of a voice change
  fCurrentForwardVoiceNumber = 1; // JMI default value??? fCurrentMusicXMLVoiceNumber;
//*/

  fOnGoingForward = true;
}

void mxsr2msrTranslator::visitEnd (S_forward& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_forward" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceForward ()) {
    gLogStream <<
      "Handling 'forward <<< " << fCurrentBackupDivisions <<
      " divisions >>>" <<
      ", fCurrentForwardStaffNumber: " <<
      fCurrentForwardStaffNumber <<
      ", fCurrentForwardVoiceNumber: " <<
      fCurrentForwardVoiceNumber <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  // the staff number should be positive
  if (fCurrentForwardStaffNumber <= 0) {
    std::stringstream s;

    s <<
      "staff number " << fCurrentForwardStaffNumber <<
      " is not positive" <<
      ", line " << inputLineNumber;

      mfAssert (
        __FILE__, __LINE__,
      false, s.str ());
  }

  // fetch the voice to be forwarded
  S_msrVoice
    voiceToBeForwarded =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentForwardStaffNumber,
        fCurrentForwardVoiceNumber);

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceToBeForwarded != nullptr,
    "voiceToBeForwarded is null");

  // compute the forward step length
#ifdef TRACING_IS_ENABLED
    if (gGlobalMxsrOahGroup->getTraceForward ()) {
      gLogStream <<
        "--> current forward divisions: " <<
        fCurrentForwardDivisions <<
        std::endl;
    }
#endif

  Rational
    forwardStepLength =
      Rational (
        fCurrentForwardDivisions,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

  // append a padding note to the voice to be forwarded
  voiceToBeForwarded ->
    appendPaddingNoteToVoice (
      inputLineNumber,
      forwardStepLength);

  // reset staff change detection
  // fCurrentStaffNumberToInsertInto = 1; // default value JMI msrStaff::K_NO_STAFF_NUMBER;
  fCurrentStaffNumberToInsertInto = msrStaff::K_NO_STAFF_NUMBER; // JMI ??? no if forward is followed by backup???

  fOnGoingForward = false;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_tied& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tied" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
        std::stringstream s;

        s << "tied type '" << fCurrentSlurType << "' inside a slur is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }

    // inner tied notes may miss the "continue" type:
    // let' complain on ligature notes outside of ligatures
    if (! fOnGoingLigature) {
      if (tiedType.size ()) {
        std::stringstream s;

        s << "tied type '" << fCurrentSlurType << "' inside a ligature is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }

  }

  // color JMI

  if (fCurrentTieKind != msrTieKind::kTieNone) {
    if (! gGlobalMxsr2msrOahGroup->getIgnoreTies ()) {
      fCurrentTie =
        msrTie::create (
          inputLineNumber,
          fCurrentTieKind);
    }
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::displaySlurStartsStack (
  const std::string& context)
{
  gLogStream <<
    std::endl <<
    ">>++++++++++++++++ " <<
    "The slurs starts std::stack contains:" <<
    std::endl;

  if (fSlurStartsStack.size ()) {
    std::list<S_msrSlur>::const_iterator
      iBegin = fSlurStartsStack.begin (),
      iEnd   = fSlurStartsStack.end (),
      i      = iBegin;

    ++gIndenter;

    for ( ; ; ) {
      gLogStream << "v " << (*i);
      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }

  gLogStream <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

//________________________________________________________________________
void mxsr2msrTranslator::displayTupletsStack (
  const std::string& context)
{
  size_t tupletsStackSize = fTupletsStack.size ();

  gLogStream <<
    std::endl <<
    ">>++++++++++++++++ " <<
    "The tuplets std::stack contains " << tupletsStackSize << " elements:" <<
    std::endl;

  if (tupletsStackSize) {
    std::list<S_msrTuplet>::const_iterator
      iBegin = fTupletsStack.begin (),
      iEnd   = fTupletsStack.end (),
      i      = iBegin;

    S_msrTuplet tuplet = (*i);

    ++gIndenter;

    int n = tupletsStackSize;
    for ( ; ; ) {
      gLogStream <<
        "v (" << n << ")" <<
        std::endl;

      ++gIndenter;
      tuplet->printShort (gLogStream);
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLogStream << std::endl;
    } // for

    --gIndenter;
  }

  gLogStream <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_slur& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_slur" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (! gGlobalMxsr2msrOahGroup->getIgnoreSlurs ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSlursDetails ()) {
      displaySlurStartsStack ("BEFORE handling slur");
    }
#endif

    /*
      Only the  first note of the chord should get the slur notation.
      Some applications print out the slur for all notes,
      i.e. a stop and a start in sequqnce:
      these should be ignored
    */
    if (fCurrentNoteBelongsToAChord) {
      std::stringstream s;

      s <<
        "ignoring a slur in a chord member note other than the first one";

      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
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
          inputLineNumber,
          placementString);

      // a phrasing slur is recognized as such
      // when the nested regular slur start is found

      size_t slurStartsStackSize = fSlurStartsStack.size ();

      if (fCurrentSlurType == "start") {
        switch (slurStartsStackSize) {
          case 0:
            fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularStart;
            break;

          case 1:
            {
              S_msrSlur
                containingSlur =
                  fSlurStartsStack.front ();

              fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularStart;

  /* JMI BUGGED?
              // the std::stack top is in fact a phrasing slur start
  #ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceSlurs ()) {
                gLogStream <<
                  "The slur start '" <<
                  containingSlur->asString () <<
                  "' contains a nested slur, it is thus a phrasing slur start" <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
  #endif

              containingSlur->
                setSlurTypeKind (
                  msrSlurTypeKind::kSlurTypePhrasingStart);
                  */
            }
            break;

          default:
            {
              std::stringstream s;

              s <<
                "only one slur nesting level is meaningfull";

        //      mxsr2msrError ( // JMI
              mxsr2msrWarning (
                gGlobalServiceRunData->getInputSourceName (),
                inputLineNumber,
        //        __FILE__, __LINE__,
                s.str ());
            }
        } // switch

        fOnGoingSlur = true;
      }

      else if (fCurrentSlurType == "continue") {
        // the current slur continue kind depends on that of the std::stack's top
        switch (fSlurStartsStack.front ()->getSlurTypeKind ()) {
          case msrSlurTypeKind::kSlurTypeRegularStart:
            fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularContinue;
            break;

          case msrSlurTypeKind::kSlurTypePhrasingStart:
            // the std::stack top is in fact a phrasing slur start
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
              std::stringstream s;

              s <<
                "a standalone slur 'stop' is meaningless, ignoring it";

              mxsr2msrWarning (
                gGlobalServiceRunData->getInputSourceName (),
                inputLineNumber,
                s.str ());
            }
            break;

          case 1:
          /* JMI
            // the current slur stop is regular
            fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularStop;

            // pop the top element off the std::stack
            fSlurStartsStack.pop_front ();
            break;
    */
          case 2:
            // the current slur stop kind depends on that of the std::stack's top
            switch (fSlurStartsStack.front ()->getSlurTypeKind ()) {
              case msrSlurTypeKind::kSlurTypeRegularStart:
                fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypeRegularStop;
                break;

              case msrSlurTypeKind::kSlurTypePhrasingStart:
                // the std::stack top is in fact a phrasing slur start
  #ifdef TRACING_IS_ENABLED
                if (gGlobalTracingOahGroup->getTraceSlurs ()) {
                  gLogStream <<
                    "A slur stop matches a phrasing slur start, it is thus a phrasing slur stop" <<
                    ", line " << inputLineNumber <<
                    std::endl;
                }
  #endif

                fCurrentSlurTypeKind = msrSlurTypeKind::kSlurTypePhrasingStop;
                break;

              default:
                ; // should not occur
            } // switch

            // pop the top element off the std::stack
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
            std::stringstream s;

            s <<
              "slur type \"" << fCurrentSlurType <<
              "\" is unknown";

            mxsr2msrError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
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
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            "slur line-type \"" + slurLineType + "\" is unknown");
        }
      }

    // color JMI

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceSlurs ()) {
        gLogStream <<
          "visitStart (S_slur&)"
          ", slurNumber: " <<
          slurNumber <<
          ", slurTypeKind: " <<
          msrSlurTypeKindAsString (
            fCurrentSlurTypeKind) <<
          ", slurLineType: " <<
          msrLineTypeKindAsString (
            slurLineTypeKind) <<
          std::endl;
      }
#endif

      S_msrSlur
        slur =
          msrSlur::create (
            inputLineNumber,
            slurNumber,
            fCurrentSlurTypeKind,
            slurLineTypeKind,
            fCurrentDirectionPlacementKind);

      fPendingSlursList.push_back (slur);

      // push slurs starts onto the std::stack
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlursDetails ()) {
    displaySlurStartsStack ("AFTER handling slur");
  }
#endif
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_bracket& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_bracket" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
        std::stringstream s;

        s <<
          "ligature type \"" << ligatureType <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
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
  else if (ligatureLineEndValue == "[NONE]") {
    ligatureLineEndKind = msrLigatureLineEndKind::kLigatureLineEndNone;
  }
  else {
    if (ligatureLineEndValue.size ()) {
      std::stringstream s;

      s <<
        "ligature line-end \"" << ligatureLineEndValue <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        "ligature line-type \"" + ligatureLineTypeValue + "\" is unknown");
    }
  }

  // color JMI

  // create the ligature
  S_msrLigature
    ligature =
      msrLigature::create (
        inputLineNumber,
        ligatureNumber,
        fCurrentLigatureKind,
        ligatureLineEndKind,
        ligatureLineTypeKind,
        fCurrentDirectionPlacementKind);

  // append it to the pending ligatures std::list
  fPendingLigaturesList.push_back (ligature);

  switch (fCurrentLigatureKind) {
    case msrLigatureKind::kLigatureStart:
      // remember this ligature spanner start
      switch (fCurrentDirectionPlacementKind) {
        case msrPlacementKind::kPlacement_NO_:
          {
#ifdef TRACING_IS_ENABLED
            if (gGlobalTracingOahGroup->getTraceLigatures ()) {
              std::stringstream s;

              s <<
                "Bracket start found with no placement, placing it above by default";

              mxsr2msrWarning (
                gGlobalServiceRunData->getInputSourceName (),
                inputLineNumber,
                s.str ());
            }
#endif

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
        case msrPlacementKind::kPlacement_NO_:
          {
#ifdef TRACING_IS_ENABLED
            if (gGlobalTracingOahGroup->getTraceLigatures ()) {
              std::stringstream s;

              s <<
                "Bracket stop found with no placement, placing it above by default";

              mxsr2msrWarning (
                gGlobalServiceRunData->getInputSourceName (),
                inputLineNumber,
                s.str ());
            }
#endif

            ligature->setLigaturePlacementKind (msrPlacementKind::kPlacementAbove);

            if (! fCurrentLigatureStartAbove) {
              std::stringstream s;

              s <<
                "Bracket 'stop above' found with no corresponding bracket start, ignoring it"; // JMI cannot occur

           // JMI   mxsr2msrError (
              mxsr2msrWarning (
                gGlobalServiceRunData->getInputSourceName (),
                inputLineNumber,
           //     __FILE__, __LINE__,
                s.str ());
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
            std::stringstream s;

            s <<
              "Bracket 'stop above' found with no corresponding bracket start, ignoring it";

         // JMI   mxsr2msrError (
            mxsr2msrWarning (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
         //     __FILE__, __LINE__,
              s.str ());
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
            std::stringstream s;

            s <<
              "Bracket 'stop below' found with no corresponding bracket start, ignoring it";

         // JMI   mxsr2msrError (
            mxsr2msrWarning (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
         //     __FILE__, __LINE__,
              s.str ());
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
void mxsr2msrTranslator::visitStart (S_wedge& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_wedge" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "unknown wedge type \"" +
        type +
        "\", should be 'crescendo', 'diminuendo' or 'stop'";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
      std::stringstream s;

      s <<
        "wedge niente \"" << nienteString <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        "wedge line-type \"" + wedgeLineType + "\" is unknown");
    }
  }

  // color JMI ???

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllWedgesBelow ()) {
    fCurrentDirectionPlacementKind = msrPlacementKind::kPlacementBelow;
  }

  if (! gGlobalMxsr2msrOahGroup->getIgnoreWedges ()) {
    S_msrWedge
      wedge =
        msrWedge::create (
          inputLineNumber,
          wedgeKind,
          wedgeNienteKind,
          wedgeLineTypeKind,
          fCurrentDirectionPlacementKind);

    fPendingWedgesList.push_back (wedge);
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_lyric& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_lyric" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // number

  {
    fCurrentStanzaNumber =
      elt->getAttributeValue ("number");

    if (fCurrentStanzaNumber.size () == 0) {
      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        "lyric number is empty, using \"1\" by default");

      fCurrentStanzaNumber = "1";
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLyrics ()) {
      gLogStream <<
        "Setting fCurrentStanzaNumber to " <<
        fCurrentStanzaNumber <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

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
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceLyrics ()) {
        gLogStream <<
          "Lyric name is empty, using \"" <<
          msrStanza::K_NO_STANZA_NAME <<
          "\" by default" <<
          std::endl;
      }
#endif

      fCurrentStanzaName = msrStanza::K_NO_STANZA_NAME;
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLyrics ()) {
      gLogStream <<
        "Setting fCurrentStanzaName to \"" <<
        fCurrentStanzaName <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif

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
      inputLineNumber,
      printObjectString);

  // color JMI

  // forget about any previous texts found,
  // in case there are <text> occurrences without <syllabic> around them
  fCurrentLyricTextsList.clear ();

  fCurrentStanzaHasText = false;

  fOnGoingLyric = true;
}

void mxsr2msrTranslator::visitStart (S_syllabic& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_syllabic" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentSyllabic = elt->getValue();

  fCurrentSyllableKind       = msrSyllableKind::kSyllableNone;

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
    std::stringstream s;

    s <<
      "syllabic \"" << fCurrentSyllabic <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // forget about any previous texts found
  fCurrentLyricTextsList.clear ();
}

void mxsr2msrTranslator::visitStart (S_text& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_text" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string textValue = elt->getValue();

  mfConvertHTMLEntitiesToPlainCharacters (textValue); // JMI ???

  // color JMI

  // there can be several <text/>'s and <elision/> in a row, hence the std::list
  fCurrentLyricTextsList.push_back (textValue);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "textValue = \"" << textValue << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentNoteHasLyrics = true;
  fCurrentStanzaHasText = true;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    ++gIndenter;

    const int fieldWidth = 23;

    gLogStream << std::left <<
      std::setw (fieldWidth) <<
      "line " << " = " << inputLineNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentSyllabic" << " = " << fCurrentSyllabic <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentLyricTextsList" << " = ";

    msrSyllable::writeTextsList (
      fCurrentLyricTextsList,
      gLogStream);

    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  // a <text/> markup puts an end to the effect of <extend/>
  fCurrentSyllableExtendKind = msrSyllableExtendKind::kSyllableExtendNone;
}

void mxsr2msrTranslator::visitStart (S_elision& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_elision" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  std::string elisionValue = elt->getValue ();

  if (! elisionValue.size ()) {
    elisionValue = ' ';
  }

  // color JMI

  // there can be several <text/>'s and <elision/> in a row, hence the std::list
  fCurrentLyricTextsList.push_back (elisionValue);

  fCurrentStanzaHasText = true;
}

void mxsr2msrTranslator::visitStart (S_extend& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_extend" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string extendType =
    elt->getAttributeValue ("type");

  // extend
  if (fOnGoingLyric) {
    fCurrentSyllableExtendKind =
      msrSyllableExtendKind::kSyllableExtendEmpty; // default value

    if      (extendType == "start") {
      fCurrentSyllableExtendKind =
        msrSyllableExtendKind::kSyllableExtendStart;
    }
    else if (extendType == "continue") {
      fCurrentSyllableExtendKind =
        msrSyllableExtendKind::kSyllableExtendContinue;
    }
    else if (extendType == "stop") {
      fCurrentSyllableExtendKind =
        msrSyllableExtendKind::kSyllableExtendStop;
    }
    else {
      if (extendType.size ()) {
        std::stringstream s;

        s <<
          "extend type \"" << extendType <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
  //   JMI   fOnGoingSyllableExtend = true;
      }
    }
  }

  else if (fOnGoingFiguredBass) { // JMI
  }

  // color JMI
}

void mxsr2msrTranslator::visitEnd (S_lyric& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_lyric" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fCurrentSyllableKind == msrSyllableKind::kSyllableNone) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLyrics ()) {
      // syllabic is not mandatory, thus:
      std::stringstream s;

      s <<
        "<lyric /> has no <syllabic/> component, using 'single' by default";

      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }
#endif

    fCurrentSyllableKind = msrSyllableKind::kSyllableSingle;
  }

  if (fCurrentNoteIsARest) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLyrics ()) {
      std::stringstream s;

      s <<
        "syllable ";

     msrSyllable::writeTextsList (
      fCurrentLyricTextsList,
      s);

      s <<
        " is attached to a rest";

      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }
#endif

    if (fCurrentLyricTextsList.size ()) {
      // register a skip in lyrics for rests with syllables
      fCurrentSyllableKind =
        msrSyllableKind::kSyllableOnRestNote;
    }
    else {
      // don't register a skip in lyrics for rests without syllables
      fCurrentSyllableKind =
        msrSyllableKind::kSyllableSkipRestNote;
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyricsDetails ()) {
    gLogStream <<
      "==> visitEnd (S_lyric&), fCurrentSyllableKind: " <<
      msrSyllableKindAsString (fCurrentSyllableKind) <<
      ", line " << inputLineNumber <<
      ", with:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      "Lyric data:" <<
      std::endl;

    {
      ++gIndenter;

      const int fieldWidth = 31;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentMusicXMLStaffNumber" << " = " << fCurrentMusicXMLStaffNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStaffNumberToInsertInto" << " = " << fCurrentStaffNumberToInsertInto <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentLyricTextsList" << " = ";

      msrSyllable::writeTextsList (
        fCurrentLyricTextsList,
        gLogStream);

      gLogStream << std::left <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentSyllableExtendKind" << " = " <<
        msrSyllableExtendKindAsString (
          fCurrentSyllableExtendKind) <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentNoteIsARest" << " = " <<
        fCurrentNoteIsARest <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentRestIsAMeasureRest" << " = " <<
        fCurrentRestIsAMeasureRest <<
        std::endl;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentTieKind" << " = \"" <<
        msrTieKindAsString (fCurrentTieKind) <<
        "\"" <<
        std::endl;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentSlurTypeKind" << " = \"" <<
        msrSlurTypeKindAsString (fCurrentSlurTypeKind) <<
        "\"" <<
        std::endl;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentLigatureKind" << " = \"" <<
				fCurrentLigatureKind <<
        "\"" <<
        std::endl;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fOnGoingSlur" << " = " <<
        fOnGoingSlur <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fOnGoingSlurHasStanza" << " = " <<
        fOnGoingSlurHasStanza <<
        std::endl;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fOnGoingLigature" << " = " <<
        fOnGoingLigature <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fOnGoingLigatureHasStanza" << " = " <<
        fOnGoingLigatureHasStanza <<
        std::endl;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fFirstSyllableInSlurKind" << " = \"" <<
        msrSyllableKindAsString (
          fFirstSyllableInSlurKind) <<
        "\"" <<
        std::endl;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fFirstSyllableInLigatureKind" << " = \"" <<
        msrSyllableKindAsString (
          fFirstSyllableInLigatureKind) <<
        "\"" <<
      std::endl;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentSyllableKind" << " = \"" <<
        msrSyllableKindAsString (
          fCurrentSyllableKind) <<
        "\"" <<
      std::endl;

      --gIndenter;
    }

    --gIndenter;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "==> visitEnd (S_lyric&), fCurrentSyllableKind: " <<
      msrSyllableKindAsString (fCurrentSyllableKind) <<
      ", line = " << inputLineNumber <<
      ", with:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      "Lyric data:" <<
      std::endl;

    {
      ++gIndenter;

      const int fieldWidth = 31;

      gLogStream << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentMusicXMLStaffNumber" << " = " << fCurrentMusicXMLStaffNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStaffNumberToInsertInto" << " = " << fCurrentStaffNumberToInsertInto <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentLyricTextsList" << " = ";

      msrSyllable::writeTextsList (
        fCurrentLyricTextsList,
        gLogStream);

      --gIndenter;
    }

    --gIndenter;
  }
#endif

  if (! gGlobalMxsr2msrOahGroup->getIgnoreLyrics ()) {
    // fetch current voice
    S_msrVoice
      currentVoice =
        fetchVoiceFromCurrentPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);

    // fetch stanzaNumber in current voice
    S_msrStanza
      stanza =
        currentVoice->
          fetchStanzaInVoice (
            inputLineNumber,
            fCurrentStanzaNumber,
            fCurrentStanzaName);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLyrics ()) {
      gLogStream <<
        "Creating a syllable '" <<
        msrSyllableKindAsString (
          fCurrentSyllableKind) <<
        "\", fCurrentLyricTextsList = \"";

      msrSyllable::writeTextsList (
        fCurrentLyricTextsList,
        gLogStream);

      gLogStream <<
        "\"" <<
        ", whole notes: " <<
        fCurrentNoteSoundingWholeNotesFromDuration <<
        " sounding from duration, " <<
         fCurrentNoteDisplayWholeNotesFromType <<
        ", display from type" <<
        ", syllabic = \"" <<
        msrSyllableKindAsString (
          fCurrentSyllableKind) << "\"" <<
        ", in stanza " << stanza->getStanzaName () <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    // create a syllable
    S_msrSyllable
      syllable =
        msrSyllable::create (
          inputLineNumber,
          fCurrentSyllableKind,
          fCurrentSyllableExtendKind,
          fCurrentStanzaNumber,
          fCurrentNoteSoundingWholeNotesFromDuration,
					msrTupletFactor (
						fCurrentNoteActualNotes,
						fCurrentNoteNormalNotes),
          stanza);

    // append the lyric texts to the syllable
    for (
      std::list<std::string>::const_iterator i = fCurrentLyricTextsList.begin ();
      i!=fCurrentLyricTextsList.end ();
      ++i
    ) {
      syllable->
        appendLyricTextToSyllable ((*i));
    } // for

    // don't forget about fCurrentLyricTextsList here,
    // this will be done in visitStart (S_syllabic& )

    // appendSyllableToNoteAndSetItsUpLinkToNote()
    // will be called in handleLyrics(),
    // after the note has been created

    // append syllable to current note's syllables std::list
    fCurrentNoteSyllables.push_back (
      syllable);

    // append syllable to stanza
    stanza->
      appendSyllableToStanza (syllable);
  }

  // DON'T register current note as having lyrics,
  // it's only the case when there are <text/> inside the <lyric/>:
  // the latter may contain only an <extend/> markup,

  fOnGoingLyric = false;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_measure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_measure" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // take this measure into account
  ++fPartMeasuresCounter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
    gLogStream <<
      "==> visitStart (S_measure" <<
      ", fPartMeasuresCounter = '" <<
        fPartMeasuresCounter <<
      "', fCurrentMeasureNumber = '" <<
        fCurrentMeasureNumber <<
      "', line " << inputLineNumber <<
      ", in part \"" <<
      fCurrentPart->getPartCombinedName () << "\"" <<
      std::endl;
  }
#endif

  // number
  fCurrentMeasureNumber =
    elt->getAttributeValue ("number");

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalOahEarlyOptions.getEarlyTracePasses ()
  ) {
    gLogStream <<
      std::endl <<
      "<!--=== measure '" << fCurrentMeasureNumber << "'" <<
      ", line " << inputLineNumber << " ===-->" <<
      std::endl;
  }
#endif

  // set next measure number in current part' previous measure
  // if this measure is not the first one
  /* JMI ??? SUPERFLOUS ???
  if (fPartMeasuresCounter > 1) {
    fCurrentPart->
      setNextMeasureNumberInPart (
        inputLineNumber,
        fCurrentMeasureNumber);
  }
    */

  // consistency check
  if (! fPartFirstMeasureNumber.size ()) {
    // this is the first measure in the part
    fPartFirstMeasureNumber = fCurrentMeasureNumber;

    if (! fScoreFirstMeasureNumber.size ()) {
      // this is the first measure of the first part in the score
      fScoreFirstMeasureNumber = fPartFirstMeasureNumber;
    }
    else {
      if (fPartFirstMeasureNumber != fScoreFirstMeasureNumber) {
        std::stringstream s;

        s <<
          "measure numbering inconsistency: first measure numbers '" <<
          fScoreFirstMeasureNumber <<
           "' and '" <<
          fPartFirstMeasureNumber <<
          "' found";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

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
    implicit =
      elt->getAttributeValue ("implicit");

  msrMeasureImplicitKind
    measureImplicitKind =
      msrMeasureImplicitKind::kMeasureImplicitKindNo; // default value

  if       (implicit == "yes") {
    measureImplicitKind =
      msrMeasureImplicitKind::kMeasureImplicitKindYes;
  }
  else  if (implicit == "no") {
    measureImplicitKind =
      msrMeasureImplicitKind::kMeasureImplicitKindNo;
  }
  else {
    if (implicit.size ()) {
      std::stringstream s;

      s <<
        "implicit \"" << implicit <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // forget about the current non-grace note JMI HERE???
// JMI  fCurrentNonGraceNote = nullptr;

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // create a new measure and append it to the current part
  fCurrentPart->
    createAMeasureAndAppendItToPart (
      inputLineNumber,
      fPreviousMeasureEndInputLineNumber,
      fCurrentMeasureNumber,
      measureImplicitKind);

  fPreviousMeasureEndInputLineNumber = inputLineNumber;

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // reset staff change detection
  fPreviousNoteMusicXMLStaffNumber = 1; // default value
  fCurrentMusicXMLStaffNumber      = 1; // default value
  fCurrentStaffNumberToInsertInto  = 1; // default value JMI msrStaff::K_NO_STAFF_NUMBER;

/* JMI
  // is this measure number in the debug set?
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDetailed ()) {
    std::set<int>::const_iterator
      it =
        gGlobalServiceRunData->fTraceDetailedMeasureNumbersSet.find (fMeasuresCounter);

    if (it != gGlobalTracingOahGroup->getTraceDetailedMeasureNumbersSet ().end ()) {
      // yes, activate detailed trace for it
      gGlobalMxsr2msrOahGroup = gGlobalMxsr2msrOahWithDetailedTrace;
      gGlobalServiceRunData  = gGlobalServiceRunDataWithDetailedTrace;
      gGlobalMsrOahGroup      = gGlobalMsrOahWithDetailedTrace;
      gGlobalLpsrOahGroup     = gGlobalLpsrOahWithDetailedTrace;
      gGlobalLpsr2lilypondOahGroup = gGlobalLilypondOahWithDetailedTrace;
    }
  }
#endif
  */
}

void mxsr2msrTranslator::visitEnd (S_measure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_measure" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // fetch the voice
  S_msrVoice
    voice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);

  // fetch note to attach to
  S_msrNote
    noteToAttachTo =
    /*
    // JMI might prove not precise enough???
//      fVoicesLastMetNoteMap [currentVoice];
      fVoicesLastMetNoteMap [
        std::make_pair (
          fCurrentStaffNumberToInsertInto,
          fCurrentMusicXMLVoiceNumber)
        ];
    */
      voice->getVoiceLastAppendedNote (); // ??? JMI

  // is there a pending grace notes group?
  if (fPendingGraceNotesGroup) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        std::endl <<
        "fCurrentGraceNotes IS NOT NULL at the end of measure '" << // JMI
        elt->getAttributeValue ("number") <<
        "'" <<
        std::endl;

      gLogStream <<
        "+++++++++++++++++" <<
        fCurrentPart <<
        std::endl;
    }
#endif

    // attach these grace notes group as an after grace notes group
    // to the last note found in its voice
    if (! noteToAttachTo) {
      std::stringstream s;

      s <<
        "cannot find voice to insert after grace notes group into: " <<
        "chordFirstNote is null" <<
        ", fCurrentStaffNumberToInsertInto = " <<
        fCurrentStaffNumberToInsertInto <<
        std::endl <<
        ", fCurrentMusicXMLVoiceNumber = " <<
        fCurrentMusicXMLVoiceNumber;

      mxsr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

    // set the pending grace notes group's kind to 'after'
    fPendingGraceNotesGroup->
      setGraceNotesGroupKind (
        msrGraceNotesGroupKind::kGraceNotesGroupAfter);

    // attach it to the note
    noteToAttachTo->
      setNoteGraceNotesGroupAfter (
        fPendingGraceNotesGroup);

    // forget about this grace notes group
    fPendingGraceNotesGroup = nullptr;
  }

  // is there an on going chord to be finalized?
  if (fOnGoingChord) {
    // finalize the current chord
    finalizeCurrentChord (
      inputLineNumber);

    fOnGoingChord = false;
  }

  if (fCurrentATupletStopIsPending) {
    if (fTupletsStack.size ()) { // JMI
      // finalize the tuplet, only now
      // in case the last element is actually a chord
      finalizeTupletAndPopItFromTupletsStack (
        inputLineNumber);
    }

    fCurrentATupletStopIsPending = false;
  }

  // attach the spanners if any to the note
  attachCurrentSpannersToNote (noteToAttachTo);

  // finalize current measure in the part,
  // to add skips if necessary and set measure kind
  fCurrentPart->
    finalizeLastAppendedMeasureInPart (
      inputLineNumber);

  // should this measure be replicated?
  const std::map<std::string,int>&
    measuresToBeReplicatedStringToIntMap =
      gGlobalMxsr2msrOahGroup->getMeasuresToBeReplicatedStringToIntMap ();

  if (measuresToBeReplicatedStringToIntMap.size ()) {
    // should we add empty measures after current measures?
    std::map<std::string,int>::const_iterator
      it =
        measuresToBeReplicatedStringToIntMap.find (
          fCurrentMeasureNumber);

    if (it != measuresToBeReplicatedStringToIntMap.end ()) {
      // fCurrentMeasureNumber is to be replicated,
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
        gLogStream <<
          std::endl <<
          "Replicating meaure " <<
          fCurrentMeasureNumber <<
          " in part " <<
          fCurrentPart->getPartCombinedName () <<
          std::endl;
      }
#endif

      int measureReplicatesNumber = (*it).second;

      fCurrentPart->
        replicateLastAppendedMeasureInPart (
          inputLineNumber,
          measureReplicatesNumber);
    }
    else {
      // fRemainingExpectedMultipleFullBarRests JMI ???
    }
  }

  // should empty measures be added after this one?
  const std::map<std::string,int>&
    addEmptyMeasuresStringToIntMap =
      gGlobalMxsr2msrOahGroup->getAddEmptyMeasuresStringToIntMap ();

  if (addEmptyMeasuresStringToIntMap.size ()) {
    // should we add empty measures after current measures?
    std::map<std::string,int>::const_iterator
      it =
        addEmptyMeasuresStringToIntMap.find (
          fCurrentMeasureNumber);

    if (it != addEmptyMeasuresStringToIntMap.end ()) {
      // fCurrentMeasureNumber is present in the std::map,
      // fetch the number of empty measures to add
      std::stringstream s;

      s << (*it).second;

      int measuresToBeAdded;

      s >> measuresToBeAdded;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
        gLogStream <<
          std::endl <<
          "Adding " <<
          mfSingularOrPlural (
            measuresToBeAdded, "empty measure", "empty measures") <<
          " to part " <<
          fCurrentPart->getPartCombinedName () <<
          std::endl;
      }
#endif

      fCurrentPart->
        addEmptyMeasuresToPart (
          inputLineNumber,
          fCurrentMeasureNumber,
          measuresToBeAdded);
    }
    else {
      // fRemainingExpectedMultipleFullBarRests JMI ???
    }
  }

//   // handle an on-going multiple full-bar rests if any only now,
//   // JMI do it before???
//   if (fOnGoingMultipleFullBarRests) {
//     handleOnGoingMultipleFullBarRestsAtTheEndOfMeasure (
//       inputLineNumber);
//   }
}

void mxsr2msrTranslator::handleOnGoingMultipleFullBarRestsAtTheEndOfMeasure (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    const int fieldWidth = 37;

    gLogStream <<
      "--> handleOnGoingMultipleFullBarRestsAtTheEndOfMeasure()" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      std::setw (fieldWidth) <<
      "fCurrentMultipleFullBarRestsHasBeenCreated " << " : " <<
      fCurrentMultipleFullBarRestsHasBeenCreated <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fRemainingExpectedMultipleFullBarRests" << " : " <<
      fRemainingExpectedMultipleFullBarRests <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif

  //  if (! fOnGoingMultipleFullBarRests) { JMI
  //   }

//   if (! fCurrentMultipleFullBarRestsHasBeenCreated) {
//     // create a pending  multiple full-bar rests,
//     // that will be handled when fRemainingExpectedMultipleFullBarRests
//     // comes down to 0 later in this very method
//     fCurrentPart->
//       appendMultipleFullBarRestsToPart (
//         inputLineNumber,
//         fCurrentMultipleFullBarRestsNumber);
//
//     fCurrentMultipleFullBarRestsHasBeenCreated = true;
//   }

  if (fRemainingExpectedMultipleFullBarRests <= 0) {
    mxsr2msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "remainingMultipleFullBarRestsMeasuresNumber problem");
  }

  // account for one more full-bar rest measure in the multiple full-bar rests
  --fRemainingExpectedMultipleFullBarRests;

  if (fRemainingExpectedMultipleFullBarRests == 0) {
    // all multiple full-bar rests have been handled,
    // the current one is the first after the  multiple full-bar rests
    fCurrentPart->
      appendPendingMultipleFullBarRestsToPart (
        inputLineNumber);

    if (fRemainingExpectedMultipleFullBarRests == 1) {
      fCurrentPart-> // JMI ??? BOF
        setNextMeasureNumberInPart (
          inputLineNumber,
          fCurrentMeasureNumber);
    }

    // forget about and  multiple full-bar rests having been created
    fCurrentMultipleFullBarRestsHasBeenCreated = false;

    fOnGoingMultipleFullBarRests = false;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    const int fieldWidth = 37;

    gLogStream <<
      "--> handleOnGoingMultipleFullBarRestsAtTheEndOfMeasure()" <<
      ", onGoingMultipleFullBarRests:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      std::setw (fieldWidth) <<
      "fCurrentMultipleFullBarRestsHasBeenCreated " << " : " <<
      fCurrentMultipleFullBarRestsHasBeenCreated <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fRemainingExpectedMultipleFullBarRests" << " : " <<
      fRemainingExpectedMultipleFullBarRests <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fOnGoingMultipleFullBarRests " << " : " <<
      fOnGoingMultipleFullBarRests <<
      std::endl;

    --gIndenter;
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_print& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_print" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
  fCurrentPrintLayout =
     msrPrintLayout::create (
      inputLineNumber);

  // handle 'staff-spacing' if present

  const std::string staffSpacing =
    elt->getAttributeValue ("staff-spacing");

  if (staffSpacing.size ()) {
    std::stringstream s;

    s << staffSpacing;
    float value;
    s >> value;

    fCurrentPrintLayout->setStaffSpacing (value);
  }

  // handle 'new-system' if present

  const std::string newSystem = elt->getAttributeValue ("new-system");

  if (newSystem.size ()) {
    fCurrentPrintLayout->setNewSystem ();

    if (newSystem == "yes") {
      // create a line break
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
        gLogStream <<
          "Creating a line break, " <<
          "line = " << inputLineNumber <<
          std::endl;
      }
#endif

      S_msrLineBreak
        lineBreak =
          msrLineBreak::create (
            inputLineNumber,
            fCurrentMeasureNumber,
            msrUserChosenLineBreakKind::kUserChosenLineBreakNo);

      // append lineBreak to the pending line breaks
      fPendingLineBreaksList.push_back (lineBreak);
    }

    else if (newSystem == "no") {
      // ignore it
    }

    else {
      std::stringstream s;

      s << "new-system \"" << newSystem <<
      "\" is unknown in '<print />', should be 'yes', 'no' or empty";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // handle 'new-page' if present and relevant
  if (! gGlobalMxsr2msrOahGroup->getIgnorePageBreaksInMusicXML ()) {
    const std::string newPage = elt->getAttributeValue ("new-page");

    if (newPage.size ()) {
      fCurrentPrintLayout->setNewPage ();

      if (newPage == "yes") { // JMI
        // create a page break
  #ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
          gLogStream <<
            "Creating a page break, " <<
            "line = " << inputLineNumber <<
            std::endl;
        }
  #endif

        S_msrPageBreak
          pageBreak =
            msrPageBreak::create (
              inputLineNumber,
              msrUserChosenPageBreakKind::kUserChosenPageBreakNo);

        // append it to the pending page breaks
        fPendingPageBreaksList.push_back (pageBreak);
       }

      else if (newPage == "no") {
        // ignore it
      }

      else {
        std::stringstream s;

        s << "new-page \"" << newPage <<
        "\" is unknown in '<print />', should be 'yes', 'no' or empty";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // handle 'blank-page' if present

  const int blankPage = elt->getAttributeIntValue ("blank-page", 0);

  if (blankPage > 0) {
    fCurrentPrintLayout->setBlankPage (blankPage);
  }

  // handle 'page-number' if present

  const int pageNumber = elt->getAttributeIntValue ("page-number", 0);

  if (pageNumber > 0) {
    fCurrentPrintLayout->setPageNumber (pageNumber);
  }

  fCurrentDisplayText = "";

  fOnGoingPrint = true;
}

void mxsr2msrTranslator::visitEnd (S_print& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_print" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // append the current print layout to voice 1 in staff 1 of the current part
  // it's not worth using specific 'layout voices' for such part-level stuff
  S_msrVoice
    voiceOneInStaffOne =
      fetchFirstVoiceFromCurrentPart (
        inputLineNumber);

  voiceOneInStaffOne->
    appendPrintLayoutToVoice (
      fCurrentPrintLayout);

  // forget about the current print layout
  fCurrentPrintLayout = nullptr;

  fOnGoingPrint = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_measure_numbering& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_measure_numbering" <<
       ", line " << elt->getInputLineNumber () <<
     std::endl;
  }
#endif

  // JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_barline& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_barline" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "barLine location \"" << location <<
        "\" is unknown, using 'right' by default";

   // JMI   mxsr2msrError (
      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
   //     __FILE__, __LINE__,
        s.str ());
    }
  }

  fOnGoingBarLine = true;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_bar_style& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_bar_style" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
  else if (barStyle == "[NONE]") {
    fCurrentBarLineStyleKind =
      msrBarLineStyleKind::kBarLineStyleNone;
  }
  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "bar-style \"" + barStyle + "\" is unknown");
  }

  // color JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_segno& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_segno" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingDirectionType) {
    // create the segno
    S_msrSegno
      segno =
        msrSegno::create (
          inputLineNumber,
          fCurrentDirectionStaffNumber);

    // append it to the pending segnos std::list
    fPendingSegnosList.push_back (segno);
  }

  else if (fOnGoingBarLine) {
    fCurrentBarLineHasSegnoKind =
      msrBarLineHasSegnoKind::kBarLineHasSegnoYes;
  }

  else {
    std::stringstream s;

    s << "<segno /> is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_coda& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_coda" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
          std::stringstream s;

          s <<
            "this is <coda /> number " << fCodasCounter <<
            ", only two are meaningful";

          mxsr2msrError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
    } // switch

    // create the coda
    S_msrCoda
      coda =
        msrCoda::create (
          inputLineNumber,
          fCurrentDirectionStaffNumber,
          codaKind);

    // append it to the pending codas std::list
    fPendingCodasList.push_back (coda);
  }

  else if (fOnGoingBarLine) {
    fCurrentBarLineHasCodaKind =
      msrBarLineHasCodaKind::kBarLineHasCodaYes;
  }

  else {
    std::stringstream s;

    s << "<coda /> is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_eyeglasses& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_eyeglasses" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingDirectionType) {
    // create the eyeglasses
    S_msrEyeGlasses
      eyeGlasses =
        msrEyeGlasses::create (
          inputLineNumber);

    // append it to the pending eyeglasses std::list
    fPendingEyeGlassesList.push_back (eyeGlasses);
  }

  else {
    std::stringstream s;

    s << "<eyeGlasses /> is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_pedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pedal" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // type

  std::string type = elt->getAttributeValue ("type");

  msrPedalTypeKind pedalTypeKind = msrPedalTypeKind::kPedalType_NO_;

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
    std::stringstream s;

    s << "pedal type '" << type << "' is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
      std::stringstream s;

      s <<
        "pedal line \"" << line <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
      std::stringstream s;

      s <<
        "pedal sign \"" << sign <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // create the pedal
  S_msrPedal
    pedal =
      msrPedal::create (
        inputLineNumber,
        pedalTypeKind,
        pedalLineKind,
        pedalSignKind);

  if (fOnGoingDirectionType) {
    // append it to the pending pedals std::list
    fPendingPedalsList.push_back (pedal);
  }
  else {
    std::stringstream s;

    s << "<pedal /> " << pedal->asShortString () << " is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_ending& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_ending" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // number

  {
    fCurrentBarLineEndingNumber =
      elt->getAttributeValue ("number"); // may be "1, 2"

    if (! fCurrentBarLineEndingNumber.size ()) {
      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
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
      std::stringstream s;

      s <<
        "ending type \"" << type <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_repeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_repeat" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "repeat direction \"" << direction <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // winged

  {
    std::string winged = elt->getAttributeValue ("winged");

    fCurrentBarLineRepeatWingedKind =
      msrBarLineRepeatWingedKind::kBarLineRepeatWingedNone; // default value

    if (winged.size ()) {
      if       (winged == "[NONE]") {
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
        std::stringstream s;

        s <<
          "repeat winged \"" << winged <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
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
void mxsr2msrTranslator::visitEnd (S_barline& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_barline" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // is there a pending tuplet?
  if (fTupletsStack.size ()) { // JMI
    // finalize the tuplet,
    // for it to be created before the barLine
    finalizeTupletAndPopItFromTupletsStack (
      inputLineNumber);
  }

  // create the barLine
  S_msrBarLine
    barLine =
      msrBarLine::create (
        inputLineNumber,
        fCurrentBarLineLocationKind,
        fCurrentBarLineStyleKind,
        fCurrentBarLineRepeatDirectionKind,
        fCurrentBarLineEndingTypeKind,
        fCurrentBarLineEndingNumber,
        fCurrentBarLineTimes,
        msrBarLineCategoryKind::kBarLineCategory_NO_, // will be set afterwards
        fCurrentBarLineHasSegnoKind,
        fCurrentBarLineHasCodaKind,
        fCurrentBarLineRepeatWingedKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Creating barLine in part " <<
      fCurrentPart->getPartCombinedName () << ":" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      barLine;

    --gIndenter;
  }
#endif

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
      if (
        fCurrentBarLineEndingTypeKind
          ==
        msrBarLineEndingTypeKind::kBarLineEndingTypeStart
      ) {
        // ending start, don't know yet whether it's hooked or hookless
        // ------------------------------------------------------
        if (! fCurrentBarLineEndingNumber.size ()) {
          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
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
      break;

    case msrBarLineLocationKind::kBarLineLocationMiddle:
      // JMI ???
      break;

    case msrBarLineLocationKind::kBarLineLocationRight:
      {
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

        // forget about current repeat ending start barLine
        fCurrentRepeatEndingStartBarLine = nullptr;
      }
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
  #ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceBarLines ()) {
          gLogStream <<
            "Appending a standalone barLine to part " <<
            fCurrentPart->getPartCombinedName () << ":" <<
            std::endl;

          ++gIndenter;

          gLogStream <<
            barLine;

          --gIndenter;
        }
  #endif

//         fCurrentPart->
//           appendBarLineToPart (barLine);
        fPendingBarLinesList.push_back (barLine); // JMIJMIJMI

        barLineHasBeenHandled = true;
        break;

      case msrBarLineStyleKind::kBarLineStyleNone:
        std::stringstream s;

        s <<
          "barLine " <<
          barLine->asString () <<
          " has no barLine style";

        mxsr2msrWarning (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
     //     __FILE__, __LINE__,
          s.str ());
        break;
    } // switch
  }

  // has this barLine been handled?
  if (! barLineHasBeenHandled) {
    std::stringstream s;

    s << std::left <<
      "cannot handle a barLine containing: " <<
      barLine->asString ();

    mxsr2msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

/* JMI
  // sanity check
  switch (barLine->getBarLineCategory ()) {
    case msrBarLineCategoryKind::kBarLineCategory_NO_:
      {
        std::stringstream s;

        s <<
          "barLine " <<
          barLine->asString () <<
          " has no barLine category";

      mxsr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
*/

  fOnGoingBarLine = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_note& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_note" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/*
====
print-dot
Controls the printing of an augmentation dot separately from the rest of the note or rest. This is especially useful for notes that overlap in different voices, or for chord sheets that contain lyrics and chords but no melody. If print-object is set to no, this attribute is also interpreted as being set to no if not present.

print-leger
Indicates whether leger lines are printed. Notes without leger lines are used to indicate indeterminate high and low notes. It is yes if not present unless print-object is set to no. This attribute is ignored for rests.
print-lyric  yes-no  No  Controls the printing of a lyric separately from the rest of the note or rest. This is especially useful for notes that overlap in different voices, or for chord sheets that contain lyrics and chords but no melody. If print-object is set to no, this attribute is also interpreted as being set to no if not present.

print-object
Specifies whether or not to print an object. It is yes if not specified.

print-spacing
Controls whether or not spacing is left for an invisible note or object. It is used only if no note, dot, or lyric is being printed. The value is yes (leave spacing) if not specified.

*/

  // save previous note staff number
  fPreviousNoteMusicXMLStaffNumber = fCurrentMusicXMLStaffNumber;

  // initialize note data to a neutral state
  initializeNoteData ();

  // transpose handling
//    fCurrentTransposeDouble = false; NO ??? JMI

  // lyrics handling
  fCurrentNoteHasLyrics = false;

  // rests
  fCurrentNoteIsARest = false;

  // unpitched notes
  fCurrentNoteIsUnpitched = false;

  // grace notes
  fCurrentNoteIsAGraceNote = false;

  // cross staff chords
  fCurrentNoteIsCrossStaves = false;

  // tremolos handling
  fCurrentNoteBelongsToADoubleTremolo = false;

  // chords handling
  fCurrentNoteBelongsToAChord = false;

  // tuplets handling
  fCurrentNoteHasATimeModification = false; // superflous JMI ???
  fCurrentNoteBelongsToATuplet = false;

  fCurrentNoteDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fCurrentNoteAlterationKind    = msrAlterationKind::kAlterationNatural;

  fCurrentNoteOctave = msrOctaveKind::kOctave_NO_;

  fCurrentNoteSoundingWholeNotes             = Rational (0, 1);
  fCurrentNoteSoundingWholeNotesFromDuration = Rational (0, 1);

  fCurrentDisplayDiatonicPitchKind      = msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fCurrentDisplayOctave                 = msrOctaveKind::kOctave_NO_;
  fCurrentNoteDisplayWholeNotes         = Rational (0, 1);
  fCurrentNoteDisplayWholeNotesFromType = Rational (0, 1);

  // note head

  fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadNormal;
  fCurrentNoteHeadFilledKind = msrNoteHeadFilledKind::kNoteHeadFilledYes;
  fCurrentNoteHeadParenthesesKind = msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo;

  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentMusicXMLStaffNumber = 1; // JMI

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentMusicXMLVoiceNumber = 1; // JMI

  // tuplets

  fCurrentNoteActualNotes = -1;
  fCurrentNoteNormalNotes = -1;

  // harmonies

  fCurrentHarmonyWholeNotesOffset = Rational (0, 1);

  // lyrics

  fCurrentStanzaNumber = msrStanza::K_NO_STANZA_NUMBER;
  fCurrentStanzaName = msrStanza::K_NO_STANZA_NAME;

  fCurrentSyllabic = "";
  // don't forget about fCurrentLyricTextsList here,
  // this will be done in visitStart (S_syllabic& )
  fCurrentSyllableKind = msrSyllableKind::kSyllableNone;

  if (fOnGoingSyllableExtend) {
    fCurrentSyllableExtendKind =
      msrSyllableExtendKind::kSyllableExtendContinue; // it may be absent
  }
  else {
    fCurrentSyllableExtendKind =
      msrSyllableExtendKind::kSyllableExtendNone;
  }

  // stems

  fCurrentStem = nullptr;

  // tremolos

  fCurrentDoubleTremoloTypeKind = msrDoubleTremoloTypeKind::kDoubleTremoloType_NO_;

  // ties

  fCurrentTie = nullptr;
  fCurrentTiedOrientation = "";

  // slurs

  fCurrentSlurType = "";
  fCurrentSlurTypeKind = msrSlurTypeKind::kSlurType_NO_;

  // ligatures

  fCurrentLigatureKind = msrLigatureKind::kLigatureNone;

  // print-object

  std::string printObjectString =
    elt->getAttributeValue ("print-object");

//   gLogStream << // JMI TEST v0.9.65
// 		"fCurrentNotePrintObjectKind = " <<
// 		fCurrentNotePrintObjectKind <<
// 		std::endl;

  fCurrentNotePrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);

  // note color, unofficial ??? JMI

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
    std::stringstream s;

    s <<
      "note color \"" << noteColorAlphaRGB <<
      "\" should contain 6 or 8 upper case hexadecimal digits prededed by a '#'";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingNote = true;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_step" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string step = elt->getValue();

  checkStep (
    inputLineNumber,
    step,
    "<step/>");

  fCurrentNoteDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (step [0]);
}

void mxsr2msrTranslator::visitStart (S_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_alter" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  float alter = (float)(*elt);

  fCurrentNoteAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      alter);

  if (fCurrentNoteAlterationKind == msrAlterationKind::kAlteration_NO_) {
    std::stringstream s;

    s <<
      "alter '" << alter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_octave& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_octave" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int octaveNumber = (int)(*elt);

  if (octaveNumber < 0 || octaveNumber > 9) {
    std::stringstream s;

    s <<
      "octave number '" << octaveNumber <<
      "' is not in the 0..9 range, '0' is assumed";

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      s.str ());

    octaveNumber = 0;
  }

  fCurrentNoteOctave =
    msrOctaveKindFromNumber (
      inputLineNumber,
      octaveNumber);
}

void mxsr2msrTranslator::visitStart (S_duration& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_duration" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int duration = (int)(*elt); // divisions

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      "Note duration: " << duration <<
      std::endl;
  }
#endif

  if (fOnGoingBackup) {
    fCurrentBackupDivisions = duration;
  }

  else if (fOnGoingForward) {
    fCurrentForwardDivisions = duration;
  }

  else if (fOnGoingNote) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
      gLogStream <<
        "fCurrentDivisionsPerQuarterNote: " <<
        fCurrentDivisionsPerQuarterNote <<
        std::endl;
    }
#endif

    // set current grace note whole notes
    fCurrentNoteSoundingWholeNotesFromDuration =
      Rational (
        duration,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
      gLogStream <<
        "fCurrentNoteSoundingWholeNotesFromDuration: " <<
        fCurrentNoteSoundingWholeNotesFromDuration <<
        std::endl;
    }
#endif

/* JMI
    // set current grace note display whole notes
    // to note sounding whole notes
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteSoundingWholeNotesFromDuration; // by default
     */
  }

  else if (fOnGoingFiguredBass) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
      gLogStream <<
        "fCurrentDivisionsPerQuarterNote: " <<
        fCurrentDivisionsPerQuarterNote <<
        std::endl;
    }
#endif

    // set current figured bass sounding whole notes
    fCurrentFiguredBassSoundingWholeNotes =
      Rational (
        duration,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
      gLogStream <<
        "fCurrentFiguredBassSoundingWholeNotes: " <<
        fCurrentFiguredBassSoundingWholeNotes <<
        std::endl;
    }
#endif

    // set current figured bass display whole notes
    fCurrentFiguredBassDisplayWholeNotes =
      Rational (
        duration,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
      gLogStream <<
        "fCurrentFiguredBassDisplayWholeNotes: " <<
        fCurrentFiguredBassDisplayWholeNotes <<
        std::endl;
    }
#endif
  }

  else {
    std::stringstream s;

    s << "<duration /> " << duration << " is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

//  gLogStream << "=== mxsr2msrTranslator::visitStart (S_duration& elt), fCurrentDuration = " << fCurrentDuration << std::endl; JMI
}

void mxsr2msrTranslator::visitStart (S_instrument& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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

void mxsr2msrTranslator::visitStart (S_dot& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_dot" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++fCurrentNoteDotsNumber;
}

void mxsr2msrTranslator::visitStart (S_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_type" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/*
 Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.
*/

  {
    std::string noteType = elt->getValue();

    // the type contains a display duration,
    fCurrentNoteGraphicDurationKind =
      msrDurationKindFromMusicXMLString (
        inputLineNumber,
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
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
            "note type size \"" + noteTypeSize + "\" is unknown");
      }
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    /* JMI
    gLogStream <<
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
#endif
}

void mxsr2msrTranslator::visitStart (S_notehead& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_notehead" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
    else if (noteHead == "[NONE]") {
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
      std::stringstream s;

      s <<
        "note head \"" << noteHead <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
        std::stringstream s;

        s <<
          "note head filled \"" << noteHeadFilled <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
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
        std::stringstream s;

        s <<
          "note head parentheses \"" << noteHeadParentheses <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // color JMI

}

void mxsr2msrTranslator::visitStart (S_accidental& elt) // JMI
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_accidental" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
        std::stringstream s;

        s <<
          "accidental \"" << accidentalValue <<
          "\" is unknown";

        mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
          s.str ());
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
        std::stringstream s;

        s <<
          "editorial accidental \"" << editorialAccidental <<
          "\" is unknown";

        mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
          s.str ());
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
        std::stringstream s;

        s <<
          "cautionary accidental \"" << cautionaryAccidental <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }
}

void mxsr2msrTranslator::visitStart (S_stem& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_stem" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

  std::string        stem = elt->getValue();

  // kind
  msrStemKind stemKind = msrStemKind::kStemNeutral;

  if      (stem == "up")
    stemKind = msrStemKind::kStemUp;

  else if (stem == "down")
    stemKind = msrStemKind::kStemDown;

  else if (stem == "[NONE]")
    stemKind = msrStemKind::kStemNeutral;

  else if (stem == "double")
    stemKind = msrStemKind::kStemDouble;

  else {
    std::stringstream s;

    s <<
      "stem \"" << fCurrentBeamValue <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // color JMI

  fCurrentStem =
    msrStem::create (
      inputLineNumber,
      stemKind);
}

void mxsr2msrTranslator::visitStart (S_beam& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_beam" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // value

  fCurrentBeamValue = elt->getValue();

  msrBeamKind beamKind = msrBeamKind::kBeam_NO_;

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
    std::stringstream s;

    s <<
      "beam \"" << fCurrentBeamValue <<
      "\"" << "is not known";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // number

  fCurrentBeamNumber =
    elt->getAttributeIntValue ("number", 0);

  S_msrBeam
    beam =
      msrBeam::create (
        inputLineNumber,
        fCurrentBeamNumber,
        beamKind);

  // color JMI ???

  fPendingBeamsList.push_back (beam);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_measure_style& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_measure_style" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // color JMI
}

void mxsr2msrTranslator::visitStart (S_beat_repeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_beat_repeat" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      inputLineNumber,
      useDotsString);
}

void mxsr2msrTranslator::visitStart (S_measure_repeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_measure_repeat" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
    msrMeasureRepeatKind::kMeasureRepeat_NO_;

  if      (measureRepeatType == "start") {
    fCurrentMeasureRepeatKind =
      msrMeasureRepeatKind::kMeasureRepeatStart; // JMI

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Creating measures repeat from its first measures" <<
      "in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", fCurrentMeasureRepeatMeasuresNumber: " <<
      fCurrentMeasureRepeatMeasuresNumber <<
      ", fCurrentMeasureRepeatSlashesNumber: " <<
      fCurrentMeasureRepeatSlashesNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

    fCurrentPart->
      createMeasureRepeatFromItsFirstMeasuresInPart (
        inputLineNumber,
        fCurrentMeasureRepeatMeasuresNumber,
        fCurrentMeasureRepeatSlashesNumber);
  }

  else if (measureRepeatType == "stop") {
    fCurrentMeasureRepeatKind =
      msrMeasureRepeatKind::kMeasureRepeatStop; // JMI

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Appending measures repeat " <<
      "to part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

    fCurrentPart->
      appendPendingMeasureRepeatToPart (
        inputLineNumber);
  }

  else {
    std::stringstream s;

    s <<
      "measure-repeat type \"" << measureRepeatType <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_multiple_rest& elt)
{
/*
<!--
  A measure-style indicates a special way to print partial
  to multiple measures within a part. This includes multiple
  rests over several measures, repeats of beats, single, or
  multiple measures, and use of slash notation.

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

  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_multiple_rest" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentMultipleFullBarRestsNumber = (int)(*elt);

  std::string useSymbols = elt->getAttributeValue ("use-symbols");

  // what do we do with fMultipleFullBarRestsUseSymbols ??? JMI v0.9.63
  if      (useSymbols == "yes") {
    fMultipleFullBarRestsUseSymbols = true;
  }
  else if (useSymbols == "no") {
    fMultipleFullBarRestsUseSymbols = false;
  }
  else {
    if (useSymbols.size ()) {
      std::stringstream s;

      s <<
        "multiple rest use symbols " <<
        useSymbols <<
        " is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }

  // create a multiple full-bar rests
  fCurrentPart->
    appendMultipleFullBarRestsToPart (
      inputLineNumber,
      fCurrentMultipleFullBarRestsNumber);

  fCurrentMultipleFullBarRestsHasBeenCreated = true;

  // register number of remeaining expected multiple full-bar rests
  fRemainingExpectedMultipleFullBarRests =
    fCurrentMultipleFullBarRestsNumber;

  // the  multiple full-bar rests will created at the end of its first measure,
  // so that the needed staves/voices have been created

  fOnGoingMultipleFullBarRests = true;
}

void mxsr2msrTranslator::visitStart (S_slash& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_slash" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // type

  std::string slashType = elt->getAttributeValue ("type");

  if      (slashType == "start")
    fCurrentSlashTypeKind = msrSlashTypeKind::kSlashTypeStart;
  else if (slashType == "stop")
    fCurrentSlashTypeKind = msrSlashTypeKind::kSlashTypeStop;
  else {
    std::stringstream s;

    s <<
      "slash type \"" << slashType <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // use-dots

  std::string useDotsString = elt->getAttributeValue ("use-dots");

  fCurrentUseDotsKind =
    msrUseDotsFromString (
      inputLineNumber,
      useDotsString);

  // use-stems

  std::string slashUseStems = elt->getAttributeValue ("use-stems");

  if      (slashUseStems == "yes")
    fCurrentSlashUseStemsKind = msrSlashUseStemsKind::kSlashUseStemsYes;
  else if (slashUseStems == "no")
    fCurrentSlashUseStemsKind = msrSlashUseStemsKind::kSlashUseStemsNo;
  else {
    if (slashUseStems.size ()) {
      std::stringstream s;

      s <<
        "slash use-stems \"" << slashUseStems <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fCurrentSlashDotsNumber = 0;
}

void mxsr2msrTranslator::visitStart (S_slash_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_slash_type" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string slashType = elt->getValue();

  // the type contains a display duration,
  fCurrentSlashGraphicDurationKind =
    msrDurationKindFromMusicXMLString (
      inputLineNumber,
      slashType);

  // size

  std::string slashTypeSize = elt->getAttributeValue ("size");

  if (slashTypeSize == "cue") { // USE IT! JMI ???
  }

  else {
    if (slashTypeSize.size ()) {
      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
          "slash type size \"" + slashTypeSize + "\" is unknown");
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlashes ()) {
    gLogStream <<
      "slashType: \"" <<
      slashType <<
      "\"" <<
      std::endl <<
      "slashTypeSize: \"" <<
      slashTypeSize <<
      "\"" <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitStart (S_slash_dot& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_slash_dot" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++fCurrentSlashDotsNumber;
}

void mxsr2msrTranslator::visitEnd (S_slash& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_slash" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrSlash
    slash =
      msrSlash::create (
        inputLineNumber,
        fCurrentSlashTypeKind,
        fCurrentUseDotsKind,
        fCurrentSlashUseStemsKind);

  fPendingSlashesList.push_back (slash);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_articulations& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_articulations" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitStart (S_accent& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_accent" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationAccent,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_breath_mark& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_breath_mark" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationBreathMark,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_caesura& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_caesura" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationCaesura,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_spiccato& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_spiccato" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationSpiccato,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_staccato& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staccato" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationStaccato,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_staccatissimo& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staccatissimo" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationStaccatissimo,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_stress& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_stress" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationStress,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_unstress& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

 #ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_unstress" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationUnstress,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_detached_legato& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_detached_legato" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationDetachedLegato,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_strong_accent& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_strong_accent" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  // type : upright inverted  (Binchois20.xml) // JMI
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationStrongAccent,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_tenuto& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tenuto" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // type : upright inverted  (Binchois20.xml) // JMI ???

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationTenuto,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_doit& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_doit" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationDoit,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_falloff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_falloff" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationFalloff,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_plop& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_plop" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationPlop,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitStart (S_scoop& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_scoop" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the articulation
  S_msrArticulation
    articulation =
      msrArticulation::create (
        inputLineNumber,
        msrArticulationKind::kArticulationScoop,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreArticulations ()) {
    fCurrentArticulations.push_back (articulation);
  }
}

void mxsr2msrTranslator::visitEnd (S_articulations& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_articulations" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_arpeggiate& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_arpeggiate" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
      std::stringstream s;

      s <<
        "arpeggiate direction \"" << directionString << "\"" << "is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // color JMI

  // create the arpeggiato
  S_msrArpeggiato
    arpeggiato =
      msrArpeggiato::create (
        inputLineNumber,
        placementKind,
        directionKind,
        number);

  fCurrentArticulations.push_back (arpeggiato);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_non_arpeggiate& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_non_arpeggiate" << // JMI
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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

      std::stringstream s;

      s <<
        "non-arpeggiate type \"" << typeString <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // number

  int number = elt->getAttributeIntValue ("number", 0);

  // color JMI

  // create the non arpeggiato
  S_msrNonArpeggiato
    nonArpeggiato =
      msrNonArpeggiato::create (
        inputLineNumber,
        placementKind,
        nonArpeggiatoTypeKind,
        number);

  fCurrentArticulations.push_back (nonArpeggiato);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_technical& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_technical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fOnGoingTechnical = true;
}

void mxsr2msrTranslator::visitEnd (S_technical& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_technical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fOnGoingTechnical = false;
}

void mxsr2msrTranslator::visitStart (S_arrow& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_arrow" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalArrow,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_bend_alter& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_bend_alter" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fBendAlterValue = (float)(*elt);
}

void mxsr2msrTranslator::visitStart (S_bend& elt) // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_bend" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitEnd (S_bend& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_bend" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical with float
  S_msrTechnicalWithFloat
    technicalWithFloat =
      msrTechnicalWithFloat::create (
        inputLineNumber,
        msrTechnicalWithFloatKind::kTechnicalWithFloatBend,
        fBendAlterValue,
        placementKind);

  fCurrentTechnicalWithFloatsList.push_back (
    technicalWithFloat);
}

void mxsr2msrTranslator::visitStart (S_double_tongue& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_double_tongue" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalDoubleTongue,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_down_bow& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_down_bow" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalDownBow,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_fingering& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_fingering" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int fingeringValue = (int)(*elt);

  if (fOnGoingTechnical) {
    // placement

    std::string placementString = elt->getAttributeValue ("placement");

    msrPlacementKind
      placementKind =
        msrPlacementKindFromString (
          inputLineNumber,
          placementString);

  // create the technical with integer
    S_msrTechnicalWithInteger
      technicalWithInteger =
        msrTechnicalWithInteger::create (
          inputLineNumber,
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
    std::stringstream s;

    s <<
      "<fingering /> \"" << fingeringValue <<
      "\" is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_fingernails& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_fingernails" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalFingernails,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_fret& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_fret" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int fretValue = (int)(*elt);

  if (fOnGoingTechnical) {
    // placement

    std::string placementString = elt->getAttributeValue ("placement");

    msrPlacementKind
      fretPlacementKind =  msrPlacementKind::kPlacement_NO_;

    // color JMI ???

    // create the technical with integer
    S_msrTechnicalWithInteger
      technicalWithInteger =
        msrTechnicalWithInteger::create (
          inputLineNumber,
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
    std::stringstream s;

    s <<
      "<fret /> \"" << fretValue <<
      "\" is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_hammer_on& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_hammer_on" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string hammerOnValue = elt->getValue ();

  // type

  std::string hammerOnType = elt->getAttributeValue ("type");

  msrTechnicalTypeKind hammerOnTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalType_NO_;

  if      (hammerOnType == "start")
    hammerOnTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalTypeStart;
  else if (hammerOnType == "stop")
    hammerOnTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalTypeStop;
  else {
    if (hammerOnType.size ()) {
      std::stringstream s;

      s <<
        "hammer-on type \"" << hammerOnType <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithStringKind::kHammerOn,
        hammerOnTechnicalTypeKind,
        hammerOnValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrTranslator::visitStart (S_handbell& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_handbell" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string handBellValue = elt->getValue ();

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithStringKind::kHandbell,
        msrTechnicalTypeKind::kTechnicalType_NO_,
        handBellValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrTranslator::visitStart (S_harmonic& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_harmonic" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalHarmonic,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_heel& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_heel" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalHeel,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_hole& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_hole" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalHole,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_open_string& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_open_string" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalOpenString,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_other_technical& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_other_technical" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string otherTechnicalValue = elt->getValue ();

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithStringKind::kOtherTechnical,
        msrTechnicalTypeKind::kTechnicalType_NO_,
        otherTechnicalValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrTranslator::visitStart (S_pluck& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pluck" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string pluckValue = elt->getValue ();

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithStringKind::kPluck,
        msrTechnicalTypeKind::kTechnicalType_NO_,
        pluckValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrTranslator::visitStart (S_pull_off& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pull_off" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string pullOffValue = elt->getValue ();

  // type

  std::string pullOffType = elt->getAttributeValue ("type");

  msrTechnicalTypeKind pullOffTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalType_NO_;

  if      (pullOffType == "start")
    pullOffTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalTypeStart;
  else if (pullOffType == "stop")
    pullOffTechnicalTypeKind = msrTechnicalTypeKind::kTechnicalTypeStop;
  else {
    if (pullOffType.size ()) {
      std::stringstream s;

      s <<
        "pull-off type \"" << pullOffType <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical with std::string
  S_msrTechnicalWithString
    technicalWithString =
      msrTechnicalWithString::create (
        inputLineNumber,
        msrTechnicalWithStringKind::kPullOff,
        pullOffTechnicalTypeKind,
        pullOffValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrTranslator::visitStart (S_snap_pizzicato& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_snap_pizzicato" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalSnapPizzicato,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_stopped& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_stopped" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalStopped,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_string& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_string" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
    std::stringstream s;

    stringIntegerValue = 0;

    s <<
      "std::string value \"" << stringValue <<
      "\" is empoty, '" << stringIntegerValue << "' is assumed";

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  if (fOnGoingTechnical) {
    // placement

    std::string placementString = elt->getAttributeValue ("placement");

    msrPlacementKind
      placementKind =
        msrPlacementKindFromString (
          inputLineNumber,
          placementString);

  // create the technical with integer
    S_msrTechnicalWithInteger
      technicalWithInteger =
        msrTechnicalWithInteger::create (
          inputLineNumber,
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
    std::stringstream s;

    s <<
      "<std::string /> \"" << stringValue <<
      "\" is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_tap& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tap" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalTap,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_thumb_position& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_thumb_position" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalThumbPosition,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_toe& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_toe" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalToe,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_triple_tongue& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_triple_tongue" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalTripleTongue,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

void mxsr2msrTranslator::visitStart (S_up_bow& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_up_bow" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the technical
  S_msrTechnical
    technical =
      msrTechnical::create (
        inputLineNumber,
        msrTechnicalKind::kTechnicalUpBow,
        placementKind);

  fCurrentTechnicalsList.push_back (technical);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_fermata& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_fermata" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "fermata kind \"" << fermataTextValue <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
      std::stringstream s;

      s <<
        "fermata type \"" << fermataTypeValue <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // create the fermata
  S_msrFermata
    fermata =
      msrFermata::create (
        inputLineNumber,
        fermataShapeKind,
        articulationFermataType);

  fCurrentArticulations.push_back (fermata);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_ornaments& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_ornaments" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitStart (S_tremolo& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting tremolo" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // value (tremolo marks number)

  std::string tremoloMarksNumberString =
    elt->getValue ();

  int tremoloMarksNumber = (int)(*elt);

  if (! tremoloMarksNumberString.size ()) {
    std::stringstream s;

    tremoloMarksNumber = 1;

    s <<
      "--> tremolo value is missing, '" << tremoloMarksNumber << "' assumed";

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  if (tremoloMarksNumber < 0 || tremoloMarksNumber > 8) { // JMI what does 0 mean?
    std::stringstream s;

    s <<
      "tremolo value \"" << tremoloMarksNumber <<
      "\" should be between 0 and 8";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
    std::stringstream s;

    s <<
      "tremolo type \"" << tremoloType <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    singleTremoloPlacementKind = msrPlacementKind::kPlacement_NO_;

  msrPlacementKind
    doubleTremoloPlacementKind = msrPlacementKind::kPlacement_NO_;

/* JMI ???
  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);
*/

  if      (placementString == "above") {
    switch (fCurrentDoubleTremoloTypeKind) {
      case msrDoubleTremoloTypeKind::kDoubleTremoloType_NO_:
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
      case msrDoubleTremoloTypeKind::kDoubleTremoloType_NO_:
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

    std::stringstream s;

    s <<
      "tremolo placement \"" << placementString <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // handle double tremolos
  switch (fCurrentDoubleTremoloTypeKind) {
    case msrDoubleTremoloTypeKind::kDoubleTremoloType_NO_:
      // just to avoid a compiler message
      break;

    case msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle:
      // create a single tremolo, it will be attached to current note
      // in attachCurrentSingleTremoloToNote()
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceTremolos ()) {
        gLogStream <<
          "Creating a single tremolo" <<
          ", line " << inputLineNumber <<
          ", " <<
          mfSingularOrPlural (
            tremoloMarksNumber, "mark", "marks") <<
          ", placement : " <<
          msrPlacementKindAsString (
            singleTremoloPlacementKind) <<
          std::endl;
      }
#endif

      fCurrentSingleTremolo =
        msrSingleTremolo::create (
          inputLineNumber,
          tremoloMarksNumber,
          singleTremoloPlacementKind);
      break;

    case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart:
  //    if (! fCurrentDoubleTremolo) { JMI
      {
        // fetch current voice
        S_msrVoice
          currentVoice =
            fetchVoiceFromCurrentPart (
              inputLineNumber,
              fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
              fCurrentMusicXMLVoiceNumber);

        // create a double tremolo start
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTremolos ()) {
          gLogStream <<
            "Creating a double tremolo" <<
            ", line " << inputLineNumber <<
            ", " <<
            mfSingularOrPlural (
              tremoloMarksNumber, "mark", "marks") <<
            ", placement : " <<
            msrPlacementKindAsString (
              doubleTremoloPlacementKind) <<
            std::endl;
        }
#endif

        fCurrentDoubleTremolo =
          msrDoubleTremolo::create (
            inputLineNumber,
            msrDoubleTremoloKind::kDoubleTremoloNotes,
            msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart,
            tremoloMarksNumber,
            doubleTremoloPlacementKind);
      }

/*
      else {
        std::stringstream s;

        s <<
          "<tremolo/> start when a current double tremolo is already open";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
*/
      break;

    case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop:
      if (fCurrentDoubleTremolo) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTremolos ()) {
          gLogStream <<
            "Meeting a double tremolo stop" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        // it will be handled in
        // handleNonChordNorTupletNoteOrRest()
      }

      else {
        std::stringstream s;

        s <<
          "<tremolo/> stop whit no preceeding <tremolo/> start";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch
}

void mxsr2msrTranslator::visitStart (S_trill_mark& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_trill_mark" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

 // type : upright inverted  (Binchois20.xml) JMI

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentTrill,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_dashes& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_dashes" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

 // type : upright inverted  (Binchois20.xml) JMI

  // number

  int dashesNumber = elt->getAttributeIntValue ("number", 0); // JMI

  // type

  std::string dashesType = elt->getAttributeValue ("type");

  msrSpannerTypeKind fDashesSpannerTypeKind = msrSpannerTypeKind::kSpannerType_NO_;

  if      (dashesType == "start")
    fDashesSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeStart;
  else if (dashesType == "continue")
    fDashesSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeContinue;
  else if (dashesType == "stop")
    fDashesSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeStop;
  else {
    if (dashesType.size ()) {
      std::stringstream s;

      s <<
        "dashes type \"" << dashesType <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // color ??? JMI

  // create the spanner
  S_msrSpanner
    spanner =
      msrSpanner::create (
        inputLineNumber,
        dashesNumber,
        msrSpannerKind::kSpannerDashes,
        fDashesSpannerTypeKind,
        placementKind,
        nullptr); // will be set later REMOVE??? JMI

  fCurrentSpannersList.push_back (spanner);
}

void mxsr2msrTranslator::visitStart (S_wavy_line& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_wavy_line" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // number

  int wavyLineNumber = elt->getAttributeIntValue ("number", 0); // JMI

  // type

  std::string wavyLineType = elt->getAttributeValue ("type");

  msrSpannerTypeKind fWavyLineSpannerTypeKind = msrSpannerTypeKind::kSpannerType_NO_;

  if      (wavyLineType == "start")
    fWavyLineSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeStart;
  else if (wavyLineType == "continue")
    fWavyLineSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeContinue;
  else if (wavyLineType == "stop")
    fWavyLineSpannerTypeKind = msrSpannerTypeKind::kSpannerTypeStop;
  else {
    if (wavyLineType.size ()) {
      std::stringstream s;

      s <<
        "wavy-line type \"" << wavyLineType <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // color JMI ???

  // create the spanner
  S_msrSpanner
    spanner =
      msrSpanner::create (
        inputLineNumber,
        wavyLineNumber,
        msrSpannerKind::kSpannerWavyLine,
        fWavyLineSpannerTypeKind,
        placementKind,
        nullptr); // will be set later REMOVE??? JMI

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
        std::stringstream s;

        s <<
          "wavy-line stop found without corresponding start, ignoring it";

        mxsr2msrWarning (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          s.str ());
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

    case msrSpannerTypeKind::kSpannerType_NO_:
      // JMI ???
      break;
  } // switch
}

void mxsr2msrTranslator::visitStart (S_turn& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_turn" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_inverted_turn& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_inverted_turn" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentInvertedTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_delayed_turn& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_delayed_turn" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentDelayedTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_delayed_inverted_turn& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_delayed_inverted_turn" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentDelayedInvertedTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_vertical_turn& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_vertical_turn" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentVerticalTurn,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_mordent& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_mordent" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentMordent,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_inverted_mordent& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_inverted_mordent" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentInvertedMordent,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_schleifer& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_schleifer" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentSchleifer,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_shake& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_shake" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentShake,
        placementKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitStart (S_accidental_mark& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_accidental_mark" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
    currentOrnamentAccidentalKind = msrAccidentalKind::kAccidentalFlatFlat;

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
    std::stringstream s;

    s <<
      "accidental-mark \"" << accidentalMark <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // create the ornament
  S_msrOrnament
    ornament =
      msrOrnament::create (
        inputLineNumber,
        msrOrnamentKind::kOrnamentAccidentalKind,
        placementKind);

  ornament->
    setOrnamentAccidentalKind (
      currentOrnamentAccidentalKind);

  if (! gGlobalMxsr2msrOahGroup->getIgnoreOrnaments ()) {
    fCurrentOrnamentsList.push_back (ornament);
  }
}

void mxsr2msrTranslator::visitEnd (S_ornaments& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_ornaments" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart( S_f& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_f" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_ff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_ff" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_fff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_fff" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicFFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_ffff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_ffff" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicFFFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_fffff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_fffff" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicFFFFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_ffffff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_ffffff" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicFFFFFF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_p& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_p" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_pp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_ppp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_ppp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicPPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_pppp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pppp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicPPPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_ppppp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_ppppp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicPPPPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_pppppp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pppppp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicPPPPPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}


void mxsr2msrTranslator::visitStart( S_mf& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_mf" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicMF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}
void mxsr2msrTranslator::visitStart( S_mp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_mp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicMP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_fp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_fp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicFP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_fz& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_fz" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicFZ,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_pf& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pf" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicPF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_rf& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_rf" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicRF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_sf& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_sf" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicSF,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_rfz& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_rfz" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicRFZ,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_sfz& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_sfz" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicSFZ,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_sfp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_sfp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicSFP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_sfpp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_sfpp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicSFPP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_sffz& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_sffz" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicSFFZ,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_sfzp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_sfzp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicSFZP,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_n& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_n" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
          msrDynamicKind::kDynamicN,
          placementKind);

    fPendingDynamicxList.push_back(dynamics);
  }
}

void mxsr2msrTranslator::visitStart( S_other_dynamics& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_other_dynamics" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string otherDynamicsValue = elt->getValue ();

  // placement

  std::string placementString = elt->getAttributeValue ("placement");

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
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
          inputLineNumber,
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
void mxsr2msrTranslator::visitStart( S_damper_pedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_damper_pedal" <<
      ", line " << inputLineNumber <<
      std::endl;
#endif

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
      std::stringstream s;

      s <<
        "damper pedal integer value \"" <<
        fCurrentDamperPedalIntegerValue <<
        "\" should be between 0 and 100";

      mxsr2msrError (
        inputLineNumber,
        s.str ());
    }

    fCurrentDamperPedalKind =
      msrDamperPedal::kDamperPedalValueZeroToAHundred;

/ * JMI
    if (damperPedalValue.size ()) {
      mxsr2msrError (
        inputLineNumber,
        "unknown damper pedal \"" +
          damperPedalValue +
          "\", should be 'yes', 'no' or a number from 0 to 100");
    }
    * /
  }


  S_msrDamperPedal
    damperPedal =
      msrDamperPedal::create (
        inputLineNumber,
        otherDynamicsValue);

}

void mxsr2msrTranslator::visitStart( S_soft_pedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_soft_pedal" <<
       ", line " << inputLineNumber <<
     std::endl;
  }
#endif

  std::string softPedalValue = elt->getValue ();

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the other dynamics
  S_msrOtherDynamic
    otherDynamic =
      msrOtherDynamic::create (
        inputLineNumber,
        otherDynamicsValue);

  fPendingOtherDynamicxList.push_back(otherDynamic);
}

void mxsr2msrTranslator::visitStart( S_sostenuto_pedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_sostenuto_pedal" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string sostenutoPedalValue = elt->getValue ();

  msrPlacementKind
    placementKind =
      msrPlacementKindFromString (
        inputLineNumber,
        placementString);

  // should the placement be forced to 'below'?
  if (gGlobalMxsr2msrOahGroup->getAllDynamicsBelow ()) {
    placementKind = msrPlacementKind::kPlacementBelow;
  }

  // create the other dynamics
  S_msrOtherDynamic
    otherDynamic =
      msrOtherDynamic::create (
        inputLineNumber,
        otherDynamicsValue);

  fPendingOtherDynamicxList.push_back(otherDynamic);
}
*/

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_cue& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_cue" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNoteIsACueNoteKind = msrNoteIsACueNoteKind::kNoteIsACueNoteYes;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_grace& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_grace" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentNoteIsAGraceNote = true;

  // slash

  std::string slash = elt->getAttributeValue ("slash");

  fCurrentGraceIsSlashed = false; // default value

  // check part group barLine
  if      (slash == "yes")
    fCurrentGraceIsSlashed = true;

  else if (slash == "no")
    fCurrentGraceIsSlashed = false;

  else {
    if (slash.size ()) {
      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        "grace slash \"" + slash + "\" unknown, should be 'yes' or 'no'");
    }
  }

  // should all grace notes be slashed?
  if (gGlobalMxsr2msrOahGroup->getSlashAllGraceNotes ()) {
    fCurrentGraceIsSlashed = true;
  }

  // should all grace notes be beamed?
  if (gGlobalMxsr2msrOahGroup->getBeamAllGraceNotes ()) {
    fCurrentGraceIsBeamed = true;
  }

  fCurrentStealTimeFollowing =
    elt->getAttributeValue ("steal-time-following");

  fCurrentStealTimePrevious =
    elt->getAttributeValue ("steal-time-previous");

  fCurrentMakeTimeSignature =
    elt->getAttributeValue ("make-time");
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_chord& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_chord" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // the current note belongs to a chord,
  // placed in the corresponding staff AND voice
  fCurrentNoteBelongsToAChord = true;

  // delay the handling until 'visitEnd (S_note& elt)',
  // because we don't know yet the voice and staff numbers for sure
  // (they can be specified after <chord/> in the <note/>)
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_time_modification& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_time_modification" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // there may be no '<tuplet number="n" type="start" />'
  // in the tuplet notes after the first one,
  // so we detect tuplet notes on '<time-modification>' ??? JMI
  fCurrentNoteHasATimeModification = true;
}

void mxsr2msrTranslator::visitStart (S_actual_notes& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_actual_notes" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int actualNotes = (int)(*elt);

  if (fOnGoingNote) {
    fCurrentNoteActualNotes = actualNotes;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "fCurrentNoteActualNotes: " <<
        fCurrentNoteActualNotes <<
        std::endl;
    }
#endif

    // notes inside a tuplet have no <tuplet/> markup
    // and 2 actual notes indicate a double tremolo
    switch (fCurrentNoteActualNotes) {
      case 2:
        fCurrentNoteBelongsToADoubleTremolo = true;
        break;
      default:
        fCurrentNoteBelongsToATuplet = true;
    } // switch
  }

  else if (fOnGoingMetronomeNote) {
    fCurrentMetronomeNoteActualNotes = actualNotes;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTempos ()) {
      gLogStream <<
        "fCurrentMetronomeNoteActualNotes: " <<
        fCurrentMetronomeNoteActualNotes <<
        std::endl;
    }
#endif
  }

  else {
    std::stringstream s;

    s <<
      "<actual-notes /> \"" << actualNotes <<
      "\" is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_normal_notes& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_normal_notes" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int normalNotes = (int)(*elt);

  if (fOnGoingNote) {
    fCurrentNoteNormalNotes = normalNotes;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "fCurrentNoteNormalNotes: " <<
        fCurrentNoteNormalNotes <<
        std::endl;
    }
#endif

    // notes inside a tuplet have no <tuplet/> markup
    // and 1 actual note indicates a double tremolo
    switch (fCurrentNoteNormalNotes) {
      case 1:
        fCurrentNoteBelongsToADoubleTremolo = true; // superflous ??? JMI
        break;
      default:
        fCurrentNoteBelongsToATuplet = true; // superflous ??? JMI
    } // switch
  }

  else if (fOnGoingMetronomeNote) {
    fCurrentMetronomeNoteNormalNotes = normalNotes;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTempos ()) {
      gLogStream <<
        "fCurrentMetronomeNoteNormalNotes: " <<
        fCurrentMetronomeNoteNormalNotes <<
        std::endl;
    }
#endif
  }

  else {
    std::stringstream s;

    s <<
      "<normal-notes /> \"" << normalNotes <<
      "\" is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_normal_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_normal_type" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string normalTypeString = elt->getValue();

  if (fOnGoingNote) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "normalTypeString: " <<
        normalTypeString <<
        std::endl;
    }
#endif

    // the type contains a display duration
    fCurrentNoteNormalTypeDuration =
      msrDurationKindFromMusicXMLString (
        inputLineNumber,
        normalTypeString);

  /*
    // there can be several <beat-unit/> in a <metronome/> markup,
    // register beat unit in in dotted durations std::list
    fCurrentMetronomeBeatUnitsVector.push_back (
      msrDottedDuration (
        fCurrentNoteNormalTypeDuration,
        0));
        */
  }

  else if (fOnGoingMetronomeNote) { // JMI ???
    fCurrentMetronomeNoteNormalType = normalTypeString;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTempos ()) {
      gLogStream <<
        "fCurrentMetronomeNoteNormalType: " <<
        fCurrentMetronomeNoteNormalType <<
        std::endl;
    }
#endif

  }

  else {
    std::stringstream s;

    s <<
      "<normal-type /> \"" << normalTypeString <<
      "\" is out of context";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_tuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tuplet" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // number

  fPreviousTupletNumber = fCurrentTupletNumber;

  fCurrentTupletNumber = elt->getAttributeIntValue ("number", 0);

  // bracket

  {
    std::string tupletBracket = elt->getAttributeValue ("bracket");

    fCurrentTupletBracketKind = msrTupletBracketKind::kTupletBracketYes; // option ??? JMI

    if      (tupletBracket == "yes")
      fCurrentTupletBracketKind = msrTupletBracketKind::kTupletBracketYes;
    else if (tupletBracket == "no")
      fCurrentTupletBracketKind = msrTupletBracketKind::kTupletBracketNo;
    else {
      if (tupletBracket.size ()) {
        std::stringstream s;

        s <<
          "tuplet bracket \"" << tupletBracket <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTuplets ()) {
          std::stringstream s;

          s <<
            "tuplet bracket is empty: this is implementation dependent," <<
            " \"yes\" is assumed"; // option ??? JMI

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            s.str ());
        }
#endif
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
        std::stringstream s;

        s <<
          "tuplet line-shape \"" << tupletLineShape <<
          "\" is unknown";

        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }
  }

  // type

  {
    std::string tupletType = elt->getAttributeValue ("type");

    msrTupletTypeKind
      previousTupletTypeKind = fCurrentTupletTypeKind;

    fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeNone;

    if      (tupletType == "start") {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceTuplets ()) {
        gLogStream <<
          "--> There is a tuplet start (kTupletTypeStart)" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStart;
    }
    else if (tupletType == "continue") {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceTuplets ()) {
        gLogStream <<
          "--> There is a tuplet continue (kTupletTypeContinue)" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeContinue;
    }
    else if (tupletType == "stop") {
      if (
        fPreviousTupletNumber == fCurrentTupletNumber
          &&
        (
          previousTupletTypeKind == msrTupletTypeKind::kTupletTypeStart
  // JMI          ||
    //      previousTupletTypeKind == msrTupletTypeKind::kTupletTypeContinue
        )
      ) {
        // this is a tuplet stop right after a tuplet start
        // for one and the same tuplet number:
        // possible if the note is a tremolo
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTuplets ()) {
          gLogStream <<
            "--> There is a tuplet stop right after a tuplet start for tuplet nummber " << fCurrentTupletNumber <<
            " (kTupletTypeStartAndStopInARow)" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStartAndStopInARow;
      }
      else {
        // this is a 'regular' tuplet stop
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTuplets ()) {
          gLogStream <<
            "--> There is a tuplet stop (kTupletTypeStop)" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStop;
      }
    }
    else {
      std::stringstream s;

      s <<
        "tuplet type \"" << tupletType <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
    else if (tupletShowNumber == "[NONE]") {
      fCurrentTupletShowNumberKind = msrTupletShowNumberKind::kTupletShowNumberNone;
    }
    else {
      if (tupletShowNumber.size ()) {
        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
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
    else if (tupletShowType == "[NONE]") {
      fCurrentTupletShowTypeKind = msrTupletShowTypeKind::kTupletShowTypeNone;
    }
    else {
      if (tupletShowType.size ()) {
        mxsr2msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          "tuplet show-type \"" + tupletShowType + "\" is unknown");
      }
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "fCurrentTupletNumber: " <<
      fCurrentTupletNumber <<
      ", fCurrentTupletTypeKind: " <<
      msrTupletTypeKindAsString (
        fCurrentTupletTypeKind) <<
      ", fCurrentTupletBracketKind: " <<
      msrTupletBracketKindAsString (
        fCurrentTupletBracketKind) <<
      ", fCurrentTupletShowNumberKind: " <<
      msrTupletShowNumberKindAsString (
        fCurrentTupletShowNumberKind) <<
      ", fCurrentTupletShowTypeKind: " <<
      msrTupletShowTypeKindAsString (
        fCurrentTupletShowTypeKind) <<
      std::endl;
  }
#endif

  fCurrentNoteBelongsToATuplet = true;
}

void mxsr2msrTranslator::visitStart (S_tuplet_actual& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tuplet_actual" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fOnGoingTupletActual = true;
}

void mxsr2msrTranslator::visitEnd (S_tuplet_actual& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_tuplet_actual" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fOnGoingTupletActual = false;
}

void mxsr2msrTranslator::visitStart (S_tuplet_normal& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tuplet_normal" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fOnGoingTupletNormal = true;
}

void mxsr2msrTranslator::visitEnd (S_tuplet_normal& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_tuplet_normal" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fOnGoingTupletNormal = false;
}

void mxsr2msrTranslator::visitStart (S_tuplet_number& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tuplet_number" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<tuplet-number /> out of context");
  }

  // color JMI

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "tuplet number (not handled): " <<
      tupletNumberValue <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitStart (S_tuplet_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tuplet_type" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<tuplet-type /> out of context");
  }

  // color JMI

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "tuplet type (not handled): " <<
      tupletTypeValue <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitStart (S_tuplet_dot& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_tuplet_dot" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingTupletActual) {
    ++fCurrentTupletActualDotsNumber;
  }
  else if (fOnGoingTupletNormal) {
    ++fCurrentTupletNormalDotsNumber;
  }
  else {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<tuplet-dot /> out of context");
  }

  // color JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_glissando& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_glissando" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
    std::stringstream s;

    s <<
      "glissando type \"" << glissandoType <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        "glissando line-type \"" + glissandoLineType + "\" is unknown");
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGlissandos ()) {
    gLogStream <<
      "glissandoNumber: " <<  glissandoNumber <<
      "glissandoType: " << glissandoTypeKind <<
      "glissandoLineType: " << glissandoLineTypeKind <<
      "glissandoTextValue: " << glissandoTextValue <<
      std::endl;
  }
#endif

  // create glissando
  S_msrGlissando
    glissando =
      msrGlissando::create (
        inputLineNumber,
        glissandoNumber,
        glissandoTypeKind,
        glissandoLineTypeKind,
        glissandoTextValue);

  // register glissando in this visitor
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGlissandos ()) {
    gLogStream <<
      "Appending glissando '" <<
      glissando->asString () <<
      "' to the glissandos pending std::list" <<
      std::endl;
  }
#endif

  fPendingGlissandosList.push_back (glissando);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_slide& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_slide" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
    std::stringstream s;

    s <<
      "slide type \"" << slideType <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        "slide line-type \"" + slideLineType + "\" is unknown");
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlides ()) {
    gLogStream <<
      "slideNumber: " <<
      slideNumber <<
      "slideType: " <<
			slideTypeKind <<
      "slideLineType: " <<
			slideLineTypeKind <<
      std::endl;
  }
#endif

  // create slide
  S_msrSlide
    slide =
      msrSlide::create (
        inputLineNumber,
        slideNumber,
        slideTypeKind,
        slideLineTypeKind,
        slideTextValue);

  // register glissando in this visitor
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlides ()) {
    gLogStream <<
      "Appending slide '" <<
      slide->asString () <<
      "' to the slides pending std::list" <<
      std::endl;
  }
#endif

  fPendingSlidesList.push_back (slide);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_rest& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_rest" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        "rest measure \"" + restMeasure + "\" is unknown");
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_display_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_display_step" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string displayStep = elt->getValue();

  checkStep (
    inputLineNumber,
    displayStep,
    "<display-step/>");

  fCurrentDisplayDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (
      displayStep [0]);

  // pitched rests don't allow for alterations since
  // the display-step merely indicates where to place them on the staff
  fCurrentNoteAlterationKind = msrAlterationKind::kAlterationNatural;
}

void mxsr2msrTranslator::visitStart (S_display_octave& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_display_octave" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int displayOctaveNumber = (int)(*elt);

  if (displayOctaveNumber < 0 || displayOctaveNumber > 9) {
    std::stringstream s;

    s <<
      "display octave number '" << displayOctaveNumber <<
      "' is not in the 0..9 range, '0' is assumed";

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());

    displayOctaveNumber = 0;
  }

  fCurrentDisplayOctave =
    msrOctaveKindFromNumber (
      inputLineNumber,
      displayOctaveNumber);
}

void mxsr2msrTranslator::visitEnd (S_unpitched& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_unpitched" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNoteIsUnpitched = true;

 // fCurrentNoteDiatonicPitch = // JMI
   // fCurrentHarmonyRootDiatonicPitch;
}

//______________________________________________________________________________
S_msrChord mxsr2msrTranslator::createChordFromItsFirstNote (
  int                  inputLineNumber,
  const S_msrVoice&           voice,
  S_msrNote            chordFirstNote,
  msrNoteKind noteKind)
{
  int firstNoteInputLineNumber =
    chordFirstNote->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "--> creating a chord from its first note '" <<
      chordFirstNote->asShortString () <<
      ", in voice \"" << voice->getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // chordFirstNote has been registered as regular note in the part element sequence,
  // but it is actually the first note of a chord

  // create a chord
  S_msrChord
    chord =
      msrChord::create (
        firstNoteInputLineNumber,
        chordFirstNote->getMeasureElementSoundingWholeNotes (),
        chordFirstNote->getNoteDisplayWholeNotes (),
        chordFirstNote->getNoteGraphicDurationKind ());

  // register note as first member of chord
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Adding first note " <<
      chordFirstNote->
        asShortString () <<
      ", line " << inputLineNumber <<
      ", to new chord" <<
      std::endl;
  }
#endif

  chord->
    addFirstNoteToChord (
      chordFirstNote, voice);

  // set chordFirstNote's kind
  chordFirstNote->
    setNoteKind (noteKind);

  // copy chordFirstNote's elements if any to the chord
  copyNoteElementsToChord (
    chordFirstNote, chord);

  // get chordFirstNote's uplink to measure
  S_msrMeasure
    chordFirstNoteDirectUpLinkToMeasure =
      chordFirstNote->
        getNoteUpLinkToMeasure ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChordsDetails ()) {
    gLogStream << // JMI
      std::endl << std::endl <<
      "++++++++++++++++ chordFirstNote =" <<
      std::endl << std::endl <<
      chordFirstNote <<
      std::endl <<
      "+++++++++++++++++" <<
      std::endl << std::endl <<
      "++++++++++++++++ chordFirstNote->getNoteUpLinkToMeasure () =";

    if (chordFirstNoteDirectUpLinkToMeasure) {
      gLogStream <<
        std::endl << std::endl;
    }
    else {
      gLogStream <<
        "[NONE]";
    }
  }
#endif

/* forget about this, too early??? JMI
  // grace notes cannot be cross staff
  if (! chordFirstNote->getNoteIsAGraceNote ()) { // JMI
    // register the chord as non cross staff
    fCurrentChordStaffNumber =
      chordFirstNoteDirectUpLinkToMeasure->
        fetchMeasureUpLinkToStaff ()->
          getStaffNumber ();
  }
    */

  return chord;
}

//______________________________________________________________________________
/* JMI
void mxsr2msrTranslator::registerVoiceCurrentChordInMap (
  int        inputLineNumber,
  const S_msrVoice& voice,
  const S_msrChord& chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Registering chord '" <<
      chord->asString () <<
      "'" <<
      " as current chord in voice \"" <<
      voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

 // fVoicesCurrentChordMap [voice] = chord;
  fVoicesCurrentChordMap [
    std::make_pair (
      voice->
        getVoiceUpLinkToStaff ()->
          getStaffNumber (),
      voice->
        getVoiceNumber ())] =
      chord;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif
}
*/

//______________________________________________________________________________
/* JMI
void mxsr2msrTranslator::printVoicesCurrentChordMap ()
{
  gLogStream <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl <<
    "fVoicesCurrentChordMap contains " <<
    mfSingularOrPlural (
      fVoicesCurrentChordMap.size (), "element", "elements") <<
    ":";

  if (fVoicesCurrentChordMap.size ()) {
    gLogStream << std::endl;

    ++gIndenter;

//    std::map<S_msrVoice, S_msrChord>::const_iterator
    std::map<std::pair<int, int>, S_msrChord>::const_iterator
      iBegin = fVoicesCurrentChordMap.begin (),
      iEnd   = fVoicesCurrentChordMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      ++gIndenter;

      / * JMI
      const S_msrVoice& voice = (*i).first;
      S_msrChord chord = (*i).second;
    * /
      gLogStream <<
//        voice->getVoiceName () <<
        "staff " << (*i).first.first <<
        ", voice " <<  (*i).first.second <<
        ":" <<
        std::endl;

// * JMI
      S_msrChord chord = (*i).second;

      gLogStream <<
        chord;
// * /

      --gIndenter;

      if (++i == iEnd) break;

      gLogStream << std::endl;
    } // for

    --gIndenter;
  }

  gLogStream <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl;
}
*/

//______________________________________________________________________________
void mxsr2msrTranslator::finalizeCurrentChord (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Finalizing current chord START:" <<
      std::endl <<
      fCurrentChord <<
      std::endl <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentChord->
    finalizeChord (
      inputLineNumber);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Finalizing current chord END: " <<
      std::endl <<
      fCurrentChord <<
      std::endl <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // forget about the current chord
  fCurrentChord = nullptr;
}

//______________________________________________________________________________
void mxsr2msrTranslator::printCurrentChord ()
{
  gLogStream <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl;

  if (fCurrentChord) {
    gLogStream <<
      "fCurrentChord contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      fCurrentChord;

    --gIndenter;
  }

  else {
    gLogStream <<
     "fCurrentChord is null" <<
     std::endl;
  }

  gLogStream <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl;
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteArticulationsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's articulations if any from the first note to chord

  std::list<S_msrArticulation>
    noteArticulations =
      note->
        getNoteArticulations ();

  for (S_msrArticulation articulation : noteArticulations) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceArticulations ()) {
      gLogStream <<
        "Copying articulation '" <<
        articulation->getArticulationKind () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->appendArticulationToChord (articulation);
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteTechnicalsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's technicals if any from the first note to chord

  std::list<S_msrTechnical>
    noteTechnicals =
      note->
        getNoteTechnicals ();

  for (S_msrTechnical technical : noteTechnicals) {
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "Copying technical '" <<
      technical->getTechnicalKind () <<
      "' from note " << note->asString () <<
      " to chord" <<
      std::endl;
    }
#endif

    chord->appendTechnicalToChord (technical);
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteTechnicalWithIntegersToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's technicals if any from the first note to chord

  std::list<S_msrTechnicalWithInteger>
    noteTechnicalWithIntegers =
      note->
        getNoteTechnicalWithIntegers ();

  for (S_msrTechnicalWithInteger technicalWithInteger : noteTechnicalWithIntegers) {
#ifdef TRACING_IS_ENABLED
		if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
			gLogStream <<
				"Copying technical '" <<
				technicalWithInteger->getTechnicalWithIntegerKind () <<
				"' from note " << note->asString () <<
				" to chord" <<
				std::endl;
			}
#endif

    chord->appendTechnicalWithIntegerToChord (technicalWithInteger);
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteTechnicalWithFloatsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's technicals if any from the first note to chord

  std::list<S_msrTechnicalWithFloat>
    noteTechnicalWithFloats =
      note->
        getNoteTechnicalWithFloats ();

  for (S_msrTechnicalWithFloat technicalWithFloat : noteTechnicalWithFloats) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
      gLogStream <<
        "Copying technical '" <<
        technicalWithFloat->getTechnicalWithFloatKind () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->appendTechnicalWithFloatToChord (technicalWithFloat);
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteTechnicalWithStringsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's technicals if any from the first note to chord

  std::list<S_msrTechnicalWithString>
    noteTechnicalWithStrings =
      note->
        getNoteTechnicalWithStrings ();

  for (S_msrTechnicalWithString technicalWithString : noteTechnicalWithStrings) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
      gLogStream <<
        "Copying technical '" <<
        technicalWithString->getTechnicalWithStringKind () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->appendTechnicalWithStringToChord (technicalWithString);
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteOrnamentsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's ornaments if any from the first note to chord

  std::list<S_msrOrnament>
    noteOrnaments =
      note->
        getNoteOrnaments ();

  for (S_msrOrnament ornament : noteOrnaments) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceOrnaments ()) {
      gLogStream <<
        "Copying ornament '" <<
        msrOrnamentKindAsString (ornament->getOrnamentKind ()) <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendOrnamentToChord (ornament);
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteSpannersToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's spanners if any from the first note to chord

  std::list<S_msrSpanner>
    noteSpanners =
      note->
        getNoteSpanners ();

  for (S_msrSpanner spanner : noteSpanners) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSpanners ()) {
      gLogStream <<
        "Copying spanner '" <<
        spanner->getSpannerKind () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendSpannerToChord (spanner);
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteSingleTremoloToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's singleTremolo if any from the first note to chord

  S_msrSingleTremolo
    noteSingleTremolo =
      note->
        getNoteSingleTremolo ();

  if (noteSingleTremolo) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTremolos ()) {
      gLogStream <<
        "Copying singleTremolo '" <<
        noteSingleTremolo->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      setChordSingleTremolo (noteSingleTremolo);
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteDynamicsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's dynamics if any from the first note to chord

  std::list<S_msrDynamic>
    noteDynamics =
      note->
        getNoteDynamics ();

  std::list<S_msrDynamic>::const_iterator i;
  for (
    i=noteDynamics.begin ();
    i!=noteDynamics.end ();
    ++i
  ) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceDynamics ()) {
      gLogStream <<
        "Copying dynamics '" <<
        msrDynamicKindAsString ((*i)->getDynamicKind ()) <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendDynamicToChord ((*i));
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteOtherDynamicsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's other dynamics if any from the first note to chord

  std::list<S_msrOtherDynamic>
    noteOtherDynamics =
      note->
        getNoteOtherDynamics ();

  std::list<S_msrOtherDynamic>::const_iterator i;
  for (
    i=noteOtherDynamics.begin ();
    i!=noteOtherDynamics.end ();
    ++i
  ) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceDynamics ()) {
      gLogStream <<
        "Copying other dynamics '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendOtherDynamicToChord ((*i));
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteWordsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's words if any from the first note to chord

  std::list<S_msrWords>
    noteWords =
      note->
        getNoteWords ();

  std::list<S_msrWords>::const_iterator i;
  for (
    i=noteWords.begin ();
    i!=noteWords.end ();
    ++i
  ) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWords ()) {
      gLogStream <<
        "Copying words '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendWordsToChord ((*i));
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteStemToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's stems if any from the first note to chord

  S_msrStem
    noteStem =
      note->
        getNoteStem ();

  if (noteStem) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceStems ()) {
      gLogStream <<
        "Copying stem '" <<
        noteStem->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendStemToChord (noteStem);
  }
}

/*
//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteBeamsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's beams if any from the first note to chord

  std::list<S_msrBeam>
    noteBeams =
      note->
        getNoteBeams ();

  std::list<S_msrBeam>::const_iterator i;
  for (
    i=noteBeams.begin ();
    i!=noteBeams.end ();
    ++i
  ) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceBeams ()) {
      gLogStream <<
        "Copying beam '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendBeamToChord ((*i));
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "==> AFTER copying beams to chord:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      chord <<
      std::endl;

    --gIndenter;
  }
#endif
}
*/

void mxsr2msrTranslator::appendNoteBeamsLinksToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // append link of note's beams if any from the first note to chord

  std::list<S_msrBeam>
    noteBeams =
      note->
        getNoteBeams ();

  std::list<S_msrBeam>::const_iterator i;
  for (
    i=noteBeams.begin ();
    i!=noteBeams.end ();
    ++i
  ) {
    S_msrBeam beam = (*i);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceBeams ()) {
      gLogStream <<
        "Adding beam link of '" <<
        beam->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    // create the beam link
    S_msrChordBeamLink
      chordBeamLink =
        msrChordBeamLink::create (
          chord->getInputLineNumber (),
          beam,
          chord);

    // append it in the chord
    chord->
      appendChordBeamLinkToChord (chordBeamLink);
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "==> AFTER appending note's beams links to chord:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      chord <<
      std::endl;

    --gIndenter;
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteTieToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's tie if any from the first note to chord

  S_msrTie
    noteTie =
      note->
        getNoteTie ();

  if (noteTie) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTies ()) {
      gLogStream <<
        "Appending tie '" <<
        noteTie->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendTieToChord (noteTie);
  }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTies ()) {
    gLogStream <<
      "==> AFTER appending tie to chord:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      chord <<
      std::endl;

    --gIndenter;
  }
#endif
}

/*
//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteSlursToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's slurs if any from the first note to chord

  std::list<S_msrSlur>
    noteSlurs =
      note->
        getNoteSlurs ();

  std::list<S_msrSlur>::const_iterator i;
  for (
    i=noteSlurs.begin ();
    i!=noteSlurs.end ();
    ++i
  ) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSlurs ()) {
      gLogStream <<
        "Copying slur '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendSlurToChord ((*i));
  } // for
}
*/

//______________________________________________________________________________
void mxsr2msrTranslator::appendNoteSlursLinksToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // append link of note's slurs if any from the first note to chord

  std::list<S_msrSlur>
    noteSlurs =
      note->
        getNoteSlurs ();

  std::list<S_msrSlur>::const_iterator i;
  for (
    i=noteSlurs.begin ();
    i!=noteSlurs.end ();
    ++i
  ) {
    S_msrSlur slur = (*i);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSlurs ()) {
      gLogStream <<
        "Adding slur link of '" <<
        slur->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    // create the slur link
    S_msrChordSlurLink
      chordSlurLink =
        msrChordSlurLink::create (
          chord->getInputLineNumber (),
          slur,
          chord);

    // append it in the chord
    chord->
      appendChordSlurLinkToChord (chordSlurLink);
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "==> AFTER appending note's slurs links to chord:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      chord <<
      std::endl;

    --gIndenter;
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteLigaturesToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's ligatures if any from the first note to chord

  std::list<S_msrLigature>
    noteLigatures =
      note->
        getNoteLigatures ();

  for (S_msrLigature ligature : noteLigatures) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLigatures ()) {
      gLogStream <<
        "Copying ligature '" <<
        ligature->getLigatureKind () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendLigatureToChord (ligature);
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNotePedalsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's pedals if any from the first note to chord

  std::list<S_msrPedal>
    notePedals =
      note->
        getNotePedals ();

  std::list<S_msrPedal>::const_iterator i;
  for (S_msrPedal pedal : notePedals) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTracePedals ()) {
      gLogStream <<
        "Copying pedal '" <<
        pedal->asString () <<
        "' from note " << note->asString () <<
        " to chord" << chord->asString () <<
        std::endl;
    }
#endif

    chord->
      appendPedalToChord ((*i));
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteSlashesToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's slashes if any from the first note to chord

  std::list<S_msrSlash>
    noteSlashes =
      note->
        getNoteSlashes ();

  std::list<S_msrSlash>::const_iterator i;
  for (
    i=noteSlashes.begin ();
    i!=noteSlashes.end ();
    ++i
  ) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSlashes ()) {
      gLogStream <<
        "Copying slash '" <<
        (*i)->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendSlashToChord ((*i));
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteWedgesToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's wedges if any from the first note to chord

  std::list<S_msrWedge>
    noteWedges =
      note->
        getNoteWedges ();

  std::list<S_msrWedge>::const_iterator i;
  for (
    i=noteWedges.begin ();
    i!=noteWedges.end ();
    ++i
  ) {
    S_msrWedge wedge = (*i);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWedges ()) {
      gLogStream <<
        "Copying wedges '" <<
        msrWedgeKindAsString (wedge->getWedgeKind ()) <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendWedgeToChord ((*i));
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteSegnosToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's segnos if any from the first note to chord

  std::list<S_msrSegno>
    noteSegnos =
      note->
        getNoteSegnos ();

  std::list<S_msrSegno>::const_iterator i;
  for (
    i=noteSegnos.begin ();
    i!=noteSegnos.end ();
    ++i
  ) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegnos ()) {
      gLogStream <<
        "Copying segno '" <<
        (*i)->asShortString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendSegnoToChord ((*i));
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteDalSegnosToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's dal segnos if any from the first note to chord

  std::list<S_msrDalSegno>
    noteDalSegnos =
      note->
        getNoteDalSegnos ();

  std::list<S_msrDalSegno>::const_iterator i;
  for (
    i=noteDalSegnos.begin ();
    i!=noteDalSegnos.end ();
    ++i
  ) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceDalSegnos ()) {
      gLogStream <<
        "Copying dal degno '" <<
        (*i)->asShortString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendDalSegnoToChord ((*i));
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteCodasToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's codas if any from the first note to chord

  std::list<S_msrCoda>
    noteCodas =
      note->
        getNoteCodas ();

  std::list<S_msrCoda>::const_iterator i;
  for (
    i=noteCodas.begin ();
    i!=noteCodas.end ();
    ++i
  ) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceCodas ()) {
      gLogStream <<
        "Copying coda '" <<
        (*i)->asShortString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      appendCodaToChord ((*i));
  } // for
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteOctaveShiftToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's wedges if any from the first note to chord

  S_msrOctaveShift
    noteOctaveShift =
      note->
        getNoteOctaveShift ();

  if (noteOctaveShift) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceOctaveShifts ()) {
      gLogStream <<
        "Copying octave shift '" <<
        noteOctaveShift->asString () <<
        "' from note " << note->asString () <<
        " to chord" <<
        std::endl;
    }
#endif

    chord->
      setChordOctaveShift (noteOctaveShift);
  }
}

/*
//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteGraceNotesGroupsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  S_msrGraceNotesGroup
    graceNotesGroupBefore =
      note->
        getNoteGraceNotesGroupBefore ();

  if (graceNotesGroupBefore) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Copying grace notes group before '" <<
        graceNotesGroupBefore->asShortString () <<
        "' from note " << note->asString () <<
        " to chord '" << chord->asString () <<
        "'" <<
        std::endl;
    }
#endif

    chord->
      setChordGraceNotesGroupBefore (graceNotesGroupBefore);
  }

  S_msrGraceNotesGroup
    graceNotesGroupAfter =
      note->
        getNoteGraceNotesGroupAfter ();

  if (graceNotesGroupAfter) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Copying grace notes group after '" <<
        graceNotesGroupAfter->asShortString () <<
        "' from note " << note->asString () <<
        " to chord '" << chord->asString () <<
        "'" <<
        std::endl;
    }
#endif

    chord->
      setChordGraceNotesGroupAfter (graceNotesGroupAfter);
  }
}
*/

//______________________________________________________________________________
void mxsr2msrTranslator::addNoteGraceNotesGroupsLinksToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // add link of note's grace notes groups if any to chord

  S_msrGraceNotesGroup
    graceNotesGroupBefore =
      note->
        getNoteGraceNotesGroupBefore ();

  if (graceNotesGroupBefore) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Adding grace notes group link before '" <<
        graceNotesGroupBefore->asShortString () <<
        "' from note " << note->asString () <<
        " to chord '" << chord->asString () <<
        "'" <<
        std::endl;
    }
#endif

    // create the grace notes group link
    S_msrChordGraceNotesGroupLink
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          chord->getInputLineNumber (),
          graceNotesGroupBefore,
          chord);

    // register it in the chord
    chord->
      setChordGraceNotesGroupLinkBefore (
        note->getInputLineNumber (), // JMI ???
        chordChordGraceNotesGroupLink);
  }

  S_msrGraceNotesGroup
    graceNotesGroupAfter =
      note->
        getNoteGraceNotesGroupAfter ();

  if (graceNotesGroupAfter) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Adding grace notes group link after '" <<
        graceNotesGroupAfter->asShortString () <<
        "' from note " << note->asString () <<
        " to chord '" << chord->asString () <<
        "'" <<
        std::endl;
    }
#endif

    // create the grace notes group link
    S_msrChordGraceNotesGroupLink
      chordChordGraceNotesGroupLink =
        msrChordGraceNotesGroupLink::create (
          chord->getInputLineNumber (),
          graceNotesGroupAfter,
          chord);

    // register it in the chord
    chord->
      setChordGraceNotesGroupLinkAfter (
        note->getInputLineNumber (), // JMI ???
        chordChordGraceNotesGroupLink);
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteHarmoniesToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's harmony if any from the first note to chord

  const std::list<S_msrHarmony>&
    noteHarmoniesList =
      note->getNoteHarmoniesList ();

  if (noteHarmoniesList.size ()) {
    std::list<S_msrHarmony>::const_iterator i;
    for (i=noteHarmoniesList.begin (); i!=noteHarmoniesList.end (); ++i) {
      S_msrHarmony harmony = (*i);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
        gLogStream <<
          "Copying harmony '" <<
          harmony->asString () <<
          "' from note " << note->asString () <<
          " to chord '" << chord->asString () <<
          "'" <<
          std::endl;
      }
#endif

      chord->
        appendHarmonyToChord (harmony);

    } // for
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::copyNoteElementsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's articulations if any to the chord
  copyNoteArticulationsToChord (note, chord);

  // copy note's technicals if any to the chord
  copyNoteTechnicalsToChord (note, chord);
  copyNoteTechnicalWithIntegersToChord (note, chord);
  copyNoteTechnicalWithFloatsToChord (note, chord);
  copyNoteTechnicalWithStringsToChord (note, chord);

  // copy note's ornaments if any to the chord
  copyNoteOrnamentsToChord (note, chord);

  // copy note's spanners if any to the chord
  copyNoteSpannersToChord (note, chord);

  // copy note's single tremolo if any to the chord
  copyNoteSingleTremoloToChord (note, chord);

  // copy note's dynamics if any to the chord
  copyNoteDynamicsToChord (note, chord);

  // copy note's other dynamics if any to the chord
  copyNoteOtherDynamicsToChord (note, chord);

  // copy note's words if any to the chord
  copyNoteWordsToChord (note, chord);

  // copy note's stem if any to the chord
  copyNoteStemToChord (note, chord);

  // copy note's beams if any to the chord
//  copyNoteBeamsToChord (note, chord);
  appendNoteBeamsLinksToChord (note, chord);

  // copy note's ties if any to the chord
  copyNoteTieToChord (note, chord);

  // copy note's slurs if any to the chord
// JMI  copyNoteSlursToChord (note, chord);
  appendNoteSlursLinksToChord (note, chord);

  // copy note's ligatures if any to the chord
  copyNoteLigaturesToChord (note, chord);

  // copy note's pedals if any to the chord
  copyNotePedalsToChord (note, chord);

  // copy note's slashes if any to the chord
  copyNoteSlashesToChord (note, chord);

  // copy note's wedges if any to the chord
  copyNoteWedgesToChord (note, chord);

  // copy note's segnos if any to the chord
  copyNoteSegnosToChord (note, chord);

  // copy note's del segnos if any to the chord
  copyNoteDalSegnosToChord (note, chord);

  // copy note's codas if any to the chord
  copyNoteCodasToChord (note, chord);

  // copy note's octave shift if any to the chord
  copyNoteOctaveShiftToChord (note, chord);

  // copy note's grace notes groups if any to the chord
//  copyNoteGraceNotesGroupsToChord (note, chord);
  addNoteGraceNotesGroupsLinksToChord (note, chord);

  // copy note's harmony if any to the chord
  copyNoteHarmoniesToChord (note, chord);
}

//______________________________________________________________________________
void mxsr2msrTranslator::createTupletWithItsFirstNoteAndPushItToTupletsStack (
  const S_msrNote& firstNote)
{
  // firstNote is the first tuplet note,
  // and is currently at the end of the voice

  int firstNoteInputLineNumber =
    firstNote->getInputLineNumber ();

  // create a tuplet
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Creating a '" <<
      fCurrentNoteActualNotes <<
      '/' <<
      fCurrentNoteNormalNotes <<
      "' tuplet with first note " <<
      firstNote->
        asShortString () <<
      std::endl;
  }
#endif

  // account for note duration
  Rational
    memberNotesSoundingWholeNotes =
      firstNote->getMeasureElementSoundingWholeNotes ();

  Rational
    memberNotesDisplayWholeNotes =
      firstNote->getNoteDisplayWholeNotes ();

  S_msrTuplet
    tuplet =
      msrTuplet::create (
        firstNoteInputLineNumber,
        fCurrentMeasureNumber,
        fCurrentTupletNumber,
        fCurrentTupletBracketKind,
        fCurrentTupletLineShapeKind,
        fCurrentTupletShowNumberKind,
        fCurrentTupletShowTypeKind,
				msrTupletFactor (
					fCurrentNoteActualNotes,
					fCurrentNoteNormalNotes),
        memberNotesSoundingWholeNotes,
        memberNotesDisplayWholeNotes);

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        firstNoteInputLineNumber,
        fCurrentStaffNumberToInsertInto, // fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // add note as first note of the std::stack top tuplet
  tuplet->
    appendNoteToTuplet (
      firstNote,
      currentVoice);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    // only after appendNoteToTuplet() has set the note's uplink to tuplet
    gLogStream <<
      "Adding first note " <<
      firstNote->
        asShortString () <<
      " to tuplet '" <<
      tuplet->asString () <<
       "'" <<
      std::endl;
  }
#endif

  // set tuplet's uplink to tuplet
  if (fTupletsStack.size ()) {
    tuplet->
      setTupletDirectUpLinkToTuplet (
        fTupletsStack.front ());
  }

  // register tuplet in this visitor
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "++> pushing tuplet '" <<
      tuplet->asString () <<
      "' to tuplets std::stack" <<
      std::endl;
  }
#endif

  fTupletsStack.push_front (tuplet);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## createTupletWithItsFirstNoteAndPushItToTupletsStack() 1");
  }
#endif

/* JMI
  // set note displayed divisions
  firstNote->
    applyTupletMemberDisplayFactor (
      fCurrentNoteActualNotes,
      fCurrentNoteNormalNotes);
  */


  // keep track of current tuplet in the current voice,
  // in case we learn later by <chord/> in the next note
  // that it is actually the first note of a chord ?? JMI XXL

  // register tuplet as last one found in this voice
  // for chords in tuplets handling
// JMI  fLastHandledTupletInVoiceMap [currentVoice] = tuplet;
  fLastHandledTupletInVoiceMap [
    std::make_pair (
      fCurrentMusicXMLStaffNumber, // JMI fCurrentStaffNumberToInsertInto,
      fCurrentMusicXMLVoiceNumber)
    ] =
    tuplet;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
    displayLastHandledTupletInVoiceMap (
      "############## createTupletWithItsFirstNoteAndPushItToTupletsStack() 2");
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::finalizeTupletAndPopItFromTupletsStack (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "mxsr2msrTranslator::finalizeTupletAndPopItFromTupletsStack(), " <<
      "line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## Before  finalizeTupletAndPopItFromTupletsStack()");
  }
#endif

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto, // fCurrentMusicXMLStaffNumber ??? JMI
        fCurrentMusicXMLVoiceNumber);

  // get tuplet from top of tuplet std::stack
  S_msrTuplet
    tuplet =
      fTupletsStack.front ();

/*  // set note displayed divisions JMI
  note->
    applyTupletMemberDisplayFactor (
      fCurrentNoteActualNotes,
      fCurrentNoteNormalNotes);
*/

/* JMI
  // add lastNote to the tuplet
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "==> adding last note " << lastNote->asString () <<
      " to tuplets std::stack top " <<
      fTupletsStack.front ()->asString () <<
      std::endl;
  }
#endif

  tuplet->appendNoteToTuplet (lastNote);
*/

  // pop from the tuplets std::stack
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Popping tuplet:" <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      tuplet;
    --gIndenter;
    gLogStream <<
      " from tuplets std::stack" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fTupletsStack.pop_front ();

  if (fTupletsStack.size ()) {
    // tuplet is a nested tuplet
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "=== adding nested tuplet '" <<
      tuplet->asString () <<
        "' to current std::stack top tuplet '" <<
      fTupletsStack.front ()->asString () <<
      "'" <<
      ", line " << inputLineNumber <<
      std::endl;
    }
#endif

    fTupletsStack.front ()->
      appendTupletToTuplet (tuplet);
  }

  else {
    // tuplet is a top level tuplet
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "Appending top level tuplet " <<
      tuplet->asString () <<
      " to voice \"" <<
      currentVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
    }
#endif

    currentVoice->
      appendTupletToVoice (tuplet);

/* JMI
    // finalize the tuplet
    tuplet->
      finalizeTuplet (
        inputLineNumber);
*/

 // JMI BAD HERE   // the tuplet stop is not to be handled later
 //   fCurrentATupletStopIsPending = false;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## After  finalizeTupletAndPopItFromTupletsStack()");
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentArticulationsToNote (
  const S_msrNote& note)
{
  // attach the current articulations if any to the note
  if (fCurrentArticulations.size ()) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceArticulations ()) {
      gLogStream <<
        "Attaching current articulations to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fCurrentArticulations.size ()) {
      S_msrArticulation
        art =
          fCurrentArticulations.front();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Attaching articulation '" <<
          art->getArticulationKind () <<
          "' to note " << note->asString () <<
          std::endl;
      }
#endif

      note->
        appendArticulationToNote (art);

      // forget about this articulation
      fCurrentArticulations.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentTechnicalsToNote (
  const S_msrNote& note)
{
  // attach the current technicals if any to the note
  if (fCurrentTechnicalsList.size ()) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
      gLogStream <<
        "Attaching current technicals to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fCurrentTechnicalsList.size ()) {
      S_msrTechnical
        tech =
          fCurrentTechnicalsList.front();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
        gLogStream <<
          "Attaching technical '" <<
          tech->asString () <<
          "' to note " << note->asString () <<
          std::endl;
      }
#endif

      note->
        appendTechnicalToNote (tech);

      // forget about this technical
      fCurrentTechnicalsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentTechnicalWithIntegersToNote (
  const S_msrNote& note)
{
  // attach the current technicals if any to the note
  if (fCurrentTechnicalWithIntegersList.size ()) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
      gLogStream <<
        "Attaching current technical with integers to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fCurrentTechnicalWithIntegersList.size ()) {
      S_msrTechnicalWithInteger
        tech =
          fCurrentTechnicalWithIntegersList.front();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
        gLogStream <<
          "Attaching technical with integer '" <<
          tech->asString () <<
          "' to note " << note->asString () <<
          std::endl;
      }
#endif

      note->
        appendTechnicalWithIntegerToNote (tech);

      // forget about this technical
      fCurrentTechnicalWithIntegersList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentTechnicalWithFloatsToNote (
  const S_msrNote& note)
{
  // attach the current technicals if any to the note
  if (fCurrentTechnicalWithFloatsList.size ()) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
      gLogStream <<
        "Attaching current technical with floats to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fCurrentTechnicalWithFloatsList.size ()) {
      S_msrTechnicalWithFloat
        tech =
          fCurrentTechnicalWithFloatsList.front();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
        gLogStream <<
          "Attaching technical with integer '" <<
          tech->asString () <<
          "' to note " << note->asString () <<
          std::endl;
      }
#endif

      note->
        appendTechnicalWithFloatToNote (tech);

      // forget about this technical
      fCurrentTechnicalWithFloatsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentTechnicalWithStringsToNote (
  const S_msrNote& note)
{
  // attach the current technicals if any to the note
  if (fCurrentTechnicalWithStringsList.size ()) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
      gLogStream <<
        "Attaching current technical with strings to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fCurrentTechnicalWithStringsList.size ()) {
      S_msrTechnicalWithString
        tech =
          fCurrentTechnicalWithStringsList.front();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
        gLogStream <<
          "Attaching technical with std::string '" <<
          tech->asString () <<
          "' to note " << note->asString () <<
          std::endl;
      }
#endif

      note->
        appendTechnicalWithStringToNote (tech);

      // forget about this technical
      fCurrentTechnicalWithStringsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentOrnamentsToNote (
  const S_msrNote& note)
{
  // attach the current ornaments if any to the note
  if (fCurrentOrnamentsList.size ()) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceOrnaments ()) {
      gLogStream <<
        "Attaching current ornaments to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fCurrentOrnamentsList.size ()) {
      S_msrOrnament
        ornament =
          fCurrentOrnamentsList.front();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Attaching ornament '" <<
          msrOrnamentKindAsString (ornament->getOrnamentKind ()) <<
          "' to note " << note->asString () <<
          std::endl;
      }
#endif

      note->
        appendOrnamentToNote (ornament);

      // forget about this ornament
      fCurrentOrnamentsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentSpannersToNote (
  const S_msrNote& note)
{
  // attach the current spanners if any to the note
  if (fCurrentSpannersList.size ()) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSpanners ()) {
      gLogStream <<
        "Attaching current spanners to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    Bool doHandleSpanner (true);
    Bool spannerStopMetForThisNote (false);

    S_msrSpanner delayedStopSpanner;

    while (fCurrentSpannersList.size ()) {
      S_msrSpanner
        spanner =
          fCurrentSpannersList.front();

      switch (spanner->getSpannerKind ()) {
        case msrSpannerKind::kSpannerDashes: // JMI
          switch (spanner->getSpannerTypeKind ()) {
            case msrSpannerTypeKind::kSpannerTypeStart:
              spannerStopMetForThisNote = true;
              break;
            case msrSpannerTypeKind::kSpannerTypeStop:
              doHandleSpanner =
                ! spannerStopMetForThisNote;
              break;
            case msrSpannerTypeKind::kSpannerTypeContinue:
              break;
            case msrSpannerTypeKind::kSpannerType_NO_:
              // JMI ???
              break;
          } // switch
          break;

        case msrSpannerKind::kSpannerWavyLine:
          switch (spanner->getSpannerTypeKind ()) {
            case msrSpannerTypeKind::kSpannerTypeStart:
              spannerStopMetForThisNote = true;
              break;
            case msrSpannerTypeKind::kSpannerTypeStop:
              doHandleSpanner =
                ! spannerStopMetForThisNote;
              break;
            case msrSpannerTypeKind::kSpannerTypeContinue:
              break;
            case msrSpannerTypeKind::kSpannerType_NO_:
              // JMI ???
              break;
          } // switch
          break;
      } // switch

      if (doHandleSpanner) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceSpanners ()) {
          gLogStream <<
            "Attaching spanner '" <<
            spanner->asShortString () <<
            "' to note " << note->asString () <<
            std::endl;
        }
#endif

        note->
          appendSpannerToNote (spanner);

        // set spanner note upLink
        spanner->
          setSpannerUpLinkToNote (note);

        // forget about this spanner
        fCurrentSpannersList.pop_front ();
      }

      else { // check it is the same spanner kind JMI
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceSpanners ()) {
          gLogStream <<
            "Spanner start amd stop on one and the same note' to note " <<
            note->asString () <<
            ", delaying 'stop' until next note" <<
            std::endl;
        }
#endif

        // keep track of this stop spanner
        delayedStopSpanner = spanner;

        // forget about this spanner to avoid infinite loop
        fCurrentSpannersList.pop_front ();
      }
    } // while

    // append delayed stop spanner if any again to the std::list
    if (delayedStopSpanner) {
      fCurrentSpannersList.push_back (
        delayedStopSpanner);
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentSingleTremoloToNote (
  const S_msrNote& note)
{
  // attach the current singleTremolo if any to the note
  if (fCurrentSingleTremolo) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "Attaching current singleTremolo to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    note->
      setNoteSingleTremolo (fCurrentSingleTremolo);

    // set single tremolo graphic duration kind
    fCurrentSingleTremolo->
      setSingleTremoloGraphicDurationKind (
        note->
          getNoteGraphicDurationKind ());

    // forget about this single tremolo
    fCurrentSingleTremolo = nullptr;
  }
}

//______________________________________________________________________________
/* JMI
void mxsr2msrTranslator::attachCurrentArticulationsToChord ( // JMI
  const S_msrChord& chord)
{
  if (fCurrentArticulations.size ()) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceArticulations ()) {
      gLogStream <<
        "Attaching current articulations to chord " <<
        chord->asString () <<
        std::endl;
    }
#endif

    std::list<S_msrArticulation>::const_iterator i;
    for (
      i=fCurrentArticulations.begin ();
      i!=fCurrentArticulations.end ();
      ++i
  ) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceArticulations ()) {
        gLogStream <<
          "Attaching articulation " <<  (*i) <<
          " to chord " << chord <<
          std::endl;
      }
#endif

      chord->
        appendArticulationToChord ((*i));
      } // for
  }
}
*/

/*
//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentOrnamentsToChord ( // JMI
  const S_msrChord& chord)
{
  if (fCurrentOrnamentsList.size ()) {

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceOrnaments ()) {
      gLogStream <<
        "Attaching current ornaments to chord " <<
        chord->asString () <<
        std::endl;
    }
#endif

    std::list<S_msrOrnament>::const_iterator i;
    for (
      i=fCurrentOrnamentsList.begin ();
      i!=fCurrentOrnamentsList.end ();
      ++i
  ) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceOrnaments ()) {
        gLogStream <<
          "Attaching ornament " <<  (*i) << " to chord " <<
          chord <<
          std::endl;
      }
#endif

      chord->
        appendOrnamentToChord ((*i));
      } // for
  }
}
*/

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingTemposToVoice (
  const S_msrVoice& voice)
{
  // attach the pending tempos if any to the voice
  if (fPendingTemposList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTempos ()) {
      gLogStream <<
        "Attaching pending tempos to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif

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

void mxsr2msrTranslator::attachPendingTemposToPart (
  const S_msrPart& part)
{
  // attach the pending tempos if any to the voice
  if (fPendingTemposList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTempos ()) {
      gLogStream <<
        "Attaching pending tempos to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;
    }
#endif

    while (fPendingTemposList.size ()) {
      S_msrTempo
        tempo =
          fPendingTemposList.front ();

      part->
        appendTempoToPart (tempo);

      fPendingTemposList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachPendingBarLinesToVoice (
  const S_msrVoice& voice)
{
  // attach the pending barlines if any to the voice
  if (fPendingBarLinesList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceBarLines ()) {
      gLogStream <<
        "Attaching pending barlines to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif

    while (fPendingBarLinesList.size ()) {
      S_msrBarLine
        barLine =
          fPendingBarLinesList.front ();

//       fCurrentPart->
//         appendBarLineToPart (barLine); // JMI ??? v0.9.63

      voice->
        appendBarLineToVoice (barLine); // JMIJMIJMI

      fPendingBarLinesList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachPendingBarLinesToPart (
  const S_msrPart& part)
{
  // attach the pending barlines if any to the voice
  if (fPendingBarLinesList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceBarLines ()) {
      gLogStream <<
        "Attaching pending barlines to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;
    }
#endif

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
}

void mxsr2msrTranslator::attachPendingRehearsalMarksToVoice (
  const S_msrVoice& voice)
{
 // attach the pending rehearsals if any to the note
  if (fPendingRehearsalMarksList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRehearsalMarks ()) {
      gLogStream <<
        "Attaching pending rehearsals to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif

    while (fPendingRehearsalMarksList.size ()) {
      S_msrRehearsalMark
        rehearsalMark =
          fPendingRehearsalMarksList.front ();

      voice->
        appendRehearsalMarkToVoice (rehearsalMark);

      fPendingRehearsalMarksList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachPendingRehearsalMarksToPart (
  const S_msrPart& part)
{
 // attach the pending rehearsals if any to the note
  if (fPendingRehearsalMarksList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceRehearsalMarks ()) {
      gLogStream <<
        "Attaching pending rehearsals to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;
    }
#endif

    while (fPendingRehearsalMarksList.size ()) {
      S_msrRehearsalMark
        rehearsalMark =
          fPendingRehearsalMarksList.front ();

      part->
        appendRehearsalMarkToPart (rehearsalMark);

      fPendingRehearsalMarksList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachLineBreaksToVoice (
  const S_msrVoice& voice)
{
 // attach the pending line breaks if any to the note
  if (fPendingLineBreaksList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
      gLogStream <<
        "Attaching pending line breaks to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif

    while (fPendingLineBreaksList.size ()) {
      S_msrLineBreak
        lineBreak =
          fPendingLineBreaksList.front ();

      // append it to the voice
      voice->
        appendLineBreakToVoice (lineBreak);

      // remove it from the std::list
      fPendingLineBreaksList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachLineBreaksToPart (
  const S_msrPart& part)
{
 // attach the pending line breaks if any to the note
  if (fPendingLineBreaksList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
      gLogStream <<
        "Attaching pending line breaks to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;
    }
#endif

    while (fPendingLineBreaksList.size ()) {
      S_msrLineBreak
        lineBreak =
          fPendingLineBreaksList.front ();

      // append it to the voice
      part->
        appendLineBreakToPart (lineBreak);

      // remove it from the std::list
      fPendingLineBreaksList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachPageBreaksToVoice (
  const S_msrVoice& voice)
{
 // attach the pending page breaks if any to the note
  if (fPendingPageBreaksList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
      gLogStream <<
        "Attaching pending page breaks to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif

    while (fPendingPageBreaksList.size ()) {
      S_msrPageBreak
        pageBreak =
          fPendingPageBreaksList.front ();

      // append it to the voice
      voice->
        appendPageBreakToVoice (pageBreak);

      // remove it from the std::list
      fPendingPageBreaksList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachPageBreaksToPart (
  const S_msrPart& part)
{
 // attach the pending page breaks if any to the note
  if (fPendingPageBreaksList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
      gLogStream <<
        "Attaching pending page breaks to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;
    }
#endif

    while (fPendingPageBreaksList.size ()) {
      S_msrPageBreak
        pageBreak =
          fPendingPageBreaksList.front ();

      // append it to the voice
      part->
        appendPageBreakToPart (pageBreak);

      // remove it from the std::list
      fPendingPageBreaksList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingSegnosToNote (
  const S_msrNote& note)
{
 // attach the pending segno if any to the note
  if (fPendingSegnosList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSegnos ()) {
      gLogStream <<
        "Attaching pending segno to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendingSegnosList.size ()) {
      S_msrSegno
        segno =
          fPendingSegnosList.front ();

      note->
        appendSegnoToNote (segno);

      fPendingSegnosList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingDalSegnosToNote (
  const S_msrNote& note)
{
 // attach the pending dal segno if any to the note
  if (fPendingDalSegnosList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceDalSegnos ()) {
      gLogStream <<
        "Attaching pending dal segno to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendingDalSegnosList.size ()) {
      S_msrDalSegno
        dalSegno =
          fPendingDalSegnosList.front ();

      note->
        appendDalSegnoToNote (dalSegno);

      fPendingDalSegnosList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachPendingDalSegnosToChord (
  const S_msrChord& chord)
{
 // attach the pending dal segno if any to the chord
  if (fPendingDalSegnosList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceDalSegnos ()) {
      gLogStream <<
        "Attaching pending dal segno to chord " <<
        chord->asString () <<
        std::endl;
    }
#endif

    while (fPendingDalSegnosList.size ()) {
      S_msrDalSegno
        dalSegno =
          fPendingDalSegnosList.front ();

      chord->
        appendDalSegnoToChord (dalSegno);

      fPendingDalSegnosList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingCodasToNote (
  const S_msrNote& note)
{
 // attach the pending coda if any to the note
  if (fPendingCodasList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceCodas ()) {
      gLogStream <<
        "Attaching pending codas to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendingCodasList.size ()) {
      S_msrCoda
        coda =
          fPendingCodasList.front ();

      note->
        appendCodaToNote (coda);

      fPendingCodasList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingCrescDecrescsToNote (
  const S_msrNote& note)
{
 // attach the pending crescDecresc if any to the note
  if (fPendinCrescDecrescsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceCrescDecrescs ()) {
      gLogStream <<
        "Attaching pending crescDecresc to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendinCrescDecrescsList.size ()) {
      S_msrCrescDecresc
        crescDecresc =
          fPendinCrescDecrescsList.front ();

      note->
        appendCrescDecrescToNote (crescDecresc);

      fPendinCrescDecrescsList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingEyeGlassesToNote (
  const S_msrNote& note)
{
 // attach the pending eyeglasses if any to the note
  if (fPendingEyeGlassesList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceEyeGlasses ()) {
      gLogStream <<
        "Attaching pending eyeglasses to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendingEyeGlassesList.size ()) {
      S_msrEyeGlasses
        eyeGlasses =
          fPendingEyeGlassesList.front ();

      note->
        appendEyeGlassesToNote (eyeGlasses);

      fPendingEyeGlassesList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingDampsToNote (
  const S_msrNote& note)
{
 // attach the pending damps if any to the note
  if (fPendingDampsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceDamps ()) {
      gLogStream <<
        "Attaching pending damps to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendingDampsList.size ()) {
      S_msrDamp
        damp =
          fPendingDampsList.front ();

      note->
        appendDampToNote (damp);

      fPendingDampsList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingDampAllsToNote (
  const S_msrNote& note)
{
 // attach the pending damp alls if any to the note
  if (fPendingDampAllsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceDampAlls ()) {
      gLogStream <<
        "Attaching pending damp alls to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendingDampAllsList.size ()) {
      S_msrDampAll
        dampAll =
          fPendingDampAllsList.front ();

      note->
        appendDampAllToNote (dampAll);

      fPendingDampAllsList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingOctaveShiftsToNote (
  const S_msrNote& note)
{
 // attach the pending octave shifts if any to the note
  if (fPendingOctaveShiftsList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceOctaveShifts ()) {
      gLogStream <<
        "Attaching pending octave shifts to note " <<
        note->asString () <<
        std::endl;
    }
#endif

/* JMI
    // fetch the voice ??? JMI
    S_msrVoice
      voice =
        fetchVoiceFromCurrentPart (
          note->getInputLineNumber (),
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);
*/

    while (fPendingOctaveShiftsList.size ()) {
      S_msrOctaveShift
        octaveShift =
          fPendingOctaveShiftsList.front ();

      note->
        setNoteOctaveShift (octaveShift);

      fPendingOctaveShiftsList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingScordaturasToNote (
  const S_msrNote& note)
{
 // attach the pending scordatura if any to the note
  if (fPendingScordaturasList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceScordaturas ()) {
      gLogStream <<
        "Attaching pending scordaturas to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendingScordaturasList.size ()) {
      S_msrScordatura
        scordatura =
          fPendingScordaturasList.front ();

      note->
        appendScordaturaToNote (scordatura);

      fPendingScordaturasList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingDynamicsToNote (
  const S_msrNote& note)
{
 // attach the pending dynamics if any to the note
  if (fPendingDynamicxList.size ()) {
    Bool delayAttachment (false);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceDynamics ()) {
      gLogStream <<
        "Attaching pending dynamics to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gGlobalMxsr2msrOahGroup->getDelayRestsDynamics ()) {
        gLogStream <<
          "Delaying dynamics attached to a rest until next note" <<
          std::endl;

        delayAttachment = true;
      }

      else {
#ifdef TRACING_IS_ENABLED
       if (gGlobalTracingOahGroup->getTraceLyrics ()) {
          std::stringstream s;

          int numberOfDynamics = fPendingDynamicxList.size ();

          if (numberOfDynamics > 1) {
            s <<
              "there are " << numberOfDynamics << " dynamics";
          }
          else {
            s <<
              "there is 1 dynamics";
          }
          s <<
            " attached to a rest";

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            s.str ());
        }
#endif
      }
    }

    if (! delayAttachment) {
      while (fPendingDynamicxList.size ()) {
        S_msrDynamic
          dynamics =
            fPendingDynamicxList.front ();

        note->appendDynamicToNote (dynamics);
        fPendingDynamicxList.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingOtherDynamicsToNote (
  const S_msrNote& note)
{
 // attach the pending other dynamics if any to the note
  if (fPendingOtherDynamicxList.size ()) {
    Bool delayAttachment (false);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceDynamics ()) {
      gLogStream <<
        "Attaching pending other dynamics to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gGlobalMxsr2msrOahGroup->getDelayRestsDynamics ()) {
        gLogStream <<
          "Delaying dynamics attached to a rest until next note" <<
          std::endl;

        delayAttachment = true;
      }

      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceDynamics ()) {
          std::stringstream s;

          int numberOfOtherDynamics = fPendingOtherDynamicxList.size ();

          if (numberOfOtherDynamics > 1) {
            s <<
              "there are " << numberOfOtherDynamics << " other dynamics";
          }
          else {
            s <<
              "there is 1 other dynamics";
          }
          s <<
            " attached to a rest";

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            s.str ());
        }
#endif
      }
    }

    if (! delayAttachment) {
      while (fPendingOtherDynamicxList.size ()) {
        S_msrOtherDynamic
          otherDynamic =
            fPendingOtherDynamicxList.front ();

        note->appendOtherDynamicToNote (otherDynamic);
        fPendingOtherDynamicxList.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingWordsToNote (
  const S_msrNote& note)
{
  // attach the pending words if any to the note
  if (fPendingWordsList.size ()) {
    Bool delayAttachment;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWords ()) {
      gLogStream <<
        "Attaching pending words to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gGlobalMxsr2msrOahGroup->getDelayRestsWords ()) {
        gLogStream <<
          "Delaying word(s) attached to a rest until next note" <<
          std::endl;

        delayAttachment = true;
      }

      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceWords ()) {
          std::stringstream s;

          int numberOfWords = fPendingWordsList.size ();

          s <<
            mfSingularOrPluralWithoutNumber (
              numberOfWords, "There is", "There are") <<
            ' ' <<
            numberOfWords <<
            ' ' <<
            mfSingularOrPluralWithoutNumber (
              numberOfWords, "word", "words") <<
            " attached to a rest";

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            s.str ());
        }
#endif
      }
    }

    if (! delayAttachment) {
      while (fPendingWordsList.size ()) {
        S_msrWords
          words =
            fPendingWordsList.front ();

        note->appendWordsToNote (words);

        fPendingWordsList.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingBeamsToNote (
  const S_msrNote& note)
{
  // attach the pending beams if any to the note
  if (fPendingBeamsList.size ()) {
    Bool delayAttachment (false);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceBeams ()) {
      gLogStream <<
        "Attaching pending beams to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gGlobalMxsr2msrOahGroup->getDelayRestsBeams ()) {
        gLogStream <<
          "Delaying beam attached to a rest until next note" <<
          std::endl;

        delayAttachment = true;
      }

      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceBeams ()) {
          std::stringstream s;

          int numberOfBeams = fPendingBeamsList.size ();

          if (numberOfBeams > 1) {
            s <<
              "there are " << numberOfBeams << " beams";
          }
          else {
            s <<
              "there is 1 beam";
          }
          s <<
            " attached to a rest";

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            s.str ());
        }
#endif
      }
    }

    if (! delayAttachment) {
      while (fPendingBeamsList.size ()) {
        S_msrBeam
          beam =
            fPendingBeamsList.front ();

        note->appendBeamToNote (beam);
        fPendingBeamsList.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingSlursToNote (
  const S_msrNote& note)
{
  // attach the pending slurs if any to the note
  if (fPendingSlursList.size ()) {
    Bool delayAttachment (false);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSlurs ()) {
      gLogStream <<
        "Attaching pending slurs to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gGlobalMxsr2msrOahGroup->getDelayRestsSlurs ()) {
        gLogStream <<
          "Delaying slur attached to a rest until next note" <<
          std::endl;

        delayAttachment = true;
      }

      else {
        std::stringstream s;

        int numberOfSlurs = fPendingSlursList.size ();

        if (numberOfSlurs > 1) {
          s <<
            "there are " << numberOfSlurs << " slurs";
        }
        else {
          s <<
            "there is 1 slur";
        }
        s <<
          " attached to a rest";

        mxsr2msrWarning (
          gGlobalServiceRunData->getInputSourceName (),
          note->getInputLineNumber (),
          s.str ());
      }
    }

    if (! delayAttachment) {
      while (fPendingSlursList.size ()) {
        S_msrSlur
          slur =
            fPendingSlursList.front ();

        note->appendSlurToNote (slur);
        fPendingSlursList.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingLigaturesToNote (
  const S_msrNote& note)
{
  int numberOfLigatures =
    fPendingLigaturesList.size ();

  // attach the pending ligatures if any to the note
  if (numberOfLigatures) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLigatures ()) {
      gLogStream <<
        "Attaching " <<
        mfSingularOrPlural (
          numberOfLigatures,
          "ligature", "ligatures") <<
        " to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    Bool delayAttachment (false);

    if (fCurrentNoteIsARest) {
      if (gGlobalMxsr2msrOahGroup->getDelayRestsLigatures ()) {
        gLogStream <<
          "Delaying ligature attached to a rest until next note" <<
          std::endl;

        delayAttachment = true;
      }

      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceLigatures ()) {
          std::stringstream s;

          s <<
            mfSingularOrPluralWithoutNumber (
              numberOfLigatures,
              "there is", "there are") <<
              ' ' <<
            numberOfLigatures <<
              ' ' <<
            mfSingularOrPluralWithoutNumber (
              numberOfLigatures,
              "ligature", "ligatures") <<
            " attached to rest " <<
            note->asShortString ();

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            s.str ());
        }
#endif
      }
    }

    if (! delayAttachment) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceLigatures ()) {
          std::stringstream s;

          s <<
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
            note->asShortString ();

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            s.str ());
      }
#endif

      // append ligatures to note only if they belong to a suitable voice,
      // i.e. above goes to voice 1 or 3, and below to voice 2 or 4

      std::list<S_msrLigature>::iterator
        iBegin = fPendingLigaturesList.begin (),
        iEnd   = fPendingLigaturesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrLigature
          ligature = (*i);

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceLigatures ()) {
          gLogStream <<
            "--> ligature: " <<
            ligature->asString () <<
            std::endl;
        }
#endif

        // fetch ligatures placement kind
        msrPlacementKind
          ligaturePlacementKind =
            ligature->
              getLigaturePlacementKind ();

//         // fetch note's measure JMI v0.9.63
//         S_msrMeasure
//           noteMeasure =
//             note->
//               getNoteUpLinkToMeasure ();
//
//         // sanity check
//         mfAssert (
//           __FILE__, __LINE__,
//           noteMeasure != nullptr,
//           "attachPendingLigaturesToNote(): noteMeasure is null");
//
//         // fetch note's segment
//         S_msrSegment
//           noteSegment =
//             noteMeasure->
//               getMeasureUpLinkToSegment ();
//
//         // sanity check
//         mfAssert (
//           __FILE__, __LINE__,
//           noteSegment != nullptr,
//           "noteSegment is null");
//
//         // fetch note's voice
//         S_msrVoice
//           noteVoice =
//             noteSegment->
//               getSegmentUpLinkToVoice ();
//
//         // sanity check
//         mfAssert (
//           __FILE__, __LINE__,
//           noteVoice != nullptr,
//           "noteVoice is null");

        // handle ligature placement kind
        switch (ligaturePlacementKind) {
          case msrPlacementKind::kPlacement_NO_:
            // should not occur
            break;

          case msrPlacementKind::kPlacementAbove:
          case msrPlacementKind::kPlacementBelow:
#ifdef TRACING_IS_ENABLED
            if (gGlobalTracingOahGroup->getTraceLigatures ()) {
              gLogStream <<
                "Attaching pending ligature " <<
                ligature->asString () <<
                " to note '" <<
                note->asString () <<
//                 "' in voice \"" <<
//                 noteVoice->getVoiceName () <<
//                 "\"" <<
                ", line " << ligature->getInputLineNumber () <<
                std::endl;
            }
#endif

            note->appendLigatureToNote (ligature);
            break;
        } // switch

        i = fPendingLigaturesList.erase (i);

        if (++i == iEnd) break;
      } // for
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingPedalsToNote (
  const S_msrNote& note)
{
  // attach the pending pedals if any to the note
  if (fPendingPedalsList.size ()) {
    Bool delayAttachment (false);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTracePedals ()) {
      gLogStream <<
        "Attaching pending pedals to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gGlobalMxsr2msrOahGroup->getDelayRestsPedals ()) {
        gLogStream <<
          "Delaying pedal attached to a rest until next note" <<
          std::endl;

        delayAttachment = true;
      }

      else {
        std::stringstream s;

        int numberOfPedals = fPendingPedalsList.size ();

        if (numberOfPedals > 1) {
          s <<
            "there are " << numberOfPedals << " pedals";
        }
        else {
          s <<
            "there is 1 pedal";
        }
        s <<
          " attached to a rest";

        mxsr2msrWarning (
          gGlobalServiceRunData->getInputSourceName (),
          note->getInputLineNumber (),
          s.str ());
      }
    }

    if (! delayAttachment) {
      while (fPendingPedalsList.size ()) {
        S_msrPedal
          pedal =
            fPendingPedalsList.front ();

        note->appendPedalToNote (pedal);
        fPendingPedalsList.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingSlashesToNote (
  const S_msrNote& note)
{
  // attach the pending slashes if any to the note
  if (fPendingSlashesList.size ()) {
    Bool delayAttachment (false);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSlashes ()) {
      gLogStream <<
        "Attaching pending slashes to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gGlobalMxsr2msrOahGroup->getDelayRestsSlashes ()) {
        gLogStream <<
          "Delaying slash attached to a rest until next note" <<
      std::endl;

        delayAttachment = true;
      }

      else {
        std::stringstream s;

        int numberOfSlashes = fPendingSlashesList.size ();

        if (numberOfSlashes > 1) {
          s <<
            "there are " << numberOfSlashes << " slashes";
        }
        else {
          s <<
            "there is 1 slash";
        }
        s <<
          " attached to a rest";

        mxsr2msrWarning (
          gGlobalServiceRunData->getInputSourceName (),
          note->getInputLineNumber (),
          s.str ());
      }
    }

    if (! delayAttachment) {
      while (fPendingSlashesList.size ()) {
        S_msrSlash
          slash =
            fPendingSlashesList.front ();

        note->appendSlashToNote (slash);
        fPendingSlashesList.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingWedgesToNote (
  const S_msrNote& note)
{
  // attach the pending wedges if any to the note
  if (fPendingWedgesList.size ()) {
    Bool delayAttachment (false);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWedges ()) {
      gLogStream <<
        "Attaching pending wedges to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    if (fCurrentNoteIsARest) {
      if (gGlobalMxsr2msrOahGroup->getDelayRestsWedges ()) {
        gLogStream <<
          "Delaying wedge attached to a rest until next note" <<
      std::endl;

        delayAttachment = true;
      }

      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceWedges ()) {
          std::stringstream s;

          int numberOfWedges = fPendingWedgesList.size ();

          if (numberOfWedges > 1) {
            s <<
              "there are " << numberOfWedges << " wedges";
          }
          else {
            s <<
              "there is 1 wedge";
          }
          s <<
            " attached to a rest";

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            s.str ());
        }
#endif
      }
    }

    if (! delayAttachment) {
      while (fPendingWedgesList.size ()) {
        S_msrWedge
          wedge =
            fPendingWedgesList.front ();

        note->appendWedgeToNote (wedge);
        fPendingWedgesList.pop_front ();
      } // while
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingGlissandosToNote (
  const S_msrNote& note)
{
 // attach the pending glissandos if any to the note
  if (fPendingGlissandosList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGlissandos ()) {
      gLogStream <<
        "Attaching pending glissandos to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendingGlissandosList.size ()) {
      S_msrGlissando
        glissando =
          fPendingGlissandosList.front ();

      note->
        appendGlissandoToNote (glissando);

      // take care of no lyrics on kGlissandoTypeStop
      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissandoTypeKind::kGlissandoTypeNone:
        case msrGlissandoTypeKind::kGlissandoTypeStart:
          break;

        case msrGlissandoTypeKind::kGlissandoTypeStop:
          int inputLineNumber =
            glissando->getInputLineNumber ();

          // fetch the voice
          S_msrVoice
            voice =
              fetchVoiceFromCurrentPart (
                inputLineNumber,
     //     fCurrentMusicXMLStaffNumber, // JMI fCurrentStaffNumberToInsertInto,
                fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
                fCurrentMusicXMLVoiceNumber);

          // get the voice's stanzas std::map
          const std::map<std::string, S_msrStanza>&
            voiceStanzasMap =
              voice->
                getVoiceStanzasMap ();

#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceGlissandos ()) {
                gLogStream <<
                  "--> attachPendingGlissandosToNote()"
                  ", voiceStanzasMap.size () = " <<
                  voiceStanzasMap.size () <<
                  ", fCurrentNoteHasLyrics = " <<
                  fCurrentNoteHasLyrics <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif

          if (voiceStanzasMap.size ()) {
            // there are lyrics in this voice
            /* JMI
            if (! fCurrentNoteHasLyrics) {
              // append a skip to lyrics the same duration as the note
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceGlissandos ()) {
                gLogStream <<
                  "Attaching a skip syllable to note '" <<
                  note->asString () <<
                  "' that has a glissando stop and no lyrics " <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif

              for (
                std::map<std::string, S_msrStanza>::const_iterator i = voiceStanzasMap.begin ();
                i != voiceStanzasMap.end ();
                ++i
              ) {
                const S_msrStanza& stanza = (*i).second;
                // create a skip syllable
                S_msrSyllable
                  syllable =
                    msrSyllable::create (
                      inputLineNumber,
                      msrSyllableKind::kSyllableSkipRest,
                      msrSyllableExtendKind::kSyllableExtendNone, // fCurrentSyllableExtendKind, // JMI
                      fCurrentStanzaNumber,
                      fCurrentNoteSoundingWholeNotesFromDuration,
                      stanza);

                // append syllable to current note's syllables std::list
                fCurrentNoteSyllables.push_back (
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

      fPendingGlissandosList.pop_front ();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingSlidesToNote (
  const S_msrNote& note)
{
 // attach the pending slides if any to the note
  if (fPendingSlidesList.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceSlides ()) {
      gLogStream <<
        "Attaching pending slides to note " <<
        note->asString () <<
        std::endl;
    }
#endif

    while (fPendingSlidesList.size ()) {
      S_msrSlide
        slide =
          fPendingSlidesList.front ();

      note->
        appendSlideToNote (slide);

      // take care of no lyrics on kSlideTypeStop
      switch (slide->getSlideTypeKind ()) {
        case msrSlideTypeKind::kSlideTypeNone:
        case msrSlideTypeKind::kSlideTypeStart:
          break;

        case msrSlideTypeKind::kSlideTypeStop:
          int inputLineNumber =
            slide->getInputLineNumber ();

          // fetch the voice
          S_msrVoice
            voice =
              fetchVoiceFromCurrentPart (
                inputLineNumber,
     //     fCurrentMusicXMLStaffNumber, // JMI fCurrentStaffNumberToInsertInto,
                fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
                fCurrentMusicXMLVoiceNumber);

          // get the voice's stanzas std::map
          const std::map<std::string, S_msrStanza>&
            voiceStanzasMap =
              voice->
                getVoiceStanzasMap ();

#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceSlides ()) {
                gLogStream <<
                  "--> attachPendingSlidesToNote()"
                  ", voiceStanzasMap.size () = " <<
                  voiceStanzasMap.size () <<
                  ", fCurrentNoteHasLyrics = " <<
                  fCurrentNoteHasLyrics <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif

          if (voiceStanzasMap.size ()) {
            // there are lyrics in this voice
            /* JMI
            if (! fCurrentNoteHasLyrics) {
              // append a skip to lyrics the same duration as the note
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceSlides ()) {
                gLogStream <<
                  "Attaching a skip syllable to note '" <<
                  note->asString () <<
                  "' that has a slide stop and no lyrics " <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif

              for (
                std::map<std::string, S_msrStanza>::const_iterator i = voiceStanzasMap.begin ();
                i != voiceStanzasMap.end ();
                ++i
              ) {
                const S_msrStanza& stanza = (*i).second;
                // create a skip syllable
                S_msrSyllable
                  syllable =
                    msrSyllable::create (
                      inputLineNumber,
                      msrSyllableKind::kSyllableSkipRest,
                      msrSyllableExtendKind::kSyllableExtendNone, // fCurrentSyllableExtendKind, // JMI
                      fCurrentStanzaNumber,
                      fCurrentNoteSoundingWholeNotesFromDuration,
                      stanza);

                // append syllable to current note's syllables std::list
                fCurrentNoteSyllables.push_back (
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
}

void mxsr2msrTranslator::attachPendingVoiceLevelElementsToVoice (
  const S_msrVoice& voice)
{
//   JMI
//   gLogStream <<
//     "attachPendingVoiceLevelElementsToVoice()" <<
//     ", fPendingTemposList.size () = " << fPendingTemposList.size () <<
//     ", fPendingBarLinesList.size () = " << fPendingBarLinesList.size () <<
//     ", fPendingLineBreaksList.size () = " << fPendingLineBreaksList.size () <<
//     ", fPendingPageBreaksList.size () = " << fPendingPageBreaksList.size () <<
//     std::endl;
//     */
//
//   // the elements pending since before the note
//   // can now be appended to the latter's voice
//   // prior to the note itself
//
//   // attach pending rehearsals if any to voice
//   attachPendingRehearsalMarksToVoice (voice);
//
//   // attach pending barlines if any to voice
//   attachPendingBarLinesToVoice (voice);
//
//   // attach pending tempos if any to voice
//   attachPendingTemposToVoice (voice);
//
//   // attach pending line breaks if any to voice
//   attachLineBreaksToVoice (voice);
//
//   // attach pending page breaks if any to voice
//   attachPageBreaksToVoice (voice);
}

void mxsr2msrTranslator::attachPendingPartLevelElementsToPart ( // JMI v0.9.63
  const S_msrPart& part)
{
  /* JMI
  gLogStream <<
    "attachPendingPartLevelElementsToPart()" <<
    ", fPendingTemposList.size () = " << fPendingTemposList.size () <<
    ", fPendingBarLinesList.size () = " << fPendingBarLinesList.size () <<
    ", fPendingLineBreaksList.size () = " << fPendingLineBreaksList.size () <<
    ", fPendingPageBreaksList.size () = " << fPendingPageBreaksList.size () <<
    std::endl;
    */

  // the elements pending since before the note // JMI
  // can now be appended to the latter's voice
  // prior to the note itself

  // attach pending rehearsals if any to part
  attachPendingRehearsalMarksToPart (part);

  // attach pending barlines if any to part
  attachPendingBarLinesToPart (part);

  // attach pending tempos if any to part
  attachPendingTemposToPart (part);

  // attach pending line breaks if any to part
  attachLineBreaksToPart (part);

  // attach pending page breaks if any to part
  attachPageBreaksToPart (part);
}


void mxsr2msrTranslator::attachPendingNoteLevelElementsToNote (
  const S_msrNote& note)
{
  // attach the pending segnos, if any, to the note
  attachPendingSegnosToNote (note);

  // attach the pending codas, if any, to the note
  attachPendingCodasToNote (note);

  // attach the pending crescDecresc, if any, to the note
  attachPendingCrescDecrescsToNote (note);

  // attach the pending eyeglasses, if any, to the note
  attachPendingEyeGlassesToNote (note);

  // attach the pending damps, if any, to the note
  attachPendingDampsToNote (note);

  // attach the pending damp alls, if any, to the note
  attachPendingDampAllsToNote (note);

  // attach the pending scordaturas, if any, to the note
  attachPendingScordaturasToNote (note);

  // attach the pending octave shifts, if any, to the note
  attachPendingOctaveShiftsToNote (note);

  // attach the pending dynamics, if any, to the note
  attachPendingDynamicsToNote (note);

  // attach the pending other dynamics, if any, to the note
  attachPendingOtherDynamicsToNote (note);

  // attach the pending words, if any, to the note
  attachPendingWordsToNote (note);

  // attach the pending beams, if any, to the note
  attachPendingBeamsToNote (note);

  // attach the pending slurs, if any, to the note
  attachPendingSlursToNote (note);

  // attach the pending ligatures, if any, to the note
  attachPendingLigaturesToNote (note);

  // attach the pending pedals, if any, to the note
  attachPendingPedalsToNote (note);

  // attach the pending slashes, if any, to the note
  attachPendingSlashesToNote (note);

  // attach the pending wedges, if any, to the note
  attachPendingWedgesToNote (note);

  // attach the pending glissandos, if any, to the note
  attachPendingGlissandosToNote (note);

  // attach the pending slides, if any, to the note
  attachPendingSlidesToNote (note);
}

//______________________________________________________________________________
S_msrNote mxsr2msrTranslator::createNote (
  int inputLineNumber)
{
  // determine quarter tones note pitch
  fCurrentNoteQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentNoteDiatonicPitchKind,
      fCurrentNoteAlterationKind);

  // determine quarter tones note display pitch
  fCurrentNoteQuarterTonesDisplayPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentDisplayDiatonicPitchKind,
      fCurrentNoteAlterationKind);

  // has the current note graphic duration been specified
  // in a '<type>' markup?
  switch (fCurrentNoteGraphicDurationKind) {
    case msrDurationKind::kDuration_NO_:
      // use the same duration as the one from the duration
      // internally ??? JMI
      fCurrentNoteDisplayWholeNotesFromType =
        fCurrentNoteSoundingWholeNotesFromDuration;
      break;

    default:
      // convert note graphic duration into whole notes
      fCurrentNoteDisplayWholeNotesFromType =
        msrDurationKindAsWholeNotes (
          fCurrentNoteGraphicDurationKind);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
        gLogStream <<
          std::endl <<
          "(1):" <<
          std::endl <<
          "fCurrentNoteGraphicDurationKind = " <<
          msrDurationKindAsString (
            fCurrentNoteGraphicDurationKind) <<
          std::endl <<
          "fCurrentNoteDisplayWholeNotesFromType = " <<
          fCurrentNoteDisplayWholeNotesFromType <<
          std::endl <<
          "fCurrentNoteDotsNumber = " <<
          fCurrentNoteDotsNumber <<
          std::endl << std::endl;
      }
#endif

      // take dots into account if any
      if (fCurrentNoteDotsNumber > 0) {
        Rational
          wholeNotesIncrement =
            fCurrentNoteDisplayWholeNotesFromType * Rational (1, 2);
        int
          dots =
            fCurrentNoteDotsNumber;

        while (dots > 0) {
          fCurrentNoteDisplayWholeNotesFromType +=
            wholeNotesIncrement;

          wholeNotesIncrement *= Rational (1, 2);

          --dots;

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
            gLogStream <<
              std::endl <<
              "(2):" <<
              std::endl <<
              "fCurrentNoteDisplayWholeNotesFromType = " <<
              fCurrentNoteDisplayWholeNotesFromType <<
              std::endl <<
              "wholeNotesIncrement = " <<
              wholeNotesIncrement <<
              std::endl <<
              "dots = " <<
              dots <<
              std::endl << std::endl;
          }
#endif
        } // while
      }

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
        gLogStream <<
          std::endl <<
          "(3):" <<
          std::endl <<
          "fCurrentNoteGraphicDurationKind = " <<
          msrDurationKindAsString (
            fCurrentNoteGraphicDurationKind) <<
          std::endl <<
          "fCurrentNoteDisplayWholeNotesFromType = " <<
          fCurrentNoteDisplayWholeNotesFromType <<
          std::endl <<
          "fCurrentNoteDotsNumber = " <<
          fCurrentNoteDotsNumber <<
          std::endl << std::endl;
      }
#endif
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Gathered note informations:" <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 42;

    gLogStream << std::left <<
      std::setw (fieldWidth) <<
      "CurrentNoteSoundingWholeNotesFromDuration" << " = " <<
      fCurrentNoteSoundingWholeNotesFromDuration <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteGraphicDuration" << " : " <<
        msrDurationKindAsString (
          fCurrentNoteGraphicDurationKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteDotsNumber" << " : " <<
        fCurrentNoteDotsNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteDisplayWholeNotesFromType" << " = " <<
      fCurrentNoteDisplayWholeNotesFromType <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteIsARest" << " = " <<
      fCurrentNoteIsARest <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentDivisionsPerQuarterNote" << " = " <<
      fCurrentDivisionsPerQuarterNote <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNotePrintObjectKind" << " = " <<
      fCurrentNotePrintObjectKind <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteRGB" << " = " <<
      fCurrentNoteRGB <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteAlpha" << " = " <<
      fCurrentNoteAlpha <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentMusicXMLStaffNumber =" << " = " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentMusicXMLStaffNumber" << " = " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentStaffNumberToInsertInto" << " = " <<
      fCurrentStaffNumberToInsertInto <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fPreviousNoteMusicXMLStaffNumber" << " = " <<
      fPreviousNoteMusicXMLStaffNumber <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentMusicXMLVoiceNumber" << " = " <<
      fCurrentMusicXMLVoiceNumber <<
      std::endl <<

      std::setw (fieldWidth) <<
      "inputLineNumber" << " = " <<
      inputLineNumber <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif

  if (fCurrentNoteIsAGraceNote) {
    // set current grace note display whole notes
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteDisplayWholeNotesFromType;
  }

  else if (
    fCurrentDoubleTremoloTypeKind == msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart
      ||
    fCurrentDoubleTremoloTypeKind == msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop
  ) {
    // double tremolo note
    if (fCurrentNoteGraphicDurationKind == msrDurationKind::kDuration_NO_) {
      std::stringstream s;

      s <<
        "double tremolo note lacks a <type/>"; // JMI a completer

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

    // set current double tremolo note display whole notes
    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteDisplayWholeNotesFromType;
  }

  else if (fCurrentNoteIsARest) {
    // rest

    // set current rest sounding and display whole notes
    fCurrentNoteSoundingWholeNotes =
      fCurrentNoteSoundingWholeNotesFromDuration;

    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteDisplayWholeNotesFromType;

/* JMI
    // set current note sounding and display whole notes
    if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
      // only <type /> was found, no <duration /> was specified
      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteDisplayWholeNotesFromType;

      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteDisplayWholeNotes; // same value by default
    }
    else {
      // <duration /> was found
      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteSoundingWholeNotesFromDuration;

      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteSoundingWholeNotes; // same value by default
    }
    */
  }

  else {
    // other note

    // set current note sounding and display whole notes
    fCurrentNoteSoundingWholeNotes =
      fCurrentNoteSoundingWholeNotesFromDuration;

    fCurrentNoteDisplayWholeNotes =
      fCurrentNoteDisplayWholeNotesFromType;

    /* JMI
    if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
      // only <type /> was found, no <duration /> was specified
      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteDisplayWholeNotesFromType;

      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteDisplayWholeNotes; // same value by default
    }
    else {
      // <duration /> was found
      fCurrentNoteSoundingWholeNotes =
        fCurrentNoteSoundingWholeNotesFromDuration;

      fCurrentNoteDisplayWholeNotes =
        fCurrentNoteSoundingWholeNotes; // same value by default
    }
    */
  }

  // create the (new) note
  S_msrNote
    newNote =
      msrNote::create (
        inputLineNumber,
	      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()

        fCurrentMeasureNumber,

        msrNoteKind::kNote_NO_,
          // will be set by 'setNoteKind()' when it becomes known later

        fCurrentNoteQuarterTonesPitchKind,
        fCurrentNoteOctave,

        fCurrentNoteSoundingWholeNotes,
        fCurrentNoteDisplayWholeNotes,

        fCurrentNoteDotsNumber,

        fCurrentNoteGraphicDurationKind,

        fCurrentNoteQuarterTonesDisplayPitchKind,
        fCurrentDisplayOctave,

        fCurrentNoteIsACueNoteKind,

        fCurrentNotePrintObjectKind,

        fCurrentNoteHeadKind,
        fCurrentNoteHeadFilledKind,
        fCurrentNoteHeadParenthesesKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      "Creating note:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      newNote <<
      std::endl;

    --gIndenter;
  }
#endif

  return newNote;
}

//______________________________________________________________________________
void mxsr2msrTranslator::populateNoteBeforeNoteItselfIsHandled (
  int       inputLineNumber,
  const S_msrNote& newNote)
{
  // set newNote's color if relevant
  if (fCurrentNoteRGB.size () || fCurrentNoteAlpha.size ()) {
    msrColorAlphaRGB
      colorAlphaRGB (
        fCurrentNoteRGB,
        fCurrentNoteAlpha);

    newNote->
      setNoteColorAlphaRGB (
        colorAlphaRGB);
  }

  // attach the regular pending elements (not dal segnos), if any, to newNote
//   attachPendingNoteLevelElementsToNote (newNote); would be too early ??? JMI

  newNote->
    setNoteAccidentalKind (
      fCurrentAccidentalKind);

  newNote->
    setNoteEditorialAccidentalKind (
      fCurrentEditorialAccidentalKind);

  newNote->
    setNoteCautionaryAccidentalKind (
      fCurrentCautionaryAccidentalKind);

  // check <duration/> and <type/> consistency if relevant
  if (
    fCurrentNoteSoundingWholeNotesFromDuration
      !=
    fCurrentNoteDisplayWholeNotesFromType
      &&
    ! fCurrentNoteHasATimeModification
  ) {
    switch (newNote->getNoteKind ()) {
      case msrNoteKind::kNote_NO_:
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
          std::stringstream s;

          s <<
            "note duration inconsistency: divisions indicates " <<
            fCurrentNoteSoundingWholeNotesFromDuration <<
            " while type indicates " <<
            fCurrentNoteDisplayWholeNotesFromType <<
            ", using the latter";

          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            s.str ());
        }

        newNote->
          setMeasureElementSoundingWholeNotes (
            fCurrentNoteDisplayWholeNotesFromType,
            "mxsr2msrTranslator::populateNoteBeforeNoteItselfIsHandled()");
        break;

      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
      case msrNoteKind::kNoteInChordInGraceNotesGroup:
      case msrNoteKind::kNoteInDoubleTremolo:
        break;
    } // switch
  }

  // set newNote tie if any
  if (fCurrentTie) {
    newNote->
      setNoteTie (fCurrentTie);
  }

  // set its stem if any
  if (fCurrentStem) {
    newNote->
      setNoteStem (fCurrentStem);
  }

/* JMI
  // attach the beams if any to the note
  if (fPendingBeamsList.size ()) {
    for (
      std::list<S_msrBeam>::const_iterator i=fPendingBeamsList.begin ();
      i!=fPendingBeamsList.end ();
      ++i
    ) {
      newNote->
        appendBeamToNote ((*i));
    } // for

    fPendingBeamsList.clear ();
  }
*/

  // attach the articulations if any to the note
  attachCurrentArticulationsToNote (newNote);

  // attach the technicals if any to the note
  attachCurrentTechnicalsToNote (newNote);
  attachCurrentTechnicalWithIntegersToNote (newNote);
  attachCurrentTechnicalWithFloatsToNote (newNote);
  attachCurrentTechnicalWithStringsToNote (newNote);

  // attach the ornaments if any to the note
  attachCurrentOrnamentsToNote (newNote);

  // attach the spanners if any to the note
  attachCurrentSpannersToNote (newNote);

  // attach the singleTremolo if any to the note
  attachCurrentSingleTremoloToNote (newNote);

  // handling the pending grace notes group if any
  if (fPendingGraceNotesGroup && ! fCurrentNoteIsAGraceNote) {
    // this is the first note after the grace notes group

    // attach the current grace notes to this note
    switch (fPendingGraceNotesGroup->getGraceNotesGroupKind ()) {
      case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
        newNote->
          setNoteGraceNotesGroupBefore (
            fPendingGraceNotesGroup);
        break;
      case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
        newNote->
          setNoteGraceNotesGroupAfter (
            fPendingGraceNotesGroup);
        break;
    } // switch

    // forget about the pending grace notes
    fPendingGraceNotesGroup = nullptr;
  }
}

void mxsr2msrTranslator::populateNoteAfterNoteItselfIsHandled (
  int       inputLineNumber,
  const S_msrNote& newNote)
{
  // attach the regular pending elements (not dal segnos), if any, to newNote
  // only now because <lyric> follows <glissando> and <slide> in MusicXML JMI ???
  attachPendingNoteLevelElementsToNote (newNote);

  // handle the pending harmonies if any
  if (fPendingHarmoniesList.size ()) {
    // get voice to insert harmonies into
    S_msrVoice
      voiceToInsertHarmoniesInto =
        fCurrentPart->
          getPartHarmoniesVoice ();

    // sanity check
    mfAssert (
      __FILE__, __LINE__,
      voiceToInsertHarmoniesInto != nullptr,
      "voiceToInsertHarmoniesInto is null");

    handlePendingHarmonies (
      newNote,
      voiceToInsertHarmoniesInto);

    // reset harmony counter
    fHarmoniesVoicesCounter = 0;
  }

  // handle the pending figured bass elements if any
  if (fPendingFiguredBassesList.size ()) {
    // get voice to insert figured bass elements into
    S_msrVoice
      voiceToInsertFiguredBassesInto =
        fCurrentPart->
          getPartFiguredBassVoice ();

    // sanity check
    mfAssert (
      __FILE__, __LINE__,
      voiceToInsertFiguredBassesInto != nullptr,
      "voiceToInsertFiguredBassesInto is null");

    handlePendingFiguredBasses (
      newNote,
      voiceToInsertFiguredBassesInto);

    // reset figured bass counter
    fFiguredBassVoicesCounter = 0;
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::createAStaffChangeIfNecessary (
  int        inputLineNumber,
  S_msrNote  newNote,
  const S_msrVoice& voiceToInsertInto)
{
  // is there a staff change?
  fCurrentStaffChangeKind = msrStaffChangeKind::kStaffChange_NO_;

  if (
    fCurrentMusicXMLStaffNumber != fPreviousNoteMusicXMLStaffNumber
      &&
    fPreviousNoteMusicXMLStaffNumber != msrStaff::K_NO_STAFF_NUMBER
  ) {
    // yes, there is a staff change

    // fetch staff to change to
    S_msrStaff
      staffToChangeTo =
        fetchStaffFromCurrentPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber);

    // is newNote a chord member note?
    if (fCurrentNoteBelongsToAChord) {
      // yes, newNote is a chord member note
      fCurrentStaffChangeKind = msrStaffChangeKind::kStaffChangeChordMemberNote;

      // register the note as cross sta
      fCurrentNoteIsCrossStaves = true;

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceStaffChanges ()) {
        gLogStream <<
          "*** There is staff change for chord member note '" <<
          newNote->asShortString () <<
          "' in voice \"" <<
          voiceToInsertInto->getVoiceName () <<
          "\"" <<
          " from staff " << fPreviousNoteMusicXMLStaffNumber <<
          " to staff " << fCurrentMusicXMLStaffNumber <<
          ", \"" << staffToChangeTo->getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fCurrentStaffChangeKind = msrStaffChangeKind::kStaffChangeChordMemberNote;

      // LilyPond doesn't support cross staff chords, JMI
      // so place newNote in its 'official' staff
    }

    else {
      // no, newNote is another note
      fCurrentStaffChangeKind = msrStaffChangeKind::kStaffChangeOtherNote;

      // a staff change is necessary
      // to remain in this staff and not use the note's one

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceStaffChanges ()) {
        gLogStream <<
          "*** There is staff change for note '" <<
          newNote->asShortString () <<
          "' in voice \"" <<
          voiceToInsertInto->getVoiceName () <<
          "\"" <<
          " from staff " << fPreviousNoteMusicXMLStaffNumber <<
          " to staff " << fCurrentMusicXMLStaffNumber <<
          ", \"" << staffToChangeTo->getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      // create the voice staff change
      S_msrVoiceStaffChange
        voiceStaffChange =
          msrVoiceStaffChange::create (
            inputLineNumber,
            staffToChangeTo);

  /* JMI
      // fetch the voice to insert into
      S_msrVoice
        voiceToInsertInto =
          fetchVoiceFromCurrentPart (
            inputLineNumber,
            fCurrentStaffNumberToInsertInto,
            fCurrentMusicXMLVoiceNumber);
*/

      // append it to the current sequence voice
      // before the note itself is appended
      voiceToInsertInto->
        appendVoiceStaffChangeToVoice (
          voiceStaffChange);

      // the actual note staff is already stored in newNote,
      // now fake its belonging to the current sequence staff
   // JMI  BOFBOFBOF ??? fCurrentMusicXMLStaffNumber = fCurrentStaffNumberToInsertInto;
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleNoteItself (
  int        inputLineNumber,
  S_msrNote  newNote)
{
  // handle the note itself
  if (fCurrentNoteBelongsToAChord) {
    if (fCurrentNoteBelongsToATuplet) {
      // note is the second, third, ..., member of a chord
      // that is a member of a tuplet
      handleNoteBelongingToAChordInATuplet (
        newNote);
    }

    else if (fCurrentNoteIsAGraceNote) {
      // note is the second, third, ..., member of a chord
      // that is a part of grace notes
      handleNoteBelongingToAChordInAGraceNotesGroup (
        newNote);
    }

    else {
      // note is the second, third, ..., member of a chord
      // whose first member is in
      // JMI ??? 'fLastHandledNoteInVoiceMap [currentVoice]'
      handleNoteBelongingToAChord (
        newNote);
    }
  }

  else if (fCurrentNoteBelongsToATuplet) {
    // note/rest is the first, second, third, ..., member of a tuplet
    handleNoteBelongingToATuplet (
      newNote);
  }

  else {
    // note/rest is a regular note or a member of grace notes

    // this terminates a tuplet if any
    handlePendingTupletStopIfAny (
      inputLineNumber,
      newNote);

    // handle it
    handleNonChordNorTupletNoteOrRest (
      newNote);
  }

  // finalizing current chord if relevant
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "--> fCurrentNoteBelongsToAChord: " <<
      fCurrentNoteBelongsToAChord <<
      "--> fOnGoingChord: " <<
      fOnGoingChord <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (! fCurrentNoteBelongsToAChord) {
    if (fOnGoingChord) {
      // newNote is the first note after the chord in the current voice

      // finalize the current chord
      if (false && ! fCurrentNoteIsAGraceNote) // JMI CAFE
      finalizeCurrentChord (
        inputLineNumber);

      fOnGoingChord = false;
    }

    if (fCurrentDoubleTremolo) {
      // forget about a double tremolo containing a chord
    // JMI XXL BOFS  fCurrentDoubleTremolo = nullptr;
    }
  }

  // register newNote as the last found note for the current voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
  /* JMI
    gLogStream <<
      "--> STORING " <<
      newNote->asShortString () <<
      " as last note found in voice " <<
      voiceToInsertInto->getVoiceName () <<
      std::endl <<
      "-->  fCurrentMusicXMLStaffNumber = " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      "--> fCurrentMusicXMLVoiceNumber  = " <<
      fCurrentMusicXMLVoiceNumber <<
      std::endl <<
      / * JMI
      "--> staff name  = " <<
      staff->getStaffName () <<
      std::endl <<
      * /
      "--> voice name  = " <<
      voiceToInsertInto->getVoiceName () <<
      std::endl;
      */
  }
#endif

  fVoicesLastMetNoteMap [
    std::make_pair (
      fCurrentStaffNumberToInsertInto, // JMI fCurrentSequenceStaffNumber,
      fCurrentMusicXMLVoiceNumber)
    ] =
      newNote;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    printVoicesLastMetNoteMap (
      inputLineNumber,
      "handleNote()");
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingGraceNotesGroupToNoteIfRelevant (
  int inputLineNumber)
{
  // is there a pending grace notes group?
  if (fPendingGraceNotesGroup) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalMxsrOahGroup->getTraceBackup ()) {
      gLogStream <<
        "Attaching pending grace notes group to current non-grace note upon backup" <<
        std::endl;
    }

    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        std::endl <<
        "fCurrentGraceNotes IS NOT NULL upon <backup/>" << // JMI
        ", line " << inputLineNumber <<
        std::endl;

      gLogStream <<
        "+++++++++++++++++ fPendingGraceNotesGroup:" <<
        std::endl <<
        fPendingGraceNotesGroup <<
        "+++++++++++++++++" <<
        std::endl;


//     // fetch the voice
//     S_msrVoice
//       voice =
//         fetchVoiceFromCurrentPart (
//           inputLineNumber,
//           fCurrentStaffNumberToInsertInto,
//           fCurrentMusicXMLVoiceNumber);
//
//     // fetch note to attach to
//     S_msrNote
//       noteToAttachTo =
//       /*
//       // JMI might prove not precise enough???
//   //      fVoicesLastMetNoteMap [currentVoice];
//         fVoicesLastMetNoteMap [
//           std::make_pair (
//             fCurrentStaffNumberToInsertInto,
//             fCurrentMusicXMLVoiceNumber)
//           ];
//       */
//         voice->getVoiceLastAppendedNote ();
    }

#endif

    if (false && fCurrentNonGraceNote) { // JMI
#ifdef TRACING_IS_ENABLED
      gLogStream <<
        "+++++++++++++++++ fCurrentNonGraceNote:" <<
        std::endl <<
        fCurrentNonGraceNote <<
        "+++++++++++++++++" <<
        std::endl;
#endif

      // set the pending grace notes group's kind to 'after'
      fPendingGraceNotesGroup->
        setGraceNotesGroupKind (
          msrGraceNotesGroupKind::kGraceNotesGroupAfter);

      // attach it to fCurrentNonGraceNote
      fCurrentNonGraceNote->
        setNoteGraceNotesGroupAfter (
          fPendingGraceNotesGroup);
    }

    else {
      std::stringstream s;

      s <<
        "cannot find voice to insert after grace notes group into: " <<
        "chordFirstNote is null" <<
        ", fCurrentStaffNumberToInsertInto = " <<
        fCurrentStaffNumberToInsertInto <<
        std::endl <<
        ", fCurrentMusicXMLVoiceNumber = " <<
        fCurrentMusicXMLVoiceNumber;

      mxsr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

    // forget about this grace notes group
    fPendingGraceNotesGroup = nullptr;
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleBackup (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceBackup ()) {
    gLogStream <<
      "Handling pending backup" <<
      ", fCurrentBackupDivisions: " <<
      fCurrentBackupDivisions <<
      ", fCurrentNonGraceNote: " <<
      fCurrentNonGraceNote->asShortString () <<
      std::endl;
  }
#endif

  // fetch current voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceOctaveShifts ()) {
    gLogStream <<
      "--> handleBackup()" <<
      ", fCurrentStaffNumberToInsertInto: " <<
      fCurrentStaffNumberToInsertInto <<
      ", fCurrentMusicXMLVoiceNumber: " <<
      fCurrentMusicXMLVoiceNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);

  // are there pending note level elements?
  attachPendingNoteLevelElementsToNote ( // JMI
    fCurrentNonGraceNote);

  // is there a pending grace notes group?
  attachPendingGraceNotesGroupToNoteIfRelevant (
    inputLineNumber);

  // compute the backup step length
  Rational
    backupStepLength =
      Rational (
        fCurrentBackupDivisions,
        fCurrentDivisionsPerQuarterNote * 4); // hence a whole note

  // let fCurrentPart handle the backup
  fCurrentPart->
    handleBackupInPart (
      inputLineNumber,
      backupStepLength);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitEnd (S_note& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_note" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

  if (fPreviousMeasureElement) {
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*fPreviousMeasureElement))
    ) {
      attachPendingDalSegnosToNote (note);
    }
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*fPreviousMeasureElement))
    ) {
      attachPendingDalSegnosToChord (chord);
    }
    else {
      // FOO JMI
    }
  }
  else {
    // FOO JMI
  }

  /* JMI
  std::map<std::pair<int, int>, S_msrNote>::iterator
    it =
      fVoicesLastMetNoteMap.find (
        std::make_pair (
          fPreviousNoteMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber)); // JMI ???

  if (it != fVoicesLastMetNoteMap.end ()) {
    S_msrNote
      lastMetNoteInVoice =
        (*it).second;

    attachPendingDalSegnosToNote (lastMetNoteInVoice);
  }
  else {
    // FOO JMI
  }
*/

  // create the note
  S_msrNote
    newNote =
      createNote (
        inputLineNumber);

  fPreviousMeasureElement = newNote;

  // fetch current note's voice
  S_msrVoice
    currentNoteVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    currentNoteVoice != nullptr,
    "currentNoteVoice is null");

  // the elements pending since before the note if any
  // can now be appended to the latter's voice
  // prior to the note itself
  attachPendingVoiceLevelElementsToVoice (
    currentNoteVoice);

  attachPendingPartLevelElementsToPart (
    fCurrentPart);

  // set current staff number to insert into if needed JMI ???
  if (fCurrentStaffNumberToInsertInto == msrStaff::K_NO_STAFF_NUMBER) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceStaffChanges ()) {
      gLogStream <<
        "==> setting fCurrentStaffNumberToInsertInto to " <<
        fCurrentMusicXMLStaffNumber <<
        ", in voice \"" <<
        currentNoteVoice->getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    fCurrentStaffNumberToInsertInto = fCurrentMusicXMLStaffNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "==> fetching voice to insert note into" <<
      ", fCurrentStaffNumberToInsertInto: " <<
      fCurrentStaffNumberToInsertInto <<
      ", fPreviousNoteMusicXMLStaffNumber: " <<
      fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      ", fCurrentMusicXMLVoiceNumber: " <<
      fCurrentMusicXMLVoiceNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // fetch voice to insert note into
  S_msrVoice
    voiceToInsertNoteInto =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceToInsertNoteInto != nullptr,
    "voiceToInsertNoteInto is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "==> is there a staff change?" <<
      " fCurrentStaffNumberToInsertInto: " <<
      fCurrentStaffNumberToInsertInto <<
      ", fPreviousNoteMusicXMLStaffNumber: " <<
      fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      ", in voice \"" <<
      voiceToInsertNoteInto->getVoiceName() <<
      "\"" <<
      /* JMI
      ", fCurrentMusicXMLStaffNumber: " << fCurrentMusicXMLStaffNumber <<
      ", in staff \"" <<
      staff->getStaffName() <<
      "\"" <<
      */
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a staff change if necessary
  createAStaffChangeIfNecessary (
    inputLineNumber,
    newNote,
    voiceToInsertNoteInto);


// HERE JMI BLARK
  // attach the pre-pending elements if any to newNote,
  // before the note itself is handled, because that may cause
  // tuplets or chords to be appended to the voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceStaffChanges ()) {
    gLogStream <<
      "==> fetching voice to insert harmonies, figured bass elements and/or frames into" <<
      ", fCurrentStaffNumberToInsertInto: " <<
      fCurrentStaffNumberToInsertInto <<
      ", fPreviousNoteMusicXMLStaffNumber: " <<
      fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      ", fCurrentMusicXMLVoiceNumber: " <<
      fCurrentMusicXMLVoiceNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/* JMI
  // fetch the staff from current part
  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto);
*/

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // populate newNote before the note itself is handled
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // CAUTION JMI v0.9.66
  // permuted the order of populateNote() and handleNoteItself()
  // to have newNote's harmonies std::list already populated if relevant
  // when newNote is appended to the voice,
  // so as to compute the harmonies positions in the measure.

  // are all newNote uplinks set alright ??? JMI v0.9.66
  populateNoteBeforeNoteItselfIsHandled (
    inputLineNumber,
    newNote);

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // handle newNote itself
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  handleNoteItself (
    inputLineNumber,
    newNote);

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // populate newNote after the note itself is handled
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // are all newNote uplinks set alright ??? JMI v0.9.66
  populateNoteAfterNoteItselfIsHandled (
    inputLineNumber,
    newNote);

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // handle the lyrics for the note after the latter itself is handled
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // lyrics if any have to be handled in all cases
  // done only now because attachPendingNoteLevelElementsToNote()
  // may append skip syllables to the notes
  handleLyricsForNoteAfterNoteItselfIsHandled (
    voiceToInsertNoteInto,
    newNote);

  fOnGoingNote = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::handlePendingHarmonies (
  S_msrNote  newNote,
  const S_msrVoice& voiceToInsertInto)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
    gLogStream <<
      "handlePendingHarmonies(), newNote: " <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      newNote <<
      std::endl;
    --gIndenter;
  }
#endif

  Rational
    newNoteSoundingWholeNotes =
      newNote->
        getMeasureElementSoundingWholeNotes (),
    newNoteDisplayWholeNotes =
      newNote->
        getNoteDisplayWholeNotes ();

  while (fPendingHarmoniesList.size ()) { // recompute at each iteration
    S_msrHarmony
      harmony =
        fPendingHarmoniesList.front ();

    /*
       MusicXML harmonies don't have a duration,
       and MSR could follow this line, but LilyPond needs one...
       So:
         - we register all harmonies with the duration of the next note
         - they will be sorted by position in the measure in finalizeMeasure(),
           at which time their duration may be shortened
           so that the offsets values are enforced
           and they don't overflow the measure
      It is VITAL that harmonies measures be finalized
      AFTER the corresponding measure in the regular voice,
      since the current sounding whole notes of the latter is needed for that
    */

    // set the harmony's sounding whole notes
    harmony->
      setMeasureElementSoundingWholeNotes (
        newNoteSoundingWholeNotes,
        "mxsr2msrTranslator::handlePendingHarmonies()");

    // set the harmony's display whole notes JMI useless???
    harmony->
      setHarmonyDisplayWholeNotes (
        newNoteDisplayWholeNotes);

    // set the harmony's tuplet factor
    harmony->
      setHarmonyTupletFactor (
        msrTupletFactor (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes));

    // register this note as the harmony note upLink
    harmony->
      setHarmonyUpLinkToNote (newNote);

    // append the harmony to newNote's harmonies std::list
    newNote->
      appendHarmonyToNoteHarmoniesList (
        harmony);

    // get the part harmonies voice for the current voice
    S_msrVoice
      partHarmoniesVoice =
        fCurrentPart->
          getPartHarmoniesVoice ();

    // sanity check
    mfAssert (
      __FILE__, __LINE__,
      partHarmoniesVoice != nullptr,
      "partHarmoniesVoice is null");

    // set the harmony's voice upLink
    // only now that we know which harmonies voice will contain it
    harmony->
      setHarmoniesUpLinkToVoice (
        partHarmoniesVoice);

//* JMI VIRER v0.9.66
    // append the harmony to the part harmonies voice
    partHarmoniesVoice->
      appendHarmonyToVoice (
        harmony);
//*/
    // don't append the harmony to the part harmonies voice,
    // this will be done when the note itself is appended to the voice

    // remove the harmony from the std::list
    fPendingHarmoniesList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrTranslator::handlePendingFiguredBasses (
  S_msrNote  newNote,
  const S_msrVoice& voiceToInsertInto)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBassDetails ()) {
    gLogStream <<
      "handlePendingFiguredBasses(), newNote: " <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      newNote <<
      std::endl;
    --gIndenter;
  }
#endif

  Rational
    newNoteSoundingWholeNotes =
      newNote->
        getMeasureElementSoundingWholeNotes (),
    newNoteDisplayWholeNotes =
      newNote->
        getNoteDisplayWholeNotes ();

  while (fPendingFiguredBassesList.size ()) { // recompute at each iteration
    S_msrFiguredBass
      figuredBass =
        fPendingFiguredBassesList.front ();

    /*
       MusicXML figured bass elements don't have a duration,
       and MSR could follow this line, but LilyPond needs one...
       So:
         - we register all figured bass elements with the duration of the next note
         - they will be sorted by position in the measure in finalizeMeasure(),
           at which time their duration may be shortened
           so that the offsets values are enforced
           and they don't overflow the measure
      It is VITAL that figured bass elements measures be finalized
      AFTER the corresponding measure in the regular voice,
      since the current sounding whole notes of the latter is needed for that
    */

    // set the figured bass element's sounding whole notes
    figuredBass->
      setMeasureElementSoundingWholeNotes (
        newNoteSoundingWholeNotes,
        "handlePendingFiguredBasses()");

    // set the figured bass element's display whole notes JMI useless???
    figuredBass->
      setFiguredBassDisplayWholeNotes (
        newNoteDisplayWholeNotes);

    // set the figured bass element's tuplet factor
    figuredBass->
      setFiguredBassTupletFactor (
        msrTupletFactor (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes));

    // register this note as the figured bass element note upLink
    figuredBass->
      setFiguredBassUpLinkToNote (newNote);

    // append the figured bass to newNote's figured bass elements list
    newNote->
      appendFiguredBassToNoteFiguredBassesList (
        figuredBass);

//* JMI
    // get the figured bass voice for the current voice
    S_msrVoice
      partFiguredBassVoice =
//         voiceToInsertInto->
//         	fetchVoiceUpLinkToPart ()->
        fCurrentPart->
        		getPartFiguredBassVoice ();

    // sanity check
    mfAssert (
      __FILE__, __LINE__,
      partFiguredBassVoice != nullptr,
      "partFiguredBassVoice is null");

    // set the figuredBass's voice upLink
    // only now that we know which figured bass voice will contain it
    figuredBass->
      setFiguredBassUpLinkToVoice (
        partFiguredBassVoice);

    // append the figured bass to the figured bass voice for the current voice
    partFiguredBassVoice->
      appendFiguredBassToVoice (
        figuredBass);
//*/

    // don't append the figured bass to the part figured bass voice
    // before the note itself has been appended to the voice

    // remove the figured bass from the std::list
    fPendingFiguredBassesList.pop_front ();
  } // while
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleNonChordNorTupletNoteOrRest (
  const S_msrNote& newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();

  ++gIndenter;

  // register note/rest kind right now, to have a nice trace below
  if (fCurrentNoteIsAGraceNote) {
    // gracenote
    newNote->
      setNoteKind (
        msrNoteKind::kNoteRegularInGraceNotesGroup);
  }

  else {
    // non-grace note
    fCurrentNonGraceNote = newNote;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) { // JMI
      gLogStream <<
        "handleNonChordNorTupletNoteOrRest(), fCurrentNonGraceNote: " <<
        fCurrentNonGraceNote->asShortString () <<
        std::endl;
    }
#endif

    if (
      fCurrentDoubleTremoloTypeKind == msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart
        ||
      fCurrentDoubleTremoloTypeKind == msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop
    ) {
      // double tremolo note
      newNote->
        setNoteKind (
          msrNoteKind::kNoteInDoubleTremolo);
    }

    else {
      // regular or unpitched note or rest
      if (fCurrentNoteIsARest) {
        newNote->
          setNoteKind (
            msrNoteKind::kNoteRestInMeasure);
      }
      else if (fCurrentNoteIsUnpitched) {
        newNote->
          setNoteKind (
            msrNoteKind::kNoteUnpitchedInMeasure);
      }
      else {
        newNote->
          setNoteKind (
            msrNoteKind::kNoteRegularInMeasure);
      }
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) { // JMI
    gLogStream <<
      "handleNonChordNorTupletNoteOrRest(), newNote: " <<
      std::endl;
    ++gIndenter;
    gLogStream <<
      newNote <<
      std::endl;
    --gIndenter;
  }
#endif

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Handling non-chord, non-tuplet note or rest " <<
       newNote->asShortString () << // NO, would lead to infinite recursion ??? JMI
      ", currentVoice = \"" <<
      currentVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      ":" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      newNote->asString () <<
      std::endl;

    const int fieldWidth = 25;

    gLogStream << std::left <<
      std::setw (fieldWidth) << "voice" << " : \"" <<
      currentVoice->getVoiceName () << "\"" <<
      std::endl <<
      std::setw (fieldWidth) << "line:" << " : " <<
      inputLineNumber <<
      std::endl <<
      std::setw (fieldWidth) << "fCurrentNoteIsAGraceNote" << " : " <<
      fCurrentNoteIsAGraceNote <<
      std::endl;

    if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
      gLogStream <<
        std::setw (fieldWidth) << "fPendingGraceNotesGroup" << " : " <<
        std::endl <<
        "======================= handleNonChordNorTupletNoteOrRest()" <<
        ", line " << inputLineNumber <<
        std::endl;
      fCurrentPart->print (gLogStream);
      gLogStream <<
        "=======================" <<
        std::endl << std::endl;

      if (fPendingGraceNotesGroup) {
        gLogStream <<
          fPendingGraceNotesGroup;
      }
      else {
 //       gLogStream <<
 //         "fCurrentGraceNotes is NULL"; // JMI
      }

      gLogStream << std::endl;
    }

    --gIndenter;
  }
#endif

/* JMI ???
  // handle the pending tuplets if any,
  // unless the current note is a grace note,
  // i.e. it is attached to a tuplet member note JMI
  if (! fCurrentNoteIsAGraceNote) {
    handleTupletsPendingOnTupletsStack (
      inputLineNumber);
  }
  */

  if (fCurrentNoteIsAGraceNote) {
    if (! fPendingGraceNotesGroup) {
      // this is the first grace note in grace notes

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
        gLogStream <<
          "Creating grace notes for note '" <<
          newNote->asString () <<
          "' in voice \"" <<
          currentVoice->getVoiceName () << "\"" <<
          std::endl;
      }
#endif

      // create grace notes group
      fPendingGraceNotesGroup =
        msrGraceNotesGroup::create (
          inputLineNumber,
          msrGraceNotesGroupKind::kGraceNotesGroupBefore, // default value
          fCurrentGraceIsSlashed,
          fCurrentGraceIsBeamed,
          fCurrentMeasureNumber);

      // should all grace notes be slurred?
      if (gGlobalMxsr2msrOahGroup->getSlurAllGraceNotes ()) {
        fPendingGraceNotesGroup->
          setGraceNotesGroupIsTied ();
      }

/* JMI
      // register that last handled note if any is followed by grace notes
      S_msrNote
        lastHandledNoteInVoice =
          currentVoice->
            getVoiceLastAppendedNote ();

      if (lastHandledNoteInVoice) {
        lastHandledNoteInVoice->
          setNoteIsFollowedByGraceNotes ();
      }
*/

      // append the grace notes to the current voice // NO JMI
      /*
      currentVoice->
        appendGraceNotesToVoice (
          fCurrentGraceNotes);
        //  */
    }

    // register that last handled note if any is followed by grace notes JMI ???
    S_msrNote
      lastHandledNoteInVoice =
        currentVoice->
          getVoiceLastAppendedNote ();

    if (lastHandledNoteInVoice) {
      lastHandledNoteInVoice->
        setNoteIsFollowedByGraceNotesGroup ();
    }

    // append newNote to the current grace notes
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Appending note " <<
        newNote->asString () <<
        " to grace notes in voice \"" <<
        currentVoice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;
    }
#endif

    // append newNote to the pending grace notes group
    fPendingGraceNotesGroup->
      appendNoteToGraceNotesGroup (newNote);
  }

  else if (fCurrentDoubleTremoloTypeKind != msrDoubleTremoloTypeKind::kDoubleTremoloType_NO_) {
    // newNote belongs to a tremolo

    switch (fCurrentDoubleTremoloTypeKind) {
      case msrDoubleTremoloTypeKind::kDoubleTremoloType_NO_:
        // just to avoid a compiler message
        break;

      case msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle:
        // append newNote to the current voice
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceNotes ()) {
          gLogStream <<
            "Appending single tremolo " <<
            newNote->asString () <<
            ", line " << newNote->getInputLineNumber () <<
            ", to voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            std::endl;
        }
#endif

        currentVoice->
          appendNoteToVoice (newNote);

        // fCurrentSingleTremolo is handled in
        // attachCurrentSingleTremoloToNote()
        break;

      case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart:
        // register newNote as first element of the current double tremolo
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceNotes ()) {
          gLogStream <<
            "Setting regular note '" <<
            newNote->asString () <<
            "', line " << newNote->getInputLineNumber () <<
            ", as double tremolo first element" <<
            " in voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            std::endl;
        }
#endif

        fCurrentDoubleTremolo->
          setDoubleTremoloNoteFirstElement (
            newNote);
        break;

      case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop:
        // register newNote as second element of the current double tremolo
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceNotes ()) {
          gLogStream <<
            "Setting regular note '" <<
            newNote->asString () <<
            "', line " << newNote->getInputLineNumber () <<
            ", as double tremolo second element" <<
            " in voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            std::endl;
        }
#endif

        fCurrentDoubleTremolo->
          setDoubleTremoloNoteSecondElement (
            newNote);

        // append current double tremolo to current voice
        currentVoice->
          appendDoubleTremoloToVoice (
            fCurrentDoubleTremolo);

        // forget about the current double tremolo
       // fCurrentDoubleTremolo = 0; // JMI not if there's a chord in the double tremolo XXL BOF
        break;
    } // switch
  }

  else {
    // regular note or rest

    // append newNote to the current voice
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "Appending regular note or rest " <<
        newNote->asString () <<
        ", line " << newNote->getInputLineNumber () <<
        ", to voice \"" <<
        currentVoice->getVoiceName () <<
        "\"" <<
        std::endl;
    }
#endif

    ++gIndenter;

    currentVoice->
      appendNoteToVoice (newNote);

    if (false) { // XXL, syllable sans fSyllableNote assigne JMI
      gLogStream <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        std::endl <<
        currentVoice <<
        std::endl;
    }

    --gIndenter;
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
    case msrSlurTypeKind::kSlurType_NO_:
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
void mxsr2msrTranslator::handleLyricsForNoteAfterNoteItselfIsHandled (
  const S_msrVoice& currentVoice,
  S_msrNote  newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Handling lyrics for note '" <<
      newNote->asShortString () <<
      "', line " << inputLineNumber <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 33;

    gLogStream <<
      std::setw (fieldWidth) <<
      "currentVoice" << " = \"" << currentVoice->getVoiceName () <<"\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "newNote" << " = \"" << newNote->asShortString () << "\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fLastHandledNoteInVoiceHasLyrics" << " = " <<
      fLastHandledNoteInVoiceHasLyrics <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentSyllableExtendKind" << "" << " = " <<
      msrSyllableExtendKindAsString (
        fCurrentSyllableExtendKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fOnGoingSyllableExtend" << " = " <<
      fOnGoingSyllableExtend <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteHasLyrics" << " = " <<
      fCurrentNoteHasLyrics <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fASkipSyllableHasBeenGeneratedForcurrentNote" << " = " <<
      fASkipSyllableHasBeenGeneratedForcurrentNote <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStanzaName" << " = " << fCurrentStanzaName << "\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentLyricTextsList" << " = ";

    msrSyllable::writeTextsList (
      fCurrentLyricTextsList,
      gLogStream);

    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  if (fCurrentNoteHasLyrics) {
    // newNote has lyrics attached to it
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceLyricsDetails ()) {
      gLogStream <<
        "Note '" <<
        newNote->asShortString () <<
        "' has lyrics attached to it" << " ***" <<
        std::endl;
    }
#endif

    for (
      std::list<S_msrSyllable>::const_iterator i =
        fCurrentNoteSyllables.begin ();
      i != fCurrentNoteSyllables.end ();
      ++i ) {
      S_msrSyllable
        syllable = (*i);

      // set syllable note upLink to newNote
      syllable->
        appendSyllableToNoteAndSetItsUpLinkToNote (
          newNote);
    } // for

    // forget about all of newNote's syllables
    fCurrentNoteSyllables.clear ();
  }

  else {
    // newNote has no lyrics attached to it:
    // don't create a skip for chord note members except the first
    // nor for grace notes

    Bool doCreateASkipSyllable =
     // ! fASkipSyllableHasBeenGeneratedForcurrentNote; JMI
      ! fCurrentNoteHasLyrics;

    switch (fCurrentSyllableExtendKind) {
      case msrSyllableExtendKind::kSyllableExtendNone:
  //      doCreateASkipSyllable = true; // JMI
        break;
      case msrSyllableExtendKind::kSyllableExtendEmpty:
  //      doCreateASkipSyllable = true; // JMI
        break;
      case msrSyllableExtendKind::kSyllableExtendSingle:
        break;
      case msrSyllableExtendKind::kSyllableExtendStart:
        break;
      case msrSyllableExtendKind::kSyllableExtendContinue:
 //       doCreateASkipSyllable = true; // JMI
        break;
      case msrSyllableExtendKind::kSyllableExtendStop:
        break;
    } // switch

    if (doCreateASkipSyllable) {
      if (
        ! (fCurrentNoteBelongsToAChord || fCurrentNoteIsAGraceNote)
      ) {
        // get the current voice's stanzas std::map
        const std::map<std::string, S_msrStanza>&
          voiceStanzasMap =
            currentVoice->
              getVoiceStanzasMap ();

        for (
          std::map<std::string, S_msrStanza>::const_iterator i = voiceStanzasMap.begin ();
          i != voiceStanzasMap.end ();
          ++i
        ) {
          const S_msrStanza& stanza = (*i).second;

          //choose the syllable kind
          msrSyllableKind
            syllableKind =
            fCurrentNoteIsARest
              ? msrSyllableKind::kSyllableSkipRestNote
              : msrSyllableKind::kSyllableSkipNonRestNote;

          // create a skip syllable
          S_msrSyllable
            syllable =
              msrSyllable::create (
                inputLineNumber,
                syllableKind,
                fCurrentSyllableExtendKind,
                fCurrentStanzaNumber,
                fCurrentNoteSoundingWholeNotesFromDuration,
                msrTupletFactor (
                  fCurrentNoteActualNotes,
                  fCurrentNoteNormalNotes),
                stanza);

          // set syllable note upLink to newNote
          syllable->
            appendSyllableToNoteAndSetItsUpLinkToNote (
              newNote);

          // append syllable to stanza
          stanza->
            appendSyllableToStanza (syllable);
        } // for
      }
    }
  }

  // register whether the new last handled note has lyrics
  fLastHandledNoteInVoiceHasLyrics =
    fCurrentNoteHasLyrics;

  // take care of ongoing extends
  switch (fCurrentSyllableExtendKind) {
    case msrSyllableExtendKind::kSyllableExtendNone:
      break;
    case msrSyllableExtendKind::kSyllableExtendEmpty: // JMI ???
      break;
    case msrSyllableExtendKind::kSyllableExtendSingle:
      fOnGoingSyllableExtend = true;
      break;
    case msrSyllableExtendKind::kSyllableExtendStart:
      fOnGoingSyllableExtend = true;
      break;
    case msrSyllableExtendKind::kSyllableExtendContinue:
      mfAssert (
        __FILE__, __LINE__,
        fOnGoingSyllableExtend,
        "fOnGoingSyllableExtend is false");
      break;
    case msrSyllableExtendKind::kSyllableExtendStop:
      fOnGoingSyllableExtend = false;
      break;
  } // switch
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleNoteBelongingToAChord (
  const S_msrNote& newChordNote)
{
  int inputLineNumber =
    newChordNote->getInputLineNumber ();

  // set newChordNote kind as a chord member
  newChordNote->
    setNoteKind (
      msrNoteKind::kNoteRegularInChord);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Handling a note belonging to a chord" <<
      ", newChordNote:" <<
      std::endl;

    ++gIndenter;

    gLogStream << newChordNote <<
      std::endl;

    --gIndenter;
  }
#endif

  if (fCurrentNoteIsARest) {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "a rest cannot belong to a chord");
  }

  // fetch current voice
  int staffNumberToUse = -1001;
    // to avoid compiler warning for uninitialized variable

  switch (fCurrentStaffChangeKind) {
    case msrStaffChangeKind::kStaffChange_NO_:
      staffNumberToUse =
        fCurrentStaffNumberToInsertInto; // JMI fCurrentMusicXMLStaffNumber;
      break;
    case msrStaffChangeKind::kStaffChangeChordMemberNote:
      if (fCurrentNoteIsCrossStaves) {
        staffNumberToUse =
          fCurrentStaffNumberToInsertInto;
    // JMI    staffNumberToUse = fCurrentMusicXMLStaffNumber; // keep it!
      }
      else {
   //   JMI   staffNumberToUse = fCurrentChordStaffNumber;
        staffNumberToUse =
          fCurrentStaffNumberToInsertInto;
      }
      break;
    case msrStaffChangeKind::kStaffChangeOtherNote:
      staffNumberToUse =
        fCurrentStaffNumberToInsertInto;
     // JMI staffNumberToUse = fCurrentMusicXMLStaffNumber;
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream << // JMI
      std::endl <<
      "***==> fCurrentStaffNumberToInsertInto: " <<
      fCurrentStaffNumberToInsertInto <<
      ", fCurrentChordStaffNumber: " <<
      fCurrentChordStaffNumber <<
      ", fPreviousNoteMusicXMLStaffNumber: " <<
      fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      ", staffNumberToUse: " <<
      staffNumberToUse <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        staffNumberToUse,
        fCurrentMusicXMLVoiceNumber);

  // sanity check JMI ???
  mfAssert (
    __FILE__, __LINE__,
    currentVoice != nullptr,
    "currentVoice is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Handling a chord member note" <<
      ", currentVoice = \"" <<
      currentVoice->getVoiceName () <<
      "\", fOnGoingChord: " <<
      fOnGoingChord <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChordsDetails ()) {
    gLogStream <<
      std::endl <<
      "======================= handleNoteBelongingToAChord" <<
      ", line " << inputLineNumber <<
      std::endl;
    fCurrentPart->print (gLogStream);
    gLogStream <<
      "=======================" <<
      std::endl << std::endl;

/* JMI
    if (fCurrentGraceNotes) {
      gLogStream <<
        fCurrentGraceNotes;
    }
    else {
//       gLogStream <<
//         "fCurrentGraceNotes is NULL"; // JMI
    }
*/

    gLogStream << std::endl;
  }
#endif

  // should a chord be created?
  if (! fOnGoingChord) {
    // newChordNote is the second note of the chord to be created

    // fetch this chord's first note,
    // i.e the last handled note for this voice

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceChords ()) {
      printVoicesLastMetNoteMap (
        inputLineNumber,
        "handleNoteBelongingToAChord()");
    }
#endif

    S_msrNote
      chordFirstNote =
  //      fVoicesLastMetNoteMap [currentVoice];
        fVoicesLastMetNoteMap [
          std::make_pair (
            fCurrentStaffNumberToInsertInto,
            fCurrentMusicXMLVoiceNumber)
          ];
      /*
        currentVoice->
          getVoiceLastAppendedNote (); ??? JMI
          */

    if (! chordFirstNote) {
      std::stringstream s;

      s <<
        "handleNoteBelongingToAChord():" <<
        std::endl <<
        "chordFirstNote is null on " <<
        newChordNote->asString () <<
        std::endl <<
        "fCurrentMusicXMLStaffNumber: " << fCurrentMusicXMLStaffNumber <<
        std::endl <<
        "fCurrentMusicXMLVoiceNumber: " << fCurrentMusicXMLVoiceNumber;

      mxsr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceChords ()) {
      gLogStream <<
        "mxsr2msrTranslator::handleNoteBelongingToAChord()" <<
        ", chordFirstNote:" <<
        std::endl;

      ++gIndenter;

      gLogStream <<
        chordFirstNote <<
        std::endl;

      --gIndenter;
    }
#endif

    // fetch chord first note's kind before createChordFromItsFirstNote(),
    // because the latter will change it to kChordMemberNote or msrNoteKind::kNoteInChordInGraceNotesGroup
    msrNoteKind
      savedChordFirstNoteKind =
        chordFirstNote->getNoteKind ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceChords ()) {
      gLogStream <<
        "Handling a note belonging to a chord" <<
        ", savedChordFirstNoteKind: " <<
        msrNoteKindAsString (savedChordFirstNoteKind) <<
        std::endl;

      ++gIndenter;

      gLogStream <<
        chordFirstNote <<
        std::endl;

      --gIndenter;
    }
#endif

    // create the current chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        inputLineNumber,
        currentVoice,
        chordFirstNote,
        msrNoteKind::kNoteRegularInChord);

    fPreviousMeasureElement = fCurrentChord;

    // handle chord's first note
    switch (savedChordFirstNoteKind) {
      case msrNoteKind::kNoteRestInMeasure:
        break;

      case msrNoteKind::kNoteSkipInMeasure:
        break;

      case msrNoteKind::kNoteUnpitchedInMeasure:
      case msrNoteKind::kNoteRegularInMeasure:
        // remove last handled (previous current) note from the current voice
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceChords ()) {
          gLogStream <<
            "Removing chord first note " <<
            chordFirstNote->asShortString () <<
            ", line " << inputLineNumber <<
            ", from voice \"" << currentVoice->getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceChordsDetails ()) {
          gLogStream <<
            std::endl << std::endl <<
            "&&&&&&&&&&&&&&&&&& fCurrentPart contents &&&&&&&&&&&&&&&&&&" <<
            std::endl <<
            fCurrentPart <<
            std::endl << std::endl;
        }
#endif

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceStaffChanges ()) {
          gLogStream << // JMI
            std::endl <<
            "***==> fCurrentStaffNumberToInsertInto: " <<
            fCurrentStaffNumberToInsertInto <<
            ", fPreviousNoteMusicXMLStaffNumber: " <<
            fPreviousNoteMusicXMLStaffNumber <<
            ", fCurrentMusicXMLStaffNumber: " <<
            fCurrentMusicXMLStaffNumber <<
            "', line " << inputLineNumber <<
            std::endl;
        }
#endif

        currentVoice->
          removeNoteFromVoice (
            inputLineNumber,
            chordFirstNote);

        // add chord to the voice instead
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceChords ()) {
          gLogStream <<
            "Appending chord " << fCurrentChord->asString () <<
            " to voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            std::endl;
        }
#endif

        currentVoice->
          appendChordToVoice (
            fCurrentChord);
        break;

      case msrNoteKind::kNoteInDoubleTremolo:
        {
          /* JMI
          // fetch chordFirstNote's sounding divisions
          int chordFirstNoteSoundingWholeNotes = // JMI
            chordFirstNote->
              getMeasureElementSoundingWholeNotes ();
              */

          /* JMI
          // updating chord's divisions // JMI
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceChords ()) {
            gLogStream <<
              "Updating sounding divisions for double tremolo chord '" <<
              "' " << chord->asString () <<
              " to '" << chordFirstNoteSoundingWholeNotes <<
              "' in voice \"" <<
              currentVoice->getVoiceName () <<
              "\"" <<
              std::endl;
          }
#endif

          chord->
            setMeasureElementSoundingWholeNotes ( // ??? JMI
              chordFirstNoteSoundingWholeNotes,
              "mxsr2msrTranslator::handleNoteBelongingToAChord()");
              */

          if (chordFirstNote->getNoteIsFirstNoteInADoubleTremolo ()) {
            // replace double tremolo's first element by chord
            fCurrentDoubleTremolo->
              setDoubleTremoloChordFirstElement (
                fCurrentChord);
          }

          else if (chordFirstNote->getNoteIsSecondNoteInADoubleTremolo ()) {
            // replace double tremolo's second element by chord
            fCurrentDoubleTremolo->
              setDoubleTremoloChordSecondElement (
                fCurrentChord);
          }

          else {
            std::stringstream s;

            s <<
              "chord first note '" <<
              chordFirstNote->asShortString () <<
              "' belongs to a double tremolo, but is not marked as such";

            mxsr2msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
        }
        break;

      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
      case msrNoteKind::kNoteInChordInGraceNotesGroup:
      /* JMI
        // remove last handled (previous current) note from the current voice
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceChords ()) {
          gLogStream <<
            "Removing chord first note " <<
            chordFirstNote->asShortString () <<
            ", line " << inputLineNumber <<
            ", from voice \"" << currentVoice->getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        currentVoice->
          removeNoteFromVoice (
            inputLineNumber,
            chordFirstNote);
*/
        break;

      case msrNoteKind::kNoteRegularInChord:
        // error? JMI
        break;

      case msrNoteKind::kNoteRegularInTuplet:
      case msrNoteKind::kNoteRestInTuplet:
      case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      case msrNoteKind::kNoteUnpitchedInTuplet:
        break;

      case msrNoteKind::kNote_NO_:
        break;
    } // switch

    // account for chord being built
    fOnGoingChord = true;
  }

  else {
    // is newChordNote in the same chord but in another voice,
    // implying it is a member of a crossstaff chord?

/* JMI
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceChords ()) {
      gLogStream <<
        "is newChordNote in the same chord but in another voice?" <<
        ", currentVoice: " <<
        currentVoice->getVoiceName () <<
        std::endl;

      printVoicesCurrentChordMap ();
    }
#endif
*/

  // register newChordNote as another member of chord
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceChords ()) {
      gLogStream <<
        "Adding another note " <<
        newChordNote->asString() <<
        ", line " << inputLineNumber <<
        " to current chord in voice " <<
        currentVoice->getVoiceName () <<
        std::endl;
    }
#endif
  }

  // add newChordNote to current chord
  fCurrentChord->
    addAnotherNoteToChord (
      newChordNote, currentVoice);

  // copy newChordNote's elements if any to the current chord
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::handlePendingTupletStopIfAny (
  int       inputLineNumber,
  const S_msrNote& note)
{
  if (fCurrentATupletStopIsPending) {
    // finalize the tuplet, only now
    // in case the last element is a chord
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "--> kTupletTypeStart: handling pending tuplet stop, note '" <<
        note->
          asShortString () <<
        "', line " << inputLineNumber <<
        std::endl;
    }
#endif

    finalizeTupletAndPopItFromTupletsStack (
      inputLineNumber);

    // the tuplet stop is not to be handled later
    fCurrentATupletStopIsPending = false;
  }
}

void mxsr2msrTranslator::handleNoteBelongingToATuplet (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

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

  if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
    // no duration has been found,
    // determine sounding from display whole notes
    note->
      determineTupletMemberSoundingFromDisplayWholeNotes (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Handling a note belonging to a tuplet" <<
      ", note: " <<
      note->
        asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // is there an ongoing chord?
  if (! fOnGoingChord) {
    // note is the first one after a chord in a tuplet,
    // JMI
  }

  switch (fCurrentTupletTypeKind) {
    case msrTupletTypeKind::kTupletTypeStart:
      {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTuplets ()) {
          gLogStream <<
            "--> kTupletTypeStart: note = '" <<
            note->
              asShortString () <<
            "', line " << inputLineNumber <<
            std::endl;
        }
#endif

        handlePendingTupletStopIfAny (
          inputLineNumber,
          note);
        /* JMI
        if (fCurrentATupletStopIsPending) {
          // finalize the tuplet, only now
          // in case the last element is a chord
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceTuplets ()) {
            gLogStream <<
              "--> kTupletTypeStart: handling pending tuplet stop, note '" <<
              note->
                asShortString () <<
              "', line " << inputLineNumber <<
              std::endl;
          }
#endif

          finalizeTupletAndPopItFromTupletsStack (
            inputLineNumber);

          // the tuplet stop is not to be handled later
          fCurrentATupletStopIsPending = false;
        }
        */

        // create the tuplet
        createTupletWithItsFirstNoteAndPushItToTupletsStack (
          note);

        // swith to continuation mode
        // this is handy in case the forthcoming tuplet members
        // are not explictly of the "continue" type
        fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeContinue;
      }
      break;

    case msrTupletTypeKind::kTupletTypeContinue:
      {
        if (fTupletsStack.size ()) {
          S_msrTuplet
            currentTuplet =
              fTupletsStack.front ();

          // populate the tuplet at the top of the std::stack
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceTuplets ()) {
            gLogStream <<
              "--> kTupletTypeContinue: adding tuplet member note '" <<
              note->
                asShortString () <<
              "' to std::stack top tuplet '" <<
              currentTuplet->asString () <<
              "', line " << inputLineNumber <<
              std::endl;
          }
#endif

          // fetch current voice
          S_msrVoice
            currentVoice =
              fetchVoiceFromCurrentPart (
                inputLineNumber,
                fCurrentStaffNumberToInsertInto, // fCurrentMusicXMLStaffNumber,
                fCurrentMusicXMLVoiceNumber);

          // add note to tuplet
          currentTuplet->
            appendNoteToTuplet (
              note,
              currentVoice);

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
            displayTupletsStack (
              "############## kTupletTypeContinue");
          }
#endif

/* JMI
          // set note displayed divisions
          note->
            applyTupletMemberDisplayFactor (
              fTupletsStack.front ()->getTupletActualNotes (),
              fTupletsStack.front ()->getTupletNormalNotes ());
*/
        }

        else {
          std::stringstream s;

          s <<
            "handleNoteBelongingToATuplet():" <<
            std::endl <<
            "tuplet member note " <<
            note->
              asShortString () <<
            " cannot be added, tuplets std::stack is empty";

          mxsr2msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }
      break;

    case msrTupletTypeKind::kTupletTypeStop:
      {
        switch (fTupletsStack.size ()) {
          case 0:
            {
              std::stringstream s;

              s <<
                "handleNoteBelongingToATuplet():" <<
                std::endl <<
                "tuplet member note " <<
                note->
                  asShortString () <<
                " cannot be added, tuplets std::stack is empty";

              mxsr2msrInternalError (
                gGlobalServiceRunData->getInputSourceName (),
                inputLineNumber,
                __FILE__, __LINE__,
                s.str ());
            }
            break;

          case 1:
            {
              // outer-most tuplet:
              // add the note to to it before finalizing it
              S_msrTuplet
                currentTuplet =
                  fTupletsStack.front ();

              // populate the tuplet at the top of the std::stack
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceTuplets ()) {
                gLogStream <<
                  "--> kTupletTypeStop: adding outer-most tuplet member note " <<
                  note->
                    asShortString () <<
                  "' to std::stack top tuplet '" <<
                  currentTuplet->asString () <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif

              // fetch current voice
              S_msrVoice
                currentVoice =
                  fetchVoiceFromCurrentPart (
                    inputLineNumber,
                    fCurrentStaffNumberToInsertInto, // fCurrentMusicXMLStaffNumber,
                    fCurrentMusicXMLVoiceNumber);

              // add note to tuplet
              currentTuplet->
                appendNoteToTuplet (
                  note,
                  currentVoice);

#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
                displayTupletsStack (
                  "############## kTupletTypeStop, outer-most");
              }
#endif

    //* JMI
              if (fCurrentATupletStopIsPending) {
                // end of a tuplet forces handling of the pending one
#ifdef TRACING_IS_ENABLED
                if (gGlobalTracingOahGroup->getTraceTuplets ()) {
                  gLogStream <<
                    "--> kTupletTypeStop: finalizing pending tuplet" <<
                    ", line " << inputLineNumber <<
                    std::endl;
                }
#endif

                finalizeTupletAndPopItFromTupletsStack (
                  inputLineNumber);

                // the tuplet stop is not to be handled later
                fCurrentATupletStopIsPending = false;
              }
              else {
#ifdef TRACING_IS_ENABLED
                if (gGlobalTracingOahGroup->getTraceTuplets ()) {
                  gLogStream <<
                    "--> kTupletTypeStop: finalizing non-pending tuplet" << // JMI ???
                    ", line " << inputLineNumber <<
                    std::endl;
                }
#endif

                finalizeTupletAndPopItFromTupletsStack (
                  inputLineNumber);
              }
    //*/

              // don't pop the inner-most tuplet from the std::stack yet

        //      fCurrentATupletStopIsPending = true;
            }
            break;

          default:
            {
              // nested tuplet:
              // finalize it before adding the note to the containing tuplet
              handlePendingTupletStopIfAny (
                inputLineNumber,
                note);
              /* JMI
              if (fCurrentATupletStopIsPending) {
                // end of a tuplet forces handling of the pending one
#ifdef TRACING_IS_ENABLED
                if (gGlobalTracingOahGroup->getTraceTuplets ()) {
                  gLogStream <<
                    "--> kTupletTypeStop: finalizing tuplet" <<
                    ", line " << inputLineNumber <<
                    std::endl;
                }
#endif

                finalizeTupletAndPopItFromTupletsStack (
                  inputLineNumber);

                // the tuplet stop is not to be handled later
                fCurrentATupletStopIsPending = false;
              }
              */

              S_msrTuplet
                currentTuplet =
                  fTupletsStack.front ();

              // populate the tuplet at the top of the std::stack
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceTuplets ()) {
                gLogStream <<
                  "--> kTupletTypeStop: adding nested tuplet member note " <<
                  note->
                    asShortString () <<
                  "' to std::stack top tuplet '" <<
                  currentTuplet->asString () <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif

              // fetch current voice
              S_msrVoice
                currentVoice =
                  fetchVoiceFromCurrentPart (
                    inputLineNumber,
                    fCurrentStaffNumberToInsertInto, // fCurrentMusicXMLStaffNumber,
                    fCurrentMusicXMLVoiceNumber);

              // add note to tuplet
              currentTuplet->
                appendNoteToTuplet (
                  note,
                  currentVoice);

#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
                displayTupletsStack (
                  "############## kTupletTypeStop, nested");
              }
#endif

      /* JMI
              // set note displayed divisions
              note->
                applyTupletMemberDisplayFactor (
                  fTupletsStack.front ()->getTupletActualNotes (),
                  fTupletsStack.front ()->getTupletNormalNotes ());
    */
            }
        } // switch

        // finalizeTupletAndPopItFromTupletsStack() should be delayed
        // in case this note
        // is the first one of a chord in a tuplet JMI XXL ???

        // the tuplet stop hast to be handled later
        fCurrentATupletStopIsPending = false; // JMI ???
      }
      break;

    case msrTupletTypeKind::kTupletTypeStartAndStopInARow:
      {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceTuplets ()) {
          gLogStream <<
            "--> kTupletTypeStartAndStopInARow: note = '" <<
            note->
              asShortString () <<
            "', line " << inputLineNumber <<
            std::endl;
        }
#endif

        if (fCurrentSingleTremolo) {
          fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStartAndStopInARow;
        }
        else {
          std::stringstream s;

          s <<
            "one-note tuplet with a non single tremolo contents found";

     // JMI     mxsr2msrError (
          mxsr2msrWarning (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
       //     __FILE__, __LINE__,
            s.str ());
        }

        // create the tuplet
        createTupletWithItsFirstNoteAndPushItToTupletsStack (
          note);

        // the tuplet stop is not to be handled later
 //       fCurrentATupletStopIsPending = false; // JMI

        // finalize it
        finalizeTupletAndPopItFromTupletsStack (
          inputLineNumber);
      }
      break;

    case msrTupletTypeKind::kTupletTypeNone:
      break;
  } // switch

/* JMI ???
  // forget about this tuplet type, needed for nested tuplets
  fCurrentTupletTypeKind =
    msrTupletTypeKind::kTupletTypeNone;
    */
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleNoteBelongingToAChordInATuplet (
  const S_msrNote& newChordNote)
{
  /*
   The first note of a chord belonging to a tuplet
   is marked in MusicXML as a tuplet member only,
   it has already been appended to the voice in
   handleNonChordNorTupletNoteOrRest(),
   and the following ones are marked as both a tuplet and a chord member
  */

  int inputLineNumber =
    newChordNote->getInputLineNumber ();

  // set new note kind as a chord or grace chord member JMI ???
  newChordNote->
    setNoteKind (
      msrNoteKind::kNoteRegularInChord);

  // apply tuplet sounding factor to note
  if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
    // no duration has been found,
    // determine sounding from display whole notes
    newChordNote->
      determineTupletMemberSoundingFromDisplayWholeNotes (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Handling a note belonging to a chord in a tuplet" <<
      ", newChordNote: " <<
      newChordNote->
        asShortString () <<
      std::endl;
  }
#endif

  if (fCurrentNoteIsARest) {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "a rest cannot belong to a chord in a tuplet");
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);

  // should a chord be created?
  if (! fOnGoingChord) {
    // this is the second note of the chord to be created,

    // fetch the current tuplet, i.e. the top of the std::stack
    S_msrTuplet currentTuplet;

    /* JMI
    if (fTupletsStack.size ()) {
      currentTuplet =
        fTupletsStack.front ();
    }
    else {
      std::stringstream s;

      s <<
        "handleNoteBelongingToAChordInATuplet():" <<
        std::endl <<
        " a tuplet member chord " <<
        "cannot be added, tuplets std::stack is empty";

      mxsr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
    */

    currentTuplet =
      fLastHandledTupletInVoiceMap [
        std::make_pair (
          fCurrentStaffNumberToInsertInto,
          fCurrentMusicXMLVoiceNumber)
      ];

    // remove and fetch tupletLastNote from the current tuplet,
    // it will be the first chord member note
    S_msrNote
      tupletLastNote =
        currentTuplet->
          removeLastNoteFromTuplet (
            inputLineNumber);

/* JMI
    S_msrNote
      tupletLastNote =
  //      fVoicesLastMetNoteMap [currentVoice];
        fVoicesLastMetNoteMap [
          std::make_pair (fCurrentMusicXMLStaffNumber, fCurrentMusicXMLVoiceNumber)
          ];

    currentVoice->
      removeNoteFromVoice (
        inputLineNumber,
        tupletLastNote);
*/

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
      displayTupletsStack (
        "############## After  removeLastNoteFromTuplet()");
    }
#endif

    // create the current chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        inputLineNumber,
        currentVoice,
        tupletLastNote,
        msrNoteKind::kNoteRegularInChord);

    if (false) { // JMI
      gLogStream <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        std::endl <<
        currentVoice <<
        std::endl;
    }

    // add chord to the current tuplet instead of tupletLastNote
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceChords () || gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "Adding chord '" <<
        fCurrentChord->asString () <<
        "' to std::stack top tuplet '" <<
        currentTuplet->asString () <<
        "', line " << inputLineNumber <<
        std::endl;
    }
#endif

    currentTuplet->
      appendChordToTuplet (fCurrentChord);

    if (fCurrentNoteSoundingWholeNotesFromDuration.getNumerator () == 0) {
      // no duration has been found,
      // determine sounding from display whole notes
      newChordNote->
        determineTupletMemberSoundingFromDisplayWholeNotes (
          fCurrentNoteActualNotes,
          fCurrentNoteNormalNotes);
    }

    // account for a chord being built
    fOnGoingChord = true;
  }

  // register note as another member of chord
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Adding another note " <<
      newChordNote->
        asShortString () <<
      ", line " << inputLineNumber <<
      " to current chord in voice " <<
      currentVoice->getVoiceName () <<
      std::endl;
  }
#endif

  fCurrentChord->
    addAnotherNoteToChord (
      newChordNote, currentVoice);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleNoteBelongingToAChordInAGraceNotesGroup (
  const S_msrNote& newChordNote)
{
  /*
   The first note of a chord belonging to a tuplet
   is marked in MusicXML as a tuplet member only,
   it has already been appended to the voice in
   handleNonChordNorTupletNoteOrRest(),
   and the following ones are marked as both a tuplet and a chord member
  */

  int inputLineNumber =
    newChordNote->getInputLineNumber ();

  // set new note kind as a grace chord member
  newChordNote->
    setNoteKind (
      msrNoteKind::kNoteInChordInGraceNotesGroup);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords () || gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Handling a note belonging to a chord in grace notes" <<
      ", newChordNote is '" <<
      newChordNote->
        asShortString () <<
      "'" <<
      std::endl;
  }
#endif

  if (fCurrentNoteIsARest) {
    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "a rest cannot belong to a chord in a grace notes group");
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto,
        fCurrentMusicXMLVoiceNumber);

  // should a chord be created?
  if (! fOnGoingChord) {
    // this is the second note of the chord to be created

    S_msrNote chordFirstNote;

    if (fPendingGraceNotesGroup) {
      // the first note of the chord is currently the last one of fPendingGraceNotesGroup
      chordFirstNote =
        fPendingGraceNotesGroup->
          removeLastNoteFromGraceNotesGroup (
            inputLineNumber);
    }

    else {
      // the last handled note for the current voice
      // is the first note of the chord

      // fetch last handled note for this voice
      chordFirstNote =
        currentVoice->
          getVoiceLastAppendedNote ();

      if (! chordFirstNote) {
        std::stringstream s;

        s <<
          "handleNoteBelongingToAChordInGraceNotes():" <<
          std::endl <<
          "chordFirstNote is null on " <<
          newChordNote->asString ();

        mxsr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
          s.str ());
      }
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceChords () || gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "The grace notes chord's first note is '" <<
        chordFirstNote->
          asShortString () <<
        "'" <<
        std::endl;
    }
#endif

    // set the first note's kind as grace chord member
    chordFirstNote->
      setNoteKind (
        msrNoteKind::kNoteInChordInGraceNotesGroup);

    // create the current chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        inputLineNumber,
        currentVoice,
        chordFirstNote,
        msrNoteKind::kNoteInChordInGraceNotesGroup);

    if (false) {
      gLogStream <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        std::endl <<
        currentVoice <<
        std::endl << std::endl;
    }

    if (fPendingGraceNotesGroup) {
      // append current chord to pending grace notes
      fPendingGraceNotesGroup->
        appendChordToGraceNotesGroup (
          fCurrentChord);
    }
    else {
      // append current chord to pending voice JMI ???
      currentVoice->
        appendChordToVoice (
          fCurrentChord);
    }


    /* JMI
    else {
      std::stringstream s;

      s <<
        "handleNoteBelongingToAChordInGraceNotes():" <<
        std::endl <<
        "tuplet member chord " << chord->asString () <<
        "cannot be added, tuplets std::stack is empty";

      mxsr2msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
    */

    // account for chord being built
    fOnGoingChord = true;
  }

  // register note as another member of chord
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Adding another note " <<
      newChordNote->
        asShortString () <<
      ", line " << inputLineNumber <<
      " to current chord in voice " <<
      currentVoice->getVoiceName () <<
      std::endl;
  }
#endif

  fCurrentChord->
    addAnotherNoteToChord (
      newChordNote, currentVoice);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleTupletsPendingOnTupletsStack (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Handling tuplets pending on tuplet std::stack" <<
      ", fCurrentStaffNumberToInsertInto: " <<
      fCurrentStaffNumberToInsertInto <<
      ", fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      ", line: " << inputLineNumber <<
      std::endl;
  }

  if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## Before  handleTupletsPendingOnTupletsStack()");
  }
#endif

/* superfluous JMI
  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentStaffNumberToInsertInto, // JMI fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);
*/

  // handle tuplets pending on the tuplet std::stack
  while (fTupletsStack.size ()) {
    S_msrTuplet
      pendingTuplet =
        fTupletsStack.front ();

    // finalize the tuplet, thus popping it off the std::stack
    finalizeTupletAndPopItFromTupletsStack (
      inputLineNumber);
  } // while

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## Before  handleTupletsPendingOnTupletsStack()");
  }
#endif
}

void mxsr2msrTranslator::displayLastHandledTupletInVoiceMap (const std::string& header)
{
  gLogStream <<
    std::endl <<
    header <<
    ", fLastHandledTupletInVoiceMap contains:";

  if (! fLastHandledTupletInVoiceMap.size ()) {
    gLogStream <<
      " [NONE]" <<
      std::endl;
  }

  else {
    std::map<std::pair<int, int>, S_msrTuplet>::const_iterator
      iBegin = fLastHandledTupletInVoiceMap.begin (),
      iEnd   = fLastHandledTupletInVoiceMap.end (),
      i      = iBegin;

    gLogStream << std::endl;

    ++gIndenter;

    for ( ; ; ) {
      gLogStream <<
        "staff " << (*i).first.first <<
        ", voice " <<  (*i).first.second <<
        std::endl;
//        "\"" << (*i).first->getVoiceName () <<
//        "\" ----> " << (*i).second->asString ();
      if (++i == iEnd) break;
      gLogStream << std::endl;
    } // for

    gLogStream << std::endl;

    --gIndenter;
  }

  gLogStream << std::endl;
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleRepeatStart (
  S_msrBarLine& barLine)
{
  int inputLineNumber =
    barLine->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start" <<
    /* JMI
      ", measure '" <<
        barLine->getBarLineMeasureNumber () <<
      "', position " <<
      barLine->getBarLineMeasurePosition () <<
      */
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // remember repeat start measure number
  fCurrentRepeatStartMeasureNumber = inputLineNumber;

  // prepare for repeat in current part
  fCurrentPart->
    handleRepeatStartInPart (
      inputLineNumber);

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleRepeatEnd (
  S_msrBarLine& barLine)
{
  int inputLineNumber =
    barLine->getInputLineNumber ();

  std::string repeatStartMeasureNumber =
    fCurrentRepeatStartMeasureNumber.size ()
      ? // there was a repeat start before hand
        fCurrentRepeatStartMeasureNumber
      : // there is an implicit repeat start at the beginning of the part
        fPartFirstMeasureNumber;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", fCurrentMeasureNumber: \"" << fCurrentMeasureNumber <<
      "\", fCurrentRepeatStartMeasureNumber: \"" << fCurrentRepeatStartMeasureNumber <<
      "\", repeatStartMeasureNumber: \"" << repeatStartMeasureNumber <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI

  fCurrentPart->
    handleRepeatEndInPart (
      inputLineNumber,
      repeatStartMeasureNumber,
      barLine->getBarLineTimes ());

  // forget about the current repeat start barLine
  fCurrentRepeatStartMeasureNumber = "";

  ++fRepeatEndCounter;
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleRepeatEndingStart (
  S_msrBarLine& barLine)
{
  int inputLineNumber =
    barLine->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    gLogStream <<
      std::endl << std::endl <<
      "****************** handleRepeatEndingStart()" <<
      ", line " << inputLineNumber <<
      std::endl <<
      fCurrentPart <<
      std::endl;
  }
#endif

  // handle the repeat ending
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending upon its start in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentPart->
    handleRepeatEndingStartInPart (
      inputLineNumber);

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleRepeatHookedEndingEnd (
  S_msrBarLine& barLine)
{
  int inputLineNumber =
    barLine->
      getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat hooked ending end in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    gLogStream <<
      std::endl << std::endl <<
      "****************** handleRepeatHookedEndingEnd()" <<
      ", line " << inputLineNumber <<
      std::endl <<
      fCurrentPart <<
      std::endl;
  }
#endif

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI

  // create a hooked repeat ending from the current segment
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hooked repeat ending in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentPart->
    handleRepeatEndingEndInPart (
      inputLineNumber,
      fCurrentBarLineEndingNumber,
      msrRepeatEndingKind::kRepeatEndingHooked);
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleRepeatHooklessEndingEnd (
  S_msrBarLine& barLine)
{
  /*
  The discontinue value is typically used for the last ending in a set,
  where there is no downward hook to mark the end of an ending:

    <barLine location="right">
      <ending type="discontinue" number="2"/>
    </barLine>
  */

  int inputLineNumber =
    barLine->
      getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat hookless ending end in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    gLogStream <<
      std::endl << std::endl <<
      "****************** handleRepeatHooklessEndingEnd()" <<
      ", line " << inputLineNumber <<
      std::endl <<
      fCurrentPart <<
      std::endl;
  }
#endif

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine);

  // create a hookless repeat ending from the current segment
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a hookless repeat ending in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentPart->
    handleRepeatEndingEndInPart (
      inputLineNumber,
      fCurrentBarLineEndingNumber,
      msrRepeatEndingKind::kRepeatEndingHookless);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_rehearsal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_rehearsal" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string rehearsalValue = elt->getValue();

  std::string rehearsalEnclosure =
    elt->getAttributeValue ("enclosure");

  msrRehearsalMarkKind
    rehearsalKind =
      msrRehearsalMarkKind::kRehearsalMarkNone; // default value

  if      (rehearsalEnclosure == "[NONE]") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkNone;
  }
  else if (rehearsalEnclosure == "rectangle") {
    rehearsalKind = msrRehearsalMarkKind::kRehearsalMarkRectangle;
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
  else {
    if (rehearsalEnclosure.size ()) {
      std::stringstream s;

      s <<
        "rehearsalMark enclosure \"" << rehearsalEnclosure <<
        "\"" << " is not handled, ignored";

      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }
  }

  // create a rehearsalMark
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRehearsalMarks ()) {
    gLogStream <<
      "Creating rehearsalMark \"" << rehearsalValue << "\"" <<
      " in part " <<
      fCurrentPart->getPartCombinedName () <<
      std::endl;
  }
#endif

  S_msrRehearsalMark
    rehearsalMark =
      msrRehearsalMark::create (
        inputLineNumber,
        rehearsalKind,
        rehearsalValue,
        fCurrentDirectionPlacementKind);

  // append the rehearsalMark to the pending tempos std::list
  fPendingRehearsalMarksList.push_back (rehearsalMark);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_harmony& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_harmony" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);

  ++fHarmoniesVoicesCounter;

  fCurrentHarmonyInputLineNumber       = inputLineNumber;

  fCurrentHarmonyRootDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fCurrentHarmonyRootAlterationKind    = msrAlterationKind::kAlterationNatural;
  fCurrentHarmonyKind                  = msrHarmonyKind::kHarmony_NO_;
  fCurrentHarmonyKindText              = "";
  fCurrentHarmonyInversion             = K_HARMONY_NO_INVERSION;
  fCurrentHarmonyBassDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_NO_;
  fCurrentHarmonyBassAlterationKind    = msrAlterationKind::kAlterationNatural;
  fCurrentHarmonyDegreeValue           = -1;
  fCurrentHarmonyDegreeAlterationKind  = msrAlterationKind::kAlterationNatural;

  fCurrentHarmonyWholeNotesOffset = Rational (0, 1);

  fOnGoingHarmony = true;
}

void mxsr2msrTranslator::visitStart (S_root& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_root" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);
}

void mxsr2msrTranslator::visitStart (S_root_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_root_step" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string step = elt->getValue ();

  checkStep (
    inputLineNumber,
    step,
    "<root-step/>");

  fCurrentHarmonyRootDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (
      step [0]);
}

void mxsr2msrTranslator::visitStart (S_root_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_root_alter" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  float rootAlter = (float)(*elt);

  fCurrentHarmonyRootAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      rootAlter);

  if (fCurrentHarmonyRootAlterationKind == msrAlterationKind::kAlteration_NO_) {
    std::stringstream s;

    s <<
      "root alter '" << rootAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_kind& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_kind" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // harmony kind
  // ----------------------------------

  std::string kind = elt->getValue ();

  fCurrentHarmonyKindText =
    elt->getAttributeValue ("text");

  // check harmony kind
  if      (kind == "major")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajor;
  else if (kind == "minor")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinor;
  else if (kind == "augmented")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyAugmented;
  else if (kind == "diminished")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDiminished;

  else if (kind == "dominant")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDominant;
  else if (kind == "major-seventh")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorSeventh;
  else if (kind == "minor-seventh")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorSeventh;
  else if (kind == "diminished-seventh")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDiminishedSeventh;

  else if (kind == "augmented-seventh")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyAugmentedSeventh;
  else if (kind == "half-diminished")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyHalfDiminished;
  else if (kind == "major-minor")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorMajorSeventh;

  else if (kind == "major-sixth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorSixth;
  else if (kind == "minor-sixth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorSixth;

  else if (kind == "dominant-ninth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDominantNinth;
  else if (kind == "major-ninth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorNinth;
  else if (kind == "minor-ninth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorNinth;

  else if (kind == "dominant-11th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDominantEleventh;
  else if (kind == "major-11th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorEleventh;
  else if (kind == "minor-11th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorEleventh;

  else if (kind == "dominant-13th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyDominantThirteenth;
  else if (kind == "major-13th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajorThirteenth;
  else if (kind == "minor-13th")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMinorThirteenth;

  else if (kind == "suspended-second")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonySuspendedSecond;
  else if (kind == "suspended-fourth")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonySuspendedFourth;

  else if (kind == "Neapolitan")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyNeapolitan;
  else if (kind == "Italian")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyItalian;
  else if (kind == "French")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyFrench;
  else if (kind == "German")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyGerman;

  else if (kind == "pedal")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyPedal;
  else if (kind == "power")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyPower;
  else if (kind == "Tristan")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyTristan;
  else if (kind == "other")
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyOther;
  else if (kind == "[NONE]") {
    fCurrentHarmonyKind = msrHarmonyKind::kHarmonyNone;
  }

  else {
    if (kind.size ()) {
      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        "harmony kind \"" + kind + "\" os unknown");
    }
    else {
      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        "empty harmony kind, replaced by 'major'");

      fCurrentHarmonyKind = msrHarmonyKind::kHarmonyMajor;
    }
  }

  // harmony use symbols
  // ----------------------------------

  std::string kindUseSymbols = elt->getAttributeValue ("use-symbols");

/* JMI
  if      (kindUseSymbols == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindUseSymbols == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindUseSymbols.size ()) {
      std::stringstream s;

      s <<
        "kind use-symbols \"" << kindUseSymbols <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }
*/

  // harmony use std::stack degrees
  // ----------------------------------

  std::string kindStackDegrees = elt->getAttributeValue ("std::stack-degrees");

/* JMI
  if      (kindStackDegrees == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindStackDegrees == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindStackDegrees.size ()) {
      std::stringstream s;

      s <<
        "kind std::stack-degrees " << kindStackDegrees <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }
  */

  // harmony use parentheses degrees
  // ----------------------------------

  std::string kindParenthesesDegrees = elt->getAttributeValue ("parentheses-degrees");

/* JMI
  if      (kindParenthesesDegrees == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindParenthesesDegrees == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindParenthesesDegrees.size ()) {
      std::stringstream s;

      s <<
        "kind parentheses-degrees \"" << kindParenthesesDegrees <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }
  */

  // harmony use bracket degrees
  // ------------------

  std::string kindBracketDegrees = elt->getAttributeValue ("bracket-degrees");

/* JMI
  if      (kindBracketDegrees == "yes")
    fCurrentTupletTypeKind = msrTuplet::kStartTuplet; // JMI
  else if (kindBracketDegrees == "no")
    fCurrentTupletTypeKind = msrTuplet::kStopTuplet;
  else {
    if (kindBracketDegrees.size ()) {
      std::stringstream s;

      s <<
        "kind bracket-degrees \"" << kindBracketDegrees <<
        "\" is unknown";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      }
  }
  */
}

void mxsr2msrTranslator::visitStart (S_inversion& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_inversion" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/*
  Inversion is a number indicating which inversion is used:
  0 for root position, 1 for first inversion, etc.
*/

  fCurrentHarmonyInversion = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_bass& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_bass" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitStart (S_bass_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_bass_step" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string step = elt->getValue();

  checkStep (
    inputLineNumber,
    step,
    "<bass-step/>");

  fCurrentHarmonyBassDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (
      step [0]);
}

void mxsr2msrTranslator::visitStart (S_bass_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_bass_alter" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  float bassAlter = (float)(*elt);

  fCurrentHarmonyBassAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      bassAlter);

  if (fCurrentHarmonyBassAlterationKind == msrAlterationKind::kAlteration_NO_) {
    std::stringstream s;

    s <<
      "bass alter '" << bassAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_degree& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_degree" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);
}

void mxsr2msrTranslator::visitStart (S_degree_value& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_degree_value" <<
       ", line " << elt->getInputLineNumber () <<
     std::endl;
  }
#endif

  fCurrentHarmonyDegreeValue = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_degree_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_degree_alter" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  float degreeAlter = (float)(*elt);

  fCurrentHarmonyDegreeAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      degreeAlter);

  if (fCurrentHarmonyDegreeAlterationKind == msrAlterationKind::kAlteration_NO_) {
    std::stringstream s;

    s <<
      "degree alter '" << degreeAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_degree_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_degree_type" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        "harmony degree-type \"" + degreeType + "\" is unknown");
  }
}

void mxsr2msrTranslator::visitEnd (S_degree& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_degree" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create harmony degree
  S_msrHarmonyDegree
    harmonyDegree =
      msrHarmonyDegree::create (
        inputLineNumber,
        fCurrentHarmonyDegreeValue,
        fCurrentHarmonyDegreeAlterationKind,
        fCurrentHarmonyDegreeTypeKind);

  // register it in current harmony degrees std::list
  fCurrentHarmonyDegreesList.push_back (
    harmonyDegree);
}

void mxsr2msrTranslator::visitEnd (S_harmony& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_harmony" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // convert root diatonic pitch to a quarter tone pitch
  fCurrentHarmonyRootQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentHarmonyRootDiatonicPitchKind,
      fCurrentHarmonyRootAlterationKind);

  // convert bass diatonic pitch to a quarter tone pitch
  fCurrentHarmonyBassQuarterTonesPitchKind =
    quarterTonesPitchKindFromDiatonicPitchAndAlteration (
      inputLineNumber,
      fCurrentHarmonyBassDiatonicPitchKind,
      fCurrentHarmonyBassAlterationKind);

  // check data consistency
  if (
    fCurrentHarmonyKind == msrHarmonyKind::kHarmonyOther
      &&
    fCurrentHarmonyDegreesList.size () == 0
  ) {
    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      "harmony kind is 'other' but there are no harmony degrees, this is strange...");
  }

  if (
    fCurrentHarmonyRootQuarterTonesPitchKind
      ==
    fCurrentHarmonyBassQuarterTonesPitchKind) {
    std::stringstream s;

    s <<
      "harmony root and bass notes are both equal to '" <<
      msrDiatonicPitchKindAsStringInLanguage (
        gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
        diatonicPitchKindFromQuarterTonesPitchKind (
          inputLineNumber,
          fCurrentHarmonyRootQuarterTonesPitchKind)) <<
      "', ignoring the latter";

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());

    fCurrentHarmonyBassQuarterTonesPitchKind =
      msrQuarterTonesPitchKind::kQTP_NO_;
  }

  if (gGlobalMxsr2msrOahGroup->getIgnoreHarmonies ()) {
 #ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      gLogStream <<
        "Ignoring harmony" <<
        ", line " <<
        inputLineNumber <<
        std::endl;
    }
#endif
  }
  else {
   // create the harmony
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
      gLogStream <<
        "Creating a harmony" <<
        ", line " << inputLineNumber << ":" <<
        std::endl;

      ++gIndenter;

      const int fieldWidth = 32;

      gLogStream << std::left <<
        std::setw (fieldWidth) << "fCurrentPart" << " = " <<
        fCurrentPart->getPartCombinedName () <<
        std::endl <<
        /* JMI
        std::setw (fieldWidth) << "harmoniesVoice" << " = " <<
        harmoniesVoice->getVoiceName () <<
        std::endl <<
        */

        std::setw (fieldWidth) << "fCurrentHarmonyRootDiatonicPitch" << " = " <<
        msrDiatonicPitchKindAsStringInLanguage (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          fCurrentHarmonyRootDiatonicPitchKind) <<
        std::endl <<
        std::setw (fieldWidth) << "fCurrentHarmonyRootAlteration" << " = " <<
        msrAlterationKindAsString(
          fCurrentHarmonyRootAlterationKind) <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyKind" << " = " <<
        msrHarmonyKindAsString (
          fCurrentHarmonyKind) <<
        std::endl <<
        std::setw (fieldWidth) << "fCurrentHarmonyKindText" << " = " <<
        fCurrentHarmonyKindText <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyInversion" << " = " <<
        fCurrentHarmonyInversion <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyBassDiatonicPitch" << " = " <<
        msrDiatonicPitchKindAsStringInLanguage (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          fCurrentHarmonyBassDiatonicPitchKind) <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyBassAlteration" << " = " <<
        msrAlterationKindAsString(
          fCurrentHarmonyBassAlterationKind) <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentNoteSoundingWholeNotes" << " = " <<
        fCurrentNoteSoundingWholeNotes <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmoniesStaffNumber" << " = " <<
        fCurrentHarmoniesStaffNumber <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyWholeNotesOffset" << " = " <<
        fCurrentHarmonyWholeNotesOffset <<
        std::endl;

      --gIndenter;
    }
#endif

    S_msrHarmony
      harmony =
        msrHarmony::create (
          fCurrentHarmonyInputLineNumber,
		      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()

          fCurrentHarmonyRootQuarterTonesPitchKind,

          fCurrentHarmonyKind,
          fCurrentHarmonyKindText,

          fCurrentHarmonyInversion,

          fCurrentHarmonyBassQuarterTonesPitchKind,

          Rational (1, 1),            // harmonySoundingWholeNotes,
                                      // will be set upon next note handling
          Rational (1, 1),            // harmonyDisplayWholeNotes,
                                      // will be set upon next note handling
          fCurrentHarmoniesStaffNumber,
          msrTupletFactor (1, 1),     // will be set upon next note handling
          fCurrentHarmonyWholeNotesOffset);

    // append pending harmony degrees if any to the harmony
    if (! fCurrentHarmonyDegreesList.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmoniesDetails ()) {
        mxsr2msrWarning (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          "harmony has no degrees contents");
      }
#endif
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

        // append it to harmony's degrees std::list
        harmony->
          appendHarmonyDegreeToHarmony (
            harmonyDegree);

        // remove it from the std::list
        fCurrentHarmonyDegreesList.pop_front ();
      } // while
    }

    // attach the current frame if any to the harmony
    if (fCurrentFrame) {
      harmony->setHarmonyFrame (fCurrentFrame);
    }

    // append the harmony to the pending harmonies std::list
    fPendingHarmoniesList.push_back (harmony);
  }

  fOnGoingHarmony = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_frame& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_frame" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (! fOnGoingHarmony) {
    std::stringstream s;

    s << "<frame /> out of context, not handled";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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

  if (fCurrentHarmonyDegreeAlterationKind == msrAlterationKind::kAlteration_NO_) {
    std::stringstream s;

    s <<
      "degree alter '" << degreeAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // color JMI

  fOnGoingFrame = true;
}

void mxsr2msrTranslator::visitStart (S_frame_strings& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_frame_strings" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentFrameStrings = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_frame_frets& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_frame_frets" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentFrameFrets = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_first_fret& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_first_fret" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/*
          <first-fret location="right" text="12fr.">12</first-fret>
*/

  fCurrentFrameFirstFret = (int)(*elt);

  // the following two are not used yet JMI
  std::string firstFretLocation = elt->getAttributeValue ("location");
  std::string firstFretText = elt->getAttributeValue ("text");
}

void mxsr2msrTranslator::visitStart (S_frame_note& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_frame_note" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentFrameNoteStringNumber = -1;
  fCurrentFrameNoteFretNumber = -1;
  fCurrentFrameNoteFingering = -1;
  fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeNone;

  fOnGoingFrameNote = true;
}

void mxsr2msrTranslator::visitStart (S_barre& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_barre" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // type

  std::string barreType = elt->getAttributeValue ("type");

  fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeNone;

  if      (barreType == "start")
    fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeStart;
  else if (barreType == "stop")
    fCurrentFrameNoteBarreTypeKind = msrBarreTypeKind::kBarreTypeStop;
  else {
    std::stringstream s;

    s <<
      "barre type \"" << barreType <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // color JMI
}

void mxsr2msrTranslator::visitEnd (S_frame_note& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_frame_note" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the frame note
  S_msrFrameNote
    frameNote =
      msrFrameNote::create (
        inputLineNumber,
        fCurrentFrameNoteStringNumber,
        fCurrentFrameNoteFretNumber,
        fCurrentFrameNoteFingering,
        fCurrentFrameNoteBarreTypeKind);

  // append the frame note to the pending frame notes std::list
  fPendingFramesNotesList.push_back (frameNote);

  fOnGoingFrameNote = false;
}

void mxsr2msrTranslator::visitEnd (S_frame& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_frame" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the frame
  fCurrentFrame =
    msrFrame::create (
      inputLineNumber,
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

      // remove it from the std::list
      fPendingFramesNotesList.pop_front ();
    } // while
  }

  fOnGoingFrame = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_figured_bass& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_figured_bass" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
      std::stringstream s;

      s <<
        "parentheses value " << parentheses <<
        " should be 'yes' or 'no'";

      mxsr2msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fCurrentFiguredBassInputLineNumber   = -1;

  fCurrentFigureNumber = -1;

  fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefix_NO_;
  fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffix_NO_;

  fCurrentFiguredBassSoundingWholeNotes = Rational (0, 1);
  fCurrentFiguredBassDisplayWholeNotes  = Rational (0, 1);

  fOnGoingFiguredBass = true;
}

void mxsr2msrTranslator::visitStart (S_figure& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_figure" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitStart (S_prefix& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_prefix" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string prefix =
    elt->getValue ();

  fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefix_NO_;

  if      (prefix == "double-flat")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixDoubleFlat;

  else if (prefix == "flat")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixFlat;

  else if (prefix == "flat-flat")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixFlatFlat;

  else if (prefix == "natural")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixNatural;

  else if (prefix == "sharp-sharp")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixSharpSharp;

  else if (prefix == "sharp")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixSharp;

  else if (prefix == "double-sharp")
    fCurrentFigurePrefixKind = msrBassFigurePrefixKind::kBassFigurePrefixDoubleSharp;

  else if (prefix.size ()) {
    std::stringstream s;

    s <<
      "prefix \"" << prefix <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

}

void mxsr2msrTranslator::visitStart (S_figure_number& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_figure_number" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentFigureNumber = (int)(*elt);

  if (fCurrentFigureNumber > 13) {
    std::stringstream s;

    s <<
      "figure-number '" << fCurrentFigureNumber <<
      "' is greater that 13, that's strange...";

    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }
}

void mxsr2msrTranslator::visitStart (S_suffix& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_suffix" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string suffix =
    elt->getValue ();

  fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffix_NO_;

  if      (suffix == "double-flat")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixDoubleFlat;

  else if (suffix == "flat")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixFlat;

  else if (suffix == "flat-flat")
    fCurrentFigureSuffixKind = msrBassFigureSuffixKind::kBassFigureSuffixFlatFlat;

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
    std::stringstream s;

    s <<
      "suffix \"" << suffix <<
      "\" is unknown";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitEnd (S_figure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_figure" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the bass figure
  S_msrBassFigure
    bassFigure =
      msrBassFigure::create (
        inputLineNumber,
        fCurrentPart,
        fCurrentFigurePrefixKind,
        fCurrentFigureNumber,
        fCurrentFigureSuffixKind);

  // append it to the pending figures std::list
  fPendingFiguredBassFiguresList.push_back (
    bassFigure);
}

void mxsr2msrTranslator::visitEnd (S_figured_bass& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_figured_bass" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the figured bass element
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating a figured bass" <<
      ", line " << inputLineNumber << ":" <<
      std::endl;
  }
#endif

  // create the figured bass element
  // if the sounding whole notes is 0/1 (no <duration /> was found), JMI ???
  // it will be set to the next note's sounding whole notes later

  S_msrFiguredBass
    figuredBass =
      msrFiguredBass::create (
        inputLineNumber,
        fCurrentFiguredBassSoundingWholeNotes,
        fCurrentFiguredBassDisplayWholeNotes,
        fCurrentFiguredBassParenthesesKind,
				msrTupletFactor (1, 1)); // will be set upon next note handling

  // attach pending figures to the figured bass
  if (! fPendingFiguredBassFiguresList.size ()) {
    mxsr2msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
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

    // append the figured bass element to the pending figured bass elements list
    fPendingFiguredBassesList.push_back (figuredBass);
  }

  fOnGoingFiguredBass = false;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_harp_pedals& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_harp_pedals" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarpPedalsTunings ()) {
    gLogStream <<
      "Creating harp pedals tuning:" <<
      std::endl;
  }
#endif

  fCurrentHarpPedalsTuning =
    msrHarpPedalsTuning::create (
      inputLineNumber);

  // add it to the current part
  fCurrentPart->
    appendHarpPedalsTuningToPart (
      fCurrentHarpPedalsTuning);
}

void mxsr2msrTranslator::visitStart (S_pedal_tuning& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pedal_tuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void mxsr2msrTranslator::visitStart (S_pedal_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pedal_step" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string tuningStep = elt->getValue();

  checkStep (
    inputLineNumber,
    tuningStep,
    "<pedal-step/>");

  fCurrentHarpPedalDiatonicPitchKind =
    msrDiatonicPitchKindFromChar (
      tuningStep [0]);
}

void mxsr2msrTranslator::visitStart (S_pedal_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_pedal_alter" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  float pedalAlter = (float)(*elt);

  fCurrentHarpPedalAlterationKind =
    msrAlterationKindFromMusicXMLAlter (
      pedalAlter);

  if (fCurrentHarpPedalAlterationKind == msrAlterationKind::kAlteration_NO_) {
    std::stringstream s;

    s <<
      "pedal alter '" << pedalAlter << "'"
      "' should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void mxsr2msrTranslator::visitEnd (S_pedal_tuning& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_pedal_tuning" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a harp pedals tuning
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarpPedalsTunings ()) {
    gLogStream <<
      "Creating harp pedal tuning:" <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 31;

    gLogStream << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentHarpPedalDiatonicPitch" << ": " <<
      msrDiatonicPitchKindAsStringInLanguage (
        gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
        fCurrentHarpPedalDiatonicPitchKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentHarpPedalAlteration" << ": " <<
      msrAlterationKindAsString (
        fCurrentHarpPedalAlterationKind) <<
      std::endl;

    --gIndenter;
  }
#endif

  fCurrentHarpPedalsTuning->
    addPedalTuning (
      inputLineNumber,
      fCurrentHarpPedalDiatonicPitchKind,
      fCurrentHarpPedalAlterationKind);
}

void mxsr2msrTranslator::visitStart( S_damp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_damp" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingDirectionType) {
    // fetch current voice
    S_msrVoice
      currentVoice =
        fetchVoiceFromCurrentPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);

    // create the damp
    S_msrDamp
      damp =
        msrDamp::create (
					inputLineNumber);

    // append it to the pending damps std::list
    fPendingDampsList.push_back (damp);
  }
}

void mxsr2msrTranslator::visitStart( S_damp_all& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_damp_all" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingDirectionType) {
    // fetch current voice
    S_msrVoice
      currentVoice =
        fetchVoiceFromCurrentPart (
          inputLineNumber,
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber);

    // create the damp all
    S_msrDampAll
      dampAll =
        msrDampAll::create (
					inputLineNumber);

    // append it to the pending damp alls std::list
    fPendingDampAllsList.push_back (dampAll);
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_capo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_capo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentStaffDetailsCapo = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_staff_size& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_staff_size" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentStaffDetailsStaffSize = (int)(*elt);
  // JMI not used
}

void mxsr2msrTranslator::visitEnd (S_staff_details& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_staff_details" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    const int fieldWidth = 29;

    gLogStream << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentStaffLinesNumber" << " = " <<
      fCurrentStaffLinesNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fStaffDetailsStaffNumber" << " = " <<
      fStaffDetailsStaffNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStaffDetailsCapo" << " = " <<
      fCurrentStaffDetailsCapo <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStaffDetailsStaffSize" << " = " <<
      fCurrentStaffDetailsStaffSize <<
      std::endl;
  }
#endif

  --gIndenter;

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
        fetchStaffFromCurrentPart (
          inputLineNumber,
          fStaffDetailsStaffNumber);

    staff->
      appendStaffDetailsToStaff (
        fCurrentStaffDetails);
  }

  // forget about this staff details
  fCurrentStaffDetails = nullptr;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_scordatura& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_scordatura" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
			inputLineNumber);
}

void mxsr2msrTranslator::visitStart (S_accord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_accord" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/*
        <accord std::string="3">
          <tuning-step>C</tuning-step>
          <tuning-octave>3</tuning-octave>
          </accord>
*/

  fCurrentAccordNumber = elt->getAttributeIntValue ("std::string", 0);

  fCurrentAccordDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_NO_;

  fCurrentAccordAlterationKind =
    msrAlterationKind::kAlterationNatural; // default value

  fCurrentAccordOctaveKind =
    msrOctaveKind::kOctave_NO_;

  fOnGoingAccord = true;
}

void mxsr2msrTranslator::visitEnd (S_accord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_accord" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fOnGoingAccord = false;

//   gLogStream <<
//     "S_tuning_octave: tuningOctaveNumber: " << tuningOctaveNumber << std::endl;

  S_msrStringTuning
    stringTuning =
      msrStringTuning::create (
        inputLineNumber,
        fCurrentAccordNumber,
        fCurrentAccordDiatonicPitchKind,
        fCurrentAccordAlterationKind,
        fCurrentAccordOctaveKind);

  fCurrentScordatura->
    addStringTuningToScordatura (
      stringTuning);
}

void mxsr2msrTranslator::visitEnd (S_scordatura& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the current scordatura to the pending scordatura std::list
  fPendingScordaturasList.push_back (fCurrentScordatura);

  // forget about this scordatura
  fCurrentScordatura = nullptr;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_instrument_sound& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_instrument_sound" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_virtual_instrument& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_virtual_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_midi_device& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_midi_device" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/*
      <midi-device>SmartMusic SoftSynth</midi-device>
*/
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_midi_instrument& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_midi_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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


/* JMI
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
      gLogStream <<
        "--> figured bass" <<
        ", line " << inputLineNumber << ":" <<
        std::endl;

      ++gIndenter;

      const int fieldWidth = 31;

      gLogStream << std::left <<
        std::setw (fieldWidth) << "fCurrentPart" << " = " <<
        fCurrentPart->getPartCombinedName () <<
        std::endl <<
        std::setw (fieldWidth) << "fCurrentFiguredBassSoundingWholeNotes" << " = " <<
        fCurrentFiguredBassSoundingWholeNotes <<
        std::endl;

      --gIndenter;
    }
#endif

    if (fCurrentFiguredBassSoundingWholeNotes.getNumerator () == 0) {
      // no duration has been found,
      // use the note's sounding whole notes
      fCurrentFiguredBassSoundingWholeNotes =
        fCurrentNoteSoundingWholeNotes;
    }

    // create the figured bass
    // if the sounding whole notes is 0/1 (no <duration /> was found),
    // it will be set to the next note's sounding whole notes later
    S_msrFiguredBass
      figuredBass =
        msrFiguredBass::create (
          inputLineNumber,
          fCurrentPart,
          fCurrentFiguredBassSoundingWholeNotes,
          fCurrentFiguredBassParenthesesKind);

    // attach pending figures to the figured bass
    if (fPendingFiguredBassFiguresList.size ()) {
      for (
        std::list<S_msrBassFigure>::const_iterator i=fPendingFiguredBassFiguresList.begin ();
        i!=fPendingFiguredBassFiguresList.end ();
        ++i
      ) {
        figuredBass->
          appendFigureToFiguredBass ((*i));
      } // for

      fPendingFiguredBassFiguresList.clear ();
    }

    // append the figured bass to the current part
    fCurrentPart->
      appendFiguredBassToPart (
        voiceToInsertInto,
        figuredBass);
    */

/*
group-symbol
group-barLine
part-symbol
// color JMI


*/

/* JMI
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) { // JMI
     const int fieldWidth = 27;

    gLogStream << std::left <<
      std::endl <<
      "==> AFTER visitEnd (S_note&) " <<
      newNote->asString () <<
      ", line " << inputLineNumber <<
      " we have:" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "--> fCurrentMusicXMLStaffNumber" << " = " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "--> fCurrentMusicXMLVoiceNumber" << " = " <<
      fCurrentMusicXMLVoiceNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "--> current voice" << " = \"" <<
      currentVoice->getVoiceName () << "\"" <<
      std::endl <<
      "<==" <<
      std::endl;
  }
#endif
*/

/* JMI
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) { // JMI
    gLogStream <<
      std::endl <<
      "==> BEFORE visitEnd (S_note&)" <<
      ", line " << inputLineNumber <<
      " we have:" <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 27;

    gLogStream << std::left <<
      std::setw (fieldWidth) << "--> fCurrentMusicXMLStaffNumber" << " = " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      std::setw (fieldWidth) << "--> fCurrentMusicXMLVoiceNumber" << " = " <<
      fCurrentMusicXMLVoiceNumber <<
      std::endl <<
      std::setw (fieldWidth) << "--> current voice" << " = \"" <<
      currentVoice->getVoiceName () << "\"" <<
      std::endl;

    --gIndenter;

    gLogStream <<
      "<==" <<
      std::endl <<
      * std::endl;
  }
#endif
*/

  /* JMI
Repeats and endings are represented by the <repeat> and <ending> elements with a <barLine>, as defined in the barLine.mod file.

In regular measures, there is no need to include the <barLine> element. It is only need to represent repeats, endings, and graphical styles such as double barLines.

A forward repeat mark is represented by a left barLine at the beginning of the measure (following the attributes element, if there is one):

  <barLine location="left">
    <bar-style>heavy-light</bar-style>
    <repeat direction="forward"/>
  </barLine>

The repeat element is what is used for sound generation; the bar-style element only indicates graphic appearance.

Similarly, a backward repeat mark is represented by a right barLine at the end of the measure:

  <barLine location="right">
    <bar-style>light-heavy</bar-style>
    <repeat direction="backward"/>
  </barLine>

While repeats can have forward or backward direction, endings can have three different type attributes: start, stop, and discontinue. The start value is used at the beginning of an ending, at the beginning of a measure. A typical first ending starts like this:

  <barLine location="left">
    <ending type="start" number="1"/>
  </barLine>

The stop value is used when the end of the ending is marked with a downward hook, as is typical for a first ending. It is usually used together with a backward repeat at the end of a measure:

  <barLine location="right">
    <bar-style>light-heavy</bar-style>
    <ending type="stop" number="1"/>
    <repeat direction="backward"/>
  </barLine>

The discontinue value is typically used for the last ending in a set, where there is no downward hook to mark the end of an ending:

  <barLine location="right">
    <ending type="discontinue" number="2"/>
  </barLine>

    */

// void mxsr2msrTranslator::convertWordsToTempo (
//   int           inputLineNumber,
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
// #ifdef TRACING_IS_ENABLED
//   if (
//     gGlobalTracingOahGroup->getTraceWords ()
//       ||
//     gGlobalTracingOahGroup->getTraceDalSegnos ()
//   ) {
//     gLogStream <<
//       "Converting words \"" <<
//       wordsValue <<
//       "\" to an MSR tempo" <<
//       ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//       ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif
//
//   fPendingTemposList.push_back (tempo);
// }

// void mxsr2msrTranslator::convertWordsToRehearsalMark (
//   int           inputLineNumber,
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
//     #ifdef TRACING_IS_ENABLED
//       if (
//         gGlobalTracingOahGroup->getTraceWords ()
//           ||
//         gGlobalTracingOahGroup->getTraceDalSegnos ()
//       ) {
//         gLogStream <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR rehearsal mark" <<
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif
//
//       // append the rehearsalMark to the pending tempos std::list
//       fPendingRehearsalMarksList.push_back (rehearsalMark);
// }
//
// void mxsr2msrTranslator::convertWordsToSegno (
//   int           inputLineNumber,
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
//     #ifdef TRACING_IS_ENABLED
//       if (
//         gGlobalTracingOahGroup->getTraceWords ()
//           ||
//         gGlobalTracingOahGroup->getTraceDalSegnos ()
//       ) {
//         gLogStream <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR dal segno" <<
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif
//
//       // append dal segno to the pending tempos std::list
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
// #ifdef TRACING_IS_ENABLED
//   if (
//     gGlobalTracingOahGroup->getTraceWords ()
//       ||
//     gGlobalTracingOahGroup->getTraceDalSegnos ()
//   ) {
//     gLogStream <<
//       "Converting words \"" <<
//       wordsValue <<
//       "\" to an MSR dal segno al fine" <<
//       ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//       ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif
//
//   // append dal segno al fine to the pending tempos std::list
//   fPendingRehearsalMarksList.push_back (rehearsalMark);
// }
//
// void mxsr2msrTranslator::convertWordsToDalSegnoAlCoda (
//   int           inputLineNumber,
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
// #ifdef TRACING_IS_ENABLED
//   if (
//     gGlobalTracingOahGroup->getTraceWords ()
//       ||
//     gGlobalTracingOahGroup->getTraceDalSegnos ()
//   ) {
//     gLogStream <<
//       "Converting words \"" <<
//       wordsValue <<
//       "\" to an MSR dal segno al coda" <<
//       ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//       ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif
//
//   // append dal segno al coda to the pending tempos std::list
//   fPendingRehearsalMarksList.push_back (rehearsalMark);
// }
// void mxsr2msrTranslator::convertWordsToCoda (
//   int           inputLineNumber,
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
//     #ifdef TRACING_IS_ENABLED
//       if (
//         gGlobalTracingOahGroup->getTraceWords ()
//           ||
//         gGlobalTracingOahGroup->getTraceDalSegnos ()
//       ) {
//         gLogStream <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR coda" <<
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif
//
//       // append coda to the pending tempos std::list
//       fPendingRehearsalMarksList.push_back (rehearsalMark);
// }
//
// void mxsr2msrTranslator::convertWordsToCresc (
//   int           inputLineNumber,
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
//     #ifdef TRACING_IS_ENABLED
//       if (
//         gGlobalTracingOahGroup->getTraceWords ()
//           ||
//         gGlobalTracingOahGroup->getTraceDalSegnos ()
//       ) {
//         gLogStream <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR cresc" <<
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif
//
//       // append the rehearsalMark to the pending tempos std::list
//       fPendinCrescDecrescsList.push_back (crescDecresc);
// }
// void mxsr2msrTranslator::convertWordsToDecresc (
//   int           inputLineNumber,
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
//     #ifdef TRACING_IS_ENABLED
//       if (
//         gGlobalTracingOahGroup->getTraceWords ()
//           ||
//         gGlobalTracingOahGroup->getTraceDalSegnos ()
//       ) {
//         gLogStream <<
//           "Converting words \"" <<
//           wordsValue <<
//           "\" to an MSR decresc" <<
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousMusicXMLVoiceNumber: " << fPreviousMusicXMLVoiceNumber <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif
//
//       // append the rehearsalMark to the pending tempos std::list
//       fPendinCrescDecrescsList.push_back (crescDecresc);
// }

//             switch (noteVoice->getRegularVoiceStaffSequentialNumber ()) {
//               case 1:
//               case 3:
// #ifdef TRACING_IS_ENABLED
//                 if (gGlobalTracingOahGroup->getTraceLigatures ()) {
//                   gLogStream <<
//                     "Attaching pending ligature above to note '" <<
//                     note->asString () <<
//                     "' in voice \"" <<
//                     noteVoice->getVoiceName () <<
//                     "\"" <<
//                     ", line " << ligature->getInputLineNumber () <<
//                     std::endl;
//                 }
// #endif
//
//                 note->appendLigatureToNote (ligature);
//                 break;
//               default:
//                 ;
//             } // switch

//             switch (noteVoice->getRegularVoiceStaffSequentialNumber ()) {
//               case 2:
//               case 4:
// #ifdef TRACING_IS_ENABLED
//                 if (gGlobalTracingOahGroup->getTraceLigatures ()) {
//                   gLogStream <<
//                     "Attaching pending ligature below to note '" <<
//                     note->asString () <<
//                     "' in voice \"" <<
//                     noteVoice->getVoiceName () <<
//                     "\"" <<
//                     ", line " << ligature->getInputLineNumber () <<
//                     std::endl;
//                 }
// #endif
//
//                 note->appendLigatureToNote (ligature);
//                 break;
//               default:
//                 ;
//             } // switch
