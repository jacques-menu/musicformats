/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfTiming.h"

#include "oahEarlyOptions.h"


#include "brailleGenerationOah.h"

#include "bsr2brailleTranslator.h"

#include "bsr2brailleTranslatorInterface.h"


using namespace std;

namespace MusicFormats
{
//_______________________________________________________________________________
void translateBsrToBraille (
  const S_bsrScore originalBsrScore,
  S_bsrOahGroup    bsrOpts,
  const string&    passNumber,
  const string&    passDescription,
  ostream&         brailleOutputStream)
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
    mfTimingItem::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### translateBsrToBraille gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }
}


}
