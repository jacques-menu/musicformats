/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "mfStaticSettings.h"

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfTiming.h"

#include "msrScores.h"

#include "bsrScores.h"

#include "msr2bsrWae.h"

#include "oahOah.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "brailleGenerationOah.h"

#include "msr2bsrInterface.h"

#include "msr2bsrTranslator.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_bsrScore translateMsrToBsr (
  const S_msrScore&    originalMsrScore,
  const S_msrOahGroup& msrOpts,
  const S_bsrOahGroup& bsrOpts,
  mfPassIDKind         passIDKind,
  const std::string&   passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalMsrScore != nullptr,
    "originalMsrScore is null");
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

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        "gIndenter value after pass 3a: " <<
        gIndenter.getIndentation ();

      msr2bsrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }

    gIndenter.resetToZero ();
  }

  if (! resultingBsr) {
    std::string message =
      "### Conversion from MSR to BSR failed ###";

    gLog <<
      message <<
      std::endl;

    throw msr2bsrException (message);
  }

  // check indentation
  if (gIndenter != 0) {
    gLog <<
      "### translateMsrToBsr gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  return resultingBsr;
}


}
