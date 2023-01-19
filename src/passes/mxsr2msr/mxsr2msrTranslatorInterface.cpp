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

#include "mfStaticSettings.h"

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

namespace MusicFormats
{

//_______________________________________________________________________________
void populateMsrSkeletonFromMxsr (
  Sxmlelement   theMxsr,
  S_msrScore    scoreSkeletonToBePopulated,
  const std::string& passNumber,
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
#endif

  // start the clock
  clock_t startClock = clock ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      passNumber << ": " << passDescription <<
      std::endl;

    gLogStream <<
      separator <<
      std::endl;
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
      std::endl;

    gIndenter.resetToZero ();
  }

  // display the populated MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsr ()) {
    displayMsrScore (
      scoreSkeletonToBePopulated,
      gWaeHandler->displayTheFirstMSRAsText ());
  }

  if (gGlobalMsrOahGroup->getDisplayFirstMsrFull ()) {
    displayMsrScoreFull (
      scoreSkeletonToBePopulated,
      gWaeHandler->displayTheFirstMSRAsText ()
      	+
      ", "
        +
      gWaeHandler->fullVersion () );
  }

  // display the populated MSR score summary if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsrSummary ()) {
    // display the score summary
    displayMsrScoreSummary (
      scoreSkeletonToBePopulated,
      gGlobalMsrOahGroup,
      gWaeHandler->displayASummaryOfTheFirstMSR ());
  }

  // display the populated MSR score names if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsrNames ()) {
    // display the score name
    displayMsrScoreNames (
      scoreSkeletonToBePopulated,
      gGlobalMsrOahGroup,
      gWaeHandler->displayTheNamesInTheFirstMSR ());
  }
}


}
