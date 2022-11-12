/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>      // std::ofstream, std::ofstream::open(), std::ofstream::close()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfTiming.h"


#include "mxsrOah.h"

#include "oahEarlyOptions.h"

#include "mxsrGeneration.h"

#include "msr2mxsrInterface.h"

#include "msr2mxsrTranslator.h"

namespace MusicFormats
{

//_______________________________________________________________________________
Sxmlelement translateMsrToMxsr (
  const S_msrScore theMsrScore,
  S_msrOahGroup    msrOpts,
  std::string           passNumber,
  std::string           passDescription,
  mfTimingItemKind mfTimingItemKind)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      passNumber + ": Translating the MSR into an MXSR" <<
      std::endl <<
      separator <<
      std::endl;
  }
#endif

  // create an msr2mxsrTranslator
  msr2mxsrTranslator
    translator (
      theMsrScore);

  // build the MXSR
  Sxmlelement
    resultingMxsr =
      translator.translateMsrToMxsr ();

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    gLogStream <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "translateMsrToMxsr(), resultingMxsr contains:" <<
      std::endl << std::endl;

    ++gIndenter;

    printMxsr (resultingMxsr, gLogStream);

    gLogStream <<
      std::endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl << std::endl;
  }
#endif

  // return it
  return resultingMxsr;
}


}
