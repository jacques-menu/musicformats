/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

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

using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
Sxmlelement translateMsrToMxsr (
  const S_msrScore theMsrScore,
  S_msrOahGroup    msrOpts,
  string           passNumber,
  string           passDescription,
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
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber + ": Translating the MSR into an MXSR" <<
      endl <<
      separator <<
      endl;
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
      endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl <<
      "translateMsrToMxsr(), resultingMxsr contains:" <<
      endl << endl;

    ++gIndenter;

    printMxsr (resultingMxsr, gLogStream);

    gLogStream <<
      endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl << endl;
  }
#endif

  // return it
  return resultingMxsr;
}


}
