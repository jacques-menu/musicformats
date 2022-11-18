/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfTiming.h"

#include "lpsrScores.h"

#include "lpsrWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "oahOah.h"
#include "waeOah.h"

#include "lpsrInterface.h"

namespace MusicFormats
{
//_______________________________________________________________________________
void displayLpsrScoreShort (
  const S_lpsrScore& theLpsrScore,
  const S_msrOahGroup&     msrOpts,
  const S_lpsrOahGroup&    lpsrOpts,
  const std::string&     passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  std::string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    std::endl <<
    gTab <<
    "Pass (optional): " << passDescription << ", short version" <<
    std::endl <<
    separator <<
    std::endl << std::endl;

  theLpsrScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the LPSR as text, short version",
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScoreFull (
  const S_lpsrScore& theLpsrScore,
  const S_msrOahGroup&     msrOpts,
  const S_lpsrOahGroup&    lpsrOpts,
  const std::string&     passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  std::string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    std::endl <<
    gTab <<
    "Pass (optional): " << passDescription << ", full version" <<
    std::endl <<
    separator <<
    std::endl << std::endl;

  theLpsrScore->printFull (gLogStream);

  gLogStream <<
    separator <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the LPSR as text, full version",
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
//         "gIndenter value after LPSR score display: " <<
//         gIndenter.getIndentation ();
//
//      lpsrWarning (
//         gGlobalServiceRunData->getInputSourceName (),
//         1, // JMI inputLineNumber,
//         s.str ());
//     }
//
//     gIndenter.resetToZero ();
//   }
