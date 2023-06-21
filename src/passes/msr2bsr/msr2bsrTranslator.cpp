/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <sstream>

#include "visitor.h"

#include "bsrWae.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfServices.h"

#include "msr2bsrComponent.h"

#include "msrBarLines.h"
#include "msrBreaks.h"
#include "msrVoiceStaffChanges.h"

#include "bsrPages.h"
#include "bsrScores.h"

#include "msr2bsrTranslator.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2bsrOah.h"
#include "brailleGenerationOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
void msr2bsrTranslator::createFirstPageHeadingIfNotYetCreated (
  int inputLineNumber)
{ // JMI
//   if (! fFirstPageHeading) {
//     // create the first page heading // JMI
//     fFirstPageHeading =
//       bsrPageHeading::create (
//         inputLineNumber,
//         workCreditTypeTitle,
//         pagination,
//         1); //    pageHeadingNumber
//   }
}

void msr2bsrTranslator::createFirstMusicHeadingIfNotYetCreated (
  int inputLineNumber)
{
  if (! fFirstMusicHeading) {
    // create the first music heading
    fFirstMusicHeading =
      bsrMusicHeading::create (
        inputLineNumber);
  }
}

//________________________________________________________________________
msr2bsrTranslator::msr2bsrTranslator ()
{};

msr2bsrTranslator::~msr2bsrTranslator ()
{}

//________________________________________________________________________
S_bsrScore msr2bsrTranslator::translateMsrToBsr (
  const S_msrScore& theMsrScore)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // the MSR score we're visiting
  fVisitedMsrScore = theMsrScore;

  // pages & lines
  fCurrentPrintPageNumber = 1;
  fCurrentPrintLineNumber = 1;

  // notes
  fCurrentNoteOctaveKind    = bsrNoteOctaveKind::kNoteOctaveNone;
  fCurrentNoteValueSizeKind = bsrNoteValueSizeKind::kNoteValueSizeLarger;

  // measures
// JMI  fMeasuresCounter = 0;

  // create the resulting BSR score
  fResultingBsr =
    bsrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
      fVisitedMsrScore);

  // create a msrScore browser
  msrBrowser<msrScore> browser (this);

  // browse the score with the browser
  browser.browse (*fVisitedMsrScore);

  // forget about the visited MSR score
  fVisitedMsrScore = nullptr;

  return fResultingBsr;
}

//________________________________________________________________________
void msr2bsrTranslator::notSupportedMessage (
  int           inputLineNumber,
  const std::string& message)
{
  // issue a warning message to the output log stream
  bsrWarning (
    gServiceRunData->getInputSourceName (),
    inputLineNumber,
    message);

  // append a note to the BSR JMI
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrLineBreak& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrLineBreak" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the line
  fCurrentLine =
    bsrLine::create (
      inputLineNumber,
      ++fCurrentPrintLineNumber,
      gGlobalBrailleGenerationOahGroup->getCellsPerLine ());

  // append it to the current page
  fCurrentPage->
    appendLineToPage (fCurrentLine);

  // a note octave will be needed for the next note to come,
  // i.e., the first one in the new line
  fCurrentNoteOctaveKind = bsrNoteOctaveKind::kNoteOctaveNone;
}

void msr2bsrTranslator::visitEnd (S_msrLineBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrLineBreak" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrPageBreak& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPageBreak" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPage =
    bsrPage::create (
      inputLineNumber,
      ++fCurrentPrintPageNumber,
      gGlobalBrailleGenerationOahGroup->getLinesPerPage ());

  fResultingBsr->
    appendPageToScore (fCurrentPage);
}

void msr2bsrTranslator::visitEnd (S_msrPageBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPageBreak" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrScore& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrScore" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get the worktitle if any
  S_msrIdentification
    identification =
      elt->getIdentification ();

  std::string
    workCreditTypeTitle =
      identification->
        getIdentificationWorkCreditTypeTitle ();

  // append a first transcription note to it
  S_bsrTranscriptionNotesElement
    transcriptionNotesElement =
      bsrTranscriptionNotesElement::create (
        inputLineNumber,
        "This Braille data created by "
          +
        gOahOahGroup->getOahOahGroupServiceName ());

  fResultingBsr->
    getTranscriptionNotes ()->
      appendElementToTranscriptionNotes (
        transcriptionNotesElement);

  // create the first page
  fCurrentPage =
    bsrPage::create (
      inputLineNumber,
      fCurrentPrintPageNumber,
      gGlobalBrailleGenerationOahGroup->getLinesPerPage ());

    /* JMI
  // create a pagination
  S_bsrPagination
    pagination =
      bsrPagination::create (
        inputLineNumber,
        1,  // printPageNumber
        1); // braillePageNumber

  // append the first page heading to the first page JMI
  createFirstPageHeadingIfNotYetCreated (
    inputLineNumber);

  fCurrentPage->
    appendPageHeadingToPage (fFirstPageHeading);
    */

  // append the first music heading to the first page
  createFirstMusicHeadingIfNotYetCreated (
    inputLineNumber);

  fCurrentPage->
    appendMusicHeadingToPage (fFirstMusicHeading);

/* JMI
  // create a foot notes
  S_bsrFootNotes
    footNotes =
      bsrFootNotes::create (
        inputLineNumber);

  // append a first transcription note to it
  S_bsrFootNotesElement
    footNotesElement =
      bsrFootNotesElement::create (
        inputLineNumber,
        "A sympathetic and most useful foot note");

  footNotes->
    appendElementToFootNotes (
      footNotesElement);

  // append foot notes to the first page
  fCurrentPage->
    appendFootNotesToPage (footNotes);
*/

  // append first page to the score
  fResultingBsr->
    appendPageToScore (fCurrentPage);

  // create the fisrt line
  fCurrentLine =
    bsrLine::create (
      inputLineNumber,
      fCurrentPrintLineNumber,
      gGlobalBrailleGenerationOahGroup->getCellsPerLine ());

  // append the fisrt line to the first page
  fCurrentPage->
    appendLineToPage (fCurrentLine);
}

void msr2bsrTranslator::visitEnd (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputEndLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrScore" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/*
  if (fWorkCreditTypeTitleKnown && fMovementTitleKnown) {
    std::string
      workCreditTypeTitle =
        fCurrentIdentification->
          getIdentificationWorkCreditTypeTitle ()->
            getVariableValue (),
      movementTitle =
        fCurrentIdentification->
          getIdentificationMovementTitle ()->
            getVariableValue ();

    if (
      workCreditTypeTitle.size () == 0
        &&
      movementTitle.size () > 0) {
      // use the movement title as the work title
      fCurrentIdentification->
        setIdentificationWorkCreditTypeTitle (
          inputLineNumber, movementTitle);

      fBsrScoreHeader->
        setIdentificationWorkCreditTypeTitle (
          inputLineNumber, movementTitle);

      // forget the movement title
      fCurrentIdentification->
        setIdentificationMovementTitle (
          inputLineNumber, "");

      fBsrScoreHeader->
        setIdentificationMovementTitle (
          inputLineNumber, "");
    }
  }

  else if (! fWorkCreditTypeTitleKnown && fMovementTitleKnown) {
    std::string
      movementTitle =
        fCurrentIdentification->
          getIdentificationMovementTitle ()->
            getVariableValue ();

    // use the movement title as the work title
    fCurrentIdentification->
      setIdentificationWorkCreditTypeTitle (
        inputLineNumber, movementTitle);

    fBsrScoreHeader->
      setIdentificationWorkCreditTypeTitle (
        inputLineNumber, movementTitle);

    // forget the movement title
    fCurrentIdentification->
      setIdentificationMovementTitle (
        inputLineNumber, "");

    fBsrScoreHeader->
      setIdentificationMovementTitle (
        inputLineNumber, "");
  }

  // set ident and short indent if needed
  setPaperIndentsIfNeeded (
    elt->getGeometry ());
    */
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPartGroup = elt;
}

void msr2bsrTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();
#endif // MF_TRACE_IS_ENABLED

  std::string
    partCombinedName =
      elt->getPartCombinedName ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPart " <<
      partCombinedName <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << inputLineNumber << " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // remember current part
  fCurrentPart = elt;

  // register the current part in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentPartIDAndName (
      elt->getPartIDAndName ());

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputEndLineNumber ();

  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaffTuning" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a staff tuning clone
  fCurrentStaffTuning = elt;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaffDetails" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentStaffTuning = nullptr;
}

void msr2bsrTranslator::visitEnd (S_msrStaffDetails& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrStaffDetails" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStaff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  fCurrentStaff = elt;

  // register the current staff number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentStaveNumber (
      fCurrentStaff->getStaffNumber ());
}

void msr2bsrTranslator::visitEnd (S_msrStaff& elt)
{
  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting S_msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrVoice& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // remember current voice
  fCurrentVoice = elt;

  // register the current voice number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentVoiceNumber (
      fCurrentVoice->getVoiceNumber ());

  // forget about the current relative octave reference
  fRelativeOctaveReference = nullptr;

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrVoice& elt)
{
  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << '\'' <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrBarLine& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBarLine" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Handling '" <<
      msrBarLineCategoryKindAsString (
        elt->getBarLineCategory ());

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get the MSR barLine attributes

  msrBarLineStyleKind
    mBarLineStyleKind =
      elt->getBarLineStyleKind ();

  // let's go

  bsrBarLineKind
    bBarLineKind = bsrBarLineKind::kBarLineKindNone;

  switch (mBarLineStyleKind) {
    case msrBarLineStyleKind::kBarLineStyleNone:
      break;
    case msrBarLineStyleKind::kBarLineStyleRegular:
      break;
    case msrBarLineStyleKind::kBarLineStyleDotted:
      bBarLineKind = bsrBarLineKind::kBarLineKindSpecial;
      break;
    case msrBarLineStyleKind::kBarLineStyleDashed:
      bBarLineKind = bsrBarLineKind::kBarLineKindSpecial;
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavy:
      break;
    case msrBarLineStyleKind::kBarLineStyleLightLight:
      bBarLineKind = bsrBarLineKind::kBarLineKindSectionalDouble;
      break;
    case msrBarLineStyleKind::kBarLineStyleLightHeavy:
      bBarLineKind = bsrBarLineKind::kBarLineKindFinalDouble;
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavyLight:
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavyHeavy:
      break;
    case msrBarLineStyleKind::kBarLineStyleTick:
      break;
    case msrBarLineStyleKind::kBarLineStyleShort:
      break;
  } // switch

  if (bBarLineKind == bsrBarLineKind::kBarLineKindNone) {
    std::stringstream ss;

    ss <<
      "MSR barLine kind '" <<
      msrBarLineStyleKindAsString (mBarLineStyleKind) <<
      "' is not supported in Braille";

    notSupportedMessage (
      inputLineNumber,
      ss.str ());
  }
  else {
    S_bsrBarLine
      barLine =
        bsrBarLine::create (
          inputLineNumber, bBarLineKind);

    fCurrentMeasure->
      appendBarLineToMeasure (barLine);
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputStartLineNumber ();

  fCurrentMeasureNumber =
    elt->getMeasureNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasure '" <<
      fCurrentMeasureNumber <<
      '\'' <<
      ", line " << inputLineNumber;

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

  // create a measure
  fCurrentMeasure =
    bsrMeasure::create (
      inputLineNumber,
      fCurrentMeasureNumber);

  // append it to the current line
  fCurrentLine->
    appendMeasureToLine (fCurrentMeasure);
}

void msr2bsrTranslator::visitEnd (S_msrMeasure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputEndLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasure '" <<
      elt->getMeasureNumber () <<
      '\'' <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrClef& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrClef" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalMsr2bsrOahGroup->getIncludeClefs ()) {
    bsrClefKind bClefKind = bsrClefKind::kClefKindNone;

  /* JMI
          kClefModifiedBassForRightHandPartKind,
          kClefModifiedTrebleForLeftHandPartKind };
          */

    msrClefKind mClefKind = elt->getClefKind ();

    switch (mClefKind) {
      case msrClefKind::kClef_UNKNOWN_:
        bClefKind = bsrClefKind::kClefKindNone;
        break;
      case msrClefKind::kClefTreble:
        bClefKind = bsrClefKind::kClefKindGTreble;
        break;
      case msrClefKind::kClefSoprano:
        bClefKind = bsrClefKind::kClefKindGSoprano;
        break;
      case msrClefKind::kClefMezzoSoprano:
        bClefKind = bsrClefKind::kClefKindGSoprano; // JMI ???
        break;
      case msrClefKind::kClefAlto:
        bClefKind = bsrClefKind::kClefKindCAlto;
        break;
      case msrClefKind::kClefTenor:
        bClefKind = bsrClefKind::kClefKindCTenor;
        break;
      case msrClefKind::kClefBaritone:
        bClefKind = bsrClefKind::kClefKindCBaritone;
        break;
      case msrClefKind::kClefBass:
        bClefKind = bsrClefKind::kClefKindFBass;
        break;
      case msrClefKind::kClefTrebleLine1:
        bClefKind = bsrClefKind::kClefKindGSoprano;
        break;
      case msrClefKind::kClefTrebleMinus15:
        break;
      case msrClefKind::kClefTrebleMinus8:
        bClefKind = bsrClefKind::kClefKindGOttavaBassa; // JMI permute???
        break;
      case msrClefKind::kClefTreblePlus8:
        bClefKind = bsrClefKind::kClefKindGOttavaAlta; // JMI permute???
        break;
      case msrClefKind::kClefTreblePlus15:
        break;
      case msrClefKind::kClefBassMinus15:
        break;
      case msrClefKind::kClefBassMinus8:
        break;
      case msrClefKind::kClefBassPlus8:
        break;
      case msrClefKind::kClefBassPlus15:
        break;
      case msrClefKind::kClefVarbaritone:
        break;
      case msrClefKind::kClefTablature4:
        break;
      case msrClefKind::kClefTablature5:
        break;
      case msrClefKind::kClefTablature6:
        break;
      case msrClefKind::kClefTablature7:
        break;
      case msrClefKind::kClefPercussion:
        break;
      case msrClefKind::kClefJianpu:
        break;
    } // switch

    if (bClefKind == bsrClefKind::kClefKindNone) {
      std::stringstream ss;

      ss <<
        "MSR clef kind '" <<
        msrClefKindAsString (mClefKind) <<
        "' is not supported in Braille";

      notSupportedMessage (
        inputLineNumber,
        ss.str ());
    }
    else {
      S_bsrClef
        clef =
          bsrClef::create (
            inputLineNumber, bClefKind);

      fCurrentMeasure->
        appendClefToMeasure (clef);
    }
  }
}

