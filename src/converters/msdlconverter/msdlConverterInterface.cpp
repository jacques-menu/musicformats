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
#include "msr2lpsrWae.h"
#include "lpsr2lilypondWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"
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

using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsError convertMsdlFile2lilypondWithHandler (
  SXMLFile&     sxmlfile,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
//     gLogStream <<
//       endl <<
//       "<!-- ----------------------------------------------------------- -->" <<
//       endl <<
//       "sxmlFile2lilypondWithHandler(), sxmlfile contains:" <<
//       endl << endl;
//
//     ++gIndenter;
//
//     sxmlfile->print (gLogStream);
//     gLogStream << endl << endl;
//
//     --gIndenter;
//
//     gLogStream <<
//       "<!-- ----------------------------------------------------------- -->" <<
//       endl << endl;
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
//         "Pass 2a",
//         "Create a first MSR skeleton from the MXSR");
//   }
//   catch (mxsr2msrException& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsError::kErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsError::kErrorInvalidFile;
//   }
//
//   // should we return now?
//   // ------------------------------------------------------
//
//   if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass2a ()) {
//     err <<
//       endl <<
//       "Quitting after creating the MSR skeleton in pass 2a of sxmlFile2lilypondWithHandler as requested" <<
//       endl;
//
//     return mfMusicformatsError::k_NoError;
//   }
//
//   // populate the first MSR skeleton from MusicXML data (pass 2b)
//   // ------------------------------------------------------
//
//   try {
//     populateMsrSkeletonFromMxsr (
//       theMxsr,
//       firstMsrScore,
//         "Pass 2b",
//         "Populate the first MSR skeleton from MusicXML data");
//   }
//   catch (mxsr2msrException& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsError::kErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsError::kErrorInvalidFile;
//   }
//
//   // should we return now?
//   // ------------------------------------------------------
//
//   if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass2b ()) {
//     err <<
//       endl <<
//       "Quitting after pass 2b as requested" <<
//       endl;
//
//     return mfMusicformatsError::k_NoError;
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
//       gLogStream <<
//         "===> firstMsrScore voice: " << voice->getVoiceName () <<
//         endl;
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
//           "Pass 3",
//           "Convert the first MSR into a second MSR",
//           pathToVoice);
//     } // for
// }
// else {
//     secondMsrScore =
//       translateMsrToMsr (
//         firstMsrScore,
//         gGlobalMsrOahGroup,
//         gGlobalMsr2msrOahGroup,
//         "Pass 3",
//         "Convert the first MSR into a second MSR");
// }
//   }
//   catch (mxsr2msrException& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsError::kErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsError::kErrorInvalidFile;
//   }
//
//   // should we return now?
//   // ------------------------------------------------------
//
//   if (gGlobalXml2lyInsiderOahGroup->getQuitAfterPass3 ()) {
//     err <<
//       endl <<
//       "Quitting after pass 3 as requested" <<
//       endl;
//
//     return mfMusicformatsError::k_NoError;
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
//           "Pass 4",
//           "Convert the second MSR into an LPSR",
//           createMsdl2lilypondConverterComponent ());
//     }
//     catch (msr2lpsrException& e) {
//       mfDisplayException (e, gOutputStream);
//       return mfMusicformatsError::kErrorInvalidFile;
//     }
//     catch (std::exception& e) {
//       mfDisplayException (e, gOutputStream);
//       return mfMusicformatsError::kErrorInvalidFile;
//     }
//
//     // display the LPSR score if requested
//     // ------------------------------------------------------
//
//     if (gGlobalLpsrOahGroup->getDisplayLpsrShort ()) {
//       displayLpsrScoreShort (
//         theLpsrScore,
//         gGlobalMsrOahGroup,
//         gGlobalLpsrOahGroup,
//         "Display the LPSR");
//     }
//
//     if (gGlobalLpsrOahGroup->getDisplayLpsrFull ()) {
//       displayLpsrScoreFull (
//         theLpsrScore,
//         gGlobalMsrOahGroup,
//         gGlobalLpsrOahGroup,
//         "Display the LPSR");
//     }
//   }
//
//   {
//     // convert the LPSR to LilyPond code (pass 5)
//     // ------------------------------------------------------
//
//     string
//       outputFileName =
//         handler->
//           fetchOutputFileNameFromTheOptions ();
//
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//       err <<
//         "xmlFile2lilypond() outputFileName = \"" <<
//         outputFileName <<
//         "\"" <<
//         endl;
//     }
// #endif
//
//     if (! outputFileName.size ()) {
// #ifdef TRACING_IS_ENABLED
//       if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//         err <<
//           "xmlFile2lilypond() output goes to standard output" <<
//           endl;
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
//       // convert the LPSR score to LilyPond code
//       try {
//         translateLpsrToLilypond (
//           theLpsrScore,
//           gGlobalMsrOahGroup,
//           gGlobalLpsrOahGroup,
//           "Pass 5",
//           "Convert the LPSR score to LilyPond code",
//           lilypondStandardOutputStream);
//       }
//       catch (lpsr2lilypondException& e) {
//         mfDisplayException (e, gOutputStream);
//         return mfMusicformatsError::kErrorInvalidFile;
//       }
//       catch (std::exception& e) {
//         mfDisplayException (e, gOutputStream);
//         return mfMusicformatsError::kErrorInvalidFile;
//       }
//     }
//
//     else {
// #ifdef TRACING_IS_ENABLED
//       if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//         err <<
//           "xmlFile2lilypond() output goes to file \"" <<
//           outputFileName <<
//           "\"" <<
//           endl;
//       }
// #endif
//
//       // open output file
// #ifdef TRACING_IS_ENABLED
//       if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
//         err <<
//           endl <<
//           "Opening file '" << outputFileName << "' for writing" <<
//           endl;
//       }
// #endif
//
//       ofstream
//         outputFileStream (
//           outputFileName.c_str (),
//           ofstream::out);
//
//       if (! outputFileStream.is_open ()) {
//         stringstream s;
//
//         s <<
//           "Could not open LilyPond output file \"" <<
//           outputFileName <<
//           "\" for writing, quitting";
//
//         string message = s.str ();
//
//         err <<
//           message <<
//           endl;
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
//       // convert the LPSR score to LilyPond code
//       try {
//         translateLpsrToLilypond (
//           theLpsrScore,
//           gGlobalMsrOahGroup,
//           gGlobalLpsrOahGroup,
//           "Pass 5",
//           "Convert the LPSR score to LilyPond code",
//           lilypondFileOutputStream);
//       }
//       catch (lpsr2lilypondException& e) {
//         mfDisplayException (e, gOutputStream);
//         return mfMusicformatsError::kErrorInvalidFile;
//       }
//       catch (std::exception& e) {
//         mfDisplayException (e, gOutputStream);
//         return mfMusicformatsError::kErrorInvalidFile;
//       }
//
//       // close output file
// #ifdef TRACE_OAH
//       if (gtracingOah->fTracePasses) {
//         gLogStream <<
//           endl <<
//           "Closing file \"" << outputFileName << "\"" <<
//           endl;
//       }
// #endif
//
//       outputFileStream.close ();
//     }
//   }

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
static mfMusicformatsError msdlFile2lilypondWithOptionsAndArguments (
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
//         return mfMusicformatsError::kErrorUnsupported;
//     }
//   }
//
//   else {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//   err <<
//     "xmlFile2musicxml(), sxmlfile is NULL" <<
//     endl;
//   }
// #endif
//
//     return mfMusicformatsError::kErrorInvalidFile;
//   }
//
//   // the service name
//   // ------------------------------------------------------
//
//   string serviceName = "xml2ly";
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
// #ifdef TRACING_IS_ENABLED
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
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
//     gLogStream <<
//       serviceName << " main()" <<
//       ", insiderOption: " << insiderOption <<
//       endl;
//   }
// #endif
//
//   // create an xml2ly insider OAH handler
//   // ------------------------------------------------------
//
//   // create an insider xml2ly OAH handler
//   S_xml2lyInsiderHandler
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
//         return mfMusicformatsError::k_NoError; // quit now
//         break;
//       case oahElementHelpOnlyKind::kElementHelpOnlyNo:
//         // go ahead
//         break;
//     } // switch
//   }
//   catch (mfOahException& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsError::kErrorInvalidOption;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutputStream);
//     return mfMusicformatsError::kErrorInvalidFile;
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
//       endl;
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

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
EXP mfMusicformatsError msdlFile2lilypond (
  const char*             fileName,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromFile (
//         fileName,
//         "Pass 1",
//         "Create an MXSR reading a MusicXML file");
//
//   if (sxmlfile) {
//     return
//       xmlFile2lilypondWithOptionsAndArguments (
//         sxmlfile,
//         handlerOptionsAndArguments,
//         out,
//         err);
//   }

  return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMsdlFile2lilypondWithHandler (
  const char*  fileName,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromFile (
//         fileName,
//         "Pass 1",
//         "Create an MXSR reading a MusicXML file");
//
//   if (sxmlfile) {
//     return
//       sxmlFile2lilypondWithHandler (
//         sxmlfile,
//         out,
//         err,
//         handler);
//   }

  return mfMusicformatsError::kErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsError msdlFd2lilypond (
  FILE*                   fd,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromFd (
//         fd,
//         "Pass 1",
//         "Create an MXSR reading a MusicXML descriptor");
//
//   if (sxmlfile) {
//     return
//       xmlFile2lilypondWithOptionsAndArguments (
//         sxmlfile,
//         handlerOptionsAndArguments,
//         out,
//         err);
//   }

  return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMsdlFd2lilypondWithHandler (
  FILE*        fd,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromFd (
//         fd,
//         "Pass 1",
//         "Create an MXSR reading a MusicXML descriptor");
//
//   if (sxmlfile) {
//     return
//       sxmlFile2lilypondWithHandler (
//         sxmlfile,
//         out,
//         err,
//         handler);
//   }

  return mfMusicformatsError::kErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsError msdlString2lilypond (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromString (
//         buffer,
//         "Pass 1",
//         "Create an MXSR reading a MusicXML buffer");
//
//   // call xmlFile2lilypond() even if sxmlfile is null,
//   // to handle the help options if any
//   return
//     xmlFile2lilypondWithOptionsAndArguments (
//       sxmlfile,
//       handlerOptionsAndArguments,
//       out,
//       err);

  return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMsdlString2lilypondWithHandler (
  const char*  buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
//   SXMLFile
//     sxmlfile =
//       createSXMLFileFromString (
//         buffer,
//         "Pass 1",
//         "Create an MXSR reading a MusicXML buffer");
//
//   // call xmlFile2lilypond() even if sxmlfile is null,
//   // to handle the help options if any
//   return
//     sxmlFile2lilypondWithHandler (
//       sxmlfile,
//       out,
//       err,
//       handler);

  return mfMusicformatsError::kErrorInvalidFile;
}


}
