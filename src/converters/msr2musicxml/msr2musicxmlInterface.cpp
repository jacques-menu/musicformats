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

#include "msrScores.h"

#include "msr2msrWae.h"
#include "msr2mxsrWae.h"
#include "mxsr2musicxmlWae.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
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


namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msrScore2musicxmlWithHandler (
  S_msrScore          theMsrScore,
  std::string         passNumber1,
  std::string         passDescription1,
  std::string         passNumber2,
  std::string         passDescription2,
  std::string         passNumber3,
  std::string         passDescription3,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Translating an MSR score to MusicXML in \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      std::endl;
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
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
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
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    gLogStream <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "msrScore2musicxmlWithHandler(), theMxsr contains:" <<
      std::endl << std::endl;

    ++gIndenter;

    printMxsr (theMxsr, gLogStream);

    gLogStream <<
      std::endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl << std::endl;
  }
#endif

  // convert the MXSR into MusicXML text
  // ------------------------------------------------------

  std::string
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
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}


}
