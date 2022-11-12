/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

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


namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsErrorKind msrScore2brailleWithHandler (
  S_msrScore   theMsrScore,
  std::string       passNumber1a,
  std::string       passDescription1a,
  std::string       passNumber1b,
  std::string       passDescription1b,
  std::string       passNumber2,
  std::string       passDescription2,
  std::ostream&     out,
  std::ostream&     err,
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Translating an MSR score to braille in \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      std::endl;
  }
#endif
  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // the first BSR score
  S_bsrScore firstBsrScore;

  {
    // create the first BSR from the MSR
    // ------------------------------------------------------

    try {
      firstBsrScore =
        translateMsrToBsr (
          theMsrScore,
          gGlobalMsrOahGroup,
          gGlobalBsrOahGroup,
          passNumber1a,
          passDescription1a);
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
    // create the finalized BSR from the first BSR
    // ------------------------------------------------------

    try {
      finalizedBsrScore =
        translateBsrToFinalizedBsr (
          firstBsrScore,
          gGlobalBsrOahGroup,
          passNumber1b,
          passDescription1b);
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
    // convert the BSR into Braille
    // ------------------------------------------------------

    std::string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2braille() outputFileName = \"" <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2braille() output goes to standard output" <<
          std::endl;
      }
#endif

      // convert the BSR score to braille
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          passNumber2,
          passDescription2,
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
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        err <<
          "xmlFile2braille() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif

      // open output file
#ifdef TRACING_IS_ENABLED
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

      // convert the finalized BSR score to braille
      try {
        translateBsrToBraille (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          passNumber2,
          passDescription2,
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

  return mfMusicformatsErrorKind::kMusicformatsError_NO_;
}


}
