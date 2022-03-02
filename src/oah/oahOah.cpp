/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...
#include <regex>

#include "visitor.h"

// #include "mfStringsHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "oahOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_insiderOahAtom insiderOahAtom::create (
  const string& longName,
  const string& shortName,
  const string& description)
{
  insiderOahAtom* o = new
    insiderOahAtom (
      longName,
      shortName,
      description);
  assert (o != nullptr);
  return o;
}

insiderOahAtom::insiderOahAtom (
  const string& longName,
  const string& shortName,
  const string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{}

insiderOahAtom::~insiderOahAtom ()
{}

void insiderOahAtom::applyElement (ostream& os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a insiderOahAtom" <<
      endl;
  }
#endif

  fOptionHasBeenSelected = true;

  // do nothing more, choosing the insider OAH handler has been done already
}

void insiderOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> insiderOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_insiderOahAtom>*
    p =
      dynamic_cast<visitor<S_insiderOahAtom>*> (v)) {
        S_insiderOahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching insiderOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void insiderOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> insiderOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_insiderOahAtom>*
    p =
      dynamic_cast<visitor<S_insiderOahAtom>*> (v)) {
        S_insiderOahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching insiderOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void insiderOahAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> insiderOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void insiderOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "insiderOahAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void insiderOahAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    "insider" <<  " : " <<
    "fOptionHasBeenSelected: " <<
    fOptionHasBeenSelected <<
    endl;
}

ostream& operator<< (ostream& os, const S_insiderOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_regularOahAtom regularOahAtom::create (
  const string& longName,
  const string& shortName,
  const string& description)
{
  regularOahAtom* o = new
    regularOahAtom (
      longName,
      shortName,
      description);
  assert (o != nullptr);
  return o;
}

regularOahAtom::regularOahAtom (
  const string& longName,
  const string& shortName,
  const string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{}

regularOahAtom::~regularOahAtom ()
{}

void regularOahAtom::applyElement (ostream& os)
{

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a regularOahAtom" <<
      endl;
  }
#endif

  fOptionHasBeenSelected = true;

  // do nothing more, choosing the regular OAH handler has been done already
}

void regularOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> regularOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_regularOahAtom>*
    p =
      dynamic_cast<visitor<S_regularOahAtom>*> (v)) {
        S_regularOahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching regularOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void regularOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> regularOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_regularOahAtom>*
    p =
      dynamic_cast<visitor<S_regularOahAtom>*> (v)) {
        S_regularOahAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching regularOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void regularOahAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> regularOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void regularOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "regularOahAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void regularOahAtom::printAtomWithVariableOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    "regular" <<  " : " <<
    "fOptionHasBeenSelected: " <<
    fOptionHasBeenSelected <<
    endl;
}

ostream& operator<< (ostream& os, const S_regularOahAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_oahOahGroup gGlobalOahOahGroup;

S_oahOahGroup oahOahGroup::create (
  const string& serviceName)
{
  oahOahGroup* o = new oahOahGroup (
    serviceName);
  assert (o != nullptr);

  return o;
}

oahOahGroup::oahOahGroup (
  const string& serviceName)
  : oahGroup (
      "Options and help group",
      "help-oah", "hoah",
  R"()",
      oahElementVisibilityKind::kElementVisibilityWhole),
    fOahOahGroupServiceName (
      serviceName)
{
  initializeOahOahGroup (serviceName);
}

oahOahGroup::~oahOahGroup ()
{}

void oahOahGroup::initializeOahBasicHelpOptions (
  const string& serviceName)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options help",
        "help-options-help", "hoh",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // the 'insider' option

  subGroup->
    appendAtomToSubGroup (
      insiderOahAtom::create (
        K_INSIDER_OPTION_LONG_NAME, K_INSIDER_OPTION_SHORT_NAME,
R"(Use the 'insider' mode for the options and help,
in which the options are grouped as they are used internally by MusicFormats.
In the 'regular' defaut mode, they are grouped by user-oriented topics,
such a slurs, tuplets and figured bass.)"));

//   // the 'regular' option
//
//   subGroup->
//     appendAtomToSubGroup (
//       regularOahAtom::create (
//         K_REGULAR_OPTION_LONG_NAME, K_REGULAR_OPTION_SHORT_NAME,
//   R"(Use the 'regular' mode for the options and help,
//   in which the options are grouped by topics
//   such a slurs, tuplets and figured bass.
//   This is the default.)"));

#ifdef TRACING_IS_ENABLED
  // trace early options

  fTraceEarlyOptionsAtom =
    oahBooleanAtom::create (
      K_TRACE_EARLY_OPTIONS_LONG_OPTION_NAME, K_TRACE_EARLY_OPTIONS_SHORT_OPTION_NAME,
R"(Trace the handling of early options, which happens
at the very beginning of options and arguments handling.
This option is handy to debug OAH.)",
      "fTraceEarlyOptions",
      fTraceEarlyOptions);

  subGroup->
    appendAtomToSubGroup (
      fTraceEarlyOptionsAtom);

  // OAH verbose mode

  fOahVerboseModeAtom =
    oahBooleanAtom::create (
     K_OAH_VERBOSE_MODE_LONG_OPTION_NAME, K_OAH_VERBOSE_MODE_SHORT_OPTION_NAME,
R"(Produce internal details about the context of options errors.
By default, only the mimimum information is output.
This option is handy to debug OAH.)",
      "fOahVerboseMode",
      fOahVerboseMode);

  subGroup->
    appendAtomToSubGroup (
      fOahVerboseModeAtom);
