/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <fstream>      // std::ofstream, std::ofstream::open(), std::ofstream::close()

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfAssert.h"
#include "mfTiming.h"

#include "oahEarlyOptions.h"


#include "brailleGenerationOah.h"

#include "bsr2brailleTranslator.h"

#include "bsr2brailleTranslatorInterface.h"


namespace MusicFormats
{
//_______________________________________________________________________________
void translateBsrToBraille (
  const S_bsrScore originalBsrScore,
  const S_bsrOahGroup&    bsrOpts,
  const std::string&    passNumber,
  const std::string&    passDescription,
  std::ostream&         brailleOutputStream)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalBsrScore != nullptr,
    "originalBsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  std::string separator =
    "%--------------------------------------------------------------";

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
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

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### translateBsrToBraille gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }
}


}
