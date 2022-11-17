/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

// WAE
#include "oahWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

// OAH
#include "oahOah.h"

// MSR
// LPSR
// MXSR
#include "msrOah.h"
#include "msr2msrOah.h"


#include "mfslInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "mfslInterpreterRegularHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_mfslInterpreterRegularHandler mfslInterpreterRegularHandler::create (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  const S_mfslInterpreterInsiderHandler&
                    insiderOahHandler)
{
  // create the regular handler
  mfslInterpreterRegularHandler* o = new
    mfslInterpreterRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler);
  assert (o != nullptr);
  return o;
}

mfslInterpreterRegularHandler::mfslInterpreterRegularHandler (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  const S_mfslInterpreterInsiderHandler&
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
  gLogStream <<
    "mfslInterpreterRegularHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    std::endl;

  ++gIndenter;

  gLogStream <<
    "===> printHelp():" <<
    std::endl;
  this->printHelp (gOutputStream); // JMI

  --gIndenter;
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    // JMI ???
  }
#endif
}

mfslInterpreterRegularHandler::~mfslInterpreterRegularHandler ()
{}

void mfslInterpreterRegularHandler::createRegularHandlerGroups ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the regular handler groups for \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;
  }
#endif

  createInformationsRegularGroup ();

  createMfslRegularUserGroup ();
  createMfslRegularMaintainanceGroup ();

  createOahRegularGroup ();

  createWarningAndErrorsRegularGroup ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "All the regular handler groups for \"" <<
      fHandlerHeader <<
      "\" have been created" <<
      std::endl;
  }
#endif
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

  registerAtomInRegularSubgroup ("display-tool-and-input", subGroup);

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

#ifdef TRACING_IS_ENABLED
  registerAtomInRegularSubgroup (K_TRACE_EARLY_OPTIONS_LONG_OPTION_NAME, subGroup);
#endif

  registerAtomInRegularSubgroup ("help", subGroup);
  registerAtomInRegularSubgroup ("help-summary", subGroup);

  registerAtomInRegularSubgroup ("help-options-usage", subGroup);

  registerAtomInRegularSubgroup ("name-help", subGroup);
  registerAtomInRegularSubgroup ("query", subGroup);
  registerAtomInRegularSubgroup ("find", subGroup);
  registerAtomInRegularSubgroup (K_INCLUDE_OPTION_LONG_NAME, subGroup);

  registerAtomInRegularSubgroup ("reverse-names-display-order", subGroup);

  registerAtomInRegularSubgroup ("display-options-and-arguments", subGroup);
  registerAtomInRegularSubgroup ("display-early-options-values", subGroup);
  registerAtomInRegularSubgroup ("display-options-values", subGroup);
  registerAtomInRegularSubgroup ("display-options-values-all", subGroup);
  registerAtomInRegularSubgroup ("show-options-and-arguments", subGroup);


#ifdef TRACING_IS_ENABLED
  registerAtomInRegularSubgroup (K_TRACE_OAH_LONG_OPTION_NAME, subGroup);
  registerAtomInRegularSubgroup (K_TRACE_OAH_DETAILS_LONG_OPTION_NAME, subGroup);

  registerAtomInRegularSubgroup (K_TRACE_PASSES_LONG_OPTION_NAME, subGroup);
#endif
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
}

//______________________________________________________________________________
void mfslInterpreterRegularHandler::checkOptionsAndArguments () const
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

std::ostream& operator << (std::ostream& os, const S_mfslInterpreterRegularHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
