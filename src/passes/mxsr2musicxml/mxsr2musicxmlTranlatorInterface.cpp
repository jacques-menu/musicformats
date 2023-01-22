/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <fstream>

#include "mfStaticSettings.h"

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "mfTiming.h"

#include "mxsr2musicxmlWae.h"

#include "mfStaticSettings.h"

#include "oahEarlyOptions.h"

#include "mfAssert.h"

#include "mxsrGeneration.h"

#include "mxsr2musicxmlTranlatorInterface.h"

namespace MusicFormats
{
//_______________________________________________________________________________
EXP void translateMxsrToMusicXML (
  Sxmlelement        theMxsr,
  std::string        outputFileName,
  std::ostream&      err,
  mfPassIDKind       passIDKind,
  const std::string& passDescription)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    theMxsr != nullptr,
    "translateMxsrToMusicXML(): theMxsr is null");
#endif

  // start the clock
  clock_t startClock = clock ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream s;

    s <<
      separator <<
      std::endl <<
      gTab <<
      gWaeHandler->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator <<
      std::endl;

    gWaeHandler->waeTrace (
      err,
      __FILE__, __LINE__,
      s.str ());
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    err <<
      "translateMxsrToMusicXML() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      std::endl;
  }
#endif

  // create the MusicXML data
  SXMLFile sxmlfile = createSxmlFile ();

  // insert the MXSR into it
  sxmlfile->set (theMxsr);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    err <<
      "xmlFile2musicxml() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      std::endl;
  }
#endif

  if (! outputFileName.size ()) {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2musicxml() output goes to standard output" <<
        std::endl;
    }
#endif

    // write the MusicXML data to the output file stream
    sxmlfile->print (gOutputStream);
    gOutputStream << std::endl;
  }

  else {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      err <<
        "xmlFile2musicxml() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        std::endl;
    }
#endif

    // open output file
    std::ofstream outputFileStream;

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
      err <<
        std::endl <<
        gWaeHandler->openingMusicXMLFileForWriting (outputFileName) <<
        std::endl;
    }
#endif

    std::ofstream
      musicxmlFileOutputStream (
        outputFileName.c_str(),
        std::ofstream::out);

    if (! musicxmlFileOutputStream.is_open ()) {
      std::stringstream s;

      s <<
        gWaeHandler->cannotOpenMusicXMLFileForWriting (outputFileName);

      std::string message = s.str ();

      err <<
        message <<
        std::endl;

      throw mxsr2musicxmlException (message);
    }

    // write the MusicXML data to the output file stream
  if (sxmlfile->elements ()) {
    sxmlfile->print (musicxmlFileOutputStream);
    musicxmlFileOutputStream << std::endl;
  }
  else {
    err << "TXMLFile::elements() is null" << std::endl;
  }

    // close output file
#ifdef TRACE_OAH
    if (gtracingOah->fTracePasses) {
      err <<
        std::endl <<
        gWaeHandler->closingMusicXMLFile (outputFileName) <<
        std::endl;
    }
#endif

    musicxmlFileOutputStream.close ();
  }

  // register time spent
  clock_t endClock = clock ();

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);
}


}
