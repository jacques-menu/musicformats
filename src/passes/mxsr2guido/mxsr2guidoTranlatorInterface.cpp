/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <fstream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"


#include "mxsr2guidoWae.h"

#include "enableTracingIfDesired.h"

#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfTiming.h"

#include "guidoGenerationOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2guidoInsiderHandler.h"

#include "msr.h"

#include "mxsrGeneration.h"

#include "xml2guidovisitor.h"


using namespace std;

namespace MusicFormats
{
//_______________________________________________________________________________
void translateMxsrToGuido (
  Sxmlelement   theMxsr,
  string        outputFileName,
  ostream&      err,
  const string& passNumber,
  const string& passDescription)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMxsr != nullptr,
    "translateMxsrToGuido(): theMxsr is null");

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";

    err <<
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

  // convert the MXSR intto Guido data
  xml2guidovisitor v (
    gGlobalGuidoGenerationOahGroup->
      getGenerateGuidoComments ().getValue (),
    gGlobalGuidoGenerationOahGroup->
      getGenerateGuidoStem ().getValue (),
    gGlobalGuidoGenerationOahGroup->
      getGenerateGuidoBars ().getValue (),
    0); // partNum, i.e. all parts

  Sguidoelement
    guidoData = v.convert (theMxsr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    err <<
      "xmlFile2gmn() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      endl;
  }
#endif

  if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2gmn() output goes to standard output" <<
        endl;
    }
#endif

    // write the Guido data to the output file stream
    gOutputStream <<
      guidoData <<
      endl;
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2gmn() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

    // open output file
    ofstream outputFileStream;

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
      err <<
        endl <<
        "Opening file '" << outputFileName << "' for writing" <<
        endl;
    }
#endif

    ofstream
      guidoFileOutputStream (
        outputFileName.c_str(),
        ofstream::out);

    if (! guidoFileOutputStream.is_open ()) {
      stringstream s;

      s <<
        "Could not open Guido output file \"" <<
        outputFileName <<
        "\" for writing - quitting";

      string message = s.str ();

      err <<
        message <<
        endl;

      throw mxsr2guidoException (message);
    }

    // write the Guido data to the output file stream
    guidoFileOutputStream <<
      guidoData <<
      endl;

    // close output file
#ifdef TRACE_OAH
    if (gtracingOah->fTracePasses) {
      err <<
        endl <<
        "Closing file \"" << outputFileName << "\"" <<
        endl;
    }
#endif

    guidoFileOutputStream.close ();
  }

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItem::kMandatory,
    startClock,
    endClock);
}


}
