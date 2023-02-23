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

#include "waeHandlers.h"


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
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

  // set the global current passID
  setGlobalCurrentPassIDKind (passIDKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLog <<
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

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    err <<
      "xmlFile2lilypond() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (! outputFileName.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      err <<
        "xmlFile2lilypond() output goes to standard output" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
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
        passIDKind,
        passDescription,
        lilypondStandardOutputStream);
    }
    catch (lpsr2lilypondException& e) {
      mfDisplayException (e, gOutput);
      return;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutput);
      return;
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

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // open output file
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTracePasses ()) {
      gWaeHandler->waeTrace (
        err,
        __FILE__, __LINE__,
        gLanguage->openingLilypondFileForWriting (outputFileName));
    }
#endif // MF_TRACE_IS_ENABLED

    // create a file output stream
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
        passIDKind,
        passDescription,
        lilypondFileOutputStream);
    }
    catch (lpsr2lilypondException& e) {
      mfDisplayException (e, gOutput);
      return;
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutput);
      return;
    }

    // close output file
#ifdef TRACE_OAH
    if (gTraceOah->fTracePasses) {
      std::stringstream ss;

      ss <<
        std::endl <<
        gLanguage->closingLilypondFile (outputFileName) <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    outputFileStream.close ();
  }
}


}
