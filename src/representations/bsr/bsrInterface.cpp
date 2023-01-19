/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "bsrWae.h"

#include "mfStaticSettings.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"
#include "waeOah.h"

#include "bsrInterface.h"

namespace MusicFormats
{
//_______________________________________________________________________________
void displayBsrScore (
  S_bsrScore    bsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts,
  const std::string& passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    bsrScore != nullptr,
    "bsrScore is null");
#endif

  // start the clock
  clock_t startClock = clock ();

  std::string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    std::endl <<
    gTab <<
    gWaeHandler->passOptional () <<
    ": "<<
    passDescription <<
    std::endl <<
    separator <<
    std::endl << std::endl <<
    bsrScore <<
    separator <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
		gWaeHandler->passIDKindAsString (mfPassIDKind::kMfPassID_0),
    gWaeHandler->displayTheBSRAsText (), // JMI ??? v0.9.66
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayBsrScoreFull (
  S_bsrScore    bsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts,
  const std::string& passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    bsrScore != nullptr,
    "bsrScore is null");
#endif

  // start the clock
  clock_t startClock = clock ();

  std::string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    std::endl <<
    gTab <<
    gWaeHandler->passOptional () <<
    ": "<<
    passDescription <<
    ", " <<
    gWaeHandler->fullVersion () <<
    std::endl <<
    separator <<
    std::endl << std::endl;

  gLogStream << bsrScore;

  gLogStream <<
    separator <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
		gWaeHandler->passIDKindAsString (mfPassIDKind::kMfPassID_0),
    gWaeHandler->displayTheBSRAsText ()
      + ", " +
    gWaeHandler->fullVersion (),
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}


}

//   if (gIndenter != 0) {
//     if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
//       std::stringstream s;
//
//       s <<
//         "gIndenter value after BSR score short display: " <<
//         gIndenter.getIndentation ();
//
//       bsrWarning (
//         gGlobalServiceRunData->getInputSourceName (),
//         1, // JMI inputLineNumber,
//         s.str ());
//     }
//
//     gIndenter.resetToZero ();
//   }
