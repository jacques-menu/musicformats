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
                        // std::ifstream, std::ifstream::open(), std::ifstream::close()

#include "mfPreprocessorSettings.h"

#include "mfBool.h"
#include "mfPasses.h"
#include "mfServices.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "oahWae.h"
#include "msdlWae.h"
#include "msr2msrWae.h"
#include "msr2bsrWae.h"
#include "bsr2bsrWae.h"
#include "bsr2brailleWae.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"

#include "oahEarlyOptions.h"

#include "waeHandlers.h"

#include "msdlParser.h"

#include "msdl2brailleInsiderHandler.h"
#include "msdl2brailleRegularHandler.h"

#include "msr2msrInterface.h"
#include "msr2bsrInterface.h"
#include "bsr2bsrFinalizerInterface.h"
#include "bsr2brailleTranslatorInterface.h"

#include "bsrScores.h"
#include "bsrInterface.h"

#include "msdl2brailleInterface.h"


namespace MusicFormats
{

//_______________________________________________________________________________
mfMusicformatsErrorKind convertMsdlStream2brailleWithHandler (
  std::string         inputSourceName,
  std::istream&       inputStream,
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err)
{
  // register the input source name
  gServiceRunData->setInputSourceName (
    inputSourceName);

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gEarlyOptions.getEarlyQuietOption ()) {
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

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTracePasses ()) {
      std::string separator =
        "%--------------------------------------------------------------";
      err <<
        std::endl <<
        separator <<
        std::endl <<
        gTab <<
        gLanguage->passIDKindAsString (mfPassIDKind::kMfPassID_1) <<
        ": " <<
        "Creating a first MSR from the MSDL input" <<
        std::endl <<
        separator <<
        std::endl;

//       gWaeHandler->waeTraceWithoutInputLocation ( // JMI v0.9.67
//         __FILE__, __LINE__,
//         ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // create the MSDL parser
    msdlParser parser (inputStream);

    // parse the input
    parser.parse ();

    // get the resulting score
    // JMI an msrBook should also be handled
    firstMsrScore = parser.getCurrentScore ();

    // register time spent
    clock_t endClock = clock ();

    gGlobalTimingItemsList.appendTimingItem (
      mfPassIDKind::kMfPassID_1,
      "Create the first MSR from the MSDL input",
      mfTimingItemKind::kMandatory,
      startClock,
      endClock);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
    if (! firstMsrScore) {
      std::stringstream ss;

      ss <<
        "Could not perform comversion of \"" <<
        inputSourceName <<
        "\" to MSR - quitting";

      std::string message = ss.str ();

      err <<
        message <<
        std::endl;

      throw msdl2msrException (message);
    }
#endif // MF_SANITY_CHECKS_ARE_ENABLED
  }
  catch (msdl2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass1 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_1));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // convert the first MSR into a second MSR (pass 2)
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      translateMsrToMsr (
        firstMsrScore,
        gMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        mfPassIDKind::kMfPassID_2,
        gLanguage->convertTheFirstMSRIntoASecondMSR ());
  }
  catch (msr2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass2 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_2));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
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
          gMsrOahGroup,
          gBsrOahGroup,
          mfPassIDKind::kMfPassID_3,
          "Create a first BSR from the MSR");
    }
    catch (msr2bsrException& e) {
      mfDisplayException (e, gOutput);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutput);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }

    // display the first BSR score if requested
    // ------------------------------------------------------

    if (gBsrOahGroup->getDisplayFirstBsr ()) {
      displayBsrScore (
        firstBsrScore,
        gMsrOahGroup,
        gBsrOahGroup,
        gLanguage->displayTheFirstMSRAsText ());
    }

    if (gBsrOahGroup->getDisplayFirstBsrFull ()) {
      displayBsrScoreFull (
        firstBsrScore,
        gMsrOahGroup,
        gBsrOahGroup,
        gLanguage->displayTheFirstMSRAsText ());
    }
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass3 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_3));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
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
          gBsrOahGroup,
          mfPassIDKind::kMfPassID_4,
          "Create the finalized BSR from the first BSR");
    }
    catch (bsr2finalizedBsrException& e) {
      mfDisplayException (e, gOutput);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutput);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }

    // display the finalized BSR score if requested
    // ------------------------------------------------------

    if (gBsrOahGroup->getDisplaySecondBsr ()) {
      displayBsrScore (
        finalizedBsrScore,
        gMsrOahGroup,
        gBsrOahGroup,
        gLanguage->displayTheFinalizedBSRAsText ());
    }

    if (gBsrOahGroup->getDisplaySecondBsrFull ()) {
      displayBsrScoreFull (
        finalizedBsrScore,
        gMsrOahGroup,
        gBsrOahGroup,
        gLanguage->displayTheFinalizedBSRAsText ());
    }
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass4 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_4));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  {
    // convert the BSR to Braille text (pass 5)
    // ------------------------------------------------------

    std::string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        err <<
          "convertMsdlStream2brailleWithHandler() outputFileName: \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

//         gWaeHandler->waeTraceWithoutInputLocation ( // JMI v0.9.67
//           __FILE__, __LINE__,
//           ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

    if (! outputFileName.size ()) {
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        err <<
          "convertMsdlStream2brailleWithHandler() output goes to standard output" <<
          std::endl;

//         gWaeHandler->waeTraceWithoutInputLocation ( // JMI v0.9.67
//           __FILE__, __LINE__,
//           ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // convert the BSR to braille
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gBsrOahGroup,
          mfPassIDKind::kMfPassID_5,
          "Convert the finalized BSR into braille",
          out);
      }
      catch (bsr2brailleException& e) {
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
          "convertMsdlStream2brailleWithHandler() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

//         gWaeHandler->waeTraceWithoutInputLocation ( // JMI v0.9.67
//           __FILE__, __LINE__,
//           ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // open output file
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getEarlyTracePasses ()) {
        gWaeHandler->waeTraceToStreamWithoutInputLocation (
          err,
          __FILE__, __LINE__,
          gLanguage->openingBrailleMusicFileForWriting (outputFileName));
      }
