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

#include "mxsr2guidoWae.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfTiming.h"

#include "guidoGenerationOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2guidoInsiderHandler.h"

#include "mxsrGeneration.h"

#include "xml2guidovisitor.h"

#include "waeHandlers.h"


namespace MusicFormats
{
//_______________________________________________________________________________
void translateMxsrToGuido (
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
    "translateMxsrToGuido(): theMxsr is null");
#endif

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

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    err <<
      "xmlFile2gmn() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
  }
#endif

  if (! outputFileName.size ()) {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceOah ()) {
      err <<
        "xmlFile2gmn() output goes to standard output" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif

    // write the Guido data to the output file stream
    gOutput <<
      guidoData <<
      std::endl;
  }

  else {
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceOah ()) {
      err <<
        "xmlFile2gmn() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif

    // open output file
    std::ofstream outputFileStream;

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTracePasses ()) {
      err <<
        std::endl <<
        gLanguage->openingGuidoFileForWriting (outputFileName) <<
        std::endl;

//     gWaeHandler->waeTrace ( JMI v0.9.67
//       __FILE__, __LINE__,
//       ss.str ());
    }
#endif

    std::ofstream
      guidoFileOutputStream (
        outputFileName.c_str(),
        std::ofstream::out);

    if (! guidoFileOutputStream.is_open ()) {
      std::stringstream ss;

      ss <<
        gLanguage->cannotOpenGuidoFileForWriting (outputFileName);

      std::string message = ss.str ();

      err <<
        message <<
        std::endl;

      throw mxsr2guidoException (message);
    }

    // write the Guido data to the output file stream
    guidoFileOutputStream <<
      guidoData <<
      std::endl;

    // close output file
#ifdef TRACE_OAH
    if (gTraceOah->fTracePasses) {
      err <<
        std::endl <<
        gLanguage->closingGuidoFile (outputFileName) <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

    guidoFileOutputStream.close ();
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
