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

#include "mfEnableTracingIfDesired.h"

#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfServiceRunData.h"

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
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

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
      K_MF_INPUT_LINE_UNKNOWN,
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
    gGlobalServiceRunData->getInputSourceName (),
    inputLineNumber,
    message);

  // append a note to the BSR JMI
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrLineBreak& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLineBreak" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrPageBreak& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageBreak" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrScore& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScore" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
        gGlobalOahOahGroup->getOahOahGroupServiceName ());

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
#ifdef OAH_TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScore" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentPartGroup = elt;
}

void msr2bsrTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrPart& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

  std::string
    partCombinedName =
      elt->getPartCombinedName ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPart " <<
      partCombinedName <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      std::endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      std::endl;
  }
#endif

  ++gIndenter;

  fCurrentPart = elt;
}

void msr2bsrTranslator::visitEnd (S_msrPart& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  --gIndenter;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffTuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a staff tuning clone
  fCurrentStaffTuning = elt;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentStaffTuning = nullptr;
}

void msr2bsrTranslator::visitEnd (S_msrStaffDetails& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStaff& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++gIndenter;

  fCurrentStaff = elt;
}

void msr2bsrTranslator::visitEnd (S_msrStaff& elt)
{
  --gIndenter;

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrVoice& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentVoice = elt;

  fRelativeOctaveReference = nullptr;

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrVoice& elt)
{
  --gIndenter;

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrBarLine& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarLine" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Handling '" <<
      msrBarLineCategoryKindAsString (
        elt->getBarLineCategory ()) <<
      std::endl;
  }
#endif

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
    std::stringstream s;

    s <<
      "MSR barLine kind '" <<
      msrBarLineStyleKindAsString (mBarLineStyleKind) <<
      "' is not supported in Braille";

    notSupportedMessage (
      inputLineNumber,
      s.str ());
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
      elt->getInputLineNumber ();

  std::string
    measureNumber =
      elt->getMeasureNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "'" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a measure
  fCurrentMeasure =
    bsrMeasure::create (
      inputLineNumber,
      measureNumber);

  // append it to the current line
  fCurrentLine->
    appendMeasureToLine (fCurrentMeasure);
}

