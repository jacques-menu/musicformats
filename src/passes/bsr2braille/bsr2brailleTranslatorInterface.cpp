/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <fstream>      // std::ofstream, std::ofstream::open(), std::ofstream::close()

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfTiming.h"

#include "oahEarlyOptions.h"

#include "brailleGenerationOah.h"

#include "bsr2brailleTranslator.h"

#include "bsr2brailleTranslatorInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
void translateBsrToBraille (
  const S_bsrScore&    originalBsrScore,
  const S_bsrOahGroup& bsrOpts,
  mfPassIDKind         passIDKind,
  const std::string&   passDescription,
  std::ostream&        brailleOutputStream)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalBsrScore != nullptr,
    "originalBsrScore is null");
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
      separator <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create a bsr2brailleTranslator
  bsr2brailleTranslator
    translator (
      originalBsrScore,
      bsrOpts,
      brailleOutputStream);

  // generate the Braille code
  translator.translateBsrToBraille ();

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLog <<
      "### translateBsrToBraille gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }
}


}
