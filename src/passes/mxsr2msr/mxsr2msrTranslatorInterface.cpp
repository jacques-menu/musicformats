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



#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfTiming.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "msrInterface.h"

#include "mxsr2msrTranslatorInterface.h"

#include "mxsr2msrTranslator.h"

#include "msr2summaryVisitor.h"
#include "msr2namesVisitor.h"

using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
void populateMsrSkeletonFromMxsr (
  Sxmlelement   theMxsr,
  S_msrScore    scoreSkeletonToBePopulated,
  const string& passNumber,
  const string& passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMxsr != nullptr,
    "populateMsrSkeletonFromMxsr(): theMxsr is null");

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    scoreSkeletonToBePopulated != nullptr,
    "scoreSkeletonToBePopulated is null");

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
      endl;

    gLogStream <<
      separator <<
      endl;
  }
#endif

  // create an mxsr2msrTranslator
  mxsr2msrTranslator
    translator (
      scoreSkeletonToBePopulated);

  // browse the MXSR
  translator.browseMxsr (
    theMxsr);

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### populateMsrSkeletonFromMxsr gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // display the populated MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsrShort ()) {
    displayMsrScoreShort (
      scoreSkeletonToBePopulated,
      "Display the first MSR as text");
  }

  if (gGlobalMsrOahGroup->getDisplayFirstMsrFull ()) {
    displayMsrScoreFull (
      scoreSkeletonToBePopulated,
      "Display the first MSR as text");
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      scoreSkeletonToBePopulated,
      gGlobalMsrOahGroup,
      "Display a summary of the first MSR");
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      scoreSkeletonToBePopulated,
      gGlobalMsrOahGroup,
      "Display the names in the first MSR");
  }
}


}
