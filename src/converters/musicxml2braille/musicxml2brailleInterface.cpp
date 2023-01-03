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

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "msrScores.h"
#include "bsrScores.h"

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2bsrWae.h"
#include "bsr2bsrWae.h"
#include "bsr2brailleWae.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "oahOah.h"
#include "oahEarlyOptions.h"
#include "waeOah.h"

#include "oahEarlyOptions.h"

#include "msrOah.h"
#include "msr2bsrOah.h"
#include "msr2msrOah.h"
#include "bsrOah.h"

#include "musicxml2brailleInsiderHandler.h"
#include "musicxml2brailleRegularHandler.h"

#include "musicxml2mxsrInterface.h"
#include "mxsr2msrSkeletonBuilderInterface.h"
#include "mxsr2msrTranslatorInterface.h"
#include "msr2mxsrInterface.h"
#include "msr2msrInterface.h"
#include "msr2bsrInterface.h"
#include "bsr2bsrFinalizerInterface.h"
#include "bsr2brailleTranslatorInterface.h"

#include "msrInterface.h"

#include "bsrInterface.h"

#include "musicxml2brailleInterface.h"


namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsErrorKind xmlFile2brailleWithHandler (
  SXMLFile&     sxmlfile,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    gLogStream <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "xmlFile2brailleWithHandler(), sxmlfile contains:" <<
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

  // get the MXSR
  // ------------------------------------------------------

  Sxmlelement
    theMxsr =
      sxmlfile->elements ();

  // the MSR score
  // ------------------------------------------------------

  S_msrScore firstMsrScore;

  // create the MSR skeleton from the MXSR (pass 2a)
  // ------------------------------------------------------

  try {
    firstMsrScore =
      translateMxsrToMsrSkeleton (
        theMxsr,
        gGlobalMsrOahGroup,
        gGlobalOahEarlyOptions.getMfWaeHandler ()->pass2a (),
        gGlobalOahEarlyOptions.getMfWaeHandler ()->createAnMSRSqueletonFromTheMXSR ());
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

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass2a ()) {
    err <<
      std::endl <<
      "Quitting after creating the MSR skeleton in pass 2a of xmlFile2brailleWithHandler as requested" <<
      std::endl;

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // populate the MSR skeleton from MusicXML data (pass 2b)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxsr (
      theMxsr,
      firstMsrScore,
        gGlobalOahEarlyOptions.getMfWaeHandler ()->pass2b (),
        gGlobalOahEarlyOptions.getMfWaeHandler ()->populateTheMSRSqueletonFromMusicXMLData ());
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

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      std::endl <<
      "Quitting after pass 2b as requested" <<
      std::endl;

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
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
        gGlobalOahEarlyOptions.getMfWaeHandler ()->pass3 (),
        gGlobalOahEarlyOptions.getMfWaeHandler ()->convertTheFirstMSRIntoASecondMSR ());
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

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass3 ()) {
    err <<
      std::endl <<
      "Quitting after pass 3 as requested" <<
      std::endl;

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // the first BSR score
  // ------------------------------------------------------

  S_bsrScore firstBsrScore;

  {
    // create the first BSR from the MSR (pass 4a)
    // ------------------------------------------------------

    try {
      firstBsrScore =
        translateMsrToBsr (
          secondMsrScore,
          gGlobalMsrOahGroup,
          gGlobalBsrOahGroup,
          gGlobalOahEarlyOptions.getMfWaeHandler ()->pass4a (),
          "Create a first BSR from the MSR");
    }
    catch (msr2bsrException& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }

    // display the first BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayFirstBsr ()) {
      displayBsrScore (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the first BSR");
    }

    if (gGlobalBsrOahGroup->getDisplayFirstBsrFull ()) {
      displayBsrScoreFull (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the first BSR");
    }
  }

  // the finalized BSR score
  S_bsrScore finalizedBsrScore;

  {
    // create the finalized BSR from the first BSR (pass 4b)
    // ------------------------------------------------------

    try {
      finalizedBsrScore =
        translateBsrToFinalizedBsr (
          firstBsrScore,
          gGlobalBsrOahGroup,
          gGlobalOahEarlyOptions.getMfWaeHandler ()->pass4b (),
          "Create the finalized BSR from the first BSR");
    }
    catch (bsr2finalizedBsrException& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
    }

    // display the finalized BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplaySecondBsr ()) {
      displayBsrScore (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the finalized BSR");
    }

    if (gGlobalBsrOahGroup->getDisplaySecondBsrFull ()) {
      displayBsrScoreFull (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the finalized BSR");
    }
  }

  {
    // convert the BSR to Braille text (pass 5)
    // ------------------------------------------------------

    std::string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2braille() outputFileName = \"" <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif

    if (! outputFileName.size ()) {
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2braille() output goes to standard output" <<
          std::endl;
      }
#endif

      // convert the BSR to braille
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          gGlobalOahEarlyOptions.getMfWaeHandler ()->pass5 (),
          "Convert the finalized BSR into braille",
          out);
      }
      catch (bsr2brailleException& e) {
      mfDisplayException (e, gOutputStream);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }
      catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }
    }

    else {
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2braille() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif

      // open output file
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
        err <<
          std::endl <<
          "Opening file \"" << outputFileName << "\" for writing" <<
          std::endl;
      }
