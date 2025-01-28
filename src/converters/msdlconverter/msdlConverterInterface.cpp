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

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2lpsrWae.h"
#include "lpsr2lilypondWae.h"

#include "mfPreprocessorSettings.h"

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
#include "mxsr2msrSkeletonPopulatorInterface.h"
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
    //     mfPassIDKind::kMfPassID_1,
//         gLanguage->convertAMusicXMLStreamIntoAnMXSR ());
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

mfMusicformatsErrorKind convertMsdlFile2lilypondWithHandler ( // JMI ??? v0.9.66
  const char*         fileName,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromFile (
//         fileName,
    //     mfPassIDKind::kMfPassID_1,
//         gLanguage->convertAMusicXMLStreamIntoAnMXSR ());
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
    //     mfPassIDKind::kMfPassID_1,
//         gLanguage->convertAMusicXMLDescriptorIntoAnMXSR ());
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
    //     mfPassIDKind::kMfPassID_1,
//         gLanguage->convertAMusicXMLDescriptorIntoAnMXSR ());
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
    //     mfPassIDKind::kMfPassID_1,
//         gLanguage->convertAMusicXMLBufferIntoAnMXSR ());
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
    //     mfPassIDKind::kMfPassID_1,
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


// //_______________________________________________________________________________
// static mfMusicformatsErrorKind convertMsdlFile2lilypondWithHandler ( // JMI UNUSED v0.9.66
//   SXMLFile&           sxmlfile,
//   std::ostream&       out,
//   std::ostream&       err,
//   const S_oahHandler& handler)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gGlobalMxsr2msrOahGroup->getDisplayMxsr ()) {
//     gLog <<
//       std::endl <<
//       "<!-- ----------------------------------------------------------- -->" <<
//       std::endl <<
//       "sxmlFile2lilypondWithHandler(), sxmlfile contains:" <<
//       std::endl << std::endl;
//
//     ++gIndenter;
//
//     sxmlfile->print (gLog);
//     ss << std::endl << std::endl;
//
//     --gIndenter;
//
//     gLog <<
//       "<!-- ----------------------------------------------------------- -->" <<
//       std::endl << std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // has quiet mode been requested?
//   // ------------------------------------------------------
//
//   if (gEarlyOptions.getEarlyQuietOption ()) {
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
//   mxsrEventsCollection eventsCollection;
//
//   try {
//     firstMsrScore =
//       translateMxsrToMsrSkeleton (
//         theMxsr,
//         gMsrOahGroup,
//         mfPassIDKind::kMfPassID_2a,
//         gLanguage->convertTheMXSRIntoAnMSRSkeleton ());
//   }
//   catch (mxsr2msrException& e) {
//     mfDisplayException (e, gOutput);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutput);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // should we return now?
//   // ------------------------------------------------------
//
//   if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass2a ()) {
//     err <<
//       std::endl <<
//       gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_2a MSR skeleton in pass 2a of sxmlFile2lilypondWithHandler as requested" <<
//       std::endl;
//
//     return mfMusicformatsErrorKind::kMusicformatsError_NONE;
//   }
//
//   // populate the MSR skeleton from the MXSR (pass 2b)
//   // ------------------------------------------------------
//
//   try {
//     populateMsrSkeletonFromMxsr (
//       theMxsr,
//       firstMsrScore,
//       eventsCollection,
// 			 mfPassIDKind::kMfPassID_2b,
// 			 gLanguage->populateTheMSRSkeletonFromMusicXMLData ());
//   }
//   catch (mxsr2msrException& e) {
//     mfDisplayException (e, gOutput);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutput);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // should we return now?
//   // ------------------------------------------------------
//
//   if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass2b ()) {
//     err <<
//       std::endl <<
//       gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_2b as requested" <<
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
//     for (S_msrVoice voice: firstMsrScore->getScoreAllVoicesList ()) {
//       gLog <<
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
//           gMsrOahGroup,
//           gGlobalMsr2msrOahGroup,
//           mfPassIDKind::kMfPassID_3,
//           gLanguage->convertTheFirstMSRIntoASecondMSR (),
//           pathToVoice);
//     } // for
// }
// else {
//     secondMsrScore =
//       translateMsrToMsr (
//         firstMsrScore,
//         gMsrOahGroup,
//         gGlobalMsr2msrOahGroup,
//         mfPassIDKind::kMfPassID_3,
//         gLanguage->convertTheFirstMSRIntoASecondMSR ());
// }
//   }
//   catch (mxsr2msrException& e) {
//     mfDisplayException (e, gOutput);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutput);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // should we return now?
//   // ------------------------------------------------------
//
//   if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass3 ()) {
//     err <<
//       std::endl <<
//       gLanguage->quittingAfterPass (mfPassIDKind::kMfPassID_3 as requested" <<
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
//           gMsrOahGroup,
//           gLpsrOahGroup,
//           mfPassIDKind::kMfPassID_4,
//           gLanguage->convertTheSecondMSRIntoAnLPSR (),
//           createMsdl2lilypondConverterComponent ());
//     }
//     catch (msr2lpsrException& e) {
//       mfDisplayException (e, gOutput);
//       return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//     }
//     catch (std::exception& e) {
//       mfDisplayException (e, gOutput);
//       return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//     }
//
//     // display the LPSR score if requested
//     // ------------------------------------------------------
//
//     if (gLpsrOahGroup->getDisplayLpsr ()) {
//       displayLpsrScore (
//         theLpsrScore,
//         gMsrOahGroup,
//         gLpsrOahGroup,
//          gLanguage->displayTheLPSRAsText ());
//     }
//
//     if (gLpsrOahGroup->getDisplayLpsrFull ()) {
//       displayLpsrScoreFull (
//         theLpsrScore,
//         gMsrOahGroup,
//         gLpsrOahGroup,
//         gLanguage->displayTheLPSRAsText ()
//            +
//         ", " <<
//            +
//        gLanguage->fullVersion ());
//     }
//
// 		// display the LPSR score summary if requested
// 		// ------------------------------------------------------
//
// 		if (gLpsrOahGroup->getDisplayLpsrSummary ()) {
// 			// display the score summary
// 			displayLpsrScoreSummary (
// 				fResultingLpsr,
// 				gMsrOahGroup,
// 				gLpsrOahGroup,
// 				gLanguage->displayASummaryOfTheLPSR ());
// 		}
//
// 		// display the LPSR score names if requested
// 		// ------------------------------------------------------
//
// 		if (gLpsrOahGroup->getDisplayLpsrNames ()) {
// 			// display the score name
// 			displayLpsrScoreNames (
// 				fResultingLpsr,
// 				gMsrOahGroup,
// 				gLpsrOahGroup,
// 				gLanguage->displayTheNamesInTheLPSR ());
// 		}
//
// 		// display the LPSR flat view if requested
// 		// ------------------------------------------------------
//
// 		if (gLpsrOahGroup->getDisplayLpsrFlatView ()) {
// 			// display the score name
// 			displayLpsrScoreFlatView (
// 				fResultingLpsr,
// 				gMsrOahGroup,
// 				gLpsrOahGroup,
// 				gLanguage->displayAFlatViewOfTheLPSR ());
// 		}
//
// 		// display the LPSR slices if requested
// 		// ------------------------------------------------------
//
// 		if (gLpsrOahGroup->getDisplayLpsrMeasuresSlices ()) {
// 			// display the score name
// 			displayLpsrScoreSlices (
// 				fResultingLpsr,
// 				gMsrOahGroup,
// 				gLpsrOahGroup,
// 				gLanguage->displayTheSlicesOfTheLPSR ());
// 		}
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
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getTraceEarlyOptions ()) {
//       err <<
//         "xmlFile2lilypond() outputFileName: \"" <<
//         outputFileName <<
//         "\"" <<
//         std::endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     if (! outputFileName.size ()) {
// #ifdef MF_TRACE_IS_ENABLED
//       if (gEarlyOptions.getTraceEarlyOptions ()) {
//         err <<
//           "xmlFile2lilypond() output goes to standard output" <<
//           std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED
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
//           gMsrOahGroup,
//           gLpsrOahGroup,
//           mfPassIDKind::kMfPassID_5,
//           gLanguage->convertTheLPSRIntoLilyPondCode (),
//           lilypondStandardOutputStream);
//       }
//       catch (lpsr2lilypondException& e) {
//         mfDisplayException (e, gOutput);
//         return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//       }
//       catch (std::exception& e) {
//         mfDisplayException (e, gOutput);
//         return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//       }
//     }
//
//     else {
// #ifdef MF_TRACE_IS_ENABLED
//       if (gEarlyOptions.getTraceEarlyOptions ()) {
//         err <<
//           "xmlFile2lilypond() output goes to file \"" <<
//           outputFileName <<
//           "\"" <<
//           std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       // open output file
// #ifdef MF_TRACE_IS_ENABLED
//       if (gEarlyOptions.getEarlyTracePasses ()) {
//         err <<
//           std::endl <<
//            gLanguage->openingLilypondFileForWriting (outputFileName) <<
//           std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       std::ofstream
//         outputFileStream (
//           outputFileName.c_str (),
//           std::ofstream::out);
//
//       if (! outputFileStream.is_open ()) {
//         std::stringstream ss;
//
//         ss <<
//           gLanguage->cannotOpenLilypondFileForWriting (outputFileName);
//
//         std::string message = ss.str ();
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
//           gMsrOahGroup,
//           gLpsrOahGroup,
//           mfPassIDKind::kMfPassID_5,
//           gLanguage->convertTheLPSRIntoLilyPondCode (),
//           lilypondFileOutputStream);
//       }
//       catch (lpsr2lilypondException& e) {
//         mfDisplayException (e, gOutput);
//         return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//       }
//       catch (std::exception& e) {
//         mfDisplayException (e, gOutput);
//         return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//       }
//
//       // close output file
// #ifdef TRACE_OAH
//       if (gTraceOah->fTracePasses) {
//         gLog <<
//           std::endl <<
//           gLanguage->closingLilypondFile (outputFileName) <<
//           std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       outputFileStream.close ();
//     }
//   }
//
//   return mfMusicformatsErrorKind::kMusicformatsError_NONE;
// }

// //_______________________________________________________________________________
// static mfMusicformatsErrorKind msdlFile2lilypondWithOptionsAndArguments ( // JMI UNUSED v0.9.66
//   SXMLFile&               sxmlfile,
//   oahOptionsAndArguments& handlerOptionsAndArguments,
//   std::ostream&           out,
//   std::ostream&           err)
// {
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
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//   err <<
//     "xmlFile2musicxml(), sxmlfile is NULL" <<
//     std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
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
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       handlerOptionsAndArguments;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // apply early options if any
//   // ------------------------------------------------------
//
//   gEarlyOptions.applyEarlyOptionsIfPresentInArgcArgv (
//     argc,
//     argv);
//
//   // has the '-insider' option been used?
//   // ------------------------------------------------------
//
//   Bool insiderOption =
//     gEarlyOptions.getEarlyInsiderOption ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     gLog <<
//       serviceName << " main()" <<
//       ", insiderOption: " << insiderOption <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
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
//   gServiceRunData =
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
//     mfDisplayException (e, gOutput);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutput);
//     return mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // check indentation
//   if (gIndenter != 0) {
//     gLog <<
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
//
//   return mfMusicformatsErrorKind::kMusicformatsError_NONE;
// }

