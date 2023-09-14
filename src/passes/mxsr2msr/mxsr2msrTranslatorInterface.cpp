/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfTiming.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "msrInterface.h"

#include "mxsr2msrTranslatorInterface.h"

#include "mxsr2msrTranslator.h"

#include "displayMsrSummaryVisitor.h"
#include "displayMsrNamesVisitor.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
void populateMsrSkeletonFromMxsr (
  const Sxmlelement& theMxsr,
  S_msrScore         scoreSkeletonToBePopulated,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMxsr != nullptr,
    "populateMsrSkeletonFromMxsr(): theMxsr is null");

  mfAssert (
    __FILE__, __LINE__,
    scoreSkeletonToBePopulated != nullptr,
    "scoreSkeletonToBePopulated is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create an mxsr2msrTranslator
  mxsr2msrTranslator
    translator (
      scoreSkeletonToBePopulated);

  // browse the MXSR
  translator.browseMxsr (
    theMxsr);

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLog <<
      "### populateMsrSkeletonFromMxsr gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  // display the populated MSR score if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplayFirstMsr ()) {
    displayMsrScore (
      scoreSkeletonToBePopulated,
      gLanguage->displayTheFirstMSRAsText ());
  }

  if (gMsrOahGroup->getDisplayFirstMsrFull ()) {
    displayMsrScoreFull (
      scoreSkeletonToBePopulated,
      gLanguage->displayTheFirstMSRAsText ());
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplayFirstMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      scoreSkeletonToBePopulated,
      gMsrOahGroup,
      gLanguage->displayASummaryOfTheFirstMSR ());
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplayFirstMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      scoreSkeletonToBePopulated,
      gMsrOahGroup,
      gLanguage->displayTheNamesInTheFirstMSR ());
  }

  // display the populated MSR score flat view if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplayFirstMsrFlatView ()) {
    // display the score name
    displayMsrScoreVoicesFlatView (
      scoreSkeletonToBePopulated,
      gMsrOahGroup,
      gLanguage->displayAFlatViewOfTheFirstMSR (),
      msrVoicesFlatViewDetailedKind::kVoicesFlatViewDetailedKindNo);
  }

  // display the populated MSR score flat view details if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplayFirstMsrFlatViewDetails ()) {
    // display the score name
    displayMsrScoreVoicesFlatView (
      scoreSkeletonToBePopulated,
      gMsrOahGroup,
      gLanguage->displayAFlatViewOfTheFirstMSR (),
      msrVoicesFlatViewDetailedKind::kVoicesFlatViewDetailedKindYes);
  }

  // display the populated MSR score slices if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplayFirstMsrMeasuresSlices ()) {
    // display the score name
    displayMsrScoreMeasuresSlices (
      scoreSkeletonToBePopulated,
      gMsrOahGroup,
      gLanguage->displayTheSlicesOfTheFirstMSR ());
  }

//   // display the populated MSR score slices details if requested
//   // ------------------------------------------------------

  if (gMsrOahGroup->getDisplayFirstMsrMeasuresSlicesDetails()) {
    // display the score name
    displayMsrScoreMeasuresSlices (
      scoreSkeletonToBePopulated,
      gMsrOahGroup,
      gLanguage->displayTheSlicesOfTheFirstMSR ());
  }
}


}
