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

#include "msr2msrWae.h"
#include "msr2mxsrWae.h"
#include "mxsr2musicxmlWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"
#include "mxsrOah.h"

#include "oahEarlyOptions.h"

#include "msr2musicxmlInsiderHandler.h"
#include "msr2musicxmlRegularHandler.h"

#include "mxsr2msrSkeletonBuilderInterface.h"
#include "mxsr2msrTranslatorInterface.h"

#include "msr2msrInterface.h"
#include "msr2mxsrInterface.h"
#include "mxsr2musicxmlTranlatorInterface.h"

#include "mxsrGeneration.h"         // printMxsr()


#include "msr2musicxmlInterface.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsError msrScore2musicxmlWithHandler (
  S_msrScore   theMsrScore,
  string       passNumber1,
  string       passDescription1,
  string       passNumber2,
  string       passDescription2,
  string       passNumber3,
  string       passDescription3,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Translating an MSR score to MusicXML in \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      endl;
  }
#endif

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // convert the first MSR score into a second MSR
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      translateMsrToMsr (
        theMsrScore,
        gGlobalMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        passNumber1,
        passDescription1);
  }
  catch (msr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // convert the second MSR into an MXSR
  // ------------------------------------------------------

  Sxmlelement theMxsr;

  try {
    theMxsr =
      translateMsrToMxsr (
        secondMsrScore,
        gGlobalMsrOahGroup,
        passNumber2,
        passDescription2,
        mfTimingItemKind::kMandatory);
  }
  catch (msr2mxsrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    gLogStream <<
      endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl <<
      "msrScore2musicxmlWithHandler(), theMxsr contains:" <<
      endl << endl;

    ++gIndenter;

    printMxsr (theMxsr, gLogStream);

    gLogStream <<
      endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl << endl;
  }
#endif

  // convert the MXSR into MusicXML text
  // ------------------------------------------------------

  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    translateMxsrToMusicXML (
      theMxsr,
      outputFileName,
      err,
      passNumber3,
      passDescription3);
  }
  catch (mxsr2musicxmlException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  return mfMusicformatsError::k_NoError;
}


}
