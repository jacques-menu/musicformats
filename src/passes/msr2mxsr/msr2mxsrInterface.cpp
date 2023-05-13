/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>      // std::ofstream, std::ofstream::open(), std::ofstream::close()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfTiming.h"

#include "mxsrOah.h"

#include "oahEarlyOptions.h"

#include "mxsrGeneration.h"

#include "msr2mxsrInterface.h"

#include "msr2mxsrTranslator.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
Sxmlelement translateMsrToMxsr (
  const S_msrScore theMsrScore,
  S_msrOahGroup    msrOpts,
  mfPassIDKind     passIDKind,
  std::string      passDescription,
  mfTimingItemKind mfTimingItemKind)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMsrScore != nullptr,
    "theMsrScore is null");
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

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "translateMsrToMxsr(), resultingMxsr contains:" <<
      std::endl << std::endl;

    ++gIndenter;

    printMxsr (resultingMxsr, gLog);

    gLog <<
      std::endl;

    --gIndenter;

    gLog <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl << std::endl;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // return it
  return resultingMxsr;
}


}
