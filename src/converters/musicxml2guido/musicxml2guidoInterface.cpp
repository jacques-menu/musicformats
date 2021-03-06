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

#include "oahWae.h"
#include "mxsr2msrWae.h"
#include "msr2msrWae.h"
#include "msr2mxsrWae.h"
#include "mxsr2guidoWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2guidoInsiderHandler.h"
#include "musicxml2guidoRegularHandler.h"

#include "musicxml2mxsrInterface.h"
#include "mxsr2msrSkeletonBuilderInterface.h"
#include "mxsr2msrTranslatorInterface.h"

#include "msr2msrInterface.h"
#include "msr2mxsrInterface.h"
#include "mxsr2guidoTranlatorInterface.h"

#include "mxsrGeneration.h"

#include "msrInterface.h"

#include "musicxml2guidoInterface.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
static mfMusicformatsError xmlFile2guidoWithHandler (
  SXMLFile&     sxmlfile,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
    gLogStream <<
      endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl <<
      "xmlFile2guidoWithHandler(), sxmlfile contains:" <<
      endl << endl;

    ++gIndenter;

    sxmlfile->print (gLogStream);
    gLogStream << endl << endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl << endl;
  }
#endif

  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // get the original MXSR
  // ------------------------------------------------------

  Sxmlelement
    originalMxsr =
      sxmlfile->elements ();

  // the MSR score
  // ------------------------------------------------------

  S_msrScore firstMsrScore;

  // create the skeleton of the first MSR from the originalMxsr (pass 2a)
  // ------------------------------------------------------

  try {
    firstMsrScore =
      translateMxsrToMsrSkeleton (
        originalMxsr,
        gGlobalMsrOahGroup,
        "Pass 2a",
        "Create the skeleton of the first MSR from the originalMxsr");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // display the first MSR score skeleton if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayMsrSkeleton ()) {
    displayMsrScore (
      firstMsrScore,
      "Display the first MSR score skeleton");
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass2a ()) {
    err <<
      endl <<
      "Quitting after creating the MSR skeleton in pass 2a of xmlFile2guidoWithHandler as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }


  // populate the first MSR skeleton from MusicXML data (pass 2b)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxsr (
      originalMxsr,
      firstMsrScore,
        "Pass 2b",
        "Populate the first MSR skeleton from MusicXML data");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // display the populated MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplayFirstMsr ()) {
    displayMsrScore (
      firstMsrScore,
      "Display the first MSR as text");
  }

  if (gGlobalMsrOahGroup->getDisplayFirstMsrShort ()) {
    displayMsrScoreShort (
      firstMsrScore,
      "Display the first MSR as text");
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      endl <<
      "Quitting after pass 2b as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // convert the first MSR score into a second MSR (pass 3)
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      translateMsrToMsr (
        firstMsrScore,
        gGlobalMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        "Pass 3",
        "Convert the first MSR into a second MSR");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // display the second MSR score if requested
  // ------------------------------------------------------

  if (gGlobalMsrOahGroup->getDisplaySecondMsr ()) {
    displayMsrScore (
      secondMsrScore,
      "Display the second MSR as text");
  }

  if (gGlobalMsrOahGroup->getDisplaySecondMsrShort ()) {
    displayMsrScoreShort (
      secondMsrScore,
      "Display the second MSR as text");
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2gmnInsiderOahGroup->getQuitAfterPass3 ()) {
    err <<
      endl <<
      "Quitting after pass 3 as requested" <<
      endl;

    return mfMusicformatsError::k_NoError;
  }

  // convert the second MSR into an MXSR (pass 4)
  // ------------------------------------------------------

  Sxmlelement secondMxsr;

  try {
    secondMxsr =
      translateMsrToMxsr (
        secondMsrScore,
        gGlobalMsrOahGroup,
        "Pass 4",
        "Convert the second MSR into an MXSR",
        mfTimingItem::kMandatory);
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // convert the MXSR to Guido (pass 5)
  // ------------------------------------------------------

  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    translateMxsrToGuido (
      secondMxsr,
      outputFileName,
      err,
      "Pass 5",
      "Convert the MXSR into Guido text");
  }
  catch (mxsr2msrException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
static mfMusicformatsError xmlFile2guidoWithOptionsAndArguments (
  SXMLFile&               sxmlfile,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
	Sxmlelement st;

	if (sxmlfile) {
	  st = sxmlfile->elements ();

    if (st) {
      if (st->getName () == "score-timewise")
        return mfMusicformatsError::kErrorUnsupported;
    }
  }

	else {
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  err <<
    "xmlFile2guido(), sxmlfile is NULL" <<
    endl;
  }
#endif

    return mfMusicformatsError::kErrorInvalidFile;
	}

  // the service name
  // ------------------------------------------------------

  string serviceName = "xml2gmn";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

	// print the options and arguments
  // ------------------------------------------------------
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      handlerOptionsAndArguments;
  }
#endif

  // apply early options if any
  // ------------------------------------------------------

  gGlobalOahEarlyOptions.applyEarlyOptionsIfPresentInOptionsAndArguments (
    handlerOptionsAndArguments);

  // has the '-insider' option been used?
  // ------------------------------------------------------

  Bool insiderOption =
    gGlobalOahEarlyOptions.getEarlyInsiderOption ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      serviceName << " main()" <<
      ", insiderOption: " << insiderOption <<
      endl;
  }
