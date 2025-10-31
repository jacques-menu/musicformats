/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <sstream>
#include <algorithm>    // for_each

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msr2lpsrWae.h"

#include "msr2lpsrComponent.h"

#include "msrAfterGraceNotes.h"
#include "msrArticulations.h"
#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrDoubleTremolos.h"
#include "msrGlissandos.h"
#include "msrLineBreaks.h"
#include "msrPageBreaks.h"
#include "msrRehearsalMarks.h"
#include "msrSlides.h"
#include "msrTablatures.h"
#include "msrTechnicals.h"
#include "msrTempos.h"
#include "msrTuplets.h"
#include "msrVoiceStaffChanges.h"

#include "msrInterface.h"

#include "lpsrBookBlockElements.h"
#include "lpsrContexts.h"
#include "lpsrHeaders.h"
#include "lpsrPartBlocks.h"
#include "lpsrStaves.h"
#include "lpsrScores.h"

#include "lpsrInterface.h"

#include "msr2lpsrTranslator.h"

#include "oahEarlyOptions.h"
#include "oahOah.h"

#include "waeOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
msr2lpsrTranslator::msr2lpsrTranslator ()
  : msr2msrTranslator ()
{}

msr2lpsrTranslator::~msr2lpsrTranslator ()
{}

//________________________________________________________________________
void msr2lpsrTranslator::computeLilypondScoreHeaderFields ()
{
  /*
    the values given through options have precedence
    over those found in the header
  */

  // title
  computeLilypondScoreHeaderTitleAndSubTitle ();

  std::list <std::pair <std::string, std::string>> nameValuePairsList; // JMI TEMP

  // dedication
  std::string
    dedicationFromOption =
      gGlobalLpsr2lilypondOahGroup->getDedication ();

  if (dedicationFromOption.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "dedication",
          dedicationFromOption));
  }
  else {
    std::string
      lilypondDedication =
        fCurrentLpsrScoreHeader->
          getLilypondDedication ();

    if (lilypondDedication.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "dedication",
          lilypondDedication));
    }

    else {
      if (
        gGlobalLpsr2lilypondOahGroup->
          getGenerateCommentedOutVariables ()
      ) {
        nameValuePairsList.push_back (
          std::make_pair (
            "% dedication",
            ""));
      }
    }
  }

  // piece
  std::string
    pieceFromOption =
      gGlobalLpsr2lilypondOahGroup->getPiece ();

  if (pieceFromOption.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "piece",
          pieceFromOption));
  }
  else {
    std::string
      lilypondPiece =
        fCurrentLpsrScoreHeader->getLilypondPiece ();

    if (lilypondPiece.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "piece",
          lilypondPiece));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameValuePairsList.push_back (
          std::make_pair (
            "% piece",
            ""));
      }
    }
  }

  // opus
  std::string
    opusFromOption =
      gGlobalLpsr2lilypondOahGroup->getOpus ();

  if (opusFromOption.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "opus",
          opusFromOption));
  }
  else {
    std::string
      lilypondOpus =
        fCurrentLpsrScoreHeader->getLilypondOpus ();

    if (lilypondOpus.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "opus",
          lilypondOpus));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameValuePairsList.push_back (
          std::make_pair (
            "% opus",
            ""));
      }
    }
  }

  // subtitle
  std::string
    subtitleFromOption =
      gGlobalLpsr2lilypondOahGroup->getSubTitle ();

  if (subtitleFromOption.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "subtitle",
          subtitleFromOption));
  }
  else {
    std::string
      lilypondSubTitle =
        fCurrentLpsrScoreHeader->getLilypondSubTitle ();

    if (lilypondSubTitle.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "subtitle",
          lilypondSubTitle));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameValuePairsList.push_back (
          std::make_pair (
            "% subtitle",
            ""));
      }
    }
  }

  // subsubtitle
  std::string
    subsubtitleFromOption =
      gGlobalLpsr2lilypondOahGroup->getSubSubTitle ();

  if (subsubtitleFromOption.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "subsubtitle",
          subsubtitleFromOption));
  }
  else {
    std::string
      lilypondSubSubTitle =
        fCurrentLpsrScoreHeader->getLilypondSubSubTitle ();

    if (lilypondSubSubTitle.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "subsubtitle",
          lilypondSubSubTitle));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameValuePairsList.push_back (
          std::make_pair (
            "% subsubtitle",
            ""));
      }
    }
  }

  // instrument
  std::string
    instrumentFromOption =
      gGlobalLpsr2lilypondOahGroup->getHeaderInstrument ();

  if (instrumentFromOption.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "instrument",
          instrumentFromOption));
  }
  else {
    std::string
      lilypondInstrument =
        fCurrentLpsrScoreHeader->getLilypondInstrument ();

    if (lilypondInstrument.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "instrument",
          lilypondInstrument));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameValuePairsList.push_back (
          std::make_pair (
            "% instrument",
            ""));
      }
    }
  }

  // meter
  std::string
    meterFromOption =
      gGlobalLpsr2lilypondOahGroup->getHeaderMeter ();

  if (meterFromOption.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "meter",
          meterFromOption));
  }
  else {
    std::string
      lilypondMeter =
        fCurrentLpsrScoreHeader->getLilypondMeter ();

    if (lilypondMeter.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "meter",
          lilypondMeter));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameValuePairsList.push_back (
          std::make_pair (
            "% meter",
            ""));
      }
    }
  }

  // copyright
  std::string
    copyrightFromOption =
      gGlobalLpsr2lilypondOahGroup->getCopyright ();

  if (copyrightFromOption.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "copyright",
          copyrightFromOption));
  }
  else {
    std::string
      lilypondCopyright =
        fCurrentLpsrScoreHeader->getLilypondCopyright ();

    if (lilypondCopyright.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "copyright",
          lilypondCopyright));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameValuePairsList.push_back (
          std::make_pair (
            "% copyright",
            ""));
      }
    }
  }

  // tagline
  std::string
    taglineFromOption =
      gGlobalLpsr2lilypondOahGroup->getTagline ();

  if (taglineFromOption.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "tagline",
          taglineFromOption));
  }
  else {
    std::string
      lilypondTagline =
        fCurrentLpsrScoreHeader->getLilypondTagline ();

    if (lilypondTagline.size ()) {
      nameValuePairsList.push_back (
        std::make_pair (
          "tagline",
          lilypondTagline));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameValuePairsList.push_back (
          std::make_pair (
            "% tagline",
            ""));
      }
    }
  }
}

void msr2lpsrTranslator::computeLilypondScoreHeaderTitleAndSubTitle ()
{
  std::string titleLabel = "title"; // superflous ??? JMI
  std::string titleValue;
  std::string subtitleValue;

  // should the title be used as filename?
  if (
    gMsrOahGroup->getUseFilenameAsWorkCreditTypeTitle ()
  ) { // ill-homed option??? JMI
    titleValue = gServiceRunData->getInputSourceName ();
  }

  else {
    std::string
      titleFromOption =
        gGlobalLpsr2lilypondOahGroup->getTitle ();

    // has the title been supplied in an option?
    if (titleFromOption.size ()) {
      titleValue = titleFromOption;
    }

    else {
      std::string
        lilypondTitle =
          fCurrentLpsrScoreHeader->getLilypondTitle ();

      // is the title contained in the score header?
      if (lilypondTitle.size ()) {
        titleValue = lilypondTitle;
      }

      else {
        std::string
          workCreditTypeTitle =
            fCurrentIdentification->
              getIdentificationWorkCreditTypeTitle (),
          movementTitle =
            fCurrentIdentification->
              getIdentificationMovementTitle ();

        fWorkCreditTypeTitleKnown = ! workCreditTypeTitle.empty ();
        fMovementTitleKnown = ! movementTitle.empty ();

        // have a work title or a movement title
        // been specified in the input?
        if (fWorkCreditTypeTitleKnown || fMovementTitleKnown) {
          if (fWorkCreditTypeTitleKnown && fMovementTitleKnown) {
            // use the work title as the LilyPond title
            titleValue = workCreditTypeTitle;

            // use the movement title as the LilyPond subtitle
            subtitleValue = movementTitle;
          }

          else if (fWorkCreditTypeTitleKnown) {
            // use the work title as the LilyPond title
            titleValue = workCreditTypeTitle;
          }

          else {
            // here only fMovementTitleKnown is true
            // use the movement title as the LilyPond title
            // good heuristics??? JMI
            titleValue = movementTitle;
          }
        }

        else if (
          gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()
        ) {
           titleLabel = "% title";
        }
      }
    }
  }

  // set the score header title
  fCurrentLpsrScoreHeader->
    setLilypondTitle (titleValue);

  if (subtitleValue.size ()) {
    fCurrentLpsrScoreHeader->
      setLilypondTitle (subtitleValue);
  }
}

