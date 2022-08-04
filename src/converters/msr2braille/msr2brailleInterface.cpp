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


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
EXP mfMusicformatsError msrScore2brailleWithHandler (
  S_msrScore   theMsrScore,
  string       passNumber1a,
  string       passDescription1a,
  string       passNumber1b,
  string       passDescription1b,
  string       passNumber2,
  string       passDescription2,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Translating an MSR score to braille in \"" <<
      handler->getHandlerHeader () <<
      "\"" <<
      endl;
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
      return mfMusicformatsError::kErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsError::kErrorInvalidFile;
    }

    // display the first BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsrFirst ()) {
      displayBsrScore (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the first BSR");
    }

    if (gGlobalBsrOahGroup->getDisplayBsrFirstShort ()) {
      displayBsrScoreShort (
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
      return mfMusicformatsError::kErrorInvalidFile;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return mfMusicformatsError::kErrorInvalidFile;
    }

    // display the finalized BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsrSecond ()) {
      displayBsrScore (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the finalized BSR");
    }

    if (gGlobalBsrOahGroup->getDisplayBsrSecondShort ()) {
      displayBsrScoreShort (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup,
        "Display the finalized BSR");
    }
  }

  {
    // convert the BSR into Braille
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
          "\" for writing, quitting";

        string message = s.str ();

        err <<
          message <<
          endl;

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


}
