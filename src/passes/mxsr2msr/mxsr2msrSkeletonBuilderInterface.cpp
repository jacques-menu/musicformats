/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "mfStaticSettings.h"

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mxsr2msrWae.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"

#include "mfServices.h"

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
  Sxmlelement        theMxsr,
  S_msrOahGroup&     msrOpts,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMxsr != nullptr,
    "translateMxsrToMsrSkeleton(): theMxsr is null");
#endif

  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gWaeHandler->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
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
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream ss;

      ss <<
        "gIndenter value after " << passIDKind << ": " <<
        gIndenter.getIndentation ();

      mxsr2msrWarning (
        gGlobalCurrentServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
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

// //_______________________________________________________________________________
// void displayMsrScoreSkeleton ( // UNUSED JMI v0.9.66
//   S_msrOahGroup&     msrOpts,
//   S_msrScore         theMsrScore,
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
// #endif
//
//   // start the clock
//   clock_t startClock = clock ();
//
//   setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_OptionalPass);
//
//   std::string separator =
//     "%--------------------------------------------------------------";
//
//   gLogStream <<
//     std::endl <<
//     separator <<
//     std::endl <<
//     gTab <<
//     gWaeHandler->passOptional () << ": "<< passDescription <<
//     std::endl <<
//     separator <<
//     std::endl << std::endl <<
//     theMsrScore;
//
//   // register time spent
//   clock_t endClock = clock ();
//
//   mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
// 		 mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling,
//     gWaeHandler->displayTheFirstMSRSkeletonAsText (), // JMI ??? v0.9.66
//     mfTimingItemKind::kOptional,
//     startClock,
//     endClock);
// }


}