//________________________________________________________________________
S_lpsrScore msr2lpsrTranslator::translateMsrToLpsr (
  const S_msrScore&          theMsrScore,
  const S_mfcMultiComponent& multiComponent)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    theMsrScore != nullptr,
    "theMsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // the MSR score we're visiting
  fVisitedMsrScore = theMsrScore;

  // create another embedded MSR score for the LPSR score
  fCurrentMsrScoreClone =
    msrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
      "msrScore::create()");

  // create the resulting LPSR score
  fResultingLpsr =
    lpsrScore::create (
      K_MF_INPUT_LINE_UNKNOWN_,
      fCurrentMsrScoreClone,
      multiComponent);

  // create the current book block
  fCurrentLpsrBookBlock =
    lpsrBookBlock::create (
      K_MF_INPUT_LINE_UNKNOWN_);

  // set its header to that of the visited LPSR score JMI ???
  fCurrentLpsrBookBlock->
    setBookBlockHeader (
      fCurrentLpsrScoreHeader);

  // append it to the current book blocks list
  fResultingLpsr->
    appendBookBlockToBookBlocksList (
      fCurrentLpsrBookBlock);

  // create the current score block if relevant
  switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
    case lpsrScoreOutputKind::kScoreOutputScoreOnly:
    case lpsrScoreOutputKind::kScoreOutputScoreAndParts:
    case lpsrScoreOutputKind::kScoreOutputPartsAndScore:
    case lpsrScoreOutputKind::kScoreOutputScoreAndPartsOneFile:
    case lpsrScoreOutputKind::kScoreOutputPartsAndScoreOneFile:
      {
        // create the current score block
        fCurrentScoreBlock =
          lpsrScoreBlock::create (
            K_MF_INPUT_LINE_UNKNOWN_);

        // append it to the book block elements list
        fCurrentLpsrBookBlock->
          appendLpsrScoreBlockToBookBlockElementsList (
            fCurrentScoreBlock);
      }
      break;
    case lpsrScoreOutputKind::kScoreOutputPartsOnly:
    case lpsrScoreOutputKind::kScoreOutputPartsOnlyOneFile:
      break;
  } // switch

  // create a msrScore browser
  msrBrowser<msrScore> browser (this);

  // set the parts browsing order
  theMsrScore->
    setStavesBrowingOrderKind (
      msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesRegularsFiguredBasses);

  // browse the visited MSR score
  browser.browse (*fVisitedMsrScore);

  // display the LPSR score if requested
  // ------------------------------------------------------

  if (gLpsrOahGroup->getDisplayLpsr ()) {
    displayLpsrScore (
      fResultingLpsr,
      gMsrOahGroup,
      gLpsrOahGroup,
      gLanguage->displayTheLPSRAsText ());
  }

  if (gLpsrOahGroup->getDisplayLpsrFull ()) {
    displayLpsrScoreFull (
      fResultingLpsr,
      gMsrOahGroup,
      gLpsrOahGroup,
      gLanguage->displayTheLPSRAsText ());
  }

  //display the MSR embeded in the LPSR score if requested
  // ------------------------------------------------------

  if (gLpsrOahGroup->getDisplayLpsrEmbeddedMsr ()) {
    displayMsrScore (
      fResultingLpsr->getEmbeddedMsrScore (),
      gLanguage->displayTheMSREmbededInTheLPSRAsText ());
  }

  // display the LPSR score summary if requested
  // ------------------------------------------------------

  if (gLpsrOahGroup->getDisplayLpsrSummary ()) {
    // display the score summary
    displayLpsrScoreSummary (
      fResultingLpsr,
      gMsrOahGroup,
      gLpsrOahGroup,
      gLanguage->displayASummaryOfTheLPSR ());
  }

  // display the LPSR score names if requested
  // ------------------------------------------------------

  if (gLpsrOahGroup->getDisplayLpsrNames ()) {
    // display the score name
    displayLpsrScoreNames (
      fResultingLpsr,
      gMsrOahGroup,
      gLpsrOahGroup,
      gLanguage->displayTheNamesInTheLPSR ());
  }

  // display the LPSR flat view if requested
  // ------------------------------------------------------

  if (gLpsrOahGroup->getDisplayLpsrFlatView ()) {
    // display the score name
    displayLpsrScoreFlatView (
      fResultingLpsr,
      gMsrOahGroup,
      gLpsrOahGroup,
      gLanguage->displayAFlatViewOfTheLPSR ());
  }

  // display the LPSR slices if requested
  // ------------------------------------------------------

  if (gLpsrOahGroup->getDisplayLpsrMeasuresSlices ()) {
    // display the score name
    displayLpsrScoreSlices (
      fResultingLpsr,
      gMsrOahGroup,
      gLpsrOahGroup,
      gLanguage->displayTheSlicesOfTheLPSR ());
  }

  // forget about the visited MSR score
  fVisitedMsrScore = nullptr;

  return fResultingLpsr;
}

//________________________________________________________________________
void msr2lpsrTranslator::displayCurrentOnGoingValues ()
{
  msr2msrTranslator::displayCurrentOnGoingValues ();

//   gLog <<
//     "Current msr2lpsrTranslator ongoing values:" <<
//     std::endl;
//
//   ++gIndenter;
//
//   constexpr int fieldWidth = 25;
//
//   gLog << std::left <<
//     std::setw (fieldWidth) <<
//     "fOnGoingSyllableExtend" << ": " << fOnGoingSyllableExtend <<
//     std::endl;
//
//   --gIndenter;
}

//________________________________________________________________________
void msr2lpsrTranslator::populateHeaderFromIdentification (
  const S_lpsrHeader&        header,
  const S_msrIdentification& identification)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    header != nullptr,
    "header is NULL");
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    identification != nullptr,
    "identification is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
    std::stringstream ss;

    ss <<
      "Populating LPSR header from identification " <<
// JMI      identification->asString () <<
      std::endl <<
      identification;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  header->setHeaderIdentification (identification);

  std::string workCreditTypeTitle = identification->getIdentificationWorkCreditTypeTitle ();
  if (workCreditTypeTitle.size ()) {
    header->setLilypondTitle (workCreditTypeTitle);
  }

  std::string opus = identification->getIdentificationOpus ();
  if (opus.size ()) {
    header->setLilypondOpus (opus);
  }

  std::string scoreInstrument = identification->getIdentificationScoreInstrument ();
  if (scoreInstrument.size ()) {
    header->setLilypondInstrument (scoreInstrument);
  }

  // setLilypondCopyright ??? JMI

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
    mfIndentedStringStream iss;

    iss <<
      "Resulting LPSR header:" <<
      '\n';
    ++gIndenter;
    iss <<
      header;
    --gIndenter;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      iss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2lpsrTranslator::setPaperIndentsIfNeeded (
  const S_msrScaling& scaling)
{
  S_lpsrPaper
    paper =
      fResultingLpsr->getScorePaper ();

  int
    scorePartGroupNamesMaxLength =
      fCurrentMsrScoreClone->
        getScorePartGroupNamesMaxLength ();

  int
    scorePartNamesMaxLength =
      fCurrentMsrScoreClone->
        getScorePartNamesMaxLength ();

  int
    scoreInstrumentNamesMaxLength =
      fCurrentMsrScoreClone->
        getScoreInstrumentNamesMaxLength ();

  int
    scoreInstrumentAbbreviationsMaxLength =
      fCurrentMsrScoreClone->
        getScoreInstrumentAbbreviationsMaxLength ();

  // compute the maximum value
  int maxValue = -1;

  if (scorePartGroupNamesMaxLength > maxValue) {
    maxValue = scorePartGroupNamesMaxLength;
  }

  if (scorePartNamesMaxLength > maxValue) {
    maxValue = scorePartNamesMaxLength;
  }

  if (scoreInstrumentNamesMaxLength > maxValue) {
    maxValue = scoreInstrumentNamesMaxLength;
  }

  // compute the maximum short value
  int maxShortValue = -1;

  if (scoreInstrumentAbbreviationsMaxLength > maxShortValue) {
    maxShortValue = scoreInstrumentAbbreviationsMaxLength;
  }

  // heuristics to determine the number of characters per centimeter
//  float charactersPerCemtimeter = 4.0; // JMI ???

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGeometry ()) {
    mfIndentedStringStream iss;

  /*
    // get the paper width
    S_msrLength
      paperWidth =
        scaling->
          getPageLayout ()->
            getPageWidth ();

    if (paperWidth) {
      iss <<
        "setPaperIndentsIfNeeded()" << // JMI ???
        '\n';
    }
*/
    ++gIndenter;

//     constexpr int fieldWidth = 40;

//     iss << std::left <<
    iss <<
//       std::setw (fieldWidth) <<
      "scorePartGroupNamesMaxLength" << ": " <<
      scorePartGroupNamesMaxLength <<
      '\n' <<
//       std::setw (fieldWidth) <<
      "scorePartNamesMaxLength" << ": " <<
      scorePartNamesMaxLength <<
      '\n' <<

//       std::setw (fieldWidth) <<
      "scoreInstrumentNamesMaxLength" << ": " <<
      scoreInstrumentNamesMaxLength <<
      '\n' <<
//       std::setw (fieldWidth) <<
      "scoreInstrumentAbbreviationsMaxLength" << ": " <<
      scoreInstrumentAbbreviationsMaxLength <<
      '\n' <<

//       std::setw (fieldWidth) <<
      "maxValue" << ": " <<
      maxValue <<
      '\n' <<
//       std::setw (fieldWidth) <<
      "maxShortValue" << ": " <<
      maxShortValue <<
      /* JMI
      '\n' <<

      std::setw (fieldWidth) <<
      "charactersPerCemtimeter" << ": " <<
      charactersPerCemtimeter <<
      */
      '\n';
/*
    iss << std::left <<
      std::setw (fieldWidth) <<
      "paperWidth" << ": ";
    if (paperWidth) {
      iss << paperWidth;
    }
    else {
      iss << "[NONE]";
    }
    iss << '\n';
*/
    --gIndenter;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      iss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  // set indent if relevant
  if (maxValue > 0) {
    paper->
      setIndent (
        maxValue / charactersPerCemtimeter);
  }

  // set short indent if relevant
  if (maxShortValue > 0) {
    paper->
      setShortIndent (
        maxShortValue / charactersPerCemtimeter);
  }
  */
}

