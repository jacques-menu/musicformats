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

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2lpsrWae.h"
#include "lpsr2lilypondWae.h"

#include "mfEnableTracingSetting.h"

#include "mfPasses.h"
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
#include "mxsr2msrTranslatorInterface.h"
#include "msr2mxsrInterface.h"
#include "msr2msrInterface.h"
#include "msr2lpsrInterface.h"
#include "lpsr2lilypondInterface.h"

#include "msrInterface.h"
#include "lpsrInterface.h"

#include "msdlConverterInterface.h"

namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsErrorKind convertMsdlFile2lilypondWithHandler ( // JMI UNUSED v0.9.66
  SXMLFile&           sxmlfile,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
//     gLogStream <<
//       std::endl <<
//       "<!-- ----------------------------------------------------------- -->" <<
//       std::endl <<
//       "sxmlFile2lilypondWithHandler(), sxmlfile contains:" <<
//       std::endl << std::endl;
//
//     ++gIndenter;
//
//     sxmlfile->print (gLogStream);
//     gLogStream << std::endl << std::endl;
//
//     --gIndenter;
//
//     gLogStream <<
//       "<!-- ----------------------------------------------------------- -->" <<
//       std::endl << std::endl;
//   }
// #endif
//
//   // has quiet mode been requested?
//   // ------------------------------------------------------
//
//   if (gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
//     // disable all trace and display options
//     handler->
//       enforceHandlerQuietness ();
//   }
//
//   // get the MXSR
//   // ------------------------------------------------------
//
//   Sxmlelement
//     theMxsr =
//       sxmlfile->elements ();
//
//   // the MSR score
//   // ------------------------------------------------------
//
//   S_msrScore firstMsrScore;
//
//   // create the skeleton of the first MSR from the originalMxsr (pass 2a)
//   // ------------------------------------------------------
//
//   try {
//     firstMsrScore =
//       translateMxsrToMsrSkeleton (
//         theMxsr,
//         gGlobalMsrOahGroup,
//         gWaeHandler->pass (mfPassIDKind::kMfPassID_2a),
//         gWaeHandler->createAnMSRSqueletonFromTheMXSR ());
//   }
//   catch (mxsr2msrException& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // should we return now?
//   // ------------------------------------------------------
//
//   if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass2a ()) {
//     err <<
//       std::endl <<
//       "Quitting after creating the MSR skeleton in pass 2a of sxmlFile2lilypondWithHandler as requested" <<
//       std::endl;
//
//     return mfMusicformatsErrorKind::kMusicformatsError_NONE;
//   }
//
//   // populate the MSR skeleton from MusicXML data (pass 2b)
//   // ------------------------------------------------------
//
//   try {
//     populateMsrSkeletonFromMxsr (
//       theMxsr,
//       firstMsrScore,
//         gWaeHandler->pass (mfPassIDKind::kMfPassID_2b),
//         gWaeHandler->populateTheMSRSqueletonFromMusicXMLData ());
//   }
//   catch (mxsr2msrException& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // should we return now?
//   // ------------------------------------------------------
//
//   if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass2b ()) {
//     err <<
//       std::endl <<
//       "Quitting after pass 2b as requested" <<
//       std::endl;
//
//     return mfMusicformatsErrorKind::kMusicformatsError_NONE;
//   }
//
//   // convert the first MSR score into a second MSR (pass 3)
//   // ------------------------------------------------------
//
//   S_msrScore secondMsrScore;
//
//   try {
// if (false) { // JMI
//     for (const S_msrVoice& voice: firstMsrScore->getScoreAllVoicesList ()) {
//       gLogStream <<
//         "===> firstMsrScore voice: " << voice->getVoiceName () <<
//         std::endl;
//
//       S_msrPathToVoice
//         pathToVoice =
//           msrPathToVoice::createFromVoice (
//             voice);
//
//       secondMsrScore =
//         translateMsrToMsrAlongPathToVoice (
//           firstMsrScore,
//           gGlobalMsrOahGroup,
//           gGlobalMsr2msrOahGroup,
//           gWaeHandler->pass (mfPassIDKind::kMfPassID_3),
//           gWaeHandler->convertTheFirstMSRIntoASecondMSR (),
//           pathToVoice);
//     } // for
// }
// else {
//     secondMsrScore =
//       translateMsrToMsr (
//         firstMsrScore,
//         gGlobalMsrOahGroup,
//         gGlobalMsr2msrOahGroup,
//         gWaeHandler->pass (mfPassIDKind::kMfPassID_3),
//         gWaeHandler->convertTheFirstMSRIntoASecondMSR ());
// }
//   }
//   catch (mxsr2msrException& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // should we return now?
//   // ------------------------------------------------------
//
//   if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass3 ()) {
//     err <<
//       std::endl <<
//       "Quitting after pass 3 as requested" <<
//       std::endl;
//
//     return mfMusicformatsErrorKind::kMusicformatsError_NONE;
//   }
//
//   // the LPSR score
//   // ------------------------------------------------------
//
//   S_lpsrScore theLpsrScore;
//
//   {
//     // create the LPSR from the MSR (pass 4)
//     // ------------------------------------------------------
//
//     try {
//       theLpsrScore =
//         translateMsrToLpsr (
//           secondMsrScore,
//           gGlobalMsrOahGroup,
//           gGlobalLpsrOahGroup,
//           gWaeHandler->pass (mfPassIDKind::kMfPassID_4),
//           gWaeHandler->convertTheSecondMSRIntoAnLPSR (),
//           createMsdl2lilypondConverterComponent ());
//     }
//     catch (msr2lpsrException& e) {
//       mfDisplayException (e, gOutputStream);
//       return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//     }
//     catch (std::exception& e) {
//       mfDisplayException (e, gOutputStream);
//       return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//     }
//
//     // display the LPSR score if requested
//     // ------------------------------------------------------
//
//     if (gGlobalLpsrOahGroup->getDisplayLpsr ()) {
//       displayLpsrScore (
//         theLpsrScore,
//         gGlobalMsrOahGroup,
//         gGlobalLpsrOahGroup,
//       	 gWaeHandler->displayTheLPSRAsText ());
//     }
//
//     if (gGlobalLpsrOahGroup->getDisplayLpsrFull ()) {
//       displayLpsrScoreFull (
//         theLpsrScore,
//         gGlobalMsrOahGroup,
//         gGlobalLpsrOahGroup,
//         gWaeHandler->displayTheLPSRAsText ()
//       	   +
//         ", " <<
//       	   +
//        gWaeHandler->fullVersion ());
//     }
//   }
//
//   {
//     // convert the LPSR to LilyPond code (pass 5)
//     // ------------------------------------------------------
//
//     std::string
//       outputFileName =
//         handler->
//           fetchOutputFileNameFromTheOptions ();
//
// #ifdef MF_TRACING_IS_ENABLED
//     if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//       err <<
//         "xmlFile2lilypond() outputFileName = \"" <<
//         outputFileName <<
//         "\"" <<
//         std::endl;
//     }
// #endif
//
//     if (! outputFileName.size ()) {
// #ifdef MF_TRACING_IS_ENABLED
//       if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//         err <<
//           "xmlFile2lilypond() output goes to standard output" <<
//           std::endl;
//       }
// #endif
//
//       // create an indented output stream for the LilyPond code
//       // to be written to outputFileStream
//       mfIndentedOstream
//         lilypondStandardOutputStream (
//           out,
//           gIndenter);
//
//       // convert the LPSR into LilyPond code
//       try {
//         translateLpsrToLilypond (
//           theLpsrScore,
//           gGlobalMsrOahGroup,
//           gGlobalLpsrOahGroup,
//           gWaeHandler->pass (mfPassIDKind::kMfPassID_5),
//           gWaeHandler->convertTheLPSRIntoLilyPondCode (),
//           lilypondStandardOutputStream);
//       }
//       catch (lpsr2lilypondException& e) {
//         mfDisplayException (e, gOutputStream);
//         return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//       }
//       catch (std::exception& e) {
//         mfDisplayException (e, gOutputStream);
//         return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//       }
//     }
//
//     else {
// #ifdef MF_TRACING_IS_ENABLED
//       if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//         err <<
//           "xmlFile2lilypond() output goes to file \"" <<
//           outputFileName <<
//           "\"" <<
//           std::endl;
//       }
// #endif
//
//       // open output file
// #ifdef MF_TRACING_IS_ENABLED
//       if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
//         err <<
//           std::endl <<
// 		       gWaeHandler->openingLilypondFileForWriting (outputFileName) <<
//           std::endl;
//       }
// #endif
//
//       std::ofstream
//         outputFileStream (
//           outputFileName.c_str (),
//           std::ofstream::out);
//
//       if (! outputFileStream.is_open ()) {
//         std::stringstream s;
//
//         s <<
//           gWaeHandler->cannotOpenLilypondFileForWriting (outputFileName);
//
//         std::string message = s.str ();
//
//         err <<
//           message <<
//           std::endl;
//
//         throw lpsr2lilypondException (message);
//       }
//
//       // create an indented output stream for the LilyPond code
//       // to be written to outputFileStream
//       mfIndentedOstream
//         lilypondFileOutputStream (
//           outputFileStream,
//           gIndenter);
//
//       // convert the LPSR into LilyPond code
//       try {
//         translateLpsrToLilypond (
//           theLpsrScore,
//           gGlobalMsrOahGroup,
//           gGlobalLpsrOahGroup,
//           gWaeHandler->pass (mfPassIDKind::kMfPassID_5),
//           gWaeHandler->convertTheLPSRIntoLilyPondCode (),
//           lilypondFileOutputStream);
//       }
//       catch (lpsr2lilypondException& e) {
//         mfDisplayException (e, gOutputStream);
//         return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//       }
//       catch (std::exception& e) {
//         mfDisplayException (e, gOutputStream);
//         return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//       }
//
//       // close output file
// #ifdef TRACE_OAH
//       if (gtracingOah->fTracePasses) {
//         gLogStream <<
//           std::endl <<
//           gWaeHandler->closingLilypondFile (outputFileName) <<
//           std::endl;
//       }
// #endif
//
//       outputFileStream.close ();
//     }
//   }

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
static mfMusicformatsErrorKind msdlFile2lilypondWithOptionsAndArguments ( // JMI UNUSED v0.9.66
  SXMLFile&               sxmlfile,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
//   Sxmlelement st;
//
//   if (sxmlfile) {
//     st = sxmlfile->elements();
//
//     if (st) {
//       if (st->getName() == "score-timewise")
//         return mfMusicformatsErrorKind::kMusicformatsErrorUnsupported;
//     }
//   }
//
//   else {
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//   err <<
//     "xmlFile2musicxml(), sxmlfile is NULL" <<
//     std::endl;
//   }
// #endif
//
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // the service name
//   // ------------------------------------------------------
//
//   std::string serviceName = "xml2ly";
//
//   // reset the global indenter
//   // ------------------------------------------------------
//
//   gIndenter.resetToZero ();
//
//   // create the global log indented output stream
//   // ------------------------------------------------------
//
//   createTheGlobalIndentedOstreams (out, err);
//
//   // print the options and arguments
//   // ------------------------------------------------------
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//     gLogStream <<
//       handlerOptionsAndArguments;
//   }
// #endif
//
//   // apply early options if any
//   // ------------------------------------------------------
//
//   gGlobalOahEarlyOptions.applyEarlyOptionsIfPresentInArgcArgv (
//     argc,
//     argv);
//
//   // has the '-insider' option been used?
//   // ------------------------------------------------------
//
//   Bool insiderOption =
//     gGlobalOahEarlyOptions.getEarlyInsiderOption ();
//
// #ifdef MF_TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//     gLogStream <<
//       serviceName << " main()" <<
//       ", insiderOption: " << insiderOption <<
//       std::endl;
//   }
// #endif
//
//   // create an xml2ly insider OAH handler
//   // ------------------------------------------------------
//
//   // create an insider xml2ly OAH handler
//   const S_xml2lyInsiderHandler&
//     insiderOahHandler =
//       xml2lyInsiderHandler::create (
//         serviceName,
//         "xml2lyInsiderHandler",
//         oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments);
//
//   // the OAH handler to be used, a regular handler is the default
//   // ------------------------------------------------------
//
//   S_oahHandler handler;
//
//   if (insiderOption) {
//     // use the insider xml2ly OAH handler
//     handler = insiderOahHandler;
//   }
//   else {
//     // create a regular xml2ly OAH handler
//     handler =
//       xml2lyRegularHandler::create (
//         serviceName,
//         serviceName + " regular OAH handler",
//         insiderOahHandler);
//   }
//
//   // create the global run data
//   // ------------------------------------------------------
//
//   gGlobalServiceRunData =
//     mfServiceRunData::create (
//       serviceName);
//
//   // handle the command line options and arguments
//   // ------------------------------------------------------
//
//   try {
//     // handle the options from the options and arguments
//     oahElementHelpOnlyKind
//       helpOnlyKind =
//         handler->
//           handleOptionsFromOptionsAndArguments (
//             serviceName);
//
//     // have help options been used?
//     switch (helpOnlyKind) {
//       case oahElementHelpOnlyKind::kElementHelpOnlyYes:
//         return mfMusicformatsErrorKind::kMusicformatsError_NONE; // quit now
//         break;
//       case oahElementHelpOnlyKind::kElementHelpOnlyNo:
//         // go ahead
//         break;
//     } // switch
//   }
//   catch (mfOahException& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // check indentation
//   if (gIndenter != 0) {
//     gLogStream <<
//       "### " <<
//       serviceName <<
//       " gIndenter value after options ands arguments checking: " <<
//       gIndenter.getIndentation () <<
//       " ###" <<
//       std::endl;
//
//     gIndenter.resetToZero ();
//   }
//
//   // do the job
//   // ------------------------------------------------------
//
//   sxmlFile2lilypondWithHandler (
//     sxmlfile,
//     out,
//     err,
//     handler);

  return mfMusicformatsErrorKind::kMusicformatsError_NONE;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msdlFile2lilypond (
  const char*             fileName,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromFile (
//         fileName,
		//     gWaeHandler->pass (mfPassIDKind::kMfPassID_1),
//         gWaeHandler->createAnMXSRFromAMusicXMLFile ());
//
//   if (sxmlfile) {
//     return
//       xmlFile2lilypondWithOptionsAndArguments (
//         sxmlfile,
//         handlerOptionsAndArguments,
//         out,
//         err);
//   }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMsdlFile2lilypondWithHandler (
  const char*         fileName,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromFile (
//         fileName,
		//     gWaeHandler->pass (mfPassIDKind::kMfPassID_1),
//         gWaeHandler->createAnMXSRFromAMusicXMLFile ());
//
//   if (sxmlfile) {
//     return
//       sxmlFile2lilypondWithHandler (
//         sxmlfile,
//         out,
//         err,
//         handler);
//   }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msdlFd2lilypond (
  FILE*                   fd,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromFd (
//         fd,
		//     gWaeHandler->pass (mfPassIDKind::kMfPassID_1),
//         gWaeHandler->createAnMXSRFromAMusicXMLDescriptor ());
//
//   if (sxmlfile) {
//     return
//       xmlFile2lilypondWithOptionsAndArguments (
//         sxmlfile,
//         handlerOptionsAndArguments,
//         out,
//         err);
//   }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMsdlFd2lilypondWithHandler (
  FILE*               fd,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromFd (
//         fd,
		//     gWaeHandler->pass (mfPassIDKind::kMfPassID_1),
//         gWaeHandler->createAnMXSRFromAMusicXMLDescriptor ());
//
//   if (sxmlfile) {
//     return
//       sxmlFile2lilypondWithHandler (
//         sxmlfile,
//         out,
//         err,
//         handler);
//   }

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msdlString2lilypond (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromString (
//         buffer,
		//     gWaeHandler->pass (mfPassIDKind::kMfPassID_1),
//         gWaeHandler->createAnMXSRFromAMusicXMLBuffer ());
//
//   // call xmlFile2lilypond() even if sxmlfile is null,
//   // to handle the help options if any
//   return
//     xmlFile2lilypondWithOptionsAndArguments (
//       sxmlfile,
//       handlerOptionsAndArguments,
//       out,
//       err);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}

mfMusicformatsErrorKind convertMsdlString2lilypondWithHandler (
  const char*         buffer,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromString (
//         buffer,
		//     gWaeHandler->pass (mfPassIDKind::kMfPassID_1),
//         "Create an MXSR from a MusicXML buffer");
//
//   // call xmlFile2lilypond() even if sxmlfile is null,
//   // to handle the help options if any
//   return
//     sxmlFile2lilypondWithHandler (
//       sxmlfile,
//       out,
//       err,
//       handler);

  return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
}


}
