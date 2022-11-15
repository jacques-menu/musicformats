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

#include "msrScores.h"

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2mxsrWae.h"
#include "mxsr2guidoWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2guidoInsiderHandler.h"
#include "musicxml2guidoRegularHandler.h"

#include "musicxml2mxsrInterface.h"
#include "mxsr2msrSkeletonBuilderInterface.h"
#include "mxsr2msrTranslatorInterface.h"

#include "msr2msrInterface.h"
#include "msr2mxsrInterface.h"
#include "mxsr2guidoTranlatorInterface.h"

#include "mxsrGeneration.h"

#include "msrInterface.h"

#include "musicxml2guidoInterface.h"


namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsErrorKind xmlFile2guidoWithHandler (
  SXMLFile&     sxmlfile,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    gLogStream <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "xmlFile2guidoWithHandler(), sxmlfile contains:" <<
      std::endl << std::endl;

    ++gIndenter;

    sxmlfile->print (gLogStream);
    gLogStream << std::endl << std::endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl << std::endl;
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
        "Create the skeleton of the first MSR from the originalMxsr");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass2a ()) {
    err <<
      std::endl <<
      "Quitting after creating the MSR skeleton in pass 2a of xmlFile2guidoWithHandler as requested" <<
      std::endl;

    return mfMusicformatsErrorKind::kMusicformatsError_NO_;
  }


  // populate the MSR skeleton from MusicXML data (pass 2b)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxsr (
      originalMxsr,
      firstMsrScore,
        "Pass 2b",
        "Populate the MSR skeleton from MusicXML data");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      std::endl <<
      "Quitting after pass 2b as requested" <<
      std::endl;

    return mfMusicformatsErrorKind::kMusicformatsError_NO_;
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
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass3 ()) {
    err <<
      std::endl <<
      "Quitting after pass 3 as requested" <<
      std::endl;

    return mfMusicformatsErrorKind::kMusicformatsError_NO_;
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
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // convert the MXSR to Guido (pass 5)
  // ------------------------------------------------------

  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    translateMxsrToGuido (
      secondMxsr,
      outputFileName,
      err,
      "Pass 5",
      "Convert the MXSR into Guido text");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NO_;
}

//_______________________________________________________________________________
static mfMusicformatsErrorKind xmlFile2guidoWithOptionsAndArguments (
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
        return mfMusicformatsErrorKind::kMusicformatsErrorUnsupported;
    }
  }

  else {
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  err <<
    "xmlFile2guido(), sxmlfile is NULL" <<
    std::endl;
  }
#endif

    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // the service name
  // ------------------------------------------------------

  std::string serviceName = "xml2gmn";

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
      std::endl;
  }
#endif

  // create an xml2gmn insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2gmn OAH handler
  S_xml2gmnInsiderHandler
    insiderOahHandler =
      xml2gmnInsiderHandler::create (
        serviceName,
        serviceName + " insider OAH handler with options and arguments");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOption) {
    // use the insider xml2gmn OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular xml2gmn OAH handler
    handler =
      xml2gmnRegularHandler::create (
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
        return mfMusicformatsErrorKind::kMusicformatsError_NO_; // quit now
        break;
      case oahElementHelpOnlyKind::kElementHelpOnlyNo:
        // go ahead
        break;
    } // switch
  }
  catch (mfOahException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
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

  xmlFile2guidoWithHandler (
    sxmlfile,
    out,
    err,
    handler);

  return mfMusicformatsErrorKind::kMusicformatsError_NO_;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlFile2guido (
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
      xmlFile2guidoWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlFile2guidoWithHandler (
  const char*  fileName,
  std::ostream&     out,
  std::ostream&     err,
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
      xmlFile2guidoWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlFd2guido (
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
      xmlFile2guidoWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlFd2guidoWithHandler (
  FILE*        fd,
  std::ostream&     out,
  std::ostream&     err,
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
      xmlFile2guidoWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlString2guido (
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

  // call xmlFile2guido() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2guidoWithOptionsAndArguments (
      sxmlfile,
      handlerOptionsAndArguments,
      out,
      err);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlString2guidoWithHandler (
  const char*  buffer,
  std::ostream&     out,
  std::ostream&     err,
  S_oahHandler handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromString (
        buffer,
        "Pass 1",
        "Create an MXSR reading a MusicXML buffer");

  // call xmlFile2guido() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2guidoWithHandler (
      sxmlfile,
      out,
      err,
      handler);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}


}
