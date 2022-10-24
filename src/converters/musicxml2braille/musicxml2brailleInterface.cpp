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

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2bsrWae.h"
#include "bsr2bsrWae.h"
#include "bsr2brailleWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
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


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsError xmlFile2brailleWithHandler (
  SXMLFile&            sxmlfile,
  std::ostream&        out,
  std::ostream&        err,
  S_oahHandler         handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    gLogStream <<
      endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl <<
      "xmlFile2brailleWithHandler(), sxmlfile contains:" <<
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

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass2a ()) {
    err <<
      endl <<
      "Quitting after creating the MSR skeleton in pass 2a of xmlFile2brailleWithHandler as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // populate the MSR skeleton from MusicXML data (pass 2b)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxsr (
      theMxsr,
      firstMsrScore,
        "Pass 2b",
        "Populate the MSR skeleton from MusicXML data");
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

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass2b ()) {
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

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass3 ()) {
    err <<
      endl <<
      "Quitting after pass 3 as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
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
          "Pass 4a",
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

    if (gGlobalBsrOahGroup->getDisplayFirstBsrShort ()) {
      displayBsrScoreShort (
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
          "Pass 4b",
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

    if (gGlobalBsrOahGroup->getDisplaySecondBsrShort ()) {
      displayBsrScoreShort (
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

    string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2braille() outputFileName = \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2braille() output goes to standard output" <<
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
          "xmlFile2braille() output goes to file \"" <<
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
          outputFileName.c_str (),
          ofstream::out);

      if (! brailleCodeFileOutputStream.is_open ()) {
        stringstream s;

        s <<
          "Could not open Braille output file \"" <<
          outputFileName <<
          "\" for writing - quitting";

        string message = s.str ();

        err <<
          message <<
          endl;

        throw bsr2brailleException (message);
      }

      // convert the finalized BSR to braille
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
static mfMusicformatsError xmlFile2brailleWithOptionsAndArguments (
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
        return mfMusicformatsError::kErrorUnsupported;
    }
  }

  else {
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  err <<
    "xmlFile2braille(), sxmlfile is NULL" <<
    endl;
  }
#endif

    return mfMusicformatsError::kErrorInvalidFile;
  }

  // the service name
  // ------------------------------------------------------

  string serviceName = "xml2brl";

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

  // create an xml2brl insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2brl OAH handler
  S_xml2brlInsiderHandler
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

  xmlFile2brailleWithHandler (
    sxmlfile,
    out,
    err,
    handler);

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
EXP mfMusicformatsError musicxmlFile2braille (
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
        "Pass 1",
        "Create an MXSR reading a MusicXML file");

  if (sxmlfile) {
    return
      xmlFile2brailleWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMusicxmlFile2brailleWithHandler (
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
      xmlFile2brailleWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsError::kErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsError musicxmlFd2braille (
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
      xmlFile2brailleWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMusicxmlFd2brailleWithHandler (
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
      xmlFile2brailleWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsError::kErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsError musicxmlString2braille (
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

  // call xmlFile2braille() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2brailleWithOptionsAndArguments (
      sxmlfile,
      handlerOptionsAndArguments,
      out,
      err);

  return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMusicxmlString2brailleWithHandler (
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

  // call xmlFile2braille() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2brailleWithHandler (
      sxmlfile,
      out,
      err,
      handler);

  return mfMusicformatsError::kErrorInvalidFile;
}


}
