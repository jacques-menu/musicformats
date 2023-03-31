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

#include "bsrScores.h"

#include "msr2bsrWae.h"
#include "bsr2bsrWae.h"
#include "bsr2brailleWae.h"

#include "mfStaticSettings.h"

#include "waeOah.h"

#include "msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"

#include "oahEarlyOptions.h"

#include "msr2brailleInsiderHandler.h"
#include "msr2brailleRegularHandler.h"

#include "msr2mxsrInterface.h"
#include "mxsr2msrSkeletonBuilderInterface.h"
#include "mxsr2msrTranslatorInterface.h"
#include "msr2mxsrInterface.h"
#include "msr2bsrInterface.h"
#include "bsr2bsrFinalizerInterface.h"
#include "bsr2brailleTranslatorInterface.h"

#include "bsrInterface.h"

#include "msr2brailleInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msrScore2brailleWithHandler (
  const S_msrScore&   theMsrScore,
  mfPassIDKind        passID_A,
  std::string         passDescription_A,
  mfPassIDKind        passID_B,
  std::string         passDescription_B,
  mfPassIDKind        passID_C,
  std::string         passDescription_C,
  std::ostream&       out,
  std::ostream&       err,
  const S_oahHandler& handler)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Translating an MSR score to braille in \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      std::endl;

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

  // the first BSR score
  S_bsrScore firstBsrScore;

  {
    // create the first BSR from the MSR (pass A)
    // ------------------------------------------------------

    try {
      firstBsrScore =
        translateMsrToBsr (
          theMsrScore,
          gMsrOahGroup,
          gBsrOahGroup,
          passID_A,
          passDescription_A);
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
    // create the finalized BSR from the first BSR (pass B)
    // ------------------------------------------------------

    try {
      finalizedBsrScore =
        translateBsrToFinalizedBsr (
          firstBsrScore,
          gBsrOahGroup,
          passID_B,
          passDescription_B);
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
    // convert the BSR into Braille
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

      // convert the BSR score to braille( pass C)
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gBsrOahGroup,
          passID_C,
          passDescription_C,
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

      // convert the finalized BSR score to braille (pass D) // JMI ??? v0.9.66
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gBsrOahGroup,
          passID_C,
          passDescription_C,
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


}