void msr2lpsrTranslator::circumventLilyPond34IssueIfRelevant (
  S_msrGraceNotesGroup& graceNotesGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "+++ circumventLilyPond34IssueIfRelevant()" <<
      ", fCurrentNonGraceNoteClone: ";

    if (fCurrentNonGraceNoteClone) {
      ss <<
        fCurrentNonGraceNoteClone->asString ();
    }
    else {
      ss << "[NULL]";
    }

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get the note this grace notes group is attached to
  S_msrNote
    noteTheGraceNotesGroupIsAttachedTo =
      graceNotesGroup->
        getGraceNotesGroupUpLinkToNote ();

  if (! noteTheGraceNotesGroupIsAttachedTo) {
    std::stringstream ss;

    ss <<
      "grace notes group '" << graceNotesGroup->asShortString () <<
      "' has an empty note upLink";

    msr2lpsrInternalError (
      gServiceRunData->getInputSourceName (),
       graceNotesGroup->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // is noteTheGraceNotesGroupIsAttachedTo the first one in its voice?
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) { // JMI
    std::stringstream ss;

    ss <<
      "--> noteTheGraceNotesGroupIsAttachedTo in voice clone " <<
      fCurrentVoiceClone->getVoiceName () <<
      " is '";

    if (noteTheGraceNotesGroupIsAttachedTo) {
      ss <<
        noteTheGraceNotesGroupIsAttachedTo->asShortString ();
    }
    else {
      ss << "[NULL]";
    }

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) { // JMI
    std::stringstream ss;

    ss <<
      "The first note of voice clone " <<
      fCurrentVoiceClone->getVoiceName () <<
      " is ";

    if (fFirstNoteCloneInVoice) {
      ss <<
        fFirstNoteCloneInVoice->asShortString ();
    }
    else {
      ss << "[NULL]";
    }

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   gLog <<
//     "==> fCurrentVoiceClone:" <<
//     std::endl;
//   gIndenter++;
//   gLog <<
//     fCurrentVoiceClone <<
//     std::endl;
//   gIndenter--;

  // fetch the original voice first non grace note
  S_msrNote
    originalVoiceFirstNonGraceNote =
      fCurrentVoiceOriginal->
        fetchVoiceFirstNonGraceNote ();

//   gLog <<
//     "==> originalVoiceFirstNonGraceNote:" <<
//     std::endl;
//   gIndenter++;
//   gLog <<
//     originalVoiceFirstNonGraceNote <<
//     std::endl;
//   gIndenter--;

  if (originalVoiceFirstNonGraceNote) { // JMI 0.9.72
    if (
//       true || noteTheGraceNotesGroupIsAttachedTo == originalVoiceFirstNonGraceNote // JMI ISSUE34
      noteTheGraceNotesGroupIsAttachedTo == originalVoiceFirstNonGraceNote
    ) {
      // don't createSkipGraceNotesGroupClone() is there's only a single voice
      // JMI ??? 0.9.70 maybe this criterion should be refined
      S_msrStaff
        currentVoiceOriginalStaff =
          fCurrentVoiceOriginal->
            getVoiceUpLinkToStaff ();

      if (currentVoiceOriginalStaff->getStaffAllVoicesList ().size () > 1) {
        // LilyPond_Issue_34 should be worked around by creating
        // skip grace notes in the other voices of the part

        // create the skip grace notes group
  #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceGraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Creating a skip clone of grace notes group " <<
              graceNotesGroup->asShortString () <<
              " to work around LilyPond_Issue_34";

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
  #endif // MF_TRACE_IS_ENABLED

        fCurrentSkipGraceNotesGroup =
          graceNotesGroup->
            createSkipGraceNotesGroupClone ();
      }
    }
  }
  else {
//     abort ();
  }

  // addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded()
  // will append the same skip grace notes to the ofhter voices if needed
  // in visitEnd (S_msrPart&)
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrScore" <<
      ", line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED


  // populate the measure numbers
  fVisitedMsrScore->
    setScoreFirstMeasureNumber (
      elt->getScoreFirstMeasureNumber ());
  fVisitedMsrScore->
    setScoreLastMeasureNumber (
      elt->getScoreLastMeasureNumber ());

  // fetch score header IDENTIFICATIONL JMI
  fCurrentLpsrScoreHeader =
    fResultingLpsr->getScoreHeader();

//   is auto-voices to be generated?
//   if (gGlobalLpsr2lilypondOahGroup->getAutoVoicesSchemeFunctionIsNeeded ()) {
//     fResultingLpsr->
//       this score needs the 'auto-voices' Scheme functions
//       setAutoVoicesSchemeFunctionIsNeeded ();
//   }

  // are the rests to be merged?
  if (gGlobalLpsr2lilypondOahGroup->getMergeStaffCommonRests ()) {
    fResultingLpsr->
      // this score needs the 'merge rests' Scheme functions
      setMergeStaffCommonRestsIsNeeded ();
  }

  // is the LilyPond macro 'boxAroundNextBarNumber' to be generated?
  if (
    gGlobalLpsr2lilypondOahGroup->getBoxAroundAllBarNumbers ()
      ||
    gGlobalLpsr2lilypondOahGroup->getBoxAroundBarNumberSet ().size ()
  ) {
    fResultingLpsr->
      // this score needs the 'boxAroundNextBarNumber' Scheme function
      setBoxAroundNextBarNumberIsNeeded ();
  }

  // is the Scheme function 'whiteNoteHeads' to be generated?
  if (gGlobalLpsr2lilypondOahGroup->getWhiteNoteHeads ()) {
    fResultingLpsr->
      // this score needs the 'whiteNoteHeads' Scheme function
      setWhiteNoteHeadsIsNeeded ();
  }

  // is the Scheme function 'jazzChordsDisplay' to be generated?
  if (gGlobalLpsr2lilypondOahGroup->getJazzChordsDisplay ()) {
    fResultingLpsr->
      // this score needs the 'jazzChordsDisplay' Scheme function
      setJazzChordsDisplayIsNeeded ();
  }

  // is the LilyJazz font file to be included?
  if (gGlobalLpsr2lilypondOahGroup->getLilyJazz ()) {
    fResultingLpsr->
      // this score needs the 'LilyJazz font file include' Scheme function
      setLilyJazzFontFileIncludeIsNeeded ();
  }

  // is jiǎnpǔ notation to be generated?
  if (gGlobalLpsr2lilypondOahGroup->getJianpu ()) {
    fResultingLpsr->
      // this score needs the 'jianpu file include' Scheme function
      setJianpuFileIncludeIsNeeded ();
  }

  // get the print notes head RGB color atom
  S_oahColorRGBAtom
    ledgerLinesColorRGBAtom =
      gGlobalLpsr2lilypondOahGroup-> // JMI BLARK put that in gGlobalMsr2lpsrOahGroup
        getLedgerLinesColorRGBAtom ();

  // has the atom been used?
  if (ledgerLinesColorRGBAtom->getSelected ()) {
    // this score needs the 'colored ledger lines' Scheme function
    fResultingLpsr->
      setColoredLedgerLinesIsNeeded ();
  }

/* JMI 0.9.69
  // push it onto this visitors's stack,
  // making it the current partGroup
  fPartGroupBlocksStack.push_front (
    partGroupBlock);
    */
}

void msr2lpsrTranslator::visitEnd (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrScore" <<
      ", line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set ident and short indent if needed
  setPaperIndentsIfNeeded ( // JMI ??? BLARK
    elt->getScaling ());

/* JMI
  // get top level partgroup block from the stack
  S_lpsrPartGroupBlock
    partGroupBlock =
      fPartGroupBlocksStack.front ();

  // pop it from the stack
  fPartGroupBlocksStack.front ();

  // the stack should now be empty
  if (fPartGroupBlocksStack.size ()) {
    msr2lpsrInternalError (
      1,
      "the partGroup block stack is not exmpty at the end of the visit");
  }
   */
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrIdentification& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set the current identification
  fCurrentIdentification = elt;

  // store it in the current MSR score clone
  fCurrentMsrScoreClone->
    setIdentification (
      fCurrentIdentification);

  // use it to populate the header // JMI
  populateHeaderFromIdentification (
    fCurrentLpsrScoreHeader,
    fCurrentIdentification);

  // compute the header fields // JMI
  computeLilypondScoreHeaderFields ();

  fOnGoingIdentification = true;
}

void msr2lpsrTranslator::visitEnd (S_msrIdentification& elt)
{
  fOnGoingIdentification = false;

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrScaling& elt)
{
// #ifdef MF_TRACE_IS_ENABLED
//   if (gMsrOahGroup->getTraceMsrVisitors ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Start visiting msrScaling" <<
//       ", line " << elt->getInputLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // create a scaling clone
//   S_msrScaling
//     scalingClone =
//       elt->createMsrScalingNewbornClone ();
//
//   // register it in the current MSR score clone
//   fCurrentMsrScoreClone-> // JMI BLARK ???
//     setScaling (
//       scalingClone);

  msr2msrTranslator::visitStart (elt);

  // get LPSR score paper
  S_lpsrPaper
    paper =
      fResultingLpsr->getScorePaper ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    paper != nullptr,
    "paper is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set the current book block's paper as a newborn clone of paper
  fCurrentLpsrBookBlock ->
    setBookBlockPaper (
      paper->
        createPaperNewbornClone ());

  // get LPSR score layout
  S_lpsrLayout
    scoreLayout =
      fResultingLpsr->getScoreLayout ();
}

void msr2lpsrTranslator::visitEnd (S_msrScaling& elt)
{
  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrScaling" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrCredit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentCredit = elt;

  // set elt as credit of the embedded MSR score of the LPSR score
  fResultingLpsr->
    getEmbeddedMsrScore ()->
      appendCreditToScore (fCurrentCredit);

  /* this variable is not used yet
  int
    creditPageNumber = elt->getCreditPageNumber ();
  */

  msrCreditTypeKind
    creditTypeKind = elt->getCreditTypeKind ();

  switch (creditTypeKind) {
    case msrCreditTypeKind::kCreditType_UNKNOWN_:
      break;

    case msrCreditTypeKind::kCreditTypeNumber:
      break;

    case msrCreditTypeKind::kCreditTypeTitle:
      break;
    case msrCreditTypeKind::kCreditTypeSubtitle:
      break;

    case msrCreditTypeKind::kCreditTypeComposer:
      break;
    case msrCreditTypeKind::kCreditTypeArranger:
      break;
    case msrCreditTypeKind::kCreditTypeLyricist:
      break;

    case msrCreditTypeKind::kCreditTypeRights:
      break;

    case msrCreditTypeKind::kCreditTypePartName:
      break;

    case msrCreditTypeKind::kCreditTypeOther:
      break;
  } // switch
}

void msr2lpsrTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // don't append it to the current credit, since the latter is no clone
  /* JMI 0.9.64
  fCurrentCredit->
    appendCreditWordsToCredit (
      elt);
      */

  /* the following variables are for further use:

    std::string
      creditWordsContents =
        elt->getCreditWordsContents ();

    float
      creditWordsDefaultX =
        elt->getCreditWordsDefaultX ();

    float
      creditWordsDefaultY =
        elt-> getCreditWordsDefaultY ();

    std::string
      creditWordsFontFamily =
        elt->getCreditWordsFontFamily ();

    float
      creditWordsFontSize =
        elt->getCreditWordsFontSize ();

    msrFontWeightKind
      creditWordsFontWeightKind =
        elt->getCreditWordsFontWeightKind ();

    msrFontStyleKind
      creditWordsFontStyleKind =
        elt->getCreditWordsFontStyleKind ();

    msrJustifyKind
      creditWordsJustifyKind =
        elt->getCreditWordsJustifyKind ();

    msrHorizontalAlignmentKind
      creditWordsHorizontalAlignmentKind =
         elt->getCreditWordsHorizontalAlignmentKind ();

    msrVerticalAlignmentKind
      creditWordsVerticalAlignmentKind =
         elt->getCreditWordsVerticalAlignmentKind ();

    msrXMLLangKind
      creditWordsXMLLang =
        elt->getCreditWordsXMLLang ();
  */
}

void msr2lpsrTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPartGroup " <<
      elt->fetchPartGroupInformationForTrace () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a partGroup clone
  // the current partGroup clone, i.e. the top of the stack,
  // is the upLink of the new one if it exists
  S_msrPartGroup
    partGroupClone =
      elt->createPartGroupNewbornClone (
        fPartGroupsStack.size ()
          ? fPartGroupsStack.front ()
          : nullptr,
        fResultingLpsr->getEmbeddedMsrScore ());

  // is this part group the implicit outer-most one?
  switch (elt->getPartGroupImplicitKind ()) {
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePartGroups ()) {
      std::stringstream ss;

      ss <<
          "Adding part group clone " <<
          partGroupClone->fetchPartGroupInformationForTrace () <<
          " to the current MSR score clone" <<
          ", line " << elt->getInputLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fCurrentMsrScoreClone->
        addPartGroupToScore (
          partGroupClone);
      break;

    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
      break;
  } // switch

  // push partGroupClone onto this visitors's stack,
  // making it the current partGroup
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Pushing part group clone " <<
      partGroupClone->fetchPartGroupInformationForTrace () <<
      " onto the stack" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartGroupsStack.push_front (
    partGroupClone);

  // create a partGroup block refering to the part group clone
  S_lpsrPartGroupBlock
    partGroupBlock =
      lpsrPartGroupBlock::create (
        partGroupClone);

  // push it onto this visitors's stack,
  // making it the current partGroup block
  fPartGroupBlocksStack.push_front (
    partGroupBlock);

  // don't append the partgroup block to the score/bookpart block now:
  // this will be done when it gets popped from the stack

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayPartGroupsStack (
      "visitStart (S_msrPartGroup& elt)");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msr2lpsrTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPartGroup " <<
      elt->fetchPartGroupInformationForTrace () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fPartGroupsStack.size () > 0) {
    // fetch the current part group at the top of the stack
    S_msrPartGroup
      currentPartGroup =
        fPartGroupsStack.front ();

    // append the current part group to the one one level higher,
    // i.e. the new current part group

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePartGroups ()) {
      std::stringstream ss;

      ss <<
        "Appending part group clone " <<
        currentPartGroup->fetchPartGroupInformationForTrace () <<
        " to the current part group" <<
        ", line " << elt->getInputLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

//     currentPartGroup-> // JMI 0.9.69
//       appendNestedPartGroupToPartGroup (
//         elt);

    // pop current partGroup from this visitors's stack
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePartGroups ()) {
      std::stringstream ss;

      ss <<
        "Popping part group clone " <<
        fPartGroupsStack.front ()->fetchPartGroupInformationForTrace () <<
        " from the stack" <<
        ", line " << elt->getInputLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
    // sanity check
    mfAssert (
      __FILE__, mfInputLineNumber (__LINE__),
      ! fPartGroupBlocksStack.empty (),
      "fPartGroupBlocksStack is EMPTY");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

    // fetch the current part group block
    S_lpsrPartGroupBlock
      currentPartGroupBlock =
        fPartGroupBlocksStack.front ();

    if (fPartGroupBlocksStack.size () == 1) {
      // add the current partgroup clone to the LPSR score's parallel music
      // if it is the top-level one, i.e it's alone in the stack

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePartGroups ()) {
        std::stringstream ss;

        ss <<
          "Adding part group block clone for part group " <<
          currentPartGroupBlock->
            getPartGroup ()->
              fetchPartGroupInformationForTrace () <<
          " to LPSR score";

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append the current partgroup block to the current score block if relevant
      switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
        case lpsrScoreOutputKind::kScoreOutputScoreOnly:
        case lpsrScoreOutputKind::kScoreOutputScoreAndParts:
        case lpsrScoreOutputKind::kScoreOutputPartsAndScore:
        case lpsrScoreOutputKind::kScoreOutputScoreAndPartsOneFile:
        case lpsrScoreOutputKind::kScoreOutputPartsAndScoreOneFile:
          {
  #ifdef MF_SANITY_CHECKS_ARE_ENABLED
            // sanity check
            mfAssert (
              __FILE__, mfInputLineNumber (__LINE__),
              fCurrentScoreBlock != nullptr,
              "fCurrentScoreBlock is NULL");
  #endif // MF_SANITY_CHECKS_ARE_ENABLED

            // append the current partgroup block to the current score block
            // if it is the top-level one, i.e it's alone in the stack JMI
            // JMI BOF if (fPartGroupBlocksStack.size () == 1)
  #ifdef MF_TRACE_IS_ENABLED
            if (gLpsrOahGroup->getTraceLpsrBlocks ()) {
              std::stringstream ss;

              ss <<
                "Appending part group block for part group " <<
                currentPartGroupBlock->
                  getPartGroup ()->
                    fetchPartGroupInformationForTrace () <<
                " to the current score block '" <<
                fCurrentScoreBlock->asShortString () <<
                '\'' <<
                std::endl;

              gWaeHandler->waeTraceWithoutInputLocation (
                __FILE__, mfInputLineNumber (__LINE__),
                ss.str ());
            }
  #endif // MF_TRACE_IS_ENABLED
            fCurrentScoreBlock->
              appendPartGroupBlockToScoreBlock (
                fPartGroupBlocksStack.front ());
          }
          break;
        case lpsrScoreOutputKind::kScoreOutputPartsOnly:
        case lpsrScoreOutputKind::kScoreOutputPartsOnlyOneFile:
          break;
      } // switch

      // append the current partgroup block to the current bookpart block if relevant
      switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
        case lpsrScoreOutputKind::kScoreOutputScoreOnly:
          break;
        case lpsrScoreOutputKind::kScoreOutputScoreAndParts:
        case lpsrScoreOutputKind::kScoreOutputPartsAndScore:
        case lpsrScoreOutputKind::kScoreOutputScoreAndPartsOneFile:
        case lpsrScoreOutputKind::kScoreOutputPartsAndScoreOneFile:
        case lpsrScoreOutputKind::kScoreOutputPartsOnly:
        case lpsrScoreOutputKind::kScoreOutputPartsOnlyOneFile:
          {
  #ifdef MF_SANITY_CHECKS_ARE_ENABLED
            // sanity check
            mfAssert (
              __FILE__, mfInputLineNumber (__LINE__),
              fCurrentBookPartBlock != nullptr,
              "fCurrentBookPartBlock is NULL");
  #endif // MF_SANITY_CHECKS_ARE_ENABLED

            // append the current partgroup block to the current bookpart block
            // if it is the top-level one, i.e it's alone in the stack JMI
            // JMI BOF if (fPartGroupBlocksStack.size () == 1)
  #ifdef MF_TRACE_IS_ENABLED

            if (gLpsrOahGroup->getTraceLpsrBlocks ()) {
              std::stringstream ss;

              ss <<
                "Appending part group block for part group " <<
                currentPartGroupBlock->
                  getPartGroup ()->
                    fetchPartGroupInformationForTrace () <<
                " to the current bookpart block '" <<
                fCurrentScoreBlock->asShortString () <<
                '\'' <<
                std::endl;

              gWaeHandler->waeTraceWithoutInputLocation (
                __FILE__, mfInputLineNumber (__LINE__),
                ss.str ());
            }
  #endif // MF_TRACE_IS_ENABLED

            fCurrentBookPartBlock->
              appendPartGroupBlockToBookPartBlock (
                fPartGroupBlocksStack.front ());
          }
          break;
      } // switch

      // pop current partGroup block from this visitors's stack,
      // only now to restore the appearence order
      fPartGroupBlocksStack.pop_front ();
    }

    else {
      // pop current partGroup block from this visitors's stack
  #ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePartGroups ()) {
        std::stringstream ss;

        ss <<
          "Popping part group block clone for part group " <<
          currentPartGroupBlock->
            getPartGroup ()->
              fetchPartGroupInformationForTrace () <<
          " from stack";

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
  #endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, mfInputLineNumber (__LINE__),
        ! fPartGroupBlocksStack.empty (),
        "fPartGroupBlocksStack is EMPTY");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      fPartGroupBlocksStack.pop_front ();

      // append the current part group block to the one one level higher,
      // i.e. the new current part group block
      fPartGroupBlocksStack.front ()->
        appendElementToPartGroupBlock (
          currentPartGroupBlock);
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTracePartGroups ()) {
      displayPartGroupsStack (
        "visitEnd (S_msrPartGroup& elt)");
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrPart& elt)
{
  std::string
    partdNameForTrace =
      elt->fetchPartNameForTrace ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPart " <<
      partdNameForTrace <<
      ", line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        std::endl <<
        "<!--=== part \"" << partdNameForTrace << "\"" <<
        ", line " <<  elt->getInputLineNumber () << " ===-->";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

  // register the current part in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentPartIDAndName (
      elt->fetchPartIDAndName ());

  ++gIndenter;

  // create a part clone
  fCurrentPartClone =
    elt->createPartNewbornClone (
      fPartGroupsStack.front ());

  // add it to the partGroup clone
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Adding part clone " <<
      fCurrentPartClone->fetchPartNameForTrace () <<
      " to part group clone \"" <<
      fPartGroupsStack.front ()->fetchPartGroupInformationForTrace () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartGroupsStack.front ()->
    appendPartToPartGroup (fCurrentPartClone);

  // create a part block
  fCurrentPartBlock =
    lpsrPartBlock::create (
      fCurrentPartClone);

  // append it to the current partGroup block
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Appending part block " <<
      fPartGroupsStack.front ()->fetchPartGroupInformationForTrace () <<
      " to part group blocks stack top";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartGroupBlocksStack.front ()->
    appendElementToPartGroupBlock (
      fCurrentPartBlock);

  // create a bookpart block if relevant
  switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
    case lpsrScoreOutputKind::kScoreOutputScoreOnly:
      break;
    case lpsrScoreOutputKind::kScoreOutputScoreAndParts:
    case lpsrScoreOutputKind::kScoreOutputPartsAndScore:
    case lpsrScoreOutputKind::kScoreOutputScoreAndPartsOneFile:
    case lpsrScoreOutputKind::kScoreOutputPartsAndScoreOneFile:
    case lpsrScoreOutputKind::kScoreOutputPartsOnly:
    case lpsrScoreOutputKind::kScoreOutputPartsOnlyOneFile:
      {
        // create the current score block
        fCurrentBookPartBlock =
          lpsrBookPartBlock::create (
             elt->getInputLineNumber ());

        // append it to the book block elements list
        fCurrentLpsrBookBlock->
          appendLpsrBookPartBlockToBookBlockElementsList (
            fCurrentBookPartBlock);
      }
      break;
  } // switch
}

