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

#include "wae.h"
#include "msr2lpsrWae.h"
#include "lpsr2lilypondWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
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

using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsError msrScore2lilypondWithHandler (
  S_msrScore   theMsrScore,
  string       passNumber1,
  string       passDescription1,
  string       passNumber2,
  string       passDescription2,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Translating an MSR score to LilyPond in \"" <<
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
      return mfMusicformatsError::kErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsError::kErrorInvalidFile;
    }
  }

  {
    // convert the LPSR into LilyPond code
    // ------------------------------------------------------

    string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2lilypond() outputFileName = \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2lilypond() output goes to standard output" <<
          endl;
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
        return mfMusicformatsError::kErrorInvalidFile;
      }
      catch (std::exception& e) {
        mfDisplayException (e, gOutputStream);
        return mfMusicformatsError::kErrorInvalidFile;
      }
    }

    else {
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2lilypond() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

      // open output file
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
        err <<
          endl <<
          "Opening file '" << outputFileName << "' for writing" <<
          endl;
      }
#endif

      ofstream
        outputFileStream (
          outputFileName.c_str (),
          ofstream::out);

      if (! outputFileStream.is_open ()) {
        stringstream s;

        s <<
          "Could not open LilyPond output file \"" <<
          outputFileName <<
          "\" for writing - quitting";

        string message = s.str ();

        err <<
          message <<
          endl;

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
        return mfMusicformatsError::kErrorInvalidFile;
      }
      catch (std::exception& e) {
        mfDisplayException (e, gOutputStream);
        return mfMusicformatsError::kErrorInvalidFile;
      }

      // close output file
#ifdef TRACE_OAH
      if (gtracingOah->fTracePasses) {
        gLogStream <<
          endl <<
          "Closing file \"" << outputFileName << "\"" <<
          endl;
      }
#endif

      outputFileStream.close ();
    }
  }

  return mfMusicformatsError::k_NoError;
}


}
