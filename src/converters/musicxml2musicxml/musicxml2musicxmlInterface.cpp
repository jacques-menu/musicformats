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

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind
#include "mfPasses.h"

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfServices.h"
#include "mfExceptions.h"

#include "msrScores.h"

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2mxsrWae.h"
#include "mxsr2musicxmlWae.h"

#include "mfStaticSettings.h"

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

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsErrorKind xmlFile2musicxmlWithHandler (
  SXMLFile&           sxmlfile,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "xmlFile2musicxmlWithHandler(), sxmlfile contains:" <<
      std::endl << std::endl;

    ++gIndenter;

    sxmlfile->print (gLog);
    gLog << std::endl << std::endl;

    --gIndenter;

    gLog <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl << std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
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
        gMsrOahGroup,
        mfPassIDKind::kMfPassID_2a,
        gLanguage->createAnMSRSqueletonFromTheMXSR ());
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2xmlInsiderOahGroup->getQuitAfterPass2a ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStream (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_2a));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // populate the MSR skeleton from MusicXML data (pass 2b)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxsr (
      originalMxsr,
      firstMsrScore,
      mfPassIDKind::kMfPassID_2b,
      "Populate the MSR skeletonfrom MusicXML data");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2xmlInsiderOahGroup->getQuitAfterPass2b ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStream (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_2b));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // convert the first MSR score into a second MSR (pass 3)
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      translateMsrToMsr (
        firstMsrScore,
        gMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        mfPassIDKind::kMfPassID_3,
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

  if (gGlobalXml2xmlInsiderOahGroup->getQuitAfterPass3 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStream (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_3));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // convert the second MSR into an MXSR (pass 4)
  // ------------------------------------------------------

  Sxmlelement secondMxsr;

  try {
    secondMxsr =
      translateMsrToMxsr (
        secondMsrScore,
        gMsrOahGroup,
        mfPassIDKind::kMfPassID_4,
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

  // convert the MXSR to MusicXML (pass 5)
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
      mfPassIDKind::kMfPassID_5,
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
static mfMusicformatsErrorKind xmlFile2musicxmlWithOptionsAndArguments (
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
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
  err <<
    "xmlFile2musicxml(), sxmlfile is NULL" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // the service name
  // ------------------------------------------------------

  std::string serviceName = "xml2xml";

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

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create an xml2xml insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2xml OAH handler
  const S_xml2xmlInsiderHandler&
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

  xmlFile2musicxmlWithHandler (
    sxmlfile,
    out,
    err,
    handler);

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlFile2musicxml (
  const char*             fileName,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFile (
        fileName,
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLStream ());

  if (sxmlfile) {
    return
      xmlFile2musicxmlWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlFile2musicxmlWithHandler (
  const char*         fileName,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFile (
        fileName,
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLStream ());

  if (sxmlfile) {
    return
      xmlFile2musicxmlWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlFd2musicxml (
  FILE*                   fd,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFd (
        fd,
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLDescriptor ());

  if (sxmlfile) {
    return
      xmlFile2musicxmlWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlFd2musicxmlWithHandler (
  FILE*               fd,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFd (
        fd,
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLDescriptor ());

  if (sxmlfile) {
    return
      xmlFile2musicxmlWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlString2musicxml (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromString (
        buffer,
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLBuffer ());

  // call xmlFile2musicxml() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2musicxmlWithOptionsAndArguments (
      sxmlfile,
      handlerOptionsAndArguments,
      out,
      err);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlString2musicxmlWithHandler (
  const char*         buffer,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromString (
        buffer,
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLBuffer ());

  // call xmlFile2musicxml() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2musicxmlWithHandler (
      sxmlfile,
      out,
      err,
      handler);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}


}
