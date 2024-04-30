/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

// WAE
#include "oahWae.h"

#include "mfStringsHandling.h"

// OAH
#include "oahOah.h"

// MSR
#include "msr.h"

// LPSR
#include "lpsr.h"

// MXSR
#include "msrOah.h"
#include "msr2msrOah.h"


#include "mfslInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "mfslInterpreterRegularHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfslInterpreterRegularHandler mfslInterpreterRegularHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader,
  S_mfslInterpreterInsiderHandler
                     insiderOahHandler)
{
  // create the regular handler
  mfslInterpreterRegularHandler* obj = new
    mfslInterpreterRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler);
  assert (obj != nullptr);

  return obj;
}

mfslInterpreterRegularHandler::mfslInterpreterRegularHandler (
  const std::string& serviceName,
  const std::string& handlerHeader,
  S_mfslInterpreterInsiderHandler
                     insiderOahHandler)
  : oahRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler)
{
  // this is done only only after the constructor has been executed,
  // because it uses pure virtual methods
  initializeOahRegularHandler ();

  // create the regular handler groups
  createRegularHandlerGroups ();

/* JMI
  // print the options handler initial state
  gLog <<
    "mfslInterpreterRegularHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    endl;

  ++gIndenter;

  gLog <<
    "===> printHelp():" <<
    endl;
  this->printHelp (gOutputStream); // JMI

  --gIndenter;
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    // JMI ???
  }
#endif // MF_TRACE_IS_ENABLED
}

mfslInterpreterRegularHandler::~mfslInterpreterRegularHandler ()
{}

void mfslInterpreterRegularHandler::createRegularHandlerGroups ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating the regular handler groups for \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  createInformationsRegularGroup ();

  createMfslRegularUserGroup ();
  createMfslRegularMaintainanceGroup ();

  createOahRegularGroup ();

  createWarningAndErrorsRegularGroup ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "All the regular handler groups for \"" <<
      fHandlerHeader <<
      "\" have been created" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void mfslInterpreterRegularHandler::createInformationsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Informations group",
        "help-informations-group", "hinfos-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Informations",
        "help-informations", "hinfos",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup ("about", subGroup);

  registerAtomInRegularSubgroup ("version", subGroup);
  registerAtomInRegularSubgroup ("version-full", subGroup);

  registerAtomInRegularSubgroup ("history", subGroup);

  registerAtomInRegularSubgroup ("mf-version", subGroup);
  registerAtomInRegularSubgroup ("mf-history", subGroup);

  registerAtomInRegularSubgroup ("contact", subGroup);

  registerAtomInRegularSubgroup ("music-formats-test", subGroup);
  registerAtomInRegularSubgroup ("display-prefixes", subGroup);
  registerAtomInRegularSubgroup ("display-single-character-options", subGroup);

  registerAtomInRegularSubgroup ("display-cpu-usage", subGroup);

  registerAtomInRegularSubgroup ("display-options-handler", subGroup);
  registerAtomInRegularSubgroup ("display-options-handler-summary", subGroup);
  registerAtomInRegularSubgroup ("display-options-handler-essentials", subGroup);
}

void mfslInterpreterRegularHandler::createMfslRegularUserGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "MFSL group",
        "help-mfsl-user-group", "hmfsl-user-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MFSL",
        "help-user", "hu",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup ("display-tokens", subGroup);

  registerAtomInRegularSubgroup ("display-service-and-input", subGroup);

  registerAtomInRegularSubgroup ("display-options", subGroup);

  registerAtomInRegularSubgroup ("input", subGroup);

  registerAtomInRegularSubgroup ("select", subGroup);

  registerAtomInRegularSubgroup ("no-launch", subGroup);
}

void mfslInterpreterRegularHandler::createMfslRegularMaintainanceGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "MFSL group",
        "help-mfsl-maintainance-group", "hmfsl--maintainance-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MFSL",
        "help-maintainer", "hm",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup ("trace-scanning", subGroup);
  registerAtomInRegularSubgroup ("trace-parsing", subGroup);

  registerAtomInRegularSubgroup ("trace-inputs", subGroup);
  registerAtomInRegularSubgroup ("trace-case-input-statements", subGroup);

  registerAtomInRegularSubgroup ("trace-choices", subGroup);
  registerAtomInRegularSubgroup ("trace-choice-statements", subGroup);
  registerAtomInRegularSubgroup ("trace-case-choice-statements", subGroup);

  registerAtomInRegularSubgroup ("trace-options-blocks", subGroup);
}

void mfslInterpreterRegularHandler::createOahRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Options and help group",
        "help-oah-group", "hoah-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options and help",
        "help-oah", "hoah",
        "",
        oahElementVisibilityKind::kElementVisibilityHeaderOnly,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup (K_INSIDER_OPTION_LONG_NAME, subGroup);
//  registerAtomInRegularSubgroup (K_REGULAR_OPTION_LONG_NAME, subGroup);

#ifdef MF_TRACE_IS_ENABLED
  registerAtomInRegularSubgroup (K_TRACE_EARLY_OPTIONS_OPTION_LONG_NAME, subGroup);
#endif // MF_TRACE_IS_ENABLED

  registerAtomInRegularSubgroup ("help", subGroup);
  registerAtomInRegularSubgroup ("help-summary", subGroup);

  registerAtomInRegularSubgroup ("help-options-usage", subGroup);

  registerAtomInRegularSubgroup ("name-help", subGroup);
  registerAtomInRegularSubgroup ("query", subGroup);
  registerAtomInRegularSubgroup ("find", subGroup);
  registerAtomInRegularSubgroup ("filter", subGroup);
  registerAtomInRegularSubgroup (K_INCLUDE_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup ("reverse-names-display-order", subGroup);

  registerAtomInRegularSubgroup ("display-options-and-arguments", subGroup);
  registerAtomInRegularSubgroup ("display-early-options-values", subGroup);
  registerAtomInRegularSubgroup ("display-options-values", subGroup);
  registerAtomInRegularSubgroup ("display-options-values-full", subGroup);
  registerAtomInRegularSubgroup ("show-options-and-arguments", subGroup);


#ifdef MF_TRACE_IS_ENABLED
  registerAtomInRegularSubgroup (K_TRACE_OAH_OPTION_LONG_NAME, subGroup);
  registerAtomInRegularSubgroup (K_TRACE_OAH_DETAILS_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup (K_TRACE_PASSES_OPTION_LONG_NAME, subGroup);
#endif // MF_TRACE_IS_ENABLED
}

void mfslInterpreterRegularHandler::createWarningAndErrorsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Warning and errors group",
        "warning-and-errors-group",
        "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Warning and errors",
        "warning-and-errors",
        "",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup (K_QUIET_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup ("dont-show-errors", subGroup);
  registerAtomInRegularSubgroup ("dont-quit-on-errors", subGroup);

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  registerAtomInRegularSubgroup ("maintainance-run", subGroup);
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
}

//______________________________________________________________________________
void mfslInterpreterRegularHandler::checkOptionsAndArguments () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

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
void mfslInterpreterRegularHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "mfslInterpreterRegularHandler '" << fHandlerHeader << "':" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);
  os << endl;

  os <<
    "Options groups (" <<
    mfSingularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

std::ostream& operator<< (std::ostream& os, const S_mfslInterpreterRegularHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}


}
