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

#include "mfStaticSettings.h"

#include "mfConstants.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"

#include "waeHandlers.h"


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
  languageOahAtom* obj = new
    languageOahAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      languageKindVariable);
  assert (obj != nullptr);
  return obj;
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
      variableName),
    fLanguageKindVariable (
      languageKindVariable)
{}

languageOahAtom::~languageOahAtom ()
{}

void languageOahAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a languageOahAtom" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fLanguageKindVariable = mfLanguageKindFromString (theString);

  fSelected = true;

  // do nothing more, choosing the insider OAH handler has been done already
}

void languageOahAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> languageOahAtom::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_languageOahAtom>*
    p =
      dynamic_cast<visitor<S_languageOahAtom>*> (v)) {
        S_languageOahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching languageOahAtom::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void languageOahAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> languageOahAtom::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_languageOahAtom>*
    p =
      dynamic_cast<visitor<S_languageOahAtom>*> (v)) {
        S_languageOahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching languageOahAtom::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void languageOahAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> languageOahAtom::browseData ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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

  os << std::left <<
    std::setw (fieldWidth) <<
    "fLanguageKindVariable" << ": " <<
    fLanguageKindVariable <<
    std::endl;

  --gIndenter;
}

void languageOahAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    "fLanguageKindVariable" << ": " <<
    fLanguageKindVariable;
  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os << ", selected";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_languageOahAtom& elt)
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
S_insiderOahAtom insiderOahAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
{
  insiderOahAtom* obj = new
    insiderOahAtom (
      longName,
      shortName,
      description);
  assert (obj != nullptr);
  return obj;
}

insiderOahAtom::insiderOahAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{}

insiderOahAtom::~insiderOahAtom ()
{}

void insiderOahAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a insiderOahAtom" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSelected = true;

  // do nothing more, choosing the insider OAH handler has been done already

  fSelected = true;
}

void insiderOahAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> insiderOahAtom::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_insiderOahAtom>*
    p =
      dynamic_cast<visitor<S_insiderOahAtom>*> (v)) {
        S_insiderOahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching insiderOahAtom::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void insiderOahAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> insiderOahAtom::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_insiderOahAtom>*
    p =
      dynamic_cast<visitor<S_insiderOahAtom>*> (v)) {
        S_insiderOahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching insiderOahAtom::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void insiderOahAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> insiderOahAtom::browseData ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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

void insiderOahAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    "insider" <<  ": ";
  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os << ", selected";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_insiderOahAtom& elt)
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
S_regularOahAtom regularOahAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
{
  regularOahAtom* obj = new
    regularOahAtom (
      longName,
      shortName,
      description);
  assert (obj != nullptr);
  return obj;
}

regularOahAtom::regularOahAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description)
  : oahValueLessAtom (
      longName,
      shortName,
      description)
{}

regularOahAtom::~regularOahAtom ()
{}

void regularOahAtom::applyValueLessAtom (std::ostream& os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> option '" << fetchNames () << "' is a regularOahAtom" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSelected = true;

  // do nothing more, choosing the regular OAH handler has been done already

  fSelected = true;
}

void regularOahAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> regularOahAtom::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_regularOahAtom>*
    p =
      dynamic_cast<visitor<S_regularOahAtom>*> (v)) {
        S_regularOahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching regularOahAtom::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void regularOahAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> regularOahAtom::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_regularOahAtom>*
    p =
      dynamic_cast<visitor<S_regularOahAtom>*> (v)) {
        S_regularOahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching regularOahAtom::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void regularOahAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> regularOahAtom::browseData ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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

void regularOahAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    "regular" <<  ": ";
  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os << ", selected";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_regularOahAtom& elt)
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
S_passIDOahAtom passIDOahAtom::create (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  mfPassIDKind&      passIDKind)
{
  passIDOahAtom* obj = new
    passIDOahAtom (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName,
      passIDKind);
  assert (obj != nullptr);
  return obj;
}

passIDOahAtom::passIDOahAtom (
  const std::string& longName,
  const std::string& shortName,
  const std::string& description,
  const std::string& valueSpecification,
  const std::string& variableName,
  mfPassIDKind&      passIDKind)
  : oahAtomStoringAValue (
      longName,
      shortName,
      description,
      valueSpecification,
      variableName),
    fPassIDKindVariable (
      passIDKind)
{}

passIDOahAtom::~passIDOahAtom ()
{}

void passIDOahAtom::applyAtomWithValue (
  const std::string& theString,
  std::ostream&      os)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a passIDOahAtom" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPassIDKindVariable = mfPassIDKindFromString (theString);

  fSelected = true;

  // do nothing more, choosing the insider OAH handler has been done already
}

