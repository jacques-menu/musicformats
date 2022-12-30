/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <fstream>      // std::ofstream, std::ofstream::open(), std::ofstream::close()

#include "mfAssert.h"
#include "mfTiming.h"

#include "lpsrScores.h"

#include "lpsr2lilypondWae.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "lpsr2lilypondOah.h"

#include "oahEarlyOptions.h"

#include "lpsr2lilypondTranslator.h"

#include "lpsr2lilypondInterface.h"


namespace MusicFormats
{

//_______________________________________________________________________________
void translateLpsrToLilypond (
  const S_lpsrScore& theLpsrScore,
  const S_msrOahGroup&     msrOpts,
  const S_lpsrOahGroup&    lpsrOpts,
  const std::string&     passNumber,
  const std::string&     passDescription,
  std::ostream&          lilypondCodeStream)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  std::string separator =
    "%--------------------------------------------------------------";

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    gLogStream <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      passNumber << ": " << passDescription <<
      std::endl <<
      separator <<
      std::endl;
  }
#endif

  // the lpsr2lilypondTranslator
  lpsr2lilypondTranslator
    translator (
      msrOpts,
      lpsrOpts,
      lilypondCodeStream);

  // generate the LilyPond code
  translator.translateLpsrToLilypondCode (
    theLpsrScore);

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### translateLpsrToLilypond gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }
}

EXP void translateLpsrToLilypondWithHandler (
  const S_lpsrScore& theLpsrScore,
  const S_msrOahGroup&     msrOpts,
  const S_lpsrOahGroup&    lpsrOpts,
  const std::string&     passNumber,
  const std::string&     passDescription,
  S_oahHandler      handler,
  std::ostream&          out,
  std::ostream&          err)
{
  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    err <<
      "xmlFile2lilypond() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      std::endl;
  }
#endif

  if (! outputFileName.size ()) {
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2lilypond() output goes to standard output" <<
        std::endl;
    }
#endif

    // create an indented output stream for the LilyPond code
    // to be written to outputFileStream
    mfIndentedOstream
      lilypondStandardOutputStream (
        out,
        gIndenter);

    // convert the LPSR score to LilyPond code
    try {
      translateLpsrToLilypond (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        passNumber,
        passDescription,
        lilypondStandardOutputStream);
    }
    catch (lpsr2lilypondException& e) {
      mfDisplayException (e, gOutputStream);
      return;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return;
    }
  }

  else {
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2lilypond() output goes to file \"" <<
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
        "Opening file '" << outputFileName << "' for writing" <<
        std::endl;
    }
#endif

    // create a file output stream
    std::ofstream
      outputFileStream (
        outputFileName.c_str (),
        std::ofstream::out);

    if (! outputFileStream.is_open ()) {
      std::stringstream s;

      s <<
        "Could not open LilyPond output file \"" <<
        outputFileName <<
        "\" for writing - quitting";

      std::string message = s.str ();

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

    // convert the LPSR score to LilyPond code
    try {
      translateLpsrToLilypond (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        passNumber,
        passDescription,
        lilypondFileOutputStream);
    }
    catch (lpsr2lilypondException& e) {
      mfDisplayException (e, gOutputStream);
      return;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutputStream);
      return;
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

    outputFileStream.close ();
  }
}


}
