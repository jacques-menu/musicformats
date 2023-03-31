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

#include "mfPasses.h"
#include "msrScores.h"

#include "bsrScores.h"

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2bsrWae.h"
#include "bsr2bsrWae.h"
#include "bsr2brailleWae.h"

#include "mfStaticSettings.h"

#include "mfServices.h"
#include "mfStringsHandling.h"

#include "oahOah.h"
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

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsErrorKind xmlFile2brailleWithHandler (
  SXMLFile&     sxmlfile,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "xmlFile2brailleWithHandler(), sxmlfile contains:" <<
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

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass2a ()) {
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
      theMxsr,
      firstMsrScore,
        mfPassIDKind::kMfPassID_2b,
        gLanguage->populateTheMSRSqueletonFromMusicXMLData ());
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

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass2b ()) {
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

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass3 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStream (
      err,
      __FILE__, __LINE__,
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_3));
#endif // MF_TRACE_IS_ENABLED

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
          gMsrOahGroup,
          gBsrOahGroup,
          mfPassIDKind::kMfPassID_4a,
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

  // the finalized BSR score
  S_bsrScore finalizedBsrScore;

  {
    // create the finalized BSR from the first BSR (pass 4b)
    // ------------------------------------------------------

    try {
      finalizedBsrScore =
        translateBsrToFinalizedBsr (
          firstBsrScore,
          gBsrOahGroup,
          mfPassIDKind::kMfPassID_4b,
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
          "xmlFile2braille() outputFileName: \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

//     gWaeHandler->waeTraceWithoutLocationDetails ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

    if (! outputFileName.size ()) {
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        err <<
          "xmlFile2braille() output goes to standard output" <<
          std::endl;

//     gWaeHandler->waeTraceWithoutLocationDetails ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
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
          "xmlFile2braille() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

//     gWaeHandler->waeTraceWithoutLocationDetails ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // open output file
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getEarlyTracePasses ()) {
        err <<
          std::endl <<
          gLanguage->openingBrailleMusicFileForWriting (outputFileName) <<
          std::endl;

//     gWaeHandler->waeTraceWithoutLocationDetails ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      std::ofstream
        brailleCodeFileOutputStream (
          outputFileName.c_str (),
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

      // convert the finalized BSR to braille
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
          gLanguage->closingBrailleMusicFile (outputFileName) <<
          std::endl;

        gWaeHandler->waeTraceWithoutLocationDetails (
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
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
  err <<
    "xmlFile2braille(), sxmlfile is NULL" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

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
      ", insiderOption: " << insiderOption <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLStream ());

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
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLStream ());

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
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLDescriptor ());

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
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLDescriptor ());

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
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLBuffer ());

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
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLBuffer ());

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
