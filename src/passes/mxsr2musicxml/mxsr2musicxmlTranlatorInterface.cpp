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

#include "mfTiming.h"

#include "mxsr2musicxmlWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "mfAssert.h"

#include "mxsrGeneration.h"

#include "msr.h"

#include "mxsr2musicxmlTranlatorInterface.h"

using namespace std;

namespace MusicFormats
{
//_______________________________________________________________________________
EXP void translateMxsrToMusicXML (
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
    "translateMxsrToMusicXML(): theMxsr is null");

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    err <<
      "translateMxsrToMusicXML() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      endl;
  }
#endif

  // create the MusicXML data
  SXMLFile sxmlfile = createSxmlFile ();

  // insert the MXSR into it
  sxmlfile->set (theMxsr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    err <<
      "xmlFile2musicxml() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      endl;
  }
#endif

  if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2musicxml() output goes to standard output" <<
        endl;
    }
#endif

    // write the MusicXML data to the output file stream
    sxmlfile->print (gOutputStream);
    gOutputStream << endl;
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2musicxml() output goes to file \"" <<
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
      musicxmlFileOutputStream (
        outputFileName.c_str(),
        ofstream::out);

    if (! musicxmlFileOutputStream.is_open ()) {
      stringstream s;

      s <<
        "Could not open MusicXML output file \"" <<
        outputFileName <<
        "\" for writing - quitting";

      string message = s.str ();

      err <<
        message <<
        endl;

      throw mxsr2musicxmlException (message);
    }

    // write the MusicXML data to the output file stream
  if (sxmlfile->elements ()) {
    sxmlfile->print (musicxmlFileOutputStream);
    musicxmlFileOutputStream << endl;
  }
  else {
    err << "TXMLFile::elements() is null" << endl;
  }

    // close output file
#ifdef TRACE_OAH
    if (gtracingOah->fTracePasses) {
      err <<
        endl <<
        "Closing file \"" << outputFileName << "\"" <<
        endl;
    }
#endif

    musicxmlFileOutputStream.close ();
  }

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passNumber,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);
}


}
