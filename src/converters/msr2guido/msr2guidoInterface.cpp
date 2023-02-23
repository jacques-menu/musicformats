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
#include "mxsr2guidoWae.h"

#include "mfStaticSettings.h"

#include "oahEarlyOptions.h"

#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"
#include "mxsrOah.h"

#include "msr2guidoInsiderHandler.h"
#include "msr2guidoRegularHandler.h"

#include "msr2msrInterface.h"
#include "msr2mxsrInterface.h"
#include "mxsr2guidoTranlatorInterface.h"

#include "mxsrGeneration.h"

#include "msr2guidoInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msrScore2guidoWithHandler (
  const S_msrScore&   theMsrScore,
  mfPassIDKind        passID_A,
  std::string         passDescription_A,
  mfPassIDKind        passID_B,
  std::string         passDescription_B,
  mfPassIDKind        passID_C,
  std::string         passDescription_C,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gEarlyOptions.getEarlyQuietOption ()) {
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
        gMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        passID_A,
        passDescription_A);
  }
  catch (msr2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // convert the second MSR into an MXSR
  // ------------------------------------------------------

  Sxmlelement secondMxsr;

  try {
    secondMxsr =
      translateMsrToMxsr (
        secondMsrScore,
        gMsrOahGroup,
        passID_B,
        passDescription_B,
        mfTimingItemKind::kMandatory);
  }
  catch (msr2mxsrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "msrScore2guidoWithHandler(), secondMxsr contains:" <<
      std::endl << std::endl;

    ++gIndenter;

    printMxsr (secondMxsr, gLog);

    gLog <<
      std::endl;

    --gIndenter;

    gLog <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // convert the MXSR to Guido
  // ------------------------------------------------------

  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    translateMxsrToGuido (
      secondMxsr,
      outputFileName,
      err,
      passID_C,
      passDescription_C);
  }
  catch (mxsr2guidoException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}


}
