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

#include "lpsrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "oahOah.h"
#include "waeOah.h"

#include "lpsrInterface.h"

using namespace std;

namespace MusicFormats
{
//_______________________________________________________________________________
void displayLpsrScore (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  const string&     passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): " << passDescription <<
    endl <<
    separator <<
    endl << endl <<
    theLpsrScore <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the LPSR as text",
    mfTimingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScoreShort (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  const string&     passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): " << passDescription << ", short version" <<
    endl <<
    separator <<
    endl << endl;

  theLpsrScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the LPSR as text, short version",
    mfTimingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayLpsrScoreFull (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  const string&     passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): " << passDescription << ", full version" <<
    endl <<
    separator <<
    endl << endl;

  theLpsrScore->printFull (gLogStream);

  gLogStream <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the LPSR as text, full version",
    mfTimingItem::kOptional,
    startClock,
    endClock);
}


}

//   if (gIndenter != 0) {
//     if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
//       stringstream s;
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
