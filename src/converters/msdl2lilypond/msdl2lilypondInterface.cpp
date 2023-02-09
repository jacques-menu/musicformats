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

#include "mfStaticSettings.h"

#include "mfPasses.h"
#include "mfServices.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "oahWae.h"
#include "msdlWae.h"
#include "msr2msrWae.h"
#include "msr2lpsrWae.h"
#include "lpsr2lilypondWae.h"

#include "mfStaticSettings.h"

#include "lpsrScores.h"

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

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
mfMusicformatsErrorKind convertMsdlStream2lilypondWithHandler (
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

  // the MSR score
  // ------------------------------------------------------

  S_msrScore theMsrScore;

  // translating the MSDL input into an MSR (pass 1)
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

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
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

    gGlobalTimingItemsList.appendTimingItem (
      mfPassIDKind::kMfPassID_1,
      "Create the first MSR from the MSDL input",
      mfTimingItemKind::kMandatory,
      startClock,
      endClock);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
    // sanity check
    if (! theMsrScore) {
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
#endif
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

  if (gGlobalMsdl2lyInsiderOahGroup->getQuitAfterPass1 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTrace (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_1));
#endif

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // the LPSR score
  // ------------------------------------------------------
  S_lpsrScore theLpsrScore;

  // create the LPSR from the MSR (pass 2)
  // ------------------------------------------------------

  try {
    theLpsrScore =
      translateMsrToLpsr (
        theMsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        mfPassIDKind::kMfPassID_2,
        "Convert the MSR into an LPSR",
        createMsdl2lilypondConverterComponent ());
  }
  catch (msr2lpsrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2lyInsiderOahGroup->getQuitAfterPass2b ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTrace (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_2b));
#endif

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // convert the LPSR to LilyPond code (pass 3)
  // ------------------------------------------------------

  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    err <<
      "msdlStream2lilypond() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif

  if (! outputFileName.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceOah ()) {
      err <<
        "msdlStream2lilypond() output goes to standard output" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif

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
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        mfPassIDKind::kMfPassID_3,
        gLanguage->convertTheLPSRIntoLilyPondCode (),
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
    if (gEarlyOptions.getEarlyTraceOah ()) {
      err <<
        "msdlStream2lilypond() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif

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
#endif

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
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        mfPassIDKind::kMfPassID_4,
        gLanguage->convertTheLPSRIntoLilyPondCode (),
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

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

    outputFileStream.close ();
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
 mfMusicformatsErrorKind convertMsdlStream2lilypondWithOptionsAndArguments (
  std::string             inputSourceName,
  std::istream&           inputStream,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  // the service name
  // ------------------------------------------------------

  std::string serviceName = "msdl2lilypond";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

  // print the options and arguments
  // ------------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  if (gEarlyOptions.getEarlyTraceOah ()) {
		std::stringstream ss;

    ss <<
      handlerOptionsAndArguments;
  }
#endif
#endif

  // apply early options if any
  // ------------------------------------------------------

  gEarlyOptions.applyEarlyOptionsIfPresentInOptionsAndArguments (
    handlerOptionsAndArguments);

  // has the '-insider' option been used?
  // ------------------------------------------------------

  Bool insiderOption =
    gEarlyOptions.getEarlyInsiderOption ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
		std::stringstream ss;

    ss <<
      serviceName << " main()" <<
      ", insiderOption: " << insiderOption <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
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

  convertMsdlStream2lilypondWithHandler (
    inputSourceName,
    inputStream,
    handler,
    out,
    err);

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

// //_______________________________________________________________________________
// EXP mfMusicformatsErrorKind convertMsdlFile2lilypondWithOptionsAndArguments (
//   std::string             inputFileName,
//   oahOptionsAndArguments& handlerOptionsAndArguments,
//   std::ostream&           out,
//   std::ostream&           err)
// {
//   // open input file
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getEarlyTracePasses ()) {
//     gWaeHandler->waeTrace (
//       err,
//       __FILE__, __LINE__,
//       gLanguage->openingLilypondFileForWriting (inputFileName));
//   }
// #endif
//
//   std::ifstream
//     inputStream (
//       inputFileName.c_str (),
//       std::ifstream::in);
//
//   if (! inputStream.is_open ()) {
//     std::stringstream ss;
//
//     ss <<
//       gLanguage->cannotOpenMSDLFileForReading (inputFileName);
//
//     std::string message = ss.str ();
//
//     err <<
//       message <<
//       std::endl;
//
//     throw msdl2lilyponException (message);
//   }
//
//   return
//     convertMsdlStream2lilypondWithOptionsAndArguments (
//       inputFileName,
//       inputStream,
//       handlerOptionsAndArguments,
//       out,
//       err);
// }

mfMusicformatsErrorKind convertMsdlFile2lilypondWithHandler (
  std::string         inputFileName,
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err)
{
  // open input file
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    gWaeHandler->waeTrace (
      err,
      __FILE__, __LINE__,
      gLanguage->openingLilypondFileForWriting (inputFileName));
  }
#endif

  std::ifstream
    inputStream (
      inputFileName.c_str (),
      std::ifstream::in);

  if (! inputStream.is_open ()) {
    std::stringstream ss;

    ss <<
      gLanguage->cannotOpenMSDLFileForReading (inputFileName);

    std::string message = ss.str ();

    err <<
      message <<
      std::endl;

    throw msdl2lilyponException (message);
  }

  return
    convertMsdlStream2lilypondWithHandler (
      inputFileName, inputStream, handler, out, err);
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlString2lilypondWithOptionsAndArguments (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  std::stringstream
    inputStream (
      buffer,
      std::ios_base::in);

  // call convertMsdlStream2lilypondWithOptionsAndArguments() even if buffer is empty,
  // to handle the help options if any
  return
    convertMsdlStream2lilypondWithOptionsAndArguments (
      "buffer",
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMsdlString2lilypondWithHandler (
  const char*         buffer,
  const S_oahHandler& handler,
  std::ostream&       out,
  std::ostream&       err)
{
  std::stringstream
    inputStream (
      buffer,
      std::ios_base::in);

  // call convertMsdlStream2lilypondWithOptionsAndArguments() even if buffer is empty,
  // to handle the help options if any
  return
    convertMsdlStream2lilypondWithHandler (
      "buffer",
      inputStream,
      handler,
      out,
      err);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}


}
