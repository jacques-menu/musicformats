/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <fstream>      // std::ofstream, std::ofstream::open(), std::ofstream::close()

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfTiming.h"

#include "lpsrScores.h"

#include "lpsr2lilypondWae.h"

#include "mfStaticSettings.h"

#include "lpsr2lilypondOah.h"

#include "oahEarlyOptions.h"

#include "lpsr2lilypondTranslator.h"

#include "lpsr2lilypondInterface.h"


namespace MusicFormats
{

//_______________________________________________________________________________
void translateLpsrToLilypond (
  const S_lpsrScore&    theLpsrScore,
  const S_msrOahGroup&  msrOpts,
  const S_lpsrOahGroup& lpsrOpts,
  mfPassIDKind          passIDKind,
  const std::string&    passDescription,
  std::ostream&         lilypondCodeStream)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");
#endif

  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gWaeHandler->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
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
    passIDKind,
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
  const S_lpsrScore&    theLpsrScore,
  const S_msrOahGroup&  msrOpts,
  const S_lpsrOahGroup& lpsrOpts,
  mfPassIDKind          passIDKind,
  const std::string&    passDescription,
  S_oahHandler          handler,
  std::ostream&         out,
  std::ostream&         err)
{
  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    err <<
      "xmlFile2lilypond() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif

  if (! outputFileName.size ()) {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2lilypond() output goes to standard output" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif

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
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        passIDKind,
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
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2lilypond() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif

    // open output file
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
      gWaeHandler->waeTrace (
        err,
        __FILE__, __LINE__,
        gWaeHandler->openingLilypondFileForWriting (outputFileName));
    }
#endif

    // create a file output stream
    std::ofstream
      outputFileStream (
        outputFileName.c_str (),
        std::ofstream::out);

    if (! outputFileStream.is_open ()) {
      std::stringstream ss;

      ss <<
        gWaeHandler->cannotOpenLilypondFileForWriting (outputFileName);

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
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup,
        passIDKind,
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
      std::stringstream ss;

      ss <<
        std::endl <<
        gWaeHandler->closingLilypondFile (outputFileName) <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

    outputFileStream.close ();
  }
}


}
