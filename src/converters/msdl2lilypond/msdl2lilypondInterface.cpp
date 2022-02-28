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
                        // ifstream, ifstream::open(), ifstream::close()

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "oahWae.h"
#include "msdlWae.h"
#include "msr2msrWae.h"
#include "msr2lpsrWae.h"
#include "lpsr2lilypondWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msdl2lilypondComponent.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"

#include "oahEarlyOptions.h"

#include "msdlParser.h"

#include "msdl2lilypondInsiderHandler.h"
#include "msdl2lilypondRegularHandler.h"

#include "lpsrInterface.h"

#include "msr2lpsrInterface.h"
#include "lpsr2lilypondInterface.h"

#include "msdl2lilypondInterface.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
mfMusicformatsError convertMsdlStream2lilypondWithHandler (
  string        inputSourceName,
  istream&      inputStream,
  S_oahHandler  handler,
  std::ostream& out,
  std::ostream& err)
{
  // register the input source name
  gGlobalServiceRunData->setInputSourceName (
    inputSourceName);

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // the MSR score
  // ------------------------------------------------------

  S_msrScore theMsrScore;

  // translating the MSDL input into an MSR (pass 1)
  // ------------------------------------------------------

  try {
    // start the clock
    clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
      string separator =
        "%--------------------------------------------------------------";
      err <<
        endl <<
        separator <<
        endl <<
        gTab <<
        "Pass 1: Creating an MSR from the MSDL input" <<
        endl <<
        separator <<
        endl;
    }
#endif

    // create the MSDL parser
    msdlParser
      parser (inputStream);

    // parse the input
    parser.parse ();

    // get the resulting score
    // JMI an msrBook should also be handled
    theMsrScore = parser.getCurrentScore ();

    // register time spent
    clock_t endClock = clock ();

    mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
      "Pass 1",
      "Create the first MSR from the MSDL input",
      mfTimingItem::kMandatory,
      startClock,
      endClock);

    // sanity check
    if (! theMsrScore) {
      stringstream s;

      s <<
        "Could not perform comversion of \"" <<
        inputSourceName <<
        "\" to MSR, quitting";

      string message = s.str ();

      err <<
        message <<
        endl;

      throw msdl2msrException (message);
    }
  }
  catch (msdl2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2lyInsiderOahGroup->getQuitAfterPass1 ()) {
    err <<
      endl <<
      "Quitting after pass 1 as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // the LPSR score
  // ------------------------------------------------------
  S_lpsrScore theLpsrScore;

  {
    // create the LPSR from the MSR (pass 2)
    // ------------------------------------------------------

    try {
      theLpsrScore =
        translateMsrToLpsr (
          theMsrScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          "Pass 2",
          "Convert the MSR into an LPSR",
          createMsdl2lilypondConverterComponent ());
    }
    catch (msr2lpsrException& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsError::kErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsError::kErrorInvalidFile;
    }

    // display the LPSR score if requested
    // ------------------------------------------------------

    if (gGlobalLpsrOahGroup->getDisplayLpsr ()) {
      displayLpsrScore_OptionalPass (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    if (gGlobalLpsrOahGroup->getDisplayLpsrShort ()) {
      displayLpsrScoreShort_OptionalPass (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    if (gGlobalLpsrOahGroup->getDisplayLpsrFull ()) {
      displayLpsrScoreFull_OptionalPass (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2lyInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      endl <<
      "Quitting after pass 2b as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  {
    // convert the LPSR to LilyPond code (pass 3)
    // ------------------------------------------------------

    string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "msdlStream2lilypond() outputFileName = \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "msdlStream2lilypond() output goes to standard output" <<
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
          "Pass 3",
          "Convert the LPSR score to LilyPond code",
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
          "msdlStream2lilypond() output goes to file \"" <<
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
          "\" for writing, quitting";

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
          "Pass 4",
          "Convert the LPSR score to LilyPond code",
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

//_______________________________________________________________________________
 mfMusicformatsError convertMsdlStream2lilypondWithOptionsAndArguments (
  string                  inputSourceName,
  istream&                inputStream,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  // the service name
  // ------------------------------------------------------

  string serviceName = "msdl2lilypond";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

	// print the options and arguments
  // ------------------------------------------------------
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      handlerOptionsAndArguments;
  }
#endif
#endif

  // apply early options if any
  // ------------------------------------------------------

  gGlobalOahEarlyOptions.applyEarlyOptionsIfPresentInOptionsAndArguments (
    handlerOptionsAndArguments);

  // has the '-insider' option been used?
  // ------------------------------------------------------

  Bool insiderOption =
    gGlobalOahEarlyOptions.getEarlyInsiderOption ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      serviceName << " main()" <<
      ", insiderOption: " << insiderOption <<
      endl;
  }
#endif

  // create an msdl2lilypond insider OAH handler
  // ------------------------------------------------------

  // create an insider msdl2lilypond OAH handler
  S_msdl2lilypondInsiderHandler
    insiderOahHandler =
      msdl2lilypondInsiderHandler::create (
        serviceName,
        serviceName + " insider OAH handler with options and arguments",
        oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments);

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOption) {
    // use the insider msdl2lilypond OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular msdl2lilypond OAH handler
    handler =
      msdl2lilypondRegularHandler::create (
        serviceName,
        serviceName + " regular OAH handler",
        insiderOahHandler);
  }

  // create the global run data
  // ------------------------------------------------------

  gGlobalServiceRunData =
    mfServiceRunData::create (
      serviceName);

  // handle the command line options and arguments
  // ------------------------------------------------------

  try {
    // handle the options from the options and arguments
    oahElementHelpOnlyKind
      helpOnlyKind =
        handler->
          handleOptionsFromOptionsAndArguments (
            serviceName);

    // have help options been used?
    switch (helpOnlyKind) {
      case oahElementHelpOnlyKind::kElementHelpOnlyYes:
        return mfMusicformatsError::k_NoError; // quit now
        break;
      case oahElementHelpOnlyKind::kElementHelpOnlyNo:
        // go ahead
        break;
    } // switch
  }
  catch (mfOahException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidOption;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### " <<
      serviceName <<
      " gIndenter value after options ands arguments checking: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // do the job
  // ------------------------------------------------------

  convertMsdlStream2lilypondWithHandler (
    inputSourceName,
    inputStream,
    handler,
    out,
    err);

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlFile2lilypondWithOptionsAndArguments (
  string                  fileName,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  // open input file
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    err <<
      endl <<
      "Opening file \"" << fileName << "\" for writing" <<
      endl;
  }
#endif

  ifstream
    inputStream (
      fileName.c_str (),
      ifstream::in);

  if (! inputStream.is_open ()) {
    stringstream s;

    s <<
      "Could not open MSDL input file \"" <<
      fileName <<
      "\" for reading, quitting";

    string message = s.str ();

    err <<
      message <<
      endl;

    throw msdl2lilyponException (message);
  }

  return
    convertMsdlStream2lilypondWithOptionsAndArguments (
      fileName,
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);
}

mfMusicformatsError convertMsdlFile2lilypondWithHandler (
  string        fileName,
  S_oahHandler  handler,
  std::ostream& out,
  std::ostream& err)
{
  // open input file
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    err <<
      endl <<
      "Opening file \"" << fileName << "\" for writing" <<
      endl;
  }
#endif

  ifstream
    inputStream (
      fileName.c_str (),
      ifstream::in);

  if (! inputStream.is_open ()) {
    stringstream s;

    s <<
      "Could not open MSDL input file \"" <<
      fileName <<
      "\" for reading, quitting";

    string message = s.str ();

    err <<
      message <<
      endl;

    throw msdl2lilyponException (message);
  }

  return
    convertMsdlStream2lilypondWithHandler (
      fileName, inputStream, handler, out, err);
}

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlString2lilypondWithOptionsAndArguments (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call convertMsdlStream2lilypondWithOptionsAndArguments() even if buffer is empty,
	// to handle the help options if any
  return
    convertMsdlStream2lilypondWithOptionsAndArguments (
      "buffer",
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);

	return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMsdlString2lilypondWithHandler (
  const char*   buffer,
  S_oahHandler  handler,
  std::ostream& out,
  std::ostream& err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call convertMsdlStream2lilypondWithOptionsAndArguments() even if buffer is empty,
	// to handle the help options if any
  return
    convertMsdlStream2lilypondWithHandler (
      "buffer",
      inputStream,
      handler,
      out,
      err);

	return mfMusicformatsError::kErrorInvalidFile;
}


}