#endif

      std::ofstream
        brailleCodeFileOutputStream (
          outputFileName.c_str (),
          std::ofstream::out);

      if (! brailleCodeFileOutputStream.is_open ()) {
        std::stringstream s;

        s <<
          "Could not open Braille output file \"" <<
          outputFileName <<
          "\" for writing - quitting";

        std::string message = s.str ();

        err <<
          message <<
          std::endl;

        throw bsr2brailleException (message);
      }

      // convert the finalized BSR to braille
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          gGlobalOahEarlyOptions.getMfWaeHandler ()->pass4 (),
          "Convert the finalized BSR into braille",
          brailleCodeFileOutputStream);
      }
      catch (bsr2brailleException& e) {
        mfDisplayException (e, gOutputStream);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }
      catch (std::exception& e) {
        mfDisplayException (e, gOutputStream);
        return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }

      // close output file
#ifdef TRACE_OAH
      if (gtracingOah->fTracePasses) {
        gLogStream <<
          std::endl <<
          "Closing file \"" << outputFileName << "\"" <<
          std::endl;
      }
#endif

      brailleCodeFileOutputStream.close ();
    }
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
static mfMusicformatsErrorKind xmlFile2brailleWithOptionsAndArguments (
  SXMLFile&               sxmlfile,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  Sxmlelement theMxsr;

  if (sxmlfile) {
    theMxsr = sxmlfile->elements ();

    if (theMxsr) {
      if (theMxsr->getName () == "score-timewise")
        return mfMusicformatsErrorKind::kMusicformatsErrorUnsupported;
    }
  }

  else {
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  err <<
    "xmlFile2braille(), sxmlfile is NULL" <<
    std::endl;
  }
#endif

    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // the service name
  // ------------------------------------------------------

  std::string serviceName = "xml2brl";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

  // print the options and arguments
  // ------------------------------------------------------
#ifdef OAH_TRACING_IS_ENABLED
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

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      serviceName << " main()" <<
      ", insiderOption: " << insiderOption <<
      std::endl;
  }
#endif

  // create an xml2brl insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2brl OAH handler
  const S_xml2brlInsiderHandler&
    insiderOahHandler =
      xml2brlInsiderHandler::create (
        serviceName,
        serviceName + " insider OAH handler with options and arguments");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOption) {
    // use the insider xml2brl OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular xml2brl OAH handler
    handler =
      xml2brlRegularHandler::create (
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
        return mfMusicformatsErrorKind::kMusicformatsError_NONE; // quit now
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

  xmlFile2brailleWithHandler (
    sxmlfile,
    out,
    err,
    handler);

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlFile2braille (
  const char*             fileName,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  //
  SXMLFile
    sxmlfile =
      createSXMLFileFromFile (
        fileName,
        gGlobalOahEarlyOptions.getMfWaeHandler ()->pass1 (),
        gGlobalOahEarlyOptions.getMfWaeHandler ()->createAnMXSRFromAMusicXMLFile ());

  if (sxmlfile) {
    return
      xmlFile2brailleWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlFile2brailleWithHandler (
  const char*         fileName,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFile (
        fileName,
        gGlobalOahEarlyOptions.getMfWaeHandler ()->pass1 (),
        gGlobalOahEarlyOptions.getMfWaeHandler ()->createAnMXSRFromAMusicXMLFile ());

  if (sxmlfile) {
    return
      xmlFile2brailleWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlFd2braille (
  FILE*                   fd,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFd (
        fd,
        gGlobalOahEarlyOptions.getMfWaeHandler ()->pass1 (),
        gGlobalOahEarlyOptions.getMfWaeHandler ()->createAnMXSRFromAMusicXMLDescriptor ());

  if (sxmlfile) {
    return
      xmlFile2brailleWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlFd2brailleWithHandler (
  FILE*               fd,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromFd (
        fd,
        gGlobalOahEarlyOptions.getMfWaeHandler ()->pass1 (),
        gGlobalOahEarlyOptions.getMfWaeHandler ()->createAnMXSRFromAMusicXMLDescriptor ());

  if (sxmlfile) {
    return
      xmlFile2brailleWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlString2braille (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromString (
        buffer,
        gGlobalOahEarlyOptions.getMfWaeHandler ()->pass1 (),
        gGlobalOahEarlyOptions.getMfWaeHandler ()->createAnMXSRFromAMusicXMLBuffer ());

  // call xmlFile2braille() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2brailleWithOptionsAndArguments (
      sxmlfile,
      handlerOptionsAndArguments,
      out,
      err);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlString2brailleWithHandler (
  const char*         buffer,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
  SXMLFile
    sxmlfile =
      createSXMLFileFromString (
        buffer,
        gGlobalOahEarlyOptions.getMfWaeHandler ()->pass1 (),
        gGlobalOahEarlyOptions.getMfWaeHandler ()->createAnMXSRFromAMusicXMLBuffer ());

  // call xmlFile2braille() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2brailleWithHandler (
      sxmlfile,
      out,
      err,
      handler);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}


}
