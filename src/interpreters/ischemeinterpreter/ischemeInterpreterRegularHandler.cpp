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

#include "enableTracingIfDesired.h"
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


#include "ischemeInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "ischemeInterpreterRegularHandler.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_ischemeInterpreterRegularHandler ischemeInterpreterRegularHandler::create (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  S_ischemeInterpreterInsiderHandler
                    insiderOahHandler)
{
  // create the regular handler
  ischemeInterpreterRegularHandler* o = new
    ischemeInterpreterRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler);
  assert (o != nullptr);
  return o;
}

ischemeInterpreterRegularHandler::ischemeInterpreterRegularHandler (
  const std::string&     serviceName,
  const std::string&     handlerHeader,
  S_ischemeInterpreterInsiderHandler
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
    "ischemeInterpreterRegularHandler \"" <<
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

ischemeInterpreterRegularHandler::~ischemeInterpreterRegularHandler ()
{}

void ischemeInterpreterRegularHandler::createRegularHandlerGroups ()
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

  createIschemeRegularUserGroup ();
  createIschemeRegularMaintainanceGroup ();

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

void ischemeInterpreterRegularHandler::createInformationsRegularGroup ()
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

void ischemeInterpreterRegularHandler::createIschemeRegularUserGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "iScheme group",
        "help-ischeme-user-group", "hischeme-user-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "iScheme",
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

void ischemeInterpreterRegularHandler::createIschemeRegularMaintainanceGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "iScheme group",
        "help-ischeme-maintainance-group", "hischeme--maintainance-group",
        "",
        oahElementVisibilityKind::kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "iScheme",
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

void ischemeInterpreterRegularHandler::createOahRegularGroup ()
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

void ischemeInterpreterRegularHandler::createWarningAndErrorsRegularGroup ()
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
void ischemeInterpreterRegularHandler::checkOptionsAndArguments () const
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
void ischemeInterpreterRegularHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "ischemeInterpreterRegularHandler '" << fHandlerHeader << "':" <<
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

std::ostream& operator << (std::ostream& os, const S_ischemeInterpreterRegularHandler& elt)
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
