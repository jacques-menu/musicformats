/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "mfPreprocessorSettings.h"

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "msrPathToVoice.h"

#include "msr2msrWae.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "msrInterface.h"

#include "msr2msrOah.h"

#include "msr2msrInterface.h"

#include "msr2msrTranslator.h"

#include "mxsr2msrSkeletonPopulatorInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msrScore translateMsrToMsr (
  const S_msrScore&        originalMsrScore,
  const S_msrOahGroup&     msrOpts,
  const S_msr2msrOahGroup& msr2msrOpts,
  mfPassIDKind             passIDKind,
  const std::string&       passDescription)
{
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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    originalMsrScore != nullptr,
    "originalMsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // this test is done here instead of not calling translateMsrToMsr(),
  // since that simplifies the caller's code
  // and avoiding this pass is essentially for tests
  if (gGlobalMsr2msrOahGroup->getAvoidMsr2msr ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTracePasses ()) {
      std::stringstream ss;

      ss <<
        gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription << '\'' <<
        " merely returns the original MSR score, since this pass is being avoided";

      msr2msrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    return originalMsrScore;
  }

  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

  // the msr2msrTranslator
  msr2msrTranslator
    translator;

  // build the resulting second MSR score
  S_msrScore
    resultingNewMsrScore =
      translator.translateMsrToMsr (
        originalMsrScore);

  clock_t endClock = clock ();

  // register time spent
  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream ss;

      ss <<
        "gIndenter value after pass 3: " <<
        gIndenter.getIndentation ();

      msr2msrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }

    gIndenter.resetToZero ();
  }

  // check indentation
  if (gIndenter != 0) {
    gLog <<
      "### translateMsrToMsr gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  // display the resulting second MSR score if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplaySecondMsr ()) {
    displayMsrScore (
      resultingNewMsrScore,
      gLanguage->displayTheSecondMSRAsText ());
  }

  if (gMsrOahGroup->getDisplaySecondMsrFull ()) {
    displayMsrScoreFull (
      resultingNewMsrScore,
      gLanguage->displayTheSecondMSRAsText ());
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplaySecondMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      resultingNewMsrScore,
      gMsrOahGroup,
      gLanguage->displayASummaryOfTheSecondMSR ());
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplaySecondMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      resultingNewMsrScore,
      gMsrOahGroup,
      gLanguage->displayTheNamesInTheSecondMSR ());
  }
  return resultingNewMsrScore;

  // display the populated MSR score flat view if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplaySecondMsrFlatView ()) {
    // display the score name
    displayMsrScoreVoicesFlatView (
      resultingNewMsrScore,
      gMsrOahGroup,
      gLanguage->displayAFlatViewOfTheSecondMSR (),
      msrVoicesFlatViewDetailedKind::kVoicesFlatViewDetailedKindNo);
  }

  // display the populated MSR score flat view details if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplaySecondMsrFlatViewDetails ()) {
    // display the score name
    displayMsrScoreVoicesFlatView (
      resultingNewMsrScore,
      gMsrOahGroup,
      gLanguage->displayAFlatViewOfTheSecondMSR (),
      msrVoicesFlatViewDetailedKind::kVoicesFlatViewDetailedKindYes);
  }

  // display the populated MSR score slices if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplaySecondMsrMeasuresSlices ()) {
    // display the score name
    displayMsrScoreMeasuresSlices (
      resultingNewMsrScore,
      gMsrOahGroup,
      gLanguage->displayTheSlicesOfTheSecondMSR ());
  }

  return resultingNewMsrScore; // JMI 0.9.75
}

//_______________________________________________________________________________
S_msrScore translateMsrToMsrAlongPathToVoice (
  const S_msrScore&        originalMsrScore,
  const S_msrOahGroup&     msrOpts,
  const S_msr2msrOahGroup& msr2msrOpts,
  mfPassIDKind             passIDKind,
  const std::string        passDescription,
  const S_msrPathToVoice&  pathToVoice)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    originalMsrScore != nullptr,
    "originalMsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  if (gGlobalMsr2msrOahGroup->getAvoidMsr2msr ()) { // for TESTS
    return originalMsrScore;
  }

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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the msr2msrTranslator
  msr2msrTranslator
    translator;

  // build the resulting new MSR score
  S_msrScore
    resultingNewMsrScore =
      translator.translateMsrToMsrAlongPathToVoice (
        originalMsrScore,
        pathToVoice);

  clock_t endClock = clock ();

  // register time spent
  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream ss;

      ss <<
        "gIndenter value after pass 3: " <<
        gIndenter.getIndentation ();

      msr2msrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }

    gIndenter.resetToZero ();
  }

  // check indentation
  if (gIndenter != 0) {
    gLog <<
      "### translateMsrToMsr gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  // display the resulting new MSR score if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplaySecondMsr ()) {
    displayMsrScore (
      resultingNewMsrScore,
      gLanguage->displayTheSecondMSRAsText ());
  }

  if (gMsrOahGroup->getDisplaySecondMsrFull ()) {
    displayMsrScoreFull (
      resultingNewMsrScore,
      gLanguage->displayTheSecondMSRAsText ());
  }

  // display the resulting new MSR score summary if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplaySecondMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      resultingNewMsrScore,
      gMsrOahGroup,
      gLanguage->displayASummaryOfTheSecondMSR ());
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gMsrOahGroup->getDisplaySecondMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      resultingNewMsrScore,
      gMsrOahGroup,
      gLanguage->displayTheNamesInTheSecondMSR ());
  }
  return resultingNewMsrScore;
}


}
