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


#include "mxsr2msrWae.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfTiming.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "msrInterface.h"

#include "mxsr2msrSkeletonBuilderInterface.h"

#include "mxsr2msrSkeletonBuilder.h"

#include "msr2summaryVisitor.h"

namespace MusicFormats
{
//_______________________________________________________________________________
S_msrScore translateMxsrToMsrSkeleton (
  Sxmlelement    theMxsr,
  S_msrOahGroup& msrOpts,
  const std::string&  passNumber,
  const std::string&  passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMxsr != nullptr,
    "translateMxsrToMsrSkeleton(): theMxsr is null");

  // start the clock
  clock_t startClock = clock ();

#ifdef OAH_TRACING_IS_ENABLED
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

  S_msrScore scoreSkeleton;

  // create an MXSR2msrSkeletonBuilder
  mxsr2msrSkeletonBuilder
    skeletonBuilder;

  // build the MSR score
  skeletonBuilder.browseMxsr (
    theMxsr);

  // fetch the score skeleton
  scoreSkeleton =
    skeletonBuilder.getMsrScore ();

  // display the first MSR score skeleton if requested
  if (gGlobalMsrOahGroup->getDisplayMsrSkeleton ()) {
    displayMsrScore (
      scoreSkeleton,
      gWaeHandler->displayTheFirstMSRSkeletonAsText ());
  }

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
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream s;

      s <<
        "gIndenter value after " << passNumber << ": " <<
        gIndenter.getIndentation ();

      mxsr2msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! scoreSkeleton) {
    std::string message =
      "### Conversion from MXSR to an MSR skeleton failed ###";

    gLogStream <<
      message <<
      std::endl;

    throw mxsr2msrException (message);
  }

  return scoreSkeleton;
}

//_______________________________________________________________________________
void displayMsrScoreSkeleton (
  const S_msrOahGroup&  msrOpts,
  const S_msrScore& theMsrScore,
  std::string     passNumber,
  std::string     passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  std::string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    std::endl <<
    separator <<
    std::endl <<
    gTab <<
    gWaeHandler->passOptional () <<
    ": "<<
    passDescription <<
    std::endl <<
    separator <<
    std::endl << std::endl <<
    theMsrScore;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the MSR skeleton as text",
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}


}
