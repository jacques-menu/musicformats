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

#include "msr2msrWae.h"

#include "enableTracingIfDesired.h"
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




using namespace std;

namespace MusicFormats
{
//_______________________________________________________________________________
S_msrScore translateMsrToMsr (
  S_msrScore        originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts,
  const string&     passNumber,
  const string&     passDescription)
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
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber << ": " << passDescription <<
      endl <<
      separator <<
      endl;
  }
#endif

  // the msr2msrTranslator
  msr2msrTranslator
    translator;

  // build the resulting MSR score
  S_msrScore
    resultingNewMsrScore =
      translator.translateMsrToMsr (
        originalMsrScore);

  clock_t endClock = clock ();

  // register time spent
  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItem::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after pass 3: " <<
        gIndenter.getIndent ();

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
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // display the resulting MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsr ()) {
    displayPopulatedMsrScore_OptionalPass (
      resultingNewMsrScore,
      "Display the resulting MSR as text");
  }

  if (gGlobalMsrOahGroup->getDisplaySecondMsrShort ()) {
    displayPopulatedMsrScore_OptionalPass (
      resultingNewMsrScore,
      "Display the resulting MSR as short text");
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      "Display a summary of the populated MSR");
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      resultingNewMsrScore,
      gGlobalMsrOahGroup);
  }
  return resultingNewMsrScore;
}

//_______________________________________________________________________________
S_msrScore translateMsrToMsrAlongPathToVoice (
  S_msrScore        originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts,
  const string&     passNumber,
  const string&     passDescription,
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
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber << ": " << passDescription <<
      endl <<
      separator <<
      endl;
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
    mfTimingItem::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after pass 3: " <<
        gIndenter.getIndent ();

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
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // display the resulting new MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsr ()) {
    displayPopulatedMsrScore_OptionalPass (
      resultingNewMsrScore,
      "Display the resulting new MSR as text");
  }

  if (gGlobalMsrOahGroup->getDisplaySecondMsrShort ()) {
    displayPopulatedMsrScore_OptionalPass (
      resultingNewMsrScore,
      "Display the resulting new MSR as short text");
  }

  // display the resulting new MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      resultingNewMsrScore,
      gGlobalMsrOahGroup,
      "Display a summary of the resulting new MSR");
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      resultingNewMsrScore,
      gGlobalMsrOahGroup);
  }
  return resultingNewMsrScore;
}

//_______________________________________________________________________________
void displayResultingNewMsrScore_OptionalPass (
  const S_msrScore originalMsrScore)
{
  // display it
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Pass (ptional): displaying the resulting MSR as text" <<
    endl <<
    separator <<
    endl << endl <<
    originalMsrScore <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the resulting MSR as text",
    mfTimingItem::kOptional,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after MSR score display: " <<
        gIndenter.getIndent ();

      msr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayResultingNewMsrScoreShort_OptionalPass (
  S_msrScore originalMsrScore)
{
  // display it

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Pass (ptional): displaying the resulting MSR as text, short version" <<
    endl <<
    separator <<
    endl << endl;

  originalMsrScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the MSR as text, short version",
    mfTimingItem::kOptional,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after MSR score short display: " <<
        gIndenter.getIndent ();

      msr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}


}
