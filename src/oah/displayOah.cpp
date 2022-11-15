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

#include "visitor.h"

#include "mfStringsHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "oahOah.h"

#include "displayOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_displayOahGroup gGlobalDisplayOahGroup;

S_displayOahGroup displayOahGroup::create ()
{
  displayOahGroup* o = new displayOahGroup ();
  assert (o != nullptr);
  return o;
}

displayOahGroup::displayOahGroup ()
  : oahGroup (
      "Display group",
      "help-display", "hdisp",
  R"()",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeDisplayOahGroup ();
}

displayOahGroup::~displayOahGroup ()
{}

void displayOahGroup::initializeDisplayOahOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "help-display-help", "hdh",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // display options and arguments

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options-and-arguments", "doaa",
R"(Write the options and arguments to standard error.)",
        "fDisplayOptionsAndArguments",
        fDisplayOptionsAndArguments));

  // display early options

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-early-options-values", "deov",
R"(Write the early options values to standard error.)",
        "fDisplayEarlyOptionsValues",
        fDisplayEarlyOptionsValues));

  // display options values

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options-values", "dov",
R"(Write the chosen options values to standard error.)",
        "fDisplayOptionsValues",
        fDisplayOptionsValues));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options-values-all", "dova",
R"(Write all the options values to standard error.)",
        "fDisplayOptionsValuesSelected",
        fDisplayOptionsValuesAll));

  // display options handler

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options-handler", "doh",
R"(Write the contents of the options handler to standard error.)",
        "fDisplayOahHandler",
        fDisplayOahHandler));

  // display options handler summary

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options-handler-summary", "dohs",
R"(Write a summary of the contents of the options handler to standard error.)",
        "fDisplayOahHandlerSummary",
        fDisplayOahHandlerSummary));

  // display options handler essentials

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options-handler-essentials", "dohe",
R"(Write the essentials of the contents of the options handler to standard error.)",
        "fDisplayOahHandlerEssentials",
        fDisplayOahHandlerEssentials));

  // CPU usage

  fDisplayCPUusageAtom =
    oahBooleanAtom::create (
      "display-cpu-usage", "cpu",
R"(Write information about CPU usage to standard error.)",
      "fDisplayCPUusage",
      fDisplayCPUusage);
  subGroup->
    appendAtomToSubGroup (
      fDisplayCPUusageAtom);
}

void displayOahGroup::initializeDisplayOahGroup ()
{
  // display
  // --------------------------------------
  initializeDisplayOahOptions ();
}

//______________________________________________________________________________
void displayOahGroup::enforceGroupQuietness ()
{
  // JMI
}

//______________________________________________________________________________
void displayOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void displayOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      "% ==> displayOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_displayOahGroup>*
    p =
      dynamic_cast<visitor<S_displayOahGroup>*> (v)) {
        S_displayOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            "% ==> Launching displayOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void displayOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      "% ==> displayOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_displayOahGroup>*
    p =
      dynamic_cast<visitor<S_displayOahGroup>*> (v)) {
        S_displayOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            "% ==> Launching displayOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void displayOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      "% ==> displayOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

void displayOahGroup::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int      valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) << "fDisplayOptionsAndArguments" << " : " <<
    fDisplayOptionsAndArguments <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayEarlyOptionsValues" << " : " <<
    fDisplayEarlyOptionsValues <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayOptionsValues" << " : " <<
    fDisplayOptionsValues <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOptionsValuesAll" << " : " <<
    fDisplayOptionsValuesAll <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayOahHandler" << " : " <<
    fDisplayOahHandler <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOahHandlerSummary" << " : " <<
    fDisplayOahHandlerSummary <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOahHandlerEssentials" << " : " <<
    fDisplayOahHandlerEssentials <<
    std::endl <<

    // CPU usage
    std::setw (valueFieldWidth) << "fDisplayCPUusage" << " : " <<
    fDisplayCPUusage <<
    std::endl;

  --gIndenter;
}

//______________________________________________________________________________
void displayOahGroup::printDisplayOptionsValues (int valueFieldWidth)
{
  gLogStream <<
    "The basic options are:" <<
    std::endl;

  ++gIndenter;

  // options and help display
  // --------------------------------------

  gLogStream << std::left <<
    std::setw (valueFieldWidth) << "Display:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (valueFieldWidth) << "fDisplayOptionsAndArguments" << " : " <<
    fDisplayOptionsAndArguments <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayEarlyOptionsValues" << " : " <<
    fDisplayEarlyOptionsValues <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayOptionsValues" << " : " <<
    fDisplayOptionsValues <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOptionsValuesAll" << " : " <<
    fDisplayOptionsValuesAll <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayOahHandler" << " : " <<
    fDisplayOahHandler <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOahHandlerSummary" << " : " <<
    fDisplayOahHandlerSummary <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOahHandlerEssentials" << " : " <<
    fDisplayOahHandlerEssentials <<
    std::endl <<

    // CPU usage
    std::setw (valueFieldWidth) << "fDisplayCPUusage" << " : " <<
    fDisplayCPUusage <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_displayOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_displayOahGroup createGlobalDisplayOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global global OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalDisplayOahGroup) {
    // create the global options group
    gGlobalDisplayOahGroup =
      displayOahGroup::create ();
    assert (gGlobalDisplayOahGroup != 0);
  }

  // return it
  return gGlobalDisplayOahGroup;
}


}
