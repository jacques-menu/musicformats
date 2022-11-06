/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "bsr2bsrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfTiming.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "brailleGenerationOah.h"

#include "bsr2bsrFinalizer.h"

#include "bsr2bsrFinalizerInterface.h"


using namespace std;

namespace MusicFormats
{
//_______________________________________________________________________________
S_bsrScore translateBsrToFinalizedBsr (
  const S_bsrScore originalBsrScore,
  S_bsrOahGroup    bsrOpts,
  const string&    passNumber,
  const string&    passDescription)
{
  S_bsrScore finalizedBsrScore;

  if (! gGlobalBrailleGenerationOahGroup->getNoBrailleCode ()) {
    // sanity check
      mfAssert (
        __FILE__, __LINE__,
      originalBsrScore != nullptr,
      "originalBsrScore is null");

    S_bsrScore result;

    // start the clock
    clock_t startClock = clock ();

    string separator =
      "%--------------------------------------------------------------";

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
      gLogStream <<
        endl <<
        separator <<
        endl <<
        gTab <<
        passNumber << ": " << passDescription <<
        endl <<
        separator <<
        endl;
    }
#endif

    // create an bsr2bsrFinalizer
    bsr2bsrFinalizer
      translator (
        bsrOpts,
        originalBsrScore);

    // build the finalized BSR score
    translator.translateBsrToFinalizedBsr ();

    // get the result
    result = translator.getBsr ();

    // register time spent
    clock_t endClock = clock ();

    mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
      passNumber,
      passDescription,
      mfTimingItemKind::kMandatory,
      startClock,
      endClock);

    return result;
  }

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after " << passNumber << ": " <<
        gIndenter.getIndentation ();

      bsr2bsrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! finalizedBsrScore) {
    string message =
      "### Conversion from first BSR to finalized BSR failed ###";

    gLogStream <<
      message <<
      endl << endl;

    throw bsr2finalizedBsrException (message);
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### translateBsrToFinalizedBsrScore gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  return finalizedBsrScore;
}


}
