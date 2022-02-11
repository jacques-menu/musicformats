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

#include "oahWae.h"
#include "msdlWae.h"
#include "msr2msrWae.h"
#include "msr2mxsrWae.h"
#include "mxsr2guidoWae.h"
#include "guidoWae.h" // JMI SAXOSAXO ???

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "oahEarlyOptions.h"

#include "msdlParser.h"

#include "msdl2guidoInsiderHandler.h"
#include "msdl2guidoRegularHandler.h"

#include "msr2msrInterface.h"
#include "msr2mxsrInterface.h"
#include "mxsr2guidoTranlatorInterface.h"

#include "msdl2guidoInterface.h"


using namespace std;

namespace MusicFormats
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/

//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
mfMusicformatsError convertMsdlStream2guidoWithHandler (
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

  if (gGlobalWaeOahGroup->getQuiet ()) {
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
    msdlParser
      parser (inputStream);

    // parse the input
    parser.parse ();

    // get the resulting score
    // JMI an msrBook should also be handled
    firstMsrScore = parser.getCurrentScore ();

    // register time spent
    clock_t endClock = clock ();

    mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
      "Pass 1",
      "Create the MSR score from the MSDL input",
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

      throw msdlException (message);
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

  if (gGlobalMsdl2gmnInsiderOahGroup->getQuitAfterPass1 ()) {
    err <<
      endl <<
      "Quitting after creating the first MSR in pass 1 of convertMsdlStream2guidoWithHandler as requested" <<
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

  // convert the second MSR into an MXSR (pass 3)
  // ------------------------------------------------------

  Sxmlelement theMxsr;

  try {
    theMxsr =
      translateMsrToMxsr (
        secondMsrScore,
        gGlobalMsrOahGroup,
        "Pass 3",
        "Convert the second MSR into an MXSR",
        mfTimingItem::kMandatory);
  }
  catch (msr2mxsrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // convert the MXSR to Guido (pass 4)
  // ------------------------------------------------------

  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    translateMxsrToGuido (
      theMxsr,
      outputFileName,
      err,
      "Pass 4",
      "Convert  the MXSR into Guido text");
  }
  catch (mxsr2guidoException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
mfMusicformatsError convertMsdlStream2guidoWithOptionsAndArguments (
  string                  inputSourceName,
  istream&                inputStream,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  // the service name
  // ------------------------------------------------------

  string serviceName = "msdl2guido";

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
    gLogStream <<
      handlerOptionsAndArguments;
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

  // create an msdl2guido insider OAH handler
  // ------------------------------------------------------

  // create an insider msdl2guido OAH handler
  S_msdl2guidoInsiderHandler
    insiderOahHandler =
      msdl2guidoInsiderHandler::create (
        serviceName,
        serviceName + " insider OAH handler with options and arguments");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOption) {
    // use the insider msdl2guido OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular msdl2guido OAH handler
    handler =
      msdl2guidoRegularHandler::create (
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

  convertMsdlStream2guidoWithHandler (
    inputSourceName,
    inputStream,
    handler,
    out,
    err);

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlFile2guidoWithOptionsAndArguments (
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
      "Opening MSDL file \"" << fileName << "\" for reading" <<
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

    throw msdl2guidoException (message);
  }

  return
    convertMsdlStream2guidoWithOptionsAndArguments (
      fileName,
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);
}

mfMusicformatsError msdlFile2guidoWithHandler (
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

    throw msdl2guidoException (message);
  }

  return
    convertMsdlStream2guidoWithHandler (
      fileName, inputStream, handler, out, err);
}

//_______________________________________________________________________________
EXP mfMusicformatsError convertMsdlString2guidoWithOptionsAndArguments (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call convertMsdlStream2guidoWithOptionsAndArguments() even if buffer is empty,
	// to handle the help options if any
  return
    convertMsdlStream2guidoWithOptionsAndArguments (
      "buffer",
      inputStream,
      handlerOptionsAndArguments,
      out,
      err);

	return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError msdlString2guidoWithHandler (
  const char*   buffer,
  S_oahHandler  handler,
  std::ostream& out,
  std::ostream& err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call convertMsdlStream2guidoWithOptionsAndArguments() even if buffer is empty,
	// to handle the help options if any
  return
    convertMsdlStream2guidoWithHandler (
      "buffer",
      inputStream,
      handler,
      out,
      err);

	return mfMusicformatsError::kErrorInvalidFile;
}


}
