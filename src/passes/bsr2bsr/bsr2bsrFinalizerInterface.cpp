/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "bsr2bsrWae.h"

#include "brailleGenerationOah.h"

#include "bsr2bsrFinalizer.h"

#include "bsr2bsrFinalizerInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_bsrScore translateBsrToFinalizedBsr (
  const S_bsrScore&    originalBsrScore,
  const S_bsrOahGroup& bsrOpts,
  mfPassIDKind         passIDKind,
  const std::string&   passDescription)
{
  S_bsrScore finalizedBsrScore;

  if (! gGlobalBrailleGenerationOahGroup->getNoBrailleCode ()) {
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
      mfAssert (
        __FILE__, __LINE__,
      originalBsrScore != nullptr,
      "originalBsrScore is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

    S_bsrScore result;

    // start the clock
    clock_t startClock = clock ();

    std::string separator =
      "%--------------------------------------------------------------";

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTracePasses ()) {
      std::stringstream ss;

      ss <<
        std::endl <<
        separator <<
        std::endl <<
        gTab <<
        gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
        std::endl <<
        separator <<
        std::endl;

      gWaeHandler->waeTraceWithoutLocationDetails (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

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

    gGlobalTimingItemsList.appendTimingItem (
      passIDKind,
      passDescription,
      mfTimingItemKind::kMandatory,
      startClock,
      endClock);

    return result;
  }

  if (gIndenter != 0) {
    if (! gEarlyOptions.getEarlyQuietOption ()) {
      std::stringstream ss;

      ss <<
        "gIndenter value after " << passIDKind << ": " <<
        gIndenter.getIndentation ();

      bsr2bsrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! finalizedBsrScore) {
    std::string message =
      "### Conversion from first BSR to finalized BSR failed ###";

    gLog <<
      message <<
      std::endl << std::endl;

    throw bsr2finalizedBsrException (message);
  }

  // check indentation
  if (gIndenter != 0) {
    gLog <<
      "### translateBsrToFinalizedBsrScore gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  return finalizedBsrScore;
}


}
