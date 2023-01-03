/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

// libmusicxml2
#include "visitor.h"

// WAE
#include "oahWae.h"

#include "mfEnableHarmoniesExtra.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

// OAH
#include "oahOah.h"
#include "waeOah.h"
#include "oahDisplayOah.h"

// early options
#include "oahEarlyOptions.h"

// components
#include "mfslInterpreterComponent.h"

#include "mfslInterpreterInterface.h"

#include "mfslInterpreterInsiderHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfslInterpreterInsiderHandler mfslInterpreterInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  mfslInterpreterInsiderHandler* o = new
    mfslInterpreterInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o != nullptr);
  return o;
}

mfslInterpreterInsiderHandler::mfslInterpreterInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                   Welcome to the MFSL interpreter,
     a generator of Guido, MusicXML, LilyPond or Braille
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
      usageInformation ())
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler" <<
      std::endl;
  }
#endif

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the mfsl prefixes
  createTheMfslInterpreterPrefixes ();

  // create the mfsl option groups
  createTheMfslInterpreterOptionGroups (
    serviceName);
}

mfslInterpreterInsiderHandler::~mfslInterpreterInsiderHandler ()
{}

void mfslInterpreterInsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createMfslInterpreterComponent ();
}

std::string mfslInterpreterInsiderHandler::usageInformation ()
{
  std::stringstream s;

  s <<
R"(Usage: mfsl [option]*
)" <<
    std::endl;

  return s.str ();
}

std::string mfslInterpreterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    mfslInterpreterAboutInformation ();
}

std::string mfslInterpreterInsiderHandler::mfslInterpreterAboutInformation () const
{
  std::string result =
R"(What mfsl does:

    This interpreter reads text input containing
    a tool name, an input source name, keywords and options,
    and launches the specified tool
    with these options applied to the input source name.

    The input can come from a file or from standard input,
    allowing the interpreter to be used interactively or in shell pipes.

    The activity log and warning/error messages go to standard error.)";

  return
    result;
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::createTheMfslInterpreterPrefixes ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the mfsl prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::createTheMfslInterpreterOptionGroups (
  const std::string& serviceName)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the \"" <<
      fHandlerHeader <<
      "\" insider option groups" <<
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

#ifdef OAH_TRACING_IS_ENABLED
  // create the tracing OAH group
  appendGroupToHandler (
    createGlobalTracingOahGroup (
      this));
#endif

  // create the display OAH group
  appendGroupToHandler (
    createGlobalDisplayOahGroup ());

  // initialize the library
  // ------------------------------------------------------

// JMI

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the mfsl OAH group
  appendGroupToHandler (
    createGlobalMfslInterpreterOahGroup ());

  // create the mfsl insider OAH group
  appendGroupToHandler (
    createGlobalMfslInterpreterInsiderOahGroup ());
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::checkOptionsAndArguments () const
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  if (MSDR_STANDARD_INPUT_NAME == std::string ("-")) {
    checkSingleInputSourceInArgumentsVector ();
  }
  else if (MSDR_STANDARD_INPUT_NAME == std::string ("")) {
    checkNoOrOneInputSourceInArgumentsVector ();
  }
  else {
    oahInternalError (
      "MSDR_STANDARD_INPUT_NAME should be empty or contain a single '-'");
  }
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::enforceHandlerQuietness ()
{
#ifdef OAH_TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMfslInterpreterInsiderOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::checkGroupOptionsConsistency ()
{
/* JMI

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) {
    std::stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (s.str ());
  }




  if (! fOutputFileName.size ()) {
    std::stringstream s;

    s <<
      "mfslInterpreterInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

    oahError (s.str ());
  }

  else if (fOutputFileName == gGlobalServiceRunData->getInputSourceName ()) {
    std::stringstream s;

    s <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (s.str ());
  }
  */
}

//______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterInsiderOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_mfslInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterInsiderOahGroup>*> (v)) {
        S_mfslInterpreterInsiderOahGroup elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfslInterpreterInsiderOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mfslInterpreterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterInsiderOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_mfslInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterInsiderOahGroup>*> (v)) {
        S_mfslInterpreterInsiderOahGroup elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfslInterpreterInsiderOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mfslInterpreterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterInsiderOahGroup::browseData ()" <<
      std::endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "mfslInterpreterInsiderHandler:" <<
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

std::ostream& operator << (std::ostream& os, const S_mfslInterpreterInsiderHandler& elt)
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
S_mfslInterpreterInsiderOahGroup gGlobalMfslInterpreterInsiderOahGroup;

S_mfslInterpreterInsiderOahGroup mfslInterpreterInsiderOahGroup::create ()
{
  mfslInterpreterInsiderOahGroup* o = new mfslInterpreterInsiderOahGroup ();
  assert (o != nullptr);
  return o;
}

mfslInterpreterInsiderOahGroup::mfslInterpreterInsiderOahGroup ()
  : oahGroup (
    "mfsl",
    "help-mfsl-group", "hmfslg",
R"(Options that are used by mfsl are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMfslInterpreterInsiderOahGroup ();
}

mfslInterpreterInsiderOahGroup::~mfslInterpreterInsiderOahGroup ()
{}

//_______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::initializeMfslInterpreterInsiderOahGroup ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream << std::left <<
      "Initializing \"" <<
      fGroupHeader <<
      "\" group" <<
      std::endl;
  }
#endif

  // JMI ???
}

//______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::printMfslInterpreterInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The mfsl options are:" <<
    std::endl;

  ++gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_mfslInterpreterInsiderOahGroup createGlobalMfslInterpreterInsiderOahGroup ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global mfsl insider OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMfslInterpreterInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMfslInterpreterInsiderOahGroup =
      mfslInterpreterInsiderOahGroup::create ();
    assert (gGlobalMfslInterpreterInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMfslInterpreterInsiderOahGroup;
}


}
