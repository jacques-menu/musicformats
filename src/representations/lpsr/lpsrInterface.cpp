/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "lpsrScores.h"

#include "displayLpsrSummaryVisitor.h"

#include "lpsrWae.h"

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
    "theLpsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    std::endl <<
    separator <<
    std::endl <<
    std::endl << std::endl <<

    theLpsrScore <<

    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
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
    "theLpsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    ", " << gLanguage->fullVersion () <<
    std::endl <<
    separator <<
    std::endl << std::endl;


  theLpsrScore->printFull (gLog);

  gLog <<
    separator <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
    gLanguage->displayTheLPSRAsText ()
      + ", " +
    gLanguage->fullVersion (),
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScoreSummary (
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
    "theLpsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    ", " << gLanguage->fullVersion () <<
    std::endl <<
    separator <<
    std::endl << std::endl;

  // create an displayLpsrSummaryVisitor visitor
  displayLpsrSummaryVisitor
    summaryVisitor (
      lpsrOpts);

  summaryVisitor.printSummaryFromLpsrScore (
    theLpsrScore);

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
    gLanguage->displayTheLPSRAsText ()
      + ", " +
    gLanguage->fullVersion (),
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScoreNames (
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
    "theLpsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    ", " << gLanguage->fullVersion () <<
    std::endl <<
    separator <<
    std::endl << std::endl;


  theLpsrScore->printNames (gLog);

  gLog <<
    separator <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
    gLanguage->displayTheLPSRAsText ()
      + ", " +
    gLanguage->fullVersion (),
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScoreFlatView (
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
    "theLpsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    ", " << gLanguage->fullVersion () <<
    std::endl <<
    separator <<
    std::endl << std::endl;


  theLpsrScore->printFlatView (gLog);

  gLog <<
    separator <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
    gLanguage->displayTheLPSRAsText ()
      + ", " +
    gLanguage->fullVersion (),
    mfTimingItemKind::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScoreSlices (
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
    "theLpsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  setGlobalCurrentPassIDKind (mfPassIDKind::kMfPassID_DisplayPass);

  std::string separator =
    "%--------------------------------------------------------------";

  gLog <<
    separator <<
    std::endl <<
    gTab <<
    gLanguage->passOptional () << ": "<< passDescription <<
    ", " << gLanguage->fullVersion () <<
    std::endl <<
    separator <<
    std::endl << std::endl;


  gLog << // JMI v0.9.70
    "theLpsrScore->printSlices (gLog);" <<
    std::endl;

  gLog <<
    separator <<
    std::endl << std::endl;

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_DisplayPass,
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
