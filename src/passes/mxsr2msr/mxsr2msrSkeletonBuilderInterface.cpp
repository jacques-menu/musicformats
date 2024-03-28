/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "mxsr2msrWae.h"

#include "msrInterface.h"

#include "mxsr2msrSkeletonBuilderInterface.h"

#include "mxsr2msrSkeletonBuilder.h"

#include "displayMsrSummaryVisitor.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msrScore translateMxsrToMsrSkeleton (
  const Sxmlelement&   theMxsr,
  const S_msrOahGroup& msrOpts,
  mfPassIDKind         passIDKind,
  const std::string&   passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMxsr != nullptr,
    "translateMxsrToMsrSkeleton(): theMxsr is null");
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
  if (gMsrOahGroup->getDisplayMsrSkeleton ()) {
    displayMsrScore (
      scoreSkeleton,
      gLanguage->displayTheFirstMSRSkeletonAsText ());
  }

  if (gMsrOahGroup->getDisplayMsrSkeletonFull ()) {
    displayMsrScoreFull(
      scoreSkeleton,
      gLanguage->displayTheFirstMSRSkeletonAsText ());
  }

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
    if (! gEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream ss;

      ss <<
        "gIndenter value after " << passIDKind << ": " <<
        gIndenter.getIndentation ();

      mxsr2msrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! scoreSkeleton) {
    std::string message =
      "### Conversion from MXSR to an MSR skeleton failed ###";

    gLog <<
      message <<
      std::endl;

    throw mxsr2msrException (message);
  }

  return scoreSkeleton;
}

// //_______________________________________________________________________________
// void displayMsrScoreSkeleton ( // UNUSED JMI v0.9.66
//   S_msrOahGroup&     msrOpts,
//   const S_msrScore&  theMsrScore,
// //   mfPassIDKind       passIDKind,
//   const std::string& passIDKind,
//   const std::string& passDescription)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     theMsrScore != nullptr,
//     "theMsrScore is null");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // start the clock
//   clock_t startClock = clock ();
//
//   setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);
//
//   std::string separator =
//     "%--------------------------------------------------------------";
//
//   gLog <<
//     std::endl <<
//     separator <<
//     std::endl <<
//     gTab <<
//     gLanguage->passOptional () << ": "<< passDescription <<
//     std::endl <<
//     separator <<
//     std::endl << std::endl <<
//     theMsrScore;
//
//   // register time spent
//   clock_t endClock = clock ();
//
//   gGlobalTimingItemsList.appendTimingItem (
//      mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling,
//     gLanguage->displayTheFirstMSRSkeletonAsText (), // JMI ??? v0.9.66
//     mfTimingItemKind::kOptional,
//     startClock,
//     endClock);
// }


}
