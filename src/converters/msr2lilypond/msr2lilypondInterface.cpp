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

#include "lpsrScores.h"

#include "waeInterface.h"
#include "msr2lpsrWae.h"
#include "lpsr2lilypondWae.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "msr2lilypondComponent.h"

#include "waeOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"

#include "oahEarlyOptions.h"

#include "msr2lilypondInsiderHandler.h"
#include "msr2lilypondRegularHandler.h"

#include "lpsrInterface.h"

#include "msr2lpsrInterface.h"
#include "lpsr2lilypondInterface.h"

#include "msr2lilypondInterface.h"

namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msrScore2lilypondWithHandler (
  S_msrScore   theMsrScore,
  std::string       passNumber1,
  std::string       passDescription1,
  std::string       passNumber2,
  std::string       passDescription2,
  std::ostream&     out,
  std::ostream&     err,
  const S_oahHandler& handler)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Translating an MSR score to LilyPond in \"" <<
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

  // the LPSR score
  S_lpsrScore theLpsrScore;

  {
    // convert the MSR into an LPSR
    // ------------------------------------------------------

    try {
      theLpsrScore =
        translateMsrToLpsr (
          theMsrScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          passNumber1,
          passDescription1,
          createMsr2lilypondConverterComponent ());
    }
    catch (msr2lpsrException& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }
  }

  {
    // convert the LPSR into LilyPond code
    // ------------------------------------------------------

    std::string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2lilypond() outputFileName = \"" <<
        outputFileName <<
        "\"" <<
        std::endl;
    }
#endif

    if (! outputFileName.size ()) {
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2lilypond() output goes to standard output" <<
          std::endl;
      }
#endif

      // create an indented output stream for the LilyPond code
      // to be written to outputFileStream
      mfIndentedOstream
        lilypondStandardOutputStream (
          out,
          gIndenter);

      // convert the LPSR score to LilyPond code
      try {
        translateLpsrToLilypond (
          theLpsrScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          passNumber2,
          passDescription2,
          lilypondStandardOutputStream);
      }
      catch (lpsr2lilypondException& e) {
        mfDisplayException (e, gOutputStream);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }
      catch (std::exception& e) {
        mfDisplayException (e, gOutputStream);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }
    }

    else {
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2lilypond() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif

      // open output file
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
        err <<
          std::endl <<
          "Opening file '" << outputFileName << "' for writing" <<
          std::endl;
      }
#endif

      std::ofstream
        outputFileStream (
          outputFileName.c_str (),
          std::ofstream::out);

      if (! outputFileStream.is_open ()) {
        std::stringstream s;

        s <<
          "Could not open LilyPond output file \"" <<
          outputFileName <<
          "\" for writing - quitting";

        std::string message = s.str ();

        err <<
          message <<
          std::endl;

        throw lpsr2lilypondException (message);
      }

      // create an indented output stream for the LilyPond code
      // to be written to outputFileStream
      mfIndentedOstream
        lilypondFileOutputStream (
          outputFileStream,
          gIndenter);

      // convert the LPSR score to LilyPond code
      try {
        translateLpsrToLilypond (
          theLpsrScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          passNumber2,
          passDescription2,
          lilypondFileOutputStream);
      }
      catch (lpsr2lilypondException& e) {
        mfDisplayException (e, gOutputStream);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }
      catch (std::exception& e) {
        mfDisplayException (e, gOutputStream);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }

      // close output file
#ifdef TRACE_OAH
      if (gtracingOah->fTracePasses) {
        gLogStream <<
          std::endl <<
          "Closing file \"" << outputFileName << "\"" <<
          std::endl;
      }
#endif

      outputFileStream.close ();
    }
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}


}