void msr2bsrTranslator::visitEnd (S_msrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrClef" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrKey& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrKey" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get MSR key attributes

  msrKeyKind
    theMsrKeyKind = elt->getKeyKind ();

  msrQuarterTonesPitchKind
    mTonicQuarterTonesPitchKind =
      elt->getKeyTonicQuarterTonesPitchKind ();

  msrSemiTonesPitchKind
    mSemiTonesPitchKind =
      semiTonesPitchKindFromQuarterTonesPitchKind (
        mTonicQuarterTonesPitchKind);

  msrModeKind
    mModeKind =
      elt->getModeKind ();

  // let's go

  bsrKeyKind theBsrKeyKind = bsrKeyKind::kKeyKindNone;
  int        numberOfAlterations = 0;

  switch (theMsrKeyKind) {
    case msrKeyKind::kKeyTraditional:

      // traditional keys
      switch (mSemiTonesPitchKind) {
        case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
          break;

        case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_C_Flat:
          break;

        case msrSemiTonesPitchKind::kSTP_C_Natural:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindNaturals;
              numberOfAlterations = 0;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 3;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_C_Sharp:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 7;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 4;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
          break;

        case msrSemiTonesPitchKind::kSTP_D_Flat:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 5;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 8;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_D_Natural:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 2;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 1;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_D_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
          break;

        case msrSemiTonesPitchKind::kSTP_E_Flat:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 3;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 6;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_E_Natural:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 4;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 1;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_E_Sharp:
        case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_F_Flat:
          break;

        case msrSemiTonesPitchKind::kSTP_F_Natural:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 1;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 4;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_F_Sharp:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 6;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 3;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_G_Flat:
          break;

        case msrSemiTonesPitchKind::kSTP_G_Natural:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 1;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 2;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_G_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
          break;

        case msrSemiTonesPitchKind::kSTP_A_Flat:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 4;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 7;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_A_Natural:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 3;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindNaturals;
              numberOfAlterations = 0;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_A_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
          break;

        case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
          break;
        case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
          break;

        case msrSemiTonesPitchKind::kSTP_B_Flat:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 2;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindFlats;
              numberOfAlterations = 5;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_B_Natural:
          switch (mModeKind) {
            case msrModeKind::kMode_UNKNOWN_:
              break;
            case msrModeKind::kModeMajor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 5;
              break;
            case msrModeKind::kModeMinor:
              theBsrKeyKind = bsrKeyKind::kKeyKindSharps;
              numberOfAlterations = 2;
              break;
            case msrModeKind::kModeIonian:
              break;
            case msrModeKind::kModeDorian:
              break;
            case msrModeKind::kModePhrygian:
              break;
            case msrModeKind::kModeLydian:
              break;
            case msrModeKind::kModeMixolydian:
              break;
            case msrModeKind::kModeAeolian:
              break;
            case msrModeKind::kModeLocrian:
              break;
          } // switch
          break;

        case msrSemiTonesPitchKind::kSTP_B_Sharp:
          break;
        case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
          break;
        case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
          break;
      } // switch
      break;

    case msrKeyKind::kKeyHumdrumScot:
      // JMI
      break;
  } // switch

  // create the BSR key
  if (theBsrKeyKind == bsrKeyKind::kKeyKindNone) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceKeys ()) {
      gLog <<
        "BSR key kind is absent, ignoring '" <<
        bsrKeyKindAsString (theBsrKeyKind) <<
        ", " <<
       msrSemiTonesPitchKindAsString (mSemiTonesPitchKind) <<
       std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }
  else {
    S_bsrKey
      key =
        bsrKey::create (
          inputLineNumber,
          theBsrKeyKind,
          numberOfAlterations);

    if (! fFirstKey) {
      // register key in first page heading
      fFirstMusicHeading->
        setMusicHeadingKey (key);
      fFirstKey = key;
    }
    else {
      // append the BSR key to the current measure
      fCurrentLine->
        appendKeyToLine (key);
    }
  }
}

void msr2bsrTranslator::visitEnd (S_msrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrKey" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTimeSignature& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTimeSignature" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get MSR time attributes

  msrTimeSignatureSymbolKind
    mTimeSignatureSymbolKind =
      elt->getTimeSignatureSymbolKind ();

/* JMI
  Bool
    mTimeIsCompound =
      elt->getTimeIsCompound ();
  */

  const std::vector<S_msrTimeSignatureItem>&
    mTimeSignatureItemsVector =
      elt->getTimeSignatureItemsVector ();

  // let's go

  bsrTimeSignatureKind bTimeKind = bsrTimeSignatureKind::kTimeSignatureNone;

  switch (mTimeSignatureSymbolKind) {
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone:
      bTimeKind = bsrTimeSignatureKind::kTimeSignatureNone;

    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon:
      bTimeKind = bsrTimeSignatureKind::kTimeSignatureCommon;
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut:
      bTimeKind = bsrTimeSignatureKind::kTimeSignatureCut;
      break;

    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote:
      bTimeKind = bsrTimeSignatureKind::kTimeSignatureNote;
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote:
      bTimeKind = bsrTimeSignatureKind::kTimeSignatureDottedNote;
      break;

    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber:
      bTimeKind = bsrTimeSignatureKind::kTimeSignatureSingleNumber;
      break;

    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
      bTimeKind = bsrTimeSignatureKind::kTimeSignatureSenzaMisura;
      break;
    break;
  } // switch

  // create the BSR time signature
  S_bsrTimeSignature
    timeSignature =
      bsrTimeSignature::create (
        inputLineNumber,
        bTimeKind);

/*
  if (mTimeIsCompound) {
    // JMI ???
  }
  else {
  */

  if (mTimeSignatureItemsVector.size ()) {
    for (
      std::vector<S_msrTimeSignatureItem>::const_iterator i =
        mTimeSignatureItemsVector.begin ();
      i != mTimeSignatureItemsVector.end ();
      ++i
    ) {
      S_msrTimeSignatureItem mTimeSignatureItem = (*i);

      // get the MSR time signature item attributes

      int
        mTimeSignatureBeatValue =
          mTimeSignatureItem->getTimeSignatureBeatValue ();

      const std::vector<int>&
        mTimeSignatureBeatsNumbersVector =
          mTimeSignatureItem->getTimeSignatureBeatsNumbersVector ();

      size_t vectorSize = mTimeSignatureBeatsNumbersVector.size ();

      for (size_t i = 0; i < vectorSize; ++i) {
        int
          mTimeSignatureItemBeatsNumber =
            mTimeSignatureBeatsNumbersVector [i];

        // create the BSR time signature item
        S_bsrTimeSignatureItem
          bTimeSignatureItem =
            bsrTimeSignatureItem::create (inputLineNumber);

        // populate it
        bTimeSignatureItem->
          appendBeatsNumber (mTimeSignatureItemBeatsNumber);
        bTimeSignatureItem->
          setTimeSignatureBeatValue (mTimeSignatureBeatValue);

        // append it to the time signature
        timeSignature->
          appendTimeSignatureItem (bTimeSignatureItem);
      } // for
    } // for
  }

  if (! fFirstTimeSignature) {
    // register timeSignature in first page heading
    createFirstMusicHeadingIfNotYetCreated ( // JMI
      inputLineNumber);

//     fFirstMusicHeading-> JMI STRANGE PROBLEM
//       setMusicHeadingTimeSignature (timeSignature);

    fFirstTimeSignature = timeSignature;
  }

  else {
    // this time signature belongs to the current measure,
    // which has already been created and appended
    // to the current line,
    // but it should be placed before it in the latter, so:
    fCurrentLine->
      insertTimeBeforeLastElementOfLine (timeSignature);
  }

  // a note octave will be needed for the next note to come,
  // i.e., the first one after a numeric indicator
  fCurrentNoteOctaveKind = bsrNoteOctaveKind::kNoteOctaveNone;
}

void msr2bsrTranslator::visitEnd (S_msrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTimeSignature" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTempo& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTempo" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalMsr2bsrOahGroup->getNoTempos ()) {
    // create the BSR tempo
    S_bsrTempo
      tempo =
        bsrTempo::create (
          inputLineNumber,
          elt);

    // append the BSR tempo to the current measure
    fCurrentLine->
      appendTempoToLine (tempo);
  }
}

