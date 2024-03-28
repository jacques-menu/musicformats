/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>      // std::ofstream, std::ofstream::open(), std::ofstream::close()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfPasses.h"
#include "msrScores.h"

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2mxsrWae.h"
#include "mxsr2guidoWae.h"

#include "mfPreprocessorSettings.h"

#include "mfServices.h"
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

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsErrorKind xmlFile2guidoWithHandler (
  SXMLFile&           sxmlfile,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getDisplayMxsr ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "xmlFile2guidoWithHandler(), sxmlfile contains:" <<
      std::endl << std::endl;

    ++gIndenter;

    sxmlfile->print (gLog);
    gLog << std::endl << std::endl;

    --gIndenter;

    gLog <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl << std::endl;

    gWaeHandler->waeTraceWithoutInputLocation (
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

  // create the skeleton of the first MSR from the originalMxsr (pass 2)
  // ------------------------------------------------------

  try {
    firstMsrScore =
      translateMxsrToMsrSkeleton (
        originalMxsr,
        gMsrOahGroup,
        mfPassIDKind::kMfPassID_2,
        "Create the skeleton of the first MSR from the originalMxsr");
  } // try

  catch ( mxsr2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch ( std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass2 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_2));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }


  // populate the MSR skeleton from MusicXML data (pass 3)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxsr (
      originalMxsr,
      firstMsrScore,
        mfPassIDKind::kMfPassID_3,
        gLanguage->populateTheMSRSkeletonFromMusicXMLData ());
  } // try

  catch ( mxsr2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch ( std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass3 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_3));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // convert the first MSR score into a second MSR (pass 4)
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      translateMsrToMsr (
        firstMsrScore,
        gMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        mfPassIDKind::kMfPassID_4,
        gLanguage->convertTheFirstMSRIntoASecondMSR ());
  } // try

  catch ( mxsr2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch ( std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass4 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_3));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // convert the second MSR into an MXSR (pass 5)
  // ------------------------------------------------------

  Sxmlelement secondMxsr;

  try {
    secondMxsr =
      translateMsrToMxsr (
        secondMsrScore,
        gMsrOahGroup,
        mfPassIDKind::kMfPassID_5,
        "Convert the second MSR into an MXSR",
        mfTimingItemKind::kMandatory);
  } // try

  catch ( mxsr2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch ( std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // convert the MXSR to Guido (pass 6)
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
      mfPassIDKind::kMfPassID_6,
      "Convert the MXSR into Guido text");
  } // try

  catch ( mxsr2msrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch ( std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
  err <<
    "xmlFile2guido(), sxmlfile is NULL" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

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

  // create an xml2gmn insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2gmn OAH handler
  const S_xml2gmnInsiderHandler&
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
  } // try

  catch ( mfOahException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption;
  }
  catch ( std::exception& e) {
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

  xmlFile2guidoWithHandler (
    sxmlfile,
    out,
    err,
    handler);

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
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
        mfPassIDKind::kMfPassID_1,
        gLanguage->convertAMusicXMLStreamIntoAnMXSR ());

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
        gLanguage->convertAMusicXMLStreamIntoAnMXSR ());

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
        mfPassIDKind::kMfPassID_1,
        gLanguage->convertAMusicXMLDescriptorIntoAnMXSR ());

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
        gLanguage->convertAMusicXMLDescriptorIntoAnMXSR ());

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
        mfPassIDKind::kMfPassID_1,
        gLanguage->convertAMusicXMLBufferIntoAnMXSR ());

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
        gLanguage->convertAMusicXMLBufferIntoAnMXSR ());

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
