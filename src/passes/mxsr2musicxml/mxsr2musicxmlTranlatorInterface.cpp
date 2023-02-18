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

#include "waeHandlers.h"


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
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // start the clock
  clock_t startClock = clock ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      separator <<
      std::endl <<
      gTab <<
      gLanguage->passIDKindAsString (passIDKind) << ": " << passDescription <<
      std::endl <<
      separator <<
      std::endl;

    gWaeHandler->waeTrace (
      err,
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    err <<
      "translateMxsrToMusicXML() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the MusicXML data
  SXMLFile sxmlfile = createSxmlFile ();

  // insert the MXSR into it
  sxmlfile->set (theMxsr);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    err <<
      "xmlFile2musicxml() outputFileName = \"" <<
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
        "xmlFile2musicxml() output goes to standard output" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // write the MusicXML data to the output file stream
    sxmlfile->print (gOutput);
    gOutput << std::endl;
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      err <<
        "xmlFile2musicxml() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // open output file
    std::ofstream outputFileStream;

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTracePasses ()) {
      err <<
        std::endl <<
        gLanguage->openingMusicXMLFileForWriting (outputFileName) <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    std::ofstream
      musicxmlFileOutputStream (
        outputFileName.c_str(),
        std::ofstream::out);

    if (! musicxmlFileOutputStream.is_open ()) {
      std::stringstream ss;

      ss <<
        gLanguage->cannotOpenMusicXMLFileForWriting (outputFileName);

      std::string message = ss.str ();

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
    if (gTraceOah->fTracePasses) {
      err <<
        std::endl <<
        gLanguage->closingMusicXMLFile (outputFileName) <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    musicxmlFileOutputStream.close ();
  }

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    passIDKind,
    passDescription,
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);
}


}