void passIDOahAtom::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> passIDOahAtom::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_passIDOahAtom>*
    p =
      dynamic_cast<visitor<S_passIDOahAtom>*> (v)) {
        S_passIDOahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching passIDOahAtom::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void passIDOahAtom::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> passIDOahAtom::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_passIDOahAtom>*
    p =
      dynamic_cast<visitor<S_passIDOahAtom>*> (v)) {
        S_passIDOahAtom elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching passIDOahAtom::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void passIDOahAtom::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> passIDOahAtom::browseData ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void passIDOahAtom::print (std::ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "passIDOahAtom:" <<
    std::endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void passIDOahAtom::displayAtomWithVariableOptionsValues (
  std::ostream& os,
  int           valueFieldWidth) const
{
  os << std::left <<
    std::setw (valueFieldWidth) <<
    "fPassIDKindVariable" <<  ": " <<
    fPassIDKindVariable;
  switch (fEarlyOptionKind) {
    case oahEarlyOptionKind::kEarlyOptionNo:
      break;
    case oahEarlyOptionKind::kEarlyOptionYes:
      os <<
        ", early";
      break;
  } // switch
  if (fSelected) {
    os << ", selected";
  }
  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_passIDOahAtom& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_oahOahGroup oahOahGroup::create (
  const std::string& serviceName)
{
  oahOahGroup* obj = new oahOahGroup (
    serviceName);
  assert (obj != nullptr);
  return obj;
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
as well as the trace messages, meant for developers and maintainers.
The NUMBER languages available are:
LANGUAGE_KINDS.
The default is 'DEFAULT_VALUE'.)",
            std::regex ("NUMBER"),
            std::to_string (gGlobalMusicFormatsLanguageKindsMap.size ())),
          std::regex ("LANGUAGE_KINDS"),
          availableMusicFormatsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
        std::regex ("DEFAULT_VALUE"),
        mfLanguageKindAsString (
          mfLanguageKindDefaultValue)),
      "LANGUAGE",
      "fEarlyLanguageKindRef",
      gEarlyOptions.getEarlyLanguageKindRef ()));

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

#ifdef MF_TRACE_IS_ENABLED
  // trace early options

  fTraceEarlyOptionsAtom =
    oahBooleanAtom::create (
      K_TRACE_EARLY_OPTIONS_OPTION_LONG_NAME, K_TRACE_EARLY_OPTIONS_OPTION_SHORT_NAME,
R"(Trace the handling of early options, which happens
at the very beginning of options and arguments handling.
This option is handy to debug OAH.)",
      "traceEarlyOptions",
      gEarlyOptions.getTraceEarlyOptionsRef ());

  subGroup->
    appendAtomToSubGroup (
      fTraceEarlyOptionsAtom);

  // OAH verbose mode

  fOahVerboseModeAtom =
    oahBooleanAtom::create (
     K_OAH_VERBOSE_MODE_OPTION_LONG_NAME, K_OAH_VERBOSE_MODE_OPTION_SHORT_NAME,
R"(Produce internal details about the context of options errors.
By default, only the mimimum information is output.
This option is handy to debug OAH.)",
      "fEarlyOahVerboseModeRef",
      gEarlyOptions.getEarlyOahVerboseModeRef ());

  subGroup->
    appendAtomToSubGroup (
      fOahVerboseModeAtom);
#endif // MF_TRACE_IS_ENABLED

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

  // find string

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
        "display-options-and-arguments", "doaa",
        regex_replace (
R"(Print the options and arguments to EXECUTABLE.)",
          std::regex ("EXECUTABLE"),
          fOahOahGroupServiceName),
        "fdisplayOptionsAndArguments",
        fdisplayOptionsAndArguments));
}

#ifdef MF_TRACE_IS_ENABLED
void oahOahGroup::initializeOahTraceOah ()
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
        "fTraceOahVisitors",
        fTraceOahVisitors));
}
#endif // MF_TRACE_IS_ENABLED

void oahOahGroup::initializeOahOahGroup (
  const std::string& serviceName)
{
  // help
  // --------------------------------------
  initializeOahBasicHelpOptions (serviceName);

  // options and arguments
  // --------------------------------------
  initializeOahOahAndArgumentsOptions ();

#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializeOahTraceOah ();
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> oahOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahOahGroup>*
    p =
      dynamic_cast<visitor<S_oahOahGroup>*> (v)) {
        S_oahOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching oahOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void oahOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> oahOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_oahOahGroup>*
    p =
      dynamic_cast<visitor<S_oahOahGroup>*> (v)) {
        S_oahOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching oahOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithoutLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void oahOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> oahOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahOahGroup::displayAtomWithVariableOptionsValues ( // JMIJMIJMI
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

    std::setw (valueFieldWidth) << "TraceOahVisitors" << ": " <<
    fTraceOahVisitors <<
    std::endl;
}

//______________________________________________________________________________
void oahOahGroup::printOahOahValues (int valueFieldWidth)
{
  gLog <<
    "The basic options are:" <<
    std::endl;

  ++gIndenter;

  // command line
  // --------------------------------------

  gLog << std::left <<
    std::setw (valueFieldWidth) << "Command line:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fdisplayOptionsAndArguments" << ": " <<
    fdisplayOptionsAndArguments <<
    std::endl;

  --gIndenter;

  // options and help display
  // --------------------------------------

  gLog << std::left <<
    std::setw (valueFieldWidth) << "Options trace and display:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (valueFieldWidth) << "fReverseNamesDisplayOrder" << ": " <<
    fReverseNamesDisplayOrder <<
    std::endl <<

    std::setw (valueFieldWidth) << "fDisplaySourceCodePositions" << ": " <<
    fDisplaySourceCodePositions <<
    std::endl <<

    std::setw (valueFieldWidth) << "fTraceOahVisitors" << ": " <<
    fTraceOahVisitors <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//________________________________________________________________________
// hidden global OAH OAH group ariable
EXP S_oahOahGroup pGlobalOahOahGroup;

EXP S_oahOahGroup getGlobalOahOahGroup ()
{
  return pGlobalOahOahGroup;
}

//______________________________________________________________________________
S_oahOahGroup createGlobalOahOahGroup (
  const std::string& serviceName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global global OAH group" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! pGlobalOahOahGroup) {
    // create the global OAH options group
    pGlobalOahOahGroup =
      oahOahGroup::create (
        serviceName);
    assert (pGlobalOahOahGroup != 0);
  }

  // return it
  return pGlobalOahOahGroup;
}


}
