/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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
#include "msrPathToVoice.h"
#include "msrScores.h"

#include "lpsrScores.h"

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2lpsrWae.h"
#include "lpsr2lilypondWae.h"

#include "mfPreprocessorSettings.h"

#include "mfServices.h"
#include "mfStringsHandling.h"

#include "musicxml2lilypondComponent.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2lilypondInsiderHandler.h"
#include "musicxml2lilypondRegularHandler.h"

#include "musicxml2mxsrInterface.h"
#include "mxsr2msrSkeletonBuilderInterface.h"
#include "mxsr2msrSkeletonPopulatorInterface.h"
#include "msr2mxsrInterface.h"
#include "msr2msrInterface.h"
#include "msr2lpsrInterface.h"
#include "lpsrInterface.h"
#include "lpsr2lilypondInterface.h"

#include "msrInterface.h"
#include "lpsrInterface.h"

#include "musicxml2lilypondInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsErrorKind sxmlFile2lilypondWithHandler (
  SXMLFile&           sxmlfile,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getDisplayMxsr ()) {
    gLog <<
      std::endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl <<
      "sxmlFile2lilypondWithHandler(), sxmlfile contains:" <<
      std::endl << std::endl;

    ++gIndenter;

    sxmlfile->print (gLog);
    gLog << std::endl << std::endl;

    --gIndenter;

    gLog <<
      "<!-- ----------------------------------------------------------- -->" <<
      std::endl << std::endl;
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

  // create the skeleton of the first MSR from the originalMxsr (pass 2)
  // ------------------------------------------------------

  mxsrEventsCollection eventsCollection;

  try {
    firstMsrScore =
      translateMxsrToMsrSkeleton (
        theMxsr,
        eventsCollection,
        gMsrOahGroup,
        mfPassIDKind::kMfPassID_2,
        gLanguage->convertTheMXSRIntoAnMSRSkeleton ());
  } // try

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

  if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass2 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, mfInputLineNumber (__LINE__),
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_2));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // populate the MSR skeleton from the MXSR (pass 3)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxsr (
      theMxsr,
      firstMsrScore,
      eventsCollection,
      mfPassIDKind::kMfPassID_3,
      gLanguage->populateTheMSRSkeletonFromMusicXMLData ());
  } // try

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

  if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass3 ()) {
#ifdef MF_TRACE_IS_ENABLED
    gWaeHandler->waeTraceToStreamWithoutInputLocation (
      err,
      __FILE__, mfInputLineNumber (__LINE__),
      gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_3));
