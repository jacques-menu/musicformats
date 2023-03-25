/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // std::setw, std::setprecision, ...

#include "xml_tree_browser.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "waeInterface.h"
#include "mxsr2msrWae.h"

#include "mxsr2msrComponent.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "mxsrOah.h"
#include "mxsr2msrOah.h"
#include "msrOah.h"

#include "mxsr2msrTranslator.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
mxsr2msrTranslator::mxsr2msrTranslator (
  const S_msrScore& scoreSkeleton)
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
  fCurrentSlashTypeKind = msrSlashTypeKind::kSlashType_UNKNOWN_;
  fCurrentUseDotsKind   = msrUseDotsKind::kUseDots_UNKNOWN_;
  fCurrentSlashUseStemsKind = msrSlashUseStemsKind::kSlashUseStems_UNKNOWN_;

  fCurrentBeatRepeatSlashes = -1;

  fCurrentMeasureRepeatKind =
    msrMeasureRepeatKind::kMeasureRepeat_UNKNOWN_;

  fCurrentMeasureRepeatMeasuresNumber = -1;
  fCurrentMeasureRepeatSlashesNumber  = -1;

  fCurrentMultipleFullBarRestsNumber  = 0;
  fRemainingExpectedMultipleFullBarRests = 0;

  fCurrentSlashDotsNumber = -1;
  fCurrentSlashGraphicNotesDurationKind = msrNotesDurationKind::kNotesDuration_UNKNOWN_;

  // staff details handling
  fStaffDetailsStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;

  fCurrentStaffTypeKind =
    msrStaffTypeKind::kStaffTypeRegular;

  fCurrentShowFretsKind =
    msrShowFretsKind::kShowFretsNumbers; // default value

  fCurrentPrintObjectKind =
    msrPrintObjectKind::kPrintObjectYes; // default value

  fCurrentPrintSpacingKind =
    msrPrintSpacingKind::kPrintSpacingNo; // default value ??? JMI

  // staff tuning handling
  fCurrentStaffTuningAlterationKind = msrAlterationKind::kAlteration_UNKNOWN_;
  fCurrentStaffTuningOctaveKind     = msrOctaveKind::kOctave_UNKNOWN_;

  fCurrentStaffDetailsCapo = 0;
  fCurrentStaffDetailsStaffSize = 0;

  // scordatura handling

  fCurrentAccordNumber = -1;
  fCurrentAccordDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentAccordAlterationKind    = msrAlterationKind::kAlteration_UNKNOWN_;
  fCurrentAccordOctaveKind        = msrOctaveKind::kOctave_UNKNOWN_;

  // voice handling
  fPreviousNoteMusicXMLVoiceNumber = msrVoice::K_VOICE_NUMBER_UNKNOWN_;
  fCurrentMusicXMLVoiceNumber  = msrVoice::K_VOICE_NUMBER_UNKNOWN_;

  // clef handling
  fCurrentClefStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;
  fCurrentClefSign = "";
  fCurrentClefLine = -1;
  fCurrentClefOctaveChange = -97;

  // key handling
  fCurrentKeyKind = msrKeyKind::kKeyTraditional;

  fCurrentKeyStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;
  fCurrentKeyFifths = -1;
  fCurrentKeyCancelFifths = -37;
  fCurrentModeKind = msrModeKind::kMode_UNKNOWN_;

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
  fCurrentMetronomeNotesDurationKind = msrNotesDurationKind::kNotesDuration_UNKNOWN_;
  fCurrentMetronomeBeamValue = "";

  fCurrentMetrenomeNormalDotsNumber = 0;

  // time handling
  fCurrentTimeStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;
  fCurrentTimeSignatureBeats = "";

  // lyrics handling
  fCurrentStanzaNumber = msrStanza::K_STANZA_NUMBER_UNKNOWN_;
  fCurrentStanzaName = msrStanza::K_STANZA_NAME_UNKNOWN_;

  fCurrentSyllabic = "";

  fCurrentSyllableKind       = msrSyllableKind::kSyllableNone;
  fCurrentSyllableExtendKind = msrSyllableExtendKind::kSyllableExtendNone;

  fFirstSyllableInSlurKind     = msrSyllableKind::kSyllableNone;
  fFirstSyllableInLigatureKind = msrSyllableKind::kSyllableNone;

  // harmonies handling
  fHarmoniesVoicesCounter = 0;

  fCurrentHarmonyRootDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentHarmonyRootAlterationKind    = msrAlterationKind::kAlteration_UNKNOWN_;

  fCurrentHarmonyFunctionText          = "";

  fCurrentHarmonyKind                  = msrHarmonyKind::kHarmony_UNKNOWN_;
  fCurrentHarmonyKindText              = "";

  fCurrentHarmonyBassDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentHarmonyBassAlterationKind    = msrAlterationKind::kAlteration_UNKNOWN_;

  fCurrentHarmonyDegreeValue           = -1;
  fCurrentHarmonyDegreeAlterationKind  = msrAlterationKind::kAlteration_UNKNOWN_;

  fCurrentHarmoniesStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;

  fCurrentHarmonyWholeNotesOffset = msrWholeNotes (0, 1);

//   fCurrentFiguredBassWholeNotesDuration = msrWholeNotes (0, 1);

  // figured bass handling
  fFiguredBassVoicesCounter = 0;

  fCurrentFiguredBassSoundingWholeNotes = msrWholeNotes (0, 1);
  fCurrentFiguredBassDisplayWholeNotes  = msrWholeNotes (0, 1);
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
  fCurrentNoteDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentNoteAlterationKind    = msrAlterationKind::kAlteration_UNKNOWN_;

  // note print object kind
  fCurrentNotePrintObjectKind = msrPrintObjectKind::kPrintObjectYes; // default value

  // note head
  fCurrentNoteHeadKind = msrNoteHeadKind::kNoteHeadNormal;
  fCurrentNoteHeadFilledKind = msrNoteHeadFilledKind::kNoteHeadFilledYes;
  fCurrentNoteHeadParenthesesKind = msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo;

  // technicals handling
  fBendAlterValue = -39.3f;

  // tremolos handling
  fCurrentDoubleTremoloTypeKind = msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_;

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

  fCurrentForwardStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;
  fCurrentForwardVoiceNumber = msrVoice::K_VOICE_NUMBER_UNKNOWN_;
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

// JMI  fCurrentNoteKind = msrNoteKind::kNote_UNKNOWN_;

  fCurrentNoteQuarterTonesPitchKind =
    msrQuarterTonesPitchKind::kQTP_UNKNOWN_;

  fCurrentNoteSoundingWholeNotes                  = msrWholeNotes (-13, 1);
  fCurrentNoteSoundingWholeNotesFromNotesDuration = msrWholeNotes (-17, 1);

  fCurrentNoteDisplayWholeNotes         = msrWholeNotes (-25, 1);
  fCurrentNoteDisplayWholeNotesFromType = msrWholeNotes (-29, 1);

  fCurrentNoteDotsNumber = 0;

  fCurrentNoteGraphicNotesDurationKind = msrNotesDurationKind::kNotesDuration_UNKNOWN_;

  fCurrentNoteOctave = msrOctaveKind::kOctave_UNKNOWN_;

  fCurrentNoteQuarterTonesDisplayPitchKind =
    msrQuarterTonesPitchKind::kQTP_UNKNOWN_;
  fCurrentDisplayDiatonicPitchKind =
    msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentDisplayOctave =
    msrOctaveKind::kOctave_UNKNOWN_;

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

  fCurrentMusicXMLVoiceNumber  = 1; // default value, it may be absent

  // staff change detection

  fPreviousNoteMusicXMLStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;
  fCurrentMusicXMLStaffNumber      = msrStaff::K_STAFF_NUMBER_UNKNOWN_;

  fPreviousNoteMusicXMLVoiceNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;

  fCurrentChordStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;

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
  int                inputLineNumber,
  const std::string& context)
{
  size_t
    voicesLastMetNoteMapSize =
      fVoicesLastMetNoteMap.size ();

  gLog <<
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
      gLog <<
  //      (*i).first->getVoiceName () <<
        "staff " << (*i).first.first <<
        ", voice " <<  (*i).first.second <<
        ":" <<
        std::endl;

      ++gIndenter;

      gLog <<
        (*i).second <<
        std::endl;

      --gIndenter;

      if (++i == iEnd) break;

      gLog << std::endl;
    } // for

    --gIndenter;
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::checkStep (
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

  else {
    std::stringstream ss;

    ss <<
      markup <<
      " value " << stepValue <<
      " should be a single letter from A to G";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
  }
}

//______________________________________________________________________________
S_msrStaff mxsr2msrTranslator::fetchStaffFromCurrentPart (
  int inputLineNumber,
  int staffNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Fetching staff " <<
      staffNumber <<
      " from current part" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssertWithInputLocationWithMeasureInfo (
    __FILE__, __LINE__,
    staffNumber != msrStaff::K_STAFF_NUMBER_UNKNOWN_,
    gServiceRunData->getInputSourceName (),
    inputLineNumber,
    "staffNumber == msrStaff::K_STAFF_NUMBER_UNKNOWN_",
    fCurrentMeasureNumber,
    fMsrScore->getScoreNumberOfMeasures ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // fetch the staff from current part
  S_msrStaff
    staff =
      fCurrentPart->
        fetchStaffFromPart (staffNumber);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (! staff) {
    std::stringstream ss;

    ss <<
      "staff " << staffNumber <<
      " not found in score skeleton's part " <<
      fCurrentPart->getPartCombinedName ();

    mxsr2msrInternalErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "--> fetchStaffFromCurrentPart returns " <<
      staff->getStaffName () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  return staff;
}

//______________________________________________________________________________
S_msrVoice mxsr2msrTranslator::fetchVoiceFromCurrentPart (
  int inputLineNumber,
  int staffNumber,
  int voiceNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Fetching voice " <<
      voiceNumber <<
      " in staff " <<
      staffNumber <<
      " from current part" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (! voice) {
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      staff <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

    std::stringstream ss;

    ss <<
      "voice " << voiceNumber <<
      " not found in score skeleton's staff \"" <<
      staff->getStaffName () <<
      "\"";

    gLog << ss.str () << std::endl;
    abort();

    mxsr2msrInternalErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

    /* JMI
  // fetch registered voice displaying staff number
  int voiceDisplayingStaffNumber = 1; //msrVoice::K_VOICE_NUMBER_UNKNOWN_; JMI
    // default, there may be no <staff /> markups
    */

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "--> fetchVoiceFromCurrentPart() returns " <<
      voice->getVoiceName () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  return voice;
}

//______________________________________________________________________________
S_msrVoice mxsr2msrTranslator::fetchFirstVoiceFromCurrentPart (
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
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (! voice) {
    staff->print (gLog); // JMI

    std::stringstream ss;

    ss <<
      "first voice not found in score skeleton's staff \"" <<
      staff->getStaffName () <<
      "\"";

    mxsr2msrInternalErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

    /* JMI
  // fetch registered voice displaying staff number
  int voiceDisplayingStaffNumber = 1; //msrVoice::K_VOICE_NUMBER_UNKNOWN_; JMI
    // default, there may be no <staff /> markups
    */

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "--> fetchVoiceFromCurrentPart() returns " <<
      voice->getVoiceName () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  return voice;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_millimeters& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_millimeters" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMillimeters = (float)(*elt);
}

void mxsr2msrTranslator::visitStart (S_tenths& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tenths" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTenths = (float)(*elt);
}

void mxsr2msrTranslator::visitEnd (S_scaling& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_scaling" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a scaling
  S_msrScaling
    scaling =
      msrScaling::create (
        inputLineNumber,
        fCurrentMillimeters,
        fCurrentTenths);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGeometry ()) {
    std::stringstream ss;

    ss <<
      "There are " << fCurrentTenths <<
      " tenths for " <<  fCurrentMillimeters <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set the MSR score's scaling
  fMsrScore->
    setScaling (scaling);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_system_layout& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_system_layout" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the system layout
  fCurrentSystemLayout =
    msrSystemLayout::create (
      inputLineNumber);

  fOnGoingSystemLayout = true;
}

void mxsr2msrTranslator::visitEnd (S_system_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_system_layout" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
void mxsr2msrTranslator::visitStart (S_system_margins& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_system_margins" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fOnGoingSystemLayout) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<system-margins /> is out of context");
  }

  fOnGoingSystemMargins = true;
}

void mxsr2msrTranslator::visitStart (S_system_distance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_system_distance" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<system-distance /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_top_system_distance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_top_system_distance" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber,
      __FILE__, __LINE__,
      "<top-system-distance /> is out of context");
  }
}

void mxsr2msrTranslator::visitEnd (S_system_margins& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_system_margins" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fOnGoingSystemLayout) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<system-margins /> is out of context");
  }

  fOnGoingSystemMargins = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_system_dividers& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_system_dividers" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

    std::stringstream ss;

    ss <<
      "<system-dividers /> is not supported yet by " <<
      gOahOahGroup->getOahOahGroupServiceName ();

/* JMI
    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
     */
}

void mxsr2msrTranslator::visitStart (S_left_divider& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_left_divider" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_right_divider" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_notations" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI
}

void mxsr2msrTranslator::visitStart (S_other_notation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_other_notation" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_page_layout& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_page_layout" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a page layout
  fCurrentPageLayout =
    msrPageLayout::create (
      inputLineNumber);

  fOnGoingPageLayout = true;
}