void msr2bsrTranslator::visitEnd (S_msrMeasure& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasure '" <<
      elt->getMeasureNumber () <<
      "'" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string
    measureNumber =
      elt->getMeasureNumber ();
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrClef& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrClef" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (gGlobalMsr2bsrOahGroup->getIncludeClefs ()) {
    bsrClefKind bClefKind = bsrClefKind::kClefKindNone;

  /* JMI
          kClefModifiedBassForRightHandPartKind,
          kClefModifiedTrebleForLeftHandPartKind };
          */

    msrClefKind mClefKind = elt->getClefKind ();

    switch (mClefKind) {
      case msrClefKind::kClef_UNKNOWN:
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
      std::stringstream s;

      s <<
        "MSR clef kind '" <<
        msrClefKindAsString (mClefKind) <<
        "' is not supported in Braille";

      notSupportedMessage (
        inputLineNumber,
        s.str ());
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrClef" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrKey& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrKey" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
            case msrModeKind::kMode_UNKNOWN:
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
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceKeys ()) {
      gLogStream <<
        "BSR key kind is absent, ignoring '" <<
        bsrKeyKindAsString (theBsrKeyKind) <<
        ", " <<
       msrSemiTonesPitchKindAsString (mSemiTonesPitchKind) <<
       std::endl;
    }
#endif
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrKey" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTimeSignature& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTimeSignature" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTimeSignature" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTempo& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTempo" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
bsrNoteOctaveIsNeeded msr2bsrTranslator::brailleOctaveMarkInNeeded (
  const S_msrNote& note)
{
  bsrNoteOctaveIsNeeded
    result = bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededNo;

  int inputLineNumber =
    note->getInputLineNumber ();

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

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    const int fieldWidth = 28;

    gLogStream << std::left <<
/*
      std::setw (fieldWidth) <<
      "% referenceDiatonicPitch" <<
      " = " <<
      referenceDiatonicPitch <<
      std::endl <<
*/
      std::setw (fieldWidth) <<
      "% fRelativeOctaveReference" <<
      " = " <<
      fRelativeOctaveReference->asShortString () <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% note" <<
      " = " <<
      note->asShortString () <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceDiatonicPitchAsString" <<
      " = " <<
      referenceDiatonicPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAbsoluteOctaveKind" <<
       " = " <<
      msrOctaveKindAsString (referenceAbsoluteOctaveKind) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAbsoluteOctaveKind" <<
       " = " <<
      msrOctaveKindAsString (noteAbsoluteOctaveKind) <<
      std::endl << std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal" <<
      " = " <<
      referenceAboluteDiatonicOrdinal <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal" <<
      " = " <<
      noteAboluteDiatonicOrdinal <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal - referenceAboluteDiatonicOrdinal" <<
      " = " <<
      noteAboluteDiatonicOrdinal - referenceAboluteDiatonicOrdinal <<
      std::endl <<
      std::setw (fieldWidth) <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::stringstream s;

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
    note->getInputLineNumber ();

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

  msrDurationKind
    noteGraphicDurationKind =
      note->
        getNoteGraphicDurationKind ();

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
        i=noteWords.begin ();
        i!=noteWords.end ();
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
            case msrPlacementKind::kPlacement_UNKNOWN:
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
    switch (noteGraphicDurationKind) {
      case msrDurationKind::kDuration_UNKNOWN:
        break;

      case msrDurationKind::kDuration1024th:
        break;
      case msrDurationKind::kDuration512th:
        break;
      case msrDurationKind::kDuration256th:
        noteValueKind = bsrNoteValueKind::kNoteValueRest256th;
        break;
      case msrDurationKind::kDuration128th:
        noteValueKind = bsrNoteValueKind::kNoteValueRest128th;
        break;
      case msrDurationKind::kDuration64th:
        noteValueKind = bsrNoteValueKind::kNoteValueRest64th;
        break;
      case msrDurationKind::kDuration32nd:
        noteValueKind = bsrNoteValueKind::kNoteValueRest32nd;
        break;
      case msrDurationKind::kDuration16th:
        noteValueKind = bsrNoteValueKind::kNoteValueRest16th;
        break;
      case msrDurationKind::kDurationEighth:
        noteValueKind = bsrNoteValueKind::kNoteValueRest8th;
        break;
      case msrDurationKind::kDurationQuarter:
        noteValueKind = bsrNoteValueKind::kNoteValueRestQuarter;
        break;
      case msrDurationKind::kDurationHalf:
        noteValueKind = bsrNoteValueKind::kNoteValueRestHalf;
        break;
      case msrDurationKind::kDurationWhole:
        noteValueKind = bsrNoteValueKind::kNoteValueRestWhole;
        break;
      case msrDurationKind::kDurationBreve:
        noteValueKind = bsrNoteValueKind::kNoteValueRestBreve;
        break;
      case msrDurationKind::kDurationLonga:
        break;
      case msrDurationKind::kDurationMaxima:
        break;
    } // switch
  }

  else {
    switch (noteDiatonicPitchKind) {
      case msrDiatonicPitchKind::kDiatonicPitch_UNKNOWN:
        break;

      case msrDiatonicPitchKind::kDiatonicPitchA:
        switch (noteGraphicDurationKind) {
          case msrDurationKind::kDuration_UNKNOWN:
            break;

          case msrDurationKind::kDuration1024th:
            break;
          case msrDurationKind::kDuration512th:
            break;
          case msrDurationKind::kDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueA256th;
            break;
          case msrDurationKind::kDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueA128th;
            break;
          case msrDurationKind::kDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueA64th;
            break;
          case msrDurationKind::kDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueA32nd;
            break;
          case msrDurationKind::kDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueA16th;
            break;
          case msrDurationKind::kDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueA8th;
            break;
          case msrDurationKind::kDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueAQuarter;
            break;
          case msrDurationKind::kDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueAHalf;
            break;
          case msrDurationKind::kDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueAWhole;
            break;
          case msrDurationKind::kDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueABreve;
            break;
          case msrDurationKind::kDurationLonga:
            break;
          case msrDurationKind::kDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchB:
        switch (noteGraphicDurationKind) {
          case msrDurationKind::kDuration_UNKNOWN:
            break;

          case msrDurationKind::kDuration1024th:
            break;
          case msrDurationKind::kDuration512th:
            break;
          case msrDurationKind::kDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueB256th;
            break;
          case msrDurationKind::kDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueB128th;
            break;
          case msrDurationKind::kDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueB64th;
            break;
          case msrDurationKind::kDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueB32nd;
            break;
          case msrDurationKind::kDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueB16th;
            break;
          case msrDurationKind::kDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueB8th;
            break;
          case msrDurationKind::kDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueBQuarter;
            break;
          case msrDurationKind::kDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueBHalf;
            break;
          case msrDurationKind::kDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueBWhole;
            break;
          case msrDurationKind::kDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueBBreve;
            break;
          case msrDurationKind::kDurationLonga:
            break;
          case msrDurationKind::kDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchC:
        switch (noteGraphicDurationKind) {
          case msrDurationKind::kDuration_UNKNOWN:
            break;

          case msrDurationKind::kDuration1024th:
            break;
          case msrDurationKind::kDuration512th:
            break;
          case msrDurationKind::kDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueC256th;
            break;
          case msrDurationKind::kDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueC128th;
            break;
          case msrDurationKind::kDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueC64th;
            break;
          case msrDurationKind::kDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueC32nd;
            break;
          case msrDurationKind::kDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueC16th;
            break;
          case msrDurationKind::kDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueC8th;
            break;
          case msrDurationKind::kDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueCQuarter;
            break;
          case msrDurationKind::kDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueCHalf;
            break;
          case msrDurationKind::kDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueCWhole;
            break;
          case msrDurationKind::kDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueCBreve;
            break;
          case msrDurationKind::kDurationLonga:
            break;
          case msrDurationKind::kDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchD:
        switch (noteGraphicDurationKind) {
          case msrDurationKind::kDuration_UNKNOWN:
            break;

          case msrDurationKind::kDuration1024th:
            break;
          case msrDurationKind::kDuration512th:
            break;
          case msrDurationKind::kDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueD256th;
            break;
          case msrDurationKind::kDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueD128th;
            break;
          case msrDurationKind::kDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueD64th;
            break;
          case msrDurationKind::kDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueD32nd;
            break;
          case msrDurationKind::kDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueD16th;
            break;
          case msrDurationKind::kDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueD8th;
            break;
          case msrDurationKind::kDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueDQuarter;
            break;
          case msrDurationKind::kDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueDHalf;
            break;
          case msrDurationKind::kDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueDWhole;
            break;
          case msrDurationKind::kDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueDBreve;
            break;
          case msrDurationKind::kDurationLonga:
            break;
          case msrDurationKind::kDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchE:
        switch (noteGraphicDurationKind) {
          case msrDurationKind::kDuration_UNKNOWN:
            break;

          case msrDurationKind::kDuration1024th:
            break;
          case msrDurationKind::kDuration512th:
            break;
          case msrDurationKind::kDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueE256th;
            break;
          case msrDurationKind::kDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueE128th;
            break;
          case msrDurationKind::kDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueE64th;
            break;
          case msrDurationKind::kDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueE32nd;
            break;
          case msrDurationKind::kDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueE16th;
            break;
          case msrDurationKind::kDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueE8th;
            break;
          case msrDurationKind::kDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueEQuarter;
            break;
          case msrDurationKind::kDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueEHalf;
            break;
          case msrDurationKind::kDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueEWhole;
            break;
          case msrDurationKind::kDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueEBreve;
            break;
          case msrDurationKind::kDurationLonga:
            break;
          case msrDurationKind::kDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchF:
        switch (noteGraphicDurationKind) {
          case msrDurationKind::kDuration_UNKNOWN:
            break;

          case msrDurationKind::kDuration1024th:
            break;
          case msrDurationKind::kDuration512th:
            break;
          case msrDurationKind::kDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueF256th;
            break;
          case msrDurationKind::kDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueF128th;
            break;
          case msrDurationKind::kDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueF64th;
            break;
          case msrDurationKind::kDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueF32nd;
            break;
          case msrDurationKind::kDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueF16th;
            break;
          case msrDurationKind::kDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueF8th;
            break;
          case msrDurationKind::kDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueFQuarter;
            break;
          case msrDurationKind::kDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueFHalf;
            break;
          case msrDurationKind::kDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueFWhole;
            break;
          case msrDurationKind::kDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueFBreve;
            break;
          case msrDurationKind::kDurationLonga:
            break;
          case msrDurationKind::kDurationMaxima:
            break;
        } // switch
        break;

      case msrDiatonicPitchKind::kDiatonicPitchG:
        switch (noteGraphicDurationKind) {
          case msrDurationKind::kDuration_UNKNOWN:
            break;

          case msrDurationKind::kDuration1024th:
            break;
          case msrDurationKind::kDuration512th:
            break;
          case msrDurationKind::kDuration256th:
            noteValueKind = bsrNoteValueKind::kNoteValueG256th;
            break;
          case msrDurationKind::kDuration128th:
            noteValueKind = bsrNoteValueKind::kNoteValueG128th;
            break;
          case msrDurationKind::kDuration64th:
            noteValueKind = bsrNoteValueKind::kNoteValueG64th;
            break;
          case msrDurationKind::kDuration32nd:
            noteValueKind = bsrNoteValueKind::kNoteValueG32nd;
            break;
          case msrDurationKind::kDuration16th:
            noteValueKind = bsrNoteValueKind::kNoteValueG16th;
            break;
          case msrDurationKind::kDurationEighth:
            noteValueKind = bsrNoteValueKind::kNoteValueG8th;
            break;
          case msrDurationKind::kDurationQuarter:
            noteValueKind = bsrNoteValueKind::kNoteValueGQuarter;
            break;
          case msrDurationKind::kDurationHalf:
            noteValueKind = bsrNoteValueKind::kNoteValueGHalf;
            break;
          case msrDurationKind::kDurationWhole:
            noteValueKind = bsrNoteValueKind::kNoteValueGWhole;
            break;
          case msrDurationKind::kDurationBreve:
            noteValueKind = bsrNoteValueKind::kNoteValueGBreve;
            break;
          case msrDurationKind::kDurationLonga:
            break;
          case msrDurationKind::kDurationMaxima:
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
      brailleOctaveMarkInNeeded (note);
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      "--> fCurrentNoteValueSizeKind = " <<
      bsrNoteValueSizeKindAsString (fCurrentNoteValueSizeKind) <<
      ", noteValueSizeKind = " <<
      bsrNoteValueSizeKindAsString (noteValueSizeKind) <<
      ", noteOctaveIsNeeded = " <<
      bsrNoteOctaveIsNeededAsString (noteOctaveIsNeeded) <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (false && noteValueSizeKind != fCurrentNoteValueSizeKind) { // JMI
    gLogStream <<
      "--> note = '" <<
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
#ifdef OAH_TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  createBsrForNote (elt);

  fRelativeOctaveReference = elt;
}

void msr2bsrTranslator::visitEnd (S_msrNote& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrDynamic& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create the dynamic
  S_bsrDynamic
    dynamic =
      bsrDynamic::create (
        elt->getInputLineNumber (),
        elt->getDynamicKind ());

  // append it to the current measure
  fCurrentMeasure->
    appendDynamicToMeasure (dynamic);
}

void msr2bsrTranslator::visitEnd (S_msrDynamic& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

/*
//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrIdentification& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}
*/

/*
//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrCredit& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentCredit = elt;

  // set elt as credit of the MSR score part of the BSR score
  fResultingBsr->
    getMsrScore ()->
      appendCreditToScore (fCurrentCredit);
}

void msr2bsrTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentCredit = nullptr;
}

void msr2bsrTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // don't append it to the current credit, since the latter is no clone
  / * JMI
  fCurrentCredit->
    appendCreditWordsToCredit (
      elt);
      * /
}

void msr2bsrTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSegment& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
  fCurrentSegmentClonesStack.push (
    segmentClone);

  // append it to the current voice
  fCurrentVoiceClone->
    appendSegmentToVoiceClone (
      segmentClone);
}

void msr2bsrTranslator::visitEnd (S_msrSegment& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // forget current segment clone
  fCurrentSegmentClonesStack.pop ();
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrHarmony '" <<
      elt->asString () <<
      ", fOnGoingNote: " << fOnGoingNote <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_msrHarmonyDegree '" <<
      elt->asString () <<
      ", fOnGoingNote: " << fOnGoingNote <<
      ", fOnGoingChord: " << fOnGoingChord <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      "', line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the harmony degree to the current harmony clone
  fCurrentHarmonyClone->
    appendHarmonyDegreeToHarmony (
      elt);
}

void msr2bsrTranslator::visitEnd (S_msrHarmony& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrFrame& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFrame '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNote) {
    // register the frame in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      setNoteFrame (elt);
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a deep clone of the figured bass
  fCurrentFiguredBass =
    elt->
      createFiguredBassDeepClone (
        fCurrentPartClone);

  if (fOnGoingNote) {
    // append the figured bass to the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendFiguredBassToNoteFiguredBassesList (fCurrentFiguredBassClone);

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBassFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the bass figure to the current figured bass
  fCurrentFiguredBass->
    appendFigureToFiguredBass (
      elt);
}

void msr2bsrTranslator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentFiguredBass = nullptr;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  std::string
    measureNumber =
      elt->getMeasureNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "'" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/ * JMI
  {
    gLogStream <<
      std::endl <<
      elt <<
      std::endl;
  }
     * /

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      std::endl <<
      "<!--=== measure '" << measureNumber <<
      "', voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      std::endl;
  }
#endif

  // measure 1 is created by default initially ??? JMI

  // create a clone of the measure
  fCurrentMeasureClone =
    elt->
      createMeasureNewbornClone (
        fCurrentSegmentClonesStack.top ());

  // append it to the current segment clone
  fCurrentSegmentClonesStack.top ()->
    appendMeasureToSegment (
      fCurrentMeasureClone);

// JMI utile???
  fCurrentPartClone->
    setPartCurrentMeasureNumber (
      measureNumber);

  // should the last bar check's measure be set?
  if (fLastBarCheck) {
    fLastBarCheck->
      setNextBarNumber (
        measureNumber);

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
  Rational
    measureWholeNotesDuration =
      fCurrentMeasureClone->
        getMeasureWholeNotesDuration ();

  // get the full measure whole notes
  Rational
    measureFullLength =
      fCurrentMeasureClone->
        getMeasureFullLength ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing measure '" << measureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl <<
      "measureWholeNotesDuration: " << measureWholeNotesDuration <<
      std::endl;
  }
#endif

  msrMeasureKind
    measureKind =
      msrMeasure::kUnknownMeasure; // JMI
 // JMI     fMeasureKind = kFullMeasure; // may be changed afterwards

  if (measureWholeNotesDuration == measureFullLength ) {
    // this measure is full
    measureKind =
      msrMeasure::kFullMeasure;
  }

  else if (measureWholeNotesDuration < measureFullLength) {
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

  else if (measureWholeNotesDuration > measureFullLength) {
    // this measure is overfull
    measureKind =
      msrMeasure::kOverfullMeasure;
  }

  if (false && / * JMI * / measureKind != originalMeasure->getMeasureKind ()) { // JMI
    std::stringstream s;

    s <<
      "line " << inputLineNumber << ":" <<
      " clone measure:" <<
      std::endl <<
      fCurrentMeasureClone <<
      std::endl <<
      "differs for measure kind from original measure:" <<
      std::endl <<
      originalMeasure;

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2bsrTranslator::visitEnd (S_msrMeasure& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasure '" <<
      elt->getMeasureNumber () <<
      "'" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string
    measureNumber =
      elt->getMeasureNumber ();

  finalizeCurrentMeasureClone ( // JMI
    inputLineNumber,
    elt); // original measure

  Bool doCreateABarCheck (false); // JMI

  switch (elt->getMeasureKind ()) {

    case msrMeasureKind::kMeasureKindUnknown:
      {
        std::stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' in voice \"" <<
          elt->
            fetchMeasureUpLinkToVoice ()->
              getVoiceName () <<
          "\" is of unknown kind in msr2bsrTranslator";

      // JMI  msrInternalError (
        msrInternalWarning (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
  //        __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrMeasureKind::kMeasureKindRegular:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindAnacrusis:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindIncompleteStandalone:
      doCreateABarCheck = true;
      break;
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeat:
      doCreateABarCheck = true;
      break;
    case msrMeasureKind::kMeasureKindIncompleteAfterRepeat:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindOvercomplete:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindCadenza:
      doCreateABarCheck = true;
      break;

    case msrMeasureKind::kMeasureKindEmpty:
      // JMI
      break;
  } // switch

  if (doCreateABarCheck) {
    // create a bar check without next bar number,
    // it will be set upon visitStart (S_msrMeasure&)
    // for the next measure
    fLastBarCheck =
      msrBarCheck::create (
        inputLineNumber);

           / * JMI
  gLogStream <<
    std::endl <<
    "***********" <<
    std::endl << std::endl;
  fCurrentPartClone->print (gLogStream);
  gLogStream <<
    "***********" <<
    std::endl << std::endl;
    * /
/ * JMI
    // append it to the current voice clone
    fCurrentVoiceClone->
      appendBarCheckToVoice (fLastBarCheck);
      * /
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStanza& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // forget about this stanza
  fCurrentStanzaClone = nullptr;

  fOnGoingStanza = false;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSyllable& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSyllable" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

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

    if (gGlobalBsrOahGroup->getAddMsrWordsFromTheMusicXMLLyrics ()) {
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
#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceLyrics () || gGlobalTracingOahGroup->getTraceWords ()) {
          gLogStream <<
            "Changing lyrics '" <<
            wordsValue <<
            "' into words for note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "'" <<
      // JMI      fCurrentSyllableClone->asString () <<
            std::endl;
        }
#endif

        S_msrWords
          words =
            msrWords::create (
              inputLineNumber,
              msrPlacementKind::kPlacement_UNKNOWN,                // default value
              wordsValue,
              msrJustifyKind::kJustifyNone,                  // default value
              msrVerticalAlignmentKind::kVerticalAlignmentNone,        // default value
              msrFontStyleKind::kFontStyleNone,                // default value
              msrFontSize::create (
                msrFontSizeKind::kFontSizeNone), // default value
              msrFontWeightKind::kFontWeightNone,               // default value
              msrWords::kItLang);            // default value

        // append it to the current non-grace note
#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceLyrics () || gGlobalTracingOahGroup->getTraceWords ()) {
          gLogStream <<
            "Appending words '" <<
            words->asShortString () <<
            "' to note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "'" <<
            std::endl;
        }
#endif
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSyllable" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}
*/

/*
//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTransposition& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTransposition" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append transpose to voice clone
  fCurrentVoiceClone->
    appendTranspositionToVoice (elt);
}

void msr2bsrTranslator::visitEnd (S_msrTransposition& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTransposition" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrRehearsalMark& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRehearsalMark" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendRehearsalMarkToVoice (elt);
}

void msr2bsrTranslator::visitEnd (S_msrRehearsalMark& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRehearsalMark" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrFermata& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSpanner& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  switch (elt->getSpannerTypeKind ()) {
    case msrSpannerTypeKind::kSpannerTypeStart:
      break;
    case msrSpannerTypeKind::kSpannerTypeStop:
      break;
    case msrSpannerTypeKind::kSpannerTypeContinue:
      break;
    case msrSpannerTypeKind::kSpannerType_UNKNOWN:
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSlide& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOtherDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOtherDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrWords& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSlur& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrLigature& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSlash& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlash" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCrescDecresc" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCrescDecresc" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrWedge& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGraceNotesGroup" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  Bool doCreateAGraceNoteClone (true); // JMI

  if (doCreateAGraceNoteClone) {
    // create a clone of this graceNotesGroup
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceGraceNotes ()) {
      gLogStream <<
        "Creating a clone of grace notes group '" <<
        elt->asShortString () <<
        "' and attaching it to clone note '" <<
        fCurrentNonGraceNoteClone->asShortString () <<
        "'" <<
        std::endl;
      }
#endif

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

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "+++++++++++++++++++++++++ 1" <<
      std::endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      gLogStream <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLogStream <<
        "[NONE]";
    }
    gLogStream << std::endl;
  }
#endif

  // get the note this grace notes group is attached to
  S_msrNote
    noteNotesGroupIsAttachedTo =
      elt->
        getGraceNotesGroupUpLinkToNote ();

  if (! noteNotesGroupIsAttachedTo) {
    std::stringstream s;

    s <<
      "grace notes group '" << elt->asShortString () <<
      "' has an empty note upLink";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingGraceNotesGroup = true;

  // is noteNotesGroupIsAttachedTo the first one in its voice?
#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceGraceNotes ()
      ||
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalTracingOahGroup->getTraceVoices ()
  ) {
    gLogStream <<
      "The noteNotesGroupIsAttachedTo voice clone FIRST_ONE??? '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (noteNotesGroupIsAttachedTo) {
      gLogStream <<
        noteNotesGroupIsAttachedTo->asShortString ();
    }
    else {
      gLogStream <<
        "[NONE]";
    }
    gLogStream <<
       "'" <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceGraceNotes ()
      ||
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalTracingOahGroup->getTraceVoices ()
  ) {
    gLogStream <<
      "The first note of voice clone KLJWLPOEF '" <<
      fCurrentVoiceClone->getVoiceName () <<
      "' is '";

    if (fFirstNoteCloneInVoice) {
      gLogStream <<
        fFirstNoteCloneInVoice->asShortString ();
    }
    else {
      gLogStream <<
        "[NONE]";
    }
    gLogStream <<
       "'" <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
        if (
            gGlobalTracingOahGroup->getTraceGraceNotes ()
              ||
            gGlobalTracingOahGroup->getTraceNotes ()
              ||
            gGlobalTracingOahGroup->getTraceVoices ()
        ) {
          gLogStream <<
            "Creating a skip clone of grace notes group '" <<
            elt->asShortString () <<
            "' to work around LilyPond issue #34" <<
            std::endl;
        }
#endif

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

#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceGraceNotes ()Group) {
        gLogStream <<
          "Optimising grace notes '" <<
          elt->asShortString () <<
          "' into after grace notes" <<
          std::endl;
      }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceGraceNotes ()Group) {
        gLogStream <<
          "Creating a skip clone of grace notes '" <<
          elt->asShortString () <<
          "' to work around LilyPond issue #34" <<
          std::endl;
      }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceGraceNotes ()Group) {
        gLogStream <<
          "Converting grace notes '" <<
          elt->asShortString () <<
          "' into after grace notes attached to:" <<
          std::endl;

        ++gIndenter;

        fCurrentAfterGraceNotesGroupElement->
          print (gLogStream);

        --gIndenter;
      }
#endif

      fPendingAfterGraceNotesGroup =
        msrAfterGraceNotesGroup::create (
          inputLineNumber,
            fCurrentAfterGraceNotesGroupElement,
            elt->getGraceNotesGroupIsSlashed (),
            fCurrentVoiceClone);

      // append it to the current note clone
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceGraceNotes ()Group) {
        gLogStream <<
          "Appending the after grace notes to current note clone" <<
          std::endl;
      }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGraceNotesGroup" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "+++++++++++++++++++++++++ 2" <<
      std::endl <<
      "fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      gLogStream <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLogStream <<
        "[NONE]";
    }
    gLogStream << std::endl;
  }
#endif

  // forget about these grace notes
  fCurrentGraceNotesGroupClone = nullptr;

  fOnGoingGraceNotesGroup = false;

/ * JMI
  if (fPendingAfterGraceNotesGroup) {
    // remove the current afterGraceNotesGroup note clone
    // from the current voice clone
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceGraceNotes ()Group) {
      gLogStream <<
        "Removing the after grace notes element from the current voice clone" <<
        std::endl;
    }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceVoices ()) {
          gLogStream <<
            "The first note of voice clone GFFF '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '";

          if (fFirstNoteCloneInVoice) {
            gLogStream <<
              fFirstNoteCloneInVoice->asShortString ();
          }
          else {
            gLogStream <<
              "[NONE]";
          }
          gLogStream <<
             "'" <<
            std::endl;
        }
#endif
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

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceNotes () || gGlobalTracingOahGroup->getTraceVoices ()) {
          gLogStream <<
            "The first note of voice clone RJIRWR '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '" <<
            fFirstNoteCloneInVoice->asShortString () <<
             "'" <<
            std::endl;
        }
#endif
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
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()Group) {
      gLogStream <<
        "Optimizing grace notes on trilled note '" <<
        elt->asShortString () <<
        "' as after grace notes " <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

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
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      "FAA fCurrentNonGraceNoteClone = " <<
      std::endl;
    if (fCurrentNonGraceNoteClone) {
      gLogStream <<
        fCurrentNonGraceNoteClone;
    }
    else {
      gLogStream <<
        "[NONE]" <<
        std::endl;
    }

    gLogStream <<
      "FAA fCurrentGraceNoteClone = " <<
      std::endl;
    if (fCurrentGraceNoteClone) {
      gLogStream <<
        fCurrentGraceNoteClone;
    }
    else {
      gLogStream <<
        "[NONE]" <<
        std::endl;
    }
  }
#endif

  switch (elt->getNoteKind ()) {

    case msrNoteKind::kNote_UNKNOWN:
      break;

    case msrNoteKind::kNoteRestInMeasure:
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending rest note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;
      }
#endif

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteSkipInMeasure: // JMI
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending skip note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;
      }
#endif

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending unpitched note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;
      }
#endif

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteRegularInMeasure:
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending regular note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;
      }
#endif

      fCurrentVoiceClone->
        appendNoteToVoiceClone (
          fCurrentNonGraceNoteClone);
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      if (fOnGoingDoubleTremolo) {

        if (fCurrentNonGraceNoteClone->getNoteIsFirstNoteInADoubleTremolo ()) {
#ifdef OAH_TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceNotes ()) {
            gLogStream <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo first element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              std::endl;
          }
#endif

          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteFirstElement (
              fCurrentNonGraceNoteClone);
        }

        else if (fCurrentNonGraceNoteClone->getNoteIsSecondNoteInADoubleTremolo ()) {
#ifdef OAH_TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceNotes ()) {
            gLogStream <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo second element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
              "\"" <<
              std::endl;
          }
#endif

          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteSecondElement (
              fCurrentNonGraceNoteClone);
        }

        else {
          std::stringstream s;

          s <<
            "note '" << fCurrentNonGraceNoteClone->asShortString () <<
            "' belongs to a double tremolo, but is not marked as such";

          msrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
      }

      else {
        std::stringstream s;

        s <<
          "double tremolo note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of a double tremolo";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    / * JMI
      gLogStream <<
        "fOnGoingGraceNotesGroup = " <<
        fOnGoingGraceNotesGroup <<
        std::endl;
        * /

      if (! fOnGoingGraceNotesGroup) {
        std::stringstream s;

        s <<
          "grace note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of grace notes";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      else {
#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceGraceNotes () || gGlobalTracingOahGroup->getTraceNotes ()) {
          gLogStream <<
            "Appending grace note '" <<
            fCurrentGraceNoteClone->asShortString () <<
            "' to the grace notes group'" <<
            fCurrentGraceNotesGroupClone->asShortString () <<
            "' in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            std::endl;
        }
#endif

        fCurrentGraceNotesGroupClone->
          appendNoteToGraceNotesGroup (
            fCurrentGraceNoteClone);
      }

    / * JMI ???
      if (fCurrentGraceNotesGroupClone) {
#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceGraceNotes () || gGlobalTracingOahGroup->getTraceNotes ()) {
          gLogStream <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            std::endl;
        }
#endif

        fCurrentGraceNotesClone->
          appendNoteToGraceNotes (
            fCurrentNonGraceNoteClone);
      }

      else if (fPendingAfterGraceNotes) {
#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceGraceNotes () || gGlobalTracingOahGroup->getTraceNotes ()) {
          gLogStream <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the after grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"" <<
            std::endl;
        }
#endif

        fPendingAfterGraceNotes->
          appendNoteToAfterGraceNotesContents (
            fCurrentNonGraceNoteClone);
      }

      else {
        std::stringstream s;

        s <<
          "both fCurrentGraceNoteGroupsClone and fPendingAfterGraceNoteGroup are null," <<
          std::endl <<
          "cannot handle grace note'" <<
          elt->asString () <<
          "'";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
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
        std::stringstream s;

        s <<
          "msr2bsrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
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
        std::stringstream s;

        s <<
          "msr2bsrTranslator::visitEnd (S_msrNote& elt): chord member note " <<
          elt->asString () <<
          " appears outside of a chord";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceNotes ()) {
        gLogStream <<
          "Appending note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "'' to voice clone " <<
          fCurrentVoiceClone->getVoiceName () <<
          std::endl;
      }
#endif

      fTupletClonesStack.top ()->
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    setNoteOctaveShift (elt);
}

void msr2bsrTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrHarpPedalsTuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the harp pedals tuning to the voice clone
  fCurrentVoiceClone->
    appendHarpPedalsTuningToVoice (elt);
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrStem& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrBeam& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
// JMI      ", fOnGoingNote: " << fOnGoingNote <<
// JMI      ", fOnGoingChord: " << fOnGoingChord <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChord" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentChordClone =
    elt->createChordNewbornClone (
      fCurrentPartClone);

  if (fTupletClonesStack.size ()) {
    // a chord in a tuplet is handled as part of the tuplet JMI
    fTupletClonesStack.top ()->
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
      std::stringstream s;

      s <<
        "chord '" << elt->asString () <<
        "' belongs to a double tremolo, but is not marked as such";

      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTuplet& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create the tuplet clone
  S_msrTuplet
    tupletClone =
      elt->createTupletNewbornClone ();

  // register it in this visitor
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "++> pushing tuplet '" <<
      tupletClone->asString () <<
      "' to tuplets stack" <<
      std::endl;
  }
#endif

  fTupletClonesStack.push (tupletClone);

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Popping tuplet '" <<
      elt->asString () <<
      "' from tuplets stack" <<
      std::endl;
  }
#endif

  fTupletClonesStack.pop ();

  if (fTupletClonesStack.size ()) {
    // tuplet is a nested tuplet
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "Adding nested tuplet '" <<
      elt->asString () <<
        "' to stack top tuplet '" <<
      fTupletClonesStack.top ()->asString () <<
      "'" <<
      std::endl;
    }
#endif

    fTupletClonesStack.top ()->
      appendTupletToTupletClone (elt);
  }

  else {
    // tuplet is a top level tuplet

#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTuplets ()) {
      gLogStream <<
        "Adding top level tuplet '" <<
      elt->asString () <<
      "' to voice" <<
      fCurrentVoiceClone->getVoiceName () <<
      std::endl;
    }
#endif

    fCurrentVoiceClone->
      appendTupletToVoice (elt);
  }
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrTie& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    setNoteTie (elt);
}

void msr2bsrTranslator::visitEnd (S_msrTie& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrSegno& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendSegnoToVoice (elt);
}

void msr2bsrTranslator::visitStart (S_msrCoda& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendCodaToVoice (elt);
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting eyeGlasses" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendEyeGlassesToNote (elt);
}

void msr2bsrTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendScordaturaToNote (elt);
}

void msr2bsrTranslator::visitStart (S_msrPedal& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting pedal" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendPedalToNote (elt);
}

void msr2bsrTranslator::visitStart (S_msrDamp& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting damp" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampToNote (elt);

  fResultingBsr->
    // this score needs the 'custom short barLine' Scheme function
    setDampMarkupIsNeeded ();
}

void msr2bsrTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting dampAll" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampAllToNote (elt);

  fResultingBsr->
    // this score needs the 'custom short barLine' Scheme function
    setDampAllMarkupIsNeeded ();
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarCheck" <<
      ", nextBarNumber: " <<
      elt->getNextBarNumber () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2bsrTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarNumberCheckToVoice (elt);
}

void msr2bsrTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/ * JMI
  gLogStream <<
    std::endl <<
    "*********** fCurrentPartClone" <<
    std::endl << std::endl;
  fCurrentPartClone->print (gLogStream);
  gLogStream <<
    "*********** fCurrentPartClone" <<
    std::endl << std::endl;
    * /

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in part clone" <<
      fCurrentPartClone->getPartCombinedName () <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a repeat newborn clone" <<
      ", line " << inputLineNumber <<
      ", in voice \"" <<
      elt->
        getRepeatUpLinkToVoice ()->
          getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentRepeatClone =
    elt->
      createRepeatNewbornClone (
        fCurrentVoiceClone);
* /
}

void msr2bsrTranslator::visitEnd (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/ * JMI
  // append the repeat clone to the current part clone
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a repeat to part clone " <<
      fCurrentPartClone->getPartCombinedName () << "\"" <<
      std::endl;
  }
#endif

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
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceRepeats ()
      ||
    gGlobalTracingOahGroup->getTraceVoicesDetails ()
  ) {
    gLogStream <<
      std::endl <<
      "Upon visitStart (S_msrRepeatCommonPart&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif
}

void msr2bsrTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceRepeats ()
      ||
    gGlobalTracingOahGroup->getTraceVoicesDetails ()
  ) {
    gLogStream <<
      std::endl <<
      "Upon visitEnd (S_msrRepeatCommonPart&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif

  // create a repeat and append it to voice clone
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a repeat to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void msr2bsrTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a repeat ending clone and append it to voice clone
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a repeat ending clone to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }

  if (gGlobalTracingOahGroup->getTraceRepeats () || gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      std::endl <<
      "*********>> msrRepeatEnding HHH " <<
      ", line " << inputLineNumber <<
      " contains:" <<
      std::endl <<
      elt <<
      std::endl <<
      " <<*********" <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndingEndInVoice (
      inputLineNumber,
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMeasureRepeat& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasureRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  --gIndenter;

  // set last segment as the measures repeat pattern segment
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Setting current last segment as measures repeat pattern segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMeasureRepeatPattern& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureRepeatPattern" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasureRepeatPattern" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  --gIndenter;

  // get the measures repeat upLink
  S_msrMeasureRepeat
    measureRepeat =
      elt->getUpLinkToMeasureRepeat ();

  // create a measures repeat and append it to voice clone
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Appending a measures repeat to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureRepeatReplicas" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasureRepeatReplicas" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  --gIndenter;

  // create a measures repeat replica clone and append it to voice clone
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending a repeat replica clone to voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

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
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMultipleRest" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()
      ||
    gGlobalTracingOahGroup->getTraceVoicesDetails ()
  ) {
    gLogStream <<
      std::endl <<
      "Upon I visitStart (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif

  fCurrentVoiceClone->
    handleMultipleRestInVoiceClone (
      inputLineNumber);

#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()
      ||
    gGlobalTracingOahGroup->getTraceVoicesDetails ()
  ) {
    gLogStream <<
      std::endl <<
      "Upon II visitStart (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif
}

void msr2bsrTranslator::visitEnd (S_msrMultipleRest& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMultipleRest" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the multiple rest clone
#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()
      ||
    gGlobalTracingOahGroup->getTraceSegments ()
      ||
    gGlobalTracingOahGroup->getTraceVoices ()
  ) {
    gLogStream <<
      "Creating a clone of multiple rest '" <<
      "'" <<
      elt->asShortString () <<
      "' in voice \"" <<
      fCurrentVoiceClone->getVoiceName () << "\"" <<
      ", fCurrentMultipleRestContentsClone =" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      fCurrentMultipleRestContentsClone;

    --gIndenter;
  }
#endif

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
#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()
      ||
    gGlobalTracingOahGroup->getTraceSegments ()
      ||
    gGlobalTracingOahGroup->getTraceVoices ()
  ) {
    gLogStream <<
      "Creating a new last segment for the remainder of voice \"" <<
      fCurrentVoiceClone->getVoiceName () << "\"" <<
      std::endl;
  }
#endif

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

#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()
      ||
    gGlobalTracingOahGroup->getTraceVoicesDetails ()
  ) {
    gLogStream <<
      std::endl <<
      "Upon visitEnd (S_msrMultipleRest&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrMultipleRestContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMultipleRestContents" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()
      ||
    gGlobalTracingOahGroup->getTraceVoicesDetails ()
  ) {
    gLogStream <<
      std::endl <<
      "Upon visitStart (S_msrMultipleRestContents&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif
}

void msr2bsrTranslator::visitEnd (S_msrMultipleRestContents& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMultipleRestContents" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  --gIndenter;

  // create a multiple rest contents clone
  fCurrentMultipleRestContentsClone =
    elt->createMultipleRestContentsNewbornClone (
      fCurrentVoiceClone);

  // set last segment as the multiple rest contents segment
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Setting current last segment as multiple rest contents segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentMultipleRestContentsClone->
    setMultipleRestContentsSegment (
      fCurrentVoiceClone->
        getVoiceLastSegment ());

#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()
      ||
    gGlobalTracingOahGroup->getTraceVoicesDetails ()
  ) {
    gLogStream <<
      std::endl <<
      "Upon visitEnd (S_msrMultipleRestContents&(), voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      ", contains:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      fCurrentVoiceClone;

    --gIndenter;
  }
#endif
}

//________________________________________________________________________
void msr2bsrTranslator::visitStart (S_msrLayout& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++gIndenter;
}

void msr2bsrTranslator::visitEnd (S_msrLayout& elt)
{
  --gIndenter;

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}
*/


} // namespace
