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

#include "lpsrScores.h"

#include "waeInterface.h"
#include "msr2lpsrWae.h"
#include "lpsr2lilypondWae.h"

#include "mfStaticSettings.h"

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

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msrScore2lilypondWithHandler (
  const S_msrScore&   theMsrScore,
  mfPassIDKind        passID_A,
  std::string         passDescription_A,
  mfPassIDKind        passID_B,
  std::string         passDescription_B,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Translating an MSR score to LilyPond in \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gEarlyOptions.getEarlyQuietOption ()) {
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
          gMsrOahGroup,
          gLpsrOahGroup,
          passID_A,
          passDescription_A,
          createMsr2lilypondConverterComponent ());
    }
    catch (msr2lpsrException& e) {
      mfDisplayException (e, gOutput);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutput);
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

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      err <<
        "xmlFile2lilypond() outputFileName: \"" <<
        outputFileName <<
        "\"" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    if (! outputFileName.size ()) {
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        err <<
          "xmlFile2lilypond() output goes to standard output" <<
          std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // create an indented output stream for the LilyPond code
      // to be written to outputFileStream
      mfIndentedOstream
        lilypondStandardOutputStream (
          out,
          gIndenter);

      // convert the LPSR into LilyPond code
      try {
        translateLpsrToLilypond (
          theLpsrScore,
          gMsrOahGroup,
          gLpsrOahGroup,
          passID_B,
          passDescription_B,
          lilypondStandardOutputStream);
      }
      catch (lpsr2lilypondException& e) {
        mfDisplayException (e, gOutput);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }
      catch (std::exception& e) {
        mfDisplayException (e, gOutput);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }
    }

    else {
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        err <<
          "xmlFile2lilypond() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // open output file
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getEarlyTracePasses ()) {
        err <<
          std::endl <<
          gLanguage->openingLilypondFileForWriting (outputFileName) <<
          std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      std::ofstream
        outputFileStream (
          outputFileName.c_str (),
          std::ofstream::out);

      if (! outputFileStream.is_open ()) {
        std::stringstream ss;

        ss <<
          gLanguage->cannotOpenLilypondFileForWriting (outputFileName);

        std::string message = ss.str ();

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

      // convert the LPSR into LilyPond code
      try {
        translateLpsrToLilypond (
          theLpsrScore,
          gMsrOahGroup,
          gLpsrOahGroup,
          passID_B,
          passDescription_B,
          lilypondFileOutputStream);
      }
      catch (lpsr2lilypondException& e) {
        mfDisplayException (e, gOutput);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }
      catch (std::exception& e) {
        mfDisplayException (e, gOutput);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }

      // close output file
#ifdef TRACE_OAH
      if (gTraceOah->fTracePasses) {
        std::stringstream ss;

        ss <<
          std::endl <<
          gLanguage->closingLilypondFile (outputFileName) <<
          std::endl;

        gWaeHandler->waeTrace ( // JMI v0.9.67
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      outputFileStream.close ();
    }
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}


}
