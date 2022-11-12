/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

// libmusicxml2
#include "visitor.h"

#include "mfStringsHandling.h"

// WAE
#include "oahWae.h"

#include "enableHarmoniesExtraOahIfDesired.h"
#ifdef EXTRA_OAH_IS_ENABLED
  #include "harmoniesExtraOah.h"
#endif

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "msrInitialization.h"
#include "lpsrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "outputFileOah.h"
#include "displayOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "oahEarlyOptions.h"

#include "msr2lilypondComponent.h"

#include "msr2lilypondInsiderHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msr2lilypondInsiderHandler msr2lilypondInsiderHandler::create (
  const std::string&           serviceName,
  const std::string&           handlerHeader,
  oahHandlerUsedThruKind  handlerUsedThruKind)
{
  // create the insider handler
  msr2lilypondInsiderHandler* o = new
    msr2lilypondInsiderHandler (
      serviceName,
      handlerHeader,
      handlerUsedThruKind);
  assert (o!=0);

  return o;
}

msr2lilypondInsiderHandler::msr2lilypondInsiderHandler (
  const std::string&           serviceName,
  const std::string&           handlerHeader,
  oahHandlerUsedThruKind  handlerUsedThruKind)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
            Welcome to the MSR to LilyPond converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
/* JMI
R"(
Usage: msr2lilypond [[option]* [MusicXMLFile|-] [[option]*
)"
*/
      usageFromUsedThruKind (handlerUsedThruKind))
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing msr2lilypond insider options handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the msr2lilypond prefixes
  createTheMsr2lilypondPrefixes ();

  // create the xml2xml option groups
  createTheMsr2lilypondOptionGroups (
    serviceName);
}

msr2lilypondInsiderHandler::~msr2lilypondInsiderHandler ()
{}

void msr2lilypondInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMsr2lilypondConverterComponent ();
}

std::string msr2lilypondInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What msr2lilypond does:

    This multi-pass converter basically performs 5 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML tree into
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                 to get a full MSR;
        Pass 3:  converts the MSR into a
                 LilyPond Score Representation (LPSR);
        Pass 4:  converts the LPSR to LilyPond code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
std::string msr2lilypondInsiderHandler::usageFromUsedThruKind (
   oahHandlerUsedThruKind handlerUsedThruKind) const
{
  std::string result;

  switch (handlerUsedThruKind) {
    case oahHandlerUsedThruKind::kHandlerUsedThruUnknown:
      {
        std::stringstream s;

        s <<
          "oahHandlerUsedThruKind::kHandlerUsedThruUnknown found in usageFromUsedThruKind() in handler \"" <<
          fHandlerHeader <<
          "\"";

        oahInternalError (s.str ());
      }
      break;
    case oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv:
      result = fHandlerServiceName + " [[option]* [MusicXMLFile|-] [[option]*"; // JMI ???
      break;
    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      result = "Usage: [options]";
      break;
  } // switch

  result = "Usage: " + result;

  return result;
}

//______________________________________________________________________________
void msr2lilypondInsiderHandler::createTheMsr2lilypondPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the msr2lilypond prefixes" <<
      std::endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msr2lilypondInsiderHandler::createTheMsr2lilypondOptionGroups (
  const std::string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the msr2lilypond insider option groups" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
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

#ifdef TRACING_IS_ENABLED
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

  initializeMSR ();
  initializeLPSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the msr2lilypond OAH group
  appendGroupToHandler (
    createGlobalMsr2lilypondInsiderOahGroup ());

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2lpsr OAH group
  appendGroupToHandler (
    createGlobalMsr2lpsrOahGroup ());

  // create the LPSR OAH group
  appendGroupToHandler (
    createGlobalLpsrOahGroup ());

  // create the LilyPond generation OAH group
  appendGroupToHandler (
    createGlobalLpsr2lilypondOahGroup ());

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    // print the options handler initial state
    gLogStream <<
      "msr2lilypondInsiderHandler has been initialized as:" <<
      std::endl;

    ++gIndenter;

    print (gLogStream);

    gLogStream <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif

#ifdef TRACING_IS_ENABLED
if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "msr2lilypondInsiderHandler help:" <<
      std::endl;

    this->printHelp (gOutputStream);
  }