#endif

  // help

  subGroup->
    appendAtomToSubGroup (
      oahHelpAtom::create (
        "help", "h",
        regex_replace (
R"(Display EXECUTABLE_NAME's full help.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // help options usage

  subGroup->
    appendAtomToSubGroup (
      oahOptionsUsageAtom::create (
        "help-options-usage", "hou",
        regex_replace (
R"(Display EXECUTABLE_NAME's options usage help.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // help summary

  subGroup->
    appendAtomToSubGroup (
      oahHelpSummaryAtom::create (
        "help-summary", "hs",
        regex_replace (
R"(Display EXECUTABLE_NAME's help summary.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // about

  subGroup->
    appendAtomToSubGroup (
      oahAboutAtom::create (
        "about", "a",
        regex_replace (
R"(Display information about EXECUTABLE_NAME.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // version

  subGroup->
    appendAtomToSubGroup (
      oahVersionAtom::create (
        "version", "v",
        regex_replace (
R"(Display EXECUTABLE_NAME's version.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName,
        oahVersionKind::kOahVersionShort));

  subGroup->
    appendAtomToSubGroup (
      oahVersionAtom::create (
        "version-full", "vf",
        regex_replace (
R"(Display EXECUTABLE_NAME's version.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName,
        oahVersionKind::kOahVersionFull));

  // library version

  subGroup->
    appendAtomToSubGroup (
      oahLibraryVersionAtom::create (
        "mf-version", "mfv",
R"(Display MusicFormats's version.)",
        serviceName));

  // history

  subGroup->
    appendAtomToSubGroup (
      oahHistoryAtom::create (
        "history", "hist",
        regex_replace (
R"(Display EXECUTABLE_NAME's history.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // library history

  subGroup->
    appendAtomToSubGroup (
      oahLibraryHistoryAtom::create (
        "mf-history", "mfhist",
R"(Display MusicFormats's history.)",
        serviceName));

  // contact

  subGroup->
    appendAtomToSubGroup (
      oahContactAtom::create (
        "contact", "c",
        regex_replace (
R"(Display information about how to contact the maintainers of EXECUTABLE_NAME.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // display prefixes

  subGroup->
    appendAtomToSubGroup (
      oahDisplayPrefixes::create (
        "display-prefixes", "prefixes",
        regex_replace (
R"(Display the prefixes known to EXECUTABLE_NAME.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // display single character options

  subGroup->
    appendAtomToSubGroup (
      oahDisplaySingleCharacterOptions::create (
        "display-single-character-options", "single",
        regex_replace (
R"(Display the single character options known to EXECUTABLE_NAME.)",
          regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // help about option name

  string defaultOptionLongName = "name-help";

  fOptionNameToProvideHelpAbout = defaultOptionLongName;

  fOptionNameHelpAtom =
    oahOptionNameHelpAtom::create (
      defaultOptionLongName, "nh",
      regex_replace (
R"(Print help about OPTION_NAME.
OPTION_NAME is optional, and the default value is 'DEFAULT_VALUE'.)",
        regex ("DEFAULT_VALUE"),
        defaultOptionLongName),
      "OPTION_NAME",
      defaultOptionLongName, // better variable name JMI ???
      fOptionNameToProvideHelpAbout,
      defaultOptionLongName);

  subGroup->
    appendAtomToSubGroup (
      fOptionNameHelpAtom);

  // query about option name

  fQueryOptionNameAtom =
    oahQueryOptionNameAtom::create (
      "query", "", // "query-option-name" ??? JMI
R"(Print help about OPTION_NAME.)",
      "OPTION_NAME",
      serviceName);

  subGroup->
    appendAtomToSubGroup (
      fQueryOptionNameAtom);

  // find string

  fFindStringAtom =
    oahFindStringAtom::create (
      "find", "", // "find-string" ??? JMI
R"(Find string STRING in the help.
The search is case insensitive, and a '-' is added in front of options name for clarity.)",
      "STRING",
      serviceName);

  subGroup->
    appendAtomToSubGroup (
      fFindStringAtom);

  // include options file

  fIncludeOptionsAndArgumentsFileAtom =
    oahIncludeOptionsAndArgumentsFileAtom::create (
      K_INCLUDE_OPTION_LONG_NAME, K_INCLUDE_OPTION_SHORT_NAME,
      regex_replace (
R"(Include the options and arguments contained in FILENAME.
FILENAME is a string and should be a path to a text file.
Such a file is expected to hold at most one option or argument per line.
A '#' starts a comment that spans to the end of the line.
Comments and empty lines are ignored and can be used at will.
'-include, -inc' options may be used to include other files,
up to a maximum level of MAXIMUM_LEVEL.
This is handy to share often used options in groups, for example.)",
        regex ("MAXIMUM_LEVEL"),
        to_string (oahHandler::getIncludeFilesMaximumLevel ())),
      "FILENAME",
      serviceName);

  subGroup->
    appendAtomToSubGroup (
      fIncludeOptionsAndArgumentsFileAtom);

  // reverse names display order

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "reverse-names-display-order", "rndo",
R"(Write the short names before the long ones.
By default, the long names appear first for readability.
This options makes it easier to find short names in the help output.)",
        "fReverseNamesDisplayOrder",
        fReverseNamesDisplayOrder));

  // display the source code position

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-source-code-position", "dscp",
        regex_replace (
R"(Display the source code file name and line number
in warning and error messages.
This is useful when debugging EXECUTABLE.)",
          regex ("EXECUTABLE"),
          fOahOahGroupServiceName),
        "fDisplaySourceCodePositions",
        fDisplaySourceCodePositions));
}

void oahOahGroup::initializeOahOahAndArgumentsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options and arguments",
        "options-and-arguments", "oaa",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // options and arguments

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "show-options-and-arguments", "soaa",
        regex_replace (
R"(Print the options and arguments to EXECUTABLE.)",
          regex ("EXECUTABLE"),
          fOahOahGroupServiceName),
        "fShowOptionsAndArguments",
        fShowOptionsAndArguments));
}

#ifdef TRACING_IS_ENABLED
void oahOahGroup::initializeOahtracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options visitors trace",
        "help-trace-options-visitors", "htov",
R"()",
    oahElementVisibilityKind::kElementVisibilityHeaderOnly,
    this);

  appendSubGroupToGroup (subGroup);

  // trace oah visitors

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-oah-visitors", "toahvis",
R"(Write a trace of the OAH tree visiting activity to standard error.)",
        "fTracingOahVisitors",
        fTracingOahVisitors));
}
#endif

void oahOahGroup::initializeOahOahGroup (
  const string& serviceName)
{
  // help
  // --------------------------------------
  initializeOahBasicHelpOptions (serviceName);

  // options and arguments
  // --------------------------------------
  initializeOahOahAndArgumentsOptions ();

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeOahtracingOah ();
#endif
}

//______________________________________________________________________________
S_mfcMultiComponent oahOahGroup::fetchOahOahGroupHandlerMultiComponent () const
{
  return
    fHandlerUpLink->
      getHandlerMultiComponent ();
}

//______________________________________________________________________________
void oahOahGroup::enforceGroupQuietness ()
{
  // JMI
}

//______________________________________________________________________________
void oahOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void oahOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      "% ==> oahOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOahGroup>*
    p =
      dynamic_cast<visitor<S_oahOahGroup>*> (v)) {
        S_oahOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            "% ==> Launching oahOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      "% ==> oahOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOahGroup>*
    p =
      dynamic_cast<visitor<S_oahOahGroup>*> (v)) {
        S_oahOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            "% ==> Launching oahOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      "% ==> oahOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void oahOahGroup::printAtomWithVariableOptionsValues ( // JMIJMIJMI
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
  /* JMI
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    oahOptionalValuesStyleKindAsString (
      fOahOptionalValuesStyleKindVariable) <<
    "\"" <<
    endl <<
*/

    setw (valueFieldWidth) << "tracingOahVisitors" << " : " <<
    fTracingOahVisitors <<
    endl;
}

//______________________________________________________________________________
void oahOahGroup::printOahOahValues (int valueFieldWidth)
{
  gLogStream <<
    "The basic options are:" <<
    endl;

  ++gIndenter;

  // command line
  // --------------------------------------

  gLogStream << left <<
    setw (valueFieldWidth) << "Command line:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fShowOptionsAndArguments" << " : " <<
    fShowOptionsAndArguments <<
    endl;

  --gIndenter;

  // options and help display
  // --------------------------------------

  gLogStream << left <<
    setw (valueFieldWidth) << "Options trace and display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (valueFieldWidth) << "fReverseNamesDisplayOrder" << " : " <<
    fReverseNamesDisplayOrder <<
    endl <<

    setw (valueFieldWidth) << "fDisplaySourceCodePositions" << " : " <<
    fDisplaySourceCodePositions <<
    endl <<

    setw (valueFieldWidth) << "fTracingOahVisitors" << " : " <<
    fTracingOahVisitors <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_oahOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahOahGroup createGlobalOahOahGroup (
  const string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global global OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalOahOahGroup) {
    // create the global options group
    gGlobalOahOahGroup =
      oahOahGroup::create (
        serviceName);
    assert (gGlobalOahOahGroup != 0);
  }

  // return it
  return gGlobalOahOahGroup;
}


}
