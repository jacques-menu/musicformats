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
void displayFirstBsr (
  const S_bsrScore originalBsrScore,
  S_msrOahGroup    msrOpts,
  S_bsrOahGroup    bsrOpts)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalBsrScore != nullptr,
    "originalBsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): display the first BSR as text" <<
    endl <<
    separator <<
    endl << endl <<
    originalBsrScore <<
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

void displayFirstBsrScoreShort (
  const S_bsrScore originalBsrScore,
  S_msrOahGroup    msrOpts,
  S_bsrOahGroup    bsrOpts)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalBsrScore != nullptr,
    "originalBsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): display the first BSR as text, short version" <<
    endl <<
    separator <<
    endl << endl;

  originalBsrScore->printShort (gLogStream);

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

//_______________________________________________________________________________
void displayBsrFirstScore_OptionalPass (
  S_bsrScore    originalBsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts)
{
  // display it
  displayFirstBsr (
    originalBsrScore,
    msrOpts,
    bsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after first BSR score display: " <<
        gIndenter.getIndentation ();

      bsrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayBsrFirstScoreShort_OptionalPass (
  S_bsrScore    originalBsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts)
{
  // display it
  displayFirstBsrScoreShort (
    originalBsrScore,
    msrOpts,
    bsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after first BSR score short display: " <<
        gIndenter.getIndentation ();

      bsrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

//_______________________________________________________________________________
void displayFinalizedBsr (
  const S_bsrScore originalBsrScore,
  S_msrOahGroup    msrOpts,
  S_bsrOahGroup    bsrOpts)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalBsrScore != nullptr,
    "originalBsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): display the finalized BSR as text" <<
    endl <<
    separator <<
    endl <<
    originalBsrScore <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the finalized BSR as text",
    mfTimingItem::kOptional,
    startClock,
    endClock);
}

void displayFinalizedBsrScoreShort (
  const S_bsrScore originalBsrScore,
  S_msrOahGroup    msrOpts,
  S_bsrOahGroup    bsrOpts)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalBsrScore != nullptr,
    "originalBsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  gLogStream <<
    separator <<
    endl <<
    gTab <<
    "Pass (optional): display the finalized BSR as text, short version" <<
    endl <<
    separator <<
    endl;

  originalBsrScore->printShort (gLogStream);

  gLogStream <<
    separator <<
    endl << endl;

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "",
    "Display the finalized BSR as text, short version",
    mfTimingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void displayFinalizedBsrScore_OptionalPass (
  S_bsrScore    originalBsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts)
{
  // display it
  displayFinalizedBsr (
    originalBsrScore,
    msrOpts,
    bsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after BSR score display: " <<
        gIndenter.getIndentation ();

      bsrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}

void displayFinalizedBsrScoreShort_OptionalPass (
  S_bsrScore    originalBsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts)
{
  // display it
  displayFinalizedBsrScoreShort (
    originalBsrScore,
    msrOpts,
    bsrOpts);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after BSR score short display: " <<
        gIndenter.getIndentation ();

      bsrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }
}


}
