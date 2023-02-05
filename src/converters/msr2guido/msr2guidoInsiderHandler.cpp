/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "oahWae.h"

#include "mfStaticSettings.h"

#include "mfInitialization.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "mfOutputFileOah.h"
#include "oahDisplayOah.h"

#include "mxsrOah.h"
#include "msrOah.h"
#include "msr2msrOah.h"
#include "msr2mxsrOah.h"

#include "oahEarlyOptions.h"

#include "msr2guidoComponent.h"

#include "msr2guidoManPageOah.h"

#include "msr2guidoInsiderHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msr2guidoInsiderHandler msr2guidoInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  msr2guidoInsiderHandler* o = new
    msr2guidoInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o!=0);

  return o;
}

msr2guidoInsiderHandler::msr2guidoInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
              Welcome to the MSR to Guido converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
R"(
Usage: msr2guido [[option]* [MusicXMLFile|-] [[option]*
)")
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the msr2guido prefixes
  createTheMsr2guidoPrefixes ();

  // create the msr2guido option groups
  createTheMsr2guidoOptionGroups (serviceName);
}

msr2guidoInsiderHandler::~msr2guidoInsiderHandler ()
{}

void msr2guidoInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMsr2guidoConverterComponent ();
}

std::string msr2guidoInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What msr2guido does:

    This multi-pass converter basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a first MusicXML tree;
        Pass 2a: converts that MusicXML tree into
                 a first Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the first MSR into a second MSR, to apply options;
        Pass 4:  converts the second MSR into a second MusicXML tree;
        Pass 5:  converts the second MusicXML tree to Guido code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void msr2guidoInsiderHandler::createTheMsr2guidoPrefixes ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating the msr2guido prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msr2guidoInsiderHandler::createTheMsr2guidoOptionGroups (
  const std::string& serviceName)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating the \"" <<
      fHandlerHeader <<
      "\" insider option groups" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // initialize options handling, phase 1
  // ------------------------------------------------------

  // create the OAH OAH group first
  appendGroupToHandler (
    createGlobalOahOahGroup (
      serviceName));

  // create the WAE OAH group
  appendGroupToHandler (
    createGlobalWaeOahGroup ());

#ifdef MF_TRACING_IS_ENABLED
  // create the tracing OAH group
  appendGroupToHandler (
    createGlobalTracingOahGroup (
      this));
#endif

  // create the output file OAH group
  appendGroupToHandler (
    createGlobalOutputFileOahGroup ());

  // create the display OAH group
  appendGroupToHandler (
    createGlobalDisplayOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  initializeMusicFormats ();

  initializeWAE ();

  initializeMSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the msr2guido OAH group
  appendGroupToHandler (
    createGlobalMsr2guidoOahGroup ());

  // create the MXSR OAH group
  appendGroupToHandler (
    createGlobalMxsrOahGroup ());

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2msr OAH group
  appendGroupToHandler (
    createGlobalMsr2msrOahGroup ());

  // create the msr2mxsr OAH group
  appendGroupToHandler (
    createGlobalMsr2mxsrOahGroup ());

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif
}

//______________________________________________________________________________
void msr2guidoInsiderHandler::checkOptionsAndArguments () const
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
std::string msr2guidoInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Fetching the output file name from the options in OAH handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  S_oahStringAtom
    outputFileNameStringAtom =
      gGlobalOutputFileOahGroup->
        getOutputFileNameStringAtom ();

  S_oahBooleanAtom
    autoOutputFileNameAtom =
      gGlobalOutputFileOahGroup->
        getAutoOutputFileNameAtom ();

  Bool
    outputFileNameHasBeenSet =
      outputFileNameStringAtom->
        getSelected ();

  Bool
    autoOutputFileNameHasBeenSet =
      autoOutputFileNameAtom->
        getSelected ();

  std::string outputFileName;

  if (outputFileNameHasBeenSet) {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has been selected
      // '-aofn, -auto-output-file-name' has been selected
      std::stringstream ss;

      ss <<
        "options' " <<
        outputFileNameStringAtom->fetchNames () <<
        "' and '" <<
        autoOutputFileNameAtom->fetchNames () <<
        "' cannot be selected simultaneously" <<
        "\")";

      oahError (ss.str ());
    }
    else {
      // '-o, -output-file-name' has been selected
      // '-aofn, -auto-output-file-name' has NOT been selected
      outputFileName =
        outputFileNameStringAtom->
          getStringVariable ();
    }
  }

  else {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has NOT been selected
      // '-aofn, -auto-output-file-name' has been selected
      std::string
        inputSourceName =
          gGlobalCurrentServiceRunData->getInputSourceName ();

      // determine output file base name
      if (inputSourceName == "-") {
        outputFileName = "stdin";
      }

      else {
        // determine output file name,
        outputFileName =
          mfBaseName (inputSourceName);

        size_t
          posInString =
            outputFileName.rfind ('.');

        // remove file extension
        if (posInString != std::string::npos) {
          outputFileName.replace (
            posInString,
            outputFileName.size () - posInString,
            "");
        }
      }

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        std::stringstream ss;

        ss <<
          "msr2guidoInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif

      // append the file extension to the output file name
       outputFileName += ".gmn";

#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        std::stringstream ss;

        ss <<
          "msr2guidoInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
          outputFileName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif
    }

    else {
      // '-o, -output-file-name' has NOT been selected
      // '-aofn, -auto-output-file-name' has NOT been selected
      // nothing to do
    }
  }

  return outputFileName;
}