void mxsr2msrTranslator::visitEnd (S_page_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_page_layout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::visitStart (S_page_height& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_page_height" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<page-height /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_page_width& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_page_width" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<page-width /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_page_margins& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_page_margins" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_page_margins" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current page layout margins group
  fCurrentPageLayoutMarginsGroup = nullptr;

  fOnGoingPageMargins = false;
}

void mxsr2msrTranslator::visitStart (S_left_margin& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

  float leftMargin = (float)(*elt);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_left_margin" <<
      ", " << leftMargin << " tenths" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
      gServiceRunData->getInputSourceName (),
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_right_margin" <<
      ", " << rightMargin << " tenths" <<
       ", line " << inputLineNumber <<
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
      gServiceRunData->getInputSourceName (),
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_top_margin" <<
      ", " << topMargin << " tenths" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
      gServiceRunData->getInputSourceName (),
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bottom_margin" <<
      ", " << bottomMargin << " tenths" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
      gServiceRunData->getInputSourceName (),
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_layout" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber,
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

void mxsr2msrTranslator::visitEnd (S_staff_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_staff_layout" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current staff layout
  fCurrentStaffLayout = nullptr;

  fOnGoingStaffLayout = false;
}

void mxsr2msrTranslator::visitStart (S_staff_distance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_distance" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_layout" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a measure layout
  fCurrentMeasureLayout =
    msrMeasureLayout::create (
      inputLineNumber);

  fOnGoingMeasureLayout = true;
}

void mxsr2msrTranslator::visitEnd (S_measure_layout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_measure_layout" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current measure layout
  fCurrentMeasureLayout = nullptr;

  fOnGoingMeasureLayout = false;
}

void mxsr2msrTranslator::visitStart (S_measure_distance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_distance" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_appearance" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber);

  // set the MSR score appearance
  fMsrScore->
    setAppearance (
      fCurrentAppearance);

  fOnGoingAppearance = true;
}

void mxsr2msrTranslator::visitEnd (S_appearance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_appearance" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current appearance
  fCurrentAppearance = nullptr;

  fOnGoingAppearance = false;
}

void mxsr2msrTranslator::visitStart (S_line_width& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_line_width" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss <<
        "<line-width /> type \"" <<
        lineWidthTypeString <<
        "\" is missing";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<line-width /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_note_size& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note_size" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      if      (noteSizeTypeString == "cue")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeCue;
      else if (noteSizeTypeString == "grace")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeGrace;
      else if (noteSizeTypeString == "large")
        noteSizeTypeKind = msrNoteSizeTypeKind::kNoteSizeTypeLarge;

      else {
        std::stringstream ss;

        ss <<
          "<note-size /> type \"" <<
          noteSizeTypeString <<
          "\" is unknown";

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss <<
        "<note-size /> type \"" <<
        noteSizePercentage <<
        "\" is missing";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<note-size /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_distance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_distance" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss <<
        "<distance /> type \"" <<
        distanceTypeString <<
        "\" is missing";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<distance /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_glyph& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_glyph" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss <<
        "<glyph /> type \"" <<
        glyphTypeString <<
        "\" is missing";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<glyph /> is out of context");
  }
}

void mxsr2msrTranslator::visitStart (S_other_appearance& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_other_appearance" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingAppearance) {
    // what can we find in such a markup??? JMI
  }
  else {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
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

//   gLog << elt; //JMI v0.9.66 create MusicFormats's own smart pointer type ???

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_part" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string partID = elt->getAttributeValue ("id");

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceParts ()
      ||
    gEarlyOptions.getEarlyTracePasses ()
  ) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== part \"" << partID << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch current part from its partID
  fCurrentPart =
    fMsrScore->
      fetchPartFromScoreByItsPartID (
        inputLineNumber,
        partID);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (! fCurrentPart) {
    // fetch fMsrScore's part list
    std::list<S_msrPart> partsList;

    fMsrScore->
      collectScorePartsList (
        inputLineNumber,
        partsList);

    if (partsList.size () == 1) {
      // there's only one part in the part list,
      // assume this is the one
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

      mxsr2msrWarningWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }

    else {
      std::stringstream ss;

      ss <<
        "part \"" << partID <<
        "\" not found in score skeleton";

      mxsr2msrInternalErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // print-object

  std::string
    printObjectString =
      elt->getAttributeValue ("print-object");

  fCurrentPrintObjectKind =
    msrPrintObjectKindFromString (
      inputLineNumber,
      printObjectString);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "--------------------------------------------" <<
      std::endl <<
      "Analyzing part " <<
      fCurrentPart->
        getPartCombinedName () <<
        " -- start" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // no time has been defined yet
  fCurrentTimeSignature = nullptr;

  // staff numbers
  fPreviousNoteMusicXMLStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;
  fCurrentMusicXMLStaffNumber      = msrStaff::K_STAFF_NUMBER_UNKNOWN_;

  // voice numbers
  fCurrentMusicXMLVoiceNumber = msrVoice::K_VOICE_NUMBER_UNKNOWN_;

  // staff change detection

  // cross staff chords
  fCurrentNoteIsCrossStaves = false; // needed ??? JMI

  // get this part's staves map
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

  ++gIndenter;
}

void mxsr2msrTranslator::visitEnd (S_part& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_part" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

/*
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Analyzing part " <<
      fCurrentPart->
        getPartCombinedName () <<
        " -- end" <<
      std::endl <<
      "--------------------------------------------" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
*/

  // fetch current note's voice
  S_msrVoice
    currentNoteVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssertWithMeasureInfo (
    __FILE__, __LINE__,
    currentNoteVoice != nullptr,
    "currentNoteVoice is null",
    fCurrentMeasureNumber,
    fMsrScore->getScoreNumberOfMeasures ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
  if (gGlobalMxsr2msrOahGroup->getMusicXMLPartsIgnoreNameSet ().size ()) {
    std::set<std::string>::iterator
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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_attributes" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitEnd (S_attributes& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_attributes" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // have the divisions been defined alright?
  if (! fCurrentDivisions) {
    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_divisions" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber,
      __FILE__, __LINE__,
      "divisions per quarter note should be between 1 and 16383");
  }

  // set current part's divisions per quarter note
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
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
      fCurrentPart->getPartCombinedName() <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_clef" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber,
      printObjectString);

  fCurrentClefLine = 0;
  fCurrentClefOctaveChange = 0;
  fCurrentClefSign = "";
}

void mxsr2msrTranslator::visitStart (S_sign& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sign" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentClefSign = elt->getValue();
}

void mxsr2msrTranslator::visitStart (S_line& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_line" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentClefLine = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_clef_octave_change& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_clef_octave_change" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentClefOctaveChange = (int)(*elt);

  if (fCurrentClefOctaveChange < -2 || fCurrentClefOctaveChange > 2) {
    std::stringstream ss;

    ss <<
      "clef-octave-change \"" << fCurrentClefOctaveChange <<
      "\" is unknown";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitEnd (S_clef& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_clef" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

          mxsr2msrErrorWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
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

          mxsr2msrErrorWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

  // is this clef sign in the replace clef map?
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceClefs ()) {
      std::stringstream ss;

      ss <<
        "Replace clef " <<
        msrClefKindAsString (clefKind) <<
        " by " <<
        msrClefKindAsString (newClefKind) <<
        ", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_key" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber,
      printObjectString);

  // traditional

  fCurrentKeyFifths       = 0;
  fCurrentKeyCancelFifths = 0;

  fCurrentModeKind = msrModeKind::kMode_UNKNOWN_;

  // Humdrum-Scot

  fCurrentHumdrumScotKeyItem = nullptr;
}

void mxsr2msrTranslator::visitStart (S_cancel& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_cancel" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentKeyCancelFifths = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_fifths& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fifths" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentKeyKind = msrKeyKind::kKeyTraditional;

  fCurrentKeyFifths = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_mode& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_mode" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_key_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_key_step" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentHumdrumScotKeyItem) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
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

  // insert it into the items vector
  fCurrentHumdrumScotKeyItemsVector.
    insert (
      fCurrentHumdrumScotKeyItemsVector.end (),
      fCurrentHumdrumScotKeyItem);
}

void mxsr2msrTranslator::visitStart (S_key_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_key_alter" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fCurrentHumdrumScotKeyItem) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
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

  if (keyAlterationKind == msrAlterationKind::kAlteration_UNKNOWN_) {
    std::stringstream ss;

    ss <<
      "key alter " << keyAlter <<
      " should be -3, -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5, +2 or +3";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_key_octave" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

  // fetch Humdrum/Scot item with 'number' in the vector
  S_msrHumdrumScotKeyItem item;

  try {
    // indices start at 0
    item = fCurrentHumdrumScotKeyItemsVector [number - 1];
  }
  catch (int e) {
    std::stringstream ss;

    ss <<
      "item " << number <<
      " not found in Humdrum/Scot key items" <<
      std::endl <<
      "An std::exception number " << e << " occurred";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

  if (! item) { // JMI
    gLog << "----------------" << std::endl;

    int counter = 0;
    for (
      std::vector<S_msrHumdrumScotKeyItem>::const_iterator i=
        fCurrentHumdrumScotKeyItemsVector.begin ();
      i!=fCurrentHumdrumScotKeyItemsVector.end ();
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_key" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
      gServiceRunData->getInputSourceName (),
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_time" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

  fCurrentTimeSignatureBeats = "";

  fOnGoingInterchangeable = false;
}

void mxsr2msrTranslator::visitStart (S_beats& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beats" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTimeSignatureBeats = elt->getValue (); // can be a string such as 3+2
}

void mxsr2msrTranslator::visitStart (S_beat_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beat_type" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  int beatType = (int)(*elt);

  // extract the numbers list from the beat type
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
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "beat type doesn't contain any beats numbers");
  }

  // append the time signature item to the current time signature items vector
  fCurrentTimeSignatureItemsVector.insert (
    fCurrentTimeSignatureItemsVector.end (),
    timeSignatureItem);
}

void mxsr2msrTranslator::visitStart (S_senza_misura& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_senza_misura" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura;
}

void mxsr2msrTranslator::visitStart (S_interchangeable& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_interchangeable" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

  fOnGoingInterchangeable = true;
}

void mxsr2msrTranslator::visitStart (S_time_relation& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_time_relation" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }
}

void mxsr2msrTranslator::visitEnd (S_time& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_time" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        gServiceRunData->getInputSourceName (),
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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_score_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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


void mxsr2msrTranslator::visitStart (S_instrument_name& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument_name" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
        <instruments>2</instruments>
*/

//  int instruments = (int)(*elt); // JMI
}

void mxsr2msrTranslator::visitStart (S_solo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_solo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
        <instruments>2</instruments>
*/

//  int instruments = (int)(*elt); // JMI
}

void mxsr2msrTranslator::visitStart (S_instruments& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instruments" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
void mxsr2msrTranslator::visitStart (S_transpose& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_transpose" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTransposeNumber = elt->getAttributeIntValue ("number", 0);

  fCurrentTransposeDiatonic     = 0;
  fCurrentTransposeChromatic    = 0;
  fCurrentTransposeOctaveChange = 0;
  fCurrentTransposeDouble       = false;
}

void mxsr2msrTranslator::visitStart (S_diatonic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_diatonic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTransposeDiatonic = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_chromatic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_chromatic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTransposeChromatic = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_octave_change& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting octave_change" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentTransposeOctaveChange = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_double& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting double" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::visitEnd (S_transpose& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_transpose" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());

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

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_direction" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_direction" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
            fCurrentMetronomeTempo->asString () <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_direction_type" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fOnGoingDirection) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "<direction-type /> is out of context");
  }

  fOnGoingDirectionType = true;
}

void mxsr2msrTranslator::visitEnd (S_direction_type& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_direction_type" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingDirectionType = false;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_offset& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_offset" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
       "\" whole notes" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentHarmonyWholeNotesOffset =
      offsetWholeNotesFromNotesDuration;
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_other_direction& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_direction" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sound" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingDirection) {
    // tempo
    std::string tempoString = elt->getAttributeValue ("tempo"); // to be seen JMI

    if (tempoString.size ()) {
      fCurrentMetronomeTempo =
        msrTempo::createTempoPerMinute (
          inputLineNumber,
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
      // JMI v0.9.66
    }
  }
}

void mxsr2msrTranslator::visitEnd (S_sound& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_sound" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_octave_shift& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_octave_shift" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // size

  std::string octaveShiftSizeString = elt->getAttributeValue ("size");
  int    octaveShiftSize = 8;

  if (! octaveShiftSizeString.size ()) {
    std::stringstream ss;

    ss <<
      "octave shift size absent, assuming 8";

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss <<
        "octave-shift size \"" << octaveShiftSize <<
    // JMI    "\" should be 8, 15, 22 or 27";
        "\" is wrong, should be 8 or 15, replaced by 8";

  // JMI    mxsr2msrError (
      mxsr2msrWarningWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
   //     __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
    std::stringstream ss;

    ss <<
      "octave-shift type \"" << type <<
      "\"" << "is unknown";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

  // create an octave shift
  S_msrOctaveShift
    octaveShift =
      msrOctaveShift::create (
        inputLineNumber,
        octaveShiftKind,
        octaveShiftSize);

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOctaveShifts ()) {
      std::stringstream ss;

      ss <<
        "Creating octave shift " <<
        octaveShift->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

  // append the octave shift to the pending octave shifts list
  fPendingOctaveShiftsList.push_back (octaveShift);
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_words& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_words" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string wordsValue = elt->getValue ();

  if (! wordsValue.size ()) {
    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
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
      std::stringstream ss;

      ss <<
        "font-style value " << wordsFontStyleString <<
        " should be 'normal' or 'italic'";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append the rehearsalMark to the pending tempos list
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
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append dal segno to the pending tempos list
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

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWords ()) {
        std::stringstream ss;

        ss <<
          "Converting words \"" <<
          wordsValue <<
          "\" to an MSR dal segno " <<
          dalSegno->asString () <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWords ()) {
        std::stringstream ss;

        ss <<
          "Converting words " <<
          wordsValue <<
          " to an MSR dal segno " <<
          dalSegno->asString () <<
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append it to the pending codas list
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
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

//       convertWordsToCresc (
//         inputLineNumber,
//         wordsValue);
      // create an msrCrescDecresc
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceCrescDecrescs ()) {
        std::stringstream ss;

        ss <<
          "Creating an msrCrescDecresc crescendo" <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      S_msrCrescDecresc
        crescDecresc =
          msrCrescDecresc::create (
            inputLineNumber,
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
          ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
          ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

//       convertWordsToDecresc (
//         inputLineNumber,
//         wordsValue);
      // create an msrCrescDecresc
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceCrescDecrescs ()) {
        std::stringstream ss;

        ss <<
          "Creating an msrCrescDecresc decrescendo" <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      S_msrCrescDecresc
        crescDecresc =
          msrCrescDecresc::create (
            inputLineNumber,
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
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accordion_registration" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::visitStart (S_accordion_high& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accordion_high" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentAccordionHigh = 1;

  ++fCurrentAccordionNumbersCounter;
}

void mxsr2msrTranslator::visitStart (S_accordion_middle& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accordion_middle" <<
       ", line " << inputLineNumber <<
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

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());

    fCurrentAccordionMiddle = 1;
  }

  ++fCurrentAccordionNumbersCounter;
}

void mxsr2msrTranslator::visitStart (S_accordion_low& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accordion_low" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentAccordionLow = 1;

  ++fCurrentAccordionNumbersCounter;
}

void mxsr2msrTranslator::visitEnd (S_accordion_registration& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_accordion_registration" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // An accordion-registration element needs to have
  // at least one of the child elements present

  if (fCurrentAccordionNumbersCounter == 0) {
    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
  fCurrentMetronomeNotesDurationKind = msrNotesDurationKind::kNotesDuration_UNKNOWN_;
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beat_unit" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string beatUnitString = elt->getValue();

  // the type contains a display duration
  msrNotesDurationKind
    beatUnitNotesDurationKind =
      msrNotesDurationKindFromMusicXMLString (
        inputLineNumber,
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_beat_unit_dot& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beat_unit_dot" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentMetronomeBeatUnitsVector.size ()) {
    fCurrentMetronomeBeatUnitsVector.back ().incrDotsNumber ();
  }
  else {
    std::stringstream ss;

    ss <<
      "beat unit dot occurs without any prior beat unit";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_per_minute& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_per_minute" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMetrenomePerMinute = elt->getValue ();
}

void mxsr2msrTranslator::visitStart (S_metronome_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_note" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingMetronomeNote = true;
}

void mxsr2msrTranslator::visitStart (S_metronome_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_type" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string noteType = elt->getValue();

  // the type contains a display duration,
  fCurrentMetronomeNotesDurationKind =
    msrNotesDurationKindFromMusicXMLString (
     inputLineNumber,
     noteType);
}

void mxsr2msrTranslator::visitStart (S_metronome_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_dot" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentMetrenomeDotsNumber;
}

void mxsr2msrTranslator::visitStart (S_metronome_beam& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_beam" <<
      ", line " << inputLineNumber <<
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
  const S_msrTempoNote& tempoNote)
{
  // attach the current articulations if any to the note
  if (fPendingMetronomeBeamsList.size ()) {

#ifdef MF_TRACE_IS_ENABLED
    if (
      gTraceOahGroup->getTraceTempos ()
        ||
      gTraceOahGroup->getTraceBeams ()
    ) {
      std::stringstream ss;

      ss <<
        "Attaching current beams to tempoNote " <<
        tempoNote->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          " to tempoNote " << tempoNote->asString () <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_metronome_note" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_relation" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_metronome_tuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_metronome_tuplet" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
      else {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTuplets ()) {
          std::stringstream ss;

          ss <<
            "tuplet bracket is empty: this is implementation dependent," <<
            " \"yes\" is assumed"; // option ??? JMI

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          inputLineNumber,
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
      fCurrentTempoTupletShowNumberKind <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingMetronomeTuplet = true;
}

void mxsr2msrTranslator::visitStart (S_normal_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_normal_dot" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentMetrenomeNormalDotsNumber;
}

void mxsr2msrTranslator::visitEnd (S_metronome_tuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_metronome_tuplet" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_metronome" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
            inputLineNumber,
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Creating tempo " <<
      fCurrentMetronomeTempo->asString () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
            fCurrentMetronomeTempo->asString () <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentMetronomeTempo->
          appendWordsToTempo (words);

        // forget about this words
        fPendingWordsList.pop_front();
      } // while


      std::stringstream ss;

      ss <<
        "<direction/> contains " <<
        pendingWordsSize <<
        " <words/> markups";

      mxsr2msrWarningWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

/* if (true) { // JMI
*/
  // append the tempo to the pending tempos list
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

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssertmfAssertWithMeasureInfo (
    __FILE__, __LINE__,
    currentDirectionsVoice != nullptr,
    "currentDirectionsVoice is null",
    fCurrentMeasureNumber,
    fMsrScore->getScoreNumberOfMeasures ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // append the tempo to the voice
  currentDirectionsVoice->
    appendTempoToVoice (fCurrentMetronomeTempo);
  }
  */
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_staves& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_direction" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_staff& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMusicXMLStaffNumber = int(*elt);

  // the staff number should be positive
  if (fCurrentMusicXMLStaffNumber <= 0) {
    std::stringstream ss;

    ss <<
      "staff number " << fCurrentMusicXMLStaffNumber <<
      " is not positive" <<
      ", line " << inputLineNumber;

      mfAssertWithMeasureInfo(
        __FILE__, __LINE__,
        false,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
  }

  S_msrStaff
    staff =
      fetchStaffFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "--> S_staff, fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      "--> S_staff, current staff name: " <<
      staff->getStaffName() <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "<staff /> " <<
      fCurrentMusicXMLStaffNumber <<
      " is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_staff_details& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_details" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss << "print-spacing " << printSpacing << " is unknown";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Handling staff details:" <<
      std::endl <<
      gTab << "StaffDetailsStaffNumber" << ": " <<
      fStaffDetailsStaffNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_type" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_staff_lines& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_lines" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffLinesNumber = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_staff_tuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_tuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffTuningLine =
    elt->getAttributeIntValue ("line", 0);

  fCurrentStaffTuningAlterationKind = msrAlterationKind::kAlterationNatural; // may be absent
  fCurrentStaffTuningOctaveKind     = msrOctaveKind::kOctave_UNKNOWN_;

  fOnGoingStaffTuning = true;
}

void mxsr2msrTranslator::visitStart (S_tuning_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuning_step" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "<tuning-step /> " <<
      tuningStep <<
      " is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_tuning_octave& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuning_octave" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "<tuning-octave /> " <<
      tuningOctaveNumber <<
      " is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_tuning_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuning_alter" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitEnd (S_staff_tuning& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_staff_tuning" <<
       ", line " << inputLineNumber <<
     std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesDetails ()) {
    std::stringstream ss;

    ss <<
      "Creating staff tuning:" <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 32;

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_details" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== voice \"" << "elt->getVoiceName ()" << "\"" <<
      ", line " << elt->getInputLineNumber () << " ===-->" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "<voice /> " <<
      fCurrentMusicXMLVoiceNumber <<
      " is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_backup& elt)
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
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_backup" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingBackup = true;
}

void mxsr2msrTranslator::visitEnd (S_backup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_backup" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceBackup ()) {
    std::stringstream ss;

    ss <<
      "Backup by " <<
      mfSingularOrPlural (
        fCurrentBackupDivisions, "division", "divisions") <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
    decrementPartDrawingMeasurePosition (
      inputLineNumber,
      backupStepLength);

  // reset notes staff numbers // JMI v0.9.67
//   fPreviousNoteMusicXMLStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_;
//   fCurrentMusicXMLStaffNumber      = msrStaff::K_STAFF_NUMBER_UNKNOWN_;

  // handle the backup right now:
//   handleBackup (
//     inputLineNumber);

  // reset staff change detection
  // only now, it is used by handleBackup() v0.9.63 JMI ???

  fOnGoingBackup = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_forward& elt)
{
  int inputLineNumber = elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_forward" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the <staff /> and <voice /> are optional,
  // in which case the same staff/voice number remains in effect
  fCurrentForwardStaffNumber = fCurrentMusicXMLStaffNumber;
  fCurrentForwardVoiceNumber = fCurrentMusicXMLVoiceNumber;

  fOnGoingForward = true;
}

void mxsr2msrTranslator::visitEnd (S_forward& elt)
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

  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_forward" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
  if (gGlobalMxsrOahGroup->getTraceForward ()) {
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
      fCurrentForwardVoiceNumber <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // account for forwardStepLength wholeNotes in the part current measure position
  fCurrentPart->
    incrementPartDrawingMeasurePosition (
      inputLineNumber,
      forwardStepLength);

  // the staff number if any should be positive
  if (fCurrentForwardStaffNumber <= 0) {
    std::stringstream ss;

    ss <<
      "staff number " << fCurrentForwardStaffNumber <<
      " is not positive" <<
      ", line " << inputLineNumber;

      mfAssertWithMeasureInfo (
        __FILE__, __LINE__,
        false,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
  }

  // the voice number if any should be positive
  if (fCurrentForwardVoiceNumber <= 0) {
    std::stringstream ss;

    ss <<
      "voice number " << fCurrentForwardVoiceNumber <<
      " is not positive" <<
      ", line " << inputLineNumber;

      mfAssertWithMeasureInfo (
        __FILE__, __LINE__,
        false,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
  }

  // fetch the voice to be forwarded to
  S_msrVoice
    voiceToBeForwardedTo =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentForwardStaffNumber,
        fCurrentForwardVoiceNumber);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssertWithMeasureInfo (
    __FILE__, __LINE__,
    voiceToBeForwardedTo != nullptr,
    "voiceToBeForwardedTo is null",
    fCurrentMeasureNumber,
    fMsrScore->getScoreNumberOfMeasures ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // compute the forward step length
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceForward ()) {
    std::stringstream ss;

    ss <<
      "--> current forward divisions: " <<
      fCurrentForwardDivisions <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append a padding note to the voice to be forwarded
  voiceToBeForwardedTo ->
    appendPaddingNoteToVoice (
      inputLineNumber,
      forwardStepLength);

  // reset staff change detection

  fOnGoingForward = false;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_tied& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tied" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
    }

    // inner tied notes may miss the "continue" type:
    // let' complain on ligature notes outside of ligatures
    if (! fOnGoingLigature) {
      if (tiedType.size ()) {
        std::stringstream ss;

        ss << "tied type \"" << fCurrentSlurType << "\" inside a ligature is unknown";

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
  gLog <<
    std::endl <<
    ">>++++++++++++++++ " <<
    "The slurs starts stack contains:" <<
    std::endl;

  if (fSlurStartsStack.size ()) {
    std::list<S_msrSlur>::const_iterator
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

//________________________________________________________________________
void mxsr2msrTranslator::displayTupletsStack (
  const std::string& context)
{
  size_t tupletsStackSize = fTupletsStack.size ();

  gLog <<
    std::endl <<
    ">>++++++++++++++++ " <<
    "The tuplets stack contains " << tupletsStackSize << " elements:" <<
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
      gLog <<
        "v (" << n << ")" <<
        std::endl;

      ++gIndenter;
      gLog << tuplet;
      --gIndenter;

      --n;

      if (++i == iEnd) break;

      gLog << std::endl;
    } // for

    --gIndenter;
  }

  gLog <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_slur& elt)
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

  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slur" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalMxsr2msrOahGroup->getIgnoreSlurs ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlursDetails ()) {
      displaySlurStartsStack ("BEFORE handling slur");
    }
#endif // MF_TRACE_IS_ENABLED

    /*
      Only the  first note of the chord should get the slur notation.
      Some applications print out the slur for all notes,
      i.e. a stop and a start in sequqnce:
      these should be ignored
    */
    if (fCurrentNoteBelongsToAChord) {
      std::stringstream ss;

      ss <<
        "ignoring a slur in a chord member note other than the first one";

      mxsr2msrWarningWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
                  ", line " << inputLineNumber <<
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
//               mxsr2msrWarningWithMeasureInfo (
//                 gServiceRunData->getInputSourceName (),
//                 inputLineNumber,
//         //        __FILE__, __LINE__,
//                 ss.str (),
//                 fCurrentMeasureNumber,
//                 fMsrScore->getScoreNumberOfMeasures ());
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

              mxsr2msrWarningWithMeasureInfo (
                gServiceRunData->getInputSourceName (),
                inputLineNumber,
                ss.str (),
                fCurrentMeasureNumber,
                fMsrScore->getScoreNumberOfMeasures ());
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
                    ", line " << inputLineNumber <<
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

            mxsr2msrErrorWithMeasureInfo (
              gServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              ss.str (),
              fCurrentMeasureNumber,
              fMsrScore->getScoreNumberOfMeasures ());
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
            inputLineNumber,
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
            slurLineTypeKind) <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      S_msrSlur
        slur =
          msrSlur::create (
            inputLineNumber,
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlursDetails ()) {
    displaySlurStartsStack ("AFTER handling slur");
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_bracket& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bracket" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

              mxsr2msrWarningWithMeasureInfo (
                gServiceRunData->getInputSourceName (),
                inputLineNumber,
                ss.str (),
                fCurrentMeasureNumber,
                fMsrScore->getScoreNumberOfMeasures ());
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

              mxsr2msrWarningWithMeasureInfo (
                gServiceRunData->getInputSourceName (),
                inputLineNumber,
                ss.str (),
                fCurrentMeasureNumber,
                fMsrScore->getScoreNumberOfMeasures ());
            }
#endif // MF_TRACE_IS_ENABLED

            ligature->setLigaturePlacementKind (msrPlacementKind::kPlacementAbove);

            if (! fCurrentLigatureStartAbove) {
              std::stringstream ss;

              ss <<
                "Bracket 'stop above' found with no corresponding bracket start, ignoring it"; // JMI cannot occur

           // JMI   mxsr2msrError (
              mxsr2msrWarningWithMeasureInfo (
                gServiceRunData->getInputSourceName (),
                inputLineNumber,
           //     __FILE__, __LINE__,
                ss.str (),
                fCurrentMeasureNumber,
                fMsrScore->getScoreNumberOfMeasures ());
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
            mxsr2msrWarningWithMeasureInfo (
              gServiceRunData->getInputSourceName (),
              inputLineNumber,
         //     __FILE__, __LINE__,
              ss.str (),
              fCurrentMeasureNumber,
              fMsrScore->getScoreNumberOfMeasures ());
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
            mxsr2msrWarningWithMeasureInfo (
              gServiceRunData->getInputSourceName (),
              inputLineNumber,
         //     __FILE__, __LINE__,
              ss.str (),
              fCurrentMeasureNumber,
              fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_wedge" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_lyric" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  {
    fCurrentStanzaNumber =
      elt->getAttributeValue ("number");

    if (fCurrentStanzaNumber.size () == 0) {
      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        "lyric number is empty, using \"1\" by default");

      fCurrentStanzaNumber = "1";
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "Setting fCurrentStanzaNumber to " <<
        fCurrentStanzaNumber <<
        ", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          msrStanza::K_STANZA_NAME_UNKNOWN_ <<
          "\" by default" <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentStanzaName = msrStanza::K_STANZA_NAME_UNKNOWN_;
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "Setting fCurrentStanzaName to \"" <<
        fCurrentStanzaName <<
        "\", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_syllabic" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "syllabic \"" << fCurrentSyllabic <<
      "\" is unknown";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

  // forget about any previous texts found
  fCurrentLyricTextsList.clear ();
}

void mxsr2msrTranslator::visitStart (S_text& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_text" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string textValue = elt->getValue();

  mfConvertHTMLEntitiesToPlainCharacters (textValue); // JMI ???

  // color JMI

  // there can be several <text/>'s and <elision/> in a row, hence the list
  fCurrentLyricTextsList.push_back (textValue);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "textValue = \"" << textValue << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteHasLyrics = true;
  fCurrentStanzaHasText = true;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    ++gIndenter;

    const int fieldWidth = 23;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "line " << ": " << inputLineNumber <<
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
      "fCurrentLyricTextsList" << " = ";

    msrSyllable::writeTextsList (
      fCurrentLyricTextsList,
      gLog);

    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // a <text/> markup puts an end to the effect of <extend/>
  fCurrentSyllableExtendKind = msrSyllableExtendKind::kSyllableExtendNone;
}

void mxsr2msrTranslator::visitStart (S_elision& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_elision" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string elisionValue = elt->getValue ();

  if (! elisionValue.size ()) {
    elisionValue = ' ';
  }

  // color JMI

  // there can be several <text/>'s and <elision/> in a row, hence the list
  fCurrentLyricTextsList.push_back (elisionValue);

  fCurrentStanzaHasText = true;
}

void mxsr2msrTranslator::visitStart (S_extend& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_extend" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        std::stringstream ss;

        ss <<
          "extend type \"" << extendType <<
          "\" is unknown";

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_lyric" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentSyllableKind == msrSyllableKind::kSyllableNone) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      // syllabic is not mandatory, thus:
      std::stringstream ss;

      ss <<
        "<lyric /> has no <syllabic/> component, using 'single' by default";

      mxsr2msrWarningWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentSyllableKind = msrSyllableKind::kSyllableSingle;
  }

  if (fCurrentNoteIsARest) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "syllable ";

     msrSyllable::writeTextsList (
      fCurrentLyricTextsList,
      ss);

      ss <<
        " is attached to a rest";

      mxsr2msrWarningWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyricsDetails ()) {
    gLog <<
      "==> visitEnd (S_lyric&), fCurrentSyllableKind: " <<
      msrSyllableKindAsString (fCurrentSyllableKind) <<
      ", line " << inputLineNumber <<
      ", with:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      "Lyric data:" <<
      std::endl;

    {
      ++gIndenter;

      const int fieldWidth = 31;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentMusicXMLStaffNumber" << ": " << fCurrentMusicXMLStaffNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaNumber" << ": " << fCurrentStanzaNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentLyricTextsList" << " = ";

      msrSyllable::writeTextsList (
        fCurrentLyricTextsList,
        gLog);

      gLog << std::left <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentSyllableExtendKind" << ": " <<
        msrSyllableExtendKindAsString (
          fCurrentSyllableExtendKind) <<
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
        msrSyllableKindAsString (
          fFirstSyllableInSlurKind) <<
        "\"" <<
        std::endl;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fFirstSyllableInLigatureKind" << ": \"" <<
        msrSyllableKindAsString (
          fFirstSyllableInLigatureKind) <<
        "\"" <<
      std::endl;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentSyllableKind" << ": \"" <<
        msrSyllableKindAsString (
          fCurrentSyllableKind) <<
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
      msrSyllableKindAsString (fCurrentSyllableKind) <<
      ", line: " << inputLineNumber <<
      ", with:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      "Lyric data:" <<
      std::endl;

    {
      ++gIndenter;

      const int fieldWidth = 31;

      gLog << std::left <<
        std::setw (fieldWidth) <<
        "fCurrentMusicXMLStaffNumber" << ": " << fCurrentMusicXMLStaffNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaNumber" << ": " << fCurrentStanzaNumber <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentStanzaName" << " = \"" << fCurrentStanzaName << "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fCurrentLyricTextsList" << " = ";

      msrSyllable::writeTextsList (
        fCurrentLyricTextsList,
        gLog);

      --gIndenter;
    }

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyrics ()) {
      std::stringstream ss;

      ss <<
        "Creating a syllable \"" <<
        msrSyllableKindAsString (
          fCurrentSyllableKind) <<
        "\", fCurrentLyricTextsList = \"";

      msrSyllable::writeTextsList (
        fCurrentLyricTextsList,
        gLog);

      ss <<
        "\"" <<
        ", whole notes: " <<
        fCurrentNoteSoundingWholeNotesFromNotesDuration <<
        " sounding from duration, " <<
         fCurrentNoteDisplayWholeNotesFromType <<
        ", display from type" <<
        ", syllabic: \"" <<
        msrSyllableKindAsString (
          fCurrentSyllableKind) << "\"" <<
        ", in stanza " << stanza->getStanzaName () <<
        ", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create a syllable
    S_msrSyllable
      syllable =
        msrSyllable::create (
          inputLineNumber,
          fCurrentSyllableKind,
          fCurrentSyllableExtendKind,
          fCurrentStanzaNumber,
          fCurrentNoteSoundingWholeNotesFromNotesDuration,
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

    // append syllable to current note's syllables list
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      ", line " << inputLineNumber <<
      ", in part \"" <<
      fCurrentPart->getPartCombinedName () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number
  fCurrentMeasureNumber =
    elt->getAttributeValue ("number");

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasures ()
      ||
    gEarlyOptions.getEarlyTracePasses ()
  ) {
      std::stringstream ss;

      ss <<
//       std::endl <<
      "<!--=== " <<
      "part \"" << fCurrentPart->getPartName () << "\"" <<
      " (partID \"" << fCurrentPart->getPartID () << "\")" <<
      ", measure \"" << fCurrentMeasureNumber << "\"" <<
      ", line " << inputLineNumber <<
      " ===-->" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the current measure number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentMeasureNumber (fCurrentMeasureNumber);

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
        std::stringstream ss;

        ss <<
          "measure numbering inconsistency: first measure numbers " <<
          fScoreFirstMeasureNumber <<
           " and " <<
          fPartFirstMeasureNumber <<
          " found";

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss <<
        "implicit \"" << implicit <<
        "\" is unknown";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

  // reset the part measure position
  fCurrentPart->
    setPartDrawingMeasurePosition (
      inputLineNumber,
      msrWholeNotes (0, 1));

  // forget about the current non-grace note JMI v0.9.67???
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
  fPreviousNoteMusicXMLStaffNumber = msrStaff::K_STAFF_NUMBER_UNKNOWN_; // default value
  fCurrentMusicXMLStaffNumber      = msrStaff::K_STAFF_NUMBER_UNKNOWN_; // default value

/* JMI
  // is this measure number in the debug set?
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDetailed ()) {
    std::set<int>::const_iterator
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
}

void mxsr2msrTranslator::visitEnd (S_measure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_measure" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch the voice
  S_msrVoice
    voice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // fetch note to attach to
  S_msrNote
    noteToAttachTo =
    /*
    // JMI might prove not precise enough???
//      fVoicesLastMetNoteMap [currentVoice];
      fVoicesLastMetNoteMap [
        std::make_pair (
          fCurrentMusicXMLStaffNumber,
          fCurrentMusicXMLVoiceNumber)
        ];
    */
      voice->getVoiceLastAppendedNote (); // ??? JMI

  // is there a pending grace notes group?
  if (fPendingGraceNotesGroup) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "fCurrentGraceNotes IS NOT NULL at the end of measure " << // JMI
        elt->getAttributeValue ("number") <<
        std::endl;

      ss <<
        "+++++++++++++++++" <<
        fCurrentPart <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    // attach these grace notes group as an after grace notes group
    // to the last note found in its voice
    if (! noteToAttachTo) {
      std::stringstream ss;

      ss <<
        "cannot find voice to insert after grace notes group into: " <<
        "chordFirstNote is null" <<
        ", fCurrentMusicXMLStaffNumber: " <<
        fCurrentMusicXMLStaffNumber <<
        std::endl <<
        ", fCurrentMusicXMLVoiceNumber: " <<
        fCurrentMusicXMLVoiceNumber;

      mxsr2msrInternalErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
  if (false && fOnGoingChord) { // JMI v0.9.67
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
  attachCurrentSpannersToNote (
    noteToAttachTo,
    "mxsr2msrTranslator::visitEnd (S_measure& elt)");

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
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "Replicating meaure " <<
          fCurrentMeasureNumber <<
          " in part " <<
          fCurrentPart->getPartCombinedName () <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
      // fCurrentMeasureNumber is present in the map,
      // fetch the number of empty measures to add
      std::stringstream ss;

      ss << (*it).second;

      int measuresToBeAdded;

      ss >> measuresToBeAdded;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "Adding " <<
          mfSingularOrPlural (
            measuresToBeAdded, "empty measure", "empty measures") <<
          " to part " <<
          fCurrentPart->getPartCombinedName () <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    const int fieldWidth = 37;

    gLog <<
      "--> handleOnGoingMultipleFullBarRestsAtTheEndOfMeasure()" <<
      std::endl;

    ++gIndenter;

    gLog <<
      std::setw (fieldWidth) <<
      "fCurrentMultipleFullBarRestsHasBeenCreated " << ": " <<
      fCurrentMultipleFullBarRestsHasBeenCreated <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fRemainingExpectedMultipleFullBarRests" << ": " <<
      fRemainingExpectedMultipleFullBarRests <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

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
      gServiceRunData->getInputSourceName (),
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    const int fieldWidth = 37;

    gLog <<
      "--> handleOnGoingMultipleFullBarRestsAtTheEndOfMeasure()" <<
      ", onGoingMultipleFullBarRests:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      std::setw (fieldWidth) <<
      "fCurrentMultipleFullBarRestsHasBeenCreated " << ": " <<
      fCurrentMultipleFullBarRestsHasBeenCreated <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fRemainingExpectedMultipleFullBarRests" << ": " <<
      fRemainingExpectedMultipleFullBarRests <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fOnGoingMultipleFullBarRests " << ": " <<
      fOnGoingMultipleFullBarRests <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_print& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_print" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber);

  // handle 'staff-spacing' if present

  const std::string staffSpacing =
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
    const std::string newSystem = elt->getAttributeValue ("new-system");

    if (newSystem.size ()) {
      fCurrentMusicXMLPrintLayout->setNewSystem ();

      if (newSystem == "yes") {
        // create a line break
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceLineBreaks ()) {
          std::stringstream ss;

          ss <<
            "Creating a line break, " <<
            "line: " << inputLineNumber <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
  #endif // MF_TRACE_IS_ENABLED

        S_msrLineBreak
          lineBreak =
            msrLineBreak::create (
              inputLineNumber,
              fCurrentMeasureNumber,
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
    }
  }

  // handle 'new-page' if present and relevant

  if (! gGlobalMxsr2msrOahGroup->getIgnoreMusicXMLPageBreaks ()) {
    const std::string newPage = elt->getAttributeValue ("new-page");

    if (newPage.size ()) {
      fCurrentMusicXMLPrintLayout->setNewPage ();

      if (newPage == "yes") { // JMI
        // create a page break
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTracePageBreaks ()) {
          gLog <<
            "Creating a page break, " <<
            "line: " << inputLineNumber <<
            std::endl;
        }
  #endif // MF_TRACE_IS_ENABLED

        S_msrPageBreak
          pageBreak =
            msrPageBreak::create (
              inputLineNumber,
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::visitEnd (S_print& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_print" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the current print layout to voice 1 in staff 1 of the current part
  // it's not worth using specific 'layout voices' for such part-level stuff
  S_msrVoice
    voiceOneInStaffOne =
      fetchFirstVoiceFromCurrentPart (
        inputLineNumber);

  voiceOneInStaffOne->
    appendMusicXMLPrintLayoutToVoice (
      fCurrentMusicXMLPrintLayout);

  // forget about the current print layout
  fCurrentMusicXMLPrintLayout = nullptr;

  fOnGoingPrint = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_measure_numbering& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_numbering" <<
       ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_barline& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_barline" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss <<
        "barLine location \"" << location <<
        "\" is unknown, using 'right' by default";

   // JMI   mxsr2msrError (
      mxsr2msrWarningWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
   //     __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

  fOnGoingBarLine = true;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_bar_style& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bar_style" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_segno" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingDirectionType) {
    // create the segno
    S_msrSegno
      segno =
        msrSegno::create (
          inputLineNumber,
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_coda& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_coda" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

          mxsr2msrErrorWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
    } // switch

    // create the coda
    S_msrCoda
      coda =
        msrCoda::create (
          inputLineNumber,
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_eyeglasses& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_eyeglasses" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingDirectionType) {
    // create the eyeglasses
    S_msrEyeGlasses
      eyeGlasses =
        msrEyeGlasses::create (
          inputLineNumber);

    // append it to the pending eyeglasses list
    fPendingEyeGlassesList.push_back (eyeGlasses);
  }

  else {
    std::stringstream ss;

    ss << "<eyeGlasses /> is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_pedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pedal" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
    // append it to the pending pedals list
    fPendingPedalsList.push_back (pedal);
  }
  else {
    std::stringstream ss;

    ss << "<pedal /> " << pedal->asShortString () << " is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_ending& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ending" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // number

  {
    fCurrentBarLineEndingNumber =
      elt->getAttributeValue ("number"); // may be "1, 2"

    if (! fCurrentBarLineEndingNumber.size ()) {
      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
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
      std::stringstream ss;

      ss <<
        "ending type \"" << type <<
        "\" is unknown";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_repeat" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_barline" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        msrBarLineCategoryKind::kBarLineCategory_UNKNOWN_, // will be set afterwards
        fCurrentBarLineHasSegnoKind,
        fCurrentBarLineHasCodaKind,
        fCurrentBarLineRepeatWingedKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Creating barLine in part " <<
      fCurrentPart->getPartCombinedName () << ":" <<
      std::endl;

    ++gIndenter;

    gLog <<
      barLine;

    --gIndenter;
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
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceBarLines ()) {
          std::stringstream ss;

          ss <<
            "Appending a standalone barLine to part " <<
            fCurrentPart->getPartCombinedName () << ":" <<
            std::endl;

          ++gIndenter;

          ss <<
            barLine;

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

        mxsr2msrWarningWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
     //     __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
        break;
    } // switch
  }

  // has this barLine been handled?
  if (! barLineHasBeenHandled) {
    std::stringstream ss;

    ss << std::left <<
      "cannot handle a barLine containing: " <<
      barLine->asString ();

    mxsr2msrInternalWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
          " has no barLine category";

      mxsr2msrInternalErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
void mxsr2msrTranslator::visitStart (S_note& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_note" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

  fCurrentNoteDiatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentNoteAlterationKind    = msrAlterationKind::kAlterationNatural;

  fCurrentNoteOctave = msrOctaveKind::kOctave_UNKNOWN_;

  fCurrentNoteSoundingWholeNotes                  = msrWholeNotes (0, 1);
  fCurrentNoteSoundingWholeNotesFromNotesDuration = msrWholeNotes (0, 1);

  fCurrentDisplayDiatonicPitchKind      = msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_;
  fCurrentDisplayOctave                 = msrOctaveKind::kOctave_UNKNOWN_;
  fCurrentNoteDisplayWholeNotes         = msrWholeNotes (0, 1);
  fCurrentNoteDisplayWholeNotesFromType = msrWholeNotes (0, 1);

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

  fCurrentHarmonyWholeNotesOffset = msrWholeNotes (0, 1);

  // lyrics

  fCurrentStanzaNumber = msrStanza::K_STANZA_NUMBER_UNKNOWN_;
  fCurrentStanzaName = msrStanza::K_STANZA_NAME_UNKNOWN_;

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

  fCurrentDoubleTremoloTypeKind = msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_;

  // ties

  fCurrentTie = nullptr;
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
    std::stringstream ss;

    ss <<
      "note color \"" << noteColorAlphaRGB <<
      "\" should contain 6 or 8 upper case hexadecimal digits prededed by a '#'";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

  fOnGoingNote = true;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_step" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_alter" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_octave& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_octave" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  int octaveNumber = (int)(*elt);

  if (octaveNumber < 0 || octaveNumber > 9) {
    std::stringstream ss;

    ss <<
      "octave number " << octaveNumber <<
      " is not in the 0..9 range, '0' is assumed";

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_duration" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  int duration = (int)(*elt); // divisions

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDurations ()) {
    std::stringstream ss;

    ss <<
      "NotesDuration: " << duration <<
      ", fOnGoingBackup: " << fOnGoingBackup <<
      ", fOnGoingForward: " << fOnGoingForward <<
      ", fOnGoingNote: " << fOnGoingNote <<
      ", fOnGoingFiguredBass: " << fOnGoingFiguredBass <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
        fCurrentDivisionsPerQuarterNote <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
        fCurrentNoteSoundingWholeNotesFromNotesDuration <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
        fCurrentDivisionsPerQuarterNote <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
        fCurrentFiguredBassSoundingWholeNotes.asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss << "<duration /> " << duration << " is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

//  gLog << "=== mxsr2msrTranslator::visitStart (S_duration& elt), fCurrentNotesDuration: " << fCurrentNotesDuration << std::endl; JMI
}

void mxsr2msrTranslator::visitStart (S_instrument& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::visitStart (S_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_dot" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentNoteDotsNumber;
}

void mxsr2msrTranslator::visitStart (S_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_type" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
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

void mxsr2msrTranslator::visitStart (S_notehead& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_notehead" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
    }
  }

  // color JMI

}

void mxsr2msrTranslator::visitStart (S_accidental& elt) // JMI
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accidental" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
    }
  }
}

void mxsr2msrTranslator::visitStart (S_stem& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_stem" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

  std::string        stem = elt->getValue();

  // kind
  msrStemKind stemKind = msrStemKind::kStemNeutral;

  if      (stem == "up")
    stemKind = msrStemKind::kStemUp;

  else if (stem == "down")
    stemKind = msrStemKind::kStemDown;

  else if (stem == "none")
    stemKind = msrStemKind::kStemNeutral;

  else if (stem == "double")
    stemKind = msrStemKind::kStemDouble;

  else {
    std::stringstream ss;

    ss <<
      "stem \"" << stem <<
      "\" is unknown";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beam" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

  // number

  fCurrentBeamNumber =
    elt->getAttributeIntValue ("number", 1); // default value

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_style" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // color JMI
}

void mxsr2msrTranslator::visitStart (S_beat_repeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_beat_repeat" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber,
      useDotsString);
}

void mxsr2msrTranslator::visitStart (S_measure_repeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_measure_repeat" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      fCurrentPart->getPartCombinedName () <<
      ", fCurrentMeasureRepeatMeasuresNumber: " <<
      fCurrentMeasureRepeatMeasuresNumber <<
      ", fCurrentMeasureRepeatSlashesNumber: " <<
      fCurrentMeasureRepeatSlashesNumber <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

    fCurrentPart->
      createMeasureRepeatFromItsFirstMeasuresInPart (
        inputLineNumber,
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
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

    fCurrentPart->
      appendPendingMeasureRepeatToPart (
        inputLineNumber);
  }

  else {
    std::stringstream ss;

    ss <<
      "measure-repeat type \"" << measureRepeatType <<
      "\" is unknown";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_multiple_rest" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
      std::stringstream ss;

      ss <<
        "multiple rest use symbols " <<
        useSymbols <<
        " is unknown";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slash" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss <<
        "slash use-stems \"" << slashUseStems <<
        "\" is unknown";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

  fCurrentSlashDotsNumber = 0;
}

void mxsr2msrTranslator::visitStart (S_slash_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slash_type" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string slashType = elt->getValue();

  // the type contains a display duration,
  fCurrentSlashGraphicNotesDurationKind =
    msrNotesDurationKindFromMusicXMLString (
      inputLineNumber,
      slashType);

  // size

  std::string slashTypeSize = elt->getAttributeValue ("size");

  if (slashTypeSize == "cue") { // USE IT! JMI ???
  }

  else {
    if (slashTypeSize.size ()) {
      mxsr2msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
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
      "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitStart (S_slash_dot& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slash_dot" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentSlashDotsNumber;
}

void mxsr2msrTranslator::visitEnd (S_slash& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_slash" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_articulations" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitStart (S_accent& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accent" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_breath_mark" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_caesura" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_spiccato" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staccato" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staccatissimo" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_stress" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

 #ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_unstress" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_detached_legato" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_strong_accent" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tenuto" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_doit" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_falloff" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_plop" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_scoop" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_articulations" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_arpeggiate& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_arpeggiate" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
      std::stringstream ss;

      ss <<
        "arpeggiate direction \"" << directionString << "\"" << "is unknown";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_non_arpeggiate" << // JMI
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

      std::stringstream ss;

      ss <<
        "non-arpeggiate type \"" << typeString <<
        "\" is unknown";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_technical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTechnical = true;
}

void mxsr2msrTranslator::visitEnd (S_technical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_technical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTechnical = false;
}

void mxsr2msrTranslator::visitStart (S_arrow& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_arrow" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bend_alter" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBendAlterValue = (float)(*elt);
}

void mxsr2msrTranslator::visitStart (S_bend& elt) // JMI
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bend" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitEnd (S_bend& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bend" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_double_tongue" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_down_bow" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fingering" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "<fingering /> \"" << fingeringValue <<
      "\" is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_fingernails& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fingernails" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fret" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
    std::stringstream ss;

    ss <<
      "<fret /> \"" << fretValue <<
      "\" is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_hammer_on& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_hammer_on" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_handbell" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        msrTechnicalTypeKind::kTechnicalType_UNKNOWN_,
        handBellValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrTranslator::visitStart (S_harmonic& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_harmonic" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_heel" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_hole" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_open_string" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_other_technical" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        msrTechnicalTypeKind::kTechnicalType_UNKNOWN_,
        otherTechnicalValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrTranslator::visitStart (S_pluck& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pluck" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        msrTechnicalTypeKind::kTechnicalType_UNKNOWN_,
        pluckValue,
        placementKind);

  fCurrentTechnicalWithStringsList.push_back (technicalWithString);
}

void mxsr2msrTranslator::visitStart (S_pull_off& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pull_off" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_snap_pizzicato" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_stopped" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_string" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
    std::stringstream ss;

    ss <<
      "<std::string /> \"" << stringValue <<
      "\" is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_tap& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tap" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_thumb_position" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_toe" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_triple_tongue" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_up_bow" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fermata" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ornaments" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitStart (S_tremolo& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting tremolo" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

  if (tremoloMarksNumber < 0 || tremoloMarksNumber > 8) { // JMI what does 0 mean?
    std::stringstream ss;

    ss <<
      "tremolo value \"" << tremoloMarksNumber <<
      "\" should be between 0 and 8";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
        inputLineNumber,
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
          ", line " << inputLineNumber <<
          ", " <<
          mfSingularOrPlural (
            tremoloMarksNumber, "mark", "marks") <<
          ", placement : " <<
          msrPlacementKindAsString (
            singleTremoloPlacementKind) <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
              fCurrentMusicXMLStaffNumber,
              fCurrentMusicXMLVoiceNumber);

        // create a double tremolo start
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTremolos ()) {
          std::stringstream ss;

          ss <<
            "Creating a double tremolo" <<
            ", line " << inputLineNumber <<
            ", " <<
            mfSingularOrPlural (
              tremoloMarksNumber, "mark", "marks") <<
            ", placement : " <<
            msrPlacementKindAsString (
              doubleTremoloPlacementKind) <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

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
        std::stringstream ss;

        ss <<
          "<tremolo/> start when a current double tremolo is already open";

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
            ", line " << inputLineNumber <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

        // it will be handled in
        // handleNonChordNorTupletNoteOrRest()
      }

      else {
        std::stringstream ss;

        ss <<
          "<tremolo/> stop whit no preceeding <tremolo/> start";

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
      break;
  } // switch
}

void mxsr2msrTranslator::visitStart (S_trill_mark& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_trill_mark" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_dashes" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_wavy_line" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrWarningWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::visitStart (S_turn& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_turn" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_inverted_turn" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_delayed_turn" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_delayed_inverted_turn" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_vertical_turn" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_mordent" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_inverted_mordent" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_schleifer" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_shake" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accidental_mark" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
    std::stringstream ss;

    ss <<
      "accidental-mark \"" << accidentalMark <<
      "\" is unknown";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_ornaments" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart( S_f& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_f" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ff" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fff" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ffff" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fffff" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ffffff" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_p" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ppp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pppp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_ppppp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pppppp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_mf" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_mp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_fz" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pf" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rf" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sf" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rfz" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sfz" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sfp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sfpp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sffz" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sfzp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_n" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_other_dynamics" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_damper_pedal" <<
      ", line " << inputLineNumber <<
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

      mxsr2msrErrorWithMeasureInfo (
        inputLineNumber,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_soft_pedal" <<
       ", line " << inputLineNumber <<
     std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_sostenuto_pedal" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_cue" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteIsACueNoteKind = msrNoteIsACueNoteKind::kNoteIsACueNoteYes;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_grace& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_grace" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        gServiceRunData->getInputSourceName (),
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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_chord" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_time_modification" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // there may be no '<tuplet number="n" type="start" />'
  // in the tuplet notes after the first one,
  // so we detect tuplet notes on '<time-modification>' ??? JMI
  fCurrentNoteHasATimeModification = true;
}

void mxsr2msrTranslator::visitStart (S_actual_notes& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_actual_notes" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
        fCurrentNoteActualNotes <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTempos ()) {
      std::stringstream ss;

      ss <<
        "fCurrentMetronomeNoteActualNotes: " <<
        fCurrentMetronomeNoteActualNotes <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "<actual-notes /> \"" << actualNotes <<
      "\" is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_normal_notes& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_normal_notes" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
        fCurrentNoteNormalNotes <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTempos ()) {
      std::stringstream ss;

      ss <<
        "fCurrentMetronomeNoteNormalNotes: " <<
        fCurrentMetronomeNoteNormalNotes <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    std::stringstream ss;

    ss <<
      "<normal-notes /> \"" << normalNotes <<
      "\" is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_normal_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_normal_type" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string normalTypeString = elt->getValue();

  if (fOnGoingNote) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      std::stringstream ss;

      ss <<
        "normalTypeString: " <<
        normalTypeString <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    // the type contains a display duration
    fCurrentNoteNormalTypeNotesDuration =
      msrNotesDurationKindFromMusicXMLString (
        inputLineNumber,
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
        fCurrentMetronomeNoteNormalType <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

  }

  else {
    std::stringstream ss;

    ss <<
      "<normal-type /> \"" << normalTypeString <<
      "\" is out of context";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_tuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        std::stringstream ss;

        ss <<
          "tuplet bracket \"" << tupletBracket <<
          "\" is unknown";

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
      else {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTuplets ()) {
          std::stringstream ss;

          ss <<
            "tuplet bracket is empty: this is implementation dependent," <<
            " \"yes\" is assumed"; // option ??? JMI

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED
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

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTuplets ()) {
        std::stringstream ss;

        ss <<
          "--> There is a tuplet start (kTupletTypeStart)" <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStart;
    }
    else if (tupletType == "continue") {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceTuplets ()) {
        std::stringstream ss;

        ss <<
          "--> There is a tuplet continue (kTupletTypeContinue)" <<
          ", line " << inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTuplets ()) {
          std::stringstream ss;

          ss <<
            "--> There is a tuplet stop right after a tuplet start for tuplet nummber " << fCurrentTupletNumber <<
            " (kTupletTypeStartAndStopInARow)" <<
            ", line " << inputLineNumber <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStartAndStopInARow;
      }
      else {
        // this is a 'regular' tuplet stop
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTuplets ()) {
          std::stringstream ss;

          ss <<
            "--> There is a tuplet stop (kTupletTypeStop)" <<
            ", line " << inputLineNumber <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStop;
      }
    }
    else {
      std::stringstream ss;

      ss <<
        "tuplet type \"" << tupletType <<
        "\" is unknown";

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
    else if (tupletShowType == "none") {
      fCurrentTupletShowTypeKind = msrTupletShowTypeKind::kTupletShowTypeNone;
    }
    else {
      if (tupletShowType.size ()) {
        mxsr2msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          "tuplet show-type \"" + tupletShowType + "\" is unknown");
      }
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
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

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteBelongsToATuplet = true;
}

void mxsr2msrTranslator::visitStart (S_tuplet_actual& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_actual" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletActual = true;
}

void mxsr2msrTranslator::visitEnd (S_tuplet_actual& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet_actual" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletActual = false;
}

void mxsr2msrTranslator::visitStart (S_tuplet_normal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_normal" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletNormal = true;
}

void mxsr2msrTranslator::visitEnd (S_tuplet_normal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_tuplet_normal" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingTupletNormal = false;
}

void mxsr2msrTranslator::visitStart (S_tuplet_number& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_number" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber,
      __FILE__, __LINE__,
      "<tuplet-number /> out of context");
  }

  // color JMI

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "tuplet number (not handled): " <<
      tupletNumberValue <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitStart (S_tuplet_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_type" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber,
      __FILE__, __LINE__,
      "<tuplet-type /> out of context");
  }

  // color JMI

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "tuplet type (not handled): " <<
      tupletTypeValue <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitStart (S_tuplet_dot& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_tuplet_dot" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_glissando" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
        inputLineNumber,
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
      "glissandoTextValue: " << glissandoTextValue <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGlissandos ()) {
    std::stringstream ss;

    ss <<
      "Appending glissando " <<
      glissando->asString () <<
      " to the glissandos pending list" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPendingGlissandosList.push_back (glissando);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_slide& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_slide" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
        inputLineNumber,
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
      slideLineTypeKind <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlides ()) {
    std::stringstream ss;

    ss <<
      "Appending slide " <<
      slide->asString () <<
      " to the slides pending list" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPendingSlidesList.push_back (slide);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_rest& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rest" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_display_step" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_display_octave" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  int displayOctaveNumber = (int)(*elt);

  if (displayOctaveNumber < 0 || displayOctaveNumber > 9) {
    std::stringstream ss;

    ss <<
      "display octave number " << displayOctaveNumber <<
      " is not in the 0..9 range, '0' is assumed";

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());

    displayOctaveNumber = 0;
  }

  fCurrentDisplayOctave =
    msrOctaveKindFromNumber (
      inputLineNumber,
      displayOctaveNumber);
}

void mxsr2msrTranslator::visitEnd (S_unpitched& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_unpitched" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNoteIsUnpitched = true;

 // fCurrentNoteDiatonicPitch = // JMI
   // fCurrentHarmonyRootDiatonicPitch;
}

//______________________________________________________________________________
S_msrChord mxsr2msrTranslator::createChordFromItsFirstNote (
  int               inputLineNumber,
  const S_msrVoice& voice,
  const S_msrNote&  chordFirstNote,
  msrNoteKind       noteKind)
{
  int firstNoteInputLineNumber =
    chordFirstNote->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "--> creating a chord from its first note " <<
      chordFirstNote->asShortString () <<
      ", in voice \"" << voice->getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // chordFirstNote has been registered as regular note in the part element sequence,
  // but it is actually the first note of a chord

  // create a chord
  S_msrChord
    chord =
      msrChord::create (
        firstNoteInputLineNumber,
        chordFirstNote->getSoundingWholeNotes (),
        chordFirstNote->getNoteDisplayWholeNotes (),
        chordFirstNote->getNoteGraphicNotesDurationKind ());

  // register note as first member of chord
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Adding first note " <<
      chordFirstNote->
        asShortString () <<
      ", line " << inputLineNumber <<
      ", to new chord" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
    chordFirstNoteShortcutUpLinkToMeasure =
      chordFirstNote->
        getNoteUpLinkToMeasure ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsDetails ()) {
    gLog << // JMI
      std::endl << std::endl <<
      "++++++++++++++++ chordFirstNote =" <<
      std::endl << std::endl <<
      chordFirstNote <<
      std::endl <<
      "+++++++++++++++++" <<
      std::endl << std::endl <<
      "++++++++++++++++ chordFirstNote->getNoteUpLinkToMeasure () =";

    if (chordFirstNoteShortcutUpLinkToMeasure) {
      gLog <<
        std::endl << std::endl;
    }
    else {
      gLog <<
        "none";
    }
  }
#endif // MF_TRACE_IS_ENABLED

/* forget about this, too early??? JMI
  // grace notes cannot be cross staff
  if (! chordFirstNote->getNoteIsAGraceNote ()) { // JMI
    // register the chord as non cross staff
    fCurrentChordStaffNumber =
      chordFirstNoteShortcutUpLinkToMeasure->
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Registering chord " <<
      chord->asString () <<
      " as current chord in voice \"" <<
      voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

 // fVoicesCurrentChordMap [voice] = chord;
  fVoicesCurrentChordMap [
    std::make_pair (
      voice->
        getVoiceUpLinkToStaff ()->
          getStaffNumber (),
      voice->
        getVoiceNumber ())] =
      chord;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif // MF_TRACE_IS_ENABLED
}
*/

//______________________________________________________________________________
/* JMI
void mxsr2msrTranslator::printVoicesCurrentChordMap ()
{
  gLog <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl <<
    "fVoicesCurrentChordMap contains " <<
    mfSingularOrPlural (
      fVoicesCurrentChordMap.size (), "element", "elements") <<
    ":";

  if (fVoicesCurrentChordMap.size ()) {
    gLog << std::endl;

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
      gLog <<
//        voice->getVoiceName () <<
        "staff " << (*i).first.first <<
        ", voice " <<  (*i).first.second <<
        ":" <<
        std::endl;

// * JMI
      S_msrChord chord = (*i).second;

      gLog <<
        chord;
// * /

      --gIndenter;

      if (++i == iEnd) break;

      gLog << std::endl;
    } // for

    --gIndenter;
  }

  gLog <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    std::endl;
}
*/

//______________________________________________________________________________
void mxsr2msrTranslator::finalizeCurrentChord (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Finalizing current chord START:" <<
      std::endl <<
      fCurrentChord <<
      std::endl <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentChord->
    finalizeChord (
      inputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Finalizing current chord END: " <<
      std::endl <<
      fCurrentChord <<
      std::endl <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about the current chord
  fCurrentChord = nullptr;
}

//______________________________________________________________________________
void mxsr2msrTranslator::printCurrentChord ()
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
void mxsr2msrTranslator::copyNoteArticulationsToChord (
  const S_msrNote&  note,
  const S_msrChord& chord)
{
  // copy note's articulations if any from the first note to chord

  std::list<S_msrArticulation>
    noteArticulations =
      note->
        getNoteArticulations ();

  for (const S_msrArticulation& articulation : noteArticulations) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceArticulations ()) {
      std::stringstream ss;

      ss <<
        "Copying articulation " <<
        articulation->getArticulationKind () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Copying technical " <<
        technicalWithFloat->getTechnicalWithFloatKind () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Copying technical " <<
        technicalWithString->getTechnicalWithStringKind () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOrnaments ()) {
      std::stringstream ss;

      ss <<
        "Copying ornament " <<
        msrOrnamentKindAsString (ornament->getOrnamentKind ()) <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSpanners ()) {
      std::stringstream ss;

      ss <<
        "Copying spanner " <<
        spanner->getSpannerKind () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTremolos ()) {
      std::stringstream ss;

      ss <<
        "Copying singleTremolo " <<
        noteSingleTremolo->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDynamics ()) {
      std::stringstream ss;

      ss <<
        "Copying dynamics " <<
        msrDynamicKindAsString ((*i)->getDynamicKind ()) <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDynamics ()) {
      std::stringstream ss;

      ss <<
        "Copying other dynamics " <<
        (*i)->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceWords ()) {
      std::stringstream ss;

      ss <<
        "Copying words " <<
        (*i)->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceStems ()) {
      std::stringstream ss;

      ss <<
        "Copying stem " <<
        noteStem->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceBeams ()) {
      gLog <<
        "Copying beam " <<
        (*i)->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendBeamToChord ((*i));
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceBeams ()) {
      std::stringstream ss;

      ss <<
        "Adding beam link of " <<
        beam->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTies ()) {
      std::stringstream ss;

      ss <<
        "Appending tie " <<
        noteTie->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      appendTieToChord (noteTie);
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

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlurs ()) {
      gLog <<
        "Copying slur " <<
        (*i)->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlurs ()) {
      std::stringstream ss;

      ss <<
        "Adding slur link of " <<
        slur->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLigatures ()) {
      std::stringstream ss;

      ss <<
        "Copying ligature " <<
        ligature->getLigatureKind () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePedals ()) {
      std::stringstream ss;

      ss <<
        "Copying pedal " <<
        pedal->asString () <<
        " from note " << note->asString () <<
        " to chord" << chord->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlashes ()) {
      std::stringstream ss;

      ss <<
        "Copying slash " <<
        (*i)->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceWedges ()) {
      std::stringstream ss;

      ss <<
        "Copying wedges " <<
        msrWedgeKindAsString (wedge->getWedgeKind ()) <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSegnos ()) {
      std::stringstream ss;

      ss <<
        "Copying segno " <<
        (*i)->asShortString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDalSegnos ()) {
      std::stringstream ss;

      ss <<
        "Copying dal degno " <<
        (*i)->asShortString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceCodas ()) {
      std::stringstream ss;

      ss <<
        "Copying coda " <<
        (*i)->asShortString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOctaveShifts ()) {
      std::stringstream ss;

      ss <<
        "Copying octave shift " <<
        noteOctaveShift->asString () <<
        " from note " << note->asString () <<
        " to chord" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Copying grace notes group before " <<
        graceNotesGroupBefore->asShortString () <<
        " from note " << note->asString () <<
        " to chord " << chord->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    chord->
      setChordGraceNotesGroupBefore (graceNotesGroupBefore);
  }

  S_msrGraceNotesGroup
    graceNotesGroupAfter =
      note->
        getNoteGraceNotesGroupAfter ();

  if (graceNotesGroupAfter) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Copying grace notes group after " <<
        graceNotesGroupAfter->asShortString () <<
        " from note " << note->asString () <<
        " to chord " << chord->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Adding grace notes group link before " <<
        graceNotesGroupBefore->asShortString () <<
        " from note " << note->asString () <<
        " to chord " << chord->asString () <<
        "" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the grace notes group link
    const S_msrChordGraceNotesGroupLink&
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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Adding grace notes group link after " <<
        graceNotesGroupAfter->asShortString () <<
        " from note " << note->asString () <<
        " to chord " << chord->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the grace notes group link
    const S_msrChordGraceNotesGroupLink&
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

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          iss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
void mxsr2msrTranslator::createAndPushTupletUponItsFirstNote (
  const S_msrNote& firstNote)
{
  // firstNote is the first tuplet note,
  // and is currently at the end of the voice

  int firstNoteInputLineNumber =
    firstNote->getInputLineNumber ();

  // account for note duration
  msrWholeNotes
    memberNotesSoundingWholeNotes =
      firstNote->getSoundingWholeNotes ();

  msrWholeNotes
    memberNotesDisplayWholeNotes =
      firstNote->getNoteDisplayWholeNotes ();

  // create a tuplet
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Creating a tuplet of " <<
      fCurrentNoteActualNotes <<
      '/' <<
      fCurrentNoteNormalNotes <<
      " with first note " <<
      firstNote->
        asShortString () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // add note as first note of the stack top tuplet
  tuplet->
    appendNoteToTuplet (
      firstNote,
      currentVoice);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    // only after appendNoteToTuplet() has set the note's uplink to tuplet
    std::stringstream ss;

    ss <<
      "Adding first note " <<
      firstNote->
        asShortString () <<
      " to tuplet " <<
      tuplet->asString () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set tuplet's uplink to tuplet
  if (fTupletsStack.size ()) {
    tuplet->
      setTupletShortcutUpLinkToTuplet (
        fTupletsStack.front ());
  }

  // register current top-level tuplet and its first note if relevant
  if (fTupletsStack.size () == 0) {
    fCurrentTopLevelTuplet = tuplet;
    fCurrentTopLevelTupletFirstNote = firstNote;

    fCurrentTopLevelTupletRelativeOffset = msrWholeNotes (0, 1);

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      // only after appendNoteToTuplet() has set the note's uplink to tuplet // JMI HARMFUL
      std::stringstream ss;

      ss <<
        "==> fCurrentTopLevelTupletFirstNote: " <<
        fCurrentTopLevelTupletFirstNote->asString () <<
        std::endl <<
        "==> fCurrentTopLevelTuplet: " <<
        fCurrentTopLevelTuplet->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // register tuplet in this visitor's tuplet stack
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "++> pushing tuplet " <<
      tuplet->asString () <<
      " to tuplets stack" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletsStack.push_front (tuplet);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## createAndPushTupletUponItsFirstNote() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // append top-level tuplet to the part at once,
  // so that we know the measure position of a harmony if any
  // before it is append to the part's harmonies voice
  currentVoice->
    appendTupletToVoice (tuplet);

//
/* JMI v0.9.67
  // set note displayed divisions
  firstNote->
    applyTupletMemberDisplayFactor (
      fCurrentNoteActualNotes,
      fCurrentNoteNormalNotes);
  */

  // keep track of current tuplet in the current voice,
  // in case we learn later by <chord/> in the next note
  // that it is actually the first note of a chord ?? JMI XXL v0.9.77

  // register tuplet as last one found in this voice
  // for chords in tuplets handling
// JMI  fLastHandledTupletInVoiceMap [currentVoice] = tuplet;
  fLastHandledTupletInVoiceMap [
    std::make_pair (
      fCurrentMusicXMLStaffNumber,
      fCurrentMusicXMLVoiceNumber)
    ] =
    tuplet;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayLastHandledTupletInVoiceMap (
      "############## createAndPushTupletUponItsFirstNote() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrTranslator::finalizeTupletAndPopItFromTupletsStack (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "mxsr2msrTranslator::finalizeTupletAndPopItFromTupletsStack() 1, " <<
      "line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## Before finalizeTupletAndPopItFromTupletsStack() 2");
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // get tuplet from top of tuplet stack
  S_msrTuplet
    tuplet =
      fTupletsStack.front ();

/*  // set note displayed divisions JMI v0.9.66
  note->
    applyTupletMemberDisplayFactor (
      fCurrentNoteActualNotes,
      fCurrentNoteNormalNotes);
*/

/* JMI v0.9.66
  // add lastNote to the tuplet
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "==> adding last note " << lastNote->asString () <<
      " to tuplets stack top " <<
      fTupletsStack.front ()->asString () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  tuplet->appendNoteToTuplet (lastNote);
*/

  // pop from the tuplets stack
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Popping tuplet:" <<
      std::endl;
    ++gIndenter;
    ss <<
      tuplet;
    --gIndenter;
    ss <<
      " from tuplets stack" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletsStack.pop_front ();

  if (fTupletsStack.size ()) {
    // tuplet is a nested tuplet
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      gLog <<
        "=== adding nested tuplet " <<
      tuplet->asString () <<
        " to current stack top tuplet " <<
      fTupletsStack.front ()->asString () <<
      ", line " << inputLineNumber <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    fTupletsStack.front ()->
      appendTupletToTuplet (tuplet);
  }

  else {
    // tuplet is a top level tuplet
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      gLog <<
        "Appending top level tuplet " <<
      tuplet->asString () <<
      " to voice \"" <<
      currentVoice->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

//     don't append the tuplet to the current voice here,
//     that has been done on the tuplet's first note
//     currentVoice->
//       appendTupletToVoice (tuplet);


/* JMI
    // finalize the tuplet
    tuplet->
      finalizeTuplet (
        inputLineNumber);
*/

    // forget about the current top-level tuplet and its first note // JMI v0.9.67 HARMFUL
//     fCurrentTopLevelTupletFirstNote = nullptr;
//     fCurrentTopLevelTuplet = nullptr;

 // JMI v0.9.67   // the tuplet stop is not to be handled later
 //   fCurrentATupletStopIsPending = false;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## After  finalizeTupletAndPopItFromTupletsStack() 3");
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentArticulationsToNote (
  const S_msrNote& note)
{
  // attach the current articulations if any to the note
  if (fCurrentArticulations.size ()) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceArticulations ()) {
      std::stringstream ss;

      ss <<
        "Attaching current articulations to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          " to note " << note->asString () <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Attaching current technicals to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          " to note " << note->asString () <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Attaching current technical with integers to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          " to note " << note->asString () <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Attaching current technical with floats to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          " to note " << note->asString () <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTechnicals ()) {
      std::stringstream ss;

      ss <<
        "Attaching current technical with strings to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          " to note " << note->asString () <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOrnaments ()) {
      std::stringstream ss;

      ss <<
        "Attaching current ornaments to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          " to note " << note->asString () <<
          std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      note->
        appendOrnamentToNote (ornament);

      // forget about this ornament
      fCurrentOrnamentsList.pop_front();
    } // while
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentSpannersToNote (
  const S_msrNote&   note,
  const std::string& context)
{
  // attach the current spanners if any to the note
  if (fCurrentSpannersList.size ()) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSpanners ()) {
      std::stringstream ss;

      ss <<
          "Attaching current spanners to note " <<
          note->asString () <<
          ", context: " << context <<
          ", line " << note->getInputLineNumber () <<
          std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
            " to note " << note->asString () <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

        note->
          appendSpannerToNote (spanner);

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
//             note->asString () <<
//             ", delaying 'stop' until next note" <<
//             std::endl;
//
//           gWaeHandler->waeTraceWithMeasureInfo (
//             __FILE__, __LINE__,
//             ss.str (),
//             fCurrentMeasureNumber,
//             fMsrScore->getScoreNumberOfMeasures ());
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
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachCurrentSingleTremoloToNote (
  const S_msrNote& note)
{
  // attach the current singleTremolo if any to the note
  if (fCurrentSingleTremolo) {

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Attaching current singleTremolo to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    note->
      setNoteSingleTremolo (fCurrentSingleTremolo);

    // set single tremolo graphic duration kind
    fCurrentSingleTremolo->
      setSingleTremoloGraphicNotesDurationKind (
        note->
          getNoteGraphicNotesDurationKind ());

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceArticulations ()) {
      gLog <<
        "Attaching current articulations to chord " <<
        chord->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    std::list<S_msrArticulation>::const_iterator i;
    for (
      i=fCurrentArticulations.begin ();
      i!=fCurrentArticulations.end ();
      ++i
  ) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceArticulations ()) {
        gLog <<
          "Attaching articulation " <<  (*i) <<
          " to chord " << chord <<
          std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOrnaments ()) {
      gLog <<
        "Attaching current ornaments to chord " <<
        chord->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    std::list<S_msrOrnament>::const_iterator i;
    for (
      i=fCurrentOrnamentsList.begin ();
      i!=fCurrentOrnamentsList.end ();
      ++i
  ) {
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceOrnaments ()) {
        gLog <<
          "Attaching ornament " <<  (*i) << " to chord " <<
          chord <<
          std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTempos ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending tempos to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::attachPendingTemposToPart (
  const S_msrPart& part)
{
  // attach the pending tempos if any to the voice
  if (fPendingTemposList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTempos ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending tempos to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
}

void mxsr2msrTranslator::attachPendingBarLinesToVoice (
  const S_msrVoice& voice)
{
  // attach the pending barlines if any to the voice
  if (fPendingBarLinesList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceBarLines ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending barlines to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceBarLines ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending barlines to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
}

void mxsr2msrTranslator::attachPendingRehearsalMarksToVoice (
  const S_msrVoice& voice)
{
 // attach the pending rehearsals if any to the note
  if (fPendingRehearsalMarksList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRehearsalMarks ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending rehearsals to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceRehearsalMarks ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending rehearsals to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
}

void mxsr2msrTranslator::attachLineBreaksToVoice (
  const S_msrVoice& voice)
{
 // attach the pending line breaks if any to the note
  if (fPendingLineBreaksList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLineBreaks ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending line breaks to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    while (fPendingLineBreaksList.size ()) {
      S_msrLineBreak
        lineBreak =
          fPendingLineBreaksList.front ();

      // append it to the voice
      voice->
        appendLineBreakToVoice (lineBreak);

      // remove it from the list
      fPendingLineBreaksList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachLineBreaksToPart (
  const S_msrPart& part)
{
 // attach the pending line breaks if any to the note
  if (fPendingLineBreaksList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLineBreaks ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending line breaks to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
}

void mxsr2msrTranslator::attachPageBreaksToVoice (
  const S_msrVoice& voice)
{
 // attach the pending page breaks if any to the note
  if (fPendingPageBreaksList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePageBreaks ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending page breaks to voice \""  <<
        voice->getVoiceName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    while (fPendingPageBreaksList.size ()) {
      S_msrPageBreak
        pageBreak =
          fPendingPageBreaksList.front ();

      // append it to the voice
      voice->
        appendPageBreakToVoice (pageBreak);

      // remove it from the list
      fPendingPageBreaksList.pop_front ();
    } // while
  }
}

void mxsr2msrTranslator::attachPageBreaksToPart (
  const S_msrPart& part)
{
 // attach the pending page breaks if any to the note
  if (fPendingPageBreaksList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePageBreaks ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending page breaks to part \""  <<
        part->getPartName () <<
        "\"" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingSegnosToNote (
  const S_msrNote& note)
{
 // attach the pending segno if any to the note
  if (fPendingSegnosList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSegnos ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending segno to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDalSegnos ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending dal segno to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDalSegnos ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending dal segno to chord " <<
        chord->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingCodasToNote (
  const S_msrNote& note)
{
 // attach the pending coda if any to the note
  if (fPendingCodasList.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceCodas ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending codas to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceCrescDecrescs ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending crescDecresc to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceEyeGlasses ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending eyeglasses to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDamps ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending damps to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDampAlls ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending damp alls to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceOctaveShifts ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending octave shifts to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceScordaturas ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending scordaturas to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDynamics ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending dynamics to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceDynamics ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending other dynamics to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceWords ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending words to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceBeams ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending beams to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlurs ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending slurs to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrWarningWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          note->getInputLineNumber (),
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLigatures ()) {
      std::stringstream ss;

      ss <<
        "Attaching " <<
        mfSingularOrPlural (
          numberOfLigatures,
          "ligature", "ligatures") <<
        " to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
            numberOfLigatures <<
              ' ' <<
            mfSingularOrPluralWithoutNumber (
              numberOfLigatures,
              "ligature", "ligatures") <<
            " attached to rest " <<
            note->asShortString ();

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
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
            note->asShortString ();

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append ligatures to note only if they belong to a suitable voice,
      // i.e. above goes to voice 1 or 3, and below to voice 2 or 4

      std::list<S_msrLigature>::iterator
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
            ligature->asString () <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
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
//             note->
//               getNoteUpLinkToMeasure ();
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//         mfAssertWithMeasureInfo (
//           __FILE__, __LINE__,
//           noteMeasure != nullptr,
//           "attachPendingLigaturesToNote(): noteMeasure is null",
//           fCurrentMeasureNumber,//
//           fMsrScore->getScoreNumberOfMeasures ());
// #endif // MF_TRACE_IS_ENABLED
//
//         // fetch note's segment
//         S_msrSegment
//           noteSegment =
//             noteMeasure->
//               getMeasureUpLinkToSegment ();
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//         mfAssertWithMeasureInfo (
//           __FILE__, __LINE__,
//           noteSegment != nullptr,
//           "noteSegment is null",
//           fCurrentMeasureNumber,//
//           fMsrScore->getScoreNumberOfMeasures ());
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
//         mfAssertWithMeasureInfo (
//           __FILE__, __LINE__,
//           noteVoice != nullptr,
//           "noteVoice is null",
//           fCurrentMeasureNumber,//
//           fMsrScore->getScoreNumberOfMeasures ());
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
                note->asString () <<
//                 " in voice \"" <<
//                 noteVoice->getVoiceName () <<
//                 "\"" <<
                ", line " << ligature->getInputLineNumber () <<
                std::endl;
            }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePedals ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending pedals to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrWarningWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          note->getInputLineNumber (),
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlashes ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending slashes to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

        mxsr2msrWarningWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          note->getInputLineNumber (),
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceWedges ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending wedges to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

          if (numberOfWedges > 1) {
            ss <<
              "there are " << numberOfWedges << " wedges";
          }
          else {
            ss <<
              "there is 1 wedge";
          }
          ss <<
            " attached to a rest";

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            note->getInputLineNumber (),
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGlissandos ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending glissandos to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
                fCurrentMusicXMLStaffNumber,
                fCurrentMusicXMLVoiceNumber);

          // get the voice's stanzas map
          const std::map<std::string, S_msrStanza>&
            voiceStanzasMap =
              voice->
                getVoiceStanzasMap ();

#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceGlissandos ()) {
                gLog <<
                  "--> attachPendingGlissandosToNote()"
                  ", voiceStanzasMap.size (): " <<
                  voiceStanzasMap.size () <<
                  ", fCurrentNoteHasLyrics: " <<
                  fCurrentNoteHasLyrics <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

          if (voiceStanzasMap.size ()) {
            // there are lyrics in this voice
            /* JMI
            if (! fCurrentNoteHasLyrics) {
              // append a skip to lyrics the same duration as the note
#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceGlissandos ()) {
                gLog <<
                  "Attaching a skip syllable to note " <<
                  note->asString () <<
                  " that has a glissando stop and no lyrics " <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

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
                      fCurrentNoteSoundingWholeNotesFromNotesDuration,
                      stanza);

                // append syllable to current note's syllables list
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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSlides ()) {
      std::stringstream ss;

      ss <<
        "Attaching pending slides to note " <<
        note->asString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
                fCurrentMusicXMLStaffNumber,
                fCurrentMusicXMLVoiceNumber);

          // get the voice's stanzas map
          const std::map<std::string, S_msrStanza>&
            voiceStanzasMap =
              voice->
                getVoiceStanzasMap ();

#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceSlides ()) {
                gLog <<
                  "--> attachPendingSlidesToNote()"
                  ", voiceStanzasMap.size (): " <<
                  voiceStanzasMap.size () <<
                  ", fCurrentNoteHasLyrics: " <<
                  fCurrentNoteHasLyrics <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

          if (voiceStanzasMap.size ()) {
            // there are lyrics in this voice
            /* JMI
            if (! fCurrentNoteHasLyrics) {
              // append a skip to lyrics the same duration as the note
#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceSlides ()) {
                gLog <<
                  "Attaching a skip syllable to note " <<
                  note->asString () <<
                  " that has a slide stop and no lyrics " <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

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
                      fCurrentNoteSoundingWholeNotesFromNotesDuration,
                      stanza);

                // append syllable to current note's syllables list
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
//   gLog <<
//     "attachPendingVoiceLevelElementsToVoice()" <<
//     ", fPendingTemposList.size (): " << fPendingTemposList.size () <<
//     ", fPendingBarLinesList.size (): " << fPendingBarLinesList.size () <<
//     ", fPendingLineBreaksList.size (): " << fPendingLineBreaksList.size () <<
//     ", fPendingPageBreaksList.size (): " << fPendingPageBreaksList.size () <<
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
  gLog <<
    "attachPendingPartLevelElementsToPart()" <<
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
  switch (fCurrentNoteGraphicNotesDurationKind) {
    case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
      // use the same duration as the one from the duration
      // internally ??? JMI
      fCurrentNoteDisplayWholeNotesFromType =
        fCurrentNoteSoundingWholeNotesFromNotesDuration;
      break;

    default:
      // convert note graphic duration into whole notes
      fCurrentNoteDisplayWholeNotesFromType =
        msrNotesDurationKindAsWholeNotes (
          fCurrentNoteGraphicNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotesDetails ()) {
        gLog <<
          std::endl <<
          "(1):" <<
          std::endl <<
          "fCurrentNoteGraphicNotesDurationKind: " <<
          msrNotesDurationKindAsString (
            fCurrentNoteGraphicNotesDurationKind) <<
          std::endl <<
          "fCurrentNoteDisplayWholeNotesFromType: " <<
          fCurrentNoteDisplayWholeNotesFromType <<
          std::endl <<
          "fCurrentNoteDotsNumber: " <<
          fCurrentNoteDotsNumber <<
          std::endl << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotesDetails ()) {
            gLog <<
              std::endl <<
              "(2):" <<
              std::endl <<
              "fCurrentNoteDisplayWholeNotesFromType: " <<
              fCurrentNoteDisplayWholeNotesFromType <<
              std::endl <<
              "wholeNotesIncrement: " <<
              wholeNotesIncrement <<
              std::endl <<
              "dots: " <<
              dots <<
              std::endl << std::endl;
          }
#endif // MF_TRACE_IS_ENABLED
        } // while
      }

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotesDetails ()) {
        gLog <<
          std::endl <<
          "(3):" <<
          std::endl <<
          "fCurrentNoteGraphicNotesDurationKind: " <<
          msrNotesDurationKindAsString (
            fCurrentNoteGraphicNotesDurationKind) <<
          std::endl <<
          "fCurrentNoteDisplayWholeNotesFromType: " <<
          fCurrentNoteDisplayWholeNotesFromType <<
          std::endl <<
          "fCurrentNoteDotsNumber: " <<
          fCurrentNoteDotsNumber <<
          std::endl << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Gathered note informations:" <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 42;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "fCurrentNoteSoundingWholeNotesFromNotesDuration" << ": " <<
      fCurrentNoteSoundingWholeNotesFromNotesDuration <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteGraphicNotesDuration" << ": " <<
        msrNotesDurationKindAsString (
          fCurrentNoteGraphicNotesDurationKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteDotsNumber" << ": " <<
        fCurrentNoteDotsNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteDisplayWholeNotesFromType" << ": " <<
      fCurrentNoteDisplayWholeNotesFromType <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteIsARest" << ": " <<
      fCurrentNoteIsARest <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentDivisionsPerQuarterNote" << ": " <<
      fCurrentDivisionsPerQuarterNote <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNotePrintObjectKind" << ": " <<
      fCurrentNotePrintObjectKind <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteRGB" << ": " <<
      fCurrentNoteRGB <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentNoteAlpha" << ": " <<
      fCurrentNoteAlpha <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentMusicXMLStaffNumber =" << ": " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fPreviousNoteMusicXMLStaffNumber" << ": " <<
      fPreviousNoteMusicXMLStaffNumber <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentMusicXMLVoiceNumber" << ": " <<
      fCurrentMusicXMLVoiceNumber <<
      std::endl <<

      std::setw (fieldWidth) <<
      "inputLineNumber" << ": " <<
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
      if (fCurrentNoteGraphicNotesDurationKind == msrNotesDurationKind::kNotesDuration_UNKNOWN_) {
        std::stringstream ss;

        ss <<
          "double tremolo note lacks a <type/>"; // JMI a completer

        mxsr2msrErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
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
      // other note

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

  // create the (new) note
  S_msrNote
    newNote =
      msrNote::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//         fCurrentMeasureNumber, // v0.9.66

        msrNoteKind::kNote_UNKNOWN_,
          // will be set by 'setNoteKind()' when it becomes known later

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
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    std::stringstream ss;

    ss <<
      "Creating note:" <<
      std::endl;

    ++gIndenter;

    gLog <<
      newNote <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
  return newNote;
}

//______________________________________________________________________________
void mxsr2msrTranslator::populateNoteBeforeNoteItselfIsHandled (
  int              inputLineNumber,
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
    fCurrentNoteSoundingWholeNotesFromNotesDuration
      !=
    fCurrentNoteDisplayWholeNotesFromType
      &&
    ! fCurrentNoteHasATimeModification
  ) {
    switch (newNote->getNoteKind ()) {
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

          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }

        newNote->
          setSoundingWholeNotes (
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
  attachCurrentSpannersToNote (
    newNote,
    "populateNoteBeforeNoteItselfIsHandled()");

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
  int              inputLineNumber,
  const S_msrNote& newNote)
{
  // attach the regular pending elements (not dal segnos), if any, to newNote
  // only now because <lyric> follows <glissando> and <slide> in MusicXML JMI ???
  attachPendingNoteLevelElementsToNote (newNote);
}

//______________________________________________________________________________
void mxsr2msrTranslator::createAStaffChangeIfNecessary (
  int               inputLineNumber,
  const S_msrNote&  newNote,
  const S_msrVoice& voiceToInsertInto)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceStaffChanges ()) {
    std::stringstream ss;

    ss <<
      "createAStaffChangeIfNecessary()" <<
      ", newNote: " << newNote->asShortString () <<
      ", fPreviousNoteMusicXMLStaffNumber: " << fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber: " << fCurrentMusicXMLStaffNumber <<
      ", voiceToInsertInto: " << voiceToInsertInto->getVoiceName () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is there a staff change?
  fCurrentStaffChangeKind = msrStaffChangeKind::kStaffChange_UNKNOWN_;

  if (
    fCurrentMusicXMLStaffNumber != fPreviousNoteMusicXMLStaffNumber
      &&
    fPreviousNoteMusicXMLStaffNumber != msrStaff::K_STAFF_NUMBER_UNKNOWN_
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

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceStaffChanges ()) {
        std::stringstream ss;

        ss <<
          "*** There is staff change for chord member note " <<
          newNote->asShortString () <<
          ", voiceToInsertInto: " << voiceToInsertInto->getVoiceName () <<
          voiceToInsertInto->getVoiceName () <<
          "\"" <<
          " from staff " << fPreviousNoteMusicXMLStaffNumber <<
          " to staff " << fCurrentMusicXMLStaffNumber <<
          ", \"" << staffToChangeTo->getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentStaffChangeKind = msrStaffChangeKind::kStaffChangeChordMemberNote;

//       LilyPond doesn't support cross staff chords, JMI v0.9.67
//       so place newNote in its 'official' staff
    }

    else {
      // no, newNote is another note
      fCurrentStaffChangeKind = msrStaffChangeKind::kStaffChangeOtherNote;

      // a staff change is necessary
      // to remain in this staff and not use the note's one

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceStaffChanges ()) {
        std::stringstream ss;

        ss <<
          "*** There is staff change for note " <<
          newNote->asShortString () <<
          ", voiceToInsertInto: \"" <<
          voiceToInsertInto->getVoiceName () <<
          "\"" <<
          ", fPreviousNoteMusicXMLStaffNumber: " << fPreviousNoteMusicXMLStaffNumber <<
          ", fCurrentMusicXMLStaffNumber: " << fCurrentMusicXMLStaffNumber <<
//           ", \"" << staffToChangeTo->getStaffName () << "\"" <<
          ", line " << inputLineNumber <<
          std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      // create the voice staff change
      S_msrVoiceStaffChange
        voiceStaffChange =
          msrVoiceStaffChange::create (
            inputLineNumber,
            staffToChangeTo);

      // fetch the voice to insert into,
      // keeping populating the staff the previous note belongs to
      S_msrVoice
        voiceToInsertInto2 =
          fetchVoiceFromCurrentPart (
            inputLineNumber,
            fPreviousNoteMusicXMLStaffNumber,
            fPreviousNoteMusicXMLVoiceNumber);

      // append it to the current sequence voice
      // before the note itself is appended
      voiceToInsertInto2->
        appendVoiceStaffChangeToVoice (
          voiceStaffChange);
    }
  }
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleNoteItself (
  int              inputLineNumber,
  const S_msrNote& newNote)
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "--> fCurrentNoteBelongsToAChord: " <<
      fCurrentNoteBelongsToAChord <<
      "--> fOnGoingChord: " <<
      fOnGoingChord <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fCurrentNoteBelongsToAChord) {
    if (fOnGoingChord) {
      // newNote is the first note after the chord in the current voice

      // finalize the current chord
      if (false && ! fCurrentNoteIsAGraceNote) // JMI v0.9.67
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
  /* JMI
    gLog <<
      "--> STORING " <<
      newNote->asShortString () <<
      " as last note found in voice " <<
      voiceToInsertInto->getVoiceName () <<
      std::endl <<
      "-->  fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      "--> fCurrentMusicXMLVoiceNumber : " <<
      fCurrentMusicXMLVoiceNumber <<
      std::endl <<
      / * JMI
      "--> staff name : " <<
      staff->getStaffName () <<
      std::endl <<
      * /
      "--> voice name : " <<
      voiceToInsertInto->getVoiceName () <<
      std::endl;
      */
  }
#endif // MF_TRACE_IS_ENABLED

  fVoicesLastMetNoteMap [
    std::make_pair (
      fCurrentMusicXMLStaffNumber,
      fCurrentMusicXMLVoiceNumber)
    ] =
      newNote;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    printVoicesLastMetNoteMap (
      inputLineNumber,
      "handleNote()");
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrTranslator::attachPendingGraceNotesGroupToNoteIfRelevant (
  int inputLineNumber)
{
  // is there a pending grace notes group?
  if (fPendingGraceNotesGroup) {
#ifdef MF_TRACE_IS_ENABLED
    if (gGlobalMxsrOahGroup->getTraceBackup ()) {
      gLog <<
        "Attaching pending grace notes group to current non-grace note upon backup" <<
        std::endl;
    }

    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "fCurrentGraceNotes IS NOT NULL upon <backup/>" << // JMI
        ", line " << inputLineNumber <<
        std::endl;

      gLog <<
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
//           fCurrentMusicXMLStaffNumber,
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
//             fCurrentMusicXMLStaffNumber,
//             fCurrentMusicXMLVoiceNumber)
//           ];
//       */
//         voice->getVoiceLastAppendedNote ();
    }

#endif // MF_TRACE_IS_ENABLED

    if (false && fCurrentNonGraceNote) { // JMI
#ifdef MF_TRACE_IS_ENABLED
      gLog <<
        "+++++++++++++++++ fCurrentNonGraceNote:" <<
        std::endl <<
        fCurrentNonGraceNote <<
        "+++++++++++++++++" <<
        std::endl;
#endif // MF_TRACE_IS_ENABLED

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
      std::stringstream ss;

      ss <<
        "cannot find voice to insert after grace notes group into: " <<
        "chordFirstNote is null" <<
        ", fCurrentMusicXMLStaffNumber: " <<
        fCurrentMusicXMLStaffNumber <<
        std::endl <<
        ", fCurrentMusicXMLVoiceNumber: " <<
        fCurrentMusicXMLVoiceNumber;

      mxsr2msrInternalErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }

    // forget about this grace notes group
    fPendingGraceNotesGroup = nullptr;
  }
}

//______________________________________________________________________________
// void mxsr2msrTranslator::handleBackup (
//   int inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gGlobalMxsrOahGroup->getTraceBackup ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling pending backup" <<
//       ", fCurrentBackupDivisions: " <<
//       fCurrentBackupDivisions <<
//       ", fCurrentNonGraceNote: " <<
//       fCurrentNonGraceNote->asShortString () <<
//       std::endl;
//
//     gWaeHandler->waeTraceWithMeasureInfo (
//       __FILE__, __LINE__,
//       ss.str (),
//       fCurrentMeasureNumber,//
//       fMsrScore->getScoreNumberOfMeasures ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // fetch current voice
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceOctaveShifts ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> handleBackup()" <<
//       ", fCurrentMusicXMLStaffNumber: " <<
//       fCurrentMusicXMLStaffNumber <<
//       ", fCurrentMusicXMLVoiceNumber: " <<
//       fCurrentMusicXMLVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//
//     gWaeHandler->waeTraceWithMeasureInfo (
//       __FILE__, __LINE__,
//       ss.str (),
//       fCurrentMeasureNumber,//
//       fMsrScore->getScoreNumberOfMeasures ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   S_msrVoice
//     currentVoice =
//       fetchVoiceFromCurrentPart (
//         inputLineNumber,
//         fCurrentMusicXMLStaffNumber,
//         fCurrentMusicXMLVoiceNumber);
//
//   // are there pending note level elements?
//   attachPendingNoteLevelElementsToNote ( // JMI
//     fCurrentNonGraceNote);
//
//   // is there a pending grace notes group?
//   attachPendingGraceNotesGroupToNoteIfRelevant (
//     inputLineNumber);
//
//   // are there pending spanners?
//   attachCurrentSpannersToNote (
//     fCurrentNonGraceNote,
//     "handleBackup()");
//
//   // compute the backup step length
//   msrWholeNotes
//     backupStepLength =
//       msrWholeNotes (
//         fCurrentBackupDivisions,
//         fCurrentDivisionsPerQuarterNote * 4); // hence a whole note
//
//   // let fCurrentPart handle the backup
//   fCurrentPart->
//     handleBackupInPart (
//       inputLineNumber,
//       backupStepLength);
// }

//______________________________________________________________________________
// void mxsr2msrTranslator::handleForward (
//   int inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gGlobalMxsrOahGroup->getTraceBackup ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Handling pending forward" <<
//       ", fCurrentForwardDivisions: " <<
//       fCurrentForwardDivisions <<
//       ", fCurrentNonGraceNote: " <<
//       fCurrentNonGraceNote->asShortString () <<
//       std::endl;
//
//     gWaeHandler->waeTraceWithMeasureInfo (
//       __FILE__, __LINE__,
//       ss.str (),
//       fCurrentMeasureNumber,//
//       fMsrScore->getScoreNumberOfMeasures ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // fetch current voice
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceOctaveShifts ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> handleForward()" <<
//       ", fCurrentMusicXMLStaffNumber: " <<
//       fCurrentMusicXMLStaffNumber <<
//       ", fCurrentMusicXMLVoiceNumber: " <<
//       fCurrentMusicXMLVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//
//     gWaeHandler->waeTraceWithMeasureInfo (
//       __FILE__, __LINE__,
//       ss.str (),
//       fCurrentMeasureNumber,//
//       fMsrScore->getScoreNumberOfMeasures ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   S_msrVoice
//     currentVoice =
//       fetchVoiceFromCurrentPart (
//         inputLineNumber,
//         fCurrentMusicXMLStaffNumber,
//         fCurrentMusicXMLVoiceNumber);
//
//   // are there pending note level elements?
//   attachPendingNoteLevelElementsToNote ( // JMI
//     fCurrentNonGraceNote);
//
//   // is there a pending grace notes group?
//   attachPendingGraceNotesGroupToNoteIfRelevant (
//     inputLineNumber);
//
//   // are there pending spanners?
//   attachCurrentSpannersToNote (
//     fCurrentNonGraceNote,
//     "handleForward()");
//
//   // compute the backup step length
//   msrWholeNotes
//     backupStepLength =
//       msrWholeNotes (
//         fCurrentBackupDivisions,
//         fCurrentDivisionsPerQuarterNote * 4); // hence a whole note
//
//   // let fCurrentPart handle the backup
//   fCurrentPart->
//     handleBackupInPart (
//       inputLineNumber,
//       backupStepLength);
// }

//______________________________________________________________________________
void mxsr2msrTranslator::visitEnd (S_note& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_note" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssertWithMeasureInfo (
    __FILE__, __LINE__,
    currentNoteVoice != nullptr,
    "currentNoteVoice is null",
    fCurrentMeasureNumber,
    fMsrScore->getScoreNumberOfMeasures ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // the elements pending since before the note if any
  // can now be appended to the latter's voice
  // prior to the note itself
  attachPendingVoiceLevelElementsToVoice (
    currentNoteVoice);

  attachPendingPartLevelElementsToPart (
    fCurrentPart);

  // set current staff number to insert into if needed JMI ???
  if (fCurrentMusicXMLStaffNumber == msrStaff::K_STAFF_NUMBER_UNKNOWN_) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceStaffChanges ()) {
      std::stringstream ss;

      ss <<
        "Setting fCurrentMusicXMLStaffNumber to " <<
        fCurrentMusicXMLStaffNumber <<
        ", in voice \"" <<
        currentNoteVoice->getVoiceName () <<
        "\"" <<
        ", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

//     fCurrentMusicXMLStaffNumber = fCurrentMusicXMLStaffNumber;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotes ()
      ||
    gTraceOahGroup->getTraceStaffChanges ()
  ) {
    std::stringstream ss;

    ss <<
      "==> fetching voice to insert note into" <<
      ", fPreviousNoteMusicXMLStaffNumber: " <<
      fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      ", fCurrentMusicXMLVoiceNumber: " <<
      fCurrentMusicXMLVoiceNumber <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch voice to insert note into
  // keeping populating the staff the previous note belongs to
  S_msrVoice
    voiceToInsertNoteInto =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber, // fPreviousNoteMusicXMLStaffNumber, fCurrentMusicXMLStaffNumber
        fCurrentMusicXMLVoiceNumber);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssertWithMeasureInfo (
    __FILE__, __LINE__,
    voiceToInsertNoteInto != nullptr,
    "voiceToInsertNoteInto is null",
    fCurrentMeasureNumber,
    fMsrScore->getScoreNumberOfMeasures ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // create a staff change if necessary
  createAStaffChangeIfNecessary (
    inputLineNumber,
    newNote,
    voiceToInsertNoteInto);

// // JMI v0.9.67
//   // attach the pre-pending elements if any to newNote,
//   // before the note itself is handled, because that may cause
//   // tuplets or chords to be appended to the voice
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceStaffChanges ()) {
//     std::stringstream ss;
//
//     ss <<
//       "==> fetching voice to insert harmonies, figured bass elements and/or frames into" <<
//       ", fPreviousNoteMusicXMLStaffNumber: " <<
//       fPreviousNoteMusicXMLStaffNumber <<
//       ", fCurrentMusicXMLStaffNumber: " <<
//       fCurrentMusicXMLStaffNumber <<
//       ", fCurrentMusicXMLVoiceNumber: " <<
//       fCurrentMusicXMLVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//
//     gWaeHandler->waeTraceWithMeasureInfo (
//       __FILE__, __LINE__,
//       ss.str (),
//       fCurrentMeasureNumber,//
//       fMsrScore->getScoreNumberOfMeasures ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// /* JMI
//   // fetch the staff from current part
//   S_msrStaff
//     staff =
//       fetchStaffFromCurrentPart (
//         inputLineNumber,
//         fCurrentMusicXMLStaffNumber);
// */

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // populate newNote before the note itself is handled
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // CAUTION JMI v0.9.66
  // permuted the order of populateNote() and handleNoteItself()
  // to have newNote's harmonies list already populated if relevant
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
      "==> newNote: ";
    if (newNote) {
      ss << newNote->asString ();
    }
    else {
      ss << "[NULL]";
    }
    ss << std::endl;

    ss <<
      "==> fCurrentTopLevelTupletFirstNote: ";
    if (fCurrentTopLevelTupletFirstNote) {
      ss << fCurrentTopLevelTupletFirstNote->asString ();
    }
    else {
      ss << "[NULL]";
    }
    ss << std::endl;

    ss <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the pending harmonies elements if any
  if (fPendingHarmoniesList.size ()) {
//     if (fTupletsStack.size () == 0) {
      handlePendingHarmonies (
        newNote);
//     }
//     else {
//       // wait until the end of the top-level tuplet
//     }
  }

  // handle the pending figured basses if any
  if (fPendingFiguredBassesList.size ()) {
//     if (fTupletsStack.size () == 0) {
      handlePendingFiguredBasses (
        newNote);
//     }
//     else {
//       // wait until the end of the top-level tuplet
//     }
  }

  // update current top-level tuplet relative offset if relevant
  if (! fCurrentNoteIsAGraceNote) {
//     if (fPendingHarmoniesList.size ()) {
//     if (fTupletsStack.size ()) {
    if (fCurrentTopLevelTuplet) {
//     if (fCurrentTopLevelTupletFirstNote) {
      fCurrentTopLevelTupletRelativeOffset +=
        newNote->getSoundingWholeNotes ();
//         fCurrentNoteSoundingWholeNotes;

#ifdef MF_TRACE_IS_ENABLED
      if (
        gTraceOahGroup->getTraceTuplets ()
          ||
        gTraceOahGroup->getTraceHarmonies ()
      ) {
        std::stringstream ss;

        ss <<
          "==> fCurrentTopLevelTupletRelativeOffset becomes: " <<
          fCurrentTopLevelTupletRelativeOffset.asString () <<
          ", newNote" << newNote->asString () <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());

        displayPendingHarmoniesList ("mxsr2msrTranslator::visitEnd (S_note& elt)");
      }
#endif // MF_TRACE_IS_ENABLED
    }
  }

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // handle the lyrics for the note after the latter itself is handled
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  // lyrics if any have to be handled in all cases
  // done only now because attachPendingNoteLevelElementsToNote() // JMI v0.9.67 HARMFUL
  // may append skip syllables to the notes
  handleLyricsForNoteAfterNoteItselfIsHandled (
    voiceToInsertNoteInto,
    newNote);

  fOnGoingNote = false;
}

//______________________________________________________________________________
void mxsr2msrTranslator::handlePendingHarmonies (
  const S_msrNote& newNote)
{
  switch (fPendingHarmoniesList.size ()) {
    case 0:
      break;

    case 1:
      handlePendingSingleHarmony (
        newNote,
        fPendingHarmoniesList.front ());
      break;

    default:
      handlePendingMultipleHarmonies (newNote);
      break;
  } //switch

  // forget about newNote's harmonies if any
  fPendingHarmoniesList.clear ();

  // reset voice harmonies counter
  fHarmoniesVoicesCounter = 0;
}

void mxsr2msrTranslator::handlePendingSingleHarmony (
  const S_msrNote&    newNote,
  const S_msrHarmony& harmony)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();

  msrWholeNotes
    newNoteSoundingWholeNotes =
      newNote->
        getSoundingWholeNotes (),
    newNoteDisplayWholeNotes =
      newNote->
        getNoteDisplayWholeNotes ();

  msrWholeNotes
    newNoteMeasurePosition =
      newNote->
        getMeasurePosition ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Handling the single pending harmony " <<
      harmony->asString () <<
      " for note " <<
      newNote->asShortString () <<
      ", newNoteSoundingWholeNotes: " << newNoteSoundingWholeNotes.asString () <<
      ", newNoteDisplayWholeNotes: " << newNoteDisplayWholeNotes.asString () <<
      ", newNoteMeasurePosition: " << newNoteMeasurePosition.asString () <<
      ", line " << inputLineNumber <<
      std::endl;

    ss <<
      "fCurrentTopLevelTupletFirstNote: ";
    if (fCurrentTopLevelTupletFirstNote) {
      ss <<
        fCurrentTopLevelTupletFirstNote->asString ();
    }
    else {
      ss << "[NULL]";
    }
    ss << std::endl;

    ss <<
      "fCurrentTopLevelTuplet: ";
    if (fCurrentTopLevelTuplet) {
      ss <<
        fCurrentTopLevelTuplet->asString ();
    }
    else {
      ss << "[NULL]";
    }
    ss << std::endl;

    ss <<
      ", fCurrentTopLevelTupletRelativeOffset: " <<
      fCurrentTopLevelTupletRelativeOffset.asString () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());

    displayPendingHarmoniesList ("handlePendingSingleHarmony() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // set harmony's sounding whole notes
  harmony->
    setHarmonySoundingWholeNotes (
      newNote->getSoundingWholeNotes (),
      "handlePendingSingleHarmony() 2");

  // set harmony's display whole notes
  harmony->
    setHarmonyDisplayWholeNotes (
      newNote->getNoteDisplayWholeNotes (),
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
  if (fCurrentTopLevelTuplet) {
#ifdef MF_TRACE_IS_ENABLED
    msrWholeNotes
      currentTopLevelTupletMeasurePosition =
        fCurrentTopLevelTuplet->getMeasurePosition ();

    // compute harmony's measure position
    msrWholeNotes
      harmonyMeasurePosition =
        currentTopLevelTupletMeasurePosition
          +
        fCurrentTopLevelTupletRelativeOffset;

    if (gTraceOahGroup->getTraceHarmonies ()) {
      std::stringstream ss;

      ss <<
        "Appending the single pending harmony " <<
        harmony->asString () <<
        ", fCurrentTopLevelTupletRelativeOffset: " <<
        fCurrentTopLevelTupletRelativeOffset.asString () <<
        ", harmonyMeasurePosition: " <<
        harmonyMeasurePosition <<
        ", for note " <<
        newNote->asShortString () <<
        " to part " <<
        fCurrentPart->getPartCombinedName () <<
        ", line " << inputLineNumber <<
        std::endl;

      ss <<
        "fCurrentTopLevelTupletFirstNote: ";
      if (fCurrentTopLevelTupletFirstNote) {
        ss <<
          fCurrentTopLevelTupletFirstNote->asString ();
      }
      else {
        ss << "[NULL]";
      }
      ss << std::endl;

      ss <<
        "fCurrentTopLevelTuplet: ";
      if (fCurrentTopLevelTuplet) {
        ss <<
          fCurrentTopLevelTuplet->asString ();
      }
      else {
        ss << "[NULL]";
      }
      ss << std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());

  //     displayPendingHarmoniesList ("handlePendingSingleHarmony() 4");
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentPart->
      appendHarmonyToPart (
        fCurrentTopLevelTuplet->getInputLineNumber (),
        harmony,
        harmonyMeasurePosition);
  }

  else {
    // harmony is at the voice level JMI v0.9.67
    fCurrentPart->
      appendHarmonyToPart (
        newNote->getInputLineNumber (),
        harmony,
        newNote->getMeasurePosition ());
  }
}

void mxsr2msrTranslator::handlePendingMultipleHarmonies (
  const S_msrNote& newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();

  msrWholeNotes
    newNoteSoundingWholeNotes =
      newNote->
        getSoundingWholeNotes (),
    newNoteDisplayWholeNotes =
      newNote->
        getNoteDisplayWholeNotes ();

  msrWholeNotes
    newNoteMeasurePosition =
      newNote->
        getMeasurePosition ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Handling the " <<
      fPendingHarmoniesList.size () <<
      " pending harmonies for note " <<
      newNote->asShortString () <<
      ", newNoteSoundingWholeNotes: " << newNoteSoundingWholeNotes.asString () <<
      ", newNoteDisplayWholeNotes: " << newNoteDisplayWholeNotes.asString () <<
      ", newNoteMeasurePosition: " << newNoteMeasurePosition.asString () <<
      ", line " << inputLineNumber <<
      std::endl;

    ss <<
      "fCurrentTopLevelTuplet: ";
    if (fCurrentTopLevelTuplet) {
      ss <<
        fCurrentTopLevelTuplet->asString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      ", fCurrentTopLevelTupletRelativeOffset: " <<
      fCurrentTopLevelTupletRelativeOffset.asString () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());

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
      newNote->asShortString () <<
      " in offset relative order " <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
        ", line " <<
        inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
      // as a fraction of newNoteSoundingWholeNotes
      mfRational
        fraction (
          offsetDelta / newNoteSoundingWholeNotes);

      // set the previous harmony's sounding whole notes
      msrWholeNotes
        currentHarmonySoundingWholeNotes =
          newNoteSoundingWholeNotes * fraction;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceHarmonies ()) {
        std::stringstream ss;

        ss <<
          "--> handlePendingHarmony, " <<
          ", currentHarmonySoundingWholeNotes: " << currentHarmonySoundingWholeNotes.asString () <<
          ", currentHarmonyWholeNotesOffset: " << currentHarmonyWholeNotesOffset <<
          ", previousWholeNotesOffsetInTheLoop: " << previousWholeNotesOffsetInTheLoop <<
          ", offsetDelta: " << offsetDelta <<
          ", newNoteSoundingWholeNotes: " << newNoteSoundingWholeNotes.asString () <<
          ", fraction: " << fraction <<
          ", line " <<
          inputLineNumber <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

      previousHarmony->
        setHarmonySoundingWholeNotes (
          currentHarmonySoundingWholeNotes,
          "mxsr2msrTranslator::handlePendingMultipleHarmonies() 3, first to next to last harmony");

      // remember the currentHarmony's whole notes offset as previous
      previousWholeNotesOffsetInTheLoop = currentHarmonyWholeNotesOffset;
    }

    // set the currentHarmony's display whole notes JMI useless??? v0.9.66
    currentHarmony->
      setHarmonyDisplayWholeNotes (
        newNoteDisplayWholeNotes,
        "mxsr2msrTranslator::handlePendingMultipleHarmonies() 4, first to next to last harmony");

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
      ", line " <<
      inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      newNoteSoundingWholeNotes - previousWholeNotesOffsetInTheLoop;

  // compute the lastHarmony's sounding whole notes
  // as a fraction of newNoteSoundingWholeNotes
  mfRational
    fraction (
      offsetDelta / newNoteSoundingWholeNotes);

  // set the sounding whole notes of the last harmony in the list
  msrWholeNotes
    lastHarmonySoundingWholeNotes =
      newNoteSoundingWholeNotes * fraction;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "--> handlePendingHarmony, LAST HARMONY OF THE LIST" <<
      ", lastHarmonySoundingWholeNotes: " << lastHarmonySoundingWholeNotes.asString () <<
      ", lastHarmonyWholeNotesOffset: " << lastHarmonyWholeNotesOffset <<
      ", previousWholeNotesOffsetInTheLoop: " << previousWholeNotesOffsetInTheLoop <<
      ", newNoteSoundingWholeNotes: " << newNoteSoundingWholeNotes.asString () <<
      ", offsetDelta: " << offsetDelta <<
      ", fraction: " << fraction <<
      ", line " <<
      inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  lastHarmony->
    setHarmonySoundingWholeNotes (
      lastHarmonySoundingWholeNotes,
      "mxsr2msrTranslator::handlePendingMultipleHarmonies() 5, last harmony in the list");

  // set the display whole notes of the last harmony in the list
  lastHarmony->
    setHarmonyDisplayWholeNotes (
      newNoteDisplayWholeNotes,
      "mxsr2msrTranslator::handlePendingMultipleHarmonies() 6, last harmony in the list");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    displayPendingHarmoniesList ("mxsr2msrTranslator::handlePendingMultipleHarmonies() 7");
  }
#endif // MF_TRACE_IS_ENABLED

  // append the figured basses list to current part
  if (fCurrentTopLevelTuplet) {
    fCurrentPart->
      appendHarmoniesListToPart (
        fCurrentTopLevelTuplet->getInputLineNumber (),
        fPendingHarmoniesList,
        fCurrentTopLevelTuplet->getMeasurePosition ());
  }
  else {
    fCurrentPart->
      appendHarmoniesListToPart (
        newNote->getInputLineNumber (),
        fPendingHarmoniesList,
        newNote->getMeasurePosition ());
  }
}

void mxsr2msrTranslator:: displayPendingHarmoniesList (
  std::string context)
{
  gLog <<
    std::endl <<
    ">>> +++++++++++++++++++++++++++++++++++++++++++++++++++++ >>>" <<
    std::endl <<
    "Pending harmonies list -- " <<
    context <<
    ", ==> fCurrentTopLevelTupletRelativeOffset: " <<
    fCurrentTopLevelTupletRelativeOffset.asString () <<
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
void mxsr2msrTranslator::handlePendingFiguredBasses (
  const S_msrNote& newNote)
{
  switch (fPendingFiguredBassesList.size ()) {
    case 0:
      break;

    case 1:
      handlePendingSingleFiguredBass (
        newNote,
        fPendingFiguredBassesList.front ());
      break;

    default:
      handlePendingMultipleFiguredBasses (newNote);
      break;
  } //switch

  // forget about newNote's figured basses if any
  fPendingFiguredBassesList.clear ();

  // reset figured basses counter
  fFiguredBassVoicesCounter = 0;
}

void mxsr2msrTranslator::handlePendingSingleFiguredBass (
  const S_msrNote&        newNote,
  const S_msrFiguredBass& figuredBass)
{
  // set figuredBass's sounding whole notes
  figuredBass->
    setFiguredBassSoundingWholeNotes (
      newNote->getSoundingWholeNotes (),
      "handlePendingSingleFiguredBass ()");

  // set figuredBass's display whole notes
  figuredBass->
    setFiguredBassDisplayWholeNotes ( // JMI useless??? v0.9.66
      newNote->getNoteDisplayWholeNotes ());

  // set figuredBass's tuplet factor
  figuredBass->
    setFiguredBassTupletFactor (
      msrTupletFactor (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes));

//   // handle the pending figured bass elements if any
//   if (newNote == fCurrentTopLevelTupletFirstNote) {
//     if (fPendingFiguredBassesList.size ()) {
//       handlePendingFiguredBasses (
//         newNote);
//     }
//   }

  // append figuredBass to fCurrentPart
  fCurrentPart->
    appendFiguredBassToPart (
      newNote->getInputLineNumber (),
      figuredBass,
      newNote->getMeasurePosition ());
}

void mxsr2msrTranslator::handlePendingMultipleFiguredBasses (
  const S_msrNote& newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();

  msrWholeNotes
    newNoteSoundingWholeNotes =
      newNote->
        getSoundingWholeNotes (),
    newNoteDisplayWholeNotes =
      newNote->
        getNoteDisplayWholeNotes ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Handling the " <<
      fPendingHarmoniesList.size () <<
      " pending figured basses for note " <<
      newNote->asShortString () <<
      ", newNoteSoundingWholeNotes: " << newNoteSoundingWholeNotes.asString () <<
      ", newNoteDisplayWholeNotes: " << newNoteDisplayWholeNotes.asString () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());

    displayPendingFiguredBassesList ("handlePendingMultipleFiguredBasses() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrFiguredBass currentFiguredBass : fPendingFiguredBassesList) {
    // get currentFiguredBass's whole notes duration
    msrWholeNotes
      figuredBassWholeNotesDuration =
        currentFiguredBass->
//           getFiguredBassWholeNotesDuration ();
          getSoundingWholeNotes ();

    // compute the currentFiguredBass's sounding whole notes
    // as a fraction of newNoteSoundingWholeNotes
    msrWholeNotes
      figuredBassSoundingWholeNotes;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceFiguredBasses ()) {
      std::stringstream ss;

      ss <<
        "handlePendingFiguredBass, " <<
        ", figuredBassSoundingWholeNotes: " << figuredBassSoundingWholeNotes.asString () <<
        ", figuredBassWholeNotesDuration: " << figuredBassWholeNotesDuration <<
        ", line " <<
        inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    // set currentFiguredBass's sounding whole notes
    currentFiguredBass->
      setFiguredBassSoundingWholeNotes (
        figuredBassSoundingWholeNotes,
        "mxsr2msrTranslator::handlePendingMultipleFiguredBasses() 3");

    // set the currentFiguredBass's display whole notes
    currentFiguredBass->
      setFiguredBassDisplayWholeNotes (
        newNoteDisplayWholeNotes);

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
      newNote->getInputLineNumber (),
      fPendingFiguredBassesList,
      newNote->getMeasurePosition ());

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    displayPendingHarmoniesList ("handlePendingMultipleFiguredBasses() 3");
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator:: displayPendingFiguredBassesList (
  std::string context)
{
  gLog <<
    "Pending figured basses list -- " <<
    context <<
    ':' <<
    std::endl;

  ++gIndenter;

  for (S_msrFiguredBass figuredBass : fPendingFiguredBassesList) {
    gLog << figuredBass->asString () << std::endl;
  } // for

  --gIndenter;
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) { // JMI
      std::stringstream ss;

      ss <<
        "handleNonChordNorTupletNoteOrRest(), fCurrentNonGraceNote: " <<
        fCurrentNonGraceNote->asShortString () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) { // JMI
    gLog <<
      "handleNonChordNorTupletNoteOrRest(), newNote: " <<
      std::endl;
    ++gIndenter;
    gLog <<
      newNote <<
      std::endl;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Handling non-chord, non-tuplet note or rest " <<
       newNote->asShortString () << // NO, would lead to infinite recursion ??? JMI
      ", currentVoice: \"" <<
      currentVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      ":" <<
      std::endl;

    ++gIndenter;

    gLog <<
      newNote->asString () <<
      std::endl;

    const int fieldWidth = 25;

    gLog << std::left <<
      std::setw (fieldWidth) << "voice" << ": \"" <<
      currentVoice->getVoiceName () << "\"" <<
      std::endl <<
      std::setw (fieldWidth) << "line:" << ": " <<
      inputLineNumber <<
      std::endl <<
      std::setw (fieldWidth) << "fCurrentNoteIsAGraceNote" << ": " <<
      fCurrentNoteIsAGraceNote <<
      std::endl;

    if (gTraceOahGroup->getTraceNotesDetails ()) {
      std::stringstream ss;

      ss <<
        std::setw (fieldWidth) << "fPendingGraceNotesGroup" << ": " <<
        std::endl <<
        "======================= handleNonChordNorTupletNoteOrRest()" <<
        ", line " << inputLineNumber <<
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
 //         "fCurrentGraceNotes is NULL"; // JMI
      }

      gLog << std::endl;
    }

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceGraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Creating grace notes for note " <<
          newNote->asString () <<
          " in voice \"" <<
          currentVoice->getVoiceName () << "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Appending note " <<
        newNote->asString () <<
        " to grace notes group in voice \"" <<
        currentVoice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    // append newNote to the pending grace notes group
    fPendingGraceNotesGroup->
      appendNoteToGraceNotesGroup (newNote);
  }

  else {
    if (
      fCurrentDoubleTremoloTypeKind != msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_
    ) {
      // newNote belongs to a tremolo

      switch (fCurrentDoubleTremoloTypeKind) {
        case msrDoubleTremoloTypeKind::kDoubleTremoloType_UNKNOWN_:
          // just to avoid a compiler message
          break;

        case msrDoubleTremoloTypeKind::kDoubleTremoloTypeSingle:
          // append newNote to the current voice
  #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Appending single tremolo " <<
              newNote->asString () <<
              ", line " << newNote->getInputLineNumber () <<
              ", to voice \"" <<
              currentVoice->getVoiceName () <<
              "\"" <<
              std::endl;

            gWaeHandler->waeTraceWithMeasureInfo (
              __FILE__, __LINE__,
              ss.str (),
              fCurrentMeasureNumber,
              fMsrScore->getScoreNumberOfMeasures ());
          }
  #endif // MF_TRACE_IS_ENABLED

          currentVoice->
            appendNoteToVoice (newNote);

          // fCurrentSingleTremolo is handled in
          // attachCurrentSingleTremoloToNote()
          break;

        case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStart:
          // register newNote as first element of the current double tremolo
  #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Setting regular note " <<
              newNote->asString () <<
              ", line " << newNote->getInputLineNumber () <<
              ", as double tremolo first element" <<
              " in voice \"" <<
              currentVoice->getVoiceName () <<
              "\"" <<
              std::endl;

            gWaeHandler->waeTraceWithMeasureInfo (
              __FILE__, __LINE__,
              ss.str (),
              fCurrentMeasureNumber,
              fMsrScore->getScoreNumberOfMeasures ());
          }
  #endif // MF_TRACE_IS_ENABLED

          fCurrentDoubleTremolo->
            setDoubleTremoloNoteFirstElement (
              newNote);
          break;

        case msrDoubleTremoloTypeKind::kDoubleTremoloTypeStop:
          // register newNote as second element of the current double tremolo
  #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Setting regular note " <<
              newNote->asString () <<
              ", line " << newNote->getInputLineNumber () <<
              ", as double tremolo second element" <<
              " in voice \"" <<
              currentVoice->getVoiceName () <<
              "\"" <<
              std::endl;

            gWaeHandler->waeTraceWithMeasureInfo (
              __FILE__, __LINE__,
              ss.str (),
              fCurrentMeasureNumber,
              fMsrScore->getScoreNumberOfMeasures ());
          }
  #endif // MF_TRACE_IS_ENABLED

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
  #ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Appending regular note or rest " <<
          newNote->asString () <<
          ", line " << newNote->getInputLineNumber () <<
          ", to voice \"" <<
          currentVoice->getVoiceName () <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithMeasureInfo (
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
  #endif // MF_TRACE_IS_ENABLED

      ++gIndenter;

      currentVoice->
        appendNoteToVoice (newNote);

      if (false) { // XXL, syllable sans fSyllableNote assigne JMI
        gLog <<
          "&&&&&&&&&&&&&&&&&& currentVoice (" <<
          currentVoice->getVoiceName () <<
          ") contents &&&&&&&&&&&&&&&&&&" <<
          std::endl <<
          currentVoice <<
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
void mxsr2msrTranslator::handleLyricsForNoteAfterNoteItselfIsHandled (
  const S_msrVoice& currentVoice,
  const S_msrNote&  newNote)
{
  int inputLineNumber =
    newNote->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Handling lyrics for note " <<
      newNote->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 33;

    gLog <<
      std::setw (fieldWidth) <<
      "currentVoice" << " = \"" << currentVoice->getVoiceName () <<"\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "newNote" << " = \"" << newNote->asShortString () << "\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fLastHandledNoteInVoiceHasLyrics" << ": " <<
      fLastHandledNoteInVoiceHasLyrics <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentSyllableExtendKind" << "" << ": " <<
      msrSyllableExtendKindAsString (
        fCurrentSyllableExtendKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fOnGoingSyllableExtend" << ": " <<
      fOnGoingSyllableExtend <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentNoteHasLyrics" << ": " <<
      fCurrentNoteHasLyrics <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fASkipSyllableHasBeenGeneratedForcurrentNote" << ": " <<
      fASkipSyllableHasBeenGeneratedForcurrentNote <<
      std::endl <<

      std::setw (fieldWidth) <<
      "fCurrentStanzaNumber" << ": " << fCurrentStanzaNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentStanzaName" << ": " << fCurrentStanzaName << "\"" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentLyricTextsList" << " = ";

    msrSyllable::writeTextsList (
      fCurrentLyricTextsList,
      gLog);

    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteHasLyrics) {
    // newNote has lyrics attached to it
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceLyricsDetails ()) {
      std::stringstream ss;

      ss <<
        "Note " <<
        newNote->asShortString () <<
        " has lyrics attached to it" << " ***" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
        // get the current voice's stanzas map
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
                fCurrentNoteSoundingWholeNotesFromNotesDuration,
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
      mfAssertWithMeasureInfo (
        __FILE__, __LINE__,
        fOnGoingSyllableExtend,
        "fOnGoingSyllableExtend is false",
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Handling a note belonging to a chord" <<
      ", newChordNote:" <<
      std::endl;

    ++gIndenter;

    gLog << newChordNote <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "a rest cannot belong to a chord");
  }

  // fetch current voice
  int staffNumberToUse = -1001;
    // to avoid compiler warning for uninitialized variable

  switch (fCurrentStaffChangeKind) {
    case msrStaffChangeKind::kStaffChange_UNKNOWN_:
      staffNumberToUse =
        fCurrentMusicXMLStaffNumber; // JMI fCurrentMusicXMLStaffNumber;
      break;
    case msrStaffChangeKind::kStaffChangeChordMemberNote:
      if (fCurrentNoteIsCrossStaves) {
        staffNumberToUse =
          fCurrentMusicXMLStaffNumber;
    // JMI    staffNumberToUse = fCurrentMusicXMLStaffNumber; // keep it!
      }
      else {
   //   JMI   staffNumberToUse = fCurrentChordStaffNumber;
        staffNumberToUse =
          fCurrentMusicXMLStaffNumber;
      }
      break;
    case msrStaffChangeKind::kStaffChangeOtherNote:
      staffNumberToUse =
        fCurrentMusicXMLStaffNumber;
     // JMI staffNumberToUse = fCurrentMusicXMLStaffNumber;
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ", fCurrentChordStaffNumber: " <<
      fCurrentChordStaffNumber <<
      ", fPreviousNoteMusicXMLStaffNumber: " <<
      fPreviousNoteMusicXMLStaffNumber <<
      ", fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      ", staffNumberToUse: " <<
      staffNumberToUse <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        staffNumberToUse,
        fCurrentMusicXMLVoiceNumber);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssertWithMeasureInfo (
    __FILE__, __LINE__,
    currentVoice != nullptr,
    "currentVoice is null",
    fCurrentMeasureNumber,
    fMsrScore->getScoreNumberOfMeasures ());
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Handling a chord member note" <<
      ", currentVoice: \"" <<
      currentVoice->getVoiceName () <<
      "\", fOnGoingChord: " <<
      fOnGoingChord <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsDetails ()) {
    gLog <<
      std::endl <<
      "======================= handleNoteBelongingToAChord" <<
      ", line " << inputLineNumber <<
      std::endl;
    fCurrentPart->print (gLog);
    gLog <<
      "=======================" <<
      std::endl << std::endl;

/* JMI
    if (fCurrentGraceNotes) {
      gLog <<
        fCurrentGraceNotes;
    }
    else {
//       gLog <<
//         "fCurrentGraceNotes is NULL"; // JMI
    }
*/

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // should a chord be created?
  if (! fOnGoingChord) {
    // newChordNote is the second note of the chord to be created

    // fetch this chord's first note,
    // i.e the last handled note for this voice

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChords ()) {
      printVoicesLastMetNoteMap (
        inputLineNumber,
        "handleNoteBelongingToAChord()");
    }
#endif // MF_TRACE_IS_ENABLED

    S_msrNote
      chordFirstNote =
  //      fVoicesLastMetNoteMap [currentVoice];
        fVoicesLastMetNoteMap [
          std::make_pair (
            fCurrentMusicXMLStaffNumber,
            fCurrentMusicXMLVoiceNumber)
          ];
      /*
        currentVoice->
          getVoiceLastAppendedNote (); ??? JMI
          */

    if (! chordFirstNote) {
      std::stringstream ss;

      ss <<
        "handleNoteBelongingToAChord():" <<
        std::endl <<
        "chordFirstNote is null on " <<
        newChordNote->asString () <<
        std::endl <<
        "fCurrentMusicXMLStaffNumber: " << fCurrentMusicXMLStaffNumber <<
        std::endl <<
        "fCurrentMusicXMLVoiceNumber: " << fCurrentMusicXMLVoiceNumber;

      mxsr2msrInternalErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChords ()) {
      gLog <<
        "mxsr2msrTranslator::handleNoteBelongingToAChord()" <<
        ", chordFirstNote:" <<
        std::endl;

      ++gIndenter;

      gLog <<
        chordFirstNote <<
        std::endl;

      --gIndenter;
    }
#endif // MF_TRACE_IS_ENABLED

    // fetch chord first note's kind before createChordFromItsFirstNote(),
    // because the latter will change it to kChordMemberNote or msrNoteKind::kNoteInChordInGraceNotesGroup
    msrNoteKind
      savedChordFirstNoteKind =
        chordFirstNote->getNoteKind ();

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChords ()) {
      gLog <<
        "Handling a note belonging to a chord" <<
        ", savedChordFirstNoteKind: " <<
        msrNoteKindAsString (savedChordFirstNoteKind) <<
        std::endl;

      ++gIndenter;

      gLog <<
        chordFirstNote <<
        std::endl;

      --gIndenter;
    }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceChords ()) {
          std::stringstream ss;

          ss <<
            "Removing chord first note " <<
            chordFirstNote->asShortString () <<
            ", line " << inputLineNumber <<
            ", from voice \"" << currentVoice->getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceChordsDetails ()) {
          gLog <<
            std::endl << std::endl <<
            "&&&&&&&&&&&&&&&&&& fCurrentPart contents &&&&&&&&&&&&&&&&&&" <<
            std::endl <<
            fCurrentPart <<
            std::endl << std::endl;
        }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceStaffChanges ()) {
          std::stringstream ss;

          ss <<
            std::endl <<
            ", fPreviousNoteMusicXMLStaffNumber: " <<
            fPreviousNoteMusicXMLStaffNumber <<
            ", fCurrentMusicXMLStaffNumber: " <<
            fCurrentMusicXMLStaffNumber <<
            ", line " << inputLineNumber <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

        currentVoice->
          removeNoteFromVoice (
            inputLineNumber,
            chordFirstNote);

        // add chord to the voice instead
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceChords ()) {
          std::stringstream ss;

          ss <<
            "Appending chord " << fCurrentChord->asString () <<
            " to voice \"" <<
            currentVoice->getVoiceName () <<
            "\"" <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

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
              getSoundingWholeNotes ();
              */

          /* JMI
          // updating chord's divisions // JMI
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceChords ()) {
            gLog <<
              "Updating sounding divisions for double tremolo chord " <<
              " " << chord->asString () <<
              " to " << chordFirstNoteSoundingWholeNotes.asString () <<
              " in voice \"" <<
              currentVoice->getVoiceName () <<
              "\"" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          chord->
            setSoundingWholeNotes ( // ??? JMI
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
            std::stringstream ss;

            ss <<
              "chord first note " <<
              chordFirstNote->asShortString () <<
              " belongs to a double tremolo, but is not marked as such";

            mxsr2msrInternalErrorWithMeasureInfo (
              gServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              ss.str (),
              fCurrentMeasureNumber,
              fMsrScore->getScoreNumberOfMeasures ());
          }
        }
        break;

      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
      case msrNoteKind::kNoteInChordInGraceNotesGroup:
      /* JMI
        // remove last handled (previous current) note from the current voice
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceChords ()) {
          std::stringstream ss;

          ss <<
            "Removing chord first note " <<
            chordFirstNote->asShortString () <<
            ", line " << inputLineNumber <<
            ", from voice \"" << currentVoice->getVoiceName () << "\"" <<
            ", line " << inputLineNumber <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

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

      case msrNoteKind::kNote_UNKNOWN_:
        break;
    } // switch

    // account for chord being built
    fOnGoingChord = true;
  }

  else {
    // is newChordNote in the same chord but in another voice,
    // implying it is a member of a crossstaff chord?

/* JMI
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChords ()) {
      gLog <<
        "is newChordNote in the same chord but in another voice?" <<
        ", currentVoice: " <<
        currentVoice->getVoiceName () <<
        std::endl;

      printVoicesCurrentChordMap ();
    }
#endif // MF_TRACE_IS_ENABLED
*/

  // register newChordNote as another member of chord
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChords ()) {
      std::stringstream ss;

      ss <<
        "Adding another note " <<
        newChordNote->asString() <<
        ", line " << inputLineNumber <<
        " to current chord in voice " <<
        currentVoice->getVoiceName () <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // add newChordNote to current chord
  fCurrentChord->
    addAnotherNoteToChord (
      newChordNote, currentVoice);

  // copy newChordNote's elements if any to the current chord
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrTranslator::handlePendingTupletStopIfAny (
  int              inputLineNumber,
  const S_msrNote& note)
{
  if (fCurrentATupletStopIsPending) {
    // finalize the tuplet, only now
    // in case the last element is a chord
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      std::stringstream ss;

      ss <<
        "--> kTupletTypeStart: handling pending tuplet stop, note " <<
        note->asShortString () <<
        ", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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

  if (fCurrentNoteSoundingWholeNotesFromNotesDuration.getNumerator () == 0) {
    // no duration has been found,
    // determine sounding from display whole notes
    note->
      determineTupletMemberSoundingFromDisplayWholeNotes (
        fCurrentNoteActualNotes,
        fCurrentNoteNormalNotes);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Handling a note belonging to a tuplet" <<
      ", note: " <<
      note->
        asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is there an ongoing chord?
  if (! fOnGoingChord) {
    // note is the first one after a chord in a tuplet,
    // JMI
  }

  switch (fCurrentTupletTypeKind) {
    case msrTupletTypeKind::kTupletTypeStart:
      {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTuplets ()) {
          std::stringstream ss;

          ss <<
            "--> kTupletTypeStart: note: " <<
            note->asShortString () <<
            ", line " << inputLineNumber <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

        handlePendingTupletStopIfAny (
          inputLineNumber,
          note);
        /* JMI
        if (fCurrentATupletStopIsPending) {
          // finalize the tuplet, only now
          // in case the last element is a chord
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceTuplets ()) {
            gLog <<
              "--> kTupletTypeStart: handling pending tuplet stop, note " <<
              note->asShortString () <<
              ", line " << inputLineNumber <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          finalizeTupletAndPopItFromTupletsStack (
            inputLineNumber);

          // the tuplet stop is not to be handled later
          fCurrentATupletStopIsPending = false;
        }
        */

        // create the tuplet
        createAndPushTupletUponItsFirstNote (
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

          // populate the tuplet at the top of the stack
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceTuplets ()) {
            gLog <<
              "--> kTupletTypeContinue: adding tuplet member note " <<
              note->asShortString () <<
              " to stack top tuplet " <<
              currentTuplet->asString () <<
              ", line " << inputLineNumber <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          // fetch current voice
          S_msrVoice
            currentVoice =
              fetchVoiceFromCurrentPart (
                inputLineNumber,
                fCurrentMusicXMLStaffNumber,
                fCurrentMusicXMLVoiceNumber);

          // add note to tuplet
          currentTuplet->
            appendNoteToTuplet (
              note,
              currentVoice);

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceTupletsDetails ()) {
            displayTupletsStack (
              "############## kTupletTypeContinue");
          }
#endif // MF_TRACE_IS_ENABLED

/* JMI
          // set note displayed divisions
          note->
            applyTupletMemberDisplayFactor (
              fTupletsStack.front ()->getTupletActualNotes (),
              fTupletsStack.front ()->getTupletNormalNotes ());
*/
        }

        else {
          std::stringstream ss;

          ss <<
            "handleNoteBelongingToATuplet():" <<
            std::endl <<
            "tuplet member note " <<
            note->
              asShortString () <<
            " cannot be added, tuplets stack is empty";

          mxsr2msrInternalErrorWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
      }
      break;

    case msrTupletTypeKind::kTupletTypeStop:
      {
        switch (fTupletsStack.size ()) {
          case 0:
            {
              std::stringstream ss;

              ss <<
                "handleNoteBelongingToATuplet():" <<
                std::endl <<
                "tuplet member note " <<
                note->
                  asShortString () <<
                " cannot be added, tuplets stack is empty";

              mxsr2msrInternalErrorWithMeasureInfo (
                gServiceRunData->getInputSourceName (),
                inputLineNumber,
                __FILE__, __LINE__,
                ss.str (),
                fCurrentMeasureNumber,
                fMsrScore->getScoreNumberOfMeasures ());
            }
            break;

          case 1:
            {
              // outer-most tuplet:
              // add the note to to it before finalizing it
              S_msrTuplet
                currentTuplet =
                  fTupletsStack.front ();

              // populate the tuplet at the top of the stack
#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceTuplets ()) {
                gLog <<
                  "--> kTupletTypeStop: adding outer-most tuplet member note " <<
                  note->asShortString () <<
                  " to stack top tuplet " <<
                  currentTuplet->asString () <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

              // fetch current voice
              S_msrVoice
                currentVoice =
                  fetchVoiceFromCurrentPart (
                    inputLineNumber,
                    fCurrentMusicXMLStaffNumber,
                    fCurrentMusicXMLVoiceNumber);

              // add note to tuplet
              currentTuplet->
                appendNoteToTuplet (
                  note,
                  currentVoice);

#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceTupletsDetails ()) {
                displayTupletsStack (
                  "############## kTupletTypeStop, outer-most");
              }
#endif // MF_TRACE_IS_ENABLED

    //* JMI
              if (fCurrentATupletStopIsPending) {
                // end of a tuplet forces handling of the pending one
#ifdef MF_TRACE_IS_ENABLED
                if (gTraceOahGroup->getTraceTuplets ()) {
                  gLog <<
                    "--> kTupletTypeStop: finalizing pending tuplet" <<
                    ", line " << inputLineNumber <<
                    std::endl;
                }
#endif // MF_TRACE_IS_ENABLED

                finalizeTupletAndPopItFromTupletsStack (
                  inputLineNumber);

                // the tuplet stop is not to be handled later
                fCurrentATupletStopIsPending = false;
              }
              else {
#ifdef MF_TRACE_IS_ENABLED
                if (gTraceOahGroup->getTraceTuplets ()) {
                  gLog <<
                    "--> kTupletTypeStop: finalizing non-pending tuplet" << // JMI ???
                    ", line " << inputLineNumber <<
                    std::endl;
                }
#endif // MF_TRACE_IS_ENABLED

                finalizeTupletAndPopItFromTupletsStack (
                  inputLineNumber);
              }
    //*/

              // don't pop the inner-most tuplet from the stack yet

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
#ifdef MF_TRACE_IS_ENABLED
                if (gTraceOahGroup->getTraceTuplets ()) {
                  gLog <<
                    "--> kTupletTypeStop: finalizing tuplet" <<
                    ", line " << inputLineNumber <<
                    std::endl;
                }
#endif // MF_TRACE_IS_ENABLED

                finalizeTupletAndPopItFromTupletsStack (
                  inputLineNumber);

                // the tuplet stop is not to be handled later
                fCurrentATupletStopIsPending = false;
              }
              */

              S_msrTuplet
                currentTuplet =
                  fTupletsStack.front ();

              // populate the tuplet at the top of the stack
#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceTuplets ()) {
                gLog <<
                  "--> kTupletTypeStop: adding nested tuplet member note " <<
                  note->asShortString () <<
                  " to stack top tuplet " <<
                  currentTuplet->asString () <<
                  ", line " << inputLineNumber <<
                  std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

              // fetch current voice
              S_msrVoice
                currentVoice =
                  fetchVoiceFromCurrentPart (
                    inputLineNumber,
                    fCurrentMusicXMLStaffNumber,
                    fCurrentMusicXMLVoiceNumber);

              // add note to tuplet
              currentTuplet->
                appendNoteToTuplet (
                  note,
                  currentVoice);

#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceTupletsDetails ()) {
                displayTupletsStack (
                  "############## kTupletTypeStop, nested");
              }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceTuplets ()) {
          std::stringstream ss;

          ss <<
            "--> kTupletTypeStartAndStopInARow: note: " <<
            note->asShortString () <<
            ", line " << inputLineNumber <<
            std::endl;

          gWaeHandler->waeTraceWithMeasureInfo (
            __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }
#endif // MF_TRACE_IS_ENABLED

        if (fCurrentSingleTremolo) {
          fCurrentTupletTypeKind = msrTupletTypeKind::kTupletTypeStartAndStopInARow;
        }
        else {
          std::stringstream ss;

          ss <<
            "one-note tuplet with a non single tremolo contents found";

     // JMI     mxsr2msrErrorWithMeasureInfo (
          mxsr2msrWarningWithMeasureInfo (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
       //     __FILE__, __LINE__,
            ss.str (),
            fCurrentMeasureNumber,
            fMsrScore->getScoreNumberOfMeasures ());
        }

        // create the tuplet
        createAndPushTupletUponItsFirstNote (
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
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "a rest cannot belong to a chord in a tuplet");
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);

  // should a chord be created?
  if (! fOnGoingChord) {
    // this is the second note of the chord to be created,

    // fetch the current tuplet, i.e. the top of the stack
    S_msrTuplet currentTuplet;

    /* JMI
    if (fTupletsStack.size ()) {
      currentTuplet =
        fTupletsStack.front ();
    }
    else {
      std::stringstream ss;

      ss <<
        "handleNoteBelongingToAChordInATuplet():" <<
        std::endl <<
        " a tuplet member chord " <<
        "cannot be added, tuplets stack is empty";

      mxsr2msrInternalErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
    */

    currentTuplet =
      fLastHandledTupletInVoiceMap [
        std::make_pair (
          fCurrentMusicXMLStaffNumber,
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

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTupletsDetails ()) {
      displayTupletsStack (
        "############## After  removeLastNoteFromTuplet()");
    }
#endif // MF_TRACE_IS_ENABLED

    // create the current chord from its first note
    fCurrentChord =
      createChordFromItsFirstNote (
        inputLineNumber,
        currentVoice,
        tupletLastNote,
        msrNoteKind::kNoteRegularInChord);

#ifdef MF_TRACE_IS_ENABLED
    if (false) { // JMI
      std::stringstream ss;

      ss <<
        "&&&&&&&&&&&&&&&&&& currentVoice (" <<
        currentVoice->getVoiceName () <<
        ") contents &&&&&&&&&&&&&&&&&&" <<
        std::endl <<
        currentVoice <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    // add chord to the current tuplet instead of tupletLastNote
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChords () || gTraceOahGroup->getTraceTuplets ()) {
      std::stringstream ss;

      ss <<
        "Adding chord " <<
        fCurrentChord->asString () <<
        " to stack top tuplet " <<
        currentTuplet->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

    currentTuplet->
      appendChordToTuplet (fCurrentChord);

    if (fCurrentNoteSoundingWholeNotesFromNotesDuration.getNumerator () == 0) {
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Adding another note " <<
      newChordNote->
        asShortString () <<
      ", line " << inputLineNumber <<
      " to current chord in voice " <<
      currentVoice->getVoiceName () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentChord->
    addAnotherNoteToChord (
      newChordNote, currentVoice);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords () || gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Handling a note belonging to a chord in grace notes" <<
      ", newChordNote is " <<
      newChordNote->asShortString () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fCurrentNoteIsARest) {
    mxsr2msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      "a rest cannot belong to a chord in a grace notes group");
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
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
        std::stringstream ss;

        ss <<
          "handleNoteBelongingToAChordInGraceNotes():" <<
          std::endl <<
          "chordFirstNote is null on " <<
          newChordNote->asString ();

        mxsr2msrInternalErrorWithMeasureInfo (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str (),
          fCurrentMeasureNumber,
          fMsrScore->getScoreNumberOfMeasures ());
      }
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceChords () || gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "The grace notes chord's first note is " <<
        chordFirstNote->asShortString () <<
        "" <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
#endif // MF_TRACE_IS_ENABLED

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
      gLog <<
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
      std::stringstream ss;

      ss <<
        "handleNoteBelongingToAChordInGraceNotes():" <<
        std::endl <<
        "tuplet member chord " << chord->asString () <<
        "cannot be added, tuplets stack is empty";

      mxsr2msrInternalErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
    */

    // account for chord being built
    fOnGoingChord = true;
  }

  // register note as another member of chord
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Adding another note " <<
      newChordNote->
        asShortString () <<
      ", line " << inputLineNumber <<
      " to current chord in voice " <<
      currentVoice->getVoiceName () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentChord->
    addAnotherNoteToChord (
      newChordNote, currentVoice);

  // copy newChordNote's elements if any to the chord
  copyNoteElementsToChord (
    newChordNote, fCurrentChord);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsDetails ()) {
    printCurrentChord ();
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleTupletsPendingOnTupletsStack (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Handling tuplets pending on tuplet stack" <<
      ", fCurrentMusicXMLStaffNumber: " <<
      fCurrentMusicXMLStaffNumber <<
      ", line: " << inputLineNumber <<
      std::endl;
  }

  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## Before  handleTupletsPendingOnTupletsStack()");
  }
#endif // MF_TRACE_IS_ENABLED

/* superfluous JMI
  // fetch current voice
  S_msrVoice
    currentVoice =
      fetchVoiceFromCurrentPart (
        inputLineNumber,
        fCurrentMusicXMLStaffNumber,
        fCurrentMusicXMLVoiceNumber);
*/

  // handle tuplets pending on the tuplet stack
  while (fTupletsStack.size ()) {
    S_msrTuplet
      pendingTuplet =
        fTupletsStack.front ();

    // finalize the tuplet, thus popping it off the stack
    finalizeTupletAndPopItFromTupletsStack (
      inputLineNumber);
  } // while

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsDetails ()) {
    displayTupletsStack (
      "############## Before  handleTupletsPendingOnTupletsStack()");
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::displayLastHandledTupletInVoiceMap (const std::string& header)
{
  gLog <<
    std::endl <<
    header <<
    ", fLastHandledTupletInVoiceMap contains:";

  if (! fLastHandledTupletInVoiceMap.size ()) {
    gLog <<
      " none" <<
      std::endl;
  }

  else {
    std::map<std::pair<int, int>, S_msrTuplet>::const_iterator
      iBegin = fLastHandledTupletInVoiceMap.begin (),
      iEnd   = fLastHandledTupletInVoiceMap.end (),
      i      = iBegin;

    gLog << std::endl;

    ++gIndenter;

    for ( ; ; ) {
      gLog <<
        "staff " << (*i).first.first <<
        ", voice " <<  (*i).first.second <<
        std::endl;
//        "\"" << (*i).first->getVoiceName () <<
//        "\" ----> " << (*i).second->asString ();
      if (++i == iEnd) break;
      gLog << std::endl;
    } // for

    gLog << std::endl;

    --gIndenter;
  }

  gLog << std::endl;
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleRepeatStart (
  const S_msrBarLine& barLine)
{
  int inputLineNumber =
    barLine->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start" <<
    /* JMI
      ", measure " <<
        barLine->getBarLineMeasureNumber () <<
      ", position " <<
      barLine->getBarLineMeasurePosition () <<
      */
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  const S_msrBarLine& barLine)
{
  int inputLineNumber =
    barLine->getInputLineNumber ();

  std::string repeatStartMeasureNumber =
    fCurrentRepeatStartMeasureNumber.size ()
      ? // there was a repeat start before hand
        fCurrentRepeatStartMeasureNumber
      : // there is an implicit repeat start at the beginning of the part
        fPartFirstMeasureNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat end in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", fCurrentMeasureNumber: \"" << fCurrentMeasureNumber <<
      "\", fCurrentRepeatStartMeasureNumber: \"" << fCurrentRepeatStartMeasureNumber <<
      "\", repeatStartMeasureNumber: \"" << repeatStartMeasureNumber <<
      "\", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  const S_msrBarLine& barLine)
{
  int inputLineNumber =
    barLine->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending start in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    std::stringstream ss;

    ss <<
      std::endl << std::endl <<
      "****************** handleRepeatEndingStart()" <<
      ", line " << inputLineNumber <<
      std::endl <<
      fCurrentPart <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the repeat ending
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending upon its start in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPart->
    handleRepeatEndingStartInPart (
      inputLineNumber);

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleRepeatHookedEndingEnd (
  const S_msrBarLine& barLine)
{
  int inputLineNumber =
    barLine->
      getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat hooked ending end in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    std::stringstream ss;

    ss <<
      std::endl << std::endl <<
      "****************** handleRepeatHookedEndingEnd()" <<
      ", line " << inputLineNumber <<
      std::endl <<
      fCurrentPart <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine); // JMI

  // create a hooked repeat ending from the current segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a hooked repeat ending in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPart->
    handleRepeatEndingEndInPart (
      inputLineNumber,
      fCurrentBarLineEndingNumber,
      msrRepeatEndingKind::kRepeatEndingHooked);
}

//______________________________________________________________________________
void mxsr2msrTranslator::handleRepeatHooklessEndingEnd (
  const S_msrBarLine& barLine)
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat hookless ending end in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    std::stringstream ss;

    ss <<
      std::endl << std::endl <<
      "****************** handleRepeatHooklessEndingEnd()" <<
      ", line " << inputLineNumber <<
      std::endl <<
      fCurrentPart <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the bar line to the current part
  fCurrentPart->
    appendBarLineToPart (barLine);

  // create a hookless repeat ending from the current segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a hookless repeat ending in part " <<
      fCurrentPart->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_rehearsal" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      std::stringstream ss;

      ss <<
        "rehearsalMark enclosure \"" << rehearsalEnclosure <<
        "\"" << " is not handled, ignored";

      mxsr2msrWarningWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
    }
  }

  // create a rehearsalMark
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRehearsalMarks ()) {
    std::stringstream ss;

    ss <<
      "Creating rehearsalMark \"" << rehearsalValue << "\"" <<
      " in part " <<
      fCurrentPart->getPartCombinedName () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRehearsalMark
    rehearsalMark =
      msrRehearsalMark::create (
        inputLineNumber,
        rehearsalKind,
        rehearsalValue,
        fCurrentDirectionPlacementKind);

  // append the rehearsalMark to the pending tempos list
  fPendingRehearsalMarksList.push_back (rehearsalMark);
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_harmony& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_harmony" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

//   fCurrentFiguredBassWholeNotesDuration = msrWholeNotes (0, 1);

  fOnGoingHarmony = true;
}

void mxsr2msrTranslator::visitStart (S_root& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_root" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_root_step" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_root_alter" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_function& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_function" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string fCurrentHarmonyFunctionText = elt->getValue ();
}

void mxsr2msrTranslator::visitStart (S_kind& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_kind" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
        inputLineNumber,
        __FILE__, __LINE__,
        "harmony kind \"" + kindValue + "\" os unknown");
    }
    else {
      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
      }
  }
  */
}

void mxsr2msrTranslator::visitStart (S_inversion& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_inversion" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
  Inversion is a number indicating which inversion is used:
  0 for root position, 1 for first inversion, etc.
*/

  fCurrentHarmonyInversion = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_bass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bass" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitStart (S_bass_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bass_step" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_bass_alter" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_degree& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_degree" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_degree_value" <<
       ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentHarmonyDegreeValue = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_degree_alter& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_degree_alter" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_degree_type& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_degree_type" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
        inputLineNumber,
        __FILE__, __LINE__,
        "harmony degree-type \"" + degreeType + "\" is unknown");
  }
}

void mxsr2msrTranslator::visitEnd (S_degree& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_degree" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create harmony degree
  S_msrHarmonyDegree
    harmonyDegree =
      msrHarmonyDegree::create (
        inputLineNumber,
        fCurrentHarmonyDegreeValue,
        fCurrentHarmonyDegreeAlterationKind,
        fCurrentHarmonyDegreeTypeKind);

  // register it in current harmony degrees list
  fCurrentHarmonyDegreesList.push_back (
    harmonyDegree);
}

void mxsr2msrTranslator::visitEnd (S_harmony& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_harmony" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
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
          inputLineNumber,
          fCurrentHarmonyRootQuarterTonesPitchKind)) <<
      ", ignoring the latter";

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());

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
        inputLineNumber <<
        std::endl;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
        ", line " << inputLineNumber << ":" <<
        std::endl;

      ++gIndenter;

      const int fieldWidth = 32;

      gLog << std::left <<
        std::setw (fieldWidth) << "fCurrentPart" << ": " <<
        fCurrentPart->getPartCombinedName () <<
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
        fCurrentNoteSoundingWholeNotes.asString () <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmoniesStaffNumber" << ": " <<
        fCurrentHarmoniesStaffNumber <<
        std::endl <<

        std::setw (fieldWidth) << "fCurrentHarmonyWholeNotesOffset" << ": " <<
        fCurrentHarmonyWholeNotesOffset <<
//         std::endl <<
//
//         std::setw (fieldWidth) << "fCurrentFiguredBassWholeNotesDuration" << ": " <<
//         fCurrentFiguredBassWholeNotesDuration <<
        std::endl;

      --gIndenter;

      gWaeHandler->waeTraceWithMeasureInfo (
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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
          inputLineNumber,
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
void mxsr2msrTranslator::visitStart (S_frame& elt)
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

  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fOnGoingHarmony) {
    std::stringstream ss;

    ss << "<frame /> out of context, not handled";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

  // color JMI

  fOnGoingFrame = true;
}

void mxsr2msrTranslator::visitStart (S_frame_strings& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame_strings" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentFrameStrings = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_frame_frets& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame_frets" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentFrameFrets = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_first_fret& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_first_fret" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::visitStart (S_frame_note& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame_note" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_barre" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

  // color JMI
}

void mxsr2msrTranslator::visitEnd (S_frame_note& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_frame_note" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the frame note
  S_msrFrameNote
    frameNote =
      msrFrameNote::create (
        inputLineNumber,
        fCurrentFrameNoteStringNumber,
        fCurrentFrameNoteFretNumber,
        fCurrentFrameNoteFingering,
        fCurrentFrameNoteBarreTypeKind);

  // append the frame note to the pending frame notes list
  fPendingFramesNotesList.push_back (frameNote);

  fOnGoingFrameNote = false;
}

void mxsr2msrTranslator::visitEnd (S_frame& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_frame" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

      // remove it from the list
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_figured_bass" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

      mxsr2msrErrorWithMeasureInfo (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str (),
        fCurrentMeasureNumber,
        fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::visitStart (S_figure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_figure" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitStart (S_prefix& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_prefix" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
    std::stringstream ss;

    ss <<
      "prefix \"" << prefix <<
      "\" is unknown";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }

}

void mxsr2msrTranslator::visitStart (S_figure_number& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_figure_number" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentFigureNumber = (int)(*elt);

  if (fCurrentFigureNumber > 13) {
    std::stringstream ss;

    ss <<
      "figure-number " << fCurrentFigureNumber <<
      " is greater that 13, that's strange...";

    mxsr2msrWarningWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitStart (S_suffix& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_suffix" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
    std::stringstream ss;

    ss <<
      "suffix \"" << suffix <<
      "\" is unknown";

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitEnd (S_figure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_figure" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the bass figure
  S_msrBassFigure
    bassFigure =
      msrBassFigure::create (
        inputLineNumber,
        fCurrentPart,
        fCurrentFigurePrefixKind,
        fCurrentFigureNumber,
        fCurrentFigureSuffixKind);

  // append it to the pending figures list
  fPendingFiguredBassFiguresList.push_back (
    bassFigure);
}

void mxsr2msrTranslator::visitEnd (S_figured_bass& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_figured_bass" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the figured bass element
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating a figured bass" <<
      ", line " << inputLineNumber << ":" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the figured bass element
  // if the sounding whole notes is 0/1 (no <duration /> was found), JMI ???
  // it will be set to the next note's sounding whole notes later

  S_msrFiguredBass
    figuredBass =
      msrFiguredBass::create (
        inputLineNumber,
        fCurrentFiguredBassSoundingWholeNotes,
        fCurrentFiguredBassDisplayWholeNotes,
//         fCurrentFiguredBassWholeNotesDuration,
        fCurrentFiguredBassParenthesesKind,
        msrTupletFactor (1, 1)); // will be set upon next note handling

  // attach pending figures to the figured bass
  if (! fPendingFiguredBassFiguresList.size ()) {
    mxsr2msrWarning (
      gServiceRunData->getInputSourceName (),
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

    // append figuredBass to the pending figured basses list
    fPendingFiguredBassesList.push_back (figuredBass);
  }

  fOnGoingFiguredBass = false;
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_harp_pedals& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_harp_pedals" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      "Creating harp pedals tuning:" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pedal_tuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsr2msrTranslator::visitStart (S_pedal_step& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pedal_step" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_pedal_alter" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

    mxsr2msrErrorWithMeasureInfo (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
}

void mxsr2msrTranslator::visitEnd (S_pedal_tuning& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_pedal_tuning" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a harp pedals tuning
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarpPedalsTunings ()) {
    gLog <<
      "Creating harp pedal tuning:" <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 31;

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
      inputLineNumber,
      fCurrentHarpPedalDiatonicPitchKind,
      fCurrentHarpPedalAlterationKind);
}

void mxsr2msrTranslator::visitStart( S_damp& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_damp" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

    // append it to the pending damps list
    fPendingDampsList.push_back (damp);
  }
}

void mxsr2msrTranslator::visitStart( S_damp_all& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_damp_all" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

    // append it to the pending damp alls list
    fPendingDampAllsList.push_back (dampAll);
  }
}

//________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_capo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_capo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffDetailsCapo = (int)(*elt);
}

void mxsr2msrTranslator::visitStart (S_staff_size& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_staff_size" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffDetailsStaffSize = (int)(*elt);
  // JMI not used
}

void mxsr2msrTranslator::visitEnd (S_staff_details& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_staff_details" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    const int fieldWidth = 29;

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
      fCurrentStaffDetailsStaffSize <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_scordatura" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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
      inputLineNumber);
}

void mxsr2msrTranslator::visitStart (S_accord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_accord" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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

void mxsr2msrTranslator::visitEnd (S_accord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_accord" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingAccord = false;

//   gLog <<
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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the current scordatura to the pending scordatura list
  fPendingScordaturasList.push_back (fCurrentScordatura);

  // forget about this scordatura
  fCurrentScordatura = nullptr;
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_instrument_sound& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_instrument_sound" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_virtual_instrument& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_virtual_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_midi_device& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_midi_device" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
      <midi-device>SmartMusic SoftSynth</midi-device>
*/
}

//______________________________________________________________________________
void mxsr2msrTranslator::visitStart (S_midi_instrument& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_midi_instrument" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
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


/*

print-object:

<!--
	The printout entity is based on MuseData print
	suggestions. They allow a way to specify not to print
	print an object (e.g. note or rest), its augmentation
	dots, or its lyrics. This is especially useful for notes
	that overlap in different voices, or for chord sheets
	that contain lyrics and chords but no melody. For wholly
	invisible notes, such as those providing sound-only data,
	the attribute for print-spacing may be set to no so that
	no space is left for this note. The print-spacing value
	is only used if no note, dot, or lyric is being printed.

	By default, all these attributes are set to yes. If
	print-object is set to no, print-dot and print-lyric are
	interpreted to also be set to no if they are not present.
-->
<!ENTITY % print-object
	"print-object  %yes-no;  #IMPLIED">

<!ENTITY % print-spacing
	"print-spacing %yes-no;  #IMPLIED">

<!ENTITY % printout
	"%print-object;
	 print-dot     %yes-no;  #IMPLIED
	 %print-spacing;
	 print-lyric   %yes-no;  #IMPLIED">




<!--
	The part-name-display and part-abbreviation-display
	elements are used in both the score.mod and direction.mod
	files. They allow more precise control of how part names
	and abbreviations appear throughout a score. The
	print-object attributes can be used to determine what,
	if anything, is printed at the start of each system.
	Formatting specified in the part-name-display and
	part-abbreviation-display elements override the formatting
	specified in the part-name and part-abbreviation elements,
	respectively.
-->
<!ELEMENT part-name-display
	((display-text | accidental-text)*)>
<!ATTLIST part-name-display
    %print-object;
>
<!ELEMENT part-abbreviation-display
	((display-text | accidental-text)*)>
<!ATTLIST part-abbreviation-display
    %print-object;
>



<!--
	The other-direction element is used to define any direction
	symbols not yet in the MusicXML format. The smufl attribute
	can be used to specify a particular direction symbol, allowing
	application interoperability without requiring every SMuFL
	glyph to have a MusicXML element equivalent. Using the
	other-direction type without the smufl attribute allows
	for extended representation, though without application
	interoperability.
-->
<!ELEMENT other-direction (#PCDATA)>
<!ATTLIST other-direction
	%print-object;
    %print-style-align;
    %smufl;
    %optional-unique-id;
>



	The harmony object may be used for analysis or for
	chord symbols. The print-object attribute controls
	whether or not anything is printed due to the harmony
	element. The print-frame attribute controls printing
	of a frame or fretboard diagram. The print-style entity
	sets the default for the harmony, but individual elements
	can override this with their own print-style values.



<!--
	A root is a pitch name like C, D, E, where a function
	is an indication like I, II, III. Root is generally
	used with pop chord symbols, function with classical
	functional harmony. It is an either/or choice to avoid
	data inconsistency. Function requires that the key be
	specified in the encoding.

	The root element has a root-step and optional root-alter
	similar to the step and alter elements in a pitch, but
	renamed to distinguish the different musical meanings.
	The root-step text element indicates how the root should
	appear in a score if not using the element contents.
	In some chord styles, this will include the root-alter
	information as well. In that case, the print-object
	attribute of the root-alter element can be set to no.
	The root-alter location attribute indicates whether
	the alteration should appear to the left or the right
	of the root-step; it is right by default.
-->
<!ELEMENT root (root-step, root-alter?)>
<!ELEMENT root-step (#PCDATA)>
<!ATTLIST root-step
    text CDATA #IMPLIED
    %print-style;
>
<!ELEMENT root-alter (#PCDATA)>
<!ATTLIST root-alter
    %print-object;
    %print-style;
    location %left-right; #IMPLIED
>
<!ELEMENT function (#PCDATA)>
<!ATTLIST function
    %print-style;
>



<!--
	Bass is used to indicate a bass note in popular music
	chord symbols, e.g. G/C. It is generally not used in
	functional harmony, as inversion is generally not used
	in pop chord symbols. As with root, it is divided into
	step and alter elements, similar to pitches. The attributes
	for bass-step and bass-alter work the same way as
	the corresponding attributes for root-step and root-alter.
-->
<!ELEMENT bass (bass-step, bass-alter?)>
<!ELEMENT bass-step (#PCDATA)>
<!ATTLIST bass-step
    text CDATA #IMPLIED
    %print-style;
>
<!ELEMENT bass-alter (#PCDATA)>
<!ATTLIST bass-alter
    %print-object;
    %print-style;
    location (left | right) #IMPLIED
>



<!--
	The degree element is used to add, alter, or subtract
	individual notes in the chord. The degree-value element
	is a number indicating the degree of the chord (1 for
	the root, 3 for third, etc). The degree-alter element
	is like the alter element in notes: 1 for sharp, -1 for
	flat, etc. The degree-type element can be add, alter, or
	subtract. If the degree-type is alter or subtract, the
	degree-alter is relative to the degree already in the
	chord based on its kind element. If the degree-type is
	add, the degree-alter is relative to a dominant chord
	(major and perfect intervals except for a minor
	seventh). The print-object attribute can be used to
	keep the degree from printing separately when it has
	already taken into account in the text attribute of
	the kind element. The plus-minus attribute is used to
	indicate if plus and minus symbols should be used
	instead of sharp and flat symbols to display the degree
	alteration; it is no by default.

	The degree-value and degree-type text attributes specify
	how the value and type of the degree should be displayed
	in a score. The degree-value symbol attribute indicates
	that a symbol should be used in specifying the degree.
	If the symbol attribute is present, the value of the text
	attribute follows the symbol.

	A harmony of kind "other" can be spelled explicitly by
	using a series of degree elements together with a root.
-->
<!ELEMENT degree (degree-value, degree-alter, degree-type)>
<!ATTLIST degree
    %print-object;
>
<!ELEMENT degree-value (#PCDATA)>
<!ATTLIST degree-value
    symbol (major | minor | augmented |
		diminished | half-diminished) #IMPLIED
    text CDATA #IMPLIED
    %print-style;
>
<!ELEMENT degree-alter (#PCDATA)>
<!ATTLIST degree-alter
    %print-style;
    plus-minus %yes-no; #IMPLIED
>
<!ELEMENT degree-type (#PCDATA)>
<!ATTLIST degree-type
    text CDATA #IMPLIED
    %print-style;
>




<!--
	The system-dividers element indicates the presence or
	absence of system dividers (also known as system separation
	marks) between systems displayed on the same page. Dividers
	on the left and right side of the page are controlled by
	the left-divider and right-divider elements respectively.
	The default vertical position is half the system-distance
	value from the top of the system that is below the divider.
	The default horizontal position is the left and right
	system margin, respectively.

	When used in the print element, the system-dividers element
	affects the dividers that would appear between the current
	system and the previous system.
-->
<!ELEMENT system-dividers (left-divider, right-divider)>
<!ELEMENT left-divider EMPTY>
<!ATTLIST left-divider
    %print-object;
    %print-style-align;
>
<!ELEMENT right-divider EMPTY>
<!ATTLIST right-divider
    %print-object;
    %print-style-align;
>



<!--
	The position and printout entities for printing suggestions
	are defined in the common.mod file.

	The print-leger attribute is used to indicate whether leger
	lines are printed. Notes without leger lines are used to
	indicate indeterminate high and low notes. By default, it
	is set to yes. If print-object is set to no, print-leger
	is interpreted to also be set to no if not present. This
	attribute is ignored for rests.

	The dynamics and end-dynamics attributes correspond to
	MIDI 1.0's Note On and Note Off velocities, respectively.
	They are expressed in terms of percentages of the default
	forte value (90 for MIDI 1.0).

	The attack and release attributes are used to alter the
	starting and stopping time of the note from when it would
	otherwise occur based on the flow of durations - information
	that is specific to a performance. They are expressed in
	terms of divisions, either positive or negative. A note that
	starts a tie should not have a release attribute, and a note
	that stops a tie should not have an attack attribute. The
	attack and release attributes are independent of each other.
	The attack attribute only changes the starting time of a
	note, and the release attribute only changes the stopping
	time of a note.

	If a note is played only particular times through a repeat,
	the time-only entity shows which times to play the note.

	The pizzicato attribute is used when just this note is
	sounded pizzicato, vs. the pizzicato element which changes
	overall playback between pizzicato and arco.
-->
<!ATTLIST note
    %print-style;
    %printout;
    print-leger %yes-no; #IMPLIED
    dynamics CDATA #IMPLIED
    end-dynamics CDATA #IMPLIED
    attack CDATA #IMPLIED
    release CDATA #IMPLIED
    %time-only;
    pizzicato %yes-no; #IMPLIED
    %optional-unique-id;
>



<!--
	Notations are musical notations, not XML notations. Multiple
	notations are allowed in order to represent multiple editorial
	levels. The print-object attribute, added in Version 3.0,
	allows notations to represent details of performance technique,
	such as fingerings, without having them appear in the score.
-->
<!ELEMENT notations
	(%editorial;,
	 (tied | slur | tuplet | glissando | slide |
	  ornaments | technical | articulations | dynamics |
	  fermata | arpeggiate | non-arpeggiate |
	  accidental-mark | other-notation)*)>
<!ATTLIST notations
    %print-object;
    %optional-unique-id;
>



<!--
	The other-notation element is used to define any notations not
	yet in the MusicXML format. It handles notations where more
	specific extension elements such as other-dynamics and
	other-technical are not appropriate. The smufl attribute can
	be used to specify a particular notation, allowing application
	interoperability without requiring every SMuFL glyph to have a
	MusicXML element equivalent. Using the other-notation element
	without the smufl attribute allows for extended representation,
	though without application interoperability.
-->
<!ELEMENT other-notation (#PCDATA)>
<!ATTLIST other-notation
    type %start-stop-single; #REQUIRED
    number %number-level; "1"
    %print-object;
    %print-style;
    %placement;
    %smufl;
    %optional-unique-id;
>



<!--
	The harmonic element indicates natural and artificial
	harmonics. Natural harmonics usually notate the base
	pitch rather than the sounding pitch. Allowing the type
	of pitch to be specified, combined with controls for
	appearance/playback differences, allows both the notation
	and the sound to be represented. Artificial harmonics can
	add a notated touching-pitch; the pitch or fret at which
	the string is touched lightly to produce the harmonic.
	Artificial pinch harmonics will usually not notate a
	touching pitch. The attributes for the harmonic element
	refer to the use of the circular harmonic symbol, typically
	but not always used with natural harmonics.
-->
<!ELEMENT harmonic
	((natural | artificial)?,
	 (base-pitch | touching-pitch | sounding-pitch)?)>
<!ATTLIST harmonic
    %print-object;
    %print-style;
    %placement;
>
<!ELEMENT natural EMPTY>
<!ELEMENT artificial EMPTY>
<!ELEMENT base-pitch EMPTY>
<!ELEMENT touching-pitch EMPTY>
<!ELEMENT sounding-pitch EMPTY>

<!--
	The open-string element represents the zero-shaped
	open string symbol.
-->
<!ELEMENT open-string EMPTY>
<!ATTLIST open-string
    %print-style;
    %placement;
>



<!--
	Text underlays for lyrics, based on Humdrum with support
	for other formats. The lyric number indicates multiple
	lines, though a name can be used as well (as in Finale's
	verse/chorus/section specification). Word extensions are
	represented using the extend element. Hyphenation is
	indicated by the syllabic element, which can be single,
	begin, end, or middle. These represent single-syllable
	words, word-beginning syllables, word-ending syllables,
	and mid-word syllables. Multiple syllables on a single
	note are separated by elision elements. A hyphen in the
	text element should only be used for an actual hyphenated
	word. Two text elements that are not separated by an
	elision element are part of the same syllable, but may have
	different text formatting.

	Humming and laughing representations are taken from
	Humdrum. The end-line and end-paragraph elements come
	from RP-017 for Standard MIDI File Lyric meta-events;
	they help facilitate lyric display for Karaoke and
	similar applications. Language names for text elements
	come from ISO 639, with optional country subcodes from
	ISO 3166. Justification is center by default; placement is
	below by default. The print-object attribute can override
	a note's print-lyric attribute in cases where only some
	lyrics on a note are printed, as when lyrics for later verses
	are printed in a block of text rather than with each note.
	The time-only attribute precisely specifies which lyrics are
	to be sung which time through a repeated section.
-->
<!ELEMENT lyric
	((((syllabic?, text),
	   (elision?, syllabic?, text)*, extend?) |
	   extend | laughing | humming),
	  end-line?, end-paragraph?, %editorial;)>
<!ATTLIST lyric
    number NMTOKEN #IMPLIED
    name CDATA #IMPLIED
    %justify;
    %position;
    %placement;
    %color;
    %print-object;
    %time-only;
    %optional-unique-id;
>




<!--
	The part-name indicates the full name of the musical part.
	The part-abbreviation indicates the abbreviated version of
	the name of the musical part. The part-name will often
	precede the first system, while the part-abbreviation will
	precede the other systems. The formatting attributes for
	these elements are deprecated in Version 2.0 in favor of
	the new part-name-display and part-abbreviation-display
	elements. These are defined in the common.mod file as they
	are used in both the part-list and print elements. They
	provide more complete formatting control for how part names
	and abbreviations appear in a score.
-->
<!ELEMENT part-name (#PCDATA)>
<!ATTLIST part-name
    %print-style;
    %print-object;
    %justify;
>
<!ELEMENT part-abbreviation (#PCDATA)>
<!ATTLIST part-abbreviation
    %print-style;
    %print-object;
    %justify;
>
<!--
	The part-group element indicates groupings of parts in the
	score, usually indicated by braces and brackets. Braces
	that are used for multi-staff parts should be defined in
	the attributes element for that part. The part-group start
	element appears before the first score-part in the group.
	The part-group stop element appears after the last
	score-part in the group.

	The number attribute is used to distinguish overlapping
	and nested part-groups, not the sequence of groups. As
	with parts, groups can have a name and abbreviation.
	Formatting attributes for group-name and group-abbreviation
	are deprecated in Version 2.0 in favor of the new
	group-name-display and group-abbreviation-display elements.
	Formatting specified in the group-name-display and
	group-abbreviation-display elements overrides formatting
	specified in the group-name and group-abbreviation
	elements, respectively.

	The group-symbol element indicates how the symbol for
	a group is indicated in the score. Values include none,
	brace, line, bracket, and square; the default is none.
	The group-barline element indicates if the group should
	have common barlines. Values can be yes, no, or
	Mensurstrich. The group-time element indicates that the
	displayed time signatures should stretch across all parts
	and staves in the group. Values for the child elements
	are ignored at the stop of a group.

	A part-group element is not needed for a single multi-staff
	part. By default, multi-staff parts include a brace symbol
	and (if appropriate given the bar-style) common barlines.
	The symbol formatting for a multi-staff part can be more
	fully specified using the part-symbol element, defined in
	the attributes.mod file.
-->
<!ELEMENT part-group (group-name?, group-name-display?,
	group-abbreviation?, group-abbreviation-display?,
	group-symbol?, group-barline?, group-time?, %editorial;)>
<!ATTLIST part-group
    type %start-stop; #REQUIRED
    number CDATA "1"
>

<!ELEMENT group-name (#PCDATA)>
<!ATTLIST group-name
    %print-style;
    %justify;
>
<!ELEMENT group-name-display
	((display-text | accidental-text)*)>
<!ATTLIST group-name-display
    %print-object;
>
<!ELEMENT group-abbreviation (#PCDATA)>
<!ATTLIST group-abbreviation
    %print-style;
    %justify;
>
<!ELEMENT group-abbreviation-display
	((display-text | accidental-text)*)>
<!ATTLIST group-abbreviation-display
    %print-object;
>

<!ELEMENT group-symbol (#PCDATA)>
<!ATTLIST group-symbol
    %position;
    %color;
>

<!ELEMENT group-barline (#PCDATA)>
<!ATTLIST group-barline
    %color;
>
<!ELEMENT group-time EMPTY>





*/



/* JMI
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceFiguredBasses ()) {
      gLog <<
        "--> figured bass" <<
        ", line " << inputLineNumber << ":" <<
        std::endl;

      ++gIndenter;

      const int fieldWidth = 31;

      gLog << std::left <<
        std::setw (fieldWidth) << "fCurrentPart" << ": " <<
        fCurrentPart->getPartCombinedName () <<
        std::endl <<
        std::setw (fieldWidth) << "fCurrentFiguredBassSoundingWholeNotes" << ": " <<
        fCurrentFiguredBassSoundingWholeNotes.asString () <<
        std::endl;

      --gIndenter;
    }
#endif // MF_TRACE_IS_ENABLED

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
//           fFiguredBassWholeNotesDuration,
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) { // JMI
     const int fieldWidth = 27;

    gLog << std::left <<
      std::endl <<
      "==> AFTER visitEnd (S_note&) " <<
      newNote->asString () <<
      ", line " << inputLineNumber <<
      " we have:" <<
      std::endl <<
      std::setw (fieldWidth) <<
      "--> fCurrentMusicXMLStaffNumber" << ": " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "--> fCurrentMusicXMLVoiceNumber" << ": " <<
      fCurrentMusicXMLVoiceNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "--> current voice" << ": \"" <<
      currentVoice->getVoiceName () << "\"" <<
      std::endl <<
      "<==" <<
      std::endl;

    gWaeHandler->waeTraceWithMeasureInfo (
      __FILE__, __LINE__,
      ss.str (),
      fCurrentMeasureNumber,
      fMsrScore->getScoreNumberOfMeasures ());
  }
#endif // MF_TRACE_IS_ENABLED
*/

/* JMI
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) { // JMI
    gLog <<
      std::endl <<
      "==> BEFORE visitEnd (S_note&)" <<
      ", line " << inputLineNumber <<
      " we have:" <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 27;

    gLog << std::left <<
      std::setw (fieldWidth) << "--> fCurrentMusicXMLStaffNumber" << ": " <<
      fCurrentMusicXMLStaffNumber <<
      std::endl <<
      std::setw (fieldWidth) << "--> fCurrentMusicXMLVoiceNumber" << ": " <<
      fCurrentMusicXMLVoiceNumber <<
      std::endl <<
      std::setw (fieldWidth) << "--> current voice" << ": \"" <<
      currentVoice->getVoiceName () << "\"" <<
      std::endl;

    --gIndenter;

    gLog <<
      "<==" <<
      std::endl <<
      * std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
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
//       ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//       ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
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
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif // MF_TRACE_IS_ENABLED
//
//       // append the rehearsalMark to the pending tempos list
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
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
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
//       ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//       ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // append dal segno al fine to the pending tempos list
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
//       ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//       ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
//       ", line " << inputLineNumber <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // append dal segno al coda to the pending tempos list
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
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif // MF_TRACE_IS_ENABLED
//
//       // append coda to the pending tempos list
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
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
//           ", line " << inputLineNumber <<
//           std::endl;
//       }
//     #endif // MF_TRACE_IS_ENABLED
//
//       // append the rehearsalMark to the pending tempos list
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
//           ", fCurrentDirectionStaffNumber: " << fCurrentDirectionStaffNumber <<
//           ", fPreviousNoteMusicXMLVoiceNumber: " << fPreviousNoteMusicXMLVoiceNumber <<
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
//                     ", line " << ligature->getInputLineNumber () <<
//                     std::endl;
//                 }
// #endif // MF_TRACE_IS_ENABLED
//
//                 note->appendLigatureToNote (ligature);
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
//                     ", line " << ligature->getInputLineNumber () <<
//                     std::endl;
//                 }
// #endif // MF_TRACE_IS_ENABLED
//
//                 note->appendLigatureToNote (ligature);
//                 break;
//               default:
//                 ;
//             } // switch
