/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <regex>

#include "visitor.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfConstants.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_languageOahAtom languageOahAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  mfLanguageKind&    languageKindVariable)
{
  languageOahAtom* o = new
    languageOahAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      languageKindVariable);
  assert (o != nullptr);
  return o;
}

languageOahAtom::languageOahAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  mfLanguageKind&    languageKindVariable)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName)
//       ,
//     fLanguageKindVariable (
//       languageKindVariable)
{}

languageOahAtom::~languageOahAtom ()
{}

void languageOahAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a languageOahAtom" <<
      std::endl;
  }
#endif

//   fLanguageKindVariable = mfLanguageKindFromString (theString);
//
//   gGlobalOahEarlyOptions.setEarlyLanguageKind (
//     mfLanguageKindFromString (theString));

  fOptionHasBeenSelected = true;

  // do nothing more, choosing the insider OAH handler has been done already
}

void languageOahAtom::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> languageOahAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_languageOahAtom>*
    p =
      dynamic_cast<visitor<S_languageOahAtom>*> (v)) {
        S_languageOahAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching languageOahAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void languageOahAtom::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> languageOahAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_languageOahAtom>*
    p =
      dynamic_cast<visitor<S_languageOahAtom>*> (v)) {
        S_languageOahAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching languageOahAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void languageOahAtom::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> languageOahAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void languageOahAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "languageOahAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void languageOahAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    "language" <<  ": " <<
    "fOptionHasBeenSelected: " <<
    fOptionHasBeenSelected <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_languageOahAtom& elt)
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
S_insiderOahAtom insiderOahAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
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
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{}

insiderOahAtom::~insiderOahAtom ()
{}

void insiderOahAtom::applyElement (std::ostream& os)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a insiderOahAtom" <<
      std::endl;
  }
#endif

  fOptionHasBeenSelected = true;

  // do nothing more, choosing the insider OAH handler has been done already
}

void insiderOahAtom::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> insiderOahAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_insiderOahAtom>*
    p =
      dynamic_cast<visitor<S_insiderOahAtom>*> (v)) {
        S_insiderOahAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching insiderOahAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void insiderOahAtom::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> insiderOahAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_insiderOahAtom>*
    p =
      dynamic_cast<visitor<S_insiderOahAtom>*> (v)) {
        S_insiderOahAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching insiderOahAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void insiderOahAtom::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> insiderOahAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void insiderOahAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "insiderOahAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void insiderOahAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    "insider" <<  ": " <<
    "fOptionHasBeenSelected: " <<
    fOptionHasBeenSelected <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_insiderOahAtom& elt)
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
S_regularOahAtom regularOahAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
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
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahAtom (
      longName,
      shortName,
      description,
      oahElementValueKind::kElementValueWithout)
{}

regularOahAtom::~regularOahAtom ()
{}

void regularOahAtom::applyElement (std::ostream& os)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a regularOahAtom" <<
      std::endl;
  }
#endif

  fOptionHasBeenSelected = true;

  // do nothing more, choosing the regular OAH handler has been done already
}

void regularOahAtom::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> regularOahAtom::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_regularOahAtom>*
    p =
      dynamic_cast<visitor<S_regularOahAtom>*> (v)) {
        S_regularOahAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching regularOahAtom::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void regularOahAtom::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> regularOahAtom::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_regularOahAtom>*
    p =
      dynamic_cast<visitor<S_regularOahAtom>*> (v)) {
        S_regularOahAtom elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching regularOahAtom::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void regularOahAtom::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> regularOahAtom::browseData ()" <<
      std::endl;
  }
#endif
}

void regularOahAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "regularOahAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void regularOahAtom::printAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    "regular" <<  ": " <<
    "fOptionHasBeenSelected: " <<
    fOptionHasBeenSelected <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_regularOahAtom& elt)
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
S_oahOahGroup gGlobalOahOahGroup;

S_oahOahGroup oahOahGroup::create (
  const std::string& serviceName)
{
  oahOahGroup* o = new oahOahGroup (
    serviceName);
  assert (o != nullptr);
  return o;
}

oahOahGroup::oahOahGroup (
  const std::string& serviceName)
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
  const std::string& serviceName)
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

  // the 'language' option

  mfLanguageKind
    mfLanguageKindDefaultValue =
      mfLanguageKind::kMusicFormatsLanguageEnglish; // default value

  subGroup->
    appendAtomToSubGroup (
      languageOahAtom::create (
        K_LANGUAGE_OPTION_LONG_NAME, K_LANGUAGE_OPTION_SHORT_NAME,
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE for the basic user interaction and passes trace.
The options names and descriptions are in english only,
as well as the tracing messages, meant for developers and maintainers.
The NUMBER languages available are:
LANGUAGE_KINDS.
The default is 'DEFAULT_VALUE'.)",
            std::regex ("NUMBER"),
            std::to_string (gGlobalMusicFormatsLanguageKindsMap.size ())),
          std::regex ("LANGUAGE_KINDS"),
          existingMusicFormatsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
        std::regex ("DEFAULT_VALUE"),
        mfLanguageKindAsString (
          mfLanguageKindDefaultValue)),
      "LANGUAGE",
      "fLanguageKind",
      fLanguageKind));

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