void msr2bsrTranslator::visitEnd (S_msrTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTempo" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
bsrNoteOctaveIsNeeded msr2bsrTranslator::brailleOctaveMarkIfNeeded (
  const S_msrNote& note)
{
  bsrNoteOctaveIsNeeded
    result = bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededNo;

  int inputLineNumber =
    note->getInputStartLineNumber ();

  // select BSR octave sign relative to fRelativeOctaveReference

  // in Braille, octave number is 4 for the octave starting with middle C
  msrOctaveKind
    noteAbsoluteOctaveKind =
      note->getNoteOctaveKind ();

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      note->
        noteDiatonicPitchKind (
          inputLineNumber);

  msrDiatonicPitchKind
    referenceDiatonicPitchKind =
      fRelativeOctaveReference->
        noteDiatonicPitchKind (
          inputLineNumber);

  std::string
    referenceDiatonicPitchKindAsString =
      fRelativeOctaveReference->
        noteDiatonicPitchKindAsString (
          inputLineNumber);

  msrOctaveKind
    referenceAbsoluteOctaveKind =
      fRelativeOctaveReference->
        getNoteOctaveKind ();

  /*
    In a melodic progression:

    (a) the octave is not marked for the second of two consecutive notes if the interval is less than a fourth,

    (b) the octave is always marked in a skip greater than a fifth, and

    (c) the octave is only marked in a skip of a fourth or fifth when the second note is in a different octave from the first.
  */

  int
    noteAboluteDiatonicOrdinal =
      (int) noteAbsoluteOctaveKind * 7
        +
      (int) noteDiatonicPitchKind - (int) msrDiatonicPitchKind::kDiatonicPitchC,

    referenceAboluteDiatonicOrdinal =
      (int) referenceAbsoluteOctaveKind * 7
        +
      (int) referenceDiatonicPitchKind - (int) msrDiatonicPitchKind::kDiatonicPitchC;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    const int fieldWidth = 28;

    std::stringstream ss;

    ss << std::left <<
/*
      std::setw (fieldWidth) <<
      "% referenceDiatonicPitch" <<
      ": " <<
      referenceDiatonicPitch <<
      std::endl <<
*/
      std::setw (fieldWidth) <<
      "% fRelativeOctaveReference" <<
      ": " <<
      fRelativeOctaveReference->asShortString () <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% note" <<
      ": " <<
      note->asShortString () <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceDiatonicPitchAsString" <<
      ": " <<
      referenceDiatonicPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAbsoluteOctaveKind" <<
       ": " <<
      msrOctaveKindAsString (referenceAbsoluteOctaveKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAbsoluteOctaveKind" <<
       ": " <<
      msrOctaveKindAsString (noteAbsoluteOctaveKind) <<
      std::endl << std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal" <<
      ": " <<
      referenceAboluteDiatonicOrdinal <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal" <<
      ": " <<
      noteAboluteDiatonicOrdinal <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal - referenceAboluteDiatonicOrdinal" <<
      ": " <<
      noteAboluteDiatonicOrdinal - referenceAboluteDiatonicOrdinal <<
      std::endl <<
      std::setw (fieldWidth) <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::stringstream ss;

  // should an octave sign be used?
  switch (noteAboluteDiatonicOrdinal - referenceAboluteDiatonicOrdinal) {
    case 0:
    case 1: case -1:
    case 2: case -2:
      // less than a fourth, no octave sign needed
      break;
    case 3: case -3:
    case 4: case -4:
      // a fourth or fifth, octave sign needed if there an octave change
      if (noteAbsoluteOctaveKind == referenceAbsoluteOctaveKind) {
        result = bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededNo;
      }
      else {
        result = bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededYes;
      }
      break;
    default:
      // more that a fifth, use an octave sign
      result = bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededYes;
  } // switch

  return result;
}

void msr2bsrTranslator::createBsrForNote (const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputStartLineNumber ();

  // get MSR note attributes

  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      note->getNoteQuarterTonesPitchKind ();

/* JMI
  msrSemiTonesPitchKind
    noteSemiTonesPitchKind =
      semiTonesPitchKindFromQuarterTonesPitchKind (
        noteQuarterTonesPitchKind);
        */

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      diatonicPitchKindFromQuarterTonesPitchKind (
        inputLineNumber,
        noteQuarterTonesPitchKind);

  msrAccidentalKind
    mAccidentalKind =
      note->
        getNoteAccidentalKind ();

  int noteDotsNumber = note->getNoteDotsNumber ();

  msrNotesDurationKind
    noteGraphicNotesDurationKind =
      note->
        getNoteGraphicNotesDurationKind ();

  msrOctaveKind
    noteOctave =
      note->getNoteOctaveKind ();

  // let's go

  // braille the note words if any,
  // which should precede the articulations in LilyPond
  // generate code for the words only
  // if note doesn't belong to a chord,
  // otherwise it will be generated for the chord itself
  if (! note->getNoteBelongsToAChord ()) {
    const std::list<S_msrWords>&
      noteWords =
        note->getNoteWords ();

    if (noteWords.size ()) {
      std::list<S_msrWords>::const_iterator i;

      for (
        i = noteWords.begin ();
        i != noteWords.end ();
        ++i
      ) {
        msrPlacementKind
          wordsPlacementKind =
            (*i)->getWordsPlacementKind ();

        std::string wordsContents =
          (*i)->getWordsContents ();

        msrFontStyleKind
          wordsFontStyleKind =
            (*i)->getWordsFontStyleKind ();

        S_msrFontSize
          wordsFontSize =
            (*i)->getWordsFontSize ();

        msrFontWeightKind
          wordsFontWeightKind =
            (*i)->getWordsFontWeightKind ();

        {
          switch (wordsPlacementKind) {
            case msrPlacementKind::kPlacement_UNKNOWN_:
              break;
            case msrPlacementKind::kPlacementAbove:
              break;
            case msrPlacementKind::kPlacementBelow:
              break;
          } // switch

          switch (wordsFontStyleKind) {
            case msrFontStyleKind::kFontStyleNone:
              break;
            case msrFontStyleKind::kFontStyleNormal:
              break;
            case msrFontStyleKind::KFontStyleItalic:
              break;
          } // switch

          switch (wordsFontWeightKind) {
            case msrFontWeightKind::kFontWeightNone:
              break;
            case msrFontWeightKind::kFontWeightNormal:
              break;
            case msrFontWeightKind::kFontWeightBold:
              break;
          } // switch

          switch (wordsFontSize->getFontSizeKind ()) {
            case msrFontSizeKind::kFontSizeNone:
              break;
            case msrFontSizeKind::kFontSizeXXSmall:
              break;
            case msrFontSizeKind::kFontSizeXSmall:
              break;
            case msrFontSizeKind::kFontSizeSmall:
              break;
            case msrFontSizeKind::kFontSizeMedium:
              break;
            case msrFontSizeKind::kFontSizeLarge:
              break;
            case msrFontSizeKind::kFontSizeXLarge:
              break;
            case msrFontSizeKind::kFontSizeXXLarge:
              break;
            case msrFontSizeKind::kFontSizeNumeric:
              break;
          } // switch

          // create the words std::string
          S_bsrWords
            bWords =
              bsrWords::create (
                inputLineNumber, wordsContents);

          // append it to the current measure
          fCurrentMeasure->
            appendWordsToMeasure (bWords);

          // a note octave will be needed for the next note to come,
          // i.e., the first one after the word sign
          fCurrentNoteOctaveKind = bsrNoteOctaveKind::kNoteOctaveNone;
        }
      } // for
    }
  }

  bsrNoteOctaveKind
    noteOctaveKind =
      bsrNoteOctaveKind::kNoteOctaveNone;

  // middle C starts octave 4, as in MusicXML
  switch (noteOctave) {
    case msrOctaveKind::kOctave1: noteOctaveKind = bsrNoteOctaveKind::kNoteOctave1; break;
    case msrOctaveKind::kOctave2: noteOctaveKind = bsrNoteOctaveKind::kNoteOctave2; break;
    case msrOctaveKind::kOctave3: noteOctaveKind = bsrNoteOctaveKind::kNoteOctave3; break;
    case msrOctaveKind::kOctave4: noteOctaveKind = bsrNoteOctaveKind::kNoteOctave4; break;
    case msrOctaveKind::kOctave5: noteOctaveKind = bsrNoteOctaveKind::kNoteOctave5; break;
    case msrOctaveKind::kOctave6: noteOctaveKind = bsrNoteOctaveKind::kNoteOctave6; break;
    case msrOctaveKind::kOctave7: noteOctaveKind = bsrNoteOctaveKind::kNoteOctave7; break;
    default:
      // kNoteOctaveBelow1Kind and kNoteOctaveAbove7Kind
      // cannot occur in MusicXML
      ;
  } // switch

  bsrNoteValueKind noteValueKind = bsrNoteValueKind::kNoteValueNone;

  if (note->fetchNoteIsARest ()) {
    switch (noteGraphicNotesDurationKind) {
      case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
        break;

      case msrNotesDurationKind::kNotesDuration1024th:
        break;
      case msrNotesDurationKind::kNotesDuration512th:
        break;
      case msrNotesDurationKind::kNotesDuration256th:
        noteValueKind = bsrNoteValueKind::kNoteValueRest256th;
        break;
      case msrNotesDurationKind::kNotesDuration128th:
        noteValueKind = bsrNoteValueKind::kNoteValueRest128th;
        break;
      case msrNotesDurationKind::kNotesDuration64th:
        noteValueKind = bsrNoteValueKind::kNoteValueRest64th;
        break;
      case msrNotesDurationKind::kNotesDuration32nd:
        noteValueKind = bsrNoteValueKind::kNoteValueRest32nd;
        break;
      case msrNotesDurationKind::kNotesDuration16th:
        noteValueKind = bsrNoteValueKind::kNoteValueRest16th;
        break;
      case msrNotesDurationKind::kNotesDurationEighth:
        noteValueKind = bsrNoteValueKind::kNoteValueRest8th;
        break;
      case msrNotesDurationKind::kNotesDurationQuarter:
        noteValueKind = bsrNoteValueKind::kNoteValueRestQuarter;
        break;
      case msrNotesDurationKind::kNotesDurationHalf:
        noteValueKind = bsrNoteValueKind::kNoteValueRestHalf;
        break;
      case msrNotesDurationKind::kNotesDurationWhole:
        noteValueKind = bsrNoteValueKind::kNoteValueRestWhole;
        break;
      case msrNotesDurationKind::kNotesDurationBreve:
        noteValueKind = bsrNoteValueKind::kNoteValueRestBreve;
        break;
      case msrNotesDurationKind::kNotesDurationLonga:
        break;
      case msrNotesDurationKind::kNotesDurationMaxima:
        break;
    } // switch
  }

  else {
    switch (noteDiatonicPitchKind) {
      case msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN_:
        break;

      case msrDiatonicPitchKind::kDiatonicPitchA:
        switch (noteGraphicNotesDurationKind) {
          case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
            break;

          case msrNotesDurationKind::kNotesDuration1024th:
            break;
          case msrNotesDurationKind::kNotesDuration512th:
            break;
          case msrNotesDurationKind::kNotesDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueA256th;
            break;
          case msrNotesDurationKind::kNotesDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueA128th;
            break;
          case msrNotesDurationKind::kNotesDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueA64th;
            break;
          case msrNotesDurationKind::kNotesDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueA32nd;
            break;
          case msrNotesDurationKind::kNotesDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueA16th;
            break;
          case msrNotesDurationKind::kNotesDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueA8th;
            break;
          case msrNotesDurationKind::kNotesDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueAQuarter;
            break;
          case msrNotesDurationKind::kNotesDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueAHalf;
            break;
          case msrNotesDurationKind::kNotesDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueAWhole;
            break;
          case msrNotesDurationKind::kNotesDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueABreve;
            break;
          case msrNotesDurationKind::kNotesDurationLonga:
            break;
          case msrNotesDurationKind::kNotesDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchB:
        switch (noteGraphicNotesDurationKind) {
          case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
            break;

          case msrNotesDurationKind::kNotesDuration1024th:
            break;
          case msrNotesDurationKind::kNotesDuration512th:
            break;
          case msrNotesDurationKind::kNotesDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueB256th;
            break;
          case msrNotesDurationKind::kNotesDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueB128th;
            break;
          case msrNotesDurationKind::kNotesDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueB64th;
            break;
          case msrNotesDurationKind::kNotesDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueB32nd;
            break;
          case msrNotesDurationKind::kNotesDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueB16th;
            break;
          case msrNotesDurationKind::kNotesDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueB8th;
            break;
          case msrNotesDurationKind::kNotesDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueBQuarter;
            break;
          case msrNotesDurationKind::kNotesDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueBHalf;
            break;
          case msrNotesDurationKind::kNotesDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueBWhole;
            break;
          case msrNotesDurationKind::kNotesDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueBBreve;
            break;
          case msrNotesDurationKind::kNotesDurationLonga:
            break;
          case msrNotesDurationKind::kNotesDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchC:
        switch (noteGraphicNotesDurationKind) {
          case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
            break;

          case msrNotesDurationKind::kNotesDuration1024th:
            break;
          case msrNotesDurationKind::kNotesDuration512th:
            break;
          case msrNotesDurationKind::kNotesDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueC256th;
            break;
          case msrNotesDurationKind::kNotesDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueC128th;
            break;
          case msrNotesDurationKind::kNotesDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueC64th;
            break;
          case msrNotesDurationKind::kNotesDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueC32nd;
            break;
          case msrNotesDurationKind::kNotesDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueC16th;
            break;
          case msrNotesDurationKind::kNotesDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueC8th;
            break;
          case msrNotesDurationKind::kNotesDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueCQuarter;
            break;
          case msrNotesDurationKind::kNotesDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueCHalf;
            break;
          case msrNotesDurationKind::kNotesDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueCWhole;
            break;
          case msrNotesDurationKind::kNotesDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueCBreve;
            break;
          case msrNotesDurationKind::kNotesDurationLonga:
            break;
          case msrNotesDurationKind::kNotesDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchD:
        switch (noteGraphicNotesDurationKind) {
          case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
            break;

          case msrNotesDurationKind::kNotesDuration1024th:
            break;
          case msrNotesDurationKind::kNotesDuration512th:
            break;
          case msrNotesDurationKind::kNotesDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueD256th;
            break;
          case msrNotesDurationKind::kNotesDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueD128th;
            break;
          case msrNotesDurationKind::kNotesDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueD64th;
            break;
          case msrNotesDurationKind::kNotesDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueD32nd;
            break;
          case msrNotesDurationKind::kNotesDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueD16th;
            break;
          case msrNotesDurationKind::kNotesDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueD8th;
            break;
          case msrNotesDurationKind::kNotesDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueDQuarter;
            break;
          case msrNotesDurationKind::kNotesDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueDHalf;
            break;
          case msrNotesDurationKind::kNotesDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueDWhole;
            break;
          case msrNotesDurationKind::kNotesDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueDBreve;
            break;
          case msrNotesDurationKind::kNotesDurationLonga:
            break;
          case msrNotesDurationKind::kNotesDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchE:
        switch (noteGraphicNotesDurationKind) {
          case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
            break;

          case msrNotesDurationKind::kNotesDuration1024th:
            break;
          case msrNotesDurationKind::kNotesDuration512th:
            break;
          case msrNotesDurationKind::kNotesDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueE256th;
            break;
          case msrNotesDurationKind::kNotesDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueE128th;
            break;
          case msrNotesDurationKind::kNotesDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueE64th;
            break;
          case msrNotesDurationKind::kNotesDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueE32nd;
            break;
          case msrNotesDurationKind::kNotesDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueE16th;
            break;
          case msrNotesDurationKind::kNotesDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueE8th;
            break;
          case msrNotesDurationKind::kNotesDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueEQuarter;
            break;
          case msrNotesDurationKind::kNotesDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueEHalf;
            break;
          case msrNotesDurationKind::kNotesDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueEWhole;
            break;
          case msrNotesDurationKind::kNotesDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueEBreve;
            break;
          case msrNotesDurationKind::kNotesDurationLonga:
            break;
          case msrNotesDurationKind::kNotesDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchF:
        switch (noteGraphicNotesDurationKind) {
          case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
            break;

          case msrNotesDurationKind::kNotesDuration1024th:
            break;
          case msrNotesDurationKind::kNotesDuration512th:
            break;
          case msrNotesDurationKind::kNotesDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueF256th;
            break;
          case msrNotesDurationKind::kNotesDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueF128th;
            break;
          case msrNotesDurationKind::kNotesDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueF64th;
            break;
          case msrNotesDurationKind::kNotesDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueF32nd;
            break;
          case msrNotesDurationKind::kNotesDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueF16th;
            break;
          case msrNotesDurationKind::kNotesDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueF8th;
            break;
          case msrNotesDurationKind::kNotesDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueFQuarter;
            break;
          case msrNotesDurationKind::kNotesDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueFHalf;
            break;
          case msrNotesDurationKind::kNotesDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueFWhole;
            break;
          case msrNotesDurationKind::kNotesDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueFBreve;
            break;
          case msrNotesDurationKind::kNotesDurationLonga:
            break;
          case msrNotesDurationKind::kNotesDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchG:
        switch (noteGraphicNotesDurationKind) {
          case msrNotesDurationKind::kNotesDuration_UNKNOWN_:
            break;

          case msrNotesDurationKind::kNotesDuration1024th:
            break;
          case msrNotesDurationKind::kNotesDuration512th:
            break;
          case msrNotesDurationKind::kNotesDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueG256th;
            break;
          case msrNotesDurationKind::kNotesDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueG128th;
            break;
          case msrNotesDurationKind::kNotesDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueG64th;
            break;
          case msrNotesDurationKind::kNotesDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueG32nd;
            break;
          case msrNotesDurationKind::kNotesDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueG16th;
            break;
          case msrNotesDurationKind::kNotesDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueG8th;
            break;
          case msrNotesDurationKind::kNotesDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueGQuarter;
            break;
          case msrNotesDurationKind::kNotesDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueGHalf;
            break;
          case msrNotesDurationKind::kNotesDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueGWhole;
            break;
          case msrNotesDurationKind::kNotesDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueGBreve;
            break;
          case msrNotesDurationKind::kNotesDurationLonga:
            break;
          case msrNotesDurationKind::kNotesDurationMaxima:
            break;
        } // switch
        break;
    } // switch
  }

  // is the note octave needed?
  bsrNoteOctaveIsNeeded
    noteOctaveIsNeeded =
      bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededNo;

  if (fCurrentNoteOctaveKind == bsrNoteOctaveKind::kNoteOctaveNone) {
    // this note is the first one in the voice,
    // or a preceding braille element forces the octave sign for it
    // unless it is a rest
    if (note->fetchNoteIsARest ()) {
      noteOctaveIsNeeded =
        bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededNo;
    }
    else {
      noteOctaveIsNeeded =
        bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededYes;
    }
  }
  else if (fRelativeOctaveReference) {
    // analyye relationship to relative octave reference
    noteOctaveIsNeeded =
      brailleOctaveMarkIfNeeded (note);
  }

  // register note's octave kind
  fCurrentNoteOctaveKind = noteOctaveKind;

  // is there an accidental attached to the note?
  bsrAccidentalKind
    bAccidentalKind =
      bsrAccidentalKind::kAccidentalNone;

  switch (mAccidentalKind) {
    case msrAccidentalKind::kAccidentalNone:
      break;

    case msrAccidentalKind::kAccidentalSharp:
      bAccidentalKind = bsrAccidentalKind::kAccidentalSharp;
      break;
    case msrAccidentalKind::kAccidentalNatural:
      bAccidentalKind = bsrAccidentalKind::kAccidentalNatural;
      break;
    case msrAccidentalKind::kAccidentalFlat:
      bAccidentalKind = bsrAccidentalKind::kAccidentalFlat;
      break;

    case msrAccidentalKind::kAccidentalDoubleSharp:
      bAccidentalKind = bsrAccidentalKind::kAccidentalDoubleSharp;
      break;
    case msrAccidentalKind::kAccidentalSharpSharp:
      break;

    case msrAccidentalKind::kAccidentalFlatFlat:
      bAccidentalKind = bsrAccidentalKind::kAccidentalDoubleFlat;
      break;
    case msrAccidentalKind::kAccidentalNaturalSharp:
      break;
    case msrAccidentalKind::kAccidentalNaturalFlat:
      break;

    case msrAccidentalKind::kAccidentalQuarterFlat:
      bAccidentalKind = bsrAccidentalKind::kAccidentalDoubleFlat;
      break;
    case msrAccidentalKind::kAccidentalQuarterSharp:
      bAccidentalKind = bsrAccidentalKind::kAccidentalQuarterSharp;
      break;

    case msrAccidentalKind::kAccidentalThreeQuartersFlat:
      bAccidentalKind = bsrAccidentalKind::kAccidentalThreeQuarterFlat;
      break;
    case msrAccidentalKind::kAccidentalThreeQuartersSharp:
      bAccidentalKind = bsrAccidentalKind::kAccidentalThreeQuarterSharp;
      break;

    case msrAccidentalKind::kAccidentalSharpDown:
      break;
    case msrAccidentalKind::kAccidentalSharpUp:
      break;
    case msrAccidentalKind::kAccidentalNaturalDown:
      break;
    case msrAccidentalKind::kAccidentalNaturalUp:
      break;
    case msrAccidentalKind::kAccidentalFlatDown:
      break;
    case msrAccidentalKind::kAccidentalFlatUp:
      break;
    case msrAccidentalKind::kAccidentalTripleSharp:
      break;
    case msrAccidentalKind::kAccidentalTripleFlat:
      break;
    case msrAccidentalKind::kAccidentalSlashQuarterSharp:
      break;
    case msrAccidentalKind::kAccidentalSlashSharp:
      break;
    case msrAccidentalKind::kAccidentalSlashFlat:
      break;
    case msrAccidentalKind::kAccidentalDoubleSlashFlat:
      break;
    case msrAccidentalKind::kAccidentalSharp_1:
      break;
    case msrAccidentalKind::kAccidentalSharp_2:
      break;
    case msrAccidentalKind::kAccidentalSharp_3:
      break;
    case msrAccidentalKind::kAccidentalSharp_5:
      break;
    case msrAccidentalKind::kAccidentalFlat_1:
      break;
    case msrAccidentalKind::kAccidentalFlat_2:
      break;
    case msrAccidentalKind::kAccidentalFlat_3:
      break;
    case msrAccidentalKind::kAccidentalFlat_4:
      break;
    case msrAccidentalKind::kAccidentalSori:
      break;
    case msrAccidentalKind::kAccidentalKoron:

      break;
    case msrAccidentalKind::kAccidentalOther:
      break;
  } // switch

  // create the note
  S_bsrNote bNote =
    bsrNote::create (
      inputLineNumber,
      noteValueKind,
      noteDotsNumber,
      noteOctaveKind,
      noteOctaveIsNeeded,
      bAccidentalKind);

  // append it to the current measure
  fCurrentMeasure->
    appendNoteToMeasure (bNote);

  // determine the note value size kind
  bsrNoteValueSizeKind
    noteValueSizeKind =
      bsrNoteValueSizeKindFromNoteValueKind (
        noteValueKind);

  // is a note value sign needed?
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    std::stringstream ss;

    ss <<
      "--> fCurrentNoteValueSizeKind: " <<
      bsrNoteValueSizeKindAsString (fCurrentNoteValueSizeKind) <<
      ", noteValueSizeKind: " <<
      bsrNoteValueSizeKindAsString (noteValueSizeKind) <<
      ", noteOctaveIsNeeded: " <<
      bsrNoteOctaveIsNeededAsString (noteOctaveIsNeeded) <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (false && noteValueSizeKind != fCurrentNoteValueSizeKind) { // JMI
    gLog <<
      "--> note: '" <<
      bNote->asShortString () <<
      "', needs a note value size sign" <<
      std::endl;

    // set the note value size kind as needed
    bNote->setNoteValueSizeIsNeeded ();
    // register new note value size kind
    fCurrentNoteValueSizeKind = noteValueSizeKind;
  }
}

void msr2bsrTranslator::visitStart (S_msrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputStartLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      '\'' <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  createBsrForNote (elt);

  fRelativeOctaveReference = elt;
}

void msr2bsrTranslator::visitEnd (S_msrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  int inputLineNumber =
    elt->getInputEndLineNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrDynamic" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the dynamic
  S_bsrDynamic
    dynamic =
      bsrDynamic::create (
        elt->getInputStartLineNumber (),
        elt->getDynamicKind ());

  // append it to the current measure
  fCurrentMeasure->
    appendDynamicToMeasure (dynamic);
}

void msr2bsrTranslator::visitEnd (S_msrDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrDynamic" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

/*
//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrIdentification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  fCurrentIdentification =
    fResultingBsr->
      getMsrScore ()->
        getIdentification ();

  fOnGoingIdentification = true;
}

void msr2bsrTranslator::visitEnd (S_msrIdentification& elt)
{
  fOnGoingIdentification = false;

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}
*/

/*
//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrCredit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentCredit = elt;

  // set elt as credit of the MSR score part of the BSR score
  fResultingBsr->
    getMsrScore ()->
      appendCreditToScore (fCurrentCredit);
}

void msr2bsrTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrCredit" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentCredit = nullptr;
}

void msr2bsrTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // don't append it to the current credit, since the latter is no clone
  / * JMI
  fCurrentCredit->
    appendCreditWordsToCredit (
      elt);
      * /
}

void msr2bsrTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrCreditWords" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSegment& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << '\'' <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/ * JMI

  // fetch the current segment clone
  fCurrentSegmentClone =
    fCurrentVoiceClone->
      getVoiceLastSegment ();
      * /

  // create a clone of the segment
  S_msrSegment
    segmentClone =
      elt->createSegmentNewbornClone (
        fCurrentVoiceClone);

  // push it onto the segment clones stack
  fCurrentSegmentClonesStack.push_front (
    segmentClone);

  // append it to the current voice
  fCurrentVoiceClone->
    appendSegmentToVoiceClone (
      segmentClone);
}

void msr2bsrTranslator::visitEnd (S_msrSegment& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << '\'' <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget current segment clone
  fCurrentSegmentClonesStack.pop_front ();
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrHarmony '" <<
      elt->asString () <<
      ", fOnGoingNote: " << fOnGoingNote <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a harmony newborn clone
  fCurrentHarmonyClone =
    elt->createHarmonyNewbornClone (
      fCurrentVoiceClone);

  if (fOnGoingNote) {
    // register this note as the harmony note upLink
    harmony->
      setHarmonyUpLinkToNote (fOnGoingNote);

    // register the harmony in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendHarmonyToNote (
        fCurrentHarmonyClone);

  // don't append the harmony to the part harmony,
  // this has been done in pass2b
  }

  else if (fOnGoingChord) {
    // register the harmony in the current chord clone
    fCurrentChordClone->
      appendHarmonyToChord (fCurrentHarmonyClone); // JMI
  }

  else if (fOnGoingHarmoniesVoice) {
    fCurrentVoiceClone->
      appendHarmonyToVoiceClone (
        fCurrentHarmonyClone);
  }
}

void msr2bsrTranslator::visitStart (S_msrHarmonyDegree& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting S_msrHarmonyDegree '" <<
      elt->asString () <<
      ", fOnGoingNote: " << fOnGoingNote <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      "', line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the harmony degree to the current harmony clone
  fCurrentHarmonyClone->
    appendHarmonyDegreeToHarmony (
      elt);
}

void msr2bsrTranslator::visitEnd (S_msrHarmony& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrHarmony '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrFrame& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrFrame '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    // register the frame in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      setNoteFrame (elt);
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a deep clone of the figured bass
  fCurrentFiguredBass =
    elt->
      createFiguredBassDeepClone (
        fCurrentPartClone);

  if (fOnGoingNote) {
    // append the figured bass to the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendFiguredBassToNote (fCurrentFiguredBassClone);

    // don't append the figured bass to the part figured bass,  JMI ???
    // this will be done below
  }

  else if (fOnGoingChord) {
    // register the figured bass in the current chord clone
    fCurrentChordClone->
      setChordFiguredBass (fCurrentFiguredBass); // JMI
  }

  else if (fOnGoingFiguredBassVoice) { // JMI
    // register the figured bass in the part clone figured bass
    fCurrentPartClone->
      appendFiguredBassToPartClone (
        fCurrentVoiceClone,
        fCurrentFiguredBass);
  }
}

void msr2bsrTranslator::visitStart (S_msrBassFigure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBassFigure '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the bass figure to the current figured bass
  fCurrentFiguredBass->
    appendFigureToFiguredBass (
      elt);
}

void msr2bsrTranslator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrFiguredBass '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentFiguredBass = nullptr;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputStartLineNumber ();

  fCurrentMeasureNumber =
    elt->getMeasureNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasure '" <<
      fCurrentMeasureNumber <<
      '\'' <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/ * JMI
  {
    gLog <<
      std::endl <<
      elt <<
      std::endl;
  }
     * /

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<!--=== " <<
      "part \"" << fCurrentPartName << "\"" <<
      " (partID \"" << fCurrentPart->getPartID () << "\")" <<
      ", measure \"" << fCurrentMeasureNumber << "\"" <<
      "', voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // measure 1 is created by default initially ??? JMI

  // create a clone of the measure
  fCurrentMeasureClone =
    elt->
      createMeasureNewbornClone (
        fCurrentSegmentClonesStack.front ());

  // append it to the current segment clone
  fCurrentSegmentClonesStack.front ()->
    appendMeasureToSegment (
      fCurrentMeasureClone);

// JMI utile???
  fCurrentPartClone->
    setPartCurrentMeasureNumber (
      fCurrentMeasureNumber);

  // should the last bar check's measure be set?
  if (fLastBarCheck) {
    fLastBarCheck->
      setNextBarNumber (
        fCurrentMeasureNumber);

    fLastBarCheck = nullptr;
  }
}

void msr2bsrTranslator::finalizeCurrentMeasureClone (
  int          inputLineNumber,
  S_msrMeasure originalMeasure)
{
  // take this measure into account
  ++fMeasuresCounter;

  // fetch the voice
  S_msrVoice
    voice =
      fCurrentMeasureClone->
        fetchMeasureUpLinkToVoice ();

  // get the measure number
  std::string
    measureNumber =
      fCurrentMeasureClone->
        getMeasureNumber ();

  // get the measure whole notes
  mfRational
    measureCurrentAccumulatedWholeNotesDuration =
      fCurrentMeasureClone->
        getMeasureCurrentAccumulatedWholeNotesDuration ();

  // get the full measure whole notes
  mfRational
    measureFullLength =
      fCurrentMeasureClone->
        getMeasureFullLength ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Finalizing measure '" << measureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl <<
      "measureCurrentAccumulatedWholeNotesDuration: " << measureCurrentAccumulatedWholeNotesDuration.asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  msrMeasureKind
    measureKind =
      msrMeasure::kUnknownMeasure; // JMI
 // JMI     fMeasureKind = kFullMeasure; // may be changed afterwards

  if (measureCurrentAccumulatedWholeNotesDuration == measureFullLength ) {
    // this measure is full
    measureKind =
      msrMeasure::kFullMeasure;
  }

  else if (measureCurrentAccumulatedWholeNotesDuration < measureFullLength) {
    / *
    if (fSegmentElementsList.size () == 1) { // JMI
      // this is the first measure in the segment
      measureKind =
        msrMeasure::kIncompleteLeftMeasure;
    }

    else {
      // this is the last measure in the segment
      measureKind =
        msrMeasure::kIncompleteRightMeasure;
    }
    * /

    // this measure is an up beat
    measureKind =
      msrMeasure::kUpbeatMeasure; // JMI
  }

  else if (measureCurrentAccumulatedWholeNotesDuration > measureFullLength) {
    // this measure is overfull
    measureKind =
      msrMeasure::kOverfullMeasure;
  }

  if (false && / * JMI * / measureKind != originalMeasure->getMeasureKind ()) { // JMI
    std::stringstream ss;

    ss <<
      "line " << inputLineNumber << ":" <<
      " clone measure:" <<
      std::endl <<
      fCurrentMeasureClone <<
      std::endl <<
      "differs for measure kind from original measure:" <<
      std::endl <<
      originalMeasure;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }
}

void msr2bsrTranslator::visitEnd (S_msrMeasure& elt)
{
  int inputLineNumber =
    elt->getInputEndLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasure '" <<
      elt->getMeasureNumber () <<
      '\'' <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string
    measureNumber =
      elt->getMeasureNumber ();

  finalizeCurrentMeasureClone ( // JMI
    inputLineNumber,
    elt); // original measure
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStanza& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

//  if (elt->getStanzaTextPresent ()) { // JMI
    fCurrentStanzaClone =
      elt->createStanzaNewbornClone (
        fCurrentVoiceClone);

    // append the stanza clone to the BSR score elements list
    fResultingBsr ->
      appendStanzaToScoreElements (
        fCurrentStanzaClone);

    // append a use of the stanza to the current staff block
    fCurrentStaffBlock ->
      appendLyricsUseToStaffBlock (
        fCurrentStanzaClone);
//  }
//  else
  //  fCurrentStanzaClone = 0; // JMI

  fOnGoingStanza = true;
}

void msr2bsrTranslator::visitEnd (S_msrStanza& elt)
{
  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about this stanza
  fCurrentStanzaClone = nullptr;

  fOnGoingStanza = false;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSyllable& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSyllable" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the syllable clone
  fCurrentSyllableClone =
    elt->createSyllableNewbornClone (
      fCurrentPartClone);

  // add it to the current stanza clone or current note clone
  if (fOnGoingStanza) { // fCurrentStanzaClone JM
    // visiting a syllable as a stanza member
    fCurrentStanzaClone->
      appendSyllableToStanza (
        fCurrentSyllableClone);
  }

  else if (fOnGoingNote) { // JMI
    // visiting a syllable as attached to the current non-grace note
    fCurrentSyllableClone->
      appendSyllableToNoteAndSetItsUpLinkToNote (
        fCurrentNonGraceNoteClone);

    if (gBsrOahGroup->getAddMsrWordsFromTheMusicXMLLyrics ()) {
      // get the syllable texts list
      const std::list<std::string>&
        syllableTextsList =
          elt->getSyllableTextsList ();

      if (syllableTextsList.size ()) {
        // build a single words value from the texts list
        // JMI create an msrWords instance for each???
        std::string wordsValue =
          elt->syllableTextsListAsString();

        // create the words
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceLyrics () || gTraceOahGroup->getTraceWords ()) {
          gLog <<
            "Changing lyrics '" <<
            wordsValue <<
            "' into words for note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            '\'' <<
      // JMI      fCurrentSyllableClone->asString ();

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        S_msrWords
          words =
            msrWords::create (
              inputLineNumber,
              msrPlacementKind::kPlacement_UNKNOWN_,                // default value
              wordsValue,
              msrJustifyKind::kJustifyNone,                  // default value
              msrVerticalAlignmentKind::kVerticalAlignmentNone,        // default value
              msrFontStyleKind::kFontStyleNone,                // default value
              msrFontSize::create (
                msrFontSizeKind::kFontSizeNone), // default value
              msrFontWeightKind::kFontWeightNone,               // default value
              msrWords::kItLang);            // default value

        // append it to the current non-grace note
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceLyrics () || gTraceOahGroup->getTraceWords ()) {
          gLog <<
            "Appending words '" <<
            words->asShortString () <<
            "' to note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "'";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        fCurrentNonGraceNoteClone->
          appendWordsToNote (
            words);
      }
    }
  }

  // a syllable ends the sysllable extend range if any
  if (fOnGoingSyllableExtend) {
    / * JMI ???
    // create melisma end command
    S_bsrMelismaCommand
      melismaCommand =
        bsrMelismaCommand::create (
          inputLineNumber,
          bsrMelismaCommand::kMelismaEnd);

    // append it to current voice clone
    fCurrentVoiceClone->
      appendOtherElementToVoice (melismaCommand);
* /

    fOnGoingSyllableExtend = false;
  }
}

void msr2bsrTranslator::visitEnd (S_msrSyllable& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSyllable" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}
*/

/*
//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTransposition& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTransposition" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append transpose to voice clone
  fCurrentVoiceClone->
    appendTranspositionToVoice (elt);
}

void msr2bsrTranslator::visitEnd (S_msrTransposition& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTransposition" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrRehearsalMark& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRehearsalMark" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendRehearsalMarkToVoice (elt);
}

void msr2bsrTranslator::visitEnd (S_msrRehearsalMark& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRehearsalMark" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrArticulation" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrArticulation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrArticulation" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrFermata& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrFermata" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // a fermata is an articulation

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrArpeggiato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // an arpeggiato is an articulation

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrNonArpeggiato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // an nonArpeggiato is an articulation

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTechnical" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalToChord (elt);
  }

  // doest the score need the 'tongue' function?
  switch (elt->getTechnicalKind ()) {
    case msrTechnicalKind::kTechnicalArrow:
      break;
    case msrTechnicalKind::kTechnicalDoubleTongue:
      fResultingBsr->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnicalKind::kTechnicalDownBow:
      break;
    case msrTechnicalKind::kTechnicalFingernails:
      break;
    case msrTechnicalKind::kTechnicalHarmonic:
      break;
    case msrTechnicalKind::kTechnicalHeel:
      break;
    case msrTechnicalKind::kTechnicalHole:
      break;
    case msrTechnicalKind::kTechnicalOpenString:
      break;
    case msrTechnicalKind::kTechnicalSnapPizzicato:
      break;
    case msrTechnicalKind::kTechnicalStopped:
      break;
    case msrTechnicalKind::kTechnicalTap:
      break;
    case msrTechnicalKind::kTechnicalThumbPosition:
      break;
    case msrTechnicalKind::kTechnicalToe:
      break;
    case msrTechnicalKind::kTechnicalTripleTongue:
      fResultingBsr->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnicalKind::kTechnicalUpBow:
      break;
  } // switch
}

void msr2bsrTranslator::visitEnd (S_msrTechnical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTechnical" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithIntegerToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithIntegerToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithFloatToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithFloatToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithStringToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithStringToChord (elt);
  }

  switch (elt->getTechnicalWithStringKind ()) {
    case msrTechnicalWithStringKind::kHammerOn:
    case msrTechnicalWithStringKind::kPullOff:
      // this score needs the 'after' Scheme function
      fResultingBsr->
        setAfterSchemeFunctionIsNeeded ();
      break;
    default:
      ;
  } // switch
}

void msr2bsrTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrOrnament" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendOrnamentToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOrnamentToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrOrnament& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrOrnament" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSpanner& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSpanner" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (elt->getSpannerTypeKind ()) {
    case msrSpannerTypeKind::kSpannerTypeStart:
      break;
    case msrSpannerTypeKind::kSpannerTypeStop:
      break;
    case msrSpannerTypeKind::kSpannerTypeContinue:
      break;
    case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
      break;
  } // switch

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendSpannerToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSpannerToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrSpanner& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSpanner" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrGlissando" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendGlissandoToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendGlissandoToChord (elt);
  }

  if (elt->getGlissandoTextValue ().size ()) {
    fResultingBsr->
      // this score needs the 'glissandoWithText' Scheme function
      addGlissandoWithTextSchemeFunctionsToScore ();
  }
}

void msr2bsrTranslator::visitEnd (S_msrGlissando& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrGlissando" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSlide& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSlide" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendSlideToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlideToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrSlide& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSlide" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      setNoteSingleTremolo (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordSingleTremolo (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrSingleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a double tremolo clone from the two elements
  fCurrentDoubleTremoloClone = elt; // JMI FIX THAT
/ * JMI
    elt->createDoubleTremoloNewbornClone (
      elt->getDoubleTremoloFirstElement ()->
        createNewBornClone (),
      elt->getDoubleTremoloSecondElement ()
        createNewBornClone ());
        * /

  fOnGoingDoubleTremolo = true;
}

void msr2bsrTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the current double tremolo clone to the current voice clone
  fCurrentVoiceClone->
    appendDoubleTremoloToVoice (
      fCurrentDoubleTremoloClone);

  // forget about it
  fCurrentDoubleTremoloClone = nullptr;

  fOnGoingDoubleTremolo = false;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrOtherDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrOtherDynamic" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendOtherDynamicToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOtherDynamicToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrOtherDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrOtherDynamic" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrWords" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendWordsToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWordsToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrWords" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSlur& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSlur" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  / *
    Only the  first note of the chord should get the slur notation.
    Some applications print out the slur for all notes,
    i.e. a stop and a start in sequqnce:
    these should be ignored
  * /

  if (fOnGoingNote) {
    // don't add slurs to chord member notes except the first one
    switch (fCurrentNonGraceNoteClone->getNoteKind ()) {
      case msrNoteKind::kNoteRegularInChord:
        if (fCurrentNonGraceNoteClone->getNoteIsAChordsFirstMemberNote ()) {
          fCurrentNonGraceNoteClone->
            appendSlurToNote (elt);
        }
        break;

      default:
        fCurrentNonGraceNoteClone->
          appendSlurToNote (elt);
    } // switch
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlurToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrSlur& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSlur" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrLigature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrLigature" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendLigatureToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendLigatureToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrLigature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrLigature" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSlash& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSlash" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendSlashToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlashToChord (elt);
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrCrescDecresc& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCrescDecresc" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendCrescDecrescToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWedgeToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrCrescDecresc& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrCrescDecresc" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrWedge& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrWedge" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendWedgeToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWedgeToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrWedge& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrWedge" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber () ;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrGraceNotesGroup" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  Bool doCreateAGraceNoteClone (true); // JMI

  if (doCreateAGraceNoteClone) {
    // create a clone of this graceNotesGroup
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceGraceNotes ()) {
      gLog <<
        "Creating a clone of grace notes group " <<
        elt->asShortString () <<
        " and attaching it to clone note " <<
        fCurrentNonGraceNoteClone->asShortString () <<
        std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

    fCurrentGraceNotesGroupClone =
      elt->
        createGraceNotesGroupNewbornClone ();

    // attach it to the current note clone
    // if (fOnGoingNote) { JMI
   // { // JMI

    switch (elt->getGraceNotesGroupKind ()) {
      case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
        fCurrentNonGraceNoteClone->
          setNoteGraceNotesGroupBefore (
            fCurrentGraceNotesGroupClone);
        break;
      case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
        fCurrentNonGraceNoteClone->
          setNoteGraceNotesGroupAfter (
            fCurrentGraceNotesGroupClone);
        break;
    } // switch
  //  }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceGraceNotes ()) {
    gLog <<
      "+++++++++++++++++++++++++ 1" <<
      std::endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      gLog <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLog <<
        "[NONE]";
    }
    gLog << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get the note this grace notes group is attached to
  S_msrNote
    noteNotesGroupIsAttachedTo =
      elt->
        getGraceNotesGroupUpLinkToNote ();

  if (! noteNotesGroupIsAttachedTo) {
    std::stringstream ss;

    ss <<
      "grace notes group '" << elt->asShortString () <<
      "' has an empty note upLink";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fOnGoingGraceNotesGroup = true;

  // is noteNotesGroupIsAttachedTo the first one in its voice?
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceGraceNotes ()
      ||
    gTraceOahGroup->getTraceNotes ()
      ||
    gTraceOahGroup->getTraceVoices ()
  ) {
    std::stringstream ss;

    ss <<
      "The noteNotesGroupIsAttachedTo voice clone FIRST_ONE??? '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (noteNotesGroupIsAttachedTo) {
      ss <<
        noteNotesGroupIsAttachedTo->asShortString ();
    }
    else {
      ss <<
        "[NONE]";
    }
    gLog <<
       "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceGraceNotes ()
      ||
    gTraceOahGroup->getTraceNotes ()
      ||
    gTraceOahGroup->getTraceVoices ()
  ) {
    std::stringstream ss;

    ss <<
      "The first note of voice clone KLJWLPOEF '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (fFirstNoteCloneInVoice) {
      ss <<
        fFirstNoteCloneInVoice->asShortString ();
    }
    else {
      ss <<
        "[NONE]";
    }
    ss <<
       "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch the original voice first non grace note
  S_msrNote
    originalVoiceFirstNonGraceNote =
      fCurrentVoiceOriginal->
        fetchVoiceFirstNonGraceNote ();

  if (originalVoiceFirstNonGraceNote) {
    if (noteNotesGroupIsAttachedTo == originalVoiceFirstNonGraceNote) {
      // bug 34 in LilyPond should be worked around by creating
      // skip grace notes in the other voices of the part

      // create the skip grace notes group
#ifdef MF_TRACE_IS_ENABLED
        if (
            gTraceOahGroup->getTraceGraceNotes ()
              ||
            gTraceOahGroup->getTraceNotes ()
              ||
            gTraceOahGroup->getTraceVoices ()
        ) {
          std::stringstream ss;

          ss <<
            "Creating a skip clone of grace notes group " <<
            elt->asShortString () <<
            " to work around LilyPond issue #34";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

      fCurrentSkipGraceNotesGroup =
        elt->
          createSkipGraceNotesGroupClone ();
    }
  }

  // addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded() will
  // append the same skip grace notes to the ofhter voices if needed
  // in visitEnd (S_msrPart&)
}

    / * JMI
  if (fFirstNoteCloneInVoice) {
    // there is at least a note before these grace notes in the voice

    if (
      fCurrentNonGraceNoteClone->getNoteTrillOrnament ()
        &&
      fCurrentNonGraceNoteClone->getNoteIsFollowedByGraceNotesGroup ()) {
      // fPendingAfterGraceNotesGroup already contains
      // the afterGraceNotesGroup to use

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceGraceNotes ()Group) {
        gLog <<
          "Optimising grace notes '" <<
          elt->asShortString () <<
          "' into after grace notes" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // attach the current after grace notes clone to the current note clone
      if (fOnGoingNote) { // JMI
        fCurrentNonGraceNoteClone->
          setNoteAfterGraceNotesGroup (
            fPendingAfterGraceNotesGroup);
      }

      doCreateAGraceNoteClone = false;
    }
  }

  if (doCreateAGraceNoteClone) {
    // are these grace notes the last element in a measure?
    if (elt->getGraceNotesGroupIsFollowedByNotes ()) {
      // yes, this is a regular grace notes

      // create a clone of this graceNotesGroup
      fCurrentGraceNotesGroupClone =
        elt->
          createGraceNotesGroupNewbornClone ();

      // attach it to the current note clone
      if (fOnGoingNote) { // JMI
        fCurrentNonGraceNoteClone->
          setNoteGraceNotesGroup (
            fCurrentGraceNotesGroupClone);
      }

     // JMI XXL find good criterion for this

      // these grace notes are at the beginning of a segment JMI
  //    doCreateAGraceNoteClone (true); // JMI

      // bug 34 in LilyPond should be worked aroud by creating
      // skip grace notes in the other voices of the part

      // create skip graceNotesGroup clone
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceGraceNotes ()Group) {
        gLog <<
          "Creating a skip clone of grace notes '" <<
          elt->asShortString () <<
          "' to work around LilyPond issue #34" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      S_msrGraceNotesGroup
        skipGraceNotesGroup =
          elt->
            createSkipGraceNotesGroupClone ();

      // prepend it to the other voices in the part
      fCurrentPartClone->
        prependSkipGraceNotesGroupToVoicesClones (
          fCurrentVoiceClone,
          skipGraceNotesGroup);
    }

    else {
      // no, we should build an msrAfterGraceNotesGroup from this
      // and the last element in the current voice clone

      // fetch the voice last element
      fCurrentAfterGraceNotesGroupElement =
        fCurrentVoiceClone->
          fetchVoiceLastMeasureElement (inputLineNumber);

      // create the after grace notes
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceGraceNotes ()Group) {
        gLog <<
          "Converting grace notes '" <<
          elt->asShortString () <<
          "' into after grace notes attached to:" <<
          std::endl;

        ++gIndenter;

        fCurrentAfterGraceNotesGroupElement->
          print (gLog);

        --gIndenter;
      }
#endif // MF_TRACE_IS_ENABLED

      fPendingAfterGraceNotesGroup =
        msrAfterGraceNotesGroup::create (
          inputLineNumber,
            fCurrentAfterGraceNotesGroupElement,
            elt->getGraceNotesGroupIsSlashed (),
            fCurrentVoiceClone);

      // append it to the current note clone
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceGraceNotes ()Group) {
        gLog <<
          "Appending the after grace notes to current note clone" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      if (fOnGoingNote) { // JMI
        fCurrentNonGraceNoteClone->
          setNoteAfterGraceNotesGroup (
            fPendingAfterGraceNotesGroup);
      }
    }
  }
*/

/*

void msr2bsrTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrGraceNotesGroup" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceGraceNotes ()) {
    gLog <<
      "+++++++++++++++++++++++++ 2" <<
      std::endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      gLog <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLog <<
        "[NONE]";
    }
    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about these grace notes
  fCurrentGraceNotesGroupClone = nullptr;

  fOnGoingGraceNotesGroup = false;

/ * JMI
  if (fPendingAfterGraceNotesGroup) {
    // remove the current afterGraceNotesGroup note clone
    // from the current voice clone
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()Group) {
      gLog <<
        "Removing the after grace notes element from the current voice clone";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentVoiceClone->
      removeElementFromVoice (
        inputLineNumber,
        fCurrentAfterGraceNotesGroupElement);

    // forget about the current after grace notes element
    fCurrentAfterGraceNotesGroupElement = nullptr;

    // forget about these after the pending grace notes
    fPendingAfterGraceNotesGroup = nullptr;
  }
  * /
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the note clone
  S_msrNote
    noteClone =
      elt->createNoteNewbornClone (
        fCurrentPartClone);

  // register clone in this tranlastors' voice notes map
  fVoiceNotesMap [elt] = noteClone; // JMI XXL

  // don't register grace notes as the current note clone,
  // but as the current grace note clone instead
/ * JMI
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceVoices ()) {
          gLog <<
            "The first note of voice clone GFFF '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '";

          if (fFirstNoteCloneInVoice) {
            gLog <<
              fFirstNoteCloneInVoice->asShortString ();
          }
          else {
            gLog <<
              "[NONE]";
          }
          gLog <<
             "'";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
* /

  switch (elt->getNoteKind ()) {

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      fCurrentGraceNoteClone = noteClone;
      break;

    default:
      fCurrentNonGraceNoteClone = noteClone;

      if (! fFirstNoteCloneInVoice) {
        fFirstNoteCloneInVoice =
          fCurrentNonGraceNoteClone;

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceNotes () || gTraceOahGroup->getTraceVoices ()) {
          gLog <<
            "The first note of voice clone RJIRWR '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '" <<
            fFirstNoteCloneInVoice->asShortString () <<
             "'";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }

      fOnGoingNote = true;
  } // switch

/ * JMI
  // can we optimize graceNotesGroup into afterGraceNotesGroup?
  if (
    elt->getNoteIsFollowedByGraceNotesGroup ()
      &&
    elt->getNoteTrillOrnament ()) {
    // yes, create the after grace notes
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()Group) {
      gLog <<
        "Optimizing grace notes on trilled note '" <<
        elt->asShortString () <<
        "' as after grace notes " <<
        ", line " << inputLineNumber;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPendingAfterGraceNotesGroup =
      msrAfterGraceNotesGroup::create (
        inputLineNumber,
        fCurrentNonGraceNoteClone,
        false, // aftergracenoteIsSlashed, may be updated later
        fCurrentVoiceClone);

    // register current afterGraceNotesGroup element
    fCurrentAfterGraceNotesGroupElement =
      fCurrentNonGraceNoteClone;
  }
* /
}

void msr2bsrTranslator::visitEnd (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputEndLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    std::stringstream ss;

    ss <<
      "FAA fCurrentNonGraceNoteClone: " <<
      std::endl;
    if (fCurrentNonGraceNoteClone) {
      gLog <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLog <<
        "[NONE]" <<
        std::endl;
    }

    gLog <<
      "FAA fCurrentGraceNoteClone: " <<
      std::endl;
    if (fCurrentGraceNoteClone) {
      gLog <<
        fCurrentGraceNoteClone;
    }
    else {
      gLog <<
        "[NONE]" <<
        std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  switch (elt->getNoteKind ()) {

    case msrNoteKind::kNote_UNKNOWN_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        gLog <<
          "Appending rest note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteSkipInMeasure: // JMI
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        gLog <<
          "Appending skip note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        gLog <<
          "Appending unpitched note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteRegularInMeasure:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        gLog <<
          "Appending regular note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      if (fOnGoingDoubleTremolo) {

        if (fCurrentNonGraceNoteClone->getNoteIsFirstNoteInADoubleTremolo ()) {
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            gLog <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputEndLineNumber () <<
              ", as double tremolo first element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteFirstElement (
              fCurrentNonGraceNoteClone);
        }

        else if (fCurrentNonGraceNoteClone->getNoteIsSecondNoteInADoubleTremolo ()) {
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            gLog <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputEndLineNumber () <<
              ", as double tremolo second element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteSecondElement (
              fCurrentNonGraceNoteClone);
        }

        else {
          std::stringstream ss;

          ss <<
            "note '" << fCurrentNonGraceNoteClone->asShortString () <<
            "' belongs to a double tremolo, but is not marked as such";

          msrInternalError (
            gServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            ss.str ());
        }
      }

      else {
        std::stringstream ss;

        ss <<
          "double tremolo note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of a double tremolo";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    / * JMI
      gLog <<
        "fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        std::endl;
        * /

      if (! fOnGoingGraceNotesGroup) {
        std::stringstream ss;

        ss <<
          "grace note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of grace notes";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      else {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes () || gTraceOahGroup->getTraceNotes ()) {
          gLog <<
            "Appending grace note '" <<
            fCurrentGraceNoteClone->asShortString () <<
            " to the grace notes group " <<
            fCurrentGraceNotesGroupClone->asShortString () <<
            " in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentGraceNotesGroupClone->
          appendNoteToGraceNotesGroup (
            fCurrentGraceNoteClone);
      }

    / * JMI ???
      if (fCurrentGraceNotesGroupClone) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes () || gTraceOahGroup->getTraceNotes ()) {
          gLog <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentGraceNotesClone->
          appendNoteToGraceNotes (
            fCurrentNonGraceNoteClone);
      }

      else if (fPendingAfterGraceNotes) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes () || gTraceOahGroup->getTraceNotes ()) {
          gLog <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the after grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fPendingAfterGraceNotes->
          appendNoteToAfterGraceNotesContents (
            fCurrentNonGraceNoteClone);
      }

      else {
        std::stringstream ss;

        ss <<
          "both fCurrentGraceNoteGroupsClone and fPendingAfterGraceNoteGroup are null," <<
          std::endl <<
          "cannot handle grace note'" <<
          elt->asString () <<
          "'";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      * /
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fOnGoingChord) {
        fCurrentChordClone->
          addAnotherNoteToChord (
            fCurrentNonGraceNoteClone,
            fCurrentVoiceClone);
      }

      else {
        std::stringstream ss;

        ss <<
          "msr2bsrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
        }
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      if (fOnGoingChord) {
        fCurrentChordClone->
          addAnotherNoteToChord (
            fCurrentGraceNoteClone,
            fCurrentVoiceClone);
      }

      else {
        std::stringstream ss;

        ss <<
          "msr2bsrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
        }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        gLog <<
          "Appending note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "'' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fTupletClonesStack.front ()->
        appendNoteToTuplet (
          fCurrentNonGraceNoteClone,
          fCurrentVoiceClone);
      break;
  } // switch

  // handle editorial accidentals
  switch (fCurrentNonGraceNoteClone->getEditorialAccidentalKind ()) {
    case msrNote::msrEditorialAccidentalKind::kEditorialAccidentalYes:
      fResultingBsr->
        // this score needs the 'editorial accidental' Scheme function
        setEditorialAccidentalSchemeFunctionIsNeeded ();
      break;
    case msrNote::msrEditorialAccidentalKind::kEditorialAccidentalNo:
      break;
  } // switch

  // handle cautionary accidentals
  switch (fCurrentNonGraceNoteClone->getCautionaryAccidentalKind ()) {
    case msrNote::msrCautionaryAccidentalKind::kCautionaryAccidentalYes:
      break;
    case msrNote::msrCautionaryAccidentalKind::kCautionaryAccidentalNo:
      break;
  } // switch

/ * JMI
  // handle melisma
  msrSyllableExtendKind
    noteSyllableExtendKind =
      elt->getNoteSyllableExtendKind ();

  switch (noteSyllableExtendKind) {
    case msrSyllable::kStandaloneSyllableExtend:
      {
        / * JMI ??? v0.9.66
        // create melisma start command
        S_bsrMelismaCommand
          melismaCommand =
            bsrMelismaCommand::create (
              inputLineNumber,
              bsrMelismaCommand::kMelismaStart);

        // append it to current voice clone
        fCurrentVoiceClone->
          appendOtherElementToVoice (melismaCommand);

        // append
        * /

        fOnGoingSyllableExtend = true;
      }
      break;
    case msrSyllable::kStartSyllableExtend:
      break;
    case msrSyllable::kContinueSyllableExtend:
      break;
    case msrSyllable::kStopSyllableExtend:
      break;
    case msrSyllable::k_NoSyllableExtend:
      break;
  } // switch
* /

  fOnGoingNote = false;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrOctaveShift& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    setNoteOctaveShift (elt);
}

void msr2bsrTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrOctaveShift" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the accordion registration to the voice clone
  fCurrentVoiceClone->
    appendAccordionRegistrationToVoice (elt);

  // the generated code needs modules scm and accreg
  fResultingBsr->
    setScmAndAccregSchemeModulesAreNeeded ();
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrHarpPedalsTuning" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the harp pedals tuning to the voice clone
  fCurrentVoiceClone->
    appendHarpPedalsTuningToVoice (elt);
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStem& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStem" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      setNoteStem (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendStemToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrStem& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrStem" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrBeam& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBeam" <<
      ", line " << elt->getInputStartLineNumber () <<
      std::endl;

// JMI      ", fOnGoingNote: " << fOnGoingNote <<
// JMI      ", fOnGoingChord: " << fOnGoingChord;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // a beam may be present at the same time
  // in a note or grace note and the chord the latter belongs to

  if (fOnGoingGraceNotesGroup) {
    fCurrentGraceNoteClone->
      appendBeamToNote (elt);
  }
  else if (fOnGoingNote) {
    fCurrentNonGraceNoteClone->
      appendBeamToNote (elt);
  }

  if (fOnGoingChord) {
    fCurrentChordClone->
      appendBeamToChord (elt);
  }
}

void msr2bsrTranslator::visitEnd (S_msrBeam& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrBeam" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrChord" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentChordClone =
    elt->createChordNewbornClone (
      fCurrentPartClone);

  if (fTupletClonesStack.size ()) {
    // a chord in a tuplet is handled as part of the tuplet JMI
    fTupletClonesStack.front ()->
      appendChordToTuplet (
        fCurrentChordClone);
  }

  else if (fOnGoingDoubleTremolo) {
    if (elt->getChordIsFirstChordInADoubleTremolo ()) {
      // replace double tremolo's first element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordFirstElement (
          elt);
    }

    else if (elt->getChordIsSecondChordInADoubleTremolo ()) {
      // replace double tremolo's second element by chord
      fCurrentDoubleTremoloClone->
        setDoubleTremoloChordSecondElement (
          elt);
    }

    else {
      std::stringstream ss;

      ss <<
        "chord '" << elt->asString () <<
        "' belongs to a double tremolo, but is not marked as such";

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  else if (fCurrentGraceNotesGroupClone) {
    // append the chord to the grace notes
    fCurrentGraceNotesGroupClone->
      appendChordToGraceNotesGroup (
        fCurrentChordClone);
  }

  else {
    // appending the chord to the voice clone at once
    fCurrentVoiceClone->
      appendChordToVoice (
        fCurrentChordClone);
  }

  fOnGoingChord = true;
}

void msr2bsrTranslator::visitEnd (S_msrChord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTuplet" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the tuplet clone
  S_msrTuplet
    tupletClone =
      elt->createTupletNewbornClone ();

  // register it in this visitor
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "++> pushing tuplet '" <<
      tupletClone->asString () <<
      "' to tuplets stack";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletClonesStack.push_front (tupletClone);

  switch (elt->getTupletLineShapeKind ()) {
    case msrTupletLineShapeKind::kTupletLineShapeStraight:
    case msrTupletLineShapeKind::kTupletLineShapeCurved:
      fResultingBsr->
        // this score needs the 'tuplets curved brackets' Scheme function
        setTupletsCurvedBracketsSchemeFunctionIsNeeded ();
      break;
  } // switch
}

void msr2bsrTranslator::visitEnd (S_msrTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Popping tuplet '" <<
      elt->asString () <<
      "' from tuplets stack";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletClonesStack.pop_front ();

  if (fTupletClonesStack.size ()) {
    // tuplet is a nested tuplet
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      gLog <<
        "Adding nested tuplet '" <<
      elt->asString () <<
        "' to stack top tuplet '" <<
      fTupletClonesStack.front ()->asString () <<
      '\'' <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    fTupletClonesStack.front ()->
      appendTupletToTupletClone (elt);
  }

  else {
    // tuplet is a top level tuplet

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTuplets ()) {
      gLog <<
        "Adding top level tuplet '" <<
      elt->asString () <<
      "' to voice" <<
      fCurrentVoiceClone->getVoiceName () <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    fCurrentVoiceClone->
      appendTupletToVoice (elt);
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTie& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTie" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    setNoteTie (elt);
}

void msr2bsrTranslator::visitEnd (S_msrTie& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTie" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSegno& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSegno" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendSegnoToVoice (elt);
}

void msr2bsrTranslator::visitStart (S_msrCoda& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendCodaToVoice (elt);
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting eyeGlasses" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendEyeGlassesToNote (elt);
}

void msr2bsrTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting scordatura" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendScordaturaToNote (elt);
}

void msr2bsrTranslator::visitStart (S_msrPedal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting pedal" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendPedalToNote (elt);
}

void msr2bsrTranslator::visitStart (S_msrDamp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting damp" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendDampToNote (elt);

  fResultingBsr->
    // this score needs the 'custom short barLine' Scheme function
    setDampMarkupIsNeeded ();
}

void msr2bsrTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting dampAll" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentNonGraceNoteClone->
    appendDampAllToNote (elt);

  fResultingBsr->
    // this score needs the 'custom short barLine' Scheme function
    setDampAllMarkupIsNeeded ();
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBarCheck" <<
      ", nextBarNumber: " <<
      elt->getNextBarNumber () <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2bsrTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrBarCheck" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendBarNumberCheckToVoice (elt);
}

void msr2bsrTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeat" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/ * JMI
  gLog <<
    std::endl <<
    "*********** fCurrentPartClone" <<
    std::endl << std::endl;
  fCurrentPartClone->print (gLog);
  gLog <<
    "*********** fCurrentPartClone" <<
    std::endl << std::endl;
    * /

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling repeat start in part clone" <<
      fCurrentPartClone->getPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/ * JMI ???
  fCurrentPartClone->
    prepareForRepeatInPart (
      inputLineNumber);
      * /
  fCurrentVoiceClone->
    prepareForRepeatInVoiceClone (
      inputLineNumber,
      elt->getRepeatTimes ());

/ * JMI
  // create a repeat clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat newborn clone" <<
      ", line " << inputLineNumber <<
      ", in voice \"" <<
      elt->
        getRepeatUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentRepeatClone =
    elt->
      createRepeatNewbornClone (
        fCurrentVoiceClone);
* /
}

void msr2bsrTranslator::visitEnd (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputEndLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeat" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/ * JMI
  // append the repeat clone to the current part clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending a repeat to part clone " <<
      fCurrentPartClone->getPartCombinedName () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentPartClone-> // no test needed JMI
    appendRepeatCloneToPart (
      inputLineNumber,
      fCurrentRepeatClone);
* /

  // forget about current repeat clone // JMI
// JMI  fCurrentRepeatClone = 0;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceRepeats ()
      ||
    gTraceOahGroup->getTraceVoicesDetails ()
  ) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Upon visitStart (S_msrRepeatCommonPart&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    ss <<
      fCurrentVoiceClone;

    --gIndenter;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msr2bsrTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputEndLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceRepeats ()
      ||
    gTraceOahGroup->getTraceVoicesDetails ()
  ) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Upon visitEnd (S_msrRepeatCommonPart&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    ss <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // create a repeat and append it to voice clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending a repeat to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    createRepeatUponItsEndAndAppendItToVoiceClone ( // JMI
      inputLineNumber,
      elt->
        getRepeatCommonPartUpLinkToRepeat ()->
          getRepeatTimes ());
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrRepeatEnding& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msr2bsrTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputEndLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a repeat ending clone and append it to voice clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending a repeat ending clone to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }

  if (gTraceOahGroup->getTraceRepeats () || gTraceOahGroup->getTraceVoices ()) {
    gLog <<
      std::endl <<
      "*********>> msrRepeatEnding HHH " <<
      ", line " << inputLineNumber <<
      " contains:" <<
      std::endl <<
      elt <<
      std::endl <<
      " <<*********";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleRepeatEndingEndInVoice (
      inputLineNumber,
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMeasureRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasureRepeat" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasureRepeat" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  // set last segment as the measures repeat pattern segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting current last segment as measures repeat pattern segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMeasureRepeatPattern& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasureRepeatPattern" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputEndLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasureRepeatPattern" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  // get the measures repeat upLink
  S_msrMeasureRepeat
    measureRepeat =
      elt->getUpLinkToMeasureRepeat ();

  // create a measures repeat and append it to voice clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending a measures repeat to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    createMeasureRepeatAndAppendItToVoiceClone (
      inputLineNumber,
      measureRepeat->
        getMeasureRepeatMeasuresNumber (),
      measureRepeat->
        getMeasureRepeatSlashesNumber ());

  // forget about the current measures repeat pattern clone
  fCurrentMeasureRepeatPatternClone = nullptr;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMeasureRepeatReplicas& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasureRepeatReplicas" <<
      ", line " << elt-> G ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputEndLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasureRepeatReplicas" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  // create a measures repeat replica clone and append it to voice clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending a repeat replica clone to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    appendMeasureRepeatReplicaToVoice (
      inputLineNumber);

  // forget about the current measures repeat replicas clone
 // JMI ??? fCurrentMeasureRepeatReplicasClone = nullptr;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMultipleRest& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMultipleRest" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    std::stringstream ss;

    ss <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMultipleFullBarRests ()
      ||
    gTraceOahGroup->getTraceVoicesDetails ()
  ) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Upon I visitStart (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    ss <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    handleMultipleRestInVoiceClone (
      inputLineNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMultipleFullBarRests ()
      ||
    gTraceOahGroup->getTraceVoicesDetails ()
  ) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Upon II visitStart (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    ss <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

void msr2bsrTranslator::visitEnd (S_msrMultipleRest& elt)
{
  int inputLineNumber =
    elt->getInputEndLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMultipleRest" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the multiple rest clone
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMultipleFullBarRests ()
      ||
    gTraceOahGroup->getTraceSegments ()
      ||
    gTraceOahGroup->getTraceVoices ()
  ) {
    std::stringstream ss;

    ss <<
      "Creating a clone of multiple rest '" <<
      '\'' <<
      elt->asShortString () <<
      "' in voice \"" <<
      fCurrentVoiceClone->getVoiceName () << "\"" <<
      ", fCurrentMultipleRestContentsClone =" <<
      std::endl;

    ++gIndenter;

    ss <<
      fCurrentMultipleRestContentsClone;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrMultipleRest
    multipleRestClone =
      elt->createMultipleRestNewbornClone (
        fCurrentVoiceClone);

  // set the multiple rest clone's contents
  multipleRestClone->
    setMultipleRestContents (
      fCurrentMultipleRestContentsClone);

  // create a new last segment to collect the remainder of the voice,
  // containing the next, yet incomplete, measure
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMultipleFullBarRests ()
      ||
    gTraceOahGroup->getTraceSegments ()
      ||
    gTraceOahGroup->getTraceVoices ()
  ) {
    std::stringstream ss;

    ss <<
      "Creating a new last segment for the remainder of voice \"" <<
      fCurrentVoiceClone->getVoiceName () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentVoiceClone->
    createNewLastSegmentForVoice (
      inputLineNumber);

  // append the multiple rest clone to the current voice clone
  fCurrentVoiceClone->
    appendMultipleRestCloneToVoice (
      inputLineNumber, // JMI ???
      multipleRestClone);

  // forget about the current multiple rest contents clone
  fCurrentMultipleRestContentsClone = nullptr;

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMultipleFullBarRests ()
      ||
    gTraceOahGroup->getTraceVoicesDetails ()
  ) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Upon visitEnd (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    ss <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMultipleRestContents& elt)
{
  int inputLineNumber =
    elt->getInputStartLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMultipleRestContents" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMultipleFullBarRests ()
      ||
    gTraceOahGroup->getTraceVoicesDetails ()
  ) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Upon visitStart (S_msrMultipleRestContents&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    ss <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

void msr2bsrTranslator::visitEnd (S_msrMultipleRestContents& elt)
{
  int inputLineNumber =
    elt->getInputEndLineNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMultipleRestContents" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  // create a multiple rest contents clone
  fCurrentMultipleRestContentsClone =
    elt->createMultipleRestContentsNewbornClone (
      fCurrentVoiceClone);

  // set last segment as the multiple rest contents segment
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleFullBarRests ()) {
    std::stringstream ss;

    ss <<
      "Setting current last segment as multiple rest contents segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentMultipleRestContentsClone->
    setMultipleRestContentsSegment (
      fCurrentVoiceClone->
        getVoiceLastSegment ());

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMultipleFullBarRests ()
      ||
    gTraceOahGroup->getTraceVoicesDetails ()
  ) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Upon visitEnd (S_msrMultipleRestContents&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    ss <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrLayout" <<
      ", line " << elt->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrLayout& elt)
{
  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrLayout" <<
      ", line " << elt->getInputEndLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}
*/


} // namespace
