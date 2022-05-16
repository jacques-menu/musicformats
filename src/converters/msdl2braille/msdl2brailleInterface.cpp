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

#include "mfBool.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "oahWae.h"
#include "msdlWae.h"
#include "msr2msrWae.h"
#include "msr2bsrWae.h"
#include "bsr2bsrWae.h"
#include "bsr2brailleWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"

#include "oahEarlyOptions.h"

#include "msdlParser.h"

#include "msdl2brailleInsiderHandler.h"
#include "msdl2brailleRegularHandler.h"

#include "msr2msrInterface.h"
#include "msr2bsrInterface.h"
#include "bsr2bsrFinalizerInterface.h"
#include "bsr2brailleTranslatorInterface.h"

#include "bsrInterface.h"

#include "msdl2brailleInterface.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
mfMusicformatsError convertMsdlStream2brailleWithHandler (
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

  // the first MSR score
  // ------------------------------------------------------

  S_msrScore firstMsrScore;

  // translating the MSDL input into a first MSR (pass 1)
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
        "Pass 1: Creating a first MSR from the MSDL input" <<
        endl <<
        separator <<
        endl;
    }
#endif

    // create the MSDL parser
    msdlParser parser (inputStream);

    // parse the input
    parser.parse ();

    // get the resulting score
    // JMI an msrBook should also be handled
    firstMsrScore = parser.getCurrentScore ();

    // register time spent
    clock_t endClock = clock ();

    mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
      "Pass 1",
      "Create the first MSR from the MSDL input",
      mfTimingItem::kMandatory,
      startClock,
      endClock);

    // sanity check
    if (! firstMsrScore) {
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

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass1 ()) {
    err <<
      endl <<
      "Quitting after creating the first MSR as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // convert the first MSR into a second MSR (pass 2)
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      translateMsrToMsr (
        firstMsrScore,
        gGlobalMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        "Pass 2",
        "Convert the first MSR into a second MSR");
  }
  catch (msr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass2 ()) {
    err <<
      endl <<
      "Quitting after creating the second MSR as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // the first BSR score
  // ------------------------------------------------------

  S_bsrScore firstBsrScore;

  {
    // convert the second MSR to the first BSR (pass 3)
    // ------------------------------------------------------

    try {
      firstBsrScore =
        translateMsrToBsr (
          secondMsrScore,
          gGlobalMsrOahGroup,
          gGlobalBsrOahGroup,
          "Pass 3",
          "Create a first BSR from the MSR");
    }
    catch (msr2bsrException& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsError::kErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsError::kErrorInvalidFile;
    }

    // display the first BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsrFirst ()) {
      displayBsrScore (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the first BSR");
    }

    if (gGlobalBsrOahGroup->getDisplayBsrFirstShort ()) {
      displayBsrScoreShort (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the first BSR");
    }
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass3 ()) {
    err <<
      endl <<
      "Quitting after creating the first BSR as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // the finalized BSR score
  // ------------------------------------------------------

  S_bsrScore finalizedBsrScore;

  {
    // create the finalized BSR from the first BSR (pass 4)
    // ------------------------------------------------------

    try {
      finalizedBsrScore =
        translateBsrToFinalizedBsr (
          firstBsrScore,
          gGlobalBsrOahGroup,
          "Pass 4",
          "Create the finalized BSR from the first BSR");
    }
    catch (bsr2finalizedBsrException& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsError::kErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsError::kErrorInvalidFile;
    }

    // display the finalized BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsrSecond ()) {
      displayBsrScore (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the finalized BSR");
    }

    if (gGlobalBsrOahGroup->getDisplayBsrSecondShort ()) {
      displayBsrScoreShort (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the finalized BSR");
    }
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass4 ()) {
    err <<
      endl <<
      "Quitting after creating the finalized BSR as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  {
    // convert the BSR to Braille text (pass 5)
    // ------------------------------------------------------

    string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "convertMsdlStream2brailleWithHandler() outputFileName = \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "convertMsdlStream2brailleWithHandler() output goes to standard output" <<
          endl;
      }
#endif

      // convert the BSR to braille
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          "Pass 5",
          "Convert the finalized BSR into braille",
          out);
      }
      catch (bsr2brailleException& e) {
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
          "convertMsdlStream2brailleWithHandler() output goes to file \"" <<
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
          "Opening file \"" << outputFileName << "\" for writing" <<
          endl;
      }
#endif

      ofstream
        brailleCodeFileOutputStream (
          outputFileName,
          ofstream::out);

      if (! brailleCodeFileOutputStream.is_open ()) {
        stringstream s;

        s <<
          "Could not open Braille output file \"" <<
          outputFileName <<
          "\" for writing, quitting";

        string message = s.str ();

        err <<
          message <<
          endl;

        throw bsr2brailleException (message);
      }

      // convert the finalized BSR to braille (pass 4)
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          "Pass 4",
          "Convert the finalized BSR into braille",
          brailleCodeFileOutputStream);
      }
      catch (bsr2brailleException& e) {
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

      brailleCodeFileOutputStream.close ();
    }
  }

	return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
mfMusicformatsError convertMsdlStream2brailleWithOptionsAndArguments (
  string                  inputSourceName,
  istream&                inputStream,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  // the service name
  // ------------------------------------------------------

  string serviceName = "msdl2braille";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

	// print the options and arguments
  // ------------------------------------------------------
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      handlerOptionsAndArguments;
  }
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

  // create an msdl2braille insider OAH handler
  // ------------------------------------------------------

  // create an insider msdl2braille OAH handler
  S_msdl2brailleInsiderHandler
    insiderOahHandler =
      msdl2brailleInsiderHandler::create (
        serviceName,
        serviceName + " insider OAH handler with options and arguments");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOption) {
    // use the insider msdl2braille OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular msdl2braille OAH handler
    handler =
      msdl2brailleRegularHandler::create (
        serviceName,
        serviceName + " regular OAH handler with options and arguments",
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
      gIndenter.getIndentation () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // do the job
  // ------------------------------------------------------

  convertMsdlStream2brailleWithHandler (
    inputSourceName,
    inputStream,
    handler,
    out,
    err);

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlFile2brailleWithOptionsAndArguments (
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

    throw bsr2brailleException (message);
  }

  return
    convertMsdlStream2brailleWithOptionsAndArguments (
      fileName,
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);
}

mfMusicformatsError convertMsdlFile2brailleWithHandler (
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

    throw msdl2brailleException (message);
  }

  return
    convertMsdlStream2brailleWithHandler (
      fileName, inputStream, handler, out, err);
}

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlString2brailleWithOptionsAndArguments (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call convertMsdlStream2brailleWithHandler() even if buffer is empty,
	// to handle the help options if any
  return
    convertMsdlStream2brailleWithOptionsAndArguments (
      "buffer",
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);
}

mfMusicformatsError convertMsdlString2brailleWithHandler (
  const char*   buffer,
  S_oahHandler  handler,
  std::ostream& out,
  std::ostream& err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call convertMsdlStream2brailleWithHandler() even if buffer is empty,
	// to handle the help options if any
  return
    convertMsdlStream2brailleWithHandler (
      "buffer", inputStream, handler, out, err);
}


}
