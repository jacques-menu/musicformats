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

#include "mfMusicformatsError.h" // for mfMusicformatsError

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfServiceRunData.h"
#include "mfExceptions.h"

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2mxsrWae.h"
#include "mxsr2musicxmlWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2musicxmlInsiderHandler.h"
#include "musicxml2musicxmlRegularHandler.h"

#include "musicxml2mxsrInterface.h"
#include "mxsr2msrSkeletonBuilderInterface.h"
#include "mxsr2msrTranslatorInterface.h"

#include "msr2msrInterface.h"
#include "msr2mxsrInterface.h"
#include "mxsr2musicxmlTranlatorInterface.h"

#include "mxsrGeneration.h"

#include "msrInterface.h"

#include "musicxml2musicxmlInterface.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsError xmlFile2musicxmlWithHandler (
  SXMLFile&     sxmlfile,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    gLogStream <<
      endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl <<
      "xmlFile2musicxmlWithHandler(), sxmlfile contains:" <<
      endl << endl;

    ++gIndenter;

    sxmlfile->print (gLogStream);
    gLogStream << endl << endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl << endl;
  }
#endif

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // get the original MXSR
  // ------------------------------------------------------

  Sxmlelement
    originalMxsr =
      sxmlfile->elements ();

  // the MSR score
  // ------------------------------------------------------

  S_msrScore firstMsrScore;

  // create the skeleton of the first MSR from the originalMxsr (pass 2a)
  // ------------------------------------------------------

  try {
    firstMsrScore =
      translateMxsrToMsrSkeleton (
        originalMxsr,
        gGlobalMsrOahGroup,
        "Pass 2a",
        "Create an MSR skeleton from the MXSR");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2xmlInsiderOahGroup->getQuitAfterPass2a ()) {
    err <<
      endl <<
      "Quitting after creating the MSR skeleton in pass 2a of xmlFile2musicxmlWithHandler as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // populate the MSR skeleton from MusicXML data (pass 2b)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxsr (
      originalMxsr,
      firstMsrScore,
      "Pass 2b",
      "Populate the MSR skeletonfrom MusicXML data");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2xmlInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      endl <<
      "Quitting after pass 2b as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // convert the first MSR score into a second MSR (pass 3)
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      translateMsrToMsr (
        firstMsrScore,
        gGlobalMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        "Pass 3",
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

  if (gGlobalXml2xmlInsiderOahGroup->getQuitAfterPass3 ()) {
    err <<
      endl <<
      "Quitting after pass 3 as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // convert the second MSR into an MXSR (pass 4)
  // ------------------------------------------------------

  Sxmlelement secondMxsr;

  try {
    secondMxsr =
      translateMsrToMxsr (
        secondMsrScore,
        gGlobalMsrOahGroup,
        "Pass 4",
        "Convert the second MSR into an MXSR",
        mfTimingItemKind::kMandatory);
  }
  catch (msr2mxsrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // convert the MXSR to MusicXML (pass 5)
  // ------------------------------------------------------

  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    translateMxsrToMusicXML (
      secondMxsr,
      outputFileName,
      err,
      "Pass 5",
      "Convert the MXSR into MusicXML text");
  }
  catch (mxsr2musicxmlException& e) {
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
static mfMusicformatsError xmlFile2musicxmlWithOptionsAndArguments (
  SXMLFile&               sxmlfile,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  Sxmlelement st;

  if (sxmlfile) {
    st = sxmlfile->elements ();

    if (st) {
      if (st->getName () == "score-timewise")
        return mfMusicformatsError::kErrorUnsupported;
    }
  }

  else {
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  err <<
    "xmlFile2musicxml(), sxmlfile is NULL" <<
    endl;
  }
#endif

    return mfMusicformatsError::kErrorInvalidFile;
  }

  // the service name
  // ------------------------------------------------------

  string serviceName = "xml2xml";

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

  // create an xml2xml insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2xml OAH handler
  S_xml2xmlInsiderHandler
    insiderOahHandler =
      xml2xmlInsiderHandler::create (
        serviceName,
        serviceName + " insider OAH handler with options and arguments");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOption) {
    // use the insider xml2xml OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular xml2xml OAH handler
    handler =
      xml2xmlRegularHandler::create (
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
      gIndenter.getIndentation () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // do the job
  // ------------------------------------------------------

  xmlFile2musicxmlWithHandler (
    sxmlfile,
    out,
    err,
    handler);

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
EXP mfMusicformatsError musicxmlFile2musicxml (
  const char*             fileName,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFile (
        fileName,
        "Pass 1",
        "Create an MXSR reading a MusicXML file");

  if (sxmlfile) {
    return
      xmlFile2musicxmlWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMusicxmlFile2musicxmlWithHandler (
  const char*  fileName,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFile (
        fileName,
        "Pass 1",
        "Create an MXSR reading a MusicXML file");

  if (sxmlfile) {
    return
      xmlFile2musicxmlWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsError::kErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsError musicxmlFd2musicxml (
  FILE*                   fd,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFd (
        fd,
        "Pass 1",
        "Create an MXSR reading a MusicXML descriptor");

  if (sxmlfile) {
    return
      xmlFile2musicxmlWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMusicxmlFd2musicxmlWithHandler (
  FILE*        fd,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFd (
        fd,
        "Pass 1",
        "Create an MXSR reading a MusicXML descriptor");

  if (sxmlfile) {
    return
      xmlFile2musicxmlWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsError::kErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsError musicxmlString2musicxml (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromString (
        buffer,
        "Pass 1",
        "Create an MXSR reading a MusicXML buffer");

  // call xmlFile2musicxml() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2musicxmlWithOptionsAndArguments (
      sxmlfile,
      handlerOptionsAndArguments,
      out,
      err);

  return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMusicxmlString2musicxmlWithHandler (
  const char*  buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromString (
        buffer,
        "Pass 1",
        "Create an MXSR reading a MusicXML buffer");

  // call xmlFile2musicxml() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2musicxmlWithHandler (
      sxmlfile,
      out,
      err,
      handler);

  return mfMusicformatsError::kErrorInvalidFile;
}


}