#ifdef OAH_TRACING_IS_ENABLED
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
          std::regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // help options usage

  subGroup->
    appendAtomToSubGroup (
      oahOptionsUsageAtom::create (
        "help-options-usage", "hou",
        regex_replace (
R"(Display EXECUTABLE_NAME's options usage help.)",
          std::regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // help summary

  subGroup->
    appendAtomToSubGroup (
      oahHelpSummaryAtom::create (
        "help-summary", "hs",
        regex_replace (
R"(Display EXECUTABLE_NAME's help summary.)",
          std::regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // about

  subGroup->
    appendAtomToSubGroup (
      oahAboutAtom::create (
        "about", "a",
        regex_replace (
R"(Display information about EXECUTABLE_NAME.)",
          std::regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // version

  subGroup->
    appendAtomToSubGroup (
      oahVersionAtom::create (
        "version", "v",
        regex_replace (
R"(Display EXECUTABLE_NAME's version.)",
          std::regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName,
        oahVersionKind::kOahVersion));

  subGroup->
    appendAtomToSubGroup (
      oahVersionAtom::create (
        "version-full", "vfull",
        regex_replace (
R"(Display EXECUTABLE_NAME's version.)",
          std::regex ("EXECUTABLE_NAME"),
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
          std::regex ("EXECUTABLE_NAME"),
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
          std::regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // display prefixes

  subGroup->
    appendAtomToSubGroup (
      oahDisplayPrefixes::create (
        "display-prefixes", "prefixes",
        regex_replace (
R"(Display the prefixes known to EXECUTABLE_NAME.)",
          std::regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // display single character options

  subGroup->
    appendAtomToSubGroup (
      oahDisplaySingleCharacterOptions::create (
        "display-single-character-options", "single",
        regex_replace (
R"(Display the single character options known to EXECUTABLE_NAME.)",
          std::regex ("EXECUTABLE_NAME"),
          serviceName),
        serviceName));

  // help about option name

  std::string defaultOptionLongName = "name-help";

  fOptionNameToProvideHelpAbout = defaultOptionLongName;

  fOptionNameHelpAtom =
    oahOptionNameHelpAtom::create (
      defaultOptionLongName, "nh",
      regex_replace (
R"(Print help about OPTION_NAME.
OPTION_NAME is optional, and the default value is 'DEFAULT_VALUE'.)",
        std::regex ("DEFAULT_VALUE"),
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

  // find std::string

  fFindStringAtom =
    oahFindStringAtom::create (
      "find", "", // "find-string" ??? JMI v0.9.66
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
        std::regex ("MAXIMUM_LEVEL"),
        std::to_string (oahHandler::getIncludeFilesMaximumLevel ())),
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
          std::regex ("EXECUTABLE"),
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
          std::regex ("EXECUTABLE"),
          fOahOahGroupServiceName),
        "fShowOptionsAndArguments",
        fShowOptionsAndArguments));
}

#ifdef OAH_TRACING_IS_ENABLED
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
  const std::string& serviceName)
{
  // help
  // --------------------------------------
  initializeOahBasicHelpOptions (serviceName);

  // options and arguments
  // --------------------------------------
  initializeOahOahAndArgumentsOptions ();

#ifdef OAH_TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeOahtracingOah ();
#endif
}

//______________________________________________________________________________
S_mfcMultiComponent oahOahGroup::fetchOahOahGroupHandlerMultiComponent () const
{
  return
    fUpLinkToHandler->
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      "% ==> oahOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahOahGroup>*
    p =
      dynamic_cast<visitor<S_oahOahGroup>*> (v)) {
        S_oahOahGroup elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            "% ==> Launching oahOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOahGroup::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      "% ==> oahOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_oahOahGroup>*
    p =
      dynamic_cast<visitor<S_oahOahGroup>*> (v)) {
        S_oahOahGroup elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            "% ==> Launching oahOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOahGroup::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      "% ==> oahOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

void oahOahGroup::printAtomWithVariableOptionsValues ( // JMIJMIJMI
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
  /* JMI
    std::setw (valueFieldWidth) <<
    fVariableName <<
    ": \"" <<
    oahOptionalValuesStyleKindAsString (
      fOahOptionalValuesStyleKindVariable) <<
    "\"" <<
    std::endl <<
*/

    std::setw (valueFieldWidth) << "tracingOahVisitors" << ": " <<
    fTracingOahVisitors <<
    std::endl;
}

//______________________________________________________________________________
void oahOahGroup::printOahOahValues (int valueFieldWidth)
{
  gLogStream <<
    "The basic options are:" <<
    std::endl;

  ++gIndenter;

  // command line
  // --------------------------------------

  gLogStream << std::left <<
    std::setw (valueFieldWidth) << "Command line:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (valueFieldWidth) << "fShowOptionsAndArguments" << ": " <<
    fShowOptionsAndArguments <<
    std::endl;

  --gIndenter;

  // options and help display
  // --------------------------------------

  gLogStream << std::left <<
    std::setw (valueFieldWidth) << "Options trace and display:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (valueFieldWidth) << "fReverseNamesDisplayOrder" << ": " <<
    fReverseNamesDisplayOrder <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplaySourceCodePositions" << ": " <<
    fDisplaySourceCodePositions <<
    std::endl <<

    std::setw (valueFieldWidth) << "fTracingOahVisitors" << ": " <<
    fTracingOahVisitors <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahOahGroup& elt)
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
S_oahOahGroup createGlobalOahOahGroup (
  const std::string& serviceName)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global global OAH group" <<
      std::endl;
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
