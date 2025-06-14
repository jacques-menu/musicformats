/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <regex>

#include "visitor.h"

#include "mfStringsHandling.h"

#include "mfPreprocessorSettings.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"

#include "oahDisplayOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_displayOahGroup gGlobalDisplayOahGroup;

S_displayOahGroup displayOahGroup::create ()
{
  displayOahGroup* obj = new displayOahGroup ();
  assert (obj != nullptr);
  return obj;
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
R"(Write the selected options values to standard error.)",
        "fDisplayOptionsValues",
        fDisplayOptionsValues));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options-values-full", "dovf",
R"(Write the full set of options values to standard error.)",
        "fDisplayOptionsValuesFull",
        fDisplayOptionsValuesFull));

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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> displayOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_displayOahGroup>*
    p =
      dynamic_cast<visitor<S_displayOahGroup>*> (v)) {
        S_displayOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching displayOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void displayOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> displayOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_displayOahGroup>*
    p =
      dynamic_cast<visitor<S_displayOahGroup>*> (v)) {
        S_displayOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching displayOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void displayOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> displayOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void displayOahGroup::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) << "fDisplayOptionsAndArguments" << ": " <<
    fDisplayOptionsAndArguments <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayEarlyOptionsValues" << ": " <<
    fDisplayEarlyOptionsValues <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayOptionsValues" << ": " <<
    fDisplayOptionsValues <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOptionsValuesFull" << ": " <<
    fDisplayOptionsValuesFull <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayOahHandler" << ": " <<
    fDisplayOahHandler <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOahHandlerSummary" << ": " <<
    fDisplayOahHandlerSummary <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOahHandlerEssentials" << ": " <<
    fDisplayOahHandlerEssentials <<
    std::endl <<

    // CPU usage
    std::setw (valueFieldWidth) << "fDisplayCPUusage" << ": " <<
    fDisplayCPUusage <<
    std::endl;
}

//______________________________________________________________________________
void displayOahGroup::displayDisplayOptionsValues (int valueFieldWidth)
{
  gLog <<
    "The basic options are:" <<
    std::endl;

  ++gIndenter;

  // options and help display
  // --------------------------------------

  gLog << std::left <<
    std::setw (valueFieldWidth) << "Display:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fDisplayOptionsAndArguments" << ": " <<
    fDisplayOptionsAndArguments <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayEarlyOptionsValues" << ": " <<
    fDisplayEarlyOptionsValues <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayOptionsValues" << ": " <<
    fDisplayOptionsValues <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOptionsValuesFull" << ": " <<
    fDisplayOptionsValuesFull <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplayOahHandler" << ": " <<
    fDisplayOahHandler <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOahHandlerSummary" << ": " <<
    fDisplayOahHandlerSummary <<
    std::endl <<
    std::setw (valueFieldWidth) << "fDisplayOahHandlerEssentials" << ": " <<
    fDisplayOahHandlerEssentials <<
    std::endl <<

    // CPU usage
    std::setw (valueFieldWidth) << "fDisplayCPUusage" << ": " <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_displayOahGroup createGlobalDisplayOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global global OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