#endif // MF_TRACE_IS_ENABLED

    return mfMusicformatsErrorKind::kMusicformatsError_NONE;
  }

  // convert the first MSR score into a second MSR if needed (pass 4)
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  if (gGlobalMsr2msrOahGroup->getAvoidMsr2msr ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTracePasses ()) {
      std::stringstream ss;

      ss <<
        gLanguage->passIDKindAsString (mfPassIDKind::kMfPassID_4) <<
        ": " <<
        gLanguage->convertTheFirstMSRIntoASecondMSR () <<
        " not run, since this pass is being avoided";

      msr2msrWarning (
        gServiceRunData->getInputSourceName (),
        1, // JMI inputLineNumber,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    secondMsrScore = firstMsrScore;
  }

  else {
    try {
//       for (S_msrVoice voice: firstMsrScore->getScoreAllVoicesList ()) {
//         gLog <<
//           "===> firstMsrScore voice: " << voice->getVoiceName () <<
//           std::endl;
//
//         S_msrPathToVoice
//           pathToVoice =
//             msrPathToVoice::createFromVoice (
//               voice);
//
//         secondMsrScore =
//           translateMsrToMsrAlongPathToVoice ( JMI ??? 0.9.70
//             firstMsrScore,
//             gMsrOahGroup,
//             gGlobalMsr2msrOahGroup,
//             mfPassIDKind::kMfPassID_4,
//             gLanguage->convertTheFirstMSRIntoASecondMSR (),
//             pathToVoice);
//       } // for

      secondMsrScore =
        translateMsrToMsr (
          firstMsrScore,
          gMsrOahGroup,
          gGlobalMsr2msrOahGroup,
          mfPassIDKind::kMfPassID_4,
          gLanguage->convertTheFirstMSRIntoASecondMSR ());
    } // try


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

    if (! gGlobalMsr2msrOahGroup->getAvoidMsr2msr ()) {
      if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass4 ()) { // JMI 0.9.70 meaningless if msr2smr is avoided
    #ifdef MF_TRACE_IS_ENABLED
        gWaeHandler->waeTraceToStreamWithoutInputLocation (
          err,
          __FILE__, mfInputLineNumber (__LINE__),
          gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_4));
    #endif // MF_TRACE_IS_ENABLED

        return mfMusicformatsErrorKind::kMusicformatsError_NONE;
      }
    }
  }

  // the LPSR score
  // ------------------------------------------------------

  S_lpsrScore theLpsrScore;

  // create the LPSR from the MSR (pass 5)
  // ------------------------------------------------------

  try {
    if (gGlobalMsr2msrOahGroup->getAvoidMsr2msr ()) {
      theLpsrScore =
        translateMsrToLpsr (
          secondMsrScore,
          gMsrOahGroup,
          gLpsrOahGroup,
          mfPassIDKind::kMfPassID_5,
          gLanguage->convertTheFirstMSRIntoAnLPSR (),
          createMusicxml2lilypondConverterComponent ());
    }
    else {
      theLpsrScore =
        translateMsrToLpsr (
          secondMsrScore,
          gMsrOahGroup,
          gLpsrOahGroup,
          mfPassIDKind::kMfPassID_5,
          gLanguage->convertTheSecondMSRIntoAnLPSR (),
          createMusicxml2lilypondConverterComponent ());
    }
  } // try

  catch (msr2lpsrException& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // convert the LPSR to LilyPond code (pass 6)
  // ------------------------------------------------------

  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    err <<
      "xmlFile2lilypond() outputFileName: \"" <<
      outputFileName <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! outputFileName.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      err <<
        "xmlFile2lilypond() output goes to standard output" <<
        std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

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
        gMsrOahGroup,
        gLpsrOahGroup,
        mfPassIDKind::kMfPassID_6,
        gLanguage->convertTheLPSRIntoLilyPondCode (),
        lilypondStandardOutputStream);
    } // try

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
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      err <<
        "xmlFile2lilypond() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // open output file
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTracePasses ()) {
      err <<
        std::endl <<
        gLanguage->openingLilypondFileForWriting (outputFileName) <<
        std::endl;

//     gWaeHandler->waeTraceWithoutInputLocation ( JMI 0.9.67
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

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
        gMsrOahGroup,
        gLpsrOahGroup,
        mfPassIDKind::kMfPassID_6,
        gLanguage->convertTheLPSRIntoLilyPondCode (),
        lilypondFileOutputStream);
    } // try

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
        gLanguage->closingLilypondFile (outputFileName);

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    outputFileStream.close ();
  }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
static mfMusicformatsErrorKind xmlFile2lilypondWithOptionsAndArguments (
  SXMLFile&               sxmlfile,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
  Sxmlelement st;

  if (sxmlfile) {
    st = sxmlfile->elements();

    if (st) {
      if (st->getName() == "score-timewise")
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

  std::string serviceName = "xml2ly";

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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create an xml2ly insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2ly OAH handler
  const S_xml2lyInsiderHandler&
    insiderOahHandler =
      xml2lyInsiderHandler::create (
        serviceName,
        "xml2lyInsiderHandler",
        oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments);

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOption) {
    // use the insider xml2ly OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular xml2ly OAH handler
    handler =
      xml2lyRegularHandler::create (
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

  sxmlFile2lilypondWithHandler (
    sxmlfile,
    out,
    err,
    handler);

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlFile2lilypond (
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
      xmlFile2lilypondWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlFile2lilypondWithHandler (
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
      sxmlFile2lilypondWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlFd2lilypond (
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
      xmlFile2lilypondWithOptionsAndArguments (
        sxmlfile,
        handlerOptionsAndArguments,
        out,
        err);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlFd2lilypondWithHandler (
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
      sxmlFile2lilypondWithHandler (
        sxmlfile,
        out,
        err,
        handler);
  }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind musicxmlString2lilypond (
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

  // call xmlFile2lilypond() even if sxmlfile is null,
  // to handle the help options if any
  return
    xmlFile2lilypondWithOptionsAndArguments (
      sxmlfile,
      handlerOptionsAndArguments,
      out,
      err);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMusicxmlString2lilypondWithHandler (
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

  // call xmlFile2lilypond() even if sxmlfile is null,
  // to handle the help options if any
  return
    sxmlFile2lilypondWithHandler (
      sxmlfile,
      out,
      err,
      handler);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}


}