void msr2lpsrTranslator::visitEnd (S_msrPart& elt)
{
  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPart " <<
      elt->fetchPartNameForTrace () <<
      ", line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string
    partInstrumentAbbreviation =
      fCurrentPartClone->
        getPartInstrumentAbbreviation ();

  // populate part instrument short name if empty and possible
  if (partInstrumentAbbreviation.empty ()) {
    std::string
      partAbbreviation =
        elt->getPartAbbreviation ();

    fCurrentPartClone->
      setPartInstrumentAbbreviation (
        partAbbreviation);
  }

  // finalize the current part clone
  fCurrentPartClone->
    finalizePartClone (
       elt->getInputLineNumber ());

  // handle the hidden measure and barLine elements
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    displayPartHiddenMeasureAndBarLineDescrList ();
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI handlePartHiddenMeasureAndBarLineDescrList ();

  // handle skip grace notes if needed
  if (fCurrentSkipGraceNotesGroup) {
    // add it ahead of the other voices in the part if needed
    fCurrentPartClone->
      addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded ( // JMI only if there's more than one voice??? 0.9.70
        fCurrentVoiceClone,
        fCurrentSkipGraceNotesGroup);

    // forget about this skip grace notes group
    fCurrentSkipGraceNotesGroup = nullptr;
  }

  // forget about the current part block
  fCurrentPartBlock = nullptr;

  // forget about the current book part block
  fCurrentBookPartBlock = nullptr;
}

