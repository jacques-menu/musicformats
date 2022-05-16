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

#include "bsrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "oahOah.h"
#include "waeOah.h"

#include "bsrInterface.h"

using namespace std;

namespace MusicFormats
{
//_______________________________________________________________________________
void displayBsrScore (
  S_bsrScore    bsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts,
  const string& passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    bsrScore != nullptr,
    "bsrScore is null");

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
    bsrScore <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the first BSR as text",
    mfTimingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayBsrScoreShort (
  S_bsrScore    bsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts,
  const string& passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    bsrScore != nullptr,
    "bsrScore is null");

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

  bsrScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the first BSR as text, short version",
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