#endif
}

//______________________________________________________________________________
void msr2lilypondInsiderHandler::checkOptionsAndArguments () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
std::string msr2lilypondInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Fetching the output file name from the options in OAH handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
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
        getSetByAnOption ();

  Bool
    autoOutputFileNameHasBeenSet =
      autoOutputFileNameAtom->
        getSetByAnOption ();

  std::string outputFileName;

  if (outputFileNameHasBeenSet) {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has been chosen
      // '-aofn, -auto-output-file-name' has been chosen
      std::stringstream s;

      s <<
        "options' " <<
        outputFileNameStringAtom->fetchNames () <<
        "' and '" <<
        autoOutputFileNameAtom->fetchNames () <<
        "' cannot be chosen simultaneously" <<
        "\")";

      oahError (s.str ());
    }
    else {
      // '-o, -output-file-name' has been chosen
      // '-aofn, -auto-output-file-name' has NOT been chosen
      outputFileName =
        outputFileNameStringAtom->
          getStringVariable ();
    }
  }

  else {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has NOT been chosen
      // '-aofn, -auto-output-file-name' has been chosen
      std::string
        inputSourceName =
          gGlobalServiceRunData->getInputSourceName ();

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

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream <<
          "msr2lilypondInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 1 = \"" <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif

      // append the file extension to the output file name
       outputFileName += ".ly";

#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream <<
          "msr2lilypondInsiderHandler::fetchOutputFileNameFromTheOptions(): outputFileName 2 = " <<
          outputFileName <<
          "\"" <<
          std::endl;
      }
#endif
    }

    else {
      // '-o, -output-file-name' has NOT been chosen
      // '-aofn, -auto-output-file-name' has NOT been chosen
      // nothing to do
    }
  }

  return outputFileName;
}

//______________________________________________________________________________
void msr2lilypondInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2lilypondInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsr2lilypondInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2lpsrOahGroup->
    enforceGroupQuietness ();

  gGlobalLpsrOahGroup->
    enforceGroupQuietness ();

  gGlobalLpsr2lilypondOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void msr2lilypondInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2lilypondInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2lilypondInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lilypondInsiderOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2lilypondInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2lilypondInsiderOahGroup>*> (v)) {
        S_msr2lilypondInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lilypondInsiderOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2lilypondInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lilypondInsiderOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_msr2lilypondInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2lilypondInsiderOahGroup>*> (v)) {
        S_msr2lilypondInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2lilypondInsiderOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2lilypondInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2lilypondInsiderOahGroup::browseData ()" <<
      std::endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msr2lilypondInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msr2lilypondInsiderHandler '" << fHandlerHeader << "':" <<
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
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msr2lilypondInsiderHandler& elt)
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
S_msr2lilypondInsiderOahGroup gGlobalMsr2lilypondInsiderOahGroup;

S_msr2lilypondInsiderOahGroup msr2lilypondInsiderOahGroup::create ()
{
  msr2lilypondInsiderOahGroup* o = new msr2lilypondInsiderOahGroup ();
  assert (o!=0);

  return o;
}

msr2lilypondInsiderOahGroup::msr2lilypondInsiderOahGroup ()
  : oahGroup (
    "msr2lilypond",
    "hx2l", "help-msr2lilypond",
R"(Options that are used by msr2lilypond are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsr2lilypondInsiderOahGroup ();
}

msr2lilypondInsiderOahGroup::~msr2lilypondInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msr2lilypondInsiderOahGroup::initializeMsr2lilypondInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
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
void msr2lilypondInsiderOahGroup::printMsr2lilypondInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The msr2lilypond options are:" <<
    std::endl;

  ++gIndenter;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fQuitAfterPass2a" << " : " <<
    fQuitAfterPass2a <<
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass2b" << " : " <<
    fQuitAfterPass2b <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_msr2lilypondInsiderOahGroup createGlobalMsr2lilypondInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2lilypond OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2lilypondInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsr2lilypondInsiderOahGroup =
      msr2lilypondInsiderOahGroup::create ();
    assert (gGlobalMsr2lilypondInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2lilypondInsiderOahGroup;
}


}
