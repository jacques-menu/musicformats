/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "msrScores.h"

#include "lpsrScores.h"

#include "msr2lpsrWae.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "lpsr2lilypondOah.h"

#include "msr2lpsrInterface.h"

#include "msr2lpsrTranslator.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_lpsrScore translateMsrToLpsr (
  const S_msrScore&          originalMsrScore,
  const S_msrOahGroup&       msrOpts,
  const S_lpsrOahGroup&      lpsrOpts,
  mfPassIDKind               passIDKind,
  std::string                passDescription,
  const S_mfcMultiComponent& multiComponent)
{
  if (gGlobalLpsr2lilypondOahGroup->getNoLilypondCode ()) {
    gLog <<
      "Option '-nolpc, -no-lilypond-code' is set, no LPSR is created" <<
      std::endl;

    return nullptr;
  }

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  if (gIndenter != 0) {
    if (! gEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream ss;

      ss <<
        "gIndenter value after pass 3: " <<
        gIndenter.getIndentation ();

      msr2lpsrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! resultingLpsr && ! gGlobalLpsr2lilypondOahGroup->getNoLilypondCode ()) {
    std::string message =
      "### Conversion from MSR to LPSR failed ###";

    gLog <<
      message <<
      std::endl;

    throw msr2lpsrException (message);
  }

  // check indentation
  if (gIndenter != 0) {
    gLog <<
      "### translateMsrToLpsrScore gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  return resultingLpsr;
}


}