#endif

  // create an xml2gmn insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2gmn OAH handler
  S_xml2gmnInsiderHandler
    insiderOahHandler =
      xml2gmnInsiderHandler::create (
        serviceName,
        serviceName + " insider OAH handler with options and arguments");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOption) {
    // use the insider xml2gmn OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular xml2gmn OAH handler
    handler =
      xml2gmnRegularHandler::create (
        serviceName,
        serviceName + " regular OAH handler",
        insiderOahHandler);
  }

  // create the global run data
  // ------------------------------------------------------

  gGlobalServiceRunData =
    mfServiceRunData::create (
      serviceName);

  // handle the command line options and arguments
  // ------------------------------------------------------

  try {
    // handle the options from the options and arguments
    oahElementHelpOnlyKind
      helpOnlyKind =
        handler->
          handleOptionsFromOptionsAndArguments (
            serviceName);

    // have help options been used?
    switch (helpOnlyKind) {
      case oahElementHelpOnlyKind::kElementHelpOnlyYes:
        return mfMusicformatsError::k_NoError; // quit now
        break;
      case oahElementHelpOnlyKind::kElementHelpOnlyNo:
        // go ahead
        break;
    } // switch
  }
  catch (mfOahException& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidOption;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return mfMusicformatsError::kErrorInvalidFile;
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### " <<
      serviceName <<
      " gIndenter value after options ands arguments checking: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // do the job
  // ------------------------------------------------------

  xmlFile2guidoWithHandler (
    sxmlfile,
    out,
    err,
    handler);

  return mfMusicformatsError::k_NoError;
}

//_______________________________________________________________________________
EXP mfMusicformatsError musicxmlFile2guido (
  const char*             fileName,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
	SXMLFile
	  sxmlfile =
	    createSXMLFileFromFile (
	      fileName,
	      "Pass 1",
	      "Create an MXSR reading a MusicXML file");

	if (sxmlfile) {
		return
		  xmlFile2guidoWithOptionsAndArguments (
		    sxmlfile,
		    handlerOptionsAndArguments,
		    out,
		    err);
	}

	return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMusicxmlFile2guidoWithHandler (
  const char*  fileName,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
	SXMLFile
	  sxmlfile =
	    createSXMLFileFromFile (
	      fileName,
	      "Pass 1",
	      "Create an MXSR reading a MusicXML file");

	if (sxmlfile) {
		return
		  xmlFile2guidoWithHandler (
		    sxmlfile,
		    out,
		    err,
		    handler);
	}

	return mfMusicformatsError::kErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsError musicxmlFd2guido (
  FILE*                   fd,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
	SXMLFile
	  sxmlfile =
	    createSXMLFileFromFd (
	      fd,
	      "Pass 1",
	      "Create an MXSR reading a MusicXML descriptor");

	if (sxmlfile) {
		return
		  xmlFile2guidoWithOptionsAndArguments (
		    sxmlfile,
		    handlerOptionsAndArguments,
		    out,
		    err);
	}

	return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMusicxmlFd2guidoWithHandler (
  FILE*        fd,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
	SXMLFile
	  sxmlfile =
	    createSXMLFileFromFd (
	      fd,
	      "Pass 1",
	      "Create an MXSR reading a MusicXML descriptor");

	if (sxmlfile) {
		return
		  xmlFile2guidoWithHandler (
		    sxmlfile,
		    out,
		    err,
		    handler);
	}

	return mfMusicformatsError::kErrorInvalidFile;
}

//_______________________________________________________________________________
EXP mfMusicformatsError musicxmlString2guido (
  const char*             buffer,
  oahOptionsAndArguments& handlerOptionsAndArguments,
  std::ostream&           out,
  std::ostream&           err)
{
	SXMLFile
	  sxmlfile =
	    createSXMLFileFromString (
	      buffer,
	      "Pass 1",
	      "Create an MXSR reading a MusicXML buffer");

	// call xmlFile2guido() even if sxmlfile is null,
	// to handle the help options if any
  return
    xmlFile2guidoWithOptionsAndArguments (
      sxmlfile,
      handlerOptionsAndArguments,
      out,
      err);

	return mfMusicformatsError::kErrorInvalidFile;
}

mfMusicformatsError convertMusicxmlString2guidoWithHandler (
  const char*  buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
	SXMLFile
	  sxmlfile =
	    createSXMLFileFromString (
	      buffer,
	      "Pass 1",
	      "Create an MXSR reading a MusicXML buffer");

	// call xmlFile2guido() even if sxmlfile is null,
	// to handle the help options if any
  return
    xmlFile2guidoWithHandler (
      sxmlfile,
      out,
      err,
      handler);

	return mfMusicformatsError::kErrorInvalidFile;
}


}
