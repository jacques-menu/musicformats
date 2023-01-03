/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfTiming.h"

#include "msrScores.h"

#include "lpsrScores.h"

#include "msr2lpsrWae.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "lpsr2lilypondOah.h"

#include "msr2lpsrInterface.h"

#include "msr2lpsrTranslator.h"




namespace MusicFormats
{
//_______________________________________________________________________________
S_lpsrScore translateMsrToLpsr (
  const S_msrScore&          originalMsrScore,
  const S_msrOahGroup&       msrOpts,
  const S_lpsrOahGroup&      lpsrOpts,
  std::string                passNumber,
  std::string                passDescription,
  const S_mfcMultiComponent& multiComponent)
{
  if (gGlobalLpsr2lilypondOahGroup->getNoLilypondCode ()) {
    gLogStream <<
      "Option '-nolpc, -no-lilypond-code' is set, no LPSR is created" <<
      std::endl;

    return nullptr;
  }

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      passNumber << ": " << passDescription <<
      std::endl <<
      separator <<
      std::endl;
  }
#endif

  // the msr2lpsrTranslator
  msr2lpsrTranslator
    translator;

  // build the LPSR score
  S_lpsrScore
    resultingLpsr =
      translator.translateMsrToLpsr (
        originalMsrScore,
        multiComponent);

  clock_t endClock = clock ();

  // register time spent
  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream s;

      s <<
        "gIndenter value after pass 3: " <<
        gIndenter.getIndentation ();

      msr2lpsrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        s.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! resultingLpsr && ! gGlobalLpsr2lilypondOahGroup->getNoLilypondCode ()) {
    std::string message =
      "### Conversion from MSR to LPSR failed ###";

    gLogStream <<
      message <<
      std::endl;

    throw msr2lpsrException (message);
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### translateMsrToLpsrScore gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  return resultingLpsr;
}


}
