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

#include "oahWae.h"
#include "msdlWae.h"
#include "msr2msrWae.h"
#include "msr2mxsrWae.h"
#include "mxsr2musicxmlWae.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "oahEarlyOptions.h"

#include "msdlParser.h"

#include "msdl2musicxmlInsiderHandler.h"
#include "msdl2musicxmlRegularHandler.h"

#include "msr2msrInterface.h"
#include "msr2mxsrInterface.h"
#include "mxsr2musicxmlTranlatorInterface.h"

#include "mxsrGeneration.h"

#include "msdl2musicxmlInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gOahOahGroup->fTrace has been initialized
*/

//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlStream2musicxmlWithHandler (
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

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
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
  catch (msdl2musicxmlException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (false && gGlobalMsdl2xmlInsiderOahGroup->getQuitAfterPass1 ()) { // JMI
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTrace (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_1));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // convert the MSR into a second MSR (pass 2)
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

  // convert the second MSR into an MXSR (pass 3)
  // ------------------------------------------------------

  Sxmlelement secondMxsr;

  try {
    secondMxsr =
      translateMsrToMxsr (
        secondMsrScore,
        gMsrOahGroup,
        mfPassIDKind::kMfPassID_3,
        "Convert the second MSR into an MXSR",
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

  // convert the MXSR to MusicXML (pass 4)
  // ------------------------------------------------------

  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    translateMxsrToMusicXML (
      secondMxsr,
      outputFileName,
      err,
      mfPassIDKind::kMfPassID_4,
      "Convert the MXSR into MusicXML text");
  }
  catch (mxsr2musicxmlException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlStream2musicxmlWithOptionsAndArguments (
  std::string             inputSourceName,
  std::istream&           inputStream,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  // the service name
  // ------------------------------------------------------

  std::string serviceName = "msdl2musicxml";

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
    gLog <<
      handlerOptionsAndArguments;
#endif // MF_TRACE_IS_ENABLED
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
      ", insiderOption: " << insiderOption <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create an msdl2musicxml insider OAH handler
  // ------------------------------------------------------

  // create an insider msdl2musicxml OAH handler
  const S_msdl2musicxmlInsiderHandler&
    insiderOahHandler =
      msdl2musicxmlInsiderHandler::create (
        serviceName,
        serviceName + " insider OAH handler with options and arguments");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOption) {
    // use the insider msdl2musicxml OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular msdl2musicxml OAH handler
    handler =
      msdl2musicxmlRegularHandler::create (
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

  convertMsdlStream2musicxmlWithHandler (
    inputSourceName,
    inputStream,
    handler,
    out,
    err);

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind convertMsdlFile2musicxmlWithOptionsAndArguments (
  std::string             inputFileName,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  // open input file
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    gWaeHandler->waeTrace (
      err,
      __FILE__, __LINE__,
      gLanguage->openingMusicXMLFileForWriting (inputFileName));
  }
#endif // MF_TRACE_IS_ENABLED

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

    throw msdl2musicxmlException (message);
  }

  return
    convertMsdlStream2musicxmlWithOptionsAndArguments (
      inputFileName,
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);
}

EXP mfMusicformatsErrorKind convertMsdlFile2musicxmlWithHandler (
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
      gLanguage->openingMusicXMLFileForWriting (inputFileName));
  }
#endif // MF_TRACE_IS_ENABLED

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

    throw msdl2musicxmlException (message);
  }

  return
    convertMsdlStream2musicxmlWithHandler (
      inputFileName, inputStream, handler, out, err);
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msdlString2musicxmlWithOptionsAndArguments (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  std::stringstream
    inputStream (
      buffer,
      std::ios_base::in);

  // call convertMsdlStream2musicxmlWithOptionsAndArguments() even if buffer is empty,
  // to handle the help options if any
  return
    convertMsdlStream2musicxmlWithOptionsAndArguments (
      "buffer",
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

EXP mfMusicformatsErrorKind convertMsdlString2musicxmlWithHandler (
  const char*         buffer,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
  std::stringstream
    inputStream (
      buffer,
      std::ios_base::in);

  // call convertMsdlStream2musicxmlWithOptionsAndArguments() even if buffer is empty,
  // to handle the help options if any
  return
    convertMsdlStream2musicxmlWithHandler (
      "buffer", inputStream, handler, out, err);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}


}
