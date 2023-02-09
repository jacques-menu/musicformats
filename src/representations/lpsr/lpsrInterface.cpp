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

#include "lpsrScores.h"

#include "lpsrWae.h"

#include "mfStaticSettings.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"
#include "waeOah.h"

#include "lpsrInterface.h"

namespace MusicFormats
{
//_______________________________________________________________________________
void displayLpsrScore (
  const S_lpsrScore&     theLpsrScore,
  const S_msrOahGroup&   msrOpts,
  const S_lpsrOahGroup&  lpsrOpts,
  const std::string&     passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");
#endif

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_OptionalPass);

#ifdef MF_TRACING_IS_ENABLED
  std::string separator =
    "%--------------------------------------------------------------";

  std::stringstream ss;

  ss <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    std::endl <<
    separator <<
    std::endl << std::endl;

  ss << theLpsrScore;

  ss <<
    separator <<
    std::endl << std::endl;

  gWaeHandler->waeTrace (
    __FILE__, __LINE__,
    ss.str ());
#endif

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
		mfPassIDKind::kMfPassID_OptionalPass,
    gLanguage->displayTheLPSRAsText (),
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScoreFull (
  const S_lpsrScore&     theLpsrScore,
  const S_msrOahGroup&   msrOpts,
  const S_lpsrOahGroup&  lpsrOpts,
  const std::string&     passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");
#endif

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_OptionalPass);

  std::string separator =
    "%--------------------------------------------------------------";

#ifdef MF_TRACING_IS_ENABLED
  std::stringstream ss;

  ss <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    ", " <<
    gLanguage->fullVersion () <<
    std::endl <<
    separator <<
    std::endl << std::endl;

  gWaeHandler->waeTrace (
    __FILE__, __LINE__,
    ss.str ());
#endif

  theLpsrScore->printFull (gLog);

  gLog <<
    separator <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
		mfPassIDKind::kMfPassID_OptionalPass,
    gLanguage->displayTheLPSRAsText ()
      + ", " +
    gLanguage->fullVersion (),
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}


}

//   if (gIndenter != 0) {
//     if (! gEarlyOptions.getEarlyQuietOption ()) {
//       std::stringstream ss;
//
//       ss <<
//         "gIndenter value after LPSR score display: " <<
//         gIndenter.getIndentation ();
//
//      lpsrWarning (
//         gServiceRunData->getInputSourceName (),
//         1, // JMI inputLineNumber,
//         ss.str ());
//     }
//
//     gIndenter.resetToZero ();
//   }
