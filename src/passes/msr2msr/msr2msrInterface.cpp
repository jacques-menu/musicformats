/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfTiming.h"

#include "msrPathToVoice.h"

#include "msr2msrWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "msrInterface.h"

#include "msr2msrOah.h"

#include "msr2msrInterface.h"

#include "msr2msrTranslator.h"

#include "mxsr2msrTranslatorInterface.h"




namespace MusicFormats
{
//_______________________________________________________________________________
S_msrScore translateMsrToMsr (
  S_msrScore        originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts,
  const std::string&     passNumber,
  const std::string&     passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is null");

  if (gGlobalMsr2msrOahGroup->getAvoidMsr2msr ()) { // for TESTS ONLY ???
    return originalMsrScore;
  }

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      passNumber << ": " << passDescription <<
      std::endl <<
      separator <<
      std::endl;
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
  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream s;

      s <<
        "gIndenter value after pass 3: " <<
        gIndenter.getIndentation ();

      msr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### translateMsrToMsr gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  // display the resulting second MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrShort ()) {
    displayMsrScoreShort (
      resultingNewMsrScore,
      "Display the second MSR as text");
  }

  if (gGlobalMsrOahGroup->getDisplaySecondMsrFull ()) {
    displayMsrScoreFull (
      resultingNewMsrScore,
      "Display the second MSR as text");
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      "Display a summary of the second MSR");
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      "Display the names in the second MSR");
  }
  return resultingNewMsrScore;
}

//_______________________________________________________________________________
S_msrScore translateMsrToMsrAlongPathToVoice (
  S_msrScore        originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts,
  const std::string&     passNumber,
  const std::string&     passDescription,
  S_msrPathToVoice  pathToVoice)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is null");

  if (gGlobalMsr2msrOahGroup->getAvoidMsr2msr ()) { // for TESTS
    return originalMsrScore;
  }

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      passNumber << ": " << passDescription <<
      std::endl <<
      separator <<
      std::endl;
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
  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream s;

      s <<
        "gIndenter value after pass 3: " <<
        gIndenter.getIndentation ();

      msr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### translateMsrToMsr gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  // display the resulting new MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrShort ()) {
    displayMsrScoreShort (
      resultingNewMsrScore,
      "Display the second MSR as text");
  }

  if (gGlobalMsrOahGroup->getDisplaySecondMsrFull ()) {
    displayMsrScoreFull (
      resultingNewMsrScore,
      "Display the second MSR as text");
  }

  // display the resulting new MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      "Display a summary of the second MSR");
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      "Display the names in the second MSR");
  }
  return resultingNewMsrScore;
}


}