//______________________________________________________________________________
void msr2guidoInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2guidoInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsr2guidoInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2mxsrOahGroup->
    enforceGroupQuietness ();

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED
  gGlobalHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void msr2guidoInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2guidoInsiderOahGroup::checkGroupOptionsConsistency ()
{
/* JMI

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) {
    std::stringstream ss;

    ss <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (ss.str ());
  }




  if (! fOutputFileName.size ()) {
    std::stringstream ss;

    ss <<
      "msr2guidoInsiderOahGroup: a MusicXML output file name must be selected with '-o, -output-file-name";

    oahError (ss.str ());
  }

  else if (fOutputFileName == gGlobalCurrentServiceRunData->getInputSourceName ()) {
    std::stringstream ss;

    ss <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (ss.str ());
  }
  */
}

//______________________________________________________________________________
void msr2guidoInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msr2guidoInsiderOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msr2guidoInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2guidoInsiderOahGroup>*> (v)) {
        S_msr2guidoInsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2guidoInsiderOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msr2guidoInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msr2guidoInsiderOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msr2guidoInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2guidoInsiderOahGroup>*> (v)) {
        S_msr2guidoInsiderOahGroup elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msr2guidoInsiderOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2guidoInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> msr2guidoInsiderOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msr2guidoInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msr2guidoInsiderHandler:" <<
    std::endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);
  os << std::endl;

  os <<
    "Options groups (" <<
    mfSingularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    std::endl;

  if (fHandlerGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msr2guidoInsiderHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msr2guidoInsiderOahGroup gGlobalMsr2guidoInsiderOahGroup;

S_msr2guidoInsiderOahGroup msr2guidoInsiderOahGroup::create ()
{
  msr2guidoInsiderOahGroup* o = new msr2guidoInsiderOahGroup ();
  assert (o!=0);

  return o;
}

msr2guidoInsiderOahGroup::msr2guidoInsiderOahGroup ()
  : oahGroup (
    "msr2guido",
    "hm2g", "help-msr2guido",
R"(Options that are used by msr2guido are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2guidoInsiderOahGroup ();
}

msr2guidoInsiderOahGroup::~msr2guidoInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msr2guidoInsiderOahGroup::initializeMsr2guidoInsiderOahGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  gLogStream << std::left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    std::endl;
  }
#endif
}

//______________________________________________________________________________
void msr2guidoInsiderOahGroup::printMsr2guidoInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The msr2guido options are:" <<
    std::endl;

  ++gIndenter;

  // Guido
  // --------------------------------------

  gLogStream << std::left <<
    std::setw (fieldWidth) << "Guido:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "generateComments" << ": " << fGenerateComments <<
    std::endl <<
    std::setw (fieldWidth) <<
    "generateStem" << ": " << fGenerateStem <<
    std::endl <<
    std::setw (fieldWidth) <<
    "generateBars" << ": " << fGenerateBars <<
    std::endl;

  --gIndenter;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fQuitAfterPass2a" << ": " <<
    fQuitAfterPass2a <<
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass2b" << ": " <<
    fQuitAfterPass2b <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_msr2guidoInsiderOahGroup createGlobalMsr2guidoOahGroup ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      "Creating global msr2guido OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2guidoInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsr2guidoInsiderOahGroup =
      msr2guidoInsiderOahGroup::create ();
    assert (gGlobalMsr2guidoInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2guidoInsiderOahGroup;
}


}