#endif // MF_TRACE_IS_ENABLED

      std::ofstream
        brailleCodeFileOutputStream (
          outputFileName,
          std::ofstream::out);

      if (! brailleCodeFileOutputStream.is_open ()) {
        std::stringstream ss;

        ss <<
          gLanguage->cannotOpenBrailleMusicFileForWriting (outputFileName);

        std::string message = ss.str ();

        err <<
          message <<
          std::endl;

        throw bsr2brailleException (message);
      }

      // convert the finalized BSR to braille (pass 4)
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gBsrOahGroup,
          mfPassIDKind::kMfPassID_4,
          "Convert the finalized BSR into braille",
          brailleCodeFileOutputStream);
      }
      catch (bsr2brailleException& e) {
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
          gLanguage->closingMusicXMLFile (outputFileName) <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      brailleCodeFileOutputStream.close ();
    }
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
mfMusicformatsErrorKind convertMsdlStream2brailleWithOptionsAndArguments (
  std::string             inputSourceName,
  std::istream&           inputStream,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  // the service name
  // ------------------------------------------------------

  std::string serviceName = "msdl2braille";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

  // print the options and arguments
  // ------------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      handlerOptionsAndArguments;
  }
#endif // MF_TRACE_IS_ENABLED

  // apply early options if any
  // ------------------------------------------------------

  gEarlyOptions.applyEarlyOptionsIfPresentInOptionsAndArguments (
    handlerOptionsAndArguments);

  // has the '-insider' option been used?
  // ------------------------------------------------------

  Bool insiderOption =
    gEarlyOptions.getEarlyInsiderOption ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      serviceName << " main()" <<
      ", insiderOption: " << insiderOption;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create an msdl2braille insider OAH handler
  // ------------------------------------------------------

  // create an insider msdl2braille OAH handler
  const S_msdl2brailleInsiderHandler&
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

  setGlobalServiceRunData (
    mfServiceRunData::create (
      serviceName));

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
        return mfMusicformatsErrorKind::kMusicformatsError_NONE; // quit now
        break;
      case oahElementHelpOnlyKind::kElementHelpOnlyNo:
        // go ahead
        break;
    } // switch
  }
  catch (mfOahException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // check indentation
  if (gIndenter != 0) {
    gLog <<
      "### " <<
      serviceName <<
      " gIndenter value after options ands arguments checking: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

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

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlFile2brailleWithOptionsAndArguments (
  std::string             inputFileName, // JMI v0.9.66 ??? output ???
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  // open input file
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, __LINE__,
      gLanguage->openingBrailleMusicFileForWriting (inputFileName));
  }
#endif // MF_TRACE_IS_ENABLED

  std::ifstream
    inputStream (
      inputFileName,
      std::ifstream::in);

  if (! inputStream.is_open ()) {
    std::stringstream ss;

    ss <<
      gLanguage->cannotOpenMSDLFileForReading (inputFileName);

    std::string message = ss.str ();

    err <<
      message <<
      std::endl;

    throw bsr2brailleException (message);
  }

  return
    convertMsdlStream2brailleWithOptionsAndArguments (
      inputFileName,
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);
}

mfMusicformatsErrorKind convertMsdlFile2brailleWithHandler (
  std::string         inputFileName, // JMI v0.9.66 ??? output ???
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err)
{
  // open input file
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, __LINE__,
      gLanguage->openingBrailleMusicFileForWriting (inputFileName));
  }
#endif // MF_TRACE_IS_ENABLED

  std::ifstream
    inputStream (
      inputFileName.c_str (),
      std::ifstream::in);

  if (! inputStream.is_open ()) {
    std::stringstream ss;

    ss <<
      gLanguage->cannotOpenBrailleMusicFileForWriting (inputFileName);

    std::string message = ss.str ();

    err <<
      message <<
      std::endl;

    throw msdl2brailleException (message);
  }

  return
    convertMsdlStream2brailleWithHandler (
      inputFileName, inputStream, handler, out, err);
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlString2brailleWithOptionsAndArguments (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  std::stringstream
    inputStream (
      buffer,
      std::ios_base::in);

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

mfMusicformatsErrorKind convertMsdlString2brailleWithHandler (
  const char*   buffer,
  const S_oahHandler&  handler,
  std::ostream& out,
  std::ostream& err)
{
  std::stringstream
    inputStream (
      buffer,
      std::ios_base::in);

  // call convertMsdlStream2brailleWithHandler() even if buffer is empty,
  // to handle the help options if any
  return
    convertMsdlStream2brailleWithHandler (
      "buffer", inputStream, handler, out, err);
}


}
