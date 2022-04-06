/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfTiming.h"

#include "msr2bsrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "brailleGenerationOah.h"

#include "msr2bsrInterface.h"

#include "msr2bsrTranslator.h"

using namespace std;

namespace MusicFormats
{
//_______________________________________________________________________________
S_bsrScore translateMsrToBsr (
  S_msrScore    originalMsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts,
  const string& passNumber,
  const string& passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

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

  // the msr2bsrTranslator
  msr2bsrTranslator
    translator;

  // build the BSR score
  S_bsrScore
    resultingBsr =
      translator.translateMsrToBsr (
        originalMsrScore);

  clock_t endClock = clock ();

  // register time spent
  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItem::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      stringstream s;

      s <<
        "gIndenter value after pass 3a: " <<
        gIndenter.getIndentation ();

      msr2bsrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! resultingBsr) {
    string message =
      "### Conversion from MSR to BSR failed ###";

    gLogStream <<
      message <<
      endl;

    throw msr2bsrException (message);
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### translateMsrToBsr gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  return resultingBsr;
}


}
