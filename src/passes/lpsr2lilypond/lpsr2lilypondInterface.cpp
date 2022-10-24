/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "mfAssert.h"
#include "mfTiming.h"

#include "lpsr2lilypondWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "lpsr2lilypondOah.h"

#include "oahEarlyOptions.h"

#include "lpsr2lilypondTranslator.h"

#include "lpsr2lilypondInterface.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
void translateLpsrToLilypond (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  const string&     passNumber,
  const string&     passDescription,
  ostream&          lilypondCodeStream)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theLpsrScore != nullptr,
    "theLpsrScore is null");

  // start the clock
  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      passNumber << ": " << passDescription <<
      endl <<
      separator <<
      endl;
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
    mfTimingItem::kMandatory,
    startClock,
    endClock);

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### translateLpsrToLilypond gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }
}

EXP void translateLpsrToLilypondWithHandler (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  const string&     passNumber,
  const string&     passDescription,
  S_oahHandler      handler,
  ostream&          out,
  ostream&          err)
{
  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    err <<
      "xmlFile2lilypond() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      endl;
  }
#endif

  if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2lilypond() output goes to standard output" <<
        endl;
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
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2lilypond() output goes to file \"" <<
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
        "Opening file '" << outputFileName << "' for writing" <<
        endl;
    }
#endif

    // create a file output stream
    ofstream
      outputFileStream (
        outputFileName.c_str (),
        ofstream::out);

    if (! outputFileStream.is_open ()) {
      stringstream s;

      s <<
        "Could not open LilyPond output file \"" <<
        outputFileName <<
        "\" for writing - quitting";

      string message = s.str ();

      err <<
        message <<
        endl;

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
        endl <<
        "Closing file \"" << outputFileName << "\"" <<
        endl;
    }
#endif

    outputFileStream.close ();
  }
}


}