//________________________________________________________________________
/* JMI
void msr2lpsrTranslator::visitStart (S_msrStaffLinesNumber& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaffLinesNumber" <<
      ", line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a staff lines number clone
  fCurrentStaffLinesNumberClone =
    elt->
      createStaffLinesNumberNewbornClone ();
}
*/

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrStaff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffPathLikeName () << "\"" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the current staff number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentStaveNumber (
      elt->getStaffNumber ());

  ++gIndenter;

  switch (elt->getStaffKind ()) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        // create a staff block
        fCurrentStaffBlock =
          lpsrStaffBlock::create (
            fCurrentStaffClone);

        std::string
          partName =
            fCurrentPartClone->getPartName (),
          partAbbreviation =
            fCurrentPartClone->getPartAbbreviation ();

        std::string staffBlockInstrumentName;
        std::string staffBlockShortInstrumentName;

        // don't set instrument name nor short instrument name // JMI
        // if the staff belongs to a piano part where they're already set
        if (! partName.size ()) {
          staffBlockInstrumentName = partName;
        }
        if (! partAbbreviation.size ()) {
          staffBlockShortInstrumentName = partAbbreviation;
        }

        if (staffBlockInstrumentName.size ()) {
          fCurrentStaffBlock->
            setStaffBlockInstrumentName (staffBlockInstrumentName);
        }

        if (staffBlockShortInstrumentName.size ()) {
          fCurrentStaffBlock->
            setStaffBlockShortInstrumentName (staffBlockShortInstrumentName);
        }

        // append the staff block to the current part block
        fCurrentPartBlock->
          appendStaffBlockToPartBlock (
            fCurrentStaffBlock);

        // handle the current staff block
        switch (gGlobalMsr2lpsrOahGroup->getScoreOutputKind ()) {
          case lpsrScoreOutputKind::kScoreOutputScoreOnly: // default value
            break;
          case lpsrScoreOutputKind::kScoreOutputScoreAndParts:
          case lpsrScoreOutputKind::kScoreOutputPartsAndScore:
          case lpsrScoreOutputKind::kScoreOutputScoreAndPartsOneFile:
          case lpsrScoreOutputKind::kScoreOutputPartsAndScoreOneFile:
            {
            /* JMI
              // create the current score block
              fCurrentScoreBlock =
                lpsrScoreBlock::create (
                  K_MF_INPUT_LINE_UNKNOWN_);

              // append it to the book block elements list
              fCurrentLpsrBookBlock->
                appendLpsrScoreBlockToBookBlockElementsList (
                  fCurrentScoreBlock);
                */
            }
            break;
          case lpsrScoreOutputKind::kScoreOutputPartsOnly:
          case lpsrScoreOutputKind::kScoreOutputPartsOnlyOneFile:
            break;
        } // switch

        fOnGoingStaff = true;
      }
      break;

    case msrStaffKind::kStaffKindHarmonies:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        // register it as the part harmonies staff
        fCurrentPartClone->
          setPartHarmoniesStaff (fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        // register it as the part figured bass staff
        fCurrentPartClone->
          setPartFiguredBassStaff (fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;
  } // switch
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrVoice& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrVoice \"" <<
      elt->asString () << "\"" <<
      ", line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string voiceName = elt->getVoiceName ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<!--=== voice \"" << voiceName << "\"" <<
      ", line " << elt->getInputLineNumber () << " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // remember current voice
  fCurrentVoiceOriginal = elt;

  // register the current voice number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentVoiceNumber (
      fCurrentVoiceOriginal->getVoiceNumber ());

  ++gIndenter;

  // create a voice clone
  fCurrentVoiceClone =
    fCurrentVoiceOriginal->
      createVoiceNewbornClone (
        fCurrentStaffClone);

  // add it to the staff clone
  fCurrentStaffClone->
    registerVoiceInStaffClone (
       elt->getInputLineNumber (),
      fCurrentVoiceClone);

  switch (fCurrentVoiceOriginal->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // append the voice clone to the LPSR score elements list
      fResultingLpsr ->
        appendVoiceToLpsrScoreElementsList (
          fCurrentVoiceClone);

      // append a use of the voice to the current staff block
      fCurrentStaffBlock->
        appendVoiceUseToStaffBlock (
          fCurrentVoiceClone);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      {
        // register the voice clone as the part harmonies voice
        fCurrentPartClone->
          setPartHarmoniesVoice (
            fCurrentVoiceClone);

        if (
          true || fCurrentVoiceOriginal->getVoiceIsMusicallyEmpty () // superfluous test JMI ??? JMI 0.9.72
        ) {
          // append the voice clone to the LPSR score elements list
          fResultingLpsr ->
            appendVoiceToLpsrScoreElementsList (
              fCurrentVoiceClone);

          // create a ChordNames context
          std::string voiceName =
            fCurrentVoiceOriginal->getVoiceName ();
          std::string voicePathLikeName =
            fCurrentVoiceOriginal->getVoicePathLikeName ();

          std::string partdNameForTrace =
            fCurrentVoiceOriginal->
              fetchVoiceUpLinkToPart ()->
                fetchPartNameForTrace ();

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceHarmonies ()) {
            std::stringstream ss;

            ss <<
              "Creating a ChordNames context for \"" << voiceName <<
              "\" in part " << partdNameForTrace <<
              std::endl;

            gWaeHandler->waeTraceWithoutInputLocation (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          S_lpsrChordNamesContext
            chordNamesContext =
              lpsrChordNamesContext::create (
                 elt->getInputLineNumber (),
//                 lpsrContextUseExistingKind::kUseExistingContextYes,
                lpsrContextUseExistingKind::kUseExistingContextNo, // default value JMI 0.9.64
                voicePathLikeName,
// JMI                fCurrentVoiceOriginal->getHarmoniesVoiceRegularVoiceBackwardLink ());
                fCurrentVoiceOriginal);

          // append it to the current part block
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceHarmonies ()) {
            std::stringstream ss;

            ss <<
              "Appending the ChordNames context for \"" << voiceName <<
              "\" in part " << partdNameForTrace <<
              std::endl;

            gWaeHandler->waeTraceWithoutInputLocation (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          fCurrentPartBlock->
            appendChordNamesContextToPartBlock (
               elt->getInputLineNumber (),
              chordNamesContext);
        }
      }

      fOnGoingHarmoniesVoice = true;
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        // register the voice clone as the part figured bass voice
        fCurrentPartClone->
          setPartFiguredBassVoice (
            fCurrentVoiceClone);

        if (
          fCurrentVoiceOriginal->getVoiceIsMusicallyEmpty () // superfluous test ??? JMI
          ) {
          // append the voice clone to the LPSR score elements list
          fResultingLpsr ->
            appendVoiceToLpsrScoreElementsList (
              fCurrentVoiceClone);

          // create a FiguredBass context
          std::string voiceName =
            fCurrentVoiceOriginal->getVoiceName ();
          std::string voicePathLikeName =
            fCurrentVoiceOriginal->getVoicePathLikeName ();

          std::string partdNameForTrace =
            fCurrentVoiceOriginal->
              fetchVoiceUpLinkToPart ()->
                fetchPartNameForTrace ();

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceHarmonies ()) {
            std::stringstream ss;

            ss <<
              "Creating a FiguredBass context for \"" << voiceName <<
              "\" in part " << partdNameForTrace <<
              std::endl;

            gWaeHandler->waeTraceWithoutInputLocation (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          S_lpsrFiguredBassContext
            figuredBassContext =
              lpsrFiguredBassContext::create (
                 elt->getInputLineNumber (),
                lpsrContextUseExistingKind::kUseExistingContextNo, // default value JMI 0.9.64
                voicePathLikeName,
                fCurrentVoiceOriginal-> getVoiceUpLinkToStaff ());

          // append it to the current part block
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceHarmonies ()) {
            std::stringstream ss;

            ss <<
              "Appending the FiguredBass context for \"" << voiceName <<
              "\" in part " << partdNameForTrace <<
              std::endl;

            gWaeHandler->waeTraceWithoutInputLocation (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          fCurrentPartBlock->
            appendFiguredBassContextToPartBlock (
              figuredBassContext);
        }
      }

      fOnGoingFiguredBassVoice = true;
      break;
  } // switch

  // get the current segment clone JMI 0.9.76 ???
//   fCurrentSegmentClone =
//     fCurrentVoiceClone->
//       getVoiceSegment ();

  // clear the voice notes map
  fVoiceNotesMap.clear ();

  fFirstNoteCloneInVoice = nullptr;
}

//________________________________________________________________________
// void msr2lpsrTranslator::visitStart (S_msrHarmony& elt)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gMsrOahGroup->getTraceMsrVisitors ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Start visiting msrHarmony '" <<
//       elt->asString () <<
//       ", fCurrentVoiceClone->getVoiceKind (): " << fCurrentVoiceClone->getVoiceKind () <<
//       ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
//       ", fOnGoingChord: " << fOnGoingChord <<
//       ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
//       ", fOnGoingHarmony: " << fOnGoingHarmony <<
//       ", fCurrentHarmonyClone: " << fCurrentHarmonyClone <<
//       ", fCurrentNonGraceNoteClone: " << fCurrentNonGraceNoteClone <<
//       ", line " << elt->getInputLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // create a harmony newborn clone
//   fCurrentHarmonyClone =
//     elt->
//       createHarmonyNewbornClone (
//         fCurrentVoiceClone);
//
//   if (fOnGoingNonGraceNote) {
//     // register fCurrentNonGraceNoteClone as the harmony note upLink
//     fCurrentHarmonyClone->
//       setHarmonyUpLinkToNote (
//         fCurrentNonGraceNoteClone);
//
//     // append the harmony in the current non-grace note clone
//     fCurrentNonGraceNoteClone->
//       appendHarmonyToNote (
//         fCurrentHarmonyClone);
//
//     // don't append the harmony to the part harmony,
//     // this has been done in pass2b // JMI ???
//   }
//
//   else if (fOnGoingChord) {
//     // register the harmony clone in the current chord clone
//     fCurrentChordClone->
//       appendHarmonyToChord (
//         fCurrentHarmonyClone); // JMI ??? 0.9.66
//   }
//
//   else if (fOnGoingHarmoniesVoice) {
//     // append the harmony clone to the current voice clone
//     fCurrentVoiceClone->
//       edacsacAppendHarmonyToVoiceClone (
//         fCurrentHarmonyClone);
//   }
//
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "harmony is out of context, cannot be handled: " <<
//       elt->asShortString ();
//
//     msr2lpsrInternalError (
//       gServiceRunData->getInputSourceName (),
//       elt->getInputLineNumber (),
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   fOnGoingHarmony = true;
// }
//
// void msr2lpsrTranslator::visitEnd (S_msrHarmony& elt)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gMsrOahGroup->getTraceMsrVisitors ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> End visiting msrHarmony '" <<
//       elt->asString () <<
//       '\'' <<
//       ", line " << elt->getInputLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // forget about current harmony clone
//   fCurrentHarmonyClone = nullptr;
//
//   fOnGoingHarmony = false;
// }

// void msr2lpsrTranslator::visitStart (S_msrBassFigure& elt)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gMsrOahGroup->getTraceMsrVisitors ()) {
//     std::stringstream ss;
//
//     ss <<
//       "--> Start visiting msrBassFigure '" <<
//       elt->asString () <<
//       '\'' <<
//       ", line " << elt->getInputLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (fOnGoingFiguredBass) {
//     // append the bass figure to the current figured bass
//     fCurrentFiguredBassClone->
//       appendFigureToFiguredBass (
//         elt);
//   }
//
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "bass figure is out of context, cannot be handled: " <<
//       elt->asShortString ();
//
//     msr2lpsrInternalError (
//       gServiceRunData->getInputSourceName (),
//       elt->getInputLineNumber (),
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// }

//________________________________________________________________________
void msr2lpsrTranslator::visitEnd (S_msrMeasure& elt)
{
  mfMeasureNumber
    nextMeasureNumber =
      elt->getNextMeasureNumber ();

#ifdef MF_TRACE_IS_ENABLED
  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasure" <<
      ", elt->getMeasureNumber (): " <<
      elt->getMeasureNumber () <<
      "', nextMeasureNumber: '" <<
      nextMeasureNumber <<
      "', measurePuristNumber: '" <<
      measurePuristNumber <<
      "', line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // finalize the current measure clone
  fMeasuresStack.front ()->
    finalizeMeasureClone (
      elt->getInputLineNumber (),
      elt, // original measure
      fCurrentVoiceClone,
      "msr2lpsrTranslator::visitEnd (S_msrMeasure&)");

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
        "--> msr2lpsrTranslator::visitEnd() " <<
        elt->asString () <<
        ", line " <<  elt->getInputLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

  Bool doCreateABarCheck (false);
  Bool doCreateABarNumberCheck (false);

  switch (elt->getMeasureKind ()) {
    case msrMeasureKind::kMeasureKindUnknown:
      {
        std::stringstream ss;

        ss <<
          "measure '" << elt->getMeasureNumber () <<
          "' in voice \"" <<
          elt->
            fetchMeasureUpLinkToVoice ()->
              getVoiceName () <<
          "\" is of unknown kind in msr2lpsrTranslator";

      // JMI  msr2lpsrInternalError (
        msr2lpsrInternalWarning (
          gServiceRunData->getInputSourceName (),
           elt->getInputLineNumber (),
  //        __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrMeasureKind::kMeasureKindRegular:
      {
        // don't create a bar check for the last measure of the score // JMI 0.9.70
        // if it is not full time-signature wise

        // fetch the measure whole notes duration from the current measure clone
        mfWholeNotes
          fullMeasureWholeNotesDuration =
            fMeasuresStack.front ()->
              getFullMeasureWholeNotesDuration ();

        // get the current voice clone time signature
        S_msrTimeSignature
          voiceCurrentTimeSignature =
            fCurrentVoiceClone->
              getVoiceCurrentTimeSignature ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
        // sanity check
        mfAssert (
          __FILE__, mfInputLineNumber (__LINE__),
          voiceCurrentTimeSignature != nullptr,
          "voiceCurrentTimeSignature is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

        mfWholeNotes
          wholeNotesPerMeasure =
            voiceCurrentTimeSignature->
              timeSignatureWholeNotesPerMeasure ();

#ifdef MF_TRACE_IS_ENABLED
        if (
          gTraceOahGroup->getTraceBarChecks ()
            ||
          gTraceOahGroup->getTraceBarNumberChecks ()
        ) {
          std::stringstream ss;

          ss <<
            "--> kMeasureKindRegular" <<
            ", fullMeasureWholeNotesDuration: " <<
            fullMeasureWholeNotesDuration.asString () <<
            "', wholeNotesPerMeasure: " <<
            wholeNotesPerMeasure;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // don't generate bar information for the last measure in the score
        mfMeasureNumber
          scoreLastMeasureNumber =
            elt->fetchMeasureUpLinkToScore ()->getScoreLastMeasureNumber ();
//         gLog << // JMI 0.9.71
//           "elt->getMeasureNumber (): " << elt->getMeasureNumber () <<
//           ", scoreLastMeasureNumber: " << scoreLastMeasureNumber <<
//           std::endl;

        doCreateABarCheck =
          elt->getMeasureNumber () != scoreLastMeasureNumber;
        doCreateABarNumberCheck = doCreateABarCheck;
      }
      break;

    case msrMeasureKind::kMeasureKindAnacrusis:
      {
        // fetch the measure whole notes duration from the current measure clone
        mfWholeNotes
          fullMeasureWholeNotesDuration =
            fMeasuresStack.front ()->
              getFullMeasureWholeNotesDuration ();

        // get the current voice clone time signature
        S_msrTimeSignature
          voiceCurrentTimeSignature =
            fCurrentVoiceClone->
              getVoiceCurrentTimeSignature ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
        // sanity check
        mfAssert (
          __FILE__, mfInputLineNumber (__LINE__),
          voiceCurrentTimeSignature != nullptr,
          "voiceCurrentTimeSignature is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

        mfWholeNotes
          wholeNotesPerMeasure =
            voiceCurrentTimeSignature->
              timeSignatureWholeNotesPerMeasure ();

#ifdef MF_TRACE_IS_ENABLED
        if (
          gTraceOahGroup->getTraceBarChecks ()
            ||
          gTraceOahGroup->getTraceBarNumberChecks ()
        ) {
          std::stringstream ss;

          ss <<
            "--> kMeasureKindAnacrusis" <<
            ", fullMeasureWholeNotesDuration: " <<
            fullMeasureWholeNotesDuration.asString () <<
            "', wholeNotesPerMeasure: " <<
            wholeNotesPerMeasure;

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        doCreateABarCheck = true;
        doCreateABarNumberCheck = true;
      }
      break;

    case msrMeasureKind::kMeasureKindIncompleteStandalone:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      // generate a bar check if relevant
//         doCreateABarCheck = true;
//         doCreateABarNumberCheck = true;

//       switch (elt-> getMeasureEndRegularKind ()) {
//         case msrMeasureEndRegularKind::kMeasureEndRegularKindUnknown:
//           break;
//         case msrMeasureEndRegularKind::kMeasureEndRegularKindYes:
//           break;
//         case msrMeasureEndRegularKind::kMeasureEndRegularKindNo:
//           break;
//       } // switch
      break;

    case msrMeasureKind::kMeasureKindIncompleteLastMeasure:
//         doCreateABarCheck = true;
//         doCreateABarNumberCheck = true;
      break;

    case msrMeasureKind::kMeasureKindOverFlowing:
      doCreateABarCheck = true;
      doCreateABarNumberCheck = true;
      break;

    case msrMeasureKind::kMeasureKindCadenza:
//         doCreateABarCheck = false;
//         doCreateABarNumberCheck = false;
      break;

    case msrMeasureKind::kMeasureKindMusicallyEmpty:
//       // JMI 0.9.72
//       // append a skip to the current voice clone to fill this empty measure
//       elt->
//         appendPaddingSkipNoteToMeasure (
//           elt->getInputLineNumber (),
//           elt->getFullMeasureWholeNotesDuration ());

      doCreateABarCheck = true;
      doCreateABarNumberCheck = true;
      break;
  } // switch

  if (doCreateABarCheck) {
//     int
//       measurePuristNumber =
//         elt->getMeasurePuristNumber ();

    // create a bar check
    fLastBarCheck =
      msrBarCheck::createWithNextBarPuristNumber (
        elt->getInputLineNumber (),
        elt->getMeasureNumber (), // JMI 0.9.75
        fCurrentVoiceClone->
          getVoiceCurrentMeasurePuristNumber () + 1,
        fMeasuresStack.front ());

    // append it to the current voice clone
    fCurrentVoiceClone->
      appendBarCheckToVoice (fLastBarCheck);
  }

  if (doCreateABarNumberCheck) {
    // create a bar number check
    // should NOT be done in cadenza, SEE TO IT JMI 0.9.70
    S_msrBarNumberCheck
      barNumberCheck =
        msrBarNumberCheck::create (
          elt->getInputLineNumber (),
          fMeasuresStack.front (),
          nextMeasureNumber,
          fCurrentVoiceClone->
            getVoiceCurrentMeasurePuristNumber ());

    // append it to the current voice clone
    fCurrentVoiceClone->
      appendBarNumberCheckToVoice (barNumberCheck);
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrStanza& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

//  if (elt->getStanzaTextPresent ()) { // JMI
    fCurrentStanzaClone =
      elt->createStanzaNewbornClone (
        fCurrentVoiceClone);

    // append the stanza clone to the LPSR score elements list
    fResultingLpsr ->
      appendStanzaToLpsrScoreElementsList (
        fCurrentStanzaClone);

    // append a use of the stanza to the current staff block
    fCurrentStaffBlock ->
      appendLyricsUseToStaffBlock (
        fCurrentStanzaClone);
//  }
//  else
  //  fCurrentStanzaClone = nullptr; // JMI

  fOnGoingStanza = true;
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSyllable& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSyllable" <<
      ", line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the syllable clone
  fCurrentSyllableClone =
    elt->createSyllableNewbornClone (
      fCurrentPartClone);

  // append it to the current stanza clone or current note clone
  if (fOnGoingStanza) { // fCurrentStanzaClone JM
    // visiting a syllable as a stanza member
    fCurrentStanzaClone->
      appendSyllableToStanzaClone (
        fCurrentSyllableClone,
        fMeasuresStack.front ());
  }

  else if (fOnGoingNonGraceNote) { // JMI
    // visiting a syllable as attached to the current non-grace note

    // append syllable to currentNote
    fCurrentNonGraceNoteClone->
      appendSyllableToNote (
        fCurrentSyllableClone);

    // set syllable upLink to note
    elt->
      setSyllableUpLinkToNote (fCurrentNonGraceNoteClone);

    if (gLpsrOahGroup->getAddLpsrWordsFromTheLyrics ()) {
      // get the syllable texts list
      const std::list <msrSyllableElement>&
        syllableElementsList =
          elt->getSyllableElementsList ();

      if (syllableElementsList.size ()) {
        // build a single words value from the texts list
        // JMI create an msrWords instance for each???
        std::string wordsValue =
          syllableElementsListAsString (
            elt-> getSyllableElementsList ());

        // create the words
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceLyrics ()) {
          std::stringstream ss;

          ss <<
            "Changing lyrics '" <<
            wordsValue <<
            "' into words for note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "'";
      // JMI      fCurrentSyllableClone->asString ();

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        S_msrWords
          words =
            msrWords::create (
               elt->getInputLineNumber (),
              msrPlacementKind::kPlacement_UNKNOWN_,                // default value
              wordsValue,
              msrJustifyKind::kJustifyNone,                  // default value
              msrHorizontalAlignmentKind::kHorizontalAlignmentNone,      // default value
              msrVerticalAlignmentKind::kVerticalAlignmentNone,        // default value
              msrFontStyleKind::kFontStyleNone,                // default value
              msrFontSize::create (
                msrFontSizeKind::kFontSizeNone), // default value
              msrFontWeightKind::kFontWeightNone,               // default value
              msrXMLLangKind::kXMLLangIt,                    // default value
              elt->
                getSyllableUpLinkToNote ()->
                  fetchUpLinkToNoteToStaff ()->
                    getStaffNumber ());

        // append it to the current non-grace note
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceWords ()) {
          std::stringstream ss;

          ss <<
            "Appending words '" <<
            words->asShortString () <<
            "' to note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "'";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        fCurrentNonGraceNoteClone->
          appendWordsToNote (
            words);
      }
    }
  }

  else {
    std::stringstream ss;

    ss <<
      "syllable is out of context, cannot be handled: " <<
      elt->asShortString ();;

    msr2lpsrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // a syllable ends the sysllable extend range if any
  if (fOnGoingSyllableExtend) {
    /* JMI ???
    // create melisma end command
    S_lpsrMelismaCommand
      melismaCommand =
        lpsrMelismaCommand::create (
           elt->getInputLineNumber (),
          lpsrMelismaCommand::kMelismaEnd);

    // append it to current voice clone
    fCurrentVoiceClone->
      appendOtherElementToVoice (melismaCommand);
*/

    fOnGoingSyllableExtend = false;
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTempo" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (elt->getTempoKind ()) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      break;

    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      fResultingLpsr->
        // this score needs the 'tempo relationship' Scheme function
        setTempoNotesRelationshipSchemeFunctionIsNeeded ();
      break;
  } // switch

  if (gLpsrOahGroup->getConvertMusicXMLTemposToMsrRehearsalMarks ()) {
    // create a rehearsal mark containing elt's words

    S_msrRehearsalMark
      rehearsalMark =
        msrRehearsalMark::create (
          elt->getInputLineNumber (),
          fMeasuresStack.front (),
          msrRehearsalMarkKind::kRehearsalMarkNone,
          elt->tempoWordsListAsString (" "), //JMI ???
          elt->getTempoPlacementKind ());

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTempos ()) {
      std::stringstream ss;

      ss <<
        "Converting tempo " <<
        elt->asShortString () <<
        " to rehearsal mark '" <<
        rehearsalMark->asShortString () <<
        "'";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // append the rehearsalMark to the current voice clone
    fMeasuresStack.front ()->
      appendRehearsalMarkToMeasure (rehearsalMark);
  }

  else {
    fMeasuresStack.front ()-> // ZOULOU
      appendTempoToMeasure (elt);
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "articulation is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2lpsrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  switch (elt->getArticulationKind ()) {
    case msrArticulationKind::kArticulationScoop:
      // this score needs the 'scoop' stencils
      fResultingLpsr->
        setScoopSchemeFunctionIsNeeded ();
      break;

    default:
      ;
  } // switch
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "technical is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2lpsrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // doest the score need the 'tongue' function?
  switch (elt->getTechnicalKind ()) {
    case msrTechnicalKind::kTechnicalArrow:
      break;
    case msrTechnicalKind::kTechnicalDoubleTongue:
      fResultingLpsr->
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
      fResultingLpsr->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnicalKind::kTechnicalUpBow:
      break;
  } // switch
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
  msr2msrTranslator::visitStart (elt);

  switch (elt->getTechnicalWithStringKind ()) {
    case msrTechnicalWithStringKind::kHammerOn:
    case msrTechnicalWithStringKind::kPullOff:
      // this score needs the 'after' Scheme function
      fResultingLpsr->
        setAfterSchemeFunctionIsNeeded ();
      break;
    default:
      ;
  } // switch
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrOrnament& elt)
{
  msr2msrTranslator::visitStart (elt);

  switch (elt->getOrnamentKind ()) {
    case msrOrnamentKind::kOrnamentSchleifer:
      // this score needs the 'schleifer' Scheme function
      fResultingLpsr->
        setSchleiferSchemeFunctionIsNeeded ();
      break;

    default:
      ;
  } // switch
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrSpanner& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSpanner" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

  // is a spanner with centered text needed?
  if (elt->getSpannerMiddleText ().size ()) {
    fResultingLpsr->
      // this score needs the 'TextSpannerWithCenteredText' Scheme function
      setTextSpannerWithCenteredTextIsNeeded ();
  }

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSpannerToNote (elt);
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSpannerToChord (elt);
  }
  else {
    std::stringstream ss;

    ss <<
      "spanner is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2lpsrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrGlissando& elt)
{
  msr2msrTranslator::visitStart (elt);

  if (elt->getGlissandoTextValue ().size ()) {
    fResultingLpsr->
      // this score needs the 'glissandoWithText' Scheme functions
      addGlissandoWithTextSchemeFunctionsToScore ();
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrDynamic" <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendDynamicToNote (elt);

    // is this a non LilyPond native dynamic?
    Bool knownToLilyPondNatively (true);

    switch (elt->getDynamicKind ()) {
      case msrDynamicKind::kDynamicFFFFF:
      case msrDynamicKind::kDynamicFFFFFF:
      case msrDynamicKind::kDynamicPPPPP:
      case msrDynamicKind::kDynamicPPPPPP:
      case msrDynamicKind::kDynamicRF:
      case msrDynamicKind::kDynamicSFPP:
      case msrDynamicKind::kDynamicSFFZ:
      case msrDynamicKind::kDynamicSFZP:
      case msrDynamicKind::kDynamicN:
      case msrDynamicKind::kDynamic_UNKNOWN_:
        knownToLilyPondNatively = false;

      default:
        ;
    } // switch

    if (! knownToLilyPondNatively) {
      // this score needs the 'dynamics' Scheme function
      fResultingLpsr->
        setDynamicsSchemeFunctionIsNeeded ();
    }
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendDynamicToChord (elt);
  }

  else {
    std::stringstream ss;

    ss <<
      "dynamic is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2lpsrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrOtherDynamic& elt)
{
  msr2msrTranslator::visitStart (elt);

  fResultingLpsr->
    // this score needs the 'otherDynamic' Scheme function
    setOtherDynamicSchemeFunctionIsNeeded ();
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrWords" <<
      ", line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingNonGraceNote || fOnGoingChord) {
    Bool wordsHasBeenHandled;

    if (gLpsrOahGroup->getConvertLpsrWordsToTempo ()) {
      // create a tempo containing elt
      S_msrTempo
        tempo =
          msrTempo::createTempoWordsOnly (
             elt->getInputLineNumber (),
            fMeasuresStack.front (),
            elt,
            msrTempoParenthesizedKind::kTempoParenthesizedNo,    // JMI
            msrPlacementKind::kPlacementAbove); // JMI

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWords ()) {
        std::stringstream ss;

        ss <<
          "Converting words '" <<
          elt->asShortString () <<
          "' to tempo '" <<
          tempo->asShortString () <<
          '\'';

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append the tempo to the current voice clone
      fCurrentVoiceClone->
        appendTempoToVoice (tempo);

      wordsHasBeenHandled = true;
    }

    else if (gLpsrOahGroup->getConvertLpsrWordsToRehearsalMarks ()) {
      // create a rehearsal mark containing elt's words contents
      S_msrRehearsalMark
        rehearsalMark =
          msrRehearsalMark::create (
             elt->getInputLineNumber (),
            fMeasuresStack.front (),
            msrRehearsalMarkKind::kRehearsalMarkNone,
            elt->getWordsContents (),
            elt->getWordsPlacementKind ()); // above ??? JMI

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceWords ()) {
        std::stringstream ss;

        ss <<
          "Converting words '" <<
          elt->asShortString () <<
          "' to rehearsal mark '" <<
          rehearsalMark->asShortString () <<
          '\'';

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // append the rehearsalMark to the current voice clone
      fCurrentVoiceClone->
        appendRehearsalMarkToVoice (rehearsalMark);

      wordsHasBeenHandled = true;
    }

    if (! wordsHasBeenHandled) {
      if (fOnGoingNonGraceNote) {
        fCurrentNonGraceNoteClone->
          appendWordsToNote (elt);
      }
      else if (fOnGoingChord) {
        fCurrentChordClone->
          appendWordsToChord (elt);
      }
    }
  }

  else {
    std::stringstream ss;

    ss <<
      "words is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2lpsrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  msr2msrTranslator::visitStart (elt);

  // circumvent LilyPond #34 issue if relevant
  circumventLilyPond34IssueIfRelevant (elt);
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      '\'' <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//  displayCurrentOnGoingValues (); // JMI

  // create the note clone
  fCurrentNoteClone =
    elt->
      createNoteNewbornClone (
        fCurrentPartClone);

  // register clone in this tranlastors' voice notes map and ongoing notes stack
  fVoiceNotesMap [elt] = fCurrentNoteClone; // JMI XXL
  fOnGoingNotesStack.push_front (fCurrentNoteClone);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) { // JMI
    displayOnGoingNotesStack ("visitStart (S_msrNote&)");
  }
#endif // MF_TRACE_IS_ENABLED

  // don't register grace notes as the current note clone,
  // but as the current grace note clone instead
/* JMI
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
          std::stringstream ss;

          ss <<
            "The first note of voice clone GFFF '" <<
            fCurrentVoiceClone->getVoiceName () <<
            "' is '";

          if (fFirstNoteCloneInVoice) {
            ss <<
              fFirstNoteCloneInVoice->asShortString ();
          }
          else {
            ss << "[NULL]";
          }
          gLog <<
             "'";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
*/

  switch (elt->getNoteKind ()) {
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      fCurrentGraceNoteClone = fCurrentNoteClone;

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "===> fCurrentGraceNoteClone: " <<
          fCurrentGraceNoteClone->asString ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    default:
      fCurrentNonGraceNoteClone = fCurrentNoteClone;


#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "===> fCurrentNonGraceNoteClone: " <<
          fCurrentNonGraceNoteClone->asString ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      if (! fFirstNoteCloneInVoice) {
        fFirstNoteCloneInVoice =
          fCurrentNonGraceNoteClone;

#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceNotes ()) {
          std::stringstream ss;

          ss <<
            "The first note of voice clone RJIRWR '" <<
            fCurrentVoiceClone->getVoiceName () <<

            "' is '" <<
            fFirstNoteCloneInVoice->asShortString () <<
             "'";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
//
//         // should an initial staff change be created?
//         int
//           firstNoteStaffNumber =
//             fFirstNoteCloneInVoice->getNoteStaffNumber ();
//
//         if (firstNoteStaffNumber > 1) { // 1 is the default LilyPond staff number
//           // create the voice staff change
//           S_msrVoiceStaffChange
//             voiceStaffChange =
//               msrVoiceStaffChange::create (
//                 elt->getInputLineNumber (),
//                 gNullMeasure,  // JMI 0.9.72 ???  // set later in setMeasureElementUpLinkToMeasure()
//                 previousNoteStaff,
//                 currentNoteStaff);
//
//           // append it to the previous note's voice
//           // before the note itself is appended
//           fCurrentPartVoicesVector.at (fVoiceNumberToInsertInto)->
//             appendVoiceStaffChangeToVoice (
//               voiceStaffChange);
//         }
      }

      fOnGoingNonGraceNote = true;
  } // switch

//* JMI 0.9.70
  // can we optimize graceNotesGroup into afterGraceNotesGroup?
  if (
    elt->getNoteIsFollowedByGraceNotesGroup ()
      &&
    elt->getNoteTrillOrnament ()
  ) {
    // yes, create the after grace notes
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceGraceNotes ()) {
      std::stringstream ss;

      ss <<
        "Optimizing grace notes on trilled note '" <<
        elt->asShortString () <<
        "' as after grace notes " <<
        ", line " <<  elt->getInputLineNumber ()<<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPendingAfterGraceNotesGroup =
      msrAfterGraceNotesGroup::create (
         elt->getInputLineNumber (),
        fCurrentNonGraceNoteClone,
        false, // aftergracenoteIsSlashed, may be updated later
        fCurrentVoiceClone);

    // register current afterGraceNotesGroup element
    fCurrentAfterGraceNotesGroupElement =
      fCurrentNonGraceNoteClone;
  }
//*/
}

void msr2lpsrTranslator::visitEnd (S_msrNote& elt)
{
  msrNoteKind
    noteKind = elt->getNoteKind ();

#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " <<  elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ss <<
        fCurrentNonGraceNoteClone->asString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      "FAA fCurrentGraceNoteClone: " <<
      std::endl;
    if (fCurrentGraceNoteClone) {
      ss <<
        fCurrentGraceNoteClone->asString ();
    }
    else {
      ss << "[NULL]";
    }

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (noteKind) {

    case msrNoteKind::kNote_UNKNOWN_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Appending rest note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
        std::stringstream ss;

        ss <<
          "Appending skip note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
        std::stringstream ss;

        ss <<
          "Appending unpitched note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
        std::stringstream ss;

        ss <<
          "Appending regular note clone '" <<
          fCurrentNonGraceNoteClone->asShortString () << "' to voice clone " <<
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
            std::stringstream ss;

            ss <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo first element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
                      std::endl;

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          fCurrentDoubleTremoloClone->
            setDoubleTremoloNoteFirstElement (
              fCurrentNonGraceNoteClone);
        }

        else if (fCurrentNonGraceNoteClone->getNoteIsSecondNoteInADoubleTremolo ()) {
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceNotes ()) {
            std::stringstream ss;

            ss <<
              "Setting note '" <<
              fCurrentNonGraceNoteClone->asString () <<
              "', line " << fCurrentNonGraceNoteClone->getInputLineNumber () <<
              ", as double tremolo second element" <<
              " in voice \"" <<
              fCurrentVoiceClone->getVoiceName () <<
                      std::endl;

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
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

          msr2lpsrInternalError (
            gServiceRunData->getInputSourceName (),
             elt->getInputLineNumber (),
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
      }

      else {
        std::stringstream ss;

        ss <<
          "double tremolo note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of a double tremolo";

        msr2lpsrInternalError (
          gServiceRunData->getInputSourceName (),
           elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMI
      gLog <<
        "fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        std::endl;
        */

      if (fCurrentGraceNotesGroupClone) { // JMI 0.9.66
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
          std::stringstream ss;

          ss <<
            "Appending grace note " <<
            fCurrentGraceNoteClone->asShortString () <<
            " to the grace notes group " <<
            fCurrentGraceNotesGroupClone->asShortString () <<
            "' in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentGraceNotesGroupClone->
          appendNoteToGraceNotesGroup (
            fCurrentGraceNoteClone);
      }
      else {
        std::stringstream ss;

        ss <<
          "grace note '" << fCurrentNonGraceNoteClone->asShortString () <<
          "' found outside of any grace notes group";

        msr2lpsrInternalError (
          gServiceRunData->getInputSourceName (),
           elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

    /* JMI ???
      if (fCurrentGraceNotesGroupClone) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
          std::stringstream ss;

          ss <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentGraceNotesClone->
          appendNoteToGraceNotes (
            fCurrentNonGraceNoteClone);
      }

      else if (fPendingAfterGraceNotes) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceGraceNotes ()) {
          std::stringstream ss;

          ss <<
            "Appending note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "' to the after grace notes in voice \"" <<
            fCurrentVoiceClone->getVoiceName () << "\"";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
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

        msr2lpsrInternalError (
          gServiceRunData->getInputSourceName (),
           elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      */
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fOnGoingChord) {
        // append fCurrentNonGraceNoteClone to fCurrentChordClone
        fCurrentChordClone->
          appendNoteToChord (
            fCurrentNonGraceNoteClone,
            fCurrentVoiceClone);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
        mfAssert (
          __FILE__, mfInputLineNumber (__LINE__),
          fCurrentNonGraceNoteClone != nullptr,
          "fCurrentNonGraceNoteClone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

        if (! fCurrentChordHasBeenPopulatedFromItsFirstNote) {
          // copy the current note's values to the current chord
          fCurrentChordClone->
            copyNoteValuesToChord (
              fCurrentNoteClone);

//           // append current chord clone to the current voice,
//           // only now that its duration is known
//           fCurrentVoiceClone->
//             appendChordToVoice (
//               fCurrentChordClone);

          fCurrentChordHasBeenPopulatedFromItsFirstNote = true;
        }
     }

      else {
        std::stringstream ss;

        ss <<
          "msr2lpsrTranslator::visitEnd (S_msrNote& elt): kNoteRegularInChord note " <<
          elt->asString () <<
          " appears outside of a chord";

        msr2lpsrInternalError (
          gServiceRunData->getInputSourceName (),
           elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
        }
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      if (fOnGoingChord) {
        // append fCurrentGraceNoteClone to fCurrentVoiceClone
        fCurrentChordClone->
          appendNoteToChord (
            fCurrentGraceNoteClone,
            fCurrentVoiceClone);
      }

      else {
        std::stringstream ss;

        ss <<
          "msr2lpsrTranslator::visitEnd (S_msrNote& elt): kNoteInChordInGraceNotesGroup note " <<
          elt->asString () <<
          " appears outside of a chord";

        msr2lpsrInternalError (
          gServiceRunData->getInputSourceName (),
           elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
        }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceNotesBasics ()) {
        std::stringstream ss;

        ss <<
          "Appending note clone " <<
          fCurrentNonGraceNoteClone->asShortString () <<
          " to voice clone " <<
          fCurrentVoiceClone->getVoiceName ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fTupletsStack.front ()->
        appendNoteToTuplet (
          fCurrentNonGraceNoteClone);

      fCurrentVoiceClone->
        registerTupletNoteInVoice (fCurrentNonGraceNoteClone);
      break;
  } // switch

  if (fCurrentNonGraceNoteClone) { // JMI 0.9.70
    // handle editorial accidentals
    switch (fCurrentNonGraceNoteClone->getNoteEditorialAccidentalKind ()) {
      case msrEditorialAccidentalKind::kEditorialAccidentalYes:
        fResultingLpsr->
          // this score needs the 'editorial accidental' Scheme function
          setEditorialAccidentalSchemeFunctionIsNeeded ();
        break;
      case msrEditorialAccidentalKind::kEditorialAccidentalNo:
        break;
    } // switch

    // handle cautionary accidentals // JMI ???
    switch (fCurrentNonGraceNoteClone->getNoteCautionaryAccidentalKind ()) {
      case msrCautionaryAccidentalKind::kCautionaryAccidentalYes:
        break;
      case msrCautionaryAccidentalKind::kCautionaryAccidentalNo:
        break;
    } // switch
  }

/* JMI
  // handle melisma
  msrSyllableExtendKind
    noteSyllableExtendKind =
      elt->getNoteSyllableExtendKind ();

  switch (noteSyllableExtendKind) {
    case msrSyllable::kStandaloneSyllableExtend:
      {
        / * JMI ???
        // create melisma start command
        S_lpsrMelismaCommand
          melismaCommand =
            lpsrMelismaCommand::create (
               elt->getInputLineNumber (),
              lpsrMelismaCommand::kMelismaStart);

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
*/

  switch (noteKind) {
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    default:
      fOnGoingNonGraceNote = false;
  } // switch

  // copy note's elements if any to the current chord if any
  if (fCurrentChordClone) {
    fCurrentChordClone->
      copyNoteElementsIfAnyToChord (
        fCurrentNoteClone);
  }

  // forget about current note
  fOnGoingNotesStack.pop_front ();
}

void msr2lpsrTranslator::visitStart (S_msrAccordionRegistration& elt)
{
  msr2msrTranslator::visitStart (elt);

  // the generated code needs modules scm and accreg
  fResultingLpsr->
    setScmAndAccregSchemeModulesAreNeeded ();
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrChordBeamLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrChordBeamLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrBeam originalBeam = elt->getOriginalBeam ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "--> visitStart (S_msrChordBeamLink&), " <<
      elt->asShortString () <<
      ", originalBeam: " <<
      originalBeam->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingChord) {
    S_msrChordBeamLink
      chordBeamLink =
        msrChordBeamLink::create (
          elt->getInputLineNumber (),
          originalBeam,
          fCurrentChordClone);

    fCurrentChordClone->
      appendChordBeamLinkToChord (chordBeamLink);
  }

  else {
    displayCurrentOnGoingValues ();

    std::stringstream ss;

    ss <<
      "beam link is out of context, cannot be handled: " <<
      elt->asShortString ();

    msr2lpsrInternalError (
      gServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  fOnGoingChordBeamLink = true;
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrTuplet& elt)
{
  msr2msrTranslator::visitStart (elt);

  // is Scheme support needed?
  switch (elt->getTupletLineShapeKind ()) {
    case msrTupletLineShapeKind::kTupletLineShapeStraight:
      break;
    case msrTupletLineShapeKind::kTupletLineShapeCurved:
      fResultingLpsr->
        // this score needs the 'tuplets curved brackets' Scheme function
        setTupletsCurvedBracketsSchemeFunctionIsNeeded ();
      break;
  } // switch
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrDalSegno& elt)
{
  msr2msrTranslator::visitStart (elt);

  // this score needs hiddenMeasureAndBarLine
  fResultingLpsr->
    setHiddenMeasureAndBarLineIsNeeded ();
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrDamp& elt)
{
  msr2msrTranslator::visitStart (elt);

  fResultingLpsr->
    // this score needs the 'custom short barLine' Scheme function
    setDampMarkupIsNeeded ();
}

void msr2lpsrTranslator::visitStart (S_msrDampAll& elt)
{
  msr2msrTranslator::visitStart (elt);

  fResultingLpsr->
    // this score needs the 'custom short barLine' Scheme function
    setDampAllMarkupIsNeeded ();
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrMultipleMeasureRest& elt)
{
  msr2msrTranslator::visitEnd (elt);

  fResultingLpsr->
    // this score needs the 'merge multiple measure rests' Scheme function
    setMergeMultipleMeasureRestsIsNeeded ();
}

//________________________________________________________________________
void msr2lpsrTranslator::visitStart (S_msrBarLine& elt)
{
  msr2msrTranslator::visitStart (elt);

  switch (elt->getBarLineStyleKind ()) {
    case msrBarLineStyleKind::kBarLineStyleNone:
      break;
    case msrBarLineStyleKind::kBarLineStyleRegular:
      break;
    case msrBarLineStyleKind::kBarLineStyleDotted:
      break;
    case msrBarLineStyleKind::kBarLineStyleDashed:
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavy:
      break;
    case msrBarLineStyleKind::kBarLineStyleLightLight:
      break;
    case msrBarLineStyleKind::kBarLineStyleLightHeavy:
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavyLight:
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavyHeavy:
      break;
    case msrBarLineStyleKind::kBarLineStyleTick:
      break;
    case msrBarLineStyleKind::kBarLineStyleShort:
      fResultingLpsr->
        // this score needs the 'custom short barLine' Scheme function
        setCustomShortBarLineSchemeFunctionIsNeeded ();
      break;
      /* JMI
    case msrBarLineStyleKind::kBarLineStyleNone:
      break;
      */
  } // switch
}


} // namespace
