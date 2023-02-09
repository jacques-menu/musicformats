/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "mfStaticSettings.h"

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "msrPathToVoice.h"

#include "msr2msrWae.h"

#include "mfStaticSettings.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "msrInterface.h"

#include "msr2msrOah.h"

#include "msr2msrInterface.h"

#include "msr2msrTranslator.h"

#include "mxsr2msrTranslatorInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{
//_______________________________________________________________________________
S_msrScore translateMsrToMsr (
  S_msrScore           originalMsrScore,
  const S_msrOahGroup& msrOpts,
  S_msr2msrOahGroup    msr2msrOpts,
  mfPassIDKind         passIDKind,
  const std::string&   passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is null");
#endif

  if (gGlobalMsr2msrOahGroup->getAvoidMsr2msr ()) { // for TESTS ONLY ???
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
      separator <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

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

  if (gGlobalMsrOahGroup->getDisplaySecondMsr ()) {
    displayMsrScore (
      resultingNewMsrScore,
      gLanguage->displayTheSecondMSRAsText ());
  }

  if (gGlobalMsrOahGroup->getDisplaySecondMsrFull ()) {
    displayMsrScoreFull (
      resultingNewMsrScore,
      gLanguage->displayTheSecondMSRAsText ()
      	+
      ", "
        +
      gLanguage->fullVersion ());
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      gLanguage->displayASummaryOfTheSecondMSR ());
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      gLanguage->displayTheNamesInTheSecondMSR ());
  }
  return resultingNewMsrScore;
}

//_______________________________________________________________________________
S_msrScore translateMsrToMsrAlongPathToVoice (
  S_msrScore           originalMsrScore,
  const S_msrOahGroup& msrOpts,
  S_msr2msrOahGroup    msr2msrOpts,
  mfPassIDKind         passIDKind,
  const std::string    passDescription,
  S_msrPathToVoice     pathToVoice)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is null");
#endif

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
      separator <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

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

  if (gGlobalMsrOahGroup->getDisplaySecondMsr ()) {
    displayMsrScore (
      resultingNewMsrScore,
      gLanguage->displayTheSecondMSRAsText ());
  }

  if (gGlobalMsrOahGroup->getDisplaySecondMsrFull ()) {
    displayMsrScoreFull (
      resultingNewMsrScore,
      gLanguage->displayTheSecondMSRAsText ());
  }

  // display the resulting new MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      gLanguage->displayASummaryOfTheSecondMSR ());
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      gLanguage->displayTheNamesInTheSecondMSR ());
  }
  return resultingNewMsrScore;
}


}
